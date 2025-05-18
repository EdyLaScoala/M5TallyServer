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
		typename T_Enabled,
		uint8_t	C_INDEX,
		typename T_InitialValue
	> class M5StackIndexedLEDElement :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void SystemStart()
		{
			Wire.beginTransmission( C_OWNER.GetLEDsAddress() );
			Wire.write( C_OWNER.GetLEDsRegister() );
			Wire.write( C_INDEX );

			if( Enabled() )
			{
				Wire.write( uint8_t( InitialValue().GetValue() >> 16 ));
				Wire.write( uint8_t( InitialValue().GetValue() >> 8 ));
				Wire.write( uint8_t( InitialValue().GetValue() ));
			}

			else
			{
				for( int i = 0; i < 3; ++i )
					Wire.write( 0 );
			}
  
			Wire.endTransmission();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;
			SystemStart();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

