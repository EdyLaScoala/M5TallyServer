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
#include <Wire.h> //I2C Arduino Library
#include <Mitov_Basic_I2C.h>
#include <Mitov_BasicMPU.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoMPU6050Filter
	{
      f_AB_260Hz_GB_256Hz_F_8KHz,
      f_AB_184Hz_GB_188Hz_F_1KHz,
      f_AB_94Hz_GB_98Hz_F_1KHz,
      f_AB_44Hz_GB_42Hz_F_1KHz,
      f_AB_21Hz_GB_20Hz_F_1KHz,
      f_AB_10Hz_GB_10Hz_F_1KHz,
      f_AB_5Hz_GB_5Hz_F_1KHz
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Accelerometer,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ClockSource,
		typename T_Enabled,
		typename T_Filter,
		typename T_FrameSynchronization,
		typename T_Gyroscope,
		typename T_Interrupt,
		typename T_LowPowerMode,
		typename T_SampleRateDivider,
		typename T_Thermometer
	> class MPU6050I2C :
		public T_Address,
		public T_Accelerometer,
		public T_ClockInputPin_o_IsConnected,
		public T_ClockSource,
		public T_Enabled,
		public T_Filter,
		public T_FrameSynchronization,
		public T_Gyroscope,
		public T_Interrupt,
		public T_LowPowerMode,
		public T_SampleRateDivider,
		public T_Thermometer
	{
	protected:
		static const uint8_t MPU6050_SMPLRT_DIV			= 0x19;
		static const uint8_t MPU6050_CONFIG				= 0x1A;
		static const uint8_t MPU6050_GYRO_CONFIG		= 0x1B;
		static const uint8_t MPU6050_ACCEL_CONFIG		= 0x1C;

		static const uint8_t MPU6050_FIFO_EN			= 0x23;
		static const uint8_t MPU6050_I2C_MST_CTRL		= 0x24;
		static const uint8_t MPU6050_I2C_SLV0_ADDR		= 0x25;
		static const uint8_t MPU6050_I2C_SLV0_REG		= 0x26;
		static const uint8_t MPU6050_I2C_SLV0_CTRL		= 0x27;
		static const uint8_t MPU6050_I2C_SLV1_ADDR		= 0x28;
		static const uint8_t MPU6050_I2C_SLV1_REG		= 0x29;
		static const uint8_t MPU6050_I2C_SLV1_CTRL		= 0x2A;
		static const uint8_t MPU6050_I2C_SLV2_ADDR		= 0x2B;
		static const uint8_t MPU6050_I2C_SLV2_REG		= 0x2C;
		static const uint8_t MPU6050_I2C_SLV2_CTRL		= 0x2D;
		static const uint8_t MPU6050_I2C_SLV3_ADDR		= 0x2E;
		static const uint8_t MPU6050_I2C_SLV3_REG		= 0x2F;
		static const uint8_t MPU6050_I2C_SLV3_CTRL		= 0x30;
		static const uint8_t MPU6050_I2C_SLV4_ADDR		= 0x31;
		static const uint8_t MPU6050_I2C_SLV4_REG		= 0x32;
		static const uint8_t MPU6050_I2C_SLV4_DO		= 0x33;
		static const uint8_t MPU6050_I2C_SLV4_CTRL		= 0x34;
		static const uint8_t MPU6050_I2C_SLV4_DI		= 0x35;
		static const uint8_t MPU6050_INT_PIN_CFG		= 0x37;
		static const uint8_t MPU6050_INT_ENABLE			= 0x38;
//		static const uint8_t MPU6050_I2C_MST_STATUS		= 0x39;
		static const uint8_t MPU6050_INT_STATUS			= 0x3A;

		static const uint8_t MPU6050_ACCEL_XOUT_H		= 0x3B;
		static const uint8_t MPU6050_ACCEL_XOUT_L		= 0x3C;
		static const uint8_t MPU6050_ACCEL_YOUT_H		= 0x3D;
		static const uint8_t MPU6050_ACCEL_YOUT_L		= 0x3E;
		static const uint8_t MPU6050_ACCEL_ZOUT_H		= 0x3F;
		static const uint8_t MPU6050_ACCEL_ZOUT_L		= 0x40;

		static const uint8_t MPU6050_EXT_SENS_DATA_00	= 0x49;
		static const uint8_t MPU6050_EXT_SENS_DATA_01	= 0x4A;
		static const uint8_t MPU6050_EXT_SENS_DATA_02	= 0x4B;
		static const uint8_t MPU6050_EXT_SENS_DATA_03	= 0x4C;
		static const uint8_t MPU6050_EXT_SENS_DATA_04	= 0x4D;
		static const uint8_t MPU6050_EXT_SENS_DATA_05	= 0x4E;
		static const uint8_t MPU6050_EXT_SENS_DATA_06	= 0x4F;
		static const uint8_t MPU6050_EXT_SENS_DATA_07	= 0x50;
		static const uint8_t MPU6050_EXT_SENS_DATA_08	= 0x51;
		static const uint8_t MPU6050_EXT_SENS_DATA_09	= 0x52;
		static const uint8_t MPU6050_EXT_SENS_DATA_10	= 0x53;
		static const uint8_t MPU6050_EXT_SENS_DATA_11	= 0x54;
		static const uint8_t MPU6050_EXT_SENS_DATA_12	= 0x55;
		static const uint8_t MPU6050_EXT_SENS_DATA_13	= 0x56;
		static const uint8_t MPU6050_EXT_SENS_DATA_14	= 0x57;
		static const uint8_t MPU6050_EXT_SENS_DATA_15	= 0x58;
		static const uint8_t MPU6050_EXT_SENS_DATA_16	= 0x59;
		static const uint8_t MPU6050_EXT_SENS_DATA_17	= 0x5A;
		static const uint8_t MPU6050_EXT_SENS_DATA_18	= 0x5B;
		static const uint8_t MPU6050_EXT_SENS_DATA_19	= 0x5C;
		static const uint8_t MPU6050_EXT_SENS_DATA_20	= 0x5D;
		static const uint8_t MPU6050_EXT_SENS_DATA_21	= 0x5E;
		static const uint8_t MPU6050_EXT_SENS_DATA_22	= 0x5F;
		static const uint8_t MPU6050_EXT_SENS_DATA_23	= 0x60;

		static const uint8_t MPU6050_USER_CTRL			= 0x6A;
		static const uint8_t MPU6050_PWR_MGMT_1			= 0x6B;
		static const uint8_t MPU6050_PWR_MGMT_2			= 0x6C;

		static const uint8_t MPU6050_FIFO_COUNTH		= 0x72;
		static const uint8_t MPU6050_FIFO_COUNTL		= 0x73;
		static const uint8_t MPU6050_FIFO_R_W			= 0x74;

	public:
//#ifdef _MITOV_MPU_ACCELEROMETER_
		_V_PROP_( Accelerometer )
//#endif

//#ifdef _MITOV_MPU_GYROSCOPE_
		_V_PROP_( Gyroscope )
//#endif

		_V_PROP_( Thermometer )

//#ifdef _MITOV_FRAMES_SYNC_
		_V_PROP_( FrameSynchronization )
//#endif

//		MPU9250Queue				Queue;
		_V_PROP_( Interrupt )
		_V_PROP_( LowPowerMode )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( ClockSource )
		_V_PROP_( Filter )
		_V_PROP_( SampleRateDivider )

	public:
		inline void UpdateClockSource()
		{
			UpdatePowerManagementReg1( false );
		}

//	protected:
//		bool	FUseFIFO : 1;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

		void ResetInputPin_o_Receive( void *_Data )
		{
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), MPU6050_USER_CTRL, 0b00000001 );
		}

	protected:
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

			if( ReadBytes( MPU6050_ACCEL_XOUT_H, sizeof( AIntValues ), AIntValues ))
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
					float AValue = (float( AIntValue ) + 12412.0 ) / 340.0;
					if( Thermometer().InFahrenheit() )
						AValue = Func::ConvertCtoF( AValue );

					T_Thermometer::SetPinValue( AValue );
				}
			}
		}

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
//#ifdef _MITOV_MPU_THERMOMETER_
						( ( Thermometer().Enabled() && T_Thermometer::GetPinIsConnected() ) ? 0 :	0b00001000 );
//#else
//						0b00001000;
//#endif

//			Serial.print( "UpdatePowerManagementReg1: " ); Serial.println( AValue, BIN );
			WriteTo( MPU6050_PWR_MGMT_1, AValue );
		}

		void UpdatePowerManagementReg2()
		{
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
			
//#if defined( _MITOV_MPU_ACCELEROMETER_ ) || defined( _MITOV_MPU_GYROSCOPE_ )

//#if defined( _MITOV_MPU_ACCELEROMETER_ )
			AValue |=	( Accelerometer().GetIsEnabledX()	? 0 : 0b00100000 ) |
						( Accelerometer().GetIsEnabledY()	? 0 : 0b00010000 ) |
						( Accelerometer().GetIsEnabledZ()	? 0 : 0b00001000 );
//#else
//			AValue |=	0b00111000;
//#endif

//#if defined( _MITOV_MPU_GYROSCOPE_ )
			AValue |=	( Gyroscope().GetIsEnabledX()	? 0 : 0b00000100 ) |
						( Gyroscope().GetIsEnabledY()	? 0 : 0b00000010 ) |
						( Gyroscope().GetIsEnabledZ()	? 0 : 0b00000001 );
//#else
//			AValue |=	0b00000111;
//#endif

//#else
//			AValue |=	0b00111111;
//#endif
			WriteTo( MPU6050_PWR_MGMT_2, (uint8_t)AValue );
		}

/*
		void UpdateUserControlReg()
		{
			if( ! Compass.DirectAccess )
				WriteTo( MPU9250_RA_USER_CTRL, 0b00100000 ); // Enable master I2C mode
		}
*/
		void UpdateConfigReg()
		{
			uint8_t AValue =	Filter() |
//								( Queue.BlockOnFull ? 0b01000000 : 0 ) |
								FrameSynchronization().OutputPin().GetPinIsConnected() ? ( FrameSynchronization().Location() << 4 ) : 0;

			WriteTo( MPU6050_CONFIG, AValue );
		}

		void UpdateGyroConfigReg()
		{
			uint8_t AValue =	(( Gyroscope().FullScaleRange() & 0b11 ) << 3 ) |
						( Gyroscope().X().SelfTest() ? 0b10000000 : 0 ) |
						( Gyroscope().Y().SelfTest() ? 0b01000000 : 0 ) |
						( Gyroscope().Z().SelfTest() ? 0b00100000 : 0 );

			WriteTo( MPU6050_GYRO_CONFIG, AValue );
		}

		void UpdateAccelerometerConfigReg()
		{
			uint8_t AValue =	(( Accelerometer().FullScaleRange() & 0b11 ) << 3 ) |
							( Accelerometer().X().SelfTest() ? 0b10000000 : 0 ) |
							( Accelerometer().Y().SelfTest() ? 0b01000000 : 0 ) |
							( Accelerometer().Z().SelfTest() ? 0b00100000 : 0 );

			WriteTo( MPU6050_ACCEL_CONFIG, AValue );
		}

		void UpdatenterruptPinAndBypassConfigReg()
		{
			uint8_t AValue =	( Interrupt().Inverted()								?		0b10000000 : 0 ) |
							( Interrupt().OpenDrain()								?		0b01000000 : 0 ) |
							( Interrupt().Latch()									?		0b00100000 : 0 )
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

			WriteTo( MPU6050_INT_PIN_CFG, AValue );
		}

		inline void UpdatenterruptPinAndBypassConfigRegNormal()
		{
			UpdatenterruptPinAndBypassConfigReg();
		}

		void UpdateSampleRateDividerReg()
		{
			WriteTo( MPU6050_SMPLRT_DIV, SampleRateDivider() );
		}

		void UpdateFIFOReg()
		{
/*
			uint8_t AValue =	( Thermometer.Queue		?		0b10000000 : 0 ) |
							( Gyroscope.X.Queue		?		0b01000000 : 0 ) |
							( Gyroscope.Y.Queue		?		0b00100000 : 0 ) |
							( Gyroscope.Z.Queue		?		0b00010000 : 0 ) |
							( Accelerometer.Queue	?		0b00001000 : 0 ) |
							( Compass.Queue			?		0b00000001 : 0 );

			FUseFIFO = ( AValue != 0 );

			WriteTo( MPU9250_RA_FIFO_EN, AValue );
*/
		}

		void UpdateInteruptEnableReg()
		{
			uint8_t AValue =
//							( Interrupt.OnQueueOverflow	?	0b00010000 : 0 ) |
							( Interrupt().OnFrameSync()		?	0b00001000 : 0 ) |
							( Interrupt().OnRawReady()		?	0b00000001 : 0 );

			WriteTo( MPU6050_INT_ENABLE, AValue );
		}

	public:
		inline void SystemInit()
		{
			UpdatePowerManagementReg1();
			UpdatePowerManagementReg2();
			UpdateConfigReg();
			UpdateGyroConfigReg();
			UpdateAccelerometerConfigReg();
			UpdateSampleRateDividerReg();
			UpdatenterruptPinAndBypassConfigReg();
//			UpdateUserControlReg();
//			UpdatenterruptPinAndBypassConfigReg( Compass.DirectAccess && ! Compass.Queue );
			UpdateFIFOReg();
			UpdateInteruptEnableReg();
		}

		inline void SystemStart() {}

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
					if( ReadBytes( MPU6050_INT_STATUS, 1, &AIntValue ) )
//					if( Queue.OverflowOutputPin.IsConnected() || MotionDetect.OutputPin.IsConnected() || FrameSynchronization.OutputPin.IsConnected() )
					{
//						if( AIntValue & 0x00010000 )
//							Queue.OverflowOutputPin.Notify( nullptr );

//						if( AIntValue & 0x10000000 )
						if( AIntValue & 0x00001000 )
							T_FrameSynchronization::ClockPin();

					}
				}
			}
		}

	public:
		inline MPU6050I2C()
		{
			ClockInputPin_o_IsConnected() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Accelerometer,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ClockSource,
		typename T_Enabled,
		typename T_Filter,
		typename T_FrameSynchronization,
		typename T_Gyroscope,
		typename T_Interrupt,
		typename T_LowPowerMode,
		typename T_SampleRateDivider,
		typename T_Thermometer
	> class MPU6050I2CBuiltin : 
		public MPU6050I2C<
				TwoWire, Wire,
				T_Accelerometer,
				T_Address,
				T_ClockInputPin_o_IsConnected,
				T_ClockSource,
				T_Enabled,
				T_Filter,
				T_FrameSynchronization,
				T_Gyroscope,
				T_Interrupt,
				T_LowPowerMode,
				T_SampleRateDivider,
				T_Thermometer
			>
	{
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif