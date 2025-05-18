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
	> class HeatIndex :
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
			// Using both Rothfusz and Steadman's equations
			// http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml

			float temperature = InitialTemperature();
			if ( ! InFahrenheit() )
				temperature = Func::ConvertCtoF(temperature);

			float hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (InitialHumidity() * 0.094));

			if( hi > 79 )
			{
				hi = -42.379 +
							 2.04901523 * temperature +
							10.14333127 * InitialHumidity() +
							-0.22475541 * temperature * InitialHumidity() +
							-0.00683783 * pow(temperature, 2) +
							-0.05481717 * pow(InitialHumidity(), 2) +
							 0.00122874 * pow(temperature, 2) * InitialHumidity() +
							 0.00085282 * temperature * pow(InitialHumidity(), 2) +
							-0.00000199 * pow(temperature, 2) * pow(InitialHumidity(), 2);

				if((InitialHumidity() < 13) && (temperature >= 80.0) && (temperature <= 112.0))
					hi -= ((13.0 - InitialHumidity() ) * 0.25) * sqrt((17.0 - fabs(temperature - 95.0)) * 0.05882);

				else if((InitialHumidity() > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
					hi += ((InitialHumidity() - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
			}

			if( ! InFahrenheit() )
				hi = Func::ConvertFtoC( hi );

			T_OutputPin::SetPinValue( hi );
		}

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