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
 * file       HopeTh06_IIC.cpp
 * brief      for HopeRF's EVB to use config soft IIC port
 * hardware   HopeRF's EVB
 *
 *
 * version    1.1
 * date       mar 09 2016
 * author     mjl
 */

#ifndef HopeTh06_IIC_h
	#define HopeTh06_IIC_h

	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif

	#define	TH06_IIC_SPEED	10			//the bigger the slow

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
	#define	_TH06_SDA		0x08		// 0		0			1		PB.3
	#define	_TH06_SCL		0x20		// 0		0			1		PB.5
	
	#define	SetTh06SDA()	(PORTB |= _TH06_SDA)
	#define	ClrTh06SDA()	(PORTB &= (~_TH06_SDA))
	
	#define	SetTh06SCL()	(PORTB |= _TH06_SCL)
	#define	ClrTh06SCL()	(PORTB &= (~_TH06_SCL))
	
	#define	Th06SdaIn()		(DDRB &= (~_TH06_SDA))
	#define	Th06SdaOut()	(DDRB |= _TH06_SDA)
	
	#define	TH06_SDA_H()	(PINB&_TH06_SDA)
	#define	TH06_SDA_L()	((PINB&_TH06_SDA)==0)


	class Th06I2cClass
	{
	 public:
	 	void Th06_vIICInit(void);				/** initialize IIC port **/
	 	bool Th06_bIICWriteSingle(byte chip, byte cmd);
	 													/** function: IIC write single word 
	 														input:
																chip--------device address
																cmd--------device command   
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
														**/
	 	byte Th06_bIICReadSingle(byte chip);
														/** function: IIC read/get single value
															input:
																chip--------device address
																*value--------pointer type
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
	 	bool Th06_bIICReadSeveral(byte chip, byte value[], byte length);
	 													/** function: IIC read/get some value for continuous mode without register address
	 														input: 
																chip--------device address
																value[]--------pointer type
																length--------length for value[]
															output:
																true--------success
																false--------no ack / device doesn't exist / other error
	 													**/
	 	bool Th06_bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length);
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
		bool Th06_bIICBurstRead(byte chip, byte addr, byte ptr[], byte length);	
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
	 	void Th06_vIICStart(void);				/** th06 IIC_STA **/
	 	void Th06_vIICStop(void);				/** th06 IIC_STO **/
		void Th06_vIICAck(void);				/** th06 IIC send ack **/						
		void Th06_vIICNack(void);				/** th06 IIC send not ack **/
		bool Th06_bIICWriteByte(byte para);	/** th06 IIC write 8bit data **/
		byte Th06_bIICReadByte(bool ack);	/** th06 IIC read 8bit data **/
	};

#else
	#warning "HopeTh06_IIC.h have been defined!"

#endif 