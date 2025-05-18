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
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

extern unsigned int __heap_start;
extern void *__brkval;

// The free list structure as maintained by the 
// avr-libc memory allocation routines.

namespace Mitov
{
	struct __freelist {
	  size_t sz;
	  struct __freelist *nx;
	};
}

// The head of the free list structure
extern struct Mitov::__freelist *__flp;

namespace Mitov
{
//---------------------------------------------------------------------------
#ifndef WDTO_15MS
	#define WDTO_15MS   0
#endif

#ifndef WDTO_30MS
	#define WDTO_30MS   1
#endif

#ifndef WDTO_60MS
	#define WDTO_60MS   2
#endif

#ifndef WDTO_120MS
	#define WDTO_120MS  3
#endif

#ifndef WDTO_250MS
	#define WDTO_250MS  4
#endif

#ifndef WDTO_500MS
	#define WDTO_500MS  5
#endif

#ifndef WDTO_1S
	#define WDTO_1S     6
#endif

#ifndef WDTO_2S
	#define WDTO_2S     7
#endif

	enum TArduinoWatchdogTimerInterval 
	{
		wdt15mS		= WDTO_15MS,
		wdt30mS		= WDTO_30MS,
		wdt60mS		= WDTO_60MS,
		wdt120mS	= WDTO_120MS,
		wdt250mS	= WDTO_250MS,
		wdt500mS	= WDTO_500MS,
		wdt1S		= WDTO_1S,
		wdt2S		= WDTO_2S,
#ifdef VISUINO_ARDUINO_NANO_EVERY
		wdt4S		= WDTO_2S,
		wdt8S		= WDTO_2S
#else
		wdt4S		= WDTO_4S,
		wdt8S		= WDTO_8S
#endif
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		TArduinoWatchdogTimerInterval C_INTERVAL
	> class ArduinoWatchdogTimer :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				wdt_enable( C_INTERVAL );

			else
				wdt_disable();

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				wdt_reset();
		}

		inline void SystemStart()
		{
			UpdateEnabled();
		}

	};
//---------------------------------------------------------------------------
	class ArduinoProcessorReset
	{
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			void(* resetFunc) (void) = 0; //declare reset function at address 0
			resetFunc();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class GetArduinoAvailableRAM : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		// Calculates the size of the free list
		int freeListSize() 
		{
			struct __freelist* current;
			int total = 0;
			for (current = __flp; current; current = current->nx) 
			{
				total += 2; /* Add two bytes for the memory block's header  */
				total += (int) current->sz;
			}

			return total;
		}

		int freeMemory()
		{
			int free_memory;
			if ((int)__brkval == 0) 
				free_memory = ((int)&free_memory) - ((int)&__heap_start);

			else 
			{
				free_memory = ((int)&free_memory) - ((int)__brkval);
				free_memory += freeListSize();
			}

			return free_memory;
		}

		void ReadValue( bool AChangeOnly )
		{
			if( ! Enabled() )
				return;

			T_OutputPin::SetPinValue( freeMemory(), AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue( false );
		}

	public:
		inline void SystemStart()
		{
			ReadValue( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValue( true );
		}

	};
//---------------------------------------------------------------------------
	inline void lowPowerBodOn( uint8_t mode )
	{
		  set_sleep_mode(mode); 
		  cli();				
		  sleep_enable();		
		  sei();				
		  sleep_cpu();			
		  sleep_disable();		
		  sei();				
	}
//---------------------------------------------------------------------------
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)
	inline void lowPowerBodOff( uint8_t mode )
	{
		set_sleep_mode( mode );
		cli();
		sleep_enable();
		sleep_bod_disable();
		sei();
		sleep_cpu();
		sleep_disable();
		sei();
	}
#endif
//---------------------------------------------------------------------------
	inline uint8_t GetArduinoProcessorIntervalValue( uint16_t ATime )
	{
		if( ATime < 30 )
			return WDTO_15MS;

		if( ATime < 60 )
			return WDTO_30MS;

		if( ATime < 120 )
			return WDTO_60MS;

		if( ATime < 250 )
			return WDTO_120MS;

		if( ATime < 500 )
			return WDTO_250MS;

		if( ATime < 1000 )
			return WDTO_500MS;

		if( ATime < 2000 )
			return WDTO_1S;

#ifdef VISUINO_ARDUINO_NANO_EVERY
		return WDTO_2S;
#else
		if( ATime < 4000 )
			return WDTO_2S;

		if( ATime < 8000 )
			return WDTO_4S;

		return WDTO_8S;
#endif
	}
//---------------------------------------------------------------------------
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega88__)
	template <
		typename T_DisableADC,
		typename T_DisableI2C,
		typename T_DisableSPI,
		typename T_DisableSerial,
		typename T_DisableTimer0,
		typename T_DisableTimer1,
		typename T_DisableTimer2,
		typename T_Interval
	> class ArduinoProcessorIdle328 :
		public T_DisableADC,
		public T_DisableI2C,
		public T_DisableSPI,
		public T_DisableSerial,
		public T_DisableTimer0,
		public T_DisableTimer1,
		public T_DisableTimer2,
		public T_Interval
	{
	public:
		_V_PROP_( DisableADC )
		_V_PROP_( DisableI2C )
		_V_PROP_( DisableSPI )
		_V_PROP_( DisableSerial )
		_V_PROP_( DisableTimer0 )
		_V_PROP_( DisableTimer1 )
		_V_PROP_( DisableTimer2 )
		_V_PROP_( Interval )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			// Temporary clock source variable
			unsigned char clockSource = 0;

//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (TCCR2B & CS22)
					clockSource |= (1 << CS22);

				if (TCCR2B & CS21)
					clockSource |= (1 << CS21);

				if (TCCR2B & CS20)
					clockSource |= (1 << CS20);

				// Remove the clock source to shutdown Timer2
				TCCR2B &= ~(1 << CS22);
				TCCR2B &= ~(1 << CS21);
				TCCR2B &= ~(1 << CS20);

				power_timer2_disable();
			}

//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
			{
				ADCSRA &= ~(1 << ADEN);
				power_adc_disable();
			}

//			if (timer1 == TIMER1_OFF)
			if( DisableTimer1().GetValue() )
				power_timer1_disable();

//			if (timer0 == TIMER0_OFF)
			if( DisableTimer0().GetValue() )
				power_timer0_disable();

//			if (spi == SPI_OFF)
			if( DisableSPI().GetValue() )
				power_spi_disable();

//			if (usart0 == USART0_OFF)
			if( DisableSerial().GetValue() )
				power_usart0_disable();

//			if (twi == TWI_OFF)
			if( DisableI2C().GetValue() )
				power_twi_disable();

			if( Interval().GetValue() )
			{
				wdt_enable( GetArduinoProcessorIntervalValue( Interval() ) );
				WDTCSR |= (1 << WDIE);
			}

			lowPowerBodOn(SLEEP_MODE_IDLE);

//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
			{
				power_adc_enable();
				ADCSRA |= (1 << ADEN);
			}

//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (clockSource & CS22)
					TCCR2B |= (1 << CS22);

				if (clockSource & CS21)
					TCCR2B |= (1 << CS21);

				if (clockSource & CS20)
					TCCR2B |= (1 << CS20);

				power_timer2_enable();
			}

//			if (timer1 == TIMER1_OFF)
			if( DisableTimer1().GetValue() )
				power_timer1_enable();

//			if (timer0 == TIMER0_OFF)
			if( DisableTimer0().GetValue() )
				power_timer0_enable();

//			if (spi == SPI_OFF)
			if( DisableSPI().GetValue() )
				power_spi_enable();

//			if (usart0 == USART0_OFF)
			if( DisableSerial().GetValue() )
				power_usart0_enable();

//			if (twi == TWI_OFF)
			if( DisableI2C().GetValue() )
				power_twi_enable();		
		}

	};
#endif
//---------------------------------------------------------------------------
#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega1280__)
	template <
		typename T_DisableADC,
		typename T_DisableI2C,
		typename T_DisableSPI,
		typename T_DisableSerial0,
		typename T_DisableSerial1,
		typename T_DisableSerial2,
		typename T_DisableSerial3,
		typename T_DisableTimer0,
		typename T_DisableTimer1,
		typename T_DisableTimer2,
		typename T_DisableTimer3,
		typename T_DisableTimer4,
		typename T_DisableTimer5,
		typename T_Interval
	> class ArduinoProcessorIdle2560 :
		public T_DisableADC,
		public T_DisableI2C,
		public T_DisableSPI,
		public T_DisableSerial0,
		public T_DisableSerial1,
		public T_DisableSerial2,
		public T_DisableSerial3,
		public T_DisableTimer0,
		public T_DisableTimer1,
		public T_DisableTimer2,
		public T_DisableTimer3,
		public T_DisableTimer4,
		public T_DisableTimer5,
		public T_Interval
	{
	public:
		_V_PROP_( DisableADC )
		_V_PROP_( DisableI2C )
		_V_PROP_( DisableSPI )
		_V_PROP_( DisableSerial0 )
		_V_PROP_( DisableSerial1 )
		_V_PROP_( DisableSerial2 )
		_V_PROP_( DisableSerial3 )
		_V_PROP_( DisableTimer0 )
		_V_PROP_( DisableTimer1 )
		_V_PROP_( DisableTimer2 )
		_V_PROP_( DisableTimer3 )
		_V_PROP_( DisableTimer4 )
		_V_PROP_( DisableTimer5 )
		_V_PROP_( Interval )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			// Temporary clock source variable
			unsigned char clockSource = 0;

//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (TCCR2B & CS22)
					clockSource |= (1 << CS22);

				if (TCCR2B & CS21)
					clockSource |= (1 << CS21);

				if (TCCR2B & CS20)
					clockSource |= (1 << CS20);

				// Remove the clock source to shutdown Timer2
				TCCR2B &= ~(1 << CS22);
				TCCR2B &= ~(1 << CS21);
				TCCR2B &= ~(1 << CS20);

				power_timer2_disable();
			}

//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
			{
				ADCSRA &= ~(1 << ADEN);
				power_adc_disable();
			}

//			if (timer5 == TIMER5_OFF)
			if( DisableTimer5().GetValue() )
				power_timer5_disable();

//			if (timer4 == TIMER4_OFF)
			if( DisableTimer4().GetValue() )
				power_timer4_disable();

//			if (timer3 == TIMER3_OFF)
			if( DisableTimer3().GetValue() )
				power_timer3_disable();

//			if (timer1 == TIMER1_OFF)
			if( DisableTimer1().GetValue() )
				power_timer1_disable();

//			if (timer0 == TIMER0_OFF)
			if( DisableTimer0().GetValue() )
				power_timer0_disable();

//			if (spi == SPI_OFF)
			if( DisableSPI().GetValue() )
				power_spi_disable();

//			if (usart3 == USART3_OFF)
			if( DisableSerial3().GetValue() )
				power_usart3_disable();

//			if (usart2 == USART2_OFF)
			if( DisableSerial2().GetValue() )
				power_usart2_disable();

//			if (usart1 == USART1_OFF)
			if( DisableSerial1().GetValue() )
				power_usart1_disable();

//			if (usart0 == USART0_OFF)
			if( DisableSerial0().GetValue() )
				power_usart0_disable();

//			if (twi == TWI_OFF)
			if( DisableI2C().GetValue() )
				power_twi_disable();

//			if (period != SLEEP_FOREVER)
			if( Interval().GetValue() )
			{
				wdt_enable( GetArduinoProcessorIntervalValue( Interval() ) );
				WDTCSR |= (1 << WDIE);
			}

			lowPowerBodOn(SLEEP_MODE_IDLE);

//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
			{
				power_adc_enable();
				ADCSRA |= (1 << ADEN);
			}

//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (clockSource & CS22)
					TCCR2B |= (1 << CS22);

				if (clockSource & CS21)
					TCCR2B |= (1 << CS21);

				if (clockSource & CS20)
					TCCR2B |= (1 << CS20);

				power_timer2_enable();
			}

//			if (timer5 == TIMER5_OFF)
			if( DisableTimer5().GetValue() )
				power_timer5_enable();

//			if (timer4 == TIMER4_OFF)
			if( DisableTimer4().GetValue() )
				power_timer4_enable();

//			if (timer3 == TIMER3_OFF)
			if( DisableTimer3().GetValue() )
				power_timer3_enable();

//			if (timer1 == TIMER1_OFF)
			if( DisableTimer1().GetValue() )
				power_timer1_enable();

//			if (timer0 == TIMER0_OFF)
			if( DisableTimer0().GetValue() )
				power_timer0_enable();

//			if (spi == SPI_OFF)
			if( DisableSPI().GetValue() )
				power_spi_enable();

//			if (usart3 == USART3_OFF)
			if( DisableSerial3().GetValue() )
				power_usart3_enable();

//			if (usart2 == USART2_OFF)
			if( DisableSerial2().GetValue() )
				power_usart2_enable();

//			if (usart1 == USART1_OFF)
			if( DisableSerial1().GetValue() )
				power_usart1_enable();

//			if (usart0 == USART0_OFF)
			if( DisableSerial0().GetValue() )
				power_usart0_enable();

//			if (twi == TWI_OFF)
			if( DisableI2C().GetValue() )
				power_twi_enable();
		}
	};
#endif
//---------------------------------------------------------------------------
	template <
		typename T_DisableADC,
		typename T_DisableTimer2,
		typename T_Interval
	> class ArduinoProcessorNoiseReduction :
		public T_DisableADC,
		public T_DisableTimer2,
		public T_Interval
	{
	public:
		_V_PROP_( DisableADC )
		_V_PROP_( DisableTimer2 )
		_V_PROP_( Interval )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			// Temporary clock source variable
			unsigned char clockSource = 0;

#if (( !defined(__AVR_ATmega32U4__) ) && ( !defined(VISUINO_ARDUINO_NANO_EVERY )))
//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if( TCCR2B & CS22 )
					clockSource |= (1 << CS22);

				if( TCCR2B & CS21 )
					clockSource |= (1 << CS21);

				if( TCCR2B & CS20 )
					clockSource |= (1 << CS20);

				// Remove the clock source to shutdown Timer2
				TCCR2B &= ~(1 << CS22);
				TCCR2B &= ~(1 << CS21);
				TCCR2B &= ~(1 << CS20);
			}
#endif

#if ( !defined(VISUINO_ARDUINO_NANO_EVERY ))
//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
				ADCSRA &= ~(1 << ADEN);

//			if (period != SLEEP_FOREVER)
			if( Interval().GetValue() )
			{
				wdt_enable( GetArduinoProcessorIntervalValue( Interval() ));
				WDTCSR |= (1 << WDIE);
			}

			lowPowerBodOn( SLEEP_MODE_ADC );
#endif

#if ( !defined(VISUINO_ARDUINO_NANO_EVERY ))
//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
				ADCSRA |= (1 << ADEN);
#endif

#if (( !defined(__AVR_ATmega32U4__) ) && ( !defined(VISUINO_ARDUINO_NANO_EVERY )))
//			if( timer2 == TIMER2_OFF )
			if( DisableTimer2().GetValue() )
			{
				if( clockSource & CS22 )
					TCCR2B |= (1 << CS22);

				if( clockSource & CS21 )
					TCCR2B |= (1 << CS21);

				if( clockSource & CS20 )
					TCCR2B |= (1 << CS20);

			}
#endif
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_DisableADC,
		typename T_DisableBrownOutDetection,
		typename T_DisableTimer2,
		typename T_Interval
	> class ArduinoProcessorPowerSave :
		public T_DisableADC,
		public T_DisableBrownOutDetection,
		public T_DisableTimer2,
		public T_Interval
	{
	public:
		_V_PROP_( DisableADC )
		_V_PROP_( DisableBrownOutDetection )
		_V_PROP_( DisableTimer2 )
		_V_PROP_( Interval )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			// Temporary clock source variable
			unsigned char clockSource = 0;

		#if (( !defined(__AVR_ATmega32U4__) ) && ( !defined(VISUINO_ARDUINO_NANO_EVERY )))
//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (TCCR2B & CS22)
					clockSource |= (1 << CS22);

				if (TCCR2B & CS21)
					clockSource |= (1 << CS21);

				if (TCCR2B & CS20)
					clockSource |= (1 << CS20);

				// Remove the clock source to shutdown Timer2
				TCCR2B &= ~(1 << CS22);
				TCCR2B &= ~(1 << CS21);
				TCCR2B &= ~(1 << CS20);
			}
		#endif

//			if (adc == ADC_OFF)
#if ( !defined(VISUINO_ARDUINO_NANO_EVERY ))
			if( DisableADC().GetValue() )
				ADCSRA &= ~(1 << ADEN);

//			if (period != SLEEP_FOREVER)
			if( Interval().GetValue() )
			{
				wdt_enable( GetArduinoProcessorIntervalValue( Interval() ) );
				WDTCSR |= (1 << WDIE);
			}

//			if (bod == BOD_OFF)
			if( DisableBrownOutDetection().GetValue() )
			{
				#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)
					lowPowerBodOff(SLEEP_MODE_PWR_SAVE);
				#else
					lowPowerBodOn(SLEEP_MODE_PWR_SAVE);
				#endif
			}
			else
				lowPowerBodOn(SLEEP_MODE_PWR_SAVE);

//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
				ADCSRA |= (1 << ADEN);
#endif

		#if (( !defined(__AVR_ATmega32U4__) ) && ( !defined(VISUINO_ARDUINO_NANO_EVERY )))
//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (clockSource & CS22)
					TCCR2B |= (1 << CS22);

				if (clockSource & CS21)
					TCCR2B |= (1 << CS21);

				if (clockSource & CS20)
					TCCR2B |= (1 << CS20);
			}
		#endif
		}

	};
//---------------------------------------------------------------------------
#if ( !defined(VISUINO_ARDUINO_NANO_EVERY ))
	template <
		typename T_DisableADC,
		typename T_DisableBrownOutDetection,
		typename T_Interval
	> class ArduinoProcessorPowerDown :
		public T_DisableADC,
		public T_DisableBrownOutDetection,
		public T_Interval
	{
	public:
		_V_PROP_( DisableADC )
		_V_PROP_( DisableBrownOutDetection )
		_V_PROP_( Interval )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			if( adc == ADC_OFF )
			if( DisableADC().GetValue() )
				ADCSRA &= ~(1 << ADEN);

//			if( period != SLEEP_FOREVER )
			if( Interval().GetValue() )
			{
				wdt_enable( GetArduinoProcessorIntervalValue( Interval() ) );
				WDTCSR |= (1 << WDIE);
			}

//			if (bod == BOD_OFF)
			if( DisableBrownOutDetection().GetValue() )
			{
				#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)
					lowPowerBodOff(SLEEP_MODE_PWR_DOWN);
				#else
					lowPowerBodOn(SLEEP_MODE_PWR_DOWN);
				#endif
			}
			else
				lowPowerBodOn(SLEEP_MODE_PWR_DOWN);

//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
				ADCSRA |= (1 << ADEN);
		}

	};
#endif
//---------------------------------------------------------------------------
#if ( !defined(VISUINO_ARDUINO_NANO_EVERY ))
	template <
		typename T_DisableADC,
		typename T_DisableBrownOutDetection,
		typename T_Interval
	> class ArduinoProcessorStandby :
		public T_DisableADC,
		public T_DisableBrownOutDetection,
		public T_Interval
	{
	public:
		_V_PROP_( DisableADC )
		_V_PROP_( DisableBrownOutDetection )
		_V_PROP_( Interval )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
				ADCSRA &= ~(1 << ADEN);

			if( Interval().GetValue() )
			{
				wdt_enable( GetArduinoProcessorIntervalValue( Interval() ) );
				WDTCSR |= (1 << WDIE);
			}

//			if (bod == BOD_OFF)
			if( DisableBrownOutDetection().GetValue() )
			{
				#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)
					lowPowerBodOff(SLEEP_MODE_STANDBY);
				#else
					lowPowerBodOn(SLEEP_MODE_STANDBY);
				#endif
			}
			else
				lowPowerBodOn(SLEEP_MODE_STANDBY);

//			if( adc == ADC_OFF )
			if( DisableADC().GetValue() )
				ADCSRA |= (1 << ADEN);		
		}

	};
#endif
//---------------------------------------------------------------------------
	template <
		typename T_DisableADC,
		typename T_DisableBrownOutDetection,
		typename T_DisableTimer2,
		typename T_Interval
	> class ArduinoProcessorExtendedStandby :
		public T_DisableADC,
		public T_DisableBrownOutDetection,
		public T_DisableTimer2,
		public T_Interval
	{
	public:
		_V_PROP_( DisableADC )
		_V_PROP_( DisableBrownOutDetection )
		_V_PROP_( DisableTimer2 )
		_V_PROP_( Interval )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			// Temporary clock source variable
			unsigned char clockSource = 0;

		#if (( !defined(__AVR_ATmega32U4__) ) && ( !defined(VISUINO_ARDUINO_NANO_EVERY )))
//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (TCCR2B & CS22)
					clockSource |= (1 << CS22);

				if (TCCR2B & CS21)
					clockSource |= (1 << CS21);

				if (TCCR2B & CS20)
					clockSource |= (1 << CS20);

				// Remove the clock source to shutdown Timer2
				TCCR2B &= ~(1 << CS22);
				TCCR2B &= ~(1 << CS21);
				TCCR2B &= ~(1 << CS20);
			}
		#endif

#if ( !defined(VISUINO_ARDUINO_NANO_EVERY ))
//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
				ADCSRA &= ~(1 << ADEN);

//			if (period != SLEEP_FOREVER)
			if( Interval().GetValue() )
			{
				wdt_enable( GetArduinoProcessorIntervalValue( Interval() ) );
				WDTCSR |= (1 << WDIE);
			}
#endif

		#if ( defined (__AVR_ATmega88__) || defined (__AVR_ATmega168__) || defined(VISUINO_ARDUINO_NANO_EVERY )) // SLEEP_MODE_EXT_STANDBY not implemented on Atmega88 / Atmega168
		#else
//				if (bod == BOD_OFF)
				if( DisableBrownOutDetection().GetValue() )
				{
					#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)
						lowPowerBodOff(SLEEP_MODE_EXT_STANDBY);
					#else
						lowPowerBodOn(SLEEP_MODE_EXT_STANDBY);
					#endif
				}
				else
					lowPowerBodOn(SLEEP_MODE_EXT_STANDBY);

		#endif

#if ( !defined(VISUINO_ARDUINO_NANO_EVERY ))
//			if (adc == ADC_OFF)
			if( DisableADC().GetValue() )
				ADCSRA |= (1 << ADEN);
#endif

		#if (( !defined(__AVR_ATmega32U4__) ) && ( !defined(VISUINO_ARDUINO_NANO_EVERY )))
//			if (timer2 == TIMER2_OFF)
			if( DisableTimer2().GetValue() )
			{
				if (clockSource & CS22)
					TCCR2B |= (1 << CS22);

				if (clockSource & CS21)
					TCCR2B |= (1 << CS21);

				if (clockSource & CS20)
					TCCR2B |= (1 << CS20);
			}
		#endif
	}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
