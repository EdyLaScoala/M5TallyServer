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
#include <Mitov_Graphics_Grayscale.h>
#include <Mitov_Graphics_Monochrome.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoLedDriverIS31FL3731AudioGainMode { igmDisabled, igmSlow, igmFast };
	enum TArduinoLedDriverIS31FL3731AudioGain { isg0dB, isg3dB, isg6dB, isg9dB, isg12dB, isg15dB, isg18dB, isg21dB };
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Gain,
		typename T_GainMode
	> class TArduinoLedDriverIS31FL3731AudioModulation :
		public T_Enabled,
		public T_Gain,
		public T_GainMode
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( GainMode )
		_V_PROP_( Gain )

	};
//---------------------------------------------------------------------------
	namespace IS31FL3731Const
	{
		const uint8_t WIDTH = 16;
		const uint8_t HEIGHT = 9;

		const uint8_t	REG_CONFIG  = 0x00;
		const uint8_t	REG_CONFIG_PICTUREMODE = 0x00;
		const uint8_t	REG_CONFIG_AUTOPLAYMODE = 0x08;
		const uint8_t	REG_CONFIG_AUDIOPLAYMODE = 0x10;

		const uint8_t	REG_PICTUREFRAME = 0x01;

		const uint8_t	REG_SHUTDOWN = 0x0A;

		const uint8_t	COMMANDREGISTER = 0xFD;
		const uint8_t	AUTO_PLAY_CONTROL_REGISTER1 = 0x02;
		const uint8_t	AUTO_PLAY_CONTROL_REGISTER2 = 0x03;
		const uint8_t	DISPLAY_OPTION_REGISTER = 0x05;
		const uint8_t	AUDIO_SYNCHRONIZATION_REGISTER = 0x06;
		const uint8_t	BREATH_CONTROL_REGISTER1 = 0x08;
		const uint8_t	BREATH_CONTROL_REGISTER2 = 0x09;
		const uint8_t	AGC_CONTROL_REGISTER = 0x0B;
		const uint8_t	AUDIO_ADC_RATE_REGISTER = 0x0C;
		const uint8_t	BANK_FUNCTIONREG = 0x0B;    // helpfully called 'page nine'
	}
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_AudioModulation,
		typename T_BlinkPeriod,
		uint32_t COUNT_Elements,
		typename T_Elements_AssignFrames,
		typename T_Elements_SystemInitStart,
		typename T_Enabled		
	> class DisplayIS31FL3731 :
		public T_Address,
		public T_AudioModulation,
		public T_BlinkPeriod,
		public T_Enabled		
	{
	public:
        _V_PROP_( Address )
        _V_PROP_( Enabled )
		_V_PROP_( BlinkPeriod )
		_V_PROP_( AudioModulation )

	public:
		inline void SelectBank( uint8_t b ) 
		{
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), IS31FL3731Const::COMMANDREGISTER, b );
		}

		void WriteRegister8(uint8_t b, uint8_t reg, uint8_t data)
		{
			SelectBank( b );
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), reg, data );
		}

		static inline void I2C_write( uint8_t AValue )
		{
			C_I2C.write( AValue );
		}

		inline void I2C_beginTransmission()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
		}

		static inline void I2C_endTransmission()
		{
			C_I2C.endTransmission();
		}
		
	public:
		void UpdateAssignFrames()
		{
			uint8_t AFrame = 0;
			for( int i = 0; i < COUNT_Elements; ++i )
				T_Elements_AssignFrames::Call( i, AFrame );
//				FElements[ i ]->AssignFrames( AFrame );
		}


		void ReassignFrames( uint32_t AStartIndex )
		{
			uint8_t AFrame = 0;
			for( uint32_t i = 0; i < COUNT_Elements; ++i )
				T_Elements_AssignFrames::Call( ( i + AStartIndex ) % COUNT_Elements, AFrame );
//				FElements[ ( i + AStartIndex ) % COUNT_Elements ]->AssignFrames( AFrame );

		}

		void UpdateAudioModulation()
		{
			uint8_t AData;

			if( AudioModulation().Enabled() )
				AData = ( 0b10 | (( AudioModulation().GainMode().GetValue() == igmFast ) ? 0b100 : 0 ) | AudioModulation().Gain().GetValue() );

			else
				AData = 0;

			WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::AGC_CONTROL_REGISTER, AData );
			WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::AUDIO_SYNCHRONIZATION_REGISTER, AudioModulation().Enabled() ? 1 : 0 );
		}

/*
		void UpdateBlink()
		{
//			return;
			uint8_t	AData = 0b100000 | (( Blink.Enabled ) ? 0b1000 : 0 ) | Blink.Period;
			WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::DISPLAY_OPTION_REGISTER, AData );
		}
*/
	public:
/*
		inline void SystemLoopEnd()
		{
			for( int i = 0; i < FElements.size(); ++i )
				FElements[ i ]->Refresh();

		}
*/
		inline void SystemInit()
		{
			// shutdown
			WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::REG_SHUTDOWN, 0x00 );

			if( ! Enabled() )
				return;

			delay(10);

			// out of shutdown
			WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::REG_SHUTDOWN, 0x01 );




//			uint8_t	AData = 0b100000 | (( Blink.Enabled ) ? 0b1000 : 0 ) | Blink.Period;
//			uint8_t	AData = 0b100000 | 0b1000;
			uint8_t	AData = 0b1000 | BlinkPeriod().GetValue();
			WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::DISPLAY_OPTION_REGISTER, AData );

/*
			SelectBank( 0 );

			FWire.beginTransmission( Address );

			FWire.write( 0x12 );
			FWire.write( 2 );
			for( int i = 1; i < 16; ++i)
				FWire.write( 0 );

			FWire.endTransmission();
*/

			UpdateAssignFrames();
//			UpdateBlink();
			UpdateAudioModulation();

//			UpdateFadeEffect();
//			display();
		}

		inline void SystemStart()
		{
			bool AStarted = false;
			T_Elements_SystemInitStart::Call( AStarted );

//			for( int i = 0; i < FElements.size(); ++i )
//				FElements[ i ]->SystemInitStart( AStarted );
				
			for (uint8_t f=0; f<8; f++)
				for (uint8_t i=0; i<=0x11; i++)
					WriteRegister8( f, i, 0xff );     // each 8 LEDs on

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_ExtinguishTime,
		typename T_FadeIn,
		typename T_FadeOut
	> class TArduinoLedDriverIS31FL3731FadeEffect :
		public T_Enabled,
		public T_ExtinguishTime,
		public T_FadeIn,
		public T_FadeOut
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( FadeIn )
		_V_PROP_( FadeOut )
		_V_PROP_( ExtinguishTime )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BackgroundColor,
		typename T_Elements_Render,
		typename T_Enabled,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_FModified,
		typename T_Orientation,
		typename T_Text
	> class DisplayIS31FL3731BasicBlink :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void Operation( bool AIsPopulateReg )
		{
			if( AIsPopulateReg )
				PopulateReg();

		}

		inline void PopulateReg()
		{
			if( Enabled() )
			{
				for( int i = 0; i < 18; ++i)
					C_OWNER.I2C_write( 0xFF );

			}

			else
			{
				for( int i = 0; i < 18; ++i)
					C_OWNER.I2C_write( 0 );

			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BackgroundColor,
		typename T_Elements_Render,
		typename T_Enabled,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_FModified,
		typename T_Orientation,
		typename T_Text
	> class DisplayIS31FL3731Blink :
		public GraphicsImpl<
			DisplayIS31FL3731Blink <
				T_OWNER, C_OWNER,
				T_BackgroundColor,
				T_Elements_Render,
				T_Enabled,
				T_FCursorX,
				T_FCursorY,
				T_FModified,
				T_Orientation,
				T_Text
			>, 
			T_BackgroundColor,
			T_Text
		>,
		public T_Enabled,
		public T_FCursorX,
		public T_FCursorY,
		public T_FModified,
//		public T_Text,
		public T_Orientation
	{
		typedef DisplayIS31FL3731Blink T_SELF;

		typedef GraphicsImpl<
				T_SELF,
				T_BackgroundColor,
				T_Text
			> graphics;

//	public:
//		_V_PROP_( Text )

	public:
		_V_PROP_( Orientation )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( FCursorX )
		_V_PROP_( FCursorY )

	protected:
		_V_PROP_( FModified )

	public:
		uint8_t	FPicture[ IS31FL3731Const::WIDTH * ( IS31FL3731Const::HEIGHT + 7 ) / 8 ] = { 0 };

	public:
		void SetCursor( int16_t x, int16_t y )
		{
			FCursorX() = x;
			FCursorY() = y;
		}

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::Call();
		}

	public:
		inline void Operation( bool AIsPopulateReg )
		{
			if( AIsPopulateReg )
				PopulateReg();

			else
				SetFrame();

		}

		inline void PopulateReg()
		{
			if( Enabled() )
			{
				for( int i = 0; i < 18; ++i)
					C_OWNER.I2C_write( FPicture[ i ] );

			}

			else
			{
				for( int i = 0; i < 18; ++i)
					C_OWNER.I2C_write( 0 );

			}
		}

		inline void SetFrame()
		{
//			graphics::setTextSize( Text().Size() );
//			graphics::setTextColor( Text().Color(), Text().BackgroundColor().GetValue() );
//			graphics::setTextWrap( Text().Wrap() );

			T_Elements_Render::Call();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );
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
				return IS31FL3731Const::WIDTH;

			return IS31FL3731Const::HEIGHT;
		}

		inline int16_t height(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return IS31FL3731Const::HEIGHT;

			return IS31FL3731Const::WIDTH;
		}

	public:
		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			if( color == tmcNone )
				return;

			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return;

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = IS31FL3731Const::WIDTH - x - 1;
					break;

				case goDown:
					x = IS31FL3731Const::WIDTH - x - 1;
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;
			}

			// x is which column
			switch (color) 
			{
				case tmcWhite :		FPicture[(x/8) + y * ( IS31FL3731Const::WIDTH / 8 ) ] |=  (1 << (x&7)); break;
				case tmcBlack :		FPicture[(x/8) + y * ( IS31FL3731Const::WIDTH / 8 ) ] &= ~(1 << (x&7)); break; 
				case tmcInvert :	FPicture[(x/8) + y * ( IS31FL3731Const::WIDTH / 8 ) ] ^=  (1 << (x&7)); break; 
			}

			FModified() = true;
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return false;

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = IS31FL3731Const::WIDTH - x - 1;
					break;

				case goDown:
					x = IS31FL3731Const::WIDTH - x - 1;
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;
			}

			return (( FPicture[ ( x / 8 ) + y * ( IS31FL3731Const::WIDTH / 8 ) ] & ( 1 << ( x & 0b111 ))) != 0 );
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

	public:
		inline DisplayIS31FL3731Blink()
		{
			FModified() = true;
//			memset( FPicture, 0, sizeof( FPicture ));
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_INSTANCE,
		typename T_BackgroundColor,
		typename T_Blink_Operation,
		typename T_Elements_Render,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_FModified,
		typename T_Orientation,
		typename T_Text
	> class DisplayIS31FL3731BasicPictureElement :
		public GraphicsGrayscale<
				DisplayIS31FL3731BasicPictureElement<
					T_OWNER, C_OWNER,
					T_INSTANCE,
					T_BackgroundColor,
					T_Blink_Operation,
					T_Elements_Render,
                    T_FCursorX,
                    T_FCursorY,
					T_FModified,
					T_Orientation,
					T_Text
				>,
				T_BackgroundColor,
				T_Text
			>,
//		public T_Blink,
        public T_FCursorX,
        public T_FCursorY,
		public T_FModified,
		public T_Orientation
//		public T_Text
	{
		typedef DisplayIS31FL3731BasicPictureElement T_SELF;

		typedef GraphicsGrayscale<
				T_SELF,
				T_BackgroundColor,
				T_Text
			> inherited;

//	public:
//		TGraphicsGrayscaleTextSettings	Text;

	protected:
		uint8_t	FPicture[ IS31FL3731Const::WIDTH * IS31FL3731Const::HEIGHT ] = { 0 };

	public:
		_V_PROP_( FCursorX )
		_V_PROP_( FCursorY )

	public:
		_V_PROP_( Orientation )
//		_V_PROP_( Blink )

	protected:
		_V_PROP_( FModified )

//	public:
//		DisplayIS31FL3731BasicBlink<T_INSTANCE> *FBlink = nullptr;

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
		static inline void I2C_write( uint8_t AValue )
		{
			T_OWNER::I2C_write( AValue );
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
				return IS31FL3731Const::WIDTH;

			return IS31FL3731Const::HEIGHT;
		}

		inline int16_t height(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return IS31FL3731Const::HEIGHT;

			return IS31FL3731Const::WIDTH;
		}

	public:
		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return;

//			Serial.println( color );
			uint8_t AAlpha = color >> 8;
			if( AAlpha == 0 )
				return;

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = IS31FL3731Const::WIDTH - x - 1;
					break;

				case goDown:
					x = IS31FL3731Const::WIDTH - x - 1;
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;
			}  

			// charlie wing is smaller
			//if (x > 15) return;
			//if (y > 7) return;

/*
			if (x > 7) {
			x=15-x;
			y += 8;
			} else {
			y = 7-y;
			}

			_swap_int16_t(x, y);

 
			if ((x < 0) || (x >= 16)) return;
			if ((y < 0) || (y >= 9)) return;
			if (color > 255) color = 255; // PWM 8bit max
*/
//			setLEDPWM(x + y*16, color, _frame);
//			C_OWNER.WriteRegister8( ( FFrame ) ? 0 : 1 , 0x24 + x + y * IS31FL3731Const::WIDTH, color );

			color &= 0xFF;
//			Serial.println( AAlpha );

			if( AAlpha < 255 )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				color = (color * ACoefficient + FPicture[ x + y * IS31FL3731Const::WIDTH ] * ( 1 - ACoefficient )) + 0.5;
			}
//				color = MitovMax( color , FPicture[ x + y * IS31FL3731Const::WIDTH ] );

//			Serial.println( "Color" );
//			Serial.println( color );
			FPicture[ x + y * IS31FL3731Const::WIDTH ] = color;
			FModified() = true;
			return;
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return 0xFF00;

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = IS31FL3731Const::WIDTH - x - 1;
					break;

				case goDown:
					x = IS31FL3731Const::WIDTH - x - 1;
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = IS31FL3731Const::HEIGHT - y - 1;
					break;
			}  

			return FPicture[ x + y * IS31FL3731Const::WIDTH ] | 0xFF00;
		}

	protected:
		void RenderImage()
		{
			uint8_t *AData = FPicture;
			for( int y = 0; y < IS31FL3731Const::HEIGHT; ++y )
			{
				C_OWNER.I2C_beginTransmission();
				C_OWNER.I2C_write( 0x24 + y * IS31FL3731Const::WIDTH );
				for( int x = 0; x < IS31FL3731Const::WIDTH; ++x )
					T_OWNER::I2C_write( *AData ++ );

				T_OWNER::I2C_endTransmission();
			}

			C_OWNER.I2C_beginTransmission();
			C_OWNER.I2C_write( 0x12 );
//			bool APopulate = ( FBlink != nullptr );
//			if( APopulate )
//				APopulate = FBlink->Enabled;

			T_Blink_Operation::Call( true );
//			if( APopulate )
//				Blink().PopulateReg( *this );

//			else
//			{
//				for( int i = 0; i < 18; ++i)
//					C_OWNER.I2C_write( 0 );

//			}

			T_OWNER::I2C_endTransmission();
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
		inline DisplayIS31FL3731BasicPictureElement()
		{
			FModified() = false;
//			memset( FPicture, 0, sizeof( FPicture ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BackgroundColor,
		typename T_Blink_Operation,
		typename T_Elements_Render,
		typename T_Enabled,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_FModified,
		typename T_Orientation,
		typename T_Text
	> class DisplayIS31FL3731FrameElement :
		public DisplayIS31FL3731BasicPictureElement<
			T_OWNER, C_OWNER,
			DisplayIS31FL3731FrameElement<
					T_OWNER, C_OWNER,
					T_BackgroundColor,
					T_Blink_Operation,
					T_Elements_Render,
					T_Enabled,
                    T_FCursorX,
                    T_FCursorY,
					T_FModified,
					T_Orientation,
					T_Text
				>,
				T_BackgroundColor,
				T_Blink_Operation,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_FModified,
				T_Orientation,
				T_Text
			>
//		public T_Text
	{
		typedef DisplayIS31FL3731BasicPictureElement<
                T_OWNER, C_OWNER,
                DisplayIS31FL3731FrameElement,
                T_BackgroundColor,
                T_Blink_Operation,
                T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
                T_FModified,
                T_Orientation,
                T_Text
			> inherited;

//	public:
//		_V_PROP_( Text )

	protected:
		uint8_t	FFrame;

	public:
		void SetFrame( uint8_t AFrame )
		{
//			inherited::setTextSize( Text().Size() );
//			inherited::setTextColor( Text().Color(), Text().BackgroundColor().GetValue() );
//			inherited::setTextWrap( Text().Wrap() );

			T_Elements_Render::Call();
//			for( int i = 0; i < inherited::FElements.size(); ++ i )
//				inherited::FElements[ i ]->Render( false );
			T_Blink_Operation::Call( false );
//			if( inherited::FBlink )
//				inherited::FBlink->SetFrame();

			FFrame = AFrame;
			Render();
		}

		void Render()
		{
//			Serial.println( "display" );
//			Serial.println( ( FFrame ) ? FFrame0 : FFrame1 );
//			T_OWNER::SelectBank( IS31FL3731Const::BANK_FUNCTIONREG );
			C_OWNER.SelectBank( FFrame );
			inherited::RenderImage();

//			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::REG_PICTUREFRAME, ( FFrame ) ? FFrame0 : FFrame1 );
			inherited::FModified() = false;
		}

//	public:
//		void RefreshInputPin_o_Receive( void * )
//		{
//			if( FModified() )
//				Render();
//		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_SamplingInterval
	> class TArduinoLedDriverIS31FL3731AudioControl :
		public T_Enabled,
		public T_SamplingInterval
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( SamplingInterval )

	public:
		uint8_t GetRateValue()
		{
			uint16_t AValue = float( SamplingInterval().GetValue() ) / 64 + 0.5;
			return AValue;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_AudioControl,
		uint32_t COUNT_Frames,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FFrameAssigned,
		typename T_FFrameStart,
		typename T_FadeEffect,
		typename T_FrameDelay,
		typename T_Frames_SetFrame,
		uint32_t C_INDEX,
		typename T_Loops
	> class DisplayIS31FL3731AnimationElement :
		public T_AudioControl,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FFrameAssigned,
		public T_FFrameStart,
		public T_FadeEffect,
		public T_FrameDelay,
		public T_Loops
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( AudioControl )
		_V_PROP_( FadeEffect )
		_V_PROP_( Loops )
		_V_PROP_( FrameDelay )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FFrameAssigned )
		_V_PROP_( FFrameStart )

	public:
		static inline void SelectBank( uint8_t b ) 
		{
			C_OWNER.SelectBank( b );
		}

		static inline void I2C_write( uint8_t AValue )
		{
			C_OWNER.I2C_write( AValue );
		}

		static inline void I2C_beginTransmission()
		{
			C_OWNER.I2C_beginTransmission();
		}

		static inline void I2C_endTransmission()
		{
			T_OWNER::I2C_endTransmission();
		}
		
	public:
		void UpdateFadeEffect()
		{
			uint8_t	AData = ( FadeEffect().FadeOut().GetValue() << 4 ) | FadeEffect().FadeIn().GetValue();
			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::BREATH_CONTROL_REGISTER1, AData );

			AData = ( Enabled().GetValue() ? 0b10000 : 0 ) | FadeEffect().ExtinguishTime().GetValue();
			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::BREATH_CONTROL_REGISTER2, AData );
		}

		void UpdateLoop()
		{
			if( AudioControl().Enabled() )
				C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::AUDIO_ADC_RATE_REGISTER, AudioControl().GetRateValue() );


			uint8_t	AData = ( ( AudioControl().Enabled().GetValue() ) ? IS31FL3731Const::REG_CONFIG_AUDIOPLAYMODE : IS31FL3731Const::REG_CONFIG_AUTOPLAYMODE ) | FFrameStart().GetValue();

			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::REG_CONFIG, AData );

			AData = ( Loops().GetValue() << 4 ) | ( COUNT_Frames & 0b111 );
			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::AUTO_PLAY_CONTROL_REGISTER1, AData );

			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::AUTO_PLAY_CONTROL_REGISTER2, FrameDelay().GetValue() );
		}

	public:
		inline void SystemInitStart( bool &AStarted )
		{
			if( ! Enabled() )
				return;

			for( int i = 0; i < COUNT_Frames; ++ i )
				T_Frames_SetFrame::Call( i, FFrameStart().GetValue() + i );

			if( AStarted )
				return;

			if( ClockInputPin_o_IsConnected() )
				return;

			AStarted = true;

			ClockInputPin_o_Receive( nullptr );
		}

		inline void AssignFrames( uint8_t &AFrame )
		{
			if( AFrame > 7 - MitovMin<uint8_t>( 7, COUNT_Frames ) )
			{
				FFrameAssigned() = false;
				return;
			}

			FFrameAssigned() = true;
			FFrameStart() = AFrame;
			AFrame += COUNT_Frames;
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			if( ! FFrameAssigned() )
				C_OWNER.ReassignFrames( C_INDEX );

			UpdateFadeEffect();
			UpdateLoop();
			// picture mode
		}

	public:
		inline DisplayIS31FL3731AnimationElement()
		{
			FFrameAssigned() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BackgroundColor,
		typename T_Blink_Operation,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Elements_Render,
		typename T_Enabled,
        typename T_FCursorX,
        typename T_FCursorY,
		typename T_FFrame,
		typename T_FFrame0,
		typename T_FFrame1,
		typename T_FFrameAssigned,
		typename T_FModified,
		typename T_FadeEffect,
		typename T_Orientation,
		typename T_RefreshInputPin_o_IsConnected,
		typename T_Text
	> class DisplayIS31FL3731PictureElement :
//		public DisplayIS31FL3731BasicElement<T_OWNER>,
		public DisplayIS31FL3731BasicPictureElement<
				T_OWNER, C_OWNER,
				DisplayIS31FL3731PictureElement<
					T_OWNER, C_OWNER,
					T_BackgroundColor,
					T_Blink_Operation,
					T_ClockInputPin_o_IsConnected,
					T_Elements_Render,
					T_Enabled,
                    T_FCursorX,
                    T_FCursorY,
					T_FFrame,
					T_FFrame0,
					T_FFrame1,
					T_FFrameAssigned,
					T_FModified,
					T_FadeEffect,
					T_Orientation,
					T_RefreshInputPin_o_IsConnected,
					T_Text
				>,
				T_BackgroundColor,
				T_Blink_Operation,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_FModified,
				T_Orientation,
				T_Text
			>,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FFrame,
		public T_FFrame0,
		public T_FFrame1,
		public T_FFrameAssigned,
		public T_FadeEffect,
		public T_RefreshInputPin_o_IsConnected
//		public T_Text
	{
		typedef DisplayIS31FL3731BasicPictureElement<
				T_OWNER, C_OWNER,
				DisplayIS31FL3731PictureElement,
				T_BackgroundColor,
				T_Blink_Operation,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_FModified,
				T_Orientation,
				T_Text
			> inherited;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( FadeEffect )
//		_V_PROP_( Text )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )
		_V_PROP_( RefreshInputPin_o_IsConnected )

	protected:
		_V_PROP_( FFrameAssigned )
		_V_PROP_( FFrame )
		_V_PROP_( FFrame0 )
		_V_PROP_( FFrame1 )

	public:
		void UpdateFadeEffect()
		{
			uint8_t	AData = ( FadeEffect().FadeOut().GetValue() << 4 ) | FadeEffect().FadeIn().GetValue();
			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::BREATH_CONTROL_REGISTER1, AData );

			AData = ( Enabled().GetValue() ? 0b10000 : 0 ) | FadeEffect().ExtinguishTime().GetValue();
			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::BREATH_CONTROL_REGISTER2, AData );
		}

	public:
		inline void AssignFrames( uint8_t &AFrame )
		{
			if( AFrame > 7 - 2 )
			{
				FFrameAssigned() = false;
				return;
			}

			FFrameAssigned() = true;
			FFrame0() = AFrame ++;
			FFrame1() = AFrame ++;
		}

	protected:
		void display()
		{
//			Serial.println( "display" );
//			Serial.println( ( FFrame ) ? FFrame0 : FFrame1 );
//			T_OWNER::SelectBank( IS31FL3731Const::BANK_FUNCTIONREG );
			C_OWNER.SelectBank( ( FFrame() ) ? FFrame0() : FFrame1() );
			inherited::RenderImage();

			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::REG_PICTUREFRAME, ( FFrame() ) ? FFrame0() : FFrame1() );
			FFrame() = ! FFrame();
			inherited::FModified() = false;
		}

	public:
		inline void SystemInitStart( bool &AStarted )
		{
			for( int i = 0; i < IS31FL3731Const::HEIGHT * IS31FL3731Const::WIDTH; ++i )
				inherited::FPicture[ i ] = inherited::BackgroundColor().GetValue();

			if( ! Enabled() )
				return;

//			inherited::setTextSize( Text().Size() );
//			inherited::setTextColor( Text().Color(), Text().BackgroundColor().GetValue() );
//			inherited::setTextWrap( Text().Wrap() );

			T_Elements_Render::Call();
//			for( int i = 0; i < DisplayIS31FL3731BasicPictureElement<T_OWNER>::FElements.size(); ++ i )
//				DisplayIS31FL3731BasicPictureElement<T_OWNER>::FElements[ i ]->Render( false );

			T_Blink_Operation::Call( false );

			if( AStarted )
				return;

			if( ClockInputPin_o_IsConnected() )
				return;

			AStarted = true;

			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline void SystemLoopEnd()
		{
			if( inherited::FModified() )
				if( ! RefreshInputPin_o_IsConnected() )
					display();
		}

	public:
		void RefreshInputPin_o_Receive( void * )
		{
			if( inherited::FModified() )
				display();
		}

		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			// picture mode
			C_OWNER.WriteRegister8( IS31FL3731Const::BANK_FUNCTIONREG, IS31FL3731Const::REG_CONFIG, IS31FL3731Const::REG_CONFIG_PICTUREMODE );
			UpdateFadeEffect();
			display();
		}

	public:
		inline DisplayIS31FL3731PictureElement()
		{
//			FModified() = true;
			FFrameAssigned() = false;
			FFrame() = false;
		}

	};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*
	DisplayIS31FL3731BasicElement::DisplayIS31FL3731BasicElement( T_OWNER &AOwner ) :
		FOwner( AOwner )
	{
		AOwner.FElements.push_back( this );
	}
//---------------------------------------------------------------------------
	DisplayIS31FL3731FrameElement::DisplayIS31FL3731FrameElement( DisplayIS31FL3731AnimationElement &AOwner, const unsigned char * AFont ) :
		inherited( AOwner.FOwner.FWire, AFont ),
		FOwner( AOwner.FOwner )
	{
		AOwner.FFrames.push_back( this );
	}
//---------------------------------------------------------------------------
	DisplayIS31FL3731BasicBlink::DisplayIS31FL3731BasicBlink( DisplayIS31FL3731BasicPictureElement &AOwner, void * ) :
		FWire( AOwner.FWire )
	{		
		AOwner.FBlink = this;
	}
*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif