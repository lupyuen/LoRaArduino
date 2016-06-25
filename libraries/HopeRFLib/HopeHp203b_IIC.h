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

#ifndef HopeHp203b_IIC_h
	#define HopeHp203b_IIC_h

	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif

	#define	HP203B_IIC_SPEED	10			//the bigger the slow

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
	//PORTD						    		//DDRB	PORTB		PINB
	#define	_HP203B_SDA		0x04		// 0		0			1		PB.2
	#define	_HP203B_SCL		0x02		// 0		0			1		PB.1
	
	#define	SetHp203bSDA()	(PORTB |= _HP203B_SDA)
	#define	ClrHp203bSDA()	(PORTB &= (~_HP203B_SDA))
	
	#define	SetHp203bSCL()	(PORTB |= _HP203B_SCL)
	#define	ClrHp203bSCL()	(PORTB &= (~_HP203B_SCL))
	
	#define	Hp203bSdaIn()	(DDRB &= (~_HP203B_SDA))
	#define	Hp203bSdaOut()	(DDRB |= _HP203B_SDA)
	
	#define	HP203B_SDA_H()	(PINB&_HP203B_SDA)
	#define	HP203B_SDA_L()	((PINB&_HP203B_SDA)==0)

	class Hp203bI2cClass
	{
	 public:
	 	void HP203B_vIICInit(void);				/** initialize IIC port **/
	 	bool HP203B_bIICWriteSingle(byte chip, byte cmd);
	 													/** function: IIC write single word 
	 														input:
																chip--------device address
																cmd--------device command   
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
														**/
		byte HP203B_bIICReadSingle(byte chip);
														/** function: IIC read/get single value
															input:
																chip--------device address
																*value--------pointer type
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
	 	bool HP203B_bIICReadSeveral(byte chip, byte value[], byte length);
	 													/** function: IIC read/get some value for continuous mode without register address
	 														input: 
																chip--------device address
																value[]--------pointer type
																length--------length for value[]
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
	 	bool HP203B_bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length);
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
		bool HP203B_bIICBurstRead(byte chip, byte addr, byte ptr[], byte length);	
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
	 	void HP203B_vIICStart(void);				/** hp203b IIC_STA **/
	 	void HP203B_vIICStop(void);				/** hp203b IIC_STO **/
		void HP203B_vIICAck(void);					/** hp203b IIC send ack **/						
		void HP203B_vIICNack(void);				/** hp203b IIC send not ack **/
		bool HP203B_bIICWriteByte(byte para);	/** hp203b IIC write 8bit data **/
		byte HP203B_bIICReadByte(bool ack);		/** hp203b IIC read 8bit data **/
	};

#else
	#warning "HopeHp203b_IIC.h have been defined!"

#endif

/**********************************************************
------	Hope hp203b i2c end		--------------------------
**********************************************************/