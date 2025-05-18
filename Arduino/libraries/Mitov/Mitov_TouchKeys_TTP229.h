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
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_OutputPins_SetPinValue
	> class TArduinoTouchKeysTTP229_I2C :
		public T_Address,
		public T_Enabled
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	public:
		inline void UpdateEnabled()
		{
			if( Enabled().GetValue() )
				return;

			for( int i = 0; i < 16; ++i )
				T_OutputPins_SetPinValue::Call( i, false );

		}

	protected:
		void ReadButtons()
		{
			if( ! Enabled().GetValue() )
				return;

			C_I2C.requestFrom( Address().GetValue(), (uint8_t) 2);

			uint16_t AValue = uint16_t( C_I2C.read() ) << 8;
			AValue |= C_I2C.read();

//			Serial.println( AValue, HEX );

			for( int i = 0; i < 16; ++i )
				T_OutputPins_SetPinValue::Call( i, AValue & ( 0b1000000000000000 >> i ));
		}

	public:
		inline void SystemStart()
		{
			ReadButtons();
		}

		inline void SystemLoopBegin()
		{
			ReadButtons();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockOutputPin,
		uint8_t C_DataInputPin,
		typename T_Enabled,
		typename T_OutputPins_SetPinValue
	> class TArduinoTouchKeysTTP229_Serial :
		public T_ClockOutputPin,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void UpdateEnabled()
		{
			if( Enabled().GetValue() )
				return;

			for( int i = 0; i < 16; ++i )
				T_OutputPins_SetPinValue::Call( i, false );

		}

	protected:
		void ReadButtons()
		{
			if( ! Enabled().GetValue() )
				return;

			if( digitalRead( C_DataInputPin ) == HIGH )
				return;

			delayMicroseconds( 10 );
			for( int i = 0; i < 16; ++i )
			{
				T_ClockOutputPin::SetPinValue( false );
				T_OutputPins_SetPinValue::Call( i, digitalRead( C_DataInputPin ) == LOW );
				delayMicroseconds( 2 );
				T_ClockOutputPin::SetPinValue( true );
				delayMicroseconds( 2 );
			}

//			delay( 1000 );
		}

	public:
		inline void SystemStart()
		{
			T_ClockOutputPin::SetPinValue( true );
			ReadButtons();
		}

		inline void SystemLoopBegin()
		{
			ReadButtons();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

