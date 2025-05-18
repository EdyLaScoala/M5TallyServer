////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Display_ST7735.h>

namespace Mitov
{
namespace ST7735
{
//---------------------------------------------------------------------------
static const uint8_t PROGMEM Rcmd2red[] = 
	{              // Init for 7735R, part 2 (red tab only)
		2,                        //  2 commands in list:
		ST7735::CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
		  0x00, 0x00,             //     XSTART = 0
		  0x00, 0x7F,             //     XEND = 127
		ST7735::RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
		  0x00, 0x00,             //     XSTART = 0
		  0x00, 0x9F 
	};           //     XEND = 159
//---------------------------------------------------------------------------
} // ST7735
}
