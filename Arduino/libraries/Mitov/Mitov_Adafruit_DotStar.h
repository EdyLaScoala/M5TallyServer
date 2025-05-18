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
#include <Adafruit_DotStar.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		uint8_t	C_PIN_CLOCK,
		uint8_t	C_PIN_DATA,
		typename T_FModified,
		typename T_InitialValue
	> class ArduinoAdafruitDotStar_SingleRGBLedModule :
		public T_Brightness,
		public T_FModified,
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Brightness )

	public:
		Adafruit_DotStar	FPixel;
		_V_PROP_( FModified )

	public:
		void UpdateBrightness()
		{
			FPixel.setBrightness( AValue * 255 );
			FModified() = true;
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( FModified().GetValue() )
			{
				FPixel.show();
				FModified() = false;
			}
		}

		void ColorInputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetValue() )
				return;

			InitialValue() = AColor;
			FPixel.setPixelColor( 0, InitialValue().GetValue().Red, InitialValue().GetValue().Green, InitialValue().GetValue().Blue );
			FModified() = true;
		}

	public:
		inline void SystemInit()
		{
//			FPixel.setPin( 17 );

			FPixel.begin();
//			FPixel.updateType(NEO_GRB + NEO_KHZ800);
//			FPixel.updateLength(10);
			UpdateBrightness();
			FPixel.setPixelColor( 0, InitialValue().GetValue().Red, InitialValue().GetValue().Green, InitialValue().GetValue().Blue );
			FPixel.show();

		}

		inline void SystemLoopEnd()
		{
			if( FModified().GetValue() )
			{
				FPixel.show();
				FModified() = false;
			}
		}

	public:
		ArduinoAdafruitDotStar_SingleRGBLedModule() :
			FPixel( 1, C_PIN_DATA, C_PIN_CLOCK, DOTSTAR_BGR )
		{
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

