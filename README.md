# LoRaArduino
Arduino code for connecting to LoRa IoT gateway at Temasek Polytechnic 

The code here runs on an Arduino Uno with LoRa shield (the LoRa Node) and connects to a Raspberry Pi 2 or 3 with LoRa shield (the LoRa Gateway). The devices and code here have been tested for sending small redundant data packets (under 50 bytes) at low speeds (100 bps) for infrequent transmissions (once every 30 seconds), at about 500 metres (or a few floors) in a campus environment, with the gateway placed behind a window.  

LoRa is not suitable for applications that require constant, lossless bandwidth.  Up to 254 nodes are supported in this implementation.  Transmissions are not secured in this implementation.

- Arduino sketch and libraries for Libelium LoRa shield:

**[SX_01a_TX_LoRa.ino](https://github.com/lupyuen/LoRaArduino/blob/master/SX_01a_TX_LoRa/SX_01a_TX_LoRa.ino)**

**[arduino-api](https://github.com/lupyuen/LoRaArduino/tree/master/libraries/arduino-api)**

**[arduinoLoRa](https://github.com/lupyuen/LoRaArduino/tree/master/libraries/arduinoLoRa)**


- Arduino sketch and libraries for Dragino LoRa shield:

**[rf95_client.ino](https://github.com/lupyuen/LoRaArduino/blob/master/rf95_client/rf95_client.ino)**

**[RadioHead](https://github.com/lupyuen/LoRaArduino/tree/master/libraries/RadioHead)**


- Python node script for Libelium LoRa shield:

**[test_lora_interface.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/test_lora_interface.py)**


## Supported LoRa Shields

- **Libelium LoRa shield for Raspberry Pi:** Based on Semtech SX1272 chip

https://www.cooking-hacks.com/sx1272-lora-shield-for-raspberry-pi-868-mhz

- **Libelium LoRa shield for Arduino:** Based on Semtech SX1272 chip

https://www.cooking-hacks.com/lora-radio-shield-for-arduino-868-mhz

- **Dragino LoRa shield for Arduino:** Cheaper than Libelium, based on Hope RF96 chip (licensed from Semtech) 

http://www.seeedstudio.com/item_detail.html?p_id=2651

**Note:** The Hope RF96 chip does not support Semtech's **Low Data Rate Optimization** (supported by Semtech SX1272 and SX1276)
so we won't be able to use LoRa Mode 1 for maximum range, minimum bandwidth transmission.  If the coding scheme were documented,
we should be able to support all modes on Hope RF96.

Refer to Page 28 of Semtech SX1272 Datasheet: http://www.semtech.com/images/datasheet/sx1272.pdf

> **Low Data Rate Optimization**
> Given the potentially long duration of the packet at high spreading factors the option is given to improve the robustness of
> the transmission to variations in frequency over the duration of the packet transmission and reception. The bit
> LowDataRateOptimize increases the robustness of the LoRa link at these low effective data rates, its use is mandated with
> **spreading factors of 11 and 12 at 125 kHz bandwidth.**

Compare with the glaring omission of Low Data Rate Optimization in Page 25 of Hope RF96 Datasheet: http://www.hoperf.com/upload/rf/RFM95_96_97_98W.pdf

Spreading factors 11/12 and 125 kHz bandwidth are needed for maximum range, minimum bandwidth transmission.

Hope RF96 seems to be derived from the Semtech SX1276: http://www.semtech.com/images/datasheet/sx1276.pdf

- **To be tested: Dragino LoRa shield for Raspberry Pi,** based on Hope RF96 chip

http://wiki.dragino.com/index.php?title=Lora/GPS_HAT

## LoRa Gateway

The software running on the Raspberry Pi + Libelium shield to function as a LoRa gateway is here:

https://github.com/lupyuen/RaspberryPiImage/tree/master/home/pi/LoRa
 
Check the installation instructions for LoRa gateway and LoRa node on Raspberry Pi here:

https://github.com/lupyuen/AWSIOT/blob/master/README.md
 
The LoRa Gateway script **[lora_gateway.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/lora_gateway.py)**
 will receive LoRa packets and forward them to AWS IoT, so that LoRa devices appear as IoT devices in the same manner as the TP-IoT environment used in the Temasek Polytechnic Smart IoT Applications course. So the same IoT rules, Lambda, Sumo Logic and Slack code will still work.

## Supported LoRa Modes

- **Mode 1:** Bandwidth = 125 KHz, Coding Rate = 4/5, Spreading Factor = 12

This mode offers the longest range.  It's supported **only on Libelium sheids** for Raspberry Pi / Arduino, not for Dragino shields, because this mode uses Low Data Rate Optimization.

Set `transmission_mode = 1` in [lora_gateway.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/lora_gateway.py),
[SX_01a_TX_LoRa.ino](https://github.com/lupyuen/LoRaArduino/blob/master/SX_01a_TX_LoRa/SX_01a_TX_LoRa.ino),
[RH_RF95.cpp](https://github.com/lupyuen/LoRaArduino/blob/master/libraries/RadioHead/RH_RF95.cpp)
and [test_lora_interface.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/test_lora_interface.py)

- **Mode 5:** Bandwidth = 250 KHz, Coding Rate = 4/5, Spreading Factor = 10

This mode supported on Libelium sheids and Dragino shields for Raspberry Pi / Arduino because it doesn't use Low Data Rate Optimization.

Set `transmission_mode = 5` in [lora_gateway.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/lora_gateway.py),
[SX_01a_TX_LoRa.ino](https://github.com/lupyuen/LoRaArduino/blob/master/SX_01a_TX_LoRa/SX_01a_TX_LoRa.ino),
[RH_RF95.cpp](https://github.com/lupyuen/LoRaArduino/blob/master/libraries/RadioHead/RH_RF95.cpp)
and [test_lora_interface.py](https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/test_lora_interface.py)


- What happens when you send a data packet from the Dragino shield in Mode 1?

The Libelium shield only receives the first 3 bytes intact, the rest of the message is scrambled.  Compare these logs:

https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/arduino_dragino.log

https://github.com/lupyuen/RaspberryPiImage/blob/master/home/pi/LoRa/arduino_libelium.log

Can we use this to send tiny packets?

## Photos

![LoRa Gateway with Raspberry Pi 3 + Libelium LoRa Shield](https://github.com/lupyuen/LoRaArduino/blob/master/images/gateway.jpg)

_LoRa Gateway with Raspberry Pi 3 + Libelium LoRa Shield_


![LoRa Node with Raspberry Pi 2 + Libelium LoRa Shield, Arduino Uno-compatible Seeeduino + Dragino LoRa Shield](https://github.com/lupyuen/LoRaArduino/blob/master/images/nodes.jpg)

_LoRa Nodes with Raspberry Pi 2 + Libelium LoRa Shield, Arduino Uno-compatible Seeeduino + Dragino LoRa Shield, and portable battery pack_


![Integration of LoRa Node to Slack via LoRa Gateway, AWS IoT Rules and AWS Lambda](https://github.com/lupyuen/LoRaArduino/blob/master/images/slack.png)

_Integration of LoRa Node to Slack via LoRa Gateway, AWS IoT Rules Engine and AWS Lambda_

## External Libraries

- Code for the Libelium LoRa shield is based on:

https://www.cooking-hacks.com/documentation/tutorials/extreme-range-lora-sx1272-module-shield-arduino-raspberry-pi-intel-galileo/ 

- Code for the Dragino LoRa shield is based on Radiohead RH_RF95:

http://www.airspayce.com/mikem/arduino/RadioHead/

## References

- Research project on the same topic:

http://cpham.perso.univ-pau.fr/LORA/RPIgateway.html

