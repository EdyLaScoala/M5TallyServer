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
#include <SPI.h>
#include <Mitov_7SegmentDigits.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_Enabled,
		typename T_FModified,
		typename T_Intensity,
		int COUNT_CONTROLLERS
	> class MaximLedController :
		public T_ChipSelectOutputPin,
		public T_Enabled,
		public T_FModified,
		public T_Intensity
	{
	protected:
		static const	uint8_t OP_DIGIT0 = 1;
		static const	uint8_t OP_DECODEMODE = 9;
		static const	uint8_t OP_INTENSITY = 10;
		static const	uint8_t OP_SCANLIMIT = 11;
		static const	uint8_t OP_SHUTDOWN = 12;
		static const	uint8_t OP_DISPLAYTEST = 15;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Intensity )

	protected:
		_V_PROP_( FModified )

		uint8_t	FRegistersCurrent[ COUNT_CONTROLLERS * 8 ];
		uint8_t	FRegisters[ COUNT_CONTROLLERS * 8 ];

	public:
		inline void UpdateIntensity()
		{
			spiTransferAll( OP_INTENSITY, Intensity() * 15 + 0.5 );
		}

		inline void UpdateShutDown()
		{
			spiTransferAll( OP_SHUTDOWN, Enabled() );
		}

	public:
		void SetPixelValue( int AIndex, bool AValue )
		{
//			Serial.print( "SetPixelValue( " );
//			Serial.print( AIndex );
//			Serial.print( ", " );
//			Serial.print( AValue );
//			Serial.println( " )" );
			if( AValue )
				FRegisters[ AIndex / 8 ] |= 1 << ( AIndex & 0b111 );  // same as % 8

			else
				FRegisters[ AIndex / 8 ] &= ~( 1 << ( AIndex & 0b111 ));  // same as % 8

			FModified() = true;
		}

		inline bool GetPixelValue( int AIndex )
		{
			return FRegisters[ AIndex / 8 ] & ( 1 << ( AIndex & 0b111 ));  // same as % 8
		}

	protected:
		void spiTransferAll( uint8_t opcode, uint8_t data )
		{
			//enable the line 
			T_ChipSelectOutputPin::SetPinValueLow();
			//Now shift out the data 

//			C_SPI.beginTransaction( SPISettings( 100000, MSBFIRST, SPI_MODE0) );

			for(int i = 0; i < COUNT_CONTROLLERS; ++i )
			{    
				C_SPI.transfer( opcode );
				C_SPI.transfer( data );
			}
    
//			C_SPI.endTransaction();

			//latch the data onto the display
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

/*
		void spiTransfer( int addr, uint8_t opcode, uint8_t data )
		{
			//enable the line 
			T_ChipSelectOutputPin::SetPinValueLow();
			//Now shift out the data 

			for(int i = 1; i < COUNT_CONTROLLERS; ++i )
				C_SPI.transfer16( 0 ); // Skip one chip
      
			C_SPI.transfer( opcode );
			C_SPI.transfer( data );
    
			//latch the data onto the display
			T_ChipSelectOutputPin::SetPinValueHigh();
		}    
*/
	protected:
		void UpdatePixels()
		{
			uint16_t ABuffer[ COUNT_CONTROLLERS ];
			FModified() = false;

			for(;;)
			{
				bool HasChange = false;
				for( int AControllerIndex = 0; AControllerIndex < COUNT_CONTROLLERS; ++AControllerIndex )
				{
					ABuffer[ AControllerIndex ] = 0;
					for( int j = 0; j < 8; ++j )
					{
						int ARegIndex = AControllerIndex * 8 + j;
						uint16_t ANewRegValue = FRegisters[ ARegIndex ];
						if( FRegistersCurrent[ ARegIndex ] != ANewRegValue )
						{
							FRegistersCurrent[ ARegIndex ] = ANewRegValue;
							//Serial.println( ANewRegValue, HEX );
							ABuffer[ AControllerIndex ] = ( ANewRegValue << 8 ) | ( OP_DIGIT0 + j );
							HasChange = true;
							break;
						}
					}
				}

				if( ! HasChange )
					break;

//				Serial.print( "ABuffer = " );
//				Serial.println( ABuffer[ 0 ], HEX );

				//enable the line 
				T_ChipSelectOutputPin::SetPinValueLow();
				C_SPI.transfer( ABuffer, COUNT_CONTROLLERS * 2 );

				//latch the data onto the display
				T_ChipSelectOutputPin::SetPinValueHigh();
			}
		}

	public:
		inline void SystemInit()
		{
//			int AStartPixel = 0;
//			for( int i = 0; i < FPixelGroups.size(); ++i )
//				FPixelGroups[ i ]->StartPixels( AStartPixel );	

//			FNumDevices = ( AStartPixel + 63 ) / 64;

//			FRegisters.SetCount( FNumDevices * 8, 0 );
//			FRegistersCurrent.SetCount( FNumDevices * 8, 0 );

			spiTransferAll( OP_DISPLAYTEST, 0 );
			spiTransferAll( OP_SCANLIMIT, 7 );
			spiTransferAll( OP_DECODEMODE, 0 );

			UpdateShutDown();
			UpdateIntensity();

//			if( FModified )


			for( int i = 0; i < COUNT_CONTROLLERS * 8; ++i )
				FRegistersCurrent[ i ] = FRegisters[ i ] + 1;

			UpdatePixels();

//			spiTransferAll( OP_DIGIT0, 255 );
		}

		inline void SystemLoopEnd()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( FModified() )
				UpdatePixels();
		}

	public:
		MaximLedController()
//			Enabled( true ),
//			FModified( false )
		{
			FModified() = false;
			for( int i = 0; i < COUNT_CONTROLLERS * 8; ++i )
				FRegisters[ i ] = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialValue
	> class MaximLedGroup :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		uint8_t		FValues[ ( C_COUNT_PIXELS + 7 ) / 8 ] = { 0 };

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			for( uint8_t i = 0; i < C_COUNT_PIXELS; ++i )
			{
				if( Enabled() )
					C_INSTANCE.SetPixelValue( C_START_PIXEL + i, ( FValues[ i / 8 ] & ( 1 << ( i & 0b111 ))) != 0 );

				else
					C_INSTANCE.SetPixelValue( C_START_PIXEL + i, false );
			}
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( Func::SetBitField( FValues, AIndex, AValue ) )
				if( Enabled() )
					C_INSTANCE.SetPixelValue( AIndex, AValue );

		}

	public:
		inline void SystemStart()
		{
			for( int i = 0; i < C_COUNT_PIXELS; ++i )
				C_INSTANCE.SetPixelValue( C_START_PIXEL + i, InitialValue() );

		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_INSTANCE, uint16_t C_COUNT_PIXELS, uint16_t C_START_PIXEL> class MaximLedGroupSection7Segments
	{
	public:
		bool	Enabled = true;
		bool	InitialValue = false;

	public:
		virtual void StartPixels( int &AStartPixel ) override
		{
			for( int i = 0; i < 7; ++i )
			{
				InputPins[ i ].FOwner = this;
				InputPins[ i ].FIndex = AStartPixel + 6 - i;
				InputPins[ i ].Value = InitialValue;
				C_INSTANCE.SetPixelValue( AStartPixel + 6 - i, InitialValue );
			}

			InputPins[ 7 ].FOwner = this;
			InputPins[ 7 ].FIndex = AStartPixel + 7;
			InputPins[ 7 ].Value = InitialValue;
			C_INSTANCE.SetPixelValue( AStartPixel + 7, InitialValue );

			inherited::StartPixels( AStartPixel );
		}

	public:
		MaximLedGroupSection7Segments()
		{
//			InputPins.SetCount( 8 );
//			CountPixels = 8;
		}
	};
*/
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_INSTANCE, typename TBASE_CLASS, typename T_DATA, uint16_t C_COUNT_PIXELS, uint16_t C_START_PIXEL> class MaximLedGroupTypedValueDisplay7Segments : public TBASE_CLASS
	{
		typedef TBASE_CLASS inherited;

	public:
		bool	Enabled : 1;
		bool	ReversedOrder : 1;

	public:
		T_DATA	InitialValue = ::Mitov::GetDefaultValue<T_DATA>();

//	protected:
//		virtual uint8_t GetSegmentsValue( int &ADigit ) = 0;

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			inherited::UpdateDisplay();
			for( int ADigit = 0; ADigit < C_COUNT_PIXELS / 8; ++ ADigit )
			{
				uint8_t AValue;

				if( Enabled )
					AValue = TBASE_CLASS::template GetSegmentsValue<C_COUNT_PIXELS>( ADigit, InitialValue );

				else
					AValue = 0;

//				Serial.print( "ADigit = " ); Serial.println( ADigit );
//				Serial.print( "AValue = " ); Serial.println( AValue, HEX );
//				Serial.println( "" );

				if( ReversedOrder )
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ( ( C_COUNT_PIXELS / 8 ) - ADigit - 1 ) * 8 + i, ( AValue >> i ) & 1 );
				}

				else
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ADigit * 8 + i, ( AValue >> i ) & 1 );
				}
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_DATA AValue = *(T_DATA *)_Data;
			if( InitialValue == AValue )
				return;

			InitialValue = AValue;
//			Serial.println( InitialValue );
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			inherited::template SystemStart<C_COUNT_PIXELS>();
			UpdateDisplay();
		}

	public:
		MaximLedGroupTypedValueDisplay7Segments() :
			Enabled( true ),
			ReversedOrder( false )
		{
		}
	};
*/
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_INSTANCE, uint16_t C_COUNT_PIXELS, uint16_t C_START_PIXEL, typename T_DATA> MaximLedGroupTypedValueDisplay7Segments<T_OWNER, C_INSTANCE, C_COUNT_PIXELS, C_START_PIXEL, T_DATA, Mitov::String>::MaximLedGroupTypedValueDisplay7Segments() :
		{
		}
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialValue
	> class MaximLedGroupUnsignedBitPixelsSection :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			for( int i = 0; i < C_COUNT_PIXELS; ++i )
				C_INSTANCE.SetPixelValue( C_START_PIXEL + i, Enabled() && ( InitialValue() >> i ) & 1 );

		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			uint32_t AValue = *(uint32_t *)_Data;
			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialValue
	> class MaximLedSingleValueGroup :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			for( int i = 0; i < C_COUNT_PIXELS; ++i )
			{
				if( Enabled() )
					C_INSTANCE.SetPixelValue( C_START_PIXEL + i, InitialValue() );

				else
					C_INSTANCE.SetPixelValue( C_START_PIXEL + i, false );
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialIndex,
		typename T_InitialValue
	> class MaximLedIndexedPixelGroup :
		public T_Enabled,
		public T_InitialIndex,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( InitialIndex )

	public:
		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > C_COUNT_PIXELS )
				AIndex = C_COUNT_PIXELS;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;
			UpdateDisplay();
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			if( Enabled() )
				C_INSTANCE.SetPixelValue( C_START_PIXEL + InitialIndex(), InitialValue() );

			else
				C_INSTANCE.SetPixelValue( C_START_PIXEL + InitialIndex(), false );
		}


	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE, 
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialIndexX,
		typename T_InitialIndexY,
		typename T_InitialValue,
		typename T_Width
	> class MaximLedIndexed2DPixelGroup :
		public T_Enabled,
		public T_InitialIndexX,
		public T_InitialIndexY,
		public T_InitialValue,
		public T_Width
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Width )
		_V_PROP_( InitialValue )
		_V_PROP_( InitialIndexX )
		_V_PROP_( InitialIndexY )

	public:
		inline void IndexInputPins_o_Receive( int AIndex, void *_Data )
		{
			if( AIndex )
				DoReceiveIndexY( _Data );

			else
				DoReceiveIndexX( _Data );

		}

		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

	protected:
		void DoReceiveIndexX( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > Width() )
				AIndex = Width();

			if( InitialIndexX() == AIndex )
				return;

			InitialIndexX() = AIndex;
			UpdateDisplay();
		}

		void DoReceiveIndexY( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			uint32_t AHeight = ( C_COUNT_PIXELS + Width() - 1 ) / Width();
			if( AIndex > AHeight )
				AIndex = AHeight;

			if( InitialIndexY() == AIndex )
				return;

			InitialIndexY() = AIndex;
			UpdateDisplay();
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			uint32_t AIndex = C_START_PIXEL + InitialIndexY() * Width() + InitialIndexX();
			if( Enabled() )
				C_INSTANCE.SetPixelValue( AIndex, InitialValue() );

			else
				C_INSTANCE.SetPixelValue( AIndex, false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_BackgroundColor,
		typename T_Elements_Render,
		typename T_Enabled,
        typename T_FCursorX,
        typename T_FCursorY,
		uint32_t C_HEIGHT,
		typename T_MirrorHorizontal,
		typename T_MirrorVertical,
		typename T_Orientation,
		typename T_ReverseHorizontal,
		typename T_ReverseVertical,
		typename T_Text,
		uint32_t C_WIDTH
	> class MaximLedGraphics2DPixelGroup : 
		public GraphicsImpl<MaximLedGraphics2DPixelGroup<
					T_OWNER, C_INSTANCE,
					C_COUNT_PIXELS,
					C_START_PIXEL,
					T_BackgroundColor,
					T_Elements_Render,
					T_Enabled,
                    T_FCursorX,
                    T_FCursorY,
					C_HEIGHT,
					T_MirrorHorizontal,
					T_MirrorVertical,
					T_Orientation,
					T_ReverseHorizontal,
					T_ReverseVertical,
					T_Text,
					C_WIDTH
				>,
				T_BackgroundColor,
				T_Text
			>,
		public T_Elements_Render,
		public T_Enabled,
        public T_FCursorX,
        public T_FCursorY,
		public T_MirrorHorizontal,
		public T_MirrorVertical,
		public T_Orientation,
		public T_ReverseHorizontal,
		public T_ReverseVertical
//		public T_Text
	{
		typedef MaximLedGraphics2DPixelGroup T_SELF;

		typedef GraphicsImpl<
				T_SELF,
				T_BackgroundColor,
				T_Text
			> inherited;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Orientation )

	public:
		_V_PROP_( MirrorHorizontal )
		_V_PROP_( MirrorVertical )

		_V_PROP_( Text )

		_V_PROP_( ReverseHorizontal )
		_V_PROP_( ReverseVertical )
        
	public:
        _V_PROP_( FCursorX )
        _V_PROP_( FCursorY )

//		uint32_t Width = 8;
//		uint32_t Height = 8;

//	protected:
//		int		FStartPixel;

/*
	public:
		virtual void StartPixels( int &AStartPixel ) override
		{
			FStartPixel = AStartPixel;
//			inherited::StartPixels( AStartPixel );
			AStartPixel += Width * Height;
		}
*/
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
		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			if( color == tmcNone )
				return;

//			Serial.println( height() );

			if((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return;

//			Serial.println( "drawPixel" );
			// check rotation, move pixel around if necessary
			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = C_WIDTH - x - 1;
					break;

				case goDown:
					x = C_WIDTH - x - 1;
					y = C_HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = C_HEIGHT - y - 1;
					break;
			}  

			uint32_t AXBlock = x / 8;
			if( ReverseHorizontal() )
				AXBlock = ( C_WIDTH / 8 ) - 1 - AXBlock; 


			uint32_t AYBlock = y / 8;
			if( ReverseVertical() )
				AYBlock = ( C_HEIGHT / 8 ) - 1 - AYBlock; 

			uint32_t AXOffset = x & 0b111;
			uint32_t AYOffset = y & 0b111;

			if( MirrorHorizontal() )
				AYOffset = 7 - AYOffset;

			if( MirrorVertical() )
				AXOffset = 7 - AXOffset;

			uint32_t ABlockIndex = AYBlock * ( C_WIDTH / 8 ) + AXBlock;
			uint32_t ABlockOffset = AYOffset * 8 + AXOffset;

//			uint32_t AIndex = C_START_PIXEL + y * Width + x;
			uint32_t AIndex = C_START_PIXEL + ABlockIndex * 64 + ABlockOffset;

//			Serial.print( AIndex ); Serial.print( " " ); Serial.println( color );

			switch( color )
			{
				case tmcWhite: C_INSTANCE.SetPixelValue( AIndex, true ); break;
				case tmcBlack: C_INSTANCE.SetPixelValue( AIndex, false ); break;
				case tmcInvert: C_INSTANCE.SetPixelValue( AIndex, ! C_INSTANCE.GetPixelValue( AIndex )); break;
			}
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return false;

			// check rotation, move pixel around if necessary
			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = C_WIDTH - x - 1;
					break;

				case goDown:
					x = C_WIDTH - x - 1;
					y = C_HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = C_HEIGHT - y - 1;
					break;
				}  

			uint32_t AXBlock = x / 8;
			if( ReverseHorizontal() )
				AXBlock = ( C_WIDTH / 8 ) - 1 - AXBlock; 

			uint32_t AYBlock = y / 8;
			if( ReverseVertical() )
				AYBlock = ( C_HEIGHT / 8 ) - 1 - AYBlock; 

			uint32_t AXOffset = x & 0b111;
			uint32_t AYOffset = y & 0b111;

			if( MirrorHorizontal() )
				AYOffset = 7 - AYOffset;

			if( MirrorVertical() )
				AXOffset = 7 - AXOffset;

			uint32_t ABlockIndex = AYBlock * ( width() / 8 ) + AXBlock;
			uint32_t ABlockOffset = AYOffset * 8 + AXOffset;

//			uint32_t AIndex = C_START_PIXEL + y * Width + x;
			uint32_t AIndex = C_START_PIXEL + ABlockIndex * 64 + ABlockOffset;

			return ( C_INSTANCE.GetPixelValue( AIndex ) );
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
				return C_WIDTH;

			return C_HEIGHT;
		}

		inline int16_t height(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return C_HEIGHT;

			return C_WIDTH;
		}

	public:
		inline void SystemStart()
		{
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

/*
		MaximLedGraphics2DPixelGroup( const unsigned char * AFont ) :
			inherited( AFont ),
			Enabled( true ),
			Orientation( goUp ),
			MirrorHorizontal( false ),
			MirrorVertical( false )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_FCurrentReversed,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Reversed
	> class MaximLedRunningPixelGroup :
		public T_Enabled,
		public T_FCurrentReversed,
		public T_InitialValue,
		public T_OutputPin,
		public T_Reversed
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reversed )

		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( FCurrentReversed )

	public:
		void UpdateReversePixels()
		{
			if( Reversed().AllPixels() )
				if( Reversed().Reversed().GetValue() != FCurrentReversed().GetValue() )
				{
					FCurrentReversed() = Reversed().Reversed();

					for( int i = 0; i < C_COUNT_PIXELS / 2; ++i )
					{
						bool AOldValue1 = C_INSTANCE.GetPixelValue( C_START_PIXEL + ( C_COUNT_PIXELS - i - 1 ));
						bool AOldValue2 = C_INSTANCE.GetPixelValue( C_START_PIXEL + i );

						C_INSTANCE.SetPixelValue( C_START_PIXEL + i, AOldValue1 );
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ( C_COUNT_PIXELS - i - 1 ), AOldValue2 );
					}
				}

		}

	protected:
		void AnimatePixels()
		{
			if( Reversed().Reversed() )
			{
				bool AOldValue = C_INSTANCE.GetPixelValue( C_START_PIXEL );
				T_OutputPin::SetPinValue( AOldValue );
				for( int i = 0; i < C_COUNT_PIXELS - 1; ++i )
				{
					AOldValue = C_INSTANCE.GetPixelValue( C_START_PIXEL + i + 1 );
					C_INSTANCE.SetPixelValue( C_START_PIXEL + i, AOldValue );
				}

				C_INSTANCE.SetPixelValue( C_START_PIXEL + C_COUNT_PIXELS - 1, InitialValue() );
			}

			else
			{
				bool AOldValue = C_INSTANCE.GetPixelValue( C_START_PIXEL + C_COUNT_PIXELS - 1 );
				T_OutputPin::SetPinValue( AOldValue );
				for( int i = C_COUNT_PIXELS - 1; i--; )
				{
					AOldValue = C_INSTANCE.GetPixelValue( C_START_PIXEL + i );
					C_INSTANCE.SetPixelValue( C_START_PIXEL + i + 1, AOldValue );
				}

				C_INSTANCE.SetPixelValue( C_START_PIXEL, InitialValue() );
			}
//			FOwner->FModified = true;
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			if( Enabled() )
				C_INSTANCE.SetPixelValue( C_START_PIXEL, InitialValue() );

			else
				C_INSTANCE.SetPixelValue( C_START_PIXEL, false );
		}

	public:
		inline void StepInputPin_o_Receive( void *_Data )
		{
			AnimatePixels();
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

		inline void SystemLoopEnd()
		{
			AnimatePixels();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		uint16_t C_REPEAT_COUNT, 
		uint16_t C_SUB_PIXEL_COUNT,
		typename T_Enabled
	> class MaximLedRepeatGroup :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		void SetPixelValue( int AIndex, bool AValue )
		{
			if( Enabled() )
				for( int i = 0; i < C_REPEAT_COUNT; ++i )
					C_INSTANCE.SetPixelValue( C_START_PIXEL + AIndex + i * C_SUB_PIXEL_COUNT, AValue );

		}

		inline bool GetPixelValue( int AIndex )
		{
			return C_INSTANCE.GetPixelValue( C_START_PIXEL + AIndex );
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			// TODO: Try to implement
		}

/*
		virtual void StartPixels( int &AStartPixel )  override
		{
			FStartPixel = AStartPixel;
			inherited::StartPixels( AStartPixel );

			FSubPixelCount = 0;
			for( int i = 0; i < FPixelGroups.size(); ++i )
				FPixelGroups[ i ]->StartPixels( FSubPixelCount );

			if( FSubPixelCount == 0 )
				FRepeatCount = 0;

			else
				FRepeatCount = ( C_COUNT_PIXELS + FSubPixelCount - 1 ) / FSubPixelCount;

		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif