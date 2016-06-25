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


#ifndef HopeDuino_UART_h
	#define HopeDuino_UART_h
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/pgmspace.h>

	#ifndef F_CPU
		#define F_CPU 16000000UL	//RFduino use 16MHz Xo
	#endif	

	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif
	
	#ifndef RX_BUF_LENGTH	
		#define	RX_BUF_LENGTH 64
	#else
		#if(RX_BUF_LENGTH>=256)
			#error "RX_BUF_LENGTH must be less than 256!"
		#endif
	#endif
	
	enum UartFormat {_8N1, _8E1, _8O1, _8N2, _8E2, _8O2};

	extern byte HeadPtr;						//RxBuf head pointer
	extern byte TailPtr;						//RxBuf tail pointer
	extern byte RxBuf[RX_BUF_LENGTH]; 			//Uart rx buffer
	
	class uartClass
	{
	 	public:
		void vUartInit(unsigned long BuardRate, UartFormat uFormat);	/** initialize hardware UARt config 
		                                                                    BuardRate: Uart buard rate, example 9600
		                                                                    uFormat:   Uart format, example _8N1
		                                                                               '8'-------8bit (this version do not support 5/6/7)
		                                                                               'N'-------no check 
		                                                                               'E'-------even check
		                                                                               'O'-------odd check
																					   '1'-------1 stop bit
																					   '2'-------2 stop bit
		                                                                **/
		void vUartPutNByte(byte ptr[], byte length);					/** print ptr[length] to uart **/	
		void vUartPutString(char str[]);								/** print string to uart **/
		byte bUartGetNByte(byte ptr[], byte length);					/** get ptr[length] from uart 
		                                                                	return: 
		                                                                		0----------does not have message
		                                                                	< length-------get message, but does not enough length
		                                                                	= length-------get message success
		                                                                	note: when call this function, buffer(which be read out) will be clear, even return less than length
		                                                                **/
		void vUartPutByte(byte ch);										/** print char to uart, just one byte **/
		void vUartNewLine(void);										/** start new line **/
		bool bUartGetByte(byte *ptr);	 								/** get char to *ptr from uart, just one byte
		                                                                    return:
		                                                                       ture--------get success;
		                                                                       false-------get faild, means does not have message
																		**/
	 	static inline void vHandleRxInt(void);							/** handle uart receive complete interrupt **/
	};
	
#else
	#warning "HopeDuino_UART.h have been defined!"

#endif
