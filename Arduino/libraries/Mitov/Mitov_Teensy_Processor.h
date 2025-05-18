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
	class TeensyProcessorRestart
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			_restart_Teensyduino_();
		}

	};
//---------------------------------------------------------------------------
	class TeensyProcessorReboot
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			_reboot_Teensyduino_();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif