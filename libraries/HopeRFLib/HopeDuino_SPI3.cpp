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
 * file       HopeDuino_3SPI.cpp
 * brief      for HopeRF's EVB to use softwave 3-SPI (for CMOSTEK RF-IC)
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.1
 * date       Jan 15 2015
 * author     QY Ruan
 */

#include "HopeDuino_SPI3.h"

/**********************************************************
**Name: 	vSpi3Init
**Func: 	Init Spi-3 Config
**Note: 	
**********************************************************/
void spi3Class::vSpi3Init(void)
{
	DDRB  |= (_CSB|_SDIO|_FCSB|_SDCK);	//SPI-3 for output
	PORTB |= (_CSB|_SDIO|_FCSB);		
	ClrSDCK();
}

/**********************************************************
**Name: 	vSpi3WriteByte
**Func: 	SPI-3 send one byte
**Input:
**Output:  
**********************************************************/
void spi3Class::vSpi3WriteByte(byte dat)
{

 	byte bitcnt;	
 
	SetFCSB();				//FCSB = 1;
 
 	OutputSDIO();			//SDA output mode
 	SetSDIO();				//    output 1
 
 	ClrSDCK();				
 	ClrCSB();

 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
		ClrSDCK();	
		_delay_us(SPI3_SPEED);
 		if(dat&0x80)
 			SetSDIO();
 		else
 			ClrSDIO();
		SetSDCK();
 		dat <<= 1; 		
 		_delay_us(SPI3_SPEED);
 		}
 	ClrSDCK();		
 	SetSDIO();
}

/**********************************************************
**Name: 	bSpi3ReadByte
**Func: 	SPI-3 read one byte
**Input:
**Output:  
**********************************************************/
byte spi3Class::bSpi3ReadByte(void)
{
	byte RdPara = 0;
 	byte bitcnt;
  
 	ClrCSB(); 
 	InputSDIO();			
 	
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		ClrSDCK();
 		RdPara <<= 1;
 		_delay_us(SPI3_SPEED);
 		SetSDCK();
 		_delay_us(SPI3_SPEED);
 		if(SDIO_H())
 			RdPara |= 0x01;
 		else
 			RdPara |= 0x00;
 		} 
 	ClrSDCK();
 	OutputSDIO();
 	SetSDIO();
 	SetCSB();			
 	return(RdPara);	
}

/**********************************************************
**Name:	 	vSpi3Write
**Func: 	SPI Write One word
**Input: 	Write word
**Output:	none
**********************************************************/
void spi3Class::vSpi3Write(word dat)
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
byte spi3Class::bSpi3Read(byte addr)
{
  	vSpi3WriteByte(addr|0x80);
 	return(bSpi3ReadByte());
}

/**********************************************************
**Name:	 	vSpi3WriteFIFO
**Func: 	SPI-3 send one byte to FIFO
**Input: 	one byte buffer
**Output:	none
**********************************************************/
void spi3Class::vSpi3WriteFIFO(byte dat)
{
 	byte bitcnt;	
 
 	SetCSB();	
	OutputSDIO();	
	ClrSDCK();
 	ClrFCSB();			//FCSB = 0
	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		ClrSDCK();
 		
 		if(dat&0x80)
			SetSDIO();		
		else
			ClrSDIO();
		_delay_us(SPI3_SPEED);
		SetSDCK();
		_delay_us(SPI3_SPEED);
 		dat <<= 1;
 		}
 	ClrSDCK();	
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	SetFCSB();
	SetSDIO();
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
}

/**********************************************************
**Name:	 	bSpi3ReadFIFO
**Func: 	SPI-3 read one byte to FIFO
**Input: 	none
**Output:	one byte buffer
**********************************************************/
byte spi3Class::bSpi3ReadFIFO(void)
{
	byte RdPara;
 	byte bitcnt;	
 	
 	SetCSB();
	InputSDIO();
 	ClrSDCK();
	ClrFCSB();
		
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		ClrSDCK();
 		RdPara <<= 1;
 		_delay_us(SPI3_SPEED);
		SetSDCK();
		_delay_us(SPI3_SPEED);
 		if(SDIO_H())
 			RdPara |= 0x01;		//NRZ MSB
 		else
 		 	RdPara |= 0x00;		//NRZ MSB
 		}
 	
 	ClrSDCK();
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	SetFCSB();
	OutputSDIO();
	SetSDIO();
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	return(RdPara);
}

/**********************************************************
**Name:	 	vSpi3BurstWriteFIFO
**Func: 	burst wirte N byte to FIFO
**Input: 	array length & head pointer
**Output:	none
**********************************************************/
void spi3Class::vSpi3BurstWriteFIFO(byte ptr[], byte length)
{
 	byte i;
 	if(length!=0x00)
	 	{
 		for(i=0;i<length;i++)
 			vSpi3WriteFIFO(ptr[i]);
 		}
 	return;
}

/**********************************************************
**Name:	 	vSpiBurstRead
**Func: 	burst wirte N byte to FIFO
**Input: 	array length  & head pointer
**Output:	none
**********************************************************/
void spi3Class::vSpi3BurstReadFIFO(byte ptr[], byte length)
{
	byte i;
 	if(length!=0)
 		{
 		for(i=0;i<length;i++)
 			ptr[i] = bSpi3ReadFIFO();
 		}	
 	return;
}

