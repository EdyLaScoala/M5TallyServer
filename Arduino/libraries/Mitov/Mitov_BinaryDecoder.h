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
		int C_NUM_OUTPUTS,
		typename T_InitialValue,
		typename T_OutputPins_SetPinValue,
		typename T_ValueBar
	> class BinaryDecoder :
		public T_InitialValue,
		public T_ValueBar
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( ValueBar )

	public:
		void UpdateValueBar()
		{
			for( int i = 0; i < InitialValue().GetValue(); ++i )
			{
				if( i >= C_NUM_OUTPUTS )
					break;

				T_OutputPins_SetPinValue::Call( i, ValueBar() );
			}
		}

	protected:
		inline void SetValue( uint32_t AValue )
		{
			uint32_t AOldValue = InitialValue().GetValue();
			InitialValue() = AValue;
			if( ValueBar() )
			{
				if( AOldValue < InitialValue().GetValue() )
				{
					for( int i = AOldValue + 1; i <= InitialValue().GetValue(); ++i )
					{
						if( i >= C_NUM_OUTPUTS )
							break;

						T_OutputPins_SetPinValue::Call( i, true );
					}
				}

				else
				{
					for( int i = InitialValue().GetValue() + 1; i <= AOldValue; ++i )
					{
						if( i >= C_NUM_OUTPUTS )
							break;

						T_OutputPins_SetPinValue::Call( i, false );
					}
				}
			}

			else
				if( InitialValue().GetValue() < C_NUM_OUTPUTS )
					T_OutputPins_SetPinValue::Call( InitialValue().GetValue(), true );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			unsigned long AValue = *(uint32_t *)_Data;
			if( InitialValue().GetValue() == AValue )
				return;

			if( ! ValueBar() )
				if( InitialValue().GetValue() < C_NUM_OUTPUTS )
					T_OutputPins_SetPinValue::Call( InitialValue().GetValue(), false );

			SetValue( AValue );
		}

	public:
		inline void SystemInit()
		{
			if( ! ValueBar().GetValue() )
			{
				for( int i = 0; i < C_NUM_OUTPUTS; ++i )
					T_OutputPins_SetPinValue::Call( i, i == InitialValue().GetValue() );
			}

			else
			{
				for( int i = 0; i < C_NUM_OUTPUTS; ++i )
					T_OutputPins_SetPinValue::Call( i, i <= InitialValue().GetValue() );
			}
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
