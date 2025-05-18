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
static const uint8_t PROGMEM Rcmd2green160x80[] = 
	{              // Init for 7735R, part 2 (mini 160x80)
		2,                  //  2 commands in list:
		ST7735::CASET,	4,  //  1: Column addr set, 4 args, no delay:
		  0x00, 0x00 + 2,       //     XSTART = 0
		  0x00, 0x7F + 2,       //     XEND = 79
		ST7735::RASET,	4,	//  2: Row addr set, 4 args, no delay:
		  0x00, 0x00,       //     XSTART = 0
		  0x00, 0x9F		//     XEND = 159
	};           
} // ST7735
//---------------------------------------------------------------------------
	class Display_ST7735R_GreenTab_160_80 : public Display_ST7735_Common_NonInverted<ST7735::MADCTL_BGR, 80, 160>
	{
	public:
		inline static void GetPixelOffset( TArduinoGraphicsOrientation AOrientation, uint32_t &AOffset_X, uint32_t &AOffset_Y )
		{
			switch( AOrientation )
            {
                case goUp : 
                case goDown : 
                {
					AOffset_X = 26;
					AOffset_Y = 1;
                    break;
                }
                    
                default:
                {
                    AOffset_X = 1;
                    AOffset_Y = 26;
                    break;
                }
            }
		}
        
	public:
		template <typename T> inline static void SystemInit( T *AInstance )
		{
			AInstance->commandList( ST7735::Rcmd1 );
			AInstance->commandList( ST7735::Rcmd2green160x80 );
			AInstance->commandList( ST7735::Rcmd3 );
			AInstance->writecommand(ST7735::MADCTL);
			AInstance->writedata(0xC0);

//			inherited::SystemInit();
		}

/*
	public:
		Display_ST7735R_GreenTab_160_80( const unsigned char * AFont ) :
			inherited( AFont )
		{
			inherited::colstart = 24;
			inherited::rowstart = 0;
		}
*/
	};
//---------------------------------------------------------------------------
	class Display_ST7735R_GreenTab_Inverted_160_80 : public Display_ST7735R_GreenTab_160_80
	{
	public:
		template <typename T> inline static void UpdateInvertedImpl( T *AInstance )
		{
			AInstance->writecommand( ( AInstance->Inverted() ) ? ST7735::INVOFF : ST7735::INVON );
		}
        
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

