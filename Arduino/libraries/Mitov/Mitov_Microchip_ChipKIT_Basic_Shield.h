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
	class Microchip_ChipKITBasicShieldLeds
	{
	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			digitalWrite( AIndex, ( *(bool *)_Data ) ? HIGH : LOW );
		}

	public:
		inline void SystemStart()
		{
			for( int i = 26; i < 34; ++i )
			{
				pinMode( i, OUTPUT );
				digitalWrite( i, LOW );
			}
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif