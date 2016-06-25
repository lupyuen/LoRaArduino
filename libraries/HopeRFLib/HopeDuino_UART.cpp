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
 * file       HopeDuino_UART.cpp
 * brief      for HopeRF's EVB to use Hardware UART
 * hardware   HopeRF's EVB  
 *            
 *
 * version    1.1
 * date       Jan 15 2015
 * author     QY Ruan
 */
#include "HopeDuino_UART.h"

byte HeadPtr;
byte TailPtr;
byte RxBuf[RX_BUF_LENGTH]; 	

/**********************************************************
**Name: 	vUartInit
**Func: 	Init Uart Config
**Note: 	
**********************************************************/
void uartClass::vUartInit(unsigned long BuardRate, UartFormat uFormat)
{
 
 UCSR0A = _BV(U2X0); 								//Double speed mode USART0
 UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);	//Enable Uart tx & rx, & enable rx_interrupt
 switch(uFormat)
 	{
 	case _8O2: UCSR0C = _BV(UCSZ00) | _BV(UCSZ01) | _BV(USBS0) | _BV(UPM01) | _BV(UPM00); break;
 	case _8E2: UCSR0C = _BV(UCSZ00) | _BV(UCSZ01) | _BV(USBS0) | _BV(UPM01); break;
 	case _8N2: UCSR0C = _BV(UCSZ00) | _BV(UCSZ01) | _BV(USBS0);  break;
 	case _8O1: UCSR0C = _BV(UCSZ00) | _BV(UCSZ01) | _BV(UPM01) | _BV(UPM00); break;
 	case _8E1: UCSR0C = _BV(UCSZ00) | _BV(UCSZ01) | _BV(UPM01); break;
 	case _8N1:
 	default:   UCSR0C = _BV(UCSZ00) | _BV(UCSZ01); break;
 	}
 UBRR0L = (uint8_t)( (F_CPU + BuardRate * 4L) / (BuardRate * 8L) - 1 );
 HeadPtr = 0;
 TailPtr = HeadPtr;
}

/**********************************************************
**Name: 	vUartPutByte
**Func: 	put one char
**Note: 	
**********************************************************/
void uartClass::vUartPutByte(byte ch)
{
 while (!(UCSR0A & _BV(UDRE0)));
 UDR0 = ch;
}

/**********************************************************
**Name: 	vUartPutNByte
**Func: 	put N char
**Note: 	
**********************************************************/
void uartClass::vUartPutNByte(byte ptr[], byte length)
{
 byte i;
 for(i=0;i<length;i++)
 	vUartPutByte(ptr[i]);
}

/**********************************************************
**Name: 	vUartPutString
**Func: 	put string
**Note: 	
**********************************************************/
void uartClass::vUartPutString(char str[])
{
 byte i = 0;
 while(str[i]!='\0')
 	vUartPutByte(str[i++]);
}

/**********************************************************
**Name: 	vUartNewLine
**Func: 	new line
**Note: 	
**********************************************************/
void uartClass::vUartNewLine(void)
{
 vUartPutByte(0x0D);
 vUartPutByte(0x0A);
}


/**********************************************************
**Name: 	vHandleRxInt
**Func: 	handle Uart rx complete 
**Note: 	
**********************************************************/
inline void uartClass::vHandleRxInt(void)
{
 RxBuf[HeadPtr] = UDR0;
 HeadPtr++;
 if(HeadPtr>=RX_BUF_LENGTH)
 	HeadPtr = 0;
}

ISR(USART_RX_vect)
{
 uartClass::vHandleRxInt();
}

/**********************************************************
**Name: 	bUartGetByte
**Func: 	get one char
**Note: 	
**********************************************************/
bool uartClass::bUartGetByte(byte *ptr)
{
 if(TailPtr!=HeadPtr)
 	{
 	(*ptr) = RxBuf[TailPtr];
 	TailPtr++;
 	if(TailPtr>=RX_BUF_LENGTH)
 		TailPtr = 0;
 	}
 else
 	return(false);
}

/**********************************************************
**Name: 	bUartGetNByte
**Func: 	get N char
**Note: 	
**********************************************************/
byte uartClass::bUartGetNByte(byte ptr[], byte length)
{
 byte i;
 if(TailPtr!=HeadPtr)
 	{
 	for(i=0;i<length;i++)
 		{
 		ptr[i] = RxBuf[TailPtr];
 		TailPtr++;
 		if(TailPtr>=RX_BUF_LENGTH)
 			TailPtr = 0;
 		if(TailPtr==HeadPtr)
 			break;
 		}
 	if(TailPtr==HeadPtr)
 		return(i);
 	else
 		return(length);
 	}
  else
 	return(0);
}
		