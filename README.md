# UPDATE: This code is obsolete, please use this instead: https://github.com/mcci-catena/arduino-lmic

# I'm now working on LoRa for the BL602 RISC-V SoC, check this out: https://lupyuen.github.io/articles/lora2

# LoRaArduino
Arduino code for connecting to LoRa IoT gateway at Temasek Polytechnic 

The code here runs on an Arduino Uno with LoRa shield (the LoRa Node) and connects to a Raspberry Pi 2 or 3 with LoRa shield (the LoRa Gateway). The devices and code here have been tested for sending small redundant data packets (under 50 bytes) at low speeds (100 bps) for infrequent transmissions (once every 30 seconds), at about 500 metres (or a few floors) in a campus environment, with the gateway placed behind a window.  

LoRa is not suitable for applications that require constant, lossless bandwidth.  Up to 254 nodes are supported in this implementation.  Transmissions are not secured in this implementation.

- Arduino sketch and libraries for Libelium LoRa shield:

**[SX_01a_TX_LoRa.ino](https://github.com/lupyuen/LoRaArduino/blob/master/SX_01a_TX_LoRa/SX_01a_TX_LoRa.ino)**

**[arduino-api](https://github.com/lupyuen/LoRaArduino/tree/master/libraries/arduino-api)**

**[arduinoLoRa](https://github.com/lupyuen/LoRaArduino/tree/master/libraries/arduinoLoRa)**


- Arduino sketch and libraries for Dragino LoRa shield:

**[send_receive_sensor_data.ino](https://github.com/lupyuen/LoRaArduino/blob/master/send_receive_sensor_data/send_receive_sensor_data.ino)**

**[arduino-lmic](https://github.com/lupyuen/LoRaArduino/tree/master/libraries/arduino-lmic)**


- Raspberry Pi Python script for sensor node with Libelium LoRa shield and Dragino HAT:

**[test_lora_interface.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/test_lora_interface.py)**


## Supported LoRa Shields

- **Libelium LoRa shield for Raspberry Pi:** Based on Semtech SX1272 chip

https://www.cooking-hacks.com/sx1272-lora-shield-for-raspberry-pi-868-mhz

- **Libelium LoRa shield for Arduino:** Based on Semtech SX1272 chip

https://www.cooking-hacks.com/lora-radio-shield-for-arduino-868-mhz

- **Dragino LoRa GPS HAT for Raspberry Pi:** Cheaper than Libelium, based on Hope RFM96 chip (licensed from Semtech). **Note:** The Dragino HAT I tested had much shorter range than the Libelium shield, around 100 metres for the Dragino HAT in a dense area compared with a few hundred metres for the Libelium shield. Maybe I didn't configure the Dragino HAT correctly.

http://wiki.dragino.com/index.php?title=Lora/GPS_HAT

- **Dragino LoRa shield for Arduino:** Cheaper than Libelium, based on Hope RFM96 chip.  Works fine for long distances.

http://www.seeedstudio.com/item_detail.html?p_id=2651

**Note:** Although not documented, the Hope RFM96 actually supports Semtech's **Low Data Rate Optimization** (supported by Semtech SX1272 and SX1276) so we can use LoRa Mode 1 for maximum range, minimum bandwidth transmission.  Refer to Page 28 of Semtech SX1272 Datasheet: http://www.semtech.com/images/datasheet/sx1272.pdf

> **Low Data Rate Optimization**
> Given the potentially long duration of the packet at high spreading factors the option is given to improve the robustness of
> the transmission to variations in frequency over the duration of the packet transmission and reception. The bit
> LowDataRateOptimize increases the robustness of the LoRa link at these low effective data rates, its use is mandated with
> **spreading factors of 11 and 12 at 125 kHz bandwidth.**

Hope RFM96 seems to be derived from the Semtech SX1276: http://www.semtech.com/images/datasheet/sx1276.pdf.  Although not documented, Hope RFM96 (but not RFM95) supports the Low Data Rate Optimization setting in RegModemConfig3 (register 0x26) exactly the same way as the SX1276.  See Page 114 of the SX1276 datasheet.  I confirmed this by checking the Hope library code (HoepRF_HSP_V1.0\libraries\HopeRFLib\HopeDuino_LoRa.cpp) at http://www.hoperf.com/demo_tools/419.html

## LoRa Gateway

The software running on the Raspberry Pi + Libelium/Dragino shields to function as a LoRa gateway is here:

https://github.com/lupyuen/RaspberryPiImage/tree/master/home/pi/LoRa

Run `run_lora_gateway.sh` to start the gateway.  Run `stop_lora.sh` to stop the gateway.
 
Check the installation instructions for LoRa gateway and LoRa node on Raspberry Pi here:

https://github.com/lupyuen/AWSIOT/blob/master/README.md
 
The LoRa Gateway script **[lora_gateway.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/lora_gateway.py)**
 will receive LoRa packets and forward them to AWS IoT, so that LoRa devices appear as IoT devices in the same manner as the TP-IoT environment used in the Temasek Polytechnic Smart IoT Applications course. So the same IoT rules, Lambda, Sumo Logic and Slack code will still work.

## Supported LoRa Modes

- **Mode 1:** Bandwidth = 125 KHz, Coding Rate = 4/5, Spreading Factor = 12

This mode offers the longest range.  It's supported **only on Libelium sheids** for Raspberry Pi / Arduino, not for Dragino shields, because this mode uses Low Data Rate Optimization.

Set `transmission_mode = 1` in [lora_gateway.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/lora_gateway.py),
[SX_01a_TX_LoRa.ino](https://github.com/lupyuen/LoRaArduino/blob/master/SX_01a_TX_LoRa/SX_01a_TX_LoRa.ino),
[send_receive_sensor_data.ino](https://github.com/lupyuen/LoRaArduino/blob/master/send_receive_sensor_data/send_receive_sensor_data.ino)
and [test_lora_interface.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/test_lora_interface.py)

## Photos

![LoRa Gateway with Raspberry Pi 3 + Libelium LoRa Shield](https://github.com/lupyuen/LoRaArduino/blob/master/images/gateway.jpg)

_LoRa Gateway with Raspberry Pi 3 + Libelium LoRa Shield_


![LoRa Node with Raspberry Pi 2 + Libelium LoRa Shield, Arduino Uno-compatible Seeeduino + Dragino LoRa Shield](https://github.com/lupyuen/LoRaArduino/blob/master/images/nodes.jpg)

_LoRa Nodes with Raspberry Pi 2 + Libelium LoRa Shield, Arduino Uno-compatible Seeeduino + Dragino LoRa Shield, and portable battery pack_


![Integration of LoRa Node to Slack via LoRa Gateway, AWS IoT Rules and AWS Lambda](https://github.com/lupyuen/LoRaArduino/blob/master/images/slack.png)

_Integration of LoRa Node to Slack via LoRa Gateway, AWS IoT Rules Engine and AWS Lambda_

## External Libraries

- Code for the Libelium Arduino and Raspberry Pi shields is based on:

https://www.cooking-hacks.com/documentation/tutorials/extreme-range-lora-sx1272-module-shield-arduino-raspberry-pi-intel-galileo/ 

- Code for the Dragino Raspberry Pi HAT is based on the LoRaWAN Single Channel Packet Forwarder:

https://github.com/tftelkamp/single_chan_pkt_fwd

- Code for the Dragino Arduino shield is based on Arduino LMIC for LoRaWAN:

https://github.com/matthijskooijman/arduino-lmic

- The Dragino LoRa shield was previously running on the RadioHead library but it seemed too complicated.

http://www.airspayce.com/mikem/arduino/RadioHead/

## References

- Dragino setup for Arduino and Raspberry Pi:

http://www.instructables.com/id/Use-Lora-Shield-and-RPi-to-Build-a-LoRaWAN-Gateway/?ALLSTEPS

- Research project on the same topic:

http://cpham.perso.univ-pau.fr/LORA/RPIgateway.html

- More about LoRa and SIGFOX:

http://www.instructables.com/id/Introducing-LoRa-/?ALLSTEPS

