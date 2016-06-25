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
 * file       rfm11x_tx.ino
 * hardware   HopeDuino 
 * software   send message via rfm11x
 * note       
 *
 * version    1.0
 * date       Feb 19 2016
 * author     QY Ruan
 */
 
 #include <HopeDuino_CMT211xA.h> 

cmt211xaClass radio;

byte str[7] = {'C', 'M', 'O', 'S', 'T', 'E', 'K'};

void setup()
{
 radio.Chipset        = CMT2110A;
 radio.SymbolTime     = 416;           
 radio.vCMT211xAInit();
 radio.vEncode(str, 7, E527);
}

void loop()
{
 radio.vTxPacket();
 _delay_ms(100);
}

