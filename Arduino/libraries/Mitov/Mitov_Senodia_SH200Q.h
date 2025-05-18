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

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace TArduinoSenodiaSH200QAccelerometerRange
	{
		enum TArduinoSenodiaSH200QAccelerometerRange 
		{
			Max4g,
			Max8g,
			Max16g
		};
	}
//---------------------------------------------------------------------------
	namespace TArduinoSenodiaSH200QAccelerometerRate
	{
		enum TArduinoSenodiaSH200QAccelerometerRate
		{
			Normal_1024_LowPower_64_Hz,
			Normal_512_LowPower_32_Hz,
			Normal_256_LowPower_16_Hz,
			Normal_128_LowPower_8_Hz
		};
	}
//---------------------------------------------------------------------------
  namespace TArduinoSenodiaSH200QGyroscopeRange
  {
		enum TArduinoSenodiaSH200QGyroscopeRange
		{
			Max2000dps,
			Max1000dps,
			Max500dps,
			Max250dps,
			Max125dps
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_FullScaleRange,
		typename T_LowPowerMode,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Rate,
		typename T_UseInternalClock
	> class SenodiaSH200QAccelerometer :
		public T_FullScaleRange,
		public T_LowPowerMode,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Rate,
		public T_UseInternalClock
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( FullScaleRange )
		_V_PROP_( LowPowerMode )
		_V_PROP_( Rate )
		_V_PROP_( UseInternalClock )

	public:
//		inline bool GetIsEnabledX() { return ( X().Enabled() && OutputPinsX().GetPinIsConnected() ); }
//		inline bool GetIsEnabledY() { return ( Y().Enabled() && OutputPinsY().GetPinIsConnected() ); }
//		inline bool GetIsEnabledZ() { return ( Z().Enabled() && OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabledX() { return OutputPinsX().GetPinIsConnected(); }
		inline bool GetIsEnabledY() { return OutputPinsY().GetPinIsConnected(); }
		inline bool GetIsEnabledZ() { return OutputPinsZ().GetPinIsConnected(); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }
	};
//---------------------------------------------------------------------------
	template <
		typename T_AutoPowerDown,
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_SampleFrequency
	> class SenodiaSH200QGyroscope :
		public T_AutoPowerDown,
		public T_FullScaleRange,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_SampleFrequency
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( AutoPowerDown )
		_V_PROP_( FullScaleRange )
		_V_PROP_( SampleFrequency )

	public:
		inline bool GetIsEnabledX() { return OutputPinsX().GetPinIsConnected(); }
		inline bool GetIsEnabledY() { return OutputPinsY().GetPinIsConnected(); }
		inline bool GetIsEnabledZ() { return OutputPinsZ().GetPinIsConnected(); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Accelerometer,
		typename T_Address,
		typename T_Enabled,
		typename T_Gyroscope,
		typename T_Thermometer
	> class SenodiaSH200Q :
		public T_Accelerometer,
		public T_Address,
		public T_Enabled,
		public T_Gyroscope,
		public T_Thermometer
	{
	protected:
		static const uint8_t SH200I_WHOAMI		= 0x30;
		static const uint8_t SH200I_ACC_CONFIG 	= 0x0E;
		static const uint8_t SH200I_GYRO_CONFIG = 0x0F;
		static const uint8_t SH200I_GYRO_DLPF 	= 0x11;
		static const uint8_t SH200I_FIFO_CONFIG = 0x12;
		static const uint8_t SH200I_ACC_RANGE 	= 0x16;
		static const uint8_t SH200I_GYRO_RANGE 	= 0x2B;
		static const uint8_t SH200I_OUTPUT_ACC 	= 0x00;
		static const uint8_t SH200I_OUTPUT_GYRO = 0x06;
		static const uint8_t SH200I_OUTPUT_TEMP = 0x0C;
		static const uint8_t SH200I_REG_SET1 	= 0xBA;
		static const uint8_t SH200I_REG_SET2 	= 0xCA;   //ADC reset
		static const uint8_t SH200I_ADC_RESET  	= 0xC2;   //drive reset
		static const uint8_t SH200I_SOFT_RESET 	= 0x7F;
		static const uint8_t SH200I_RESET 		= 0x75;

	public:
		_V_PROP_( Accelerometer )
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Gyroscope )
		_V_PROP_( Thermometer )

	public:
		void UpdateAccelerometerConfig()
		{
			uint8_t	AValue = 0b10000001; // Bypass

			if( ! Accelerometer().UseInternalClock().GetValue() )
				AValue |= 0b01000000;

			if( Accelerometer().LowPowerMode().GetValue() )
				AValue |= 0b00000100;

			switch( Accelerometer().Rate() )
			{
				case TArduinoSenodiaSH200QAccelerometerRate::Normal_512_LowPower_32_Hz :
					AValue |= 0b00001000;
					break;

				case TArduinoSenodiaSH200QAccelerometerRate::Normal_256_LowPower_16_Hz :
					AValue |= 0b00010000;
					break;

				case TArduinoSenodiaSH200QAccelerometerRate::Normal_128_LowPower_8_Hz :
					AValue |= 0b00011000;
					break;
			}

//			Write8bit( SH200I_ACC_CONFIG, 0x91 ); //0x81 1024hz   //0x89 512hz    //0x91  256hz 
			Write8bit( SH200I_ACC_CONFIG, AValue ); //0x81 1024hz   //0x89 512hz    //0x91  256hz 

//			Serial.println( AValue, BIN );
		}

		void UpdateAccelerometerRange()
		{
			uint8_t	AValue = 0;
			switch( Accelerometer().FullScaleRange() )
			{
				case TArduinoSenodiaSH200QAccelerometerRange::Max8g :
					AValue |= 0b00000001;
					break;

				case TArduinoSenodiaSH200QAccelerometerRange::Max16g :
					AValue |= 0b00000010;
					break;

			}

			Write8bit( SH200I_ACC_RANGE, AValue );

//			Serial.println( AValue, BIN );
		}

		void UpdateGyroscopeConfig()
		{
			uint8_t AValue = 0;
			if( Gyroscope().SampleFrequency().GetValue() < 250 ) // 31.25Hz
				AValue |= 0b0110;

			else if( Gyroscope().SampleFrequency().GetValue() < 500 ) // 250Hz
				AValue |= 0b0100;

			else if( Gyroscope().SampleFrequency().GetValue() < 1000 ) // 500Hz
				AValue |= 0b0010;

			else if( Gyroscope().SampleFrequency().GetValue() < 8000 ) // 1000Hz
				; //AValue |= 0b0000;

			else if( Gyroscope().SampleFrequency().GetValue() < 16000 ) // 8000Hz
				AValue |= 0b1000;

			else if( Gyroscope().SampleFrequency().GetValue() < 32000 ) // 16000Hz
				AValue |= 0b1010;

			else // 32000Hz
				AValue |= 0b1100;

			if( Gyroscope().AutoPowerDown().GetValue() )
				AValue |= 0b10000000;

			Write8bit( SH200I_GYRO_CONFIG, AValue ); //0x11 1000hz    //0x13  500hz   //0x15  256hz

			//set gyro dlpf 50hz
			Write8bit( SH200I_GYRO_DLPF, 0x03 ); //0x00 250hz   //0x01 200hz   0x02 100hz  0x03 50hz  0x04 25hz
		}

		void UpdateGyroscopeRange()
		{
			uint8_t	AValue = 0;
			switch( Gyroscope().FullScaleRange() )
			{
				case TArduinoSenodiaSH200QGyroscopeRange::Max1000dps :
					AValue |= 0b00000001;
					break;

				case TArduinoSenodiaSH200QGyroscopeRange::Max500dps :
					AValue |= 0b00000010;
					break;

				case TArduinoSenodiaSH200QGyroscopeRange::Max250dps :
					AValue |= 0b00000011;
					break;

				case TArduinoSenodiaSH200QGyroscopeRange::Max125dps :
					AValue |= 0b00000100;
					break;

			}

			Write8bit( SH200I_GYRO_RANGE, AValue );
		}

	protected:
		void BeginRead( uint8_t ARegisterAddress, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegisterAddress );  
			C_I2C.endTransmission( false );
			C_I2C.requestFrom( uint8_t( Address().GetValue()), ACount );
		}

		uint16_t ReadSingle16bit()
		{
			uint16_t AResult = C_I2C.read();
			AResult |= uint16_t( C_I2C.read() ) << 8;
			return AResult;
		}

		uint8_t Read8bit( uint8_t ARegisterAddress )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegisterAddress );  
			C_I2C.endTransmission( false );
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ));
			return C_I2C.read();
		}

		uint16_t Read16bit( uint8_t ARegisterAddress )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegisterAddress );  
			C_I2C.endTransmission( false );
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 2 ));

			uint16_t AResult = C_I2C.read();
			AResult |= uint16_t( C_I2C.read() ) << 8;
			return AResult;
		}

		void Write8bit( uint8_t ARegisterAddress, uint8_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegisterAddress );  
			C_I2C.write( AValue ); 
			C_I2C.endTransmission();
		}

		void ADCReset()
		{
			//set 0xC2 bit2 1-->0
			uint8_t AValue = Read8bit( SH200I_ADC_RESET );

			AValue |= 0x04;
			Write8bit( SH200I_ADC_RESET, AValue );
			//tempdata[0] = 0x0E; //CC
			//I2C_Write_NBytes(SH200I_ADDRESS, SH200I_ADC_RESET, 1, tempdata);
			delay(1);
  
			AValue &= 0xFB;
			//tempdata[0] = 0x0A; //C8
			Write8bit( SH200I_ADC_RESET, AValue );
		}

		void ReadSensors( bool AChangeOnly )
		{
			if( Thermometer().GetIsEnabled() )
			{
				float AValue = int16_t( Read16bit( SH200I_OUTPUT_TEMP )) / 333.87 + 21.0;
				if( Thermometer().InFahrenheit() )
					AValue = Func::ConvertCtoF( AValue );

				Thermometer().OutputPin().SetPinValue( AValue, AChangeOnly );
			}

			if( Accelerometer().GetIsEnabled() )
			{
				float ACoefficient;

				switch( Accelerometer().FullScaleRange() )
				{
					case TArduinoSenodiaSH200QAccelerometerRange::Max4g :
						ACoefficient = 4.0 / 32768.0;
						break;

					case TArduinoSenodiaSH200QAccelerometerRange::Max8g :
						ACoefficient = 8.0 / 32768.0;
						break;

					default :
						ACoefficient = 16.0 / 32768.0;
						break;
				}

				BeginRead( SH200I_OUTPUT_ACC, 6 );

				float AValueX = int16_t( ReadSingle16bit() ) * ACoefficient;
				float AValueY = int16_t( ReadSingle16bit() ) * ACoefficient;
				float AValueZ = int16_t( ReadSingle16bit() ) * ACoefficient;

				Accelerometer().OutputPinsX().SetPinValue( AValueX, AChangeOnly );
				Accelerometer().OutputPinsY().SetPinValue( AValueY, AChangeOnly );
				Accelerometer().OutputPinsZ().SetPinValue( AValueZ, AChangeOnly );
			}

			if( Gyroscope().GetIsEnabled() )
			{
				float ACoefficient;

				switch( Gyroscope().FullScaleRange() )
				{
					case TArduinoSenodiaSH200QGyroscopeRange::Max2000dps :
						ACoefficient = 2000.0 / 32768.0;
						break;

					case TArduinoSenodiaSH200QGyroscopeRange::Max1000dps :
						ACoefficient = 1000.0 / 32768.0;
						break;

					case TArduinoSenodiaSH200QGyroscopeRange::Max500dps :
						ACoefficient = 500.0 / 32768.0;
						break;

					case TArduinoSenodiaSH200QGyroscopeRange::Max250dps :
						ACoefficient = 250.0 / 32768.0;
						break;

					default :
						ACoefficient = 125.0 / 32768.0;
						break;
				}

				BeginRead( SH200I_OUTPUT_GYRO, 6 );

				float AValueX = int16_t( ReadSingle16bit() ) * ACoefficient;
				float AValueY = int16_t( ReadSingle16bit() ) * ACoefficient;
				float AValueZ = int16_t( ReadSingle16bit() ) * ACoefficient;

				Gyroscope().OutputPinsX().SetPinValue( AValueX, AChangeOnly );
				Gyroscope().OutputPinsY().SetPinValue( AValueY, AChangeOnly );
				Gyroscope().OutputPinsZ().SetPinValue( AValueZ, AChangeOnly );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemStartInit()
		{
			ADCReset();

			uint8_t AValue = Read8bit( 0xD8 );
			AValue |= 0x80;
			Write8bit( 0xD8, AValue );

			delay(1);

			AValue &= 0x7F;
			Write8bit( 0xD8, AValue );

			Write8bit( 0x78, 0x61 );
  
			delay(1);
      
			Write8bit( 0x78, 0x00 );
  
			//set acc odr 256hz
			UpdateAccelerometerConfig();
  
			//set gyro odr 500hz
			UpdateGyroscopeConfig();
  
			//set no buffer mode
			Write8bit( SH200I_FIFO_CONFIG, 0x00 );
  
			//set acc range +-8G
			UpdateAccelerometerRange();
//			Write8bit( SH200I_ACC_RANGE, 0x01 );
  
			//set gyro range +-2000¶È/s
			UpdateGyroscopeRange();
//			Write8bit( SH200I_GYRO_RANGE, 0x00 );
  
			Write8bit( SH200I_REG_SET1, 0xC0 );

			AValue = Read8bit( SH200I_REG_SET2 );
  
			//ADC Reset
			AValue |= 0x10;
			Write8bit( SH200I_REG_SET2, AValue );
  
			delay(1);
        
			AValue &= 0xEF;
			Write8bit( SH200I_REG_SET2, AValue );
  
			delay(10);

		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
}


