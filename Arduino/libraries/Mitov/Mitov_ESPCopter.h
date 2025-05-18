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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		uint8_t C_BLUE_PIN,
		uint8_t C_GREEN_PIN,
		uint8_t C_RED_PIN,
		typename T_InitialValue
	> class ArduinoRGBESPCopterModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
			Analog.Write( C_RED_PIN, float( 255 - InitialValue().GetReference().Red ) / 255.0 );
			Analog.Write( C_GREEN_PIN, float( 255 - InitialValue().GetReference().Green ) / 255.0 );
			Analog.Write( C_BLUE_PIN, float( 255 - InitialValue().GetReference().Blue ) / 255.0 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif