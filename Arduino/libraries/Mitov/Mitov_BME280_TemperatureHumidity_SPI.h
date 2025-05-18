////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_BME280_TemperatureHumidity.h>
#include <SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif


namespace Mitov
{
//---------------------------------------------------------------------------
	template <typename T_SPI, T_SPI &C_SPI, typename T_ChipSelectOutputPin> class BME280_TemperatureHumiditySPI_Impl : public T_ChipSelectOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin)

	protected:
		uint8_t read8(uint8_t reg)
		{
			C_SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( reg | 0x80 ); // read, bit 7 high
			uint8_t AValue = C_SPI.transfer( 0 );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 

			return AValue;
		}

		uint16_t read16(uint8_t reg)
		{
			C_SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( reg | 0x80 ); // read, bit 7 high
			uint16_t AValue = C_SPI.transfer( 0 );
			AValue = ( AValue << 8 ) | C_SPI.transfer( 0 );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 

			return AValue;
		}

		uint16_t read16_LE(uint8_t reg)
		{
			C_SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( reg | 0x80 ); // read, bit 7 high
			uint16_t AValue = C_SPI.transfer( 0 );
			AValue |= uint16_t( C_SPI.transfer( 0 ) ) << 8;

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 

			return AValue;
		}

		uint32_t read24(uint8_t reg)
		{
			C_SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( reg | 0x80 ); // read, bit 7 high
			uint32_t AValue = C_SPI.transfer( 0 );
			AValue <<= 8;
			AValue |= C_SPI.transfer( 0 );
			AValue <<= 8;
			AValue |= C_SPI.transfer( 0 );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 

			return AValue;
		}

		void write8(uint8_t reg, uint8_t value)
		{
			C_SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( reg | 0x80 ); // read, bit 7 high
			C_SPI.transfer( value );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI, 
		typename T_ChipSelectOutputPin,
		typename T_Enabled,
//		typename T_HumidityOutputPin,
		typename T_IIRFilterCoefficient,
		typename T_InFahrenheit,
		typename T_InactiveDuration,
//		typename T_Oversampling_Humidity,
		typename T_Oversampling_Pressure,
		typename T_Oversampling_Temperature,
		typename T_PressureOutputPin,
		typename T_SPI3Wire,
		typename T_TemperatureOutputPin
	> class BMP280_TemperaturePressure_SPI : 
		public BME280_TemperatureHumidity<
			BME280_TemperatureHumiditySPI_Impl<T_SPI, C_SPI, T_ChipSelectOutputPin>, 
				T_Enabled,
				Mitov::AnalogPin_NoImplementation<88>,
				T_IIRFilterCoefficient,
				T_InFahrenheit,
				T_InactiveDuration,
				Mitov::ConstantProperty<66, uint32_t, 1>,
				T_Oversampling_Pressure,
				T_Oversampling_Temperature,
				T_PressureOutputPin,
				T_SPI3Wire,
				T_TemperatureOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_Enabled,
		typename T_HumidityOutputPin,
		typename T_IIRFilterCoefficient,
		typename T_InFahrenheit,
		typename T_InactiveDuration,
		typename T_Oversampling_Humidity,
		typename T_Oversampling_Pressure,
		typename T_Oversampling_Temperature,
		typename T_PressureOutputPin,
		typename T_SPI3Wire,
		typename T_TemperatureOutputPin
	> class BME280_TemperatureHumidity_SPI : 
		public BME280_TemperatureHumidity<
			BME280_TemperatureHumiditySPI_Impl<T_SPI, C_SPI, T_ChipSelectOutputPin>, 
				T_Enabled,
				T_HumidityOutputPin,
				T_IIRFilterCoefficient,
				T_InFahrenheit,
				T_InactiveDuration,
				T_Oversampling_Humidity,
				T_Oversampling_Pressure,
				T_Oversampling_Temperature,
				T_PressureOutputPin,
				T_SPI3Wire,
				T_TemperatureOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
