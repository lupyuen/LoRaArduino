/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: 
 *  (1) "AS IS" WITH NO WARRANTY; 
 *  (2) TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, HopeRF SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) HopeRF
 *
 * website: www.HopeRF.com
 *          www.HopeRF.cn    
 */

/*! 
 * file       RFduino_RFM66.cpp
 * brief      driver for RFM66 TRx
 * hardware   HopeRF's RFduino TRx & with HopeRF RFM66 COB rf-module  
 *            
 *
 * version    1.0
 * date       Jun 3 2014
 * author     QY Ruan
 */
 
#include "HopeDuino_RFM66.h"

/********************************************************** 
**RF69 Regsister define                                      
**********************************************************/ 
#define 	RegFifo 			0x00 
#define 	RegOpMode 			0x01 
#define 	RegBitrateMsb 		0x02
#define 	RegBitrateLsb 		0x03 
#define 	RegFdevMsb 		    0x04 
#define 	RegFdevLsb 		    0x05 
#define 	RegFrMsb 			0x06 
#define 	RegFrMid 			0x07 
#define 	RegFrLsb 			0x08
#define		RegPaConfig			0x09
#define 	RegPaRamp 			0x0A 
#define 	RegOcp 			    0x0B
#define 	RegLna 			    0x0C 
#define		RegRxConfig			0x0D
#define 	RegRssiConfig 		0x0E
#define		RegRssiCollision	0x0F
#define 	RegRssiThresh 		0x10 
#define 	RegRssiValue 		0x11
#define 	RegRxBw 			0x12
#define 	RegAfcBw 			0x13
#define 	RegOokPeak 		    0x14 
#define 	RegOokFix 			0x15 
#define 	RegOokAvg 			0x16  
#define 	RegAfcFei 			0x1A
#define 	RegAfcMsb 			0x1B
#define 	RegAfcLsb 			0x1C 
#define 	RegFeiMsb 			0x1D
#define 	RegFeiLsb 			0x1E 
#define		RegPreambleDetect	0x1F
#define 	RegRxTimeout1 		0x20 
#define 	RegRxTimeout2 		0x21 
#define 	RegRxTimeout3 		0x22 
#define		RegRxDelay			0x23
#define 	RegOsc 				0x24
#define 	RegPreambleMsb 	    0x25 
#define 	RegPreambleLsb 	    0x26 
#define 	RegSyncConfig 		0x27
#define 	RegSyncValue1		0x28 
#define 	RegSyncValue2       0x29 
#define 	RegSyncValue3       0x2A 
#define 	RegSyncValue4       0x2B 
#define 	RegSyncValue5       0x2C 
#define 	RegSyncValue6       0x2D 
#define 	RegSyncValue7       0x2E 
#define 	RegSyncValue8       0x2F
#define 	RegPacketConfig1 	0x30 
#define 	RegPacketConfig2 	0x31 
#define 	RegPayloadLength 	0x32 
#define 	RegNodeAdrs 		0x33 
#define 	RegBroadcastAdrs 	0x34 
#define 	RegFifoThresh 		0x35
#define		RegSeqConfig1		0x36
#define		RegSeqConfig2		0x37
#define		RegTimerResol		0x38
#define		RegTimer1Coef		0x39
#define		RegTimer2Coef		0x3A
#define		RegImageCal			0x3B
#define		RegTemp				0x3C
#define		RegLowBat			0x3D
#define 	RegIrqFlags1 		0x3E
#define 	RegIrqFlags2 		0x3F
#define 	RegDioMapping1 	    0x40 
#define 	RegDioMapping2 	    0x41 
#define 	RegVersion 		    0x42 
#define		RegAgcRef			0x43
#define		RegAgcThresh1		0x44
#define		RegAgcThresh2		0x45
#define		RegAgcThresh3		0x46
#define		RegPllHop			0x4B
#define		RegTcxo				0x58
#define		RegPaDac			0x5A
#define		RegPll				0x5C
#define		RegPllLowPn			0x5E
#define		RegFormerTemp		0x6C
#define		RegBitrateFrac		0x70

/**********************************************************      
**RF69 mode status                                          
**********************************************************/      
#define		RADIO_SLEEP			(0x00)
#define		RADIO_STANDBY		(0x01)
#define		RADIO_TX			(0x03)
#define		RADIO_RX			(0x05)

#define		FskMode				(0<<5)
#define		OokMode				(1<<5) 

#define		Shaping				(2<<3)

#define 	MODE_MASK			0xF8
#define		MOUDLE_MASK			0x87

#define		AFC_ON				(1<<4)
#define		AGC_ON				(1<<3)
#define		RX_TRIGGER			0x06

#define		PREAMBLE_DECT_ON	(1<<7)
#define		PREAMBLE_DECT_1BYTE	(0<<5)
#define		PREAMBLE_DECT_2BYTE	(1<<5)
#define		PREAMBLE_DECT_3BYTE	(2<<5)

#define		AUTO_RST_RX_OFF		(0<<6)
#define		AUTO_RST_RX_ON		(1<<6)
#define		AUTO_RST_RX_ONwPLL	(2<<6)	
#define		SYNC_ON				(1<<4)

//for PacketConfig
#define		VariablePacket		(1<<7)
#define		DcFree_NRZ			(0<<5)
#define		DcFree_MANCHESTER	(1<<5)
#define		DcFree_WHITENING	(2<<5)
#define		CrcOn				(1<<4)
#define		CrcDisAutoClear		(1<<3)
#define		AddrFilter_NONE		(0<<1)
#define		AddrFilter_NODE		(1<<1)
#define		AddrFilter_ALL		(2<<1)
#define		CrcCalc_CCITT		0x00
#define		CrcCalc_IBM			0x01

#define		PacketMode			(1<<6)
#define		ContinuousMode		(0<<6)








/**********************************************************
**Name:     vInitialize
**Function: initialize rfm66
**Input:    none
**Output:   none
**********************************************************/
void rf66Class::vInitialize(void)
{
 ClrPOR();
 PORIn();
 DIO0In();
 Spi.vSpiInit();									//init port
 						
 FrequencyValue.Freq = (Frequency<<11)/125;			//calc frequency
 BitRateValue  = (SymbolTime<<5)/1000;				//calc bitrate
 DevationValue = (Devation<<11)/125;				//calc deviation
 BandWidthValue = bSelectBandwidth(BandWidth);	
 vConfig();
 vGoStandby();
}

/**********************************************************
**Name:     vConfig
**Function: config RF66
**Input:    none
**Output:   none
**********************************************************/
void rf66Class::vConfig(void)
{
 byte i, j;
 byte sync;
 vRF66Reset();
 vGoStandby();

 //Frequency
 Spi.vSpiWrite(((word)RegFrMsb<<8)+FrequencyValue.freq.FreqH);
 Spi.vSpiWrite(((word)RegFrMid<<8)+FrequencyValue.freq.FreqM);
 Spi.vSpiWrite(((word)RegFrLsb<<8)+FrequencyValue.freq.FreqL);
 
 // Mode
 i = Spi.bSpiRead(RegOpMode);
 i &= MOUDLE_MASK;
 switch(Modulation)
 	{
 	case OOK:
		i |= OokMode+Shaping;
 		break;
 	case GFSK:
 		i |= FskMode+Shaping;
 		break;
 	case FSK:
 	default:
 		i |= FskMode;
 		break;
 	}
 Spi.vSpiWrite(((word)RegOpMode<<8)+i);

 //BitRate 
 Spi.vSpiWrite(((word)RegBitrateMsb<<8)+(byte)(BitRateValue>>8));
 Spi.vSpiWrite(((word)RegBitrateLsb<<8)+(byte)BitRateValue);
 
 //Devation
 Spi.vSpiWrite(((word)RegFdevMsb<<8)+(((byte)(DevationValue>>8))&0x3F));
 Spi.vSpiWrite(((word)RegFdevLsb<<8)+(byte)(DevationValue&0xFF));
 
 //PA Config
 if(OutputPower>20)
 	{ i = 0x0F; j = 0x87; }
 else if(OutputPower>17)
 	{ i = (OutputPower-3); j = 0x87; }
 else if(OutputPower>=2)
 	{ i = (OutputPower-2); j = 0x84; }
 else
 	{ i = 0; j = 0x84; }
 Spi.vSpiWrite(((word)RegPaConfig<<8)+0x80+i);
 Spi.vSpiWrite(((word)RegPaDac<<8)+j);
 Spi.vSpiWrite(((word)RegPaRamp<<8)+bSelectRamping(SymbolTime));

 //Ocp
 Spi.vSpiWrite(((word)RegOcp<<8)+0x0F);			//Disable Ocp

 //LNA
 Spi.vSpiWrite(((word)RegLna<<8)+0x20);			//High & LNA Enable
 
 //RxConfig
 Spi.vSpiWrite(((word)RegRxConfig<<8)+AGC_ON+RX_TRIGGER);
 
 //RxBw
 Spi.vSpiWrite(((word)RegRxBw<<8)+BandWidthValue);	
 
 //OOK
 Spi.vSpiWrite(((word)RegOokPeak<<8)+0x20+(0x02<<3)+0x00);
 
 //PreambleDetect
 Spi.vSpiWrite(((word)RegPreambleDetect<<8)+PREAMBLE_DECT_ON+PREAMBLE_DECT_3BYTE);
 Spi.vSpiWrite(((word)RegPreambleMsb<<8)+(byte)(PreambleLength>>8));
 Spi.vSpiWrite(((word)RegPreambleLsb<<8)+(byte)PreambleLength);

 //Osc
 Spi.vSpiWrite(((word)RegOsc<<8)+0x07);			//Close OscClk Output
 
 //SyncConfig
 if(SyncLength==0)
 	sync = 0;
 else
 	sync = SyncLength-1;
 Spi.vSpiWrite(((word)RegSyncConfig<<8)+AUTO_RST_RX_ONwPLL+SYNC_ON+(sync&0x07));
 for(i=0;i<8;i++)								//SyncWordSetting
 	Spi.vSpiWrite(((word)(RegSyncValue1+i)<<8)+SyncWord[i]);	

 i = DcFree_NRZ + AddrFilter_NONE + CrcCalc_CCITT;
 if(!FixedPktLength)
 	i += VariablePacket;
 if(!CrcDisable)
 	i += CrcOn;
 Spi.vSpiWrite(((word)RegPacketConfig1<<8)+i);		
 Spi.vSpiWrite(((word)RegPacketConfig2<<8)+PacketMode);	

 if(FixedPktLength)								//Set Packet length
 	Spi.vSpiWrite(((word)RegPayloadLength<<8)+PayloadLength);
 else	
 	Spi.vSpiWrite(((word)RegPayloadLength<<8)+0xFF);

 Spi.vSpiWrite(((word)RegFifoThresh<<8)+0x01);
 
 Spi.vSpiWrite(((word)RegDioMapping2<<8)+0x61);	//DIO4 PllLock / DIO5 Data / PreambleDetect
}

/**********************************************************
**Name:     vGoRx
**Function: set rf66 to receive mode
**Input:    none
**Output:   none
**********************************************************/
void rf66Class::vGoRx(void)
{
 byte tmp;

 if(CrcDisable)	
 	Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x00);	//DIO0 PayloadReady / DIO1 FifoLevel / DIO2 FifoFull /DIO3 FifoEmpty
 else
 	Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x40);	//DIO0 CrcOk  / DIO1 FifoLevel / DIO2 FifoFull /DIO3 FifoEmpty
 
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_RX;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
}

/**********************************************************
**Name:     vGoStandby
**Function: set rf66 to standby mode
**Input:    none
**Output:   none
**********************************************************/
void rf66Class::vGoStandby(void)
{
 byte tmp;
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_STANDBY;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
}	

/**********************************************************
**Name:     vGoSleep
**Function: set rf66 to sleep mode
**Input:    none
**Output:   none
**********************************************************/
void rf66Class::vGoSleep(void)
{
 byte tmp;
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_SLEEP;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
}	

/**********************************************************
**Name:     bSendMessage
**Function: set rf66 to sleep mode
**Input:    msg------for which message to send
            length---message length
**Output:   true-----send ok
            false----send error/over time
**********************************************************/
bool rf66Class::bSendMessage(byte msg[], byte length)
{
 byte tmp;
 lword overtime;
 word bittime;

 Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x00);	//DIO0 PacketSend  / DIO1 FifoLevel / DIO2 FifoFull /DIO3 FifoEmpty

 if(!FixedPktLength)
 	Spi.vSpiWrite(((word)RegFifo<<8)+length);
 Spi.vSpiBurstWrite(RegFifo, msg, length);
 
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_TX;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
 
 //µÈ´ý·¢ÉäÍê±Ï
 bittime  = SymbolTime/1000;		//unit: us
 overtime = SyncLength+PreambleLength+length;
 if(!FixedPktLength)				//SyncWord & PktLength & 2ByteCRC
	overtime += 1;
 if(!CrcDisable)
 	overtime += 2;
 overtime<<=3;						//8bit == 1byte
 overtime*= bittime;
 overtime/= 1000;					//unit: ms
 if(overtime==0) 
 	overtime = 1;
 _delay_ms(overtime);				//
 for(tmp=0;tmp<100;tmp++)			//about 10ms for overtime
 	{
	if(DIO0_H())
		break; 	
	_delay_us(100);
 	}
 vGoStandby();	
 if(tmp>=100)
 	return(false);
 else
 	return(true);
}

/**********************************************************
**Name:     bGetMessage
**Function: check receive packet
**Input:    msg------for which message to read
**Output:   packet length
			0--------have no packet
**********************************************************/
byte rf66Class::bGetMessage(byte msg[])
{
 byte length;	
 if(DIO0_H())				//Receive CrcOk or PayloadReady
 	{
 	if(FixedPktLength)
 		length = PayloadLength;
 	else
 		length = Spi.bSpiRead(RegFifo);
 	Spi.vSpiBurstRead(RegFifo, msg, length);
 	vGoStandby();
 	vGoRx();
 	return(length);
 	}
 return(0);
}

/**********************************************************
**Name:     vRF66Reset
**Function: hardware reset rf69 chipset
**Input:    none
**Output:   none
**********************************************************/
void rf66Class::vRF66Reset(void)
{
 POROut();
 SetPOR();
 _delay_us(200);					//at least 100us for reset
 ClrPOR();
 PORIn();							//set POR for free
 _delay_ms(6);						//wait for ready
}	

/**********************************************************
**Name:     bSelectBandwidth
**Function: 
**Input:    BandWidth
**Output:   BandWidthValue
**********************************************************/
byte rf66Class::bSelectBandwidth(byte rx_bw)
{
 if(rx_bw<=10)
	return 0x15;						//10.4KHz 	Min
 else if(rx_bw<13)
 	return 0x0D;						//12.5KHz	
 else if(rx_bw<16)	
 	return 0x05;						//15.6KHz
 else if(rx_bw<21)		
 	return 0x14;						//20.8KHz
 else if(rx_bw<=25)	
 	return 0x0C;						//25.0KHz
 else if(rx_bw<32)
 	return 0x04;						//31.3KHz
 else if(rx_bw<42)
 	return 0x13;						//41.7KHz
 else if(rx_bw<=50)
 	return 0x0B;						//50.0KHz
 else if(rx_bw<63)
 	return 0x03;						//62.5KHz
 else if(rx_bw<84)
 	return 0x12;						//83.3KHz
 else if(rx_bw<=100)
 	return 0x0A;						//100KHz
 else if(rx_bw<=125)
 	return 0x02;						//125KHz
 else if(rx_bw<167)
 	return 0x11;						//167KHz
 else if(rx_bw<=200)
 	return 0x09;						//200KHz
 else if(rx_bw<=250)
 	return 0x01;						//250KHz
 else if(rx_bw<=333)
 	return 0x10;						//333KHz
 else if(rx_bw<=400)
 	return 0x08;						//400KHz
 else
 	return 0x00;						//500KHz Max
}	

/**********************************************************
**Name:     bSelectRamping
**Function: 
**Input:    symbol time
**Output:   ramping value
**********************************************************/
byte rf66Class::bSelectRamping(lword symbol)
{
 lword SymbolRate;
 
 SymbolRate = symbol/1000;			//ns->us
 SymbolRate = SymbolRate/4;			// 1/4 ramping
 
 if(SymbolRate<=10)		
 	return 0x0F;					//10us
 else if(SymbolRate<=12)			
 	return 0x0E;					//12us
 else if(SymbolRate<=15)			
 	return 0x0D;					//15us
 else if(SymbolRate<=20)
 	return 0x0C;					//20us
 else if(SymbolRate<=25)
 	return 0x0B;					//25us
 else if(SymbolRate<=31)
 	return 0x0A;					//31us
 else if(SymbolRate<=40)
 	return 0x09;					//40us
 else if(SymbolRate<=50)
 	return 0x08;					//50us
 else if(SymbolRate<=62)
 	return 0x07;					//62us
 else if(SymbolRate<=100)
 	return 0x06;					//100us
 else if(SymbolRate<=125)
 	return 0x05;					//125us
 else if(SymbolRate<=250)
 	return 0x04;					//250us
 else if(SymbolRate<=500)
 	return 0x03;					//500us
 else if(SymbolRate<=1000)
	return 0x02;					//1000us
 else if(SymbolRate<=2000)
 	return 0x01;					//2000us
 else 
 	return 0x00;
}	
	
	
	