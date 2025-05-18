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
//---------------------------------------------------------------------------
	template <
		typename T_Averaging,
		typename T_Resolution
	> class TeensyAnalogInputSettings :
		public T_Averaging,
		public T_Resolution
	{
	public:
		_V_PROP_( Averaging )
		_V_PROP_( Resolution )

	public:
		inline void UpdateAveraging()
		{
			analogReadAveraging( Averaging().GetValue() );
		}

		inline void UpdateResolution()
		{
			Mitov::GAnalogInDivider = pow( 2, Resolution() ) - 1;
		}

	public:
		inline void SystemInit()
		{
			Mitov::GAnalogInDivider = pow( 2, Resolution() ) - 1;
			analogReadAveraging( Averaging().GetValue() );
		}

	};
//---------------------------------------------------------------------------
}

