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
//		typename T_Address,
		typename T_Buttons_GetChannelConfig,
		typename T_Buttons_UpdateValues,
		typename T_Enabled,
		typename T_FNeedsUpdate,
		typename T_Keys_GetChannelConfig,
		typename T_Keys_UpdateValues
	> class YahboomPianoBreakoutKeyboard :
		public T_Enabled,
		public T_FNeedsUpdate
	{
		const	uint8_t C_KEY_STATUS0 = 0x08;
		const	uint8_t C_KEY_STATUS1 = 0x09;

		const	uint8_t C_OPTION1 = 0xB0;
		const	uint8_t C_OPTION2 = 0xB4;

		const	uint8_t C_ADDRESS = 0x50;

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FNeedsUpdate )

	public:
		void UpdateSettings()
		{
//			Serial.println( "UpdateSettings" );
			BeginReadRegister( C_KEY_STATUS0 );
			C_I2C.requestFrom( C_ADDRESS, uint8_t( 1 ));
			if( ! C_I2C.available() )
			{
				FNeedsUpdate() = true;
				return;
			}

//			return;
			C_I2C.beginTransmission( C_ADDRESS );
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

//			for( int i = 0; i < C_CHANNELS - 1; ++i )
			for( int i = 0; i < 15; ++i )
			{
				uint8_t AConfig = 0b00010000; // Default value is 16
				T_Buttons_GetChannelConfig::Call( i, AConfig );
				T_Keys_GetChannelConfig::Call( i, AConfig );
				C_I2C.write( AConfig );
//				Serial.println( AConfig, BIN );
				AChecksum += AConfig;
			}

			C_I2C.write( 0b01010000 ); // Set to Interrupt
			AChecksum += 0b01010000;
			C_I2C.write( AChecksum );

			C_I2C.endTransmission();
			FNeedsUpdate() = false;
		}

	protected:
		void BeginReadRegister( uint8_t ARegister )
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
		}

		void ReadValues( bool AChangeOnly )
		{
			if( FNeedsUpdate() )
				UpdateSettings();

			BeginReadRegister( C_KEY_STATUS0 );
			C_I2C.requestFrom( C_ADDRESS, uint8_t( 2 ));

			if( ! C_I2C.available() )
			{
				if( AChangeOnly )
					return;

				T_Buttons_UpdateValues::Call( 0, AChangeOnly );
				T_Keys_UpdateValues::Call( 0, AChangeOnly );
				return;
			}

			uint16_t AValue = C_I2C.read();
			AValue |= uint16_t( C_I2C.read() ) << 8;

//			Serial.println( AValue, BIN );

			T_Buttons_UpdateValues::Call( AValue, AChangeOnly );
			T_Keys_UpdateValues::Call( AValue, AChangeOnly );

//			BeginReadRegister( 0xB5 );
/*
			BeginReadRegister( 0xC4 );
			C_I2C.requestFrom( C_ADDRESS, uint8_t( 1 ));

			Serial.println( uint8_t( C_I2C.read() ), BIN );
*/
//			UpdateSettings();

//			BeginReadRegister( 0xC4 );
//			C_I2C.requestFrom( C_ADDRESS, uint8_t( 1 ));

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
		inline YahboomPianoBreakoutKeyboard()
		{
			FNeedsUpdate() = true;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

