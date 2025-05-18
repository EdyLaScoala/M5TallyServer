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
	template <
		typename T_OutputPins_Up,
		typename T_OutputPins_Select,
		typename T_OutputPins_Down
	> class DeautherESP8266_Watch_Navigation :
		public T_OutputPins_Up,
		public T_OutputPins_Select,
		public T_OutputPins_Down
	{
	public:
		_V_PIN_( OutputPins_Up )
		_V_PIN_( OutputPins_Down )
		_V_PIN_( OutputPins_Select )

	public:
		inline void SystemInit()
		{
			if( T_OutputPins_Up::GetPinIsConnected() )
				pinMode( 12, INPUT_PULLUP );

			if( T_OutputPins_Down::GetPinIsConnected() )
				pinMode( 13, INPUT_PULLUP );

			if( T_OutputPins_Select::GetPinIsConnected() )
				pinMode( 14, INPUT_PULLUP );

		}

		inline void SystemLoopBegin()
		{
			if( T_OutputPins_Up::GetPinIsConnected() )
				T_OutputPins_Up::SetPinValue( digitalRead( 12 ) != HIGH );

			if( T_OutputPins_Down::GetPinIsConnected() )
				T_OutputPins_Down::SetPinValue( digitalRead( 13 ) != HIGH );

			if( T_OutputPins_Select::GetPinIsConnected() )
				T_OutputPins_Select::SetPinValue( digitalRead( 14 ) != HIGH );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

