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
#include <Picaso_Const4D.h>
#include <Mitov_Graphics.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Color,
		typename T_Font,
		typename T_Size
	> class PicasoDisplayFont :
		public T_Color,
		public T_Font,
		public T_Size
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( Font )
		_V_PROP_( Size )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Enabled,
		typename T_Font,
		typename T_OutputPin,
		typename T_Text,
		typename T_TouchUpdated,
		typename T_X,
		typename T_Y
	> class PicasoDisplayElementDrawButton :
		public T_Color,
		public T_Enabled,
		public T_Font,
		public T_OutputPin,
		public T_Text,
		public T_TouchUpdated,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( Enabled )
		_V_PROP_( Font )
		_V_PROP_( OutputPin )
		_V_PROP_( Text )
		_V_PROP_( TouchUpdated )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if( Enabled().GetValue() )
				C_OWNER.DrawButton( 0, X(), Y(), Color(), Font().Color(), Font().Font(), Font().Size().X(), Font().Size().Y(), Text().GetValue() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_TouchedOutputPin,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class PicasoDisplayTouched :
		public T_Enabled,
		public T_TouchedOutputPin,
		public T_XOutputPin,
		public T_YOutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( TouchedOutputPin )
		_V_PROP_( XOutputPin )
		_V_PROP_( YOutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_BackgroundColor,
		uint32_t C_HEIGHT,
//		bool C_HasTouch,
		uint8_t C_RESET_PIN,
		uint32_t C_WIDTH,
		typename T_Elements_Render,
		typename T_FCountResultWords,
		typename T_FResponseType,
		typename T_FWaitAcknowledge,
		typename T_Timeout,
		typename T_Touch
	> class PicasoDisplay :
		public T_BackgroundColor,
		public T_FCountResultWords,
		public T_FResponseType,
		public T_FWaitAcknowledge,
		public T_Timeout,
		public T_Touch
	{
		typedef PicasoDisplay T_SELF;

	public:
		_V_PROP_( BackgroundColor )
		_V_PROP_( Timeout )
		_V_PROP_( Touch )

	protected:
		_V_PROP_( FCountResultWords )
		_V_PROP_( FResponseType )
		_V_PROP_( FWaitAcknowledge )

		unsigned long FLastTime = 0;

		uint16_t	FTextX = 0;
		uint16_t	FTextY = 0;

	protected:
		void Reset()
		{
			digitalWrite( C_RESET_PIN, HIGH );
			delay(100);
			digitalWrite( C_RESET_PIN, LOW );
			delay( 5000 );
//			delay( 5000 );

			FWaitAcknowledge() = false;
			Serial1.flush();
		}

		void CheckAcknowledge()
		{
//			Serial.print( "CheckAcknowledge : " );
//			Serial.println( FWaitAcknowledge() );
//			Serial.println( FResponseType() );

			if( ! FWaitAcknowledge().GetValue() )
				return;

			do
			{
				bool ARead = false;
				uint8_t readx;
				do
				{
					if( Serial1.available() != 0 )
					{
						readx = Serial1.read();
						ARead = true;
					}
				}
				while (( ARead != 1 ) && ( millis() - FLastTime < Timeout().GetValue() ));

				if( ARead )
					ProcessAcknowledgeResult( ARead, readx );

				else
				{
					Serial.println( "TIMEOUT 1" );
					FWaitAcknowledge() = false;
					FResponseType() = 0;
				}

			}
			while( FResponseType().GetValue() );

//				return;
		}

		void ProcessAcknowledgeResult( bool ARead, uint8_t AValue )
		{
			if ( ! ARead )
			{
/*
				Error4D = Err4D_Timeout ;
				if (Callback4D != NULL)
					Callback4D(Error4D, Error4D_Inv);
*/
			}

			else if ( AValue != 6 )
			{
/*
				Error4D     = Err4D_NAK ;
				Error4D_Inv = readx ;
				if (Callback4D != NULL)
					Callback4D(Error4D, Error4D_Inv);
*/
			}

			FWaitAcknowledge() = false;
//			Serial.println( "FWaitAcknowledge() = false" );

//			Serial.print( "CheckAcknowledge: " );
//			Serial.print( AValue );
//			Serial.print( " - " );
			Serial.println( FResponseType().GetValue() );

			if( AValue != 6 )
			{
				FResponseType() = 0;
				FCountResultWords() = 0;
			}

			else
				switch( FResponseType().GetValue() )
				{
					case 1:
					{
						uint16_t AValue = Get16BitData();
						Serial.print( "PARAM: " );
						Serial.println( AValue );
						switch( AValue )
						{
//							case 0: // No Touch
//							{
//							}

							case 1: // Press
								Touch().TouchedOutputPin().SetPinValueHigh();
								// Continue to the next state...

							case 3: // Moving
							{
								if( Touch().XOutputPin().GetPinIsConnected() )
								{
									Command_2x16( F_touch_Get, 1 );
									FResponseType() = 2;
								}

								else if( Touch().YOutputPin().GetPinIsConnected() )
								{
									Command_2x16( F_touch_Get, 2 );
									FResponseType() = 3;
								}

								break;
							}

							case 2: // Release
							{
								Touch().TouchedOutputPin().SetPinValueLow();
							}

						}

/*
							if( )
							Command_2x16( F_touch_Get, 0 );
							FResponseType() = 1;			
*/
							break;
						}

					case 2:
					{
						uint32_t AValue = Get16BitData();
						Touch().XOutputPin().SetPinValue( AValue );
						break;
					}

					default:
					{
						Serial.println( "TEST1111" );
						FResponseType() = 0;
						for( uint8_t i = 0; i < FCountResultWords(); i++ )
							Get16BitData();

						FCountResultWords() = 0;
					}
				}

		}

	protected:
		inline void StartWaitAcknowledge()
		{
//			Serial.println( "StartWaitAcknowledge" );
			FLastTime = millis();
			FWaitAcknowledge() = true;
		}

		inline static void Write16( uint16_t AValue )
		{
			Serial1.write( AValue >> 8 );
			Serial1.write( AValue );
		}

		inline static void Write16Integer( int16_t AValue )
		{
			Serial1.print( char( AValue >> 8 ));
			Serial1.print( char( AValue ));
		}

		void Command16( uint16_t ACommand )
		{
//			Serial.print( "Command16 : " );
//			Serial.println( ACommand );

			CheckAcknowledge();

			Write16( ACommand );

			StartWaitAcknowledge();
		}

		void Command_2x16( int16_t ACommand, uint16_t AValue )
		{
//			Serial.print( "Command_2x16 : " );
//			Serial.println( ACommand );

			
			CheckAcknowledge();

			Write16Integer( ACommand );
			Write16( AValue );

			StartWaitAcknowledge();
		}

		void Command_3x16( uint16_t ACommand, uint16_t AValue1, uint16_t AValue2 )
		{
//			Serial.print( "Command_3x16 : " );
//			Serial.println( ACommand );

			CheckAcknowledge();

			Write16( ACommand );
			Write16( AValue1 );
			Write16( AValue2 );

			StartWaitAcknowledge();
		}

		void Command_6x16( uint16_t ACommand, uint16_t AValue1, uint16_t AValue2, uint16_t AValue3, uint16_t AValue4, uint16_t AValue5 )
		{
//			Serial.print( "Command_6x16 : " );
//			Serial.println( ACommand );

			CheckAcknowledge();

			Write16( ACommand );
			Write16( AValue1 );
			Write16( AValue2 );
			Write16( AValue3 );
			Write16( AValue4 );
			Write16( AValue5 );

			StartWaitAcknowledge();
		}

		void Command_4x16( uint16_t ACommand, uint16_t AValue1, uint16_t AValue2, uint16_t AValue3 )
		{
//			Serial.print( "Command_4x16 : " );
//			Serial.println( ACommand );

			CheckAcknowledge();

			Write16( ACommand );
			Write16( AValue1 );
			Write16( AValue2 );
			Write16( AValue3 );

			StartWaitAcknowledge();
		}

		uint16_t Get16BitData()
		{
			uint8_t readx[2];
  
//			if (Error4D != Err4D_OK)
//				return 0;

			unsigned long sttime   = millis();
			int readc    = 0 ;
			while ((readc != 2) && (millis() - sttime < Timeout().GetValue()))
				if( Serial1.available()) 
					readx[readc++] = Serial1.read();
  
			if (readc != 2)
			{
/*
				Error4D  = Err4D_Timeout ;
				if (Callback4D != NULL)
					Callback4D(Error4D, Error4D_Inv) ;
*/
				return 0 ;
			}

			return uint16_t( readx[ 0 ] ) << 8 | readx[ 1 ];
		}

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY ) { AX = 0; AY = 0; }

	public:
		inline void SetCursor( uint32_t AX, uint32_t AY )
		{
//			Command_3x16( F_txt_MoveCursor, AY, AX );
			Command_3x16( F_gfx_MoveTo, AX, AY );
		}

		inline void SetBackgroundColor( uint32_t AColor )
		{
			Command_2x16( F_gfx_BGcolour, Func::Color565( AColor ) );
			FCountResultWords() = 1;
		}

		inline void ClearScreen() // uint32_t AColor )
		{
			Command16( F_gfx_Cls );
		}

		inline static void ClearScreen( T_SELF &AImplementation, TUniColor AColor )
		{
			AImplementation.SetBackgroundColor( AColor );
			AImplementation.ClearScreen();
		}

		static inline void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			AImplementation.Command_6x16( F_gfx_RectangleFilled, x, y, x + w, y + h, Func::Color565( color ));
		}

		static inline void drawRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			AImplementation.Command_6x16( F_gfx_Rectangle, x, y, x + w, y + h, Func::Color565( color ));
		}

		inline static void fillEllipseWH( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color ) 
		{
			AImplementation.Command_6x16( F_gfx_EllipseFilled, x + w / 2, y + h / 2, w / 2, h / 2, Func::Color565( color ));
		}

		inline static void drawEllipseWH( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color ) 
		{
			AImplementation.Command_6x16( F_gfx_Ellipse, x + w / 2, y + h / 2, w / 2, h / 2, Func::Color565( color ));
		}

		static void drawLine( T_SELF &AImplementation, int16_t x0, int16_t y0, int16_t x1, int16_t y1, TUniColor color) 
		{
			AImplementation.Command_6x16( F_gfx_Line, x0, y0, x1, y1, Func::Color565( color ) );
		}

		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
//			Serial.println( "TEST" );
			Command_4x16( F_gfx_PutPixel, x, y, Func::Color565( color ) );
		}

		static void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			// Rudimentary clipping
			if((x >= AImplementation.width() ) || (y >= AImplementation.height() )) 
				return;

			drawLine( AImplementation, x, y, x + w, y, color );
		}

	public:
		void DrawButton( uint16_t AState, uint16_t AX, uint16_t AY, TUniColor AColor, TUniColor AFontColor, uint16_t AFont, uint16_t AFontX, uint16_t AFontY, Mitov::String AText )
		{
			CheckAcknowledge();

			Write16( F_gfx_Button );
			Write16( AState );
			Write16( AX );
			Write16( AY );
			Write16( Func::Color565( AColor ) );
			Write16( Func::Color565( AFontColor ) );
			Write16( AFont );
			Write16( AFontX );
			Write16( AFontY );

			char *APtr = (char *)AText.c_str();

			do
			{
				Serial1.write( *APtr );
			}
			while( *APtr ++ );

//			Write16( AValue );

//			Write16( Func::Color565( AColor ) );

			StartWaitAcknowledge();
		}

	public:
		constexpr inline static bool ImplementsPolygon()
		{
			return true;
		}

		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor )
		{
			CheckAcknowledge();

			Write16( AFill ? F_gfx_PolygonFilled : F_gfx_Polygon );

			Write16( APolygon.GetPointsCount() + 1 );

//			Serial.println( APolygon.GetPointsCount() + 1 );

			Write16( AX );

			if( APolygon.GetPointsIsConstant() )
			{
				const PROGMEM Mitov::GraphicsPoint *APoints = APolygon.GetPointsValue();
				for( uint32_t i = 0; i < APolygon.GetPointsCount(); ++i )
				{
#ifdef __32BIT_GRAPHICS_COORDINATES__
					TGraphicsPos	AX1 = pgm_read_dword( APoints + i ) + AX;
#else
					TGraphicsPos	AX1 = pgm_read_word( APoints + i ) + AX;
#endif
//					Serial.println( AX1 );
					Write16( AX1 );
				}

				Write16( AY );

				for( uint32_t i = 0; i < APolygon.GetPointsCount(); ++i )
				{
#ifdef __32BIT_GRAPHICS_COORDINATES__
					TGraphicsPos	AY1 = pgm_read_dword( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + AY;
#else
					TGraphicsPos	AY1 = pgm_read_word( ((uint8_t *)( APoints + i ) ) + sizeof( TGraphicsPos ) ) + AY;
#endif
//					Serial.println( AX1 );
					Write16( AY1 );
				}
			}

			else
			{
				const Mitov::GraphicsPoint *APoints = APolygon.GetPointsValue();
				for( uint32_t i = 0; i < APolygon.GetPointsCount(); ++i )
				{
					TGraphicsPos	AX1 = APoints[ i ].X + AX;
					Write16( AX1 );
				}

				Write16( AY );

				for( uint32_t i = 0; i < APolygon.GetPointsCount(); ++i )
				{
					TGraphicsPos	AY1 = APoints[ i ].Y + AY;
					Write16( AY1 );
				}

			}

			Write16( Func::Color565( AColor ) );

			StartWaitAcknowledge();
		}

		inline int16_t width(void) const
		{
			return C_WIDTH;
/*
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return C_WIDTH;

			return C_HEIGHT;
*/
		}

		inline int16_t height(void) const
		{
			return C_HEIGHT;
/*
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return C_HEIGHT;

			return C_WIDTH;
*/
		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			AImplementation.CheckAcknowledge();

			Write16( F_putCH );
			Write16( AValue );

			AImplementation.StartWaitAcknowledge();
			return 1;
//			return graphics::write( AImplementation, AConfig, AValue );
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
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
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
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	public:
		inline void SystemInit()
		{
			Serial1.begin( 200000 );
			pinMode( C_RESET_PIN, OUTPUT );
			Reset();

			Command_2x16( F_touch_Set, 0 );
			delay( 1000 );
			Command_2x16( F_touch_Set, 2 );
//			delay( 1000 );
//			Command_2x16( F_touch_Set, 0 );
		}

		inline void SystemStart()
		{
//			ClearScreen( *this, NonTransparent( BackgroundColor().GetValue() )); // BackgroundColor().GetValue() );
			ClearScreen( *this, BackgroundColor().GetValue() ); // BackgroundColor().GetValue() );
			T_Elements_Render::Call();
		}

		inline void SystemAllLoopBegin()
		{
			if( FWaitAcknowledge() )
			{
//				Serial.println( "SystemAllLoopBegin()" );

				bool ARead = false;
				uint8_t readx;
				if( Serial1.available() != 0 )
				{
					readx = Serial1.read();
					ARead = true;
				}

				else if ( millis() - FLastTime < Timeout().GetValue() )
					return;

				if( ARead )
					ProcessAcknowledgeResult( ARead, readx );

				else
				{
					Serial.println( "TIMEOUT" );
					FWaitAcknowledge() = false;
					FResponseType() = 0;
				}
			}

		}

		inline void SystemLoopBeginTouch()
		{
//			return;
			if( FWaitAcknowledge().GetValue() )
				return;

//			Serial.print( "SystemLoopBeginTouch: " );

			Command_2x16( F_touch_Get, 0 );
			FResponseType() = 1;			

//			Serial.println( FWaitAcknowledge().GetValue() );
		}

	public:
		inline PicasoDisplay()
		{
			pinMode( C_RESET_PIN, OUTPUT );
			FWaitAcknowledge() = false;
			FResponseType() = 0;
			FCountResultWords() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

