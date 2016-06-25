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
 * file       HopeHp203b_IIC.cpp
 * brief      for HopeRF's EVB to use config soft IIC port
 * hardware   HopeRF's EVB
 *
 *
 * version    1.1
 * date       mar 09 2016
 * author     mjl
 */

#ifndef HopeHp206c_IIC_h
	#define HopeHp206c_IIC_h

	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif

	#define	HP206C_IIC_SPEED	10			//the bigger the slow

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
	/********************************************************
	--- DDRxn:	in/out (0=in;1=out)
	--- POTTxn:
	--- PINxn:	read pord
	*******************************************************/
	//PORTB						    		//DDRB	PORTB		PINB
	#define	_HP206C_SDA		0x01		// 0		0			1		PB.0
	#define	_HP206C_SCL		0x10		// 0		0			1		PB.4
	
	//PORTD						    		//DDRD	PORTD		PIND
	#define	_HP03S_MCLK		0x40		// 0		0			1		PD.6
	#define	_HP03S_XCLR		0x80		// 0		0			1		PD.7

	#define	_KEY_S1			0x10		// 0		0			1		PD.4
	#define	_KEY_S2			0x20		// 0		0			1		PD.5
	//PORTC						    		//DDRC	PORTC		PINC
	#define	_KEY_S3			0x10		// 0		0			1		PC.4
	#define	_KEY_S4			0x20		// 0		0			1		PC.5
	
	
	#define	SetHp03sXCLR()	(PORTD |= _HP03S_XCLR)
	#define	ClrHp03sXCLR()	(PORTD &= (~_HP03S_XCLR))
	
	#define	SetHp03sMCLK()	(PORTD |= _HP03S_MCLK)
	#define	ClrHp03sMCLK()	(PORTD &= (~_HP03S_MCLK))

	#define	Hp03sMCLK_H()	(PIND&_HP03S_MCLK)
	
	#define	SetHp206cSDA()	(PORTB |= _HP206C_SDA)
	#define	ClrHp206cSDA()	(PORTB &= (~_HP206C_SDA))
	
	#define	SetHp206cSCL()	(PORTB |= _HP206C_SCL)
	#define	ClrHp206cSCL()	(PORTB &= (~_HP206C_SCL))
	
	#define	Hp206cSdaIn()	(DDRB &= (~_HP206C_SDA))
	#define	Hp206cSdaOut()	(DDRB |= _HP206C_SDA)

	#define	HP206C_SDA_H()	(PINB&_HP206C_SDA)
	#define	HP206C_SDA_L()	((PINB&_HP206C_SDA)==0)

	class Hp206cI2cClass
	{
	 public:
	 	void HP206c_vIICInit(void);				/** initialize IIC port **/
	 	bool HP206C_bIICWriteSingle(byte chip, byte cmd);
	 													/** function: IIC write single word 
	 														input:
																chip--------device address
																cmd--------device command   
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
														**/
		byte HP206C_bIICReadSingle(byte chip);
														/** function: IIC read/get single value
															input:
																chip--------device address
																*value--------pointer type
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
	 	bool HP206C_bIICReadSeveral(byte chip, byte value[], byte length);
	 													/** function: IIC read/get some value for continuous mode without register address
	 														input: 
																chip--------device address
																value[]--------pointer type
																length--------length for value[]
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
	 	bool HP206C_bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length);
	 													/** function: IIC burst write some value with register address
	 														input: 
																chip--------device address
																addr--------register address
																ptr[]--------pointer type
																length--------length for ptr[]
	 														output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
		bool HP206C_bIICBurstRead(byte chip, byte addr, byte ptr[], byte length);
			 											/** function: IIC burst read some value with register address
															input: 
																chip--------device address
																addr--------register address
																ptr[]--------pointer type
																length--------length for ptr[]
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
	 private:
	 	void HP206C_vIICStart(void);				/** hp203b IIC_STA **/
	 	void HP206C_vIICStop(void);				/** hp203b IIC_STO **/
		void HP206C_vIICAck(void);					/** hp203b IIC send ack **/						
		void HP206C_vIICNack(void);				/** hp203b IIC send not ack **/
		bool HP206C_bIICWriteByte(byte para);	/** hp203b IIC write 8bit data **/
		byte HP206C_bIICReadByte(bool ack);		/** hp203b IIC read 8bit data **/
	};

#else
	#warning "HopeHp206c_IIC.h have been defined!"

#endif

/**********************************************************
------	Hope hp203b i2c end		--------------------------
**********************************************************/