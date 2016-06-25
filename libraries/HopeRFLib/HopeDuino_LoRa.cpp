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
 * file       HopeDuino_LoRa.cpp
 * brief      driver for RFM92/95/96/98
 * hardware   HopeRF's RFduino TRx & with HopeRF's LoRa COB rf-module  
 *            
 *
 * version    1.0
 * date       Jun 3 2014
 * author     QY Ruan
 */

#include "HopeDuino_LoRa.h"

/********************************************************** 
**RF69 Regsister define                                      
**********************************************************/ 
//Common Regsister
#define 	RegFifo 			0x00 
#define 	RegOpMode 			0x01 
#define 	RegFrMsb 			0x06 
#define 	RegFrMid 			0x07 
#define 	RegFrLsb 			0x08
#define		RegPaConfig			0x09
#define 	RegPaRamp 			0x0A 
#define 	RegOcp 			    0x0B
#define 	RegLna 			    0x0C 

#define 	RegDioMapping1 	    0x40 
#define 	RegDioMapping2 	    0x41 
#define 	RegVersion 		    0x42 

//for 6/7/8
#define		RegPllHop			0x44
#define		RegTcxo				0x4B
#define		RegPaDac			0x4D
#define		RegFormerTemp		0x5B
#define		RegBitrateFrac		0x5D
#define		RegAgcRef			0x61
#define		RegAgcThresh1		0x62
#define		RegAgcThresh2		0x63
#define		RegAgcThresh3		0x64
#define		RegPll				0x70

//for 2/3
#define		RegAgcRef_2			0x43
#define		RegAgcThresh1_2		0x44
#define		RegAgcThresh2_2		0x45
#define		RegAgcThresh3_2		0x46
#define		RegPllHop_2			0x4B
#define		RegTcxo_2			0x58
#define		RegPaDac_2			0x5A
#define		RegPll_2			0x5C
#define		RegPllLowPn			0x5E
#define		RegFormerTemp_2		0x6C
#define		RegBitrateFrac_2	0x70

//ASK/FSK/GFSK Regsister
#define 	RegBitrateMsb 		0x02
#define 	RegBitrateLsb 		0x03 
#define 	RegFdevMsb 		    0x04 
#define 	RegFdevLsb 		    0x05 

#define		RegRxConfig			0x0D
#define 	RegRssiConfig 		0x0E
#define		RegRssiCollision	0x0F
#define 	RegRssiThresh 		0x10 
#define 	RegRssiValue 		0x11
#define 	RegRxBw 			0x12
#define 	RegAfcBw 			0x13
#define 	RegOokPeak 		    0x14 
#define 	RegOokFix 			0x15 
#define 	RegOokAvg 			0x16  
#define 	RegAfcFei 			0x1A
#define 	RegAfcMsb 			0x1B
#define 	RegAfcLsb 			0x1C 
#define 	RegFeiMsb 			0x1D
#define 	RegFeiLsb 			0x1E 
#define		RegPreambleDetect	0x1F
#define 	RegRxTimeout1 		0x20 
#define 	RegRxTimeout2 		0x21 
#define 	RegRxTimeout3 		0x22 
#define		RegRxDelay			0x23
#define 	RegOsc 				0x24
#define 	RegPreambleMsb 	    0x25 
#define 	RegPreambleLsb 	    0x26 
#define 	RegSyncConfig 		0x27
#define 	RegSyncValue1		0x28 
#define 	RegSyncValue2       0x29 
#define 	RegSyncValue3       0x2A 
#define 	RegSyncValue4       0x2B 
#define 	RegSyncValue5       0x2C 
#define 	RegSyncValue6       0x2D 
#define 	RegSyncValue7       0x2E 
#define 	RegSyncValue8       0x2F
#define 	RegPacketConfig1 	0x30 
#define 	RegPacketConfig2 	0x31 
#define 	RegPayloadLength 	0x32 
#define 	RegNodeAdrs 		0x33 
#define 	RegBroadcastAdrs 	0x34 
#define 	RegFifoThresh 		0x35
#define		RegSeqConfig1		0x36
#define		RegSeqConfig2		0x37
#define		RegTimerResol		0x38
#define		RegTimer1Coef		0x39
#define		RegTimer2Coef		0x3A
#define		RegImageCal			0x3B
#define		RegTemp				0x3C
#define		RegLowBat			0x3D
#define 	RegIrqFlags1 		0x3E
#define 	RegIrqFlags2 		0x3F


//LoRa Regsister
#define		RegFifoAddrPtr				0x0D
#define 	RegFifoTxBaseAddr			0x0E
#define		RegFifoRxBaseAddr			0x0F
#define 	RegFifoRxCurrentAddr		0x10 
#define 	RegIrqFlagsMask				0x11
#define 	RegIrqFlags					0x12
#define 	RegRxNbBytes				0x13
#define 	RegRxHeaderCntValueMsb    	0x14 
#define 	RegRxHeaderCntValueLsb		0x15 
#define 	RegRxPacketCntValueMsb		0x16  
#define 	RegRxPacketCntValueLsb		0x17
#define 	RegModemStat				0x18
#define 	RegPktSnrValue				0x19
#define 	RegPktRssiValue				0x1A
#define 	RegRssiValue_LR				0x1B
#define		RegHopChannel				0x1C
#define 	RegModemConfig1				0x1D
#define 	RegModemConfig2 			0x1E
#define 	RegSymbTimeoutLsb 			0x1F
#define		RegPreambleMsb_LR			0x20
#define 	RegPreambleLsb_LR			0x21
#define 	RegPayloadLength_LR 	    0x22
#define		RegMaxPayloadLength			0x23
#define		RegHopPeriod				0x24
#define		RegFifoRxByteAddr			0x25
#define		RegModemConfig3				0x26		// only for 6/7/8



/**********************************************************      
**RF69 mode status                                          
**********************************************************/      
#define		RADIO_SLEEP			(0x00)
#define		RADIO_STANDBY		(0x01)
#define		RADIO_TX			(0x03)
#define		RADIO_RX			(0x05)

#define		FskMode				(0<<5)
#define		OokMode				(1<<5) 

#define		Shaping				2

#define 	MODE_MASK			0xF8

#define		MOUDLE_MASK_2		0x87			//for RFM92/93
#define		MOUDLE_MASK_1		0x9F			//for RFM95/96/97/98

#define		AFC_ON				(1<<4)
#define		AGC_ON				(1<<3)
#define		RX_TRIGGER			0x06

#define		PREAMBLE_DECT_ON	(1<<7)
#define		PREAMBLE_DECT_1BYTE	(0<<5)
#define		PREAMBLE_DECT_2BYTE	(1<<5)
#define		PREAMBLE_DECT_3BYTE	(2<<5)

#define		AUTO_RST_RX_OFF		(0<<6)
#define		AUTO_RST_RX_ON		(1<<6)
#define		AUTO_RST_RX_ONwPLL	(2<<6)	
#define		SYNC_ON				(1<<4)

//for PacketConfig
#define		VariablePacket		(1<<7)
#define		DcFree_NRZ			(0<<5)
#define		DcFree_MANCHESTER	(1<<5)
#define		DcFree_WHITENING	(2<<5)
#define		CrcOn				(1<<4)
#define		CrcDisAutoClear		(1<<3)
#define		AddrFilter_NONE		(0<<1)
#define		AddrFilter_NODE		(1<<1)
#define		AddrFilter_ALL		(2<<1)
#define		CrcCalc_CCITT		0x00
#define		CrcCalc_IBM			0x01

#define		PacketMode			(1<<6)
#define		ContinuousMode		(0<<6)

//for LoRa
#define		AllIrqMask				0xFF
#define		RxTimeoutMask			(1<<7)
#define		RxDoneMask				(1<<6)
#define		PayloadCrcErrorMask		(1<<5)
#define		ValidHeaderMask			(1<<4)
#define		TxDoneMask				(1<<3)
#define		CadDoneMask				(1<<2)
#define		FhssChangeChannelMask	(1<<1)
#define		CadDetectedMask			(1<<0)




/**********************************************************
**Name:     vInitialize
**Function: initialize rfm9x
**Input:    none
**Output:   none
**********************************************************/
void loraClass::vInitialize(void)
{
 ClrPOR();
 PORIn();
 DIO0In();
 Spi.vSpiInit();									//init port
 						
 FrequencyValue.Freq = (Frequency<<11)/125;			//calc frequency
 BitRateValue  = (SymbolTime<<5)/1000;				//calc bitrate
 DevationValue = (Devation<<11)/125;				//calc deviation
 BandWidthValue = bSelectBandwidth(BandWidth);	
 
 switch(SFSel)
 	{
 	case SF6:  SFValue = 6; break;
 	case SF7:  SFValue = 7; break;
 	case SF8:  SFValue = 8; break;
 	case SF10: SFValue = 10;break;
 	case SF11: SFValue = 11;break;
 	case SF12: SFValue = 12;break;
 	case SF9:  
 	default:   SFValue = 9; break;
 	}
 switch(BWSel)
 	{
 	case BW62K:  BWValue = 6; break;				//for RFM95/96/97/98
 	case BW250K: BWValue = 8; break;
 	case BW500K: BWValue = 9; break;
 	case BW125K: 
 	default:     BWValue = 7; break;
 	}
 switch(CRSel)
 	{
 	default:
 	case CR4_5: CRValue = 1; break;
 	case CR4_6: CRValue = 2; break;
 	case CR4_7: CRValue = 3; break;
 	case CR4_8: CRValue = 4; break;
 	}
 
 if((SFValue-4)>=BWValue)	
 	RsOptimize = true;	
 else
 	RsOptimize = false;								
 	
 vConfig();
 vGoStandby();
}

/**********************************************************
**Name:     vConfig
**Function: config rfm9x
**Input:    none
**Output:   none
**********************************************************/
void loraClass::vConfig(void)
{
 byte i, j;
 byte sync;
 
 vReset();
 vGoStandby();

 //Frequency
 Spi.vSpiWrite(((word)RegFrMsb<<8)+FrequencyValue.freq.FreqH);
 Spi.vSpiWrite(((word)RegFrMid<<8)+FrequencyValue.freq.FreqM);
 Spi.vSpiWrite(((word)RegFrLsb<<8)+FrequencyValue.freq.FreqL);

 //PA Config
 i = Spi.bSpiRead(RegPaConfig);
 i &= 0x70;
 if(OutputPower>20)
 	{ i |= 0x0F; j = 0x87; }
 else if(OutputPower>17)
 	{ i |= (OutputPower-3); j = 0x87; }
 else if(OutputPower>=2)
 	{ i |= (OutputPower-2); j = 0x84; }
 else
 	{ i |= 0; j = 0x84; }
 Spi.vSpiWrite(((word)RegPaConfig<<8)+0x80+i);	//PA_BOOST
 switch(COB)
 	{
 	case RFM92:
 	case RFM93: Spi.vSpiWrite(((word)RegPaDac_2<<8)+j); break;
 	default:	Spi.vSpiWrite(((word)RegPaDac<<8)+j); break;
 	}
 j = Spi.bSpiRead(RegPaRamp);
 j&= 0x0F;
 j|= bSelectRamping(SymbolTime);
 Spi.vSpiWrite(((word)RegPaRamp<<8)+j);

 //Ocp
 Spi.vSpiWrite(((word)RegOcp<<8)+0x0F);			//Disable Ocp

 //LNA
 //Spi.vSpiWrite(((word)RegLna<<8)+0x20);		//High & LNA Enable


 
 // Mode
 i = Spi.bSpiRead(RegOpMode);
 j = Spi.bSpiRead(RegPaRamp);		//for RFM95/96/97/98
 if(Modulation==LORA)
 	{
 	i &= 0x87;	
	switch(COB)
		{
		case RFM96:
		case RFM98: i |= 0x08; break;
		default: break;
		}
	Spi.vSpiWrite(((word)RegOpMode<<8)+i);

	i  = Spi.bSpiRead(0x31);			//SetNbTrigPeaks
	i &= 0xF8;
	if(SFSel==SF6)
		{
		i |= 0x05; 
		Spi.vSpiWrite(0x3100+i);
		Spi.vSpiWrite(0x3700+0x0C);
		FixedPktLength = true;			//SF6 must be ImplicitHeaderMode
		}
	else
		{
		i |= 0x03;
		Spi.vSpiWrite(0x3100+i);
		}

	switch(COB)
		{
		byte tmp;
		case RFM92:
		case RFM93:
			if(BWValue>6)
				tmp = BWValue - 7;
			else
				tmp = 0;
			tmp <<= 6;					//BandWidth
			tmp |= (CRValue<<3);
			if(FixedPktLength)			//ImplicitHeader
				tmp |= 0x04;			
			if(!CrcDisable)				//
				tmp |= 0x02;
			if(RsOptimize)				//mandated for when the symbol length exceeds 16ms
				tmp |= 0x01;
			Spi.vSpiWrite(((word)RegModemConfig1<<8)+tmp);
			tmp = (SFValue<<4);			//SF rate
			tmp |= (0x04+0x03);			//AGC ON & Max timeout
			Spi.vSpiWrite(((word)RegModemConfig2<<8)+tmp);
			break;
		case RFM95:
		case RFM97:
		case RFM96:
		case RFM98:
		default:
			tmp = (BWValue<<4) + (CRValue<<1);
			if(FixedPktLength)
				tmp |= 0x01;
			Spi.vSpiWrite(((word)RegModemConfig1<<8)+tmp);
			tmp = (SFValue<<4);
			if(!CrcDisable)
				tmp |= 0x04;
			tmp += 0x03;
			Spi.vSpiWrite(((word)RegModemConfig2<<8)+tmp);
			tmp = 0x04;				//AGC ON
			if(RsOptimize)			//mandated for when the symbol length exceeds 16ms
				tmp |= 0x08;
			Spi.vSpiWrite(((word)RegModemConfig3<<8)+tmp);
			break;
		}
 	Spi.vSpiWrite(((word)RegSymbTimeoutLsb<<8)+0xFF);	//RegSymbTimeoutLsb Timeout = 0x3FF(Max)	
 	Spi.vSpiWrite(((word)RegPreambleMsb_LR<<8)+(byte)(PreambleLength>>8));
 	Spi.vSpiWrite(((word)RegPreambleLsb_LR<<8)+(byte)PreambleLength);

 	Spi.vSpiWrite(((word)RegDioMapping2<<8)+0x40);						//RegDioMapping2	DIO5=00(ModeReady), DIO4=01(PllLock)
 	}
 else
 	{
 	switch(COB)
		{
		case RFM92:
		case RFM93:
			i &= MOUDLE_MASK_2; 
 			switch(Modulation)
 				{
 				case OOK:  i |= OokMode+(Shaping<<3); break;
 				case GFSK: i |= FskMode+(Shaping<<3); break;
 				case FSK:
 				default:   i |= FskMode; break;
 				}			
			break;
		default:
			i &= MOUDLE_MASK_1; 
			j &= 0x9F;
			switch(Modulation)
 				{
 				case OOK:  i |= OokMode; j |= (Shaping<<5); break;
 				case GFSK: i |= FskMode; j |= (Shaping<<5); break;
 				case FSK: 
 				default:   i |= FskMode; break;
 				}			
			break;
		}
 	Spi.vSpiWrite(((word)RegOpMode<<8)+i);
 	Spi.vSpiWrite(((word)RegPaRamp<<8)+j);

 	//BitRate 
 	Spi.vSpiWrite(((word)RegBitrateMsb<<8)+(byte)(BitRateValue>>8));
 	Spi.vSpiWrite(((word)RegBitrateLsb<<8)+(byte)BitRateValue);
 
	//Devation
 	Spi.vSpiWrite(((word)RegFdevMsb<<8)+(((byte)(DevationValue>>8))&0x3F));
 	Spi.vSpiWrite(((word)RegFdevLsb<<8)+(byte)(DevationValue&0xFF)); 	
 	
 	//RxConfig
 	Spi.vSpiWrite(((word)RegRxConfig<<8)+AGC_ON+RX_TRIGGER);
 	
 	//RxBw
 	Spi.vSpiWrite(((word)RegRxBw<<8)+BandWidthValue);	
 	
 	//OOK
 	Spi.vSpiWrite(((word)RegOokPeak<<8)+0x20+(0x02<<3)+0x00);
 	
 	//PreambleDetect
 	Spi.vSpiWrite(((word)RegPreambleDetect<<8)+PREAMBLE_DECT_ON+PREAMBLE_DECT_3BYTE);
 	Spi.vSpiWrite(((word)RegPreambleMsb<<8)+(byte)(PreambleLength>>8));
 	Spi.vSpiWrite(((word)RegPreambleLsb<<8)+(byte)PreambleLength);
 	
 	//Osc
 	Spi.vSpiWrite(((word)RegOsc<<8)+0x07);			//Close OscClk Output
 
 	//SyncConfig
 	if(SyncLength==0)
 		sync = 0;
 	else
 		sync = SyncLength-1;
 	Spi.vSpiWrite(((word)RegSyncConfig<<8)+AUTO_RST_RX_ONwPLL+SYNC_ON+(sync&0x07));
 	for(i=0;i<8;i++)								//SyncWordSetting
 		Spi.vSpiWrite(((word)(RegSyncValue1+i)<<8)+SyncWord[i]);	
 	
 	i = DcFree_NRZ + AddrFilter_NONE + CrcCalc_CCITT;
 	if(!FixedPktLength)
 		i += VariablePacket;
 	if(!CrcDisable)
 		i += CrcOn;
 	Spi.vSpiWrite(((word)RegPacketConfig1<<8)+i);		
 	Spi.vSpiWrite(((word)RegPacketConfig2<<8)+PacketMode);		

 	if(FixedPktLength)								//Set Packet length
 		Spi.vSpiWrite(((word)RegPayloadLength<<8)+PayloadLength);
 	else	
 		Spi.vSpiWrite(((word)RegPayloadLength<<8)+0xFF); 	
 	
 	Spi.vSpiWrite(((word)RegFifoThresh<<8)+0x01);
 	Spi.vSpiWrite(((word)RegDioMapping2<<8)+0x61);	//DIO4 PllLock / DIO5 Data / PreambleDetect
	}
}

/**********************************************************
**Name:     vGoRx
**Function: set rf9x to receive mode
**Input:    none
**Output:   none
**********************************************************/
void loraClass::vGoRx(void)
{
 byte tmp;
 if(Modulation==LORA)
 	{
 	if(FixedPktLength)									//Set Packet length 	
 		Spi.vSpiWrite(((word)RegPayloadLength_LR<<8)+PayloadLength);
 	else	
 		Spi.vSpiWrite(((word)RegPayloadLength_LR<<8)+0xFF); 

 	Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x22);		//DIO0 RxDone / DIO1 CadDetected / DIO2 FhssChangeChannel /DIO3 PayloadCrcError
 	Spi.vSpiWrite(((word)RegIrqFlags<<8)+AllIrqMask);	//Clear All Interrupt
 	Spi.vSpiWrite(((word)RegIrqFlagsMask<<8)+(AllIrqMask&(~(RxDoneMask|RxTimeoutMask))));	//just enable RxDone & Timeout
 	
 	tmp = Spi.bSpiRead(RegFifoRxBaseAddr);				//Read RxBaseAddr
 	Spi.vSpiWrite(((word)RegFifoAddrPtr<<8)+tmp);		//RxBaseAddr -> FiFoAddrPtr¡¡
	}
 else
 	{
 	if(CrcDisable)	
 		Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x00);	//DIO0 PayloadReady / DIO1 FifoLevel / DIO2 FifoFull /DIO3 FifoEmpty
 	else
 		Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x40);	//DIO0 CrcOk  / DIO1 FifoLevel / DIO2 FifoFull /DIO3 FifoEmpty 	
 	}

 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_RX;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
}

/**********************************************************
**Name:     vGoStandby
**Function: set rf9x to standby mode
**Input:    none
**Output:   none
**********************************************************/
void loraClass::vGoStandby(void)
{
 byte tmp;
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_STANDBY;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
}	

/**********************************************************
**Name:     vGoSleep
**Function: set rf9x to sleep mode
**Input:    none
**Output:   none
**********************************************************/
void loraClass::vGoSleep(void)
{
 byte tmp;
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_SLEEP;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
}	

/**********************************************************
**Name:     bSendMessage
**Function: set rf9x to sleep mode
**Input:    msg------for which message to send
            length---message length
**Output:   true-----send ok
            false----send error/over time
**********************************************************/
bool loraClass::bSendMessage(byte msg[], byte length)
{
 byte tmp;
 lword overtime;
 word bittime;

 if(Modulation==LORA)
 	{ 
 	Spi.vSpiWrite(((word)RegPayloadLength_LR<<8)+length);	
 	Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x62);		//DIO0 TxDone / DIO1 CadDetected / DIO2 FhssChangeChannel /DIO3 PayloadCrcError
 	Spi.vSpiWrite(((word)RegIrqFlags<<8)+AllIrqMask);	//Clear All Interrupt
 	Spi.vSpiWrite(((word)RegIrqFlagsMask<<8)+(AllIrqMask&(~TxDoneMask)));	//just enable TxDone
 	
 	tmp = Spi.bSpiRead(RegFifoTxBaseAddr);				//Read TxBaseAddr
 	Spi.vSpiWrite(((word)RegFifoAddrPtr<<8)+tmp);		//RxBaseAddr -> FiFoAddrPtr¡¡
 	
 	Spi.vSpiBurstWrite(RegFifo, msg, length);
	}
 else
 	{
 	Spi.vSpiWrite(((word)RegDioMapping1<<8)+0x00);	//DIO0 PacketSend  / DIO1 FifoLevel / DIO2 FifoFull /DIO3 FifoEmpty

 	if(!FixedPktLength)
 		Spi.vSpiWrite(((word)RegFifo<<8)+length);
 	Spi.vSpiBurstWrite(RegFifo, msg, length);
 	}
 
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_TX;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
 
 if(Modulation==LORA)
 	{
 	overtime = PreambleLength + 5 + 8;	//unit: byte
 	bittime  = (length<<3) + 28 - (SFValue<<2);
 	if(!CrcDisable)
 		bittime += 16;
 	if(FixedPktLength)
 		bittime -= 20;
 	switch(CRSel)
 		{
 		default:
 		case CR4_5: bittime *= 5; break;
 		case CR4_6: bittime *= 6; break;
 		case CR4_7:	bittime *= 7; break;
 		case CR4_8:	bittime *= 8; break;
 		}
 	bittime >>= 2;		
 	if(RsOptimize)
 		bittime /= 10;
 	else
 		bittime /= 12;
 	overtime += bittime;			//unit: byte,  total = Payload + Preamble + Header
 	
 	if(SFValue>=BWValue)			//unit: ms
 		overtime <<= (SFValue-BWValue);
 	else
 		overtime >>= (BWValue-SFValue);
	_delay_ms(overtime);			//
 	for(bittime=0;bittime<5000;bittime++)	//about 500ms for overtime
 		{
		if(DIO0_H())
			break; 	
		_delay_us(100);
 		}
 	Spi.vSpiWrite(((word)RegIrqFlags<<8)+AllIrqMask);	//Clear All Interrupt 		
 	vGoStandby();	
 	if(bittime>=5000)
 		return(false);
 	else
 		return(true);	
 	}
 else
 	{
 
 	//µÈ´ý·¢ÉäÍê±Ï
 	bittime  = SymbolTime/1000;		//unit: us
 	overtime = SyncLength+PreambleLength+length;
 	if(!FixedPktLength)				//SyncWord & PktLength & 2ByteCRC
		overtime += 1;
 	if(!CrcDisable)
 		overtime += 2;
 	overtime<<=3;						//8bit == 1byte
 	overtime*= bittime;
 	overtime/= 1000;					//unit: ms
 	if(overtime==0) 
 		overtime = 1;
 	_delay_ms(overtime);				//
 	for(tmp=0;tmp<100;tmp++)			//about 10ms for overtime
 		{
		if(DIO0_H())
			break; 	
		_delay_us(100);
 		}
 	vGoStandby();	
 	if(tmp>=100)
 		return(false);
 	else
 		return(true);
 	}
}

/**********************************************************
**Name:     bGetMessage
**Function: check receive packet
**Input:    msg------for which message to read
**Output:   packet length
			0--------have no packet
**********************************************************/
byte loraClass::bGetMessage(byte msg[])
{
 byte length;	
 if(DIO0_H())				//Receive CrcOk or PayloadReady
 	{
 	if(Modulation==LORA)
 		{
 		byte addr;	
 		Spi.bSpiRead(RegIrqFlags);						//Read Interrupt Flag for do something
 		
 		Spi.bSpiRead(RegPktSnrValue);
 		Spi.bSpiRead(RegRssiValue_LR);
 		Spi.bSpiRead(RegPktRssiValue);
 		
 		addr = Spi.bSpiRead(RegFifoRxCurrentAddr);			
 		Spi.vSpiWrite(((word)RegFifoAddrPtr<<8)+addr);
 		length = Spi.bSpiRead(RegRxNbBytes);		
 		Spi.vSpiBurstRead(RegFifo, msg, length);
 		Spi.vSpiWrite(((word)RegIrqFlags<<8)+AllIrqMask);	//Clear All Interrupt
 		}
 	else
 		{
 		if(FixedPktLength)
 			length = PayloadLength;
 		else
 			length = Spi.bSpiRead(RegFifo);
 		Spi.vSpiBurstRead(RegFifo, msg, length);
 		}
 	vGoStandby();
 	vGoRx();
 	return(length);
 	}
 return(0);
}

/**********************************************************
**Name:     vReset
**Function: hardware reset rf69 chipset
**Input:    none
**Output:   none
**********************************************************/
void loraClass::vReset(void)
{
 byte tmp;	
 POROut();
 switch(COB)
 	{
	case RFM92:						//High Reset; Normal for Low
	case RFM93:
		ClrPOR();
		_delay_us(200);				//at least 100us for reset
		SetPOR();
		break;
	case RFM95:
	case RFM96:
	case RFM97:
	case RFM98:
	default:
 		SetPOR();
 		_delay_us(200);				//at least 100us for reset
 		ClrPOR();
 		break;		
	}
 PORIn();							//set POR for free
 _delay_ms(6);						//wait for ready	
 ClrPOR();							//note: help for LowReset
 
 tmp = Spi.bSpiRead(RegOpMode);
 tmp&= MODE_MASK;
 tmp |= RADIO_SLEEP;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);

 tmp &= 0x7F; 
 if(Modulation==LORA)
	tmp |= 0x80;
 Spi.vSpiWrite(((word)RegOpMode<<8)+tmp);
}	

/**********************************************************
**Name:     bSelectBandwidth
**Function: 
**Input:    BandWidth
**Output:   BandWidthValue
**********************************************************/
byte loraClass::bSelectBandwidth(byte rx_bw)
{
 if(rx_bw<=10)
	return 0x15;						//10.4KHz 	Min
 else if(rx_bw<13)
 	return 0x0D;						//12.5KHz	
 else if(rx_bw<16)	
 	return 0x05;						//15.6KHz
 else if(rx_bw<21)		
 	return 0x14;						//20.8KHz
 else if(rx_bw<=25)	
 	return 0x0C;						//25.0KHz
 else if(rx_bw<32)
 	return 0x04;						//31.3KHz
 else if(rx_bw<42)
 	return 0x13;						//41.7KHz
 else if(rx_bw<=50)
 	return 0x0B;						//50.0KHz
 else if(rx_bw<63)
 	return 0x03;						//62.5KHz
 else if(rx_bw<84)
 	return 0x12;						//83.3KHz
 else if(rx_bw<=100)
 	return 0x0A;						//100KHz
 else if(rx_bw<=125)
 	return 0x02;						//125KHz
 else if(rx_bw<167)
 	return 0x11;						//167KHz
 else if(rx_bw<=200)
 	return 0x09;						//200KHz
 else if(rx_bw<=250)
 	return 0x01;						//250KHz
 else if(rx_bw<=333)
 	return 0x10;						//333KHz
 else if(rx_bw<=400)
 	return 0x08;						//400KHz
 else
 	return 0x00;						//500KHz Max
}	

/**********************************************************
**Name:     bSelectRamping
**Function: 
**Input:    symbol time
**Output:   ramping value
**********************************************************/
byte loraClass::bSelectRamping(lword symbol)
{
 lword SymbolRate;
 
 SymbolRate = symbol/1000;			//ns->us
 SymbolRate = SymbolRate/4;			// 1/4 ramping
 
 if(SymbolRate<=10)		
 	return 0x0F;					//10us
 else if(SymbolRate<=12)			
 	return 0x0E;					//12us
 else if(SymbolRate<=15)			
 	return 0x0D;					//15us
 else if(SymbolRate<=20)
 	return 0x0C;					//20us
 else if(SymbolRate<=25)
 	return 0x0B;					//25us
 else if(SymbolRate<=31)
 	return 0x0A;					//31us
 else if(SymbolRate<=40)
 	return 0x09;					//40us
 else if(SymbolRate<=50)
 	return 0x08;					//50us
 else if(SymbolRate<=62)
 	return 0x07;					//62us
 else if(SymbolRate<=100)
 	return 0x06;					//100us
 else if(SymbolRate<=125)
 	return 0x05;					//125us
 else if(SymbolRate<=250)
 	return 0x04;					//250us
 else if(SymbolRate<=500)
 	return 0x03;					//500us
 else if(SymbolRate<=1000)
	return 0x02;					//1000us
 else if(SymbolRate<=2000)
 	return 0x01;					//2000us
 else 
 	return 0x00;
}	
	
	
	