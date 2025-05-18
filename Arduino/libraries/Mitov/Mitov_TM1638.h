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
//#include <Mitov_7SegmentDigits.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	static const uint8_t PROGMEM C_TM1638_7SegmentDigitSegments[16] =
	{
		0b00111111, // 0x3F, // 0 
		0b00000110, // 0x06, // 1 
		0b01011011, // 0x5B, // 2 
		0b01001111, // 0x4F, // 3 
		0b01100110, // 0x66, // 4 
		0b01101101, // 0x6D, // 5 
		0b01111101, // 0x7D, // 6 
		0b00000111, // 0x07, // 7 
		0b01111111, // 0x7F, // 8 
		0b01101111, // 0x6F, // 9 
		0b01110111, // 0x77, // A 
		0b01111100, // 0x7C, // B 
		0b00111001, // 0x39, // C 
		0b01011110, // 0x5E, // D 
		0b01111001, // 0x79, // E 
		0b01110001  // 0x71  // F 
	};
//---------------------------------------------------------------------------
namespace TM1638
{
//	const uint8_t ACTIVATE_TM	= 0x8F; // Start up
	const uint8_t BUTTONS_MODE	= 0x42; // Buttons mode
	const uint8_t DISPLAY_OFF	= 0x80;
	const uint8_t WRITE_INC		= 0x40; // Incremental write
	const uint8_t WRITE_LOC		= 0x44; // Write to a location
	const uint8_t BRIGHT_ADR	= 0x88; // Brightness address
	const uint8_t SEG_ADR		= 0xC0;  // leftmost segment Address C0 C2 C4 C6 C8 CA CC CE
	const uint8_t LEDS_ADR		= 0xC1;  // Leftmost LED address C1 C3 C5 C7 C9 CB CD CF
}
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue
	> class TM1638_LED_Channel :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

/*
	public:
		inline void GetDisplayValue( uint8_t &AValue )
		{
			if( InitialValue() )
				AValue |= uint8_t( 1 ) << C_INDEX;

		}
*/
	protected:
		void DisplayLED( bool AValue )
		{
			pinMode(C_OWNER.GetOwner().GetDataOutputPin(), OUTPUT);
			C_OWNER.GetOwner().SendCommand( TM1638::WRITE_LOC );
			C_OWNER.GetOwner().ChipSelectOutputPin().SetPinValueLow();

			shiftOut( C_OWNER.GetOwner().GetDataOutputPin(), C_OWNER.GetOwner().GetClockOutputPin(), LSBFIRST, TM1638::LEDS_ADR + ( C_INDEX << 1));
//			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR );   // set starting address to 0
			shiftOut( C_OWNER.GetOwner().GetDataOutputPin(), C_OWNER.GetOwner().GetClockOutputPin(), LSBFIRST, AValue ? 1 : 0 );

			C_OWNER.GetOwner().ChipSelectOutputPin().SetPinValueHigh();		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			DisplayLED( *(bool *)_Data );
//			InitialValue() = *(bool *)_Data;
//			C_OWNER.Invalidate();
		}

	public:
		inline void SystemInit()
		{
			DisplayLED( InitialValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue
	> class TM1638_Segments :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		uint8_t	FValues;

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			if( Enabled() )
				C_OWNER.SetDigitSegments( C_START_DIGIT, FValues );

			else
				C_OWNER.SetDigitSegments( C_START_DIGIT, 0x00 );

		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( Func::SetBitField( &FValues, AIndex, AValue ) )
				if( Enabled() )
					C_OWNER.SetDigitSegments( C_START_DIGIT, FValues );

		}

	public:
		inline void SystemInit()
		{
			if( InitialValue() )
				FValues = 0xFF;

			else
				FValues = 0;

			C_OWNER.SetDigitSegments( C_START_DIGIT, FValues );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialDecmalPointValue,
		typename T_InitialValue
	> class TM1638_ValueSection7Segments :
		public T_Enabled,
		public T_InitialDecmalPointValue,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialDecmalPointValue )
		_V_PROP_( InitialValue )

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
//			Serial.println( pgm_read_byte( C_TM1638_7SegmentDigitSegments ), BIN );

			uint8_t AValue = ( Enabled() ) ? pgm_read_byte( C_TM1638_7SegmentDigitSegments + ( InitialValue().GetValue() & 0xF )) | (( InitialDecmalPointValue() & 1 ) ? 0b10000000 : 0 ) : 0;
//			uint8_t AValue = ( Enabled() ) ? 0 | C_TM1638_7SegmentDigitSegments[ InitialDecmalPointValue() & 1 ] : 0;
			C_OWNER.SetDigitSegments( C_START_DIGIT, AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t *)_Data;
			if( AValue < 0 )
				AValue = 0;

			if( AValue > 0xF )
				AValue = 0xF;

			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
			UpdateDisplay();
		}

		inline void DecmalPointInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( InitialDecmalPointValue().GetValue() == AValue )
				return;

			InitialDecmalPointValue() = AValue;
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_LeadingZeroes
	> class TM1638_IntegerDisplay7Segments :
		public T_Enabled,
		public T_InitialValue,
		public T_LeadingZeroes
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( LeadingZeroes )

	protected:
		uint8_t	GetSegmentsValue( int ADigit, int32_t AInitialValue )
		{
//			Serial.println( AInitialValue );
//			Serial.println( pow( 10, ADigit ));
			int AValue;
			if( AInitialValue > ( pow( 10, C_COUNT_DIGITS ) - 1 ) )
				AValue = 0b0000001; // Overflow +

			else if( AInitialValue < ( -( pow( 10, C_COUNT_DIGITS - 1 ) - 1 ) ) )
				AValue = 0b0001000; // Overflow -

			else
			{
				AValue = int32_t( abs( AInitialValue ) ) / ( pow( 10, ADigit ));
//				Serial.print( ADigit ); Serial.print( " - " ); Serial.println( AValue );
				if( ( AInitialValue < 0 ) && ( ADigit > 0 ))
				{
					if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
					{
						int APreviousValue = ( -AInitialValue ) / ( pow( 10, ADigit - 1  ));
						if( APreviousValue > 0 )
							AValue = 0b1000000; // -

						else
							AValue = 0;

					}

					else
					{
						if( ADigit == C_COUNT_DIGITS - 1 )
							AValue = 0b0000001;

						else
						{
							int ADigitValue = AValue % 10;
							AValue = pgm_read_byte( C_TM1638_7SegmentDigitSegments + ADigitValue );
						}
					}
				}

				else if( ( AInitialValue == 0 ) && ( ADigit == 0 ) )
					AValue = pgm_read_byte( C_TM1638_7SegmentDigitSegments );

				else
				{					
					if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
						AValue = 0;

					else
					{
						int ADigitValue = AValue % 10;
						AValue = pgm_read_byte( C_TM1638_7SegmentDigitSegments + ADigitValue );
					}
				}
			}

			return AValue;
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			for( int ADigit = 0; ADigit < C_COUNT_DIGITS; ++ ADigit )
			{
				uint8_t AValue;

				if( Enabled() )
					AValue = GetSegmentsValue( C_COUNT_DIGITS - ADigit - 1, InitialValue() );

				else
					AValue = 0;

//				Serial.print( "ADigit = " ); Serial.println( ADigit );
//				Serial.print( "AValue = " ); Serial.println( AValue, HEX );
//				Serial.println( "" );

				C_OWNER.SetDigitSegments( C_START_DIGIT + ADigit, AValue );
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t *)_Data;
			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
//			Serial.println( InitialValue );
			UpdateDisplay();
		}

	public:
		inline void SystemStart() 
		{
			UpdateDisplay();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Precision
	> class TM1638_AnalogDisplay7Segments :
		public T_Enabled,
		public T_InitialValue,
		public T_Precision
	{
	public:
		_V_PROP_( Precision )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( InitialValue )

	protected:
		char	*FBuffer;
		char	*FStartChar;
		uint8_t	FTextLength;
		uint8_t	FFirstPos;

		char	FDecimalPointPos;

	public:
		uint8_t	GetSegmentsValue( int ADigit, float AInitialValue )
		{
			int AValue;
			int ANumDigits = C_COUNT_DIGITS;

//			Serial.print( "ANumDigits = " );
//			Serial.println( ANumDigits );

			if( ! FStartChar )
			{
				FStartChar = dtostrf( AInitialValue, 1, Precision(), FBuffer );
//				Serial.print( "FStartChar = " );
//				Serial.println( FStartChar );
				Mitov::String AText( FStartChar );
				FTextLength = AText.length(); // strlen( FStartChar ); 
//				FTextLength = strlen( FStartChar ); 
//				Serial.println( FTextLength );
				FDecimalPointPos = AText.indexOf( '.' );
				if( Precision() == 0 )
				{
					if( FTextLength <= ANumDigits )
						FFirstPos = FTextLength - 1;

					else
						FFirstPos = ANumDigits - 1;

				
//					Serial.println( FStartChar );
//					Serial.println( FTextLength );
//					Serial.println( FFirstPos );
//					Serial.println( FDecimalPointPos );
				}

				else
				{
					if( FTextLength <= ANumDigits + 1 )
						FFirstPos = FTextLength - 1;

					else
						FFirstPos = ANumDigits + 1 - 1;
				}				

//				Serial.print( "FFirstPos = " );
//				Serial.println( FFirstPos );
			}

			int ACorrectedTextLength = FTextLength;
			if( Precision() > 0 )
				--ACorrectedTextLength;

			if( ( ACorrectedTextLength - Precision() ) > ANumDigits )
			{
				if( AInitialValue > 0 )
					AValue = 0b0000001; // Overflow +

				else
					AValue = 0b0001000; // Overflow -
			}

			else
			{
				int ATextPos = FFirstPos - ADigit;
				if( ATextPos < 0 )
					AValue = 0;

				else
				{
					if( ATextPos < 0 )
						return( 0 );

					bool ADecimalPoint = ( FStartChar[ ATextPos ] == '.' );
					if( ATextPos <= FDecimalPointPos )
						--ATextPos;

					else if( ADecimalPoint )
						--ATextPos;

					if( ATextPos < 0 )
						return( 0 );

//					Serial.print( "ATextPos = " );
//					Serial.println( ATextPos );
/*
					if( FDigit == 0 )
					{
						Serial.println( FStartChar );
						Serial.println( ATextPos );
					}
*/
					if( FStartChar[ ATextPos ] == '-' )
						AValue = 0b1000000;

					else
					{
						AValue = FStartChar[ ATextPos ] - '0';
						AValue = pgm_read_byte( C_TM1638_7SegmentDigitSegments + AValue );
					}

					if( ADecimalPoint )
						AValue |= 0x80;

//					Serial.println( ADigit );
//					Serial.println( AValue, HEX );
				}
			}

			return AValue;
		}

	public:
		inline void SystemStart() 
		{
			FBuffer = new char[ 15 + Precision() ];

			UpdateDisplay();
		}

//	public:
//		using inherited::inherited;

//	protected:
//		virtual uint8_t GetSegmentsValue( int &ADigit ) = 0;

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			FStartChar = nullptr;
			for( int ADigit = 0; ADigit < C_COUNT_DIGITS; ++ ADigit )
			{
				uint8_t AValue;

				if( Enabled() )
					AValue = GetSegmentsValue( C_COUNT_DIGITS - ADigit - 1, InitialValue() );

				else
					AValue = 0;

//				Serial.print( "ADigit = " ); Serial.println( ADigit );
//				Serial.print( "AValue = " ); Serial.println( AValue, HEX );
//				Serial.println( "" );

				C_OWNER.SetDigitSegments( C_START_DIGIT + ADigit, AValue );
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
//			Serial.println( InitialValue );
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Brightness,
		typename T_Enabled
//		typename T_FModified,
//		typename T_LEDs_GetDisplayValue
	> class TM1638_8Keys_8Leds_Display :
		public T_Brightness,
		public T_Enabled
//		public T_FModified
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Brightness )

//	protected:
//		_V_PROP_( FModified )

	public:
		static T_OWNER &GetOwner() { return C_OWNER; }

		void SetDigitSegments( uint8_t AIndex, uint8_t AValue )
		{
			if( AIndex > 7 )
				return;

			C_OWNER.SendCommand( TM1638::WRITE_LOC );
			C_OWNER.ChipSelectOutputPin().SetPinValueLow();
			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR + (AIndex << 1));
			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, AValue );
			C_OWNER.ChipSelectOutputPin().SetPinValueHigh();
		}

	public:
		void UpdateBrightness()
		{
			if( Enabled() )
			{
				uint8_t AValue = TM1638::BRIGHT_ADR | ( uint8_t( Brightness().GetValue() * 7 + 0.5 ) & 0x07 );
//				Serial.println( AValue, BIN );
//				delay( 5000 );
				C_OWNER.SendCommand( AValue );
			}

			else
				C_OWNER.SendCommand( TM1638::DISPLAY_OFF );

/*
			C_OWNER.ChipSelectOutputPin().SetPinValueLow();
			C_OWNER.GetSPI().beginTransaction(SPISettings( 8000000, LSBFIRST, SPI_MODE0 ));

			if( Enabled() )
			{
				uint8_t AValue = Brightness().GetValue() * 7 + 0.5;
				C_OWNER.GetSPI().transfer( TM1638::BRIGHT_ADR + AValue & 0x7 );
			}

			else
				shiftOut(_DATA_IO, _CLOCK_IO, LSBFIRST, value);
				C_OWNER.GetSPI().transfer( TM1638::DISPLAY_OFF );

			C_OWNER.GetSPI().endTransaction();
			
			C_OWNER.ChipSelectOutputPin().SetPinValueHigh();
*/
		}

/*
	public:
		inline void Invalidate()
		{
			FModified() = true;
		}
*/

	protected:
/*
		void UpdateLEDs()
		{
			FModified() = false;
//			Serial.println( "UpdateLEDs" );
			uint8_t AValue = 0;
			T_LEDs_GetDisplayValue::Call( AValue );

//			Serial.println( AValue, BIN );

			pinMode(C_OWNER.GetDataOutputPin(), OUTPUT);
			C_OWNER.SendCommand( TM1638::WRITE_LOC );
			C_OWNER.ChipSelectOutputPin().SetPinValueLow();

			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::LEDS_ADR + (7 << 1));
//			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR );   // set starting address to 0
			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, AValue );

//			for (uint8_t i = 0; i < 16; i++)
//				C_OWNER.GetSPI().transfer( 0x00 );

//			C_OWNER.GetSPI().endTransaction();
			C_OWNER.ChipSelectOutputPin().SetPinValueHigh();
		}
*/

	public:
		inline void SystemStart()
		{
			UpdateBrightness();

//			C_OWNER.GetSPI().beginTransaction(SPISettings( 8000000, LSBFIRST, SPI_MODE0 ));

			C_OWNER.SendCommand( TM1638::WRITE_INC ); // set auto increment mode
//			C_OWNER.SendCommand( TM1638::WRITE_LOC );
			C_OWNER.ChipSelectOutputPin().SetPinValueLow();

			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR );   // set starting address to 0
			for( uint8_t i = 0; i < 16; ++i )
				shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, 0x00 );

//			for (uint8_t i = 0; i < 16; i++)
//				C_OWNER.GetSPI().transfer( 0x00 );

//			C_OWNER.GetSPI().endTransaction();
			C_OWNER.ChipSelectOutputPin().SetPinValueHigh();

//			UpdateLEDs();
		}

/*
		inline void SystemLoopUpdateHardware()
		{
			if( FModified().GetValue())
				UpdateLEDs();

		}

	public:
		inline TM1638_8Keys_8Leds_Display()
		{
			FModified() = false;
		}
*/
	};
//---------------------------------------------------------------------------	
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Brightness,
		typename T_Enabled,
		typename T_FModified
	> class TM1638_16Keys_Display :
		public T_Brightness,
		public T_Enabled,
		public T_FModified
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Brightness )

	protected:
		_V_PROP_( FModified )

	protected:
		uint8_t FBuffer[ 8 ] = { 0 };

	public:
		static T_OWNER &GetOwner() { return C_OWNER; }

		void SetDigitSegments( uint8_t AIndex, uint8_t AValue )
		{
			if( AIndex > 7 )
				return;

			for( uint8_t i = 0; i < 8; ++i )
			{
				if( AValue & ( 1 << i ) )
					FBuffer[ i ] |= ( 0b10000000 >> AIndex );

				else
					FBuffer[ i ] &= ~( 0b10000000 >> AIndex );
				
			}

			FModified() = true;

/*
			C_OWNER.SendCommand( TM1638::WRITE_LOC );
			C_OWNER.ChipSelectOutputPin().SetPinValueLow();
			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR + (AIndex << 1));
			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, AValue );
			C_OWNER.ChipSelectOutputPin().SetPinValueHigh();
*/
		}

	protected:
		void RefreshSegments()
		{
/*
			C_OWNER.SendCommand( TM1638::WRITE_LOC );
			for( int i = 0; i < 8; ++i )
			{
				C_OWNER.ChipSelectOutputPin().SetPinValueLow();
				shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR + ( i << 1 ));
				shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, FBuffer[ i ] );
				C_OWNER.ChipSelectOutputPin().SetPinValueHigh();
			}
*/
			C_OWNER.SendCommand( TM1638::WRITE_INC ); // set auto increment mode
			C_OWNER.ChipSelectOutputPin().SetPinValueLow();

			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR );   // set starting address to 0
			for( uint8_t i = 0; i < 8; ++i )
			{
				shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, FBuffer[ i ] );
				shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, 0x00 );
			}

			C_OWNER.ChipSelectOutputPin().SetPinValueHigh();

			FModified() = false;
		}

	public:
		void UpdateBrightness()
		{
			if( Enabled() )
			{
				uint8_t AValue = TM1638::BRIGHT_ADR | ( uint8_t( Brightness().GetValue() * 7 + 0.5 ) & 0x07 );
//				Serial.println( AValue, BIN );
//				delay( 5000 );
				C_OWNER.SendCommand( AValue );
			}

			else
				C_OWNER.SendCommand( TM1638::DISPLAY_OFF );

		}

	public:
		inline void SystemStart()
		{
			UpdateBrightness();

/*
			C_OWNER.SendCommand( TM1638::WRITE_INC ); // set auto increment mode
			C_OWNER.ChipSelectOutputPin().SetPinValueLow();

			shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, TM1638::SEG_ADR );   // set starting address to 0
			for( uint8_t i = 0; i < 16; ++i )
				shiftOut( C_OWNER.GetDataOutputPin(), C_OWNER.GetClockOutputPin(), LSBFIRST, 0x00 );

			C_OWNER.ChipSelectOutputPin().SetPinValueHigh();
*/
		}

		inline void SystemLoopUpdateHardware()
		{
			if( FModified() )
				RefreshSegments();

		}

	public:
		inline TM1638_16Keys_Display()
		{
			FModified() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_SPI, T_SPI &C_SPI,
		typename T_ButtonsOutputPins_0,
		typename T_ButtonsOutputPins_1,
		typename T_ButtonsOutputPins_2,
		typename T_ButtonsOutputPins_3,
		typename T_ButtonsOutputPins_4,
		typename T_ButtonsOutputPins_5,
		typename T_ButtonsOutputPins_6,
		typename T_ButtonsOutputPins_7,
		typename T_ChipSelectOutputPin,
		uint8_t C_ClockOutputPin,
		uint8_t C_DataOutputPin
	> class TM1638_8Keys_8Leds : 
		public T_ButtonsOutputPins_0,
		public T_ButtonsOutputPins_1,
		public T_ButtonsOutputPins_2,
		public T_ButtonsOutputPins_3,
		public T_ButtonsOutputPins_4,
		public T_ButtonsOutputPins_5,
		public T_ButtonsOutputPins_6,
		public T_ButtonsOutputPins_7,
		public T_ChipSelectOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

		_V_PIN_( ButtonsOutputPins_0 )
		_V_PIN_( ButtonsOutputPins_1 )
		_V_PIN_( ButtonsOutputPins_2 )
		_V_PIN_( ButtonsOutputPins_3 )
		_V_PIN_( ButtonsOutputPins_4 )
		_V_PIN_( ButtonsOutputPins_5 )
		_V_PIN_( ButtonsOutputPins_6 )
		_V_PIN_( ButtonsOutputPins_7 )

	public:
		void SendCommand( uint8_t ACommand )
		{
			T_ChipSelectOutputPin::SetPinValueLow();

			shiftOut( C_DataOutputPin, C_ClockOutputPin, LSBFIRST, ACommand );
/*
			SPI.beginTransaction(SPISettings( 8000000, LSBFIRST, SPI_MODE0 ));

			SPI.transfer( ACommand );

			SPI.endTransaction();
*/
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	public:
//		inline static T_SPI &GetSPI() { return C_SPI; }
		inline static uint8_t GetClockOutputPin() { return C_ClockOutputPin; }
		inline static uint8_t GetDataOutputPin() { return C_DataOutputPin; }

	protected:
		void ReadButtons( bool AChangeOnly )
		{
			uint8_t buttons = 0;
			T_ChipSelectOutputPin::SetPinValueLow();
			shiftOut( C_DataOutputPin, C_ClockOutputPin, LSBFIRST, TM1638::BUTTONS_MODE );
			pinMode( C_DataOutputPin, INPUT );

			for (uint8_t i = 0; i < 4; i++)
			{
				uint8_t v = shiftIn( C_DataOutputPin, C_ClockOutputPin, LSBFIRST ) << i;
				buttons |= v;
			}

			pinMode( C_DataOutputPin, OUTPUT );
			T_ChipSelectOutputPin::SetPinValueHigh();

//			Serial.println( buttons, BIN );

			T_ButtonsOutputPins_0::SetPinValue( ( buttons & 0b00000001 ) != 0, AChangeOnly );
			T_ButtonsOutputPins_1::SetPinValue( ( buttons & 0b00000010 ) != 0, AChangeOnly );
			T_ButtonsOutputPins_2::SetPinValue( ( buttons & 0b00000100 ) != 0, AChangeOnly );
			T_ButtonsOutputPins_3::SetPinValue( ( buttons & 0b00001000 ) != 0, AChangeOnly );
			T_ButtonsOutputPins_4::SetPinValue( ( buttons & 0b00010000 ) != 0, AChangeOnly );
			T_ButtonsOutputPins_5::SetPinValue( ( buttons & 0b00100000 ) != 0, AChangeOnly );
			T_ButtonsOutputPins_6::SetPinValue( ( buttons & 0b01000000 ) != 0, AChangeOnly );
			T_ButtonsOutputPins_7::SetPinValue( ( buttons & 0b10000000 ) != 0, AChangeOnly );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_ClockOutputPin, OUTPUT );
			pinMode( C_DataOutputPin, OUTPUT );

			SendCommand( TM1638::WRITE_INC ); // set auto increment mode
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
		uint8_t C_START_KEY_INDEX,
		typename T_OutputPin
	> class TArduinoTM1638_DigitalKeypadKey :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void UpdateKeys( uint8_t AIndex, bool AValue )
		{
			if( AIndex == C_START_KEY_INDEX )
				T_OutputPin::SetPinValue( AValue );

		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_SPI, T_SPI &C_SPI,
		typename T_ButtonsOutputPins_SetPinValue,
		typename T_ChipSelectOutputPin,
		uint8_t C_ClockOutputPin,
		uint8_t C_DataOutputPin
//		typename T_Keys_UpdateKeys
//		uint8_t C_COUNT_KEYS
	> class TArduino_QYF_TM1638_16Keys :
		public T_ChipSelectOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	protected:
		void ReadButtons( bool AChangeOnly )
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			shiftOut( C_DataOutputPin, C_ClockOutputPin, MSBFIRST, TM1638::BUTTONS_MODE );
			pinMode( C_DataOutputPin, INPUT );
			for( uint8_t i = 0; i < 4; ++i )
			{
				uint8_t AValue = shiftIn( C_DataOutputPin, C_ClockOutputPin, LSBFIRST );
				T_ButtonsOutputPins_SetPinValue::Call( ( i * 2 ) & 0xF, AValue == 0x04 );
				T_ButtonsOutputPins_SetPinValue::Call( ( i * 2 + 1 ) & 0xF, AValue == 0x40 );

				T_ButtonsOutputPins_SetPinValue::Call( ( i * 2 + 8 ) & 0xF, AValue == 0x02 );
				T_ButtonsOutputPins_SetPinValue::Call( ( i * 2 + 1 + 8 ) & 0xF, AValue == 0x20 );
			}

			pinMode( C_DataOutputPin, OUTPUT );
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	public:
		void SendCommand( uint8_t ACommand )
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			shiftOut( C_DataOutputPin, C_ClockOutputPin, LSBFIRST, ACommand );
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	public:
		inline static uint8_t GetClockOutputPin() { return C_ClockOutputPin; }
		inline static uint8_t GetDataOutputPin() { return C_DataOutputPin; }

	public:
		inline void SystemInit()
		{
			pinMode( C_ClockOutputPin, OUTPUT );
			pinMode( C_DataOutputPin, OUTPUT );

			SendCommand( TM1638::WRITE_INC ); // set auto increment mode
			ReadButtons( false );
		}

		inline void SystemLoopBegin()
		{
			ReadButtons( true );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
