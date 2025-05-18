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

#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "hardware/structs/scb.h"
#include "hardware/pll.h"
#include "hardware/xosc.h"
#include "hardware/structs/rosc.h"
#include "hardware/regs/io_bank0.h"
//#include "stdlib.h"
#include <stdio.h>
//#include "pico/stdlib.h"
//#include "stdio.h"
//#include "include/pico/stdlib.h"
//#include "pico/sleep.h"
//#include <RP2040_RTC.h>
//#include "hardware/rtc.h"
//#include "pico/util/datetime.h"

//#include "rtc_api.h"

//extern "C" 
//{
//void setup_default_uart();
//}


namespace Mitov
{
//---------------------------------------------------------------------------
	namespace RP2040Sleep
	{
		inline bool rosc_write_okay(void) 
		{
			return !(rosc_hw->status & ROSC_STATUS_BADWRITE_BITS);
		}

		inline void rosc_clear_bad_write(void) 
		{
			hw_clear_bits(&rosc_hw->status, ROSC_STATUS_BADWRITE_BITS);
		}

		inline void rosc_write(io_rw_32 *addr, uint32_t value) 
		{
			rosc_clear_bad_write();
			assert(rosc_write_okay());
			*addr = value;
			assert(rosc_write_okay());
		};

		void rosc_disable(void) 
		{
			uint32_t tmp = rosc_hw->ctrl;
			tmp &= (~ROSC_CTRL_ENABLE_BITS);
			tmp |= (ROSC_CTRL_ENABLE_VALUE_DISABLE << ROSC_CTRL_ENABLE_LSB);
			rosc_write(&rosc_hw->ctrl, tmp);
			// Wait for stable to go away
			while(rosc_hw->status & ROSC_STATUS_STABLE_BITS);
		}

		enum dormant_source_t 
		{
			DORMANT_SOURCE_NONE,
			DORMANT_SOURCE_XOSC,
			DORMANT_SOURCE_ROSC
		};

		// In order to go into dormant mode we need to be running from a stoppable clock source:
		// either the xosc or rosc with no PLLs running. This means we disable the USB and ADC clocks
		// and all PLLs
		void sleep_run_from_dormant_source(dormant_source_t dormant_source) 
		{
//			assert(dormant_source_valid(dormant_source));
//			_dormant_source = dormant_source;

			// FIXME: Just defining average rosc freq here.
			uint src_hz = (dormant_source == DORMANT_SOURCE_XOSC) ? XOSC_MHZ * MHZ : 6.5 * MHZ;
			uint clk_ref_src = (dormant_source == DORMANT_SOURCE_XOSC) ?
							   CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC :
							   CLOCKS_CLK_REF_CTRL_SRC_VALUE_ROSC_CLKSRC_PH;

			// CLK_REF = XOSC or ROSC
			clock_configure(clk_ref,
							clk_ref_src,
							0, // No aux mux
							src_hz,
							src_hz);

			// CLK SYS = CLK_REF
			clock_configure(clk_sys,
							CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF,
							0, // Using glitchless mux
							src_hz,
							src_hz);

			// CLK USB = 0MHz
			clock_stop(clk_usb);

			// CLK ADC = 0MHz
			clock_stop(clk_adc);

			// CLK RTC = ideally XOSC (12MHz) / 256 = 46875Hz but could be rosc
			uint clk_rtc_src = (dormant_source == DORMANT_SOURCE_XOSC) ? 
							   CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_XOSC_CLKSRC : 
							   CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_ROSC_CLKSRC_PH;

			clock_configure(clk_rtc,
							0, // No GLMUX
							clk_rtc_src,
							src_hz,
							46875);

			// CLK PERI = clk_sys. Used as reference clock for Peripherals. No dividers so just select and enable
			clock_configure(clk_peri,
							0,
							CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
							src_hz,
							src_hz);

			pll_deinit( pll_sys );
			pll_deinit( pll_usb );

			// Assuming both xosc and rosc are running at the moment
			if (dormant_source == DORMANT_SOURCE_XOSC) 
				// Can disable rosc
				rosc_disable();

			else 
				// Can disable xosc
				xosc_disable();


			// Reconfigure uart with new clocks
//			setup_default_uart();
		}

		void recover_from_sleep(uint scb_orig, uint clock0_orig, uint clock1_orig)
		{
			//Re-enable ring Oscillator control
			rosc_write(&rosc_hw->ctrl, ROSC_CTRL_ENABLE_BITS);

			//reset procs back to default
			scb_hw->scr = scb_orig;
			clocks_hw->sleep_en0 = clock0_orig;
			clocks_hw->sleep_en1 = clock1_orig;

			//reset clocks
			clocks_init();
//			stdio_init_all();
		}

		void rosc_set_dormant(void) 
		{
			// WARNING: This stops the rosc until woken up by an irq
			rosc_write(&rosc_hw->dormant, ROSC_DORMANT_VALUE_DORMANT);
			// Wait for it to become stable once woken up
			while(!(rosc_hw->status & ROSC_STATUS_STABLE_BITS));
		}
	}
//---------------------------------------------------------------------------
	template <
		typename T_UseRingOscillator
	> class RP2040ProcessorSleep :
		public T_UseRingOscillator
	{
	public:
		_V_PROP_( UseRingOscillator )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			RP2040Sleep::sleep_run_from_dormant_source( ( UseRingOscillator() ) ? RP2040Sleep::DORMANT_SOURCE_ROSC : RP2040Sleep::DORMANT_SOURCE_XOSC );

			uint scb_orig = scb_hw->scr;
			uint clock0_orig = clocks_hw->sleep_en0;
			uint clock1_orig = clocks_hw->sleep_en1;

//			gpio_set_dormant_irq_enabled( 0, IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_HIGH_BITS, true );

			// Turn off all clocks when in sleep mode except for RTC
			clocks_hw->sleep_en0 = CLOCKS_SLEEP_EN0_CLK_RTC_RTC_BITS;
			clocks_hw->sleep_en1 = 0x0;

//			rtc_set_alarm(t, callback);

			uint save = scb_hw->scr;
			// Enable deep sleep at the proc
			scb_hw->scr = save | M0PLUS_SCR_SLEEPDEEP_BITS;

			// Go to sleep
			__wfi();

			//reset processor and clocks back to defaults
			RP2040Sleep::recover_from_sleep( scb_orig, clock0_orig, clock1_orig );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Edge,
		typename T_High,
		typename T_Pin
	> class RP2040ProcessorWakeUpByPin :
		public T_Edge,
		public T_High,
		public T_Pin
	{
	public:
		_V_PROP_( Edge )
		_V_PROP_( High )
		_V_PROP_( Pin )

	public:
		inline void UpdateRegister()
		{
			uint32_t AEvent = 0;

			if ( ( ! Edge().GetValue() ) && ( ! High().GetValue() ) ) 
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_LOW_BITS;

			if ( ( ! Edge().GetValue() ) && High().GetValue() )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_HIGH_BITS;

			if ( Edge().GetValue() && High().GetValue() )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_HIGH_BITS;

			if ( Edge().GetValue() && ( ! High().GetValue() ) )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_LOW_BITS;

			gpio_set_dormant_irq_enabled( Pin().GetValue(), AEvent, true );
		}

		inline void UpdateUnRegister()
		{
			gpio_set_dormant_irq_enabled( Pin().GetValue(), IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_LOW_BITS, false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Count,
		typename T_Edge,
		typename T_High,
		typename T_StartPin
	> class RP2040ProcessorWakeUpByPinsCount :
		public T_Count,
		public T_Edge,
		public T_High,
		public T_StartPin
	{
	public:
		_V_PROP_( Count )
		_V_PROP_( Edge )
		_V_PROP_( High )
		_V_PROP_( StartPin )

	public:
		inline void UpdateRegister()
		{
			uint32_t AEvent = 0;

			if ( ( ! Edge().GetValue() ) && ( ! High().GetValue() ) ) 
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_LOW_BITS;

			if ( ( ! Edge().GetValue() ) && High().GetValue() )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_HIGH_BITS;

			if ( Edge().GetValue() && High().GetValue() )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_HIGH_BITS;

			if ( Edge().GetValue() && ( ! High().GetValue() ) )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_LOW_BITS;

			uint8_t AMax = MitovMin<uint8_t>(  StartPin().GetValue() + Count(), 30 );
			for( uint8_t i = StartPin().GetValue(); i < AMax; ++ i )
				gpio_set_dormant_irq_enabled( i, AEvent, true );

		}

		inline void UpdateUnRegister()
		{
			uint32_t AEvent = 0;

			if ( ( ! Edge().GetValue() ) && ( ! High().GetValue() ) ) 
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_LOW_BITS;

			if ( ( ! Edge().GetValue() ) && High().GetValue() )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_HIGH_BITS;

			if ( Edge().GetValue() && High().GetValue() )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_HIGH_BITS;

			if ( Edge().GetValue() && ( ! High().GetValue() ) )
					AEvent = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_LOW_BITS;

			uint8_t AMax = MitovMin<uint8_t>(  StartPin().GetValue() + Count(), 30 );
			for( uint8_t i = StartPin().GetValue(); i < AMax; ++ i )
			    gpio_acknowledge_irq( i, AEvent );

			for( uint8_t i = StartPin().GetValue(); i < AMax; ++ i )
				gpio_set_dormant_irq_enabled( i, IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_LOW_BITS, false );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_UseRingOscillator,
		typename T_Wakeup_UpdateRegister,
		typename T_Wakeup_UpdateUnRegister
	> class RP2040ProcessorDormant :
		public T_UseRingOscillator
	{
	public:
		_V_PROP_( UseRingOscillator )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_Wakeup_UpdateRegister::Call();
			if( UseRingOscillator() )
				RP2040Sleep::rosc_set_dormant();

			else
				xosc_dormant();

			T_Wakeup_UpdateUnRegister::Call();
		}

	};
//---------------------------------------------------------------------------
}

