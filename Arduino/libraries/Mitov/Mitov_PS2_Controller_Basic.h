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
		typename T_OWNER, T_OWNER &C_OWNER,
		int C_INDEX,
		typename T_OutputPin
	> class PS2DigitalSensor :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Process( bool AChangeOnly )
		{
			bool AValue = C_OWNER.ReadDigital( C_INDEX );
			T_OutputPin::SetPinValue( AValue, AChangeOnly );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		int C_INDEX,
		typename T_OutputPin
	> class PS2AnalogSensor :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Process( bool AChangeOnly )
		{
			float AValue = C_OWNER.ReadAnalog( C_INDEX );
			T_OutputPin::SetPinValue( AValue, AChangeOnly );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif