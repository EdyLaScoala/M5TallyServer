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
		typename T_OutputPin
	> class CustomClockOutput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Clock()
		{
			T_OutputPin::ClockPin();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_DATA
	> class CustomOutput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline CustomOutput<T_OutputPin, T_DATA> & operator = (T_DATA AData ) { Send( AData ); return *this; }

	public:
		inline void Send( T_DATA AData )
		{
			T_OutputPin::SetPinValue( AData );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif