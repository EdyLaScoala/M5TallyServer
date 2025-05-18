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
#include <Mitov_Display_ILI9341_Basic.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
namespace ILI9341Standard
{
	static const uint8_t PROGMEM
		Init[] =
		{
			0xCF, 3,	// 1. Power Control B - http://www.newhavendisplay.com/app_notes/ILI9341.pdf
			0x00,
			0xC1,
			0x30,

			0xED, 4,	//  2. Power on sequence contro
			0x64,
			0x03, 
			0x12, 
			0x81,

			0xE8, 3,	//  3. Driver timing control A
			0x85,
			0x00, 
			0x78, 

			0xCB, 5,	//  4. Power control A 
			0x39,
			0x2C, 
			0x00, 
			0x34, 
			0x02, 

			0xF7, 1,	// 5. Pump ratio control
			0x20,

			0xEA, 2,	// 6. Driver timing control B
			0x00, 
			0x00,

			0xC0, 1,    //  7. Power control - https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf
			0x18,		//VRH[5:0] 

			0xC1, 1,    // 8. Power control 
			0x12,		//SAP[2:0];BT[3:0] 

			0xC5, 2,    // 9. VCOM control 
			0x31,
			0x3C,

			0xC7, 1,    // 10. VCOM control2 
			0x99,

			//--

			0x3A, 1,    // 11. Pixel Format Set
			0x55,

			0xB1, 2,    // 12. Frame Control
			0x00,
			0x16,

			0xB6, 2,    // 13. Display Function Control 
			0x0A,
			0xA2,

			0xF6, 2,	// 14. Interface Control 
			0x01,
			0x30,

			0xF2, 1,    // 15. 3Gamma Function Disable 
			0x00,

			0x26, 1,    // 16. Gamma curve selected 
			0x02,

			0xE0, 15,   // 17. Set Gamma 
			0x0F,
			0x1D,
			0x1A,
			0x0A,
			0x0D,
			0x07,
			0x49,
			0x66,
			0x3B,
			0x07,
			0x11,
			0x01,
			0x09,
			0x05,
			0x04,

			0xE1, 15,   // 18. Set Gamma 
			0x00,
			0x18,
			0x1D,
			0x02,
			0x0F,
			0x04,
			0x36,
			0x13,
			0x4C,
			0x07,
			0x13,
			0x0F,
			0x2E,
			0x2F,
			0x05,


			0x11, 0,    // 19. Exit Sleep 

			ILI9341Extended::INIT_DELAY, 120,	// 20.

			0x29, 0,    // 21. Display on 
	
			0x2A, 4,    // 22. Set Gamma 
			0x00,
			0x00,
			0x00,
			0xEF,

			0x2B, 4,    // 23. Set Gamma 
			0x00,
			0x00,
			0x01,
			0x3F,

			0x2C		// 24. Memory Write  
		};

};

/*
	namespace DisplayILI9341Const
	{
		const uint16_t WIDTH = 240;
		const uint16_t HEIGHT = 320;
	};
*/
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
		typename T_Inverted,
		typename T_Orientation,
		typename T_Text
	> class DisplayILI9341 : 
		public DisplayILI9341_Basic <
				T_0_IMPLEMENTATION,
                T_0_ORIENTATION_IMPLEMENTATION,
				T_BackgroundColor,
				C_HAS_READ,
                C_HEIGHT,
                C_WIDTH,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				true,
				T_Orientation,
				T_Text
			>,
		public T_Inverted
	{
		typedef DisplayILI9341_Basic <
				T_0_IMPLEMENTATION,
                T_0_ORIENTATION_IMPLEMENTATION,
				T_BackgroundColor,
				C_HAS_READ,
                C_HEIGHT,
                C_WIDTH,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				true,
				T_Orientation,
				T_Text
			> inherited;
            
         typedef DisplayILI9341 T_SELF;

//	public:
//		TArduinoGraphicsOrientation	Orientation = goUp;

	public:
		_V_PROP_( Inverted )

	public:
		inline void UpdateInverted()
		{
			if( Inverted() )
				inherited::cmd( 0x21 );

			else
				inherited::cmd( 0x20 );

		}

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::Call();
		}

//	public:
//		constexpr inline static bool ImplementsPolygon()
//		{
//			return false;
//		}

//		template <typename T_POLYGON> void RenderPolygon( T_POLYGON &APolygon, TGraphicsPos AX, TGraphicsPos AY, bool AFill, TUniColor AColor ) {} // Placeholder

/*
		inline int16_t width(void)
		{
			return T_0_ORIENTATION_IMPLEMENTATION::template GetWidth<T_SELF>( this );
		}

		inline int16_t height(void)
		{
			return T_0_ORIENTATION_IMPLEMENTATION::template GetHeight<T_SELF>( this );
		}
*/
	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			inherited::SendCommandSequence( ILI9341Standard::Init, sizeof( ILI9341Standard::Init ) );
		}

		inline void SystemStart()
		{
			UpdateInverted();
			inherited::SystemStart();
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

