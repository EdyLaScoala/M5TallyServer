////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Display_ST7735.h>
//#include <Mitov_Display_ST7735_Const_R.h>
//#include <Mitov_Display_ST7735_Const_R2.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
namespace ST7789
{
	static const uint8_t PROGMEM
	  InitCmd1[] =
		{                 
			9,                       // 9 commands in list:
			ST7735::SWRESET,   ST7735::DELAY,  //  1: Software reset, 0 args, w/delay
			  150,                    //     150 ms delay
			ST7735::SLPOUT ,   ST7735::DELAY,  //  2: Out of sleep mode, 0 args, w/delay
			  10,                    //     10 ms delay

			ST7735::COLMOD , 1 + ST7735::DELAY,  // 15: set color mode, 1 arg, no delay:
			  0x55,
			  10,                           //     10 ms delay

			ST7735::MADCTL , 1,              //  4: Mem access ctrl (directions), 1 arg:
			  0x08,                         //     Row/col addr, bottom-top refresh
			ST7735::CASET  , 4,              //  5: Column addr set, 4 args, no delay:
			  0x00,
			  0,        //     XSTART = 0
			  0,
			  240,  //     XEND = 240
			ST7735::RASET  , 4,              //  6: Row addr set, 4 args, no delay:
			  0x00,
			  0,             //     YSTART = 0
			  320>>8,
			  320&0xFF,  //     YEND = 320
			ST7735::INVON  ,   ST7735::DELAY,  //  7: hack
			  10,
			ST7735::NORON  ,   ST7735::DELAY, //  8: Normal display on, no args, w/delay
			  10,                           //     10 ms delay
			ST7735::DISPON ,   ST7735::DELAY, //  9: Main screen turn on, no args, delay
			  10                            //    10 ms delay
		};

/*
	  InitCmd2[] =
		{
			3,                       // 3 commands in list:
			ST77XX_INVON  ,   ST_CMD_DELAY,  //  7: hack
				10,
			ST77XX_NORON  ,   ST_CMD_DELAY, //  8: Normal display on, no args, w/delay
				10,                           //     10 ms delay
			ST77XX_DISPON ,   ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
				10 
	  };                          //    10 ms delay
*/
/*
		ST7735::FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
		  0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
		ST7735::FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
		  0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
		ST7735::FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
		  0x01, 0x2C, 0x2D,       //     Dot inversion mode
		  0x01, 0x2C, 0x2D,       //     Line inversion mode
		ST7735::INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
		  0x07,                   //     No inversion
		ST7735::PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
		  0xA2,
		  0x02,                   //     -4.6V
		  0x84,                   //     AUTO mode
		ST7735::PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
		  0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
		ST7735::PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
		  0x0A,                   //     Opamp current small
		  0x00,                   //     Boost frequency
		ST7735::PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
		  0x8A,                   //     BCLK/2, Opamp current small & Medium low
		  0x2A,  
		ST7735::PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
		  0x8A, 0xEE,
		ST7735::VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
		  0x0E,
		ST7735::INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
		ST7735::MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
		  0xC8,                   //     row addr/col addr, bottom to top refresh
		ST7735::COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
		  0x05 };                 //     16-bit color
*/
}
//---------------------------------------------------------------------------
	template <uint16_t C_WIDTH, uint16_t C_HEIGHT> class Display_ST7789_Basic : public Display_ST7735_Common<ST7735::MADCTL_RGB, C_WIDTH, C_HEIGHT>
	{
	public:
		template <typename T> inline static void UpdateInvertedImpl( T *AInstance )
		{
			AInstance->writecommand( ( AInstance->Inverted() ) ? ST7735::INVOFF : ST7735::INVON );
		}

	public:
		template <typename T> inline static void SystemInit( T *AInstance )
		{
			AInstance->commandList( ST7789::InitCmd1 );
		}

	};
//---------------------------------------------------------------------------
	class Display_ST7789_240_240 : public Display_ST7789_Basic<240, 240>
	{
	public:
		inline static void GetPixelOffset( TArduinoGraphicsOrientation AOrientation, uint32_t &AOffset_X, uint32_t &AOffset_Y )
		{
			switch( AOrientation )
            {
                case goUp : 
                {
                    AOffset_X = 0;
                    AOffset_Y = 80;
                    break;
                }
                    
                case goRight : 
                {
                    AOffset_X = 80;
                    AOffset_Y = 0;
                    break;
                }
                
                default:
                {
                    AOffset_X = 0;
                    AOffset_Y = 0;
                    break;
                }
            }           
		}
        
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


