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
#include <Mitov_BME280_TemperatureHumidity.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
//		typename T_Address,
		typename T_Enabled,
		typename T_HumidityOutputPin,
		typename T_IIRFilterCoefficient,
		typename T_InFahrenheit,
		typename T_InactiveDuration,
		typename T_Oversampling,
		typename T_PressureOutputPin,
		typename T_TemperatureOutputPin
	> class M5StackEnvironmentalUnitI2C : 
//		public T_Address,
		public T_Enabled,
		public T_HumidityOutputPin,
		public T_IIRFilterCoefficient,
		public T_InFahrenheit,
		public T_InactiveDuration,
		public T_Oversampling,
		public T_PressureOutputPin,
		public T_TemperatureOutputPin
	{
	protected:
		static const uint8_t C_AddressDHT12		= 0x5C;
		static const uint8_t C_AddressBMP280	= 0x76;

	protected:
		struct
		{
		  uint16_t dig_T1;
		  int16_t  dig_T2;
		  int16_t  dig_T3;

		  uint16_t dig_P1;
		  int16_t  dig_P2;
		  int16_t  dig_P3;
		  int16_t  dig_P4;
		  int16_t  dig_P5;
		  int16_t  dig_P6;
		  int16_t  dig_P7;
		  int16_t  dig_P8;
		  int16_t  dig_P9;

		  uint8_t  dig_H1;
		  int16_t  dig_H2;
		  uint8_t  dig_H3;
		  int16_t  dig_H4;
		  int16_t  dig_H5;
		  int8_t   dig_H6;
		} CalibrationData;

	public:
//		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )
		_V_PROP_( InactiveDuration )
		_V_PROP_( Oversampling )
		_V_PROP_( IIRFilterCoefficient )

	public:
		_V_PIN_( HumidityOutputPin )
		_V_PIN_( PressureOutputPin )
		_V_PIN_( TemperatureOutputPin )

	protected:
		inline int16_t BMP280ReadS16_LE( uint8_t reg ) 
		{
			return int16_t( BMP280Read16_LE( reg ));
		}

		uint8_t BMP280Read8(uint8_t reg)
		{
			C_I2C.beginTransmission( C_AddressBMP280 );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_AddressBMP280, (uint8_t)1 );
			return C_I2C.read();
		}

		uint16_t BMP280Read16(uint8_t reg)
		{
			C_I2C.beginTransmission( C_AddressBMP280 );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_AddressBMP280, (uint8_t)2 );
			uint16_t AValue = C_I2C.read();
			return ( AValue << 8 ) | C_I2C.read();
		}

		uint16_t BMP280Read16_LE(uint8_t reg)
		{
			C_I2C.beginTransmission( C_AddressBMP280 );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_AddressBMP280, (uint8_t)2 );
			uint16_t AValue = C_I2C.read();
			return AValue | ( uint16_t( C_I2C.read() ) << 8 );
		}

		uint32_t BMP280Read24(uint8_t reg)
		{
			C_I2C.beginTransmission( C_AddressBMP280 );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_AddressBMP280, (uint8_t)3 );
    
			uint32_t AValue = C_I2C.read();
			AValue <<= 8;
			AValue |= C_I2C.read();
			AValue <<= 8;
			AValue |= C_I2C.read();

			return AValue;
		}

		void BMP280Write8(uint8_t reg, uint8_t value)
		{
			C_I2C.beginTransmission( C_AddressBMP280 );
			C_I2C.write((uint8_t)reg);
			C_I2C.write((uint8_t)value);
			C_I2C.endTransmission();
		}

		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

			uint8_t ADatos[ 5 ];

			C_I2C.beginTransmission( C_AddressDHT12 );
			C_I2C.write( 0 );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_AddressDHT12, uint8_t( 5 ));

			for( int i=0; i < 5; ++ i )
				ADatos[ i ] = C_I2C.read();

//			Serial.println( "TEST: " );
//			for( int i=0; i < 5; ++ i )
//			{
//				Serial.print( ADatos[ i ], HEX ); Serial.print( " " );
//			}

//			Serial.println( "" );

			if ( ADatos[4] != ( ADatos[0] + ADatos[1] + ADatos[2] + ADatos[3] ))
				return; // Error

//			Serial.println( "TEST1" );
			float ATemperature = ( ADatos[2] + float( ADatos[ 3 ] ) / 10 );
			T_TemperatureOutputPin::SetPinValue( ATemperature );

			float AHumidity = ( ADatos[ 0 ] + float( ADatos[ 1 ] ) / 10 );
			T_HumidityOutputPin::SetPinValue( AHumidity );

			// Read BMP280

			int32_t adc_T = BMP280Read24( BME280Const::BME280_REGISTER_TEMPDATA );
//			Serial.println( adc_T );
			adc_T >>= 4;

			int32_t var1  = ((((adc_T>>3) - ((int32_t)CalibrationData.dig_T1 <<1))) *
				((int32_t)CalibrationData.dig_T2)) >> 11;

			int32_t var2  = (((((adc_T>>4) - ((int32_t)CalibrationData.dig_T1)) *
					((adc_T>>4) - ((int32_t)CalibrationData.dig_T1))) >> 12) *
						((int32_t)CalibrationData.dig_T3)) >> 14;

			int32_t t_fine = var1 + var2;

			if( T_PressureOutputPin::GetPinIsConnected() )
			{
				int32_t adc_P = BMP280Read24( BME280Const::BME280_REGISTER_PRESSUREDATA );
				adc_P >>= 4;

				int64_t var1 = ((int64_t)t_fine) - 128000;
				int64_t var2 = var1 * var1 * (int64_t)CalibrationData.dig_P6;
				var2 = var2 + ((var1*(int64_t)CalibrationData.dig_P5)<<17);
				var2 = var2 + (((int64_t)CalibrationData.dig_P4)<<35);
				var1 = ((var1 * var1 * (int64_t)CalibrationData.dig_P3)>>8) +
				((var1 * (int64_t)CalibrationData.dig_P2)<<12);
				var1 = (((((int64_t)1)<<47)+var1))*((int64_t)CalibrationData.dig_P1)>>33;

				float AResult;
				if( var1 == 0 )
					AResult = 0;  // avoid exception caused by division by zero

				else
				{
					int64_t p = 1048576 - adc_P;
					p = (((p<<31) - var2)*3125) / var1;
					int64_t var1 = (((int64_t)CalibrationData.dig_P9) * (p>>13) * (p>>13)) >> 25;
					int64_t var2 = (((int64_t)CalibrationData.dig_P8) * p) >> 19;

					p = ((p + var1 + var2) >> 8) + (((int64_t)CalibrationData.dig_P7)<<4);
					AResult = float( p ) / 256;
				}

				T_PressureOutputPin::SetPinValue( AResult );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline void SystemInit()
		{
			CalibrationData.dig_T1 = BMP280Read16_LE( BME280Const::BME280_REGISTER_DIG_T1 );
			CalibrationData.dig_T2 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_T2 );
			CalibrationData.dig_T3 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_T3 );

			CalibrationData.dig_P1 = BMP280Read16_LE( BME280Const::BME280_REGISTER_DIG_P1 );
			CalibrationData.dig_P2 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P2 );
			CalibrationData.dig_P3 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P3 );
			CalibrationData.dig_P4 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P4 );
			CalibrationData.dig_P5 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P5 );
			CalibrationData.dig_P6 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P6 );
			CalibrationData.dig_P7 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P7 );
			CalibrationData.dig_P8 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P8 );
			CalibrationData.dig_P9 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_P9 );

			CalibrationData.dig_H1 = BMP280Read8( BME280Const::BME280_REGISTER_DIG_H1 );
			CalibrationData.dig_H2 = BMP280ReadS16_LE( BME280Const::BME280_REGISTER_DIG_H2 );
			CalibrationData.dig_H3 = BMP280Read8( BME280Const::BME280_REGISTER_DIG_H3 );
			CalibrationData.dig_H4 = ( BMP280Read8( BME280Const::BME280_REGISTER_DIG_H4 ) << 4 ) | ( BMP280Read8( BME280Const::BME280_REGISTER_DIG_H4 + 1 ) & 0xF );
			CalibrationData.dig_H5 = ( BMP280Read8( BME280Const::BME280_REGISTER_DIG_H5 + 1 ) << 4) | ( BMP280Read8( BME280Const::BME280_REGISTER_DIG_H5 ) >> 4 );
			CalibrationData.dig_H6 = (int8_t)BMP280Read8( BME280Const::BME280_REGISTER_DIG_H6 );

//			BMP280Write8( BME280Const::BME280_REGISTER_CONTROLHUMID, Oversampling_Humidity() );
//			BMP280Write8( BME280Const::BME280_REGISTER_CONTROLHUMID, Oversampling_Humidity() );

			uint8_t AInactiveDurationIndex;
			if( InactiveDuration() >= 1000 )
				AInactiveDurationIndex = 5;

			else if( InactiveDuration() >= 500 )
				AInactiveDurationIndex = 4;

			else if( InactiveDuration() >= 250 )
				AInactiveDurationIndex = 3;

			else if( InactiveDuration() >= 125 )
				AInactiveDurationIndex = 2;

			else if( InactiveDuration() >= 62.5 )
				AInactiveDurationIndex = 1;

			else if( InactiveDuration() >= 20 )
				AInactiveDurationIndex = 7;

			else if( InactiveDuration() >= 10 )
				AInactiveDurationIndex = 6;

			else
				AInactiveDurationIndex = 0;

			uint8_t AIIRFilterCoefficientValue;

			if( IIRFilterCoefficient() >= 16 )
				AIIRFilterCoefficientValue = 4;

			else if( IIRFilterCoefficient() >= 8 )
				AIIRFilterCoefficientValue = 3;

			else if( IIRFilterCoefficient() >= 4 )
				AIIRFilterCoefficientValue = 2;

			else if( IIRFilterCoefficient() >= 2 )
				AIIRFilterCoefficientValue = 1;

			else
				AIIRFilterCoefficientValue = 0;

			uint8_t AValue = ( ( AInactiveDurationIndex & 0b111 ) << 5 ) | (( AIIRFilterCoefficientValue & 0b111 ) << 2 ) | ( 0 & 1 );
			BMP280Write8( BME280Const::BME280_REGISTER_CONFIG, AValue );

			AValue = ( ( Oversampling().GetTemperatureOrder() & 0b111 ) << 5 ) | (( Oversampling().GetPressureOrder() & 0b111 ) << 2 ) | 0b11;
//			Serial.println( AValue );
//			AValue = 0xB7; // 16x ovesampling, normal mode
			BMP280Write8( BME280Const::BME280_REGISTER_CONTROL, AValue );
		}

		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


