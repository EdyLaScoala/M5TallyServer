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
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_OutputPin,
		typename T_Threshold,
		typename T_Wakeup
	> class TArduinoHoltekTouchSensor_BS811XAChannel :
		public T_OutputPin,
		public T_Threshold,
		public T_Wakeup
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Wakeup )
		_V_PROP_( Threshold )

	public:
		inline void UpdateValues( uint16_t AValue, bool AChangeOnly )
		{
			T_OutputPin::SetPinValue( (( AValue & ( 1 << C_INDEX ) ) != 0 ), AChangeOnly );
		}

		inline void GetChannelConfig( uint8_t &AValue )
		{
			AValue = 8 + ( 55 * Threshold().GetValue() + 0.5 );
			if( Wakeup().GetValue() ) 
				AValue |= 0b10000000;

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		uint8_t C_CHANNELS,
		typename T_Channels_GetChannelConfig,
		typename T_Channels_UpdateValues,
		typename T_EnableInterruptPin,
		typename T_Enabled,
		typename T_FNeedsUpdate,
		typename T_LowerCurrentOutputPin
	> class HoltekTouchSensor_I2C : 
		public T_Address,
		public T_EnableInterruptPin,
		public T_Enabled,
		public T_FNeedsUpdate,
		public T_LowerCurrentOutputPin
	{
		const	uint8_t C_KEY_STATUS0 = 0x08;
		const	uint8_t C_KEY_STATUS1 = 0x09;

		const	uint8_t C_OPTION1 = 0xB0;
		const	uint8_t C_OPTION2 = 0xB4;

	public:
		_V_PIN_( LowerCurrentOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( EnableInterruptPin )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FNeedsUpdate )

	public:
		void UpdateSettings()
		{
//			Serial.println( "UpdateSettings" );
			BeginReadRegister( C_KEY_STATUS0 );
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ));
			if( ! C_I2C.available() )
			{
				FNeedsUpdate() = true;
				return;
			}

//			return;
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( C_OPTION1 );

			uint8_t AChecksum = uint8_t
			(
				0x01 + 
				0x00 +
				0x83 +
				0xF3 +
				0b10011000 
			);

			C_I2C.write( 0x01 ); // One Shot Interrupt
			C_I2C.write( 0x00 );
			C_I2C.write( 0x83 );
			C_I2C.write( 0xF3 );
			C_I2C.write( 0b10011000 ); // B4

			for( int i = 0; i < C_CHANNELS - 1; ++i )
			{
				uint8_t AConfig = 0b00010000; // Default value is 16
				T_Channels_GetChannelConfig::Call( i, AConfig );
				C_I2C.write( AConfig );
//				Serial.println( AConfig, BIN );
				AChecksum += AConfig;
			}

			uint8_t AConfig = ( EnableInterruptPin().GetValue() ) ? 0b01010000 : 0b00010000; // Default value is 16
			T_Channels_GetChannelConfig::Call( C_CHANNELS - 1, AConfig );
			C_I2C.write( AConfig );
			AChecksum += AConfig;
			C_I2C.write( AChecksum );

			C_I2C.endTransmission();
			FNeedsUpdate() = false;
		}

	protected:
		void BeginReadRegister( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );	
			C_I2C.endTransmission();
		}

		void ReadValues( bool AChangeOnly )
		{
			if( FNeedsUpdate() )
				UpdateSettings();

			BeginReadRegister( C_KEY_STATUS0 );
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 2 ));

			if( ! C_I2C.available() )
			{
				if( AChangeOnly )
					return;

				T_Channels_UpdateValues::Call( 0, AChangeOnly );

				LowerCurrentOutputPin().SetPinValue( false, AChangeOnly );
				return;
			}

			uint16_t AValue = C_I2C.read();
			AValue |= uint16_t( C_I2C.read() ) << 8;

//			Serial.println( AValue, BIN );

			T_Channels_UpdateValues::Call( AValue, AChangeOnly );

			if( LowerCurrentOutputPin().GetPinIsConnected() )
			{
				BeginReadRegister( C_OPTION2 );
				C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ));
				uint8_t AValue = C_I2C.read();

				LowerCurrentOutputPin().SetPinValue( ( AValue & 0b01000000 ) != 0, AChangeOnly );
			}

//			BeginReadRegister( 0xB5 );
/*
			BeginReadRegister( 0xC4 );
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ));

			Serial.println( uint8_t( C_I2C.read() ), BIN );
*/
//			UpdateSettings();

//			BeginReadRegister( 0xC4 );
//			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ));

//			Serial.println( uint8_t( C_I2C.read() ), BIN );

		}

	public:
		inline void SystemStart()
		{
//			UpdateSettings();
			ReadValues( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValues( true );
		}

	public:
		inline HoltekTouchSensor_I2C()
		{
			FNeedsUpdate() = true;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

