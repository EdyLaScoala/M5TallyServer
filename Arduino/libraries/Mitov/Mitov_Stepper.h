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
	const uint8_t C_StepperBits4[] =
	{
			0b1100,
			0b0110,
			0b0011,
			0b1001
	};
//---------------------------------------------------------------------------
	const uint8_t C_StepperBits8[] =
	{
			0b1000,
			0b1100,
			0b0100,
			0b0110,
			0b0010,
			0b0011,
			0b0001,
			0b1001
	};
//---------------------------------------------------------------------------
	const uint8_t C_Stepper3Bits3[] =
	{
			0b110,
			0b011,
			0b101
	};
//---------------------------------------------------------------------------
	const uint8_t C_Stepper3Bits6[] =
	{
			0b100,
			0b110,
			0b010,
			0b011,
			0b001,
			0b101
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FLastTime,
		typename T_FStep,
		typename T_HalfStep,
		typename T_OutputPins0,
		typename T_OutputPins1,
		typename T_OutputPins2,
		typename T_OutputPins3,
		typename T_Reversed,
		typename T_StepsPerSecond
	> class StepperMotor4Wire :
		public T_Enabled,
		public T_FLastTime,
		public T_FStep,
		public T_HalfStep,
		public T_OutputPins0,
		public T_OutputPins1,
		public T_OutputPins2,
		public T_OutputPins3,
		public T_Reversed,
		public T_StepsPerSecond
	{
	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )
		_V_PIN_( OutputPins2 )
		_V_PIN_( OutputPins3 )

	public:
		_V_PROP_( FStep )
		_V_PROP_( FLastTime )

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
			uint8_t AValue = HalfStep() ? C_StepperBits8[ FStep() & 0b0111 ] : C_StepperBits4[ FStep() & 0b0011 ];
			for( int i = 0; i < 4; ++i )
			{
				bool APinValue = ( AValue & 1 );

				switch( i )
				{
					case 0 : T_OutputPins0::SetPinValue( APinValue ); break;
					case 1 : T_OutputPins1::SetPinValue( APinValue ); break;
					case 2 : T_OutputPins2::SetPinValue( APinValue ); break;
					case 3 : T_OutputPins3::SetPinValue( APinValue ); break;
				}

				AValue >>= 1;
			}
		}

	public:
		inline void SystemStart()
		{
			FLastTime() = micros();
		}

		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ! Enabled() )
				FLastTime() = ACurrentMicros;

			else
			{
				unsigned long APeriod = ( 1000000 / fabs( StepsPerSecond() )) + 0.5;
				if( ( ACurrentMicros - FLastTime().GetValue() ) >= APeriod )
				{
					FLastTime() = FLastTime().GetValue() + APeriod;
					if( StepsPerSecond() )
					{
						if(( StepsPerSecond() > 0 ) ^ Reversed() )
							FStep() = FStep() + 1;

						else
							FStep() = FStep() - 1;

						UpdatePins();
					}
				}
			}
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
		inline StepperMotor4Wire()
		{
			FStep() = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
//		int C_NUMOUTPINS,
		typename T_Enabled,
		typename T_FLastTime,
		typename T_FStep,
		typename T_HalfStep,
		typename T_OutputPins0,
		typename T_OutputPins1,
		typename T_OutputPins2,
		typename T_Reversed,
//		typename T_Speed_Implementation,
		typename T_StepsPerSecond
	> class StepperMotor3Wire :
		public T_Enabled,
		public T_FLastTime,
		public T_FStep,
		public T_HalfStep,
		public T_OutputPins0,
		public T_OutputPins1,
		public T_OutputPins2,
		public T_Reversed,
//		public T_Speed_Implementation,
		public T_StepsPerSecond
	{
	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )
		_V_PIN_( OutputPins2 )

	public:
		_V_PROP_( FStep )
		_V_PROP_( FLastTime )

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
			uint8_t AValue = HalfStep() ? C_Stepper3Bits3[ FStep() % 3 ] : C_Stepper3Bits6[ FStep() % 6 ];
			for( int i = 0; i < 3; ++i )
			{
				bool APinValue = ( AValue & 1 );

				switch( i )
				{
					case 0 : T_OutputPins0::SetPinValue( APinValue ); break;
					case 1 : T_OutputPins1::SetPinValue( APinValue ); break;
					case 2 : T_OutputPins2::SetPinValue( APinValue ); break;
				}

				AValue >>= 1;
			}
		}

	public:
		inline void SystemStart()
		{
			FLastTime() = micros();
		}

		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ! Enabled() )
				FLastTime() = ACurrentMicros;

			else
			{
				unsigned long APeriod = ( 1000000 / fabs( StepsPerSecond() )) + 0.5;
				if( ( ACurrentMicros - FLastTime().GetValue() ) >= APeriod )
				{
					FLastTime() = FLastTime().GetValue() + APeriod;
					if( StepsPerSecond() )
					{
						if(( StepsPerSecond() > 0 ) ^ Reversed() )
							FStep() = FStep() + 1;

						else
							FStep() = FStep() - 1;

						UpdatePins();
					}
				}
			}
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
		inline StepperMotor3Wire()
		{
			FStep() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif