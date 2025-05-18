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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	class PLDiono
	{
	public:
		enum Pin
		{
			RELAY1 = 2,
			RELAY2 = 3,
			RELAY3 = 4,
			RELAY4 = 5,
			RELAY5 = 6,
			RELAY6 = 7,

			UART_RXD = 14,
			UART_TXD = 15,

			SD_CS = 53,

			LED_PIN = 13,

			SPEAKER_PIN = 9,

			RTC_INT = 19,
			RTC_SDA = 20,
			RTC_SCL = 21,

			LCD_PWR 	= 27,
			LCD_DC 		= 49,
			LCD_CS 		= 47,
			LCD_MOSI 	= 51,
			LCD_CLK 	= 52,
			LCD_RST 	= 48,
			LCD_MISO 	= 50,
			LCD_BACKLIGHT = 46,

			TOUCH_IRQ = 18,
			TOUCH_DO  = 50,
			TOUCH_DIN = 51,
			TOUCH_CS  = 28,
			TOUCH_CLK = 52,

			ESP_PWR		= 26,
			ESP_CHPD	= 25,
			ESP_GPIO0	= 24,
			ESP_GPIO2	= 23,
			ESP_RST		= 22
		};

	public:
		static void Init()
		{
			// Setup digital input pins
			for(int i=0; i<8; ++i)
			{
				pinMode(30+i, INPUT);
				digitalWrite(30+i, HIGH);
			}

			// Setup analog input pins
			for(int i=0; i<8; ++i)
			{
				pinMode(A0+i, INPUT);
				digitalWrite(A0+i, LOW);
			}

			// Setup digital output pins
			for(int i=0; i<8; ++i)
				pinMode(38+i, OUTPUT);

			// Setup relays
			pinMode(RELAY1, OUTPUT);
			pinMode(RELAY2, OUTPUT);
			pinMode(RELAY3, OUTPUT);
			pinMode(RELAY4, OUTPUT);
			pinMode(RELAY5, OUTPUT);
			pinMode(RELAY6, OUTPUT);

			// Setup LED
			pinMode(LED_PIN, OUTPUT);

			// Enable 5v-3.3v converter
			DDRH |= (1<<PH2); // BOE on PH2 = output
			PORTH |= (1<<PH2); // set BOE High
			delay(200);
			PORTH &= ~(1<<PH2); // BOE low
	    
			pinMode( LCD_CS, OUTPUT);
			digitalWrite( LCD_CS, HIGH);
		}

		static void EnableMicroSD()
		{
			pinMode( TOUCH_CS, OUTPUT); digitalWrite( TOUCH_CS, HIGH ); 
			pinMode( LCD_PWR, OUTPUT); digitalWrite( LCD_PWR, HIGH );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_0_PINS,
		typename T_Enabled
//		typename T_Serial
	> class ArduinoPLDuinoWiFi :
		public T_Enabled
//		public T_Serial
	{
	public:
		_V_PROP_( Enabled )
//		_V_PROP_( Serial )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
				pinMode( PLDiono::ESP_CHPD, OUTPUT); digitalWrite( PLDiono::ESP_CHPD, HIGH);
				pinMode( PLDiono::ESP_GPIO0, OUTPUT); digitalWrite( PLDiono::ESP_GPIO0, HIGH);
				pinMode( PLDiono::ESP_GPIO2, OUTPUT); digitalWrite( PLDiono::ESP_GPIO2, HIGH);
				pinMode( PLDiono::ESP_RST, OUTPUT); digitalWrite( PLDiono::ESP_RST, HIGH);
				pinMode( PLDiono::ESP_PWR, OUTPUT); digitalWrite( PLDiono::ESP_PWR, HIGH);

				Reset();
			}

			else
			{
//				pinMode( PLDiono::ESP_CHPD, INPUT); digitalWrite( PLDiono::ESP_CHPD, HIGH);
//				pinMode( PLDiono::ESP_GPIO0, INPUT); digitalWrite( PLDiono::ESP_GPIO0, HIGH);
//				pinMode( PLDiono::ESP_GPIO2, INPUT); digitalWrite( PLDiono::ESP_GPIO2, HIGH);
//				pinMode( PLDiono::ESP_RST, INPUT); digitalWrite( PLDiono::ESP_RST, HIGH);
				pinMode( PLDiono::ESP_PWR, OUTPUT); digitalWrite( PLDiono::ESP_PWR, LOW);
			}

			T_0_PINS::UpdatePin_0();
			T_0_PINS::UpdatePin_2();
			T_0_PINS::UpdatePin_15();
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	protected:
		void Reset()
		{
			if( Enabled() )
			{
				digitalWrite( PLDiono::ESP_RST, LOW );
				digitalWrite( PLDiono::ESP_CHPD, LOW );
				delay(250);
				digitalWrite( PLDiono::ESP_CHPD, HIGH );
				digitalWrite( PLDiono::ESP_RST, HIGH );
			}

			T_0_PINS::UpdatePin_0();
			T_0_PINS::UpdatePin_2();
			T_0_PINS::UpdatePin_15();
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

		inline void SystemStart()
		{
		}

		inline void SystemLoopBegin() {} // Placeholder
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif