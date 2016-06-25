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
 * file       rfm21x_rx_1527.ino
 * hardware   HopeDuino with RFM21x & CMOSTEK CMT2150A-EM
 * software   get message via rfm21x
 * note       
 *
 * version    1.0
 * date       Mar 25 2016
 * author     QY Ruan
 */
 
#include <HopeDuino_CMT221xA.h> 
#include <Hopeduino_UART.h>

cmt221xaClass radio;
uartClass uart;

byte getstr[3];
byte sendstr[32];
byte length;
void setup()
{
 radio.Chipset        = CMT2210A;
 radio.SymbolTime     = 416;
 radio.Decode         = E527; 
 radio.vCMT221xAInit();
 uart.vUartInit(9600, _8N1);
}

void loop()
{
 byte i; 
 if(radio.bGetMessage(getstr, 3)!=0)
    {
    for(i=0; i<3; i++)
    	getstr[i] = bInverseByte(getstr[i]);
    vAnalysisMsg(getstr, sendstr); 
    uart.vUartPutNByte(sendstr, length);
    uart.vUartNewLine();
    for(i=0; i<3; i++)
        getstr[i] = 0x00;
    }  	 
}

void vAnalysisMsg(byte inptr[], byte outptr[])			//Analysis 1527 Code, assemble uart message 
{
 byte i, j;
 i = 0;
 outptr[i++] = 'I';
 outptr[i++] = 'D';
 outptr[i++] = ':';
 outptr[i++] = '0';
 outptr[i++] = 'x';
 for(j=0; j<2; j++)
   	{
   	outptr[i++] = bChangeToAscii(inptr[j]);	
   	outptr[i++] = bChangeToAscii((inptr[j]>>4));
   	}
 outptr[i++] = bChangeToAscii(inptr[j]);
 outptr[i++] = ' ';
 outptr[i++] = 'K';
 outptr[i++] = 'E';
 outptr[i++] = 'Y';
 outptr[i++] = ':';
 outptr[i++] = '0';
 outptr[i++] = 'x'; 
 outptr[i++] = bChangeToAscii((inptr[j]>>4));
 length = i;
}

byte bInverseByte(byte inver)						//Inverse buffer, just like 0x01 to 0x80
{
 byte i, j, z;
 z = 0;
 j = 0x80;
 for(i=0x01;i!=0;i<<=1)
 	{
   	if(inver&i)
		z |= j;
	j >>= 1;   
   	}
 return(z);
}


byte bChangeToAscii(byte ch)						//Change buffer to ASCII
{
 ch &= 0x0F;
 if(ch<0x0A)
 	ch += '0';
 else
 	{
 	ch -= 0x0A;
 	ch += 'A';
	}
 return(ch);
}

