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
 * file       lora_rx.ino
 * hardware   HopeDuino 
 * software   get message via HopeRF's LoRa COB, & send message to uart
 * note       can talk to HopeRF's EVB or DK demo           
 *
 * version    1.0
 * date       Jun 10 2014
 * author     QY Ruan
 */

#include <HopeDuino_LoRa.h> 
#include <HopeDuino_UART.h>

loraClass radio;
uartClass uart;

byte getstr[21];

void setup()
{
 radio.Modulation     = LORA;
 radio.COB            = RFM98;
 radio.Frequency      = 434000;
 radio.OutputPower    = 17;             //17dBm OutputPower
 radio.PreambleLength = 16;             //16Byte preamble
 radio.FixedPktLength = false;          //explicit header mode for LoRa
 radio.PayloadLength  = 21;
 radio.CrcDisable     = false;
 
 radio.SFSel          = SF9;
 radio.BWSel          = BW125K;
 radio.CRSel          = CR4_5;

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
 