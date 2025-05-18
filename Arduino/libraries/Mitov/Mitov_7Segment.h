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
	const uint8_t C_Segments[16] =
	{
			0b0111111,  // = 0
			0b0000110,  // = 1
			0b1011011,  // = 2
			0b1001111,  // = 3
			0b1100110,  // = 4
			0b1101101,  // = 5
			0b1111101,  // = 6
			0b0000111,  // = 7
			0b1111111,  // = 8
			0b1101111,  // = 9
			0b1110111,  // = A
			0b1111100,  // = B
			0b1011000,  // = C
			0b1011110,  // = D
			0b1111001,  // = E
			0b1110001   // = F
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_InvertedSegments,
		typename T_SegmentPins_0,
		typename T_SegmentPins_1,
		typename T_SegmentPins_2,
		typename T_SegmentPins_3,
		typename T_SegmentPins_4,
		typename T_SegmentPins_5,
		typename T_SegmentPins_6
	> class Display7Segment :
		public T_Enabled,
		public T_InitialValue,
		public T_InvertedSegments,
		public T_SegmentPins_0,
		public T_SegmentPins_1,
		public T_SegmentPins_2,
		public T_SegmentPins_3,
		public T_SegmentPins_4,
		public T_SegmentPins_5,
		public T_SegmentPins_6
	{
	public:
		_V_PROP_( InvertedSegments )
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t*)_Data;
			UpdateDisplay();
		}

	public:
		inline void SystemInit()
		{
			UpdateDisplay();
		}

	public:
		void UpdateDisplay()
		{
			int AValue = ( Enabled() ) ? C_Segments[ InitialValue() & 0xF ] : 0;

			for( int i = 0; i < 7; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				switch( i )
				{
					case 0 : T_SegmentPins_0::SetPinValue( ABitValue ); break;
					case 1 : T_SegmentPins_1::SetPinValue( ABitValue ); break;
					case 2 : T_SegmentPins_2::SetPinValue( ABitValue ); break;
					case 3 : T_SegmentPins_3::SetPinValue( ABitValue ); break;
					case 4 : T_SegmentPins_4::SetPinValue( ABitValue ); break;
					case 5 : T_SegmentPins_5::SetPinValue( ABitValue ); break;
					case 6 : T_SegmentPins_6::SetPinValue( ABitValue ); break;
				}

				AValue >>= 1;
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_DigitPins_SetPinValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_InvertedDigits,
		typename T_InvertedSegments,
		typename T_LeadingZeroes,
		typename T_RefreshInterval,
		typename T_SegmentPins_0,
		typename T_SegmentPins_1,
		typename T_SegmentPins_2,
		typename T_SegmentPins_3,
		typename T_SegmentPins_4,
		typename T_SegmentPins_5,
		typename T_SegmentPins_6
	> class DisplayIntegerDynamic7Segment :
		public T_Enabled,
		public T_InitialValue,
		public T_InvertedDigits,
		public T_InvertedSegments,
		public T_LeadingZeroes,
		public T_RefreshInterval,
		public T_SegmentPins_0,
		public T_SegmentPins_1,
		public T_SegmentPins_2,
		public T_SegmentPins_3,
		public T_SegmentPins_4,
		public T_SegmentPins_5,
		public T_SegmentPins_6
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( InvertedDigits )
		_V_PROP_( InvertedSegments )
		_V_PROP_( LeadingZeroes )
		_V_PROP_( RefreshInterval )

	protected:
		unsigned long	FLastTime = 0;
		int				FDigit = 0;

	protected:
		inline int	GetSegmentsValue()
		{
			int AValue;
			if( InitialValue() > pow( 10, C_NUM_INPUTS ) - 1 )
				AValue = 0b0000001;

			else if( InitialValue() < -( pow( 10, C_NUM_INPUTS - 1 ) - 1 ) )
				AValue = 0b0001000;

			else
			{
				uint32_t APower = pow( 10, FDigit );
				AValue = ( (int)abs( InitialValue() ) ) / APower;
				if( ( InitialValue() < 0 ) && ( FDigit > 0 ))
				{
					if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
					{
						uint32_t APower1 = pow( 10, FDigit - 1 );
						int APreviousValue = ( - InitialValue() ) / APower1;
						if( APreviousValue > 0 )
							AValue = 0b1000000;

						else
							AValue = 0;

					}
					else
					{
						if( FDigit == C_NUM_INPUTS - 1 )
							AValue = 0b1000000;

						else
						{
							int ADigitValue = AValue % 10;
							AValue = C_Segments[ ADigitValue ];
						}
					}
				}
				else
				{
					if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
						AValue = 0;

					else
					{
						int ADigitValue = AValue % 10;
						AValue = C_Segments[ ADigitValue ];
					}
				}
			}

			return AValue;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t*)_Data;
		}

	public:
		void UpdateDisplay()
		{
			int AValue;
			if( Enabled() )
				AValue = GetSegmentsValue();

			else
				AValue = 0;

			bool ABitValue = false;
			ABitValue ^= InvertedDigits();
			for( int i = 0; i < C_NUM_INPUTS; ++i )
				T_DigitPins_SetPinValue::Call( i, ABitValue );

			ComponentsHardware::SystemUpdateHardware();

			for( int i = 0; i < 7; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				switch( i )
				{
					case 0 : T_SegmentPins_0::SetPinValue( ABitValue ); break;
					case 1 : T_SegmentPins_1::SetPinValue( ABitValue ); break;
					case 2 : T_SegmentPins_2::SetPinValue( ABitValue ); break;
					case 3 : T_SegmentPins_3::SetPinValue( ABitValue ); break;
					case 4 : T_SegmentPins_4::SetPinValue( ABitValue ); break;
					case 5 : T_SegmentPins_5::SetPinValue( ABitValue ); break;
					case 6 : T_SegmentPins_6::SetPinValue( ABitValue ); break;
				}

				AValue >>= 1;
			}

/*
			for( int i = 0; i < 16; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				SegmentPins[ i ].Notify( &ABitValue );
				AValue >>= 1;
			}
*/
			AValue = FDigit;
			for( int i = 0; i < C_NUM_INPUTS; ++i )
			{
				bool ABitValue = ( AValue-- == 0 );
				ABitValue ^= InvertedDigits();
				T_DigitPins_SetPinValue::Call( i, ABitValue );
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateDisplay();
		}

		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ( ACurrentMicros - FLastTime ) < RefreshInterval() )
				return;

			++FDigit;

			if( FDigit >= C_NUM_INPUTS )
				FDigit = 0;

			FLastTime = ACurrentMicros;
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_DigitPins_SetPinValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_InvertedDigits,
		typename T_InvertedSegments,
		typename T_Precision,
		typename T_RefreshInterval,
		typename T_SegmentPins_0,
		typename T_SegmentPins_1,
		typename T_SegmentPins_2,
		typename T_SegmentPins_3,
		typename T_SegmentPins_4,
		typename T_SegmentPins_5,
		typename T_SegmentPins_6,
		typename T_SegmentPins_7
	> class DisplayAnalogDynamic7Segment :
		public T_Enabled,
		public T_InitialValue,
		public T_InvertedDigits,
		public T_InvertedSegments,
		public T_Precision,
		public T_RefreshInterval,
		public T_SegmentPins_0,
		public T_SegmentPins_1,
		public T_SegmentPins_2,
		public T_SegmentPins_3,
		public T_SegmentPins_4,
		public T_SegmentPins_5,
		public T_SegmentPins_6,
		public T_SegmentPins_7
	{
	public:
		_V_PROP_( Precision )
		_V_PROP_( InvertedSegments )
		_V_PROP_( InvertedDigits )
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( RefreshInterval )

	protected:
		char	*FBuffer;
		char	*FStartChar;
		unsigned char	FTextLength;
		unsigned char	FFirstPos;

		char			FDecimalPointPos;

		unsigned long	FLastTime = 0;
		int				FDigit = 0;

	protected:
		int	GetSegmentsValue()
		{
			int AValue;
			int ANumDigits = C_NUM_INPUTS;
			if( ! FStartChar )
			{
				FStartChar = dtostrf( InitialValue(), 1, Precision(), FBuffer );
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

//				Serial.println( FFirstPos );
			}

			int ACorrectedTextLength = FTextLength;
			if( Precision() > 0 )
				--ACorrectedTextLength;

			if( ( ACorrectedTextLength - Precision() ) > ANumDigits )
			{
				if( InitialValue() > 0 )
					AValue = 0b0000001; // Overflow +

				else
					AValue = 0b0001000; // Overflow -
			}

			else
			{
				int ATextPos = FFirstPos - FDigit;
				if( ATextPos < 0 )
					AValue = 0;

				else
				{
					if( ATextPos < 0 )
						return( 0 );

					bool ADecimalPoint = ( FStartChar[ ATextPos ] == '.' );
					if( ATextPos <= FDecimalPointPos )
						--ATextPos;

//					if( ADecimalPoint )
//						--ATextPos;

					if( ATextPos < 0 )
						return( 0 );

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
						AValue = C_Segments[ AValue ];
					}

					if( ADecimalPoint )
						AValue |= 0x80;

				}
			}

			return AValue;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FStartChar = nullptr;
			InitialValue() = *(float *)_Data;
		}

	public:
		void UpdateDisplay()
		{
			int AValue;
			if( Enabled() )
				AValue = GetSegmentsValue();

			else
				AValue = 0;

			bool ABitValue = false;
			ABitValue ^= InvertedDigits();

			for( int i = 0; i < C_NUM_INPUTS; ++i )
				T_DigitPins_SetPinValue::Call( i, ABitValue );

			ComponentsHardware::SystemUpdateHardware();

			for( int i = 0; i < 8; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				switch( i )
				{
					case 0 : T_SegmentPins_0::SetPinValue( ABitValue ); break;
					case 1 : T_SegmentPins_1::SetPinValue( ABitValue ); break;
					case 2 : T_SegmentPins_2::SetPinValue( ABitValue ); break;
					case 3 : T_SegmentPins_3::SetPinValue( ABitValue ); break;
					case 4 : T_SegmentPins_4::SetPinValue( ABitValue ); break;
					case 5 : T_SegmentPins_5::SetPinValue( ABitValue ); break;
					case 6 : T_SegmentPins_6::SetPinValue( ABitValue ); break;
					case 7 : T_SegmentPins_7::SetPinValue( ABitValue ); break;
				}

				AValue >>= 1;
			}

/*
			for( int i = 0; i < 16; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments;
				SegmentPins[ i ].Notify( &ABitValue );
				AValue >>= 1;
			}
*/
			AValue = FDigit;
			for( int i = 0; i < C_NUM_INPUTS; ++i )
			{
				bool ABitValue = ( AValue-- == 0 );
				ABitValue ^= InvertedDigits();
				T_DigitPins_SetPinValue::Call( i, ABitValue );
			}
		}

	public:
		inline void SystemInit()
		{
			FBuffer = new char[ 15 + Precision() ];
			UpdateDisplay();
		}

		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ( ACurrentMicros - FLastTime ) < RefreshInterval() )
				return;

			++FDigit;

			if( FDigit >= C_NUM_INPUTS )
				FDigit = 0;

			FLastTime = ACurrentMicros;
			UpdateDisplay();
		}


/*
		virtual ~DisplayAnalogDynamic7Segment()
		{
			delete [] FBuffer;
		}
*/
	};

}
//---------------------------------------------------------------------------
#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif