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
#include <Mitov_Graphics_Color.h>
#include <Mitov_Arduino_SPI.h>
#include <Mitov_Display_ILI9341.h>
#include <Mitov_PLDuino.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	namespace DisplayILI9341Const
	{
		const uint16_t WIDTH = 240;
		const uint16_t HEIGHT = 320;
	};

/*
	template <
		typename T_Precision
	> class ArduinoPLDuinoTouch :
		public T_Precision
	{
	protected:
		static const uint32_t CAL_X			= 0x00378F66UL;
		static const uint32_t CAL_Y			= 0x03C34155UL;
		static const uint32_t CAL_S			= 0x000EF13FUL;
		static const int32_t touch_x_left	= (CAL_X >> 14) & 0x3FFF;
		static const int32_t touch_x_right	= CAL_X & 0x3FFF;
		static const int32_t touch_y_top	= (CAL_Y >> 14) & 0x3FFF;
		static const int32_t touch_y_bottom	= CAL_Y & 0x3FFF;
		static const int32_t disp_x_size	= (CAL_S >> 12) & 0x0FFF;
		static const int32_t disp_y_size	= CAL_S & 0x0FFF;
		static const int32_t touch_width	= touch_x_right - touch_x_left;
		static const int32_t touch_height	= touch_y_bottom - touch_y_top;

	public:
		_V_PROP_( Precision )

	public:
		inline void SystemInit()
		{
//			Serial.println( "INIT" );
			pinMode( Mitov::PLDiono::TOUCH_IRQ, INPUT); digitalWrite( Mitov::PLDiono::TOUCH_IRQ, HIGH);
			pinMode( Mitov::PLDiono::TOUCH_CS, OUTPUT); digitalWrite( Mitov::PLDiono::TOUCH_CS, HIGH);
		}

	protected:
		word ctl( uint8_t command )
		{
			SPI.transfer(command);
			uint8_t f1 = SPI.transfer(0x00);
			uint8_t f2 = SPI.transfer(0x00);
			return word(f1, f2) >> 3;
		}
	};
*/
//---------------------------------------------------------------------------
	class ArduinoPLDuinoTouch_NoImplementation
	{
	public:
		inline void Init() {}
		inline void ReadData() {}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Precision,
		typename T_TouchedOutputPin,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class ArduinoPLDuinoTouch :
		public T_Precision,
		public T_TouchedOutputPin,
		public T_XOutputPin,
		public T_YOutputPin
	{
	protected:
		static const uint32_t CAL_X			= 0x00378F66UL;
		static const uint32_t CAL_Y			= 0x03C34155UL;
		static const uint32_t CAL_S			= 0x000EF13FUL;
		static const int32_t touch_x_left	= (CAL_X >> 14) & 0x3FFF;
		static const int32_t touch_x_right	= CAL_X & 0x3FFF;
		static const int32_t touch_y_top	= (CAL_Y >> 14) & 0x3FFF;
		static const int32_t touch_y_bottom	= CAL_Y & 0x3FFF;
		static const int32_t disp_x_size	= (CAL_S >> 12) & 0x0FFF;
		static const int32_t disp_y_size	= CAL_S & 0x0FFF;
		static const int32_t touch_width	= touch_x_right - touch_x_left;
		static const int32_t touch_height	= touch_y_bottom - touch_y_top;

	public:
		_V_PIN_( XOutputPin )
		_V_PIN_( YOutputPin )
		_V_PIN_( TouchedOutputPin )

	public:
		_V_PROP_( Precision )

	protected:
		word ctl( uint8_t command )
		{
			SPI.transfer(command);
			uint8_t f1 = SPI.transfer(0x00);
			uint8_t f2 = SPI.transfer(0x00);
			return word(f1, f2) >> 3;
		}

	public:
		inline void SystemInit()
		{
//			Serial.println( "INIT" );
			pinMode( Mitov::PLDiono::TOUCH_IRQ, INPUT); digitalWrite( Mitov::PLDiono::TOUCH_IRQ, HIGH);
			pinMode( Mitov::PLDiono::TOUCH_CS, OUTPUT); digitalWrite( Mitov::PLDiono::TOUCH_CS, HIGH);
		}

		inline void SystemLoopBegin()
		{
			if( digitalRead( Mitov::PLDiono::TOUCH_IRQ ))
				return;

			unsigned long tx=0;
			unsigned long ty=0;
			int datacount=0;

			SPI.begin();
			SPI.setClockDivider(SPI_CLOCK_DIV32);
			digitalWrite( Mitov::PLDiono::TOUCH_CS, LOW);
			for(int i=0; datacount < Precision().GetValue() && i < Precision().GetValue() * 2; i++)
			{
				unsigned long x = ctl( 0x90 );
				unsigned long y = ctl( 0xD0 );
				if (!(x>max( touch_x_left, touch_x_right ) || x==0 || y>max( touch_y_top, touch_y_bottom ) || y==0))
				{
					ty += x;
					tx += y;
					datacount++;
				}
			}

			digitalWrite( Mitov::PLDiono::TOUCH_CS, HIGH);
			SPI.end();
			if( ! datacount )
				return;

			long tpx = long(tx / datacount - touch_x_left ) * disp_x_size / touch_width;
			long tpy = long(ty / datacount - touch_y_top ) * disp_y_size / touch_height;

//			Serial.print( tpx ); Serial.print( " x " ); Serial.println( tpy );

			uint32_t AX;
			uint32_t AY;
			switch( C_OWNER.Orientation() )
			{
				case goUp:
					AX = tpy;
					AY = DisplayILI9341Const::WIDTH - 1 - tpx;
					break;

				case goDown:
					AX = DisplayILI9341Const::HEIGHT - 1 - tpy;
					AY = tpx;
					break;

				case goLeft:
					AX = tpx;
					AY = tpy;
					break;

				case goRight:
					AX = DisplayILI9341Const::WIDTH - 1 - tpx;
					AY = DisplayILI9341Const::HEIGHT - 1 - tpy;
					break;

			}

			T_XOutputPin::SetPinValue( AX );
			T_YOutputPin::SetPinValue( AY );
			T_TouchedOutputPin::ClockPin();
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_Precision,
		typename T_TouchedOutputPin,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class ArduinoPLDuinoTouch_Up :
		public ArduinoPLDuinoTouch<T_Precision>,
		public T_TouchedOutputPin,
		public T_XOutputPin,
		public T_YOutputPin
	{
		typedef ArduinoPLDuinoTouch<T_Precision> inherited;

	public:
		_V_PIN_( XOutputPin )
		_V_PIN_( YOutputPin )
		_V_PIN_( TouchedOutputPin )

	public:
		inline void SystemLoopBegin()
		{
			if( digitalRead( Mitov::PLDiono::TOUCH_IRQ ))
				return;

			unsigned long tx=0;
			unsigned long ty=0;
			int datacount=0;

			SPI.begin();
			SPI.setClockDivider(SPI_CLOCK_DIV32);
			digitalWrite( Mitov::PLDiono::TOUCH_CS, LOW);
			for(int i=0; datacount < inherited::Precision().GetValue() && i < inherited::Precision().GetValue() * 2; i++)
			{
				unsigned long x = inherited::ctl( 0x90 );
				unsigned long y = inherited::ctl( 0xD0 );
				if (!(x>max( inherited::touch_x_left, inherited::touch_x_right ) || x==0 || y>max( inherited::touch_y_top, inherited::touch_y_bottom ) || y==0))
				{
					ty += x;
					tx += y;
					datacount++;
				}
			}

			digitalWrite( Mitov::PLDiono::TOUCH_CS, HIGH);
			SPI.end();
			if( ! datacount )
				return;

			long tpx = long(tx / datacount - inherited::touch_x_left ) * inherited::disp_x_size / inherited::touch_width;
			long tpy = long(ty / datacount - inherited::touch_y_top ) * inherited::disp_y_size / inherited::touch_height;

//			Serial.print( tpx ); Serial.print( " x " ); Serial.println( tpy );
			uint32_t AX = tpy;
			uint32_t AY = DisplayILI9341Const::WIDTH - 1 - tpx;

			T_XOutputPin::SetPinValue( AX );
			T_YOutputPin::SetPinValue( AY );
			T_TouchedOutputPin::ClockPin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Precision,
		typename T_TouchedOutputPin,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class ArduinoPLDuinoTouch_Down :
		public ArduinoPLDuinoTouch<T_Precision>,
		public T_TouchedOutputPin,
		public T_XOutputPin,
		public T_YOutputPin
	{
		typedef ArduinoPLDuinoTouch<T_Precision> inherited;

	public:
		_V_PIN_( XOutputPin )
		_V_PIN_( YOutputPin )
		_V_PIN_( TouchedOutputPin )

	public:
		inline void SystemLoopBegin()
		{
			if( digitalRead( Mitov::PLDiono::TOUCH_IRQ ))
				return;

			unsigned long tx=0;
			unsigned long ty=0;
			int datacount=0;

			SPI.begin();
			SPI.setClockDivider(SPI_CLOCK_DIV32);
			digitalWrite( Mitov::PLDiono::TOUCH_CS, LOW);
			for(int i=0; datacount < inherited::Precision().GetValue() && i < inherited::Precision().GetValue() * 2; i++)
			{
				unsigned long x = inherited::ctl(0x90);
				unsigned long y = inherited::ctl(0xD0);
				if (!(x>max( inherited::touch_x_left, inherited::touch_x_right ) || x==0 || y>max( inherited::touch_y_top, inherited::touch_y_bottom ) || y==0))
				{
					ty += x;
					tx += y;
					datacount++;
				}
			}

			digitalWrite( Mitov::PLDiono::TOUCH_CS, HIGH);
			SPI.end();
			if( !datacount )
				return;

			long tpx = long(tx / datacount - inherited::touch_x_left) * inherited::disp_x_size / inherited::touch_width;
			long tpy = long(ty / datacount - inherited::touch_y_top) * inherited::disp_y_size / inherited::touch_height;

//			Serial.print( tpx ); Serial.print( " x " ); Serial.println( tpy );
			uint32_t AX = DisplayILI9341Const::HEIGHT - 1 - tpy;
			uint32_t AY = tpx;

			T_XOutputPin::SetPinValue( AX );
			T_YOutputPin::SetPinValue( AY );
			T_TouchedOutputPin::ClockPin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Precision,
		typename T_TouchedOutputPin,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class ArduinoPLDuinoTouch_Left :
		public ArduinoPLDuinoTouch<T_Precision>,
		public T_TouchedOutputPin,
		public T_XOutputPin,
		public T_YOutputPin
	{
		typedef ArduinoPLDuinoTouch<T_Precision> inherited;

	public:
		inline void SystemLoopBegin()
		{
			if( digitalRead( Mitov::PLDiono::TOUCH_IRQ ))
				return;

			unsigned long tx=0;
			unsigned long ty=0;
			int datacount=0;

			SPI.begin();
			SPI.setClockDivider(SPI_CLOCK_DIV32);
			digitalWrite( Mitov::PLDiono::TOUCH_CS, LOW);
			for(int i=0; datacount < inherited::Precision().GetValue() && i < inherited::Precision().GetValue() * 2; i++)
			{
				unsigned long x = inherited::ctl(0x90);
				unsigned long y = inherited::ctl(0xD0);
				if (!(x>max( inherited::touch_x_left, inherited::touch_x_right ) || x==0 || y>max( inherited::touch_y_top, inherited::touch_y_bottom ) || y==0))
				{
					ty += x;
					tx += y;
					datacount++;
				}
			}

			digitalWrite( Mitov::PLDiono::TOUCH_CS, HIGH);
			SPI.end();
			if( !datacount )
				return;

			long tpx = long(tx / datacount - inherited::touch_x_left ) * inherited::disp_x_size / inherited::touch_width;
			long tpy = long(ty / datacount - inherited::touch_y_top ) * inherited::disp_y_size / inherited::touch_height;

//			Serial.print( tpx ); Serial.print( " x " ); Serial.println( tpy );
			uint32_t AX = tpx;
			uint32_t AY = tpy;

			T_XOutputPin::SetPinValue( AX );
			T_YOutputPin::SetPinValue( AY );
			T_TouchedOutputPin::ClockPin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Precision,
		typename T_TouchedOutputPin,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class ArduinoPLDuinoTouch_Right :
		public ArduinoPLDuinoTouch<T_Precision>,
		public T_TouchedOutputPin,
		public T_XOutputPin,
		public T_YOutputPin
	{
		typedef ArduinoPLDuinoTouch<T_Precision> inherited;

	public:
		inline void SystemLoopBegin()
		{
			if( digitalRead( Mitov::PLDiono::TOUCH_IRQ ))
				return;

			unsigned long tx=0;
			unsigned long ty=0;
			int datacount=0;

			SPI.begin();
			SPI.setClockDivider(SPI_CLOCK_DIV32);
			digitalWrite( Mitov::PLDiono::TOUCH_CS, LOW);
			for(int i=0; datacount < inherited::Precision().GetValue() && i < inherited::Precision().GetValue() * 2; i++)
			{
				unsigned long x = inherited::ctl(0x90);
				unsigned long y = inherited::ctl(0xD0);
				if (!(x>max( inherited::touch_x_left, inherited::touch_x_right ) || x==0 || y>max( inherited::touch_y_top, inherited::touch_y_bottom ) || y==0))
				{
					ty += x;
					tx += y;
					datacount++;
				}
			}

			digitalWrite( Mitov::PLDiono::TOUCH_CS, HIGH);
			SPI.end();
			if( !datacount )
				return;

			long tpx = long(tx / datacount - inherited::touch_x_left) * inherited::disp_x_size / inherited::touch_width;
			long tpy = long(ty / datacount - inherited::touch_y_top) * inherited::disp_y_size / inherited::touch_height;

//			Serial.print( tpx ); Serial.print( " x " ); Serial.println( tpy );
			uint32_t AX = DisplayILI9341Const::WIDTH - 1 - tpx;
			uint32_t AY = DisplayILI9341Const::HEIGHT - 1 - tpy;

			T_XOutputPin::SetPinValue( AX );
			T_YOutputPin::SetPinValue( AY );
			T_TouchedOutputPin::ClockPin();
		}

	};
*/
//---------------------------------------------------------------------------
	template <
//		typename T_Touch,
		typename T_Backlight
	> class ArduinoPLDuinoDisplayImplementation :
//		public T_Touch,
		public T_Backlight
	{
	public:
		_V_PROP_( Backlight )

//		_V_PROP_( Touch )

	public:
		void UpdateBacklight()
		{
			Analog.Write( Mitov::PLDiono::LCD_BACKLIGHT, 1.0 - Backlight().GetValue() );
		}

	protected:
		static inline void FastSPI( uint8_t AValue )
		{
		    SPDR = AValue;
		    while(!(SPSR & _BV(SPIF)));
		}

	protected:
		inline void InitPins()
		{
//			Serial.println( "RESET" );
//			pinMode( FPins[ RESET_PIN ], OUTPUT );
			pinMode( PLDiono::LCD_CS, OUTPUT );
			pinMode( PLDiono::LCD_DC, OUTPUT );

			pinMode(Mitov::PLDiono::LCD_PWR, OUTPUT); digitalWrite(Mitov::PLDiono::LCD_PWR, LOW);
			pinMode(Mitov::PLDiono::LCD_BACKLIGHT, OUTPUT); digitalWrite(Mitov::PLDiono::LCD_BACKLIGHT, HIGH);

			// Needed to allow reading from the Display!
			pinMode(Mitov::PLDiono::TOUCH_CS, OUTPUT); digitalWrite(Mitov::PLDiono::TOUCH_CS, HIGH);

			delay(1500); // give LCD enough time to reset completely
			pinMode(Mitov::PLDiono::LCD_PWR, OUTPUT); digitalWrite(Mitov::PLDiono::LCD_PWR, HIGH);

			delay(250);
			digitalWrite(Mitov::PLDiono::LCD_RST, LOW);
			delay(50);
			digitalWrite(Mitov::PLDiono::LCD_RST, HIGH);
			delay(50);

//			Analog.WriteRaw( Mitov::PLDiono::LCD_BACKLIGHT, 250 );

//			digitalWrite(Mitov::PLDiono::LCD_BACKLIGHT, LOW);

			UpdateBacklight();

/*
			digitalWrite(FPins[ RESET_PIN ],HIGH);
			delay(50);
			digitalWrite(FPins[ RESET_PIN ],LOW);
			delay(100);

			digitalWrite(FPins[ RESET_PIN ],HIGH);
*/
			delay(50);
			digitalWrite(PLDiono::LCD_CS,HIGH);

//			Touch().Init();
		}

	public:
//#if ( defined(__ILI9341_SCROLL__))
		template <typename T_INSTANCE> static void InternalScroll( T_INSTANCE &AInstance, int16_t AInWidth, int16_t AInHeight, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
//			Serial.print( X ); Serial.print( " " );	Serial.println( Y );

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
						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
						digitalWrite( PLDiono::LCD_DC, LOW );
						digitalWrite( PLDiono::LCD_CS, LOW );

						FastSPI( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						FastSPI(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						SPI.endTransaction();
						digitalWrite( PLDiono::LCD_CS, HIGH );
//						DDRD |= 0b11111100;
//						DDRB |= 0b00000011;


						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );
						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

						digitalWrite( PLDiono::LCD_CS, LOW );
						digitalWrite( PLDiono::LCD_DC, LOW );

						FastSPI( 0x2C );

						digitalWrite( PLDiono::LCD_DC, HIGH );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							FastSPI( AColor >> 8 );
							FastSPI( AColor );
						}

						SPI.endTransaction();

						digitalWrite( PLDiono::LCD_CS, HIGH );

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
					}

					else
					{
//				Serial.println( "SCROLL" );
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
						digitalWrite( PLDiono::LCD_DC, LOW );
						digitalWrite( PLDiono::LCD_CS, LOW );

						FastSPI( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						FastSPI(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						SPI.endTransaction();
						digitalWrite( PLDiono::LCD_CS, HIGH );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );
						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

						digitalWrite( PLDiono::LCD_CS, LOW );
						digitalWrite( PLDiono::LCD_DC, LOW );

						FastSPI( 0x2C );

						digitalWrite( PLDiono::LCD_DC, HIGH );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							FastSPI( AColor >> 8 );
							FastSPI( AColor );
						}

						SPI.endTransaction();

						digitalWrite( PLDiono::LCD_CS, HIGH );

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
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
						digitalWrite( PLDiono::LCD_DC, LOW );
						digitalWrite( PLDiono::LCD_CS, LOW );

						FastSPI( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						FastSPI(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						SPI.endTransaction();
						digitalWrite( PLDiono::LCD_CS, HIGH );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );

						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

						digitalWrite( PLDiono::LCD_CS, LOW );
						digitalWrite( PLDiono::LCD_DC, LOW );

						FastSPI( 0x2C );

						digitalWrite( PLDiono::LCD_DC, HIGH );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							FastSPI( AColor >> 8 );
							FastSPI( AColor );
						}

						SPI.endTransaction();

						digitalWrite( PLDiono::LCD_CS, HIGH );

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, AWidth, ay + AToYIndex, AInWidth - AWidth, color );
					}

					else
					{
						uint16_t *ABuffer = new uint16_t[ AWidth ];

						setAddrWindow( AFromXIndex, ay + AFromYIndex, AFromXIndex + AWidth - 1, ay + AFromYIndex );

						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
						digitalWrite( PLDiono::LCD_DC, LOW );
						digitalWrite( PLDiono::LCD_CS, LOW );

						FastSPI( 0x2E ); // Read command
//						cmd( 0x2E ); // Read command

						FastSPI(0x00); // Fake read

						for( int ax = 0; ax < AWidth; ++ax )
						{
							TUniColor AResult = SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);
							AResult <<= 8;
							AResult |= SPI.transfer(0x00);

							ABuffer[ ax ] = Func::Color565( AResult );
						}

						SPI.endTransaction();
						digitalWrite( PLDiono::LCD_CS, HIGH );

						setAddrWindow( AToXIndex, ay + AToYIndex, AToXIndex + AWidth - 1, ay + AToYIndex );

//						cmd( 0x2C );
						SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

						digitalWrite( PLDiono::LCD_CS, LOW );
						digitalWrite( PLDiono::LCD_DC, LOW );

						FastSPI( 0x2C );

						digitalWrite( PLDiono::LCD_DC, HIGH );

						for( int ax = 0; ax < AWidth; ++ax )
						{
							uint16_t AColor = ABuffer[ ax ];
							FastSPI( AColor >> 8 );
							FastSPI( AColor );
						}

						SPI.endTransaction();

						digitalWrite( PLDiono::LCD_CS, HIGH );

						delete [] ABuffer;

						T_INSTANCE::drawFastHLine( AInstance, 0, ay + AToYIndex, X, color );
					}
				}

				T_INSTANCE::fillRect( AInstance, 0, 0, AInWidth, Y, color );
			}
		}
//#endif
		static inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
//			return 0xFFFFFFFF;
			setAddrWindow(x,y,x+1,y+1);

			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			digitalWrite( PLDiono::LCD_DC, LOW );
			digitalWrite( PLDiono::LCD_CS, LOW );

			FastSPI( 0x2E ); // Read command
//			cmd( 0x2E ); // Read command

			FastSPI( 0x00 ); // Fake read

			TUniColor AResult = SPI.transfer(0x00);
			AResult <<= 8;
			AResult |= SPI.transfer(0x00);
			AResult <<= 8;
			AResult |= SPI.transfer(0x00);

			SPI.endTransaction();
			digitalWrite( PLDiono::LCD_CS, HIGH );

//			if( AResult )
//				Serial.println( int( AResult ), HEX );

//			return 0xFF000000 | Func::ColorFrom565( AResult ); // Not supported
			return 0xFF030303 | AResult; // Not supported
		}

		static inline void fillRectImplementation( int16_t AWidth, int16_t AHeight, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			// rudimentary clipping (drawChar w/big text requires this)
			if((x + w - 1) >= AWidth)
				w = AWidth - x;

			if((y + h - 1) >= AHeight)
				h = AHeight - y;

			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t hi = color >> 8, lo = color;

//			cmd( 0x2C );
			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

			digitalWrite( PLDiono::LCD_CS, LOW );
			digitalWrite( PLDiono::LCD_DC, LOW );

			FastSPI( 0x2C );

			digitalWrite( PLDiono::LCD_DC, HIGH );

			for( y=0; y < h; ++y )
				for( x=0; x < w; ++x )
				{
					FastSPI( hi );
					FastSPI( lo );
				}

			SPI.endTransaction();

			digitalWrite( PLDiono::LCD_CS, HIGH );
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

			digitalWrite( PLDiono::LCD_CS, LOW );
			digitalWrite( PLDiono::LCD_DC, LOW );

			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

			FastSPI( 0x2C );

			digitalWrite( PLDiono::LCD_DC, HIGH );

			FastSPI( color >> 8 );
			FastSPI( color );

			SPI.endTransaction();


			digitalWrite( PLDiono::LCD_CS, HIGH );

/*
			cmd( 0x2C );

			w_data( color >> 8 );
			w_data( color );
*/
		}

/*
	protected:
		inline static int16_t width( TArduinoGraphicsOrientation AOrientation, int16_t AWidth, int16_t AHeight )
		{
			if( AOrientation == goUp || AOrientation == goDown )
				return AHeight;

			return AWidth;
		}

		inline static int16_t height( TArduinoGraphicsOrientation AOrientation, int16_t AWidth, int16_t AHeight )
		{
			if( AOrientation == goUp || AOrientation == goDown )
				return AWidth;

			return AHeight;
		}
*/
/*
	protected:
		static inline void UpdateOrientation( TArduinoGraphicsOrientation	AOrientation )
		{
			cmd( ILI9341Extended::MADCTL);

			switch( AOrientation )
			{
				case goRight: 	w_data(ILI9341Extended::MADCTL_BGR); break;
				case goLeft:	w_data(ILI9341Extended::MADCTL_MX | ILI9341Extended::MADCTL_MY | ILI9341Extended::MADCTL_BGR); break;
				case goUp: 		w_data(ILI9341Extended::MADCTL_MY | ILI9341Extended::MADCTL_MV | ILI9341Extended::MADCTL_BGR); break;
				case goDown:	w_data(ILI9341Extended::MADCTL_MX | ILI9341Extended::MADCTL_MV | ILI9341Extended::MADCTL_BGR); break;
/ *
				case goUp: 		w_data(ILI9341Extended::MADCTL_BGR); break;
				case goDown:	w_data(ILI9341Extended::MADCTL_MX | ILI9341Extended::MADCTL_MY | ILI9341Extended::MADCTL_BGR); break;
				case goLeft: 	w_data(ILI9341Extended::MADCTL_MY | ILI9341Extended::MADCTL_MV | ILI9341Extended::MADCTL_BGR); break;
				case goRight:	w_data(ILI9341Extended::MADCTL_MX | ILI9341Extended::MADCTL_MV | ILI9341Extended::MADCTL_BGR); break;
* /
			}

//			setAddrWindow( 0, 0, width() - 1, height() - 1 ); // CS_IDLE happens here
		}
*/
	protected:
/*
		uint8_t r_data()
		{
//			digitalWrite( FPins[ WRITE_PIN ], HIGH );
			digitalWrite( PLDiono::LCD_DC, HIGH );
//			digitalWrite( PLDiono::LCD_CS, LOW );


//			uint8_t AResult = 0;
//			for( int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; ++i )
//				if( digitalRead( FPins[ i ] ) == HIGH )
//					AResult |= ( 1 << i );

			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			uint8_t AResult = SPI.transfer(0x00);
			SPI.endTransaction();

			digitalWrite( PLDiono::LCD_DC, LOW );
//			digitalWrite( PLDiono::LCD_CS, HIGH );

//			if( AResult )
//				Serial.println( AResult );

			return AResult;
		}
*/
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

		static inline void writeRegister32(uint8_t r, uint32_t d)
		{
			digitalWrite( PLDiono::LCD_CS, LOW );
			digitalWrite( PLDiono::LCD_DC, LOW );

			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			FastSPI( r );

			digitalWrite( PLDiono::LCD_DC, HIGH );

			FastSPI( d >> 24 );
			FastSPI( d >> 16 );
			FastSPI( d >> 8 );
			FastSPI( d );

			SPI.endTransaction();

			digitalWrite( PLDiono::LCD_CS, HIGH );
		}

		static inline void  w_data( uint8_t data )
		{
//			Serial.print( "w_data: " ); Serial.println( data, HEX );

			digitalWrite( PLDiono::LCD_DC, HIGH );
			digitalWrite( PLDiono::LCD_CS, LOW );

			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			FastSPI( data );
			SPI.endTransaction();

			digitalWrite( PLDiono::LCD_CS, HIGH );
		}

		static inline void w_data_internal( uint8_t data )
		{
			FastSPI( data );
		}

		static inline void w_data_start()
		{
			digitalWrite( PLDiono::LCD_DC, HIGH );
			digitalWrite( PLDiono::LCD_CS, LOW );

			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
		}

		static inline void w_data_end()
		{
			SPI.endTransaction();

			digitalWrite( PLDiono::LCD_CS, HIGH );
		}

		static inline void cmd( uint8_t cmd )
		{
//			Serial.print( "cmd: " ); Serial.println( cmd, HEX );
			digitalWrite( PLDiono::LCD_DC, LOW );
			digitalWrite( PLDiono::LCD_CS, LOW );

			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			FastSPI( cmd );
			SPI.endTransaction();

			digitalWrite( PLDiono::LCD_CS, HIGH );
		}

	public:
/*
		inline void SystemInit()
		{
//			pinMode(TOUCH_IRQ, INPUT);

			pinMode(PLDiono::LCD_PWR, OUTPUT); digitalWrite(PLDiono::LCD_PWR, LOW);
			pinMode(PLDiono::LCD_BACKLIGHT, OUTPUT); digitalWrite(PLDiono::LCD_BACKLIGHT, HIGH);

//			pinMode(TOUCH_CS, OUTPUT); digitalWrite(TOUCH_CS, HIGH);

			delay(1500); // give LCD enough time to reset completely
			pinMode(PLDiono::LCD_PWR, OUTPUT); digitalWrite(PLDiono::LCD_PWR, HIGH);

			delay(250);
			digitalWrite(PLDiono::LCD_RST, LOW);
			delay(50);
			digitalWrite(PLDiono::LCD_RST, HIGH);
			delay(50);

			digitalWrite(PLDiono::LCD_BACKLIGHT, LOW);
		}
*/

	};
//---------------------------------------------------------------------------
	template <
		typename T_BackgroundColor,
		typename T_Backlight,
		typename T_Elements_Render,
        typename T_FCursorX,
        typename T_FCursorY,
		typename T_Inverted,
		typename T_Orientation,
		typename T_Text
//		typename T_Touch
	> class ArduinoPLDuinoDisplay :
		public Mitov::DisplayILI9341 <
				ArduinoPLDuinoDisplayImplementation<
//					T_Touch,
					T_Backlight
				>,
				Mitov::DisplayILI9341_Otientation_Left,
				T_BackgroundColor,
				false,
				320, // Height
				240, // Width
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_Text
			>
	{
		typedef Mitov::DisplayILI9341 <
				ArduinoPLDuinoDisplayImplementation<
//					T_Touch,
					T_Backlight
				>,
				Mitov::DisplayILI9341_Otientation_Left,
				T_BackgroundColor,
				false,
				320, // Height
				240, // Width
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_Text
			> inherited;

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif