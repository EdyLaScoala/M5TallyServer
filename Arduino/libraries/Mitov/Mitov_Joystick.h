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
		typename T_AxesOutputPins_X,
		typename T_AxesOutputPins_Y,
		typename T_ButtonOutputPin
	> class ArduinoModuleJoystick : 
		public T_AxesOutputPins_X,
		public T_AxesOutputPins_Y,
		public T_ButtonOutputPin
	{
	public:
		_V_PIN_( AxesOutputPins_X )
		_V_PIN_( AxesOutputPins_Y )
		_V_PIN_( ButtonOutputPin )

	public:
		inline void SystemLoopBeginButton()
		{
	    	T_ButtonOutputPin::SetPinValue( digitalRead( 8 ) == LOW, true );
		}

		inline void SystemStartButton()
		{
	    	T_ButtonOutputPin::SetPinValue( digitalRead( 8 ) == LOW, false );
		}

		inline void SystemLoopBeginJoystick()
		{
	    	T_AxesOutputPins_X::SetPinValue( analogRead( 0 ) / float( VISUINO_ANALOG_IN_RANGE ), true );
	    	T_AxesOutputPins_Y::SetPinValue( analogRead( 1 ) / float( VISUINO_ANALOG_IN_RANGE ), true );
		}

		inline void SystemStartJoystick()
		{
	    	T_AxesOutputPins_X::SetPinValue( analogRead( 0 ) / float( VISUINO_ANALOG_IN_RANGE ), false );
	    	T_AxesOutputPins_Y::SetPinValue( analogRead( 1 ) / float( VISUINO_ANALOG_IN_RANGE ), false );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_DirectionOutputPins_Up,
		typename T_DirectionOutputPins_Down,
		typename T_DirectionOutputPins_Left,
		typename T_DirectionOutputPins_Right,
		typename T_OutputPins_E,
		typename T_OutputPins_F
	> class ArduinoModuleJoystickButtons : 
		public T_DirectionOutputPins_Up,
		public T_DirectionOutputPins_Down,
		public T_DirectionOutputPins_Left,
		public T_DirectionOutputPins_Right,
		public T_OutputPins_E,
		public T_OutputPins_F
	{
	public:
		_V_PIN_( DirectionOutputPins_Up )
		_V_PIN_( DirectionOutputPins_Down )
		_V_PIN_( DirectionOutputPins_Left )
		_V_PIN_( DirectionOutputPins_Right )
		_V_PIN_( OutputPins_E )
		_V_PIN_( OutputPins_F )

	public:
		inline void SystemLoopBeginDirection()
		{
	    	T_DirectionOutputPins_Up::SetPinValue( digitalRead( 2 ) == LOW, true );
	    	T_DirectionOutputPins_Down::SetPinValue( digitalRead( 4 ) == LOW, true );
	    	T_DirectionOutputPins_Left::SetPinValue( digitalRead( 5 ) == LOW, true );
	    	T_DirectionOutputPins_Right::SetPinValue( digitalRead( 3 ) == LOW, true );			
		}

		inline void SystemStartDirection()
		{
	    	T_DirectionOutputPins_Up::SetPinValue( digitalRead( 2 ) == LOW, false );
	    	T_DirectionOutputPins_Down::SetPinValue( digitalRead( 4 ) == LOW, false );
	    	T_DirectionOutputPins_Left::SetPinValue( digitalRead( 5 ) == LOW, false );
	    	T_DirectionOutputPins_Right::SetPinValue( digitalRead( 3 ) == LOW, false );
		}

		inline void SystemLoopBeginButtons()
		{
	    	T_OutputPins_E::SetPinValue( digitalRead( 6 ) == LOW, true );
	    	T_OutputPins_F::SetPinValue( digitalRead( 7 ) == LOW, true );
		}

		inline void SystemStartButtons()
		{
	    	T_OutputPins_E::SetPinValue( digitalRead( 6 ) == LOW, false );
	    	T_OutputPins_F::SetPinValue( digitalRead( 7 ) == LOW, false );
		}

	};	
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif