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
 * file       HopeDuino_Disp.h
 * brief      
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.0
 * date       Feb 15 2016
 * author     QY Ruan
 */
 
#ifndef HopeDuino_DISP_h
	#define HopeDuino_DISP_h

	#include <HopeDuino_LCD.h>

	class dispClass
	{
	 public:
		void vDispInit(void);
		void vDispOn(void);
		void vDispOff(void);
		void vDispLight(bool on);
		void vDispFull(void);
		void vDispClear(void);
		
		void vDispGrap32x32(byte page, byte column, byte ptr[]);
		void vDispGrap16x16(byte page, byte column, byte ptr[]);
		void vDispGrap8x16(byte page, byte column, byte ptr[]);
		void vDispGrap8x8(byte page, byte column, byte ptr[]);
		void vDispString8x8(byte page, byte column, byte text[]);
		
		void vDisHopeRFLogo(byte line, bool positive);				
		void vDisArduinoLogo(bool positive);
		
		void vDispSetDot(byte x_axis, byte y_axis);
		void vDispFillDot(byte x_axis, byte y_axis);
		void vDispClrYaxis(byte x_axis);
	 private:
	 	lcdClass JLX12864;
	
	};
#else
	#warning "RFduino_Disp.h have been defined!"

#endif
 
