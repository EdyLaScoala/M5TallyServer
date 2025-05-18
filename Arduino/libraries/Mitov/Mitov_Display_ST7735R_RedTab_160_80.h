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
#include <Mitov_Display_ST7735_Const_R.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
namespace ST7735
{
static const uint8_t PROGMEM Rcmd2red160x80[] = 
	{              // Init for 7735R, part 2 (mini 160x80)
		2,                  //  2 commands in list:
		ST7735::CASET,	4,  //  1: Column addr set, 4 args, no delay:
		  0x00, 0x00,       //     XSTART = 0
		  0x00, 0x7F,       //     XEND = 79
		ST7735::RASET,	4,	//  2: Row addr set, 4 args, no delay:
		  0x00, 0x00,       //     XSTART = 0
		  0x00, 0x9F		//     XEND = 159
	};           
} // ST7735
//---------------------------------------------------------------------------
	class Display_ST7735R_RedTab_160_80 : public Display_ST7735_Common_NonInverted<ST7735::MADCTL_BGR, 80, 160>
	{
	public:
		inline static void GetPixelOffset( TArduinoGraphicsOrientation AOrientation, uint32_t &AOffset_X, uint32_t &AOffset_Y )
		{
            AOffset_X = 24;
            AOffset_Y = 0;
		}
        
	public:
		template <typename T> inline static void SystemInit( T *AInstance )
		{
			AInstance->commandList( ST7735::Rcmd1 );
			AInstance->commandList( ST7735::Rcmd2red160x80 );
			AInstance->commandList( ST7735::Rcmd3 );
			AInstance->writecommand(ST7735::MADCTL);
			AInstance->writedata(0xC0);
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif