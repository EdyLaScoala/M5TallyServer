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
namespace ILI9341_M5_Stack
{
	static const uint8_t PROGMEM
		Init[] =
		{
			0xEF, 3,
			0x03,
			0x80,
			0x02,

			0xCF, 3,
			0x00,
			0XC1,
			0X30,

			0xED, 4,
			0x64,
			0x03,
			0X12,
			0X81,


			0xE8, 3,
			0x85,
			0x00,
			0x78,

			0xCB, 5,
			0x39,
			0x2C,
			0x00,
			0x34,
			0x02,

			0xF7, 1,
			0x20,

			0xEA, 2,
			0x00,
			0x00,

			ILI9341Extended::PWCTR1, 1,    // Power control
			0x23,   // VRH[5:0]

			ILI9341Extended::PWCTR2, 1,    // Power control
			0x10,   // SAP[2:0];BT[3:0]

			ILI9341Extended::VMCTR1, 2,    // VCM control
			0x3e,
			0x28,

			ILI9341Extended::VMCTR2, 1,   // VCM control2
			0x86,  // --

			ILI9341Extended::MADCTL, 1,    // Memory Access Control
			0xA8, // Rotation 0 (portrait mode)

			ILI9341Extended::PIXFMT, 1,
			0x55,

			ILI9341Extended::FRMCTR1, 2,
			0x00,
			0x13, // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz

			ILI9341Extended::DFUNCTR, 3,    // Display Function Control
			0x08,
			0x82,
			0x27,

			0xF2, 1,    // 3Gamma Function Disable
			0x00,

			ILI9341Extended::GAMMASET, 1,    // Gamma curve selected
			0x01,

			ILI9341Extended::GMCTRP1, 15,    // Set Gamma
			0x0F,
			0x31,
			0x2B,
			0x0C,
			0x0E,
			0x08,
			0x4E,
			0xF1,
			0x37,
			0x07,
			0x10,
			0x03,
			0x0E,
			0x09,
			0x00,

			ILI9341Extended::GMCTRN1, 15,    // Set Gamma
			0x00,
			0x0E,
			0x14,
			0x03,
			0x11,
			0x07,
			0x31,
			0xC1,
			0x48,
			0x08,
			0x0F,
			0x0C,
			0x31,
			0x36,
			0x0F,

			ILI9341Extended::SLPOUT, 0,     // Exit Sleep
			ILI9341Extended::INIT_DELAY, 120,	// 20.
			ILI9341Extended::DISPON, 0     // Display on

//			0x21, 0 // Invert
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
		typename T_BackgroundColor,
		typename T_Brightness,
		uint8_t C_BRIGHNESS_PIN,
        bool C_HAS_READ,
		typename T_Elements_Render,
        typename T_FCursorX,
        typename T_FCursorY,
		typename T_Inverted,
		typename T_Orientation,
		typename T_Text
	> class DisplayILI9341_M5Stack :
		public DisplayILI9341_Basic <
				T_0_IMPLEMENTATION,
                Mitov::DisplayILI9341_Otientation_Up,
				T_BackgroundColor,
				C_HAS_READ,
                240,
                320,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				false,
				T_Orientation,
				T_Text
			>,
		public T_Brightness,
		public T_Inverted
	{
		typedef DisplayILI9341_Basic<
				T_0_IMPLEMENTATION,
                Mitov::DisplayILI9341_Otientation_Up,
				T_BackgroundColor,
				C_HAS_READ,
                240,
                320,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				false,
				T_Orientation,
				T_Text
			> inherited;

	public:
		_V_PROP_( Brightness )
		_V_PROP_( Inverted )

	public:
		inline void UpdateBrightness()
		{
			Analog.Write( C_BRIGHNESS_PIN, Brightness() );
		}

		inline void UpdateInverted()
		{
			if( Inverted() )
				inherited::cmd( 0x20 );

			else
				inherited::cmd( 0x21 );

		}

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			inherited::SendCommandSequence( ILI9341_M5_Stack::Init, sizeof( ILI9341_M5_Stack::Init ) );
		}

		inline void SystemStart()
		{
			pinMode( C_BRIGHNESS_PIN, OUTPUT );
			UpdateInverted();
			UpdateBrightness();
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

