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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
namespace ILI9341Extended
{
	const uint8_t INIT_DELAY = 0xFF;

	const uint8_t MADCTL		= 0x36;
	const uint8_t MADCTL_MY  = 0x80;
	const uint8_t MADCTL_MX  = 0x40;
	const uint8_t MADCTL_MV  = 0x20;
	const uint8_t MADCTL_ML  = 0x10;
	const uint8_t MADCTL_RGB = 0x00;
	const uint8_t MADCTL_BGR = 0x08;
	const uint8_t MADCTL_MH  = 0x04;

	const uint8_t COLADDRSET = 0x2A;
	const uint8_t PAGEADDRSET = 0x2B;

	const uint8_t SLPOUT = 0x11;
	const uint8_t DISPON = 0x29;
	const uint8_t PWCTR1 = 0xC0;
	const uint8_t PWCTR2 = 0xC1;
	const uint8_t VMCTR1 = 0xC5;
	const uint8_t VMCTR2 = 0xC7;
	const uint8_t PIXFMT = 0x3A;
	const uint8_t FRMCTR1 = 0xB1;
	const uint8_t DFUNCTR = 0xB6;
	const uint8_t GAMMASET = 0x26;
	const uint8_t GMCTRP1 = 0xE0;
	const uint8_t GMCTRN1 = 0xE1;
}
//---------------------------------------------------------------------------
	class DisplayILI9341_Otientation_Up
	{
	public:
		template <typename T> static inline int16_t GetWidth( T *AInstance )
		{
			switch( AInstance->Orientation() )
			{
				case goUp:
				case goDown:
					return AInstance->GetConstWidth();

			}

			return AInstance->GetConstHeight();
		}

		template <typename T> static inline int16_t GetHeight( T *AInstance )
		{
			switch( AInstance->Orientation() )
			{
				case goUp:
				case goDown:
					return AInstance->GetConstHeight();

			}

			return AInstance->GetConstWidth();
		}

		template <typename T> static inline int8_t GetControl( T *AInstance )
		{
			switch( AInstance->Orientation() )
			{
				case goUp:
					return Mitov::ILI9341Extended::MADCTL_BGR;

				case goDown:
					return Mitov::ILI9341Extended::MADCTL_MX | Mitov::ILI9341Extended::MADCTL_MY | Mitov::ILI9341Extended::MADCTL_BGR;

				case goLeft:
					return Mitov::ILI9341Extended::MADCTL_MY | Mitov::ILI9341Extended::MADCTL_MV | Mitov::ILI9341Extended::MADCTL_BGR;

				case goRight:
					return Mitov::ILI9341Extended::MADCTL_MX | Mitov::ILI9341Extended::MADCTL_MV | Mitov::ILI9341Extended::MADCTL_BGR;
			}
		}

	};
//---------------------------------------------------------------------------
	class DisplayILI9341_Otientation_Left
	{
	public:
		template <typename T> static inline int16_t GetWidth( T *AInstance )
		{
			switch( AInstance->Orientation() )
			{
				case goUp:
				case goDown:
					return AInstance->GetConstHeight();

			}

			return AInstance->GetConstWidth();
		}

		template <typename T> static inline int16_t GetHeight( T *AInstance )
		{
			switch( AInstance->Orientation() )
			{
				case goUp:
				case goDown:
					return AInstance->GetConstWidth();

			}

			return AInstance->GetConstHeight();
		}

		template <typename T> static inline int8_t GetControl( T *AInstance )
		{
			switch( AInstance->Orientation() )
			{
//				case goUp:
                case goRight:
					return Mitov::ILI9341Extended::MADCTL_BGR;

//				case goDown:
                case goLeft:
					return Mitov::ILI9341Extended::MADCTL_MX | Mitov::ILI9341Extended::MADCTL_MY | Mitov::ILI9341Extended::MADCTL_BGR;

//				case goLeft:
                case goUp:
					return Mitov::ILI9341Extended::MADCTL_MY | Mitov::ILI9341Extended::MADCTL_MV | Mitov::ILI9341Extended::MADCTL_BGR;

//				case goRight:
                case goDown:
					return Mitov::ILI9341Extended::MADCTL_MX | Mitov::ILI9341Extended::MADCTL_MV | Mitov::ILI9341Extended::MADCTL_BGR;
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_0_IMPLEMENTATION,
		typename T_0_ORIENTATION_IMPLEMENTATION,
		typename T_BackgroundColor,
		bool C_HAS_READ,
		uint16_t C_HEIGHT,
		uint16_t C_WIDTH,
		typename T_Elements_Render,
        typename T_FCursorX,
        typename T_FCursorY,
		bool C_SUPPORTS_PIXEL_READ,
		typename T_Orientation,
		typename T_Text
	> class DisplayILI9341_Basic : 
		public T_0_IMPLEMENTATION,
		public GraphicsImpl<
                DisplayILI9341_Basic<
					T_0_IMPLEMENTATION,
					T_0_ORIENTATION_IMPLEMENTATION,
					T_BackgroundColor,
					C_HAS_READ,
					C_HEIGHT,
					C_WIDTH,
					T_Elements_Render,
                    T_FCursorX,
                    T_FCursorY,
					C_SUPPORTS_PIXEL_READ,
					T_Orientation,
					T_Text
				>, 
                T_BackgroundColor,
                T_Text
            >,
        public T_FCursorX,
        public T_FCursorY,
		public T_Orientation
	{
		typedef DisplayILI9341_Basic T_SELF;

		typedef GraphicsImpl<
                T_SELF,
                T_BackgroundColor,
                T_Text
            > inherited;

	public:
		_V_PROP_( Orientation )
        
	public:
        _V_PROP_( FCursorX )
        _V_PROP_( FCursorY )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::Call();
		}

	public:
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		inline int16_t width(void)
		{
			return T_0_ORIENTATION_IMPLEMENTATION::template GetWidth<T_SELF>( this );
		}

		inline int16_t height(void)
		{
			return T_0_ORIENTATION_IMPLEMENTATION::template GetHeight<T_SELF>( this );
		}
        
        static inline int16_t GetConstWidth() { return C_WIDTH; }
        static inline int16_t GetConstHeight() { return C_HEIGHT; }
        
	public:
		void SetCursor( int16_t x, int16_t y )
		{
			FCursorX() = x;
			FCursorY() = y;
		}

	protected:
		void SendCommandSequence( const uint8_t *APointer, int ASize )
		{
			register int i = 0;
			while( i < ASize )
			{
				uint8_t a = pgm_read_byte(& APointer[i++]);
				uint8_t ACount = pgm_read_byte(& APointer[i++]);
				if( a == ILI9341Extended::INIT_DELAY ) 
					delay( ACount );

				else
				{
					cmd( a );
					for( int j = 0; j < ACount; ++j )
						w_data( pgm_read_byte(& APointer[i++] ));

				}
			}
		}

	public:
		inline void SystemInit()
		{
			T_0_IMPLEMENTATION::InitPins();
		}

		inline void SystemStart()
		{
			UpdateOrientation();

//			ClearScreen( *this, NonTransparent( inherited::BackgroundColor().GetValue() ));
			ClearScreen( *this, inherited::BackgroundColor().GetValue() );

//			inherited::setTextSize( Text().Size() );
//			inherited::setTextColor( Text().Color(), Text().BackgroundColor().GetValue() );
//			inherited::setTextWrap( Text().Wrap() );

			inherited::RenderElements();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );

		}

		void UpdateOrientation()
		{
			cmd( ILI9341Extended::MADCTL);

			w_data( T_0_ORIENTATION_IMPLEMENTATION::template GetControl<T_SELF>( this ));

			setAddrWindow( 0, 0, width() - 1, height() - 1 ); // CS_IDLE happens here
		}

		void setAddrWindow(int x1, int y1, int x2, int y2)
		{
			T_0_IMPLEMENTATION::setAddrWindow( x1, y1, x2, y2 );
		}

		void writeRegister32(uint8_t r, uint32_t d) 
		{
			T_0_IMPLEMENTATION::writeRegister32( r, d );
		}

	public:
		static void drawAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			// check rotation, move pixel around if necessary

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARGBColor = pgm_read_dword( bitmap++ );
					uint8_t AAlpha = ARGBColor >> 24;
					if( AAlpha == 0xFF )
					{
						uint16_t AColor = Func::Color565( ARGBColor );
						AImplementation.w_data_internal( AColor >> 8 );
						AImplementation.w_data_internal( AColor );
					}

					else if( AAlpha == 0 )
					{
						AImplementation.w_data_internal( 0 );
						AImplementation.w_data_internal( 0 );
					}

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
						AImplementation.w_data_internal( AColor >> 8 );
						AImplementation.w_data_internal( AColor );
					}
				}

			AImplementation.w_data_end();
		}

		static void drawAlphaColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawAlphaColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			// check rotation, move pixel around if necessary

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

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
						AImplementation.w_data_internal( AColor >> 8 );
						AImplementation.w_data_internal( AColor );
					}

					else if( AAlpha == 0 )
					{
						AImplementation.w_data_internal( 0 );
						AImplementation.w_data_internal( 0 );
					}

					else
					{
						float ACoefficient = float( AAlpha ) / 255.0f;

						r = float( r ) * ACoefficient + 0.5;
						g = float( g ) * ACoefficient + 0.5;
						b = float( b ) * ACoefficient + 0.5;

						uint16_t AColor = Func::Color565( r, g, b );
						AImplementation.w_data_internal( AColor >> 8 );
						AImplementation.w_data_internal( AColor );
					}
				}

			AImplementation.w_data_end();
		}

		static void drawColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint32_t AGreen = pgm_read_byte( bitmap ++ );
					uint32_t ABlue = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGreen << 8 ) | ( ARed << 16 ) | ABlue );

					AImplementation.w_data_internal( AColor >> 8 );
					AImplementation.w_data_internal( AColor );
				}

			AImplementation.w_data_end();
		}

		static void drawColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
					uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );

					AImplementation.w_data_internal( ARedGreen );
					AImplementation.w_data_internal( AGreenBlue );
				}

			AImplementation.w_data_end();
		}

		static void drawGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawGrayscaleBitmap( AImplementation, x, y, bitmap, w, h, AAlpha, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawGrayscaleBitmap( AImplementation, x, y, bitmap, w, h, AAlpha, size );
				return;
			}

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

					AImplementation.w_data_internal( AColor >> 8 );
					AImplementation.w_data_internal( AColor );
				}

			AImplementation.w_data_end();
		}

		static void drawAlphaGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint16_t *_bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawAlphaGrayscaleBitmap( AImplementation, x, y, _bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaGrayscaleBitmap( AImplementation, x, y, _bitmap, w, h, size );
				return;
			}

			const uint8_t *bitmap = (const uint8_t *)_bitmap;

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );
					bitmap += 2;

					AImplementation.w_data_internal( AColor >> 8 );
					AImplementation.w_data_internal( AColor );
				}

			AImplementation.w_data_end();
		}

		static void drawBWBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, TUniColor color, TUniColor bg, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawBWBitmap( AImplementation, x, y, bitmap, w, h, color, bg, size );
				return;
			}

			int16_t byteWidth = (w + 7) / 8;

//			color = Func::Color565( color );
//			bg = Func::Color565( bg );
  
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint16_t AColor;
					if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> ( i & 0b111 )))
						AColor = 0xFFFF;

					else 
      					AColor = 0;

					AImplementation.w_data_internal( AColor >> 8 );
					AImplementation.w_data_internal( AColor );
				}

			AImplementation.w_data_end();
		}

		static void drawRepeatAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

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
				{
					AImplementation.w_data_internal( AHi );
					AImplementation.w_data_internal( ALo );
				}
			}

			AImplementation.w_data_end();
		}

		static void drawRepeatAlphaColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawRepeatAlphaColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatAlphaColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

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
				{
					AImplementation.w_data_internal( AHi );
					AImplementation.w_data_internal( ALo );
				}
			}

			AImplementation.w_data_end();
		}

		static void drawRepeatColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

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
				{
					AImplementation.w_data_internal( AHi );
					AImplementation.w_data_internal( ALo );
				}
			}

			AImplementation.w_data_end();
		}

		static void drawRepeatColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );

				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data_internal( ARedGreen );
					AImplementation.w_data_internal( AGreenBlue );
				}
			}

			AImplementation.w_data_end();
		}

		static void drawRepeatGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawRepeatGrayscaleBitmap( AImplementation, x, y, bitmap, AListCount, w, h, AAlpha, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatGrayscaleBitmap( AImplementation, x, y, bitmap, AListCount, w, h, AAlpha, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data_internal( AHi );
					AImplementation.w_data_internal( ALo );
				}
			}

			AImplementation.w_data_end();
		}

		static void drawAlphaGrayscaleRepeatBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( C_HAS_READ )
			{
				inherited::drawAlphaGrayscaleRepeatBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaGrayscaleRepeatBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap );

				bitmap += 2;

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data_internal( AHi );
					AImplementation.w_data_internal( ALo );
				}
			}

			AImplementation.w_data_end();
		}

		static void drawGrayscalePixel( T_SELF &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			uint16_t AGray = ( color & 0xFF );
			AImplementation.drawPixel( x, y, color << 16 | ( AGray << 8 ) | AGray );
		}

		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			uint16_t AHeight = height();
			uint16_t AWidth = width();

			if((x < 0) ||(x >= AWidth) || (y < 0) || (y >= AHeight)) 
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( ! C_SUPPORTS_PIXEL_READ )
				color |= 0xFF000000;

			T_0_IMPLEMENTATION::drawPixel( x, y, color, AAlpha );
		}

		static void fillGrayscaleRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
//			Serial.print( x ); Serial.print( " " ); Serial.print( y ); Serial.print( " " ); Serial.print( w ); Serial.print( " " ); Serial.print( h ); Serial.print( " " ); Serial.println( uint32_t( color ));

			uint16_t AGray = ( color & 0xFF );
			fillRect( AImplementation, x, y, w, h, color << 16 | ( AGray << 8 ) | AGray );
		}

//#if ( defined(__ILI9341_SCROLL__) && defined(__PARALLEL_COMPONENT_DISPLAY_ILI9341__))

		static inline void Scroll( T_SELF &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			if( C_SUPPORTS_PIXEL_READ )
				T_0_IMPLEMENTATION::template InternalScroll<T_SELF>( AImplementation, AImplementation.width(), AImplementation.height(), X, Y, color );
		}

//#endif

		static void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();
			if((x >= AWidth ) || (y >= AHeight )) 
				return;

			uint8_t AAlpha = color >> 24;
			if( C_SUPPORTS_PIXEL_READ && AAlpha < 255 )
			{
				inherited::drawFastHLine( AImplementation, x, y, w, color );
				return;
			}

			if((x+w-1) >= AWidth )
				w = AWidth - x;

			color = Func::Color565( color );
			AImplementation.setAddrWindow(x, y, x+w-1, y);

			uint8_t hi = color >> 8, lo = color;

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			while (w--) 
			{
				AImplementation.w_data_internal(hi);
				AImplementation.w_data_internal(lo);
			}

			AImplementation.w_data_end();
		}

		static void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();
			if((x >= AWidth) || (y >= AHeight)) 
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( C_SUPPORTS_PIXEL_READ && AAlpha < 255 )
			{
				inherited::drawFastVLine( AImplementation, x, y, h, color );
				return;
			}

			if((y+h-1) >= AHeight) 
				h = AHeight-y;

			color = Func::Color565( color );
			AImplementation.setAddrWindow(x, y, x, y+h-1);

			uint8_t hi = color >> 8, lo = color;

			AImplementation.cmd( 0x2C );
			AImplementation.w_data_start();

			while (h--) 
			{
				AImplementation.w_data_internal(hi);
				AImplementation.w_data_internal(lo);
			}
    
			AImplementation.w_data_end();
		}

		static void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			// rudimentary clipping (drawChar w/big text requires this)
			int16_t AWidth = AImplementation.width();
			int16_t AHeight = AImplementation.height();

			if((x >= AWidth) || (y >= AHeight))
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( C_SUPPORTS_PIXEL_READ && AAlpha < 255 )
			{
				inherited::fillRect( AImplementation, x, y, w, h, color );
				return;
			}

			AImplementation.fillRectImplementation( AWidth, AHeight, x, y, w, h, color );
		}

		static inline TUniColor NonTransparent( TUniColor color )
		{
			return 0xFF000000 | ::Mitov::Func::SwapRB( color );
		}

		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			AImplementation.fillRectImplementation( AImplementation.width(), AImplementation.height(), 0, 0, AImplementation.width(), AImplementation.height(), NonTransparent( color ));
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if( ! C_SUPPORTS_PIXEL_READ )
				return 0x0000;

			if((x < 0) ||(x >= width()) || (y < 0) || (y >= height())) 
				return 0x0000;

			return T_0_IMPLEMENTATION::GetPixelColor( x, y );
		}

	protected:
		inline void cmd( uint8_t cmd )
		{
			T_0_IMPLEMENTATION::cmd( cmd );
		}

		inline void w_data( uint8_t data )
		{
			T_0_IMPLEMENTATION::w_data( data );
		}

		inline void w_data_internal( uint8_t data )
		{
			T_0_IMPLEMENTATION::w_data_internal( data );
		}

		inline void w_data_start()
		{
			T_0_IMPLEMENTATION::w_data_start();
		}

		inline void w_data_end()
		{
			T_0_IMPLEMENTATION::w_data_end();
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
//		template <typename T> size_t print( T AValue )
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

//		template <typename T> size_t println( T AValue )
		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
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
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


