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

// Uses http://www.thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_A,
		typename T_B,
		typename T_C,
		typename T_Enabled,
		typename T_FixedResistor,
		typename T_InFahrenheit,
		typename T_OutputPin,
		typename T_ThermistorToGround
	> class ThermistorSteinhartHart :
		public T_A,
		public T_B,
		public T_C,
		public T_Enabled,
		public T_FixedResistor,
		public T_InFahrenheit,
		public T_OutputPin,
		public T_ThermistorToGround
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( FixedResistor )
		_V_PROP_( A )
		_V_PROP_( B )
		_V_PROP_( C )

		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )
		_V_PROP_( ThermistorToGround )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			float AValue = MitovConstrain( *((float *)_Data), 0.0f, 1.0f );
			if( ThermistorToGround() )
				AValue = 1.0 - AValue;

			if( AValue == 0 )
				AValue = 0.0001;

			double resistance = (( FixedResistor() / AValue ) - FixedResistor() );

			double log_r  = log( resistance );
			double log_r3 = log_r * log_r * log_r;

			float ASteinhartHart = 1.0 / ( A() + B() * log_r + C() * log_r3 );
			ASteinhartHart -= 273.15;
			if( InFahrenheit() )
				ASteinhartHart = ASteinhartHart * 9/5 + 32;

			T_OutputPin::SetPinValue( ASteinhartHart );

//			float AResult = ASteinhartHart;

//			return steinhartHart(resistance) - voltage * voltage / (K * _resistance);
		}
	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_INDEX,
		typename T_Resistance,
		typename T_Temperature
	> class TArduinoThermistorPoint :
		public T_Resistance,
		public T_Temperature
	{
	public:
		_V_PROP_( Resistance )
		_V_PROP_( Temperature )

	public:
		inline void GetTableValues( float * AResistance, float * ATemperature )
		{
			AResistance[ C_INDEX ] = Resistance().GetValue();
			ATemperature[ C_INDEX ] = Temperature().GetValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_InFahrenheit,
		typename T_Points_GetTableValues
	> class TArduinoThermistorPointsSettings :
		public T_InFahrenheit
	{
	public:
		_V_PROP_( InFahrenheit )

	public:
		inline void GetTableValues( float * AResistance, float * ATemperature )
		{
			T_Points_GetTableValues::Call( AResistance, ATemperature );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FixedResistor,
		typename T_InFahrenheit,
		typename T_OutputPin,
		typename T_Table,
		typename T_ThermistorToGround
	> class ThermistorSteinhartHart3Points :
		public T_Enabled,
		public T_FixedResistor,
		public T_InFahrenheit,
		public T_OutputPin,
		public T_Table,
		public T_ThermistorToGround
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( FixedResistor )
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )
		_V_PROP_( ThermistorToGround )

		_V_PROP_( Table )

	protected:
		inline float GetKTemt( float AValue )
		{
			if( Table().InFahrenheit() )
				AValue = ( AValue - 32 ) / ( 9/5 ) ;

			return AValue + 273.15;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			float AValue = MitovConstrain( *((float *)_Data), 0.0f, 1.0f );
			if( ThermistorToGround() )
				AValue = 1.0 - AValue;

			if( AValue == 0 )
				AValue = 0.0001;

			double resistance = (( FixedResistor() / AValue ) - FixedResistor() );

			double log_r  = log( resistance );
			double log_r3 = log_r * log_r * log_r;
            
            float AResistance[3];
            float ATemperature[3];
            
            T_Table::GetTableValues( AResistance, ATemperature );

			float L1 = log( AResistance[ 0 ] );
			float L2 = log( AResistance[ 1 ] );
			float L3 = log( AResistance[ 2 ] );

			float Y1 = 1 / GetKTemt( ATemperature[ 0 ] );
			float Y2 = 1 / GetKTemt( ATemperature[ 1 ] );
			float Y3 = 1 / GetKTemt( ATemperature[ 2 ] );

			float YY2 = ( Y2 - Y1 ) / ( L2 - L1 );
			float YY3 = ( Y3 - Y1 ) / ( L3 - L1 );
			float C = (( YY3 - YY2 ) / ( L3 - L2 )) * pow( L1 + L2 + L3, -1 );
			float B = YY2 - C * ( L1 * L1 + L1 * L2 + L2 * L2 );
			float A = Y1 - ( B + L1 * L1 * C ) * L1;

			float ASteinhartHart = 1.0 / ( A + B * log_r + C * log_r3 );

			ASteinhartHart -= 273.15;
			if( InFahrenheit() )
				ASteinhartHart = ASteinhartHart * 9/5 + 32;

			T_OutputPin::SetPinValue( ASteinhartHart );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif