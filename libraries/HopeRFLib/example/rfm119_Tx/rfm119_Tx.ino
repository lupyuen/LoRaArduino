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
 * file       rfm119_Tx.ino
 * hardware   HopeDuino 
 * software   send message via rfm119
 * note       
 *
 * version    1.0
 * date       Feb 19 2016
 * author     QY Ruan
 */
 
 #include <HopeDuino_CMT211xA.h> 

cmt211xaClass radio;

byte str[31] = {
            0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x2D, 0xD4,
	         'H',  'o',  'p',  'e',  'R',  'F',  ' ',  'R',  'F',  'M', 
	         ' ',  'C',  'O',  'B',  'R',  'F',  'M',  '1',  '1',  '9',
	         'S', 
	        };

word CfgTbl[21] = {
				 0x007F,         // Mode                = Advanced                                             	
				 0x1400,         // Part Number         = CMT2119A                         
				 0x0000,         // Frequency           = 434 MHz                          
				 0x0000,         // Modulation          = FSK                              
				 0x0000,         // Symbol Rate         = 0.5-100.0 ksps                   
				 0xF000,         // Tx Power            = +14 dBm                          
				 0x0000,         // Deviation           = 35.0 kHz                         
				 0xC4EC,         // PA Ramping Time     = NA                               
				 0x4208,         // Xtal Cload          = 15.00 pF                         
				 0x0160,         // Data Representation = 0:F-low,1:F-high                 
				 0x2400,         // Tx Start by         = DATA Pin Falling Edge            
				 0x0081,         // Tx Stop by          = DATA Pin Holding Low For 20 ms   
				 0x8000,         // Increase XO Current = No                               
				 0x0000,         // FILE CRC            = D150                             
				 0xFFFF,
				 0x0020,
				 0x5FE8,
				 0xA2D6,
				 0x0E13,
				 0x0019,
				 0x0000
                 };

void setup()
{
 radio.Chipset        = CMT2119A;
 radio.SymbolTime     = 416;           
 radio.vCMT2119AInit(CfgTbl, 21);
 radio.vEncode(str, 31, ENRZ);
}

void loop()
{
 radio.vTxPacket();
 _delay_ms(100);
}
