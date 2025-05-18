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
#include <Mitov_Graphics_RGBW.h>

#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
  #include <NeoPixelBrightnessBus.h>
#else
//  #include <MitovEmbedded_Adafruit_NeoPixel/MitovEmbedded_Adafruit_NeoPixel.h>
  #include <Adafruit_NeoPixel.h>
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION, 
		T_IMPLEMENTATION &C_IMPLEMENTATION,
		typename T_FModified
	> class NeoPixelsBasic :
		public T_FModified
	{
	public:
		_V_PROP_( FModified )

	public:
		inline void SetPixelColor( int AIndex, TRGBWColor AColor )
		{
//			Serial.print( "SetPixelColor: " );
//			Serial.print( AIndex );
//			Serial.print( " " );
//			Serial.print( AColor.ToString() );

#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			RgbColor ARgbColor( AColor.Red, AColor.Green, AColor.Blue );
			C_IMPLEMENTATION.SetPixelColor( AIndex, ARgbColor );
#else
			C_IMPLEMENTATION.setPixelColor( AIndex, AColor.Red, AColor.Green, AColor.Blue, AColor.White );
#endif
//			C_IMPLEMENTATION.show();
//			delay( 200 );
			FModified() = true;
		}

		inline TRGBWColor GetPixelColor( int AIndex )
		{
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			RgbColor ARgbColor = C_IMPLEMENTATION.GetPixelColor( AIndex );
			return TRGBWColor( ARgbColor.R, ARgbColor.G, ARgbColor.B, 0 );
#else
			return TRGBWColor( C_IMPLEMENTATION.getPixelColor( AIndex ), true );
#endif
		}

/*
		inline void SystemLoopBegin()
		{
			for( int i = 0; i < PixelGroups.size(); ++i )
				PixelGroups[ i ]->PixelsClock();

		}
*/
		inline void SystemLoopEnd()
		{
			ClockInputPin_o_Receive( nullptr );
		}

		inline void SystemInit()
		{
//			FPixel.setPin( C_PIN_NUMBER );

#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			C_IMPLEMENTATION.Begin();
#else
			C_IMPLEMENTATION.begin();
#endif
		}

	protected:
		void IntSetBrightness( float AValue )
		{
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			C_IMPLEMENTATION.SetBrightness( AValue * 255 );
#else
			C_IMPLEMENTATION.setBrightness( AValue * 255 );
#endif
			FModified() = true;
		}

	public:
		void ClockInputPin_o_Receive( void * )
		{
			if( FModified() )
			{
//				Serial.println( "TEST11" );
//				Serial.println( "TEST12" );
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
				C_IMPLEMENTATION.Show();
#else
				C_IMPLEMENTATION.show();
#endif
				FModified() = false;
			}
		}

	public:
		inline NeoPixelsBasic()
		{
			FModified() = true;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION, 
		T_IMPLEMENTATION &C_IMPLEMENTATION,
		typename T_Brightness,
		typename T_FModified
	> class NeoPixels : public NeoPixelsBasic <
				T_IMPLEMENTATION, 
				C_IMPLEMENTATION,
				T_FModified
			>,
		public T_Brightness
	{
		typedef NeoPixelsBasic <
				T_IMPLEMENTATION, 
				C_IMPLEMENTATION,
				T_FModified
		> inherited;

	public:
		_V_PROP_( Brightness )

	public:
		void UpdateBrightness()
		{
			inherited::IntSetBrightness( Brightness() );
		}

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
//			FPixel.setPin( C_PIN_NUMBER );

			inherited::IntSetBrightness( Brightness() );
		}

	};
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsBasicInitialColorGroup :
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsBasicColorGroup :
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor );
		}

	public:
		void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	protected:
		void ApplyColors() {}

		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor == AValue )
				return;

			InitialColor() = AValue;
			ApplyColors(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void SystemStart()
		{
			inherited::SystemStart();
			IntSetColor( InitialColor().GetValue() );
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsGroup :
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	protected:
		TRGBWColor	FValues[ C_COUNT_PIXELS ];

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
			{
				FValues[ i ] = TRGBWColor( InitialColor().GetValue());
				C_OWNER.SetPixelColor( C_START_PIXEL + i, TRGBWColor( InitialColor().GetValue()) );
			}
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			TRGBWColor AValue = *(TRGBWColor *)_Data;
			if( FValues[ AIndex ] == AValue )
				return;

			FValues[ AIndex ] = AValue;
			C_OWNER.SetPixelColor( AIndex, AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_REPEAT_COUNT,
		uint16_t C_SUB_PIXEL_COUNT,
		uint16_t C_START_PIXEL
	> class NeoPixelsRepeatGroup : 
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	public:
		void SetPixelColor( int AIndex, TRGBWColor AColor )
		{
			for( int i = 0; i < C_REPEAT_COUNT; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + AIndex + i * C_SUB_PIXEL_COUNT, AColor );
		}

		TRGBWColor GetPixelColor( int AIndex )
		{
			return C_OWNER.GetPixelColor( C_START_PIXEL + AIndex );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsSingleColorGroup : 
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	protected:
		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor().GetValue() == AValue )
				return;

			InitialColor() = AValue;
			ApplyColorsAll(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void SystemStart()
		{
			ApplyColorsAll();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Brightness,
		typename T_InitialColor
	> class NeoPixelsSingleColorLed : 
		public T_Brightness,
		public T_InitialColor
	{
	public:
		_V_PROP_( Brightness )
		_V_PROP_( InitialColor )

	public:
		inline void SetBrightness( float AValue ) // Used for Owner Delegation
		{
			C_OWNER.SetBrightness( AValue );
		}

		inline void UpdateBrightness()
		{
			C_OWNER.SetBrightness( Brightness() );
		}

	public:
		void ApplyColorsAll()
		{
			C_OWNER.SetPixelColor( 0, Func::SwapRB( InitialColor().GetValue() ));
		}

	protected:
		void IntSetColor( TColor AValue )
		{
			if( InitialColor().GetValue() == AValue )
				return;

			InitialColor() = AValue;
			ApplyColorsAll(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TColor *)_Data );
		}

	public:
		inline void SystemStart()
		{
			ApplyColorsAll();
		}

	public:
		inline void SystemInit()
		{
			C_OWNER.SystemInit();
		}

		inline void SystemLoopEnd()
		{
			C_OWNER.SystemLoopEnd();
		}

	};
//---------------------------------------------------------------------------
/*
	class NeoPixelsReversedProperty
	{
	public:
		bool Reversed : 1;
		bool AllPixels : 1;

	public:
		NeoPixelsReversedProperty() :
			Reversed( false ),
			AllPixels( false )
		{
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_ColorOutputPin,
		typename T_InitialColor,
		typename T_Reversed,
		uint16_t C_START_PIXEL,
		typename T_StepInputPin_o_IsConnected
	> class NeoPixelsRunningColorGroup : 
		public T_ColorOutputPin,
		public T_InitialColor,
		public T_Reversed,
		public T_StepInputPin_o_IsConnected
	{
	public:
		_V_PIN_( ColorOutputPin )

	public:
		_V_PROP_( InitialColor )
		_V_PROP_( Reversed )

	public:
		_V_PROP_( StepInputPin_o_IsConnected )

	public:
		inline void SystemStart()
		{
			ApplyColorsAll();
		}

	public:
		inline void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	protected:
		void ApplyColors() {}

		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor().GetValue() == AValue )
				return;

			InitialColor() = AValue;
			ApplyColors(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void UpdateReversePixels()
		{
			if( Reversed().AllPixels() )
				ReversePixels();

		}

	public:
		void ReversePixels()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS / 2; ++i )
			{
				TRGBWColor AOldColor1 = C_OWNER.GetPixelColor( C_START_PIXEL + ( C_COUNT_PIXELS - i - 1 ));
				TRGBWColor AOldColor2 = C_OWNER.GetPixelColor( C_START_PIXEL + i );

				C_OWNER.SetPixelColor( C_START_PIXEL + i, AOldColor1 );
				C_OWNER.SetPixelColor( C_START_PIXEL + ( C_COUNT_PIXELS - i - 1 ), AOldColor2 );
			}
		}

	protected:
		void AnimatePixels()
		{
			if( Reversed().Reversed() )
			{
				TRGBWColor AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL );
				T_ColorOutputPin::SetPinValue( AOldColor );
				for( uint32_t i = 0; i < C_COUNT_PIXELS - 1; ++i )
				{
					AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL + i + 1 );
					C_OWNER.SetPixelColor( C_START_PIXEL + i, AOldColor );
				}

				C_OWNER.SetPixelColor( C_START_PIXEL + C_COUNT_PIXELS - 1, InitialColor().GetValue() );
			}

			else
			{
				TRGBWColor AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL + C_COUNT_PIXELS - 1 );
				T_ColorOutputPin::SetPinValue( AOldColor );
				for( int i = C_COUNT_PIXELS - 1; i--; )
				{
					AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL + i );
					C_OWNER.SetPixelColor( C_START_PIXEL + i + 1, AOldColor );
				}

				C_OWNER.SetPixelColor( C_START_PIXEL, InitialColor().GetValue() );
			}
//			FOwner.FModified = true;
		}

	public:
		inline void StepInputPin_o_Receive( void *_Data )
		{
			AnimatePixels();
		}

	public:
		inline void SystemLoopBegin()
		{
			if( StepInputPin_o_IsConnected() )
				return;

			AnimatePixels();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_FModified,
		typename T_InitialColor,
		typename T_InitialIndex,
		uint16_t C_START_PIXEL
	> class NeoPixelsColorPixelGroup : 
		public T_FModified,
		public T_InitialColor,
		public T_InitialIndex
	{
	public:
		_V_PROP_( InitialIndex )

	protected:
		_V_PROP_( FModified )

	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	public:
		inline void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	protected:
		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor().GetValue() == AValue )
				return;

			InitialColor() = AValue;
			ApplyColors(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex >= C_COUNT_PIXELS )
				AIndex = C_COUNT_PIXELS - 1;

			if( InitialIndex().GetValue() == AIndex )
				return;

			InitialIndex() = AIndex;
			FModified() = true;
		}

	protected:
		inline void ApplyColors()
		{
			FModified() = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FModified() )
			{
				C_OWNER.SetPixelColor( C_START_PIXEL + InitialIndex(), InitialColor().GetValue() );
//				FOwner.FModified = true;
				FModified() = false;
			}
		}

	public:
		inline NeoPixelsColorPixelGroup()
		{
			FModified() = true;
		}

	};
//---------------------------------------------------------------------------
	class TRGBWColorTransfrom
	{
	public:
		static const TUniColor BLACK = 0xFF00000000;

	public:
		inline static uint8_t	ExtractGray( TUniColor AColor )
		{
			return ( AColor & 0xFF );
		}

		inline static uint8_t	ExtractAlpha( TUniColor AColor )
		{
			return ( AColor >> 32 );
		}

		inline static TUniColor	ExtractColor( TUniColor AColor )
		{
			return ( AColor & 0xFFFFFFFF );
		}

		inline static uint32_t	MakeGray( TUniColor AColor )
		{
			uint32_t AGray = ( AColor & 0xFF );
			return ( 0xFF00000000 | ( AGray << 16 ) | ( AGray << 16 ) | ( AGray << 8 ) | AGray );
		}

		inline static TUniColor	MakeColor( TUniColor AColor )
		{
			uint64_t AAlpha = ( AColor & 0xFF000000 );
			return ( AAlpha << 8 ) | ( AColor & 0xFFFFFF );
		}

		inline static TUniColor	BlendColor( TUniColor AOldColor, TUniColor AColor, float AAlpha )
		{
			float ACoefficient = AAlpha / 255.0f;

			uint8_t AWhite = (( AOldColor >> 24 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 24 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t ARed = (( AOldColor >> 16 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 16 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t AGreen = (( AOldColor >> 8 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 8 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t ABlue = ( AOldColor & 0xFF ) * ( 1 - ACoefficient ) + ( AColor & 0xFF ) * ACoefficient + 0.5;

			return ( ( uint32_t( AWhite ) << 24 ) | ( uint32_t( ARed ) << 16 ) | ( uint32_t( AGreen ) << 8 ) | ABlue );
		}

		inline static TUniColor	TransformBackgroundColor( TUniColor AColor )
		{
			return AColor;
		}
	};
//---------------------------------------------------------------------------
	class TRGBColorTransfrom
	{
	public:
		static const TUniColor BLACK = 0xFF000000;

	public:
		inline static uint8_t	ExtractGray( TUniColor AColor )
		{
			return ( AColor & 0xFF );
		}

		inline static uint8_t	ExtractAlpha( TUniColor AColor )
		{
			return ( AColor >> 24 );
		}

		inline static TUniColor	ExtractColor( TUniColor AColor )
		{
			return ( AColor & 0xFFFFFF );
		}

		inline static uint32_t	MakeGray( TUniColor AColor )
		{
			uint16_t AGray = ( AColor & 0xFF );
			return ( 0xFF000000 | ( AColor << 16 ) | ( AGray << 8 ) | AGray );
		}

		inline static TUniColor	MakeColor( TUniColor AColor )
		{
			return AColor;
//			uint64_t AAlpha = ( AColor & 0xFF000000 );
//			return ( AAlpha << 8 ) | ( AColor & 0xFFFFFF );
		}

		inline static TUniColor	BlendColor( TUniColor AOldColor, TUniColor AColor, float AAlpha )
		{
			float ACoefficient = AAlpha / 255.0f;

			uint8_t ARed = (( AOldColor >> 16 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 16 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t AGreen = (( AOldColor >> 8 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 8 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t ABlue = ( AOldColor & 0xFF ) * ( 1 - ACoefficient ) + ( AColor & 0xFF ) * ACoefficient + 0.5;

			return ( ( uint32_t( ARed ) << 16 ) | ( uint32_t( AGreen ) << 8 ) | ABlue );
		}

		inline static TUniColor	TransformBackgroundColor( TUniColor AColor )
		{
			return (( AColor & 0xFF ) << 16 ) | ( AColor & 0x00FF00 ) | (( AColor >> 16 ) & 0xFF );
		}
	};
//---------------------------------------------------------------------------
	class TSequentialMatrixMap
	{
	public:
		inline static uint32_t	Remap( uint32_t AIndex )
		{
			return AIndex;
		}

	};
//---------------------------------------------------------------------------
//#if 0
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_BackgroundColor,
		uint16_t C_COUNT_PIXELS, 
		typename T_Elements_Render,
        typename T_FCursorX,
        typename T_FCursorY,
		int HEIGHT, 
		typename T_MATRIX_MAP, 
		typename T_Orientation,
		uint16_t C_START_PIXEL,
		typename T_COLOR_TRANSFORM, 
		typename T_Text,
		int WIDTH
	> class NeoPixels2DGraphicsGroup : 
		public GraphicsImpl<
			NeoPixels2DGraphicsGroup<
						T_OWNER, C_OWNER, 
						T_BackgroundColor,
						C_COUNT_PIXELS, 
						T_Elements_Render,
                        T_FCursorX,
                        T_FCursorY,
						HEIGHT,
						T_MATRIX_MAP, 
						T_Orientation,
						C_START_PIXEL,
						T_COLOR_TRANSFORM, 
						T_Text,
						WIDTH
					>, 
				T_BackgroundColor,
				T_Text
			>,
        public T_FCursorX,
        public T_FCursorY,
		public T_Orientation
//		public T_Text
	{
		typedef NeoPixels2DGraphicsGroup T_SELF;

		typedef GraphicsImpl<
				T_SELF,
				T_BackgroundColor,
				T_Text
			>	graphics;

	public:
		_V_PROP_( Orientation )

	public:
		_V_PROP_( Text )

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
		constexpr inline static bool ImplementsPolygon()
		{
			return false;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

		inline int16_t width(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return WIDTH;

			return HEIGHT;
		}

		inline int16_t height(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return HEIGHT;

			return WIDTH;
		}

		static inline TUniColor NonTransparent( TUniColor color )
		{
			return 0xFF000000 | ::Mitov::Func::SwapRB( color );
		}

	public:
		inline T_OWNER &GetOwner() { return C_OWNER; }

	public:
		inline void SetBrightness( float AValue ) // Used for Owner Delegation
		{
			C_OWNER.SetBrightness( AValue );
		}

	public:
		inline void RefreshInputPin_o_Receive( void *_Data ) // Used for Owner Delegation
		{
			C_OWNER.ClockInputPin_o_Receive( _Data );
		}

	public:
		static inline void drawGrayscalePixel( T_SELF &AImplementation, int16_t x, int16_t y, TUniColor color)
		{
			AImplementation.drawPixel( x, y, T_COLOR_TRANSFORM::MakeGray( color ));
		}

		static inline void fillGrayscaleRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			fillGrayscaleRect( AImplementation, x, y, w, h, T_COLOR_TRANSFORM::MakeGray( color ));
		}

		static inline void drawColorPixel( T_SELF &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			AImplementation.drawPixel( x, y, T_COLOR_TRANSFORM::MakeColor( color ) );
		}

		static inline void fillColorRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
//			Serial.println( "TEST88" );
			fillGrayscaleRect( AImplementation, x, y, w, h, T_COLOR_TRANSFORM::MakeColor( color ) );
		}

		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
//			Serial.println( "GRAPHICS" );
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return;

//			Serial.println( uint32_t( color ), HEX );
//			Serial.print( "drawPixel: " );
//			color = 0xFFFFFF0000;
			uint8_t AAlpha = T_COLOR_TRANSFORM::ExtractAlpha( color );
			if( AAlpha == 0 )
				return;

//			Serial.print( x ); Serial.print( ":" ); Serial.print( y ); Serial.print( " = " ); Serial.print( uint32_t( AAlpha ), HEX ); Serial.print( " " ); Serial.println( uint32_t( color ), HEX ); 

			switch ( Orientation() ) 
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

			color = T_COLOR_TRANSFORM::ExtractColor( color );

//			Serial.print( "Color: " ); Serial.println( uint32_t( color ), HEX ); 

			uint32_t APixelIndex = C_START_PIXEL + T_MATRIX_MAP::Remap( x + y * WIDTH );

			if( AAlpha < 255 )
			{
				TUniColor AOldColor = C_OWNER.GetPixelColor( APixelIndex );
				color = T_COLOR_TRANSFORM::BlendColor( AOldColor, color, AAlpha );
			}

//			color = 0xFFFF0000;
//			Serial.print( C_START_PIXEL + x + y * WIDTH ); Serial.print( " = " ); Serial.println( uint32_t( color ));
			C_OWNER.SetPixelColor( APixelIndex, color );
//			FBuffer[ x + y * WIDTH ] = color;
//			FModified = true;
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return T_COLOR_TRANSFORM::BLACK;

			switch ( Orientation() ) 
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

			return TUniColor( C_OWNER.GetPixelColor( C_START_PIXEL + T_MATRIX_MAP::Remap( x + y * WIDTH ) )) | T_COLOR_TRANSFORM::BLACK;
//			return TUniColor( FBuffer[ x + y * WIDTH ] ) | 0xFF00000000;
		}

/*
	protected:
		inline void RenderElements()
		{
			graphics::RenderElements();
		}
*/
	public:
		inline void SystemStart()
		{
//			graphics::setTextSize( Text().Size() );
//			graphics::setTextColor( Text().Color(), Text().BackgroundColor().GetValue() );
//			graphics::setTextWrap( Text().Wrap() );

			graphics::ClearScreen( *this, T_COLOR_TRANSFORM::TransformBackgroundColor( graphics::BackgroundColor().GetValue() ) | T_COLOR_TRANSFORM::BLACK );
//			graphics::fillRect( *this, 0, 0, width(), height(), graphics::BackgroundColor | T_COLOR_TRANSFORM::BLACK );
//			Serial.println( "SystemStart" );
			graphics::RenderElements();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );
		}

/*
		virtual void PixelsClock()
		{
			if( FModified )
			{
				for( int i = 0; i < WIDTH * HEIGHT; ++i )
				{
//					Serial.print( i ); Serial.print( " = " ); Serial.println( FBuffer[ i ], HEX ); 
					C_OWNER.SetPixelColor( C_START_PIXEL + i, FBuffer[ i ] );
				}

				FModified = false;
			}
		}
*/
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

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		uint8_t C_INDEX,
		typename T_Enabled,
		typename T_InitialValue
	> class NeoPixelSingleLED :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				C_OWNER.SetPixelColor( C_INDEX, TRGBWColor( InitialValue().GetValue() ));

			else
				C_OWNER.SetPixelColor( C_INDEX, TRGBWColor( 0 ));

//			C_OWNER.SetChannelValue( C_INDEX, 1 - InitialValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;
			SystemStart();
		}
	};
//---------------------------------------------------------------------------
	template <
//		uint8_t C_Pin0,
		typename T_OWNER, T_OWNER &C_OWNER,
//		uint8_t C_Pin1
		uint8_t C_COUNT,
		uint8_t C_START
	> class GroveNeoPixelLEDs
	{
	public:
		inline void SetPixelColor( int AIndex, TRGBWColor AColor )
		{
//			Serial.println( AIndex );
//			Serial.println( AColor.ToString() );
			if( AIndex < C_COUNT )				
				C_OWNER.SetPixelColor( C_START + AIndex, AColor );

		}

		inline TRGBWColor GetPixelColor( int AIndex )
		{
			if( AIndex < C_COUNT )				
				return C_OWNER.GetPixelColor( C_START + AIndex );

			return TRGBWColor();
		}


	public:
		inline void SystemInit()
		{
			C_OWNER.SystemInit();
		}

		inline void SystemLoopEnd()
		{
			C_OWNER.SystemLoopEnd();
		}

};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_BackgroundColor,
		typename T_Brightness,
		uint16_t C_COUNT_PIXELS, 
		typename T_Elements_Render,
		typename T_Enabled,
        typename T_FCursorX,
        typename T_FCursorY,
		int HEIGHT, 
		typename T_MATRIX_MAP, 
		typename T_Orientation,
		typename T_COLOR_TRANSFORM, 
		typename T_Text,
		int WIDTH
	> class ArduinoNeoPixels2DGraphicsModule : 
		public NeoPixels2DGraphicsGroup <
				T_OWNER, 
				C_OWNER, 
				T_BackgroundColor,
				C_COUNT_PIXELS, 
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				HEIGHT,
				T_MATRIX_MAP,
				T_Orientation,
				0,
				T_COLOR_TRANSFORM, 
				T_Text,
				WIDTH
			>,
		public T_Enabled
	{
		typedef NeoPixels2DGraphicsGroup <
				T_OWNER, 
				C_OWNER, 
				T_BackgroundColor,
				C_COUNT_PIXELS, 
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				HEIGHT,
				T_MATRIX_MAP,
				T_Orientation,
				0,
				T_COLOR_TRANSFORM, 
				T_Text,
				WIDTH
			> inherited;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Brightness )

	public:
		inline void UpdateBrightness()
		{
			C_OWNER.IntSetBrightness( Brightness() );
		}

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
//			FPixel.setPin( C_PIN_NUMBER );

			C_OWNER.SystemInit();
			C_OWNER.IntSetBrightness( Brightness() );
		}

		inline void SystemLoopEnd()
		{
			C_OWNER.SystemLoopEnd();
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
	class Mitov_NeoGbrFeature : public Neo3ByteElementsNoSettings
	{
	public:
		static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
		{
			uint8_t* p = getPixelAddress(pPixels, indexPixel);

			*p++ = color.G;
			*p++ = color.B;
			*p = color.R;
		}

		static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
		{
			ColorObject color;
			const uint8_t* p = getPixelAddress(pPixels, indexPixel);

			color.G = *p++;
			color.B = *p++;
			color.R = *p;

			return color;
		}
    
		static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
		{
			ColorObject color;
			const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

			color.G = pgm_read_byte(p++);
			color.B = pgm_read_byte(p++);
			color.R = pgm_read_byte(p);

			return color;
		}
    
	};
//---------------------------------------------------------------------------
	class Mitov_NeoGrbFeature : public Neo3ByteElementsNoSettings
	{
	public:
		static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
		{
			uint8_t* p = getPixelAddress(pPixels, indexPixel);

			*p++ = color.G;
			*p++ = color.R;
			*p = color.B;
		}

		static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
		{
			ColorObject color;
			const uint8_t* p = getPixelAddress(pPixels, indexPixel);

			color.G = *p++;
			color.R = *p++;
			color.B = *p;

			return color;
		}
    
		static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
		{
			ColorObject color;
			const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

			color.G = pgm_read_byte(p++);
			color.R = pgm_read_byte(p++);
			color.B = pgm_read_byte(p);

			return color;
		}
    
	};
#endif // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
//---------------------------------------------------------------------------
	class NeoPixelSingleLEDModule_GBRAdapter
	{
	public:
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
		NeoPixelBrightnessBus<Mitov_NeoGbrFeature, Neo800KbpsMethod>	FPixel;
#else // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
		Adafruit_NeoPixel	FPixel;
#endif // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__

	public:
		NeoPixelSingleLEDModule_GBRAdapter( uint16_t APinNumber ) :
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			FPixel( 1, APinNumber )
#else // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			FPixel( 1, APinNumber, NEO_GBR + NEO_KHZ800 )
#endif // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
		{
		}

	};
//---------------------------------------------------------------------------
	class NeoPixelSingleLEDModule_GRBAdapter
	{
	public:
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
		NeoPixelBrightnessBus<Mitov_NeoGrbFeature, Neo800KbpsMethod>	FPixel;
#else // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
		Adafruit_NeoPixel	FPixel;
#endif // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__

	public:
		NeoPixelSingleLEDModule_GRBAdapter( uint16_t APinNumber ) :
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			FPixel( 1, APinNumber )
#else // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			FPixel( 1, APinNumber, NEO_GRB + NEO_KHZ800 )
#endif // __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		uint8_t C_PIN,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_TYPE
	> class NeoPixelSingleLEDModule :
		public T_TYPE,
		public T_Brightness,
		public T_Enabled,
		public T_InitialValue
	{
		typedef T_TYPE inherited;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Brightness )
		_V_PROP_( InitialValue )

	public:
		inline void SetPixelColor( TColor AColor )
		{
//			Serial.println( AColor.ToString() );
			if( Enabled() )
				IntSetPixelColor( 0, AColor.Red, AColor.Green, AColor.Blue );

			else
				IntSetPixelColor( 0, 0, 0, 0 );

//			inherited::FPixel.setPixelColor( AIndex, 255, 0, 0 );
		}

	public:
		inline void UpdateOutputs()
		{
			SetPixelColor( InitialValue() );
		}

		void UpdateBrightness()
		{
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			inherited::FPixel.SetBrightness( Brightness().GetValue() * 255 + 0.5 );
#else
			inherited::FPixel.setBrightness( Brightness().GetValue() * 255 + 0.5 );
#endif
		}

	protected:
		inline void IntSetPixelColor( uint8_t AWhite, uint8_t ARed, uint8_t AGreen, uint8_t ABlue )
		{
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			RgbColor ARgbColor( ARed, AGreen, ABlue );
			inherited::FPixel.SetPixelColor( 0, ARgbColor );
			inherited::FPixel.Show();
#else
			inherited::FPixel.setPixelColor( 0, ARed, AGreen, ABlue );
			inherited::FPixel.show();
#endif
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;
			UpdateOutputs();
		}

	public:
		inline void SystemStart()
		{
//			inherited::FPixel.setPin( C_PIN );

#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			inherited::FPixel.Begin();
#else
			inherited::FPixel.begin();
#endif
//			inherited::FPixel.updateType(NEO_GRB + NEO_KHZ800);
//			inherited::FPixel.updateLength(1);
			UpdateBrightness();
			UpdateOutputs();
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			inherited::FPixel.Show();
#else
			inherited::FPixel.show();
#endif
		}

	public:
		inline NeoPixelSingleLEDModule() :
			inherited( C_PIN )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		uint8_t C_PIN,
		uint8_t C_POWER_PIN,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_TYPE
	> class NeoPixelSinglePowerControlLEDModule :
		public NeoPixelSingleLEDModule <
				T_Brightness,
				C_PIN,
				T_Enabled,
				T_InitialValue,
				T_TYPE
			>
	{
		typedef NeoPixelSingleLEDModule <
				T_Brightness,
				C_PIN,
				T_Enabled,
				T_InitialValue,
				T_TYPE
			> inherited;

	public:
		inline void UpdateEnabled()
		{
			if( inherited::Enabled() )
				digitalWrite( C_POWER_PIN, HIGH );

			inherited::UpdateOutputs();

			if( ! inherited::Enabled().GetValue() )
				digitalWrite( C_POWER_PIN, LOW );

		}

		inline void SystemStart()
		{
			pinMode( C_POWER_PIN, OUTPUT );
			digitalWrite( C_POWER_PIN, inherited::Enabled() ? HIGH : LOW );

			inherited::SystemStart();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

