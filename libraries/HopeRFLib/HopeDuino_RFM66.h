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
 * file       HopeDuino_RFM66.h
 * brief      driver for RFM66 TRx
 * hardware   HopeDuino with HopeRF RFM66 COB rf-module  
 *            
 *
 * version    1.0
 * date       Jun 3 2014
 * author     QY Ruan
 */

#ifndef HopeDuino_RFM66_h
	#define HopeDuino_RFM66_h
	
	#ifndef F_CPU
		#define F_CPU 16000000UL		//for delay.h to use, & RFduino use 16MHz Xo
	#endif	

	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif
	
	#ifndef lword
		typedef unsigned long lword;
	#endif

	#ifndef __DELAY_BACKWARD_COMPATIBLE__
		#define __DELAY_BACKWARD_COMPATIBLE__
	#endif 

	#include <avr/io.h>
	#include <util/delay.h>
	#include "HopeDuino_SPI.h"
	
	/** Hardware brief **/
	/** POR & DIO0 is must be connect **/
	//PORTB						//DDRx		PORTx		PINx
	#define DIO0	0x01		// 0		  0          1
	#define	POR		0x02		// 0          0          1

	//PORTD
	/** DIO1~DIO4 can be select to connect **/
	#define	DIO4	0x10		// 0          0          1
	#define	DIO3	0x20		// 0          0          1
	#define	DIO2    0x40		// 0          0          1
	#define	DIO1    0x80		// 0          0          1
	
	//Output
	#define POROut()	(DDRB |= POR)
	#define	PORIn()		(DDRB &= (~POR))
	#define SetPOR()	(PORTB |= POR)
	#define	ClrPOR()	(PORTB &= (~POR))	
	
	//Input
	#define DIO0In()	(DDRD &= (~DIO0))
	#define	DIO0_H()	((PINB&DIO0)==DIO0)
	#define	DIO0_L()	((PINB&DIO0)==0)
	
	typedef union  
	{
	 struct
	 	{
	  	byte FreqL: 8;
	  	byte FreqM: 8;
	  	byte FreqH: 8;
	  	byte FreqX: 8;
	 	}freq;
	 unsigned long Freq;
	}FreqStruct;

	enum modulationType {OOK, FSK, GFSK};
	
	class rf66Class
	{
	 public:
	 	modulationType Modulation;					//OOK/FSK/GFSK
	 	lword Frequency;							//unit: KHz
 		lword SymbolTime;							//unit: ns
 		lword Devation;								//unit: KHz
 		word  BandWidth;							//unit: KHz
 		byte  SyncWord[8];
 		byte  SyncLength;							//unit: none, range: 1-8[Byte], value '0' is not allowed
 		byte  OutputPower;							//unit: dBm   range: 2-20[2dBm-20dBm]
 		word  PreambleLength;						//unit: byte
 		bool  FixedPktLength;						//false: for contain packet length in Tx message, the same mean with variable lenth
 		                                            //true : for doesn't include packet length in Tx message, the same mean with fixed length
 		byte  PayloadLength;						//PayloadLength is need to be set a value, when FixedPktLength is true.
 		bool  CrcDisable;							//fasle: CRC enable£¬ & use CCITT 16bit 
 													//true : CRC disable
 		
		void vInitialize(void);
		void vConfig(void);
		void vGoRx(void);
		void vGoStandby(void);
		void vGoSleep(void);
		bool bSendMessage(byte msg[], byte length);
		byte bGetMessage(byte msg[]);
		
	 private:
	 	spiClass Spi;
	 	FreqStruct FrequencyValue;
	 	word BitRateValue;
		word DevationValue;
		byte BandWidthValue;

		void vRF66Reset(void);
		byte bSelectBandwidth(byte rx_bw);
		byte bSelectRamping(lword symbol);
	};
#else
	#warning "HopeDuino_RFM66.h have been defined!"

#endif