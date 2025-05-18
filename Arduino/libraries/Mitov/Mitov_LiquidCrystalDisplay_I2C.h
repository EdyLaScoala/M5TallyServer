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
#include <Mitov_LiquidCrystalDisplay.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Backlight,
		typename T_BacklightPositive
	> class LiquidCrystalDisplayI2C : 
		public T_Address,
		public T_Backlight,
		public T_BacklightPositive
	{
	public:
		_V_PROP_( Address )
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
			AValue <<= 4;
			const uint8_t AEnableMask = 0b100;
			if( ! ACommand )
				AValue |= 1; // Set RS

			WriteRegister( AValue | AEnableMask );   // En HIGH
			WriteRegister( AValue & ~AEnableMask );  // En LOW
		}

		void WriteRegister( uint8_t AValue )
		{
			if( BacklightPositive() )
			{
				if( Backlight() )
					AValue |= 0b1000;

			}

			else
			{
				if( ! Backlight() )
					AValue |= 0b1000;

			}

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( AValue );
//			Serial.println( AValue, HEX );
			C_I2C.endTransmission();
		}

	public:
		void UpdateBacklight()
		{
			WriteRegister( 0 );
/*
			if( Backlight )
				inherited::FLcd->setBacklight( 255 );

			else
				inherited::FLcd->setBacklight( 0 );
*/
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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif