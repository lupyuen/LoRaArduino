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
 *
 * website: www.HopeRF.com
 *          www.HopeRF.cn     
 */

/*! 
 * file       HopeDuino_CMT221xA.cpp
 * brief      driver for CMT221xA 
 * hardware   HopeDuino with RFM21x
 *            
 *
 * version    1.0
 * date       Feb 18 2016
 * author     QY Ruan
 */
 
#include <HopeDuino_CMT221xA.h>
 
/**********************************************************
**Name:     vCMT221xAInit
**Function: Init. CMT221xA
**Input:    none
**Output:   none
**********************************************************/
void cmt221xaClass::vCMT221xAInit(void)
{
 vSpi3Init();	

 TIFR0  = 0x00;
 TIMSK0 = 0x00;
 OCR0A  = 0x00;
 TCCR0A = 0x0E;		//
 TCCR0B = 0x00; 
 TCNT0  = 0x00; 
}

/**********************************************************
**Name:     bReadRssi
**Function: Read Rssi
**Input:    none
**Output:   none
**********************************************************/
byte cmt221xaClass::bReadRssi(void)
{
 return(bSpi3Read(0x45));
}
	
/**********************************************************
**Name:     bGetMessage
**Function: receive message
**Input:    msg, array pointer, pklen 
**Output:   msg. length, and zero mean nothing 
**********************************************************/
byte cmt221xaClass::bGetMessage(byte msg[], byte pklen)
{
 byte length=0;
 word count;
 word limit;
 word llim;
 byte rate;
 word bitcnt;
 word det_h;
 word det_l;
 byte i;
 if(DATA_L())				//Low Active
 	{
 	rate  = 8;				
 	limit = SymbolTime<<2;	//detect sync for 4*SymbolTime 	
 	limit = SymbolTime>>3;	//because rate=8us;
 	switch(Decode)
 		{
 		case E527:
 			for(count=0; count<limit; count++)
 				{
 				if(DATA_H())	
 					break;
				vDelayUs(rate);
 				}
 			if(count<limit)	//not enough
 				return(0);
 			count = (SymbolTime<<5)+(SymbolTime<<2);	//Max. Sync = 32*SymbolTime
 			count >>= 3;		//rate = 8us
 			for( ;count!=0; count--)
 				{
 				if(DATA_H())
 					break;
 				vDelayUs(rate);
 				}
 			for( bitcnt=0;  bitcnt<((word)pklen<<3) ; bitcnt++)
 				{
 				limit = (SymbolTime<<2)+SymbolTime;	//max  = 5*SymbolTime
 				limit = limit>>3;					//rate = 8us	
 				
 				llim  = SymbolTime>>1;
 				llim  = llim>>3;					//rate = 8us, for Low Limit
 				det_h = 0;
 				det_l = 0;
 				
 				//ClrSDAT();						//for test
 				//SetSDAT();						//for test
 				
 				for(count=0; count<limit; count++)
 					{
 					if(DATA_L())
 						break;
 					vDelayUs(rate);
 					det_h++;
 					}
 				if(count>=limit)					//over 
 					return(0);
 				if(count<llim)						//no enough
 					return(0);
		 				
 				if(bitcnt==(((word)pklen<<3)-1))			//最后一位
 					{
 					for(i=0; i<(pklen-1); i++)			//shift
 						{
 						msg[i] <<= 1;
 						if(msg[i+1]&0x80)
 							msg[i] |= 0x01;
 						else
 							msg[i] |= 0x00;
 						}
 					msg[pklen-1] <<= 1;

 					if(count>((SymbolTime<<1)>>3))		//
 						msg[pklen-1] |= 0x01;
 					else
 						msg[pklen-1] |= 0x00;
 					
 					length = pklen;
 					break;
 					}
				
 				for(count=0; count<limit; count++)
 					{
 					if(DATA_H())
 						break;
 					vDelayUs(rate);
 					det_l++;
 					}
 				if(count>=limit)					//over 
 					return(0);
 				if(count<llim)						//no enough
 					return(0); 				
				
 				for(i=0; i<(pklen-1); i++)			//shift
 					{
 					msg[i] <<= 1;
 					if(msg[i+1]&0x80)
 						msg[i] |= 0x01;
 					else
 						msg[i] |= 0x00;
 					}
 				msg[pklen-1] <<= 1;
				
 				if(det_h>=det_l)
 					msg[pklen-1] |= 0x01;
 				else
 					msg[pklen-1] |= 0x00;
 				}
 			length = (bitcnt>>3);
 			break;
 		case E201:
 			for(count=0; count<limit; count++)
 				{
 				if(DATA_H())	
 					break;
				vDelayUs(rate);
 				}
 			if(count<limit)	//not enough
 				return(0);
 			count = (SymbolTime<<4)+(SymbolTime<<2);	//Max. Sync = 16*SymbolTime
 			count >>= 3;		//rate = 8us
 			for( ;count!=0; count--)
 				{
 				if(DATA_H())
 					break;
 				vDelayUs(rate);
 				}
 			for( bitcnt=0;  bitcnt<((word)pklen<<3) ; bitcnt++)
 				{
 				limit = (SymbolTime<<2);			//max  = 4*SymbolTime
 				limit = limit>>3;					//rate = 8us	
 				
 				llim  = SymbolTime>>1;
 				llim  = llim>>3;					//rate = 8us, for Low Limit
 				det_h = 0;
 				det_l = 0;
 				
 				ClrSDAT();						//for test
 				SetSDAT();						//for test
 				
 				for(count=0; count<limit; count++)
 					{
 					if(DATA_L())
 						break;
 					vDelayUs(rate);
 					det_h++;
 					}
 				if(count>=limit)					//over 
 					return(0);
 				if(count<llim)						//no enough
 					return(0);
		 				
 				if(bitcnt==(((word)pklen<<3)-1))			//最后一位
 					{
 					for(i=0; i<(pklen-1); i++)			//shift
 						{
 						msg[i] <<= 1;
 						if(msg[i+1]&0x80)
 							msg[i] |= 0x01;
 						else
 							msg[i] |= 0x00;
 						}
 					msg[pklen-1] <<= 1;

 					if(count>((SymbolTime<<1)>>3))		//
 						msg[pklen-1] |= 0x00;
 					else
 						msg[pklen-1] |= 0x01;
 					
 					length = pklen;
 					break;
 					}
				
 				for(count=0; count<limit; count++)
 					{
 					if(DATA_H())
 						break;
 					vDelayUs(rate);
 					det_l++;
 					}
 				if(count>=limit)					//over 
 					return(0);
 				if(count<llim)						//no enough
 					return(0); 				
				
 				for(i=0; i<(pklen-1); i++)			//shift
 					{
 					msg[i] <<= 1;
 					if(msg[i+1]&0x80)
 						msg[i] |= 0x01;
 					else
 						msg[i] |= 0x00;
 					}
 				msg[pklen-1] <<= 1;
				
 				if(det_h>=det_l)
 					msg[pklen-1] |= 0x00;
 				else
 					msg[pklen-1] |= 0x01;
 				}
 			length = (bitcnt>>3); 			
 			break;
 		default:
 			length = 0;
 			break;
 		}
 	return(length);
	}
 else
 	return(0);
}
 
 

//***************************HAL***************************
/**********************************************************
**Name: 	vSpi3Init
**Func: 	Init Spi-3 Config
**Note: 	
**********************************************************/
void cmt221xaClass::vSpi3Init(void)
{
	DDRD  |= (_CSB|_SDAT|_SCLK);	//SPI-3 for output
	DDRD  &= (~_DATA);
	
	PORTD |= (_CSB|_SDAT);	
	PORTD &= (~(_SCLK|_DATA));
	ClrSCLK();
}

/**********************************************************
**Name: 	vSpi3WriteByte
**Func: 	SPI-3 send one byte
**Input:
**Output:  
**********************************************************/
void cmt221xaClass::vSpi3WriteByte(byte dat)
{

 	byte bitcnt;	
 
 	OutputSDAT();			//SDA output mode
 	SetSDAT();				//    output 1
 
	ClrCSB();
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
		ClrSCLK();	
 		if(dat&0x80)
 			SetSDAT();
 		else
 			ClrSDAT();
		SetSCLK();
 		dat <<= 1; 		
 		}
 	ClrSCLK();
 	SetSDAT();
}

/**********************************************************
**Name: 	bSpi3ReadByte
**Func: 	SPI-3 read one byte
**Input:
**Output:  
**********************************************************/
byte cmt221xaClass::bSpi3ReadByte(void)
{
	byte RdPara = 0;
 	byte bitcnt;
  
 	ClrCSB(); 
 	InputSDAT();			
 	
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		ClrSCLK();
 		RdPara <<= 1;
 		SetSCLK();
 		if(SDAT_H())
 			RdPara |= 0x01;
 		else
 			RdPara |= 0x00;
 		} 
 	ClrSCLK();
 	OutputSDAT();
 	SetSDAT();
 	SetCSB();			
 	return(RdPara);	
}

/**********************************************************
**Name:	 	vSpi3Write
**Func: 	SPI Write One word
**Input: 	Write word
**Output:	none
**********************************************************/
void cmt221xaClass::vSpi3Write(word dat)
{
 	vSpi3WriteByte((byte)(dat>>8)&0x7F);
 	vSpi3WriteByte((byte)dat);
 	SetCSB();
}

/**********************************************************
**Name:	 	bSpi3Read
**Func: 	SPI-3 Read One byte
**Input: 	readout addresss
**Output:	readout byte
**********************************************************/
byte cmt221xaClass::bSpi3Read(byte addr)
{
  	vSpi3WriteByte(addr|0x80);
 	return(bSpi3ReadByte());
}

/**********************************************************
**Name:	 	bSpi3Read
**Func: 	SPI-3 Read One byte
**Input: 	readout addresss
**Output:	readout byte
**********************************************************/
void cmt221xaClass::vDelayUs(byte delay)
{
 byte i;	
 if(delay>=16)
 	delay = 15;
 i = (delay<<4);
 i-= 6;					//compensate
 OCR0A  = i;			//unit = 1/16 us	
 TCCR0B = 0x01;			//CLK/1 = 16MHz/1 = 16MHz Start	
 while((TIFR0&0x02)!=0x02);
 TIFR0 = 0x02;			//Clear flag
 TCCR0B = 0x00;
}	