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
#include <Mitov_Stepper.h>
#include <Mitov_TexasInstruments_74HC595.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_OWNER, uint8_t C_STEP0, uint8_t C_STEP1, uint8_t C_STEP2, uint8_t C_STEP3> class AdafruitMotorDriverStepperInterface : public StepperMotor4Wire
	{
	public:
		inline void SetChannelValue( uint8_t AIndex, bool AValue )
		{
			static const uint8_t Pins[] = { C_STEP0, C_STEP1, C_STEP2, C_STEP3 };
			C_OWNER.SetChannelValue( Pins[ AIndex ], AValue );
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_STEP0,
		uint8_t C_STEP1,
		uint8_t C_STEP2,
		uint8_t C_STEP3,
		typename T_Enabled,
		typename T_FStep,
		typename T_HalfStep,
		typename T_Reversed,
		typename T_Speed_Implementation,
		typename T_StepsPerSecond
	> class AdafruitMotorDriverStepper :
		public T_Enabled,
		public T_FStep,
		public T_HalfStep,
		public T_Reversed,
		public T_Speed_Implementation,
		public T_StepsPerSecond
	{
	public:
		_V_PROP_( FStep )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reversed )
		_V_PROP_( HalfStep )
		_V_PROP_( StepsPerSecond )

	public:
		inline void StepInputPin_o_Receive( void *_Data )
		{
			Step();
		}

	public:
		void UpdatePins()
		{
			static const uint8_t Pins[] = { C_STEP0, C_STEP1, C_STEP2, C_STEP3 };
			uint8_t AValue = HalfStep() ? C_StepperBits8[ FStep() & 0b0111 ] : C_StepperBits4[ FStep() & 0b0011 ];
			for( int i = 0; i < 4; ++i )
			{
				bool APinValue = ( AValue & 1 );

				C_OWNER.SetChannelValue( Pins[ i ], APinValue );
				AValue >>= 1;
			}
		}

	public:
		inline void SystemStart()
		{
			T_Speed_Implementation::SystemStart();
		}

		inline void SystemLoopBegin()
		{
			T_Speed_Implementation::SystemLoopBegin( this );
		}

	public:
		void Step()
		{
			if( Reversed() ) //^ ( StepsPerSecond > 0 ) )
				FStep() = FStep() - 1;

			else
				FStep() = FStep() + 1;

			UpdatePins();
		}

	public:
		inline AdafruitMotorDriverStepper()
		{
			FStep() = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_BACK,
		uint8_t C_FORWARD,
		uint8_t C_SPEED,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class AdafruitMotorDriverChannel :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

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
				float AOutSpeed = fabs( InitialValue() - 0.5 ) * 2;
				bool ADirection = InitialValue() > 0.5;

				C_OWNER.SetChannelValue( C_BACK, ADirection );
				ADirection = ! ADirection;

				C_OWNER.SetChannelValue( C_FORWARD, ADirection );

				Analog.Write( C_SPEED, AOutSpeed );
			}

			else
			{
				C_OWNER.SetChannelValue( C_BACK, false );
				C_OWNER.SetChannelValue( C_FORWARD, false );
				Analog.WriteRaw( C_SPEED, 0 );
			}

		}

	public:
		inline void SystemStart()
		{
			pinMode( C_SPEED, OUTPUT );
			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for *_Slopped compatibility
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_BACK,
		uint8_t C_FORWARD,
		uint8_t C_SPEED,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class AdafruitMotorDriverChannel_Slopped :
		public T_Enabled,
		public T_InitialValue,
		public T_SlopeRun,
		public T_SlopeStop
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( SlopeRun )
		_V_PROP_( SlopeStop )

	protected:
		float	FCurrentValue = 0.0f;

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
				float AOutSpeed = fabs( FCurrentValue - 0.5 ) * 2;
				bool ADirection = FCurrentValue > 0.5;

				C_OWNER.SetChannelValue( C_BACK, ADirection );
				ADirection = ! ADirection;

				C_OWNER.SetChannelValue( C_FORWARD, ADirection );

				Analog.Write( C_SPEED, AOutSpeed );
			}
			else
			{
				C_OWNER.SetChannelValue( C_BACK, false );
				C_OWNER.SetChannelValue( C_FORWARD, false );
				Analog.WriteRaw( C_SPEED, 0 );
			}

		}

	public:
		inline void SystemStart()
		{
			pinMode( C_SPEED, OUTPUT );
			FCurrentValue = InitialValue();

			UpdateOutputs();
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

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER, uint8_t C_STEP0, uint8_t C_STEP1, uint8_t C_STEP2, uint8_t C_STEP3> class AdafruitMotorDriverDirectControl
	{
	public:
		inline void SetChannelValue( uint8_t AIndex, bool AValue )
		{
			static const uint8_t Pins[] = { C_STEP0, C_STEP1, C_STEP2, C_STEP3 };
			C_OWNER.SetChannelValue( Pins[ AIndex ], AValue );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class AdafruitMotorDriverDCMotors
	{
	public:
		inline void SetChannelValue( uint8_t AIndex, bool AValue )
		{
			C_OWNER.SetChannelValue( AIndex, AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled
	> class AdafruitMotorDriverControl :
		public T_Enabled
	{
	private:
		static const uint8_t MOTORLATCH = 12;
		static const uint8_t MOTORCLK = 4;
		static const uint8_t MOTORDATA = 8;
		static const uint8_t MOTORENABLE = 7;

	public:
		_V_PROP_( Enabled )

	protected:
		uint8_t	FBits = 0;
		uint8_t	FCurrentBits = 0;

	public:
		inline void UpdateEnabled()
		{
			digitalWrite( MOTORENABLE, Enabled ? LOW : HIGH );
		}

	protected:
		void UpdateAll()
		{
			FCurrentBits = FBits;

			digitalWrite( MOTORLATCH, LOW);
			digitalWrite(MOTORDATA, LOW);

			for ( uint8_t i=0; i<8; ++i )
			{
				digitalWrite( MOTORCLK, LOW );
				digitalWrite( MOTORDATA, ( FBits & ( 1 << ( 7-i )) ) ? HIGH : LOW );
				digitalWrite(MOTORCLK, HIGH);
			}

			digitalWrite(MOTORLATCH, HIGH);
		}

	public:
		void SetChannelValue( uint8_t AIndex, bool AValue )
		{
			if( AValue )
				FBits |= ( (uint8_t)1 ) << AIndex;

			else
				FBits &= ~( ((uint8_t)1 ) << AIndex );
		}

	public:
		inline void SystemStart()
		{
			pinMode( MOTORLATCH, OUTPUT );
			pinMode( MOTORCLK, OUTPUT );
			pinMode( MOTORDATA, OUTPUT );
			pinMode( MOTORENABLE, OUTPUT );

			UpdateAll();
			digitalWrite( MOTORENABLE, Enabled() ? LOW : HIGH );
		}

		inline void SystemLoopUpdateHardware()
		{
			if( FCurrentBits != FBits )
				UpdateAll();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif