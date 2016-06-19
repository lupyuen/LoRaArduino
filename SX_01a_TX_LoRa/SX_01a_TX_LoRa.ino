/*  
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.2
 *  Design:            David Gascón
 *  Implementation:    Covadonga Albiñana, Victor Boria, Ruben Martin
 */

#include <Wire.h>
// Cooking API libraries
#include <arduinoUtils.h>
// Include the SX1272 and SPI library:
#include "arduinoLoRa.h"
#include <SPI.h>

//  TP-IoT
const int device_address = 4;  //  Must be unique for each device.
const int gateway_address = 1;
//const int transmission_mode = 1;  //  Max range, slow data rate.
const int transmission_mode = 5;  //  Better reach, medium time on air. Test this mode because it doesn't mandate Low Data Rate Optimisation, which is not supported on Hope RF95.
//const int transmission_channel = CH_10_868;
const char transmission_power = 'H';
const int receive_timeout = 10000;
char message [] = "4|1|2|3";

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // Print a start message
  Serial.println(F("SX1272 module and Arduino: send packets without ACK"));
  
  // Power ON the module
  int e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  
  // Set transmission mode and print the result
  e |= sx1272.setMode(transmission_mode);
  Serial.print(F("Setting Mode: state "));
  Serial.println(e, DEC);
  
  // Set header
  e |= sx1272.setHeaderON();
  Serial.print(F("Setting Header ON: state "));
  Serial.println(e, DEC);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  Serial.print(F("Setting Channel: state "));
  Serial.println(e, DEC);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  Serial.print(F("Setting CRC ON: state "));
  Serial.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower(transmission_power);
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  
  // Set the node address and print the result
  e |= sx1272.setNodeAddress(device_address);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));
}

void loop(void)
{
  // Send message1 and print the result
  int e = sx1272.sendPacketTimeout(gateway_address, message);
  Serial.print(F("Packet sent, state "));
  Serial.println(e, DEC);

  delay(10000);  
}

