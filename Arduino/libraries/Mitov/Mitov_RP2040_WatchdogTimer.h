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
extern "C" {
#include <hardware/watchdog.h>
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_EnableCausedOutputPin,
		typename T_TimeoutOutputPin
	> class TArduinoRP2040WatchdogRebootReason :
		public T_EnableCausedOutputPin,
		public T_TimeoutOutputPin
	{
	public:
		_V_PIN_( EnableCausedOutputPin )
		_V_PIN_( TimeoutOutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_PauseOnDebug,
		typename T_Period,
		typename T_RebootReason,
		typename T_TimeLeftOutputPin
	> class RP2040_WatchdogTimer :
		public T_Enabled,
		public T_PauseOnDebug,
		public T_Period,
		public T_RebootReason,
		public T_TimeLeftOutputPin
	{
	public:
		_V_PIN_( TimeLeftOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( PauseOnDebug )
		_V_PROP_( Period )
		_V_PROP_( RebootReason )

	public:
		inline void UpdatePeriod()
		{
			watchdog_enable( Period().GetValue(), PauseOnDebug().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{			
			watchdog_update();
		}

	public:
		inline void SystemStart()
		{
			UpdatePeriod();
		}

		inline void SystemLoopBegin()
		{
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif