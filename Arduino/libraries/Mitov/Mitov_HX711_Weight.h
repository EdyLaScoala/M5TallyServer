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
		typename T_DATA_PIN, T_DATA_PIN &C_DATA_PIN, 
		typename T_Average,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FNeedsRead,
		typename T_Gain128,
		typename T_Offset,
		typename T_OutputPin,
		typename T_Powered,
		typename T_ScaleDivider,
		uint16_t C_SensorClockOutputPin,
		typename T_UseChannelB
	> class HX711_Weight :
		public T_Average,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FNeedsRead,
		public T_Gain128,
		public T_Offset,
		public T_OutputPin,
		public T_Powered,
		public T_ScaleDivider,
		public T_UseChannelB
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ScaleDivider )
		_V_PROP_( Offset )
		_V_PROP_( Average )

		_V_PROP_( UseChannelB )
		_V_PROP_( Gain128 )
		_V_PROP_( Powered )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FNeedsRead )

	protected:
		float	FCurrentValue = 0.0f;

	public:
		void SendOutput()
		{
			float AValue = ( FCurrentValue + Offset() ) / ScaleDivider();
			T_OutputPin::SetPinValue( AValue );
		}

		void UpdatePower()
		{
			digitalWrite( C_SensorClockOutputPin, LOW );
//			SensorClockOutputPin.SendValue( false );
			if( Powered() )
				UpdateGain();

			else
				digitalWrite( C_SensorClockOutputPin, HIGH );
//				SensorClockOutputPin.SendValue( true );
		}

		void UpdateGain()
		{
			if( ! Powered() )
				return;

			digitalWrite( C_SensorClockOutputPin, LOW );
//			SensorClockOutputPin.SendValue( false );
			ReadSensorOnes();
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_SensorClockOutputPin, OUTPUT );
			UpdatePower();
		}

		inline void SystemLoopBegin()
		{
			if( FNeedsRead() || ( ! ClockInputPin_o_IsConnected() ) )
				ReadSensor();

		}

	protected:
		void ReadSensor()
		{
//			Serial.println( "ReadSensor" );
			if( T_OutputPin::GetPinIsConnected() )
			{
				if( Powered() ) 
				{
//					if( SensorDataInputPin.Value )
					if( C_DATA_PIN.DigitalRead() )
					{
						FNeedsRead() = true;
						return;
					}

					uint32_t AAccumulator = 0;
					for( int i = 0; i < Average(); ++i )
						AAccumulator += ReadSensorOnes();

					FCurrentValue = AAccumulator / Average();
					FNeedsRead() = false;
				}

				SendOutput();
			}
		}

		uint32_t ReadSensorOnes()
		{
			while( C_DATA_PIN.DigitalRead() )
				;

			uint32_t AValue = 0;
			for( int i = 0; i < 24; ++i )
			{
				AValue <<= 1;
				digitalWrite( C_SensorClockOutputPin, HIGH );

				if( C_DATA_PIN.DigitalRead() )
					AValue |= 1;

				digitalWrite( C_SensorClockOutputPin, LOW );
			}

//			Serial.println( AValue );
			int ACount;
			if( UseChannelB() )
				ACount = 2;

			else if( Gain128() )
				ACount = 1;

			else
				ACount = 3;

			// set the channel and the gain factor for the next reading using the clock pin
			for (int i = 0; i < ACount; i++) 
			{
				digitalWrite( C_SensorClockOutputPin, HIGH );
				digitalWrite( C_SensorClockOutputPin, LOW );
//				SensorClockOutputPin.SendValue( true );
//				SensorClockOutputPin.SendValue( false );
			}				
				
			return AValue;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			FNeedsRead() = true;
			ReadSensor();
		}

		inline void ZeroInputPin_o_Receive( void *_Data )
		{
//			Serial.print( "TEST---------------: " );
			Offset() = -FCurrentValue;
//			Serial.println( Offset );
		}

		inline void CalibrateInputPin_o_Receive( void *_Data )
		{
			ScaleDivider() = ( FCurrentValue + Offset() );
			if( ! ScaleDivider() )
				ScaleDivider() = 1.0;
		}

	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif