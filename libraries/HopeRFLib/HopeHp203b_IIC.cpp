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

#include "HopeHp203b_IIC.h"

/**********************************************************
**Name: 	HP203B_vIICStart
**Func: 	IIC Start Singal
**Note: 	
**********************************************************/
void Hp203bI2cClass::HP203B_vIICStart(void)
{
	SetHp203bSDA();
	_delay_us(HP203B_IIC_SPEED);
	SetHp203bSCL();
	_delay_us(HP203B_IIC_SPEED);
	ClrHp203bSDA();
	_delay_us(HP203B_IIC_SPEED);
	ClrHp203bSCL();
}

/**********************************************************
**Name: 	HP203B_vIICStop
**Func: 	IIC Stop Singal
**Note: 	
**********************************************************/
void Hp203bI2cClass::HP203B_vIICStop(void)
{
	ClrHp203bSCL();
	_delay_us(HP203B_IIC_SPEED);
	ClrHp203bSDA();
	_delay_us(HP203B_IIC_SPEED);
	SetHp203bSCL();
	_delay_us(HP203B_IIC_SPEED);
	SetHp203bSDA();
}

/**********************************************************
**Name: 	HP203B_vIICAck
**Func: 	IIC ACK Singal
**Note: 	
**********************************************************/
void Hp203bI2cClass::HP203B_vIICAck(void)
{
	ClrHp203bSDA();
	_delay_us(HP203B_IIC_SPEED);
	SetHp203bSCL();
	_delay_us(HP203B_IIC_SPEED);
	ClrHp203bSCL();
}

/**********************************************************
**Name: 	HP203B_vIICNack
**Func: 	IIC NACK Singal
**Note: 	
**********************************************************/
void Hp203bI2cClass::HP203B_vIICNack(void)	
{
	SetHp203bSDA();
	_delay_us(HP203B_IIC_SPEED);
	SetHp203bSCL();
	_delay_us(HP203B_IIC_SPEED);
	ClrHp203bSCL();
}

/**********************************************************
**Name: 	HP203B_bIICWriteByte
**Func: 	IIC Write one byte
**Output:   true-------ACK
            false------NACK
**********************************************************/
bool Hp203bI2cClass::HP203B_bIICWriteByte(byte para)
{
	byte i;

	for(i=0x80;i!=0;i>>=1)	
	{
		ClrHp203bSCL();
		if(para&i)
			SetHp203bSDA();
		else
			ClrHp203bSDA();
		_delay_us(HP203B_IIC_SPEED);
		SetHp203bSCL();
		_delay_us(HP203B_IIC_SPEED);
	}
	ClrHp203bSCL();					//waite for ack or nack
	SetHp203bSDA();					//shift to input mode
	Hp203bSdaIn();					//input
	_delay_us(HP203B_IIC_SPEED);
	_delay_us(HP203B_IIC_SPEED);
	SetHp203bSCL();					//
	for(i=100;i!=0;i--)		//Timeout check
	{
		_delay_us(HP203B_IIC_SPEED);
		if(HP203B_SDA_L())				//ack is active
			break;
	}
	ClrHp203bSCL();
	Hp203bSdaOut();
	_delay_us(HP203B_IIC_SPEED);
	SetHp203bSDA();
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
byte Hp203bI2cClass::HP203B_bIICReadByte(bool ack)
{
	byte i;
	byte para = 0;

 	ClrHp203bSCL();
 	Hp203bSdaIn();					//input
	_delay_us(HP203B_IIC_SPEED);
	_delay_us(HP203B_IIC_SPEED);
	for(i=0x80;i!=0;i>>=1)
 	{
 		SetHp203bSCL();
 		_delay_us(HP203B_IIC_SPEED);
 		if(HP203B_SDA_H())
 			para |= i;
 		ClrHp203bSCL();
 		_delay_us(HP203B_IIC_SPEED);
 	}
 	Hp203bSdaOut();
	_delay_us(HP203B_IIC_SPEED);
	if(ack)
		HP203B_vIICAck();
	else
		HP203B_vIICNack();
	return(para);
}

/**********************************************************
**Name: 	HP203B_vIICInit
**Func: 	Init Port & config
**Note: 	
**********************************************************/
void Hp203bI2cClass::HP203B_vIICInit(void)
{
	DDRB  |= (_HP203B_SDA|_HP203B_SCL);
	PORTB |= (_HP203B_SDA|_HP203B_SCL);
}

/**********************************************************
**Name: 	HP203B_bIICWriteSingle
**Func: 	IIC wirte command
**input: 	chip---------chip address
			cmd----------command value
**Output:	true---------write success
            false--------write faild
**********************************************************/
bool Hp203bI2cClass::HP203B_bIICWriteSingle(byte chip, byte cmd)
{
	HP203B_vIICStart();
	if(HP203B_bIICWriteByte(chip))
	{
		if(HP203B_bIICWriteByte(cmd))
		{
			HP203B_vIICStop();
			return(true);
		}
	}
	HP203B_vIICStop();	
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
byte Hp203bI2cClass::HP203B_bIICReadSingle(byte chip)
{
	byte value = 0;

	HP203B_vIICStart();
	if(HP203B_bIICWriteByte(chip|0x01))	//read
	{
		value = HP203B_bIICReadByte(false);	
		HP203B_vIICStop();
		return(value);
	}
	HP203B_vIICStop();
	return(value);
}

/*
bool Hp203bI2cClass::HP203B_bIICReadSingle(byte chip, byte *value)
{
	HP203B_vIICStart();
	if(HP203B_bIICWriteByte(chip|0x01))	//read
	{
		*value = HP203B_bIICReadByte(false);	
		HP203B_vIICStop();
		return(true);
	}
	HP203B_vIICStop();
	return(false);
}
*/

/**********************************************************
**Name: 	HP203B_bIICReadSeveral
**Func: 	IIC read single value
**input: 	chip---------chip address
			*value-------read out value
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool Hp203bI2cClass::HP203B_bIICReadSeveral(byte chip, byte value[], byte length)
{
	byte i;

	HP203B_vIICStart();
	if(HP203B_bIICWriteByte(chip|0x01))	//read
	{
		for(i=0;i<(length-1);i++)
			value[i] = HP203B_bIICReadByte(true);
		value[i] = HP203B_bIICReadByte(false);	
		HP203B_vIICStop();
		return(true);
	}
	HP203B_vIICStop();	
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
bool Hp203bI2cClass::HP203B_bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length)
{
	byte i;

	HP203B_vIICStart();
	if(HP203B_bIICWriteByte(chip))
	{
		if(HP203B_bIICWriteByte(addr))
		{
			for(i=0;i<length;i++)
			{
				if(!HP203B_bIICWriteByte(ptr[i]))
					break;
			}
			if(i>=length)
			{
				HP203B_vIICStop();
				return(true);
			}
		}
	}
	HP203B_vIICStop();	
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
bool Hp203bI2cClass::HP203B_bIICBurstRead(byte chip, byte addr, byte ptr[], byte length)
{
	byte i;

	HP203B_vIICStart();
	if(HP203B_bIICWriteByte(chip))
	{
		if(HP203B_bIICWriteByte(addr))
		{
			HP203B_vIICStart();
			if(HP203B_bIICWriteByte(chip|0x01))	//chip read address
			{
				for(i=0;i<(length-1);i++)
					ptr[i] = HP203B_bIICReadByte(true);
				ptr[i] = HP203B_bIICReadByte(false);	
				HP203B_vIICStop();
				return(true);
			}
		}
	}
	HP203B_vIICStop();
	return(false);
}

/**********************************************************
------	Hope hp203b i2c end		--------------------------
**********************************************************/