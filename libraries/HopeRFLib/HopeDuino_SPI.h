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
 * file       HopeDuino_SPI.cpp
 * brief      for HopeRF's EVB to use Hardware SPI
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.1
 * date       Jan 15 2015
 * author     QY Ruan
 */


#ifndef HopeDuino_SPI_h
	#define HopeDuino_SPI_h
	#include <avr/io.h>

	#ifndef nSS_Value
		#warning "Does not define nSS_Value/nSS_Port/nSS_Dir! Default setting PortB_4 for nSS."
		#define	nSS_Value	0x04
		#define	nSS_Port	PORTB
		#define nSS_Dir		DDRB
	#endif
	
	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif

	/** Hardware brief **/    
	//PORTB						//DDRx		PORTx		PINx
	#define	_SDI     0x08       // 1          1          0
	#define	_SDO     0x10		// 0          0          1
	#define	_SCK	 0x20		// 1          0          0

	#define	SetnSS()	(nSS_Port |= nSS_Value)
	#define	ClrnSS()	(nSS_Port &= (~nSS_Value))

	class spiClass
	{
	 public:
		void vSpiInit(void);				/** initialize hardware SPI config, SPI_CLK = Fcpu/4 **/	
	 	void vSpiWrite(word dat);			/** SPI send one word **/
		byte bSpiRead(byte addr);			/** SPI read one byte **/
		void vSpiBurstWrite(byte addr, byte ptr[], byte length);	/** SPI burst send N byte **/
		void vSpiBurstRead(byte addr, byte ptr[], byte length);	 	/** SPI burst rend N byte **/
	 private:
	 	byte bSpiTransfer(byte dat);		/**	SPI send/read one byte **/
	};

#else
	#warning "HopeDuino_SPI.h have been defined!"

#endif