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
 * website: www.HopeRF.com
 *          www.HopeRF.cn
 *
 */

/*! 
 * file       press_hpxxx.cpp
 * brief      for HopeRF's EVB to use read press data
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.0
 * date       mar 09 2016
 * author     mjl
 */
/********************************************************
------ include file xxx.h
********************************************************/
#include <HopeHp203b_IIC.h>
#include <HopeHp206c_IIC.h>
#include <HopeTh06_IIC.h>
#include <HopeDuino_Disp.h>
#include <HopeDuino_LCD.h>
#include <HopeDuino_UART.h>
#include <stdio.h>
#include <avr/interrupt.h>

/********************************************************
------ consant define
*******************************************************/
#define HP20X_SOFT_RST    0x06
#define HP20X_ADDRESSCMD  0xec      //csb = VCC

#define HP20XX_RDY      0x8d

#define HP20X_READ_P      0x30      //read_p command
#define HP20X_READ_A      0x31      //read_a command
#define HP20X_READ_T      0x32      //read_t command
#define HP20X_READ_PT   0x10      //read_pt command
#define HP20X_READ_AT   0x11      //read_at command

#define HP20X_WR_CONVERT_CMD   0x40
#define HP20X_CONVERT_OSR4096  0<<2
#define HP20X_CONVERT_OSR2048  1<<2
#define HP20X_CONVERT_OSR1024  2<<2
#define HP20X_CONVERT_OSR512   3<<2
#define HP20X_CONVERT_OSR256   4<<2
#define HP20X_CONVERT_OSR128   5<<2

#define TH06_ADDRESSCMD		0x80
#define TH06_SOFT_RST		0xfe
#define TH06_USER_REG		0xe7
#define TH06_READ_HUMI		0xf5
#define TH06_READ_TEMP		0xf3

#define HP03S_ADDRESSCMD		0xee
#define HP03S_E_ADDRESSCMD		0xa0
#define HP03S_EEPROMADDR		0x10


/********************************************************
------ public define
*******************************************************/
Hp203bI2cClass hp203b;
Hp206cI2cClass	hp206c;
Th06I2cClass	th06;
dispClass	disp;

/********************************************************
------ void define
*******************************************************/
static void Th06ReadHumiTemperature(void);

static void Hp203bReadPressureTemperature(void);
static void Hp206cReadPressureTemperature(void);

static void ReadTemperaturePreesureAD(void);
static void MCLKOn(void);
static void MCLKOff(void);
static word IIC_ReadTempretureAD(void);
static word IIC_ReadPressureAD(void);
bool IIC_ReadCalData(void);

static void CalculatePressTemp(void);
static long int Get2_x(byte i);

static void Hex_to_Dec(unsigned long hexinput);
static void DispDataNop(void);

static void Delay_uMs(byte d_delay);

static void T2_CTC_Init(void);
static void T1_Ovf_init(void);

static void Save_Hp203bAvgAltitudeBuf(void);
static void Save_Hp203bAvgPressureBuf(void);
static void Save_Hp206cAvgAltitudeBuf(void);
static void Save_Hp206cAvgPressureBuf(void);

static void Key_Scaning(void);
static void KeyDo(void);

/********************************************************
------ var define
*******************************************************/
byte Rdy_Hp203b;
byte Rdy_Hp206c;
byte Rdy_Th06;

byte DispBuf[10];
byte Disp_Value;
byte Set_Value;
byte Disp_Auto;
byte Mode;
byte SetModeCnt;
byte AutoCnt;

word Osr_Value;
byte Osr_Cfg;
byte Osr_ConvertTime;

byte KeyTemp;
byte KeyCode;
byte KeyBounce;
byte F_DispAuto;

bool F_key;
bool F_KeyValume;
bool F_Tm2Flag;
bool F_KeyRelease;
bool F_HalfSec;
bool F_DispAvg;
bool F_Hp03sFlag;

word TimeCount;

word Hp03s_C1;
word Hp03s_C2;
word Hp03s_C3;
word Hp03s_C4;
word Hp03s_C5;
word Hp03s_C6;
word Hp03s_C7;
byte Hp03s_AA;
byte Hp03s_BB;
byte Hp03s_CC;
byte Hp03s_DD;

word Hp03s_D1 ;
word Hp03s_D2 ;

lword Hp03sTemperature;
lword Hp03sPressure;

lword	Hp203bPressure;
lword	Hp203bAvgPressure;
long	Hp203bTemperature;
long	Hp203bAltitude;
long	Hp203bAvgAltitude;
long	Hp203b_AltRel;
long	Hp203b_AltRelRef;

lword	Hp206cPressure;
lword	Hp206cAvgPressure;
long	Hp206cTemperature;
long	Hp206cAltitude;
long	Hp206cAvgAltitude;
long	Hp206c_AltRel;
long	Hp206c_AltRelRef;


lword Hp203bPressureBuf[32];
long Hp203bAltitudeBuf[32];
lword Hp206cPressureBuf[32];
long Hp206cAltitudeBuf[32];

long Th06Temp;
lword Th06Humi;

/********************************************************
------ table define
*******************************************************/
byte Disp_HP203B[] = {"HP203B: "};
byte Disp_HP206C[] = {"HP206C: "};
byte Disp_HP03S[]  = {"HP03S: "};
byte Disp_TH06[]   = {"TH06: "};

byte Disp_OSR[]    = {"OSR: "};
byte Disp_AUTO[]   = {"AUTO: "};
byte Disp_ON[]     = {"     ON "};
byte Disp_OFF[]    = {"     OFF "};

byte Disp_PRESS[]        = {"P: "};
byte Disp_TEMPERATURE[]  = {"T: "};
byte Disp_ALTITUDE[]     = {"A: "};
byte Disp_HUMI[]         = {"H: "};
byte Disp_ALTREL[]       = {"R: "};

byte Disp_PRESS_UNIT[]   = {" Hpa"};
byte Disp_TEMP_UNIT[]    = {" C"};
byte Disp_ALT_UNIT[]     = {" M"};
byte Disp_HUMI_UNIT[]    = {" %"};

/********************************************************
------ initialize: hp203b,hp206c,th02
------
------
********************************************************/
void setup()
{
	byte res;
	
	MCUCR &= (~PUD);
	hp203b.HP203B_vIICInit();
	hp206c.HP206c_vIICInit();
	th06.Th06_vIICInit();

	Osr_Value = 1024;
	Osr_Cfg = HP20X_CONVERT_OSR1024;
	Osr_ConvertTime = 16;
	Disp_Value = 0;

	hp203b.HP203B_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_SOFT_RST);
	hp206c.HP206C_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_SOFT_RST);
	th06.Th06_bIICWriteSingle(TH06_ADDRESSCMD,TH06_SOFT_RST);
	_delay_ms(10);

	hp203b.HP203B_bIICWriteSingle(HP20X_ADDRESSCMD,0x8d);
	Rdy_Hp203b = hp203b.HP203B_bIICReadSingle(HP20X_ADDRESSCMD);
	if(Rdy_Hp203b==0x40)
	{
		Disp_Value = 1;
		Hp203bReadPressureTemperature();
		Hp203bAvgPressure = Hp203bPressure;
		Hp203bAvgAltitude = Hp203bAltitude;
		Hp203b_AltRelRef = Hp203bAltitude;
		for(byte i=0;i<32;i++)
		{
			Hp203bAltitudeBuf[i] = Hp203bAltitude;
			Hp203bPressureBuf[i] = Hp203bPressure;
		}
	}
	hp206c.HP206C_bIICWriteSingle(HP20X_ADDRESSCMD,0x8d);
	Rdy_Hp206c = hp206c.HP206C_bIICReadSingle(HP20X_ADDRESSCMD);
	if(Rdy_Hp206c==0x40)
	{
		Disp_Value = 2;
		Hp206cReadPressureTemperature();
		Hp206cAvgPressure = Hp206cPressure;
		Hp206cAvgAltitude = Hp206cAltitude;
		Hp206c_AltRelRef = Hp206cAltitude;
		for(byte i=0;i<32;i++)
		{
			Hp206cAltitudeBuf[i] = Hp206cAltitude;
			Hp206cPressureBuf[i] = Hp206cPressure;
		}
	}
	th06.Th06_bIICWriteSingle(TH06_ADDRESSCMD,TH06_USER_REG);
	Rdy_Th06 = th06.Th06_bIICReadSingle(TH06_ADDRESSCMD);
	if(Rdy_Th06==0x3a)
	{
		Disp_Value = 3;
		Th06ReadHumiTemperature();
	}
	F_Hp03sFlag = 0;
	res = IIC_ReadCalData();
	if(res)
	{
		F_Hp03sFlag = 1;
		Disp_Value = 3;
	}
	T2_CTC_Init();

	disp.vDispInit();
	disp.vDispOn();
	disp.vDispClear();
	disp.vDispLight(0x01);
	disp.vDisHopeRFLogo(0x03,0x01);
	_delay_ms(500);
	Mode = 0;
	F_DispAuto = 0;
	Disp_Auto = 0;
	sei();
}

/********************************************************
------
------
------
********************************************************/
void loop()
{
	if(F_Tm2Flag)
	{
		F_Tm2Flag = 0;
		Key_Scaning();
		KeyDo();
	}
	if(F_HalfSec)
	{
		F_HalfSec = 0;
		if(Mode==0)
		{
			switch(Disp_Auto)
 			{
 				case 0:
					if(Rdy_Hp203b==0x40)
					{
						Hp203bReadPressureTemperature();
						Save_Hp203bAvgPressureBuf();
						Save_Hp203bAvgAltitudeBuf();
					}
					break;
				case 1:
					if(Rdy_Hp206c==0x40)
					{
						Hp206cReadPressureTemperature();
						Save_Hp206cAvgPressureBuf();
						Save_Hp206cAvgAltitudeBuf();
					}
					break;
				case 2:
					if(Rdy_Th06==0x3a)
					{
						Th06ReadHumiTemperature();
						ReadTemperaturePreesureAD();
						CalculatePressTemp();
					}
					break;
				case 3:
 					switch(Set_Value)
 					{
 						case 1:
							break;
 						default:
 							break;
 					}
 					break;
				default:
 					break;
 			}
		}
		disp.vDispClear();
		switch(Mode)
		{
			case 0:
				switch(Disp_Auto)
				{
					case 0:
						disp.vDispString8x8(0x01,0x01,Disp_HP203B);
						disp.vDispString8x8(0x03,0x01,Disp_PRESS);
						disp.vDispString8x8(0x04,0x01,Disp_TEMPERATURE);
						disp.vDispString8x8(0x05,0x01,Disp_ALTITUDE);
						disp.vDispString8x8(0x06,0x01,Disp_ALTREL);
						if(Rdy_Hp203b==0x40)
						{
							Hex_to_Dec(Hp203bTemperature);
							disp.vDispString8x8(0x04,24,DispBuf);
							disp.vDispString8x8(0x04,88,Disp_TEMP_UNIT);
							if(F_DispAvg)
							{
								Hex_to_Dec(Hp203bPressure);
								disp.vDispString8x8(0x03,24,DispBuf);
								disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
								Hex_to_Dec(Hp203bAltitude);
								disp.vDispString8x8(0x05,24,DispBuf);
								disp.vDispString8x8(0x05,88,Disp_ALT_UNIT);
								Hp203b_AltRel = Hp203bAltitude - Hp203b_AltRelRef;
							}
							else
							{
								Hex_to_Dec(Hp203bAvgPressure);
								disp.vDispString8x8(0x03,24,DispBuf);
								disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
								Hex_to_Dec(Hp203bAvgAltitude);
								disp.vDispString8x8(0x05,24,DispBuf);
								disp.vDispString8x8(0x05,88,Disp_ALT_UNIT);
								Hp203b_AltRel = Hp203bAvgAltitude - Hp203b_AltRelRef;
							}
							Hex_to_Dec(Hp203b_AltRel);
							disp.vDispString8x8(0x06,24,DispBuf);
							disp.vDispString8x8(0x06,88,Disp_ALT_UNIT);
						}
						else
						{
							DispDataNop();
							disp.vDispString8x8(0x03,24,DispBuf);
							disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
							disp.vDispString8x8(0x04,24,DispBuf);
							disp.vDispString8x8(0x04,88,Disp_TEMP_UNIT);
							disp.vDispString8x8(0x05,24,DispBuf);
							disp.vDispString8x8(0x05,88,Disp_ALT_UNIT);
							disp.vDispString8x8(0x06,24,DispBuf);
							disp.vDispString8x8(0x06,88,Disp_ALT_UNIT);
						}
						break;
					case 1:
						disp.vDispString8x8(0x01,0x01,Disp_HP206C);
						disp.vDispString8x8(0x03,0x01,Disp_PRESS);
						disp.vDispString8x8(0x04,0x01,Disp_TEMPERATURE);
						disp.vDispString8x8(0x05,0x01,Disp_ALTITUDE);
						disp.vDispString8x8(0x06,0x01,Disp_ALTREL);
						if(Rdy_Hp206c==0x40)
						{
							Hex_to_Dec(Hp206cTemperature);
							disp.vDispString8x8(0x04,24,DispBuf);
							disp.vDispString8x8(0x04,88,Disp_TEMP_UNIT);
							if(F_DispAvg)
							{
								Hex_to_Dec(Hp206cPressure);
								disp.vDispString8x8(0x03,24,DispBuf);
								disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
								Hex_to_Dec(Hp206cAltitude);
								disp.vDispString8x8(0x05,24,DispBuf);
								disp.vDispString8x8(0x05,88,Disp_ALT_UNIT);
								Hp206c_AltRel = Hp206cAltitude - Hp206c_AltRelRef;
							}
							else
							{
								Hex_to_Dec(Hp206cAvgPressure);
								disp.vDispString8x8(0x03,24,DispBuf);
								disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
								Hex_to_Dec(Hp206cAvgAltitude);
								disp.vDispString8x8(0x05,24,DispBuf);
								disp.vDispString8x8(0x05,88,Disp_ALT_UNIT);
								Hp206c_AltRel = Hp206cAvgAltitude - Hp206c_AltRelRef;
							}
							Hex_to_Dec(Hp206c_AltRel);
							disp.vDispString8x8(0x06,24,DispBuf);
							disp.vDispString8x8(0x06,88,Disp_ALT_UNIT);
						}
						else
						{
							DispDataNop();
							disp.vDispString8x8(0x03,24,DispBuf);
							disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
							disp.vDispString8x8(0x04,24,DispBuf);
							disp.vDispString8x8(0x04,88,Disp_TEMP_UNIT);
							disp.vDispString8x8(0x05,24,DispBuf);
							disp.vDispString8x8(0x05,88,Disp_ALT_UNIT);
							disp.vDispString8x8(0x06,24,DispBuf);
							disp.vDispString8x8(0x06,88,Disp_ALT_UNIT);
						}
						break;
					case 2:
						disp.vDispString8x8(0x01,0x01,Disp_HP03S);
						disp.vDispString8x8(0x03,0x01,Disp_PRESS);
						disp.vDispString8x8(0x04,0x01,Disp_TEMPERATURE);
						disp.vDispString8x8(0x06,0x01,Disp_TH06);
						disp.vDispString8x8(0x07,0x01,Disp_TEMPERATURE);
						disp.vDispString8x8(0x08,0x01,Disp_HUMI);
						if(Rdy_Th06==0x3a)
						{
							if(F_Hp03sFlag)
							{
								Hex_to_Dec(Hp03sPressure);
								disp.vDispString8x8(0x03,24,DispBuf);
								disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
								Hex_to_Dec(Hp03sTemperature);
								disp.vDispString8x8(0x04,24,DispBuf);
								disp.vDispString8x8(0x04,88,Disp_TEMP_UNIT);
							}
							else
							{
								DispDataNop();
								disp.vDispString8x8(0x03,24,DispBuf);
								disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
								disp.vDispString8x8(0x04,24,DispBuf);
								disp.vDispString8x8(0x04,88,Disp_TEMP_UNIT);
							}
							Hex_to_Dec(Th06Temp);
							disp.vDispString8x8(0x07,24,DispBuf);
							disp.vDispString8x8(0x07,88,Disp_TEMP_UNIT);
							Hex_to_Dec(Th06Humi);
							disp.vDispString8x8(0x08,24,DispBuf);
							disp.vDispString8x8(0x08,88,Disp_HUMI_UNIT);
						}
						else
						{
							DispDataNop();
							disp.vDispString8x8(0x03,24,DispBuf);
							disp.vDispString8x8(0x03,88,Disp_PRESS_UNIT);
							disp.vDispString8x8(0x04,24,DispBuf);
							disp.vDispString8x8(0x04,88,Disp_TEMP_UNIT);
							disp.vDispString8x8(0x07,24,DispBuf);
							disp.vDispString8x8(0x07,88,Disp_TEMP_UNIT);
							disp.vDispString8x8(0x08,24,DispBuf);
							disp.vDispString8x8(0x08,88,Disp_HUMI_UNIT);
						}
						break;
				}
				break;
			case 1:
				disp.vDispString8x8(0x01,0x01,Disp_OSR);
				Hex_to_Dec(Osr_Value);
				disp.vDispString8x8(0x03,32,DispBuf);
				break;
			case 2:
				disp.vDispString8x8(0x01,0x01,Disp_AUTO);
				if(F_DispAuto)
				{
					disp.vDispString8x8(0x03,0x01,Disp_ON);
				}
				else
				{
					disp.vDispString8x8(0x03,0x01,Disp_OFF);
				}
				break;
		}
	}
}

/**********************************************************
**Name:     Key_Scaning
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Key_Scaning(void)
{
	byte key_buf;
	byte temp;

	temp |= (PINC&0x30)^0x30;
	temp = temp>>4;
	key_buf |= (PIND&0x30)^0x30;
	key_buf |= temp;
	switch(key_buf)
	{
		case 0x01:
		case 0x02:
		case 0x10:
		case 0x20:
			KeyTemp = key_buf;
			break;
		default:
			KeyTemp = 0;
			break;
	}
	if(KeyTemp == 0)
	{
		KeyCode = 0;
		KeyBounce = 0;
		F_key = 0;
		F_KeyValume = 0;
	}
	else
	{
		if(F_key==0)
		{
			KeyBounce ++;
			if(KeyBounce > 5)
			{
				KeyCode = KeyTemp;
				KeyBounce = 0;
				F_key = 1;
			}
		}
	}
}

/**********************************************************
**Name:     KeyDo
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void KeyDo(void)
{
	if(F_key == 0) return;
	if(F_KeyValume == 1) return;
	F_KeyValume = 1;
	switch(KeyCode)
	{
		case 0x10:
			SetModeCnt = 10;
			Mode++;
			if(Mode>2) Mode = 0;
			break;
		case 0x20:
			Mode = 0;
			F_DispAuto = 0;
			Disp_Auto++;
			if(Disp_Auto>2) Disp_Auto = 0;
			break;
		case 0x01:
			Mode = 0;
			Disp_Auto = 0;
			Hp203b_AltRelRef = Hp203bAvgAltitude;
			Hp206c_AltRelRef = Hp206cAvgAltitude;
			break;
		case 0x02:
			switch(Mode)
			{
				case 1:
					switch(Osr_Value)
					{
						case 128:
							Osr_Value = 256;
							Osr_Cfg = HP20X_CONVERT_OSR256;
							Osr_ConvertTime = 4;
							break;
						case 256:
							Osr_Value = 512;
							Osr_Cfg = HP20X_CONVERT_OSR512;
							Osr_ConvertTime = 8;
							break;
						case 512:
							Osr_Value = 1024;
							Osr_Cfg = HP20X_CONVERT_OSR1024;
							Osr_ConvertTime = 16;
							break;
						case 1024:
							Osr_Value = 2048;
							Osr_Cfg = HP20X_CONVERT_OSR2048;
							Osr_ConvertTime = 32;
							break;
						case 2048:
							Osr_Value = 4096;
							Osr_Cfg = HP20X_CONVERT_OSR4096;
							Osr_ConvertTime = 64;
							break;
						case 4096:
							Osr_Value = 128;
							Osr_Cfg = HP20X_CONVERT_OSR128;
							Osr_ConvertTime = 2;
							break;
					}
					SetModeCnt = 10;
					break;
				case 2:
					F_DispAuto = ~F_DispAuto; 
					SetModeCnt = 10;
					AutoCnt = 0;
					break;
			}
			break;
	}
	TimeCount = 0;
	F_HalfSec = 1;
	KeyCode = 0;
	KeyBounce = 0;
}


/**********************************************************
**Name:     T1_Ovf_init
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void T1_Ovf_init(void)
{
	TCCR1B = 0x00;
	TCCR1A = 0x00;
	TCCR1B = 001;
	TCNT1 = 0xc2f6;
	TIMSK1 = 0x01;
}
/**********************************************************
**Name:     T0_CTC_Init
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void T2_CTC_Init(void)
{
	TCCR2B = 0x00;
	TCCR2A = 0x00;
	TCCR2B = 7;
	TCNT2 = 0xc2f6;
	TIMSK2 = 0x01;
}
/**********************************************************
**Name:     TIMER0_OVF_vect
**Function: 10ms
**Input:    none
**Output:   none
**********************************************************/
ISR(TIMER2_OVF_vect)
{
	TCNT2 = 100;
	TimeCount++;
	F_Tm2Flag = 1;
	if(TimeCount >= 100)
	{
		TimeCount = 0;
		F_HalfSec = 1;
		if(SetModeCnt)
		{
			SetModeCnt--;
			if(SetModeCnt == 0)
			{
				Mode = 0;
			}
		}
		if(F_DispAuto)
		{
			AutoCnt++;
			if(AutoCnt >= 5)
			{
				AutoCnt = 0;
				Disp_Auto++;
				if(Disp_Auto>2) Disp_Auto = 0;
			}
		}
	}
} 

/**********************************************************
**Name:     TIMER1_OVF_vect
**Function: 
**Input:    none
**Output:   none
**********************************************************/
ISR(TIMER1_OVF_vect)
{
	TCNT1 = 0xff08;
	if(Hp03sMCLK_H())
		ClrHp03sMCLK();
	else
		SetHp03sMCLK();
}
/**********************************************************
**Name:     ReadTemperaturePreesureAD
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void ReadTemperaturePreesureAD(void)
{
	MCLKOn();
	Hp03s_D1 = IIC_ReadPressureAD();
	Hp03s_D2 = IIC_ReadTempretureAD();
	MCLKOff();
}

/**********************************************************
**Name:     MCLKOn
**Function: 		HP03S_XCLR = PB.1;	HP03S_MCLK = PB.5
**Input:    none
**Output:   none
**********************************************************/
static void MCLKOn(void)
{
	DDRD |= ((1<<DDD6)|(1<<DDD7));
	T1_Ovf_init();
 	SetHp03sXCLR();
 	ClrHp03sMCLK();
 	_delay_ms(2);
}

/**********************************************************
**Name:     MCLKOff
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void MCLKOff(void)
{
	TCCR1A = 0x00;
	TCCR1B = 000;
	TCNT1 = 0x0;
	TIMSK1 = 0x00;
	ClrHp03sXCLR();
	ClrHp03sMCLK();
}

/**********************************************************
**Name:     IIC_ReadTempretureAD
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static word IIC_ReadTempretureAD(void)
{ 
	byte DataBuf[2];
	word wADT;

	DataBuf[0] = 0xe8;
	hp206c.HP206C_bIICBurstWrite(HP03S_ADDRESSCMD,0xff,DataBuf,1);
	_delay_ms(40);

	hp206c.HP206C_bIICWriteSingle(HP03S_ADDRESSCMD,0xfd);
	hp206c.HP206C_bIICReadSeveral(HP03S_ADDRESSCMD,DataBuf,2);
	wADT = DataBuf[0];
	wADT <<= 8;
	wADT |= DataBuf[1];
	
	return wADT;
}

/**********************************************************
**Name:     IIC_ReadPressureAD
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static word IIC_ReadPressureAD(void)
{ 
	byte DataBuf[2];
	word wADT;

	DataBuf[0] = 0xf0;
	hp206c.HP206C_bIICBurstWrite(HP03S_ADDRESSCMD,0xff,DataBuf,1);
	_delay_ms(40);

	hp206c.HP206C_bIICWriteSingle(HP03S_ADDRESSCMD,0xfd);
	hp206c.HP206C_bIICReadSeveral(HP03S_ADDRESSCMD,DataBuf,2);
	wADT = DataBuf[0];
	wADT <<= 8;
	wADT |= DataBuf[1];
	
	return wADT;
}

/**********************************************************
**Name:     CalculatePressTemp
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void CalculatePressTemp(void)
{
	float MiddleData1;
	float MiddleData2;
	float MiddleData3;
	float MiddleData4;
	float DUT;
	float OFF;
	float SENS;
	float X;

	MiddleData1 = (long)Hp03s_D2-Hp03s_C5;
	MiddleData2 = MiddleData1/128;
	MiddleData2 = MiddleData2*MiddleData2;
	MiddleData3 = Get2_x(Hp03s_CC);
	MiddleData3 = MiddleData2/MiddleData3;
	if(Hp03s_D2 < Hp03s_C5)
	{
		MiddleData4 = MiddleData3*Hp03s_BB;
	}
	else
	{
		MiddleData4 = MiddleData3*Hp03s_AA;
	}
	DUT = MiddleData1 - MiddleData4;
	MiddleData1 = Get2_x(14);
	MiddleData2 = DUT/MiddleData1;
	MiddleData3 = (long)Hp03s_C4-1024;
	MiddleData4 = MiddleData3*MiddleData2;
	MiddleData1 = (long)Hp03s_C2+MiddleData4;
	OFF = MiddleData1*4;
	MiddleData1 = Get2_x(10);
	MiddleData2 = DUT/MiddleData1;
	MiddleData3 = (long)Hp03s_C3*MiddleData2;
	SENS = (long)Hp03s_C1+MiddleData3;
	MiddleData1 = (long)Hp03s_D1-7168;
	MiddleData2 = Get2_x(14);
	MiddleData3 = MiddleData1/MiddleData2;
	MiddleData4 = MiddleData3*SENS;
	X = MiddleData4-OFF;
	MiddleData1 = X*0.3125;
	MiddleData2 = (long)Hp03s_C7+MiddleData1;
	MiddleData2	= MiddleData2*10;
	Hp03sPressure = (unsigned long)MiddleData2;

	MiddleData1 = DUT*Hp03s_C6/Get2_x(16)-DUT/Get2_x(Hp03s_DD);
	MiddleData1 = 250 + MiddleData1;
	MiddleData1 = MiddleData1*10;
	Hp03sTemperature = (unsigned long)MiddleData1;
}

/***************************************************
**function:calculate powl for 2
**input   :
**output  :
***************************************************/
static long int Get2_x(byte i)
{
	long int uiData;

	uiData=2;
	i=i-1;
	while(i)
	{
		uiData <<= 1;
		i--;
	}
	return uiData;
}
/**********************************************************
**Name:     IIC_ReadCalData
**Function: 
**Input:    none
**Output:   none
**********************************************************/
bool IIC_ReadCalData(void)
{
	byte DataBuf[19];
	bool res;

	res = hp206c.HP206C_bIICWriteSingle(HP03S_E_ADDRESSCMD,HP03S_EEPROMADDR);
	if(res == 0)
	{
		return(false);
	}
	res = hp206c.HP206C_bIICReadSeveral(HP03S_E_ADDRESSCMD,DataBuf,18);
	if(res == 0)
	{
		return(false);
	}
	Hp03s_C1 = DataBuf[0];
	Hp03s_C1 <<= 8;
	Hp03s_C1 |= DataBuf[1];
	
	Hp03s_C2 = DataBuf[2];
	Hp03s_C2 <<= 8;
	Hp03s_C2 |= DataBuf[3];
	
	Hp03s_C3 = DataBuf[4];
	Hp03s_C3 <<= 8;
	Hp03s_C3 |= DataBuf[5];
	
	Hp03s_C4 = DataBuf[6];
	Hp03s_C4 <<= 8;
	Hp03s_C4 |= DataBuf[7];
	
	Hp03s_C5 = DataBuf[8];
	Hp03s_C5 <<= 8;
	Hp03s_C5 |= DataBuf[9];
	
	Hp03s_C6 = DataBuf[10];
	Hp03s_C6 <<= 8;
	Hp03s_C6 |= DataBuf[11];
	
	Hp03s_C7 = DataBuf[12];
	Hp03s_C7 <<= 8;
	Hp03s_C7 |= DataBuf[13];
	
	Hp03s_AA = DataBuf[14];
	Hp03s_BB = DataBuf[15];
	Hp03s_CC = DataBuf[16];
	Hp03s_DD = DataBuf[17];

	return(true);
}
/**********************************************************
**Name:     TimeDelay
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Delay_uMs(byte d_delay)
{
	byte i;

	for(i=0;i<d_delay;i++)
	{
		_delay_us(1330);
	}
}

/**********************************************************
**Name:     Th06ReadHumiTemperature
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Th06ReadHumiTemperature(void)
{
	byte DataBuf[2];

	th06.Th06_bIICWriteSingle(TH06_ADDRESSCMD,TH06_READ_HUMI);
	_delay_ms(20);

	th06.Th06_bIICReadSeveral(TH06_ADDRESSCMD,DataBuf,2);
	Th06Humi = DataBuf[0];
	Th06Humi <<= 8;
	Th06Humi |= DataBuf[1];
	Th06Humi *= 15625;
	Th06Humi >>= 13;
	Th06Humi -= 6000;
	Th06Humi /= 10;

	th06.Th06_bIICWriteSingle(TH06_ADDRESSCMD,TH06_READ_TEMP);
	_delay_ms(20);
	th06.Th06_bIICReadSeveral(TH06_ADDRESSCMD,DataBuf,2);
	Th06Temp = DataBuf[0];
	Th06Temp <<= 8;
	Th06Temp |= DataBuf[1];
	Th06Temp *= 21965;
	Th06Temp >>= 13;
	Th06Temp /= 10;
	if(Th06Temp >= 4685)
	{
		Th06Temp -= 4685;
	}
	else
	{
		Th06Temp = 4685-Th06Temp;
	}
}

/**********************************************************
**Name:     Hp203bReadPressureTemperature
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Hp203bReadPressureTemperature(void)
{
  byte DataBuf[6];

	hp203b.HP203B_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_WR_CONVERT_CMD|Osr_Cfg);
	Delay_uMs(Osr_ConvertTime*2);
  
	hp203b.HP203B_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_READ_PT);
	hp203b.HP203B_bIICReadSeveral(HP20X_ADDRESSCMD,DataBuf,6);
	Hp203bTemperature = DataBuf[0];
	Hp203bTemperature <<= 8;
	Hp203bTemperature |= DataBuf[1];
	Hp203bTemperature <<= 8;
	Hp203bTemperature |= DataBuf[2];
	if(Hp203bTemperature & 0x800000)
		Hp203bTemperature |= 0xff000000;
  
  Hp203bPressure = DataBuf[3];
  Hp203bPressure <<= 8;
  Hp203bPressure |= DataBuf[4];
  Hp203bPressure <<= 8;
  Hp203bPressure |= DataBuf[5];
  
  hp203b.HP203B_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_READ_A);
  hp203b.HP203B_bIICReadSeveral(HP20X_ADDRESSCMD,DataBuf,3);
  Hp203bAltitude = DataBuf[0];
  Hp203bAltitude <<= 8;
  Hp203bAltitude |= DataBuf[1];
  Hp203bAltitude <<= 8;
  Hp203bAltitude |= DataBuf[2];
  if(Hp203bAltitude & 0x800000)
    Hp203bAltitude |= 0xff000000;
}

/**********************************************************
**Name:     Hp206cReadPressureTemperature
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Hp206cReadPressureTemperature(void)
{
  byte DataBuf[6];

	hp206c.HP206C_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_WR_CONVERT_CMD|Osr_Cfg);
	Delay_uMs(Osr_ConvertTime*2);
  
  hp206c.HP206C_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_READ_PT);
  hp206c.HP206C_bIICReadSeveral(HP20X_ADDRESSCMD,DataBuf,6);
  Hp206cTemperature = DataBuf[0];
  Hp206cTemperature <<= 8;
  Hp206cTemperature |= DataBuf[1];
  Hp206cTemperature <<= 8;
  Hp206cTemperature |= DataBuf[2];
  if(Hp206cTemperature & 0x800000)
    Hp206cTemperature |= 0xff000000;
  
  Hp206cPressure = DataBuf[3];
  Hp206cPressure <<= 8;
  Hp206cPressure |= DataBuf[4];
  Hp206cPressure <<= 8;
  Hp206cPressure |= DataBuf[5];
  
  hp206c.HP206C_bIICWriteSingle(HP20X_ADDRESSCMD,HP20X_READ_A);
  hp206c.HP206C_bIICReadSeveral(HP20X_ADDRESSCMD,DataBuf,3);
  Hp206cAltitude = DataBuf[0];
  Hp206cAltitude <<= 8;
  Hp206cAltitude |= DataBuf[1];
  Hp206cAltitude <<= 8;
  Hp206cAltitude |= DataBuf[2];
  if(Hp206cAltitude & 0x800000)
    Hp206cAltitude |= 0xff000000;
}

/**********************************************************
**Name:     Save_Hp203bAvgAltitudeBuf
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Save_Hp203bAvgAltitudeBuf(void)
{
	byte i;
	byte j = 1;

	Hp203bAvgAltitude = 0;
	for(i=0;i<32;i++)
	{
		Hp203bAltitudeBuf[i] = Hp203bAltitudeBuf[j];
		j++;	
	}
	Hp203bAltitudeBuf[31] = Hp203bAltitude;
	for(i=0;i<32;i++)
	{
		Hp203bAvgAltitude += Hp203bAltitudeBuf[i];
	}
	Hp203bAvgAltitude = Hp203bAvgAltitude/32;
}

/**********************************************************
**Name:     Save_Hp203bAvgPressureBuf
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Save_Hp203bAvgPressureBuf(void)
{
	byte i;
	byte j = 1;

	Hp203bAvgPressure = 0;
	for(i=0;i<32;i++)
	{
		Hp203bPressureBuf[i] = Hp203bPressureBuf[j];
		j++;	
	}
	Hp203bPressureBuf[31] = Hp203bPressure;
	for(i=0;i<32;i++)
	{
		Hp203bAvgPressure += Hp203bPressureBuf[i];
	}
	Hp203bAvgPressure = Hp203bAvgPressure/32;
}

/**********************************************************
**Name:     Save_Hp206cAvgAltitudeBuf
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Save_Hp206cAvgAltitudeBuf(void)
{
	byte i;
	byte j = 1;

	Hp206cAvgAltitude = 0;
	for(i=0;i<32;i++)
	{
		Hp206cAltitudeBuf[i] = Hp206cAltitudeBuf[j];
		j++;	
	}
	Hp206cAltitudeBuf[31] = Hp206cAltitude;
	for(i=0;i<32;i++)
	{
		Hp206cAvgAltitude += Hp206cAltitudeBuf[i];
	}
	Hp206cAvgAltitude = Hp206cAvgAltitude/32;
}

/**********************************************************
**Name:     Save_Hp206cAvgPressureBuf
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Save_Hp206cAvgPressureBuf(void)
{
	byte i;
	byte j = 1;

	Hp206cAvgPressure = 0;
	for(i=0;i<32;i++)
	{
		Hp206cPressureBuf[i] = Hp206cPressureBuf[j];
		j++;	
	}
	Hp206cPressureBuf[31] = Hp206cPressure;
	for(i=0;i<32;i++)
	{
		Hp206cAvgPressure += Hp206cPressureBuf[i];
	}
	Hp206cAvgPressure = Hp206cAvgPressure/32;
}

/**********************************************************
**Name:     Hex_to_Dec
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void DispDataNop(void)
{
	byte i;

	for(i=0;i<9;i++)
	{
		DispBuf[i] = 0x2d;
	}
}
/**********************************************************
**Name:     Hex_to_Dec
**Function: 
**Input:    none
**Output:   none
**********************************************************/
static void Hex_to_Dec(unsigned long hexinput)
{
	byte i;
	byte F_PlusFlag;
	unsigned long Disp_Dec_code;

	Disp_Dec_code = hexinput;
	F_PlusFlag = 0;
	if(Disp_Dec_code & 0x80000000)
	{
		F_PlusFlag = 1;
		Disp_Dec_code = ~Disp_Dec_code+1;
	}
	Disp_Dec_code = Disp_Dec_code&0x00ffffff;
	for(i=0;i<9;i++)
	{
		DispBuf[i] = 0x30;
	}
	while(Disp_Dec_code > 999999)
	{
		Disp_Dec_code = Disp_Dec_code-1000000;
		DispBuf[0]++;
	}
	while(Disp_Dec_code > 99999)
	{
		Disp_Dec_code = Disp_Dec_code-100000;
		DispBuf[1]++;
	}
	while(Disp_Dec_code > 9999)
	{
		Disp_Dec_code = Disp_Dec_code-10000;
		DispBuf[2]++;
	}
	while(Disp_Dec_code > 999)
	{
		Disp_Dec_code = Disp_Dec_code-1000;
		DispBuf[3]++;
	}
	while(Disp_Dec_code > 99)
	{
		Disp_Dec_code = Disp_Dec_code-100;
		DispBuf[4]++;
	}
	if(Mode==1)
	{
		while(Disp_Dec_code > 9)
		{
			Disp_Dec_code = Disp_Dec_code-10;
			DispBuf[5]++;
		}
		DispBuf[6] += Disp_Dec_code;
		DispBuf[7] = 0x20;
		DispBuf[8] = 0x20;
		if(DispBuf[0] == 0x30)
		{
			DispBuf[0] = 0x20;
			if(DispBuf[1] == 0x30)
			{
				DispBuf[1] = 0x20;
				if(DispBuf[2] == 0x30)
				{
					DispBuf[2] = 0x20;
					if(DispBuf[3] == 0x30)
					{
						DispBuf[3] = 0x20;
					}
				}
			}
		}
	}
	else
	{
		while(Disp_Dec_code > 9)
		{
			Disp_Dec_code = Disp_Dec_code-10;
			DispBuf[6]++;
		}
		DispBuf[7] += Disp_Dec_code;
		DispBuf[5] = 0x2e;
		if(DispBuf[0] == 0x30)
		{
			DispBuf[0] = 0x20;
			if(DispBuf[1] == 0x30)
			{
				DispBuf[1] = 0x20;
				if(DispBuf[2] == 0x30)
				{
					DispBuf[2] = 0x20;
					if(DispBuf[3] == 0x30)
					{
						DispBuf[3] = 0x20;
					}
					else
					{
						if(F_PlusFlag)	DispBuf[2] = 0x2d;
					}
				}
				else
				{
					if(F_PlusFlag)	DispBuf[1] = 0x2d;
				}
			}
			else
			{
				if(F_PlusFlag)	DispBuf[0] = 0x2d;
			}
		}
	}
}
	
/********************************************************
------  read press end
********************************************************/
