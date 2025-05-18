////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Wire.h> //I2C Arduino Library
#include <Mitov_BME280_TemperatureHumidity.h>
#include <Mitov_Basic_I2C.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C, 
		typename T_Address
	> class BME280_TemperatureHumidityI2C_Impl :
		public T_Address
	{
	public:
		_V_PROP_( Address )

	protected:
		uint8_t read8(uint8_t reg)
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ));
			return C_I2C.read();
		}

		uint16_t read16(uint8_t reg)
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ));
			uint16_t AValue = C_I2C.read();
			return ( AValue << 8 ) | C_I2C.read();
		}

		uint16_t read16_LE(uint8_t reg)
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ));
			uint16_t AValue = C_I2C.read();
			return AValue | ( uint16_t( C_I2C.read() ) << 8 );
		}

		uint32_t read24(uint8_t reg)
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 3 ));
    
			uint32_t AValue = C_I2C.read();
			AValue <<= 8;
			AValue |= C_I2C.read();
			AValue <<= 8;
			AValue |= C_I2C.read();

			return AValue;
		}

		void write8(uint8_t reg, uint8_t value)
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write((uint8_t)reg);
			C_I2C.write((uint8_t)value);
			C_I2C.endTransmission();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
//		typename T_HumidityOutputPin,
		typename T_IIRFilterCoefficient,
		typename T_InFahrenheit,
		typename T_InactiveDuration,
//		typename T_Oversampling_Humidity,
		typename T_Oversampling,
//		typename T_Oversampling_Temperature,
		typename T_PressureOutputPin,
		typename T_TemperatureOutputPin
	> class BMP280_TemperaturePressure_I2C : 
		public BME280_TemperatureHumidity<
				BME280_TemperatureHumidityI2C_Impl<T_I2C, C_I2C, T_Address>, 
				T_Enabled,
				Mitov::AnalogPin_NoImplementation<88>,
				T_IIRFilterCoefficient,
				T_InFahrenheit,
				T_InactiveDuration,
				T_Oversampling,
				T_PressureOutputPin,
				Mitov::ConstantProperty<3, bool, false>,
				T_TemperatureOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_HumidityOutputPin,
		typename T_IIRFilterCoefficient,
		typename T_InFahrenheit,
		typename T_InactiveDuration,
		typename T_Oversampling,
		typename T_PressureOutputPin,
		typename T_TemperatureOutputPin
	> class BME280_TemperatureHumidity_I2C : 
		public BME280_TemperatureHumidity<
			BME280_TemperatureHumidityI2C_Impl<T_I2C, C_I2C, T_Address>, 
				T_Enabled,
				T_HumidityOutputPin,
				T_IIRFilterCoefficient,
				T_InFahrenheit,
				T_InactiveDuration,
				T_Oversampling,
				T_PressureOutputPin,
				Mitov::ConstantProperty<3, bool, false>,
				T_TemperatureOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif