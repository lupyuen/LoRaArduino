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
 * file       HopeDuino_TWI.cpp
 * brief      for HopeRF's EVB to use config soft TWI port
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.1
 * date       Jan 15 2015
 * author     QY Ruan
 */

 

#ifndef HopeDuino_TWI_h
	#define HopeDuino_TWI_h
	
	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif	

	#define	TWI_SPEED	2			//the bigger the slow

	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif
	
	#ifndef lword
		typedef unsigned long lword;
	#endif

	#include <avr/io.h>
	#include <util/delay.h>

	/** Hardware brief **/

	/** Hardware brief **/    
	//PORTB						//DDRx		PORTx		PINx
	#define	_TCLK    0x10		// 1          1          0
	
	//PORTD
	#define	_TDAT	 0x40		// 1          0 

	#define	SetTCLK()	(PORTB |= _TCLK)
	#define	ClrTCLK()	(PORTB &= (~_TCLK))
	
	#define	InputTDAT() (DDRD &= (~_TDAT))
	#define	OutputTDAT() (DDRD |= _TDAT)
	
	#define	SetTDAT()	(PORTD |= _TDAT)
	#define	ClrTDAT()	(PORTD &= (~_TDAT))
	
	#define TDAT_H()	(PIND&_TDAT)
	#define	TDAT_L()	((PIND&_TDAT)==0)

	class twiClass
	{
	 public:
	 	void vTWIInit(void);							/** initialize TWI port **/
	 	void vTWIWrite(byte adr, byte dat);		
	 	byte bTWIRead(byte adr);
	 	void vTWIReset(void);
	 	
	 private:
	 	void vTWIWriteByte(byte dat);
	 	byte bTWIReadByte(void);
	};

#else
	#warning "HopeDuino_TWI.h have been defined!"

#endif 