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
	template <
		typename T_DirectionOutputPins_Up,
		typename T_DirectionOutputPins_Down,
		typename T_DirectionOutputPins_Left,
		typename T_DirectionOutputPins_Right,
		typename T_PressOutputPin
	> class TArduino_Seeeduino_Wio_Terminal_NavigationModule :
		public T_DirectionOutputPins_Up,
		public T_DirectionOutputPins_Down,
		public T_DirectionOutputPins_Left,
		public T_DirectionOutputPins_Right,
		public T_PressOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPins_Up )
		_V_PIN_( DirectionOutputPins_Down )
		_V_PIN_( DirectionOutputPins_Left )
		_V_PIN_( DirectionOutputPins_Right )
		_V_PIN_( PressOutputPin )


	protected:
		void SendData()
		{
			T_DirectionOutputPins_Up::SetPinValue( digitalRead( WIO_5S_UP ) != HIGH );
			T_DirectionOutputPins_Down::SetPinValue( digitalRead( WIO_5S_DOWN ) != HIGH );
			T_DirectionOutputPins_Left::SetPinValue( digitalRead( WIO_5S_LEFT ) != HIGH );
			T_DirectionOutputPins_Right::SetPinValue( digitalRead( WIO_5S_RIGHT ) != HIGH );
			T_PressOutputPin::SetPinValue( digitalRead( WIO_5S_PRESS ) != HIGH );
		}

	public:
		inline void SystemStart()
		{
			pinMode( WIO_5S_UP, INPUT_PULLUP );
			pinMode( WIO_5S_LEFT, INPUT_PULLUP );
			pinMode( WIO_5S_RIGHT, INPUT_PULLUP );
			pinMode( WIO_5S_DOWN, INPUT_PULLUP );
			pinMode( WIO_5S_PRESS, INPUT_PULLUP );

			SendData();
		}

		inline void SystemLoopBegin()
		{
			SendData();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif