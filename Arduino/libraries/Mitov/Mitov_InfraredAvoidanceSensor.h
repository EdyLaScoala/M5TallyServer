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
		typename T_EnableDelay,
		typename T_EnableOutputPin,
		typename T_Enabled,
		typename T_FCurrentValue,
		typename T_FDisabled,
		typename T_OutputPin
	> class InfraredAvoidanceSensor :
		public T_EnableDelay,
		public T_EnableOutputPin,
		public T_Enabled,
		public T_FCurrentValue,
		public T_FDisabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( EnableOutputPin )

	public:
		_V_PROP_( EnableDelay )
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FDisabled )
		_V_PROP_( FCurrentValue )
		unsigned long	FStartTime = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FCurrentValue() = *(bool *)_Data;
			if( ! FDisabled() )
				T_OutputPin::SetPinValue( FCurrentValue() );
		}

		inline void EnableInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( AValue == Enabled() )
				return;

			Enabled() = AValue;
			TryStartTimer();
		}

	protected:
		void TryStartTimer()
		{
			if( Enabled() )
			{
				FDisabled() = true;
				FStartTime = millis();
			}

			T_EnableOutputPin::SetPinValue( Enabled() );
		}

	public:
		inline void SystemInit()
		{
			TryStartTimer();
		}

		inline void SystemLoopBegin()
		{
			if( FDisabled() )
			{
				unsigned long	currentMilis = millis();
				if( currentMilis - FStartTime >= EnableDelay() )
				{
					FDisabled() = false;
					T_OutputPin::SetPinValue( FCurrentValue() );
				}
			}
		}

	public:
		inline InfraredAvoidanceSensor()
		{
			FCurrentValue() = false;
			FDisabled() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
