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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace TArduinoBNO0Mode
	{
		typedef enum
		{
			Accelerometer,
			Compass,
			Gyroscope,
			AccelerometerCompass,
			AccelerometerGyroscope,
			GyroscopeCompass,
			AccelerometerGyroscopeCompass,

			FusionAccelerometerGyroscopeRelative,
			FusionAccelerometerCompassAbsolute,
			FusionAccelerometerCompassRelative,

			FusionFull,
			FusionFullCalibrated,

			Auto
		} TArduinoBNO0Mode;

	}
//---------------------------------------------------------------------------
	namespace TArduinoBNO0PowerMode
	{
		typedef enum
		{
			Normal,
			LowPower,
			Suspend
		} TArduinoBNO0PowerMode;
	}
//---------------------------------------------------------------------------
	namespace TArduinoBNO0AxisOrientation
	{
		typedef enum
		{
			X = 0b000,
			Y = 0b001,
			Z = 0b010,

			InvertedX = 0b100,
			InvertedY = 0b101,
			InvertedZ = 0b110
		} TArduinoBNO0AxisOrientation;
	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FromGyroscope,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class TArduinoBNO055Thermometer :
		public T_Enabled,
		public T_FromGyroscope,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( FromGyroscope )
		_V_PROP_( InFahrenheit )

	};
//---------------------------------------------------------------------------
	namespace TArduinoBNO0AccelerometerMode
	{
		typedef enum
		{
			Normal,
			Suspend,
			LowPower1,
			Standby,
			LowPower2,
			DeepSuspend
		} TArduinoBNO0AccelerometerMode;
	}
//---------------------------------------------------------------------------
	template <
		typename T_Bandwidth,
		typename T_Enabled,
		typename T_Mode,
		typename T_Offsets,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Range
	> class TArduinoBNO055Accelerometer :
		public T_Bandwidth,
		public T_Enabled,
		public T_Mode,
		public T_Offsets,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Range
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Range )
		_V_PROP_( Mode )
		_V_PROP_( Enabled )

		_V_PROP_( Bandwidth )

//#ifdef _MITOV_BNO055_ACCELEROMETER_OFFSETS_
		_V_PROP_( Offsets )
//#endif

	};
//---------------------------------------------------------------------------
	namespace TArduinoBNO0GyroscopeMode
	{
		typedef enum
		{
			Normal,
			FastPowerUp,
			DeepSuspend,
			Suspend,
			AdvancedPowersave
		} TArduinoBNO0GyroscopeMode;
	}
//---------------------------------------------------------------------------
	template <
		typename T_Bandwidth,
		typename T_Enabled,
		typename T_Mode,
		typename T_Offsets,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Range
	> class TArduinoBNO055Gyroscope :
		public T_Bandwidth,
		public T_Enabled,
		public T_Mode,
		public T_Offsets,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Range
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Range )
		_V_PROP_( Bandwidth )
		_V_PROP_( Mode )
		_V_PROP_( Enabled )

//#ifdef _MITOV_BNO055_GYROSCOPE_OFFSETS_
		_V_PROP_( Offsets )
//#endif

	};
//---------------------------------------------------------------------------
	namespace TArduinoBNO055CompassOperationMode
	{
		typedef enum
		{
			LowPower,
			Regular,
			EnhancedRegular,
			HighAccuracy
		} TArduinoBNO055CompassOperationMode;
	}
//---------------------------------------------------------------------------
	namespace TArduinoBNO055CompassPowerMode
	{
		typedef enum
		{
			Normal,
			Sleep,
			Suspend,
			ForceMode
		} TArduinoBNO055CompassPowerMode;
	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Offsets,
		typename T_OperationMode,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PowerMode,
		typename T_Rate
	> class TArduinoBNO055Compass :
		public T_Enabled,
		public T_Offsets,
		public T_OperationMode,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_PowerMode,
		public T_Rate
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Rate )
		_V_PROP_( OperationMode )
		_V_PROP_( PowerMode )
		_V_PROP_( Enabled )

//#ifdef _MITOV_BNO055_COMPASS_OFFSETS_
		_V_PROP_( Offsets )
//#endif

	};
//---------------------------------------------------------------------------
	template <
		typename T_Calibration,
		typename T_Enabled,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ
//		typename T_Relative
	> class TArduinoBNO055Fused :
		public T_Calibration,
		public T_Enabled,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ
//		public T_Relative
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Calibration )

	};
//---------------------------------------------------------------------------
	template <
		typename T_InvertZ,
		typename T_X,
		typename T_Y
	> class TArduinoBNO055Orientation :
		public T_InvertZ,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( InvertZ )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PositiveOnly,
		typename T_QuaternionOutputPin,
		typename T_Units
	> class TArduinoBNO055Rotation :
		public T_Enabled,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_PositiveOnly,
		public T_QuaternionOutputPin,
		public T_Units
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

		_V_PIN_( QuaternionOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Units )
		_V_PROP_( PositiveOnly )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Accelerometer,
		typename T_Address,
		typename T_Compass,
		typename T_Enabled,
		typename T_ExternalOscillator,
		typename T_Fused,
		typename T_Gyroscope,
		typename T_Mode,
		typename T_Orientation,
		typename T_PowerMode,
		typename T_Rotation,
		typename T_Thermometer
	> class BNO055I2C :
		public T_Accelerometer,
		public T_Address,
		public T_Compass,
		public T_Enabled,
		public T_ExternalOscillator,
		public T_Fused,
		public T_Gyroscope,
		public T_Mode,
		public T_Orientation,
		public T_PowerMode,
		public T_Rotation,
		public T_Thermometer
	{
	protected:
		static const uint8_t BNO055_ID = 0xA0;

    enum ERegisters
    {
      // Page id register definition
      BNO055_PAGE_ID_ADDR                                     = 0X07,

      // PAGE0 REGISTER DEFINITION START
      BNO055_CHIP_ID_ADDR                                     = 0x00,
      BNO055_ACCEL_REV_ID_ADDR                                = 0x01,
      BNO055_MAG_REV_ID_ADDR                                  = 0x02,
      BNO055_GYRO_REV_ID_ADDR                                 = 0x03,
      BNO055_SW_REV_ID_LSB_ADDR                               = 0x04,
      BNO055_SW_REV_ID_MSB_ADDR                               = 0x05,
      BNO055_BL_REV_ID_ADDR                                   = 0X06,

      // Accel data register
      BNO055_ACCEL_DATA_X_LSB_ADDR                            = 0X08,
      BNO055_ACCEL_DATA_X_MSB_ADDR                            = 0X09,
      BNO055_ACCEL_DATA_Y_LSB_ADDR                            = 0X0A,
      BNO055_ACCEL_DATA_Y_MSB_ADDR                            = 0X0B,
      BNO055_ACCEL_DATA_Z_LSB_ADDR                            = 0X0C,
      BNO055_ACCEL_DATA_Z_MSB_ADDR                            = 0X0D,

      // Mag data register
      BNO055_MAG_DATA_X_LSB_ADDR                              = 0X0E,
      BNO055_MAG_DATA_X_MSB_ADDR                              = 0X0F,
      BNO055_MAG_DATA_Y_LSB_ADDR                              = 0X10,
      BNO055_MAG_DATA_Y_MSB_ADDR                              = 0X11,
      BNO055_MAG_DATA_Z_LSB_ADDR                              = 0X12,
      BNO055_MAG_DATA_Z_MSB_ADDR                              = 0X13,

      // Gyro data registers
      BNO055_GYRO_DATA_X_LSB_ADDR                             = 0X14,
      BNO055_GYRO_DATA_X_MSB_ADDR                             = 0X15,
      BNO055_GYRO_DATA_Y_LSB_ADDR                             = 0X16,
      BNO055_GYRO_DATA_Y_MSB_ADDR                             = 0X17,
      BNO055_GYRO_DATA_Z_LSB_ADDR                             = 0X18,
      BNO055_GYRO_DATA_Z_MSB_ADDR                             = 0X19,

      // Euler data registers
      BNO055_EULER_H_LSB_ADDR                                 = 0X1A,
      BNO055_EULER_H_MSB_ADDR                                 = 0X1B,
      BNO055_EULER_R_LSB_ADDR                                 = 0X1C,
      BNO055_EULER_R_MSB_ADDR                                 = 0X1D,
      BNO055_EULER_P_LSB_ADDR                                 = 0X1E,
      BNO055_EULER_P_MSB_ADDR                                 = 0X1F,

      // Quaternion data registers
      BNO055_QUATERNION_DATA_W_LSB_ADDR                       = 0X20,
      BNO055_QUATERNION_DATA_W_MSB_ADDR                       = 0X21,
      BNO055_QUATERNION_DATA_X_LSB_ADDR                       = 0X22,
      BNO055_QUATERNION_DATA_X_MSB_ADDR                       = 0X23,
      BNO055_QUATERNION_DATA_Y_LSB_ADDR                       = 0X24,
      BNO055_QUATERNION_DATA_Y_MSB_ADDR                       = 0X25,
      BNO055_QUATERNION_DATA_Z_LSB_ADDR                       = 0X26,
      BNO055_QUATERNION_DATA_Z_MSB_ADDR                       = 0X27,

      // Linear acceleration data registers
      BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR                     = 0X28,
      BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR                     = 0X29,
      BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR                     = 0X2A,
      BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR                     = 0X2B,
      BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR                     = 0X2C,
      BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR                     = 0X2D,

      // Gravity data registers
      BNO055_GRAVITY_DATA_X_LSB_ADDR                          = 0X2E,
      BNO055_GRAVITY_DATA_X_MSB_ADDR                          = 0X2F,
      BNO055_GRAVITY_DATA_Y_LSB_ADDR                          = 0X30,
      BNO055_GRAVITY_DATA_Y_MSB_ADDR                          = 0X31,
      BNO055_GRAVITY_DATA_Z_LSB_ADDR                          = 0X32,
      BNO055_GRAVITY_DATA_Z_MSB_ADDR                          = 0X33,

      // Temperature data register
      BNO055_TEMP_ADDR                                        = 0X34,

      // Status registers
      BNO055_CALIB_STAT_ADDR                                  = 0X35,
      BNO055_SELFTEST_RESULT_ADDR                             = 0X36,
      BNO055_INTR_STAT_ADDR                                   = 0X37,

      BNO055_SYS_CLK_STAT_ADDR                                = 0X38,
      BNO055_SYS_STAT_ADDR                                    = 0X39,
      BNO055_SYS_ERR_ADDR                                     = 0X3A,

      // Unit selection register
      BNO055_UNIT_SEL_ADDR                                    = 0X3B,
      BNO055_DATA_SELECT_ADDR                                 = 0X3C,

      // Mode registers
      BNO055_OPR_MODE_ADDR                                    = 0X3D,
      BNO055_PWR_MODE_ADDR                                    = 0X3E,

      BNO055_SYS_TRIGGER_ADDR                                 = 0X3F,
      BNO055_TEMP_SOURCE_ADDR                                 = 0X40,

      // Axis remap registers
      BNO055_AXIS_MAP_CONFIG_ADDR                             = 0X41,
      BNO055_AXIS_MAP_SIGN_ADDR                               = 0X42,

      // SIC registers
      BNO055_SIC_MATRIX_0_LSB_ADDR                            = 0X43,
      BNO055_SIC_MATRIX_0_MSB_ADDR                            = 0X44,
      BNO055_SIC_MATRIX_1_LSB_ADDR                            = 0X45,
      BNO055_SIC_MATRIX_1_MSB_ADDR                            = 0X46,
      BNO055_SIC_MATRIX_2_LSB_ADDR                            = 0X47,
      BNO055_SIC_MATRIX_2_MSB_ADDR                            = 0X48,
      BNO055_SIC_MATRIX_3_LSB_ADDR                            = 0X49,
      BNO055_SIC_MATRIX_3_MSB_ADDR                            = 0X4A,
      BNO055_SIC_MATRIX_4_LSB_ADDR                            = 0X4B,
      BNO055_SIC_MATRIX_4_MSB_ADDR                            = 0X4C,
      BNO055_SIC_MATRIX_5_LSB_ADDR                            = 0X4D,
      BNO055_SIC_MATRIX_5_MSB_ADDR                            = 0X4E,
      BNO055_SIC_MATRIX_6_LSB_ADDR                            = 0X4F,
      BNO055_SIC_MATRIX_6_MSB_ADDR                            = 0X50,
      BNO055_SIC_MATRIX_7_LSB_ADDR                            = 0X51,
      BNO055_SIC_MATRIX_7_MSB_ADDR                            = 0X52,
      BNO055_SIC_MATRIX_8_LSB_ADDR                            = 0X53,
      BNO055_SIC_MATRIX_8_MSB_ADDR                            = 0X54,

      // Accelerometer Offset registers
      ACCEL_OFFSET_X_LSB_ADDR                                 = 0X55,
      ACCEL_OFFSET_X_MSB_ADDR                                 = 0X56,
      ACCEL_OFFSET_Y_LSB_ADDR                                 = 0X57,
      ACCEL_OFFSET_Y_MSB_ADDR                                 = 0X58,
      ACCEL_OFFSET_Z_LSB_ADDR                                 = 0X59,
      ACCEL_OFFSET_Z_MSB_ADDR                                 = 0X5A,

      // Magnetometer Offset registers
      MAG_OFFSET_X_LSB_ADDR                                   = 0X5B,
      MAG_OFFSET_X_MSB_ADDR                                   = 0X5C,
      MAG_OFFSET_Y_LSB_ADDR                                   = 0X5D,
      MAG_OFFSET_Y_MSB_ADDR                                   = 0X5E,
      MAG_OFFSET_Z_LSB_ADDR                                   = 0X5F,
      MAG_OFFSET_Z_MSB_ADDR                                   = 0X60,

      // Gyroscope Offset registers
      GYRO_OFFSET_X_LSB_ADDR                                  = 0X61,
      GYRO_OFFSET_X_MSB_ADDR                                  = 0X62,
      GYRO_OFFSET_Y_LSB_ADDR                                  = 0X63,
      GYRO_OFFSET_Y_MSB_ADDR                                  = 0X64,
      GYRO_OFFSET_Z_LSB_ADDR                                  = 0X65,
      GYRO_OFFSET_Z_MSB_ADDR                                  = 0X66,

      // Radius registers
      ACCEL_RADIUS_LSB_ADDR                                   = 0X67,
      ACCEL_RADIUS_MSB_ADDR                                   = 0X68,
      MAG_RADIUS_LSB_ADDR                                     = 0X69,
      MAG_RADIUS_MSB_ADDR                                     = 0X6A,

      // PAGE1 REGISTER DEFINITION START
      ACC_CONFIG_ADDR										  = 0X08,
      MAG_CONFIG_ADDR										  = 0X09,
      GYR_CONFIG_0_ADDR										  = 0X0A,
      GYR_CONFIG_1_ADDR										  = 0X0B
    };

	enum EOperationModes
    {
      /* Operation mode settings*/
      OPERATION_MODE_CONFIG                                   = 0X00,
      OPERATION_MODE_ACCONLY                                  = 0X01,
      OPERATION_MODE_MAGONLY                                  = 0X02,
      OPERATION_MODE_GYRONLY                                  = 0X03,
      OPERATION_MODE_ACCMAG                                   = 0X04,
      OPERATION_MODE_ACCGYRO                                  = 0X05,
      OPERATION_MODE_MAGGYRO                                  = 0X06,
      OPERATION_MODE_AMG                                      = 0X07,
      OPERATION_MODE_IMUPLUS                                  = 0X08,
      OPERATION_MODE_COMPASS                                  = 0X09,
      OPERATION_MODE_M4G                                      = 0X0A,
      OPERATION_MODE_NDOF_FMC_OFF                             = 0X0B,
      OPERATION_MODE_NDOF                                     = 0X0C
    };

    enum EPowerMode
    {
      POWER_MODE_NORMAL                                       = 0X00,
      POWER_MODE_LOWPOWER                                     = 0X01,
      POWER_MODE_SUSPEND                                      = 0X02
    };


	public:
		_V_PROP_( Rotation )
		_V_PROP_( Orientation )
		_V_PROP_( Thermometer )
		_V_PROP_( Accelerometer )
		_V_PROP_( Gyroscope )
		_V_PROP_( Compass )
		_V_PROP_( Fused )

	public:
		_V_PROP_( Address )

		_V_PROP_( Enabled )
		_V_PROP_( ExternalOscillator )

		_V_PROP_( Mode )
		_V_PROP_( PowerMode )

	public:
		void UpdatePowerMode()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdatePowerModeInternal();
			UpdateMode();
		}

		void UpdateOrientation()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateOrientationInternal();
			UpdateMode();
		}

		void UpdateMode()
		{
			uint8_t	AValue;
//			Serial.println( "OPR_MOD000" );
//			Serial.println( int( Mode().GetValue() ));

			if( Mode() != TArduinoBNO0Mode::Auto )
				AValue = Mode();

			else
			{
//				Serial.println( "OPR_MOD111" );

				AValue = 0;
				bool AGyroscope = false;
				bool AAccelerometer = false;
				bool AFused	= false;
				bool ACalibration = false;
				bool ARelative = false;

				AAccelerometer = Accelerometer().Enabled();
				AGyroscope = Gyroscope().Enabled();

				if( Fused().Enabled() || Rotation().Enabled() )
				{
					AFused = true;
					ACalibration = Fused().Calibration();
				}

				if( AFused )
				{
					if( Compass().Enabled() )
					{
						if( AGyroscope )
						{
							if( ACalibration )
								AValue = TArduinoBNO0Mode::FusionFullCalibrated;

							else
								AValue = TArduinoBNO0Mode::FusionFull;
						}
						else
						{
							if( ARelative )
								AValue = TArduinoBNO0Mode::FusionAccelerometerCompassRelative;

							else
								AValue = TArduinoBNO0Mode::FusionAccelerometerCompassAbsolute;
						}
					}

					else
						AValue = TArduinoBNO0Mode::FusionAccelerometerGyroscopeRelative;
				}

				else
				{
//					Serial.println( "TEST1" );
					if( AAccelerometer )
					{
//						Serial.println( "TEST2" );
						if( Compass().Enabled() )
						{
							if( AGyroscope )
								AValue = TArduinoBNO0Mode::AccelerometerGyroscopeCompass;

							else
								AValue = TArduinoBNO0Mode::AccelerometerCompass;
						}

						else
						{
							if( AGyroscope )
								AValue = TArduinoBNO0Mode::AccelerometerGyroscope;

							else
								AValue = TArduinoBNO0Mode::Accelerometer;
						}
					}
					else
					{
						if( Compass().Enabled() )
						{
							if( AGyroscope )
								AValue = TArduinoBNO0Mode::GyroscopeCompass;

							else
								AValue = TArduinoBNO0Mode::Compass;
						}
						else
						{
							if( AGyroscope )
								AValue = TArduinoBNO0Mode::Gyroscope;

							else
								AValue = TArduinoBNO0Mode::Compass; // The default mode
						}
					}
				}
			}

//			++ AValue; // Work around a compiler bug!
//			Serial.println( "OPR_MOD" );
//			Serial.println( AValue + 1, HEX );


			uint8_t ACorrectedValue = AValue + 1; // Work around a compiler bug!
			WriteTo( BNO055_OPR_MODE_ADDR, ACorrectedValue ); //OPERATION_MODE_NDOF ); // OPERATION_MODE_NDOF
//			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF ); // OPERATION_MODE_NDOF
		}

		void UpdateTemperatureSource()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateTemperatureSourceInternal();
			UpdateMode();
		}

		void UpdateAccelerometerMode()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateAccelerometerModeInternal();
			UpdateMode();
		}

		void UpdateAccelerometerOffsets()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateAccelerometerOffsetsInternal();
			UpdateMode();
		}

		void UpdateGyroscopeOffsets()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateGyroscopeOffsetsInternal();
			UpdateMode();
		}

		void UpdateCompassOffsets()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateCompassOffsetsInternal();
			UpdateMode();
		}

		void UpdateGyroscopeMode()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateGyroscopeModeInternal();
			UpdateMode();
		}

		void UpdateCompassMode()
		{
			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			UpdateCompassModeInternal();
			UpdateMode();
		}

		void UpdateInits()
		{
			uint8_t AValue = 0; //0b00010000; // Temp always in C
			// TODO: Add other units
			WriteTo( BNO055_UNIT_SEL_ADDR, AValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
//			UpdatePowerManagementReg1( true );
		}

	protected:
		void UpdatePowerModeInternal()
		{
			WriteTo( BNO055_PWR_MODE_ADDR, PowerMode() );
			delay(10);
		}

		void UpdateAccelerometerOffsetsInternal()
		{
			int16_t	ABuffer[ 3 ];
			ABuffer[ 0 ] = Accelerometer().Offsets().X() * 100 + 0.5;
			ABuffer[ 1 ] = Accelerometer().Offsets().Y() * 100 + 0.5;
			ABuffer[ 2 ] = Accelerometer().Offsets().Z() * 100 + 0.5;

			WriteVector( ACCEL_OFFSET_X_LSB_ADDR, ABuffer );
		}

		void UpdateGyroscopeOffsetsInternal()
		{
			int16_t	ABuffer[ 3 ];
			ABuffer[ 0 ] = Gyroscope().Offsets().X() * 16 + 0.5;
			ABuffer[ 1 ] = Gyroscope().Offsets().Y() * 16 + 0.5;
			ABuffer[ 2 ] = Gyroscope().Offsets().Z() * 16 + 0.5;

			WriteVector( GYRO_OFFSET_X_LSB_ADDR, ABuffer );
		}

		void UpdateCompassOffsetsInternal()
		{
			int16_t	ABuffer[ 3 ];
			ABuffer[ 0 ] = Compass().Offsets().X() * 16 + 0.5;
			ABuffer[ 1 ] = Compass().Offsets().Y() * 16 + 0.5;
			ABuffer[ 2 ] = Compass().Offsets().Z() * 16 + 0.5;

			WriteVector( MAG_OFFSET_X_LSB_ADDR, ABuffer );
		}

		void UpdateOrientationInternal()
		{
//#ifdef _MITOV_BNO055_ORIENTATION_
			uint8_t	AXMap = Orientation().X() & 0b11;
			uint8_t	AYMap = Orientation().Y() & 0b11;
			uint8_t	AZMap;
			for( AZMap = 0; AZMap < 3; ++AZMap )
			{
				if( AZMap == AXMap )
					continue;

				if( AZMap == AYMap )
					continue;

				break;
			}

			uint8_t	AValue = AXMap |
				( AYMap << 2 ) |
				( AZMap << 4 );

			WriteTo( BNO055_AXIS_MAP_CONFIG_ADDR, AValue );

			AValue = 0;
			if( Orientation().X() & 0b100 )
				AValue |= 0b001;

			if( Orientation().Y() & 0b100 )
				AValue |= 0b010;

			if( Orientation().InvertZ() )
				AValue |= 0b100;

			WriteTo( BNO055_AXIS_MAP_SIGN_ADDR, AValue );

//#else
//			WriteTo( BNO055_AXIS_MAP_CONFIG_ADDR, 0x24 );
//			WriteTo( BNO055_AXIS_MAP_SIGN_ADDR, 0 );
//#endif
		}

		void UpdateCompassModeInternal()
		{
/*
			WriteTo(BNO055_PAGE_ID_ADDR, 1);
			Serial.println( "MAG_CONFIG" );
			Serial.println( ReadByte( MAG_CONFIG_ADDR ), HEX );
//			WriteTo( MAG_CONFIG_ADDR, 0x6D );
////			WriteTo( MAG_CONFIG_ADDR, 0x6D );
			WriteTo( MAG_CONFIG_ADDR, 0xB );
			WriteTo(BNO055_PAGE_ID_ADDR, 0);
			return;
*/
			WriteTo(BNO055_PAGE_ID_ADDR, 1);
//#ifdef _MITOV_BNO055_COMPASS_
			static const uint8_t CRate[] =
			{
				2,
				6,
				8,
				10,
				15,
				20,
				25,
				30
			};

			uint8_t ARateOrder = MITOV_ARRAY_SIZE( CRate ) - 1;
			for( int i = MITOV_ARRAY_SIZE( CRate ); i--; )
			{
				if( Compass().Rate() > CRate[ i ] )
					break;

				ARateOrder = i;

				if( Compass().Rate() == CRate[ i ] )
					break;

			}

			uint8_t AValue = ARateOrder | ( uint8_t( Compass().OperationMode() ) << 3 ) | ( uint8_t( Compass().PowerMode() ) << 5 );

//			Serial.println( "MAG_CONFIG" );
//			Serial.println( AValue, HEX );

			WriteTo( MAG_CONFIG_ADDR, AValue );

//#else
//			WriteTo( MAG_CONFIG_ADDR, 0x6D );
//#endif
			WriteTo(BNO055_PAGE_ID_ADDR, 0);
		}

		void UpdateGyroscopeModeInternal()
		{
			WriteTo(BNO055_PAGE_ID_ADDR, 1);
//#ifdef _MITOV_BNO055_GYROSCOPE_
			static const uint16_t CRange[] =
			{
				125,
				250,
				500,
				1000,
				2000
			};

			uint8_t ARangeOrder = MITOV_ARRAY_SIZE( CRange ) - 1;
			for( int i = MITOV_ARRAY_SIZE( CRange ); i--; )
			{
				if( Gyroscope().Range() > CRange[ i ] )
					break;

				ARangeOrder = i;

				if( Gyroscope().Range() == CRange[ i ] )
					break;

			}

			static const uint16_t CBandwidth[] =
			{
				12,
				23,

				32,

				47,

				64,

				116,
				230,
				523
			};

			static const uint8_t CBandwidthOrders[] =
			{
				0b00101000, // 12
				0b00100000, // 23

				0b00111000, // 32

				0b00011000, // 47

				0b00110000, // 64

				0b00010000, // 116
				0b00001000, // 230
				0b00000000  // 523
			};

			uint8_t ABandwidthOrder = MITOV_ARRAY_SIZE( CBandwidth ) - 1;
			for( int i = MITOV_ARRAY_SIZE( CBandwidth ); i--; )
			{
				if( Gyroscope().Bandwidth() > CBandwidth[ i ] )
					break;

				ABandwidthOrder = i;

				if( Gyroscope().Bandwidth() == CBandwidth[ i ] )
					break;

			}

			uint8_t AValue = ARangeOrder | CBandwidthOrders[ ABandwidthOrder ];
			WriteTo( GYR_CONFIG_0_ADDR, AValue );

			WriteTo( GYR_CONFIG_1_ADDR, uint8_t( Gyroscope().Mode() ) );
//#else
//			WriteTo( GYR_CONFIG_0_ADDR, 0x38 );
//			WriteTo( GYR_CONFIG_1_ADDR, 0x00 );
//#endif
			WriteTo(BNO055_PAGE_ID_ADDR, 0);
		}

		void UpdateAccelerometerModeInternal()
		{
			WriteTo(BNO055_PAGE_ID_ADDR, 1);
//#ifdef _MITOV_BNO055_ACCELEROMETER_
			static const float CBandwidth[] =
			{
				7.81,
				15.63,
				31.25,
				62.5,
				125,
				250,
				500,
				1000
			};

			uint8_t ABandwidthOrder = MITOV_ARRAY_SIZE( CBandwidth ) - 1;
			for( int i = MITOV_ARRAY_SIZE( CBandwidth ); i--; )
			{
				if( Accelerometer().Bandwidth() > CBandwidth[ i ] )
					break;

				ABandwidthOrder = i;

				if( Accelerometer().Bandwidth() == CBandwidth[ i ] )
					break;

			}

			uint8_t ARangeOrder = Mitov::MitovIlog2( Accelerometer().Range() ) - 1;

			uint8_t AValue = ARangeOrder | ( ABandwidthOrder << 2 ) | ( uint8_t( Accelerometer().Mode() ) << 5 );
			WriteTo( ACC_CONFIG_ADDR, AValue );
//#else
//			WriteTo( ACC_CONFIG_ADDR, 0x0D );
//#endif
			WriteTo(BNO055_PAGE_ID_ADDR, 0);
		}

		void UpdateTemperatureSourceInternal()
		{
			WriteTo( BNO055_TEMP_SOURCE_ADDR, Thermometer().FromGyroscope() ? 1 : 0 );
		}

	protected:
		void Reset()
		{
//			WriteTo( BNO055_SYS_TRIGGER_ADDR, 0x0 );
//			delay(10);
//			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
//			delay(10);

			while (ReadByte(BNO055_CHIP_ID_ADDR) != BNO055_ID)
				delay(10);

//			WriteTo( BNO055_SYS_TRIGGER_ADDR, 0x0 );

			WriteTo( BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG );
			delay(10);

			WriteTo(BNO055_SYS_TRIGGER_ADDR, 0x20);
			delay(30);

			while (ReadByte(BNO055_CHIP_ID_ADDR) != BNO055_ID)
				delay(10);

//			delay(150);
			delay(50);

//			delay(10);
// Set to normal power mode
			WriteTo( BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL );
			delay(10);

			WriteTo(BNO055_PAGE_ID_ADDR, 0);

// Set the output units
/*
uint8_t unitsel = (0 << 7) | // Orientation = Android
                (0 << 4) | // Temperature = Celsius
                (0 << 2) | // Euler = Degrees
                (1 << 1) | // Gyro = Rads
                (0 << 0);  // Accelerometer = m/s^2
write8(BNO055_UNIT_SEL_ADDR, unitsel);
*/

// Configure axis mapping (see section 3.4)
/*
write8(BNO055_AXIS_MAP_CONFIG_ADDR, REMAP_CONFIG_P2); // P0-P7, Default is P1
delay(10);
write8(BNO055_AXIS_MAP_SIGN_ADDR, REMAP_SIGN_P2); // P0-P7, Default is P1
delay(10);
*/

			WriteTo(BNO055_SYS_TRIGGER_ADDR, 0x0);
			delay(10);
//			UpdateMode();
//			delay(20);

			UpdateInits();
			if( ExternalOscillator() )
				WriteTo( BNO055_SYS_TRIGGER_ADDR, 0x80 );

			UpdateTemperatureSourceInternal();
			UpdatePowerModeInternal();
			UpdateOrientationInternal();
			UpdateAccelerometerModeInternal();
			UpdateGyroscopeModeInternal();
			UpdateCompassModeInternal();
			UpdateAccelerometerOffsetsInternal();
			UpdateGyroscopeOffsetsInternal();
			UpdateCompassOffsetsInternal();
			UpdateMode();
//			delay( 1000 );

//			Serial.println( "Reset Completed" );
		}

		void ReadSensors()
		{
//			Serial.println( "ReadSensors" );
			if( Thermometer().Enabled() )
			{
//				Serial.println( "TEST1" );
				float AValue = (int8_t)ReadByte(BNO055_TEMP_ADDR);
				if( Thermometer().InFahrenheit() )
					AValue = Func::ConvertCtoF( AValue );

				Thermometer().OutputPin().SetPinValue( AValue );
			}

			if( GetAccelerometerDataAvailable() )
			{
				int16_t AIntData[ 6 ];
				ReadVector( BNO055_ACCEL_DATA_X_LSB_ADDR, AIntData, 3 );

				float AFloatValue = AIntData[ 0 ]; // Needed to work around compiler bug!
				Accelerometer().OutputPinsX().SetPinValue( AFloatValue / 100.0 ); // in m/s^2

				AFloatValue = AIntData[ 1 ]; // Needed to work around compiler bug!
				Accelerometer().OutputPinsY().SetPinValue( AFloatValue / 100.0 ); // in m/s^2

				AFloatValue = AIntData[ 2 ]; // Needed to work around compiler bug!
				Accelerometer().OutputPinsZ().SetPinValue( AFloatValue / 100.0 ); // in m/s^2
			}

			if( GetGyroscopeDataAvailable() )
			{
				int16_t AIntData[ 6 ];
				ReadVector( BNO055_GYRO_DATA_X_LSB_ADDR, AIntData, 3 );

				float AFloatValue = AIntData[ 0 ]; // Needed to work around compiler bug!
				Gyroscope().OutputPinsX().SetPinValue( AFloatValue / 16.0 ); // in Dps

				AFloatValue = AIntData[ 1 ]; // Needed to work around compiler bug!
				Gyroscope().OutputPinsY().SetPinValue( AFloatValue / 16.0 ); // in Dps

				AFloatValue = AIntData[ 2 ]; // Needed to work around compiler bug!
				Gyroscope().OutputPinsZ().SetPinValue( AFloatValue / 16.0 ); // in Dps
			}

			if( GetCompassDataAvailable() )
			{
				int16_t AIntData[ 6 ];
				ReadVector( BNO055_MAG_DATA_X_LSB_ADDR, AIntData, 3 );

				float AFloatValue = AIntData[ 0 ]; // Needed to work around compiler bug!
				Compass().OutputPinsX().SetPinValue( AFloatValue / 16.0 ); // in uT

				AFloatValue = AIntData[ 1 ]; // Needed to work around compiler bug!
				Compass().OutputPinsY().SetPinValue( AFloatValue / 16.0 ); // in uT

				AFloatValue = AIntData[ 2 ]; // Needed to work around compiler bug!
				Compass().OutputPinsZ().SetPinValue( AFloatValue / 16.0 ); // in uT
			}

			if( GetFusedDataAvailable() )
			{
				int16_t AIntData[ 6 ];
				ReadVector( BNO055_EULER_H_LSB_ADDR, AIntData, 3 );

				float AFloatValue = AIntData[ 0 ]; // Needed to work around compiler bug!
				Fused().OutputPinsX().SetPinValue( AFloatValue / 16.0 ); // in m/s^2

				AFloatValue = AIntData[ 1 ]; // Needed to work around compiler bug!
				Fused().OutputPinsY().SetPinValue( AFloatValue / 16.0 ); // in m/s^2

				AFloatValue = AIntData[ 2 ]; // Needed to work around compiler bug!
				Fused().OutputPinsZ().SetPinValue( AFloatValue / 16.0 ); // in m/s^2
			}

			if( GetEulerDataAvailable() )
			{
				if( Rotation().OutputPinsX().GetPinIsConnected() || Rotation().OutputPinsY().GetPinIsConnected() || Rotation().OutputPinsZ().GetPinIsConnected() )
				{
					int16_t AIntData[ 6 ];
					ReadVector( BNO055_EULER_H_LSB_ADDR, AIntData, 3 );

					float AFloatValue = AIntData[ 0 ]; // Needed to work around compiler bug!
					AFloatValue /= 16.0;
					if( PositiveOnly() )
						AFloatValue += 180;

					Rotation().OutputPinsY().SetPinValue( AFloatValue ); // in m/s^2

					AFloatValue = AIntData[ 1 ]; // Needed to work around compiler bug!
					AFloatValue /= 16.0;
					if( PositiveOnly() )
						AFloatValue += 180;

					Rotation().OutputPinsZ().SetPinValue( AFloatValue ); // in m/s^2

					AFloatValue = AIntData[ 2 ]; // Needed to work around compiler bug!
					AFloatValue /= 16.0;
					if( PositiveOnly() )
						AFloatValue += 180;

					Rotation().OutputPinsX().SetPinValue( AFloatValue ); // in m/s^2
				}

				if( Rotation().QuaternionOutputPin().GetPinIsConnected() )
				{
					int16_t AIntData[ 8 ];
					ReadVector( BNO055_QUATERNION_DATA_W_LSB_ADDR, AIntData, 4 );

					TQuaternion AQuaternion;

					float AFloatValue = AIntData[ 0 ]; // Needed to work around compiler bug!
					AFloatValue /= ( 1 << 14 );

					AQuaternion.Values[ 3 ] = AFloatValue;

					for( int i = 0; i < 3; ++i )
					{
						AFloatValue = AIntData[ i + 1 ]; // Needed to work around compiler bug!
						AFloatValue /= ( 1 << 14 );

						AQuaternion.Values[ i ] = AFloatValue;
					}

					Rotation().QuaternionOutputPin().SetPinValue( AQuaternion, false );
				}
			}
		}

	protected:
		bool GetAccelerometerDataAvailable()
		{
			if( Mode() == TArduinoBNO0Mode::Auto )
				return Accelerometer().Enabled();

			if( Mode() == TArduinoBNO0Mode::Accelerometer )
				return true;

			if( Mode() == TArduinoBNO0Mode::AccelerometerCompass )
				return true;

			if( Mode() == TArduinoBNO0Mode::AccelerometerGyroscope )
				return true;

			if( Mode() >= TArduinoBNO0Mode::AccelerometerGyroscopeCompass )
				return true;

			return false;
		}

		bool GetGyroscopeDataAvailable()
		{
			if( Mode() == TArduinoBNO0Mode::Auto )
				return Gyroscope().Enabled();

			if( Mode() == TArduinoBNO0Mode::Gyroscope )
				return true;

			if( Mode() == TArduinoBNO0Mode::AccelerometerGyroscope )
				return true;

			if( Mode() == TArduinoBNO0Mode::GyroscopeCompass )
				return true;

			if( Mode() == TArduinoBNO0Mode::AccelerometerGyroscopeCompass )
				return true;

			if( Mode() == TArduinoBNO0Mode::FusionAccelerometerGyroscopeRelative )
				return true;

			if( Mode() >= TArduinoBNO0Mode::FusionFull )
				return true;

			return false;
		}

		bool GetCompassDataAvailable()
		{
			if( Mode() == TArduinoBNO0Mode::Auto )
				return Compass().Enabled();

			if( Mode() == TArduinoBNO0Mode::Compass )
				return true;

			if( Mode() == TArduinoBNO0Mode::GyroscopeCompass )
				return true;

			if( Mode() == TArduinoBNO0Mode::AccelerometerGyroscopeCompass )
				return true;

			if( Mode() >= TArduinoBNO0Mode::FusionAccelerometerCompassAbsolute )
				return true;

			return false;
		}

		bool GetFusedDataAvailable()
		{
			if( Mode() == TArduinoBNO0Mode::Auto )
				return Fused().Enabled();

			return ( Mode() >= TArduinoBNO0Mode::FusionAccelerometerGyroscopeRelative );
		}

		bool GetEulerDataAvailable()
		{
			if( Mode() == TArduinoBNO0Mode::Auto )
				return Rotation().Enabled();

			return ( Mode() >= TArduinoBNO0Mode::FusionAccelerometerGyroscopeRelative );
		}

	protected:
/*
		void BeginReadVector( uint8_t ARegister, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
  			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), ACount );
		}
*/
		void ReadVector( uint8_t ARegister, int16_t *AData, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
  			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( ACount * 2 ));

			for (uint8_t i = 0; i < ACount; i++)
			{
				int16_t ALowByte = C_I2C.read();
				int16_t AHighByte = C_I2C.read();
				AData[ i ] = ( ALowByte * 0xFF ) | ( AHighByte << 8 );
			}
		}

		void WriteVector( uint8_t ARegister, int16_t *AData )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));

  			C_I2C.write( ARegister );
			for (uint8_t i = 0; i < 3; i++)
			{
				C_I2C.write( uint8_t( AData[ i ] ));
				C_I2C.write( uint8_t( AData[ i ] >> 8 ));
			}

			C_I2C.endTransmission();
		}

		void WriteTo( uint8_t ARegister, uint8_t AData )
		{
//			Serial.print( "write8: " ); Serial.print( ARegister, HEX ); Serial.print( " = " ); Serial.println( AData, HEX );

			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue() ), ARegister, AData );
		}

		bool ReadBytes( uint8_t ARegister, uint8_t length, void *data )
		{
			return I2C::ReadBytes( C_I2C, uint8_t( Address().GetValue() ), ARegister, length, data );
		}

		uint8_t ReadByte( uint8_t ARegister ) // Do not inline! C++ Bug! :-(
		{
			return I2C::ReadByte( C_I2C, uint8_t( Address().GetValue() ), ARegister );
		}

	public:
		inline void SystemStart()
		{
//	BNO055 clock stretches for 500us or more!
#ifdef ESP8266
			C_I2C.setClockStretchLimit(1000); // Allow for 1000us of clock stretching
#endif
			ReadByte(BNO055_CHIP_ID_ADDR);


//			Serial.println( "" );
//			Serial.println( "START!!!" );
//			Serial.println( "" );
//			delay( 1000 );

// Switch to config mode (just in case since this is the default)
			Reset();
//			delay( 1000 );
//			Reset();


// Set the requested operating mode (see section 3.3)
//			setMode(mode);
//			delay(20);
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() )
				ReadSensors();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
