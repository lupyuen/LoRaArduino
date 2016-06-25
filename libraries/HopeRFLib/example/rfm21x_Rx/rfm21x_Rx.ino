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
 * file       rfm21x_rx.ino
 * hardware   HopeDuino with RFM21x
 * software   get message via rfm21x
 * note       
 *
 * version    1.0
 * date       Feb 19 2016
 * author     QY Ruan
 */
 
#include <HopeDuino_CMT221xA.h> 
#include <Hopeduino_UART.h>

cmt221xaClass radio;
uartClass uart;

byte getstr[21];
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
 if(radio.bGetMessage(getstr, 7)!=0)
    {
    uart.vUartPutNByte(getstr, 7);
    uart.vUartNewLine();
    for(i=0; i<7; i++)
        getstr[i] = 0x00;
    }  	 
}
