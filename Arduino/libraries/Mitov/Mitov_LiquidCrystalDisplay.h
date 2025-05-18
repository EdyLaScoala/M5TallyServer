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
	namespace LCDConsts
	{
		const uint8_t LCD_8BITMODE				= 0x10;
//		const uint8_t LCD_4BITMODE				= 0x00;
		const uint8_t LCD_2LINE					= 0x08;
//		const uint8_t LCD_1LINE					= 0x00;
		const uint8_t LCD_5x10DOTS				= 0x04;
//		const uint8_t LCD_5x8DOTS				= 0x00;

		const uint8_t LCD_ENTRYLEFT				= 0x02;
		const uint8_t LCD_ENTRYSHIFTINCREMENT	= 0x01;

		const uint8_t LCD_DISPLAYON				= 0x04;
		const uint8_t LCD_CURSORON				= 0x02;
		const uint8_t LCD_BLINKON				= 0x01;

		const uint8_t LCD_DISPLAYMOVE			= 0x08;
		const uint8_t LCD_MOVELEFT				= 0x00;
		const uint8_t LCD_MOVERIGHT				= 0x04;

		const uint16_t HOME_CLEAR_EXEC			= 2000;

// LCD Commands
// ---------------------------------------------------------------------------
		const uint8_t LCD_CLEARDISPLAY      = 0x01;
		const uint8_t LCD_RETURNHOME        = 0x02;
		const uint8_t LCD_ENTRYMODESET      = 0x04;
		const uint8_t LCD_DISPLAYCONTROL    = 0x08;
		const uint8_t LCD_CURSORSHIFT       = 0x10;
		const uint8_t LCD_FUNCTIONSET       = 0x20;
		const uint8_t LCD_SETCGRAMADDR      = 0x40;
		const uint8_t LCD_SETDDRAMADDR		= 0x80;
	}
//---------------------------------------------------------------------------
/*
	class LiquidCrystalDisplayBasic
	{
	public:
		bool Enabled : 1;
		bool AutoScroll : 1;
		bool RightToLeft : 1;
		bool ShowCursor : 1;
		bool Blink : 1;
		bool Hight10Pixels : 1;
		bool Backlight : 1; // = true; // In base class to save memory
		bool BacklightPositive : 1; // = true; // In base class to save memory

	public:
		LiquidCrystalDisplayBasic() :
			Enabled( true ),
			AutoScroll( false ),
			RightToLeft( false ),
			ShowCursor( false ),
			Blink( false ),
			Hight10Pixels( false ),
			Backlight( true ),
			BacklightPositive( true )
		{
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_BASE,
		typename T_AutoScroll,
		typename T_Blink,
		typename T_Columns,
		typename T_Enabled,
		typename T_FCursorLine,
		typename T_FCursorPos,
		typename T_Hight10Pixels,
		typename T_RightToLeft,
		typename T_Rows,
		typename T_ShowCursor
	> class LiquidCrystalDisplay :
		public T_BASE,
		public T_AutoScroll,
		public T_Blink,
		public T_Columns,
		public T_Enabled,
		public T_FCursorLine,
		public T_FCursorPos,
		public T_Hight10Pixels,
		public T_RightToLeft,
		public T_Rows,
		public T_ShowCursor
	{
		typedef	T_BASE	inherited;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( AutoScroll )
		_V_PROP_( RightToLeft )
		_V_PROP_( ShowCursor )
		_V_PROP_( Blink )
		_V_PROP_( Hight10Pixels )
		_V_PROP_( Columns )
		_V_PROP_( Rows )

	protected:
		_V_PROP_( FCursorLine )
		_V_PROP_( FCursorPos )

	public: // Must be public for the implementation
		inline uint8_t GetWidth() { return Columns().GetValue(); }
		inline uint8_t GetHeight() { return Rows().GetValue(); }

	public:
		void SetCursor(uint8_t col, uint8_t row)
		{
		   const uint8_t row_offsetsDef[]   = { 0x00, 0x40, 0x14, 0x54 }; // For regular LCDs
		   const uint8_t row_offsetsLarge[] = { 0x00, 0x40, 0x10, 0x50 }; // For 16x4 LCDs

		   if ( row >= GetHeight() )
			  row = GetHeight() - 1;    // rows start at 0

		   // 16x4 LCDs have special memory map layout
		   // ----------------------------------------
		   if ( GetWidth() == 16 && GetHeight() == 4 )
			  Command( LCDConsts::LCD_SETDDRAMADDR | (col + row_offsetsLarge[row]));

		   else
			  Command( LCDConsts::LCD_SETDDRAMADDR | (col + row_offsetsDef[row]));

		}

		size_t Print( const Mitov::String AValue )
		{
			const char *APtr = AValue.c_str();
			size_t ACount = 0;
			while( *APtr )
			{
				Write( *APtr++ );
				++ ACount;
			}

			return ACount;
		}

		size_t Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			return Print( Mitov::String( AText ));
		}

		size_t Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			return Print( Mitov::String( AText ));
		}

		size_t Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			return Print( Mitov::String( AText ));
		}

		inline size_t Print( char AValue )
		{
			Write( (uint8_t)AValue );
			return 1;
		}

		inline size_t Print( uint8_t AValue )
		{
			Write( AValue );
			return 1;
		}

	public:
		inline void Command( uint8_t AValue )
		{
			inherited::Send( AValue, true, false );
		}

		void Write(uint8_t AValue)
		{
			inherited::Send( AValue, false, false );
		}

	public:
		void ClearLine()
		{
			SetCursor( 0, FCursorLine() );
			FCursorPos() = 0;
//			for( int i = 0; i < FCols; ++ i )
//				Interface::FLcd->print( ' ' );

//			Interface::FLcd->setCursor( 0, FCursorLine );
		}

		void NewLine( int AClearSize )
		{
			for( int i = 0; i < GetWidth() - FCursorPos() - AClearSize; ++ i )
				Print( ' ' );

//			++FCursorLine;
			FCursorLine() = FCursorLine() + 1;
			FCursorPos() = 0;
			if( FCursorLine() >= GetHeight() )
				FCursorLine() = 0;

//			Interface::FLcd->setCursor( 0, FCursorLine() );
		}

		inline void MoveChars( int ACount )
		{
			FCursorPos() = FCursorPos() + 1;
//			++FCursorPos;
		}

	public:
		void UpdateDisplayControl()
		{
			uint8_t AValue = 0;
			if( Enabled() )
				AValue |= LCDConsts::LCD_DISPLAYON;

			if( ShowCursor() )
				AValue |= LCDConsts::LCD_CURSORON;

			if( Blink() )
				AValue |= LCDConsts::LCD_BLINKON;

			Command( LCDConsts::LCD_DISPLAYCONTROL | AValue );
		}

		void UpdateEntryModeSet()
		{
			uint8_t AValue = 0;

			if( ! RightToLeft() )
				AValue |= LCDConsts::LCD_ENTRYLEFT;

			if( AutoScroll() )
				AValue |= LCDConsts::LCD_ENTRYSHIFTINCREMENT;

			Command( LCDConsts::LCD_ENTRYMODESET | AValue );
		}

		void Clear()
		{
			Command( LCDConsts::LCD_CLEARDISPLAY );             // clear display, set cursor position to zero
			delayMicroseconds( LCDConsts::HOME_CLEAR_EXEC );    // this command is time consuming
		}

	public:
		inline void ScrollLeftInputPin_o_Receive( void * )
		{
			Command( LCDConsts::LCD_CURSORSHIFT | LCDConsts::LCD_DISPLAYMOVE | LCDConsts::LCD_MOVELEFT );
		}

		inline void ScrollRightInputPin_o_Receive( void * )
		{
			Command( LCDConsts::LCD_CURSORSHIFT | LCDConsts::LCD_DISPLAYMOVE | LCDConsts::LCD_MOVERIGHT );
		}

		inline void ClearInputPin_o_Receive( void * )
		{
			Clear();
		}

		void HomeInputPin_o_Receive( void * )
		{
			Command( LCDConsts::LCD_RETURNHOME );             // set cursor position to zero
			delayMicroseconds( LCDConsts::HOME_CLEAR_EXEC );  // This command is time consuming
		}

	public:
		inline void SystemInit()
		{
			FCursorLine() = 0;
			FCursorPos() = 0;
			T_BASE::DisplayInit( this );
		}

//	public:
//		LiquidCrystalDisplay( uint8_t AWidth, uint8_t AHeight ) :
//			FWidth( AWidth ),
//			FHeight( AHeight )
//		{
//		}

/*
		template <typename T> LiquidCrystalDisplay( uint8_t AWidth, uint8_t AHeight, T &AWire ) :
			T_BASE( AWire ),
			FWidth( AWidth ),
			FHeight( AHeight )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3
	> class LiquidCrystalDisplayParallel4DataPins :
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )

		T_OutputPins_0 &OutputPins_7() { return OutputPins_0(); }
		T_OutputPins_1 &OutputPins_8() { return OutputPins_1(); }
		T_OutputPins_2 &OutputPins_9() { return OutputPins_2(); }
		T_OutputPins_3 &OutputPins_10() { return OutputPins_3(); }

	protected:
		template <typename T_IMPLEMENT> inline void SetDataPinsValue( T_IMPLEMENT *AObject, uint8_t AValue, bool A4Bits )
		{
			if( ! A4Bits )
			{
				T_OutputPins_0::SetPinValue( AValue & 0b00010000 );
				T_OutputPins_1::SetPinValue( AValue & 0b00100000 );
				T_OutputPins_2::SetPinValue( AValue & 0b01000000 );
				T_OutputPins_3::SetPinValue( AValue & 0b10000000 );

				AObject->PulseEnable();
			}

			T_OutputPins_0::SetPinValue( AValue & 0b0001 );
			T_OutputPins_1::SetPinValue( AValue & 0b0010 );
			T_OutputPins_2::SetPinValue( AValue & 0b0100 );
			T_OutputPins_3::SetPinValue( AValue & 0b1000 );

			AObject->PulseEnable();
		}

	protected:
		inline void ApplyMode( uint8_t &ADisplayFunction )
		{
		}

		template <typename T_IMPLEMENT> void InitRegisters( T_IMPLEMENT *AObject )
		{
			// 4 bit mode
			// this is according to the hitachi HD44780 datasheet
			// figure 24, pg 46

			// we start in 8bit mode, try to set 4 bit mode
			AObject->Send( 0x03, true, true );
//			delayMicroseconds(4500); // wait min 4.1ms

			// second try
			AObject->Send( 0x03, true, true );
//			delayMicroseconds(4500); // wait min 4.1ms

			// third go!
			AObject->Send( 0x03, true, true );
//			delayMicroseconds(150);

			// finally, set to 4-bit interface
			AObject->Send( 0x02, true, true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3,
		typename T_OutputPins_4,
		typename T_OutputPins_5,
		typename T_OutputPins_6,
		typename T_OutputPins_7
	> class LiquidCrystalDisplayParallel8DataPins : public T_OutputPins_0, public T_OutputPins_1, public T_OutputPins_2, public T_OutputPins_3, public T_OutputPins_4, public T_OutputPins_5, public T_OutputPins_6, public T_OutputPins_7
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )
		_V_PIN_( OutputPins_4 )
		_V_PIN_( OutputPins_5 )
		_V_PIN_( OutputPins_6 )
		_V_PIN_( OutputPins_7 )

	protected:
		template <typename T_IMPLEMENT> inline void SetDataPinsValue( T_IMPLEMENT *AObject, uint8_t AValue, bool A4Bits )
		{
			T_OutputPins_0::SetPinValue( AValue & 0b00000001 );
			T_OutputPins_1::SetPinValue( AValue & 0b00000010 );
			T_OutputPins_2::SetPinValue( AValue & 0b00000100 );
			T_OutputPins_3::SetPinValue( AValue & 0b00001000 );
			T_OutputPins_4::SetPinValue( AValue & 0b00010000 );
			T_OutputPins_5::SetPinValue( AValue & 0b00100000 );
			T_OutputPins_6::SetPinValue( AValue & 0b01000000 );
			T_OutputPins_7::SetPinValue( AValue & 0b10000000 );

			AObject->PulseEnable();
		}

	protected:
		inline void ApplyMode( uint8_t &ADisplayFunction )
		{
			ADisplayFunction |= LCDConsts::LCD_8BITMODE;
		}

		template <typename T_IMPLEMENT> void InitRegisters( T_IMPLEMENT *AObject )
		{
			uint8_t ADisplayFunction = LCDConsts::LCD_8BITMODE;
			if( AObject->GetHeight() > 1 )
				ADisplayFunction |= LCDConsts::LCD_2LINE;

			if( AObject->Hight10Pixels() && ( AObject->GetHeight() == 1 ))
				ADisplayFunction |= LCDConsts::LCD_5x10DOTS;

			// this is according to the hitachi HD44780 datasheet
			// page 45 figure 23

			// Send function set command sequence
			AObject->Command( LCDConsts::LCD_FUNCTIONSET | ADisplayFunction);
			delayMicroseconds(4500);  // wait more than 4.1ms

			// second try
			AObject->Command( LCDConsts::LCD_FUNCTIONSET | ADisplayFunction);
			delayMicroseconds(150);

			// third go
			AObject->Command( LCDConsts::LCD_FUNCTIONSET | ADisplayFunction);
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_DATA_PINS,
		typename T_OutputPins_RS,
		typename T_OutputPins_RW,
		typename T_OutputPins_ENABLE
	> class LiquidCrystalDisplayParallel :
		public T_DATA_PINS,
		public T_OutputPins_RS,
		public T_OutputPins_RW,
		public T_OutputPins_ENABLE
	{
	public:
		_V_PIN_( OutputPins_RS )
		_V_PIN_( OutputPins_RW )
		_V_PIN_( OutputPins_ENABLE )

	protected:
		static const uint8_t	EXEC_TIME = 37;
		static const uint8_t	RS_PIN_INDEX = 0;
		static const uint8_t	RW_PIN_INDEX = 1;
		static const uint8_t	ENABLE_PIN_INDEX = 2;

	public:
		void Send( uint8_t value, bool ACommand, bool A4Bits )
		{
			T_OutputPins_RS::SetPinValue( ! ACommand );
//			digitalWrite( __Pins[ RS_PIN_INDEX ], ( ! ACommand ) );

			// if there is a RW pin indicated, set it low to Write
			// ---------------------------------------------------
			T_OutputPins_RW::SetPinValueLow();
//			if( __Pins[ RW_PIN_INDEX ] != 255 )
//				digitalWrite( __Pins[ RW_PIN_INDEX ], LOW );

//			if( ! A4Bits )
//				WriteNbits( value >> 4, 4 );

//			WriteNbits( value, 4 );

			T_DATA_PINS::SetDataPinsValue( this, value, A4Bits );

			delayMicroseconds( EXEC_TIME ); // wait for the command to execute by the LCD
		}

/*
		void WriteNbits(uint8_t value, uint8_t numBits)
		{
			for (uint8_t i = 0; i < numBits; i++)
				digitalWrite( __Pins[ i + 3 ], (value >> i) & 0x01);

			PulseEnable();
		}
*/
		void PulseEnable(void)
		{
		   // There is no need for the delays, since the digitalWrite operation
		   // takes longer.
//		   digitalWrite( __Pins[ ENABLE_PIN_INDEX ], HIGH );
		   T_OutputPins_ENABLE::SetPinValueHigh();
		   delayMicroseconds( 1 );          // enable pulse must be > 450ns
//		   digitalWrite( __Pins[ ENABLE_PIN_INDEX ], LOW );
		   T_OutputPins_ENABLE::SetPinValueLow();
		}

/*
	protected:
		template <typename T_IMPLEMENT> void InitRegisters( T_IMPLEMENT *AObject )
		{
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
		}
*/
	protected:
		template <typename T_IMPLEMENT> void DisplayInit( T_IMPLEMENT *AObject )
		{
//			for (uint8_t i = 0; i < C_PIN_COUNT + 3; i++)
//				pinMode( __Pins[ i ], OUTPUT );

			// Now we pull both RS and R/W low to begin commands
//			digitalWrite( __Pins[ RS_PIN_INDEX ], LOW);
//			digitalWrite( __Pins[ ENABLE_PIN_INDEX ], LOW);

			T_OutputPins_RS::SetPinValueLow();
			T_OutputPins_ENABLE::SetPinValueLow();

//			if( __Pins[ RW_PIN_INDEX ] != 255 )
//				digitalWrite( __Pins[ RW_PIN_INDEX ], LOW);

			T_OutputPins_RW::SetPinValueLow();

			// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
			// according to datasheet, we need at least 40ms after power rises above 2.7V
			// before sending commands. Arduino can turn on way before 4.5V so we'll wait
			// 50
			// ---------------------------------------------------------------------------
			delay( 100 ); // 100ms delay

//			Serial.println( "STEP1" );


			T_DATA_PINS::InitRegisters( AObject );

			// finally, set # lines, font size, etc.
			uint8_t ADisplayFunction = 0;
			T_DATA_PINS::ApplyMode( ADisplayFunction );
//			if( C_PIN_COUNT == 8 )
//				ADisplayFunction |= LCDConsts::LCD_8BITMODE;

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
//			_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
			// set the entry mode
//			command(LCD_ENTRYMODESET | _displaymode);

//			UpdateBacklight();
		}

	};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_Index,
		typename T_BYTES
	> class LiquidCrystalElementDefineCustomCharacter :
		public T_Index
	{
	public:
		_V_PROP_( Index )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_INSTANCE.Command( LCDConsts::LCD_SETCGRAMADDR | ( Index().GetValue() << 3 ));
			delayMicroseconds( 30 );

			for( int i = 0; i < 8; i ++ )
			{
//				C_INSTANCE.Write( pgm_read_byte( _CharMap + i ) );
				C_INSTANCE.Write( T_BYTES::GetValue( i ) );
				delayMicroseconds( 40 );
			}
		}

	public:
		inline void SystemInit()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_Column,
		typename T_FModified,
		typename T_Index,
		typename T_Row
	> class LiquidCrystalElementCustomCharacterField :
		public T_Column,
		public T_FModified,
		public T_Index,
		public T_Row
	{
	public:
		_V_PROP_( Column )
		_V_PROP_( Row )
		_V_PROP_( Index )

	protected:
		_V_PROP_( FModified )

	public:
		inline void UpdateIndex()
		{
			FModified() = true;
		}

		void SetColumn( uint8_t AValue )
		{
			if( Column() == AValue )
				return;

			T_Column::SetValue( AValue );
			FModified() = true;
		}

		void SetRow( uint8_t AValue )
		{
			if( Row() == AValue )
				return;

			T_Row::SetValue( AValue );
			FModified() = true;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			DisplayCharacter();
		}

	public:
		inline void SystemStart()
		{
			DisplayCharacter();
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
			{
				DisplayCharacter();
				FModified() = false;
			}
		}

	protected:
		void DisplayCharacter()
		{
			C_INSTANCE.SetCursor( Column(), Row() );
			C_INSTANCE.Write( (uint8_t)Index() );
		}

	public:
		LiquidCrystalElementCustomCharacterField()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class LiquidCrystalDisplayInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( *(T*)_Data );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class LiquidCrystalDisplayInput<T_LCD, C_LCD, uint64_t>
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( Func::UInt64ToString( *(uint64_t*)_Data ) );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class LiquidCrystalDisplayInput<T_LCD, C_LCD, int64_t>
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( Func::Int64ToString( *(int64_t*)_Data ) );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class LiquidCrystalDisplayInput<T_LCD, C_LCD, bool>
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( *(bool*)_Data ? "true" : "false" );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class LiquidCrystalDisplayInput<T_LCD, C_LCD, char>
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.println( "DoReceive" );
//			Serial.print( (char*)_Data );
//			if( *(uint8_t*)_Data < ' ' )
//				return;

			if( *(char*)_Data == '\r' )
				C_LCD.NewLine( 0 );

			else if( *(char*)_Data == '\n' )
				C_LCD.ClearLine();

			else if( *(char*)_Data >= ' ' )
			{
				int AClearSize = C_LCD.Print( *(char*)_Data );
				C_LCD.MoveChars( AClearSize );
			}
//			int AClearSize = C_LCD.FLcd->print( (char*)_Data );
//			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class LiquidCrystalDisplayInput_String
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( (char*)_Data );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class LiquidCrystalDisplayInput_Clock
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( "(Clock)" );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T_OBJECT> class LiquidCrystalDisplayInput_Object
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( ((T_OBJECT *)_Data)->ToString().c_str() );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T_OBJECT> class LiquidCrystalDisplayInput_Binary
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_LCD.ClearLine();
			int AClearSize = C_LCD.Print( ((T_OBJECT *)_Data)->ToString().c_str() );
			C_LCD.NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class LiquidCrystalDisplayFieldInput : public LiquidCrystalDisplayInput<T_LCD, C_LCD, T> {};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class LiquidCrystalDisplayFieldInput_Binary : public LiquidCrystalDisplayInput_Binary<T_LCD, C_LCD, T> {};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class LiquidCrystalDisplayFieldInput_Object : public LiquidCrystalDisplayInput_Object<T_LCD, C_LCD, T> {};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class LiquidCrystalDisplayFieldInput_String : public LiquidCrystalDisplayInput_String<T_LCD, C_LCD, T> {};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class LiquidCrystalDisplayFieldInput<T_LCD, C_LCD, char>
    {
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.println( "LiquidCrystalDisplayFieldInput" );
			C_LCD.ClearLine();
            C_LCD.Write( *(char*)_Data );
			C_LCD.NewLine( 1 );
		}
    };
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_AllignLeft,
		typename T_Column,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Row,
		typename T_Width
	> class LiquidCrystalElementTextField :
		public T_AllignLeft,
		public T_Column,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Row,
		public T_Width
	{
	public:
		_V_PROP_( Column )
		_V_PROP_( Row )
		_V_PROP_( Width )
		_V_PROP_( AllignLeft )
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )

	public:
		inline void ClearLine()
		{
			C_INSTANCE.SetCursor( Column(), Row() );
		}

		void NewLine( int AClearSize )
		{
			for( int i = 0; i < (int)Width() - AClearSize; ++ i )
				C_INSTANCE.Print( FillCharacter() );
		}

	public:
/*
		template <typename T> inline size_t Print( T AValue )
		{
			C_INSTANCE.Print( AValue );
		}
*/
		size_t Print( const Mitov::String AValue )
		{
			const char *APtr = AValue.c_str();
			size_t ACount = 0;
			while( *APtr )
			{
				if( *APtr == '\r' )
				{
					ClearLine();
					ACount = 0;
					continue;
				}

				C_INSTANCE.Write( *APtr++ );
				++ ACount;
				if( ACount == Width() )
					break;

			}

			return ACount;
		}

		size_t Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			return Print( Mitov::String( AText ));
		}

		size_t Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			return Print( Mitov::String( AText ));
		}

		size_t Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			return Print( Mitov::String( AText ));
		}

		inline size_t Print( char AValue )
		{
			C_INSTANCE.Write( (uint8_t)AValue );
			return 1;
		}

		inline size_t Print( uint8_t AValue )
		{
			C_INSTANCE.Write( AValue );
			return 1;
		}

		inline void Write( uint8_t AValue )
		{
			C_INSTANCE.Write( AValue );
		}

		inline void MoveChars( int ACount )
		{
//			FCursorPos() = FCursorPos() + 1;
//			++FCursorPos;
		}

	public:
		inline void SystemStart()
		{
			ClearLine();
			int AClearSize = C_INSTANCE.Print( InitialValue() );
			NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_INSTANCE, typename T_INITIAL_VALUE, typename T_WIDTH, typename T_LEFT_ALLIGN, typename T_FILL_CHAR, typename T_COLUMN, typename T_ROW> class LiquidCrystalElementTextField_Clocked : public LiquidCrystalElementTextField<T_OWNER, C_INSTANCE, T_INITIAL_VALUE, T_WIDTH, T_LEFT_ALLIGN, T_FILL_CHAR, T_COLUMN, T_ROW>
	{
		typedef Mitov::LiquidCrystalElementTextField<T_OWNER, C_INSTANCE, T_INITIAL_VALUE, T_WIDTH, T_LEFT_ALLIGN, T_FILL_CHAR, T_COLUMN, T_ROW> inherited;

	protected:
		Mitov::String	FCurrentValue;

	public:
		inline void ClearLine()
		{
			FCurrentValue = "";
		}

		inline void NewLine( int AClearSize ) {}

		size_t Print( const Mitov::String AValue )
		{
			const char *APtr = AValue.c_str();
			size_t ACount = 0;
			while( *APtr )
			{
				if( *APtr == '\r' )
				{
					ClearLine();
					ACount = 0;
					continue;
				}

				Write( *APtr++ );
				++ ACount;
			}

			return ACount;
		}

		size_t Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			FCurrentValue = AText;
			return FCurrentValue.length();
		}

		size_t Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			FCurrentValue = AText;
			return FCurrentValue.length();
		}

		size_t Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			FCurrentValue = AText;
			return FCurrentValue.length();
		}

		inline size_t Print( char AValue )
		{
			FCurrentValue += AValue;
			return 1;
		}

		inline size_t Print( uint8_t AValue )
		{
			FCurrentValue += *(char *)&AValue;
			return 1;
		}

		inline void Write(uint8_t AValue)
		{
			FCurrentValue += *(char *)&AValue;
		}

		void ClockInputPin_o_Receive( void *_Data )
		{
			inherited::ClearLine();
			int AClearSize = inherited::Print( FCurrentValue );
			inherited::NewLine( AClearSize );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_AllignLeft,
		typename T_Base,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Column,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Row,
		typename T_Width
	> class LiquidCrystalElementIntegerField :
		public T_AllignLeft,
		public T_Base,
		public T_ClockInputPin_o_IsConnected,
		public T_Column,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Row,
		public T_Width
	{
	public:
		_V_PROP_( Base )
		_V_PROP_( Column )
		_V_PROP_( Row )
		_V_PROP_( Width )
		_V_PROP_( AllignLeft )
		_V_PROP_( FillCharacter )

	public:
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		int PrintValue( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, Base() );
			if( Width() < 15 )
				AText[ Width() ] = '\0';

			if( AllignLeft() )
				return C_INSTANCE.Print( AText );

			for( int i = 0; i < Width() - strlen( AText ); ++i )
				C_INSTANCE.Print( FillCharacter() );

			C_INSTANCE.Print( AText );
			return Width();
		}

	public:
		inline void ClearLine()
		{
			C_INSTANCE.SetCursor( Column(), Row() );
		}

		void NewLine( int AClearSize )
		{
			for( int i = 0; i < (int)Width() - AClearSize; ++ i )
				C_INSTANCE.Print( FillCharacter() );
		}

	protected:
		void ClearPrintValue()
		{
//			Serial.println( AValue );
			ClearLine();
			int AClearSize = PrintValue( InitialValue() );
			NewLine( AClearSize );
		}

	public:
		inline void SystemStart()
		{
			ClearPrintValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t*)_Data;

			if( ! ClockInputPin_o_IsConnected() )
				ClearPrintValue();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ClearPrintValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_AllignLeft,
		typename T_Base,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Column,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Row,
		typename T_Width
	> class LiquidCrystalElementUnsignedField :
		public T_AllignLeft,
		public T_Base,
		public T_ClockInputPin_o_IsConnected,
		public T_Column,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Row,
		public T_Width
	{
	public:
		_V_PROP_( Base )
		_V_PROP_( Column )
		_V_PROP_( Row )
		_V_PROP_( Width )
		_V_PROP_( AllignLeft )
		_V_PROP_( FillCharacter )

	public:
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		int PrintValue( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, Base() );
			if( Width() < 15 )
				AText[ Width() ] = '\0';

			return C_INSTANCE.Print( AText );
		}

	public:
		inline void ClearLine()
		{
			C_INSTANCE.SetCursor( Column(), Row() );
		}

		void NewLine( int AClearSize )
		{
			for( int i = 0; i < (int)Width() - AClearSize; ++ i )
				C_INSTANCE.Print( FillCharacter() );
		}

	protected:
		void ClearPrintValue()
		{
//			Serial.println( AValue );
			ClearLine();
			int AClearSize = PrintValue( InitialValue() );
			NewLine( AClearSize );
		}

	public:
		inline void SystemStart()
		{
			ClearPrintValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(uint32_t*)_Data;

			if( ! ClockInputPin_o_IsConnected() )
				ClearPrintValue();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ClearPrintValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_AllignLeft,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Column,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_MinWidth,
		typename T_Precision,
		typename T_Row,
		typename T_Width
	> class LiquidCrystalElementAnalogField :
		public T_AllignLeft,
		public T_Column,
		public T_ClockInputPin_o_IsConnected,
		public T_FillCharacter,
		public T_InitialValue,
		public T_MinWidth,
		public T_Precision,
		public T_Row,
		public T_Width
	{
	public:
		_V_PROP_( MinWidth )
		_V_PROP_( Precision )
		_V_PROP_( Width )
		_V_PROP_( AllignLeft )
		_V_PROP_( FillCharacter )
		_V_PROP_( Column )
		_V_PROP_( Row )

	public:
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		inline int PrintValue( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  MinWidth(), Precision(), AText );
			if( Width() < 15 )
				AText[ Width() ] = '\0';

//			Serial.println( AText );
			return C_INSTANCE.Print( AText );
		}

	public:
		inline void ClearLine()
		{
			C_INSTANCE.SetCursor( Column(), Row() );
		}

		void NewLine( int AClearSize )
		{
			for( int i = 0; i < (int)Width() - AClearSize; ++ i )
				C_INSTANCE.Print( FillCharacter() );
		}

	protected:
		void ClearPrintValue()
		{
//			Serial.println( AValue );
			ClearLine();
			int AClearSize = PrintValue( InitialValue() );
			NewLine( AClearSize );
		}

	public:
		inline void SystemStart()
		{
			ClearPrintValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float*)_Data;

			if( ! ClockInputPin_o_IsConnected() )
				ClearPrintValue();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ClearPrintValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_AllignLeft,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Column,
		typename T_FalseValue,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Row,
		typename T_TrueValue,
		typename T_Width
	> class LiquidCrystalElementDigitalField :
		public T_AllignLeft,
		public T_ClockInputPin_o_IsConnected,
		public T_Column,
		public T_FalseValue,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Row,
		public T_TrueValue,
		public T_Width
	{
	public:
		_V_PROP_( TrueValue )
		_V_PROP_( FalseValue )
		_V_PROP_( Column )
		_V_PROP_( Row )
		_V_PROP_( Width )
		_V_PROP_( AllignLeft )
		_V_PROP_( FillCharacter )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( InitialValue )

	protected:
		int PrintValue( bool AValue )
		{
			if( AValue )
				return C_INSTANCE.Print( TrueValue() );

			return C_INSTANCE.Print( FalseValue() );
		}


	public:
		inline void ClearLine()
		{
			C_INSTANCE.SetCursor( Column(), Row() );
		}

		void NewLine( int AClearSize )
		{
			for( int i = 0; i < (int)Width() - AClearSize; ++ i )
				C_INSTANCE.Print( FillCharacter() );
		}

	protected:
		void ClearPrintValue()
		{
//			Serial.println( AValue );
			ClearLine();
			int AClearSize = PrintValue( InitialValue() );
			NewLine( AClearSize );
		}

	public:
		inline void SystemStart()
		{
			ClearPrintValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(bool*)_Data;

			if( ! ClockInputPin_o_IsConnected() )
				ClearPrintValue();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ClearPrintValue();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif