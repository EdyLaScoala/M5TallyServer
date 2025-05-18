////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Display_ST7735.h>

namespace Mitov
{
namespace ST7735
{
//---------------------------------------------------------------------------
	static const uint8_t PROGMEM
	  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
		15,                       // 15 commands in list:
		ST7735::SWRESET,   ST7735::DELAY,  //  1: Software reset, 0 args, w/delay
		  150,                    //     150 ms delay
		ST7735::SLPOUT ,   ST7735::DELAY,  //  2: Out of sleep mode, 0 args, w/delay
		  255,                    //     500 ms delay
		ST7735::FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
		  0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
		ST7735::FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
		  0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
		ST7735::FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
		  0x01, 0x2C, 0x2D,       //     Dot inversion mode
		  0x01, 0x2C, 0x2D,       //     Line inversion mode
		ST7735::INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
		  0x07,                   //     No inversion
		ST7735::PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
		  0xA2,
		  0x02,                   //     -4.6V
		  0x84,                   //     AUTO mode
		ST7735::PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
		  0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
		ST7735::PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
		  0x0A,                   //     Opamp current small
		  0x00,                   //     Boost frequency
		ST7735::PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
		  0x8A,                   //     BCLK/2, Opamp current small & Medium low
		  0x2A,  
		ST7735::PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
		  0x8A, 0xEE,
		ST7735::VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
		  0x0E,
		ST7735::INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
		ST7735::MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
		  0xC8,                   //     row addr/col addr, bottom to top refresh
		ST7735::COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
		  0x05 };                 //     16-bit color

//---------------------------------------------------------------------------
	static const uint8_t PROGMEM
	  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
		4,                        //  4 commands in list:
		ST7735::GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
		  0x02, 0x1c, 0x07, 0x12,
		  0x37, 0x32, 0x29, 0x2d,
		  0x29, 0x25, 0x2B, 0x39,
		  0x00, 0x01, 0x03, 0x10,
		ST7735::GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
		  0x03, 0x1d, 0x07, 0x06,
		  0x2E, 0x2C, 0x29, 0x2D,
		  0x2E, 0x2E, 0x37, 0x3F,
		  0x00, 0x00, 0x02, 0x10,
		ST7735::NORON  ,    ST7735::DELAY, //  3: Normal display on, no args, w/delay
		  10,                     //     10 ms delay
		ST7735::DISPON ,    ST7735::DELAY, //  4: Main screen turn on, no args w/delay
		  100 };                  //     100 ms delay
//---------------------------------------------------------------------------
}
}
