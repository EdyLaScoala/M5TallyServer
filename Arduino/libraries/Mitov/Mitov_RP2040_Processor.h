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
	class RP2040ProcessorReset
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			#define AIRCR_Register (*((volatile uint32_t*)(PPB_BASE + 0x0ED0C)))
//			AIRCR_Register = 0x5FA0004;
			(*((volatile uint32_t*)(PPB_BASE + 0x0ED0C))) = 0x5FA0004;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif