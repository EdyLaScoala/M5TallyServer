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

#ifndef Notify
  #define Notify(...) ((void)0)
#endif

#include <hidboot.h>

#ifdef Notify
  #undef Notify
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin,
		typename T_Value
	> class TArduinoSamdUSBHostKeyboardKeyElement :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline void KeyPress( uint8_t AKey, bool ADown )
		{
			if( Value().GetValue() == AKey )
				T_OutputPin::SetPinValue( ADown );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin,
		typename T_Value
	> class TArduinoSamdUSBHostKeyboardOemKeyElement :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline void OemKeyPress( uint8_t AKey, bool ADown )
		{
			if( Value().GetValue() == AKey )
				T_OutputPin::SetPinValue( ADown );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Left,
		typename T_Right
	> class TArduinoSamdUSBHostKeyboardSpecialKeys :
		public T_Left,
		public T_Right
	{
	public:
		_V_PROP_( Left )
		_V_PROP_( Right )

	};
//---------------------------------------------------------------------------
	template <
		typename T_AltOutputPin,
		typename T_CommandOutputPin,
		typename T_ControlOutputPin,
		typename T_ShiftOutputPin
	> class TArduinoSamdUSBHostKeyboardSpecialKeysGroup :
		public T_AltOutputPin,
		public T_CommandOutputPin,
		public T_ControlOutputPin,
		public T_ShiftOutputPin
	{
	public:
		_V_PIN_( AltOutputPin )
		_V_PIN_( CommandOutputPin )
		_V_PIN_( ControlOutputPin )
		_V_PIN_( ShiftOutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Down,
		typename T_Up
	> class TArduinoSamdUSBHostKeyboardOutput :
		public T_Down,
		public T_Up
	{
	public:
		_V_PROP_( Down )
		_V_PROP_( Up )

	public:
		constexpr static inline bool IfNeedsDownKey()
		{
			return T_Down::IfNeedsKey();
		}

		constexpr static inline bool IfNeedsUpKey()
		{
			return T_Up::IfNeedsKey();
		}

		inline void OnKeyDown( uint8_t key )
		{			
			Down().OnKey( key );
		}

		inline void OnKeyUp( uint8_t key )
		{
			Up().OnKey( key );
		}

		inline void OnOemKeyDown( uint8_t mod, uint8_t key )
		{			
			Down().OnOemKey( mod, key );
		}

		inline void OnOemKeyUp( uint8_t mod, uint8_t key )
		{
			Up().OnOemKey( mod, key );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OemOutputPin,
		typename T_OutputPin
	> class TArduinoSamdUSBHostKeyboardOutputItem :
		public T_OemOutputPin,
		public T_OutputPin
	{
	public:
		_V_PIN_( OemOutputPin )
		_V_PIN_( OutputPin )

		constexpr static inline bool IfNeedsKey()
		{
			return T_OutputPin::GetPinIsConnected();
		}

		inline void OnKey( uint8_t key )
		{
			T_OutputPin::SetPinValue( key );
		}

		inline void OnOemKey( uint8_t mod, uint8_t key )
		{
			T_OemOutputPin::SetPinValue( key );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Frequency,
		typename T_InitialDelay
	> class TArduinoSamdUSBHostKeyboardRepeat :
		public T_Enabled,
		public T_Frequency,
		public T_InitialDelay
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Frequency )
		_V_PROP_( InitialDelay )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_KeyPress,
		typename T_Elements_OemKeyPress,
		typename T_FAfterInitialDelay,
		typename T_FCharacter,
		typename T_FLastTime,
		bool C_FNeedsAscii,
		typename T_FPressed,
		typename T_KeyPressed,
		typename T_OutputPin,
		typename T_Repeat,
		typename T_SpecialKeys
	> class TArduinoSamdUSBHostKeyboard :
		public KeyboardReportParser,
		public T_FAfterInitialDelay,
		public T_FCharacter,
		public T_FLastTime,
		public T_FPressed,
		public T_KeyPressed,
		public T_OutputPin,
		public T_Repeat,
		public T_SpecialKeys
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( KeyPressed )
		_V_PROP_( Repeat )
		_V_PROP_( SpecialKeys )

	protected:
		_V_PROP_( FAfterInitialDelay )
		_V_PROP_( FCharacter )
		_V_PROP_( FLastTime )
		_V_PROP_( FPressed )

	protected:
		constexpr static inline bool IfNeedsDownKey()
		{
			if( C_FNeedsAscii )
				return true;

			if( T_OutputPin::GetPinIsConnected() )
				return true;

			return T_KeyPressed::IfNeedsDownKey();
		}

		constexpr static inline bool IfNeedsUpKey()
		{
			if( C_FNeedsAscii )
				return true;

			if( T_OutputPin::GetPinIsConnected() )
				return true;

			return T_KeyPressed::IfNeedsUpKey();
		}

	protected:
		virtual void OnKeyDown( uint8_t mod, uint8_t key ) override
		{
			if( IfNeedsDownKey())
			{
				uint8_t AAsciiChar = OemToAscii( mod, key );
				KeyPressed().OnKeyDown( AAsciiChar );
				FCharacter() = AAsciiChar;
				FLastTime() = millis();
				FAfterInitialDelay() = false;
				FPressed() = true;
				T_OutputPin::SetPinValue( AAsciiChar );
				T_Elements_KeyPress::Call( AAsciiChar, true );
			}

			KeyPressed().OnOemKeyDown( mod, key );
			T_Elements_OemKeyPress::Call( key, true );
		}

		virtual void OnKeyUp( uint8_t mod, uint8_t key ) override
		{
			if( IfNeedsUpKey())
			{
				uint8_t AAsciiChar = OemToAscii( mod, key );
				KeyPressed().OnKeyUp( AAsciiChar );
				if( FCharacter() == AAsciiChar )
					FPressed() = false;

				T_Elements_KeyPress::Call( AAsciiChar, false );
			}

			KeyPressed().OnOemKeyUp( mod, key );
			T_Elements_OemKeyPress::Call( key, false );
		}

	private:
		HIDBoot<HID_PROTOCOL_KEYBOARD> hostKeyboard;

	public:
		inline void SystemStart()
		{
		}

		inline void SystemLoopBegin()
		{
//			if( FLastTime().GetIsConstant() )
//				return;

			if( ! FPressed() )
				return;

			unsigned long ACurrentTime = millis();
			if( FAfterInitialDelay() )
			{
				unsigned long ARepeatDelay = 1000 / Repeat().Frequency();
				if( ACurrentTime - FLastTime().GetValue() >= ARepeatDelay )
				{
					T_OutputPin::SetPinValue( FCharacter().GetValue() );
					FLastTime() = ACurrentTime;
				}

				return;
			}

			if( ACurrentTime - FLastTime().GetValue() >= Repeat().InitialDelay() )
			{
				FLastTime() = ACurrentTime;
				FAfterInitialDelay() = true;
			}
		}

	public:
		inline TArduinoSamdUSBHostKeyboard() : 
			hostKeyboard( C_OWNER.GetUSBHost() )
		{
			hostKeyboard.SetReportParser( 0, this );
			FPressed() = false;
		};

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif