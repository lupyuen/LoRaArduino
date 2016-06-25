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
 * file       HopeDuino_CMT2219A.h
 * brief      show how to use CMT2219A
 * hardware   HopeDuino with RFM219S
 *            
 *
 * version    1.0
 * date       Feb 20 2016
 * author     QY Ruan
 */
 
 #ifndef HopeDuino_CMT2219A_h
	#define HopeDuino_CMT2219A_h

	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif	

	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif	

	#include <avr/io.h>
	#include <util/delay.h>
	
	#include <HopeDuino_SPI3.h>
	
	/** Hardware brief **/    
	//PORTB						//DDRx		PORTx		PINx
	#define _GPO1    0x01       // 0          0          0
	
	//PORTD
	#define _GPO2    0x80       // 0          0          0
	#define _GPO4    0x40       // 0          0          0
	#define _GPO3    0x20       // 0          0          0 

	#define GPO1_H() 	(PINB&_GPO1)	
	#define GPO1_L()	((PINB&_GPO1)==0)

	#define GPO2_H() 	(PIND&_GPO2)	
	#define GPO2_L()	((PIND&_GPO2)==0)

	#define GPO3_H() 	(PIND&_GPO3)	
	#define GPO3_L()	((PIND&_GPO3)==0)

	#define GPO4_H() 	(PIND&_GPO4)	
	#define GPO4_L()	((PIND&_GPO4)==0)
	
	//*********************************************************
	//Reg mapping
	//*********************************************************
	#define	INT_EN				0x3F00
		#define RX_DONE_EN		0x01
		#define CRC_PS_EN		0x02
		#define	NODE_PS_EN		0x04
		#define	SYNC_PS_EN		0x08
		#define PREAM_PS_EN		0x10
		#define	RSSI_VLD_EN		0x20
		#define	RX_TMO_EN		0x40
		#define	SL_TMO_EN		0x80

	#define	IO_SEL				0x4000
		#define GPIO1_POR		0x00
		#define GPIO1_INT1		0x01
		#define	GPIO1_INT2		0x02
		#define	GPIO1_Dout		0x03
		
		#define	GPIO2_INT1		0x00
		#define	GPIO2_INT2		0x04
		#define	GPIO2_DCLK		0x08
		#define	GPIO2_0			0x0C	
		
		#define	GPIO3_CLK		0x00
		#define GPIO3_INT1		0x10
		#define	GPIO3_INT2		0x20
		#define	GPIO3_DCLK		0x30
		
		#define	GPIO4_Dout		0x00
		#define	GPIO4_INT1		0x40
		#define	GPIO4_INT2		0x80
		#define	GPIO4_DCLK		0xC0
	

	#define	INTCTL_A			0x4100
		#define	RSSI_VLD		0x010		//Ò»´ÎÐÔ
			
		#define	PREAM_PS		0x020
		#define	SYNC_PS			0x030
		
		#define	NODE_PS			0x040
		#define	CRC_PS			0x050
		#define	RX_DONE			0x060
    	
		#define	SL_TMO			0x040		//+OFFSET
		#define	RX_TMO			0x050
		#define	FIFO_NMTY		0x060
		#define	FIFO_TH			0x070
		#define	FIFO_FULL		0x080
		#define	FIFO_WBYTE		0x090
		#define FIFO_OVF		0x0A0
		#define	RSSI_INDI		0x0B0
		
		#define	OFFSET			0x030

	#define	INTCTL_B			0x4200
		#define	RX_DONE_CLR		0x01
		#define	CRC_PS_CLR		0x02
		#define	NODE_PS_CLR		0x04
		#define	SYNC_PS_CLR		0x08
		#define	PREAM_PS_CLR	0x10
		#define	RSSI_VLD_CLR	0x20
		#define RX_TMO_CLR		0x40
		#define	SL_TMO_CLR		0x80

	#define	INTCTL_C			0x4300
		#define	RX_DONE_FLG		0x01
		#define	CRC_PS_FLG		0x02
		#define	NODE_PS_FLG		0x04
		#define	SYNC_PS_FLG		0x08
		#define	PREAM_PS_FLG	0x10
		#define	RSSI_VLD_FLG	0x20
		#define	RX_TMO_FLG		0x40			
		#define	SL_TMO_FLG		0x80

	#define	INTCTL_D			0x4400
		#define	FIFO_WR_METH	0x01
		#define	FIFO_WR_EN		0x02
		#define	FIFO_CLR		0x04
		#define	FIFO_OVF_FLG	0x08
		#define	FIFO_TH_FLG		0x10
		#define	FIFO_NMTY_FLG	0x20
		#define	FIFO_FULL_FLG	0x40
		#define	PKT_LEN_ERR_FLG	0x80	//?

	#define	RSSI_ADDR			0x4500

	#define	FUNC_EN				0x4600
		#define	EEPROM_LOCK		0x01
		#define	EEPROM_PWRON 	0x02
		#define FAST_SPI_EN		0x04
		#define	SA_TEST_EN		0x08

	#define	OP_MODE				0x4700
		#define OP_EEPROM		0x01
		#define	OP_STANDBY		0x02	
		#define	OP_FS			0x04
		#define	OP_RX			0x08
		#define	OP_SLEEP		0x10
	
		#define	RD_IDLE			0x00
		#define	RD_SLEEP		0x20
		#define	RD_STBY			0x40
		#define	RD_FS			0x60
		#define	RD_RX			0x80
		#define	RD_EEPROM		0xA0
		                    	
		#define OP_MASK			0xE0

	#define	SOFT_RST 			0x4FFF

	#define	EE_DAT_LOW			0x5000
	#define EE_DAT_HIGH			0x5100
	#define	EE_ADD				0x5200

	#define	EE_CTL				0x5300
		#define	EE_ERASE		0x01
		#define	EE_PROG			0x02
		#define	EE_READ			0x04
	
	class cmt2219aClass
	{
	 public:	
		bool CrcDisable;							//false: CRC enable£¬ & use CCITT 16bit 
													//true : CRC disable
		bool FixedPktLength;						//false: for contain packet length in Tx message, the same mean with variable lenth
 		                                            //true : for doesn't include packet length in Tx message, the same mean with fixed length
		bool NodeDisable;							//false: Node Address Enable
													//true : Node Address Disable
		byte PktLength;											

		void vInit(byte cfg[]);	
		void vGoRx(void);
		void vGoSleep(void);
		void vGoStandby(void);
		void vSoftReset(void);
		void vClearFIFO(void);
		byte bReadStatus(void);
		byte bReadRssi(void);
		byte bReadIngFlag(void);
		void vClearIntFlag(void);
		void vGpioFuncCfg(byte io_cfg);
		void vIntSourcCfg(byte int_1, byte int_2);
		void vEnableIntSource(byte en_int);
		byte bGetMessage(byte msg[]);
					
	 private:
	 	spi3Class Spi3;
	 	
	};
#else
	#warning "HopeDuino_CMT2219A.h have been defined!"

#endif
