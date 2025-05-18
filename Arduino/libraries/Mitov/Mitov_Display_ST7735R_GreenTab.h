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
	static const uint8_t PROGMEM
		Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)
		2,                        //  2 commands in list:
		ST7735::CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
			0x00, 0x02,             //     XSTART = 0
			0x00, 0x7F+0x02,        //     XEND = 127
		ST7735::RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
			0x00, 0x01,             //     XSTART = 0
			0x00, 0x9F+0x01 };      //     XEND = 159
} // ST7735
//---------------------------------------------------------------------------
	class Display_ST7735R_GreenTab : public Display_ST7735_Common_NonInverted<ST7735::MADCTL_RGB, 128, 160>
	{
	public:
		inline static void GetPixelOffset( TArduinoGraphicsOrientation AOrientation, uint32_t &AOffset_X, uint32_t &AOffset_Y )
		{
//            AOffset_X = 2;
//            AOffset_Y = 1;

			switch( AOrientation )
            {
                case goUp : 
                case goDown : 
                {
					AOffset_X = 2;
					AOffset_Y = 1;
					break;
				}

                default:
                {
					AOffset_X = 1;
					AOffset_Y = 2;
					break;
				}
			}
/*
			switch( AOrientation )
            {
                case goUp : 
                case goDown : 
                {
					AOffset_X = 1;
					AOffset_Y = 2;
                    break;
                }
                    
                default:
                {
                    AOffset_X = 0;
                    AOffset_Y = 0;
                    break;
                }
			}
//			AOffset_X = 0;
//            AOffset_Y = 0;
*/
		}
        
	public:
		template <typename T> inline static void SystemInit( T *AInstance )
		{
			AInstance->commandList( ST7735::Rcmd1 );
			AInstance->commandList( ST7735::Rcmd2green );
			AInstance->commandList( ST7735::Rcmd3 );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif