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

namespace Mitov
{
	template <
		typename T_IMPLEMENTATION,
		typename T_IMPLEMENTATION_ORIENTATION,
		typename T_IMPLEMENTATION_TYPE,
		uint8_t C_2_BRIGHTNESS_PIN,
		typename T_BackgroundColor,
		typename T_Brightness,
		typename T_Elements_Render,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_Inverted,
        typename T_Orientation,
		typename T_OverridePixelOffset,
		typename T_ResetOutputPin,
		typename T_Text
	> class DisplayST77XXBrightness :
		public DisplayST7735Basic<
				T_IMPLEMENTATION,
				T_IMPLEMENTATION_ORIENTATION,
				T_IMPLEMENTATION_TYPE,
				T_BackgroundColor,
				T_Elements_Render,
				T_FCursorX,
				T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_OverridePixelOffset,
				T_ResetOutputPin,
				T_Text
			>,
		public T_Brightness
	{
		typedef DisplayST7735Basic<
				T_IMPLEMENTATION,
				T_IMPLEMENTATION_ORIENTATION,
				T_IMPLEMENTATION_TYPE,
				T_BackgroundColor,
				T_Elements_Render,
				T_FCursorX,
				T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_OverridePixelOffset,
				T_ResetOutputPin,
				T_Text
			> inherited;

	public:
        _V_PROP_( Brightness )

	public:
		inline void UpdateBrightness()
		{
			Analog.Write( C_2_BRIGHTNESS_PIN, Brightness() );
		}

	public:
		inline void SystemStart()
		{
			inherited::SystemStart();
            UpdateBrightness();
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif