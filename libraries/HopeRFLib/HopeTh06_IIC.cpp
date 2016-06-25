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

#include "HopeTh06_IIC.h"

/**********************************************************
**Name: 	Th06_vIICStart
**Func: 	IIC Start Singal
**Note: 	
**********************************************************/
void Th06I2cClass::Th06_vIICStart(void)
{
	SetTh06SDA();
	_delay_us(TH06_IIC_SPEED);
	SetTh06SCL();
	_delay_us(TH06_IIC_SPEED);
	ClrTh06SDA();
	_delay_us(TH06_IIC_SPEED);
	ClrTh06SCL();
}

/**********************************************************
**Name: 	Th06_vIICStop
**Func: 	IIC Stop Singal
**Note: 	
**********************************************************/
void Th06I2cClass::Th06_vIICStop(void)
{
	ClrTh06SCL();
	_delay_us(TH06_IIC_SPEED);
	ClrTh06SDA();
	_delay_us(TH06_IIC_SPEED);
	SetTh06SCL();
	_delay_us(TH06_IIC_SPEED);
	SetTh06SDA();
}

/**********************************************************
**Name: 	Th06_vIICAck
**Func: 	IIC ACK Singal
**Note: 	
**********************************************************/
void Th06I2cClass::Th06_vIICAck(void)
{
	ClrTh06SDA();
	_delay_us(TH06_IIC_SPEED);
	SetTh06SCL();
	_delay_us(TH06_IIC_SPEED);
	ClrTh06SCL();
}

/**********************************************************
**Name: 	Th06_vIICNack
**Func: 	IIC NACK Singal
**Note: 	
**********************************************************/
void Th06I2cClass::Th06_vIICNack(void)	
{
	SetTh06SDA();
	_delay_us(TH06_IIC_SPEED);
	SetTh06SCL();
	_delay_us(TH06_IIC_SPEED);
	ClrTh06SCL();
}

/**********************************************************
**Name: 	Th06_bIICWriteByte
**Func: 	IIC Write one byte
**Output:   true-------ACK
            false------NACK
**********************************************************/
bool Th06I2cClass::Th06_bIICWriteByte(byte para)
{
	byte i;

	for(i=0x80;i!=0;i>>=1)	
	{
		ClrTh06SCL();
		if(para&i)
			SetTh06SDA();
		else
			ClrTh06SDA();
		_delay_us(TH06_IIC_SPEED);
		SetTh06SCL();
		_delay_us(TH06_IIC_SPEED);
	}
	ClrTh06SCL();					//waite for ack or nack
	SetTh06SDA();
	Th06SdaIn();
	_delay_us(TH06_IIC_SPEED);
	_delay_us(TH06_IIC_SPEED);
	SetTh06SCL();					//
	for(i=100;i!=0;i--)		//Timeout check
	{
		_delay_us(TH06_IIC_SPEED);
		if(TH06_SDA_L())				//ack is active
			break;
	}
	ClrTh06SCL();
	Th06SdaOut();
 	_delay_us(TH06_IIC_SPEED);
 	SetTh06SDA();
	if(i==0) 					//Timeout
		return(false);
	else
		return(true);
}

/**********************************************************
**Name: 	Th06_bIICReadByte
**Func: 	IIC Read one byte
**Input:    true-------Send ACK
            false------Send NACK
**********************************************************/
byte Th06I2cClass::Th06_bIICReadByte(bool ack)
{
	byte i;
	byte para = 0;
 
	ClrTh06SCL();
	Th06SdaIn();
	_delay_us(TH06_IIC_SPEED);
	_delay_us(TH06_IIC_SPEED);
	for(i=0x80;i!=0;i>>=1)
 	{
 		SetTh06SCL();
 		_delay_us(TH06_IIC_SPEED);
 		if(TH06_SDA_H())	
 			para |= i;
 		ClrTh06SCL();
 		_delay_us(TH06_IIC_SPEED);
 	}
 	Th06SdaOut();
 	_delay_us(TH06_IIC_SPEED);
	if(ack)
		Th06_vIICAck();
	else
		Th06_vIICNack();
	return(para);
}

/**********************************************************
**Name: 	Th06_vIICInit
**Func: 	Init Port & config
**Note: 	
**********************************************************/
void Th06I2cClass::Th06_vIICInit(void)
{
	DDRB  |= (_TH06_SDA|_TH06_SCL);
	PORTB |= (_TH06_SDA|_TH06_SCL);
}

/**********************************************************
**Name: 	Th06_bIICWriteSingle
**Func: 	IIC wirte command
**input: 	chip---------chip address
			cmd----------command value
**Output:	true---------write success
            false--------write faild
**********************************************************/
bool Th06I2cClass::Th06_bIICWriteSingle(byte chip, byte cmd)
{
	Th06_vIICStart();
	if(Th06_bIICWriteByte(chip))
	{
		if(Th06_bIICWriteByte(cmd))
		{
			Th06_vIICStop();
			return(true);
		}
	}
	Th06_vIICStop();	
	return(false);
}

/**********************************************************
**Name: 	Th06_bIICReadSingle
**Func: 	IIC read single value
**input: 	chip---------chip address
			*value-------read out value
**Output:	true---------read success
            false--------read faild
**********************************************************/
byte Th06I2cClass::Th06_bIICReadSingle(byte chip)
{
	byte value = 0;

	Th06_vIICStart();
	if(Th06_bIICWriteByte(chip|0x01))	//read
	{
		value = Th06_bIICReadByte(false);	
		Th06_vIICStop();
		return(value);
	}
	Th06_vIICStop();
	return(value);
}

/**********************************************************
**Name: 	Th06_bIICReadSeveral
**Func: 	IIC read single value
**input: 	chip---------chip address
			*value-------read out value
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool Th06I2cClass::Th06_bIICReadSeveral(byte chip, byte value[], byte length)
{
	byte i;

	Th06_vIICStart();
	if(Th06_bIICWriteByte(chip|0x01))	//read
	{
		for(i=0;i<(length-1);i++)
			value[i] = Th06_bIICReadByte(true);
		value[i] = Th06_bIICReadByte(false);	
		Th06_vIICStop();
		return(true);
	}
	Th06_vIICStop();	
	return(false);
}


/**********************************************************
**Name: 	Th06_bIICBurstWrite
**Func: 	IIC wirte some byte 
**input: 	chip---------chip address
			addr---------regesisiter address
            ptr----------write buffer pointer    
            length-------buffer length
**Output:	true---------write success
            false--------write faild
**********************************************************/
bool Th06I2cClass::Th06_bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length)
{
	byte i;

	Th06_vIICStart();
	if(Th06_bIICWriteByte(chip))
	{
		if(Th06_bIICWriteByte(addr))
		{
			for(i=0;i<length;i++)
			{
				if(!Th06_bIICWriteByte(ptr[i]))
					break;
			}
			if(i>=length)
			{
				Th06_vIICStop();
				return(true);
			}
		}
	}
	Th06_vIICStop();	
	return(false);
}

/**********************************************************
**Name: 	Th06_bIICBurstRead
**Func: 	IIC read some byte 
**input: 	chip---------chip address
			addr---------regesisiter address
            ptr----------readout buffer pointer    
            length-------readout buffer length
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool Th06I2cClass::Th06_bIICBurstRead(byte chip, byte addr, byte ptr[], byte length)
{
	byte i;

	Th06_vIICStart();
	if(Th06_bIICWriteByte(chip))
	{
		if(Th06_bIICWriteByte(addr))
		{
			Th06_vIICStart();
			if(Th06_bIICWriteByte(chip|0x01))	//chip read address
			{
				for(i=0;i<(length-1);i++)
					ptr[i] = Th06_bIICReadByte(true);
				ptr[i] = Th06_bIICReadByte(false);	
				Th06_vIICStop();
				return(true);
			}
		}
	}
	Th06_vIICStop();
	return(false);
}

/**********************************************************
------	Hope Th06 i2c end		----------------------------
**********************************************************/