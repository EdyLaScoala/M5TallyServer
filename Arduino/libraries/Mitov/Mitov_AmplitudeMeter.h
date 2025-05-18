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
	const float FLT_MAX = 3.4028235E+38;

	template <
		typename T_Enabled,
		typename T_MaxOutputPin,
		typename T_MinOutputPin,
		typename T_OutputPin,
		typename T_Period
	> class AmplitudeMeter : 
		public T_Enabled,
		public T_MaxOutputPin, 
		public T_MinOutputPin, 
		public T_OutputPin,
		public T_Period
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( MinOutputPin )
		_V_PIN_( MaxOutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Period )

	protected:
		unsigned long	FStartTime = 0;
		double			FAccumulator = 0.0f;
		float			FMin = 0;
		float			FMax = 0;
		unsigned long	FCount = 0;

	public:
		void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
				return;

			float AValue = *(float *)_Data;
			FAccumulator += AValue;
			if( FCount == 0 )
			{
				FMin = AValue;
				FMax = AValue;
			}

			else
			{
				if( AValue < FMin )
					FMin = AValue;

				if( AValue > FMax )
					FMax = AValue;

			}

			++FCount;

			unsigned long ANow = micros();
			unsigned long APeriod = ANow - FStartTime;

			if( APeriod < ((unsigned long)Period() ) * 1000 )
				return;

			FStartTime = ANow;
			if( FCount == 0 )
				--FCount;

			float AAverage = FAccumulator / FCount;
			FAccumulator = 0;
			FCount = 0;

			T_OutputPin::SetPinValue( AAverage );
			T_MinOutputPin::SetPinValue( FMin );
			T_MaxOutputPin::SetPinValue( FMax );
		}

	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif