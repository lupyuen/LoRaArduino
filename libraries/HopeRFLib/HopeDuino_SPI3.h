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


#ifndef HopeDuino_SPI3_h
	#define HopeDuino_SPI3_h
	
	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif	

	#define	SPI3_SPEED	2			//the bigger the slow
	
	#include <avr/io.h>
	#include <util/delay.h>
	
	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif

	/** Hardware brief **/    
	//PORTB						//DDRx		PORTx		PINx
	#define _CSB	 0x04		// 1          1          0
	#define	_SDIO    0x08       // 1          1          0
	#define	_FCSB    0x10		// 1          1          0
	#define	_SDCK	 0x20		// 1          0          0

	#define	SetCSB()	(PORTB |= _CSB)
	#define	ClrCSB()	(PORTB &= (~_CSB))
	
	#define	SetFCSB()	(PORTB |= _FCSB)
	#define	ClrFCSB()	(PORTB &= (~_FCSB))
	
	#define	SetSDCK()	(PORTB |= _SDCK)
	#define	ClrSDCK()	(PORTB &= (~_SDCK))
	
	#define	SetSDIO()	(PORTB |= _SDIO)
	#define	ClrSDIO()	(PORTB &= (~_SDIO))
	
	#define InputSDIO()	(DDRB &= (~_SDIO))
	#define	OutputSDIO() (DDRB |= (_SDIO))
	
	#define	SDIO_H()	(PINB&_SDIO)
	#define	SDIO_L()	((PINB&_SDIO)==0)
	
	

	class spi3Class
	{
	 public:
		void vSpi3Init(void);				/** initialize software SPI-3 **/	
	 	void vSpi3Write(word dat);			/** SPI-3 send one word **/
		byte bSpi3Read(byte addr);			/** SPI-3 read one byte **/
		
		void vSpi3WriteFIFO(byte dat);		/** SPI-3 send one byte to FIFO **/
		byte bSpi3ReadFIFO(void);			/** SPI-3 read one byte from FIFO **/
		void vSpi3BurstWriteFIFO(byte ptr[], byte length);			/** SPI-3 burst send N byte to FIFO**/
		void vSpi3BurstReadFIFO(byte ptr[], byte length);			/** SPI-3 burst read N byte to FIFO**/
	 private:
	    void vSpi3WriteByte(byte dat);		/** SPI-3 send one byte **/
	    byte bSpi3ReadByte(void);			/** SPI-3 read one byte **/

	};

#else
	#warning "HopeDuino_SPI3.h have been defined!"

#endif