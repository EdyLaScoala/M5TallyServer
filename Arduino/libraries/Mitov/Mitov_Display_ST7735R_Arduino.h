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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __ST7735__DEBUG__

namespace Mitov
{
namespace ST7735
{
static const uint8_t PROGMEM
  Gcmd[] = {                  // Initialization commands for 7735B screens
    19,                       // 18 commands in list:
    ST7735::SWRESET,   ST7735::DELAY,  //  1: Software reset, no args, w/delay
      50,                     //     50 ms delay
    ST7735::SLPOUT ,   ST7735::DELAY,  //  2: Out of sleep mode, no args, w/delay
      100,                    //     255 = 500 ms delay
    0x26 , 1,  			// 3: Set default gamma
      0x04,                     //     16-bit color
    0xb1, 2,              	// 4: Frame Rate
      0x0b,
      0x14,
    0xc0, 2,                    // 5: VRH1[4:0] & VC[2:0]
      0x08,
      0x00,
    0xc1, 1,                    // 6: BT[2:0]
      0x05,
    0xc5, 2,                    // 7: VMH[6:0] & VML[6:0]
      0x41,
      0x30,
    0xc7, 1,                    // 8: LCD Driving control
      0xc1,
    0xEC, 1,                    // 9: Set pumping color freq
      0x1b,
    0x3a , 1 + ST7735::DELAY,  	        // 10: Set color format
      0x55,                     //     16-bit color
      100,
    0x2a, 4,                    // 11: Set Column Address
      0x00,
      0x00,
      0x00,
      0x7f,
    0x2b, 4,                    // 12: Set Page Address
      0x00,
      0x00,
      0x00,
      0x9f,
    0x36, 1,                    // 12+1: Set Scanning Direction
      0xc8,
    0xb7, 1,			// 14: Set Source Output Direciton
      0x00,
    0xf2, 1,			// 15: Enable Gamma bit
      0x00,
    0xe0, 15 + ST7735::DELAY,		// 16: magic
      0x28, 0x24, 0x22, 0x31,
      0x2b, 0x0e, 0x53, 0xa5,
      0x42, 0x16, 0x18, 0x12,
      0x1a, 0x14, 0x03,
      50,
    0xe1, 15 + ST7735::DELAY,		// 17: more magic
      0x17, 0x1b, 0x1d, 0x0e,
      0x14, 0x11, 0x2c, 0xa5,
      0x3d, 0x09, 0x27, 0x2d,
      0x25, 0x2b, 0x3c, 
      50, 
    ST7735::NORON  ,   ST7735::DELAY,  // 17: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735::DISPON ,   ST7735::DELAY,  // 18: Main screen turn on, no args, w/delay
      255 };                  //     255 = 500 ms delay
} // ST7735
//---------------------------------------------------------------------------
	class Display_ST7735R_Arduino : public Display_ST7735_Common_NonInverted<ST7735::MADCTL_BGR, 128, 160>
	{
	public:
		inline static void GetPixelOffset( TArduinoGraphicsOrientation AOrientation, uint32_t &AOffset_X, uint32_t &AOffset_Y )
		{
            AOffset_X = 2;
            AOffset_Y = 1;
		}
        
	public:
		template <typename T> inline static void SystemInit( T *AInstance )
		{
			AInstance->commandList( ST7735::Gcmd );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif