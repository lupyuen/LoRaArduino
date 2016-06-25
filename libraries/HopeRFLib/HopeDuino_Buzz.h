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
 * file       HopeDuino_Buzz.h
 * brief      
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.0
 * date       Feb 15 2016
 * author     QY Ruan
 */
 
#ifndef HopeDuino_BUZZ_h
	#define HopeDuino_BUZZ_h

	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif
	
	#ifndef lword
		typedef unsigned long lword;
	#endif

	#include <avr/io.h>
	#include <util/delay.h>

	#define SL_1		236 	              	
	#define SL_1H	    223 
	#define SL_2	    214 
	#define SL_2H	    198 
	#define SL_3	    187 
	#define SL_4	    177 
	#define SL_4H	    167 
	#define SL_5	    157 
	#define SL_5H       149 
	#define SL_6	    140 
	#define SL_6H	    132 
	#define SL_7	    125 
	               
	#define SM_1		118  
	#define SM_1H	    111  
	#define SM_2	    105  
	#define SM_2H	    99   
	#define SM_3	    94   
	#define SM_4	    88   
	#define SM_4H	    83   
	#define SM_5	    79   
	#define SM_5H	    74   
	#define SM_6	    70   
	#define SM_6H	    66   
	#define SM_7	    62   
	               
	#define SH_1		59 
	#define SH_1H	    56 
	#define SH_2	    53 
	#define SH_2H	    50 
	#define SH_3	    47 
	#define SH_4	    44 
	#define SH_4H	    42 
	#define SH_5	    39 
	#define SH_5H	    37 
	#define SH_6	    35 
	#define SH_6H	    33 
	#define SH_7	    31 

	class buzzClass
	{
	 public:
		void vBuzzInit(void);
		void vBuzzPlay(byte ptr[], byte length);

	};
#else
	#warning "RFduino_Buzz.h have been defined!"

#endif
  