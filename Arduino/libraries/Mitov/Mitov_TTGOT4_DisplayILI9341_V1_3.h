////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov.h>
#include <Mitov_Display_ILI9341.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_0_IMPLEMENTATION,
		typename T_0_ORIENTATION_IMPLEMENTATION,
		typename T_BackgroundColor,
		typename T_Brightness,
		bool C_HAS_READ,
		uint16_t C_HEIGHT,
		uint16_t C_WIDTH,
		typename T_Elements_Render,
        typename T_FCursorX,
        typename T_FCursorY,
		typename T_Inverted,
		typename T_Orientation,
		typename T_Text
	> class TTGOT4_DisplayILI9341_V1_3 :
		public DisplayILI9341<
                T_0_IMPLEMENTATION,
                T_0_ORIENTATION_IMPLEMENTATION,
				T_BackgroundColor,
				C_HAS_READ,
                C_HEIGHT,
                C_WIDTH,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_Text
			>,
		public T_Brightness
	{
		typedef DisplayILI9341<
                T_0_IMPLEMENTATION,
                T_0_ORIENTATION_IMPLEMENTATION,
				T_BackgroundColor,
				C_HAS_READ,
                C_HEIGHT,
                C_WIDTH,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_Text
			> inherited;

		static const uint8_t C_BRIGHNESS_PIN = 4;

	public:
		_V_PROP_( Brightness )

	public:
		void UpdateBrightness()
		{
			Analog.Write( C_BRIGHNESS_PIN, Brightness() );
		}

	public:
		inline void SystemStart()
		{
			UpdateBrightness();
			inherited::SystemStart();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
