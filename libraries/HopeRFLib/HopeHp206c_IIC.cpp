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
 * version    1.0
 * date       mar 09 2016
 * author     mjl
 */

#include "HopeHp206c_IIC.h"

/**********************************************************
**Name: 	HP203B_vIICStart
**Func: 	IIC Start Singal
**Note: 	
**********************************************************/
void Hp206cI2cClass::HP206C_vIICStart(void)
{
	SetHp206cSDA();
	_delay_us(HP206C_IIC_SPEED);
	SetHp206cSCL();
	_delay_us(HP206C_IIC_SPEED);
	ClrHp206cSDA();
	_delay_us(HP206C_IIC_SPEED);
	ClrHp206cSCL();
}

/**********************************************************
**Name: 	HP203B_vIICStop
**Func: 	IIC Stop Singal
**Note: 	
**********************************************************/
void Hp206cI2cClass::HP206C_vIICStop(void)
{
	ClrHp206cSCL();
	_delay_us(HP206C_IIC_SPEED);
	ClrHp206cSDA();
	_delay_us(HP206C_IIC_SPEED);
	SetHp206cSCL();
	_delay_us(HP206C_IIC_SPEED);
	SetHp206cSDA();
}

/**********************************************************
**Name: 	HP203B_vIICAck
**Func: 	IIC ACK Singal
**Note: 	
**********************************************************/
void Hp206cI2cClass::HP206C_vIICAck(void)
{
	ClrHp206cSDA();
	_delay_us(HP206C_IIC_SPEED);
	SetHp206cSCL();
	_delay_us(HP206C_IIC_SPEED);
	ClrHp206cSCL();
}

/**********************************************************
**Name: 	HP203B_vIICNack
**Func: 	IIC NACK Singal
**Note: 	
**********************************************************/
void Hp206cI2cClass::HP206C_vIICNack(void)	
{
	SetHp206cSDA();
	_delay_us(HP206C_IIC_SPEED);
	SetHp206cSCL();
	_delay_us(HP206C_IIC_SPEED);
	ClrHp206cSCL();
}

/**********************************************************
**Name: 	HP203B_bIICWriteByte
**Func: 	IIC Write one byte
**Output:   true-------ACK
            false------NACK
**********************************************************/
bool Hp206cI2cClass::HP206C_bIICWriteByte(byte para)
{
	byte i;

	for(i=0x80;i!=0;i>>=1)	
	{
		ClrHp206cSCL();
		if(para&i)
			SetHp206cSDA();
		else
			ClrHp206cSDA();
		_delay_us(HP206C_IIC_SPEED);
		SetHp206cSCL();
		_delay_us(HP206C_IIC_SPEED);
	}
	ClrHp206cSCL();					//waite for ack or nack
	SetHp206cSDA();					//shift to input mode
	Hp206cSdaIn();					//input
	_delay_us(HP206C_IIC_SPEED);
	_delay_us(HP206C_IIC_SPEED);
	SetHp206cSCL();					//
	for(i=100;i!=0;i--)		//Timeout check
	{
		_delay_us(HP206C_IIC_SPEED);
		if(HP206C_SDA_L())				//ack is active
			break;
	}
	ClrHp206cSCL();
	Hp206cSdaOut();					//output
	_delay_us(HP206C_IIC_SPEED);
	SetHp206cSDA();
	if(i==0) 					//Timeout
		return(false);
	else
		return(true);
}

/**********************************************************
**Name: 	HP203B_bIICReadByte
**Func: 	IIC Read one byte
**Input:    true-------Send ACK
            false------Send NACK
**********************************************************/
byte Hp206cI2cClass::HP206C_bIICReadByte(bool ack)
{
	byte i;
	byte para = 0;

	ClrHp206cSCL();
	Hp206cSdaIn();			//input
	_delay_us(HP206C_IIC_SPEED);
	_delay_us(HP206C_IIC_SPEED);
	for(i=0x80;i!=0;i>>=1)
 	{
 		SetHp206cSCL();
 		_delay_us(HP206C_IIC_SPEED);
 		if(HP206C_SDA_H())
 			para |= i;
 		ClrHp206cSCL();
 		_delay_us(HP206C_IIC_SPEED);
 	}
	Hp206cSdaOut();					//output
	_delay_us(HP206C_IIC_SPEED);
	if(ack)
		HP206C_vIICAck();
	else
		HP206C_vIICNack();
	return(para);
}

/**********************************************************
**Name: 	HP203B_vIICInit
**Func: 	Init Port & config
**Note: 	--- DDRxn:	in/out (0=in;1=out)
			--- POTTxn:
			--- PINxn:	read pord
**********************************************************/
void Hp206cI2cClass::HP206c_vIICInit(void)
{
	DIDR1 &= (_KEY_S3|_KEY_S4);
	DIDR0 &= ~(_KEY_S3|_KEY_S4);

	DDRB  |= (_HP206C_SDA|_HP206C_SCL);
	PORTB |= (_HP206C_SDA|_HP206C_SCL);

	DDRD |= ((1<<DDD6)|(1<<DDD7));
	ClrHp03sXCLR();
	ClrHp03sMCLK();
	
	DDRD  &= ~(_KEY_S1|_KEY_S2);
	DDRC  &= ~(_KEY_S3|_KEY_S4);
}

/**********************************************************
**Name: 	HP203B_bIICWriteSingle
**Func: 	IIC wirte command
**input: 	chip---------chip address
			cmd----------command value
**Output:	true---------write success
            false--------write faild
**********************************************************/
bool Hp206cI2cClass::HP206C_bIICWriteSingle(byte chip, byte cmd)
{
	HP206C_vIICStart();
	if(HP206C_bIICWriteByte(chip))
	{
		if(HP206C_bIICWriteByte(cmd))
		{
			HP206C_vIICStop();
			return(true);
		}
	}
	HP206C_vIICStop();	
	return(false);
}

/**********************************************************
**Name: 	HP203B_bIICReadSingle
**Func: 	IIC read single value
**input: 	chip---------chip address
			*value-------read out value
**Output:	true---------read success
            false--------read faild
**********************************************************/
byte Hp206cI2cClass::HP206C_bIICReadSingle(byte chip)
{
	byte value = 0;

	HP206C_vIICStart();
	if(HP206C_bIICWriteByte(chip|0x01))	//read
	{
		value = HP206C_bIICReadByte(false);	
		HP206C_vIICStop();
		return(value);
	}
	HP206C_vIICStop();
	return(value);
}

/**********************************************************
**Name: 	HP203B_bIICReadSeveral
**Func: 	IIC read single value
**input: 	chip---------chip address
			*value-------read out value
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool Hp206cI2cClass::HP206C_bIICReadSeveral(byte chip, byte value[], byte length)
{
	byte i;

	HP206C_vIICStart();
	if(HP206C_bIICWriteByte(chip|0x01))	//read
	{
		for(i=0;i<(length-1);i++)
			value[i] = HP206C_bIICReadByte(true);
		value[i] = HP206C_bIICReadByte(false);	
		HP206C_vIICStop();
		return(true);
	}
	HP206C_vIICStop();	
	return(false);
}


/**********************************************************
**Name: 	HP203B_bIICBurstWrite
**Func: 	IIC wirte some byte 
**input: 	chip---------chip address
			addr---------regesisiter address
            ptr----------write buffer pointer    
            length-------buffer length
**Output:	true---------write success
            false--------write faild
**********************************************************/
bool Hp206cI2cClass::HP206C_bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length)
{
	byte i;

	HP206C_vIICStart();
	if(HP206C_bIICWriteByte(chip))
	{
		if(HP206C_bIICWriteByte(addr))
		{
			for(i=0;i<length;i++)
			{
				if(!HP206C_bIICWriteByte(ptr[i]))
					break;
			}
			if(i>=length)
			{
				HP206C_vIICStop();
				return(true);
			}
		}
	}
	HP206C_vIICStop();	
	return(false);
}

/**********************************************************
**Name: 	HP203B_bIICBurstRead
**Func: 	IIC read some byte 
**input: 	chip---------chip address
			addr---------regesisiter address
            ptr----------readout buffer pointer    
            length-------readout buffer length
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool Hp206cI2cClass::HP206C_bIICBurstRead(byte chip, byte addr, byte ptr[], byte length)
{
	byte i;

	HP206C_vIICStart();
	if(HP206C_bIICWriteByte(chip))
	{
		if(HP206C_bIICWriteByte(addr))
		{
			HP206C_vIICStart();
			if(HP206C_bIICWriteByte(chip|0x01))	//chip read address
			{
				for(i=0;i<(length-1);i++)
					ptr[i] = HP206C_bIICReadByte(true);
				ptr[i] = HP206C_bIICReadByte(false);	
				HP206C_vIICStop();
				return(true);
			}
		}
	}
	HP206C_vIICStop();
	return(false);
}

/**********************************************************
------	Hope hp203b i2c end		--------------------------
**********************************************************/