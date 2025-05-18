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
		typename T_OutputPins_Down,
		typename T_OutputPins_Left,
		typename T_OutputPins_Press
	> class Makerfocus_ESP8266_OLED_Joystick :
		public T_OutputPins_Up,
		public T_OutputPins_Down,
		public T_OutputPins_Left,
		public T_OutputPins_Press
	{
	public:
		_V_PIN_( OutputPins_Up )
		_V_PIN_( OutputPins_Down )
		_V_PIN_( OutputPins_Left )
		_V_PIN_( OutputPins_Press )

	public:
		inline void SystemInit()
		{
			if( T_OutputPins_Up::GetPinIsConnected() )
				pinMode( 12, INPUT_PULLUP );

			if( T_OutputPins_Down::GetPinIsConnected() )
				pinMode( 13, INPUT_PULLUP );

			if( T_OutputPins_Left::GetPinIsConnected() )
				pinMode( 0, INPUT_PULLUP );

			if( T_OutputPins_Press::GetPinIsConnected() )
				pinMode( 14, INPUT_PULLUP );

		}

		inline void SystemLoopBegin()
		{
			if( T_OutputPins_Up::GetPinIsConnected() )
				T_OutputPins_Up::SetPinValue( digitalRead( 12 ) != HIGH );

			if( T_OutputPins_Down::GetPinIsConnected() )
				T_OutputPins_Down::SetPinValue( digitalRead( 13 ) != HIGH );

			if( T_OutputPins_Left::GetPinIsConnected() )
				T_OutputPins_Left::SetPinValue( digitalRead( 0 ) != HIGH );

			if( T_OutputPins_Press::GetPinIsConnected() )
				T_OutputPins_Press::SetPinValue( digitalRead( 14 ) != HIGH );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

