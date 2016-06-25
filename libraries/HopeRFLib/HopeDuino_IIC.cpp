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

#include "HopeDuino_IIC.h"

/**********************************************************
**Name: 	vIICStart
**Func: 	IIC Start Singal
**Note: 	
**********************************************************/
void iicClass::vIICStart(void)
{
 PORTC &= ~(_SDA|_SCL);			//PortC for Low	
 SetSDA();
 _delay_us(IIC_SPEED);
 SetSCL();
 _delay_us(IIC_SPEED);
 ClrSDA();
 _delay_us(IIC_SPEED);
 ClrSCL();
}

/**********************************************************
**Name: 	vIICStop
**Func: 	IIC Stop Singal
**Note: 	
**********************************************************/
void iicClass::vIICStop(void)
{
 ClrSCL();
 _delay_us(IIC_SPEED);
 ClrSDA();
 _delay_us(IIC_SPEED);
 SetSCL();
 _delay_us(IIC_SPEED);
 SetSDA();
}

/**********************************************************
**Name: 	vIICAck
**Func: 	IIC ACK Singal
**Note: 	
**********************************************************/
void iicClass::vIICAck(void)
{
 ClrSDA();
 _delay_us(IIC_SPEED);
 SetSCL();
 _delay_us(IIC_SPEED);
 ClrSCL();
}

/**********************************************************
**Name: 	vIICNack
**Func: 	IIC NACK Singal
**Note: 	
**********************************************************/
void iicClass::vIICNack(void)	
{
 SetSDA();
 _delay_us(IIC_SPEED);
 SetSCL();
 _delay_us(IIC_SPEED);
 ClrSCL();
}

/**********************************************************
**Name: 	bIICWriteByte
**Func: 	IIC Write one byte
**Output:   true-------ACK
            false------NACK
**********************************************************/
bool iicClass::bIICWriteByte(byte para)
{
 byte i;
 
 for(i=0x80;i!=0;i>>=1)	
 	{
 	ClrSCL();
 	if(para&i)
 		SetSDA();
 	else
 		ClrSDA();
 	_delay_us(IIC_SPEED);
 	SetSCL();
 	_delay_us(IIC_SPEED);
 	}
 _delay_us(IIC_SPEED);
 SetSDA();					//shift to input mode
 ClrSCL();					//waite for ack or nack
 _delay_us(IIC_SPEED);
 _delay_us(IIC_SPEED);
 SetSCL();					//
 for(i=100;i!=0;i--)		//Timeout check
 	{
 	_delay_us(IIC_SPEED);
 	if(SDA_L())				//ack is active
 		break;
 	}
 ClrSCL();			
 if(i==0) 					//Timeout
	return(false);
 else
 	return(true);
}	

/**********************************************************
**Name: 	bIICReadByte
**Func: 	IIC Read one byte
**Input:    true-------Send ACK
            false------Send NACK
**********************************************************/
byte iicClass::bIICReadByte(bool ack)
{
 byte i;
 byte para = 0;
 
 SetSDA();					//Shift to input mode
 
 for(i=0x80;i!=0;i>>=1)
 	{
 	SetSCL();
 	_delay_us(IIC_SPEED);
 	if(SDA_H())	
 		para |= i;
 	ClrSCL();
 	_delay_us(IIC_SPEED);
 	}
 if(ack)
 	vIICAck();
 else
 	vIICNack();
 return(para);
}

/**********************************************************
**Name: 	vIICInit
**Func: 	Init Port & config
**Note: 	
**********************************************************/
void iicClass::vIICInit(void)
{
 DIDR0 &= ~(_SDA|_SCL);				//A4~A5 use for digital IO port
 DDRC  &= ~(_SDA|_SCL);				//Lcd Port for Input
 PINC  |= (_SDA|_SCL);				//pull up enable
 PORTC &= ~(_SDA|_SCL);				//PortC for Low
}

/**********************************************************
**Name: 	bIICWriteSingle
**Func: 	IIC wirte command
**input: 	chip---------chip address
			cmd----------command value
**Output:	true---------write success
            false--------write faild
**********************************************************/
bool iicClass::bIICWriteSingle(byte chip, byte cmd)
{
 vIICStart();
 if(bIICWriteByte(chip))
 	{
 	if(bIICWriteByte(cmd))
 		{
 		vIICStop();
		return(true);
 		}
 	}
 vIICStop();	
 return(false);
}

/**********************************************************
**Name: 	bIICReadSingle
**Func: 	IIC read single value
**input: 	chip---------chip address
			*value-------read out value
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool iicClass::bIICReadSingle(byte chip, byte *value)
{
 vIICStart();
 if(bIICWriteByte(chip|0x01))	//read
 	{
 	*value = bIICReadByte(false);	
 	vIICStop();
	return(true);
 	}
 vIICStop();	
 return(false);
}

/**********************************************************
**Name: 	bIICReadSeveral
**Func: 	IIC read single value
**input: 	chip---------chip address
			*value-------read out value
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool iicClass::bIICReadSeveral(byte chip, byte value[], byte length)
{
 byte i;
 vIICStart();
 if(bIICWriteByte(chip|0x01))	//read
 	{
 	for(i=0;i<(length-1);i++)
 		value[i] = bIICReadByte(true);
 	value[i] = bIICReadByte(false);	
 	vIICStop();
	return(true);
 	}
 vIICStop();	
 return(false);
}


/**********************************************************
**Name: 	bIICBurstWrite
**Func: 	IIC wirte some byte 
**input: 	chip---------chip address
			addr---------regesisiter address
            ptr----------write buffer pointer    
            length-------buffer length
**Output:	true---------write success
            false--------write faild
**********************************************************/
bool iicClass::bIICBurstWrite(byte chip, byte addr, byte ptr[], byte length)
{
 byte i;	
 vIICStart();
 if(bIICWriteByte(chip))
 	{
 	if(bIICWriteByte(addr))
 		{
 		for(i=0;i<length;i++)
 			{
 			if(!bIICWriteByte(ptr[i]))
 				break;
 			
 			}
 		if(i>=length)
 			{
 			vIICStop();
			return(true);
 			}		
 		}
 	}
 vIICStop();	
 return(false);
}

/**********************************************************
**Name: 	bIICBurstRead
**Func: 	IIC read some byte 
**input: 	chip---------chip address
			addr---------regesisiter address
            ptr----------readout buffer pointer    
            length-------readout buffer length
**Output:	true---------read success
            false--------read faild
**********************************************************/
bool iicClass::bIICBurstRead(byte chip, byte addr, byte ptr[], byte length)
{
 byte i;	
 vIICStart();
 if(bIICWriteByte(chip))
 	{
 	if(bIICWriteByte(addr))
 		{
 		vIICStart();
 		if(bIICWriteByte(chip|0x01))	//chip read address
 			{
 			for(i=0;i<(length-1);i++)
 				ptr[i] = bIICReadByte(true);
 			ptr[i] = bIICReadByte(false);	
 			vIICStop();
 			return(true);
 			}
 		}
 	}
 vIICStop();	
 return(false);
}
