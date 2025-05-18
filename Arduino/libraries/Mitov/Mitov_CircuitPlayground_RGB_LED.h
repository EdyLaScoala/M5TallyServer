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
#include <Adafruit_NeoPixel.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		typename T_FModified
	> class ArduinoCircuitPlaygroundRGBLedsModule :
		public T_Brightness,
		public T_FModified
	{
	public:
		_V_PROP_( Brightness )

	public:
		Adafruit_NeoPixel	FPixel;
		_V_PROP_( FModified )

	public:
		void SetBrightness( float AValue )
		{
			if( Brightness().GetValue() == AValue )
				return;

			IntSetBrightness( AValue );
		}

	public:
		void SetPixelColor( int AIndex, TColor AColor )
		{
//			Serial.println( AColor.ToString() );
			FPixel.setPixelColor( AIndex, AColor.Red, AColor.Green, AColor.Blue );
//			FPixel.setPixelColor( AIndex, 255, 0, 0 );
			FModified() = true;
		}

	protected:
		void IntSetBrightness( float AValue )
		{
			Brightness() = AValue;
			FPixel.setBrightness( AValue * 255 );
			FModified() = true;
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( FModified() )
			{
				FPixel.show();
				FModified = false;
			}
		}

	public:
		inline void SystemInit()
		{
			FPixel.setPin( 17 );

			FPixel.begin();
			FPixel.updateType(NEO_GRB + NEO_KHZ800);
			FPixel.updateLength(10);
			IntSetBrightness( Brightness() );
			FPixel.show();

		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
			{
				FPixel.show();
				FModified() = false;
			}

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_INDEX,
		typename T_InitialValue
	> class ArduinoCircuitPlaygroundRGBLed :
		public T_InitialValue
	{
	public:
//		TColor	InitialValue;
		_V_PROP_( InitialValue )

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;
			C_OWNER.SetPixelColor( C_INDEX, InitialValue() );
		}

	public:
		inline void SystemStart()
		{
			C_OWNER.SetPixelColor( C_INDEX, InitialValue() );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif