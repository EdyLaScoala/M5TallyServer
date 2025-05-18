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

#include <Adafruit_Circuit_Playground.h>

//#ifdef __AVR__ // Circuit Playground 'classic'
//  #include <utility/CPlay_CapacitiveSensor.h>
//#else
//  #include <utility/Adafruit_CPlay_FreeTouch.h>
//#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Samples
	> class TArduinoCircuitPlaygroundTouch :
		public T_Enabled,
		public T_Samples
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Samples )

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN_NUMBER,
		typename T_OutputPin,
		typename T_Touch
	> class ArduinoCircuitPlaygroundTouchAnalogInputChannel :
		public T_OutputPin,
		public T_Touch
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Touch )

	protected:
#ifdef __AVR__ // Circuit Playground 'classic'
		CPlay_CapacitiveSensor		FTouch;
#else
		Adafruit_CPlay_FreeTouch	FTouch;
#endif

	protected:
		void SedOutput( bool AChangeOnly )
		{
			float ALastOutput;
			if( Touch().Enabled() )
			{
#ifdef __AVR__ // Circuit Playground 'classic'
				ALastOutput = FTouch.capacitiveSensor( Touch().Samples() );
#else // Circuit Playground Express // Circuit Playground Express
				ALastOutput = FTouch.measure();
#endif
			}

			else
				ALastOutput = analogRead( C_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE );

	    	T_OutputPin::SetPinValue( ALastOutput, AChangeOnly );
		}

	public:
		inline void SystemInit()
		{
		}

		inline void SystemStart()
		{
			SedOutput( false );
		}

		inline void SystemLoopBegin()
		{
			SedOutput( true );
		}

	public:
		inline ArduinoCircuitPlaygroundTouchAnalogInputChannel() :
#ifdef __AVR__
			FTouch( CPLAY_CAPSENSE_SHARED, C_PIN_NUMBER )
#else
			FTouch( C_PIN_NUMBER, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE )
#endif
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN_NUMBER,
		typename T_IsTouch,
		typename T_OutputPin
	> class ArduinoCircuitPlaygroundExpressTouchAnalogInputChannel :
		public T_IsTouch,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsTouch )

	protected:
#ifndef __AVR__ // Circuit Playground Express
		Adafruit_CPlay_FreeTouch	FTouch;
#endif

	protected:
		void SedOutput( bool AChangeOnly )
		{
			float ALastOutput;
			if( IsTouch() )
			{
#ifndef __AVR__ // Circuit Playground Express
				ALastOutput = FTouch.measure();
#endif
			}

			else
				ALastOutput = analogRead( C_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE );

	    	T_OutputPin::SetPinValue( ALastOutput, AChangeOnly );
		}

	public:
		inline void SystemInit()
		{
		}

		inline void SystemStart()
		{
			SedOutput( false );
		}

		inline void SystemLoopBegin()
		{
			SedOutput( true );
		}

	public:
#ifndef __AVR__
		inline ArduinoCircuitPlaygroundExpressTouchAnalogInputChannel() :
			FTouch( C_PIN_NUMBER, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE )
		{
		}
#endif

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


