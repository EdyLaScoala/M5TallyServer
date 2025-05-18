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
		typename T_DebounceInterval,
		typename T_FLastValue,
		typename T_FValue,
		typename T_OutputPin
	> class Button : 
		public T_DebounceInterval,
		public T_FLastValue,
		public T_FValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		unsigned long	FLastTime = 0;
		_V_PROP_( FLastValue )
		_V_PROP_( FValue )

	public:
		_V_PROP_( DebounceInterval )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			if( AValue != FLastValue() )
				FLastTime = millis();

			FLastValue() = AValue;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FValue() != FLastValue() )
				if( millis() - FLastTime > DebounceInterval() )
				{
					FValue() = FLastValue();
					T_OutputPin::SetPinValue( FValue() );
				}

		}

		inline void SystemStart()
		{
			FValue() = FLastValue();
			T_OutputPin::SetPinValue( FValue() );
		}

	public:
		inline Button()
		{
			FLastValue() = false;
			FValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FAfterInitialDelay,
		typename T_FCurrentValue,
		typename T_FFirstTimePassed,
		typename T_InitialDelay,
		typename T_OutputPin,
		typename T_RepeatFrequency
	> class AutoRepeatButton : 
		public T_Enabled,
		public T_FAfterInitialDelay,
		public T_FCurrentValue,
		public T_FFirstTimePassed,
		public T_InitialDelay,
		public T_OutputPin,
		public T_RepeatFrequency
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialDelay )
		_V_PROP_( RepeatFrequency )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FFirstTimePassed )
		_V_PROP_( FCurrentValue )
		_V_PROP_( FAfterInitialDelay )
		unsigned long	FLastTime = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			if( ! Enabled() )
			{
				T_OutputPin::SetPinValue( AValue );
				return;
			}

			if( FFirstTimePassed() )
				if( AValue == FCurrentValue() )
					return;

			FFirstTimePassed() = true;
			FCurrentValue() = AValue;

			if( AValue )
			{
				FLastTime = millis();
				FAfterInitialDelay() = false;
			}


			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! FCurrentValue() )
				return;

			unsigned long ACurrentTime = millis();
			if( FAfterInitialDelay() )
			{
				unsigned long ARepeatDelay = 1000 / RepeatFrequency();
				if( ACurrentTime - FLastTime >= ARepeatDelay )
				{
					T_OutputPin::SetPinValueLow();
					T_OutputPin::SetPinValueHigh();
//					OutputPin.template SendValue<bool>( false );
//					OutputPin.template SendValue<bool>( true );
					FLastTime = ACurrentTime;
				}

				return;
			}

			if( ACurrentTime - FLastTime >= InitialDelay() )
			{
				FLastTime = ACurrentTime;
				FAfterInitialDelay() = true;
			}
		}

	public:
		inline AutoRepeatButton()
		{
			FFirstTimePassed() = false;
			FCurrentValue() = false;
			FAfterInitialDelay() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

