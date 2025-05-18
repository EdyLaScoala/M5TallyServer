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
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_OutputPin
	> class M5StackKeyboardUnitI2C : 
		public T_Address,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ));
			while( C_I2C.available() )
				T_OutputPin::SetPinValue( C_I2C.read() );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

