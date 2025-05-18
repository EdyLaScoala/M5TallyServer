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

// http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_AngleOutputPins_X,
		typename T_AngleOutputPins_Y,
		typename T_AngleOutputPins_Z,
		typename T_ButtonOutputPins_C,
		typename T_ButtonOutputPins_Z,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FClockedCount,
		typename T_FStarted,
		typename T_ReadDelay,
		typename T_StickOutputPins_X,
		typename T_StickOutputPins_Y
	> class WiiNunchuck :
		public T_Address,
		public T_AngleOutputPins_X,
		public T_AngleOutputPins_Y,
		public T_AngleOutputPins_Z,
		public T_ButtonOutputPins_C,
		public T_ButtonOutputPins_Z,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FClockedCount,
		public T_FStarted,
		public T_ReadDelay,
		public T_StickOutputPins_X,
		public T_StickOutputPins_Y
	{
	public:
		_V_PIN_( AngleOutputPins_X )
		_V_PIN_( AngleOutputPins_Y )
		_V_PIN_( AngleOutputPins_Z )

		_V_PIN_( ButtonOutputPins_C )
		_V_PIN_( ButtonOutputPins_Z )

		_V_PIN_( StickOutputPins_X )
		_V_PIN_( StickOutputPins_Y )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( ReadDelay )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FStarted )
		_V_PROP_( FClockedCount )
		unsigned long	FLastTime = 0;

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			FClockedCount() = FClockedCount() + 1;
			StartRead();
		}

	protected:
		void ReadData( bool AChangeOnly )
		{
			if( ! Enabled() )
				return;

			ReadController( AChangeOnly );
		}

		void StartRead()
		{
			C_I2C.beginTransmission(uint8_t( Address().GetValue()));
			C_I2C.write(0x00);
			C_I2C.endTransmission();

			delayMicroseconds( 10 );

			C_I2C.beginTransmission(uint8_t( Address().GetValue()));
			C_I2C.write(0x00);
			C_I2C.endTransmission();
			FLastTime = micros();
		}

		void ReadController( bool AChangeOnly )
		{
			uint8_t AData[6];
			C_I2C.requestFrom(uint8_t( Address().GetValue()), uint8_t( 6 ));

			for( int i = 0; i < 6; ++i )
				AData[ i ] = C_I2C.read();

//#ifdef _MITOV_WII_NUNCHUCK_STICK_OUTPUT_PINS_
			float AStickX = (float)AData[0] / 255;
			float AStickY = (float)AData[1] / 255;
			T_StickOutputPins_X::SetPinValue( AStickX, AChangeOnly );
			T_StickOutputPins_Y::SetPinValue( AStickY, AChangeOnly );
//#endif // _MITOV_WII_NUNCHUCK_STICK_OUTPUT_PINS_

//#ifdef _MITOV_WII_NUNCHUCK_ANGLE_OUTPUT_PINS_
			float AAngleX = ((float)(((uint16_t)AData[2] ) << 2) + ((AData[5] >> 2) & 0x03)) / 1023;
			float AAngleY = ((float)(((uint16_t)AData[3] ) << 2) + ((AData[5] >> 4) & 0x03)) / 1023;
			float AAngleZ = ((float)(((uint16_t)AData[4] ) << 2) + ((AData[5] >> 6) & 0x03)) / 1023;

			T_AngleOutputPins_X::SetPinValue( AAngleX, AChangeOnly );
			T_AngleOutputPins_Y::SetPinValue( AAngleY, AChangeOnly );
			T_AngleOutputPins_Z::SetPinValue( AAngleZ, AChangeOnly );
//#endif // _MITOV_WII_NUNCHUCK_ANGLE_OUTPUT_PINS_

//#ifdef _MITOV_WII_NUNCHUCK_BUTTONS_OUTPUT_PINS_
			bool AButtonValue = !( AData[5] & 0b10 );
			T_ButtonOutputPins_C::SetPinValue( AButtonValue, AChangeOnly );

			AButtonValue = !( AData[5] & 1 );
			T_ButtonOutputPins_Z::SetPinValue( AButtonValue, AChangeOnly );
//#endif // _MITOV_WII_NUNCHUCK_BUTTONS_OUTPUT_PINS_

			if( ( ! ClockInputPin_o_IsConnected() ) || FClockedCount() )
				StartRead();
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! FClockedCount() )
				if( ! ClockInputPin_o_IsConnected() )
					FClockedCount() = FClockedCount() + 1;

			if( FClockedCount() )
				if( micros() - FLastTime > ReadDelay().GetValue() )
				{
					FClockedCount() = FClockedCount() - 1;
					ReadData( FStarted() );
					FStarted() = true;
				}

		}

		inline void SystemStart()
		{
			// Init the controller
			C_I2C.beginTransmission( uint8_t( Address().GetValue()));
			C_I2C.write(0xF0);
			C_I2C.write(0x55);
			C_I2C.endTransmission();

			C_I2C.beginTransmission(uint8_t( Address().GetValue()));
			C_I2C.write(0xFB);
			C_I2C.write(0x00);
			C_I2C.endTransmission();

			StartRead();
		}

	public:
		inline WiiNunchuck()
		{
			FStarted() = false;
			FClockedCount() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif