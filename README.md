# LoRaArduino
Arduino code for connecting to LoRa IoT gateway at Temasek Polytechnic 

The code here runs on an Arduino Uno with LoRa shield (the LoRa Node) and connects to a Raspberry Pi 2 or 3 with LoRa shield (the LoRa Gateway)

## Supported LoRa Shields

- **Libelium LoRa shield for Raspberry Pi:** Based on Semtech SX1272 chip

https://www.cooking-hacks.com/sx1272-lora-shield-for-raspberry-pi-868-mhz

- **Libelium LoRa shield for Arduino:** Based on Semtech SX1272 chip

https://www.cooking-hacks.com/lora-radio-shield-for-arduino-868-mhz

- **Dragino LoRa shield for Arduino:** Cheaper than Libelium, based on Hope RF96 chip (licensed from Semtech) 

http://www.seeedstudio.com/item_detail.html?p_id=2651

**Note:** The Hope RF96 chip does not support Semtech's *Low Data Rate Optimization* (supported by Semtech SX1272 and SX1276)
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
 
Check the installation instructions for LoRa on Raspberry Pi here:

https://github.com/lupyuen/AWSIOT/blob/master/README.md
 
The `lora_gateway.py` script will receive LoRa packets and forward them to AWS IoT, so that LoRa devices appear as IoT devices in the same manner as the TP-IoT environment used in the Temasek Polytechnic Smart IoT Applications course. So the same IoT rules, Lambda, Sumo Logic and Slack code will still work.

## Photos

![LoRa Gateway with Raspberry Pi 3 + Libelium LoRa Shield](https://github.com/lupyuen/LoRaArduino/blob/master/images/gateway.jpg)

![LoRa Node with Raspberry Pi 2 + Libelium LoRa Shield, Arduino Uno-compatible Seeeduino + Dragino LoRa Shield](https://github.com/lupyuen/LoRaArduino/blob/master/images/nodes.jpg)

![Integration of LoRa Node to Slack via LoRa Gateway, AWS IoT Rules and AWS Lambda](https://github.com/lupyuen/LoRaArduino/blob/master/images/slack.jpg)

## External Libraries

- Code for the Libelium LoRa shield is based on:

https://www.cooking-hacks.com/documentation/tutorials/extreme-range-lora-sx1272-module-shield-arduino-raspberry-pi-intel-galileo/ 

- Code for the Dragino LoRa shield is based on Radiohead RH_RF95:

http://www.airspayce.com/mikem/arduino/RadioHead/

