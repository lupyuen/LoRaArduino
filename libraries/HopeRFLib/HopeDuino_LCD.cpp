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
 * file       HopeDuino_LCD.cpp
 * brief      HAL for JLX12864
 * hardware   HopeRF's EVB & LcdShiled 
 *            
 *
 * version    1.1
 * date       Jan 15 2016
 * author     QY Ruan
 */
 
#include "HopeDuino_LCD.h"

/**********************************************************
**Name:     vSendCmd
**Function: send command to JLX12864
**Input:    dat for command value (8bits)
**Output:   none
**********************************************************/
void lcdClass::vSendCmd(byte dat) 
{ 
 	byte i;	
 	
 	OutputLSDIO();
 	SetLSCK();
 	
 	SetRCS();
 	ClrLCS();
 	ClrLCD();
 	
 	for(i=0; i<8; i++)
 		{
 		ClrLSCK();
 		if(dat&0x80)
 			SetLSDIO();
 		else
 			ClrLSDIO();
 		SetLSCK();
 		dat <<= 1;
 		}
 	
 	SetLSCK();
 	SetLCD(); 
 	SetLCS();
 	SetLSDIO();
} 

/**********************************************************
**Name:     vSendDBCmd
**Function: send double byte command to JLX12864
**Input:    cmd & value == 16bits command
**Output:   none
**********************************************************/
void lcdClass::vSendDBCmd(byte cmd,byte value) 
{
	byte i;	
	word dat;
	
	dat = (((word)cmd<<8)|value);
 	
 	OutputLSDIO();
 	SetLSCK();
 	
 	SetRCS();
 	ClrLCS();
 	ClrLCD();
 	
 	for(i=0; i<16; i++)
 		{
 		ClrLSCK();
 		if(dat&0x8000)
 			SetLSDIO();
 		else
 			ClrLSDIO();
 		SetLSCK();
 		dat <<= 1;
 		}
 	
 	SetLSCK();
 	SetLCD(); 
 	SetLCS();
 	SetLSDIO();
}

/**********************************************************
**Name:     vSendData
**Function: send data to JLX12864
**Input:    dat
**Output:   none
**********************************************************/
void lcdClass::vSendData(byte dat)
{
 	byte i;
 	OutputLSDIO();
 	SetLSCK();
	
	SetRCS();
 	SetLCD();
 	ClrLCS();
	for(i=0; i<8; i++)
 		{
 		ClrLSCK();
 		if(dat&0x80)
 			SetLSDIO();
 		else
 			ClrLSDIO();
 		SetLSCK();
 		dat <<= 1;
 		}
	SetLSCK();
	SetLSDIO();
}	

/**********************************************************
**Name:     vBurstSendData
**Function: continuous send data to JLX12864
**Input:    ptr & length
**Output:   none
**********************************************************/ 
void lcdClass::vBurstSendData(byte ptr[], byte length)
{
	byte i, j;	
 	byte dat;
 	
 	if(length!=0)
 		{
 		OutputLSDIO();
 		SetLSCK();
 		
 		SetRCS();
 		SetLCD();
 		ClrLCS();
		
		for(j=0; j<length; j++)
			{
			dat = ptr[j];
 			for(i=0; i<8; i++)
 				{
 				ClrLSCK();
 				if(dat&0x80)
 					SetLSDIO();
 				else
 					ClrLSDIO();
 				SetLSCK();
 				dat <<= 1;
 				}
 			}	
 		SetLSCK();
 		SetLCS();			//not need, for burst write mode
 		SetLSDIO();
		}
}

/**********************************************************
**Name:     vLcdLight
**Function: turn on led for LED background
**Input:    on or off
**Output:   none
**********************************************************/ 
void lcdClass::vLcdLight(bool on)
{
 if(on)
 	SetLight();
 else
 	ClrLight();
}


/**********************************************************
**Name:     vLcdInit
**Function: Init. for JLX12864
**Input:    none
**Output:   none
**********************************************************/ 
void lcdClass::vLcdInit(void) 
{ 	
	
	ADCSRA = 0x00;
	ADCSRB = 0x00;
	DIDR0  = 0x00;		//disable ADC Port
	
	DDRC  |= (_LSCK|_LCS|_RCS);
	DDRC  &= (~_RSDO);
	PORTC |= (_LSCK|_LCS|_RCS);
	
	DDRD  |= _LIGHT;
	PORTD |= _LIGHT;
	
	DDRE  |= (_LSDIO|_LCD|_LRST);
	PORTE |= (_LSDIO|_LCD|_LRST);
	
 	ClrLRST();			//Hardware Reset
 	_delay_us(10);
 	SetLRST();
 	_delay_ms(10);
 	vSendCmd(LcdSoftReset);	//SoftReset
 	_delay_ms(5);
 	vSendCmd(0x2C);		//Set Power Control 3step or only set 0x2F
 	_delay_us(10);
 	vSendCmd(0x2E);
 	_delay_us(10);
 	vSendCmd(0x2F);
 	_delay_us(10);
 	vSendCmd(0x23);		//Set Vlcd Resistor Ratio, range for 0x20~0x27
 	vSendDBCmd(0x81, 40);	//Set Electronic volume, range for 0x00~0x3F
 	vSendCmd(0xA2);		//1/9 bias
 	vSendCmd(0xC8);		//Set COM Direction, from top to button
 	vSendCmd(0xA0);		//Set SEG Direction, from left to right
 	vSendCmd(0x40);		//Set Scroll Line, for No.1 Line
 	vLcdEnable();		//Set Display enable, for 0xAE Disable
}	

/**********************************************************
**Name:     vLcdEnable
**Function: turn on LCD
**Input:    none
**Output:   none
**********************************************************/  
void lcdClass::vLcdEnable(void) 
{
 	vSendCmd(0xAF);
}

/**********************************************************
**Name:     vLcdDisable
**Function: turn off LCD
**Input:    none
**Output:   none
**********************************************************/   
void lcdClass::vLcdDisable(void)  
{
	vSendCmd(0xAE);
}

/**********************************************************
**Name:     vLcdLocate
**Function: locate address for picture array
**Input:    page, column
**Output:   none
**********************************************************/   
void lcdClass::vLcdLocate(byte page, byte column)
{
	page -= 1;
	column -= 1;				//begin from 0
 	vSendCmd(0xB0+page);		//full screen have 64lines, 8 pages, every page have 8lines
 	vSendCmd(((column>>4)&0x0F)|0x10);	//set column address for MSB
 	vSendCmd(column&0x0f);				//set column address for LSB
}

/**********************************************************
**Name:     vLcdClearScreen
**Function: clear screen
**Input:    none
**Output:   none
**********************************************************/   	
void lcdClass::vLcdClearScreen(void)
{
 	byte i, j;
 	for(i=1;i<10;i++)	
 		{
 		vLcdLocate(i, 1);
 		for(j=0;j<132;j++)
 			vSendData(0);
 		SetLCS();	
 		}
}

/**********************************************************
**Name:     vLcdFullDisplay
**Function: show full display
**Input:    none
**Output:   none
**********************************************************/ 
void lcdClass::vLcdFullDisplay(void)
{
	byte i, j;
	for(i=1;i<9;i++)
		{
		vLcdLocate(i, 1);
		for(j=0;j<128;j++)
 			vSendData(0xFF);
 		SetLCS();	
		}
}

/**********************************************************
**Name:     vLcdLibSend
**Function: send command to lib
**Input:    dat
**Output:   none
**********************************************************/ 
void lcdClass::vLcdLibSend(byte dat)
{
 	byte i;
 	for(i=0; i<8; i++)
 		{
 		ClrLSCK();
 		if(dat&0x80)
 			SetLSDIO();
 		else
 			ClrLSDIO();
 		SetLSCK();
 		dat <<= 1;
 		}
}


/**********************************************************
**Name:     bLcdLibRead
**Function: read data from lib
**Input:    none
**Output:   data
**********************************************************/
byte lcdClass::bLcdLibRead(void) 
{
	byte i, dat;
	SetLSDIO();
	for(i=0; i<8; i++)
		{
		ClrLSCK();		
		dat <<= 1;	
		SetLSCK();			
		if(RSDO_H())
			dat |= 0x01;
		else
			dat |= 0x00;
		}
	return(dat);
}


/**********************************************************
**Name:     vGetASCII8x8
**Function: read 8*8 ASCII array 
**Input:    none
**Output:   ptr
**********************************************************/
void lcdClass::vGetASCII8x8(byte ascii_code, byte ptr[])				/*读取一个ASCII的数组，7*8格式*/
{
	word addr;
	byte i;	
	if((ascii_code>=0x20)&&(ascii_code<0x7F))
		{
		addr = ((word)(ascii_code-0x20)<<3)+0x66C0;
		
  		OutputLSDIO();
 		SetLSCK();
 		SetLCS();
 		SetLCD(); 	
 		ClrRCS();
 		
 		vLcdLibSend(0x0B);		//Fast Read
 		vLcdLibSend(0x00);
 		vLcdLibSend((byte)(addr>>8));
 		vLcdLibSend((byte)addr);
 		vLcdLibSend(0xAA);
 		
 		for(i=0; i<8; i++)
 			ptr[i] = bLcdLibRead();
 		
 		SetLSCK();
		SetRCS();
 		SetLSDIO();
		}	
}

























