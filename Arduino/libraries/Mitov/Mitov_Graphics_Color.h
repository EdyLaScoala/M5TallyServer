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
#include <Mitov_Graphics.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorElementDrawAlphaBitmap :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawAlphaColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
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
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorElementDrawAlphaBitmap_565 :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawAlphaColorBitmap_565( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
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
		uint32_t C_COUNT,
		typename T_Centered,
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorRepeatElementDrawAlphaBitmap :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawRepeatAlphaColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, Size() );
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
		uint32_t C_COUNT,
		typename T_Centered,
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorRepeatElementDrawAlphaBitmap_565 :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawRepeatAlphaColorBitmap_565( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, Size() );
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
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorElementDrawBitmap :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
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
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorElementDrawBitmap_565 :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawColorBitmap_565( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
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
		uint32_t C_COUNT,
		typename T_Centered,
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorRepeatElementDrawBitmap :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawRepeatColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, Size() );
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
		uint32_t C_COUNT,
		typename T_Centered,
		typename T_DATA,
		typename T_Enabled,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsColorRepeatElementDrawBitmap_565 :
		public T_Enabled,
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Enabled )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawRepeatColorBitmap_565( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------	
	template <
		typename T_OWNER,
		typename T_CARD,
		T_OWNER &C_OWNER,
		T_CARD &C_CARD,
		typename T_Enabled,
		typename T_X, 
		typename T_Y
	> class GraphicsColorElementDrawGraphics : 
		public GraphicsDelegate<
			GraphicsColorElementDrawGraphics<
				T_OWNER,
				T_CARD,
				C_OWNER,
				C_CARD,
				T_Enabled,
				T_X,
				T_Y
			>, T_OWNER, C_OWNER>,
		public T_Enabled,
		public T_X,
		public T_Y
	{
		typedef GraphicsColorElementDrawGraphics<
				T_OWNER,
				T_CARD,
				C_OWNER,
				C_CARD,
				T_Enabled,
				T_X,
				T_Y
			> T_SELF;

    public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void Render()
		{
			if( ! Enabled().GetValue() )
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			C_CARD.template RenderGraphics<T_SELF>( this, AParentX + X(), AParentY + Y() );
		}

	public:
		inline TUniColor MakeColorColor( uint8_t r, uint8_t g, uint8_t b )
		{
			return 0xFF000000 | ( uint32_t( r ) << 16 ) | ( uint32_t( g ) << 8 ) | b;
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
		typename T_BackgroundColor,
		typename T_BlackColor,
		typename T_Elements_Render,
		typename T_Enabled,
		typename T_Text,
		typename T_WhiteColor,
		typename T_X,
		typename T_Y
	> class GraphicsElementColorMonohromeDrawScene : 
		public GraphicsIntf<T_OWNER>, 
		public Graphics<
			GraphicsElementColorMonohromeDrawScene<
				T_OWNER, C_OWNER,
				T_BackgroundColor,
				T_BlackColor,
				T_Elements_Render,
				T_Enabled,
				T_Text,
				T_WhiteColor,
				T_X,
				T_Y
			>
		>,
		public T_Enabled,
		public T_WhiteColor,
		public T_BlackColor,
		public T_BackgroundColor,
		public T_X,
		public T_Y
	{
		typedef GraphicsIntf<T_OWNER> inherited;
		typedef GraphicsElementColorMonohromeDrawScene<
				T_OWNER, C_OWNER,
				T_BackgroundColor,
				T_BlackColor,
				T_Elements_Render,
				T_Enabled,
				T_Text,
				T_WhiteColor,
				T_X,
				T_Y
			>	T_SELF;

		typedef Graphics<T_SELF>	graphics;

    public:
		typedef typename T_Text::TYPE_T_FONT TYPE_T_FONT;

    public:
		_V_PROP_( Enabled )
		_V_PROP_( WhiteColor )
		_V_PROP_( BlackColor )
		_V_PROP_( BackgroundColor )
		_V_PROP_( Text )
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
		template <typename T_SELF_DELEGATOR> static inline uint8_t DrawChar( T_SELF_DELEGATOR &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			return T_Text::TYPE_T_FONT::DrawFontChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{ 
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			AX = AParentX + X();
			AY = AParentY + Y();
		}

	public:
		inline T_BackgroundColor &GetBackgroundColor()
		{
			return BackgroundColor();
		}

		inline int16_t width(void) const
		{
			return C_OWNER.width() - T_X::GetValue();
		}

		inline int16_t height(void) const
		{
			return C_OWNER.height() - T_Y::GetValue();
		}

		inline void SetCursor( int16_t x, int16_t y )
		{		
			C_OWNER.SetCursor( x, y );
		}

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( color == tmcNone )
				return;

			switch( color )
			{
				case tmcWhite:	color =	WhiteColor().GetValue(); break;
				case tmcBlack:	color =	BlackColor().GetValue(); break;
				case tmcInvert:	color =	( C_OWNER.GetPixelColor( x, y ) > 0xFF000000 ) ? WhiteColor().GetValue() : BlackColor().GetValue(); break;
				default:
					return;
			}

			C_OWNER.drawPixel( x, y, color );
		}

		static inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return ( C_OWNER.GetPixelColor( x, y ) > 0xFF000000 ) ? tmcWhite : tmcBlack;
		}

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				T_Elements_Render::Call();

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return graphics::write( AImplementation, AConfig, AValue );
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
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &GraphicsElementColorMonohromeDrawScene::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &GraphicsElementColorMonohromeDrawScene::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
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

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_BackgroundColor,
		typename T_Elements_Render,
		typename T_Enabled,
		typename T_Text,
		typename T_X,
		typename T_Y
	> class GraphicsElementColorGrayscaleDrawScene : 
		public GraphicsIntf<T_OWNER>, 
		public Graphics<
			GraphicsElementColorGrayscaleDrawScene<
				T_OWNER, C_OWNER, 
				T_BackgroundColor,
				T_Elements_Render,
				T_Enabled,
				T_Text,
				T_X,
				T_Y
			>
		>,
		public T_Enabled,
		public T_BackgroundColor,
//		public T_Text,
		public T_X,
		public T_Y
	{
		typedef GraphicsIntf<T_OWNER> inherited;
		typedef GraphicsElementColorGrayscaleDrawScene<
				T_OWNER, C_OWNER, 
				T_BackgroundColor,
				T_Elements_Render,
				T_Enabled,
				T_Text,
				T_X,
				T_Y
			>	T_SELF;

		typedef Graphics<T_SELF>	graphics;

    public:
		typedef typename T_Text::TYPE_T_FONT TYPE_T_FONT;

    public:
		_V_PROP_( Enabled )
//		_V_PROP_( Text )
		_V_PROP_( BackgroundColor )
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
		template <typename T_SELF_DELEGATOR> static inline uint8_t DrawChar( T_SELF_DELEGATOR &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
//			Serial.println( "drawChar" );
			return T_Text::TYPE_T_FONT::DrawFontChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{ 
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			AX = AParentX + X();
			AY = AParentY + Y();
		}

	public:
		inline T_BackgroundColor &GetBackgroundColor()
		{
			return BackgroundColor();
		}

		inline int16_t width(void) const
		{
			return C_OWNER.width() - int32_t( T_X::GetValue() );
		}

		inline int16_t height(void) const
		{
			return C_OWNER.height() - int32_t( T_Y::GetValue() );
		}

		inline void SetCursor( int16_t x, int16_t y )
		{		
			C_OWNER.SetCursor( x, y );
		}

/*
		static inline void ClearScreen( GraphicsElementColorGrayscaleDrawScene<T_OWNER, C_OWNER> &AImplementation, TUniColor color )
		{
			C_OWNER.ClearScreen( C_OWNER, color );
		}
*/
		static void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			if( !( color & 0xFF00 ))
				return;

			uint16_t AGray = color & 0xFF;
			color = ( color << 16 ) | ( AGray << 8 ) | AGray;

			C_OWNER.drawPixel( x, y, color );
		}

		static TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			uint32_t AColor = C_OWNER.GetPixelColor( x, y );
			uint8_t ARed = AColor >> 16;
			uint8_t AGreen = AColor >> 8;
			uint8_t ABlue = AColor >> 8;
			uint16_t AGray = 0.2989 * ARed + 0.5870 * AGreen + 0.1140 * ABlue + 0.5;
			AGray |= ( AColor >> 16 ) & 0xFF00;
			return AGray;
		}

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				T_Elements_Render::Call();

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return graphics::write( AImplementation, AConfig, AValue );
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
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
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
/*
	public:
		GraphicsElementColorGrayscaleDrawScene( const unsigned char *AFont ) :
			graphics( AFont )
		{
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif