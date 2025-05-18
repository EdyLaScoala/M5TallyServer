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
#include <Mitov_Microchip_MCP23017.h>
#include <Mitov_LiquidCrystalDisplay.h>

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
	> class ArduinoAdafruitLCDKeypadKeys :
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
			uint8_t AValue = C_OWNER.ReadRegister8( Microchip_MCP23017_Const::REG_GPIOA );

			T_SelectOutputPin::SetPinValue(           ( AValue & 0b00001 ) == 0, AChangeOnly );
			T_DirectionOutputPins_Up::SetPinValue(    ( AValue & 0b01000 ) == 0, AChangeOnly );
			T_DirectionOutputPins_Down::SetPinValue(  ( AValue & 0b00100 ) == 0, AChangeOnly );
			T_DirectionOutputPins_Left::SetPinValue(  ( AValue & 0b10000 ) == 0, AChangeOnly );
			T_DirectionOutputPins_Right::SetPinValue( ( AValue & 0b00010 ) == 0, AChangeOnly );
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
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Backlight,
		typename T_BacklightPositive
	> class ArduinoAdafruitLCDKeypadShieldDisplay_Implementation :
		public T_Backlight,
		public T_BacklightPositive
	{
	public:
		_V_PROP_( Backlight )
		_V_PROP_( BacklightPositive )

	public:
		void Send( uint8_t AValue, bool ACommand, bool A4Bits )
		{
			if( ! A4Bits )
				Write4bits( AValue >> 4, ACommand );

			Write4bits( AValue & 0xF, ACommand );
		}

		void Write4bits( uint8_t AValue, bool ACommand )
		{
//			AValue <<= 4;
			uint8_t AOutValue = AValue & 0b0100;
			const uint8_t AEnableMask = 0b00100000;
			if( ! ACommand )
				AOutValue |= 0b10000000; // Set RS

            if( AValue & 0b0001 )
                AOutValue |= 0b10000;

            if( AValue & 0b0010 )
                AOutValue |= 0b01000;

//            if( AValue & 0b0100 )
//                AOutValue |= 0b00100;

            if( AValue & 0b1000 )
                AOutValue |= 0b00010;

			WriteRegister( AOutValue | AEnableMask );   // En HIGH
			WriteRegister( AOutValue & ~AEnableMask );  // En LOW
		}

		void WriteRegister( uint8_t AValue )
		{
            C_OWNER.WriteRegister8( Microchip_MCP23017_Const::REG_OLATB, AValue );
		}

	public:
		void UpdateBacklight()
		{
//		    Serial.println( "UpdateBacklight" );
		    uint8_t AValue = 0;
			if( BacklightPositive() )
			{
				if( Backlight() )
					AValue |= 0b01000000;

			}

			else
			{
				if( ! Backlight() )
					AValue |= 0b01000000;

			}

            C_OWNER.WriteRegister8( Microchip_MCP23017_Const::REG_OLATA, AValue );
//			WriteRegister( 0 );
		}

	protected:
		template <typename T_IMPLEMENT> void DisplayInit( T_IMPLEMENT *AObject )
		{
			WriteRegister( 0 );

			// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
			// according to datasheet, we need at least 40ms after power rises above 2.7V
			// before sending commands. Arduino can turn on way before 4.5V so we'll wait
			// 50
			// ---------------------------------------------------------------------------
			delay( 100 ); // 100ms delay

//			Serial.println( "STEP1" );

			// 4 bit mode
			// this is according to the hitachi HD44780 datasheet
			// figure 24, pg 46

			// we start in 8bit mode, try to set 4 bit mode
			Send( 0x03, true, true );
//			delayMicroseconds(4500); // wait min 4.1ms

			// second try
			Send( 0x03, true, true );
//			delayMicroseconds(4500); // wait min 4.1ms

			// third go!
			Send( 0x03, true, true );
//			delayMicroseconds(150);

			// finally, set to 4-bit interface
			Send( 0x02, true, true );

			// finally, set # lines, font size, etc.
			uint8_t ADisplayFunction = 0;
			if( AObject->GetHeight() > 1 )
				ADisplayFunction |= LCDConsts::LCD_2LINE;

			if( AObject->Hight10Pixels() && ( AObject->GetHeight() == 1 ))
				ADisplayFunction |= LCDConsts::LCD_5x10DOTS;

//			Serial.print( "ADisplayFunction: " );
//			Serial.println( ADisplayFunction, HEX );

			AObject->Command( LCDConsts::LCD_FUNCTIONSET | ADisplayFunction );

//			Serial.println( "UpdateDisplayControl()" );
			AObject->UpdateDisplayControl();

			// clear the LCD
//			Serial.println( "Clear()" );
			AObject->Clear();

//			Serial.println( "UpdateEntryModeSet()" );
			AObject->UpdateEntryModeSet();

			// Initialize to default text direction (for romance languages)
//			_displaymode = LCDConsts::LCD_ENTRYLEFT | LCDConsts::LCD_ENTRYSHIFTDECREMENT;
			// set the entry mode
//			command( LCDConsts::LCD_ENTRYMODESET | _displaymode);

//			UpdateBacklight();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Backlight,
		typename T_BacklightPositive
	> class ArduinoAdafruitRGBLCDKeypadShieldDisplay_Implementation :
		public T_Backlight,
		public T_BacklightPositive
	{
	public:
		_V_PROP_( Backlight )
		_V_PROP_( BacklightPositive )

	public:
		void Send( uint8_t AValue, bool ACommand, bool A4Bits )
		{
			if( ! A4Bits )
				Write4bits( AValue >> 4, ACommand );

			Write4bits( AValue & 0xF, ACommand );
		}

		void Write4bits( uint8_t AValue, bool ACommand )
		{
//			AValue <<= 4;
			uint8_t AOutValue = AValue & 0b0100;
			const uint8_t AEnableMask = 0b00100000;
			if( ! ACommand )
				AOutValue |= 0b10000000; // Set RS

            if( AValue & 0b0001 )
                AOutValue |= 0b10000;

            if( AValue & 0b0010 )
                AOutValue |= 0b01000;

//            if( AValue & 0b0100 )
//                AOutValue |= 0b00100;

            if( AValue & 0b1000 )
                AOutValue |= 0b00010;

			if( BacklightPositive() )
			{
				if( Backlight().Blue() )
					AValue |= 0b00000001;

			}

			else
			{
				if( ! Backlight().Blue() )
					AValue |= 0b00000001;

			}

			WriteRegister( AOutValue | AEnableMask );   // En HIGH
			WriteRegister( AOutValue & ~AEnableMask );  // En LOW
		}

		void WriteRegister( uint8_t AValue )
		{
            C_OWNER.WriteRegister8( Microchip_MCP23017_Const::REG_OLATB, AValue );
		}

	public:
		void UpdateBacklight()
		{
//		    Serial.println( "UpdateBacklight" );
		    uint8_t AValue = 0;
			if( BacklightPositive() )
			{
				if( Backlight().Red() )
					AValue |= 0b01000000;

				if( Backlight().Green() )
					AValue |= 0b10000000;

			}

			else
			{
				if( ! Backlight().Red() )
					AValue |= 0b01000000;

				if( ! Backlight().Green() )
					AValue |= 0b10000000;

			}

            C_OWNER.WriteRegister8( Microchip_MCP23017_Const::REG_OLATA, AValue );
//			WriteRegister( 0 );
		}

	protected:
		template <typename T_IMPLEMENT> void DisplayInit( T_IMPLEMENT *AObject )
		{
			WriteRegister( 0 );

			// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
			// according to datasheet, we need at least 40ms after power rises above 2.7V
			// before sending commands. Arduino can turn on way before 4.5V so we'll wait
			// 50
			// ---------------------------------------------------------------------------
			delay( 100 ); // 100ms delay

//			Serial.println( "STEP1" );

			// 4 bit mode
			// this is according to the hitachi HD44780 datasheet
			// figure 24, pg 46

			// we start in 8bit mode, try to set 4 bit mode
			Send( 0x03, true, true );
//			delayMicroseconds(4500); // wait min 4.1ms

			// second try
			Send( 0x03, true, true );
//			delayMicroseconds(4500); // wait min 4.1ms

			// third go!
			Send( 0x03, true, true );
//			delayMicroseconds(150);

			// finally, set to 4-bit interface
			Send( 0x02, true, true );

			// finally, set # lines, font size, etc.
			uint8_t ADisplayFunction = 0;
			if( AObject->GetHeight() > 1 )
				ADisplayFunction |= LCDConsts::LCD_2LINE;

			if( AObject->Hight10Pixels() && ( AObject->GetHeight() == 1 ))
				ADisplayFunction |= LCDConsts::LCD_5x10DOTS;

//			Serial.print( "ADisplayFunction: " );
//			Serial.println( ADisplayFunction, HEX );

			AObject->Command( LCDConsts::LCD_FUNCTIONSET | ADisplayFunction );

//			Serial.println( "UpdateDisplayControl()" );
			AObject->UpdateDisplayControl();

			// clear the LCD
//			Serial.println( "Clear()" );
			AObject->Clear();

//			Serial.println( "UpdateEntryModeSet()" );
			AObject->UpdateEntryModeSet();

			// Initialize to default text direction (for romance languages)
//			_displaymode = LCDConsts::LCD_ENTRYLEFT | LCDConsts::LCD_ENTRYSHIFTDECREMENT;
			// set the entry mode
//			command( LCDConsts::LCD_ENTRYMODESET | _displaymode);

//			UpdateBacklight();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AutoScroll,
        typename T_Backlight,
        typename T_BacklightPositive,
		typename T_Blink,
		typename T_Columns,
		typename T_Enabled,
		typename T_FCursorLine,
		typename T_FCursorPos,
		typename T_Hight10Pixels,
		typename T_RightToLeft,
		typename T_Rows,
		typename T_ShowCursor
	> class ArduinoAdafruitLCDKeypadShieldDisplay :
	    public LiquidCrystalDisplay<
	            ArduinoAdafruitLCDKeypadShieldDisplay_Implementation <
	                    T_OWNER, C_OWNER,
                		T_Backlight,
                		T_BacklightPositive
	                >,
        		T_AutoScroll,
        		T_Blink,
        		T_Columns,
        		T_Enabled,
        		T_FCursorLine,
        		T_FCursorPos,
        		T_Hight10Pixels,
        		T_RightToLeft,
        		T_Rows,
        		T_ShowCursor
	        >
    {
    };
//---------------------------------------------------------------------------
	template <
	    typename T_Blue,
	    typename T_Green,
	    typename T_Red
    > class TArduinoAdafruitShieldLCDKeypadRGBDisplayBacklight :
	    public T_Blue,
	    public T_Green,
	    public T_Red
    {
        public:
            _V_PROP_( Blue )
            _V_PROP_( Green )
            _V_PROP_( Red )

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AutoScroll,
        typename T_Backlight,
        typename T_BacklightPositive,
		typename T_Blink,
		typename T_Columns,
		typename T_Enabled,
		typename T_FCursorLine,
		typename T_FCursorPos,
		typename T_Hight10Pixels,
		typename T_RightToLeft,
		typename T_Rows,
		typename T_ShowCursor
	> class ArduinoAdafruitRGBLCDKeypadShieldDisplay :
	    public LiquidCrystalDisplay<
	            ArduinoAdafruitRGBLCDKeypadShieldDisplay_Implementation <
	                    T_OWNER, C_OWNER,
                		T_Backlight,
                		T_BacklightPositive
	                >,
        		T_AutoScroll,
        		T_Blink,
        		T_Columns,
        		T_Enabled,
        		T_FCursorLine,
        		T_FCursorPos,
        		T_Hight10Pixels,
        		T_RightToLeft,
        		T_Rows,
        		T_ShowCursor
	        >
    {
    };
//---------------------------------------------------------------------------
	class ArduinoAdafruitLCDKeypadShield
	{
	public:
		void WriteRegister8( uint8_t ARegister, uint8_t value )
		{
//		    Serial.print( "W8 " ); Serial.print( ARegister, HEX ); Serial.print( " " ); Serial.println( value, BIN );
			Wire.beginTransmission( uint8_t( 0x20 ) );
			Wire.write( ARegister );
			Wire.write( value );
			Wire.endTransmission();
		}

		void WriteRegister16( uint8_t ARegister, uint16_t value )
		{
//		    Serial.print( "W16 " ); Serial.print( ARegister, HEX ); Serial.print( " " ); Serial.println( value, BIN );

			Wire.beginTransmission( uint8_t( 0x20 ) );
			Wire.write( ARegister );
			Wire.write( uint8_t( value ));
			Wire.write( uint8_t( value >> 8 ));
			Wire.endTransmission();
		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			Wire.beginTransmission( uint8_t( 0x20 ) );
			Wire.write( ARegister );
			Wire.endTransmission();

#ifdef VISUINO_ARDUINO_AVR4809
			Wire.requestFrom( uint8_t( 0x20 ), size_t( 1 ) );
#else
			Wire.requestFrom( uint8_t( 0x20 ), uint8_t( 1 ) );
#endif // VISUINO_ARDUINO_AVR4809

			return Wire.read();
		}

	public:
        inline void SystemInit()
        {
            WriteRegister16( Microchip_MCP23017_Const::REG_IODIRA, 0b00011111 );
            WriteRegister16( Microchip_MCP23017_Const::REG_IPOLA, 0 );
            WriteRegister16( Microchip_MCP23017_Const::REG_GPPUA, 0b00011111 );
            WriteRegister16( Microchip_MCP23017_Const::REG_OLATA, 0 );
        }
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif