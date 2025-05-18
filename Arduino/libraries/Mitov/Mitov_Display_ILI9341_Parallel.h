////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Graphics_Color.h>
#include <Mitov_Display_ILI9341.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <uint16_t C_D0, uint16_t C_D1, uint16_t C_D2, uint16_t C_D3, uint16_t C_D4, uint16_t C_D5, uint16_t C_D6, uint16_t C_D7, uint16_t C_READ, uint16_t C_WRITE, uint16_t C_COMMAND_DATA, uint16_t C_CHIP_SELECT, uint16_t C_RESET> class DisplayILI9341_Parallel
	{
	public:
		inline void SystemLoopBegin( TArduinoGraphicsOrientation AOrientation ) {}

	protected:
		template <typename T> static void SetPinModes( T AMode )
		{
			pinMode( C_D0, AMode );
			pinMode( C_D1, AMode );
			pinMode( C_D2, AMode );
			pinMode( C_D3, AMode );
			pinMode( C_D4, AMode );
			pinMode( C_D5, AMode );
			pinMode( C_D6, AMode );
			pinMode( C_D7, AMode );
		}

		static inline void InitPins()
		{
			SetPinModes( OUTPUT );
			pinMode( C_READ, OUTPUT );
			pinMode( C_WRITE, OUTPUT );
			pinMode( C_COMMAND_DATA, OUTPUT );
			pinMode( C_CHIP_SELECT, OUTPUT );
			pinMode( C_RESET, OUTPUT );

			digitalWrite( C_RESET, HIGH );
			delay(50);
			digitalWrite( C_RESET, LOW );
			delay(100);
	
			digitalWrite( C_RESET, HIGH );
			delay(50);
			digitalWrite(C_CHIP_SELECT,HIGH);
			digitalWrite(C_READ,HIGH);
			digitalWrite(C_WRITE,HIGH);
			delay(10);
		}

		static inline void writeRegister32(uint8_t r, uint32_t d) 
		{
			cmd( r );
			w_data(d >> 24);
			w_data(d >> 16);
			w_data(d >> 8);
			w_data(d);
		}

		static void SetPins( uint8_t cmd )
		{
			digitalWrite( C_D0, ( cmd & ( 1 << 0 ) ) ? HIGH : LOW );
			digitalWrite( C_D1, ( cmd & ( 1 << 1 ) ) ? HIGH : LOW );
			digitalWrite( C_D2, ( cmd & ( 1 << 2 ) ) ? HIGH : LOW );
			digitalWrite( C_D3, ( cmd & ( 1 << 3 ) ) ? HIGH : LOW );
			digitalWrite( C_D4, ( cmd & ( 1 << 4 ) ) ? HIGH : LOW );
			digitalWrite( C_D5, ( cmd & ( 1 << 5 ) ) ? HIGH : LOW );
			digitalWrite( C_D6, ( cmd & ( 1 << 6 ) ) ? HIGH : LOW );
			digitalWrite( C_D7, ( cmd & ( 1 << 7 ) ) ? HIGH : LOW );
		}

		static inline void cmd( uint8_t cmd )
		{
			SetPins( cmd );
//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				digitalWrite( FPins[ i ], ( cmd & ( 1 << i ) ) ? HIGH : LOW );

//			PORTC=PORTC&0Xf0|0x01;

//			PORTB=(0xfc&PORTB)|(0x03&cmd);
//			PORTD=(0x03&PORTD)|(0xfc&cmd);
//			PORTC|=0x02;
//			PORTC|=0x08;

//			digitalWrite( C_READ, HIGH );
			digitalWrite( C_COMMAND_DATA, LOW );
			digitalWrite( C_WRITE, LOW );
			digitalWrite( C_CHIP_SELECT, LOW );

//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				digitalWrite( FPins[ i ], ( cmd & ( 1 << i ) ) ? HIGH : LOW );

			digitalWrite( C_WRITE, HIGH );
			digitalWrite( C_CHIP_SELECT, HIGH );
		}

		static inline void  w_data( uint8_t data )
		{
			SetPins( data );
//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				digitalWrite( FPins[ i ], ( data & ( 1 << i ) ) ? HIGH : LOW );

//			PORTC=PORTC&0Xf0|0x05;
//			PORTB=(0xfc&PORTB)|(0x03&data);
//			PORTD=(0x03&PORTD)|(0xfc&data);
//			PORTC|=0x02;
//			PORTC|=0x08;

			digitalWrite( C_READ, HIGH );
			digitalWrite( C_COMMAND_DATA, HIGH );
			digitalWrite( C_WRITE, LOW );
			digitalWrite( C_CHIP_SELECT, LOW );

//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				digitalWrite( FPins[ i ], ( data & ( 1 << i ) ) ? HIGH : LOW );

			digitalWrite( C_WRITE, HIGH );
			digitalWrite( C_CHIP_SELECT, HIGH );
		}

		static uint8_t r_data()
		{
//			digitalWrite( C_WRITE, HIGH );
			digitalWrite( C_COMMAND_DATA, HIGH );
			digitalWrite( C_CHIP_SELECT, LOW );

			digitalWrite( C_READ, LOW );
//			digitalWrite( C_READ, LOW ); // Delay
//			digitalWrite( C_READ, LOW ); // Delay

			uint8_t AResult = 0;

//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				if( digitalRead( FPins[ i ] ) == HIGH )
//					AResult |= ( 1 << i );

			if( digitalRead( C_D0 ) == HIGH )
				AResult |= ( 1 << 0 );

			if( digitalRead( C_D1 ) == HIGH )
				AResult |= ( 1 << 1 );

			if( digitalRead( C_D2 ) == HIGH )
				AResult |= ( 1 << 2 );

			if( digitalRead( C_D3 ) == HIGH )
				AResult |= ( 1 << 3 );

			if( digitalRead( C_D4 ) == HIGH )
				AResult |= ( 1 << 4 );

			if( digitalRead( C_D5 ) == HIGH )
				AResult |= ( 1 << 5 );

			if( digitalRead( C_D6 ) == HIGH )
				AResult |= ( 1 << 6 );

			if( digitalRead( C_D7 ) == HIGH )
				AResult |= ( 1 << 7 );

			digitalWrite( C_READ, HIGH );
			digitalWrite( C_CHIP_SELECT, HIGH );

//			if( AResult )
//				Serial.println( AResult );

			return AResult;
		}

		inline void w_data_internal( uint8_t data )
		{
			w_data( data );
		}

		inline void w_data_start()
		{
		}

		inline void w_data_end()
		{
		}

		static inline void setAddrWindow(int x1, int y1, int x2, int y2) 
		{
			uint32_t t;

			t = x1;
			t <<= 16;
			t |= x2;
			writeRegister32( ILI9341Extended::COLADDRSET, t );  // HX8357D uses same registers!
			t = y1;
			t <<= 16;
			t |= y2;
			writeRegister32( ILI9341Extended::PAGEADDRSET, t ); // HX8357D uses same registers!
		}

	protected:
//#if ( defined(__ILI9341_SCROLL__) && defined(__PARALLEL_COMPONENT_DISPLAY_ILI9341__))
		template <typename T_INSTANCE> static void InternalScroll( T_INSTANCE &AInstance, int16_t AInWidth, int16_t AInHeight, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			if( X == 0 && Y == 0 )
				return;

			int AFromYIndex, AToYIndex;
			int AHeight = AInHeight - abs( Y );
			if( Y < 0 )
			{
				AFromYIndex = -Y;
				AToYIndex = 0;
			}

			else
			{
				AFromYIndex = 0;
				AToYIndex = Y;
			}

			int AFromXIndex, AToXIndex;
			int AWidth = AInWidth - abs( X );
			if( X < 0 )
			{
				AFromXIndex = -X;
				AToXIndex = 0;
			}

			else
			{
				AFromXIndex = 0;
				AToXIndex = X;
			}

			if( Y < 0 )
			{
				for( int ay = 0; ay < AHeight; ++ay )
				{
					if( X < 0 )
					{
						register uint16_t *ABuffer = new uint16_t[ AWidth ];
						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

//						DDRD &= 0b00000011;
//						DDRB &= 0b11111100;

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						SetPinModes( INPUT );

						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

//						DDRD |= 0b11111100;
//						DDRB |= 0b00000011;

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						SetPinModes( OUTPUT );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
					}

					else
					{
						register uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						SetPinModes( INPUT );

						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						SetPinModes( OUTPUT );
						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, 0, ay + AToYIndex, X, color );
					}
				}

				T_INSTANCE::fillRect( AInstance, 0, AHeight, AInWidth, AInHeight, color );
			}

			else
			{
				for( int ay = AHeight; ay--; )
				{
					if( X < 0 )
					{
						register uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						SetPinModes( INPUT );
						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						SetPinModes( OUTPUT );
						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
					}

					else
					{
						register uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						SetPinModes( INPUT );
						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

//						if( FPins[ FIRST_DATA_PIN ] != _VISUINO_MAX_PIN_NO_ )
//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						SetPinModes( OUTPUT );
						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, 0, ay + AToYIndex, X, color );
					}
				}

				T_INSTANCE::fillRect( AInstance, 0, 0, AInWidth, Y, color );
			}
		}
//#endif

		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
//			return 0xFFFFFFFF;
			setAddrWindow(x,y,x+1,y+1);

			cmd( 0x2E ); // Read command

//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				pinMode( FPins[ i ], INPUT );

			SetPinModes( INPUT );
			r_data(); // Fake read

			TUniColor AResult = r_data();
			AResult <<= 8;
			AResult |= r_data();
			AResult <<= 8;
			AResult |= r_data();

//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				pinMode( FPins[ i ], OUTPUT );

			SetPinModes( OUTPUT );

//			if( AResult )
//				Serial.println( int( AResult ), HEX );

//			return 0xFF000000 | Func::ColorFrom565( AResult ); // Not supported
			return 0xFF030303 | AResult; // Not supported
		}

		static void fillRectImplementation( int16_t AWidth, int16_t AHeight, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
//			Serial.println( "FILL" );
			if((x + w - 1) >= AWidth)
				w = AWidth - x;

			if((y + h - 1) >= AHeight)
				h = AHeight - y;

			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t hi = color >> 8, lo = color;

			cmd( 0x2C );
			for( y=0; y < h; ++y )
				for( x=0; x < w; ++x )
				{
					w_data(hi);
					w_data(lo);
				}

//			Serial.println( "END" );
		}

		inline void drawPixel(int16_t x, int16_t y, TUniColor color, uint8_t AAlpha )
		{
			if( AAlpha < 255 )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				uint8_t	r = color >> 16;
				uint8_t	g = color >> 8;
				uint8_t	b = color;

				TUniColor AOldColor = GetPixelColor( x, y );
				uint8_t	OldR = AOldColor >> 16;
				uint8_t	OldG = AOldColor >> 8;
				uint8_t	OldB = AOldColor;

//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

				r = float( r ) * ACoefficient + float( OldR ) * ( 1 - ACoefficient ) + 0.5;
				g = float( g ) * ACoefficient + float( OldG ) * ( 1 - ACoefficient ) + 0.5;
				b = float( b ) * ACoefficient + float( OldB ) * ( 1 - ACoefficient ) + 0.5;

				color = Func::Color565( r, g, b );
//				color = Func::Color565( 255, 255, 255 );
			}

			else
				color = Func::Color565( color );

			setAddrWindow(x,y,x+1,y+1);

			cmd( 0x2C );

			w_data( color >> 8 ); 
			w_data( color );
		}
/*
		inline static int16_t width( TArduinoGraphicsOrientation AOrientation, int16_t AWidth, int16_t AHeight )
		{
			if( AOrientation == goUp || AOrientation == goDown )
				return AWidth;

			return AHeight;
		}

		inline static int16_t height( TArduinoGraphicsOrientation AOrientation, int16_t AWidth, int16_t AHeight )
		{
			if( AOrientation == goUp || AOrientation == goDown )
				return AHeight;

			return AWidth;
		}
*/
	};
//---------------------------------------------------------------------------
#ifndef __AVR__
	typedef DisplayILI9341_Parallel<8, 9, 2, 3, 4, 5, 6, 7, 14, 15, 16, 17, 18> DisplayILI9341OptimizedD2D9A0A4;
#else
	class DisplayILI9341OptimizedD2D9A0A4 : public DisplayILI9341_Parallel<8, 9, 2, 3, 4, 5, 6, 7, 14, 15, 16, 17, 18>
	{
	protected:
		static inline void cmd( uint8_t cmd )
		{
			PORTC=PORTC&0Xf0|0x01;

			PORTB=(0xfc&PORTB)|(0x03&cmd);
			PORTD=(0x03&PORTD)|(0xfc&cmd);
			PORTC|=0x02;
			PORTC|=0x08;
		}

		static inline void  w_data( uint8_t data )
		{
			PORTC=PORTC&0Xf0|0x05;
			PORTB=(0xfc & PORTB)|(0x03 & data);
			PORTD=(0x03 & PORTD)|(0xfc & data);
			PORTC|=0x02;
			PORTC|=0x08;
		}

		static inline void w_data_internal( uint8_t data )
		{
			w_data( data );
		}

		static inline uint8_t r_data()
		{
			PORTC=PORTC & 0b11110000 | 0b00000110;

			asm volatile(       
				"rjmp .+0" "\n\t" 
				"rjmp .+0" "\n\t" 
				"rjmp .+0" "\n\t" 
				"nop"      "\n"   
				::);

			uint8_t AData = ( PIND & 0b11111100 ) | ( PINB & 0b00000011 );

			PORTC|=0b00000001;
			PORTC|=0x08;

			return AData;
		}

		static inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			setAddrWindow(x,y,x+1,y+1);

			cmd( 0x2E ); // Read command

			DDRD &= 0b00000011;
			DDRB &= 0b11111100;
//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				pinMode( FPins[ i ], INPUT );

			r_data(); // Fake read

			TUniColor AResult = r_data();
			AResult <<= 8;
			AResult |= r_data();
			AResult <<= 8;
			AResult |= r_data();

			DDRD |= 0b11111100;
			DDRB |= 0b00000011;
//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				pinMode( FPins[ i ], OUTPUT );

//			if( AResult )
//				Serial.println( int( AResult ), HEX );

//			return 0xFF000000 | Func::ColorFrom565( AResult ); // Not supported
			if( AResult & 0x00FC0000 )
				AResult |= 0x00030000;

			if( AResult & 0x0000FC00 )
				AResult |= 0x00000300;

			if( AResult & 0x000000FC )
				AResult |= 0x00000003;

			return 0xFF000000 | AResult;
//			return 0xFF030303 | AResult; // Not supported
		}

//#ifdef __ILI9341_SCROLL__
		template <typename T_INSTANCE> static void InternalScroll( T_INSTANCE &AInstance, int16_t AInWidth, int16_t AInHeight, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			if( X == 0 && Y == 0 )
				return;

			int AFromYIndex, AToYIndex;
			int AHeight = AInHeight - abs( Y );
			if( Y < 0 )
			{
				AFromYIndex = -Y;
				AToYIndex = 0;
			}

			else
			{
				AFromYIndex = 0;
				AToYIndex = Y;
			}

			int AFromXIndex, AToXIndex;
			int AWidth = AInWidth - abs( X );
			if( X < 0 )
			{
				AFromXIndex = -X;
				AToXIndex = 0;
			}

			else
			{
				AFromXIndex = 0;
				AToXIndex = X;
			}

			if( Y < 0 )
			{
				for( int ay = 0; ay < AHeight; ++ay )
				{
					if( X < 0 )
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];
						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

						DDRD &= 0b00000011;
						DDRB &= 0b11111100;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						DDRD |= 0b11111100;
						DDRB |= 0b00000011;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

//						for( int ax = 0; ax < AWidth; ++ax )
//							drawPixel( ax + AToXIndex, ay + AToYIndex, GetPixelColor( ax + AFromXIndex, ay + AFromYIndex ));

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
//						for( int ax = AWidth; ax < AInWidth; ++ax )
//							drawPixel( ax, ay + AToYIndex, color );
					}

					else
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

						DDRD &= 0b00000011;
						DDRB &= 0b11111100;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						DDRD |= 0b11111100;
						DDRB |= 0b00000011;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

//						for( int ax = AWidth; ax--; )
//							drawPixel( ax + AToXIndex, ay + AToYIndex, GetPixelColor( ax + AFromXIndex, ay + AFromYIndex ));

						T_INSTANCE::drawFastHLine( AInstance, 0, ay + AToYIndex, X, color );
//						for( int ax = 0; ax < X; ++ax )
//							drawPixel( ax, ay + AToYIndex, color );
					}
				}

				T_INSTANCE::fillRect( AInstance, 0, AHeight, AInWidth, AInHeight, color );
//				for( int ay = AHeight; ay < AInHeight; ++ay )
//					for( int ax = 0; ax < AInWidth; ++ax )
//						drawPixel( ax, ay, color );
			}

			else
			{
				for( int ay = AHeight; ay--; )
				{
					if( X < 0 )
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

						DDRD &= 0b00000011;
						DDRB &= 0b11111100;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						DDRD |= 0b11111100;
						DDRB |= 0b00000011;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

//						for( int ax = 0; ax < AWidth; ++ax )
//							drawPixel( ax + AToXIndex, ay + AToYIndex, GetPixelColor( ax + AFromXIndex, ay + AFromYIndex ));

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
//						for( int ax = AWidth; ax < AInWidth; ++ax )
//							drawPixel( ax, ay + AToYIndex, color );
					}

					else
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						cmd( 0x2E ); // Read command

						DDRD &= 0b00000011;
						DDRB &= 0b11111100;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], INPUT );

						r_data(); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = r_data();
							AResult <<= 8;
							AResult |= r_data();
							AResult <<= 8;
							AResult |= r_data();

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						DDRD |= 0b11111100;
						DDRB |= 0b00000011;

//						for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//							pinMode( FPins[ i ], OUTPUT );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

						cmd( 0x2C );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							w_data( AColor >> 8 ); 
							w_data( AColor );
						}

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, 0, ay + AToYIndex, X, color );
					}
				}

				T_INSTANCE::fillRect( AInstance, 0, 0, AInWidth, Y, color );
			}
		}
//#endif

		static void fillRectImplementation( int16_t AWidth, int16_t AHeight, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			if((x + w - 1) >= AWidth)
				w = AWidth - x;

			if((y + h - 1) >= AHeight)
				h = AHeight - y;

			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t hi = color >> 8, lo = color;

			cmd( 0x2C );
			for( y=0; y < h; ++y )
				for( x=0; x < w; ++x )
				{
					w_data(hi);
					w_data(lo);
				}

		}

		static inline void drawPixel(int16_t x, int16_t y, TUniColor color, uint8_t AAlpha )
		{
			if( AAlpha < 255 )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				uint8_t	r = color >> 16;
				uint8_t	g = color >> 8;
				uint8_t	b = color;

				TUniColor AOldColor = GetPixelColor( x, y );
				uint8_t	OldR = AOldColor >> 16;
				uint8_t	OldG = AOldColor >> 8;
				uint8_t	OldB = AOldColor;

//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

				r = float( r ) * ACoefficient + float( OldR ) * ( 1 - ACoefficient ) + 0.5;
				g = float( g ) * ACoefficient + float( OldG ) * ( 1 - ACoefficient ) + 0.5;
				b = float( b ) * ACoefficient + float( OldB ) * ( 1 - ACoefficient ) + 0.5;

				color = Func::Color565( r, g, b );
//				color = Func::Color565( 255, 255, 255 );
			}

			else
				color = Func::Color565( color );

			setAddrWindow(x,y,x+1,y+1);

			cmd( 0x2C );

			w_data( color >> 8 ); 
			w_data( color );
		}

		static inline void UpdateOrientation( TArduinoGraphicsOrientation	AOrientation )
		{
			switch( AOrientation )
			{
				case goUp: 		w_data(ILI9341Extended::MADCTL_BGR); break;
				case goDown:	w_data(ILI9341Extended::MADCTL_MX | ILI9341Extended::MADCTL_MY | ILI9341Extended::MADCTL_BGR); break;
				case goLeft: 	w_data(ILI9341Extended::MADCTL_MY | ILI9341Extended::MADCTL_MV | ILI9341Extended::MADCTL_BGR); break;
				case goRight:	w_data(ILI9341Extended::MADCTL_MX | ILI9341Extended::MADCTL_MV | ILI9341Extended::MADCTL_BGR); break;
			}
		}

		inline static int16_t width( TArduinoGraphicsOrientation AOrientation, int16_t AWidth, int16_t AHeight )
		{
			if( AOrientation == goUp || AOrientation == goDown )
				return AWidth;

			return AHeight;
		}

		inline static int16_t height( TArduinoGraphicsOrientation AOrientation, int16_t AWidth, int16_t AHeight )
		{
			if( AOrientation == goUp || AOrientation == goDown )
				return AHeight;

			return AWidth;
		}

	};
#endif // __AVR__

}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif