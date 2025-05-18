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
#include <TVout.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoTVOutRCAElementColor { tvcBlack, tvcWhite, tvcInvert, tvcNone = -1 };
	typedef TArduinoTVOutRCAElementColor TArduinoTVOutRCAElementColorFill;
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TVBegin,
		typename T_Enabled,
		typename T_Height,
		typename T_IsPAL,
		typename T_Width
	> class TVOut_RCA :
		public T_Enabled,
		public T_Height,
		public T_IsPAL,
		public T_Width
	{
    public:
        _V_PROP_( Enabled )
		_V_PROP_( IsPAL )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				TVBegin();

			else
				FTV.end();
		}

		void UpdateSize()
		{
			if( ! Enabled().GetValue )
				return;

			FTV.end();
			UpdateEnabled();
		}

	public:
		TVout	FTV;

	public:
		void TV_shift( bool _InVertical, int32_t _Distance )
		{
			if( ! Enabled() )
				return;

			uint8_t ADistance = abs( _Distance );
			uint8_t ADirection;
			if( _InVertical )
			{
				if( _Distance < 0 )
					ADirection = UP;

				else
					ADirection = DOWN;

			}
			else
			{
				if( _Distance < 0 )
					ADirection = LEFT;

				else
					ADirection = RIGHT;

			}

			FTV.shift( ADistance, ADirection );
		}

		void TV_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c)
		{
			if( Enabled() )
				FTV.draw_line( x0, y0, x1, y1, c );
		}

		void TV_draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, char c, char fc = -1)
		{
			if( Enabled() )
				FTV.draw_circle( x0, y0, radius, c, fc );
		}

		void TV_draw_rect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char c, char fc = -1)
		{
			if( Enabled() )
				FTV.draw_rect( x0, y0, w, h, c, fc );
		}

		void TV_set_pixel(uint8_t x, uint8_t y, char c)
		{
			if( Enabled() )
				FTV.set_pixel( x, y, c );
		}

		unsigned char TV_get_pixel(uint8_t x, uint8_t y)
		{
			return FTV.get_pixel( x, y );
		}

		void TV_bitmap(uint8_t x, uint8_t y, const unsigned char * bmp, uint16_t i = 0, uint8_t width = 0, uint8_t lines = 0)
		{
			if( Enabled() )
				FTV.bitmap( x, y, bmp, i, width, lines );
		}

		void TV_set_cursor( uint8_t x, uint8_t y )
		{
			if( Enabled() )
				FTV.set_cursor( x, y );
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled();
		}

		inline TVout &GetTV()
		{
			return FTV;
		}

	protected:
		void TVBegin()
		{
			FTV.begin( IsPAL ? PAL : NTSC, Width(), Height() );

			T_Elements_TVBegin::Call();

//			for( int i = 0; i < FElements.size(); i ++ )
//				FElements[ i ]->TVBegin();

//			FTV.select_font( font4x6 );
		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				FTV.begin( IsPAL() ? PAL : NTSC, Width(), Height() );

		}

		inline void SystemStart()
		{
			if( Enabled() )
				T_Elements_TVBegin::Call();
//				for( int i = 0; i < FElements.size(); i ++ )
//					FElements[ i ]->TVBegin();

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Color
	> class TVOutRCAElementFillScreen :
		public T_ClockInputPin_o_IsConnected,
		public T_Color
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Color )

	public:
		void Render()
		{
			if( C_OWNER.IsEnabled() )
				C_OWNER.GetTV().fill( (int)Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Duration,
		typename T_Frequency
	> class TVOutRCAElementPlayTone :
		public T_ClockInputPin_o_IsConnected,
		public T_Duration,
		public T_Frequency
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Frequency )
		_V_PROP_( Duration )

	public:
		void Render()
		{
			if( C_OWNER.IsEnabled() )
			{
				if( Frequency() == 0 )
					C_OWNER.GetTV().noTone();

				else
					C_OWNER.GetTV().tone( Frequency(), Duration() );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BorderColor,
		typename T_ClockInputPin_o_IsConnected,
		typename T_FillColor,
		typename T_Radius,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementDrawCircle :
		public T_BorderColor,
		public T_ClockInputPin_o_IsConnected,
		public T_FillColor,
		public T_Radius,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( BorderColor )
		_V_PROP_( FillColor )

		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Radius )

	public:
		void Render()
		{
			C_OWNER.TV_draw_circle( X(), Y(), Radius(), (int)BorderColor().GetValue(), (int)FillColor().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BorderColor,
		typename T_ClockInputPin_o_IsConnected,
		typename T_FillColor,
		typename T_Height,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementDrawRectangle :
		public T_BorderColor,
		public T_ClockInputPin_o_IsConnected,
		public T_FillColor,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( BorderColor )
		_V_PROP_( FillColor )

		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		void Render()
		{
			C_OWNER.TV_draw_rect( X(), Y(), Width(), Height(), (int)BorderColor().GetValue(), (int)FillColor().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Color,
		typename T_X1,
		typename T_X2,
		typename T_Y1,
		typename T_Y2
	> class TVOutRCAElementDrawLine :
		public T_ClockInputPin_o_IsConnected,
		public T_Color,
		public T_X1,
		public T_X2,
		public T_Y1,
		public T_Y2
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Color )

	public:
		_V_PROP_( X1 )
		_V_PROP_( Y1 )
		_V_PROP_( X2 )
		_V_PROP_( Y2 )

	public:
		void Render()
		{
			C_OWNER.TV_draw_line( X1(), Y1(), X2(), Y2(), (int)Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Color,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementDrawPixel :
		public T_ClockInputPin_o_IsConnected,
		public T_Color,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Color )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void Render()
		{
			C_OWNER.TV_set_pixel( X(), Y(), (int)Color().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Distance,
		typename T_InVertical
	> class TVOutRCAElementShiftScreen :
		public T_ClockInputPin_o_IsConnected,
		public T_Distance,
		public T_InVertical
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Distance )
		_V_PROP_( InVertical )

	public:
		void Render()
		{
			C_OWNER.TV_shift( InVertical(), Distance() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Elements_TVBegin,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementDrawScene :
		public T_ClockInputPin_o_IsConnected,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline bool IsEnabled()
		{
			return C_OWNER.IsEnabled();
		}

		inline TVout &GetTV()
		{
			return C_OWNER.GetTV();
		}

		inline void TV_shift( bool _InVertical, int32_t _Distance )
		{
			C_OWNER.TV_shift( _InVertical, _Distance );
		}

		inline void TV_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c)
		{
			C_OWNER.TV_draw_line( X() + x0, Y() + y0, X() + x1, Y() + y1, c );
		}

		inline void TV_bitmap(uint8_t x, uint8_t y, const unsigned char * bmp, uint16_t i = 0, uint8_t width = 0, uint8_t lines = 0)
		{
			C_OWNER.TV_bitmap( X() + x , Y() + y, bmp, i, width, lines );
		}

		inline void TV_set_cursor( uint8_t x, uint8_t y )
		{
			C_OWNER.TV_set_cursor( X() + x , Y() + y );
		}

		inline void TV_draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, char c, char fc = -1)
		{
			C_OWNER.TV_draw_circle( X() + x0, Y() + y0, radius, c, fc );
		}

		inline void TV_draw_rect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char c, char fc = -1)
		{
			C_OWNER.TV_draw_rect( X() + x0, Y() + y0, w, h, c, fc );
		}

		inline void TV_set_pixel(uint8_t x, uint8_t y, char c)
		{
			C_OWNER.TV_set_pixel( X() + x , Y() + y, c );
		}

		inline unsigned char TV_get_pixel(uint8_t x, uint8_t y)
		{
			return C_OWNER.TV_get_pixel( X() + x , Y() + y );
		}

	public:
		void Render()
		{
			T_Elements_TVBegin::Call();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_OutputPin,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementCheckPixel :
		public T_ClockInputPin_o_IsConnected,
		public T_OutputPin,
		public T_X,
		public T_Y
	{
    public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void Render()
		{
			if( C_OWNER.IsEnabled() )
				T_OutputPin::SetPinValue( C_OWNER.TV_get_pixel( X(), Y() ) > 0 );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementDrawBitmap :
		public T_ClockInputPin_o_IsConnected,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )

    public:
		uint8_t	Width;
		uint8_t	Height;

		const unsigned char *_Bytes;

	public:
		void Render()
		{
			C_OWNER.TV_bitmap( X(), Y(), _Bytes, 0, Width, Height );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementSetCursor :
		public T_ClockInputPin_o_IsConnected,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void Render()
		{
			C_OWNER.TV_set_cursor( X(), Y() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NewLine
	> class TVOutRCAElementPrintText :
		public T_NewLine
	{
	public:
		_V_PROP_( NewLine )

	protected:
		const unsigned char *FFont;

	public:
		template <typename T> void Print( T AValue )
		{
			if( C_OWNER.IsEnabled() )
			{
				C_OWNER.GetTV().select_font( FFont );
				if( NewLine )
					C_OWNER.GetTV().println( AValue );

				else
					C_OWNER.GetTV().print( AValue );
			}
		}

	public:
		inline void TVBegin()
		{
		}

	public:
		TVOutRCAElementPrintText( const unsigned char * AFont ) :
			FFont( AFont )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_InitialValue,
		typename T_X,
		typename T_Y
	> class TVOutRCAElementTextAt :
		public T_ClockInputPin_o_IsConnected,
		public T_InitialValue,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( InitialValue )

	protected:
		const unsigned char *FFont;

	public:
		template <typename T> void Print( T AValue )
		{
			 InitialValue() = Mitov::String( AValue );
		}

	public:
		void Render()
		{
			if( C_OWNER.IsEnabled() )
			{
				C_OWNER.GetTV().select_font( FFont );
				C_OWNER.TV_set_cursor( X(), Y() );
				C_OWNER.GetTV().print( InitialValue().c_str() );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		inline void TVBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				Render();
		}

	public:
		TVOutRCAElementTextAt( const unsigned char * AFont ) :
			FFont( AFont )
		{
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif