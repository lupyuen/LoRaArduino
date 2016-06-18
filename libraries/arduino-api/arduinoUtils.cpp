/*
 *  Copyright (C) 2015 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:		1.4
 *  Design:			David Gascón
 *  Implementation:	Alberto Bielsa, David Cuartielles, Yuri Carmona,
 *                      Ruben Martin, Ahmad Saad
 */ 

  
#include <Wire.h>

#ifndef __WPROGRAM_H__
	#include <arduinoClasses.h>
#endif

#include <inttypes.h>
#include <arduinoUtils.h>
#include <arduinoMultiprotocol.h>

  //////////////////////////////////////////////////
  //Support for "Multiprotocol Radio Shield v1.0" //
  //////////////////////////////////////////////////

/* setMuxSocket0() - set multiplexer on UART_0 to SOCKET0
 *
*/
void arduinoUtils::setMuxSocket0()
{
  pinMode(MUX_SOCKET0_nSOCKET1,OUTPUT);
  digitalWrite(MUX_SOCKET0_nSOCKET1,LOW);
}

/* setMuxSocket1() - set multiplexer on UART_0 to SOCKET1
 *
*/
void arduinoUtils::setMuxSocket1()
{
  pinMode(MUX_SOCKET0_nSOCKET1,OUTPUT);
  digitalWrite(MUX_SOCKET0_nSOCKET1,HIGH);
}

/* setMuxSocketUSB() - set multiplexer on UART_0 to default
 *
*/
void arduinoUtils::setMuxUSB()
{
  pinMode(MUX_SOCKET0_nSOCKET1,OUTPUT);
  digitalWrite(MUX_SOCKET0_nSOCKET1,LOW);
}
/*
 * Constructor
 */
arduinoUtils::arduinoUtils (void)
{
}


  //////////////////////////////////////////////////
  //Support for "Multiprotocol Radio Shield v2.0" //
  //////////////////////////////////////////////////

/*
 * It converts a hexadecimal number stored in an array to a string (length is an 
 * input parameter). This function is used by the XBee module library in order to 
 * convert mac addresses
 * 
 */
void arduinoUtils::hex2str(uint8_t* number, char* macDest, uint8_t length)
{
	uint8_t aux_1=0;
	uint8_t aux_2=0;

	for(int i=0;i<length;i++)
	{
		aux_1=number[i]/16;
		aux_2=number[i]%16;
		if (aux_1<10)
		{
			macDest[2*i]=aux_1+'0';
		}
		else{
			macDest[2*i]=aux_1+('A'-10);
		}
		if (aux_2<10){
			macDest[2*i+1]=aux_2+'0';
		}
		else{
			macDest[2*i+1]=aux_2+('A'-10);
		}
	} 
	macDest[length*2]='\0';
}


//!*************************************************************
//!	Name: multiprotocolBegin()												
//!	Description: Initializes the I/O pins of theshield
//!	Param : void									
//!	Returns: void
//!*************************************************************
void arduinoUtils::multiprotocolBegin()
{
	mp.begin();
	delay(10);
	
	// I/O configuration
	mp.pinMode(0, OUTPUT);
	mp.pinMode(1, OUTPUT);
	mp.pinMode(2, OUTPUT);
	mp.pinMode(3, OUTPUT);
	mp.pinMode(4, OUTPUT);
	mp.pinMode(5, OUTPUT);
	mp.pinMode(6, INPUT);
	mp.pinMode(7, OUTPUT);
	delay(100);
	
	// Initial State
	mp.digitalWrite(0, LOW);
	mp.digitalWrite(1, LOW);
	mp.digitalWrite(2, LOW);
	mp.digitalWrite(3, HIGH);
	mp.digitalWrite(4, HIGH);
	mp.digitalWrite(5, LOW);
	//mp.digitalWrite(6, HIGH);
	mp.digitalWrite(7, HIGH);
	delay(100);
}


//!*************************************************************
//!	Name: disableMUX()											
//!	Description: Disables the multiplexor 
//!	Param : void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::disableMUX(void)
{
	mp.digitalWrite(4, HIGH);
}


//!*************************************************************
//!	Name: setONSocket0()												
//!	Description: Switches ON the power supply of the SOCKET0
//!	Param : void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::setONSocket0()
{
	if (Utils.socket1_state != 1)
		multiprotocolBegin();
	
	delay(5);
	mp.digitalWrite(1, HIGH);

	Utils.socket0_state = 1;
	delay(100);                     //wait for stablility
}


//!*************************************************************
//!	Name: setOFFSocket0()											
//!	Description: Switches OFF the power supply of the SOCKET0
//!	Param : void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::setOFFSocket0()
{
	mp.digitalWrite(1, LOW);
	Utils.socket0_state = 0;
	delay(100);                     //wait for stablility
}


//!*************************************************************
//!	Name: setMUXSocket0()											
//!	Description: Configures the multiplexor in the SOCKET0
//!	Param : void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::setMUXSocket0()
{
	mp.digitalWrite(4, LOW);
	mp.digitalWrite(5, LOW);
	delay(100);                     //wait for stablility
}


//!*************************************************************
//!	Name: setCSSocket0(void)
//!	Description: Enables the SPI of the SOCKET0 (3V3 Level)
//!	Param : void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::setCSSocket0()
{
	mp.digitalWrite(3, LOW);
	delay(3);
}


//!*************************************************************
//!	Name: unsetCSSocket0()
//!	Description: Disables the SPI of the SOCKET0 (3V3 Level)
//!	Param : void										
//!	Returns: void 
//!*************************************************************
void arduinoUtils::unsetCSSocket0()
{	
	mp.digitalWrite(3, HIGH);
	delay(2);
}


//!*************************************************************
//!	Name: setONSocket1()												
//!	Description: Switches ON the power suplly of the SOCKET1
//!	Param : void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::setONSocket1()
{
	
	if (Utils.socket0_state != 1)
		multiprotocolBegin();
	
	delay(5);
	mp.digitalWrite(0, HIGH);
	
	Utils.socket1_state = 1;
	delay(100);                     //wait for stablility
}


//!*************************************************************
//!	Name: setOFFSocket1()										
//!	Description: Switches OFF the power suplly of the SOCKET1 
//!	Param : void								
//!	Returns: void
//!*************************************************************
void arduinoUtils::setOFFSocket1()
{
	mp.digitalWrite(0, LOW);
	Utils.socket1_state = 0;
	delay(100);                     //wait for stablility
}
	
	
//!*************************************************************
//!	Name: setMUXSocket1(void)
//!	Description: Configures the multiplexor in the SOCKET1
//!	Param : void										
//!	Returns: void
//!*************************************************************	
void arduinoUtils::setMUXSocket1(void)
{
	mp.digitalWrite(4, LOW);
	mp.digitalWrite(5, HIGH);
	delay(100);                     //wait for stablility
}


//!*************************************************************
//!	Name: setCSSocket1()								
//!	Description: Enables the SPI of the SOCKET1 (5V Level)
//!	Param : void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::setCSSocket1(void)
{	
	mp.digitalWrite(2, LOW);
	delay(1);
}


//!*************************************************************
//!	Name: unsetCSSocket1()											
//!	Description: Disables the SPI of the SOCKET1
//!	Param: void										
//!	Returns: void
//!*************************************************************
void arduinoUtils::unsetCSSocket1(void)
{
	mp.digitalWrite(2, HIGH);
	delay(1);
}


//WakeUp default Class
arduinoUtils Utils = arduinoUtils();

