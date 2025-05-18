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
	template <
		typename T_ColorOutputPins_Red,
		typename T_ColorOutputPins_Green,
		typename T_ColorOutputPins_Blue,
		int C_NUM_LEDS,
		typename T_Enabled,
		typename T_Inverted,
		typename T_PixelOutputPins_SetPinValue,
		typename T_RefreshInterval
	> class MitovDynamicRGBBar :
		public T_ColorOutputPins_Red,
		public T_ColorOutputPins_Green,
		public T_ColorOutputPins_Blue,
		public T_Enabled,
		public T_Inverted,
		public T_RefreshInterval
	{
//		_V_PIN_( ColorOutputPins_Red )
//		_V_PIN_( ColorOutputPins_Green )
//		_V_PIN_( ColorOutputPins_Blue )

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FColorValues[ AIndex ] = *(TColor *)_Data;
		}

	public:
		_V_PROP_( Enabled )
		_V_PROP_( RefreshInterval )
		_V_PROP_( Inverted )

	public:
		void UpdateInverted()
		{
			for( int i = 0; i < C_NUM_LEDS; ++i )
				T_PixelOutputPins_SetPinValue::Call( i, Inverted() );

			if( Enabled() )
				T_PixelOutputPins_SetPinValue::Call( FPixel, ! Inverted() );

		}

	protected:
		unsigned long	FLastTime = 0;
		uint8_t			FPixel = 0;

		Mitov::TColor	FColorValues[ C_NUM_LEDS ];

	public:
		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ( ACurrentMicros - FLastTime ) < RefreshInterval() )
				return;

			T_PixelOutputPins_SetPinValue::Call( FPixel, Inverted() );

			++FPixel;

			if( FPixel >= C_NUM_LEDS )
				FPixel = 0;

			Mitov::TColor AColor = FColorValues[ FPixel ];

			T_ColorOutputPins_Red::SetPinValue( float( AColor.Red ) / 255 );
			T_ColorOutputPins_Green::SetPinValue( float( AColor.Green ) / 255 );
			T_ColorOutputPins_Blue::SetPinValue( float( AColor.Blue ) / 255 );

			if( Enabled() )
				T_PixelOutputPins_SetPinValue::Call( FPixel, ! Inverted() );

			FLastTime = ACurrentMicros;
//			UpdateDisplay();
		}

		inline void SystemInit()
		{
			for( int i = 0; i < C_NUM_LEDS; ++i )
				T_PixelOutputPins_SetPinValue::Call( i, Inverted() );

			FLastTime = micros();
		}

	public:
		inline MitovDynamicRGBBar( Mitov::TColor AInitialColor ) :
			FPixel( C_NUM_LEDS - 1 )
		{			
			for( int i = 0; i < C_NUM_LEDS; ++i )
				FColorValues[ i ] = AInitialColor;
		}

	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif