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
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_DirectionOutputPins_Up,
		typename T_DirectionOutputPins_Down,
		typename T_DirectionOutputPins_Left,
		typename T_DirectionOutputPins_Right,
		typename T_SelectOutputPin
	> class ArduinoDFRobotLCDKeypadKeys :
		public T_DirectionOutputPins_Up,
		public T_DirectionOutputPins_Down,
		public T_DirectionOutputPins_Left,
		public T_DirectionOutputPins_Right,
		public T_SelectOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPins_Up )
		_V_PIN_( DirectionOutputPins_Down )
		_V_PIN_( DirectionOutputPins_Left )
		_V_PIN_( DirectionOutputPins_Right )
		_V_PIN_( SelectOutputPin )

	protected:
		void UpdatePins( bool AChangeOnly )
		{
			int AAnalogValue = analogRead( 0 );

			bool AUp = false;
			bool ADown = false;
			bool ALeft = false;
			bool ARight = false;
			bool ASelect = false;

			if( AAnalogValue < 50 )
				ARight = true;

			else if( AAnalogValue < C_OWNER.GetKeyUp() )
				AUp = true;

			else if( AAnalogValue < C_OWNER.GetKeyDown() )
				ADown = true;

			else if( AAnalogValue < C_OWNER.GetKeyLeft() )
				ALeft = true;

			else if( AAnalogValue < C_OWNER.GetKeySelect() )
				ASelect = true;

			T_SelectOutputPin::SetPinValue( ASelect, AChangeOnly );
			T_DirectionOutputPins_Up::SetPinValue( AUp, AChangeOnly );
			T_DirectionOutputPins_Down::SetPinValue( ADown, AChangeOnly );
			T_DirectionOutputPins_Left::SetPinValue( ALeft, AChangeOnly );
			T_DirectionOutputPins_Right::SetPinValue( ARight, AChangeOnly );
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
		typename T_IsVersion_1_0
	> class ArduinoDFRobotLCDKeypadShield :
		public T_IsVersion_1_0
	{
	public:
		_V_PROP_( IsVersion_1_0 )

	public:
		inline int GetKeyUp()
		{
			return ( IsVersion_1_0().GetValue() ) ? 195 : 250;
		}

		inline int GetKeyDown()
		{
			return ( IsVersion_1_0().GetValue() ) ? 380 : 450;
		}

		inline int GetKeyLeft()
		{
			return ( IsVersion_1_0().GetValue() ) ? 555 : 650;
		}

		inline int GetKeySelect()
		{
			return ( IsVersion_1_0().GetValue() ) ? 790 : 850;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif