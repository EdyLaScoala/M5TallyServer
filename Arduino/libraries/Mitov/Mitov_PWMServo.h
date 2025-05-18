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
#include <PWMServo.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER,
		typename T_Reversed
	> class MitovPWMServo :
		public T_Enabled,
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse,
		public T_Reversed
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )
		_V_PROP_( Reversed )

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
			{
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
				UpdatePosition();
//				InputPin_o_Receive( &InitialValue );
			}
			else
				FServo.detach();
		}

	protected:
		PWMServo	FServo;
		
	protected:
		inline void UpdatePosition()
		{
			float AValue = MitovConstrain( InitialValue().GetValue(), 0.0f, 1.0f );
			if( Reversed() )
				AValue = 1.0 - AValue;

			AValue *= 180;
			FServo.write( AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *((float *)_Data);
			if( Enabled() )
				UpdatePosition();

		}

	public:
		inline void SystemStart()
		{
			if( Enabled() )
			{
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
				UpdatePosition();
			}
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_Enabled,
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER,
		typename T_Reversed
	> class PWMServoShieldElement :
//		public T_Enabled,
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse,
		public T_Reversed
	{
	public:
//		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )
		_V_PROP_( Reversed )

	protected:
		PWMServo	FServo;

	public:
		inline void SystemInit( float AInitialValue )
		{
			FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
			InputPin_o_Receive( &AInitialValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = MitovConstrain( *((float *)_Data), 0.0f, 1.0f );
			if( Reversed() )
				AValue = 1.0 - AValue;

			AValue *= 180;
			FServo.write( AValue );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif