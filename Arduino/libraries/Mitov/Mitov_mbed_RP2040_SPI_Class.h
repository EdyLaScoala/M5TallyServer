////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Arduino_SPI.h>

//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		MbedSPI &C_SPI
//		typename T_MISO,
//		typename T_MOSI,
//		typename T_SCK
	> class Arduino_RP2040_SPI :
		public ArduinoBasicSPI<MbedSPI, C_SPI>
//		public T_MISO,
//		public T_MOSI,
//		public T_SCK
	{
/*
	public:
		_V_PROP_( MISO )
		_V_PROP_( MOSI )
		_V_PROP_( SCK )

	public:
		inline void SystemInit()
		{
			C_SPI.begin( SCK(), MISO(), MOSI() );
		}
*/
	};
//---------------------------------------------------------------------------
} // Mitov
