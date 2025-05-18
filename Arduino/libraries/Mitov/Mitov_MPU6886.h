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
#include <Mitov_BasicMPU.h>
//#include <Mitov_MPU9250.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_FilterCutoffFrequency,
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class MPU6886Accelerometer :
		public T_FilterCutoffFrequency,
		public T_FullScaleRange,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( FilterCutoffFrequency )
		_V_PROP_( FullScaleRange )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

		inline bool GetIsEnabledX() { return ( X().Enabled() && OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( Y().Enabled() && OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( Z().Enabled() && OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }
	};
//---------------------------------------------------------------------------
	template <
		typename T_FilterCutoffFrequency,
		typename T_FullScaleRange,
//		typename T_HighSampleRate,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Standby,
		typename T_Units,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class MPU6886Gyroscope :
		public T_FilterCutoffFrequency,
		public T_FullScaleRange,
//		public T_HighSampleRate,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Standby,
		public T_Units,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	public:
		_V_PROP_( FilterCutoffFrequency )
		_V_PROP_( FullScaleRange )
//		_V_PROP_( HighSampleRate )
		_V_PROP_( Units )
		_V_PROP_( Standby )

	public:
		inline bool GetIsEnabledX() { return ( X().Enabled() && OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( Y().Enabled() && OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( Z().Enabled() && OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Accelerometer,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ClockSource,
		typename T_Enabled,
		typename T_FrameSynchronization,
		typename T_Gyroscope,
		typename T_Interrupt,
		typename T_LowPowerMode,
		typename T_SampleRateDivider,
		typename T_Thermometer
	> class MPU6886I2C :
		public T_Accelerometer,
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_ClockSource,
		public T_Enabled,
		public T_FrameSynchronization,
		public T_Gyroscope,
		public T_Interrupt,
		public T_LowPowerMode,
		public T_SampleRateDivider,
		public T_Thermometer
	{
	protected:
		static const uint8_t MPU6886_WHOAMI            = 0x75;
		static const uint8_t MPU6886_ACCEL_INTEL_CTRL  = 0x69;
		static const uint8_t MPU6886_SMPLRT_DIV        = 0x19;
		static const uint8_t MPU6886_I2C_MST_STATUS    = 0x36;
		static const uint8_t MPU6886_INT_PIN_CFG       = 0x37;
		static const uint8_t MPU6886_INT_ENABLE        = 0x38;
		static const uint8_t MPU6886_INT_STATUS		   = 0x3A;

		static const uint8_t MPU6886_ACCEL_XOUT_H      = 0x3B;
		static const uint8_t MPU6886_ACCEL_XOUT_L      = 0x3C;
		static const uint8_t MPU6886_ACCEL_YOUT_H      = 0x3D;
		static const uint8_t MPU6886_ACCEL_YOUT_L      = 0x3E;
		static const uint8_t MPU6886_ACCEL_ZOUT_H      = 0x3F;
		static const uint8_t MPU6886_ACCEL_ZOUT_L      = 0x40;

		static const uint8_t MPU6886_TEMP_OUT_H        = 0x41;
		static const uint8_t MPU6886_TEMP_OUT_L        = 0x42;

		static const uint8_t MPU6886_GYRO_XOUT_H       = 0x43;
		static const uint8_t MPU6886_GYRO_XOUT_L       = 0x44;
		static const uint8_t MPU6886_GYRO_YOUT_H       = 0x45;
		static const uint8_t MPU6886_GYRO_YOUT_L       = 0x46;
		static const uint8_t MPU6886_GYRO_ZOUT_H       = 0x47;
		static const uint8_t MPU6886_GYRO_ZOUT_L       = 0x48;

		static const uint8_t MPU6886_USER_CTRL         = 0x6A;
		static const uint8_t MPU6886_PWR_MGMT_1        = 0x6B;
		static const uint8_t MPU6886_PWR_MGMT_2        = 0x6C;
		static const uint8_t MPU6886_CONFIG            = 0x1A;
		static const uint8_t MPU6886_GYRO_CONFIG       = 0x1B;
		static const uint8_t MPU6886_ACCEL_CONFIG      = 0x1C;
		static const uint8_t MPU6886_ACCEL_CONFIG2     = 0x1D;
		static const uint8_t MPU6886_FIFO_EN           = 0x23;

	public:
		_V_PROP_( Accelerometer )
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( FrameSynchronization )
		_V_PROP_( ClockSource )
		_V_PROP_( Gyroscope )
		_V_PROP_( Interrupt )
		_V_PROP_( LowPowerMode )
		_V_PROP_( SampleRateDivider )
		_V_PROP_( Thermometer )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		inline void WriteTo( uint8_t ARegister, uint8_t AData )
		{
//			Serial.print( "REG: " ); Serial.print( ARegister, HEX ); Serial.print( " = " ); Serial.println( AData, BIN );
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), ARegister, AData );
		}

		bool ReadBytes( uint8_t ARegister, uint8_t ALength, void *AData )
		{
			return I2C::ReadBytes( C_I2C, uint8_t( Address().GetValue()), ARegister, ALength, AData );
		}

	public:
		inline void UpdateClockSource()
		{
			UpdatePowerManagementReg1( false );
		}

		void UpdatePowerManagementReg1( bool AReset = false )
		{
			uint8_t AValue;
			if( ClockSource() == mcsAutoSelect )
			{
				if( Gyroscope().GetIsEnabledX() )
					AValue = mcsGyroX;

				else if( Gyroscope().GetIsEnabledY() )
					AValue = mcsGyroY;

				else if( Gyroscope().GetIsEnabledZ() )
					AValue = mcsGyroZ;

				else
					AValue = mcsInternal;
			}

			else
				AValue = ClockSource();

			AValue |=	( AReset					?		0b10000000 : 0 ) |
						( Enabled()					? 0 :	0b01000000 ) |
						( LowPowerMode().Enabled()	?		0b00100000 : 0 ) |
						( ( Thermometer().Enabled() && T_Thermometer::GetPinIsConnected() ) ? 0 :	0b00001000 );
//						0b00001000;

//			Serial.print( "UpdatePowerManagementReg1: " ); Serial.println( AValue, BIN );
			WriteTo( MPU6886_PWR_MGMT_1, AValue );
		}

		void UpdatePowerManagementReg2()
		{
/*
			const float CLevels[] =
			{
				1.25,
				5,
				20,
				40
			};

			int AValue = -1;
			for( int i = 0; i < sizeof( CLevels ) / sizeof( CLevels[ 0 ] ); ++i )
				if( LowPowerMode().SampleFrequency() <= CLevels[ i ] )
				{
					AValue = i;
					break;
				}

			if( AValue == -1 )
				AValue = 0;

			AValue <<= 6;

			AValue |=	( Accelerometer().GetIsEnabledX()	? 0 : 0b00100000 ) |
						( Accelerometer().GetIsEnabledY()	? 0 : 0b00010000 ) |
						( Accelerometer().GetIsEnabledZ()	? 0 : 0b00001000 );

			AValue |=	( Gyroscope().GetIsEnabledX()	? 0 : 0b00000100 ) |
						( Gyroscope().GetIsEnabledY()	? 0 : 0b00000010 ) |
						( Gyroscope().GetIsEnabledZ()	? 0 : 0b00000001 );

			WriteTo( MPU6886_PWR_MGMT_2, (uint8_t)AValue );
*/
/*
			uint8_t AValue =	( Accelerometer().GetIsEnabledX()	? 0 : 0b00100000 ) |
								( Accelerometer().GetIsEnabledY()	? 0 : 0b00010000 ) |
								( Accelerometer().GetIsEnabledZ()	? 0 : 0b00001000 ) |

								( Gyroscope().GetIsEnabledX()		? 0 : 0b00000100 ) |
								( Gyroscope().GetIsEnabledY()		? 0 : 0b00000010 ) |
								( Gyroscope().GetIsEnabledZ()		? 0 : 0b00000001 );

//			AValue = 0;
//			AValue = 0b00111111;
			WriteTo( MPU6886_PWR_MGMT_2, AValue );
*/
			// For now always enable all channels. Seems to cause problems if channel is disabled
			WriteTo( MPU6886_PWR_MGMT_2, 0 );
		}

		void UpdateConfigReg()
		{
			uint8_t AValue;

			if( Accelerometer().FilterCutoffFrequency() < 10.2 )
				AValue = 6;

			else if( Accelerometer().FilterCutoffFrequency() < 21.2 )
				AValue = 5;

			else if( Accelerometer().FilterCutoffFrequency() < 44.8 )
				AValue = 4;

			else if( Accelerometer().FilterCutoffFrequency() < 99.0 )
				AValue = 3;

			else if( Accelerometer().FilterCutoffFrequency() < 218.1 )
				AValue = 2;

			else if( Accelerometer().FilterCutoffFrequency() < 420.0 )
				AValue = 0;

			else if( Accelerometer().FilterCutoffFrequency() < 1046.0 )
				AValue = 7;

			else
				AValue = 8;

			AValue |=
//						( Queue.BlockOnFull ? 0b01000000 : 0 ) |
						FrameSynchronization().OutputPin().GetPinIsConnected() ? ( FrameSynchronization().Location() << 4 ) : 0;

			WriteTo( MPU6886_CONFIG, AValue );
		}

		void UpdateGyroConfigReg()
		{
			uint8_t AValue =	(( Gyroscope().FullScaleRange() & 0b11 ) << 3 ) |
						( Gyroscope().X().SelfTest() ? 0b10000000 : 0 ) |
						( Gyroscope().Y().SelfTest() ? 0b01000000 : 0 ) |
						( Gyroscope().Z().SelfTest() ? 0b00100000 : 0 );

			WriteTo( MPU6886_GYRO_CONFIG, AValue );
		}

		void UpdateAccelerometerConfigReg()
		{
			uint8_t AValue =	(( Accelerometer().FullScaleRange() & 0b11 ) << 3 ) |
							( Accelerometer().X().SelfTest() ? 0b10000000 : 0 ) |
							( Accelerometer().Y().SelfTest() ? 0b01000000 : 0 ) |
							( Accelerometer().Z().SelfTest() ? 0b00100000 : 0 );

			WriteTo( MPU6886_ACCEL_CONFIG, AValue );
		}

		void UpdateSampleRateDividerReg()
		{
			WriteTo( MPU6886_SMPLRT_DIV, SampleRateDivider() );
		}

		inline void UpdatenterruptPinAndBypassConfigRegNormal() // For Visual Live Binding!
		{
			UpdatenterruptPinAndBypassConfigReg();
		}

		void UpdatenterruptPinAndBypassConfigReg()
		{
			uint8_t AValue =	( Interrupt().Inverted()								?	0b10000000 : 0 ) |
							( Interrupt().OpenDrain()								?	0b01000000 : 0 ) |
							( Interrupt().Latch()									?	0b00100000 : 0 ) |
							( Interrupt().OnFrameSync()                             ?	0b00000100 : 0 )
//#ifdef _MITOV_FRAMES_SYNC_
//							| ( FrameSynchronization.InterruptOnLowLevel			?		0b00001000 : 0 ) |
//							( FrameSynchronization.EnableInterrupt				?		0b00000100 : 0 )
//#endif
							;
//							( /*Compass.Queue & */ ( ! ADirectCompassAccess )		? 0 :	0b00000010 );

//			Serial.print( "UpdatenterruptPinAndBypassConfigReg: " ); Serial.println( AValue, BIN );

			if( FrameSynchronization().OutputPin().GetPinIsConnected() )
				AValue |=	( FrameSynchronization().InterruptOnLowLevel()			?		0b00001000 : 0 ) |
							( FrameSynchronization().EnableInterrupt()				?		0b00000100 : 0 );

			WriteTo( MPU6886_INT_PIN_CFG, AValue );
		}

		void UpdateInteruptEnableReg()
		{
			uint8_t AValue =
//							( Interrupt.OnQueueOverflow	?	0b00010000 : 0 ) |
//							( Interrupt().OnFrameSync()		?	0b00001000 : 0 ) |
//							( Interrupt().OnRawReady()		?	0b00000001 : 0 );
							( Interrupt().OnRawReady()		?	0b11100101 : 0 );

			WriteTo( MPU6886_INT_ENABLE, AValue );
		}

	protected:
		void Reset()
		{
			WriteTo(MPU6886_PWR_MGMT_1, 0 );
			delay(10);

			WriteTo(MPU6886_PWR_MGMT_1, 10000000 );
			delay(10);

			UpdatePowerManagementReg1();
			delay(10);
			UpdatePowerManagementReg2();
			UpdateConfigReg();
			UpdateGyroConfigReg();
			UpdateAccelerometerConfigReg();
			UpdateSampleRateDividerReg();
			UpdatenterruptPinAndBypassConfigReg();
//			UpdateUserControlReg();
//			UpdateFIFOReg();

			UpdateInteruptEnableReg();
		}

		void ReadSensors()
		{
//			Serial.println( "ReadSensors" );
			const float AccelerometerCoefficients [] =
			{
				2.0f / 32768,
				4.0f / 32768,
				8.0f / 32768,
				16.0f / 32768
			};

			const float GyroscopeCoefficients [] =
			{
				250.0f / 32768,
				500.0f / 32768,
				1000.0f / 32768,
				2000.0f / 32768
			};

			const float GyroscopeUnits [] =
			{
				1,
				PI / 180,
				1 / 360
			};

			uint8_t AIntValues[ 7 * 2 ];

			if( ReadBytes( MPU6886_ACCEL_XOUT_H, sizeof( AIntValues ), AIntValues ))
			{
				if( T_Accelerometer::GetIsEnabled() )
					for( int i = 0; i < 3; ++i )
					{
//						Serial.println( i );
						int16_t AIntValue = ( int16_t( int8_t( AIntValues[ i * 2 ] )) << 8 ) | AIntValues[ i * 2 + 1 ]; // Use temporary variable for ESP8266 compatibility!
//						Serial.println( AIntValue );
						float AValue = AIntValue * AccelerometerCoefficients[ Accelerometer().FullScaleRange() ];
						switch( i )
						{
							case 0: T_Accelerometer::OutputPinsX().SetPinValue( AValue ); break;
							case 1: T_Accelerometer::OutputPinsY().SetPinValue( AValue ); break;
							case 2: T_Accelerometer::OutputPinsZ().SetPinValue( AValue ); break;
						}
//						Accelerometer.OutputPins[ i ].Notify( &AValue );
					}

				if( T_Gyroscope::GetIsEnabled() )
					for( int i = 0; i < 3; ++i )
					{
						int16_t AIntValue = ( int16_t( int8_t( AIntValues[ ( i + 4 ) * 2 ] )) << 8 ) | AIntValues[ ( i + 4 ) * 2 + 1 ]; // Use temporary variable for ESP8266 compatibility!
						float AValue = AIntValue * GyroscopeCoefficients[ Gyroscope().FullScaleRange() ] * GyroscopeUnits[ Gyroscope().Units() ];
						switch( i )
						{
							case 0: T_Gyroscope::OutputPinsX().SetPinValue( AValue ); break;
							case 1: T_Gyroscope::OutputPinsY().SetPinValue( AValue ); break;
							case 2: T_Gyroscope::OutputPinsZ().SetPinValue( AValue ); break;
						}
//						Gyroscope.OutputPins[ i ].Notify( &AValue );
					}

//				float AValue = ((((int16_t)AIntValues[ 3 * 2 ] ) << 8 ) | AIntValues[ 3 * 2 + 1 ] ) / 333.87 + 21.0;
				if( T_Thermometer::GetIsEnabled() )
				{
					int16_t AIntValue = ( int16_t( int8_t( AIntValues[ 3 * 2 ] )) << 8 ) | AIntValues[ 3 * 2 + 1 ]; // Use temporary variable for ESP8266 compatibility!
//					float AValue = (float( AIntValue ) + 12412.0 ) / 340.0;
					float AValue = float( AIntValue ) / 326.8 + 25.0;
//					Serial.println( AValue );
					if( Thermometer().InFahrenheit() )
						AValue = Func::ConvertCtoF( AValue );

					T_Thermometer::SetPinValue( AValue );
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	public:
		inline void SystemInit()
		{
/*
			UpdatePowerManagementReg1();
			UpdatePowerManagementReg2();
			UpdateConfigReg();
			UpdateGyroConfigReg();
			UpdateAccelerometerConfigReg();
			UpdateSampleRateDividerReg();
			UpdatenterruptPinAndBypassConfigReg();
//			UpdateUserControlReg();
//			UpdateFIFOReg();

			UpdateInteruptEnableReg();
*/
			Reset();
/*
  uint8_t regdata;

  regdata = 0x00;
  WriteTo(MPU6886_PWR_MGMT_1, regdata);
  delay(10);

  regdata = (0x01<<7);
  WriteTo(MPU6886_PWR_MGMT_1, regdata);
  delay(10);

  regdata = (0x01<<0);
  WriteTo(MPU6886_PWR_MGMT_1, regdata);
  delay(10);

//			UpdatePowerManagementReg1();
//			UpdatePowerManagementReg2();


			UpdatePowerManagementReg1();
//			UpdatePowerManagementReg2();
			UpdateConfigReg();
			UpdateGyroConfigReg();
			UpdateAccelerometerConfigReg();
			UpdateSampleRateDividerReg();
			UpdatenterruptPinAndBypassConfigReg();
//			UpdateUserControlReg();
//			UpdateFIFOReg();

			UpdateInteruptEnableReg();
*/
/*
  regdata = 0x10;
  WriteTo(MPU6886_ACCEL_CONFIG, regdata);
  delay(1);

  regdata = 0x18;
  WriteTo(MPU6886_GYRO_CONFIG, regdata);
  delay(1);

  regdata = 0x01;
  WriteTo(MPU6886_CONFIG, regdata);
  delay(1);

  regdata = 0x05;
  WriteTo(MPU6886_SMPLRT_DIV, regdata);
  delay(1);

  regdata = 0x00;
  WriteTo(MPU6886_INT_ENABLE, regdata);
  delay(1);

  regdata = 0x00;
  WriteTo(MPU6886_ACCEL_CONFIG2, regdata);
  delay(1);

  regdata = 0x00;
  WriteTo(MPU6886_USER_CTRL, regdata);
  delay(1);

  regdata = 0x00;
  WriteTo(MPU6886_FIFO_EN, regdata);
  delay(1);

  regdata = 0x22;
  WriteTo(MPU6886_INT_PIN_CFG, regdata);
  delay(1);

  regdata = 0x01;
  WriteTo(MPU6886_INT_ENABLE, regdata);
*/
		}

		inline void SystemStart() {}

/*
		inline void SystemStartGenerate()
		{
			SystemStartInit();
			ReadSensors( false );
		}
*/
		inline void SystemLoopBegin()
		{
			if( Enabled() )
			{
				if( ! ClockInputPin_o_IsConnected() )
					ReadSensors();

/*
				if( FUseFIFO )
				{
					if( Queue.SizeOutputPin.IsConnected() )
					{
						uint8_t AIntValues[ 2 ];
						if( ReadBytes( MPU9250_RA_FIFO_COUNTH, 2, AIntValues ))
						{
							uint32_t ASize = (( ((int16_t)AIntValues[ 0 ] ) << 8 ) | AIntValues[ 1 ] );
							Queue.SizeOutputPin.Notify( &ASize );
						}
					}
				}
*/
				if( FrameSynchronization().OutputPin().GetPinIsConnected() )
				{
					uint8_t AIntValue;
//					if( ReadBytes( MPU9250_RA_INT_STATUS, 1, &AIntValue ) )
					if( ReadBytes( MPU6886_INT_STATUS, 1, &AIntValue ) )
//					if( Queue.OverflowOutputPin.IsConnected() || MotionDetect.OutputPin.IsConnected() || FrameSynchronization.OutputPin.IsConnected() )
					{
//						if( AIntValue & 0x00010000 )
//							Queue.OverflowOutputPin.Notify( nullptr );

						if( AIntValue & 0x00001000 )
							T_FrameSynchronization::ClockPin();

					}
				}
			}
		}

	};
//---------------------------------------------------------------------------
}

