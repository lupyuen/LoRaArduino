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
 * file       HopeDuino_LCD.h
 * brief      Driver for JLX12864 
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.1
 * date       Jan 15 2016
 * author     QY Ruan
 */


#ifndef HopeDuino_LCD_h
	#define HopeDuino_LCD_h
	
	#ifndef F_CPU
		#define F_CPU 16000000UL	
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

	#include <Arduino.h>
	#include <util/delay.h>

	
	/** Hardware brief **/
	//PORTC						//DDRx		PORTx		PINx
	#define _RSDO   0x01        // 0         0           0			//ROM_SDO
	#define _RCS	0x02        // 1 		 1			 0			//ROM_CS
	#define _LSCK	0x04        // 1         1           0   		//LCD_SCK & ROM_SCK	
	#define	_LCS	0x08		// 1         1           0          //LCD_CS 	
	
	//PORTD
	#define	_LIGHT	0x04		// 1         1           0   		//LCD_LIGHT
	
	//PORTE						//DDRx      PORTx       PINx
	#define	_LCD	0x02		// 1         1           0          //LCD_RS  ¼Ä´æÆ÷	
	#define	_LSDIO	0x08		// 1(0)	     1  		 0			//LCD_SDA & ROM_SDI	
	#define	_LRST	0x40		// 1         1           0			//LCD_Reset
	

	#define	SetLight()	(PORTD &= (~_LIGHT))
	#define	ClrLight()	(PORTD |= _LIGHT)
	
	#define	SetLCS()	(PORTC |= _LCS)
	#define	ClrLCS()	(PORTC &= (~_LCS))
	
	#define	SetLSCK()	(PORTC |= _LSCK)
	#define ClrLSCK()	(PORTC &= (~_LSCK))
	
	#define	SetRCS()	(PORTC |= _RCS)
	#define	ClrRCS()	(PORTC &= (~_RCS))
	
	#define	SetLCD()	(PORTE |= _LCD)
	#define	ClrLCD()	(PORTE &= (~_LCD))
	
	#define	SetLRST()	(PORTE |= _LRST)
	#define	ClrLRST()	(PORTE &= (~_LRST))
	
	#define OutputLSDIO()	(DDRE |= _LSDIO)	
	#define InputLSDIO()	(DDRE &= (~_LSDIO))
	
	#define	SetLSDIO()	(PORTE |= _LSDIO)
	#define	ClrLSDIO()	(PORTE &= (~_LSDIO))
	
	#define	LSDIO_H()	(PINE&_LSDIO)
	#define	LSDIO_L()	((PINE&_LSDIO)==0)
	
	#define	RSDO_H()	(PINC&_RSDO)
	#define	RSDO_L()	((PINC&_RSDO)==0)	
	
	//Command Value for JLX12864
	#define	LcdSoftReset 0xE2
	
	class lcdClass
	{
		
	 public:
	 	void vLcdInit(void);							//LCD Init.
	 	void vLcdEnable(void);							//Turn On LCD
	 	void vLcdDisable(void);							//Trun Off LCD
	 	void vLcdLight(bool on);						//blackground led
		void vLcdLocate(byte page, byte column);		//Locate start address
		void vLcdClearScreen(void);						//Clear Screen
		void vLcdFullDisplay(void);						//Test for full display
	
		void vLcdLibSend(byte dat);						//Send Command to Lib.
		byte bLcdLibRead(void);							//Read data from Lib.
		void vGetASCII8x8(byte ascii_code, byte ptr[]);	//Get ASCII 8*8 array 
		
		void vSendData(byte dat);						//Send data to LCD
		void vSendCmd(byte dat);						//Send command to LCD
	 	void vSendDBCmd(byte cmd,byte value);			//Send double-command to LCD
	 	void vBurstSendData(byte ptr[], byte length);	//Continuous send data to LCD
	};
#else
	#warning "RFduino_LCD.h have been defined!"

#endif