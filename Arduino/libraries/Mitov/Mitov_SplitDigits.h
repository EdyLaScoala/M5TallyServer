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
#include "Mitov_RandomGenerator.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Base,
		uint8_t COUNT_OutputPins,
		typename T_FillValue,
		typename T_OutputPins_SetPinValue
	> class SplitIntegerDigits : 
		public T_Base,
		public T_FillValue
	{
	public:
		_V_PROP_( Base )
		_V_PROP_( FillValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t *)_Data;
//			for( int i = COUNT_OutputPins; i--; )
			for( int i = 0; i < COUNT_OutputPins; ++i )
			{
				uint32_t ADigit = AValue % Base().GetValue();
				AValue /= Base().GetValue();

				if( ! FillValue().GetValue() )
					T_OutputPins_SetPinValue::Call( i, ADigit );

				else
				{
					if( ( i ) && ( ! AValue ) && ( ! ADigit ) )
						T_OutputPins_SetPinValue::Call( i, FillValue().GetValue() );

					else
						T_OutputPins_SetPinValue::Call( i, ADigit );

				}
			}
		}

	};	
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif