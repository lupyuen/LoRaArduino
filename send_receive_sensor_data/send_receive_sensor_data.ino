//  Send sample sensor data to LoRa gateway.  Tested on Dragino LoRa GPS HAT for Raspberry Pi.
//  Based on https://github.com/matthijskooijman/arduino-lmic

//  TP-IoT Mode 1 is max range. TP-IoT Gateway runs on: CR = 4/5, SF = 12, BW = 125 KHz.
extern int transmission_mode;
int transmission_mode = 1;
const int device_address = 3;  //  My own address as assigned by TP-IoT.
const int gateway_address = 1;  //  TP-IoT Gateway address.
const int transmit_interval = 5000;  //  How often to send a message to gateway, in milliseconds.
uint8_t packet_num = 1;  //  Number of packets sent to gateway so far.

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
osjob_t txjob, timeoutjob;  //  Transmission and timeout tasks.

static void tx_func (osjob_t* job) {
  //  Send a message containing sample sensor data to the LoRa gateway, delimited by "|".
  //  The data sequence must match lora_gateway.py: temperature, humidity, light_level, message.  We may skip fields by sending "||".
  char sensor_data[] = "28.1|72||OK";
  tx(sensor_data, txdone_func);  //  Send the data to gateway.
  
  // Send the next set of sensor data every TX_INTERVAL milliseconds, plus a bit of randomness to prevent systematic collisions.
  os_setTimedCallback(job, os_getTime() + ms2osticks(transmit_interval + random(500)), tx_func);
}

//  Program starts here.
void setup() {
  Serial.begin(9600); Serial.println("Starting");
  pinMode(LED_BUILTIN, OUTPUT);  //  Allow use of LED.
  os_init();  //  Initialize runtime environment.

  //  Set up these settings once, and use them for both sending and receiving.
  LMIC.freq = 865200000; // TP-IoT runs on Channel 10: 865.20 MHz.
  LMIC.datarate = DR_SF12;  //  TP-IoT Mode 1: Use Spreading Factor 12 for encoding data.
  LMIC.txpow = 27;  //  Maximum transmission power.
  
  // This sets the default Coding Rate and bandwidth, to be updated in radio.c when we transmit.
  LMIC.rps = updr2rps(LMIC.datarate);
  Serial.println("Started"); Serial.flush();
  
  // First task to be run is to send sensor data.
  os_setCallback(&txjob, tx_func);
}

void loop() {
  // Loop repeatedly to execute scheduled tasks and handle events.
  os_runloop_once();
}

//  Pin mapping for Dragino LoRa Shield.
const lmic_pinmap lmic_pins = {
    .nss = 10,  // Connected to pin D10
    .rxtx = LMIC_UNUSED_PIN,  // For placeholder only, Do not connected on RFM92/RFM95
    .rst = 9,  // Needed on RFM92/RFM95? (probably not)
    .dio = {2, 6, 7},  // Specify pin numbers for DIO0, 1, 2 connected to D2, D6, D7 
};

void tx(const char *str, osjobcb_t func) {
  //  Transmit the given string to the LoRa gateway and call the given function afterwards.
  //  Message header contains destination address, source address, packet number, packet length.
  uint8_t header[4];
  header[0] = gateway_address;
  header[1] = device_address;
  header[2] = packet_num++;
  header[3] = (uint8_t) strlen(str);
  
  os_radio(RADIO_RST); //  Before sending, stop the receiving first.
  delay(1); //  Wait a bit, without this os_radio below asserts, apparently because the state hasn't changed yet.
  
  //  Copy the header into the message.
  LMIC.dataLen = 0;
  for (int i = 0; i < sizeof(header); i++)
    LMIC.frame[LMIC.dataLen++] = header[i];    
    
  //  Copy the data into the message.
  const char *s = str;
  while (*s)
    LMIC.frame[LMIC.dataLen++] = *s++;
  LMIC.osjob.func = func;
  
  //  Send the message.
  Serial.print("Sending to gateway "); Serial.print(gateway_address);
  Serial.print(": "); Serial.println(str);
  os_radio(RADIO_TX);
}

void rx(osjobcb_t func) {
  // Enable receive mode and call func when a packet is received.
  LMIC.osjob.func = func;
  LMIC.rxtime = os_getTime(); // RX _now_
  // Enable "continuous" RX (e.g. without a timeout, still stops after receiving a packet)
  os_radio(RADIO_RXON);
  Serial.println("Receiving...");
}

static void rxtimeout_func(osjob_t *job) {
  //  Switch off the LED on timeout.
  digitalWrite(LED_BUILTIN, LOW);
}

static void rx_func (osjob_t* job) {
  // Blink once to confirm reception and then keep the led on
  digitalWrite(LED_BUILTIN, LOW); // off
  delay(10);
  digitalWrite(LED_BUILTIN, HIGH); // on

  //  TODO: Test whether this is necessary.
  // Timeout RX (i.e. update led status) after 3 periods without RX
  os_setTimedCallback(&timeoutjob, os_getTime() + ms2osticks(3*transmit_interval), rxtimeout_func);
  // Reschedule TX so that it should not collide with the other side's next TX
  os_setTimedCallback(&txjob, os_getTime() + ms2osticks(transmit_interval/2), tx_func);

  Serial.print("Got ");
  Serial.print(LMIC.dataLen);
  Serial.println(" bytes");
  Serial.write(LMIC.frame, LMIC.dataLen);
  Serial.println();

  // Restart RX
  rx(rx_func);
}

static void txdone_func (osjob_t* job) {
  //  This is called just after sending a message to the gateway.
  rx(rx_func);
}

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
void onEvent (ev_t ev) { }

#if !defined(DISABLE_INVERT_IQ_ON_RX)
#error This program requires DISABLE_INVERT_IQ_ON_RX to be set. Update config.h in the lmic library to set it.
#endif

