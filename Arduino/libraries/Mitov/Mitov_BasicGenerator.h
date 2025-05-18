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
		typename T_CompleteCycle,
		typename T_FCycleCompleted,
		typename T_Reset
	> class TArduinoGeneratorWhenDisabled :
		public T_CompleteCycle,
		public T_FCycleCompleted,
		public T_Reset
	{
	public:
		_V_PROP_( CompleteCycle )
		_V_PROP_( Reset )

	public:
		_V_PROP_( FCycleCompleted )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Amplitude,
		typename T_Asymmetry,
		typename T_BASE,
//		Mitov::TElapsedMicros &C_ElapsedMicros,
		typename T_Enabled, 
		typename T_Frequency,
		typename T_Offset,
		typename T_OutputPin,
		typename T_Phase,
		typename T,
		typename T_WhenDisabled
	> class TypedFrequencyGenerator :
		public T_BASE,
		public T_Amplitude,
		public T_Asymmetry,
		public T_Offset,
		public T_Frequency,
		public T_Phase,
		public T_WhenDisabled,
		public T_Enabled, 
		public T_OutputPin
	{
	public:
		typedef T T_DATA;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Frequency )
		_V_PROP_( Amplitude )
		_V_PROP_( Asymmetry )
		_V_PROP_( Offset )

		// 0 - 1
		_V_PROP_( Phase )

		_V_PROP_( WhenDisabled )

	public: // Needs to be public for the base class
		float	FPhase = 0.0f;

	public:
		inline void UpdateEnabled()
		{
//			Serial.println( Enabled() );
			if( Enabled() )
			{
				WhenDisabled().FCycleCompleted() = false;
//				FLastTime = micros();
				if( WhenDisabled().Reset() )
					FPhase = 0.0;

//				Generate( false );
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
			WhenDisabled().FCycleCompleted() = true;
			FPhase = 0;
			Generate( false );
		}

	public:
		inline void SystemStartInit()
		{
			FPhase = 0.0f; //Phase();
		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
			Generate( false );
		}

		inline void SystemLoopBeginPhase( unsigned long AElapsedMicros )
		{
			if( ( Enabled().GetValue() || ( ! WhenDisabled().FCycleCompleted().GetValue() ) ) && ( Frequency() != 0 ) )
			{
//				Serial.println( "." );
				float APeriod = 1000000 / Frequency();
				float ATime = AElapsedMicros; //C_ElapsedMicros.GetElapsedTime();
				ATime /= APeriod;
				float APhase = FPhase + ATime;
				APhase = V_FMOD( APhase, 1 );
				if( ! Enabled().GetValue() )
					if( WhenDisabled().CompleteCycle() )
					{
						if( FPhase == 0.0 )
						{
//							Serial.println( "STEP1" );
							ResetOutput();
							return;
						}

						if( APhase < FPhase )
						{
//							Serial.println( "STEP3" );
//							Serial.println( APhase );
//							Serial.println( FPhase );
							ResetOutput();
							return;
						}

					}

				FPhase = APhase;
			}
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
			if( ! Enabled() )
				if( WhenDisabled().FCycleCompleted().GetValue() )
					return;

			T AValue = T_BASE::CalculateValue( this );
			T_OutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			FPhase = 0.0f;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif