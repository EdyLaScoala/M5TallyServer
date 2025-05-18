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
#include <Mitov_LIS3MDL.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __LSM9DS1__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace LSM9DS1Const
	{
		const uint8_t WHO_AM_I           = 0x0F;
		const uint8_t CTRL_REG1_G        = 0x10;

		const uint8_t OUT_TEMP_L		 = 0x15;
		const uint8_t OUT_TEMP_H		 = 0x16;

		const uint8_t STATUS_REG         = 0x17;
		const uint8_t OUT_X_G            = 0x18;
		const uint8_t CTRL_REG5_XL       = 0x1F;
		const uint8_t CTRL_REG6_XL       = 0x20;
		const uint8_t CTRL_REG7_XL       = 0x21;
		const uint8_t CTRL_REG8          = 0x22;
		const uint8_t OUT_X_XL           = 0x28;
	}
//---------------------------------------------------------------------------
	template <
		typename T_AccelerometerGyroscope,
		typename T_Compass
	> class TArduinoLSM9DS1_I2C_Address : 
		public T_AccelerometerGyroscope,
		public T_Compass
	{
	public:
		_V_PROP_( AccelerometerGyroscope )
		_V_PROP_( Compass )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address
	> class Interface_LSM9DS1_I2C : 
		public T_Address
	{
	public:
		_V_PROP_( Address )

	protected:
		inline void Init()
		{
		}

		void WriteRegister8( uint8_t ARegister, uint8_t value )
		{
//			Serial.print( ARegister, HEX ); Serial.print( " : " ); Serial.println( value, HEX );

			C_I2C.beginTransmission( uint8_t( Address().AccelerometerGyroscope().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( value );
			C_I2C.endTransmission();
		}

		uint8_t ReadRegister8(uint8_t ARegister)
		{
			C_I2C.beginTransmission( uint8_t( Address().AccelerometerGyroscope().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().AccelerometerGyroscope().GetValue() ), uint8_t( 1 ) );
			return C_I2C.read();
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().AccelerometerGyroscope().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().AccelerometerGyroscope().GetValue() ), uint8_t( 2 ) );

			uint16_t AResult = C_I2C.read();

			AResult |= uint16_t( C_I2C.read() ) << 8;

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().AccelerometerGyroscope().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().AccelerometerGyroscope().GetValue() ), ACount );
		}


		void WriteCompassRegister8( uint8_t ARegister, uint8_t value )
		{
			C_I2C.beginTransmission( uint8_t( Address().Compass().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( value );
			C_I2C.endTransmission();
		}

		uint8_t ReadCompassRegister8(uint8_t ARegister)
		{
			C_I2C.beginTransmission( uint8_t( Address().Compass().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().Compass().GetValue() ), uint8_t( 1 ) );
			return C_I2C.read();
		}

		uint16_t ReadCompassRegister16( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().Compass().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().Compass().GetValue() ), uint8_t( 2 ) );

			uint16_t AResult = C_I2C.read();

			AResult |= uint16_t( C_I2C.read() ) << 8;

			return AResult;
		}

		void StartReadCompassRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().Compass().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().Compass().GetValue() ), ACount );
		}


		inline uint8_t ReadUInt8()
		{
			return C_I2C.read();
		}

		inline void EndReadRegisters()
		{
		}

		inline void EndReadCompassRegisters()
		{
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_CutoffFrequencyDivider,
		typename T_Enabled,
		typename T_HighResolution
	> class TArduinoLSM9DS1AccelerometerFilter :
		public T_CutoffFrequencyDivider,
		public T_Enabled,
		public T_HighResolution
	{
	public:
		_V_PROP_( CutoffFrequencyDivider )
		_V_PROP_( Enabled )
		_V_PROP_( HighResolution )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Filter,
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PowerDown,
		typename T_SampleRate
	> class TArduinoLSM9DS1Accelerometer :
		public T_Filter,
		public T_FullScaleRange,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_PowerDown,
		public T_SampleRate
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Filter )
		_V_PROP_( FullScaleRange )
		_V_PROP_( PowerDown )
		_V_PROP_( SampleRate )

	public:
		inline bool GetIsEnabledX() { return ( OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }
	};
//---------------------------------------------------------------------------
	namespace TArduinoLSM9DS1GyroscopeBandwidth
	{
		enum TArduinoLSM9DS1GyroscopeBandwidth { Lowest, Lower, Higher, Highest };
	}
//---------------------------------------------------------------------------
	template <
		typename T_FastRate,
		typename T_FullScaleRange,
		typename T_LowPowerMode,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PowerDown,
		typename T_SampleRate,
		typename T_SelfTest,
//		typename T_SingleShot,
		typename T_TemperatureCompensation,
		typename T_XYPerformanceMode,
		typename T_ZPerformanceMode
	> class TArduinoLSM9DS1Compass :
		public T_FastRate,
		public T_FullScaleRange,
		public T_LowPowerMode,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_PowerDown,
		public T_SampleRate,
		public T_SelfTest,
//		public T_SingleShot,
		public T_TemperatureCompensation,
		public T_XYPerformanceMode,
		public T_ZPerformanceMode		
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( FastRate )
		_V_PROP_( FullScaleRange )
		_V_PROP_( LowPowerMode )
		_V_PROP_( PowerDown )
		_V_PROP_( SampleRate )
		_V_PROP_( SelfTest )
//		_V_PROP_( SingleShot )
		_V_PROP_( TemperatureCompensation )
		_V_PROP_( XYPerformanceMode )
		_V_PROP_( ZPerformanceMode )

	public:
		inline bool GetIsEnabledX() { return ( OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Bandwidth,
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PowerDown,
		typename T_SampleRate
	> class TArduinoLSM9DS1Gyroscope :
		public T_Bandwidth,
		public T_FullScaleRange,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_PowerDown,
		public T_SampleRate
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Bandwidth )
		_V_PROP_( FullScaleRange )
		_V_PROP_( PowerDown )
		_V_PROP_( SampleRate )

	public:
		inline bool GetIsEnabledX() { return ( OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Accelerometer,
		typename T_Decimation,
		typename T_Gyroscope
	> class TArduinoLSM9DS1AccelerometerGyroscope : 
		public T_Accelerometer,
		public T_Decimation,
		public T_Gyroscope
	{
	public:
		_V_PROP_( Accelerometer )
		_V_PROP_( Decimation )
		_V_PROP_( Gyroscope )

	};
//---------------------------------------------------------------------------
	template <
		typename T_InFahrenheit,
		typename T_OutputPin
	> class TArduinoLSM9DS1Thermometer :
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InFahrenheit )

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_AccelerometerGyroscope,
		typename T_Compass,
		typename T_Enabled,
		typename T_Thermometer
	> class LSM9DS1 : 
		public T_IMPLEMENTATION,
		public T_AccelerometerGyroscope,
		public T_Compass,
		public T_Enabled,
		public T_Thermometer
	{
	public:
		_V_PROP_( AccelerometerGyroscope )
		_V_PROP_( Compass )
		_V_PROP_( Enabled )
		_V_PROP_( Thermometer )

	public:
		void Update_CTRL_REG5_XL()
		{
			uint8_t AValue;

			if( AccelerometerGyroscope().Decimation().GetValue() > 4 )
				AValue = 0b11000000;

			else if( AccelerometerGyroscope().Decimation().GetValue() > 2 )
				AValue = 0b10000000;

			else if( AccelerometerGyroscope().Decimation().GetValue() > 0 )
				AValue = 0b01000000;

			else
				AValue = 0b00000000;

			T_IMPLEMENTATION::WriteRegister8( LSM9DS1Const::CTRL_REG5_XL, AValue );
		}

		void Update_CTRL_REG6_XL()
		{
			uint8_t AValue;

			if( AccelerometerGyroscope().Accelerometer().PowerDown().GetValue() )
				AValue = 0;

			else
			{
				if( AccelerometerGyroscope().Accelerometer().SampleRate().GetValue() > 476.0 )
					AValue = 0b11000000;

				else if( AccelerometerGyroscope().Accelerometer().SampleRate().GetValue() > 238.0 )
					AValue = 0b10100000;

				else if( AccelerometerGyroscope().Accelerometer().SampleRate().GetValue() > 119.0 )
					AValue = 0b10000000;

				else if( AccelerometerGyroscope().Accelerometer().SampleRate().GetValue() > 50.0 )
					AValue = 0b01100000;

				else if( AccelerometerGyroscope().Accelerometer().SampleRate().GetValue() > 10.0 )
					AValue = 0b01000000;

				else
					AValue = 0b00100000;

			}

			if( AccelerometerGyroscope().Accelerometer().FullScaleRange().GetValue() > 8 )
				AValue |= 00001000;

			else if( AccelerometerGyroscope().Accelerometer().FullScaleRange().GetValue() > 4 )
				AValue |= 00011000;

			else if( AccelerometerGyroscope().Accelerometer().FullScaleRange().GetValue() > 2 )
				AValue |= 00010000;

			T_IMPLEMENTATION::WriteRegister8( LSM9DS1Const::CTRL_REG6_XL, AValue );
		}

		void Update_CTRL_REG7_XL()
		{
			uint8_t AValue;

			if( AccelerometerGyroscope().Accelerometer().Filter().CutoffFrequencyDivider().GetValue() > 100 )
				AValue = 0b01100000;

			else if( AccelerometerGyroscope().Accelerometer().Filter().CutoffFrequencyDivider().GetValue() > 50 )
				AValue = 0b00100000;

			else if( AccelerometerGyroscope().Accelerometer().Filter().CutoffFrequencyDivider().GetValue() > 9 )
				AValue = 0b00000000;

			else 
				AValue = 0b01000000;

			if( AccelerometerGyroscope().Accelerometer().Filter().HighResolution() )
				AValue = 0b10000000;

			if( AccelerometerGyroscope().Accelerometer().Filter().Enabled() )
				AValue = 0b00000001;

			T_IMPLEMENTATION::WriteRegister8( LSM9DS1Const::CTRL_REG7_XL, AValue );
		}

		void Update_CTRL_REG1_G()
		{
			uint8_t AValue;

			if( AccelerometerGyroscope().Gyroscope().PowerDown() )
				AValue = 0;

			else
			{
				if( AccelerometerGyroscope().Gyroscope().SampleRate().GetValue() > 476.0 )
					AValue = 0b11000000;

				else if( AccelerometerGyroscope().Gyroscope().SampleRate().GetValue() > 238.0 )
					AValue = 0b10100000;

				else if( AccelerometerGyroscope().Gyroscope().SampleRate().GetValue() > 119.0 )
					AValue = 0b10000000;

				else if( AccelerometerGyroscope().Gyroscope().SampleRate().GetValue() > 59.5 )
					AValue = 0b01100000;

				else if( AccelerometerGyroscope().Gyroscope().SampleRate().GetValue() > 14.9 )
					AValue = 0b01000000;

				else
					AValue = 0b00100000;

			}

			switch( AccelerometerGyroscope().Gyroscope().Bandwidth() )
			{
//				case TArduinoLSM9DS1GyroscopeBandwidth::Lowest	:
//					AValue |= 0b00000000;
//					break;

				case TArduinoLSM9DS1GyroscopeBandwidth::Lower	:
					AValue |= 0b00000001;
					break;

				case TArduinoLSM9DS1GyroscopeBandwidth::Higher	:
					AValue |= 0b00000010;
					break;

				case TArduinoLSM9DS1GyroscopeBandwidth::Highest	:
					AValue |= 0b00000011;

			}

			T_IMPLEMENTATION::WriteRegister8( LSM9DS1Const::CTRL_REG1_G, AValue );
		}

		void Update_CTRL_REG1_M()
		{
			uint8_t AValue = ( Compass().TemperatureCompensation() ) ? 0b10000000 : 0;

			if( Compass().SelfTest() )
				AValue |= 0b00000001;

			if( Compass().FastRate() )
				AValue |= 0b00000010;

			switch( Compass().XYPerformanceMode() )
			{
//				case TArduinoLIS3MDLCompassPerformanceMode::Low	:
//					AValue |= 0b00000000;
//					break;

				case TArduinoLIS3MDLCompassPerformanceMode::Medium	:
					AValue |= 0b00100000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::High	:
					AValue |= 0b01000000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::UltraHigh	:
					AValue |= 0b01100000;
			}

			if( Compass().SampleRate().GetValue() > 40.0 )
				AValue |= 0b01110000;

			else if( Compass().SampleRate().GetValue() > 20.0 )
				AValue |= 0b01100000;

			else if( Compass().SampleRate().GetValue() > 10.0 )
				AValue |= 0b01010000;

			else if( Compass().SampleRate().GetValue() > 5.5 )
				AValue |= 0b01000000;

			else if( Compass().SampleRate().GetValue() > 2.5 )
				AValue |= 0b00110000;

			else if( Compass().SampleRate().GetValue() > 1.25 )
				AValue |= 0b00100000;

			else if( Compass().SampleRate().GetValue() > 0.625 )
				AValue |= 0b00010000;

			T_IMPLEMENTATION::WriteCompassRegister8( LIS3MDLConst::CTRL_REG1_M, AValue );
		}

		void Update_CTRL_REG2_M()
		{
			uint8_t AValue;

			if( Compass().FullScaleRange().GetValue() > 12 )
				AValue |= 0b01100000;

			else if( Compass().FullScaleRange().GetValue() > 8 )
				AValue |= 0b01000000;

			else if( Compass().FullScaleRange().GetValue() > 8 )
				AValue |= 0b00100000;

			else
				AValue |= 0b00000000;

			T_IMPLEMENTATION::WriteCompassRegister8( LIS3MDLConst::CTRL_REG2_M, AValue );
		}

		void Update_CTRL_REG3_M()
		{
			uint8_t AValue = ( Compass().LowPowerMode() ) ? 00100000 : 0;

			if( Compass().PowerDown() )
				AValue |= 0b00000010;

//			if( Compass().SingleShot() )
//				AValue |= 0b00000001;

			T_IMPLEMENTATION::WriteCompassRegister8( LIS3MDLConst::CTRL_REG3_M, AValue );
		}

		void Update_CTRL_REG4_M()
		{
			uint8_t AValue;

			switch( Compass().ZPerformanceMode() )
			{
				case TArduinoLIS3MDLCompassPerformanceMode::Low	:
					AValue = 0b00000000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::Medium	:
					AValue = 0b00000100;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::High	:
					AValue = 0b00001000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::UltraHigh	:
					AValue = 0b00001100;
			}

			T_IMPLEMENTATION::WriteCompassRegister8( LIS3MDLConst::CTRL_REG4_M, AValue );
		}

	protected:
		void ResetAccelerometerGyroscope()
		{
			T_IMPLEMENTATION::WriteRegister8( LSM9DS1Const::CTRL_REG8, 0x05 );
		}

		void ResetCompass()
		{
			T_IMPLEMENTATION::WriteCompassRegister8( LIS3MDLConst::CTRL_REG2_M, 0x0C );
		}

		void ReadSensorsAccelerometerGyroscope( bool AClocked )
		{
			if( ! Enabled().GetValue() )
				return;

			uint8_t AStaus = T_IMPLEMENTATION::ReadRegister8( LSM9DS1Const::STATUS_REG );
			if( AccelerometerGyroscope().Accelerometer().GetIsEnabled() )
				if( ( AStaus & 0b00000001 ) || AClocked )
				{
					T_IMPLEMENTATION::StartReadRegisters( LSM9DS1Const::OUT_X_XL, 6 );
					int16_t AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					float AFloatValue = ( float( AValue ) / 0xFFFF ) * AccelerometerGyroscope().Accelerometer().FullScaleRange().GetValue();

					AccelerometerGyroscope().Accelerometer().OutputPinsX().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * AccelerometerGyroscope().Accelerometer().FullScaleRange().GetValue();

					AccelerometerGyroscope().Accelerometer().OutputPinsY().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * AccelerometerGyroscope().Accelerometer().FullScaleRange().GetValue();

					AccelerometerGyroscope().Accelerometer().OutputPinsZ().SetPinValue( AFloatValue );

					T_IMPLEMENTATION::EndReadRegisters();
				}		

			if( AccelerometerGyroscope().Gyroscope().GetIsEnabled() )
				if( ( AStaus & 0b00000010 ) || AClocked )
				{
					T_IMPLEMENTATION::StartReadRegisters( LSM9DS1Const::OUT_X_G, 6 );
					int16_t AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					float AFloatValue = ( float( AValue ) / 0xFFFF ) * AccelerometerGyroscope().Gyroscope().FullScaleRange().GetValue();

					AccelerometerGyroscope().Gyroscope().OutputPinsX().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * AccelerometerGyroscope().Gyroscope().FullScaleRange().GetValue();

					AccelerometerGyroscope().Gyroscope().OutputPinsY().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * AccelerometerGyroscope().Gyroscope().FullScaleRange().GetValue();

					AccelerometerGyroscope().Gyroscope().OutputPinsZ().SetPinValue( AFloatValue );

					T_IMPLEMENTATION::EndReadRegisters();
				}

			if( Thermometer().OutputPin().GetPinIsConnected() )
				if( ( AStaus & 0b00000100 ) || AClocked )
				{
					T_IMPLEMENTATION::StartReadRegisters( LSM9DS1Const::OUT_TEMP_L, 2 );

					int16_t AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					float AFloatValue = 25.0 + float( AValue ) / 8; //( float( AValue ) / 0xFFFF ) * AccelerometerGyroscope().Gyroscope().FullScaleRange().GetValue();

					Thermometer().OutputPin().SetPinValue( AFloatValue );

					T_IMPLEMENTATION::EndReadRegisters();
				}
			
		}

		void ReadSensorsCompass( bool AClocked )
		{
			if( ! Enabled().GetValue() )
				return;

			if( Compass().GetIsEnabled() )
			{
				uint8_t ACompassStaus = T_IMPLEMENTATION::ReadCompassRegister8( LIS3MDLConst::STATUS_REG_M );
//				Serial.println( ACompassStaus, BIN );
				if( ( ACompassStaus & 0b00001000 ) || AClocked )
				{
					T_IMPLEMENTATION::StartReadCompassRegisters( LIS3MDLConst::OUT_X_L_M, 6 );
					int16_t AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					float AFloatValue = ( float( AValue ) / 0xFFFF ) * Compass().FullScaleRange().GetValue() * 2 * 100.0;

					Compass().OutputPinsX().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * Compass().FullScaleRange().GetValue() * 2 * 100.0;

					Compass().OutputPinsY().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * Compass().FullScaleRange().GetValue() * 2 * 100.0;

					Compass().OutputPinsZ().SetPinValue( AFloatValue );

					T_IMPLEMENTATION::EndReadCompassRegisters();
				}
			}
		}

	public:
		inline void AccelerometerGyroscope_ResetInputPin_o_Receive( void *_Data )
		{
			ResetAccelerometerGyroscope();
			SystemStartClock();
		}

		inline void Compass_ResetInputPin_o_Receive( void *_Data )
		{
			ResetCompass();
			SystemStartCompassClock();
		}

		inline void AccelerometerGyroscope_ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensorsAccelerometerGyroscope( true );
		}

		inline void Compass_ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensorsCompass( true );
		}

	public:
		inline void SystemInit()
		{
			T_IMPLEMENTATION::Init();

			ResetAccelerometerGyroscope();
			ResetCompass();

			delay( 10 );
		}

		inline void SystemStartCompassClock()
		{
			Update_CTRL_REG1_M();
			Update_CTRL_REG2_M();
			Update_CTRL_REG3_M();
			Update_CTRL_REG4_M();
		}

		inline void SystemStartCompass()
		{
			SystemStartCompassClock();
			ReadSensorsCompass( false );
		}

		inline void SystemStartClock()
		{
			Update_CTRL_REG5_XL();
			Update_CTRL_REG6_XL();
			Update_CTRL_REG7_XL();
			Update_CTRL_REG1_G();
		}

		inline void SystemStartAccelerometerGyroscope()
		{
			SystemStartClock();
			ReadSensorsAccelerometerGyroscope( false );
		}

		inline void SystemLoopBeginCompass()
		{
			ReadSensorsCompass( false );
		}

		inline void SystemLoopBeginAccelerometerGyroscope()
		{
			ReadSensorsAccelerometerGyroscope( false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

