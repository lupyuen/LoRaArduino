// RH_RF95.cpp
//
// Copyright (C) 2011 Mike McCauley
// $Id: RH_RF95.cpp,v 1.11 2016/04/04 01:40:12 mikem Exp mikem $

#include <RH_RF95.h>

// Interrupt vectors for the 3 Arduino interrupt pins
// Each interrupt can be handled by a different instance of RH_RF95, allowing you to have
// 2 or more LORAs per Arduino
RH_RF95* RH_RF95::_deviceForInterrupt[RH_RF95_NUM_INTERRUPTS] = {0, 0, 0};
uint8_t RH_RF95::_interruptCount = 0; // Index into _deviceForInterrupt for next device

//  Fixed constants according to http://www.hoperf.com/upload/rf/RFM95_96_97_98W.pdf
const int FIXED_RH_RF95_BW_125KHZ                             = 0x70;
const int FIXED_RH_RF95_BW_250KHZ                             = 0x80;
const int FIXED_RH_RF95_CODING_RATE_4_5                       = 0x02;
const int FIXED_RH_RF95_RX_PAYLOAD_CRC_IS_ON                  = 0x04;


// These are indexed by the values of ModemConfigChoice
// Stored in flash (program) memory to save SRAM
PROGMEM static const RH_RF95::ModemConfig MODEM_CONFIG_TABLE[] =
{
    //  1d,     1e,      26
    { 0x72,   0x74,    0x00}, // Bw125Cr45Sf128 (the chip default)
    { 0x92,   0x74,    0x00}, // Bw500Cr45Sf128
    { 0x48,   0x94,    0x00}, // Bw31_25Cr48Sf512
    { 0x78,   0xc4,    0x00}, // Bw125Cr48Sf4096
	////  TP-IoT Gateway runs on:
	////    case 1:     setCR(CR_5);        // CR = 4/5
    ////                setSF(SF_12);       // SF = 12
    ////                setBW(BW_125);      // BW = 125 KHz
    { FIXED_RH_RF95_BW_125KHZ + FIXED_RH_RF95_CODING_RATE_4_5,   // TP-IoT: Bw125Cr45Sf4096
        RH_RF95_SPREADING_FACTOR_4096CPS + FIXED_RH_RF95_RX_PAYLOAD_CRC_IS_ON,    0x00},

	////  Testing TP-IoT Gateway on mode 5 (better reach, medium time on air)
    ////    case 5:     setCR(CR_5);        // CR = 4/5
    ////                setSF(SF_10);       // SF = 10
    ////                setBW(BW_250);      // BW = 250 KHz -> 0x80
    { FIXED_RH_RF95_BW_250KHZ + FIXED_RH_RF95_CODING_RATE_4_5,   // TP-IoT: Bw250Cr45Sf1024
        RH_RF95_SPREADING_FACTOR_1024CPS + FIXED_RH_RF95_RX_PAYLOAD_CRC_IS_ON,    0x00},
};

RH_RF95::RH_RF95(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI& spi)
    :
    RHSPIDriver(slaveSelectPin, spi),
    _rxBufValid(0)
{
    _interruptPin = interruptPin;
    _myInterruptIndex = 0xff; // Not allocated yet
}

bool RH_RF95::init()
{
    if (!RHSPIDriver::init())
	return false;

    // Determine the interrupt number that corresponds to the interruptPin
    int interruptNumber = digitalPinToInterrupt(_interruptPin);
    if (interruptNumber == NOT_AN_INTERRUPT)
	return false;
#ifdef RH_ATTACHINTERRUPT_TAKES_PIN_NUMBER
    interruptNumber = _interruptPin;
#endif

    // No way to check the device type :-(

    // Set sleep mode, so we can also set LORA mode:
    spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE);
    delay(10); // Wait for sleep mode to take over from say, CAD
    // Check we are in sleep mode, with LORA set
    if (spiRead(RH_RF95_REG_01_OP_MODE) != (RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE))
    {
//	Serial.println(spiRead(RH_RF95_REG_01_OP_MODE), HEX);
	return false; // No device present?
    }

    // Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
    // ARM M4 requires the below. else pin interrupt doesn't work properly.
    // On all other platforms, its innocuous, belt and braces
    pinMode(_interruptPin, INPUT);

    // Set up interrupt handler
    // Since there are a limited number of interrupt glue functions isr*() available,
    // we can only support a limited number of devices simultaneously
    // ON some devices, notably most Arduinos, the interrupt pin passed in is actuallt the
    // interrupt number. You have to figure out the interruptnumber-to-interruptpin mapping
    // yourself based on knwledge of what Arduino board you are running on.
    if (_myInterruptIndex == 0xff)
    {
	// First run, no interrupt allocated yet
	if (_interruptCount <= RH_RF95_NUM_INTERRUPTS)
	    _myInterruptIndex = _interruptCount++;
	else
	    return false; // Too many devices, not enough interrupt vectors
    }
    _deviceForInterrupt[_myInterruptIndex] = this;
    if (_myInterruptIndex == 0)
	attachInterrupt(interruptNumber, isr0, RISING);
    else if (_myInterruptIndex == 1)
	attachInterrupt(interruptNumber, isr1, RISING);
    else if (_myInterruptIndex == 2)
	attachInterrupt(interruptNumber, isr2, RISING);
    else
	return false; // Too many devices, not enough interrupt vectors

    // Set up FIFO
    // We configure so that we can use the entire 256 byte FIFO for either receive
    // or transmit, but not both at the same time
    spiWrite(RH_RF95_REG_0E_FIFO_TX_BASE_ADDR, 0);
    spiWrite(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 0);

    // Packet format is preamble + explicit-header + payload + crc
    // Explicit Header Mode
    // payload is TO + FROM + ID + FLAGS + message data
    // RX mode is implmented with RXCONTINUOUS
    // max message data length is 255 - 4 = 251 octets

    setModeIdle();

    // Set up default configuration
    // No Sync Words in LORA mode.
    ////setModemConfig(Bw125Cr45Sf128); // Radio default
    ////setModemConfig(Bw125Cr48Sf4096); // slow and reliable?
	////  TP-IoT Gateway runs on:
	////    case 1:     setCR(CR_5);        // CR = 4/5
    ////                setSF(SF_12);       // SF = 12
    ////                setBW(BW_125);      // BW = 125 KHz
    //setModemConfig(Bw125Cr45Sf4096);  ////  TP-IoT
	////  Testing TP-IoT Gateway on mode 5 (better reach, medium time on air)
    ////    case 5:     setCR(CR_5);        // CR = 4/5
    ////                setSF(SF_10);       // SF = 10
    ////                setBW(BW_250);      // BW = 250 KHz -> 0x80
    setModemConfig(Bw250Cr45Sf1024);  ////  TP-IoT

    setPreambleLength(8); // Default is 8

    // An innocuous ISM frequency, same as RF22's
    ////setFrequency(434.0);
    ////  TP-IoT: uint32_t LORA_CH_10_868 = CH_10_868; //  0xD84CCC; // channel 10, central freq = 865.20MHz  ////  Lup Yuen
    setFrequency(865.20); ////  TP-IoT

    // Lowish power
    setTxPower(13);
    ////  TP-IoT: TODO: Set power.

    ////  TP-IoT: TODO: Tell gateway to skip CRC check.

#ifdef PATCH_REGISTERS
    ////  TP-IoT: Patch registers based on values from SX1272 mode 1.
    spiWrite(0x8, 0xCC);  //  RegFrLsb. Must be set in idle mode.
    spiWrite(0xB, 0x3B);  //  RegOcp
    spiWrite(0xC, 0x23);  //  RegLna
    spiWrite(0x1F, 0xFF);  //  RegSymbTimeoutLsb
#endif

#ifdef INVERT_IQ
    ////  TP-IoT: TODO: To allow SX1276 to talk to SX1272, use inverted I/Q signal (prevent mote-to-mote communication)
    // Refer to http://openlora.com/forum/viewtopic.php?t=887
    // Also see example of SX1272-SX1276 interop: http://cpham.perso.univ-pau.fr/LORA/RPIgateway.html
    //printf("setupLoRa: Before inverting I/Q REG_NODE_ADRS = 0x%02x\n", sx1272.readRegister(REG_NODE_ADRS));
    const int REG_NODE_ADRS = 0x33;
    spiWrite(REG_NODE_ADRS, spiRead(REG_NODE_ADRS)|(1<<6));
    //printf("setupLoRa: After inverting I/Q REG_NODE_ADRS = 0x%02x\n", sx1272.readRegister(REG_NODE_ADRS));
#endif  //  INVERT_IQ
    return true;
}

extern int testRF95; ////  TP-IoT
int testRF95 = -1; ////  TP-IoT


// C++ level interrupt handler for this instance
// LORA is unusual in that it has several interrupt lines, and not a single, combined one.
// On MiniWirelessLoRa, only one of the several interrupt lines (DI0) from the RFM95 is usefuly
// connnected to the processor.
// We use this to get RxDone and TxDone interrupts
void RH_RF95::handleInterrupt()
{
    // Read the interrupt register
    uint8_t irq_flags = spiRead(RH_RF95_REG_12_IRQ_FLAGS);
    if (_mode == RHModeRx && irq_flags & (RH_RF95_RX_TIMEOUT | RH_RF95_PAYLOAD_CRC_ERROR))
    {
	_rxBad++;
    }
    else if (_mode == RHModeRx && irq_flags & RH_RF95_RX_DONE)
    {
	// Have received a packet
	uint8_t len = spiRead(RH_RF95_REG_13_RX_NB_BYTES);

	// Reset the fifo read ptr to the beginning of the packet
	spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
	spiBurstRead(RH_RF95_REG_00_FIFO, _buf, len);
	_bufLen = len;
	spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags

	// Remember the RSSI of this packet
	// this is according to the doc, but is it really correct?
	// weakest receiveable signals are reported RSSI at about -66
	_lastRssi = spiRead(RH_RF95_REG_1A_PKT_RSSI_VALUE) - 137;

	// We have received a message.
	validateRxBuf();
	if (_rxBufValid)
	    setModeIdle(); // Got one
    }
    else if (_mode == RHModeTx && irq_flags & RH_RF95_TX_DONE)
    {
	_txGood++;
	setModeIdle();
    }

    spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
}

// These are low level functions that call the interrupt handler for the correct
// instance of RH_RF95.
// 3 interrupts allows us to have 3 different devices
void RH_RF95::isr0()
{
    if (_deviceForInterrupt[0])
	_deviceForInterrupt[0]->handleInterrupt();
}
void RH_RF95::isr1()
{
    if (_deviceForInterrupt[1])
	_deviceForInterrupt[1]->handleInterrupt();
}
void RH_RF95::isr2()
{
    if (_deviceForInterrupt[2])
	_deviceForInterrupt[2]->handleInterrupt();
}

// Check whether the latest received message is complete and uncorrupted
void RH_RF95::validateRxBuf()
{
    if (_bufLen < 4)
	return; // Too short to be a real message
    // Extract the 4 headers
    _rxHeaderTo    = _buf[0];
    _rxHeaderFrom  = _buf[1];
    _rxHeaderId    = _buf[2];
    _rxHeaderFlags = _buf[3];
    if (_promiscuous ||
	_rxHeaderTo == _thisAddress ||
	_rxHeaderTo == RH_BROADCAST_ADDRESS)
    {
	_rxGood++;
	_rxBufValid = true;
    }
}

bool RH_RF95::available()
{
    if (_mode == RHModeTx)
	return false;
    setModeRx();
    return _rxBufValid; // Will be set by the interrupt handler when a good message is received
}

void RH_RF95::clearRxBuf()
{
    ATOMIC_BLOCK_START;
    _rxBufValid = false;
    _bufLen = 0;
    ATOMIC_BLOCK_END;
}

bool RH_RF95::recv(uint8_t* buf, uint8_t* len)
{
    if (!available())
	return false;
    if (buf && len)
    {
	ATOMIC_BLOCK_START;
	// Skip the 4 headers that are at the beginning of the rxBuf
	if (*len > _bufLen-RH_RF95_HEADER_LEN)
	    *len = _bufLen-RH_RF95_HEADER_LEN;
	memcpy(buf, _buf+RH_RF95_HEADER_LEN, *len);
	ATOMIC_BLOCK_END;
    }
    clearRxBuf(); // This message accepted and cleared
    return true;
}

bool RH_RF95::send(const uint8_t* data, uint8_t len)
{
    if (len > RH_RF95_MAX_MESSAGE_LEN)
	return false;

    ////  TP-IoT:
	Serial.print("*** _txHeaderFlags: 0x");
	Serial.println(_txHeaderFlags, HEX);
#ifdef DUMP_REGISTERS
	for (int i = 0; i <= 0x3f; i++) {
	    Serial.print("Reg[0x");
	    Serial.print(i, HEX);
	    Serial.print("] = 0x");
	    Serial.print(spiRead(i), HEX);
	    Serial.println("");
	}
#endif

    waitPacketSent(); // Make sure we dont interrupt an outgoing message
    setModeIdle();

    // Position at the beginning of the FIFO
    spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
    // The headers
#define ORIGINAL_HEADER
#ifdef ORIGINAL_HEADER
    spiWrite(RH_RF95_REG_00_FIFO, _txHeaderTo);
    spiWrite(RH_RF95_REG_00_FIFO, _txHeaderFrom);
    spiWrite(RH_RF95_REG_00_FIFO, _txHeaderId);
    spiWrite(RH_RF95_REG_00_FIFO, _txHeaderFlags + RH_RF95_HEADER_LEN);  //  TP-IoT: Must add header length because gateway uses this to determine packet length.
#else
    ////  TP-IoT: dst, src, count, len
    uint8_t header[] = { _txHeaderTo, _txHeaderFrom, _txHeaderId,
        _txHeaderFlags + RH_RF95_HEADER_LEN };  //  TP-IoT: Must add header length because gateway uses this to determine packet length.
    for (int i = 0; i < sizeof(header); i++) {
	    Serial.print("Header[0x");
	    Serial.print(i, HEX);
	    Serial.print("] = 0x");
	    Serial.print(header[i], HEX);
	    Serial.println("");
        spiWrite(RH_RF95_REG_00_FIFO, header[i]);
    }
#endif

    // The message data
#define ORIGINAL_BURST_WRITE
#ifdef ORIGINAL_BURST_WRITE
    spiBurstWrite(RH_RF95_REG_00_FIFO, data, len);
    spiWrite(RH_RF95_REG_22_PAYLOAD_LENGTH, len + RH_RF95_HEADER_LEN);
#else
    ////  TP-IoT
    for (int i = 0; i < len; i++) {
	    Serial.print("FIFO[0x");
	    Serial.print(i, HEX);
	    Serial.print("] = 0x");
	    Serial.print(data[i], HEX);
	    Serial.println("");
        spiWrite(RH_RF95_REG_00_FIFO, data[i]);
    }
    spiWrite(RH_RF95_REG_22_PAYLOAD_LENGTH, len + RH_RF95_HEADER_LEN);
#endif

    setModeTx(); // Start the transmitter
    // when Tx is done, interruptHandler will fire and radio mode will return to STANDBY
    return true;
}

bool RH_RF95::printRegisters()
{
#ifdef RH_HAVE_SERIAL
    uint8_t registers[] = { 0x01, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x014, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};

    uint8_t i;
    for (i = 0; i < sizeof(registers); i++)
    {
	Serial.print(registers[i], HEX);
	Serial.print(": ");
	Serial.println(spiRead(registers[i]), HEX);
    }
#endif
    return true;
}

uint8_t RH_RF95::maxMessageLength()
{
    return RH_RF95_MAX_MESSAGE_LEN;
}

bool RH_RF95::setFrequency(float centre)
{
    // Frf = FRF / FSTEP
    uint32_t frf = (centre * 1000000.0) / RH_RF95_FSTEP;
    spiWrite(RH_RF95_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    spiWrite(RH_RF95_REG_07_FRF_MID, (frf >> 8) & 0xff);
    spiWrite(RH_RF95_REG_08_FRF_LSB, frf & 0xff);

    return true;
}

void RH_RF95::setModeIdle()
{
    if (_mode != RHModeIdle)
    {
	spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_STDBY);
	_mode = RHModeIdle;
    }
}

bool RH_RF95::sleep()
{
    if (_mode != RHModeSleep)
    {
	spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_SLEEP);
	_mode = RHModeSleep;
    }
    return true;
}

void RH_RF95::setModeRx()
{
    if (_mode != RHModeRx)
    {
	spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_RXCONTINUOUS);
	spiWrite(RH_RF95_REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone
	_mode = RHModeRx;
    }
}

void RH_RF95::setModeTx()
{
    if (_mode != RHModeTx)
    {
	spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_TX);
	spiWrite(RH_RF95_REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone
	_mode = RHModeTx;
    }
}

void RH_RF95::setTxPower(int8_t power, bool useRFO)
{
    // Sigh, different behaviours depending on whther the module use PA_BOOST or the RFO pin
    // for the transmitter output
    if (useRFO)
    {
	if (power > 14)
	    power = 14;
	if (power < -1)
	    power = -1;
	spiWrite(RH_RF95_REG_09_PA_CONFIG, RH_RF95_MAX_POWER | (power + 1));
    }
    else
    {
	if (power > 23)
	    power = 23;
	if (power < 5)
	    power = 5;

	// For RH_RF95_PA_DAC_ENABLE, manual says '+20dBm on PA_BOOST when OutputPower=0xf'
	// RH_RF95_PA_DAC_ENABLE actually adds about 3dBm to all power levels. We will us it
	// for 21, 22 and 23dBm
	if (power > 20)
	{
	    spiWrite(RH_RF95_REG_4D_PA_DAC, RH_RF95_PA_DAC_ENABLE);
	    power -= 3;
	}
	else
	{
	    spiWrite(RH_RF95_REG_4D_PA_DAC, RH_RF95_PA_DAC_DISABLE);
	}

	// RFM95/96/97/98 does not have RFO pins connected to anything. Only PA_BOOST
	// pin is connected, so must use PA_BOOST
	// Pout = 2 + OutputPower.
	// The documentation is pretty confusing on this topic: PaSelect says the max power is 20dBm,
	// but OutputPower claims it would be 17dBm.
	// My measurements show 20dBm is correct
	spiWrite(RH_RF95_REG_09_PA_CONFIG, RH_RF95_PA_SELECT | (power-5));
    }
}

// Sets registers from a canned modem configuration structure
void RH_RF95::setModemRegisters(const ModemConfig* config)
{
    spiWrite(RH_RF95_REG_1D_MODEM_CONFIG1,       config->reg_1d);
    spiWrite(RH_RF95_REG_1E_MODEM_CONFIG2,       config->reg_1e);
    spiWrite(RH_RF95_REG_26_MODEM_CONFIG3,       config->reg_26);
}

// Set one of the canned FSK Modem configs
// Returns true if its a valid choice
bool RH_RF95::setModemConfig(ModemConfigChoice index)
{
    if (index > (signed int)(sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
        return false;

    ModemConfig cfg;
    memcpy_P(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(RH_RF95::ModemConfig));
    setModemRegisters(&cfg);

    return true;
}

void RH_RF95::setPreambleLength(uint16_t bytes)
{
    spiWrite(RH_RF95_REG_20_PREAMBLE_MSB, bytes >> 8);
    spiWrite(RH_RF95_REG_21_PREAMBLE_LSB, bytes & 0xff);
}
