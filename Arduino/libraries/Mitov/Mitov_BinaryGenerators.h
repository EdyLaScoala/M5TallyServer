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
#include <Mitov_BasicGenerator.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_Asymmetry,
		typename T_Enabled,
		typename T_FPhase,
		typename T_FValue,
		typename T_Frequency,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Phase,
		typename T_WhenDisabled
	> class PulseGenerator :
		public T_Asymmetry,
		public T_Enabled,
		public T_FPhase,
		public T_FValue,
		public T_Frequency,
		public T_InitialValue,
		public T_OutputPin,
		public T_Phase,
		public T_WhenDisabled
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Frequency )

		// -1 - 1
		_V_PIN_( Asymmetry )

		// 0 - 1
		_V_PIN_( Phase )

		_V_PIN_( WhenDisabled )

	protected:
		_V_PROP_( FValue )
		_V_PROP_( FPhase )

	protected:
		unsigned long FLastTime = 0;


	public:
		inline void UpdatePhase()
		{
			FPhase() = Phase();
		}

		inline void UpdateEnabled()
		{
//			Serial.println( Enabled() );
			if( Enabled() )
			{
				WhenDisabled().FCycleCompleted() = false;
				FLastTime = micros();
				if( WhenDisabled().Reset() )
					FPhase() = Phase();

			}

			else if( WhenDisabled().Reset() )
			{
//				Serial.println( "RESET" );
				ResetOutput();
//				T_OutputPin::SetPinValue( InitialValue(), true );
			}
		}

	protected:
		void ResetOutput()
		{
			T_OutputPin::SetPinValue( InitialValue(), true );
			WhenDisabled().FCycleCompleted() = true;
			FPhase() = Phase();
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			FPhase() = Phase();
			FLastTime = micros();
			T_OutputPin::SetPinValue( InitialValue(), true );
		}

	public:
		inline void SystemLoopBegin()
		{
			if( Frequency() == 0 )
				return;

			if( ( Enabled().GetValue() || ( ! WhenDisabled().FCycleCompleted().GetValue() ) ) && ( Frequency() != 0 ) )
			{
				if( T_FPhase::GetIsConstant() )
				{
					unsigned long APeriod = ( (( 1 / ( Frequency() * 2 )) * 1000000 ) + 0.5 ); // 2 times one for each switch
//					Serial.println( APeriod );
					while( micros() - FLastTime >= APeriod )
					{
						FValue() = ! FValue();

						if( ! Enabled().GetValue() )
							if( WhenDisabled().CompleteCycle() )
								if( FValue() == InitialValue() )
								{
									ResetOutput();
									break;
								}

//						Serial.println( "PULSE" );
						T_OutputPin::SetPinValue( FValue(), true );

						FLastTime += APeriod;
					}

				}

				else
				{
					unsigned long ACurrentMicros = micros();
					float APeriod = 1000000 / Frequency();

					float ATime = ( ACurrentMicros - FLastTime );
					ATime /= APeriod;
					float APhase = FPhase() + ATime;

					APhase = V_FMOD( APhase, 1 );

					if( ! Enabled().GetValue() )
						if( WhenDisabled().CompleteCycle() )
						{
							if( FPhase() == Phase() )
							{
//								Serial.println( "STEP1" );
								ResetOutput();
								return;
							}

							if( FPhase() < Phase() )
								if( APhase >= Phase() )
								{
//									Serial.println( "STEP3" );
									ResetOutput();
									return;
								}

						}

					FPhase() = APhase;

					bool AValue = ( FPhase() < 0.5 + Asymmetry() / 2 );
					if( AValue != FValue() )
					{
						FValue() = AValue;
						T_OutputPin::SetPinValue( FValue(), true );
					}

					FLastTime = ACurrentMicros;
				}
			}
		}

		inline void SystemStart()
		{
			FPhase() = Phase();
			FLastTime = micros();
			FValue() = InitialValue();
			WhenDisabled().FCycleCompleted() = ! Enabled().GetValue();
			T_OutputPin::SetPinValue( FValue(), false );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif