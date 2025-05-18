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
#include <SPI.h>

#ifdef _MITOV_DISPLAY_ST7735R_BLACKTAB_
  #define _MITOV_DISPLAY_ST7735R_REDTAB_
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __ST7735__DEBUG__

namespace Mitov
{
namespace ST7735
{
	const uint8_t	NOP		= 0x00;
	const uint8_t	SWRESET	= 0x01;
	const uint8_t	RDDID	= 0x04;
	const uint8_t	RDDST	= 0x09;

	const uint8_t	SLPIN	= 0x10;
	const uint8_t	SLPOUT	= 0x11;
	const uint8_t	PTLON	= 0x12;
	const uint8_t	NORON	= 0x13;

	const uint8_t	INVOFF	= 0x20;
	const uint8_t	INVON	= 0x21;
	const uint8_t	DISPOFF	= 0x28;
	const uint8_t	DISPON	= 0x29;
	const uint8_t	CASET	= 0x2A;
	const uint8_t	RASET	= 0x2B;
	const uint8_t	RAMWR	= 0x2C;
	const uint8_t	RAMRD	= 0x2E;

	const uint8_t	PTLAR	= 0x30;
	const uint8_t	COLMOD	= 0x3A;
	const uint8_t	MADCTL	= 0x36;

	const uint8_t	FRMCTR1	= 0xB1;
	const uint8_t	FRMCTR2	= 0xB2;
	const uint8_t	FRMCTR3	= 0xB3;
	const uint8_t	INVCTR	= 0xB4;
	const uint8_t	DISSET5	= 0xB6;

	const uint8_t	PWCTR1	= 0xC0;
	const uint8_t	PWCTR2	= 0xC1;
	const uint8_t	PWCTR3	= 0xC2;
	const uint8_t	PWCTR4	= 0xC3;
	const uint8_t	PWCTR5  = 0xC4;
	const uint8_t	VMCTR1  = 0xC5;

	const uint8_t	RDID1   = 0xDA;
	const uint8_t	RDID2   = 0xDB;
	const uint8_t	RDID3   = 0xDC;
	const uint8_t	RDID4   = 0xDD;

	const uint8_t	PWCTR6  = 0xFC;

	const uint8_t	GMCTRP1 = 0xE0;
	const uint8_t	GMCTRN1 = 0xE1;

	const uint8_t	DELAY	= 0x80;

	const uint8_t	MADCTL_MY  = 0x80;
	const uint8_t	MADCTL_MX  = 0x40;
	const uint8_t	MADCTL_MV  = 0x20;
	const uint8_t	MADCTL_ML  = 0x10;
	const uint8_t	MADCTL_RGB = 0x00;
	const uint8_t	MADCTL_BGR = 0x08;
	const uint8_t	MADCTL_MH  = 0x04;

#if defined (SPI_HAS_TRANSACTION)
#elif defined (__AVR__)
	uint8_t mySPCR;
#endif
}

static const uint8_t PROGMEM
  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735::SWRESET,   ST7735::DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735::SLPOUT ,   ST7735::DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735::FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735::FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735::FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735::INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735::PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735::PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735::PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735::PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,  
    ST7735::PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735::VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735::INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735::MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735::COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 };                 //     16-bit color

//---------------------------------------------------------------------------
#ifdef _MITOV_DISPLAY_ST7735R_GREENTAB_160_80_
#endif
//---------------------------------------------------------------------------
#ifndef _MITOV_DISPLAY_ST7735B_
static const uint8_t PROGMEM
  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735::GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735::GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735::NORON  ,    ST7735::DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735::DISPON ,    ST7735::DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay
#endif
//---------------------------------------------------------------------------
#ifdef _MITOV_DISPLAY_ST7735B_
#endif
//---------------------------------------------------------------------------
    template <uint8_t C_PIXEL_MODE, uint16_t C_WIDTH, uint16_t C_HEIGHT> class Display_ST7735_Common
	{
	public:
		static const uint16_t WIDTH = C_WIDTH;
		static const uint16_t HEIGHT = C_HEIGHT;

	public:
        constexpr inline static uint8_t GetColorMode() { return C_PIXEL_MODE; }
        
	};
//---------------------------------------------------------------------------
    template <uint8_t C_PIXEL_MODE, uint16_t C_WIDTH, uint16_t C_HEIGHT> class Display_ST7735_Common_NonInverted : public Display_ST7735_Common<C_PIXEL_MODE, C_WIDTH, C_HEIGHT>
	{
	public:
		template <typename T> inline static void UpdateInvertedImpl( T *AInstance )
		{
			AInstance->writecommand( ( AInstance->Inverted() ) ? ST7735::INVON : ST7735::INVOFF );
		}

	};
//---------------------------------------------------------------------------
	template <
        typename T_Enabled,
        typename T_X,
        typename T_Y
    > class TArduinoDisplayST7735PixelOffset :
        public T_Enabled,
        public T_X,
        public T_Y
    {
    public:
        _V_PROP_( Enabled )
        _V_PROP_( X )
        _V_PROP_( Y )
        
    };
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_IMPLEMENTATION_ORIENTATION,
		typename T_IMPLEMENTATION_TYPE,
		typename T_BackgroundColor,
		typename T_Elements_Render,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_Inverted,
        typename T_Orientation,
		typename T_OverridePixelOffset,
		typename T_ResetOutputPin,
		typename T_Text
	> class DisplayST7735Basic :
		public T_IMPLEMENTATION,
		public T_IMPLEMENTATION_TYPE,
		public GraphicsImpl<
			DisplayST7735Basic<
					T_IMPLEMENTATION,
					T_IMPLEMENTATION_ORIENTATION,
					T_IMPLEMENTATION_TYPE,
					T_BackgroundColor,
					T_Elements_Render,
					T_FCursorX,
					T_FCursorY,
					T_Inverted,
                    T_Orientation,
                    T_OverridePixelOffset,
					T_ResetOutputPin,
					T_Text
				>,
				T_BackgroundColor,
				T_Text
			>, 

//		public T_Text,
		public T_FCursorX,
		public T_FCursorY,
		public T_Inverted,
        public T_Orientation,
        public T_OverridePixelOffset,
        public T_ResetOutputPin
	{
		typedef DisplayST7735Basic T_SELF;

		typedef GraphicsImpl<
				T_SELF,
				T_BackgroundColor,
				T_Text
			> inherited;

	public:
        _V_PIN_( ResetOutputPin )
        
	public:
		_V_PROP_( Text )
		_V_PROP_( Inverted )
        _V_PROP_( Orientation )
		_V_PROP_( OverridePixelOffset )

	public:
		_V_PROP_( FCursorX )
		_V_PROP_( FCursorY )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::Call();
		}

	public:
		void SetCursor( int16_t x, int16_t y )
		{
			FCursorX() = x;
			FCursorY() = y;
		}

	public:
		inline void UpdateInverted() // Used by the Binding Setters
		{
			T_IMPLEMENTATION_TYPE::UpdateInvertedImpl( this );
//			writecommand( ( Inverted ) ? ST7735::INVON : ST7735::INVOFF );
		}

	public:
		inline void UpdateOrientation()
		{
			T_IMPLEMENTATION::writecommand( ST7735::MADCTL );

//			T_IMPLEMENTATION::writedata( C_PIXEL_MODE );
			switch( T_IMPLEMENTATION_ORIENTATION::ConvertOrientation( Orientation().GetValue() ) )
			{
				case goUp:		T_IMPLEMENTATION::writedata(ST7735::MADCTL_MX | ST7735::MADCTL_MY | T_IMPLEMENTATION_TYPE::GetColorMode() ); break;
				case goLeft:	T_IMPLEMENTATION::writedata(ST7735::MADCTL_MX | ST7735::MADCTL_MV | T_IMPLEMENTATION_TYPE::GetColorMode() ); break;
				case goDown: 	T_IMPLEMENTATION::writedata( T_IMPLEMENTATION_TYPE::GetColorMode() ); break;
				case goRight: 	T_IMPLEMENTATION::writedata(ST7735::MADCTL_MY | ST7735::MADCTL_MV | T_IMPLEMENTATION_TYPE::GetColorMode() ); break;
			}
/*
			switch( inherited::Orientation )
			{
				case goUp:		inherited::writedata(ST7735::MADCTL_MX | ST7735::MADCTL_MY | ST7735::MADCTL_RGB); break;
				case goLeft:	inherited::writedata(ST7735::MADCTL_MX | ST7735::MADCTL_MV | ST7735::MADCTL_RGB); break;
				case goDown: 	inherited::writedata(ST7735::MADCTL_RGB); break;
				case goRight: 	inherited::writedata(ST7735::MADCTL_MY | ST7735::MADCTL_MV | ST7735::MADCTL_RGB); break;
			}
*/
//			T_IMPLEMENTATION_TYPE::UpdateOrientation( this );
		}

/*
		inline void UpdateInverted() // Used by the Binding Setters
		{
			T_IMPLEMENTATION::writecommand( ( Inverted() ) ? ST7735::INVON : ST7735::INVOFF );
		}
*/
	public:
		inline int16_t width(void)
		{
			switch( T_IMPLEMENTATION_ORIENTATION::ConvertOrientation( Orientation().GetValue() ) )
			{
				case goUp:
				case goDown:
                    return T_IMPLEMENTATION_TYPE::WIDTH;
            }

			return T_IMPLEMENTATION_TYPE::HEIGHT;
		}

		inline int16_t height(void)
		{
			switch( T_IMPLEMENTATION_ORIENTATION::ConvertOrientation( Orientation().GetValue() ) )
			{
				case goUp:
				case goDown:
                    return T_IMPLEMENTATION_TYPE::HEIGHT;
            }
                
			return T_IMPLEMENTATION_TYPE::WIDTH;
		}
        
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		static inline TUniColor NonTransparent( TUniColor color )
		{
			return 0xFF000000 | ::Mitov::Func::SwapRB( color );
		}

    protected:
		void setAddrWindow( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 )
		{
/*
			Serial.print( "x0: " );
			Serial.println( x0 );
			Serial.print( "y0: " );
			Serial.println( y0 );
			Serial.print( "x1: " );
			Serial.println( x1 );
			Serial.print( "y1: " );
			Serial.println( y1 );
*/
			T_IMPLEMENTATION::writecommand(ST7735::CASET); // Column addr set
//			writedata(0x00);
//			writedata16(x0 + C_OFFSET_X);     // XSTART 
//			writedata(0x00);
//			writedata16(x1 + C_OFFSET_X);     // XEND

            uint32_t AOffset_X;
            uint32_t AOffset_Y;
            
            if( OverridePixelOffset().Enabled() )
            {
                AOffset_X = OverridePixelOffset().X().GetValue();
                AOffset_Y = OverridePixelOffset().X().GetValue();
            }

            else
                T_IMPLEMENTATION_TYPE::GetPixelOffset( T_IMPLEMENTATION_ORIENTATION::ConvertOrientation( Orientation().GetValue() ), AOffset_X, AOffset_Y );

			T_IMPLEMENTATION::writedata32( ( uint32_t( x0 + AOffset_X ) << 16  ) | x1 + AOffset_X );

			T_IMPLEMENTATION::writecommand(ST7735::RASET); // Row addr set
//			writedata(0x00);
//			writedata16(y0 + C_OFFSET_Y);     // YSTART
//			writedata(0x00);
//			writedata16(y1 + C_OFFSET_Y);     // YEND

			T_IMPLEMENTATION::writedata32( ( uint32_t( y0 + AOffset_Y ) << 16  ) | y1 + AOffset_Y );

			T_IMPLEMENTATION::writecommand( ST7735::RAMWR ); // write to RAM
		}

	public:
		static inline void drawAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.drawAlphaColorBitmapImplementation( x, y, bitmap, w, h, size );
		}

		void drawAlphaColorBitmapImplementation( int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			// check rotation, move pixel around if necessary

			setAddrWindow(x,y,x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARGBColor = pgm_read_dword( bitmap++ );
					uint8_t AAlpha = ARGBColor >> 24;
					if( AAlpha == 0xFF )
					{
						uint16_t AColor = Func::Color565( ARGBColor );
						T_IMPLEMENTATION::spiwrite16( AColor );
					}

					else if( AAlpha == 0 )
						T_IMPLEMENTATION::spiwrite16( 0 );

					else
					{
						float ACoefficient = float( AAlpha ) / 255.0f;
						uint8_t	r = ARGBColor >> 16;
						uint8_t	g = ARGBColor >> 8;
						uint8_t	b = ARGBColor;

		//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

						r = float( r ) * ACoefficient + 0.5;
						g = float( g ) * ACoefficient + 0.5;
						b = float( b ) * ACoefficient + 0.5;

						uint16_t AColor = Func::Color565( r, g, b );
						T_IMPLEMENTATION::spiwrite16( AColor );
					}
				}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawAlphaColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.drawAlphaColorBitmapImplementation_565( x, y, bitmap, w, h, size );
		}

		void drawAlphaColorBitmapImplementation_565( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			// check rotation, move pixel around if necessary

			setAddrWindow(x,y,x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
					uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );
					uint8_t AAlpha = pgm_read_byte( bitmap ++ );

					uint32_t r = ARedGreen & 0b11111000;
					uint32_t b = 0xFF & ( AGreenBlue << 3 );
					uint32_t g = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

					if( AAlpha == 0xFF )
					{
						uint16_t AColor = Func::Color565( r, g, b );
						T_IMPLEMENTATION::spiwrite16( AColor );
					}

					else if( AAlpha == 0 )
					{
						T_IMPLEMENTATION::spiwrite16( 0 );
					}

					else
					{
						float ACoefficient = float( AAlpha ) / 255.0f;

						r = float( r ) * ACoefficient + 0.5;
						g = float( g ) * ACoefficient + 0.5;
						b = float( b ) * ACoefficient + 0.5;

						uint16_t AColor = Func::Color565( r, g, b );
						T_IMPLEMENTATION::spiwrite16( AColor );
					}
				}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.drawColorBitmapImplementation( x, y, bitmap, w, h, size );
		}

		void drawColorBitmapImplementation( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			setAddrWindow(x,y,x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint32_t AGreen = pgm_read_byte( bitmap ++ );
					uint32_t ABlue = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGreen << 8 ) | ( ARed << 16 ) | ABlue );

					T_IMPLEMENTATION::spiwrite16( AColor );
				}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.drawColorBitmapImplementation_565( x, y, bitmap, w, h, size );
		}

		void drawColorBitmapImplementation_565( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			setAddrWindow(x,y,x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint16_t AColor = pgm_read_word( bitmap );
					bitmap += 2;
					T_IMPLEMENTATION::spiwrite16( AColor );
				}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawGrayscaleBitmap( AImplementation, x, y, bitmap, w, h, AAlpha, size );
				return;
			}

			AImplementation.drawGrayscaleBitmapImplementation( x, y, bitmap, w, h, size );
		}

		void drawGrayscaleBitmapImplementation( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			setAddrWindow(x,y,x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

					T_IMPLEMENTATION::spiwrite16( AColor );
				}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawAlphaGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint16_t *_bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawAlphaGrayscaleBitmap( AImplementation, x, y, _bitmap, w, h, size );
				return;
			}

			AImplementation.drawAlphaGrayscaleBitmapImplementation( x, y, _bitmap, w, h, size );
		}

		void drawAlphaGrayscaleBitmapImplementation( int16_t x, int16_t y, const uint16_t *_bitmap, int16_t w, int16_t h, uint8_t size )
		{
			const uint8_t *bitmap = (const uint8_t *)_bitmap;

			setAddrWindow(x,y,x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );
					bitmap += 2;

					T_IMPLEMENTATION::spiwrite16( AColor );
				}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawBWBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, TUniColor color, TUniColor bg, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawBWBitmap( AImplementation, x, y, bitmap, w, h, color, bg, size );
				return;
			}

			AImplementation.drawBWBitmapImplementation( x, y, bitmap, w, h, size );
		}

		void drawBWBitmapImplementation( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, TUniColor color, TUniColor bg, uint8_t size )
		{
			int16_t byteWidth = (w + 7) / 8;

//			color = Func::Color565( color );
//			bg = Func::Color565( bg );
  
			setAddrWindow(x,y,x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint16_t AColor;
					if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> ( i & 0b111 )))
						AColor = 0xFFFF;

					else 
      					AColor = 0;

					T_IMPLEMENTATION::spiwrite16( AColor );
				}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawRepeatAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.drawRepeatAlphaColorBitmapImplementation( x, y, bitmap, AListCount, w, h, size );
		}

		void drawRepeatAlphaColorBitmapImplementation( int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			setAddrWindow(x,y, x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint8_t	r = pgm_read_byte( bitmap++ );
				uint8_t	g = pgm_read_byte( bitmap++ );
				uint8_t	b = pgm_read_byte( bitmap++ );
				uint8_t AAlpha = pgm_read_byte( bitmap++ );

				uint8_t	AHi, ALo;

				if( AAlpha == 0xFF )
				{
					uint16_t AColor = Func::Color565( r, g, b );
					AHi = ( AColor >> 8 );
					ALo = AColor;
				}

				else if( AAlpha == 0 )
				{
					AHi = 0;
					ALo = 0;
				}

				else
				{
					float ACoefficient = float( AAlpha ) / 255.0f;

					r = float( r ) * ACoefficient + 0.5;
					g = float( g ) * ACoefficient + 0.5;
					b = float( b ) * ACoefficient + 0.5;

					uint16_t AColor = Func::Color565( r, g, b );
					AHi = ( AColor >> 8 );
					ALo = AColor;
				}

				for( int16_t j = 0; j <= ACount; ++ j )
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawRepeatAlphaColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatAlphaColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.drawRepeatAlphaColorBitmapImplementation_565( x, y, bitmap, AListCount, w, h, size );
		}

		void drawRepeatAlphaColorBitmapImplementation_565( int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			setAddrWindow(x,y, x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );
				uint8_t AAlpha = pgm_read_byte( bitmap ++ );

				uint32_t r = ARedGreen & 0b11111000;
				uint32_t b = 0xFF & ( AGreenBlue << 3 );
				uint32_t g = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

				uint8_t AHi, ALo;

				if( AAlpha == 0xFF )
				{
					uint16_t AColor = Func::Color565( r, g, b );
					AHi = AColor >> 8;
					ALo = AColor;
				}

				else if( AAlpha == 0 )
				{
					AHi = 0;
					ALo = 0;
				}

				else
				{
					float ACoefficient = float( AAlpha ) / 255.0f;

					r = float( r ) * ACoefficient + 0.5;
					g = float( g ) * ACoefficient + 0.5;
					b = float( b ) * ACoefficient + 0.5;

					uint16_t AColor = Func::Color565( r, g, b );
					AHi = AColor >> 8;
					ALo = AColor;
				}

				for( int16_t j = 0; j <= ACount; ++ j )
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawRepeatColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.drawRepeatColorBitmapImplementation( x, y, bitmap, AListCount, w, h, size );
		}

		void drawRepeatColorBitmapImplementation( int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			setAddrWindow(x,y, x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARed = pgm_read_byte( bitmap ++ );
				uint32_t AGreen = pgm_read_byte( bitmap ++ );
				uint32_t ABlue = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( ARed << 16 ) | ( AGreen << 8 ) | ABlue );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
					T_IMPLEMENTATION::spiwrite( AHi, ALo );
			}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawRepeatColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.drawRepeatColorBitmapImplementation_565( x, y, bitmap, AListCount, w, h, size );
		}

		void drawRepeatColorBitmapImplementation_565( int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			setAddrWindow(x,y, x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint8_t AHi = pgm_read_byte( bitmap ++ );
				uint8_t ALo = pgm_read_byte( bitmap ++ );

				for( int16_t j = 0; j <= ACount; ++ j )
					T_IMPLEMENTATION::spiwrite( AHi, ALo );
			}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawRepeatGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatGrayscaleBitmap( AImplementation, x, y, bitmap, AListCount, w, h, AAlpha, size );
				return;
			}

			AImplementation.drawRepeatGrayscaleBitmapImplementation( x, y, bitmap, w, h, size );
		}

		void drawRepeatGrayscaleBitmapImplementation( int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			setAddrWindow(x,y, x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static void drawAlphaGrayscaleRepeatBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawAlphaGrayscaleRepeatBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.drawAlphaGrayscaleRepeatBitmapImplementation( x, y, bitmap, AListCount, w, h, size );
		}

		void drawAlphaGrayscaleRepeatBitmapImplementation( int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			setAddrWindow(x,y, x + w - 1, y + h - 1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap );

				bitmap += 2;

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			}

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void drawGrayscalePixel( T_SELF &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			uint16_t AGray = ( color & 0xFF );
			AImplementation.drawPixel( x, y, color << 16 | ( AGray << 8 ) | AGray );
		}

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			uint16_t AHeight = height();
			uint16_t AWidth = width();

			if((x < 0) ||(x >= AWidth) || (y < 0) || (y >= AHeight)) 
				return;

			color = Func::Color565( color );

			setAddrWindow(x,y,x+1,y+1);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			T_IMPLEMENTATION::spiwrite16(color);

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void fillGrayscaleRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			uint16_t AGray = ( color & 0xFF );
			fillRect( AImplementation, x, y, w, h, color << 16 | ( AGray << 8 ) | AGray );
		}

		static inline void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color)
		{
			AImplementation.drawFastHLineImplementation( x, y, w, color );
		}

		void drawFastHLineImplementation( int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = height();
			uint16_t AWidth = width();
			if((x >= AWidth ) || (y >= AHeight )) 
				return;

			if((x+w-1) >= AWidth )
				w = AWidth - x;

			color = Func::Color565( color );
			setAddrWindow(x, y, x+w-1, y);

			uint8_t AHi = color >> 8, ALo = color;

			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );
			while (w--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			AImplementation.drawFastVLineImplementation( x, y, h, color );
		}

		void drawFastVLineImplementation( int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = height();
			uint16_t AWidth = width();
			if((x >= AWidth) || (y >= AHeight)) 
				return;

			if((y+h-1) >= AHeight) 
				h = AHeight-y;

			color = Func::Color565( color );
			setAddrWindow(x, y, x, y+h-1);

			uint8_t AHi = color >> 8, ALo = color;
    
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );
			while (h--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return 0x0000; // Not supported
		}

		static inline void Scroll( T_SELF &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			// Not supported
		}

/*
		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if((x < 0) || (x >= width() ) || (y < 0) || (y >= height()))
				return 0x0000;

			setAddrWindow(x,y,x+1,y+1, ST7735::RAMRD);
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );

			uint16_t AColor = uint16_t( spiread()) << 8;
			AColor |= spiread();
//			T_IMPLEMENTATION::spiwrite(color >> 8);
//			T_IMPLEMENTATION::spiwrite(color);

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
			return AColor;
		}
*/
		static inline void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			// rudimentary clipping (drawChar w/big text requires this)
			AImplementation.fillRectImplementation( x, y, w, h, color );
		}

		void fillRectImplementation( int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			int16_t AWidth = width();
			int16_t AHeight = height();

			if((x >= AWidth) || (y >= AHeight))
				return;

			if((x + w - 1) >= AWidth)
				w = AWidth - x;

			if((y + h - 1) >= AHeight)
				h = AHeight - y;

/*
			Serial.print( x );
			Serial.print( ", " );
			Serial.print( y );
			Serial.print( ", " );
			Serial.print( x+w-1 );
			Serial.print( ", " );
			Serial.println( y+h-1 );
*/
			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t AHi = color >> 8, ALo = color;
    
			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectOutput( true );
			T_IMPLEMENTATION::SetChipSelectOutput( false );
			for(y=h; y>0; y--)
				for(x=w; x>0; x--)
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectOutput( true );
			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			fillRect( AImplementation, 0, 0, AImplementation.width(), AImplementation.height(), 0xFF000000 | color );
		}

	public:
//		inline void SystemInit()
//		{
//		}

		inline void SystemStart()
		{
			UpdateOrientation();
			UpdateInverted();

			ClearScreen( *this, NonTransparent( inherited::BackgroundColor().GetValue() ));

//			inherited::setTextSize( Text().Size() );
//			inherited::setTextColor( Text().Color(), Text().BackgroundColor().GetValue() );
//			inherited::setTextWrap( Text().Wrap() );

			inherited::RenderElements();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );

		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return inherited::write( AImplementation, AConfig, AValue );
		}

		template <typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template <typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template <typename T> inline size_t print( T AValue )
		{
			return print<T_SELF, T>( this, AValue );
		}

		template <typename T> inline size_t println( T AValue )
		{
			return println<T_SELF, T>( this, AValue );
		}

	public:
		inline void SystemInit()
		{
			T_IMPLEMENTATION::CommonInit();

			if( T_ResetOutputPin::GetPinIsConnected()) 
			{
				T_ResetOutputPin::SetPinValueHigh();
				delay(500);
				T_ResetOutputPin::SetPinValueLow();
				delay(500);
				T_ResetOutputPin::SetPinValueHigh();
				delay(500);
			}

			T_IMPLEMENTATION_TYPE::SystemInit( this );

//			commandList(cmdList);
		}

	};
//---------------------------------------------------------------------------
/*
	template <uint32_t C_UNIQUE, typename T_BASE> class ST7735_Reset_EmbeddedPinImplementation : public DigitalPin_EmbeddedPinImplementation<C_UNIQUE, T_BASE>
	{
		typedef DigitalPin_EmbeddedPinImplementation<C_UNIQUE, T_BASE> inherited;

	public:
		void ResetDisplay()
		{
			inherited::SetPinValueHigh();
			delay(500);
			inherited::SetPinValueLow();
			delay(500);
			inherited::SetPinValueHigh();
			delay(500);
		}

	};
	//---------------------------------------------------------------------------
	template <uint8_t C_PIN_NUMBER>  class ST7735_Reset_NoImplementation
	{
	public:
		inline void ResetDisplay()
		{
		}

	};
//---------------------------------------------------------------------------
	template <uint8_t C_PIN_NUMBER>  class ST7735_Reset_DirectBoardPinImplementation
	{
	public:
		void ResetDisplay()
		{
			digitalWrite( C_PIN_NUMBER, HIGH );
			delay(500);
			digitalWrite( C_PIN_NUMBER, LOW );
			delay(500);
			digitalWrite( C_PIN_NUMBER, HIGH );
			delay(500);
		}

	public:
		inline ST7735_Reset_DirectBoardPinImplementation()
		{
			pinMode( C_PIN_NUMBER, OUTPUT );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_PIN, T_PIN &C_PIN>  class ST7735_Reset_DirectPinImplementation
	{
	public:
		void ResetDisplay()
		{
			C_PIN.DigitalWrite( true );
			delay(500);
			C_PIN.DigitalWrite( false );
			delay(500);
			C_PIN.DigitalWrite( true );
			delay(500);
		}

	public:
		inline ST7735_Reset_DirectPinImplementation()
		{
			C_PIN.SetMode( OUTPUT );
		}

	};
*/
//---------------------------------------------------------------------------
	template <
			typename T_SPI, 
			T_SPI &C_SPI, 
			typename T_ChipSelectOutputPin,
			typename T_RegisterSelectOutputPin,
			typename T_SPISpeed
//			typename T_RESET_IMPLEMENTATION, 
//			typename T_REGISTER_SELECT_IMPLEMENTATION, 
//			typename T_CHIP_SELECT_IMPLEMENTATION, 
//			uint8_t C_OFFSET_X, 
//			uint8_t C_OFFSET_Y
		> class BasicSPI_ST7735 : 
			public T_ChipSelectOutputPin,
			public T_RegisterSelectOutputPin,
			public T_SPISpeed
	{
//	public:
//		TArduinoGraphicsOrientation	Orientation : 2; //= goUp;

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( RegisterSelectOutputPin )

	public:
		_V_PROP_( SPISpeed )

	public:
		inline void BeginTransaction()
		{
#if defined (SPI_HAS_TRANSACTION)
			C_SPI.beginTransaction(SPISettings( SPISpeed().GetValue(), MSBFIRST, T_ChipSelectOutputPin::GetPinIsConnected() ? SPI_MODE0 : SPI_MODE2 ));
#endif
		}

		inline void EndTransaction()
		{
#if defined (SPI_HAS_TRANSACTION)
			C_SPI.endTransaction();
#endif
		}

		inline void SetRegisterSelectOutput( bool AValue )
		{
			T_RegisterSelectOutputPin::SetPinValue( AValue );
		}

		inline void SetChipSelectOutput( bool AValue )
		{
			T_ChipSelectOutputPin::SetPinValue( AValue );
		}

	public:
		inline void spiwrite( uint8_t AHi, uint8_t ALo) 
		{
#ifdef __ST7735__DEBUG__
		  Serial.println(AHi, HEX);
		  Serial.println(ALo, HEX);
#endif

#if defined (SPI_HAS_TRANSACTION)
		  C_SPI.transfer( AHi );
		  C_SPI.transfer( ALo );
#elif defined (__AVR__)
		  uint8_t SPCRbackup = SPCR;
		  SPCR = ST7735::mySPCR;
		  C_SPI.transfer( AHi );
		  C_SPI.transfer( ALo );
		  SPCR = SPCRbackup;
//      SPDR = c;
//      while(!(SPSR & _BV(SPIF)));
#elif defined (__arm__)
		  C_SPI.setClockDivider(21); //4MHz
		  C_SPI.setDataMode(SPI_MODE0);
		  C_SPI.transfer( AHi );
		  C_SPI.transfer( ALo );
#endif
		}

		inline void spiwrite16( uint16_t AColor ) 
		{
#ifdef __ST7735__DEBUG__
		  Serial.println(AColor, HEX);
#endif

#if defined (SPI_HAS_TRANSACTION)
		  C_SPI.transfer( AColor >> 8 );
		  C_SPI.transfer( AColor );
#elif defined (__AVR__)
		  uint8_t SPCRbackup = SPCR;
		  SPCR = ST7735::mySPCR;
		  C_SPI.transfer( AColor >> 8 );
		  C_SPI.transfer( AColor );
		  SPCR = SPCRbackup;
//      SPDR = c;
//      while(!(SPSR & _BV(SPIF)));
#elif defined (__arm__)
		  C_SPI.setClockDivider(21); //4MHz
		  C_SPI.setDataMode(SPI_MODE0);
		  C_SPI.transfer( AColor >> 8 );
		  C_SPI.transfer( AColor );
#endif
		}

		inline void spiwrite32( uint32_t AValue ) 
		{
#ifdef __ST7735__DEBUG__
		  Serial.println(AValue, HEX);
#endif

#if defined (SPI_HAS_TRANSACTION)
		  C_SPI.transfer( AValue >> 24 );
		  C_SPI.transfer( AValue >> 16 );
		  C_SPI.transfer( AValue >> 8 );
		  C_SPI.transfer( AValue );
#elif defined (__AVR__)
		  uint8_t SPCRbackup = SPCR;
		  SPCR = ST7735::mySPCR;
		  C_SPI.transfer( AValue >> 24 );
		  C_SPI.transfer( AValue >> 16 );
		  C_SPI.transfer( AValue >> 8 );
		  C_SPI.transfer( AValue );
		  SPCR = SPCRbackup;
//      SPDR = c;
//      while(!(SPSR & _BV(SPIF)));
#elif defined (__arm__)
		  C_SPI.setClockDivider(21); //4MHz
		  C_SPI.setDataMode(SPI_MODE0);
		  C_SPI.transfer( AValue >> 24 );
		  C_SPI.transfer( AValue >> 16 );
		  C_SPI.transfer( AValue >> 8 );
		  C_SPI.transfer( AValue );
#endif
		}

		inline void spiwrite(uint8_t c) 
		{
#ifdef __ST7735__DEBUG__
		  Serial.println(c, HEX);
#endif

#if defined (SPI_HAS_TRANSACTION)
		  C_SPI.transfer(c);
#elif defined (__AVR__)
		  uint8_t SPCRbackup = SPCR;
		  SPCR = ST7735::mySPCR;
		  C_SPI.transfer(c);
		  SPCR = SPCRbackup;
//      SPDR = c;
//      while(!(SPSR & _BV(SPIF)));
#elif defined (__arm__)
		  C_SPI.setClockDivider(21); //4MHz
		  C_SPI.setDataMode(SPI_MODE0);
		  C_SPI.transfer(c);
#endif
		}

/*
		inline uint8_t spiread()
		{
		  //Serial.println(c, HEX);

#if defined (SPI_HAS_TRANSACTION)
			uint8_t AResult = C_SPI.transfer(0);
#elif defined (__AVR__)
			uint8_t SPCRbackup = SPCR;
			SPCR = mySPCR;
			uint8_t AResult = C_SPI.transfer(0);
			SPCR = SPCRbackup;
//      SPDR = c;
//      while(!(SPSR & _BV(SPIF)));
#elif defined (__arm__)
			C_SPI.setClockDivider(21); //4MHz
			C_SPI.setDataMode(SPI_MODE0);
			uint8_t AResult = C_SPI.transfer(0);
#endif
			return AResult;
		}
*/
		void writecommand(uint8_t c) 
		{
#ifdef __ST7735__DEBUG__
			Serial.print("C ");
#endif

			BeginTransaction();
			SetRegisterSelectOutput( false );
			SetChipSelectOutput( false );

			spiwrite(c);

			SetChipSelectOutput( true );

			EndTransaction();
		}

		void writedata(uint8_t c) 
		{
#ifdef __ST7735__DEBUG__
			Serial.print("D ");
#endif

			BeginTransaction();
			SetRegisterSelectOutput( true );
			SetChipSelectOutput( false );
    
			spiwrite(c);

			SetChipSelectOutput( true );
			EndTransaction();
		}

		void writedata32(uint32_t c) 
		{
#ifdef __ST7735__DEBUG__
			Serial.print("D16 ");
#endif

			BeginTransaction();
			SetRegisterSelectOutput( true );
			SetChipSelectOutput( false );
    
			spiwrite32( c );

			SetChipSelectOutput( true );
			EndTransaction();
		}

		// Companion code to the above tables.  Reads and issues
		// a series of LCD commands stored in PROGMEM byte array.
		void commandList(const uint8_t *addr) 
		{
			uint8_t  numCommands, numArgs;
			uint16_t ms;

			numCommands = pgm_read_byte(addr++);   // Number of commands to follow
			while(numCommands--) 
			{                 // For each command...
				writecommand(pgm_read_byte(addr++)); //   Read, issue command
				numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
				ms       = numArgs & ST7735::DELAY;          //   If hibit set, delay follows args
				numArgs &= ~ST7735::DELAY;                   //   Mask out delay bit
				while(numArgs--)                     //   For each argument...
					writedata(pgm_read_byte(addr++));  //     Read, issue argument

				if(ms) 
				{
					ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
					if(ms == 255) 
						ms = 500;     // If 255, delay for 500 ms

					delay(ms);
				}
			}
		}

		void CommonInit()
		{
#if defined (SPI_HAS_TRANSACTION)
#elif defined (__AVR__)
			uint8_t SPCRbackup = SPCR;
//			C_SPI.begin();
			C_SPI.setClockDivider(SPI_CLOCK_DIV4);
			C_SPI.setDataMode(SPI_MODE0);
			mySPCR = SPCR; // save our preferred state
			//Serial.print("mySPCR = 0x"); Serial.println(SPCR, HEX);
			SPCR = SPCRbackup;  // then restore
#elif defined (__SAM3X8E__)
//			C_SPI.begin();
			C_SPI.setClockDivider(21); //4MHz
			C_SPI.setDataMode(SPI_MODE0);
#endif
		}

	public:
//		BasicSPI_ST7735() :
//			Orientation( goUp ),
//			Inverted( false )
//		{
//		}
	};
//---------------------------------------------------------------------------
/*
template <typename T_SPI, T_SPI &C_SPI> class DisplayST7735 : public DisplayST7735Basic<T_SPI, C_SPI>
	{
		typedef DisplayST7735Basic<T_SPI, C_SPI> inherited;

	public:
		using inherited::inherited;

	};
*/
//---------------------------------------------------------------------------
#ifdef _MITOV_DISPLAY_ST7735B_
#endif
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

