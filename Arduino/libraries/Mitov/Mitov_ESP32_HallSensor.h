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
		typename T_OutputPin
	> class ESP32HallSensorModule :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
			{
				float AValue = hallRead();
				T_OutputPin::SetPinValue( AValue );
			}
		}

	protected:
		inline void ReadSensor( bool AOnlyChange )
		{
			if( Enabled() )
			{
				float AValue = hallRead();
				T_OutputPin::SetPinValue( AValue, AOnlyChange );
			}
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

		inline void SystemStart()
		{
			ReadSensor( false );
		}

	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

