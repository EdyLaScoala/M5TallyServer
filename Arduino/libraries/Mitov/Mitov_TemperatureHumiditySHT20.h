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
	namespace TArduinoTemperatureHumiditySHT20_Resolution
	{
		enum TArduinoTemperatureHumiditySHT20_Resolution
		{
			Temperature_14_Humidity_12,
			Temperature_12_Humidity_8,
			Temperature_13_Humidity_10,
			Temperature_11_Humidity_11
		};
	}
//---------------------------------------------------------------------------
	namespace SHT20
	{
		const uint32_t	SHIFTED_DIVISOR				= 0x988000;
		const uint8_t	TRIGGER_TEMP_MEASURE_NOHOLD = 0xF3;
		const uint8_t	TRIGGER_HUMD_MEASURE_NOHOLD	= 0xF5;
		const uint8_t	WRITE_USER_REG				= 0xE6;
		const uint8_t	READ_USER_REG				= 0xE7;
		const uint8_t	TRIGGER_SOFT_RESET			= 0xFE;
	}
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_FReadingHumidity,
		typename T_FReadingTemperature,
		typename T_HumidityOutputPin,
		typename T_InFahrenheit,
		typename T_Resolution,
		typename T_TemperatureOutputPin
	> class TemperatureHumiditySHT20_I2C : 
		public T_Address,
		public T_Enabled,
		public T_FReadingHumidity,
		public T_FReadingTemperature,
		public T_HumidityOutputPin,
		public T_InFahrenheit,
		public T_Resolution,
		public T_TemperatureOutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Address )
		_V_PROP_( InFahrenheit )
		_V_PROP_( Resolution )

	public:
		_V_PIN_( HumidityOutputPin )
		_V_PIN_( TemperatureOutputPin )

	protected:
		_V_PIN_( FReadingHumidity )
		_V_PIN_( FReadingTemperature )

	public:
		void UpdateConfig()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( SHT20::READ_USER_REG );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ));
			uint8_t AValue = C_I2C.read();

			AValue &= 0b01111110;

			if( Resolution().GetValue() & 0b10 )
				AValue |= 0b10000000;

			AValue |= ( Resolution().GetValue() & 0b1 );
			
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( SHT20::WRITE_USER_REG );
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

	protected:
		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

			if( FReadingTemperature().GetValue() )
				return;

			if( FReadingHumidity().GetValue() )
				return;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
			{
				BeginReadThermometer();
				return;
			}

			if( T_HumidityOutputPin::GetPinIsConnected() )
				BeginReadHumidity();

		}

		bool check_crc( uint16_t data_in, uint8_t crc ) 
		{
			uint32_t remainder = data_in << 8;
			uint32_t divsor = SHT20::SHIFTED_DIVISOR;

			remainder |= crc;
			for(int i = 0 ; i < 16 ; i++)
			{
				if(remainder & (uint32_t)1 << (23 - i))
					remainder ^= divsor;

				divsor >>= 1;
			}
    
			return ( remainder == 0 );
		}

		void BeginReadThermometer()
		{
//			Serial.println( "BeginReadThermometer" );
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( SHT20::TRIGGER_TEMP_MEASURE_NOHOLD );
			if( ! C_I2C.endTransmission())
				FReadingTemperature() = true;

		}

		void BeginReadHumidity()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( SHT20::TRIGGER_HUMD_MEASURE_NOHOLD );
			if( ! C_I2C.endTransmission())
				FReadingHumidity() = true;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( SHT20::TRIGGER_SOFT_RESET );
			C_I2C.endTransmission();

			delay( 15 );
		}

	public:
		inline void SystemStartInit()
		{
			delay( 15 );
			UpdateConfig();
		}

		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

		inline void SystemLoopBeginCheck()
		{
			if( FReadingTemperature() )
			{
				if( C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 3 )) != 3 )
					return;

				uint16_t rawValue = uint16_t( C_I2C.read() ) << 8;
				rawValue |= C_I2C.read();

				uint8_t ACheckSum = C_I2C.read();

				if( check_crc( rawValue, ACheckSum ) != true)
				{
					BeginReadThermometer();
					return;
				}
				
				float ATemperature = - 46.85 + float( rawValue & 0xFFFC ) * (175.72 / 65536.0);
				if( InFahrenheit() )
					ATemperature = Func::ConvertCtoF( ATemperature );

				T_TemperatureOutputPin::SetPinValue( ATemperature );
				FReadingTemperature() = false;

				if( T_HumidityOutputPin::GetPinIsConnected() )
					if( Enabled().GetValue() )
						BeginReadHumidity();

				return;
			}

			if( FReadingHumidity() )
			{
				if( C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 3 )) != 3 )
					return;

				uint16_t rawValue = uint16_t( C_I2C.read() ) << 8;
				rawValue |= C_I2C.read();

				uint8_t ACheckSum = C_I2C.read();

				if( check_crc( rawValue, ACheckSum ) != true)
				{
					BeginReadHumidity();
					return;
				}
				
				float AHumidity = - 6.0 + rawValue * (125.0 / 65536.0);
				T_HumidityOutputPin::SetPinValue( AHumidity );
				FReadingHumidity() = false;
			}
		}

	public:
		inline TemperatureHumiditySHT20_I2C()
		{
			FReadingHumidity() = false;
			FReadingTemperature() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

