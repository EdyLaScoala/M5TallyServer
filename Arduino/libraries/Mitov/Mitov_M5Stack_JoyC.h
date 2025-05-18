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
	namespace M5StackJoyC
	{
		const uint8_t	ADDRESS				= 0x38;
		const uint8_t	COLOR_REG			= 0x20;

		const uint8_t	LEFT_X_REG        	= 0x60;
		const uint8_t	LEFT_Y_REG        	= 0x61;
		const uint8_t	RIGHT_X_REG       	= 0x62;
		const uint8_t	RIGHT_Y_REG       	= 0x63;
		const uint8_t	PRESS_REG         	= 0x64;
		const uint8_t	LEFT_ANGLE_REG    	= 0x70;
		const uint8_t	LEFT_DISTANCE_REG 	= 0x74;
		const uint8_t	RIGHT_ANGLE_REG   	= 0x72;
		const uint8_t	RIGHT_DISTANCE_REG	= 0x76;
	}
//---------------------------------------------------------------------------
	template <
		typename T_Joysticks_NeedsUpdateButtons,
		typename T_Joysticks_UpdateButtons
	> class M5StackJoyCI2C
	{
	public:
//		constexpr static inline uint8_t GetLEDsAddress() { return M5StackJoyC::ADDRESS; }
//		constexpr static inline uint8_t GetLEDsRegister() { return M5StackJoyC::COLOR_REG; }

		void SetRGBColor( uint32_t AColor )
		{
			Wire.beginTransmission( M5StackJoyC::ADDRESS );
			Wire.write( M5StackJoyC::COLOR_REG );

			Wire.write( uint8_t( AColor >> 16 ));
			Wire.write( uint8_t( AColor >> 8 ));
			Wire.write( uint8_t( AColor ));
  
			Wire.endTransmission();
		}

	protected:
		void ReadButtons( bool AChangeOnly )
		{
			bool AValue = false;
			T_Joysticks_NeedsUpdateButtons::Call( AValue );
			if( ! AValue )
				return;

			Wire.beginTransmission( M5StackJoyC::ADDRESS );			
			Wire.write( M5StackJoyC::PRESS_REG );
			Wire.endTransmission( false );

			Wire.requestFrom( M5StackJoyC::ADDRESS, uint8_t( 1 ));
			if( Wire.available() )
				T_Joysticks_UpdateButtons::Call( Wire.read(), AChangeOnly );

		}

	public:
		inline void SystemStart()
		{
			ReadButtons( false );
		}

		inline void SystemLoopBegin()
		{
			ReadButtons( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue
	> class M5Stack_JoyCLEDColor :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				C_OWNER.SetRGBColor( InitialValue().GetValue() );

			else
				C_OWNER.SetRGBColor( 0 );
  
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;
			SystemStart();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_AxesOutputPins_X,
		typename T_AxesOutputPins_Y,
		typename T_ButtonOutputPin,
		typename T_ClockInputPin_o_IsConnected,
		uint8_t C_INDEX
	> class M5Stack_JoyCJoystick : 
		public T_AxesOutputPins_X,
		public T_AxesOutputPins_Y,
		public T_ButtonOutputPin,
		public T_ClockInputPin_o_IsConnected
	{
	public:
		_V_PIN_( AxesOutputPins_X )
		_V_PIN_( AxesOutputPins_Y )
		_V_PIN_( ButtonOutputPin )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			Wire.beginTransmission( M5StackJoyC::ADDRESS );
			Wire.write( M5StackJoyC::LEFT_X_REG + C_INDEX * 2 );
			Wire.endTransmission( false );	

			Wire.requestFrom( M5StackJoyC::ADDRESS, uint8_t( 2 ));
			if( ! Wire.available() )
				return;

			float x_data = 1.0f - ( float( Wire.read() ) / 200 );
			float y_data = float( Wire.read() ) / 200;
//			bool button_data = ( C_I2C.read() != 0 );

			T_AxesOutputPins_X::SetPinValue( x_data, AChangeOnly );
			T_AxesOutputPins_Y::SetPinValue( y_data, AChangeOnly );
//			T_ButtonOutputPin::SetPinValue( button_data, AChangeOnly );
		}

	protected:
		inline void UpdateButtonsInternal( uint8_t AValue, bool AChangeOnly )
		{
			if( C_INDEX )
				T_ButtonOutputPin::SetPinValue( ( 0x01 & AValue ) != 0, AChangeOnly );

			else
				T_ButtonOutputPin::SetPinValue( ( 0x10 & AValue ) != 0, AChangeOnly );

//    return (press_value & ((pos == 0) ? 0x10 : 0x01)) != 0;
		}

	public:
		inline void UpdateButtons( uint8_t AValue, bool AChangeOnly )
		{
			if( ! ClockInputPin_o_IsConnected() )
				UpdateButtonsInternal( AValue, AChangeOnly );

		}

		inline void NeedsUpdateButtons( bool & AValue )
		{
			if( T_ButtonOutputPin::GetPinIsConnected() && ( ! ClockInputPin_o_IsConnected() ) )
				AValue = true;

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );

			Wire.beginTransmission( M5StackJoyC::ADDRESS );			
			Wire.write( M5StackJoyC::PRESS_REG );
			Wire.endTransmission( false );

			Wire.requestFrom( M5StackJoyC::ADDRESS, uint8_t( 1 ));
			if( Wire.available() )
				UpdateButtonsInternal( Wire.read(), false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

