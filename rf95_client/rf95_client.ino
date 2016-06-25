// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with the RFM95W 
//  Hope RF95 Datasheet: http://www.hoperf.com/upload/rf/RFM95_96_97_98W.pdf

#include <SPI.h>
#include <RH_RF95.h>

extern int transmission_mode = 1;  //  Long range but NOT supported on Dragino and Hope RF96.
//extern int transmission_mode = 5;  //  Medium range and supported on Dragino and Hope RF96.
const int device_address = 3;  //  My own address as assigned by TP-IoT.
const int gateway_address = 1;  //  TP-IoT Gateway address.
const int random_digits = 8;  //  Number of digits for Low Data Rate Optimization testing.
int seed = 0x1;  //  Random seed for Low Data Rate Optimization testing.  Set to 0 to disable testing.

// Singleton instance of the radio driver
RH_RF95 rf95;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) {} // Wait for serial port to be available

  //  Initialise the LoRa shield.
  if (!rf95.init())
    Serial.println("Init failed");

  //  Set the addresses of this device and the gateway.
  rf95.setThisAddress(device_address);
  rf95.setHeaderFrom(device_address);
  rf95.setHeaderTo(gateway_address);
  rf95.setHeaderId(0);  //  Byte 3 of header is the message counter.  
  if (seed > 0) randomSeed(seed);  //  Set the random seed for test mode.
}

//  Buffer for sending sensor data.
char buffer[256];
char buffer2[20];

//  Number of messages sent and received.
int send_count = 0;
int receive_count = 0;
int random_sequence = 0;

void sendMessage(char *msg, int msg_len)
{
  //  Send the message.
  Serial.print("Sending to LoRa gateway: 0x");
  Serial.print(msg_len, HEX);
  Serial.print(" bytes / ");
  Serial.println(msg);
  rf95.setHeaderFlags(msg_len, 0xff);  //  Byte 4 of header is the message length. 0xff will clear all bits and replace by buffer_len.
  rf95.send((uint8_t *) buffer, msg_len);
  send_count++;
  rf95.waitPacketSent();
  Serial.println("Sent");
}

char *getRandomPacket()
{
  //  Return a random packet for Low Data Rate Optimization testing.
  Serial.print("getRandomPacket: seed,random_sequence=");
  Serial.print(seed);
  Serial.print(",");
  Serial.println(random_sequence);

  if (random_sequence == 0) {
    //  For the first sequence, send 1 packet with source=0 followed by 1 packet with source=seed value.
    rf95.setHeaderFrom(0);
    sendMessage("00000000", 9);
    delay(5000);
    sendMessage("00000000", 9);
    delay(5000);
    sendMessage("00000000", 9);
    delay(5000);
    sendMessage("00000000", 9);
    delay(5000);
    rf95.setHeaderFrom(seed);
    sendMessage("00000000", 9);
    delay(5000);
    sendMessage("00000000", 9);
    delay(5000);
    sendMessage("00000000", 9);
    delay(5000);
    sendMessage("00000000", 9);
    delay(5000);
  }

  //  Set the from address to the sequence ID for correlation.
  rf95.setHeaderFrom(random_sequence);
  sprintf(buffer2, "0%d", seed);
  int buffer2_len = strlen(buffer2);
  buffer2[0] = (buffer2_len - 1)+ '0';
  
  //  Compose a sequence of random digits.
  for (int i = 0; i < random_digits; i++) {
    int digit = random('0', '9');
    //  For the first sequence, repeat the seed value.
    if (random_sequence == 0) digit = buffer2[i % buffer2_len];
    buffer[i] = digit;  
  }  
  buffer[random_digits] = 0;
  random_sequence++;
  if (random_sequence > 255) {
    random_sequence = 0;
    seed++;
    randomSeed(seed);
  }
  return buffer;
}

void loop()
{
  // Send and receive data loop.
  // Send sensor data to the gateway.  Sequence must match the gateway definition.
  int status = -1, setup_done = -1;  
  sprintf(buffer, "%d|%d|%d|%d|%d|%d", device_address, gateway_address, status, setup_done, send_count, receive_count);
  if (seed > 0) getRandomPacket();  //  For testing Low Data Rate Optimization.
  int buffer_len = strlen(buffer) + 1;  //  Including trailing 0.  
  sendMessage(buffer, buffer_len);
    
  // Now wait for a reply from the gateway.
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.waitAvailableTimeout(1000))
  { 
   if (rf95.recv(buf, &len))
   {
      //  Show the reply.
      Serial.print("Got reply from LoRa gateway: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      receive_count++;
   }
   else
   {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply");
  }
  delay(1000);
}

// The default transmitter power is 13dBm, using PA_BOOST.
// If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
// you can set transmitter powers from 5 to 23 dBm:
//  driver.setTxPower(23, false);
// If you are using Modtronix inAir4 or inAir9,or any other module which uses the
// transmitter RFO pins and not the PA_BOOST pins
// then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
// Failure to do that will result in extremely low transmit powers.
//  driver.setTxPower(14, true);

// dst, src, packnum, length, data, retry
//uint8_t data[] = "3|1|2|3";
//uint8_t data[] = "1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20";
//uint8_t data[] = { 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x00 };
//uint8_t data[] = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x00 };
//uint8_t data[] = { 0x1, 0x2, 0x00 };
/*
  uint8_t buffer[] = { 0x01, 0x00, 0x01, 0x00, 0x01, 0x00 };
  int buffer_len = 6;
*/  

