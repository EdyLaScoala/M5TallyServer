////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_InfraredRemote_Structure.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	PROGMEM const IR_Button C_InfraredRemote_YahBoom[] =
	{
		{ 16711935, '\0' }, // Power
		{ 16744575, '\0' }, // Up
		{ 16728255, '\0' }, // Light

		{ 16720095, '\0' }, // Left
		{ 16752735, '\0' }, // Sound
		{ 16736415, '\0' }, // Right

		{ 16716015, '\0' }, // Turn Left
		{ 16748655, '\0' }, // Down
		{ 16732335, '\0' }, // Turn Right

		{ 16724175, '+' },
		{ 16756815, '0' },
		{ 16740495, '-' },

		{ 16713975, '1' },
		{ 16746615, '2' },
		{ 16730295, '3' },

		{ 16722135, '4' },
		{ 16754775, '5' },
		{ 16738455, '6' },

		{ 16718055, '7' },
		{ 16750695, '8' },
		{ 16734375, '9' }
	};
//---------------------------------------------------------------------------
} // Mitov
