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
		typename T_InitialHumidity,
		typename T_InitialTemperature,
		typename T_OutputPin
	> class DewPoint :
		public T_Enabled,
		public T_InFahrenheit,
		public T_InitialHumidity,
		public T_InitialTemperature,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )
		_V_PROP_( InitialTemperature )
		_V_PROP_( InitialHumidity )

	public:
		inline void TemperatureInputPin_o_Receive( void *_Data )
		{
			float AValue = *(float*)_Data;
			if( InitialTemperature() == AValue )
				return;

			InitialTemperature() = AValue;
			CalculateOutput();
		}

		inline void HumidityInputPin_o_Receive( void *_Data )
		{
			float AValue = *(float*)_Data;
			if( InitialHumidity() == AValue )
				return;

			InitialHumidity() = AValue;
			CalculateOutput();
		}

	protected:
		void CalculateOutput()
		{
			// dewPoint function NOAA
			// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
			// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
			//

			float celsius = InitialTemperature();
			if ( InFahrenheit() )
				celsius = Func::ConvertFtoC( celsius );

//			Serial.print( "T: " ); Serial.println( celsius );

			// (1) Saturation Vapor Pressure = ESGG(T)
			double RATIO = 373.15 / (273.15 + celsius);
			double RHS = -7.90298 * (RATIO - 1);
			RHS += 5.02808 * log10(RATIO);
			RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
			RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
			RHS += log10(1013.246);

				// factor -3 is to adjust units - Vapor Pressure SVP * humidity
			double VP = pow(10, RHS - 3) * InitialHumidity();

				// (2) DEWPOINT = F(Vapor Pressure)
			double T = log(VP/0.61078);   // temp var

			float ADewPoint = (241.88 * T) / (17.558 - T);

			if( InFahrenheit() )
				ADewPoint = Func::ConvertCtoF( ADewPoint );

			T_OutputPin::SetPinValue( ADewPoint );
		}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
/*
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
	double Td = (b * temp) / (a - temp);
	return Td;
}
*/

	public:
		inline void SystemStart() 
		{
			CalculateOutput();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif