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
	> class AbsoluteHumidity :
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
			float temperature = InitialTemperature();
			if ( InFahrenheit() )
				temperature = Func::ConvertFtoC( temperature );

			float AbsoluteHumidity = (2.167 * 6.112) * InitialHumidity();

			AbsoluteHumidity *= exp( (17.62 * temperature) / (243.12 + temperature) );
			AbsoluteHumidity /=  (273.15 + temperature);

			T_OutputPin::SetPinValue( AbsoluteHumidity );
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