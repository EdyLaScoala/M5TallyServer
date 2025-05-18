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
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ButtonOutputPin,
		typename T_Enabled,
		typename T_OutputPins_X,
		typename T_OutputPins_Y
	> class M5StackJoystickUnitI2C : 
		public T_Address,
		public T_ButtonOutputPin,
		public T_Enabled,
		public T_OutputPins_X,
		public T_OutputPins_Y
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( ButtonOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 3 ));
			if( ! C_I2C.available() )
				return;

			float x_data = float( C_I2C.read() ) / 255;
			float y_data = float( C_I2C.read() ) / 255;
			bool button_data = ( C_I2C.read() != 0 );

			T_OutputPins_X::SetPinValue( x_data, AChangeOnly );
			T_OutputPins_Y::SetPinValue( y_data, AChangeOnly );
			T_ButtonOutputPin::SetPinValue( button_data, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AxesOutputPins_X,
		typename T_AxesOutputPins_Y,
		typename T_ButtonOutputPin,
		typename T_Enabled
	> class M5StackJoystickHatI2C : 
		public T_AxesOutputPins_X,
		public T_AxesOutputPins_Y,
		public T_ButtonOutputPin,
		public T_Enabled
	{
		static const uint8_t ADDRESS = 0x38;

	public:
		_V_PIN_( AxesOutputPins_X )
		_V_PIN_( AxesOutputPins_Y )
		_V_PIN_( ButtonOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			Wire.beginTransmission( ADDRESS );
			Wire.write(0x02); 
			Wire.endTransmission();

			Wire.requestFrom( ADDRESS, uint8_t( 3 ));
			if( ! Wire.available() )
				return;

//			float x_data = float( Wire.read() ) / 255;
//			float y_data = float( Wire.read() ) / 255;

//			Serial.println( int8_t( Wire.read() ));

//			float x_data = MitovConstrain( 1.0f - ( float( int8_t( Wire.read() )) + 120 ) / 240, 0.0f, 1.0f );

			int8_t AValue = int8_t( Wire.read() );
//			if( AValue < -127 )
//				AValue = 120;

			float x_data = 1.0f - MitovConstrain( ( float( AValue ) + 120 ) / 240, 0.0f, 1.0f );

			AValue = int8_t( Wire.read() );
//			Serial.println( "" );
//			Serial.println( AValue );

//			if( AValue < -127 )
//				AValue = 120;


			float y_data = 1.0f - MitovConstrain( ( float( AValue ) + 120 ) / 240, 0.0f, 1.0f );

			bool button_data = ( Wire.read() == 0 );

			T_AxesOutputPins_X::SetPinValue( x_data, AChangeOnly );
			T_AxesOutputPins_Y::SetPinValue( y_data, AChangeOnly );
			T_ButtonOutputPin::SetPinValue( button_data, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

