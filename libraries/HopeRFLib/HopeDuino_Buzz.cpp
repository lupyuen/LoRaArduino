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
 * file       HopeDuino_Buzz.cpp
 * brief      
 * hardware   HopeRF's EVB & LcdShiled 
 *            
 *
 * version    1.0
 * date       Feb 15 2016
 * author     QY Ruan
 */
 
#include "HopeDuino_Buzz.h"

/**********************************************************
**Name:     vBuzzInit
**Function: Buzzer Init.
**Input:    
**Output:   none
**********************************************************/
void buzzClass::vBuzzInit(void)
{
 DDRD  |= 0x08;		//PD3 for output
 ASSR = 0x00;
 TIMSK2 &= (~OCIE2B);
 TIFR2  &= (~OCF2B);
 OCR2A  = 0x00;
 OCR2B  = 0x00;
 TCCR2A &= 0xC0;
 TCCR2A |= 0x02;	//OC2B output toggle
 TCCR2B = 0x05;
}

/**********************************************************
**Name:     vBuzzPlay
**Function: paly music
**Input:    ptr
**Output:   none
**********************************************************/
void buzzClass::vBuzzPlay(byte ptr[], byte length)
{
 byte i, j;
 
 for(i=0;i<length;i++)	
 	{
 	OCR2A = ptr[i];		//freq
 	TCCR2A |= 0x10;
 	for(j=0;j<ptr[length+i];j++)
 		_delay_ms(8);	
 	}
 
 TCCR2A &= (~0x10);
 PORTD &= (~0x08);
 OCR2A = 0; 		
}