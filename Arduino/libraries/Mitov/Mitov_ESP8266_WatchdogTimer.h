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
#include <Esp.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled
	> class ESP8266_WatchdogTimer :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				ESP.wdtEnable( 1000 ); // Fake interval!

			else
				ESP.wdtDisable();

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				ESP.wdtFeed();
		}

		inline void SystemStart()
		{
			UpdateEnabled();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

