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
 * file       HopeDuino_CMT2119A.cpp
 * brief      driver for CMT2119A 
 * hardware   HopeDuino with RFM219S
 *            
 *
 * version    1.0
 * date       Feb 25 2016
 * author     QY Ruan
 */

#include <HopeDuino_CMT2219A.h>

/**********************************************************
**Name:     vGoRx
**Function: Entry Rx Mode
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vGoRx(void)
{
 byte tmp;
 tmp = Spi3.bSpi3Read((byte)(OP_MODE>>8));		//基本配置
 tmp &= OP_MASK;
 Spi3.vSpi3Write(OP_MODE+tmp+OP_RX);
}

/**********************************************************
**Name:     vGoSleep
**Function: Entry Sleep Mode
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vGoSleep(void)
{
 byte tmp;	
 tmp = Spi3.bSpi3Read((byte)(OP_MODE>>8));
 tmp &= OP_MASK;
 Spi3.vSpi3Write(OP_MODE+tmp+OP_SLEEP);
}

/**********************************************************
**Name:     vGoStandby
**Function: Entry Standby Mode
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vGoStandby(void)
{
 byte tmp;	
 tmp = Spi3.bSpi3Read((byte)(OP_MODE>>8));
 tmp &= OP_MASK;
 Spi3.vSpi3Write(OP_MODE+tmp+OP_STANDBY);	
}

/**********************************************************
**Name:     vSoftReset
**Function: Software reset Chipset
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vSoftReset(void)
{
 Spi3.vSpi3Write(SOFT_RST); 
}

/**********************************************************
**Name:     vClearIntFlag
**Function: clear all irq flag
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vClearIntFlag(void)
{
 Spi3.vSpi3Write(INTCTL_B+0xFF);
}

/**********************************************************
**Name:     vClearFIFO
**Function: clear FIFO buffer
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vClearFIFO(void)
{
 byte tmp;	
 tmp = Spi3.bSpi3Read((byte)(INTCTL_D>>8));
 Spi3.vSpi3Write(INTCTL_D+tmp+FIFO_CLR);
}

/**********************************************************
**Name:     bReadStatus
**Function: read chipset status
**Input:    none
**Output:   none
**********************************************************/
byte cmt2219aClass::bReadStatus(void)
{
 return(0xE0&(Spi3.bSpi3Read((byte)(OP_MODE>>8))));
}

/**********************************************************
**Name:     vInit
**Function: Init. CMT2119A
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vInit(byte cfg[])
{
 byte i;
 Spi3.vSpi3Init();
 vGoSleep();					//
 _delay_ms(10);						
 vGoStandby();
 
 for(i=0; i<62; i++)			//exp file have 62
 	Spi3.vSpi3Write(((word)i<<8)|cfg[i]);

 vClearIntFlag();
 vGoStandby();					//进入STB状态
}

/**********************************************************
**Name:     vGpioFuncCfg
**Function: GPIO Function config
**Input:    none
**Output:   none
**********************************************************/
void cmt2219aClass::vGpioFuncCfg(byte io_cfg)
{
 Spi3.vSpi3Write(IO_SEL+io_cfg);
}

/**********************************************************
**Name:     vIntSourceCfg
**Function: config 
**Input:    int_1, int_2
**Output:   none
**********************************************************/
void cmt2219aClass::vIntSourcCfg(byte int_1, byte int_2)
{
 Spi3.vSpi3Write(INTCTL_A+int_2+((int_1)>>4));
}

/**********************************************************
**Name:     vEnableIntSource
**Function: enable interrupt source 
**Input:    en_int
**Output:   none
**********************************************************/
void cmt2219aClass::vEnableIntSource(byte en_int)
{
 Spi3.vSpi3Write(INT_EN+en_int);				
}

/**********************************************************
**Name:     bReadIngFlag
**Function: Read interrupt flag(INTCTL_C)
**Input:    none
**Output:   interrupt flag
**********************************************************/
byte cmt2219aClass::bReadIngFlag(void)
{
 return(Spi3.bSpi3Read((byte)(INTCTL_C>>8)));
}	

/**********************************************************
**Name:     bReadRssi
**Function: Read Rssi
**Input:    none
**Output:   none
**********************************************************/
byte cmt2219aClass::bReadRssi(void)
{
 return(Spi3.bSpi3Read((byte)(RSSI_ADDR>>8)));
}
	
/******************************************************************************
**函数名称：vbGetMessage
**函数功能：接收一包数据
**输入参数：无
**输出参数：1——接收成功
**          0——接收失败
******************************************************************************/
byte cmt2219aClass::bGetMessage(byte msg[])
{
 byte i;
 byte tmp;
 byte length=0;
 
 
 tmp = bReadIngFlag();
 if(CrcDisable)
 	{
 	if(tmp&RX_DONE_FLG)		
 		{
 		if(FixedPktLength)
 			length = PktLength;
 		else
 			length = Spi3.bSpi3ReadFIFO();
		Spi3.vSpi3BurstReadFIFO(msg, length);
		vClearIntFlag();
		vClearFIFO();
		return(length);
 		}
 	}
 else
 	{
 	if(tmp&CRC_PS_FLG)
 		{
		if(FixedPktLength)
 			length = PktLength;
 		else
 			length = Spi3.bSpi3ReadFIFO();
		Spi3.vSpi3BurstReadFIFO(msg, length);
		vClearIntFlag();
		vClearFIFO();
		return(length);
 		}
 	}
 return(0);
}






