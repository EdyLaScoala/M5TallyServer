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
#include <Mitov_Display_ILI9341.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_0_IMPLEMENTATION,
		typename T_0_ORIENTATION_IMPLEMENTATION,
		typename T_BackgroundColor,
		typename T_Brightness,
		bool C_HAS_READ,
		uint16_t C_HEIGHT,
		uint16_t C_WIDTH,
		typename T_Elements_Render,
        typename T_FCursorX,
        typename T_FCursorY,
		typename T_Inverted,
		typename T_Orientation,
		typename T_Text
	> class TArduino_Seeeduino_Wio_Terminal_DisplayModule :
		public DisplayILI9341 <
                T_0_IMPLEMENTATION,
                T_0_ORIENTATION_IMPLEMENTATION,
				T_BackgroundColor,
				C_HAS_READ,
                C_HEIGHT,
                C_WIDTH,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_Text
			>,
		public T_Brightness
	{
		typedef DisplayILI9341 <
                T_0_IMPLEMENTATION,
                T_0_ORIENTATION_IMPLEMENTATION,
				T_BackgroundColor,
				C_HAS_READ,
                C_HEIGHT,
                C_WIDTH,
				T_Elements_Render,
                T_FCursorX,
                T_FCursorY,
				T_Inverted,
				T_Orientation,
				T_Text
			> inherited;

    protected:
        const uint8_t C_MAX_BRIGHTNESS = 100;

	public:
		_V_PROP_( Brightness )

	public:
		void UpdateBrightness()
		{
//			Analog.Write( LCD_BACKLIGHT, Brightness() );
//			digitalWrite( LCD_BACKLIGHT, HIGH );
//			this->currentBrightness = brightness < this->maxBrightness ? brightness : this->maxBrightness;
			TC0->COUNT8.CC[0].reg = Brightness() * C_MAX_BRIGHTNESS + 0.5;
			while(TC0->COUNT8.SYNCBUSY.bit.CC0);
		}

	public:
		inline void SystemInit()
		{
//			pinMode( LCD_BACKLIGHT, OUTPUT );

			// Enable Peripheral Clocks
			GCLK->PCHCTRL[9].reg = 0 | (1u<<6);         // TC0, TC1
			while(!GCLK->PCHCTRL[9].bit.CHEN);
			GCLK->PCHCTRL[11].reg = 0 | (1u<<6);    // EVSYS[0]
			while(!GCLK->PCHCTRL[11].bit.CHEN);
			GCLK->PCHCTRL[33].reg = 0 | (1u<<6);    // CCL
			while(!GCLK->PCHCTRL[33].bit.CHEN);
			// Enable Peropheral APB Clocks
			MCLK->APBAMASK.bit.TC0_ = 1;
			MCLK->APBBMASK.bit.EVSYS_ = 1;
			MCLK->APBCMASK.bit.CCL_ = 1;

			// Configure PORT
			PORT->Group[2].DIRSET.reg = (1<<5);
			PORT->Group[2].EVCTRL.reg = 0x85; // PC05, OUT
			// Configure EVSYS
			EVSYS->USER[1].reg = 0x01;  // Channel0 -> PORT_EV0
			EVSYS->Channel[0].CHANNEL.reg = 0x74 | (0x02<<8) | (0x00<<10);  // CCL_LUTOUT0, ASYNCHRONOUS, NO_EVT_OUTPUT
			// Configure CCL
			CCL->CTRL.reg = (1<<0); // SWRST
			CCL->SEQCTRL[0].reg = 0x0; // Disable SEQCTRL
			CCL->LUTCTRL[0].reg = (0xaau << 24) | (1u<<22) | (0x6<<8) | (1<<1); // TRUTH=0xaa, LUTEO, INSEL0=0x06(TC), ENABLE
			CCL->CTRL.reg = (1<<1); // ENABLE

			// Configure TC0
			TC0->COUNT8.CTRLA.reg = (1u<<0);   // SWRST;
			while( TC0->COUNT8.SYNCBUSY.bit.SWRST );

			TC0->COUNT8.CTRLA.reg = (0x01 << 2) | (0x01 << 4) | (0x04 << 8);   // MODE=COUNT8, PRESCALER=DIV16, PRESCSYNC=PRESC
			TC0->COUNT8.WAVE.reg  = 0x02; // WAVEGEN=NPWM;
			TC0->COUNT8.CTRLBSET.reg = (1u<<1); // LUPD
			TC0->COUNT8.PER.reg = C_MAX_BRIGHTNESS;
			TC0->COUNT8.CC[0].reg = C_MAX_BRIGHTNESS;
			TC0->COUNT8.CC[1].reg = 0u;
			TC0->COUNT8.DBGCTRL.bit.DBGRUN = 1;
			TC0->COUNT8.INTFLAG.reg = 0x33;    // Clear all flags
			while( TC0->COUNT8.SYNCBUSY.reg );

			TC0->COUNT8.CTRLA.bit.ENABLE = 1;   // ENABLE
			while( TC0->COUNT8.SYNCBUSY.bit.ENABLE );

			inherited::SystemInit();
		}

		inline void SystemStart()
		{
			UpdateBrightness();
			inherited::SystemStart();
		}


	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
