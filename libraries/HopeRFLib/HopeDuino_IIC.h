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
 * file       HopeDuino_IIC.cpp
 * brief      for HopeRF's EVB to use config soft IIC port
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.1
 * date       Jan 15 2015
 * author     QY Ruan
 */

 

#ifndef HopeDuino_IIC_h
	#define HopeDuino_IIC_h
	
	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif	

	#define	IIC_SPEED	1			//the bigger the slow

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
	//PORTC						    //DDRC		PORTC		PINC
	#define	_SDA		0x10		// 0          0          1
	#define	_SCL		0x20		// 0          0          1	
	
	#define	SetSDA()	(DDRC &= (~_SDA))
	#define	ClrSDA()	(DDRC |= _SDA)
	
	#define	SetSCL()	(DDRC &= (~_SCL))
	#define	ClrSCL()	(DDRC |= _SCL)
	
	#define	SDA_H()		(PINC&_SDA)
	#define	SDA_L()		((PINC&_SDA)==0)	

	class iicClass
	{
	 public:
	 	void vIICInit(void);							/** initialize IIC port **/
	 	bool bIICWriteSingle(byte chip, byte cmd);		/** function: IIC write single word 
	 														input:
		 	                                                    chip--------device address
		 	                                                     cmd--------device command   
		 	                                                output:
		 	                                                    true--------success
		 	                                                   false--------no ack / device doesn't exist / other error
	 	                                                **/
	 	bool bIICReadSingle(byte chip, byte *value);	/** function: IIC read/get single value
	 	                                                    input:
	 	                                                        chip--------device address
	 														  *value--------pointer type
	 														output:
	 															true--------success
		 	                                                   false--------no ack / device doesn't exist / other error
	 													**/
	 	bool bIICReadSeveral(byte chip, byte value[], byte length);	
	 													/** function: IIC read/get some value for continuous mode without register address
	 													    input: 
	 													        chip--------device address
	 														 value[]--------pointer type
	 														  length--------length for value[]
	 														output:
	 															true--------success
		 	                                                   false--------no ack / device doesn't exist / other error
	 													**/
	 	bool bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length);
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
		bool bIICBurstRead(byte chip, byte addr, byte ptr[], byte length);	
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
	 	void vIICStart(void);							/** IIC_STA **/
	 	void vIICStop(void);							/** IIC_STO **/
		void vIICAck(void);								/** IIC send ack **/						
		void vIICNack(void);							/** IIC send not ack **/
		bool bIICWriteByte(byte para);					/** IIC write 8bit data **/
		byte bIICReadByte(bool ack);					/** IIC read 8bit data **/
	};

#else
	#warning "HopeDuino_IIC.h have been defined!"

#endif 