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
		typename T_OutputPins_Left,
		typename T_OutputPins_Center,
		typename T_OutputPins_Right
	> class ElegooRobotLineFollowingSensors :
		public T_OutputPins_Left,
		public T_OutputPins_Center,
		public T_OutputPins_Right
	{
	public:
		_V_PIN_( OutputPins_Left )
		_V_PIN_( OutputPins_Center )
		_V_PIN_( OutputPins_Right )

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			T_OutputPins_Left::SetPinValue( digitalRead( 10 ) == HIGH, AChangeOnly );
			T_OutputPins_Center::SetPinValue( digitalRead( 4 ) == HIGH, AChangeOnly );
			T_OutputPins_Right::SetPinValue( digitalRead( 2 ) == HIGH, AChangeOnly );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_CENTER,
		uint8_t C_LEFT,
		uint8_t C_RIGHT,
		typename T_OutputPins_Left,
		typename T_OutputPins_Center,
		typename T_OutputPins_Right
	> class ElegooRobotAnalogLineFollowingSensors :
		public T_OutputPins_Left,
		public T_OutputPins_Center,
		public T_OutputPins_Right
	{
	public:
		_V_PIN_( OutputPins_Left )
		_V_PIN_( OutputPins_Center )
		_V_PIN_( OutputPins_Right )

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			T_OutputPins_Left::SetPinValue( 1.0 - ( analogRead( C_LEFT ) / float( VISUINO_ANALOG_IN_RANGE )), AChangeOnly );
			T_OutputPins_Center::SetPinValue( 1.0 - ( analogRead( C_CENTER ) / float( VISUINO_ANALOG_IN_RANGE )), AChangeOnly );
			T_OutputPins_Right::SetPinValue( 1.0 - ( analogRead( C_RIGHT ) / float( VISUINO_ANALOG_IN_RANGE )), AChangeOnly );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		int PIN_DIRECTION,
		int PIN_SPEED,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class MotorShieldDirectionSpeedChannel :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
			UpdateOutputs();
		}

	public:
		void UpdateOutputs()
		{
			if( Enabled() )
			{
				float AInitialValue = InitialValue().GetValue();
				float AOutSpeed = fabs( AInitialValue - 0.5 ) * 2;
				bool ADirection = AInitialValue > 0.5;

				Analog.Write( PIN_SPEED, AOutSpeed );
				digitalWrite( PIN_DIRECTION, ADirection ? HIGH : LOW );
			}

			else
			{
				Analog.WriteRaw( PIN_SPEED, 0 );
				digitalWrite( PIN_DIRECTION, LOW );
			}
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
			pinMode( 3, OUTPUT );
			digitalWrite( 3, HIGH );
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for *_Slopped compatibility

	public:
		MotorShieldDirectionSpeedChannel()
		{
			pinMode( PIN_SPEED, OUTPUT );
			pinMode( PIN_DIRECTION, OUTPUT );
		}
	};

//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		int PIN_DIRECTION,
		int PIN_SPEED,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class MotorShieldDirectionSpeedChannel_Slopped :
		public T_Enabled,
		public T_InitialValue,
		public T_SlopeRun,
		public T_SlopeStop
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )
		_V_PROP_( SlopeRun )
		_V_PROP_( SlopeStop )

	protected:
		float	FCurrentValue = 0.0f;
		unsigned long	FLastTime = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
			UpdateOutputs();
		}

	public:
		void UpdateOutputs()
		{
			if( Enabled() )
			{
				float AInitialValue = InitialValue().GetValue();
				float AOutSpeed = fabs( AInitialValue - 0.5 ) * 2;
				bool ADirection = AInitialValue > 0.5;

				Analog.Write( PIN_SPEED, AOutSpeed );
				digitalWrite( PIN_DIRECTION, ADirection ? HIGH : LOW );
			}

			else
			{
				Analog.WriteRaw( PIN_SPEED, 0 );
				digitalWrite( PIN_DIRECTION, LOW );
			}
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();
			UpdateOutputs();
			pinMode( 3, OUTPUT );
			digitalWrite( 3, HIGH );
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros )
		{
			float ATargetValue = ( Enabled() ) ? InitialValue() : 0.5;
			if( FCurrentValue != ATargetValue )
			{
				float ASlope = SlopeRun();
				if( FCurrentValue > ATargetValue )
				{
					if( ATargetValue > 0.5 )
						ASlope = SlopeStop();
				}

				else
				{
					if( ATargetValue < 0.5 )
						ASlope = SlopeStop();
				}

				if( ASlope == 0 )
					FCurrentValue = ATargetValue;

				else
				{
					float ARamp = fabs( AElapsedMicros * ASlope / 1000000 );
					if( FCurrentValue < ATargetValue )
					{
						FCurrentValue += ARamp;
						if( FCurrentValue > ATargetValue )
							FCurrentValue = ATargetValue;

					}
					else
					{
						FCurrentValue -= ARamp;
						if( FCurrentValue < ATargetValue )
							FCurrentValue = ATargetValue;

					}
				}

				UpdateOutputs();
			}
		}

	public:
		MotorShieldDirectionSpeedChannel_Slopped()
		{
			pinMode( PIN_SPEED, OUTPUT );
			pinMode( PIN_DIRECTION, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif