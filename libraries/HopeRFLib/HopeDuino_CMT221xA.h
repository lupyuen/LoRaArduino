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
 * file       HopeDuino_CMT221xA.h
 * brief      show how to use CMT221xA
 * hardware   HopeDuino with RFM21x
 *            
 *
 * version    1.0
 * date       Feb 18 2016
 * author     QY Ruan
 */
 
 #ifndef HopeDuino_CMT221XA_h
	#define HopeDuino_CMT221XA_h

	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif	

	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif	

	#include <avr/io.h>
	#include <util/delay.h>
	
	/** Hardware brief **/    
	//PORTD						//DDRx		PORTx		PINx
	#define	_SCLK	 0x10		// 1          0          0	
	#define	_SDAT    0x20       // 1          1          0
	#define	_DATA    0x40		// 0          0          0
	#define _CSB	 0x80		// 1          1          0
	
	#define	SetCSB()	(PORTD |= _CSB)
	#define	ClrCSB()	(PORTD &= (~_CSB))
	
	#define	SetSCLK()	(PORTD |= _SCLK)
	#define	ClrSCLK()	(PORTD &= (~_SCLK))
	
	#define	SetSDAT()	(PORTD |= _SDAT)
	#define	ClrSDAT()	(PORTD &= (~_SDAT))
	
	#define InputSDAT()	(DDRD &= (~_SDAT))
	#define	OutputSDAT() (DDRD |= (_SDAT))
	
	#define	SDAT_H()	(PIND&_SDAT)
	#define	SDAT_L()	((PIND&_SDAT)==0)

	#define	DATA_H()	(PIND&_DATA)
	#define	DATA_L()	((PIND&_DATA)==0)
	

	enum chipsetType {CMT2210A, CMT2213A, CMT2217A};
	enum decodeType  {E527, E201};
	
	class cmt221xaClass
	{
	 public:	
	 	chipsetType Chipset;						//Chipset part number
	 	decodeType Decode;
		word SymbolTime;							//unit: us  Range: 10 - 4000
	
		void vCMT221xAInit(void);
		byte bReadRssi(void);
		byte bGetMessage(byte msg[], byte pklen);
				
	 private:
	 	void vSpi3Init(void);
	 	void vSpi3WriteByte(byte dat);
	 	byte bSpi3ReadByte(void);
	 	void vSpi3Write(word dat);
	 	byte bSpi3Read(byte addr);		
	 	void vDelayUs(byte delay);					//limit 0~16, unit: us
	 	
	};
#else
	#warning "HopeDuino_CMT221xA.h have been defined!"

#endif
