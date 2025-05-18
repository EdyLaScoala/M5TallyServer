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
	namespace M5Stack8ServosHat
	{
		const uint8_t	ADDRESS			= 0x38;
		const uint8_t	LED_REG_ADDRESS	= 0x20;
	}
//---------------------------------------------------------------------------
	class M5Stack8ServosHatI2C
	{
	public:
//		constexpr static inline uint8_t GetLEDsAddress() { return M5Stack8ServosHat::ADDRESS; }
//		constexpr static inline uint8_t GetLEDsRegister() { return M5Stack8ServosHat::LED_REG_ADDRESS; }

		void SetRGBColor( uint32_t AColor )
		{
			Wire.beginTransmission( M5Stack8ServosHat::ADDRESS );
			Wire.write( M5Stack8ServosHat::LED_REG_ADDRESS );

			Wire.write( uint8_t( AColor >> 8 )); // G
			Wire.write( uint8_t( AColor >> 16 )); // R
			Wire.write( uint8_t( AColor )); // B

			Wire.endTransmission();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		typename T_Reversed
	> class M5Stack8ServosHatServo :
		public T_Enabled,
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse,
		public T_Reversed
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )
		_V_PROP_( Reversed )

	public:
		inline void UpdateEnabled()
		{
			UpdatePosition();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *((float *)_Data);
			if( Enabled() )
				UpdatePosition();

		}

	protected:
		void UpdatePosition()
		{
			float AAnalogValue = MitovConstrain( InitialValue().GetValue(), 0.0f, 1.0f );
			if( Reversed() )
				AAnalogValue = 1.0 - AAnalogValue;

			uint16_t AValue = ( AAnalogValue * (MaxPulse().GetValue() - MinPulse().GetValue())) + MinPulse().GetValue() + 0.5;

//			Serial.println( AValue );

			Wire.beginTransmission( M5Stack8ServosHat::ADDRESS );

//			2*servo_ch+16
			Wire.write( 16 + C_INDEX * 2 );
			Wire.write( uint8_t( AValue >> 8 )); //MSB
			Wire.write( uint8_t( AValue ) ); //LSB
			Wire.endTransmission();
/*
#ifdef VISUINO_ANY_ESP32
			float AValue = MitovConstrain( InitialValue().GetValue(), 0.0f, 1.0f ) * 180;
			Mitov::ServoWrite( PIN_NUMBER, AValue, MinPulse().GetValue(), MaxPulse().GetValue() );
#else
//			FServo.write( AValue );
			float AValue = (MitovConstrain( InitialValue().GetValue(), 0.0f, 1.0f ) * (MaxPulse().GetValue() - MinPulse().GetValue())) + MinPulse().GetValue(); // Scale input to microseconds
			FServo.writeMicroseconds( AValue );
#endif
*/
		}

	public:
		inline void SystemStart()
		{
			UpdateEnabled();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

