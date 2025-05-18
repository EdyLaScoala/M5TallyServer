////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	const uint8_t PROGMEM C_7SegmentDigitSegments[ 16 ] =
	{
		0b01111110,  // = 0
		0b00110000,  // = 1
		0b01101101,  // = 2
		0b01111001,  // = 3
		0b00110011,  // = 4
		0b01011011,  // = 5
		0b01011111,  // = 6
		0b01110000,  // = 7
		0b01111111,  // = 8
		0b01111011,  // = 9
		0b01110111,  // = A
		0b00011111,  // = B
		0b00001101,  // = C
		0b00111101,  // = D
		0b01001111,  // = E
		0b01000111   // = F
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialDecmalPointValue,
		typename T_InitialValue
	> class ValueSection7SegmentsElement :
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
			uint8_t AValue = ( Enabled() ) ? pgm_read_byte( C_7SegmentDigitSegments + ( InitialValue() & 0xF )) | (( InitialDecmalPointValue() & 1 ) ? 0b10000000 : 0 ) : 0;

			for( int i = 0; i < 8; ++i )
				C_INSTANCE.SetPixelValue( C_START_PIXEL + i, ( AValue >> i ) & 1 );
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
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_LeadingZeroes,
		typename T_ReversedOrder
	> class IntegerDisplay7SegmentsElement :
		public T_Enabled,
		public T_InitialValue,
		public T_LeadingZeroes,
		public T_ReversedOrder
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( LeadingZeroes )
		_V_PROP_( ReversedOrder )

	protected:
		uint8_t	GetSegmentsValue( int &ADigit, int32_t AInitialValue )
		{
//			Serial.println( AInitialValue );
//			Serial.println( pow( 10, ADigit ));
			int AValue;
			if( AInitialValue > ( pow( 10, ( C_COUNT_PIXELS / 8 ) ) - 1 ) )
				AValue = 0b1000000;

			else if( AInitialValue < ( -( pow( 10, ( C_COUNT_PIXELS / 8 ) - 1 ) - 1 ) ) )
				AValue = 0b0001000;

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
							AValue = 0b0000001;

						else
							AValue = 0;

					}

					else
					{
						if( ADigit == ( C_COUNT_PIXELS / 8 ) - 1 )
							AValue = 0b1000000;

						else
						{
							int ADigitValue = AValue % 10;
							AValue = pgm_read_byte( C_7SegmentDigitSegments + ADigitValue );
						}
					}
				}

				else if( ( AInitialValue == 0 ) && ( ADigit == 0 ) )
					AValue = pgm_read_byte( C_7SegmentDigitSegments );

				else
				{
					
					if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
						AValue = 0;

					else
					{
						int ADigitValue = AValue % 10;
						AValue = pgm_read_byte( C_7SegmentDigitSegments + ADigitValue );
					}
				}
			}

			return AValue;
		}

	public:
		inline void SystemStart() 
		{
			UpdateDisplay();
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			for( int ADigit = 0; ADigit < C_COUNT_PIXELS / 8; ++ ADigit )
			{
				uint8_t AValue;

				if( Enabled() )
					AValue = GetSegmentsValue( ADigit, InitialValue() );

				else
					AValue = 0;

//				Serial.print( "ADigit = " ); Serial.println( ADigit );
//				Serial.print( "AValue = " ); Serial.println( AValue, HEX );
//				Serial.println( "" );

				if( ! ReversedOrder().GetValue() )
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ( ( C_COUNT_PIXELS / 8 ) - ADigit - 1 ) * 8 + i, ( AValue >> i ) & 1 );
				}

				else
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ADigit * 8 + i, ( AValue >> i ) & 1 );
				}
			}
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t *)_Data;
			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
//			Serial.println( InitialValue );
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Precision,
		typename T_ReversedOrder
	> class AnalogDisplay7SegmentsElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Precision,
		public T_ReversedOrder
	{
	public:
		_V_PROP_( Precision )
		_V_PROP_( Enabled )
		_V_PROP_( ReversedOrder )

	public:
		_V_PROP_( InitialValue )

	protected:
		char	*FBuffer;
		char	*FStartChar;
		uint8_t	FTextLength;
		uint8_t	FFirstPos;

		char	FDecimalPointPos;

	public:
		uint8_t	GetSegmentsValue( int &ADigit, float AInitialValue )
		{
			int AValue;
			int ANumDigits = ( C_COUNT_PIXELS / 8 );

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
					AValue = 0b1000000; // Overflow +

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
						AValue = 0b0000001;

					else
					{
						AValue = FStartChar[ ATextPos ] - '0';
						AValue = pgm_read_byte( C_7SegmentDigitSegments + AValue );
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

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			FStartChar = nullptr;
			for( int ADigit = 0; ADigit < C_COUNT_PIXELS / 8; ++ ADigit )
			{
				uint8_t AValue;

				if( Enabled() )
					AValue = GetSegmentsValue( ADigit, InitialValue() );

				else
					AValue = 0;

//				Serial.print( "ADigit = " ); Serial.println( ADigit );
//				Serial.print( "AValue = " ); Serial.println( AValue, HEX );
//				Serial.println( "" );

				if( ! ReversedOrder().GetValue() )
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ( ( C_COUNT_PIXELS / 8 ) - ADigit - 1 ) * 8 + i, ( AValue >> i ) & 1 );
				}

				else
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ADigit * 8 + i, ( AValue >> i ) & 1 );
				}
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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
