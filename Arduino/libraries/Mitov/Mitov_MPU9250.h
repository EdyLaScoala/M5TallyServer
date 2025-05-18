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
#include <Mitov_Compass_AK8963.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __MPU9250_DEBUG_WRITE__
// #define __MPU9250_DEBUG_READ__

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace MPU9250Const
	{
		const uint8_t MPU9150_RA_MAG_ADDRESS	= 0x0C;

		const uint8_t MPU9150_RA_MAG_ST1		= 0x02;
		const uint8_t MPU9150_RA_MAG_XOUT_L		= 0x03;
		const uint8_t MPU9150_RA_MAG_XOUT_H		= 0x04;
		const uint8_t MPU9150_RA_MAG_YOUT_L		= 0x05;
		const uint8_t MPU9150_RA_MAG_YOUT_H		= 0x06;
		const uint8_t MPU9150_RA_MAG_ZOUT_L		= 0x07;
		const uint8_t MPU9150_RA_MAG_ZOUT_H		= 0x08;
		const uint8_t MPU9150_RA_MAG_ST2		= 0x09;
		const uint8_t MPU9150_RA_MAG_CNTL1		= 0x0A;
		const uint8_t MPU9150_RA_MAG_CNTL2		= 0x0B;
		const uint8_t MPU9150_RA_MAG_ASTC		= 0x0C;

		const uint8_t MPU9150_RA_MAG_ASAX		= 0x10;
		const uint8_t MPU9150_RA_MAG_ASAY		= 0x11;
		const uint8_t MPU9150_RA_MAG_ASAZ		= 0x12;
	};
//---------------------------------------------------------------------------
	template <
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Standby,
		typename T_Units,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class MPU9250Gyroscope : // public MPUOptionalAxesSensor
		public T_FullScaleRange,
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
		_V_PROP_( FullScaleRange )
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
		typename T_CompareWithPreviousSample,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Threshold
	> class MPU9250MotionDetect : // : public MPUOptionalSensor
		public T_CompareWithPreviousSample,
		public T_Enabled,
		public T_OutputPin,
		public T_Threshold
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( CompareWithPreviousSample )
		_V_PROP_( Enabled )
		_V_PROP_( Threshold )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Filter,
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class MPU9250Accelerometer :
		public T_Filter,
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
		_V_PROP_( Filter )
		_V_PROP_( FullScaleRange )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	public:
		inline bool GetIsEnabledX() { return ( X().Enabled() && OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( Y().Enabled() && OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( Z().Enabled() && OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }

	};
//---------------------------------------------------------------------------
	template <
//		typename T_I2C, T_I2C &C_I2C,
//		uint8_t C_ADDRESS,
		typename T_DirectAccess,
		typename T_Enabled,
		typename T_HighResolution,
		typename T_Mode,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_OverflowOutputPin,
		typename T_SelfTest
	> class MPU9250Compass : // : public MPUBasicSensor
		public T_DirectAccess,
		public T_Enabled,
		public T_HighResolution,
		public T_Mode,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_OverflowOutputPin,
		public T_SelfTest
	{
	public:
		_V_PROP_( Mode )
		_V_PROP_( Enabled )
		_V_PROP_( SelfTest ) // Added to save space as bitfield
		_V_PROP_( HighResolution )
		_V_PROP_( DirectAccess )
//		bool Queue : 1;

	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

		_V_PIN_( OverflowOutputPin )

	};
//---------------------------------------------------------------------------
	enum TArduinoMPU9250GyroscopeThermometerFilter
	{
		gtf_GB_8800Hz_GF_32KHz_TB_4000Hz,
		gtf_GB_3600Hz_GF_32KHz_TB_4000Hz,
		gtf_GB_250Hz_GF_8KHz_TB_4000Hz,
		gtf_GB_184Hz_GF_1KHz_TB_188Hz,
		gtf_GB_92Hz_GF_1KHz_TB_98Hz,
		gtf_GB_41Hz_GF_1KHz_TB_42Hz,
		gtf_GB_20Hz_GF_1KHz_TB_20Hz,
		gtf_GB_10Hz_GF_1KHz_TB_10Hz,
		gtf_GB_5Hz_GF_1KHz_TB_5Hz,
		gtf_GB_3600Hz_GF_8KHz_TB_4000Hz
	};
//---------------------------------------------------------------------------
	template <
		typename T_Inverted,
		typename T_Latch,
		typename T_OnFrameSync,
		typename T_OnMotion,
		typename T_OnRawReady,
		typename T_OpenDrain
	> class MPU9250Interrupt :
		public T_Inverted,
		public T_Latch,
		public T_OnFrameSync,
		public T_OnMotion,
		public T_OnRawReady,
		public T_OpenDrain
	{
	public:
		_V_PROP_( Inverted )
		_V_PROP_( OpenDrain )
		_V_PROP_( Latch )
		_V_PROP_( OnMotion )
//		_V_PROP_( OnQueueOverflow )
		_V_PROP_( OnFrameSync )
		_V_PROP_( OnRawReady )
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Accelerometer,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ClockSource,
		typename T_Compass,
		typename T_Enabled,
		typename T_FrameSynchronization,
		typename T_Gyroscope,
		typename T_GyroscopeThermometerFilter,
		typename T_Interrupt,
		typename T_LowPowerMode,
		typename T_MotionDetect,
		typename T_SampleRateDivider,
		typename T_Thermometer
	> class MPU9250I2C :
		public T_Accelerometer,
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_ClockSource,
		public T_Compass,
		public T_Enabled,
		public T_FrameSynchronization,
		public T_Gyroscope,
		public T_GyroscopeThermometerFilter,
		public T_Interrupt,
		public T_LowPowerMode,
		public T_MotionDetect,
		public T_SampleRateDivider,
		public T_Thermometer
	{
	protected:
		static const uint8_t MPU9250_RA_SMPLRT_DIV			= 0x19;
		static const uint8_t MPU9250_RA_CONFIG				= 0x1A;
		static const uint8_t MPU9250_RA_GYRO_CONFIG			= 0x1B;
		static const uint8_t MPU9250_RA_ACCEL_CONFIG		= 0x1C;
		static const uint8_t MPU9250_RA_ACCEL_CONFIG_2		= 0x1D;
		static const uint8_t MPU9250_RA_LP_ACCEL_ODR		= 0x1E;

		static const uint8_t MPU9250_RA_MOT_THR				= 0x1F;

		static const uint8_t MPU9250_RA_FIFO_EN				= 0x23;
		static const uint8_t MPU9250_RA_I2C_MST_CTRL		= 0x24;
		static const uint8_t MPU9250_RA_I2C_SLV0_ADDR		= 0x25;
		static const uint8_t MPU9250_RA_I2C_SLV0_REG		= 0x26;
		static const uint8_t MPU9250_RA_I2C_SLV0_CTRL		= 0x27;
		static const uint8_t MPU9250_RA_I2C_SLV1_ADDR		= 0x28;
		static const uint8_t MPU9250_RA_I2C_SLV1_REG		= 0x29;
		static const uint8_t MPU9250_RA_I2C_SLV1_CTRL		= 0x2A;
		static const uint8_t MPU9250_RA_I2C_SLV2_ADDR		= 0x2B;
		static const uint8_t MPU9250_RA_I2C_SLV2_REG		= 0x2C;
		static const uint8_t MPU9250_RA_I2C_SLV2_CTRL		= 0x2D;
		static const uint8_t MPU9250_RA_I2C_SLV3_ADDR		= 0x2E;
		static const uint8_t MPU9250_RA_I2C_SLV3_REG		= 0x2F;
		static const uint8_t MPU9250_RA_I2C_SLV3_CTRL		= 0x30;
		static const uint8_t MPU9250_RA_I2C_SLV4_ADDR		= 0x31;
		static const uint8_t MPU9250_RA_I2C_SLV4_REG		= 0x32;
		static const uint8_t MPU9250_RA_I2C_SLV4_DO			= 0x33;
		static const uint8_t MPU9250_RA_I2C_SLV4_CTRL		= 0x34;
		static const uint8_t MPU9250_RA_I2C_SLV4_DI			= 0x35;
		static const uint8_t MPU9250_RA_I2C_MST_STATUS		= 0x36;
		static const uint8_t MPU9250_RA_INT_PIN_CFG			= 0x37;
		static const uint8_t MPU9250_RA_INT_ENABLE			= 0x38;
		static const uint8_t MPU9250_RA_DMP_INT_STATUS		= 0x39;
		static const uint8_t MPU9250_RA_INT_STATUS			= 0x3A;

		static const uint8_t MPU9250_RA_ACCEL_XOUT_H		= 0x3B;
		static const uint8_t MPU9250_RA_ACCEL_XOUT_L		= 0x3C;
		static const uint8_t MPU9250_RA_ACCEL_YOUT_H		= 0x3D;
		static const uint8_t MPU9250_RA_ACCEL_YOUT_L		= 0x3E;
		static const uint8_t MPU9250_RA_ACCEL_ZOUT_H		= 0x3F;
		static const uint8_t MPU9250_RA_ACCEL_ZOUT_L		= 0x40;

		static const uint8_t MPU9250_RA_EXT_SENS_DATA_00	= 0x49;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_01	= 0x4A;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_02	= 0x4B;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_03	= 0x4C;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_04	= 0x4D;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_05	= 0x4E;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_06	= 0x4F;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_07	= 0x50;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_08	= 0x51;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_09	= 0x52;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_10	= 0x53;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_11	= 0x54;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_12	= 0x55;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_13	= 0x56;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_14	= 0x57;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_15	= 0x58;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_16	= 0x59;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_17	= 0x5A;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_18	= 0x5B;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_19	= 0x5C;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_20	= 0x5D;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_21	= 0x5E;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_22	= 0x5F;
		static const uint8_t MPU9250_RA_EXT_SENS_DATA_23	= 0x60;

		static const uint8_t MPU9250_RA_MOT_DETECT_CTRL		= 0x69;
		static const uint8_t MPU9250_RA_USER_CTRL			= 0x6A;
		static const uint8_t MPU9250_RA_PWR_MGMT_1			= 0x6B;
		static const uint8_t MPU9250_RA_PWR_MGMT_2			= 0x6C;

		static const uint8_t MPU9250_RA_FIFO_COUNTH			= 0x72;
		static const uint8_t MPU9250_RA_FIFO_COUNTL			= 0x73;
		static const uint8_t MPU9250_RA_FIFO_R_W			= 0x74;

	public:
		_V_PROP_( Accelerometer )
		_V_PROP_( Gyroscope )
//		MPU9250Compass<MPU9250Const::MPU9150_RA_MAG_ADDRESS, T_I2C, C_I2C>	Compass;
		_V_PROP_( Compass )
		_V_PROP_( Thermometer )
		_V_PROP_( FrameSynchronization )
//		MPU9250Queue				Queue;
		_V_PROP_( Interrupt )
		_V_PROP_( MotionDetect )
		_V_PROP_( LowPowerMode )

	protected:
		float CompassAdjustmentValues[ 3 ];

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( ClockSource )
		_V_PROP_( GyroscopeThermometerFilter )

		_V_PROP_( SampleRateDivider )

	public:
		inline void UpdateClockSource()
		{
			UpdatePowerManagementReg1( false );
		}

		void SetCompassDirectAccess( bool AOldValueValue, bool AValue )
		{
			if( AValue == AOldValueValue )
				return;

			Compass().DirectAccess() = AValue;

			UpdateCompassSlaveI2CReg();
			UpdateUserControlReg();
			UpdatenterruptPinAndBypassConfigRegNormal();
		}

//	protected:
//		bool	FUseFIFO : 1;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			UpdatePowerManagementReg1( true );
		}

		inline void Compass_ResetInputPin_o_Receive( void *_Data )
		{
			I2C::WriteByte( C_I2C, MPU9250Const::MPU9150_RA_MAG_ADDRESS, MPU9250Const::MPU9150_RA_MAG_CNTL2, 0b00000001 );
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

			const float CompassCoefficients[] =
			{
				10.0f *4219.0f / 8190.0f,
				10.0f *4219.0f / 32760.0f
			};

			uint8_t AIntValues[ 7 * 2 ];

			if( ReadBytes( MPU9250_RA_ACCEL_XOUT_H, sizeof( AIntValues ), AIntValues ))
			{
				for( int i = 0; i < 3; ++i )
				{
					int16_t AIntValue = ( int16_t( int8_t( AIntValues[ i * 2 ] )) << 8 ) | AIntValues[ i * 2 + 1 ];
					float AValue = AIntValue * AccelerometerCoefficients[ Accelerometer().FullScaleRange() ];
//					Serial.print( i );	Serial.print( " - " );	Serial.print( AIntValue ); Serial.print( " - " );	Serial.println( AValue );
					switch( i )
					{
						case 0: Accelerometer().OutputPinsX().SetPinValue( AValue ); break;
						case 1: Accelerometer().OutputPinsY().SetPinValue( AValue ); break;
						case 2: Accelerometer().OutputPinsZ().SetPinValue( AValue ); break;
					}
				}

				for( int i = 0; i < 3; ++i )
				{
					int16_t AIntValue = ( int16_t( int8_t( AIntValues[ ( i + 4 ) * 2 ] )) << 8 ) | AIntValues[ ( i + 4 ) * 2 + 1 ];
					float AValue = AIntValue * GyroscopeCoefficients[ Gyroscope().FullScaleRange() ] * GyroscopeUnits[ Gyroscope().Units() ];
					switch( i )
					{
						case 0: Gyroscope().OutputPinsX().SetPinValue( AValue ); break;
						case 1: Gyroscope().OutputPinsY().SetPinValue( AValue ); break;
						case 2: Gyroscope().OutputPinsZ().SetPinValue( AValue ); break;
					}
				}

				int16_t AIntValue = ( int16_t( int8_t(  AIntValues[ 3 * 2 ] )) << 8 ) | AIntValues[ 3 * 2 + 1 ];
				float AValue = AIntValue / 333.87 + 21.0;
				if( Thermometer().InFahrenheit() )
					AValue = Func::ConvertCtoF( AValue );

				Thermometer().OutputPin().SetPinValue( AValue );
			}

/*
			if( Compass.Queue )
			{
				if( ReadBytes( MPU9250_RA_FIFO_R_W, 1, AIntValues ))
				{
					Compass.OverflowOutputPin.SendValue( AIntValues[ 0 ] & 0b00000010 );
					if( AIntValues[ 0 ] & 0b00000001 )
						for( int i = 0; i < 3; ++i )
							if( ReadBytes( MPU9250_RA_FIFO_R_W, 1, AIntValues ))
								if( ReadBytes( MPU9250_RA_FIFO_R_W, 1, AIntValues + 1 ))
								{
										float AValue = (( int16_t( int8_t( AIntValues[ i * 2 + 1 ] ) ) << 8 ) | AIntValues[ i * 2 ] ) * CompassCoefficients[ Compass.HighResolution & 1 ] * CompassAdjustmentValues[ i ];
										Compass.OutputPins[ i ].Notify( &AValue );
								}


				}
			}

			else
*/
			if( Compass().DirectAccess() )
			{
				if( I2C::ReadBytes( C_I2C, MPU9250Const::MPU9150_RA_MAG_ADDRESS, MPU9250Const::MPU9150_RA_MAG_ST1, 1, AIntValues ))
				{
					if( AIntValues[ 0 ] & 0b00000010 )
						Compass().OverflowOutputPin().ClockPin();

					if( AIntValues[ 0 ] & 0b00000001 )
						if( I2C::ReadBytes( C_I2C, MPU9250Const::MPU9150_RA_MAG_ADDRESS, MPU9250Const::MPU9150_RA_MAG_XOUT_L, 7, AIntValues ))
							for( int i = 0; i < 3; ++i )
							{
								float AValue = (( int16_t( int8_t( AIntValues[ i * 2 + 1 ] )) << 8 ) | AIntValues[ i * 2 ] ) * CompassCoefficients[ Compass().HighResolution() & 1 ] * CompassAdjustmentValues[ i ];
								switch( i )
								{
									case 0: Compass().OutputPinsX().SetPinValue( AValue ); break;
									case 1: Compass().OutputPinsY().SetPinValue( AValue ); break;
									case 2: Compass().OutputPinsZ().SetPinValue( AValue ); break;
								}
//								Compass.OutputPins[ i ].Notify( &AValue );
							}

				}
			}
			else
			{
				if( ReadBytes( MPU9250_RA_EXT_SENS_DATA_00, 1, AIntValues ))
				{
	//				Serial.println( AIntValues[ 0 ], BIN );
					if( AIntValues[ 0 ] & 0b00000010 )
						Compass().OverflowOutputPin().ClockPin();

					if( AIntValues[ 0 ] & 0b00000001 )
						if( ReadBytes( MPU9250_RA_EXT_SENS_DATA_01, 7, AIntValues ))
							for( int i = 0; i < 3; ++i )
							{
								float AValue = (( int16_t( int8_t( AIntValues[ i * 2 + 1 ] )) << 8 ) | AIntValues[ i * 2 ] ) * CompassCoefficients[ Compass().HighResolution() & 1 ] * CompassAdjustmentValues[ i ];
								switch( i )
								{
									case 0: Compass().OutputPinsX().SetPinValue( AValue ); break;
									case 1: Compass().OutputPinsY().SetPinValue( AValue ); break;
									case 2: Compass().OutputPinsZ().SetPinValue( AValue ); break;
								}
//								Compass.OutputPins[ i ].Notify( &AValue );
							}

				}
			}
		}

	protected:
		inline void WriteTo( uint8_t ARegister, uint8_t AData )
		{
#ifdef __MPU9250_DEBUG_WRITE__
			Serial.print( "WRITE REG: " ); Serial.print( ARegister, HEX ); Serial.print( " = " ); Serial.println( AData, BIN );
#endif
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), ARegister, AData );
		}

		inline bool ReadBytes( uint8_t ARegister, uint8_t length, void *data )
		{
#ifdef __MPU9250_DEBUG_READ__
			Serial.print( "READ REG: " ); Serial.println( ARegister, HEX );
#endif
			return I2C::ReadBytes( C_I2C, uint8_t( Address().GetValue()), ARegister, length, data );
		}

	public:
		void UpdatePowerManagementReg1( bool AReset = false )
		{
			uint8_t AValue;
			if( ClockSource() == mcsAutoSelect )
			{
				if( Gyroscope().X().Enabled() && Gyroscope().OutputPinsX().GetPinIsConnected() )
					AValue = mcsGyroX;

				else if( Gyroscope().Y().Enabled() && Gyroscope().OutputPinsY().GetPinIsConnected() )
					AValue = mcsGyroY;

				else if( Gyroscope().Z().Enabled() && Gyroscope().OutputPinsZ().GetPinIsConnected() )
					AValue = mcsGyroZ;

				else
					AValue = mcsInternal;
			}

			else
				AValue = ClockSource();

			AValue |=	( AReset				?		0b10000000 : 0 ) |
						( Enabled()				? 0 :	0b01000000 ) |
						( LowPowerMode().Enabled()	?       0b00100000 : 0 ) |
						( Gyroscope().Standby()		?		0b00010000 : 0 ) |
						( Thermometer().GetIsEnabled()	? 0 :	0b00001000 );

//			Serial.print( "UpdatePowerManagementReg1: " ); Serial.println( AValue, BIN );
//			AValue = 0b10000001;
			WriteTo( MPU9250_RA_PWR_MGMT_1, AValue );
		}

		void UpdatePowerManagementReg2()
		{
			uint8_t AValue =	( Accelerometer().GetIsEnabledX()	? 0 : 0b00100000 ) |
								( Accelerometer().GetIsEnabledY()	? 0 : 0b00010000 ) |
								( Accelerometer().GetIsEnabledZ()	? 0 : 0b00001000 ) |

								( Gyroscope().GetIsEnabledX()		? 0 : 0b00000100 ) |
								( Gyroscope().GetIsEnabledY()		? 0 : 0b00000010 ) |
								( Gyroscope().GetIsEnabledZ()		? 0 : 0b00000001 );

//			AValue = 0;
			WriteTo( MPU9250_RA_PWR_MGMT_2, AValue );
		}

		void UpdateUserControlReg()
		{
			if( ! Compass().DirectAccess() )
				WriteTo( MPU9250_RA_USER_CTRL, 0b00100000 ); // Enable master I2C mode
		}

		void UpdateConfigReg()
		{
			uint8_t AValue =	(( GyroscopeThermometerFilter() - 2 ) & 0b111 ) |
//								( Queue.BlockOnFull ? 0b01000000 : 0 ) |
								( FrameSynchronization().Location() << 4 );

			WriteTo( MPU9250_RA_CONFIG, AValue );
		}

		void UpdateGyroConfigReg()
		{
			uint8_t AValue;
			switch( GyroscopeThermometerFilter() )
			{
				case gtf_GB_8800Hz_GF_32KHz_TB_4000Hz: AValue = 0b11; break;
				case gtf_GB_3600Hz_GF_32KHz_TB_4000Hz: AValue = 0b10; break;
				default : AValue = 0b00;
			}

			AValue |=	(( Gyroscope().FullScaleRange() & 0b11 ) << 3 ) |
						( Gyroscope().X().SelfTest() ? 0b10000000 : 0 ) |
						( Gyroscope().Y().SelfTest() ? 0b01000000 : 0 ) |
						( Gyroscope().Z().SelfTest() ? 0b00100000 : 0 );

			WriteTo( MPU9250_RA_GYRO_CONFIG, AValue );
		}

		void UpdateAccelerometerConfigReg()
		{
			uint8_t AValue =	(( Accelerometer().FullScaleRange() & 0b11 ) << 3 ) |
							( Accelerometer().X().SelfTest() ? 0b10000000 : 0 ) |
							( Accelerometer().Y().SelfTest() ? 0b01000000 : 0 ) |
							( Accelerometer().Z().SelfTest() ? 0b00100000 : 0 );

			WriteTo( MPU9250_RA_ACCEL_CONFIG, AValue );
		}

		void UpdateAccelerometerConfig2Reg()
		{
			const uint8_t Values[] =
			{
				0b00001000,
				0,
				1,
				2,
				3,
				4,
				5,
				6
			};

			uint8_t AValue = Values[ Accelerometer().Filter() ];

			WriteTo( MPU9250_RA_ACCEL_CONFIG_2, AValue );
		}

		void UpdatenterruptPinAndBypassConfigReg( bool ADirectCompassAccess )
		{
			uint8_t AValue =	( Interrupt().Inverted()								?		0b10000000 : 0 ) |
							( Interrupt().OpenDrain()								?		0b01000000 : 0 ) |
							( Interrupt().Latch()									?		0b00100000 : 0 ) |
							( FrameSynchronization().InterruptOnLowLevel()			?		0b00001000 : 0 ) |
							( FrameSynchronization().EnableInterrupt()				?		0b00000100 : 0 ) |
							( /*Compass.Queue & */ ( ! ADirectCompassAccess )		? 0 :	0b00000010 );

//			Serial.print( "UpdatenterruptPinAndBypassConfigReg: " ); Serial.println( AValue, BIN );

			WriteTo( MPU9250_RA_INT_PIN_CFG, AValue );
		}

		void UpdatenterruptPinAndBypassConfigRegNormal()
		{
			UpdatenterruptPinAndBypassConfigReg( Compass().DirectAccess() );
		}

		void UpdateSampleRateDividerReg()
		{
			WriteTo( MPU9250_RA_SMPLRT_DIV, SampleRateDivider().GetValue() - 1 );
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
			uint8_t AValue =	( Interrupt().OnMotion()		?	0b01000000 : 0 ) |
//							( Interrupt().OnQueueOverflow()	?	0b00010000 : 0 ) |
							( Interrupt().OnFrameSync()		?	0b00001000 : 0 ) |
							( Interrupt().OnRawReady()		?	0b00000001 : 0 );

			WriteTo( MPU9250_RA_INT_ENABLE, AValue );
		}

		void UpdateMotionThresholdReg()
		{
			uint8_t AValue = MotionDetect().Threshold() * 255 / 1020 + 0.5;
			WriteTo( MPU9250_RA_MOT_THR, AValue );
		}

		void UpdateMotionDetectReg()
		{
			uint8_t AValue =	( MotionDetect().Enabled()						?	0b10000000 : 0 ) |
							( MotionDetect().CompareWithPreviousSample()	?	0b01000000 : 0 );

			WriteTo( MPU9250_RA_MOT_DETECT_CTRL, AValue );
		}


		void UpdateAccelerometerODRControlReg()
		{
			const float CLevels[] =
			{
				0.24,
				0.49,
				0.98,
				1.95,
				3.91,
				7.81,
				15.63,
				31.25,
				62.50,
				125,
				250,
				500
			};

			int AValue = -1;
			for( int i = 0; i < sizeof( CLevels ) / sizeof( CLevels[ 0 ] ); ++i )
				if( LowPowerMode().SampleFrequency() <= CLevels[ i ] )
				{
					AValue = i;
					break;
				}

			if( AValue == -1 )
				AValue = 11;

			WriteTo( MPU9250_RA_LP_ACCEL_ODR, AValue );
		}


		void UpdateCompassSlaveI2CReg()
		{
//			WriteTo( MPU9250_RA_I2C_MST_CTRL, 0b10000000 );
			WriteTo( MPU9250_RA_I2C_SLV0_ADDR, 0b10000000 | MPU9250Const::MPU9150_RA_MAG_ADDRESS );
			WriteTo( MPU9250_RA_I2C_SLV0_REG, MPU9250Const::MPU9150_RA_MAG_ST1 );
//			WriteTo( MPU9250_RA_I2C_SLV0_CTRL, ( ( ! Compass.DirectAccess ) || Compass.Queue ) ? 0b10000000 | 8 : 0 );
			WriteTo( MPU9250_RA_I2C_SLV0_CTRL, ( ! Compass().DirectAccess() ) ? 0b10000000 | 8 : 0 );
		}

		void UpdateCompassControlReg()
		{
			uint8_t AValue;
			if( Compass().Enabled() )
				AValue = CompassAK8963Const::CompassModes[ Compass().Mode() ];

			else
				AValue = 0;

			AValue |=	( Compass().HighResolution()	? 0b00010000 : 0 );


//			Serial.println( "COMPASS" );
//			Serial.println( AValue );

//			Serial.println( Compass().Mode() );
//			Serial.println( Compass().HighResolution() );

			I2C::WriteByte( C_I2C, MPU9250Const::MPU9150_RA_MAG_ADDRESS, MPU9250Const::MPU9150_RA_MAG_CNTL1, AValue );
		}

		void UpdateCompassSelfTestReg()
		{
			uint8_t AValue = ( Compass().SelfTest()	? 0b01000000 : 0 );
			I2C::WriteByte( C_I2C, MPU9250Const::MPU9150_RA_MAG_ADDRESS, MPU9250Const::MPU9150_RA_MAG_ASTC, AValue );
		}

		void ReadCompassAdjustmentValues()
		{
			uint8_t AValues[ 3 ];

			I2C::ReadBytes( C_I2C, MPU9250Const::MPU9150_RA_MAG_ADDRESS, MPU9250Const::MPU9150_RA_MAG_ASAX, sizeof( AValues ), AValues );
			for( int i = 0; i < 3; ++i )
				CompassAdjustmentValues[ i ] = (((float) AValues[ i ] ) - 128.0f) / 256.0f + 1.0f;

		}

	public:
		inline void SystemStart()
		{
			UpdatePowerManagementReg1( true );
			UpdatePowerManagementReg2();
			UpdateConfigReg();
			UpdateGyroConfigReg();
			UpdateAccelerometerConfigReg();
			UpdateAccelerometerConfig2Reg();
			UpdateSampleRateDividerReg();
			UpdatenterruptPinAndBypassConfigReg( true );
			UpdateCompassControlReg();
			UpdateCompassSlaveI2CReg();
			UpdateCompassSelfTestReg();
			UpdateUserControlReg();
			ReadCompassAdjustmentValues();
//			UpdatenterruptPinAndBypassConfigReg( Compass.DirectAccess && ! Compass.Queue );
			UpdatenterruptPinAndBypassConfigRegNormal();
			UpdateFIFOReg();
			UpdateMotionThresholdReg();
			UpdateMotionDetectReg();
			UpdateAccelerometerODRControlReg();
			UpdateInteruptEnableReg();
		}

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
				uint8_t AIntValue;
				if( ReadBytes( MPU9250_RA_INT_STATUS, 1, &AIntValue ) )
//				if( Queue.OverflowOutputPin.IsConnected() || MotionDetect.OutputPin.IsConnected() || FrameSynchronization.OutputPin.IsConnected() )
				{
					if( AIntValue & 0x01000000 )
						MotionDetect().OutputPin().ClockPin();

//					if( AIntValue & 0x00010000 )
//						Queue.OverflowOutputPin.Notify( nullptr );

					if( AIntValue & 0x00001000 )
						FrameSynchronization().OutputPin().ClockPin();

				}
			}
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif