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
		typename T_ClockInputPin_o_IsConnected,
		typename T_DerivativeGain,
		typename T_Enabled,
		typename T_InitialOutput,
		typename T_InitialValue,
		typename T_IntegralGain,
		typename T_OutputPin,
		typename T_ProportionalGain,
		typename T_SetPoint
	> class PIDController :
		public T_ClockInputPin_o_IsConnected,
		public T_DerivativeGain,
		public T_Enabled,
		public T_InitialOutput,
		public T_InitialValue,
		public T_IntegralGain,
		public T_OutputPin,
		public T_ProportionalGain,
		public T_SetPoint
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( ProportionalGain )
		_V_PROP_( IntegralGain )
		_V_PROP_( DerivativeGain )

		_V_PROP_( SetPoint )

		_V_PROP_( InitialOutput )
		_V_PROP_( InitialValue )

	protected:
		unsigned long	FLastTime;
		float	FLastInput;
		float	FITerm;

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				Initialize();

		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float *)_Data;
		}

		void ManualControlInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				return;

			InitialOutput() = *(float *)_Data;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( InitialOutput() );
		}

	protected:
		void Initialize()
		{
			FITerm = InitialOutput();
			FLastInput = InitialValue();
			FLastTime = micros();
		}

	public:
		inline void SystemStart()
		{
			Initialize();
			T_OutputPin::SetPinValue( InitialOutput() );
		}

		inline void SystemStartClock()
		{
			Initialize();
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) 
				return;

			unsigned long ACurrentMicros = micros();
			unsigned long timeChange = ( ACurrentMicros - FLastTime );
			float ANormalizedTime = float( timeChange ) / 1000000;

			// Compute all the working error variables
			double error = SetPoint() - InitialValue();
//			ITerm += ( ki * error ) * ANormalizedTime;
//			Serial.println( FITerm + ( IntegralGain * error ) * ANormalizedTime );

			FITerm = MitovConstrain<float>( FITerm + ( IntegralGain() * error ) * ANormalizedTime, -1.0f, 1.0f );
//			FITerm += ( IntegralGain * error ) * ANormalizedTime;

			if( ! ANormalizedTime )
				ANormalizedTime = 0.0000001;

			double dInput = ( InitialValue() - FLastInput ) / ANormalizedTime;
 
			// Compute PID Output
			float AOutput = MitovConstrain<float>( ProportionalGain() * error + FITerm - DerivativeGain() * dInput, 0.0f, 1.0f );
	  
			// Remember some variables for next time
			FLastInput = InitialValue();
			FLastTime = ACurrentMicros;

			if( AOutput == InitialOutput().GetValue() )
				return;

			InitialOutput() = AOutput;

			if( ! ClockInputPin_o_IsConnected() )
				T_OutputPin::SetPinValue( InitialOutput() );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif