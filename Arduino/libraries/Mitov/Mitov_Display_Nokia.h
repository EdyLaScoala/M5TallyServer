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
#include <Mitov_Graphics_Monochrome.h>

#define MitovNokiaPartialUpdate

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
/*
	template <uint16_t WIDTH, uint16_t HEIGHT> class DisplayOrientation_Up
	{
	protected:
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		constexpr inline static int16_t width(void)
		{
			return WIDTH;
		}

		constexpr inline static int16_t height(void)
		{
			return HEIGHT;
		}

	protected:
		static inline void CorrectOrientation( int16_t &x, int16_t &y )
		{
		}

		static inline void CorrectScrollOrientation( TGraphicsPos &x, TGraphicsPos &y )
		{
		}

		template <typename T> static inline void drawFastHLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color) 
		{
			AImplementation.drawFastHLineInternal(x, y, w, color);
		}

		template <typename T> static inline void drawFastVLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color) 
		{
			AImplementation.drawFastVLineInternal(x, y, h, color);
		}

	};
//---------------------------------------------------------------------------
	template <uint16_t WIDTH, uint16_t HEIGHT> class DisplayOrientation_Down
	{
	protected:
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		constexpr inline static int16_t width(void)
		{
			return WIDTH;
		}

		constexpr inline static int16_t height(void)
		{
			return HEIGHT;
		}

	protected:
		static inline void CorrectOrientation( int16_t &x, int16_t &y )
		{
			x = WIDTH - x - 1;
			y = HEIGHT - y - 1;
		}
		static inline void CorrectScrollOrientation( TGraphicsPos &x, TGraphicsPos &y )
		{
			x = -x;
			y = -y;
		}

		template <typename T> static inline void drawFastHLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color) 
		{
			// 180 degree rotation, invert x and y - then shift y around for height.
			x = WIDTH - x - 1;
			y = HEIGHT - y - 1;
			x -= (w-1);

			AImplementation.drawFastHLineInternal(x, y, w, color);
		}

		template <typename T> static inline void drawFastVLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color) 
		{
			x = WIDTH - x - 1;
//			y = HEIGHT - y - 1;
			y = HEIGHT - y - h;
//			x -= (w-1);

			AImplementation.drawFastVLineInternal(x, y, h, color);
		}

	};
//---------------------------------------------------------------------------
	template <uint16_t WIDTH, uint16_t HEIGHT> class DisplayOrientation_Left
	{
	protected:
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		constexpr inline static int16_t width(void)
		{
			return HEIGHT;
		}

		constexpr inline static int16_t height(void)
		{
			return WIDTH;
		}

	protected:
		static inline void CorrectOrientation( int16_t &x, int16_t &y )
		{
			swap(x, y);
			y = HEIGHT - y - 1;
		}

		static inline void CorrectScrollOrientation( TGraphicsPos &x, TGraphicsPos &y )
		{
			swap(x, y);
			y = -y;
		}

		template <typename T> static inline void drawFastHLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color) 
		{
			swap(x, y);
			y = HEIGHT - y - 1;
			y -= (w-1);

			AImplementation.drawFastVLineInternal(x, y, w, color);
		}

		template <typename T> static inline void drawFastVLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color) 
		{
			swap(x, y);
			y = HEIGHT - y - 1;

			AImplementation.drawFastHLineInternal(x, y, h, color);
		}

	};
//---------------------------------------------------------------------------
	template <uint16_t WIDTH, uint16_t HEIGHT> class DisplayOrientation_Right
	{
	protected:
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		constexpr inline static int16_t width(void)
		{
			return HEIGHT;
		}

		constexpr inline static int16_t height(void)
		{
			return WIDTH;
		}

	protected:
		static inline void CorrectOrientation( int16_t &x, int16_t &y )
		{
			swap(x, y);
			x = WIDTH - x - 1;
		}

		static inline void CorrectScrollOrientation( TGraphicsPos &x, TGraphicsPos &y )
		{
			swap( x, y );
			x = -x;
		}

		template <typename T> static inline void drawFastHLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color) 
		{
			// 90 degree rotation, swap x & y for rotation, then invert x
			swap(x, y);
			x = WIDTH - x - 1;
			AImplementation.drawFastVLineInternal(x, y, w, color);
		}

		template <typename T> static inline void drawFastVLineDispatch( T &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color) 
		{
			swap(x, y);
			x = WIDTH - x - h;

			AImplementation.drawFastHLineInternal(x, y, h, color);
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_BackgroundColor,
		typename T_BiasSystem,
		typename T_ChipSelectOutputPin,
		typename T_Contrast,
		typename T_DataCommandOutputPin,
		typename T_Elements_Render,
        typename T_FCursorX,
        typename T_FCursorY,
		typename T_FModified,
		typename T_Inverted,
		typename T_Orientation,
		typename T_PoweredUp,
		typename T_ResetOutputPin,
		typename T_RESET_IMPLEMENTATION,
		typename T_SPISpeed,
		typename T_Text
	> class DisplayNokia5110 :
		public T_BiasSystem,
		public T_ChipSelectOutputPin,
		public T_Contrast,
		public T_DataCommandOutputPin,
        public T_FCursorX,
        public T_FCursorY,
		public T_FModified,
		public T_Inverted,
		public T_Orientation,
		public T_PoweredUp,
		public T_ResetOutputPin,
		public T_RESET_IMPLEMENTATION,
		public T_SPISpeed,
//		public T_Text,
		public GraphicsImpl <
				DisplayNokia5110 <
						T_SPI, C_SPI,
						T_BackgroundColor,
						T_BiasSystem,
						T_ChipSelectOutputPin,
						T_Contrast,
						T_DataCommandOutputPin,
						T_Elements_Render,
                        T_FCursorX,
                        T_FCursorY,
						T_FModified,
						T_Inverted,
						T_Orientation,
						T_PoweredUp,
						T_ResetOutputPin,
						T_RESET_IMPLEMENTATION,
						T_SPISpeed,
						T_Text
					>,
					T_BackgroundColor,
					T_Text
				>
	{
		typedef DisplayNokia5110 T_SELF;

		typedef GraphicsImpl<
			T_SELF,
			T_BackgroundColor,
			T_Text
		> inherited;

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( DataCommandOutputPin )
		_V_PIN_( ResetOutputPin )

	protected:
		static	const uint8_t WIDTH = 84;
		static	const uint8_t HEIGHT = 48;

		static	const uint8_t PCD8544_POWERDOWN = 0x04;
		static	const uint8_t PCD8544_ENTRYMODE = 0x02;
		static	const uint8_t PCD8544_EXTENDEDINSTRUCTION = 0x01;

		static	const uint8_t PCD8544_DISPLAYBLANK = 0x0;
		static	const uint8_t PCD8544_DISPLAYNORMAL = 0x4;
		static	const uint8_t PCD8544_DISPLAYALLON = 0x1;
		static	const uint8_t PCD8544_DISPLAYINVERTED = 0x5;

		// H = 0
		static	const uint8_t PCD8544_FUNCTIONSET = 0x20;
		static	const uint8_t PCD8544_DISPLAYCONTROL = 0x08;
		static	const uint8_t PCD8544_SETYADDR = 0x40;
		static	const uint8_t PCD8544_SETXADDR = 0x80;

		// H = 1
		static	const uint8_t PCD8544_SETTEMP = 0x04;
		static	const uint8_t PCD8544_SETBIAS = 0x10;
		static	const uint8_t PCD8544_SETVOP = 0x80;

	public:
		_V_PROP_( SPISpeed )
		_V_PROP_( Text )
		_V_PROP_( Contrast )
		_V_PROP_( BiasSystem )
		_V_PROP_( Inverted )
		_V_PROP_( PoweredUp )
		_V_PROP_( Orientation )

//	public:
//		TArduinoGraphicsOrientation	Orientation : 2; //= goUp;

	public:
		_V_PROP_( FCursorX )
		_V_PROP_( FCursorY )

	protected:
		_V_PROP_( FModified )

#ifdef MitovNokiaPartialUpdate
		uint8_t xUpdateMin : 7;
		uint8_t xUpdateMax : 7;
		uint8_t yUpdateMin : 6;
		uint8_t yUpdateMax : 6;
#endif

		uint8_t FBuffer[ HEIGHT * WIDTH / 8 ];

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
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		inline int16_t width(void) const
		{
			switch( Orientation().GetValue() )
			{
				case goUp:
				case goDown:
					return WIDTH;
			}

			return HEIGHT;
		}

		inline int16_t height(void)
		{
			switch( Orientation().GetValue() )
			{
				case goUp:
				case goDown:
					return HEIGHT;
			}

			return WIDTH;
		}

	public:
		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
//			Serial.println( "drawPixel" );
			if( color == tmcNone )
				return;

			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return;

			// check rotation, move pixel around if necessary
			switch( Orientation().GetValue() )
			{
//				case goUp:
//					break;

				case goDown:
					x = WIDTH - x - 1;
					y = HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = HEIGHT - y - 1;
					break;

				case goRight:
					swap(x, y);
					x = WIDTH - x - 1;
					break;

//			T_Orientation::CorrectOrientation( x, y );
			}
/*
			switch ( Orientation ) 
			{
				case goRight:
					swap(x, y);
					x = WIDTH - x - 1;
					break;

				case goDown:
					x = WIDTH - x - 1;
					y = HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = HEIGHT - y - 1;
					break;
			}  
*/
			// x is which column
			switch (color) 
			{
				case tmcWhite :		FBuffer[x+ (y/8) * WIDTH ] |=  (1 << (y & 0b111 )); break;
				case tmcBlack :		FBuffer[x+ (y/8) * WIDTH ] &= ~(1 << (y & 0b111 )); break; 
				case tmcInvert :	FBuffer[x+ (y/8) * WIDTH ] ^=  (1 << (y & 0b111 )); break; 
			}

			FModified() = true;
			updateBoundingBox(x,y,x,y);
    	}

		static void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
//			Serial.println( "ClearScreen" );
			switch( color )
			{
				case tmcNone :
					return;

				case tmcBlack :
					memset( AImplementation.FBuffer, 0, sizeof( AImplementation.FBuffer ));
					break;

				case tmcWhite :
					memset( AImplementation.FBuffer, 255, sizeof( AImplementation.FBuffer ));
					break;

				case tmcInvert :
				{
					register uint8_t *pBuf = AImplementation.FBuffer;
					for( int i = 0; i < sizeof( AImplementation.FBuffer ); ++i )
						pBuf[ i ] = ~pBuf[ i ];

					break;
				}
			}

			AImplementation.updateBoundingBox(0,0,WIDTH - 1,HEIGHT - 1);
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return false;

			// check rotation, move pixel around if necessary
//			T_Orientation::CorrectOrientation( x, y );

			switch ( Orientation().GetValue() ) 
			{
				case goDown:
					x = WIDTH - x - 1;
					y = HEIGHT - y - 1;
					break;

				case goRight:
					swap(x, y);
					x = WIDTH - x - 1;
					break;

				case goLeft:
					swap(x, y);
					y = HEIGHT - y - 1;
					break;
			}  

			return (( FBuffer[ x + ( y / 8 ) * WIDTH ] & ( 1 << ( y & 0b111 ))) != 0 );
		}

		static void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color) 
		{
//			T_Orientation::drawFastHLineDispatch( AImplementation, x, y, w, color ); 
			switch( AImplementation.Orientation().GetValue() ) 
			{ 
				case goUp:
					AImplementation.drawFastHLineInternal(x, y, w, color);
					// 0 degree rotation, do nothing
					break;

				case goRight:
					// 90 degree rotation, swap x & y for rotation, then invert x
					swap(x, y);
					x = WIDTH - x - 1;
					AImplementation.drawFastVLineInternal(x, y, w, color);
					break;

				case goDown:
					// 180 degree rotation, invert x and y - then shift y around for height.
					x = WIDTH - x - 1;
					y = HEIGHT - y - 1;
					x -= (w-1);
					AImplementation.drawFastHLineInternal(x, y, w, color);
					break;

				case goLeft:
					// 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
					swap(x, y);
					y = HEIGHT - y - 1;
					y -= (w-1);
					AImplementation.drawFastVLineInternal(x, y, w, color);
					break;
			}
/*
			if( bSwap ) 
				AImplementation.drawFastVLineInternal(x, y, w, color);

			else
				AImplementation.drawFastHLineInternal(x, y, w, color);
*/
		}

		static void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color) 
		{
			switch( AImplementation.Orientation().GetValue() )
			{
				case goUp:
					AImplementation.drawFastVLineInternal(x, y, h, color);
					break;

				case goDown:
					x = WIDTH - x - 1;
		//			y = HEIGHT - y - 1;
					y = HEIGHT - y - h;
		//			x -= (w-1);

					AImplementation.drawFastVLineInternal(x, y, h, color);
					break;

				case goLeft:
					swap(x, y);
					y = HEIGHT - y - 1;

					AImplementation.drawFastHLineInternal(x, y, h, color);
					break;

				case goRight:
					swap(x, y);
					x = WIDTH - x - h;

					AImplementation.drawFastHLineInternal(x, y, h, color);
					break;
			}

//			T_Orientation::drawFastVLineDispatch( AImplementation, x, y, w, color ); 
		}

		static void Scroll( T_SELF &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			if( X == 0 && Y == 0 )
				return;

//			Serial.println( "Scroll" );
			// check rotation, move pixel around if necessary
//			T_Orientation::CorrectScrollOrientation( X, Y );

			switch ( AImplementation.Orientation().GetValue() ) 
			{
				case goRight:
					swap(X, Y);
					X = -X;
					break;

				case goDown:
					X = -X;
					Y = -Y;
					break;

				case goLeft:
					swap(X, Y);
					Y = -Y;
					break;
			}  		

			uint8_t	AOffset = abs( Y ) & 0b111;
			int32_t	AYBytes = Y / 8;
			AYBytes *= WIDTH;

/*
			if( ! Y )
			{
				uint8_t *ABuffer = FBuffer;
				if( X > 0 )
					for( int ayi = 0; ayi < HEIGHT / 8; ++ ayi, ABuffer += WIDTH )
					{
						memmove( ABuffer + X, ABuffer, WIDTH - X );
						switch( color )
						{
							case tmcBlack:
								memset( ABuffer, 0, X ); break;

							case tmcWhite:
								memset( ABuffer, 255, X ); break;

							case tmcInvert:
								for( int axi = 0; axi < X; ++ axi )
									ABuffer[ axi ] = ~ABuffer[ axi ];
						}
					}

				else // Y = 0 && X < 0
					for( int ayi = 0; ayi < HEIGHT / 8; ++ ayi, ABuffer += WIDTH )
					{
						memmove( ABuffer, ABuffer - X, WIDTH + X );
						switch( color )
						{
							case tmcBlack:
								memset( ABuffer + WIDTH + X, 0, -X ); break;

							case tmcWhite:
								memset( ABuffer + WIDTH + X, 255, -X ); break;

							case tmcInvert:
								for( int axi = WIDTH + X; axi < WIDTH; ++ axi )
									ABuffer[ axi ] = ~ABuffer[ axi ];
						}
					}

			}

			else // Y <> 0
*/
			{
				if( Y > 0 )
				{
					if( X == 0 )
					{
						if( AYBytes )
							memmove( AImplementation.FBuffer + AYBytes, AImplementation.FBuffer, ( HEIGHT / 8 ) * WIDTH - AYBytes );

					}

					else if( X > 0 )
					{
						register uint8_t *ABuffer = AImplementation.FBuffer + WIDTH * (( ( HEIGHT - Y + 7 ) / 8 ) - 1 );
						for( int ayi = 0; ayi < ( HEIGHT - Y + 7 ) / 8; ++ ayi, ABuffer -= WIDTH )
						{
							memmove( ABuffer + AYBytes + X, ABuffer, WIDTH - X );
							switch( color )
							{
								case tmcBlack:
									memset( ABuffer + AYBytes, 0, X ); break;

								case tmcWhite:
									memset( ABuffer + AYBytes, 255, X ); break;

								case tmcInvert:
									for( int axi = 0; axi < X; ++ axi )
										ABuffer[ axi + AYBytes ] = ~ABuffer[ axi + AYBytes ];
							}
						}
					}

					else // Y > 0 && X < 0
					{ 
						register uint8_t *ABuffer = AImplementation.FBuffer + WIDTH * (( ( HEIGHT - Y + 7 ) / 8 ) - 1 );
						for( int ayi = 0; ayi < ( HEIGHT - Y + 7 ) / 8; ++ ayi, ABuffer -= WIDTH )
						{
							memmove( ABuffer + AYBytes, ABuffer - X, WIDTH + X );
							switch( color )
							{
								case tmcBlack:
									memset( ABuffer + AYBytes + WIDTH + X, 0, -X ); break;

								case tmcWhite:
									memset( ABuffer + AYBytes + WIDTH + X, 255, -X ); break;

								case tmcInvert:
									for( int axi = WIDTH + X; axi < WIDTH; ++ axi )
										ABuffer[ axi + AYBytes ] = ~ABuffer[ axi + AYBytes ];
							}
						}
					}

					register uint8_t * AToPtr = AImplementation.FBuffer + WIDTH * HEIGHT / 8 - 1;
					register uint8_t *AFromPtr = AToPtr - WIDTH;
					for( int ayi = 0; ayi < ( HEIGHT - Y + 7 ) / 8 - 1; ++ ayi )
						for( int axi = 0; axi < WIDTH; ++ axi, --AFromPtr, --AToPtr )
							*AToPtr = *AToPtr << AOffset | ( *AFromPtr >> ( 8 - AOffset ));

					if( color != tmcNone )
					{
						uint8_t	AMask;
						if( color == tmcBlack )
							AMask = 0xFF << AOffset;

						else
							AMask = 0xFF >> ( 8 - AOffset );

						AToPtr = AImplementation.FBuffer + AYBytes;
						for( int axi = 0; axi < WIDTH; ++ axi, ++AToPtr )
						{
							switch( color )
							{
								case tmcBlack:
									*AToPtr <<= AOffset;
									*AToPtr &= AMask; 
									break;

								case tmcWhite:
									*AToPtr <<= AOffset;
									*AToPtr |= AMask; 
									break;

								case tmcInvert:
									*AToPtr <<= AOffset | ( *AToPtr & ( ~AMask ));
									*AToPtr ^= AMask; 
									break;
							}
						}
					}
				}

				else // Y < 0
				{
					if( X == 0 )
					{
						if( AYBytes )
							memmove( AImplementation.FBuffer, AImplementation.FBuffer - AYBytes, ( HEIGHT / 8 ) * WIDTH + AYBytes );

					}

					else if( X > 0 )
					{
						register uint8_t *ABuffer = AImplementation.FBuffer;
						for( int ayi = 0; ayi < ( HEIGHT + Y + 7 ) / 8; ++ ayi, ABuffer += WIDTH )
						{
							memmove( ABuffer + X, ABuffer - AYBytes, WIDTH - X );
							switch( color )
							{
								case tmcBlack:
									memset( ABuffer, 0, X ); break;

								case tmcWhite:
									memset( ABuffer, 255, X ); break;

								case tmcInvert:
									for( int axi = 0; axi < X; ++ axi )
										ABuffer[ axi ] = ~ABuffer[ axi ];
							}
						}
					}
					else // Y < 0 && X < 0
					{
						register uint8_t *ABuffer = AImplementation.FBuffer;
						for( int ayi = 0; ayi < ( HEIGHT + Y + 7 ) / 8; ++ ayi, ABuffer += WIDTH )
						{
							memmove( ABuffer, ABuffer - AYBytes - X, WIDTH + X );
							switch( color )
							{
								case tmcBlack:
									memset( ABuffer + WIDTH + X, 0, -X ); break;

								case tmcWhite:
									memset( ABuffer + WIDTH + X, 255, -X ); break;

								case tmcInvert:
									for( int axi = WIDTH + X; axi < WIDTH; ++ axi )
										ABuffer[ axi ] = ~ABuffer[ axi ];
							}
						}
					}

					if( AOffset )
					{
						register uint8_t * AToPtr = AImplementation.FBuffer;
						register uint8_t *AFromPtr = AToPtr + WIDTH;
						for( int ayi = 0; ayi < ( HEIGHT + Y + 7 ) / 8 - 1; ++ ayi )
							for( int axi = 0; axi < WIDTH; ++ axi, ++AFromPtr, ++AToPtr )
								*AToPtr = *AToPtr >> AOffset | ( *AFromPtr << ( 8 - AOffset ));

						if( color != tmcNone )
						{
							uint8_t	AMask;
							if( color == tmcBlack )
								AMask = 0xFF >> AOffset;

							else
								AMask = 0xFF << ( 8 - AOffset );

							AToPtr = AImplementation.FBuffer + WIDTH * HEIGHT / 8 + AYBytes - WIDTH;
							for( int axi = 0; axi < WIDTH; ++ axi, ++AToPtr )
							{
								switch( color )
								{
									case tmcBlack:
										*AToPtr >>= AOffset;
										*AToPtr &= AMask; 
										break;

									case tmcWhite:
										*AToPtr >>= AOffset;
										*AToPtr |= AMask; 
										break;

									case tmcInvert:
										*AToPtr >>= AOffset | ( *AToPtr & ( ~AMask ));
										*AToPtr ^= AMask; 
										break;
								}
							}
						}
					}
				}

				if( AYBytes )
				{
					if( Y > 0 )
					{
						switch( color )
						{
							case tmcBlack:
								memset( AImplementation.FBuffer, 0, AYBytes ); break;

							case tmcWhite:
								memset( AImplementation.FBuffer, 255, AYBytes ); break;

							case tmcInvert:
							{
								register uint8_t * APtr = AImplementation.FBuffer;
								for( int axi = 0; axi < WIDTH; ++ axi )
									APtr[ axi ] = ~APtr[ axi ];
							}
						}
					}

					else // Y < 0
					{
						switch( color )
						{
							case tmcBlack:
								memset( AImplementation.FBuffer + HEIGHT * WIDTH / 8 + AYBytes, 0, - AYBytes ); break;

							case tmcWhite:
								memset( AImplementation.FBuffer + HEIGHT * WIDTH / 8 + AYBytes, 255, - AYBytes ); break;

							case tmcInvert:
							{
								register uint8_t * APtr = AImplementation.FBuffer + HEIGHT * WIDTH / 8 + AYBytes;
								for( int axi = 0; axi < WIDTH; ++ axi )
									APtr[ axi ] = ~APtr[ axi ];
//									FBuffer[ axi + HEIGHT * WIDTH / 8 + AYBytes ] = ~FBuffer[ axi + HEIGHT * WIDTH / 8 + AYBytes ];
							}
						}
					}
				}
			}

			AImplementation.updateBoundingBox(0,0,WIDTH - 1,HEIGHT - 1);
		}

/*
		void ScrollBufferUp( uint8_t ANumLines )
		{
			memmove( FBuffer, FBuffer + ANumLines * WIDTH / 8, ( HEIGHT - ANumLines ) * WIDTH / 8 );
			memset( FBuffer + ( HEIGHT - ANumLines ) * WIDTH / 8, 0, ANumLines * WIDTH / 8 );
		}
*/
	public:
		void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, TUniColor color) 
		{
			if( color == tmcNone )
				return;

			// Do bounds/limit checks
			if(y < 0 || y >= HEIGHT) 
				return;

//			Serial.println( "drawFastHLineInternal" );
			// make sure we don't try to draw below 0
			if(x < 0) 
			{ 
				w += x;
				x = 0;
			}

			// make sure we don't go off the edge of the display
			if( (x + w) > WIDTH) 
				w = (WIDTH - x);

			// if our width is now negative, punt
			if(w <= 0) 
				return;

			// set up the pointer for  movement through the FBuffer
			register uint8_t *pBuf = FBuffer;
			// adjust the FBuffer pointer for the current row
			pBuf += ((y/8) * WIDTH);
			// and offset x columns in
			pBuf += x;

			register uint8_t mask = 1 << (y&7);

//			Serial.print( x ); Serial.print( " " ); Serial.print( y ); Serial.print( " " ); Serial.println( w );

			updateBoundingBox( x, y, x + w - 1, y );

			switch (color) 
			{
				case tmcWhite:         while(w--) { *pBuf++ |= mask; }; break;
				case tmcBlack: mask = ~mask;   while(w--) { *pBuf++ &= mask; }; break;
				case tmcInvert:         while(w--) { *pBuf++ ^= mask; }; break;
			}

		}

		void drawFastVLineInternal(int16_t x, int16_t __y, int16_t 
			__h, TUniColor color) 
		{
			if( color == tmcNone )
				return;

			// do nothing if we're off the left or right side of the screen
			if(x < 0 || x >= WIDTH) 
				return;

//			Serial.println( "drawFastVLineInternal" );

			// make sure we don't try to draw below 0
			if(__y < 0) 
			{ 
				// __y is negative, this will subtract enough from __h to account for __y being 0
				__h += __y;
				__y = 0;
			} 

			// make sure we don't go past the height of the display
			if( (__y + __h) > HEIGHT)
				__h = (HEIGHT - __y);
			
			// if our height is now negative, punt 
			if(__h <= 0)
				return;			

			// this display doesn't need ints for coordinates, use local byte registers for faster juggling
			register uint8_t y = __y;
			register uint8_t h = __h;


			// set up the pointer for fast movement through the FBuffer
			register uint8_t *pBuf = FBuffer;
			// adjust the FBuffer pointer for the current row
			pBuf += ((y/8) * WIDTH);
			// and offset x columns in
			pBuf += x;

			// do the first partial byte, if necessary - this requires some masking
			register uint8_t mod = (y&7);
			if(mod) 
			{
				// mask off the high n bits we want to set 
				mod = 8-mod;

				// note - lookup table results in a nearly 10% performance improvement in fill* functions
				// register uint8_t mask = ~(0xFF >> (mod));
				static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
				register uint8_t mask = premask[mod];

				// adjust the mask if we're not going to reach the end of this byte
				if( h < mod)
					mask &= (0XFF >> (mod-h));

				switch (color) 
				{
					case tmcWhite :  *pBuf |=  mask;  break;
					case tmcBlack :  *pBuf &= ~mask;  break;
					case tmcInvert : *pBuf ^=  mask;  break;
				}
  
				// fast exit if we're done here!
				if(h<mod) 
					return;

				h -= mod;

				pBuf += WIDTH;
			}


			// write solid bytes while we can - effectively doing 8 rows at a time
			if(h >= 8) 
			{ 
				if (color == tmcInvert)  
				{          // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
					do  
					{
						*pBuf=~(*pBuf);

						// adjust the FBuffer forward 8 rows worth of data
						pBuf += WIDTH;

						// adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
						h -= 8;
					} 
					while(h >= 8);
				}

				else 
				{
					// store a local value to work with 
					register uint8_t val = (color == tmcWhite) ? 255 : 0;

					do  
					{
						// write our value in
						*pBuf = val;

						// adjust the FBuffer forward 8 rows worth of data
						pBuf += WIDTH;

						// adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
						h -= 8;
					} 
					while(h >= 8);
				}
			}

			// now do the final partial byte, if necessary
			if(h) 
			{
				mod = h & 0b111;
				// this time we want to mask the low bits of the byte, vs the high bits we did above
				// register uint8_t mask = (1 << mod) - 1;
				// note - lookup table results in a nearly 10% performance improvement in fill* functions
				static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
				register uint8_t mask = postmask[mod];
				switch (color) 
				{
					case tmcWhite:   *pBuf |=  mask;  break;
					case tmcBlack:   *pBuf &= ~mask;  break;
					case tmcInvert:  *pBuf ^=  mask;  break;
				}
			}

			updateBoundingBox( x, y, x, y + __h - 1 );
		}

	protected:
		void SendCommand(uint8_t ACommand )
		{
//			DataCommandOutputPin.SendValue<bool>( false );
			T_DataCommandOutputPin::SetPinValueLow();
//			ChipSelectOutputPin.SendValue<bool>( false );
			T_ChipSelectOutputPin::SetPinValueLow();
//			Serial.println( ACommand, HEX );
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ));
			C_SPI.transfer( ACommand );
			C_SPI.endTransaction();
//			ChipSelectOutputPin.SendValue<bool>( true );
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void SendBuffer()
		{
//			Serial.println( xUpdateMin );
//			Serial.println( xUpdateMax );
			uint8_t col, maxcol, p;
  
			for( p = 0; p < 6; p++) 
			{
#ifdef MitovNokiaPartialUpdate
				// check if this page is part of update
				if ( yUpdateMin >= ((p+1)*8) )
					continue;   // nope, skip it!
			
				if (yUpdateMax < p*8)
					break;
			
#endif

				SendCommand(PCD8544_SETYADDR | p);

#ifdef MitovNokiaPartialUpdate
				col = xUpdateMin;
				maxcol = xUpdateMax;
#else
			// start at the beginning of the row
				col = 0;
				maxcol = WIDTH-1;
#endif
				SendCommand(PCD8544_SETXADDR | col);

//				DataCommandOutputPin.SendValue<bool>( true );
				T_DataCommandOutputPin::SetPinValueHigh();
//				ChipSelectOutputPin.SendValue<bool>( false );
				T_ChipSelectOutputPin::SetPinValueLow();

				C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ));

				for(; col <= maxcol; col++)
					C_SPI.transfer( FBuffer[ ( WIDTH * p ) + col ] );

				C_SPI.endTransaction();
			
//				for(; col <= maxcol; col++)
//					C_SPI.transfer( 0x55 );

//				ChipSelectOutputPin.SendValue<bool>( true );
				T_ChipSelectOutputPin::SetPinValueHigh();
			}

			SendCommand(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
#ifdef MitovNokiaPartialUpdate
			xUpdateMin = WIDTH - 1;
			xUpdateMax = 0;
			yUpdateMin = HEIGHT-1;
			yUpdateMax = 0;
#endif		
		}

		void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) 
		{
			FModified() = true;
/*
			Serial.print( "updateBoundingBox " );
			Serial.print( xmin );
			Serial.print( " " );
			Serial.print( ymin );
			Serial.print( " " );
			Serial.print( xmax );
			Serial.print( " " );
			Serial.println( ymax );
*/
#ifdef MitovNokiaPartialUpdate
			if (xmin < xUpdateMin) 
				xUpdateMin = xmin;

			if (xmax > xUpdateMax) 
			{
				xUpdateMax = xmax;
//				if( xUpdateMax > WIDTH - 1 )
//					xUpdateMax = WIDTH - 1;

			}

			if (ymin < yUpdateMin) 
				yUpdateMin = ymin;

			if (ymax > yUpdateMax) 
				yUpdateMax = ymax;
#endif
/*
			Serial.print( xUpdateMin );
			Serial.print( " " );
			Serial.print( yUpdateMin );
			Serial.print( " " );
			Serial.print( xUpdateMax );
			Serial.print( " " );
			Serial.println( yUpdateMax );
*/
		}

	public:
		void display()
		{
//			Serial.println( "display()" );
			SendBuffer();

			FModified() = false;
		}

		void UpdateContrast()
		{
			// get into the EXTENDED mode!
			SendCommand(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

			uint8_t AContrast = ( Contrast().GetValue() * 0x7F ) + 0.5;

			SendCommand( PCD8544_SETVOP | AContrast); // Experimentally determined

			// normal mode
			if( PoweredUp() )
				SendCommand(PCD8544_FUNCTIONSET );

			else
				SendCommand(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN );
		}

		void UpdateInverted()
		{
			if( Inverted() )
				SendCommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED);

			else
				// Set display to Normal
				SendCommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
		}

		void UpdatePoweredUp()
		{
			if( PoweredUp() )
			{
				SendCommand(PCD8544_FUNCTIONSET );
				display();
			}

			else
				SendCommand(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN );
		}

		void UpdateBiasSystem()
		{
			// get into the EXTENDED mode!
			SendCommand(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

			SendCommand(PCD8544_SETBIAS | BiasSystem );

			// normal mode
			if( PoweredUp() )
				SendCommand(PCD8544_FUNCTIONSET );

			else
				SendCommand(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN );
		}

	public:
		inline void SetResetValueHigh() // Used by the T_RESET_IMPLEMENTATION
		{
			T_ResetOutputPin::SetPinValueHigh();
		}

		inline void SetResetValueLow() // Used by the T_RESET_IMPLEMENTATION
		{
			T_ResetOutputPin::SetPinValueLow();
		}

	public:
		inline void SystemLoopEnd()
		{
			if( FModified() )
				display();

		}

		inline void SystemInit()
		{
			T_RESET_IMPLEMENTATION::ExecuteReset( this, 500 );
/*
#ifdef _MITOV_NOKIA_5110_RESET_PIN_
			if( ResetOutputPin.IsConnected() )
			{
				ResetOutputPin.SendValue<bool>( false );
				delay(500);
				ResetOutputPin.SendValue<bool>( true );
			}
#endif
*/
//			Serial.println( "SystemInit" );
			// get into the EXTENDED mode!
			SendCommand(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

			// LCD bias select (4 is optimal?)
			SendCommand(PCD8544_SETBIAS | BiasSystem().GetValue() );

			uint8_t AContrast = ( Contrast().GetValue() * 0x7F ) + 0.5;

//			Serial.println( AContrast );

			SendCommand( PCD8544_SETVOP | AContrast); // Experimentally determined

			// normal mode
			if( PoweredUp() )
				SendCommand(PCD8544_FUNCTIONSET );

			else
				SendCommand(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN );
//			SendCommand(PCD8544_FUNCTIONSET);

//			Serial.println( "Test1" );
		}

		inline void SystemStart()
		{
//			Serial.println( "SystemStart" );
			UpdateInverted();

//			inherited::setTextSize( Text().Size() );
//			inherited::setTextColor( Text().Color(), Text().BackgroundColor().GetValue() );
//			inherited::setTextWrap( Text().Wrap() );
			

//			FDisplay.begin( SSD1306_SWITCHCAPVCC, Address, false );
//			FValue = InitialValue;

//			drawPixel( 20, 20, WHITE );

//			Serial.println( "ClearScreen" );
			ClearScreen( *this, inherited::BackgroundColor().GetValue() );
			inherited::RenderElements();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );

			display();
		}

	public:
		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			display();
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
//		DisplayNokia5110( const unsigned char * AFont ) :
		DisplayNokia5110()
//			inherited( AFont )
//			Orientation( goUp )
#ifdef MitovNokiaPartialUpdate
			:
			xUpdateMin( 0 ),
			xUpdateMax( WIDTH - 1 ),
			yUpdateMin( 0 ),
			yUpdateMax( HEIGHT - 1 )
#endif
		{
			FModified() = false;
			ClearScreen( *this, inherited::BackgroundColor().GetValue() );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif