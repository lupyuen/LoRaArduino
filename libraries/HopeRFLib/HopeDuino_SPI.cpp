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
 * file       HopeDuino_SPI.cpp
 * brief      for HopeRF's EVB to use Hardware SPI
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.1
 * date       Jan 15 2015
 * author     QY Ruan
 */

#include "HopeDuino_SPI.h"

/**********************************************************
**Name: 	vSpiInit
**Func: 	Init Spi Config
**Note: 	SpiClk = Fcpu/4
**********************************************************/
void spiClass::vSpiInit(void)
{
 	nSS_Dir |= (nSS_Value+_SDI+_SCK);	//nSS/SDI/SCK as Output
 	nSS_Dir &= (~_SDO);					//SDO as Input
	SetnSS();							//Output High level
	SPCR = _BV(SPE)|_BV(MSTR);			//SPI Master/MSB First/00/ Fcpu/4
	SPSR = 0x00;
}

/**********************************************************
**Name: 	bSpiTransfer
**Func: 	Transfer One Byte by SPI
**Input:
**Output:  
**********************************************************/
byte spiClass::bSpiTransfer(byte dat)
{
 SPDR = dat;
 while (!(SPSR & _BV(SPIF)));		//wait for send over
 return(SPDR);
}

/**********************************************************
**Name:	 	vSpiWrite
**Func: 	SPI Write One word
**Input: 	Write word
**Output:	none
**********************************************************/
void spiClass::vSpiWrite(word dat)
{
 ClrnSS();
 bSpiTransfer((byte)(dat>>8)|0x80);
 bSpiTransfer((byte)dat);
 SetnSS();
}

/**********************************************************
**Name:	 	bSpiRead
**Func: 	SPI Read One byte
**Input: 	readout addresss
**Output:	readout byte
**********************************************************/
byte spiClass::bSpiRead(byte addr)
{
 byte tmp;
 ClrnSS();
 bSpiTransfer(addr);
 tmp = bSpiTransfer(0xFF);
 SetnSS();
 return(tmp);
}

/**********************************************************
**Name:	 	vSpiBurstWirte
**Func: 	burst wirte N byte
**Input: 	array length & start address & head pointer
**Output:	none
**********************************************************/
void spiClass::vSpiBurstWrite(byte addr, byte ptr[], byte length)
{
 byte i;
 ClrnSS();
 bSpiTransfer(addr|0x80);
 for(i=0; i<length; i++)
 	bSpiTransfer(ptr[i]);
 SetnSS();
}

/**********************************************************
**Name:	 	vSpiBurstRead
**Func: 	burst read N byte
**Input: 	array length & start address & head pointer
**Output:	none
**********************************************************/
void spiClass::vSpiBurstRead(byte addr, byte ptr[], byte length)
{
 if(length!=0)
 	{
 	if(length==1)
 		{
 		ClrnSS();
 		bSpiTransfer(addr);
		ptr[0] = bSpiTransfer(0xFF);
		SetnSS();
 		}
 	else
 		{
 		byte i;	
 		ClrnSS();
 		bSpiTransfer(addr);
 		for(i=0; i<length; i++)
 			ptr[i] = bSpiTransfer(0xFF);
 		SetnSS();
 		}
 	}	
 return;
}

