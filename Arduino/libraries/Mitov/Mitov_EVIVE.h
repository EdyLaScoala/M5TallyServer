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
#include <Mitov_Microchip_MCP4725.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_0,
		typename T_OutputPins_1
	> class ArduinoEVIVEPotentiometers :
		public T_OutputPins_0,
		public T_OutputPins_1
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )

	public:
		inline void SystemLoopBegin()
		{
	    	T_OutputPins_0::SetPinValue( analogRead( 9 ) / 1023.0, true );
	    	T_OutputPins_1::SetPinValue( analogRead( 10 ) / 1023.0, true );
		}

		inline void SystemStart()
		{
	    	T_OutputPins_0::SetPinValue( analogRead( 9 ) / 1023.0, false );
	    	T_OutputPins_1::SetPinValue( analogRead( 10 ) / 1023.0, false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AnalogOutputPin,
		typename T_DirectionOutputPins_0,
		typename T_DirectionOutputPins_1,
		typename T_DirectionOutputPins_2,
		typename T_DirectionOutputPins_3,
		typename T_PressOutputPin
	> class ArduinoEVIVENavigation :
		public T_AnalogOutputPin,
		public T_DirectionOutputPins_0,
		public T_DirectionOutputPins_1,
		public T_DirectionOutputPins_2,
		public T_DirectionOutputPins_3,
		public T_PressOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPins_0 )
		_V_PIN_( DirectionOutputPins_1 )
		_V_PIN_( DirectionOutputPins_2 )
		_V_PIN_( DirectionOutputPins_3 )

		_V_PIN_( PressOutputPin )
		_V_PIN_( AnalogOutputPin )

	protected:
		static const int NAVKEYMOVE_UP_LOW       = 190;
		static const int NAVKEYMOVE_UP_HIGH      = 230;
		static const int NAVKEYMOVE_RIGHT_LOW    = 385;
		static const int NAVKEYMOVE_RIGHT_HIGH   = 425;
		static const int NAVKEYMOVE_DOWN_LOW     = 585;
		static const int NAVKEYMOVE_DOWN_HIGH    = 625;
		static const int NAVKEYMOVE_LEFT_LOW     = 820;
		static const int NAVKEYMOVE_LEFT_HIGH    = 860;

	protected:
		void UpdatePins( bool AChangeOnly )
		{
			int AAnalogValue = analogRead( 11 );
	    	T_AnalogOutputPin::SetPinValue( AAnalogValue / 1023.0, AChangeOnly );
//	    	AnalogOutputPin.SetValue( AAnalogValue, AChangeOnly );
			T_PressOutputPin::SetPinValue( digitalRead( 19 ), AChangeOnly );

			bool AUp = (( AAnalogValue > NAVKEYMOVE_UP_LOW ) && ( AAnalogValue < NAVKEYMOVE_UP_HIGH ));
			T_DirectionOutputPins_0::SetPinValue( AUp, AChangeOnly );

			bool ADown = (( AAnalogValue > NAVKEYMOVE_DOWN_LOW ) && ( AAnalogValue < NAVKEYMOVE_DOWN_HIGH ));
			T_DirectionOutputPins_1::SetPinValue( ADown, AChangeOnly );

			bool ALeft = (( AAnalogValue > NAVKEYMOVE_LEFT_LOW ) && ( AAnalogValue < NAVKEYMOVE_LEFT_HIGH ));
			T_DirectionOutputPins_2::SetPinValue( ALeft, AChangeOnly );

			bool ARight = (( AAnalogValue > NAVKEYMOVE_RIGHT_LOW ) && ( AAnalogValue < NAVKEYMOVE_RIGHT_HIGH ));
			T_DirectionOutputPins_3::SetPinValue( ARight, AChangeOnly );
		}

	public:
		inline void SystemLoopBegin()
		{
			UpdatePins( true );
		}

		inline void SystemStart()
		{
			UpdatePins( false );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_ValueOutputPin
	> class ArduinoEVIVESlideSwitch :
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_ValueOutputPin
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( ValueOutputPin )

	public:
		inline void Process( int APin1, int APin2, bool AChangeOnly )
		{
			bool AOut0 = digitalRead( APin1 ) != LOW;
			bool AOut1 = digitalRead( APin2 ) != LOW;

	    	T_OutputPins_0::SetPinValue( AOut0, AChangeOnly );
	    	T_OutputPins_1::SetPinValue( AOut1, AChangeOnly );

			if( AOut0 )
				T_ValueOutputPin::SetPinValue( 0, AChangeOnly );

			else if( AOut1 )
				T_ValueOutputPin::SetPinValue( 2, AChangeOnly );

			else
				T_ValueOutputPin::SetPinValue( 1, AChangeOnly );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Switch1,
		typename T_Switch2
	> class ArduinoEVIVESlideSwitches :
		public T_Switch1,
		public T_Switch2
	{
	public:
		_V_PROP_( Switch1 )
		_V_PROP_( Switch2 )

	public:
		inline void SystemLoopBegin()
		{
			Switch1().Process( 40, 41, true );
			Switch2().Process( 42, 43, true );
		}

		inline void SystemStart()
		{
			Switch1().Process( 40, 41, false );
			Switch2().Process( 42, 43, false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif