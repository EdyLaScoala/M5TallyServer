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
		typename T_ClockInputPin_o_IsConnected,
		typename T_DataRange,
		typename T_Enabled,
		typename T_Gain,
		typename T_Normalize,
		typename T_OutputPin
	> class TexasInstrumentsADS1100 : 
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_DataRange,
		public T_Enabled,
		public T_Gain,
		public T_Normalize,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Address )
		_V_PROP_( DataRange )
		_V_PROP_( Gain )
		_V_PROP_( Normalize )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PIN_( OutputPin )

	public:
		void UpdateConfig()
		{
			uint8_t AValue = 0;
//			if( Gain().GetValue() < 2 )
//				AValue |= 0x00;

//			else
			if( Gain().GetValue() < 4 )
				AValue |= 0x01;

			else if( Gain().GetValue() < 8 )
				AValue |= 0x02;

			else
				AValue |= 0x03;

			if( DataRange().GetValue() < 16 )
				AValue |= 0x0C;

			else if( DataRange().GetValue() < 32 )
				AValue |= 0x08;

			else if( DataRange().GetValue() < 128 )
				AValue |= 0x04;

			writeRegister( AValue );
		}

	protected:
		void writeRegister( uint8_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

		int16_t readRegister()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ));
			uint16_t AResult = uint16_t( C_I2C.read()) << 8;
			AResult |= C_I2C.read();
//			Serial.println( AResult );
			return *((int16_t *)&AResult );
		}

		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			float AValue = float( readRegister() );

			if( Normalize().GetValue() )
				AValue /= 0x7FFF;

			else
				AValue /= ( 0x7FFF ) / 12.0f;

			T_OutputPin::SetPinValue( AValue, AChangeOnly );
/*
			uint8_t ADatos[ 5 ];

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( 0 );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address() ), uint8_t( 5 ));

			for( int i=0; i < 5; ++ i )
				ADatos[ i ] = C_I2C.read();

//			Serial.println( "TEST: " );
//			for( int i=0; i < 5; ++ i )
//			{
//				Serial.print( ADatos[ i ], HEX ); Serial.print( " " );
//			}

//			Serial.println( "" );

			if( ADatos[4] != ( ADatos[0] + ADatos[1] + ADatos[2] + ADatos[3] ))
				return; // Error

//			Serial.println( "TEST1" );
			float ATemperature = ( ADatos[2] + float( ADatos[ 3 ] ) / 10 );
			T_TemperatureOutputPin::SetPinValue( ATemperature );

			float AHumidity = ( ADatos[ 0 ] + float( ADatos[ 1 ] ) / 10 );
			T_HumidityOutputPin::SetPinValue( AHumidity );
*/
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor( false );
		}

	public:
		inline void SystemStart()
		{
			UpdateConfig();

			if( ! ClockInputPin_o_IsConnected() )
				ReadSensor( false );

		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
