//  Based on http://www.hoperf.com/demo_tools/419.html
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
 * file       lora_tx.ino
 * hardware   HopeDuino 
 * software   send message via HopeRF's LoRa COB
 * note       can talk to HopeRF's EVB or DK demo           
 *
 * version    1.0
 * date       Jun 10 2014
 * author     QY Ruan
 */

#include <HopeDuino_LoRa.h> 


loraClass radio;

byte str[21] = {'H','o','p','e','R','F',' ','R','F','M',' ','C','O','B','R','F','M','9','8','S'};

void setup()
{
 radio.Modulation     = LORA;
 ////radio.COB        = RFM98;
 radio.COB            = RFM96;  ////  TP-IoT: RFM96

 // Set center frequency
 ////uint32_t  freq = 868100000; // in Mhz! (868.1)
 ////  TP-IoT: uint32_t LORA_CH_10_868 = CH_10_868; //  0xD84CCC; // channel 10, central freq = 865.20MHz  ////  Lup Yuen

 ////radio.Frequency  = 434000;
 radio.Frequency      = 865200; ////  TP-IoT: 865.20 MHz
 radio.OutputPower    = 17;             //17dBm OutputPower
 ////radio.PreambleLength = 16;             //16Byte preamble
 radio.PreambleLength = 8;             //  TP-IoT: 8 Byte preamble
 radio.FixedPktLength = false;          //explicit header mode for LoRa
 radio.PayloadLength  = 21;
 radio.CrcDisable     = true;
 for (int i = 0; i < 8; i++)
  radio.SyncWord[i] = 0x12;  ////  TP-IoT

  ////  Mode 1 is max range but does NOT work with Dragino shield and Hope RF96 chip.
  ////  TP-IoT Gateway runs on:
  ////    case 1:     setCR(CR_5);        // CR = 4/5
  ////                setSF(SF_12);       // SF = 12
  ////                setBW(BW_125);      // BW = 125 KHz
  //  TP-IoT Mode 1: Bw125Cr45Sf4096
 ////radio.SFSel          = SF9;
 ////radio.BWSel          = BW125K;
 ////radio.CRSel          = CR4_5;
 radio.SFSel          = SF12;
 radio.BWSel          = BW125K;
 radio.CRSel          = CR4_5;

 radio.vInitialize();
 radio.vGoStandby();
}

void loop()
{
 radio.bSendMessage(str, 21);
 delay(1000);
}
 
