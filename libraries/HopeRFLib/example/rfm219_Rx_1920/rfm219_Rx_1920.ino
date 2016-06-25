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
 * file       rfm219_rx_1920.ino
 * hardware   HopeDuino with RFM219 & CMOSTEK CMT2157A-EM
 * software   get message via rfm21x
 * note       
 *
 * version    1.0
 * date       Mar 28 2016
 * author     QY Ruan
 */
 
#include <HopeDuino_CMT2219A.h> 
#include <HopeDuino_UART.h>

cmt2219aClass radio;
uartClass uart;


byte CfgTbl[62] = {
					0x72,			//  Mode                   = Advanced           
					0x42,           //  Part Number            = CMT2219A           
					0x44,           //  Frequency              = 868.350 MHz        
					0x15,           //  Demodulation           = (G)FSK             
					0x0D,           //  Symbol Rate            = 4.8 ksps           
					0x63,           //  Xtal Tolerance         = +/- 10 ppm         
					0x9A,           //  Xtal Stabilizing Time  = 310 us             
					0x80,           //  Squelch TH             = 0                  
					0xC6,           //  Sleep Timer            = Off                
					0xA9,           //  Sleep Time             = NA                 
					0x00,           //  Rx Timer               = Off                
					0x00,           //  Rx Time                = NA                 
					0x62,           //  Rx Time Ext            = NA                 
					0x2E,           //  Rx Early Exit          = Off                
					0x00,           //  State After Rx Exit    = NA                 
					0x90,           //  System Clock Output    = Off                
					0x84,           //  System Clock Frequency = NA                 
					0x14,           //  Wake-On Radio          = Off                
					0xE0,           //  Wake-On Condition      = NA                 
					0x00,           //  Demod Method           = NA                 
					0x27,           //  Fixed Demod TH         = NA                 
					0x9F,           //  Peak Drop              = NA                 
					0x53,           //  Peak Drop Step         = NA                 
					0x53,           //  Peak Drop Rate         = NA                 
					0xCA,           //  Deviation              = 35.0 kHz           
					0xCA,           //  Sync Clock Type        = Tracing            
					0x00,           //  Data Representation    = 0:F-low  1:F-high  
					0x3C,           //  Rising Relative TH     = 21                 
					0x01,           //  Falling Relative TH    = 255                
					0x01,           //  AFC                    = On                 
					0x55, //Length  //  Data Mode              = Packet             
					0x21,           //  Packet Type            = Fixed Length       
					0x07,           //  FIFO Threshold         = 32                 
					0x84,           //  De-Whitening Seed      = NA                 
					0x00,           //  DC-Free Decode         = None               
					0x00,           //  FILE CRC               = AAAD               
					0x19,                                                           
					0x00,
					0x00,
					0x00,
					0xAC,
					0xAE,
					0x53,
					0xD4,
					0x40,
					0x49,
					0xFF,
					0x1B,
					0x12,
					0x00,
					0x90,
					0xFA,
					0x00,
					0x00,
					0x40,
					0xC0,
					0x00,
					0x00,
					0x20,
					0xCA,
					0x07,
					0x00
                 };

byte getstr[32];					//Rx Buffer 
byte hexstr[5];						//Analysis Buffer to HexCode according to BitFormat
byte sendstr[32];					//Change HexCode to ASCII, and print by UART
byte length;						//sendstr[]'s length
byte keycode;						//CMT2157A press key code
byte crc_rx;

byte crc_calc;

byte BitFormat;						//CMT2157A BitFormat, range for [3-6]
byte IDBitLength;					//CMT2157A ID Bit length, range for [1-32]

void setup()
{
 BitFormat   = 5;		//5-Symbol to 1-logic-bit
 IDBitLength = 27;		//27bits for ID


 radio.CrcDisable     = true;
 radio.FixedPktLength = true;
 radio.NodeDisable    = true;
 radio.PktLength      = 21;			// CMT2157A-1920: [((ID-Length+4)*BitFormat)+8]/8      "4" for KeyValue
 									// just like: ID-Length=27bits, BitFormat=5Symbol/bit
 									// so that:   ((27+4)*5+8)/8 = 21Byte
 radio.vInit(CfgTbl);
 radio.vGpioFuncCfg(GPIO1_INT1|GPIO2_DCLK|GPIO3_CLK|GPIO4_Dout);
 radio.vIntSourcCfg((FIFO_WBYTE+OFFSET), 0);
 radio.vEnableIntSource(0xFF);
 radio.vGoRx();
 uart.vUartInit(9600, _8N1);
}

void loop()
{
 byte i;

 if(radio.bGetMessage(getstr)!=0)
    {
    for(i=0; i<5; i++)
    	hexstr[i] = 0x00;
    keycode = 0;	
    crc_rx = bAnalysisMsg(getstr, BitFormat, IDBitLength, &keycode, hexstr);
    if(bCalcCrc8(getstr, BitFormat, IDBitLength, crc_rx))
        vAssembleSendMsg(hexstr, IDBitLength, keycode, true, sendstr);
    else
        vAssembleSendMsg(hexstr, IDBitLength, keycode, false, sendstr);
    uart.vUartPutNByte(sendstr, length);
    uart.vUartNewLine();
    }  	
}

//*********************************************************
//Analysis 1920 Code, assemble uart message 
//input:  inptr:       
//        bit_format: 
//        id_length:
//        *key: 
//        outptr:
//output: crc-8 value 
//*********************************************************
byte bAnalysisMsg(byte inptr[], byte bit_format, byte id_length, byte *key, byte outptr[])			
{
 byte i; 	//initial value
 byte j;
 byte k;
 byte x;
 byte y;
 byte z;
 
 byte crc = 0;
 
 switch(bit_format)
 	{
 	case 4: i = 0x40; break; 
 	case 5: i = 0x20; break;
 	case 6: i = 0x10; break;
 	case 3:
 	default:i = 0x40; bit_format = 3; break;
 	}
 
 for(j=0, k=0, x=0, y=0x01; j<id_length; j++)	//Analysis ID Part
 	{
 	if(i&inptr[k])
 		outptr[x] |= y;

 	y <<= 1;
 	if(0==y)
 		{
 		if((bit_format==3)||(bit_format==5))
 			outptr[x] ^= 0xFF;
 		x++;
 		y = 0x01;
 		}
 	
	for(z=0; z<bit_format; z++) 		
		{
		i >>= 1;	
		if(0==i)
			{
			k++;
			i = 0x80;
			}
		}
 	}
 if((id_length%8)!=0)
 	{	
 	byte tmp;
 	if((bit_format==3)||(bit_format==5))
 		outptr[x] ^= 0xFF;
 	tmp = (id_length%8)-1;
 	for(y=0x01;tmp!=0;tmp--)
 		{
 		y <<= 1;
 		y |= 0x01;
 		}
 	outptr[x] &= y;
 	}
 
 for(y=0x01; ; )					//Analysis Key Part		
	{
 	if(i&inptr[k])
 		(*key) |= y;

 	y <<= 1;
 	if((y&0x0F)==0)
 		{
 		if((bit_format==3)||(bit_format==5))
 			(*key) ^= 0xFF;
 		if(bit_format==3)
 			z = 2;
 		else
 			z = 3;
 		for( ; z!=0; z--)
 			{
 			i >>= 1;
 			if(0==i)
 				{
 				k++;
 				i = 0x80;
 				}
 			}
 		break;
 		}
 	else
 		{
		for(z=0; z<bit_format; z++) 		
			{
			i >>= 1;	
			if(0==i)
				{
				k++;
				i = 0x80;
				}
			}
		}
 	} 			
 
 for(j=0, y=0x80; j<8; j++)					//Analysis Crc Part
 	{
 	if(i&inptr[k])
 		crc |= y;
 	y >>= 1;
	i >>= 1;	
	if(0==i)
		{
		k++;
		i = 0x80;
		}
 	}
 	
 return(crc);
} 

//********************************************************* 
//Assemble Message Send via UART
//*********************************************************
void vAssembleSendMsg(byte inptr[], byte id_length, byte key, bool crc_ok, byte outptr[])
{
 byte i, j;
 i = 0;
 outptr[i++] = 'I';
 outptr[i++] = 'D';
 outptr[i++] = ':';
 outptr[i++] = '0';
 outptr[i++] = 'x';
 for(j=0; j<(id_length>>3); j++)
   	{
   	outptr[i++] = bChangeToAscii(inptr[j]);	
   	outptr[i++] = bChangeToAscii((inptr[j]>>4));
   	}
 if((id_length%8)!=0)
 	{
 	outptr[i++] = bChangeToAscii(inptr[j]);
 	if((id_length%8)>4)
 		outptr[i++] = bChangeToAscii((inptr[j]>>4));	
 	}
 outptr[i++] = ' ';
 outptr[i++] = 'K';
 outptr[i++] = 'E';
 outptr[i++] = 'Y';
 outptr[i++] = ':';
 outptr[i++] = '0';
 outptr[i++] = 'x'; 
 outptr[i++] = bChangeToAscii(key);
 outptr[i++] = ' ';
 outptr[i++] = 'C';
 outptr[i++] = 'R';
 outptr[i++] = 'C';
 outptr[i++] = '-';
 if(crc_ok)
 	{
 	outptr[i++] = 'O';
 	outptr[i++] = 'K'; 
 	}
 else
 	{
 	outptr[i++] = 'F';
 	outptr[i++] = 'a';
 	outptr[i++] = 'i';
 	outptr[i++] = 'l'; 	
 	}

 length = i;
}

//********************************************************* 
//Change buffer to ASCII
//********************************************************* 
byte bChangeToAscii(byte ch)						
{
 ch &= 0x0F;
 if(ch<0x0A)
 	ch += '0';
 else
 	{
 	ch -= 0x0A;
 	ch += 'A';
	}
 return(ch);
}

//********************************************************* 
//Calculate CRC-8, CMOSTEK format
//********************************************************* 
bool bCalcCrc8(byte inptr[], byte bit_format, byte id_length, byte crc_check)
{
 byte crc_tmp;
 byte crc_bitcnt;
 byte i, z;
 
 if((bit_format<3)||(bit_format>6))
 	bit_format = 3;
 crc_bitcnt = (id_length+4)*bit_format;
 
 crc_tmp = 0;
 for(i=0;i<crc_bitcnt;)
 	{
	for(z=0x80;z!=0;z>>=1)
		{
		if(inptr[(i/8)]&z)
			crc_tmp = bCMOSCRC8Bit(0x80, crc_tmp);
		else
			crc_tmp = bCMOSCRC8Bit(0x00, crc_tmp);
		i++;
		if(i>=crc_bitcnt)		
			break;
		}
	}

 crc_calc = crc_tmp;
	
 if(crc_tmp==crc_check)
 	return(true);
 else
 	return(false);
}

byte bCMOSCRC8Bit(byte RCRCData, byte RCRCResult)
{
 //RCRCData = 0x80 or 0x00
 byte tmp;
 if((RCRCData^RCRCResult)&0x80)		//
 	{
	RCRCResult ^= 0x18;
	tmp = 0x01;
	}		   
 else
 	tmp = 0x00;
 RCRCResult <<= 1; 	
 RCRCResult |= tmp;
 
 return(RCRCResult);
}
