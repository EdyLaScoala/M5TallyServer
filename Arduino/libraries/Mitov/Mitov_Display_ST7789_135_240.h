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
#include <Mitov_Display_ST7789_240_240.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	class Display_ST7789_135_240 : public Display_ST7789_Basic<135, 240>
	{
	public:
		inline static void GetPixelOffset( TArduinoGraphicsOrientation AOrientation, uint32_t &AOffset_X, uint32_t &AOffset_Y )
		{
//        goUp : ADefaultOffset := '53, 40';
//        goRight : ADefaultOffset := '40, 52';
//        goDown : ADefaultOffset := '52, 40';
//        goLeft : ADefaultOffset := '40, 53';
			switch( AOrientation )
            {
                case goUp : 
                {
                    AOffset_X = 53;
                    AOffset_Y = 40;
                    break;
                }
                    
                case goRight : 
                {
                    AOffset_X = 40;
                    AOffset_Y = 52;
                    break;
                }
                
                case goDown : 
                {
                    AOffset_X = 52;
                    AOffset_Y = 40;
                    break;
				}

                default:
                {
                    AOffset_X = 40;
                    AOffset_Y = 53;
                    break;
                }
            }           
		}
        
	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


