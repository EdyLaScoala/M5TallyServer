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

namespace Mitov
{
	template <
		typename T_Enabled,
		int PIN_NUMBER
	> class M5Stack_StickC_Speaker_Hat : 
		public DirectOuputPin<PIN_NUMBER>,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void UpdateEnabled()
		{
			digitalWrite( 0, ( Enabled().GetValue() ) ? HIGH : LOW );
		}

	public:
		inline void SystemInit()
		{
			pinMode( 0, OUTPUT );
			UpdateEnabled();
		}

	};
//---------------------------------------------------------------------------
}
