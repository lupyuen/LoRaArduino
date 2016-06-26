//  Send sample sensor data to LoRa gateway.  Tested on Dragino LoRa GPS HAT for Raspberry Pi.
//  Based on https://github.com/matthijskooijman/arduino-lmic

//  TP-IoT Mode 1 is max range. TP-IoT Gateway runs on: CR = 4/5, SF = 12, BW = 125 KHz.
extern int transmission_mode;
int transmission_mode = 1;
const int device_address = 3;  //  My own address as assigned by TP-IoT.
const int gateway_address = 1;  //  TP-IoT Gateway address.
const int TX_INTERVAL = 5000;  //  How often to send a packet, in milliseconds.
int packet_num = 1;  //  Number of packets sent to gateway so far.

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
osjob_t txjob, timeoutjob;  //  Transmission and timeout tasks.

static void tx_func (osjob_t* job) {
  //  Send a message containing sample sensor data to the gateway, delimited by "|".
  //  The sequence must match lora_gateway.py: temperature, humidity, message.
  tx("28.1|72|OK", txdone_func);
  
  // Send the next set of sensor data every TX_INTERVAL (plus a bit of random to prevent systematic collisions).
  os_setTimedCallback(job, os_getTime() + ms2osticks(TX_INTERVAL + random(500)), tx_func);
}

// Program starts here.
void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(LED_BUILTIN, OUTPUT);  //  Allow use of LED.

  // Initialize runtime env.
  os_init();

  // Set up these settings once, and use them for both sending and receiving.
  LMIC.freq = 865200000; // TP-IoT: 865.20 MHz, CH_10_868, central freq = 865.20MHz
  LMIC.datarate = DR_SF12;  //  TP-IoT Mode 1: Spread Factor 12.

  // Maximum transmission power.
  LMIC.txpow = 27;

  // This sets the default coding and bandwidth, to be updated in radio.c when we transmit.
  LMIC.rps = updr2rps(LMIC.datarate);
  Serial.println("Started"); Serial.flush();

  // First task to be run is to send sensor data.
  os_setCallback(&txjob, tx_func);
}

void loop() {
  // Loop repeatedly to execute scheduled jobs and events.
  os_runloop_once();
}

//  TP-IoT Pin mapping for Dragino LoRa GPS HAT.
const lmic_pinmap lmic_pins = {
    .nss = 10,// Connected to pin D10
    .rxtx = LMIC_UNUSED_PIN,// For placeholder only, Do not connected on RFM92/RFM95
    .rst = 9,// Needed on RFM92/RFM95? (probably not)
    .dio = {2, 6, 7},// Specify pin numbers for DIO0, 1, 2 connected to D2, D6, D7 
};

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

void onEvent (ev_t ev) { }

// Transmit the given string to the LoRa gateway and call the given function afterwards.
void tx(const char *str, osjobcb_t func) {
  //  Header contains destination address, source address, packet number, packet length.
  uint8_t header[4] = { gateway_address, device_address, packet_num++, strlen(str) };
  os_radio(RADIO_RST); // Stop RX first
  delay(1); // Wait a bit, without this os_radio below asserts, apparently because the state hasn't changed yet
  LMIC.dataLen = 0;
  //  Copy the header.
  for (int i = 0; i < sizeof(header); i++)
    LMIC.frame[LMIC.dataLen++] = header[i];    
  //  Copy the data.
  while (*str)
    LMIC.frame[LMIC.dataLen++] = *str++;
  LMIC.osjob.func = func;
  //  Send the data.
  Serial.print("Sending to gateway ");
  Serial.print(gateway_address);
  Serial.print(": ");
  Serial.println(str);
  os_radio(RADIO_TX);
}

// Enable rx mode and call func when a packet is received.
void rx(osjobcb_t func) {
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
  os_setTimedCallback(&timeoutjob, os_getTime() + ms2osticks(3*TX_INTERVAL), rxtimeout_func);
  // Reschedule TX so that it should not collide with the other side's next TX
  os_setTimedCallback(&txjob, os_getTime() + ms2osticks(TX_INTERVAL/2), tx_func);

  Serial.print("Got ");
  Serial.print(LMIC.dataLen);
  Serial.println(" bytes");
  Serial.write(LMIC.frame, LMIC.dataLen);
  Serial.println();

  // Restart RX
  rx(rx_func);
}

static void txdone_func (osjob_t* job) {
  rx(rx_func);
}

#if !defined(DISABLE_INVERT_IQ_ON_RX)
#error This program requires DISABLE_INVERT_IQ_ON_RX to be set. Update config.h in the lmic library to set it.
#endif

