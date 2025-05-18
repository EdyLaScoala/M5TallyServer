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
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin
	> class SpeedChangeMeter :
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FHasTime )
		unsigned long	FLastTime = 0;
		float			FLastValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
				return;

			float AValue = *(float *)_Data;
			unsigned long	ATime = micros();
			if( ! FHasTime() )
				FHasTime() = true;

			else
			{
				float ASpeed = AValue - FLastValue;
				ASpeed /= ( ATime - FLastTime ) / 1000000.0f;
				T_OutputPin::SetPinValue( ASpeed );
			}

			FLastValue = AValue;
			FLastTime = ATime;
		}

	public:
		inline SpeedChangeMeter()
		{
			FHasTime() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif