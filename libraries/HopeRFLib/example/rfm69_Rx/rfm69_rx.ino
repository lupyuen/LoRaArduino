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
 * website: www.HopeRF.com
 *          www.HopeRF.cn
 *
 */

/*! 
 * file       rfm69_rx.ino
 * hardware   HopeRF's RFduino TRx
 * software   get message via rfm69, & send message to uart
 * note       can talk to HopeRF's EVB or DK demo           
 *
 * version    1.0
 * date       Jun 10 2014
 * author     QY Ruan
 */

#include <HopeDuino_RFM69.h> 
#include <HopeDuino_UART.h>

rf69Class radio;
uartClass uart;

byte getstr[21];
void setup()
{
 radio.Modulation     = FSK;
 radio.COB            = RFM69;
 radio.Frequency      = 434000;
 radio.OutputPower    = 10+18;          //10dBm OutputPower
 radio.PreambleLength = 16;             //16Byte preamble
 radio.FixedPktLength = true;           //packet length didn't in message which need to be send
 radio.PayloadLength  = 21;
 radio.CrcDisable     = true;
 radio.AesOn          = false;

 radio.SymbolTime     = 416000;         //2.4Kbps
 radio.Devation       = 35;             //35KHz for devation
 radio.BandWidth      = 100;            //100KHz for bandwidth
 radio.SyncLength     = 3;              //
 radio.SyncWord[0]    = 0xAA;
 radio.SyncWord[1]    = 0x2D;
 radio.SyncWord[2]    = 0xD4;
 radio.vInitialize();
 radio.vGoRx();
 uart.vUartInit(9600, _8N1);
}

void loop()
{
 if(radio.bGetMessage(getstr)!=0)
    {
    uart.vUartPutNByte(getstr, 21);
    uart.vUartNewLine();
    }  	
}
 