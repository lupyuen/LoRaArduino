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
 * file       rfm219_Rx.ino
 * hardware   HopeDuino 
 * software   rx message via rfm219
 * note       
 *
 * version    1.0
 * date       Feb 26 2016
 * author     QY Ruan
 */
 
#include <HopeDuino_CMT2219A.h> 
#include <HopeDuino_UART.h>

cmt2219aClass radio;
uartClass uart;

byte getstr[21];

byte CfgTbl[62] = {
					0x72,			// Mode                   = Advanced            
					0x21,           // Part Number            = CMT2219A            
					0x9B,           // Frequency              = 434.000 MHz         
					0x7F,           // Demodulation           = (G)FSK              
					0x06,           // Symbol Rate            = 2.4 ksps            
					0x63,           // Xtal Tolerance         = +/- 20 ppm          
					0x9A,           // Xtal Stabilizing Time  = 310 us              
					0x80,           // Squelch TH             = 0                   
					0xC6,           // Sleep Timer            = Off                 
					0x53,           // Sleep Time             = NA                  
					0x01,           // Rx Timer               = Off                 
					0x00,           // Rx Time                = NA                  
					0x62,           // Rx Time Ext            = NA                  
					0x1E,           // Rx Early Exit          = Off                 
					0x00,           // State After Rx Exit    = STBY                
					0x10,           // System Clock Output    = Off                 
					0x84,           // System Clock Frequency = NA                  
					0x14,           // Wake-On Radio          = Off                 
					0xE0,           // Wake-On Condition      = NA                  
					0x00,           // Demod Method           = NA                  
					0x27,           // Fixed Demod TH         = NA                  
					0x9F,           // Peak Drop              = NA                  
					0x00,           // Peak Drop Step         = NA                  
					0xD4,           // Peak Drop Rate         = NA                  
					0x2D,           // Deviation              = 35.0 kHz            
					0xAA,           // Sync Clock Type        = Counting            
					0x00,           // Data Representation    = 0:F-low  1:F-high   
					0x38,           // Rising Relative TH     = 21                  
					0x00,           // Falling Relative TH    = 255                 
					0x01,           // AFC                    = Off                 
					0x55,           // Data Mode              = Packet              
					0x21,           // Packet Type            = Fixed Length        
					0x07,           // FIFO Threshold         = 32                  
					0x84,           // De-Whitening Seed      = NA                  
					0x00,           // DC-Free Decode         = None                
					0x00,           // FILE CRC               = DB2F                
					0x19,                                                           
					0x00,
					0x00,
					0x00,
					0xAC,
					0x56,
					0x53,
					0xD4,
					0x40,
					0x49,
					0xFF,
					0x5D,
					0x12,
					0x00,
					0x90,
					0xFA,
					0x00,
					0x00,
					0x40,
					0xC0,
					0x00,
					0x00,
					0x20,
					0xEB,
					0x07,
					0x00
                 };

void setup()
{
 radio.CrcDisable     = true;
 radio.FixedPktLength = true;
 radio.NodeDisable    = true;
 radio.PktLength      = 21;
 radio.vInit(CfgTbl);
 radio.vGpioFuncCfg(GPIO1_INT1|GPIO2_DCLK|GPIO3_CLK|GPIO4_Dout);
 radio.vIntSourcCfg((FIFO_WBYTE+OFFSET), 0);
 radio.vEnableIntSource(0xFF);
 radio.vGoRx();
 uart.vUartInit(9600, _8N1);
}

void loop()
{
 if(radio.bGetMessage(getstr)!=0)
    {
    uart.vUartPutNByte(getstr, radio.PktLength);
    uart.vUartNewLine();
    }  	
}

