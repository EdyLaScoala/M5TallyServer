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

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

/// Font data stored PER GLYPH
typedef struct {
	uint16_t bitmapOffset;     ///< Pointer into GFXfont->bitmap
	uint8_t  width;            ///< Bitmap dimensions in pixels
    uint8_t  height;           ///< Bitmap dimensions in pixels
	uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
	int8_t   xOffset;          ///< X dist from cursor pos to UL corner
    int8_t   yOffset;          ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
	uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;       ///< Glyph array
	uint8_t   first;       ///< ASCII extents (first char)
    uint8_t   last;        ///< ASCII extents (last char)
	uint8_t   yAdvance;    ///< Newline distance (y axis)
} GFXfont;

#endif // _GFXFONT_H_

namespace Mitov
{
//	class Graphics;
//---------------------------------------------------------------------------
#ifdef __32BIT_GRAPHICS_COORDINATES__
	typedef int32_t		TGraphicsPos;
	typedef uint32_t	TGraphicsSize;
#else
	typedef int16_t		TGraphicsPos;
	typedef uint16_t	TGraphicsSize;
#endif
//---------------------------------------------------------------------------
	typedef uint64_t TUniColor;
//---------------------------------------------------------------------------
	enum TArduinoMonochromeColor { tmcBlack, tmcWhite, tmcInvert, tmcNone };
//---------------------------------------------------------------------------
	class NoFont
	{
	public:
		inline static bool IsImplemented() { return false; }

		inline static constexpr uint8_t GetPixelTextFontHeight() { return 1; }
		inline static constexpr uint8_t GetPixelTextFontWidth() { return 1; }

	public:
		template <typename T_IMPLEMENTATION> inline static uint8_t DrawFontChar( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			return 0;
		}

	};
//---------------------------------------------------------------------------
	template <
		const uint8_t *T_FontPointer,
		uint8_t	C_Width,
		uint8_t	C_Height
	> class FixedSizeFont
	{
	public:
		inline static bool IsImplemented() { return true; }

		inline static constexpr uint8_t GetPixelTextFontHeight() { return C_Height; }
		inline static constexpr uint8_t GetPixelTextFontWidth() { return C_Width; }

	public:
		template <typename T_IMPLEMENTATION> static uint8_t DrawFontChar( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
//		  Serial.println( "drawChar" );
//		  Serial.println( AImplementation.width() );
//		  Serial.println( x );
//		  Serial.println( y );
		  if(( x >= AImplementation.width() )            || // Clip right
			 ( y >= AImplementation.height() )           || // Clip bottom
			 (( x + 6 * size - 1 ) < 0 ) || // Clip left
			 (( y + 8 * size - 1 ) < 0 ))   // Clip top
			return C_Width;

//		  Serial.println( c );

		  if( ! AUseCp437 && ( c >= 176 ) )
			  c++; // Handle 'classic' charset behavior

		  for( int8_t i=0; i<6; i++ )
		  {
			uint8_t line;
			if( i == 5 )
			  line = 0x0;

			else
			  line = pgm_read_byte( T_FontPointer + 3 + (c*5) + i ); // For now ifnore size information

			for( int8_t j = 0; j<8; j++ )
			{
			  if( line & 0x1 )
			  {
				if( size == 1 ) // default size
				  AImplementation.drawPixel( x+i, y+j, color );

				else // big size
				  T_IMPLEMENTATION::fillRect( AImplementation, x+(i*size), y+(j*size), size, size, color );

			  }

			  else if( bg != color )
			  {
				if( size == 1 ) // default size
				  AImplementation.drawPixel( x+i, y+j, bg );

				else  // big size
				  T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, bg );

			  }

			  line >>= 1;
			}
		  }

		  return C_Width * size;
		}

	};
//---------------------------------------------------------------------------
	template <
		const GFXfont *T_FontPointer,
		uint8_t	C_Width,
		uint8_t	C_Height
	> class AdafruitFlexibleSizeFont
	{
	public:
		inline static bool IsImplemented() { return true; }

		inline static constexpr uint8_t GetPixelTextFontHeight() { return C_Height; }
		inline static constexpr uint8_t GetPixelTextFontWidth() { return C_Width; }

	public:
		template <typename T_IMPLEMENTATION> static uint8_t DrawFontChar( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
//			const GFXfont *&gfxFont = (const GFXfont *)T_FontPointer;
			c -= pgm_read_byte( &T_FontPointer->first );
			GFXglyph *glyph  = &(((GFXglyph *)mitov_pgm_read_pointer(&T_FontPointer->glyph))[c]);
			uint8_t  *bitmap = (uint8_t *)mitov_pgm_read_pointer(&T_FontPointer->bitmap);
			int8_t   yAdvance = T_FontPointer->yAdvance;

			uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
			uint8_t  w  = pgm_read_byte(&glyph->width);
			uint8_t  h  = pgm_read_byte(&glyph->height);
			uint8_t  xa = pgm_read_byte(&glyph->xAdvance);
			int8_t   xo = pgm_read_byte(&glyph->xOffset);
			int8_t   yo = int8_t( pgm_read_byte(&glyph->yOffset)) - 1 + C_Height;
			uint8_t  bits, bit = 0;
			int16_t  xo16, yo16;

		  if(( x >= AImplementation.width() )            || // Clip right
			 ( y >= AImplementation.height() )           || // Clip bottom
			 (( x + w * size - 1 ) < 0 ) || // Clip left
			 (( y + C_Height * size - 1 ) < 0 ))   // Clip top
			return C_Width;

//			Serial.print( w ); Serial.print( " - " ); Serial.println( h );

//            Serial.print( xo );Serial.print( " - " ); Serial.println( yo );

			if( size > 1 )
			{
				xo16 = xo;
				yo16 = yo;
			}

			if( size == 1 )
			{
				for( uint8_t yy=0; yy < h; ++yy )
				{
					for( uint8_t xx=0; xx < w; ++xx )
					{
						if( !( bit++ & 7 ))
							bits = pgm_read_byte( &bitmap[ bo++ ] );

						if( bits & 0x80 )
							AImplementation.drawPixel( x+xo+xx, y+yo+yy, color );

						else
							AImplementation.drawPixel( x+xo+xx, y+yo+yy, bg );

						bits <<= 1;
					}
				}
			}
			else
			{
				for( uint8_t yy=0; yy < h; ++yy )
				{
					for( uint8_t xx=0; xx < w; ++xx )
					{
						if( !( bit++ & 7 ))
							bits = pgm_read_byte( &bitmap[ bo++ ] );

						if( bits & 0x80 )
							T_IMPLEMENTATION::fillRect( AImplementation, x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color );

						else
							T_IMPLEMENTATION::fillRect( AImplementation, x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, bg );

						bits <<= 1;
					}
				}
			}

//            Serial.println( w );

			return ( w + 1 ) * size;
		}

	};
//---------------------------------------------------------------------------
	class MonochromeColorOperations
	{
	public:
		static inline bool IsTransparent( TArduinoMonochromeColor AColor ) { return AColor == tmcNone; }
		static inline TArduinoMonochromeColor ExtractPixelColor( uint64_t AColor ) { return ( AColor ) ? tmcWhite : tmcBlack; }
	};
//---------------------------------------------------------------------------
	class GrayscaleColorOperations
	{
	public:
		static inline bool IsTransparent( uint16_t AColor ) { return ( AColor & 0xFF00 ) == 0; }
		static inline uint8_t ExtractPixelColor( uint64_t AColor ) { return AColor; }
	};
//---------------------------------------------------------------------------
	class RGBColorOperations
	{
	public:
		static inline bool IsTransparent( uint32_t AColor ) { return ( AColor & 0xFF000000 ) == 0; }
		static inline uint32_t ExtractPixelColor( uint64_t AColor ) { return AColor; }
	};
//---------------------------------------------------------------------------
	class RGBWColorOperations
	{
	public:
		static inline bool IsTransparent( uint64_t AColor ) { return ( AColor & 0xFF00000000 ) == 0; }
		static inline uint64_t ExtractPixelColor( uint64_t AColor ) { return AColor; }
	};
//---------------------------------------------------------------------------
	struct GraphicsPoint
	{
		TGraphicsPos	X;
		TGraphicsPos	Y;
	};
//---------------------------------------------------------------------------
	template <typename T_BASE> class TTextFieldWrapper : public T_BASE
	{
	public:
		TGraphicsPos	cursor_x;
		TGraphicsPos	cursor_y;

	public:
		inline TGraphicsPos &FCursorX() { return cursor_x; }
		inline TGraphicsPos &FCursorY() { return cursor_y; }

	public:
		TTextFieldWrapper( TGraphicsPos ACursorX, TGraphicsPos ACursorY ) :
			cursor_x( ACursorX ),
			cursor_y( ACursorY )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class GraphicsIntf
	{
	protected:
		inline void RenderElements()
		{
			T_OWNER::Elements_CallChain();
		}

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY ) { AX = 0; AY = 0; }

	};
//---------------------------------------------------------------------------
	template <typename T_X, typename T_Y> class TArduinoGraphicsCornerSize :
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	};
//---------------------------------------------------------------------------
	enum TArduinoTextHorizontalAlign { thaLeft, thaCenter, thaRight };
//---------------------------------------------------------------------------
	enum TArduinoTextVerticalAlign { tvaTop, tvaCenter, tvaBottom };
//---------------------------------------------------------------------------
	template <typename T_IMPLEMENTATION> class Graphics : public OpenWire::Object
	{
//	protected:
//		int16_t	cursor_x = 0;
//		int16_t	cursor_y = 0;

//	protected:
//		const unsigned char * FFont;

	protected:
//		TUniColor	textcolor = 0xFFFF;
//		TUniColor	textbgcolor = 0;

//		uint8_t	textsize = 1;

//		bool wrap : 1;   // If set, 'wrap' text at right edge of display
//		bool _cp437 : 1; // If set, use correct CP437 charset (default is off)

//	public:
//		inline uint8_t GetTextSize() { return textsize; }
/*
	public:
		inline void PrintChar( char AValue )
		{
			write( AValue );
		}

		inline void PrintChar( uint8_t AValue )
		{
			write( AValue );
		}

	public:
		inline template <typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
		template <typename T> size_t print( T AValue )
		{
			GraphicsPrint APrint( this, (GraphicsPrint::TCallback) &Graphics::write );
			return APrint.print( AValue );
		}

		template <typename T> size_t println( T AValue )
		{
			GraphicsPrint APrint( this, (GraphicsPrint::TCallback) &Graphics::write );
			return APrint.println( AValue );
		}
*/
	public:
		static void Scroll( T_IMPLEMENTATION &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
//			Serial.println( "Scroll" );
			if( X == 0 && Y == 0 )
				return;

			int AFromYIndex, AToYIndex;
			int AHeight = AImplementation.height() - abs( Y );
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
			int AWidth = AImplementation.width() - abs( X );
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
						for( int ax = 0; ax < AWidth; ++ax )
							AImplementation.drawPixel( ax + AToXIndex, ay + AToYIndex, AImplementation.GetPixelColor( ax + AFromXIndex, ay + AFromYIndex ));

						T_IMPLEMENTATION::drawFastHLine( AImplementation, AWidth, ay + AToYIndex, AImplementation.width() - AWidth, color );
					}

					else
					{
						for( int ax = AWidth; ax--; )
							AImplementation.drawPixel( ax + AToXIndex, ay + AToYIndex, AImplementation.GetPixelColor( ax + AFromXIndex, ay + AFromYIndex ));

						T_IMPLEMENTATION::drawFastHLine( AImplementation, 0, ay + AToYIndex, X, color );
					}
				}

				T_IMPLEMENTATION::fillRect( AImplementation, 0, AHeight, AImplementation.width(), AImplementation.height(), color );
			}

			else
			{
				for( int ay = AHeight; ay--; )
				{
					if( X < 0 )
					{
						for( int ax = 0; ax < AWidth; ++ax )
							AImplementation.drawPixel( ax + AToXIndex, ay + AToYIndex, AImplementation.GetPixelColor( ax + AFromXIndex, ay + AFromYIndex ));

						T_IMPLEMENTATION::drawFastHLine( AImplementation, AWidth, ay + AToYIndex, AImplementation.width() - AWidth, color );
					}

					else
					{
						for( int ax = AWidth; ax--; )
							AImplementation.drawPixel( ax + AToXIndex, ay + AToYIndex, AImplementation.GetPixelColor( ax + AFromXIndex, ay + AFromYIndex ));

						T_IMPLEMENTATION::drawFastHLine( AImplementation, 0, ay + AToYIndex, X, color );
					}
				}

				T_IMPLEMENTATION::fillRect( AImplementation, 0, 0, AImplementation.width(), Y, color );
			}
		}

		static void drawGrayscalePixel( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			AImplementation.drawPixel( x, y, color );
		}

		static void drawColorPixel( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			AImplementation.drawPixel( x, y, color );
		}

		static void fillGrayscaleRect( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			T_IMPLEMENTATION::fillRect( AImplementation, x, y, w, h, color );
		}

		static void fillColorRect( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			T_IMPLEMENTATION::fillRect( AImplementation, x, y, w, h, color );
		}

	public:
		// This MUST be defined by the subclass:
//		virtual void drawPixel(int16_t x, int16_t y, TUniColor color) = 0;
//		virtual TUniColor	GetBackgroundColor() = 0;

	public:
/*
		void setFont( const unsigned char *AFont )
		{
			FFont = AFont;
		}

		const unsigned char *getFont()
		{
			return FFont;
		}

		void getTextColor( TUniColor &AColor, TUniColor &ABackgroundColor )
		{
			AColor = textcolor;
			ABackgroundColor = textbgcolor;
		}
*/
		static void drawEllipse( T_IMPLEMENTATION &AImplementation, int16_t xc, int16_t yc, int16_t rx, int16_t ry, TUniColor color, bool ACorrectX, bool ACorrectY )
		{
			if( ACorrectY )
			{
				++yc;
				++ry;
			}

			uint32_t hh = ry * ry;
			uint32_t ww = rx * rx;
			uint32_t hhww = hh * ww;
			TGraphicsPos x0 = rx;
			TGraphicsPos dx = 0;
			TGraphicsPos ALastX = rx;

			if( ! ACorrectY )
			{
				AImplementation.drawPixel( xc - rx, yc, color );
				if( ACorrectX )
					AImplementation.drawPixel( xc + rx + 1, yc, color );

				else
					AImplementation.drawPixel( xc + rx, yc, color );

			}

			for (int y = 1; y <= ry; y++)
			{
				uint32_t x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more

				for ( ; x1 > 0; x1--)
					if (x1*x1*hh + y*y*ww <= hhww)
						break;

				dx = x0 - x1;  // current approximation of the slope
				x0 = x1;

				int ay1 = ( ACorrectY ) ? y - 1 : y;
				int aX0R = ( ACorrectX ) ? x0 - 1 : x0;

				if( ALastX - x0 )
				{
					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc - ALastX, yc - y, ALastX - x0, color );
					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc - ALastX, yc + ay1, ALastX - x0, color );

					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc + x0, yc - y, ALastX - x0, color );
					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc + x0, yc + ay1, ALastX - x0, color );
				}

				else
				{
					AImplementation.drawPixel( xc - x0, yc - y, color );
					AImplementation.drawPixel( xc - x0, yc + ay1, color );

					AImplementation.drawPixel( xc + aX0R, yc - y, color );
					AImplementation.drawPixel( xc + aX0R, yc + ay1, color );
				}

				ALastX = x0;
			}
		}

		static void drawEllipseWH( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_IMPLEMENTATION::drawEllipse( AImplementation, x + w / 2, y + h / 2, w / 2, h / 2, color, !( w & 1 ), !( h & 1 ) );
		}

		static void drawEllipseHelper( T_IMPLEMENTATION &AImplementation, int16_t xc, int16_t yc, int16_t rx, int16_t ry, uint8_t ACorners, TUniColor color ) //, bool ACorrectX = false, bool ACorrectY = false )
		{
//			if( ACorrectY )
//				++ry;

			uint32_t hh = ry * ry;
			uint32_t ww = rx * rx;
			uint32_t hhww = hh * ww;
			TGraphicsPos x0 = rx;
			TGraphicsPos dx = 0;
			TGraphicsPos ALastX = rx;

/*
			if( ! ACorrectY )
			{
				drawPixel( xc - rx, yc, color );
				if( ACorrectX )
					drawPixel( xc + rx + 1, yc, color );

				else
					drawPixel( xc + rx, yc, color );

			}
*/
			for (int y = 1; y <= ry; y++)
			{
				uint32_t x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more

				for ( ; x1 > 0; x1--)
					if (x1*x1*hh + y*y*ww <= hhww)
						break;

				dx = x0 - x1;  // current approximation of the slope
				x0 = x1;

//				int ay1 = ( ACorrectY ) ? y - 1 : y;
//				int aX0R = ( ACorrectX ) ? x0 - 1 : x0;

				if( ALastX - x0 )
				{
					if( ACorners & 1 )
						T_IMPLEMENTATION::drawFastHLine( AImplementation, xc - ALastX, yc - y, ALastX - x0, color );

					if( ACorners & 8 )
						T_IMPLEMENTATION::drawFastHLine( AImplementation, xc - ALastX, yc + y, ALastX - x0, color );

					if( ACorners & 2 )
						T_IMPLEMENTATION::drawFastHLine( AImplementation, xc + x0, yc - y, ALastX - x0, color );

					if( ACorners & 4 )
						T_IMPLEMENTATION::drawFastHLine( AImplementation, xc + x0, yc + y, ALastX - x0, color );
				}

				else
				{
					if( ACorners & 1 )
						AImplementation.drawPixel( xc - x0, yc - y, color );

					if( ACorners & 8 )
						AImplementation.drawPixel( xc - x0, yc + y, color );

					if( ACorners & 2 )
						AImplementation.drawPixel( xc + x0, yc - y, color );

					if( ACorners & 4 )
						AImplementation.drawPixel( xc + x0, yc + y, color );
				}

				ALastX = x0;
			}
		}

		static void fillEllipse( T_IMPLEMENTATION &AImplementation, int16_t ax, int16_t ay, int16_t rx, int16_t ry, TUniColor color, bool ACorrectX, bool ACorrectY )
		{
//			Serial.print( "fillEllipse: " ); Serial.print( ax ); Serial.print( "x" ); Serial.println( ay );
			if( ACorrectY )
			{
				++ay;
				++ry;
			}

			uint32_t hh = ry * ry;
			uint32_t ww = rx * rx;
			uint32_t hhww = hh * ww;
			TGraphicsPos x0 = rx;
			TGraphicsPos dx = 0;

			if( ! ACorrectY )
			{
				if( ACorrectX )
					T_IMPLEMENTATION::drawFastHLine( AImplementation, ax - rx, ay, rx * 2 + 1, color );

				else
					T_IMPLEMENTATION::drawFastHLine( AImplementation, ax - rx, ay, rx * 2, color );

			}

			for (int y = 1; y <= ry; y++)
			{
				uint32_t x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more

				for ( ; x1 > 0; x1--)
					if (x1*x1*hh + y*y*ww <= hhww)
						break;

				dx = x0 - x1;  // current approximation of the slope
				x0 = x1;

				int ay1 = ( ACorrectY ) ? y - 1 : y;
				int aWidth = ( ACorrectX ) ? x0 * 2 + 1 : x0 * 2;

				T_IMPLEMENTATION::drawFastHLine( AImplementation, ax - x0, ay - y, aWidth, color );
				T_IMPLEMENTATION::drawFastHLine( AImplementation, ax - x0, ay + ay1, aWidth, color );
			}
		}

		static void fillEllipseWH( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_IMPLEMENTATION::fillEllipse( AImplementation, x + w / 2, y + h / 2, w / 2, h / 2, color, !( w & 1 ), !( h & 1 ) );
		}

		static void fillEllipseHelper( T_IMPLEMENTATION &AImplementation, int16_t xc, int16_t yc, int16_t rx, int16_t ry, uint8_t ACorners, TUniColor color )
		{
//			if( ACorrectY )
//				++ry;

			uint32_t hh = ry * ry;
			uint32_t ww = rx * rx;
			uint32_t hhww = hh * ww;
			TGraphicsPos x0 = rx;
			TGraphicsPos dx = 0;

/*
			if( ! ACorrectY )
			{
				if( ACorrectX )
					drawFastHLine( ax - rx, ay, rx * 2 + 1, color );

				else
					drawFastHLine( ax - rx, ay, rx * 2, color );

			}
*/
			for (int y = 1; y <= ry; y++)
			{
				uint32_t x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more

				for ( ; x1 > 0; x1--)
					if (x1*x1*hh + y*y*ww <= hhww)
						break;

				dx = x0 - x1;  // current approximation of the slope
				x0 = x1;

//				int ay1 = ( ACorrectY ) ? y - 1 : y;
//				int aWidth = ( ACorrectX ) ? x0 * 2 + 1 : x0 * 2;

				if( ACorners & 1 )
					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc - x0, yc - y, x0, color );

				if( ACorners & 8 )
					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc - x0, yc + y, x0, color );

				if( ACorners & 2 )
					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc, yc - y, x0, color );

				if( ACorners & 4 )
					T_IMPLEMENTATION::drawFastHLine( AImplementation, xc, yc + y, x0, color );

//				drawFastHLine( ax - x0, ay - y, aWidth, color );
//				drawFastHLine( ax - x0, ay + ay1, aWidth, color );

			}
		}

		static inline void ClearScreen( T_IMPLEMENTATION &AImplementation, TUniColor color )
		{
			T_IMPLEMENTATION::fillRect( AImplementation, 0, 0, AImplementation.width(), AImplementation.height(), color );
		}
/*
		inline bool getTextWrap()
		{
			return wrap;
		}
*/
		static void drawRect( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_IMPLEMENTATION::drawFastHLine( AImplementation, x, y, w, color);
			T_IMPLEMENTATION::drawFastHLine( AImplementation, x, y+h-1, w, color);
			T_IMPLEMENTATION::drawFastVLine( AImplementation, x, y, h, color);
			T_IMPLEMENTATION::drawFastVLine( AImplementation, x+w-1, y, h, color);
		}

		static void fillRect( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
//			Serial.println( "fillRect" );
			// Update in subclasses if desired!
			for (int16_t i=x; i<x+w; i++)
				T_IMPLEMENTATION::drawFastVLine( AImplementation, i, y, h, color );

		}

		static inline void fillScreen( T_IMPLEMENTATION &AImplementation, TUniColor color )
		{
			T_IMPLEMENTATION::fillRect( AImplementation, 0, 0, AImplementation.width(), AImplementation.height(), color);
		}

		static void drawGrayscaleBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			TUniColor AAlpha16 = uint16_t( AAlpha ) << 8;

			if (size == 1) // default size
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
						T_IMPLEMENTATION::drawGrayscalePixel( AImplementation, x+i, y+j, AAlpha16 | pgm_read_byte( bitmap ++ ));

			}

			else
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
						T_IMPLEMENTATION::fillGrayscaleRect( AImplementation, x+i * size, y+j * size, size, size, AAlpha16 | pgm_read_byte( bitmap ++ ));

			}
		}

		static void drawRepeatGrayscaleBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			TUniColor AAlpha16 = uint16_t( AAlpha ) << 8;

			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint32_t AGrayscale = pgm_read_byte( bitmap ++ );

				uint32_t AColor = AAlpha16 | AGrayscale;

				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::drawGrayscalePixel( AImplementation, x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillGrayscaleRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawAlphaGrayscaleRepeatBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint16_t AGrayscale = pgm_read_byte( bitmap ++ );
				uint16_t AColor = ( AGrayscale << 8 ) | pgm_read_byte( bitmap ++ );

				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::drawGrayscalePixel( AImplementation, x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillGrayscaleRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawAlphaColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if (size == 1) // default size
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
						AImplementation.drawPixel( x+i, y+j, pgm_read_dword( bitmap ++ ));
			}

			else
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
						T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, pgm_read_dword( bitmap ++ ));
			}

		}

		static void drawAlphaColorBitmap_565( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if (size == 1) // default size
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
					{
						uint32_t ARedGreen = pgm_read_byte( bitmap ++ );
						uint32_t AGreenBlue = pgm_read_byte( bitmap ++ );
						uint32_t AAlpha = pgm_read_byte( bitmap ++ );

						uint32_t ARed = ARedGreen & 0b11111000;
						uint32_t ABlue = 0xFF & ( AGreenBlue << 3 );
						uint32_t AGreen = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

						uint32_t AColor = ( AAlpha << 24 ) | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

						AImplementation.drawPixel( x+i, y+j, AColor );
					}
			}

			else
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
					{
						uint32_t ARedGreen = pgm_read_byte( bitmap ++ );
						uint32_t AGreenBlue = pgm_read_byte( bitmap ++ );
						uint32_t AAlpha = pgm_read_byte( bitmap ++ );

						uint32_t ARed = ARedGreen & 0b11111000;
						uint32_t ABlue = 0xFF & ( AGreenBlue << 3 );
						uint32_t AGreen = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

						uint32_t AColor = ( AAlpha << 24 ) | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

						T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, AColor );
					}
			}

		}

		static void drawAlphaRGBWColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint16_t AGreen = pgm_read_byte( bitmap ++ );
					uint8_t ABlue = pgm_read_byte( bitmap ++ );
					uint32_t AWhite = pgm_read_byte( bitmap ++ );
					uint64_t AAlpha = pgm_read_byte( bitmap ++ );
					uint64_t AColor = ( AAlpha << 32 ) | (AWhite << 24) | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

					if (size == 1) // default size
						AImplementation.drawPixel( x+i, y+j, AColor );

					else
						T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, AColor );
				}

		}

		static void drawColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint32_t AGreen = pgm_read_byte( bitmap ++ );
					uint32_t ABlue = pgm_read_byte( bitmap ++ );
					uint32_t AColor = 0xFF000000 | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

					if (size == 1) // default size
						T_IMPLEMENTATION::drawColorPixel( AImplementation, x+i, y+j, AColor );

					else
						T_IMPLEMENTATION::fillColorRect( AImplementation, x+i*size, y+j*size, size, size, AColor );
				}

		}

		static void drawColorBitmap_565( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARedGreen = pgm_read_byte( bitmap ++ );
					uint32_t AGreenBlue = pgm_read_byte( bitmap ++ );
					uint32_t ARed = ARedGreen & 0b11111000;
					uint32_t ABlue = 0xFF & ( AGreenBlue << 3 );
					uint32_t AGreen = 0xFF & ( ( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

					uint32_t AColor = 0xFF000000 | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

					if (size == 1) // default size
						T_IMPLEMENTATION::drawColorPixel( AImplementation, x+i, y+j, AColor );

					else
						T_IMPLEMENTATION::fillColorRect( AImplementation, x+i*size, y+j*size, size, size, AColor );
				}

		}

		static void drawRGBWColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint16_t AGreen = pgm_read_byte( bitmap ++ );
					uint8_t ABlue = pgm_read_byte( bitmap ++ );
					uint64_t AWhite = pgm_read_byte( bitmap ++ );
					uint64_t AColor = 0xFF00000000 | ( AWhite << 32 ) | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

					if (size == 1) // default size
						AImplementation.drawPixel( x+i, y+j, AColor );

					else
						T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, AColor );
				}

		}

		static void drawRepeatColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARed = pgm_read_byte( bitmap ++ );
				uint32_t AGreen = pgm_read_byte( bitmap ++ );
				uint32_t ABlue = pgm_read_byte( bitmap ++ );
				uint32_t AColor = 0xFF000000 | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::drawColorPixel( AImplementation, x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillColorRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawRepeatColorBitmap_565( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint32_t AGreenBlue = pgm_read_byte( bitmap ++ );
				uint32_t ARed = ARedGreen & 0b11111000;
				uint32_t ABlue = AGreenBlue << 3;
				uint32_t AGreen = 0xFC & ( ( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

				uint32_t AColor = 0xFF000000 | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::drawColorPixel( AImplementation, x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillColorRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawRepeatRGBWColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARed = pgm_read_byte( bitmap ++ );
				uint16_t AGreen = pgm_read_byte( bitmap ++ );
				uint8_t ABlue = pgm_read_byte( bitmap ++ );
				uint64_t AWhite = pgm_read_byte( bitmap ++ );
				uint64_t AColor = 0xFF00000000 | ( AWhite << 24 ) | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						AImplementation.drawPixel( x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawRepeatAlphaColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint32_t AColor = pgm_read_dword( bitmap );
				bitmap += 4;
				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::drawColorPixel( AImplementation, x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillColorRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawRepeatAlphaColorBitmap_565( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint32_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint32_t AGreenBlue = pgm_read_byte( bitmap ++ );
				uint32_t AAlpha = pgm_read_byte( bitmap ++ );

				uint32_t ARed = ARedGreen & 0b11111000;
				uint32_t ABlue = 0xFF & ( AGreenBlue << 3 );
				uint32_t AGreen = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

				uint32_t AColor = ( AAlpha << 24 ) | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;

				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::drawColorPixel( AImplementation, x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillColorRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawRepeatAlphaRGBWColorBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			int16_t x1 = 0;
			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARed = pgm_read_byte( bitmap ++ );
				uint16_t AGreen = pgm_read_byte( bitmap ++ );
				uint8_t ABlue = pgm_read_byte( bitmap ++ );
				uint32_t AWhite = pgm_read_byte( bitmap ++ );
				uint64_t AAlpha = pgm_read_byte( bitmap ++ );

				uint64_t AColor = ( AAlpha << 32 ) | (AWhite << 24) | ( ARed << 16 ) | ( AGreen << 8 ) | ABlue;
				if (size == 1) // default size
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						AImplementation.drawPixel( x + x1++, y, AColor );

						if( x1 >= w )
						{
							x1 = 0;
							++ y;
						}
					}
				}

				else
				{
					for( int16_t j = 0; j <= ACount; ++ j )
					{
						T_IMPLEMENTATION::fillRect( AImplementation, x + x1 * size, y, size, size, AColor );
						++ x1;

						if( x1 >= w )
						{
							x1 = 0;
							y += size;
						}
					}
				}
			}
		}

		static void drawAlphaGrayscaleBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if (size == 1) // default size
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
						T_IMPLEMENTATION::drawGrayscalePixel( AImplementation, x+i, y+j, pgm_read_word( bitmap ++ ));

			}

			else
			{
				for( int16_t j=0; j<h; j++)
					for( int16_t i=0; i<w; ++i )
						T_IMPLEMENTATION::fillGrayscaleRect( AImplementation, x+i * size, y+j * size, size, size, pgm_read_word( bitmap ++ ));

			}
		}

		// Draw a 1-bit color bitmap at the specified x, y position from the
		// provided bitmap buffer (must be PROGMEM memory) using color as the
		// foreground color and bg as the background color.
		static void drawBWBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, TUniColor color, TUniColor bg, uint8_t size )
		{
			int16_t byteWidth = (w + 7) / 8;

			if (size == 1) // default size
			{
				for( int16_t j=0; j<h; ++j )
					for( int16_t i=0; i<w; ++i )
					{
						if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> ( i & 0b111 )))
							T_IMPLEMENTATION::drawGrayscalePixel( AImplementation, x+i, y+j, color );

						else
      						T_IMPLEMENTATION::drawGrayscalePixel( AImplementation, x+i, y+j, bg );
					}
			}

			else
			{
				for( int16_t j=0; j<h; ++j )
					for( int16_t i=0; i<w; ++i )
					{
						if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> ( i & 0b111 )))
							T_IMPLEMENTATION::fillGrayscaleRect( AImplementation, x+i*size, y+j*size, size, size, color );

						else
							T_IMPLEMENTATION::fillGrayscaleRect( AImplementation, x+i*size, y+j*size, size, size, bg );
					}

			}

		}

		static void drawTransparencyBitmap( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			int16_t byteWidth = (w + 3) / 4;

			for( int16_t j=0; j<h; ++j )
				for( int16_t i=0; i<w; ++i )
				{
					uint8_t AByte = ( pgm_read_byte(bitmap + j * byteWidth + i / 4) >> ((3 - (i & 0b11)) * 2 )) & 0b11;

				if (size == 1) // default size
					switch( AByte )
					{
						case 1 : AImplementation.drawPixel( x+i, y+j, tmcInvert ); break;
						case 2 : AImplementation.drawPixel( x+i, y+j, tmcBlack ); break;
						case 3 : AImplementation.drawPixel( x+i, y+j, tmcWhite ); break;
					}

				else  // big size
					switch( AByte )
					{
						case 1 : T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, tmcInvert ); break;
						case 2 : T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, tmcBlack ); break;
						case 3 : T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, tmcWhite ); break;
					}
				}

		}

		static void drawRoundRect( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, int16_t rx, int16_t ry, TUniColor color )
		{
			if( rx == 0 || ry == 0 )
			{
				T_IMPLEMENTATION::drawRect( AImplementation, x, y, w, h, color );
				return;
			}

			T_IMPLEMENTATION::drawFastHLine( AImplementation, x+rx , y    , w-2 * rx - 1, color ); // Top
			T_IMPLEMENTATION::drawFastHLine( AImplementation, x+rx , y+h-1, w-2 * rx - 1, color ); // Bottom
			T_IMPLEMENTATION::drawFastVLine( AImplementation, x    , y+ry , h-2 * ry, color ); // Left
			T_IMPLEMENTATION::drawFastVLine( AImplementation, x+w-1, y+ry , h-2 * ry, color ); // Right

			T_IMPLEMENTATION::drawEllipseHelper( AImplementation, x+rx,		y+ry,	  rx, ry, 1, color);
			T_IMPLEMENTATION::drawEllipseHelper( AImplementation, x+w-rx-1,	y+ry,	  rx, ry, 2, color);
			T_IMPLEMENTATION::drawEllipseHelper( AImplementation, x+w-rx-1,	y+h-ry-1, rx, ry, 4, color);
			T_IMPLEMENTATION::drawEllipseHelper( AImplementation, x+rx,		y+h-ry-1, rx, ry, 8, color);
		}

		static void fillRoundRect( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, int16_t rx, int16_t ry, TUniColor color )
		{
			if( rx == 0 || ry == 0 )
			{
				T_IMPLEMENTATION::fillRect( AImplementation, x, y, w, h, color );
				return;
			}

			T_IMPLEMENTATION::fillRect( AImplementation, x+rx, y, w-rx*2 - 1, ry, color );
			T_IMPLEMENTATION::fillRect( AImplementation, x+rx, y+h-ry, w-rx*2 - 1, ry, color );
			T_IMPLEMENTATION::fillRect( AImplementation, x, y+ry, w, h-ry * 2, color );
			T_IMPLEMENTATION::fillEllipseHelper( AImplementation, x+rx,		y+ry,	  rx, ry, 1, color);
			T_IMPLEMENTATION::fillEllipseHelper( AImplementation, x+w-rx-1,	y+ry,	  rx, ry, 2, color);
			T_IMPLEMENTATION::fillEllipseHelper( AImplementation, x+w-rx-1,	y+h-ry-1, rx, ry, 4, color);
			T_IMPLEMENTATION::fillEllipseHelper( AImplementation, x+rx,		y+h-ry-1, rx, ry, 8, color);
		}

		// Bresenham's algorithm - thx wikpedia
		static void drawLine( T_IMPLEMENTATION &AImplementation, int16_t x0, int16_t y0, int16_t x1, int16_t y1, TUniColor color)
		{
//			Serial.println( "drawLine" );
			int16_t steep = abs(y1 - y0) > abs(x1 - x0);
			if (steep)
			{
				swap(x0, y0);
				swap(x1, y1);
			}

			if (x0 > x1)
			{
				swap(x0, x1);
				swap(y0, y1);
			}

			int16_t dx, dy;
			dx = x1 - x0;
			dy = abs(y1 - y0);

			int16_t err = dx / 2;
			int16_t ystep;

			if (y0 < y1)
				ystep = 1;

			else
				ystep = -1;

			for (; x0<=x1; x0++)
			{
				if (steep)
					AImplementation.drawPixel(y0, x0, color);

				else
					AImplementation.drawPixel(x0, y0, color);

				err -= dy;
				if (err < 0)
				{
					y0 += ystep;
					err += dx;
				}
			}
		}

		static inline void drawFastVLine( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color)
		{
//			Serial.println( "drawFastVLine" );
			// Update in subclasses if desired!
			T_IMPLEMENTATION::drawLine( AImplementation, x, y, x, y+h-1, color);
		}

		static inline void drawFastHLine( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color)
		{
			// Update in subclasses if desired!
			T_IMPLEMENTATION::drawLine( AImplementation, x, y, x+w-1, y, color );
		}

		static inline TUniColor NonTransparent( TUniColor color )
		{
			return color;
		}

		template <typename T_CONFIG> static size_t write( T_IMPLEMENTATION &AImplementation, T_CONFIG &AConfig, uint8_t c )
		{
			AImplementation.height();
//			return 0;
//			Serial.println( c );
			if(( c != '\n' ) && ( c != '\r' ))
				if( AConfig.FCursorY() + AConfig.GetPixelTextHeight() > AImplementation.height() )
//				if( AConfig.FCursorY() >= AImplementation.height() )
				{
//					Serial.println( "Scroll" );
//                    Serial.println( AConfig.cursor_y );
//                    Serial.println( AConfig.GetPixelTextHeight() );
					int ACorrection = ( AConfig.FCursorY() - AImplementation.height() ) + AConfig.GetPixelTextHeight();
					T_IMPLEMENTATION::Scroll( AImplementation, 0, - ACorrection, AImplementation.NonTransparent( AImplementation.GetBackgroundColor().GetValue() ) );
					AConfig.FCursorY() = AConfig.FCursorY() - ACorrection;
//                    Serial.println( AConfig.cursor_y );
				}

			if (c == '\n')
			{
				AConfig.FCursorY() = AConfig.FCursorY() + AConfig.GetPixelTextHeight();
				AConfig.FCursorX()  = 0;
			}

			else if (c == '\r')
				AConfig.FCursorX()  = 0;

			else
			{
//				Serial.println( c );
//				Serial.println( AImplementation.cursor_x );
				AConfig.FCursorX() = AConfig.FCursorX() + T_CONFIG::DrawChar( AImplementation, AConfig.FCursorX(), AConfig.FCursorY(), c, AConfig.GetTextColor(), AConfig.GetTextBackgroundColor(), AConfig.GetTextSize(), AConfig.GetCodePage437() );
//				AConfig.cursor_x += AConfig.GetPixelTextWidth();
				if ( AConfig.GetTextWrap() && ( AConfig.FCursorX() > ( AImplementation.width() - AConfig.GetPixelTextWidth() )))
				{
					AConfig.FCursorY() = AConfig.FCursorY() + AConfig.GetPixelTextHeight();
					AConfig.FCursorX() = 0;
				}
			}

			return 1;
		}

/*
		// Draw a character
		static void DrawChar( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
//		  Serial.println( "drawChar" );
//		  Serial.println( AImplementation.width() );
//		  Serial.println( x );
//		  Serial.println( y );
		  if((x >= AImplementation.width() )            || // Clip right
			 (y >= AImplementation.height() )           || // Clip bottom
			 ((x + 6 * size - 1) < 0) || // Clip left
			 ((y + 8 * size - 1) < 0))   // Clip top
			return;

//		  Serial.println( c );

		  if(! AUseCp437 && (c >= 176))
			  c++; // Handle 'classic' charset behavior

		  for (int8_t i=0; i<6; i++ )
		  {
			uint8_t line;
			if (i == 5)
			  line = 0x0;

			else
			  line = pgm_read_byte( AImplementation.FFont + 3 + (c*5)+i); // For now ifnore size information

			for (int8_t j = 0; j<8; j++)
			{
			  if (line & 0x1)
			  {
				if (size == 1) // default size
				  AImplementation.drawPixel(x+i, y+j, color);

				else  // big size
				  T_IMPLEMENTATION::fillRect( AImplementation, x+(i*size), y+(j*size), size, size, color );

			  }

			  else if (bg != color)
			  {
				if (size == 1) // default size
				  AImplementation.drawPixel(x+i, y+j, bg);

				else  // big size
				  T_IMPLEMENTATION::fillRect( AImplementation, x+i*size, y+j*size, size, size, bg );

			  }

			  line >>= 1;
			}
		  }
		}
*/
/*
		void GetCursor( int16_t &x, int16_t &y ) const
		{
			x = cursor_x;
			y = cursor_y;
		}

		inline void setTextSize(uint8_t s)
		{
			textsize = (s > 0) ? s : 1;
		}

		inline uint8_t getTextSize()
		{
			return textsize;
		}

		void setTextColor( TUniColor c, TUniColor b)
		{
			textcolor   = c;
			textbgcolor = b;
		}

		inline void setTextWrap(bool w)
		{
			wrap = w;
		}

		inline void SetCp437(bool x)
		{
			_cp437 = x;
		}

		inline bool GetCp437()
		{
			return _cp437;
		}
*/
	public:
		// Return the size of the display (per current rotation)
//		virtual int16_t width(void) const = 0;
//		virtual int16_t height(void) const = 0;

/*
	public:
		Graphics( const unsigned char * AFont ) :
			FFont( AFont )
		{
		}
*/
	};
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_OWNER> class GraphicsTextElementFont
	{
	protected:
		const unsigned char *FFont;

	protected:
		const unsigned char *FOldFont;

	public:
		void EnterLeave( bool AEnter )
		{
			if( AEnter )
			{
				FOldFont = C_OWNER.getFont();
				C_OWNER.setFont( FFont );
			}

			else
			{
				C_OWNER.setFont( FOldFont );
			}
		}

	public:
		GraphicsTextElementFont( const unsigned char *AFont ) :
			FFont( AFont )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template <typename T_DELEGATOR, typename T_OWNER, T_OWNER &C_OWNER> class GraphicsDelegate // : public GraphicsIntf<T_OWNER>
	{
	public:
		template <typename T> static inline void PrintChar( T AValue )
		{
			C_OWNER.PrintChar( AValue );
		}

	public:
		template <typename T> static inline void Print( T AValue )
		{
			C_OWNER.Print( AValue );
		}

	public:
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
			return C_OWNER.print( AConfig, AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			return C_OWNER.println( AConfig, AValue );
		}

		template <typename T> static inline size_t print( T AValue )
		{
			return C_OWNER.print( AValue );
		}

		template <typename T> static inline size_t println( T AValue )
		{
			return C_OWNER.println( AValue );
		}

	public:
		inline static void Scroll( T_DELEGATOR &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			T_OWNER::Scroll( C_OWNER, X, Y, color );
		}

		inline static void drawGrayscalePixel( T_DELEGATOR &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			T_OWNER::drawGrayscalePixel( C_OWNER, x, y, color );
		}

		inline static void drawColorPixel( T_DELEGATOR &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			T_OWNER::drawColorPixel( C_OWNER, x, y, color );
		}

		inline static void fillGrayscaleRect( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_OWNER::fillGrayscaleRect( C_OWNER, x, y, w, h, color );
		}

		inline static void fillColorRect( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_OWNER::fillColorRect( C_OWNER, x, y, w, h, color );
		}

	public:
		inline static void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			C_OWNER.drawPixel( x, y, color );
		}

		inline static TUniColor GetBackgroundColor()
		{
			return C_OWNER.GetBackgroundColor().GetValue();
		}

	public:
//		inline static void setFont( const unsigned char *AFont )
//		{
//			C_OWNER.setFont( AFont );
//		}

//		inline const unsigned char *getFont()
//		{
//			return C_OWNER.getFont();
//		}

/*
		inline static void getTextColor( TUniColor &AColor, TUniColor &ABackgroundColor )
		{
			C_OWNER.getTextColor( AColor, ABackgroundColor );
		}
*/
		inline static void drawEllipse( T_DELEGATOR &AImplementation, int16_t xc, int16_t yc, int16_t rx, int16_t ry, TUniColor color, bool ACorrectX, bool ACorrectY )
		{
			T_OWNER::drawEllipse( C_OWNER, xc, yc, rx, ry, color, ACorrectX, ACorrectY );
		}

		inline static void drawEllipseWH( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_OWNER::drawEllipseWH( C_OWNER, x, y, w, h, color );
		}

		inline static void fillEllipse( T_DELEGATOR &AImplementation, int16_t ax, int16_t ay, int16_t rx, int16_t ry, TUniColor color, bool ACorrectX, bool ACorrectY )
		{
			T_OWNER::fillEllipse( C_OWNER, ax, ay, rx, ry, color, ACorrectX, ACorrectY );
		}

		inline static void fillEllipseWH( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_OWNER::fillEllipseWH( C_OWNER, x, y, w, h, color );
		}

		inline static void ClearScreen( T_DELEGATOR &AImplementation, TUniColor color )
		{
			T_OWNER::ClearScreen( C_OWNER, color );
		}

		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return C_OWNER.GetPixelColor( x, y );
		}

		inline bool getTextWrap()
		{
			return C_OWNER.getTextWrap();
		}

		static inline void drawRect( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			T_OWNER::drawRect( C_OWNER, x, y, w, h, color );
		}

		static inline void fillRect( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			T_OWNER::fillRect( C_OWNER, x, y, w, h, color );
		}

		static inline void fillScreen( T_DELEGATOR &AImplementation, TUniColor color )
		{
			T_OWNER::fillScreen( C_OWNER, color );
		}

		static inline void drawGrayscaleBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			T_OWNER::drawGrayscaleBitmap( C_OWNER, x, y, bitmap, w, h, AAlpha, size );
		}

		static inline void drawRepeatGrayscaleBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			T_OWNER::drawRepeatGrayscaleBitmap( C_OWNER, x, y, bitmap, AListCount, w, h, AAlpha, size );
		}

		static inline void drawAlphaGrayscaleRepeatBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawAlphaGrayscaleRepeatBitmap( C_OWNER, x, y, bitmap, AListCount, w, h, size );
		}

		static inline void drawAlphaColorBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawAlphaColorBitmap( C_OWNER, x, y, bitmap, w, h, size );
		}

		static inline void drawAlphaColorBitmap_565( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawAlphaColorBitmap_565( C_OWNER, x, y, bitmap, w, h, size );
		}

		static inline void drawAlphaRGBWColorBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawAlphaRGBWColorBitmap( C_OWNER, x, y, bitmap, w, h, size );
		}

		static inline void drawColorBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawColorBitmap( C_OWNER, x, y, bitmap, w, h, size );
		}

		static inline void drawColorBitmap_565( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawColorBitmap_565( C_OWNER, x, y, bitmap, w, h, size );
		}

		static inline void drawRGBWColorBitmap_565( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawRGBWColorBitmap_565( C_OWNER, x, y, bitmap, w, h, size );
		}

		static inline void drawRepeatColorBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawRepeatColorBitmap( C_OWNER, x, y, bitmap, AListCount, w, h, size );
		}

		static inline void drawRepeatColorBitmap_565( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawRepeatColorBitmap_565( C_OWNER, x, y, bitmap, AListCount, w, h, size );
		}

		static inline void drawRepeatRGBWColorBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawRepeatRGBWColorBitmap( C_OWNER, x, y, bitmap, AListCount, w, h, size );
		}

		static inline void drawRepeatAlphaColorBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawRepeatAlphaColorBitmap( C_OWNER, x, y, bitmap, AListCount, w, h, size );
		}

		static inline void drawRepeatAlphaColorBitmap_565( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawRepeatAlphaColorBitmap_565( C_OWNER, x, y, bitmap, AListCount, w, h, size );
		}

		static inline void drawRepeatAlphaRGBWColorBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawRepeatAlphaRGBWColorBitmap( C_OWNER, x, y, bitmap, AListCount, w, h, size );
		}

		static inline void drawAlphaGrayscaleBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawAlphaGrayscaleBitmap( C_OWNER, x, y, bitmap, w, h, size );
		}

		// Draw a 1-bit color bitmap at the specified x, y position from the
		// provided bitmap buffer (must be PROGMEM memory) using color as the
		// foreground color and bg as the background color.
		static inline void drawBWBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, TUniColor color, TUniColor bg, uint8_t size )
		{
			T_OWNER::drawBWBitmap( C_OWNER, x, y, bitmap, w, h, color, bg, size );
		}

		static inline void drawTransparencyBitmap( T_DELEGATOR &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			T_OWNER::drawTransparencyBitmap( C_OWNER, x, y, bitmap, w, h, size );
		}

		static inline void drawRoundRect( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, int16_t rx, int16_t ry, TUniColor color )
		{
			T_OWNER::drawRoundRect( C_OWNER, x, y, w, h, rx, ry, color );
		}

		static inline void fillRoundRect( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, int16_t rx, int16_t ry, TUniColor color )
		{
			T_OWNER::fillRoundRect( C_OWNER, x, y, w, h, rx, ry, color );
		}

		// Bresenham's algorithm - thx wikpedia
		static inline void drawLine( T_DELEGATOR &AImplementation, int16_t x0, int16_t y0, int16_t x1, int16_t y1, TUniColor color )
		{
			T_OWNER::drawLine( C_OWNER, x0, y0, x1, y1, color );
		}

		static inline void drawFastVLine( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			T_OWNER::drawFastVLine( C_OWNER, x, y, h, color );
		}

		static inline void drawFastHLine( T_DELEGATOR &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			T_OWNER::drawFastHLine( C_OWNER, x, y, w, color );
		}

		static inline size_t write( T_DELEGATOR &AImplementation, uint8_t c )
		{
			return T_OWNER::write( C_OWNER, c );
		}

		// Draw a character
		template <typename T_SELF_DELEGATOR> static inline uint8_t DrawChar( T_SELF_DELEGATOR &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
//			Serial.println( "drawChar" );
			return T_OWNER::DrawChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

		inline void SetCursor( int16_t x, int16_t y )
		{
			C_OWNER.SetCursor( x, y );
		}

		inline void GetCursor( int16_t &x, int16_t &y ) const
		{
			C_OWNER.GetCursor( x, y );
		}

/*
		inline void setTextSize( uint8_t s )
		{
			C_OWNER.setTextSize( s );
		}

		inline uint8_t getTextSize()
		{
			return C_OWNER.getTextSize();
		}

		inline void setTextColor( TUniColor c, TUniColor b )
		{
			C_OWNER.setTextColor( c, b );
		}
*/
		inline void setTextWrap(bool w )
		{
			C_OWNER.setTextWrap( w );
		}

		inline void SetCp437(bool x)
		{
			C_OWNER.SetCp437( x );
		}

		inline bool GetCp437()
		{
			return C_OWNER.GetCp437();
		}

	public:
		// Return the size of the display (per current rotation)
		inline static int16_t width()
		{
			return C_OWNER.width();
		}

		inline static int16_t height()
		{
			return C_OWNER.height();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		typename T_BackgroundColor,
		typename T_Text
	> class GraphicsImpl :
		public Graphics<T_OWNER>,
		public GraphicsIntf<T_OWNER>,
		public T_BackgroundColor,
		public T_Text
	{
		typedef Graphics<T_OWNER> inherited;

	public:
		_V_PROP_( BackgroundColor )
		_V_PROP_( Text )

	public:
		inline T_BackgroundColor &GetBackgroundColor()
		{
			return BackgroundColor();
		}

		inline uint8_t GetTextSize() { return Text().Size(); }
		inline uint8_t GetPixelTextHeight() { return Text().Size() * T_Text::TYPE_T_FONT::GetPixelTextFontHeight(); }
		inline uint8_t GetPixelTextWidth() { return Text().Size() * T_Text::TYPE_T_FONT::GetPixelTextFontWidth(); }
		inline TUniColor GetTextColor() { return Text().Color().GetValue(); }
		inline TUniColor GetTextBackgroundColor() { return Text().BackgroundColor().GetValue(); }
		inline bool GetTextWrap() { return Text().Wrap(); }
		inline bool GetCodePage437() { return Text().UseCodePage437Symbols(); }

	public:
		template <typename T_IMPLEMENTATION> static uint8_t DrawChar( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			return T_Text::TYPE_T_FONT::DrawFontChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_X,
		typename T_Y
	> class GraphicsElementSetCursor :
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				C_OWNER.SetCursor( X(), Y() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockOutputPin,
		typename T_Enabled
	> class GraphicsElementRenderEvent :
		public T_ClockOutputPin,
		public T_Enabled
	{
	public:
		_V_PIN_( ClockOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				T_ClockOutputPin::ClockPin();

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_Render,
		typename T_Enabled,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawScene :
		public GraphicsDelegate<
				GraphicsElementDrawScene<
					T_OWNER, C_OWNER,
					T_Elements_Render,
					T_Enabled,
					T_X,
					T_Y
				>, T_OWNER, C_OWNER
			>,
		public GraphicsIntf<T_OWNER>,
		public T_Enabled,
		public T_X,
		public T_Y
	{
		typedef GraphicsIntf<T_OWNER> inherited;

    public:
		typedef typename T_OWNER::TYPE_T_FONT TYPE_T_FONT;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

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

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			AX = AParentX + X();
			AY = AParentY + Y();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				T_Elements_Render::Call();

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
		typename T_OPERATIONS
	> class GraphicsElementFillScreen :
		public T_Color,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				T_OWNER::ClearScreen( C_OWNER, Color().GetValue() );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
		typename T_OPERATIONS,
		typename T_X,
		typename T_Y
	> class GraphicsElementScroll :
		public T_Color,
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				T_OWNER::Scroll( C_OWNER, X(), Y(), Color().GetValue() );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
		typename T_OPERATIONS,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawPixel :
		public T_Color,
		public T_Enabled,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

//			Serial.println( "TEST3" );
//			Serial.println( Color().GetValue(), HEX );
			if( T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

//			Serial.println( "TEST4" );
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			C_OWNER.drawPixel( AParentX + X(), AParentY + Y(), Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_Color,
		typename T_Enabled,
		typename T_FillColor,
		typename T_Height,
		typename T_OPERATIONS,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawRectangle :
		public T_Centered,
		public T_Color,
		public T_Enabled,
		public T_FillColor,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( Enabled )
		_V_PROP_( FillColor )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

//			if( !( ( FillColor & 0xFF00 ) || ( Color & 0xFF00 )))
			if( T_OPERATIONS::IsTransparent( FillColor().GetValue() ) && T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= Width() / 2;
				AParentY -= Height() / 2;
			}

//			if( FillColor & 0xFF00 )
			if( ! T_OPERATIONS::IsTransparent( FillColor().GetValue() ))
				T_OWNER::fillRect( C_OWNER, AParentX + X() + 1, AParentY + Y() + 1, Width() - 2, Height() - 2, FillColor().GetValue() );

//			if( Color & 0xFF00 )
			if( ! T_OPERATIONS::IsTransparent( Color().GetValue() ))
				T_OWNER::drawRect( C_OWNER, AParentX + X(), AParentY + Y(), Width(), Height(), Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_Color,
		typename T_CornerSize,
		typename T_Enabled,
		typename T_FillColor,
		typename T_Height,
		typename T_OPERATIONS,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawRoundRectangle :
		public T_Centered,
		public T_Color,
		public T_CornerSize,
		public T_Enabled,
		public T_FillColor,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( Enabled )
		_V_PROP_( FillColor )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( CornerSize )

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

//			if( !( ( FillColor & 0xFF00000000 ) || ( Color & 0xFF00000000 )))
			if( T_OPERATIONS::IsTransparent( FillColor().GetValue() ) && T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= Width() / 2;
				AParentY -= Height() / 2;
			}

//			if( FillColor & 0xFF00000000 )
			if( ! T_OPERATIONS::IsTransparent( FillColor().GetValue() ))
				T_OWNER::fillRoundRect( C_OWNER, AParentX + X(), AParentY + Y(), Width(), Height(), CornerSize().X(), CornerSize().Y(), FillColor().GetValue() );

//			if( Color & 0xFF00000000 )
			if( ! T_OPERATIONS::IsTransparent( Color().GetValue() ))
				T_OWNER::drawRoundRect( C_OWNER, AParentX + X(), AParentY + Y(), Width(), Height(), CornerSize().X(), CornerSize().Y(), Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_Color,
		typename T_Enabled,
		typename T_FillColor,
		typename T_Height,
		typename T_OPERATIONS,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawEllipse :
		public T_Centered,
		public T_Color,
		public T_Enabled,
		public T_FillColor,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( FillColor )
		_V_PROP_( Enabled )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

//			if( !( ( FillColor & 0xFF000000 ) || ( Color & 0xFF000000 )))
			if( T_OPERATIONS::IsTransparent( FillColor().GetValue() ) && T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= Width() / 2;
				AParentY -= Height() / 2;
			}

//			Serial.println( "Render" );

//			if( FillColor & 0xFF000000 )
			if( ! T_OPERATIONS::IsTransparent( FillColor().GetValue() ))
				T_OWNER::fillEllipseWH( C_OWNER, AParentX + X(), AParentY + Y(), Width(), Height(), FillColor().GetValue() );

//			if( Color & 0xFF000000 )
			if( ! T_OPERATIONS::IsTransparent( Color().GetValue() ))
				T_OWNER::drawEllipseWH( C_OWNER, AParentX + X(), AParentY + Y(), Width(), Height(), Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
		typename T_Height,
		typename T_OPERATIONS,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawLine :
		public T_Color,
		public T_Enabled,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

//			if( ! ( Color & 0xFF00 ))
//				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			T_OWNER::drawLine( C_OWNER, AParentX + X(), AParentY + Y(), AParentX + X() + Width(), AParentY + Y() + Height(), Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Angle,
		typename T_Begin,
		typename T_Color,
		typename T_Enabled,
		typename T_End,
		typename T_OPERATIONS,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawAngledLine :
		public T_Angle,
		public T_Begin,
		public T_Color,
		public T_Enabled,
		public T_End,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Begin )
		_V_PROP_( End )
		_V_PROP_( Angle )

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			float ARadians = Angle() * PI / 180.0f;
			T_OWNER::drawLine( C_OWNER, AParentX + X() + Begin() * cos( ARadians ), AParentY + Y() + Begin() * sin( ARadians ), AParentX + X() + End() * cos( ARadians ), AParentY + Y() + End() * sin( ARadians ), Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
		typename T_OPERATIONS,
		typename T_Points,
        typename T_Points_GetValue,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawLines :
		public T_Color,
		public T_Enabled,
		public T_Points,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )

		inline GraphicsPoint *Points() { return T_Points::GetInstance(); }

	public:
		inline void SetIndexedValue( uint32_t AIndex )
        {
//            Render();
        }

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_Points::GetCount() == 0 )
				return;

			if( T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			TGraphicsPos	AX1 = AParentX + X();
			TGraphicsPos	AY1 = AParentY + Y();

			if( T_Points::GetIsConstant() )
			{
//				const PROGMEM
				const Mitov::GraphicsPoint *APoints = T_Points::GetValue();

				for( uint32_t i = 0; i < T_Points::GetCount(); ++i )
				{
#ifdef __32BIT_GRAPHICS_COORDINATES__
					TGraphicsPos	AX2 = pgm_read_dword( APoints + i ) + AParentX + X();
					TGraphicsPos	AY2 = pgm_read_dword( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + AParentY + Y();
#else
					TGraphicsPos	AX2 = pgm_read_word( APoints + i ) + AParentX + X();
					TGraphicsPos	AY2 = pgm_read_word( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + AParentY + Y();
#endif
                    T_Points_GetValue::Call( i, AX2, AY2 );
					T_OWNER::drawLine( C_OWNER, AX1, AY1, AX2, AY2, Color().GetValue() );

					AX1 = AX2;
					AY1 = AY2;
				}
			}

			else
			{
				const Mitov::GraphicsPoint *APoints = T_Points::GetValue();

				for( uint32_t i = 0; i < T_Points::GetCount(); ++i )
				{
					TGraphicsPos	AX2 = APoints[ i ].X + AParentX + X();
					TGraphicsPos	AY2 = APoints[ i ].Y + AParentY + Y();

					T_OWNER::drawLine( C_OWNER, AX1, AY1, AX2, AY2, Color().GetValue() );

					AX1 = AX2;
					AY1 = AY2;
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
		typename T_FillColor,
		typename T_OPERATIONS,
		typename T_Points,
        typename T_Points_GetValue,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawPolygon :
		public T_Color,
		public T_Enabled,
		public T_FillColor,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( Enabled )
		_V_PROP_( FillColor )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		static constexpr bool GetPointsIsConstant() { return T_Points::GetIsConstant(); }
		static constexpr uint32_t GetPointsCount() { return T_Points::GetCount(); }
		static constexpr const typename T_Points::T_TYPE *GetPointsValue() { return T_Points::GetValue(); }

	public:
		inline void SetIndexedValue( uint32_t AIndex )
        {
//            Render();
        }

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_Points::GetCount() == 0 )
				return;

//			if( !( ( FillColor & 0xFF000000 ) || ( Color & 0xFF000000 )))
			if( T_OPERATIONS::IsTransparent( FillColor().GetValue() ) && T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			TGraphicsPos	AStartX = AParentX + X();
			TGraphicsPos	AStartY = AParentY + Y();

			if( C_OWNER.ImplementsPolygon())
			{
				if( ! T_OPERATIONS::IsTransparent( FillColor().GetValue() ))
					C_OWNER.RenderPolygon( *this, AStartX, AStartY, true, FillColor().GetValue() );

				if( ! T_OPERATIONS::IsTransparent( Color().GetValue() ))
					C_OWNER.RenderPolygon( *this, AStartX, AStartY, false, Color().GetValue() );

				return;
			}

			if( T_Points::GetIsConstant() )
			{
//				const PROGMEM
				const Mitov::GraphicsPoint *APoints = T_Points::GetValue();

	//			if( FillColor & 0xFF000000 )
				if( ! T_OPERATIONS::IsTransparent( FillColor().GetValue() ))
				{
					TGraphicsPos	AMinX = AStartX;
					TGraphicsPos	AMinY = AStartY;
					TGraphicsPos	AMaxX = AStartX;
					TGraphicsPos	AMaxY = AStartY;

					for( uint32_t i = 0; i < T_Points::GetCount(); ++i )
					{
#ifdef __32BIT_GRAPHICS_COORDINATES__
						TGraphicsPos	AX2 = pgm_read_dword( APoints + i ) + AStartX;
						TGraphicsPos	AY2 = pgm_read_dword( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + AStartY;
#else
						TGraphicsPos	AX2 = pgm_read_word( APoints + i ) + AStartX;
						TGraphicsPos	AY2 = pgm_read_word( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + AStartY;
#endif
//						TGraphicsPos	AX2 = APoints[ i ].X + AStartX;
//						TGraphicsPos	AY2 = APoints[ i ].Y + AStartY;

                        T_Points_GetValue::Call( i, AX2, AY2 );

						if( AX2 < AMinX )
							AMinX = AX2;

						if( AX2 > AMaxX )
							AMaxX = AX2;

						if( AY2 < AMinY )
							AMinY = AY2;

						if( AY2 > AMaxY )
							AMaxY = AY2;

					}

					if( AMinX < 0 )
					 AMinX = 0;

					if( AMinY < 0 )
					 AMinY = 0;

					TGraphicsPos	AWidth = C_OWNER.width();
					TGraphicsPos	AHeight = C_OWNER.height();

					if( AMaxX > AWidth )
					 AMaxX = AWidth;

					if( AMaxY > AHeight )
					 AMaxY = AHeight;

					if( AMinX > AMaxX )
						return;

					if( AMinY > AMaxY )
						return;

					TGraphicsPos *nodeX = new TGraphicsPos[ T_Points::GetCount() ];
					for( int pixelY = AMinY; pixelY < AMaxY; ++ pixelY )
					{
						//  Build a list of nodes.
						int nodes = 0;
						int32_t j = int32_t( T_Points::GetCount()) - 1;
						for( int32_t i = 0; i < int32_t( T_Points::GetCount()); i++ )
						{
#ifdef __32BIT_GRAPHICS_COORDINATES__
							TGraphicsPos	AX_I = pgm_read_dword( APoints + i );
							TGraphicsPos	AY_I = pgm_read_dword( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) );

							TGraphicsPos	AX_J = pgm_read_dword( APoints + j );
							TGraphicsPos	AY_J = pgm_read_dword( ((uint8_t *)( APoints + j ) ) + sizeof( TGraphicsPos ) );
#else
							TGraphicsPos	AX_I = pgm_read_word( APoints + i );
							TGraphicsPos	AY_I = pgm_read_word( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) );

							TGraphicsPos	AX_J = pgm_read_word( APoints + j );
							TGraphicsPos	AY_J = pgm_read_word( ((uint8_t *)( APoints + j ) ) + sizeof( TGraphicsPos ) );
#endif
//							TGraphicsPos AX_I = APoints[ i ].X;
//							TGraphicsPos AX_J = APoints[ J ].X;

//							TGraphicsPos AY_I = APoints[ i ].Y;
//							TGraphicsPos AY_J = APoints[ J ].Y;

                            T_Points_GetValue::Call( i, AX_I, AY_I );
                            T_Points_GetValue::Call( j, AX_J, AY_J );
							if ( (( AY_I + AStartY ) < pixelY && ( AY_J + AStartY ) >= pixelY ) || (( AY_J + AStartY ) < pixelY && ( AY_I + AStartY ) >= pixelY ))
								nodeX[nodes ++] = ( AX_I + AStartX ) + float( pixelY - ( AY_I + AStartY ) ) / ( AY_J - AY_I ) * float( (AX_J - AX_I ) ) + 0.5;

							j = i;
						}

						//  Sort the nodes, via a simple “Bubble” sort.
						int i = 0;
						while( i < nodes - 1 )
						{
							if( nodeX[ i ] > nodeX[ i + 1 ] )
							{
								TGraphicsPos swap = nodeX[ i ];
								nodeX[ i ] = nodeX[ i + 1 ];
								nodeX[ i + 1 ] = swap;
								if( i )
									i--;
							}

							else
								i++;

						}

						//  Fill the pixels between node pairs.
						for( int i = 0; i < nodes; i += 2 )
						{
							TGraphicsPos ALeft = nodeX[ i ];
							TGraphicsPos ARight = nodeX[ i + 1 ];

							if( ALeft >= AWidth )
								break;

							if( ARight > 0 )
							{
								if( ALeft < 0 )
									ALeft = 0;

								if( ARight > AWidth )
									ARight = AWidth;

								T_OWNER::drawFastHLine( C_OWNER, ALeft, pixelY, ARight - ALeft, FillColor().GetValue() );

		//					for (pixelX=nodeX[i]; pixelX<nodeX[i+1]; pixelX++) fillPixel(pixelX,pixelY);
							}
						}

					}

				}

	//			if( Color & 0xFF000000 )
				if( ! T_OPERATIONS::IsTransparent( Color().GetValue() ))
				{
					TGraphicsPos	AX1 = X();
					TGraphicsPos	AY1 = Y();

					for( uint32_t i = 1; i < T_Points::GetCount(); ++i )
					{
#ifdef __32BIT_GRAPHICS_COORDINATES__
						TGraphicsPos	AX2 = pgm_read_dword( APoints + i ) + AStartX;
						TGraphicsPos	AY2 = pgm_read_dword( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + X();
#else
						TGraphicsPos	AX2 = pgm_read_word( APoints + i ) + AStartX;
						TGraphicsPos	AY2 = pgm_read_word( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + Y();
#endif
//						TGraphicsPos	AX2 = APoints[ i ].X + X();
//						TGraphicsPos	AY2 = APoints[ i ].Y + Y();

						T_OWNER::drawLine( C_OWNER, AParentX + AX1, AParentY + AY1, AParentX + AX2, AParentY + AY2, Color().GetValue() );

						AX1 = AX2;
						AY1 = AY2;
					}

					T_OWNER::drawLine( C_OWNER, AParentX + AX1, AParentY + AY1, AParentX + X(), AParentY + Y(), Color().GetValue() );
				}
			}

			else
			{
				const Mitov::GraphicsPoint *APoints = T_Points::GetValue();

	//			if( FillColor & 0xFF000000 )
				if( ! T_OPERATIONS::IsTransparent( FillColor().GetValue() ))
				{
					TGraphicsPos	AMinX = AStartX;
					TGraphicsPos	AMinY = AStartY;
					TGraphicsPos	AMaxX = AStartX;
					TGraphicsPos	AMaxY = AStartY;

					for( uint32_t i = 0; i < T_Points::GetCount(); ++i )
					{
						TGraphicsPos	AX2 = APoints[ i ].X + AStartX;
						TGraphicsPos	AY2 = APoints[ i ].Y + AStartY;

						if( AX2 < AMinX )
							AMinX = AX2;

						if( AX2 > AMaxX )
							AMaxX = AX2;

						if( AY2 < AMinY )
							AMinY = AY2;

						if( AY2 > AMaxY )
							AMaxY = AY2;

					}

					if( AMinX < 0 )
					 AMinX = 0;

					if( AMinY < 0 )
					 AMinY = 0;

					TGraphicsPos	AWidth = C_OWNER.width();
					TGraphicsPos	AHeight = C_OWNER.height();

					if( AMaxX > AWidth )
					 AMaxX = AWidth;

					if( AMaxY > AHeight )
					 AMaxY = AHeight;

					if( AMinX > AMaxX )
						return;

					if( AMinY > AMaxY )
						return;

					TGraphicsPos *nodeX = new TGraphicsPos[ T_Points::GetCount() ];
					for( int pixelY = AMinY; pixelY < AMaxY; ++ pixelY )
					{
						//  Build a list of nodes.
						int nodes = 0;
						int32_t j = int32_t( T_Points::GetCount()) - 1;
						for( int32_t i = 0; i < int32_t( T_Points::GetCount()); i++ )
						{
							if ( (( APoints[ i ].Y + AStartY ) < pixelY && ( APoints[ j ].Y + AStartY ) >= pixelY ) || (( APoints[ j ].Y + AStartY ) < pixelY && ( APoints[ i ].Y + AStartY ) >= pixelY ))
								nodeX[nodes ++] = ( APoints[i].X + AStartX ) + float( pixelY - ( APoints[i].Y + AStartY ) ) / ( APoints[j].Y - APoints[i].Y ) * float( (APoints[j].X - APoints[i].X ) ) + 0.5;

							j = i;
						}

						//  Sort the nodes, via a simple “Bubble” sort.
						int i = 0;
						while( i < nodes - 1 )
						{
							if( nodeX[ i ] > nodeX[ i + 1 ] )
							{
								TGraphicsPos swap = nodeX[ i ];
								nodeX[ i ] = nodeX[ i + 1 ];
								nodeX[ i + 1 ] = swap;
								if( i )
									i--;
							}

							else
								i++;

						}

						//  Fill the pixels between node pairs.
						for( int i = 0; i < nodes; i += 2 )
						{
							TGraphicsPos ALeft = nodeX[ i ];
							TGraphicsPos ARight = nodeX[ i + 1 ];

							if( ALeft >= AWidth )
								break;

							if( ARight > 0 )
							{
								if( ALeft < 0 )
									ALeft = 0;

								if( ARight > AWidth )
									ARight = AWidth;

								T_OWNER::drawFastHLine( C_OWNER, ALeft, pixelY, ARight - ALeft, FillColor().GetValue() );

		//					for (pixelX=nodeX[i]; pixelX<nodeX[i+1]; pixelX++) fillPixel(pixelX,pixelY);
							}
						}

					}

				}

	//			if( Color & 0xFF000000 )
				if( ! T_OPERATIONS::IsTransparent( Color().GetValue() ))
				{
					TGraphicsPos	AX1 = X();
					TGraphicsPos	AY1 = Y();

					for( uint32_t i = 1; i < T_Points::GetCount(); ++i )
					{
						TGraphicsPos	AX2 = APoints[ i ].X + X();
						TGraphicsPos	AY2 = APoints[ i ].Y + Y();

						T_OWNER::drawLine( C_OWNER, AParentX + AX1, AParentY + AY1, AParentX + AX2, AParentY + AY2, Color().GetValue() );

						AX1 = AX2;
						AY1 = AY2;
					}

					T_OWNER::drawLine( C_OWNER, AParentX + AX1, AParentY + AY1, AParentX + X(), AParentY + Y(), Color().GetValue() );
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
//		typename T_Elements_EnterLeave,
		typename T_FillColor,
		typename T_Font,
		typename T_OPERATIONS,
		typename T_Size,
		typename T_Text,
		typename T_UseCodePage437Symbols,
		typename T_Wrap,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawText :
		public GraphicsDelegate<
			GraphicsElementDrawText<
				T_OWNER, C_OWNER,
				T_Color,
				T_Enabled,
//				T_Elements_EnterLeave,
				T_FillColor,
				T_Font,
				T_OPERATIONS,
				T_Size,
				T_Text,
				T_UseCodePage437Symbols,
				T_Wrap,
				T_X,
				T_Y
			>, T_OWNER, C_OWNER>,
		public T_Color,
		public T_Enabled,
		public T_FillColor,
		public T_Size,
		public T_Text,
		public T_UseCodePage437Symbols,
		public T_Wrap,
		public T_X,
		public T_Y
	{
		typedef GraphicsElementDrawText <
				T_OWNER, C_OWNER,
				T_Color,
				T_Enabled,
//				T_Elements_EnterLeave,
				T_FillColor,
				T_Font,
				T_OPERATIONS,
				T_Size,
				T_Text,
				T_UseCodePage437Symbols,
				T_Wrap,
				T_X,
				T_Y
			> T_SELF;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )
		_V_PROP_( FillColor )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Size )
		_V_PROP_( UseCodePage437Symbols )
		_V_PROP_( Wrap )
		_V_PROP_( Text )

	public:
		inline uint8_t GetTextSize() { return Size(); }
		inline uint8_t GetPixelTextHeight()
		{
			if( T_Font::IsImplemented() )
				return Size() * T_Font::GetPixelTextFontHeight();

			return Size() * T_OWNER::TYPE_T_FONT::GetPixelTextFontHeight();
		}

		inline uint8_t GetPixelTextWidth()
		{
			if( T_Font::IsImplemented() )
				return Size() * T_Font::GetPixelTextFontWidth();

			return Size() * T_OWNER::TYPE_T_FONT::GetPixelTextFontWidth();
		}

		inline TUniColor GetTextColor() { return Color().GetValue(); }
		inline TUniColor GetTextBackgroundColor() { return FillColor().GetValue(); }
		inline bool GetTextWrap() { return Wrap(); }
		inline bool GetCodePage437() { return UseCodePage437Symbols(); }

	public:
		template <typename T_IMPLEMENTATION> static uint8_t DrawChar( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			if( T_Font::IsImplemented() )
				return T_Font::DrawFontChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );

			return T_OWNER::DrawChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_OPERATIONS::IsTransparent( FillColor().GetValue() ) && T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			int16_t ACursorY;
			int16_t ACursorX;

//			C_OWNER.getCursor( ACursorX, ACursorY );
//			TUniColor AOldColor;
//			TUniColor AOldBackgroundColor;

//			uint8_t	AOldSize = C_OWNER.getTextSize();
//			bool AOldWrap = C_OWNER.getTextWrap();

//			T_Elements_EnterLeave::Call( true ); // Enter/Leave

//			C_OWNER.setTextSize( Size() );
//			C_OWNER.setTextWrap( Wrap() );

//			C_OWNER.getTextColor( AOldColor, AOldBackgroundColor );

//			C_OWNER.setTextColor( Color(), FillColor().GetValue() );

//			C_OWNER.setCursor( X(), Y() );

			TTextFieldWrapper<T_SELF> AWrapper( X(), Y() ); //( this );
			C_OWNER.print( &AWrapper, Text() );

//			C_OWNER.setTextColor( AOldColor, AOldBackgroundColor );

//			C_OWNER.setCursor( ACursorX, ACursorY );

//			C_OWNER.setTextWrap( AOldWrap );
//			C_OWNER.setTextSize( AOldSize );

//			T_Elements_EnterLeave::Call( false ); // Enter/Leave
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AutoSize,
		typename T_Color,
		typename T_Enabled,
//		typename T_Elements_EnterLeave,
		typename T_FillColor,
		typename T_Font,
		typename T_HorizontalAlign,
		typename T_InitialValue,
		typename T_OPERATIONS,
		typename T_Size,
		typename T_UseCodePage437Symbols,
		typename T_Width,
		typename T_Wrap,
		typename T_X,
		typename T_Y
	> class GraphicsElementTextField :
		public GraphicsDelegate<
			GraphicsElementTextField<
				T_OWNER, C_OWNER,
				T_AutoSize,
				T_Color,
				T_Enabled,
//				T_Elements_EnterLeave,
				T_FillColor,
				T_Font,
				T_HorizontalAlign,
				T_InitialValue,
				T_OPERATIONS,
				T_Size,
				T_UseCodePage437Symbols,
				T_Width,
				T_Wrap,
				T_X,
				T_Y
			>, T_OWNER, C_OWNER>,
		public T_Color,
		public T_Enabled,
		public T_FillColor,
		public T_HorizontalAlign,
		public T_InitialValue,
		public T_AutoSize,
		public T_Wrap,
		public T_UseCodePage437Symbols,
		public T_Size,
		public T_X,
		public T_Y,
		public T_Width
	{
		typedef GraphicsElementTextField<
				T_OWNER, C_OWNER,
				T_AutoSize,
				T_Color,
				T_Enabled,
//				T_Elements_EnterLeave,
				T_FillColor,
				T_Font,
				T_HorizontalAlign,
				T_InitialValue,
				T_OPERATIONS,
				T_Size,
				T_UseCodePage437Symbols,
				T_Width,
				T_Wrap,
				T_X,
				T_Y
			> T_SELF;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Color )
		_V_PROP_( FillColor )
		_V_PROP_( HorizontalAlign )
		_V_PROP_( AutoSize )
		_V_PROP_( Wrap )
		_V_PROP_( UseCodePage437Symbols )
		_V_PROP_( Size )

		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )

	public:
		_V_PROP_( InitialValue )

	protected:
		uint8_t	FOldSize = 0;

	public:
		inline uint8_t GetTextSize() { return Size(); }
		inline uint8_t GetPixelTextHeight()
		{
			if( T_Font::IsImplemented() )
				return Size() * T_Font::GetPixelTextFontHeight();

			return Size() * T_OWNER::TYPE_T_FONT::GetPixelTextFontHeight();
		}

		inline uint8_t GetPixelTextWidth()
		{
			if( T_Font::IsImplemented() )
				return Size() * T_Font::GetPixelTextFontWidth();

			return Size() * T_OWNER::TYPE_T_FONT::GetPixelTextFontWidth();
		}

		inline TUniColor GetTextColor() { return Color().GetValue(); }
		inline TUniColor GetTextBackgroundColor() { return FillColor().GetValue(); }
		inline bool GetTextWrap() { return Wrap(); }
		inline bool GetCodePage437() { return UseCodePage437Symbols(); }

	public:
		template <typename T_IMPLEMENTATION> static uint8_t DrawChar( T_IMPLEMENTATION &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			if( T_Font::IsImplemented() )
				return T_Font::DrawFontChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );

			return T_OWNER::DrawChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

//			if( !( ( FillColor & 0xFF000000 ) || ( Color & 0xFF000000 )))
			if( T_OPERATIONS::IsTransparent( FillColor().GetValue() ) && T_OPERATIONS::IsTransparent( Color().GetValue() ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			int16_t ACursorY; // = C_OWNER.getCursorY();
			int16_t ACursorX; // = C_OWNER.getCursorX();

//			C_OWNER.getCursor( ACursorX, ACursorY );

//			TUniColor AOldColor;
//			TUniColor AOldBackgroundColor;

//			T_Elements_EnterLeave::Call( true ); // Enter/Leave
//			uint8_t	AOldSize = C_OWNER.getTextSize();
//			bool AOldWrap = C_OWNER.getTextWrap();

//			C_OWNER.setTextSize( Size() );
//			C_OWNER.setTextWrap( Wrap() );

//			C_OWNER.getTextColor( AOldColor, AOldBackgroundColor );

//			C_OWNER.setTextColor( Color(), FillColor().GetValue() );

//			C_OWNER.setCursor( X(), Y() );
			Mitov::String AText = InitialValue();
			if( AutoSize() )
			{
				uint8_t ANewSize = AText.length();
				while( AText.length() < FOldSize )
					AText += " ";

				FOldSize = ANewSize;
			}

			else
			{
				while( AText.length() < Width() )
				{
					switch( HorizontalAlign() )
					{
						case thaLeft :
							AText += " ";
							break;

						case thaRight :
							AText = " " + AText;
							break;

						case thaCenter :
							AText = " " + AText + " ";
							break;
					}
				}

				AText = AText.substring( 0, Width() );
			}

			TTextFieldWrapper<T_SELF> AWrapper( X(), Y() ); //( this );

			C_OWNER.print( &AWrapper, AText );

//			C_OWNER.setTextColor( AOldColor, AOldBackgroundColor );
//			C_OWNER.setCursor( ACursorX, ACursorY );
//			C_OWNER.setTextWrap( AOldWrap );
//			C_OWNER.setTextSize( AOldSize );
//			T_Elements_EnterLeave::Call( false ); // Enter/Leave
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = (char *)_Data;
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AutoSize,
		typename T_Color,
		typename T_Enabled,
//		typename T_Elements_EnterLeave,
		typename T_FillColor,
		typename T_Font,
		typename T_HorizontalAlign,
		typename T_InitialValue,
		typename T_OPERATIONS,
		typename T_Size,
		typename T_UseCodePage437Symbols,
		typename T_Width,
		typename T_Wrap,
		typename T_X,
		typename T_Y
	> class GraphicsElementTextField_Clocked :
		public GraphicsElementTextField<
			T_OWNER, C_OWNER,
			T_AutoSize,
			T_Color,
			T_Enabled,
//			T_Elements_EnterLeave,
			T_FillColor,
			T_Font,
			T_HorizontalAlign,
			T_InitialValue,
			T_OPERATIONS,
			T_Size,
			T_UseCodePage437Symbols,
			T_Width,
			T_Wrap,
			T_X,
			T_Y
		>
	{
		typedef GraphicsElementTextField<
			T_OWNER, C_OWNER,
			T_AutoSize,
			T_Color,
			T_Enabled,
//			T_Elements_EnterLeave,
			T_FillColor,
			T_Font,
			T_HorizontalAlign,
			T_InitialValue,
			T_OPERATIONS,
			T_Size,
			T_UseCodePage437Symbols,
			T_Width,
			T_Wrap,
			T_X,
			T_Y
		> inherited;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			inherited::Render();
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			inherited::InitialValue() = (char *)_Data;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_OPERATIONS,
		typename T_OutputPin,
		typename T_X,
		typename T_Y
	> class GraphicsElementCheckPixel :
		public T_Enabled,
		public T_OutputPin,
		public T_X,
		public T_Y
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				T_OutputPin::SetPinValue( T_OPERATIONS::ExtractPixelColor( C_OWNER.GetPixelColor( X(), Y() ) ) );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_BackgroundColor,
		typename T_Color,
		typename T_Font,
		typename T_Size,
		typename T_UseCodePage437Symbols,
		typename T_Wrap
	> class TGraphicsTextSettings :
		public T_BackgroundColor,
		public T_Color,
		public T_Font,
		public T_Size,
		public T_UseCodePage437Symbols,
		public T_Wrap
	{
	public:
		typedef T_Font TYPE_T_FONT;

	public:
		_V_PROP_( Size )
		_V_PROP_( Wrap )
		_V_PROP_( Color )
//		_V_PROP_( Font )
		_V_PROP_( BackgroundColor )
		_V_PROP_( UseCodePage437Symbols )

/*
	private:
		using T_Size::GetInstance;
		using T_Wrap::GetInstance;
		using T_Color::GetInstance;
		using T_BackgroundColor::GetInstance;
*/
	};
//---------------------------------------------------------------------------
    template <
        typename T_OWNER, T_OWNER &C_OWNER,
        uint32_t C_INDEX,
        typename T_X,
        typename T_Y
    > class TArduinoGraphicsPoint :
        public T_X,
        public T_Y
    {
    public:
        _V_PROP_( X )
        _V_PROP_( Y )

    public:
        inline void UpdateValue()
        {
            C_OWNER.SetIndexedValue( C_INDEX );
        }

    public:
        inline void GetValue( Mitov::TGraphicsPos &AX, Mitov::TGraphicsPos &AY )
        {
            AX = X().GetValue();
            AY = Y().GetValue();
        }

/*
        inline void ApplyValues( Mitov::GraphicsPoint * ABuffer )
        {
            ABuffer[ C_INDEX ].X = X().GetValue();
            ABuffer[ C_INDEX ].Y = Y().GetValue();
        }
*/
    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif