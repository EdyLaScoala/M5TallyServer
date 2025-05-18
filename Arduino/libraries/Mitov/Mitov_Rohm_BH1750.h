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
	enum TArduinoLightSensorRohmBH1750ResolutionMode { rmVeryHigh, rmHigh, rmLow };

	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ContinuousMode,
		typename T_Enabled,
		typename T_InLumens,
		typename T_MeasurementTime,
		typename T_OutputPin,
		typename T_Resolution
	> class LightSensorBH1750 :
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_ContinuousMode,
		public T_Enabled,
		public T_InLumens,
		public T_MeasurementTime,
		public T_OutputPin,
		public T_Resolution
	{

		// No active state
		static const uint8_t BH1750_POWER_DOWN = 0x00;

		// Wating for measurment command
		static const uint8_t BH1750_POWER_ON = 0x01;

		// Reset data register value - not accepted in POWER_DOWN mode
		static const uint8_t BH1750_RESET = 0x07;

		// Start measurement at 1lx resolution. Measurement time is approx 120ms.
		static const uint8_t BH1750_CONTINUOUS_HIGH_RES_MODE  = 0x10;

		// Start measurement at 0.5lx resolution. Measurement time is approx 120ms.
		static const uint8_t BH1750_CONTINUOUS_HIGH_RES_MODE_2  = 0x11;

		// Start measurement at 4lx resolution. Measurement time is approx 16ms.
		static const uint8_t BH1750_CONTINUOUS_LOW_RES_MODE  = 0x13;

		// Start measurement at 1lx resolution. Measurement time is approx 120ms.
		// Device is automatically set to Power Down after measurement.
		static const uint8_t BH1750_ONE_TIME_HIGH_RES_MODE  = 0x20;

		// Start measurement at 0.5lx resolution. Measurement time is approx 120ms.
		// Device is automatically set to Power Down after measurement.
		static const uint8_t BH1750_ONE_TIME_HIGH_RES_MODE_2  = 0x21;

		// Start measurement at 1lx resolution. Measurement time is approx 120ms.
		// Device is automatically set to Power Down after measurement.
		static const uint8_t BH1750_ONE_TIME_LOW_RES_MODE  = 0x23;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( MeasurementTime )

		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( ContinuousMode )
		_V_PROP_( InLumens )
		_V_PROP_( Resolution )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		void UpdateEnabled()
		{
			if( ! Enabled() )
			{
				write8( BH1750_POWER_DOWN );
				return;
			}

			write8( BH1750_POWER_ON );
            delay(10);

			uint8_t AMode;
			if( ContinuousMode() || ! ClockInputPin_o_IsConnected()  )
			{
				switch ( Resolution() )
				{
					case rmVeryHigh : AMode = BH1750_CONTINUOUS_HIGH_RES_MODE_2; break; 
					case rmHigh : AMode = BH1750_CONTINUOUS_HIGH_RES_MODE; break; 
					case rmLow : AMode = BH1750_CONTINUOUS_LOW_RES_MODE; break; 
				}
			}
			else
			{
				switch ( Resolution() )
				{
					case rmVeryHigh : AMode = BH1750_ONE_TIME_HIGH_RES_MODE_2; break; 
					case rmHigh : AMode = BH1750_ONE_TIME_HIGH_RES_MODE; break; 
					case rmLow : AMode = BH1750_ONE_TIME_LOW_RES_MODE; break; 
				}
			}

			write8( AMode );
            delay(10);
			UpdateMeasurementTime();
		}

		void UpdateMeasurementTime()
		{
			uint8_t ATime = Func::MapRange<float>( MeasurementTime(), 0.45, 3.68, 31, 254 ) + 0.5;

			uint8_t AHighBits = 0b01000000 | ( ATime >> 5 );
			write8( AHighBits );

			uint8_t ALowBits = 0b01100000 | ( ATime & 0b11111 );
			write8( ALowBits );
		}

	protected:
		void write8(uint8_t AData ) 
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( AData );
			C_I2C.endTransmission();
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			uint16_t level;

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 2 ));
			level = C_I2C.read();
			level <<= 8;
			level |= C_I2C.read();
			C_I2C.endTransmission();

			float AFloatLevel = level;
			AFloatLevel = InLumens() ? AFloatLevel / 1.2 : AFloatLevel / 0xFFFF;
			T_OutputPin::SetPinValue( AFloatLevel );
		}

		void ResetInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				write8( BH1750_RESET );
		}

	public:
		inline void SystemStart()
		{
			UpdateEnabled();
		}

		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};

}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif