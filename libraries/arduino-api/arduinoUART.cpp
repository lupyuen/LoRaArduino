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
 *  Implementation:	Yuri Carmona, Ruben Martin
 */
 

  

#ifndef __WPROGRAM_H__
	#include "arduinoClasses.h"
#endif

#include "arduinoUART.h"

/*
 * 
 * name: beginUART
 * @param void
 * @return void
 * 
 */
void arduinoUART::beginUART()
{
	// open UART
	beginSerial(_baudrate, _uart); 
}


/*
 * 
 * name: closeUART
 * @param void
 * @return void
 * 
 */
void arduinoUART::closeUART()
{
	// close UART
    closeSerial(_uart); 
	
}

/*
 * 
 * name: setTimeout
 * @param uint32_t newTimeout: new default timeout to be used
 * @return void
 * 
 */
void arduinoUART::setTimeout( uint32_t newTimeout )
{
	_def_timeout = newTimeout;
}

/*
 * 
 * name: getTimeout
 * @param void
 * @return _def_timeout attribute
 * 		// power up
        pinMode(XBEE_PW,OUTPUT);
        digitalWrite(XBEE_PW,HIGH);	

 */
uint32_t arduinoUART::getTimeout()
{
	return _def_timeout;
}

/*
 * 
 * name: setDelay
 * @param uint32_t newDelay: new default delay time to wait after command query
 * @return void
 * 
 */
void arduinoUART::setDelay( uint32_t newDelay )
{
	_def_delay = newDelay;
}

/*
 * 
 * name: getDelay
 * @param void
 * @return _def_timeout attribute
 * 
 */
uint32_t arduinoUART::getDelay()
{
	return _def_delay;
}


/*
 * 
 * name: sendCommand
 * @param	char* command: command to be sent
 * @param	char* ans1: expected answer
 * @param	bool flush: flush needed before sending command (1: flush; 0: not)
 * @return 	'0' if timeout error, 
 * 			'1' if ans1
 * 
 */
uint8_t arduinoUART::sendCommand(	char* command, 
								char* ans1 )
{
	return sendCommand(command, ans1, NULL, NULL, NULL, _def_timeout );	
}

uint8_t arduinoUART::sendCommand(	char* command, 
								char* ans1, 
								uint32_t timeout)
{
	return sendCommand(command, ans1, NULL, NULL, NULL, timeout );	
}


uint8_t arduinoUART::sendCommand(	char* command, 
								char* ans1, 
								char* ans2 )
{
	return sendCommand(command, ans1, ans2, NULL, NULL, _def_timeout );	
}
		
		
uint8_t arduinoUART::sendCommand(	char* command, 
								char* ans1, 
								char* ans2, 
								uint32_t timeout)	
{
	return sendCommand(command, ans1, ans2, NULL, NULL, timeout );	
}				
	

uint8_t arduinoUART::sendCommand(	char* command, 
								char* ans1, 
								char* ans2, 
								char* ans3 )
{
	return sendCommand(command, ans1, ans2, ans3, NULL, _def_timeout );	
}				
						

uint8_t arduinoUART::sendCommand(	char* command, 
								char* ans1, 
								char* ans2,
								char* ans3,  
								uint32_t timeout)
{
	return sendCommand(command, ans1, ans2, ans3, NULL, timeout );	
}				
			
	
uint8_t arduinoUART::sendCommand(	char* command, 
								char* ans1, 
								char* ans2, 
								char* ans3, 
								char* ans4)
{
	return sendCommand(command, ans1, ans2, ans3, ans4, _def_timeout );	
}		


/*
 * 
 * name: sendCommand
 * @param	char* command: command to be sent
 * @param	char* ans1: expected answer
 * @param	char* ans2: expected answer
 * @param	char* ans3: expected answer
 * @param	char* ans4: expected answer
 * @param	uint32_t timeout: time to wait for response
 * @return 	'0' if timeout error, 
 * 			'1' if ans1
 * 			'2' if ans2
 * 			'3' if ans3
 * 			'4' if ans4
 * 
 */
uint8_t  arduinoUART::sendCommand(	char* command, 
								char* ans1, 
								char* ans2, 
								char* ans3, 
								char* ans4, 
								uint32_t timeout)
{
	// index counter
	uint16_t i = 0;
  			
	// clear uart buffer before sending command
	if( _flush_mode == true )
	{
        serialFlush(_uart); 		
	}
	
	/// 1. print command
    printString( command, _uart ); 
    delay( _def_delay );
	
	
	/// 2. read answer	
	// clear _buffer
	memset( _buffer, 0x00, sizeof(_buffer) );
	_length = 0;
	
	// get actual instant
	unsigned long previous = millis();
	
	// check available data for 'timeout' milliseconds
    while( (millis() - previous) < timeout )
    {

  
		if( serialAvailable(_uart) )
		{
			if ( i < (sizeof(_buffer)-1) )
			{	
				_buffer[i++] = serialRead(_uart);				
				_length++;			
			}
		}
			
		// Check 'ans1'
		if( find( _buffer, _length, ans1 ) == true )
		{		
			return 1;
		}
		
		// Check 'ans2'
		if( ans2 != NULL )
		{
			if( find( _buffer, _length, ans2 ) == true )
			{	
				return 2;
			}
		}
		
		// Check 'ans3'
		if( ans3 != NULL )
		{
			if( find( _buffer, _length, ans3 ) == true )
			{	
				return 3;
			}
		}
		
		// Check 'ans4'
		if( ans4 != NULL )
		{
			if( find( _buffer, _length, ans4 ) == true )
			{	
				return 4;
			}
		}	
		
		// Condition to avoid an overflow (DO NOT REMOVE)
		if( millis() < previous) previous = millis();
	}
		
	// timeout
	return 0; 
	
}




/*
 * 
 * name: find
 * @param	uint8_t* buffer: pointer to buffer to be scanned
 * @param	uint16_t length: actual length of buffer
 * @param	char* pattern: pattern to find
 * @return 	'0' not found, 
 * 			'1' pattern found
 * 
 */
bool arduinoUART::find( uint8_t* buffer, uint16_t length, char* pattern)
{
	int result;
	
	if( length >= strlen(pattern) )
	{		
		for(uint16_t i = 0; i <= length-strlen(pattern); i++)
		{
			result = memcmp( &buffer[i], pattern, strlen(pattern) );
			
			if( result == 0 )
			{
				return true;
			}
		}
	}
	
	return false;
}






/*
 * 
 * name: sendCommand
 * @param	uint8_t* command: pointer to the buffer with the command to be sent
 * @param	uint16_t length: length of the buffer
 * @return 	void 
 */
void  arduinoUART::sendCommand( uint8_t* command, uint16_t length )
{  	
	// clear uart buffer before sending command
	if( _flush_mode == true )
	{
		serialFlush(_uart); 		
	}
	
	/// print command
	for (int i = 0; i < length; i++)
	{
		printByte(command[i], _uart); 
	}	

	delay( _def_delay );	
}






/*
 * 
 * name: sendCommand
 * @param	char* command: command to be sent
 * @param	char* ans1: expected answer
 * @param	bool flush: flush needed before sending command (1: flush; 0: not)
 * @return 	'0' if timeout error, 
 * 			'1' if ans1
 * 
 */
uint8_t arduinoUART::waitFor(	char* ans1 )
{
	return waitFor( ans1, NULL, NULL, NULL, _def_timeout );	
}

uint8_t arduinoUART::waitFor(	char* ans1, 
							uint32_t timeout)
{
	return waitFor( ans1, NULL, NULL, NULL, timeout );	
}


uint8_t arduinoUART::waitFor(	char* ans1, 
							char* ans2 )
{
	return waitFor( ans1, ans2, NULL, NULL, _def_timeout );	
}
		
		
uint8_t arduinoUART::waitFor(	char* ans1, 
							char* ans2, 
							uint32_t timeout)	
{
	return waitFor( ans1, ans2, NULL, NULL, timeout );	
}				
	

uint8_t arduinoUART::waitFor(	char* ans1, 
							char* ans2, 
							char* ans3 )
{
	return waitFor( ans1, ans2, ans3, NULL, _def_timeout );	
}				
						

uint8_t arduinoUART::waitFor(	char* ans1, 
							char* ans2,
							char* ans3,  
							uint32_t timeout)
{
	return waitFor( ans1, ans2, ans3, NULL, timeout );	
}				
			
	
uint8_t arduinoUART::waitFor(	char* ans1, 
							char* ans2, 
							char* ans3, 
							char* ans4)
{
	return waitFor( ans1, ans2, ans3, ans4, _def_timeout );	
}		


/*
 * 
 * name: waitFor
 * @brief	This function waits for one of the answers during a certain period 
 * 			of time. The result is stored in '_buffer'.
 * @param	char* command: command to be sent
 * @param	char* ans1: expected answer
 * @param	char* ans2: expected answer
 * @param	char* ans3: expected answer
 * @param	char* ans4: expected answer
 * @return 	'0' if timeout error, 
 * 			'1' if ans1
 * 			'2' if ans2
 * 			'3' if ans3
 * 			'4' if ans4
 */
uint8_t  arduinoUART::waitFor( char* ans1, 
							char* ans2, 
							char* ans3, 
							char* ans4, 
							uint32_t timeout )
{ 
	// index counter
	uint16_t i = 0;
	
	// clear _buffer
	memset( _buffer, 0x00, sizeof(_buffer) );
	_length = 0;
	
	// get actual instant
	unsigned long previous = millis();
	
	// check available data for 'timeout' milliseconds
    while( (millis() - previous) < timeout )
    {
		if( serialAvailable((_uart)) )
		{
			if ( i < (sizeof(_buffer)-1) )
			{	
				_buffer[i++] = serialRead(_uart);				
				_length++;				
			}
		}
			
		// Check 'ans1'
		if( find( _buffer, _length, ans1 ) == true )
		{		
			return 1;
		}
		
		// Check 'ans2'
		if( ans2 != NULL )
		{
			if( find( _buffer, _length, ans2 ) == true )
			{	
				return 2;
			}
		}
		
		// Check 'ans3'
		if( ans3 != NULL )
		{
			if( find( _buffer, _length, ans3 ) == true )
			{	
				return 3;
			}
		}
		
		// Check 'ans4'
		if( ans4 != NULL )
		{
			if( find( _buffer, _length, ans4 ) == true )
			{	
				return 4;
			}
		}	
		
		// Condition to avoid an overflow (DO NOT REMOVE)
		if( millis() < previous) previous = millis();
	}
		
	// timeout
	return 0; 

}




/*
 * 
 * name: readBuffer
 * This function reads from UART rx buffer and stores the contents in _buffer
 * attribute. _length indicates the number of valid bytes in _buffer
 * 
 * @return 	number of bytes read from UART rx buffer (stored in _buffer)
 */
uint16_t  arduinoUART::readBuffer(uint16_t requestBytes)
{ 
	return readBuffer(requestBytes, true);
}


/*
 * 
 * name: readBuffer
 * This function reads from UART rx buffer and stores the contents in _buffer
 * attribute. _length indicates the number of valid bytes in _buffer
 * 
 * @param bool clearBuffer: indicates if the buffer has to be cleared before reading
 * 
 * @return 	number of bytes read 
 */
uint16_t  arduinoUART::readBuffer(uint16_t requestBytes, bool clearBuffer)
{ 
	// index counter
	uint16_t i = 0;
	uint16_t nBytes = 0;
	
	if( clearBuffer == true )
	{
		// clear _buffer
		memset( _buffer, 0x00, sizeof(_buffer) );
		_length = 0;
	}
	
	// get actual instant
	unsigned long previous = millis();
	
	// check available data for 'timeout' milliseconds
	while( serialAvailable(_uart) )
	{
		if ( (i < (sizeof(_buffer)-1)) && (requestBytes>0) )
		{
			_buffer[i++] = serialRead(_uart);				
			_length++;
			nBytes++;
			requestBytes--;	
			latencyDelay();
		}
		else
		{
			break;
		}
	}
		
	return nBytes;
}


/*
 * 
 * name: latencyDelay
 * This function waits for specific time depending on the baudrate used.
 * 
 * Latency time for ensure there is no more data is calculated suposing the 
 * worst condition: 1start+8data+1parity+2stop = 12 bits per byte
 * 
 */
void arduinoUART::latencyDelay()
{
	// In the case there are not any bytes left in the incoming uart
	// wait for the time one byte needs for tranmission regarding the baudrate
	if (serialAvailable(_uart) == 0)
	{
		switch (_baudrate)
		{
			case 115200:	delayMicroseconds(105); 	break;
			case 57600:		delayMicroseconds(209); 	break;
			case 38400:		delayMicroseconds(313); 	break;
			case 19200:		delayMicroseconds(625); 	break;
			case 9600:		delay(1); 	break;
			case 4800:		delay(3); 	break;
			case 2400:		delay(5); 	break;
			case 1800:		delay(7); 	break;
			case 1200:		delay(10); 	break;
			case 600:		delay(20); 	break;
			case 300:		delay(40); 	break;
			default:					break;			
		}
	}
}

/////////////////////////////////////////////////////////////////////////





// connects the internal peripheral in the processor and configures it
void beginSerial(long baud, uint8_t portNum)
{
	if (portNum == 0) {
        Serial.begin(baud);				
	} else {
        #if defined(HAVE_HWSERIAL1)
        Serial1.begin(baud);				
        #endif // HAVE_HWSERIAL1
    }
}

// disconnects the internal peripheral in the processor
void closeSerial(uint8_t portNum)
{
	if (portNum == 0) {
        Serial.end();	
	} else {
        #if defined(HAVE_HWSERIAL1)
        Serial1.end();	
        #endif // HAVE_HWSERIAL1
	}
}

void serialWrite(unsigned char c, uint8_t portNum)
{
	if (portNum == 0) {
        Serial.write(c);
	} else {
        #if defined(HAVE_HWSERIAL1)
        Serial1.write(c);
        #endif // HAVE_HWSERIAL1
    }
}

int serialAvailable(uint8_t portNum)
{
	if (portNum == 0)
		return Serial.available();
	else {
        #if defined(HAVE_HWSERIAL1)
		return Serial1.available();
        #endif // HAVE_HWSERIAL1
    }
}

int serialRead(uint8_t portNum)
{
	if (portNum == 0) {
        return Serial.read();
	}
	else {
        #if defined(HAVE_HWSERIAL1)
        return Serial1.read();
        #endif // HAVE_HWSERIAL1
	}
}

void serialFlush(uint8_t portNum)
{
	if (portNum == 0){
        Serial.flush();
	}
	else {
        #if defined(HAVE_HWSERIAL1)
        Serial1.flush();
        #endif // HAVE_HWSERIAL1
	}
}


void printByte(unsigned char c, uint8_t portNum)
{
	serialWrite(c, portNum);
}

void printString(const char *s, uint8_t portNum)
{
	while (*s)
		printByte(*s++, portNum);
}





