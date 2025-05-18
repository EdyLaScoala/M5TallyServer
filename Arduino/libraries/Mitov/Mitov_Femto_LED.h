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

namespace Mitov
{
//---------------------------------------------------------------------------
	class ArduinoFemtoLED
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			Analog.Write( 4, float( AColor.Red ) / 255.0 );
			Analog.Write( 3, float( AColor.Green ) / 255.0 );
			Analog.Write( 10, float( AColor.Blue ) / 255.0 );
		}

	};
//---------------------------------------------------------------------------
}
