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
	PROGMEM const IR_Button C_InfraredRemote_Taidacent_CarMP3[] =
	{
		{ 16753245, '\0' }, // CH-
		{ 16736925, '\0' }, // CH
		{ 16769565, '\0' }, // CH+

		{ 16720605, '\0' }, // Previous
		{ 16712445, '\0' }, // Next
		{ 16761405, '\0' }, // Play/Pause

		{ 16769055, '-' },
		{ 16754775, '+' },
		{ 16748655, '\r' }, // EQ

		{ 16738455, '0' }, 
		{ 16750695, '\0' }, // 100+
		{ 16756815, '\0' }, // 200+

		{ 16724175, '1' },
		{ 16718055, '2' },
		{ 16743045, '3' },

		{ 16716015, '4' },
		{ 16726215, '5' },
		{ 16734885, '6' },

		{ 16728765, '7' },
		{ 16730805, '8' },
		{ 16732845, '9' }
	};
//---------------------------------------------------------------------------
} // Mitov
