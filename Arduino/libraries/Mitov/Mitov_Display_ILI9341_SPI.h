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
#include <Mitov_Arduino_SPI.h>
#include <Mitov_Display_ILI9341_Basic.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_DataCommandOutputPin,
		typename T_ResetOutputPin,
		typename T_RESET_IMPLEMENTATION,
		typename T_SPISpeed
	> class DisplayILI9341_SPI :
		public T_ChipSelectOutputPin,
		public T_DataCommandOutputPin,
		public T_ResetOutputPin,
		public T_RESET_IMPLEMENTATION,
		public T_SPISpeed
	{
	public:
		inline void SystemLoopBegin() {}

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( DataCommandOutputPin )
		_V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( SPISpeed )

	public:
		inline void SetResetValueHigh() // Used by the T_RESET_IMPLEMENTATION
		{
			T_ResetOutputPin::SetPinValueHigh();
		}

		inline void SetResetValueLow() // Used by the T_RESET_IMPLEMENTATION
		{
			T_ResetOutputPin::SetPinValueLow();
		}

		inline void SetDataPinValueHigh()
		{
			T_DataCommandOutputPin::SetPinValueHigh();
		}

		inline void SetDataPinValueLow()
		{
			T_DataCommandOutputPin::SetPinValueLow();
		}

		inline void SetChipSeletPinValueHigh()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void SetChipSeletPinValueLow()
		{
			T_ChipSelectOutputPin::SetPinValueLow();
		}

	protected:
		inline void setAddrWindowInternal(int x1, int y1, int x2, int y2) 
		{
/*
			Serial.println( "setAddrWindow" );
			Serial.println( x1 );
			Serial.println( y1 );
			Serial.println( x2 );
			Serial.println( y2 );
*/
			uint32_t t;

			t = x1;
			t <<= 16;
			t |= x2;
			writeRegister32Internal( ILI9341Extended::COLADDRSET, t );  // HX8357D uses same registers!
			t = y1;
			t <<= 16;
			t |= y2;
			writeRegister32Internal( ILI9341Extended::PAGEADDRSET, t ); // HX8357D uses same registers!
		}

		inline void setAddrWindow(int x1, int y1, int x2, int y2) 
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
			setAddrWindowInternal( x1, y1, x2, y2);
			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void InitPins()
		{
//			Serial.println( "RESET" );
			T_RESET_IMPLEMENTATION::ExecuteReset( this, 50, 100, 50 );

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	protected:
//#if ( defined(__ILI9341_SCROLLInternalScroll
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
/*
				cmd( 0x33 );
				w_data( 0 );
				w_data( 0 );

				w_data( ( HEIGHT + Y ) >> 8 );
				w_data( HEIGHT + Y );

				w_data( ( -Y ) >> 8 );
				w_data( -Y );

//				setAddrWindow( AFromXIndex, 0, AFromXIndex + AWidth - 1, HEIGHT + Y );
//				setAddrWindow( 0, 0, WIDTH - 1, HEIGHT - 1 );

				cmd( 0x37 );
//				w_data( ( -Y ) >> 8 );
				//w_data( -Y );
				w_data( 0 );
				w_data( 8 );
//				delay( 100 );

				return;
//				Serial.println( "SCROLL" );
*/
				for( int ay = 0; ay < AHeight; ++ay )
				{
					if( X < 0 )
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];
						AInstance.setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						C_SPI.transfer(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						C_SPI.endTransaction();
//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();
//						DDRD |= 0b11111100;
//						DDRB |= 0b00000011;


						AInstance.setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );
						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));

						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2C );

//						T_DataCommandOutputPin::SetPinValueHigh();
						AInstance.SetDataPinValueHigh();

						for( register int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							C_SPI.transfer( AColor >> 8 );
							C_SPI.transfer( AColor );
						}

						C_SPI.endTransaction();

//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
					}

					else
					{
//				Serial.println( "SCROLL" );
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						AInstance.setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						C_SPI.transfer(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						C_SPI.endTransaction();
//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();

						AInstance.setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );
						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));

						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2C );

//						T_DataCommandOutputPin::SetPinValueHigh();
						AInstance.SetDataPinValueHigh();

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							C_SPI.transfer( AColor >> 8 );
							C_SPI.transfer( AColor );
						}

						C_SPI.endTransaction();

//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, 0, ay + AToYIndex, X, color );
					}
				}

				T_INSTANCE::fillRect( AInstance, 0, AHeight, AInWidth, AInHeight - AHeight, color );
			}

			else
			{
				for( int ay = AHeight; ay--; )
				{
					if( X < 0 )
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						AInstance.setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						C_SPI.transfer(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						C_SPI.endTransaction();
//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();

						AInstance.setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );

						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));

						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2C );

//						T_DataCommandOutputPin::SetPinValueHigh();
						AInstance.SetDataPinValueHigh();

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							C_SPI.transfer( AColor >> 8 );
							C_SPI.transfer( AColor );
						}

						C_SPI.endTransaction();

//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
					}

					else
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						AInstance.setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						C_SPI.transfer(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= C_SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						C_SPI.endTransaction();
//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();

						AInstance.setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );
						C_SPI.beginTransaction(SPISettings( AInstance.SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));

						AInstance.SetDataPinValueLow();
						AInstance.SetChipSeletPinValueLow();
//						T_ChipSelectOutputPin::SetPinValueLow();
//						T_DataCommandOutputPin::SetPinValueLow();

						C_SPI.transfer( 0x2C );

//						T_DataCommandOutputPin::SetPinValueHigh();
						AInstance.SetDataPinValueHigh();

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							C_SPI.transfer( AColor >> 8 );
							C_SPI.transfer( AColor );
						}

						C_SPI.endTransaction();

//						T_ChipSelectOutputPin::SetPinValueHigh();
						AInstance.SetChipSeletPinValueHigh();

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

			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
			T_DataCommandOutputPin::SetPinValueLow();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( 0x2E ); // Read command
//			cmd( 0x2E ); // Read command

			C_SPI.transfer(0x00); // Fake read

			TUniColor AResult = C_SPI.transfer(0x00);
			AResult <<= 8;
			AResult |= C_SPI.transfer(0x00);
			AResult <<= 8;
			AResult |= C_SPI.transfer(0x00);

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();

//			if( AResult )
//				Serial.println( int( AResult ), HEX );

//			return 0xFF000000 | Func::ColorFrom565( AResult ); // Not supported
			return 0xFF030303 | AResult; // Not supported
		}

		void fillRectImplementation( int16_t AWidth, int16_t AHeight, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
/*
			Serial.println( "fillRectImplementation" );
			Serial.println( AWidth );
			Serial.println( AHeight );
			Serial.println( x );
			Serial.println( y );
			Serial.println( w );
			Serial.println( h );
*/
			if((x + w - 1) >= AWidth)
				w = AWidth - x;

			if((y + h - 1) >= AHeight)
				h = AHeight - y;

			T_ChipSelectOutputPin::SetPinValueLow();

			SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ));
			setAddrWindowInternal(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t hi = color >> 8, lo = color;

//			cmd( 0x2C );

			T_DataCommandOutputPin::SetPinValueLow();

			C_SPI.transfer( 0x2C );

			T_DataCommandOutputPin::SetPinValueHigh();

			for( register uint16_t i=0; i < h; ++i )
				for( register uint16_t j=0; j < w; ++j )
				{
					C_SPI.transfer( hi );
					C_SPI.transfer( lo );
				}

			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
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

//			cmd( 0x2C );

//			w_data( color >> 8 );
//			w_data( color );


			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));

//			cmd( 0x2C );
			T_ChipSelectOutputPin::SetPinValueLow();
			T_DataCommandOutputPin::SetPinValueLow();

			C_SPI.transfer( 0x2C );

			T_DataCommandOutputPin::SetPinValueHigh();

			C_SPI.transfer( color >> 8 );
			C_SPI.transfer( color );

			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();

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
	protected:
/*
		uint8_t r_data()
		{
//			digitalWrite( FPins[ WRITE_PIN ], HIGH );
			T_DataCommandOutputPin::SetPinValueHigh();
//			T_ChipSelectOutputPin::SetPinValueLow();


//			uint8_t AResult = 0;
//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				if( digitalRead( FPins[ i ] ) == HIGH )
//					AResult |= ( 1 << i );

			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
			uint8_t AResult = C_SPI.transfer(0x00);
			C_SPI.endTransaction();

			T_DataCommandOutputPin::SetPinValueLow();
//			T_ChipSelectOutputPin::SetPinValueHigh();

//			if( AResult )
//				Serial.println( AResult );

			return AResult;
		}
*/
		inline void writeRegister32Internal(uint8_t r, uint32_t d)
		{
			T_DataCommandOutputPin::SetPinValueLow();

			C_SPI.transfer( r );

			T_DataCommandOutputPin::SetPinValueHigh();

			C_SPI.transfer( d >> 24 );
			C_SPI.transfer( d >> 16 );
			C_SPI.transfer( d >> 8 );
			C_SPI.transfer( d );
		}

		inline void writeRegister32(uint8_t r, uint32_t d)
		{
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));

			writeRegister32Internal( r, d );

			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void w_data( uint8_t data )
		{
//			Serial.print( "w_data: " ); Serial.println( data, HEX );

			T_DataCommandOutputPin::SetPinValueHigh();
			T_ChipSelectOutputPin::SetPinValueLow();

/*
    uint8_t backupSPCR = SPCR;
    SPCR = mySPCR;
    SPDR = c;
    while(!(SPSR & _BV(SPIF)));
    SPCR = backupSPCR;
*/
/*
    C_SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (full! speed!)
    C_SPI.setBitOrder(MSBFIRST);
    C_SPI.setDataMode(SPI_MODE0);
*/
			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
			C_SPI.transfer( data );
			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void w_data_internal( uint8_t data )
		{
			C_SPI.transfer( data );
		}

		inline void w_data_start()
		{
			T_DataCommandOutputPin::SetPinValueHigh();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
		}

		inline void w_data_end()
		{
			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
		}


		inline void cmd( uint8_t cmd )
		{
//			Serial.print( "cmd: " ); Serial.println( cmd, HEX );
			T_DataCommandOutputPin::SetPinValueLow();
			T_ChipSelectOutputPin::SetPinValueLow();

/*
    uint8_t backupSPCR = SPCR;
    SPCR = mySPCR;
    SPDR = c;
    while(!(SPSR & _BV(SPIF)));
    SPCR = backupSPCR;
*/
/*
	C_SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (full! speed!)
    C_SPI.setBitOrder(MSBFIRST);
    C_SPI.setDataMode(SPI_MODE0);
*/
			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0));
			C_SPI.transfer( cmd );
			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif