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
	const uint8_t C_7SegmentsText[] PROGMEM =
	{
		B10100000, // !
		B00100010, // "
		B00000000, // #
		B01011011, // $
		B00100101, // %
		B01111111, // &
		B00000010, // '

		B01001110, // (
		B01111000, // )
		B01100011, // *
		B00110001, // +
		B00000100, // ,
		B00000001, // -
		B10000000, // .
		B00100101, // /

		B01111110, // 0
		B00110000, // 1
		B01101101, // 2
		B01111001, // 3
		B00110011, // 4
		B01011011, // 5
		B01011111, // 6
		B01110000, // 7

		B01111111, // 8
		B01111011, // 9
		B00000000, // :
		B00000000, // ;
		B00000000, // <
		B01000001, // =
		B00000000, // >
		B01100101, // ?

		B01101111, // @
		B01110111, // A
		B00011111, // B
		B00001101, // C
		B00111101, // D
		B01001111, // E
		B01000111, // F
		B01011110, // G

		B00110111, // H
		B00110000, // I
		B00111100, // J
		B00110111, // K ???
		B00001110, // L
		B00000000, // M ???
		B00010101, // N
		B01111110, // O

		B01100111, // P
		B01111110, // Q
		B00000101, // R
		B01011011, // S
		B00001111, // T
		B00111110, // U
		B00111110, // V
		B00000000, // W ???

		B00110111, // X
		B00110011, // Y
		B01101101, // Z
		B01001110, // [
		B00010011, // backslash
		B01111000, // ]
		B01100010, // ^
		B00001000, // _

		B00100000, // `
		B01110111, // a
		B00011111, // b
		B00001101, // c
		B00111101, // d
		B01001111, // e
		B01000111, // f
		B01111011, // g

		B00010111, // h
		B00010000, // i
		B00111100, // j
		B00110111, // k ???
		B00110000, // l
		B00000000, // m ???
		B00010101, // n
		B00011101, // o

		B01100111, // P
		B01110011, // q
		B00000101, // r
		B01011011, // s
		B00001111, // t
		B00011100, // u
		B00011100, // v
		B00000000, // w ???

		B00110111, // x
		B00110011, // y
		B01101101, // z
		B01001110, // {
		B00000110, // |
		B01111000, // }
		B00010011  // ~
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_ReversedOrder
	> class TextDisplay7SegmentsElement :
		public T_Enabled,
		public T_InitialValue,
		public T_ReversedOrder
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( ReversedOrder )

	protected:
		uint8_t GetSegmentsValue( int &ADigit, Mitov::String AInitialValue )
		{
			int ARealDigit = ( C_COUNT_PIXELS / 8 ) - ADigit - 1;
			if( ARealDigit >= AInitialValue.length() )
				return 0;

			uint8_t AValue = AInitialValue[ ARealDigit ];

			if( AValue <= ' ' )
				return 0;

			if( AValue > 126 )
				return 0;

//			uint8_t AResult = C_7SegmentsText[ AValue ];
			uint8_t AResult = pgm_read_byte( C_7SegmentsText + AValue - ' ' - 1 );
			if( AResult & 0x80 )
				return AResult;

			if( ARealDigit + 1 >= AInitialValue.length() )
				return AResult;

			if( AInitialValue[ ARealDigit + 1 ] != '.' )
				return AResult;

			++ARealDigit;
			AResult |= 0x80;

			return AResult;
		}

//	protected:
//		virtual uint8_t GetSegmentsValue( int &ADigit ) = 0;

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

				if( ReversedOrder() )
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
			Mitov::String AValue = (char *)_Data;
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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
