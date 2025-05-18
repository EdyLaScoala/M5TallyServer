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
	template <
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class ArduinoBpiBitThermometer :
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	protected:
		static const int SERIESRESISTOR = 4700;

		// resistance at 25 degrees C
		static const int THERMISTORNOMINAL = 10000;
		// temp. for nominal resistance (almost always 25 C)
		static const int TEMPERATURENOMINAL = 25;

		// The beta coefficient of the thermistor (usually 3000-4000)
		static const int BCOEFFICIENT = 3950;
		// the value of the 'other' resistor

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	public:
		void ReadData( bool ASendAll = false ) // Used By Setters
		{
			if( ! Enabled() )
				 return;

			float reading = analogRead( 34 );

//			Serial.print("Thermistor reading: "); Serial.println(reading);

			// convert the value to resistance
			reading = (( float( VISUINO_ANALOG_IN_RANGE ) * float( SERIESRESISTOR )) / reading);
			reading -= SERIESRESISTOR;

			//Serial.print("Thermistor resistance: "); Serial.println(reading);

			float steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
			steinhart = log(steinhart);                  // ln(R/Ro)
			steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
			steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
			steinhart = 1.0 / steinhart;                 // Invert
			steinhart -= 273.15;                         // convert to C

			if( InFahrenheit() )
				steinhart = steinhart * ( 9.0/5.0 ) + 32.0;

			T_OutputPin::SetPinValue( steinhart, ASendAll );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( true );
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadData( false );
		}

		inline void SystemStart()
		{
			ReadData( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		typename T_Enabled,
		typename T_InLux,
		typename T_OutputPin
	> class ArduinoBpiBitLightSensor :
		public T_Enabled,
		public T_InLux,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InLux )

	public:
		void ReadData( bool ASendAll = false )  // Used by Binding Setter!
		{
			if( ! Enabled() )
				 return;

			float AValue = analogRead( C_PIN ); // / float( VISUINO_ANALOG_IN_RANGE );
			if( InLux() )
				AValue *= 1500.0;

			T_OutputPin::SetPinValue( AValue, ASendAll );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( true );
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadData( false );
		}

		inline void SystemStart()
		{
			ReadData( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif