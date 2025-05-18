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
	namespace LSM6DS3
	{
		const uint8_t FUNC_CFG_ACCESS				= 0x01;
		const uint8_t SENSOR_SYNC_TIME_FRAME		= 0x04;
		const uint8_t FIFO_CTRL1					= 0x06;
		const uint8_t FIFO_CTRL2					= 0x07;
		const uint8_t FIFO_CTRL3					= 0x08;
		const uint8_t FIFO_CTRL4					= 0x09;
		const uint8_t FIFO_CTRL5					= 0x0A;
		const uint8_t ORIENT_CFG_G					= 0x0B;
		const uint8_t INT1_CTRL						= 0x0D;
		const uint8_t INT2_CTRL						= 0x0E;
		const uint8_t WHO_AM_I						= 0x0F;
		const uint8_t CTRL1_XL						= 0x10;
		const uint8_t CTRL2_G						= 0x11;
		const uint8_t CTRL3_C						= 0x12;
		const uint8_t CTRL4_C						= 0x13;
		const uint8_t CTRL5_C						= 0x14;
		const uint8_t CTRL6_C						= 0x15;
		const uint8_t CTRL7_G						= 0x16;
		const uint8_t CTRL8_XL						= 0x17;
		const uint8_t CTRL9_XL						= 0x18;
		const uint8_t CTRL10_C						= 0x19;
		const uint8_t MASTER_CONFIG					= 0x1A;
		const uint8_t WAKE_UP_SRC					= 0x1B;
		const uint8_t TAP_SRC						= 0x1C;
		const uint8_t D6D_SRC						= 0x1D;
		const uint8_t STATUS_REG					= 0x1E;
		const uint8_t OUT_TEMP_L					= 0x20;
		const uint8_t OUT_TEMP_H					= 0x21;
		const uint8_t OUTX_L_G						= 0x22;
		const uint8_t OUTX_H_G						= 0x23;
		const uint8_t OUTY_L_G						= 0x24;
		const uint8_t OUTY_H_G						= 0x25;
		const uint8_t OUTZ_L_G						= 0x26;
		const uint8_t OUTZ_H_G						= 0x27;
		const uint8_t OUTX_L_XL						= 0x28;
		const uint8_t OUTX_H_XL						= 0x29;
		const uint8_t OUTY_L_XL						= 0x2A;
		const uint8_t OUTY_H_XL						= 0x2B;
		const uint8_t OUTZ_L_XL						= 0x2C;
		const uint8_t OUTZ_H_XL						= 0x2D;

		const uint8_t SENSORHUB1_REG				= 0x2E;
		const uint8_t SENSORHUB2_REG				= 0x2F;
		const uint8_t SENSORHUB3_REG				= 0x30;
		const uint8_t SENSORHUB4_REG				= 0x31;
		const uint8_t SENSORHUB5_REG				= 0x32;
		const uint8_t SENSORHUB6_REG				= 0x33;
		const uint8_t SENSORHUB7_REG				= 0x34;
		const uint8_t SENSORHUB8_REG				= 0x35;
		const uint8_t SENSORHUB9_REG				= 0x36;
		const uint8_t SENSORHUB10_REG				= 0x37;
		const uint8_t SENSORHUB11_REG				= 0x38;
		const uint8_t SENSORHUB12_REG				= 0x39;
		const uint8_t FIFO_STATUS1					= 0x3A;
		const uint8_t FIFO_STATUS2					= 0x3B;
		const uint8_t FIFO_STATUS3					= 0x3C;
		const uint8_t FIFO_STATUS4					= 0x3D;
		const uint8_t FIFO_DATA_OUT_L				= 0x3E;
		const uint8_t FIFO_DATA_OUT_H				= 0x3F;
		const uint8_t TIMESTAMP0_REG				= 0x40;
		const uint8_t TIMESTAMP1_REG				= 0x41;
		const uint8_t TIMESTAMP2_REG				= 0x42;
		const uint8_t STEP_TIMESTAMP_L				= 0x49;
		const uint8_t STEP_TIMESTAMP_H				= 0x4A;
		const uint8_t STEP_COUNTER_L				= 0x4B;
		const uint8_t STEP_COUNTER_H				= 0x4C;
		const uint8_t SENSORHUB13_REG				= 0x4D;
		const uint8_t SENSORHUB14_REG				= 0x4E;
		const uint8_t SENSORHUB15_REG				= 0x4F;
		const uint8_t SENSORHUB16_REG				= 0x50;
		const uint8_t SENSORHUB17_REG				= 0x51;
		const uint8_t SENSORHUB18_REG				= 0x52;
		const uint8_t FUNC_SRC						= 0x53;
		const uint8_t TAP_CFG						= 0x58;
		const uint8_t TAP_THS_6D					= 0x59;
		const uint8_t INT_DUR2						= 0x5A;
		const uint8_t WAKE_UP_THS					= 0x5B;
		const uint8_t WAKE_UP_DUR					= 0x5C;
		const uint8_t FREE_FALL						= 0x5D;
		const uint8_t MD1_CFG						= 0x5E;
		const uint8_t MD2_CFG						= 0x5F;
		const uint8_t OUT_MAG_RAW_X_L				= 0x66;
		const uint8_t OUT_MAG_RAW_X_H				= 0x67;
		const uint8_t OUT_MAG_RAW_Y_L				= 0x68;
		const uint8_t OUT_MAG_RAW_Y_H				= 0x69;
		const uint8_t OUT_MAG_RAW_Z_L				= 0x6A;
		const uint8_t OUT_MAG_RAW_Z_H				= 0x6B;

		// Embedded registers
		const uint8_t SLV0_ADD						= 0x02;
		const uint8_t SLV0_SUBADD					= 0x03;
		const uint8_t SLAVE0_CONFIG					= 0x04;
		const uint8_t SLV1_ADD						= 0x05;
		const uint8_t SLV1_SUBADD					= 0x06;
		const uint8_t SLAVE1_CONFIG					= 0x07;
		const uint8_t SLV2_ADD						= 0x08;
		const uint8_t SLV2_SUBADD					= 0x09;
		const uint8_t SLAVE2_CONFIG					= 0x0A;
		const uint8_t SLV3_ADD						= 0x0B;
		const uint8_t SLV3_SUBADD					= 0x0C;
		const uint8_t SLAVE3_CONFIG					= 0x0D;
		const uint8_t DATAWRITE_SRC_MODE_SUB_SLV0	= 0x0E;
		const uint8_t PEDO_THS_REG					= 0x0F;
		const uint8_t SM_THS						= 0x13;
		const uint8_t PEDO_DEB_REG					= 0x14;
		const uint8_t STEP_COUNT_DELTA				= 0x15;
		const uint8_t MAG_SI_XX						= 0x24;
		const uint8_t MAG_SI_XY						= 0x25;
		const uint8_t MAG_SI_XZ						= 0x26;
		const uint8_t MAG_SI_YX						= 0x27;
		const uint8_t MAG_SI_YY						= 0x28;
		const uint8_t MAG_SI_YZ						= 0x29;
		const uint8_t MAG_SI_ZX						= 0x2A;
		const uint8_t MAG_SI_ZY						= 0x2B;
		const uint8_t MAG_SI_ZZ						= 0x2C;
		const uint8_t MAG_OFFX_L					= 0x2D;
		const uint8_t MAG_OFFX_H					= 0x2E;
		const uint8_t MAG_OFFY_L					= 0x2F;
		const uint8_t MAG_OFFY_H					= 0x30;
		const uint8_t MAG_OFFZ_L					= 0x31;
		const uint8_t MAG_OFFZ_H					= 0x32;

	}
//---------------------------------------------------------------------------
	namespace TArduinoLSM6DS3GyroscopeOrientationAxes
	{
		enum TArduinoLSM6DS3GyroscopeOrientationAxes
		{
			XYZ,
			XZY,
			YXZ,
			YZX,
			ZXY,
			ZYX
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_CutoffFrequency,
		typename T_Enabled
	> class TArduinoLSM6DS3GyroscopeHighPassFilter :
		public T_CutoffFrequency,
		public T_Enabled
	{
	public:
		_V_PROP_( CutoffFrequency )
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Axes,
		typename T_InvertX,
		typename T_InvertY,
		typename T_InvertZ
	> class TArduinoLSM6DS3GyroscopeOrientation :
		public T_Axes,
		public T_InvertX,
		public T_InvertY,
		public T_InvertZ
	{
	public:
		_V_PROP_( Axes )
		_V_PROP_( InvertX )
		_V_PROP_( InvertY )
		_V_PROP_( InvertZ )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Bandwidth,
		typename T_Enabled
	> class TArduinoLSM6DS3AccelerometerAntiAliasingFilter :
		public T_Bandwidth,
		public T_Enabled
	{
	public:
		_V_PROP_( Bandwidth )
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_AntiAliasingFilter,
		typename T_Enabled,
		typename T_FullScaleRange,
		typename T_HighPerformance,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_PowerDown,
		typename T_SampleRate
	> class TArduinoLSM6DS3Accelerometer :
		public T_AntiAliasingFilter,
		public T_Enabled,
		public T_FullScaleRange,
		public T_HighPerformance,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_PowerDown,
		public T_SampleRate
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	public:
		_V_PROP_( AntiAliasingFilter )
		_V_PROP_( Enabled )
		_V_PROP_( FullScaleRange )
		_V_PROP_( HighPerformance )
		_V_PROP_( PowerDown )
		_V_PROP_( SampleRate )

	public:
		inline bool GetIsEnabled()
		{
			return Enabled().GetValue() && ( T_OutputPins_X::GetPinIsConnected() || T_OutputPins_Y::GetPinIsConnected() || T_OutputPins_Z::GetPinIsConnected() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FullScaleRange,
		typename T_HighPassFilter,
		typename T_HighPerformance,
		typename T_Orientation,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_PowerDown,
		typename T_SampleRate,
		typename T_Sleep
	> class TArduinoLSM9DS1Gyroscope :
		public T_Enabled,
		public T_FullScaleRange,
		public T_HighPassFilter,
		public T_HighPerformance,
		public T_Orientation,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_PowerDown,
		public T_SampleRate,
		public T_Sleep
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( FullScaleRange )
		_V_PROP_( HighPassFilter )
		_V_PROP_( HighPerformance )
		_V_PROP_( Orientation )
		_V_PROP_( PowerDown )
		_V_PROP_( SampleRate )
		_V_PROP_( Sleep )

	public:
		inline bool GetIsEnabled()
		{
			return Enabled().GetValue() && ( T_OutputPins_X::GetPinIsConnected() || T_OutputPins_Y::GetPinIsConnected() || T_OutputPins_Z::GetPinIsConnected() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Threshold,
		typename T_Time
	> class TArduinoLSM6DS3PedometerDebounce :
		public T_Threshold,
		public T_Time
	{
	public:
		_V_PROP_( Threshold )
		_V_PROP_( Time )

	};
//---------------------------------------------------------------------------
	template <
		typename T_CountOutputPin,
		typename T_Debounce,
		typename T_Enabled,
		typename T_MinThreshold,
		typename T_OverflowOutputPin,
		typename T_StepOutputPin
//		typename T_UseMaxScale4G
	> class TArduinoLSM6DS3Pedometer :
		public T_CountOutputPin,
		public T_Debounce,
		public T_Enabled,
		public T_MinThreshold,
		public T_OverflowOutputPin,
		public T_StepOutputPin
//		public T_UseMaxScale4G
	{
	public:
		_V_PIN_( CountOutputPin )
		_V_PIN_( OverflowOutputPin )
		_V_PIN_( StepOutputPin )

	public:
		_V_PROP_( Debounce )
		_V_PROP_( Enabled )
		_V_PROP_( MinThreshold )
//		_V_PROP_( UseMaxScale4G )

	public:
		bool GetIsEnabled() { return ( Enabled() || T_CountOutputPin::GetPinIsConnected() || T_OverflowOutputPin::GetPinIsConnected() || T_StepOutputPin::GetPinIsConnected() ); }

	};
//---------------------------------------------------------------------------
	template <
		typename T_DoubleOutputPins_Up,
		typename T_DoubleOutputPins_Down,
		typename T_Enabled,
		uint8_t INDEX,
		typename T_SingleOutputPins_Up,
		typename T_SingleOutputPins_Down
	> class TArduinoLSM6DS3TapChannel :
		public T_DoubleOutputPins_Up,
		public T_DoubleOutputPins_Down,
		public T_Enabled,
		public T_SingleOutputPins_Up,
		public T_SingleOutputPins_Down
	{
	public:
		_V_PIN_( DoubleOutputPins_Up )
		_V_PIN_( DoubleOutputPins_Down )
		_V_PIN_( SingleOutputPins_Up )
		_V_PIN_( SingleOutputPins_Down )

	public:
		_V_PROP_( Enabled )

	public:
		inline void GetTapsEnabled( uint8_t & ATapsEnabled )
		{
//			Serial.println( INDEX );
			if( Enabled() )
				ATapsEnabled |= ( 0b00001000 >> INDEX );

//			Serial.println( ATapsEnabled, BIN );
		}

		inline void GetDoubleTapsEnabled( bool & ATapsEnabled )
		{
			if( ATapsEnabled )
				return;

			if( Enabled() )
				ATapsEnabled = ( T_DoubleOutputPins_Up::GetPinIsConnected() || T_DoubleOutputPins_Down::GetPinIsConnected() );

		}

		inline void GetAnyTapsEnabled( bool & ATapsEnabled )
		{
			if( ATapsEnabled )
				return;

			if( Enabled() )
				ATapsEnabled = ( T_DoubleOutputPins_Up::GetPinIsConnected() || T_DoubleOutputPins_Down::GetPinIsConnected() || T_SingleOutputPins_Up::GetPinIsConnected() || T_SingleOutputPins_Down::GetPinIsConnected() );

		}

		inline void ProcessTaps( uint8_t AValue )
		{
			if( AValue & ( 0b00000100 >> INDEX ) )
			{
				if( 0b00100000 ) // Single
				{
					if( 0b00001000 )
						T_SingleOutputPins_Down::ClockPin();

					else
						T_SingleOutputPins_Up::ClockPin();

				}

				if( 0b00010000 ) // Double
				{
					if( 0b00001000 )
						T_DoubleOutputPins_Down::ClockPin();

					else
						T_DoubleOutputPins_Up::ClockPin();

				}

			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_MaxGapDuration
	> class TArduinoLSM6DS3DoubleTap :
		public T_Enabled,
		public T_MaxGapDuration
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( MaxGapDuration )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Quiet,
		typename T_Shock
	> class TArduinoLSM6DS3TapDuration :
		public T_Quiet,
		public T_Shock
	{
	public:
		_V_PROP_( Quiet )
		_V_PROP_( Shock )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Axes_GetAnyTapsEnabled,
		typename T_Axes_GetDoubleTapsEnabled,
		typename T_Axes_GetTapsEnabled,
		typename T_Axes_ProcessTaps,
		typename T_DoubleTap,
		typename T_Duration,
		typename T_Threshold
	> class TArduinoMMA845XQAccelerometerTap :
		public T_DoubleTap,
		public T_Duration,
		public T_Threshold
	{
	public:
		_V_PROP_( DoubleTap )
		_V_PROP_( Duration )
		_V_PROP_( Threshold )

	public:
		uint8_t GetTapsEnabled()
		{
			uint8_t ATapsEnabled = 0;
			T_Axes_GetTapsEnabled::Call( ATapsEnabled );

			return ATapsEnabled;
		}

		bool GetDoubleTapEnabled()
		{
			if( ! DoubleTap().Enabled().GetValue() )
				return false;

			bool AEnabled = false;
			T_Axes_GetDoubleTapsEnabled::Call( AEnabled );

			return AEnabled;
		}

		bool GetIsEnabled()
		{
			bool AEnabled = false;
			T_Axes_GetAnyTapsEnabled::Call( AEnabled );

			return AEnabled;
		}

		inline void ProcessTaps( uint8_t AValue )
		{
			T_Axes_ProcessTaps::Call( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Sleep,
		typename T_Wake
	> class TArduinoLSM6DS3SleepDuration :
		public T_Sleep,
		public T_Wake
	{
	public:
		_V_PROP_( Sleep )
		_V_PROP_( Wake )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Duration,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Threshold,
		typename T_UseHighPassFilter,
		typename T_WakeUpOutputPins_X,
		typename T_WakeUpOutputPins_Y,
		typename T_WakeUpOutputPins_Z
	> class TArduinoLSM6DS3Sleep :
		public T_Duration,
		public T_Enabled,
		public T_OutputPin,
		public T_Threshold,
		public T_UseHighPassFilter,
		public T_WakeUpOutputPins_X,
		public T_WakeUpOutputPins_Y,
		public T_WakeUpOutputPins_Z
	{
	public:
		_V_PIN_( WakeUpOutputPins_X )
		_V_PIN_( WakeUpOutputPins_Y )
		_V_PIN_( WakeUpOutputPins_Z )

	public:
		_V_PROP_( Duration )
		_V_PROP_( Enabled )
		_V_PROP_( OutputPin )
		_V_PROP_( Threshold )
		_V_PROP_( UseHighPassFilter )

	public:
		bool GetIsEnabled()
		{
			return ( Enabled().GetValue() && ( T_OutputPin::GetPinIsConnected() || T_WakeUpOutputPins_X::GetPinIsConnected() || T_WakeUpOutputPins_Y::GetPinIsConnected() || T_WakeUpOutputPins_Z::GetPinIsConnected() ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Duration,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Threshold
	> class TArduinoLSM6DS3FreeFall :
		public T_Duration,
		public T_Enabled,
		public T_OutputPin,
		public T_Threshold
	{
	public:
		_V_PROP_( Duration )
		_V_PROP_( Enabled )
		_V_PROP_( OutputPin )
		_V_PROP_( Threshold )

	public:
		bool GetIsEnabled()
		{
			return ( Enabled().GetValue() && T_OutputPin::GetPinIsConnected() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Mode2D,
		typename T_OutputPin,
		typename T_Threshold
	> class TArduinoLSM6DS3OrientationDetection :
		public T_Enabled,
		public T_Mode2D,
		public T_OutputPin,
		public T_Threshold
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Mode2D )
		_V_PROP_( Threshold )

	public:
		inline bool GetIsEnabled()
		{
			return ( Enabled().GetValue() && T_OutputPin::GetPinIsConnected() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_INTERFACE,
		typename T_Accelerometer,
		typename T_DataReadyMaskEnabled,
		typename T_DisableI2C,
		typename T_Enabled,
		typename T_FreeFall,
		typename T_Gyroscope,
		typename T_Orientation,
		typename T_Pedometer,
		typename T_SPI3Wire,
		typename T_Sleep,
		typename T_Tap,
		typename T_Thermometer
	> class STMicroelectronicsLSM6DS3 :
		public T_INTERFACE,
		public T_Accelerometer,
		public T_DataReadyMaskEnabled,
		public T_DisableI2C,
		public T_Enabled,
		public T_FreeFall,
		public T_Gyroscope,
		public T_Orientation,
		public T_Pedometer,
		public T_SPI3Wire,
		public T_Sleep,
		public T_Tap,
		public T_Thermometer
	{
	public:
		_V_PROP_( Accelerometer )
		_V_PROP_( DataReadyMaskEnabled )
		_V_PROP_( DisableI2C )
		_V_PROP_( Enabled )
		_V_PROP_( FreeFall )
		_V_PROP_( Gyroscope )
		_V_PROP_( Orientation )
		_V_PROP_( Pedometer )
		_V_PROP_( SPI3Wire )
		_V_PROP_( Sleep )
		_V_PROP_( Tap )
		_V_PROP_( Thermometer )

	public:
		void Update_ORIENT_CFG_G()
		{
			uint8_t AValue = Gyroscope().Orientation().Axes();
			if( Gyroscope().Orientation().InvertX() )
				AValue |= 0b00100000;

			if( Gyroscope().Orientation().InvertY() )
				AValue |= 0b00010000;

			if( Gyroscope().Orientation().InvertZ() )
				AValue |= 0b00001000;

			T_INTERFACE::WriteRegister8( LSM6DS3::ORIENT_CFG_G, AValue );
		}

		void Update_CTRL2_G()
		{
			uint8_t AValue;
			if( Gyroscope().PowerDown() )
				AValue = 0;

			else
			{
				if( Gyroscope().FullScaleRange() >= 2000.0 )
					AValue = 0b00001100;

				else if( Gyroscope().FullScaleRange() >= 1000.0 )
					AValue = 0b00001000;

				else if( Gyroscope().FullScaleRange() >= 500.0 )
					AValue = 0b00000100;

				else if( Gyroscope().FullScaleRange() >= 250.0 )
					AValue = 0b00000000;

				else
					AValue = 0b00000010;

				if( Gyroscope().SampleRate() >= 1660.0 )
					AValue |= 0b10000000;

				else if( Gyroscope().SampleRate() >= 833.0 )
					AValue |= 0b01110000;

				else if( Gyroscope().SampleRate() >= 416.0 )
					AValue |= 0b01100000;

				else if( Gyroscope().SampleRate() >= 208.0 )
					AValue |= 0b01010000;

				else if( Gyroscope().SampleRate() >= 104.0 )
					AValue |= 0b01000000;

				else if( Gyroscope().SampleRate() >= 52.0 )
					AValue |= 0b00110000;

				else if( Gyroscope().SampleRate() >= 26.0 )
					AValue |= 0b00100000;

				else
					AValue |= 0b00010000;

			}

			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL2_G, AValue );
		}

		void Update_CTRL1_XLInt( bool APowerDown )
		{
//			Serial.println( "Update_CTRL1_XL" );
			uint8_t AValue;
			if( APowerDown )
				AValue = 0;

			else
			{
				if( Accelerometer().FullScaleRange() >= 16 )
					AValue = 0b00000100;

				else if( Accelerometer().FullScaleRange() >= 8 )
					AValue = 0b00001100;

				else if( Accelerometer().FullScaleRange() >= 4 )
					AValue = 0b00001000;

				else
					AValue = 0b00000000;

				if( Accelerometer().SampleRate() >= 6660.0 )
					AValue |= 0b10100000;

				else if( Accelerometer().SampleRate() >= 3330.0 )
					AValue |= 0b10010000;

				else if( Accelerometer().SampleRate() >= 1660.0 )
					AValue |= 0b10000000;

				else if( Accelerometer().SampleRate() >= 833.0 )
					AValue |= 0b01110000;

				else if( Accelerometer().SampleRate() >= 416.0 )
					AValue |= 0b01100000;

				else if( Accelerometer().SampleRate() >= 208.0 )
					AValue |= 0b01010000;

				else if( Accelerometer().SampleRate() >= 104.0 )
					AValue |= 0b01000000;

				else if( Accelerometer().SampleRate() >= 52.0 )
					AValue |= 0b00110000;

				else if( Accelerometer().SampleRate() >= 26.0 )
					AValue |= 0b00100000;

				else
					AValue |= 0b00010000;

				if( Accelerometer().AntiAliasingFilter().Bandwidth() < 100.0 )
					AValue |= 0b00000011;

				else if( Accelerometer().AntiAliasingFilter().Bandwidth() < 200.0 )
					AValue |= 0b00000010;

				else if( Accelerometer().AntiAliasingFilter().Bandwidth() < 400.0 )
					AValue |= 0b00000001;

			}

//			Serial.println( "CTRL1_XL" );
//			Serial.println( AValue, BIN );
			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL1_XL, AValue );
		}

		inline void Update_CTRL1_XL()
		{
			Update_CTRL1_XLInt( Accelerometer().PowerDown().GetValue() );
		}

		void Update_CTRL3_CInt( bool AReset, bool ARebootMemory )
		{
			uint8_t AValue = ( AReset ) ? 0b00000101 : 0b00000100;
			
			if( ARebootMemory )
				AValue |= 0b10000000;

			if( SPI3Wire() )
				AValue |= 0b00001000;

			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL3_C, AValue );
		}

		void Update_CTRL3_C()
		{
			Update_CTRL3_CInt( false, false );
		}

		void Update_CTRL4_C()
		{
			uint8_t AValue = ( Gyroscope().Sleep() ) ? 0b01000000 : 0;

			if( Accelerometer().AntiAliasingFilter().Enabled() )
				AValue |= 0b10000000;

			if( DisableI2C() )
				AValue |= 0b00000100;

			if( DataReadyMaskEnabled() )
				AValue |= 0b00001000;
			
			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL4_C, AValue );
		}

		void Update_CTRL6_C()
		{
			Update_CTRL1_XLInt( true ); // Needed to update HighPerformance
			uint8_t AValue = ( Gyroscope().HighPerformance() ) ? 0 : 0b00010000;

			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL6_C, AValue );
			Update_CTRL1_XL();
		}

		void Update_CTRL7_G()
		{
			uint8_t AValue = ( Gyroscope().HighPerformance() ) ? 0b10001000 : 0b00001000; // Reset the filter

			if( Gyroscope().HighPassFilter().Enabled() )
			{
				AValue |= 0b01000000;

				if( Gyroscope().HighPassFilter().CutoffFrequency() >= 16.32 )
					AValue |= 0b00110000;

				else if( Gyroscope().HighPassFilter().CutoffFrequency() >= 2.07 )
					AValue |= 0b00100000;

				else if( Gyroscope().HighPassFilter().CutoffFrequency() >= 0.0324 )
					AValue |= 0b00010000;

				}

			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL7_G, AValue );
		}

		void Update_CTRL8_XL()
		{
			// Set the ODR config register to ODR/4
			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL8_XL, 0x09 ); // ?????
		}

		void Update_TAP_CFG()
		{
			uint8_t AValue = ( Pedometer().GetIsEnabled() ) ? 0b01000000 : 0b00000000;

			AValue |= Tap().GetTapsEnabled();

			if( Sleep().UseHighPassFilter() )
				AValue |= 0b00010000;

//			Serial.println( "Update_TAP_CFG" );
//			Serial.println( AValue, BIN );

			T_INTERFACE::WriteRegister8( LSM6DS3::TAP_CFG, AValue );
		}

		void Update_WAKE_UP_DUR()
		{
			uint8_t AValue = Sleep().Duration().Sleep() * 0b1111 + 0.5;

			uint8_t ASubValue = Sleep().Duration().Wake() * 0b11 + 0.5;
			AValue |= ( ASubValue << 5 );

			if( FreeFall().Duration().GetValue() >= 0.5 )
				AValue |= 0b10000000;

			T_INTERFACE::WriteRegister8( LSM6DS3::WAKE_UP_DUR, AValue );
		}

		void Update_WAKE_UP_THS()
		{
			uint8_t AValue = Sleep().Threshold().GetValue() * 0b111111;
			if( Tap().GetDoubleTapEnabled() ) 
				AValue |= 0b10000000;

			if( Sleep().GetIsEnabled() )
				AValue |= 0b01000000;

			T_INTERFACE::WriteRegister8( LSM6DS3::WAKE_UP_THS, AValue );
		}

		void Update_FREE_FALL()
		{
			uint8_t AValue = FreeFall().Threshold() * 0b111 + 0.5;
			uint8_t ASubValue = FreeFall().Duration() * 0b111111 + 0.5;
			AValue |= ( ASubValue << 3 );

			T_INTERFACE::WriteRegister8( LSM6DS3::FREE_FALL, AValue );
		}

		inline void Update_WAKE_UP_DUR__FREE_FALL()
		{
			Update_WAKE_UP_DUR();
			Update_FREE_FALL();
		}

		void Update_INT_DUR2()
		{
/*
		float ATimeUnit;
			if( Accelerometer().SampleRate() >= 6660.0 )
				ATimeUnit = 1000.0 / 6660.0;

			else if( Accelerometer().SampleRate() >= 3330.0 )
				ATimeUnit = 1000.0 / 3330.0;

			else if( Accelerometer().SampleRate() >= 1660.0 )
				ATimeUnit = 1000.0 / 1660.0;

			else if( Accelerometer().SampleRate() >= 833.0 )
				ATimeUnit = 1000.0 / 833.0;

			else if( Accelerometer().SampleRate() >= 416.0 )
				ATimeUnit = 1000.0 / 416.0;

			else if( Accelerometer().SampleRate() >= 208.0 )
				ATimeUnit = 1000.0 / 208.0;

			else if( Accelerometer().SampleRate() >= 104.0 )
				ATimeUnit = 1000.0 / 104.0;

			else if( Accelerometer().SampleRate() >= 52.0 )
				ATimeUnit = 1000.0 / 52.0;

			else if( Accelerometer().SampleRate() >= 26.0 )
				ATimeUnit = 1000.0 / 26.0;

			else
				ATimeUnit = 1000.0 / 12.5;

			Serial.println( "ATimeUnit" );
			Serial.println( ATimeUnit );

			float AFloatValue = Tap().DoubleTap().MaxGapDuration().GetValue();
			AFloatValue /= 32 * ATimeUnit;
			AFloatValue -= 16 * ATimeUnit;
			if( AFloatValue > 15 )
				AFloatValue = 15;

			uint8_t ASubValue = AFloatValue + 0.5;

			uint8_t AValue = ASubValue << 4;

			AFloatValue = Tap().Duration().Quiet();
			AFloatValue /= 4 * ATimeUnit;
			AFloatValue -= 2 * ATimeUnit;
			if( AFloatValue > 3 )
				AFloatValue = 3;

			ASubValue = AFloatValue + 0.5;

			AValue |= ( ASubValue << 2 );

			AFloatValue = Tap().Duration().Shock();
			AFloatValue /= 8 * ATimeUnit;
			AFloatValue -= 4 * ATimeUnit;
			if( AFloatValue > 3 )
				AFloatValue = 3;

			ASubValue = AFloatValue + 0.5;

			AValue |= ASubValue;
*/
			uint8_t ASubValue = ( Tap().DoubleTap().MaxGapDuration().GetValue() * 0b1111 ) + 0.5;
			uint8_t AValue = ASubValue << 4;

			ASubValue = ( Tap().Duration().Quiet() * 0b11 ) + 0.5;
			AValue |= ( ASubValue << 2 );

			ASubValue = ( Tap().Duration().Shock() * 0b11 ) + 0.5;
			AValue |= ASubValue;

//			AValue = 0b110;
//			Serial.println( "INT_DUR2" );
//			Serial.println( AValue, BIN );

			T_INTERFACE::WriteRegister8( LSM6DS3::INT_DUR2, AValue );
		}

		void Update_CTRL9_XL()
		{
			uint8_t AValue = 0;
			if( Accelerometer().Enabled() )
			{
				if( Tap().GetIsEnabled() || Pedometer().GetIsEnabled() || Sleep().GetIsEnabled() || FreeFall().GetIsEnabled() || Orientation().GetIsEnabled() )
					AValue |= 0b00111000;

				else
				{
					if( Accelerometer().OutputPins_X().GetPinIsConnected() )
						AValue |= 0b00001000;

					if( Accelerometer().OutputPins_Y().GetPinIsConnected() )
						AValue |= 0b00010000;

					if( Accelerometer().OutputPins_Z().GetPinIsConnected() )
						AValue |= 0b00100000;
				}
			}

			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL9_XL, AValue );
		}

		inline void Update_TAP_CFG__CTRL10_C__CTRL9_XL()
		{
			Update_TAP_CFG();
			Update_CTRL9_XL();
			Update_CTRL10_C();
		}

		inline void Update_WAKE_UP_THS__CTRL9_XL()
		{
			Update_WAKE_UP_THS();
			Update_CTRL9_XL();
		}

		void Update_TAP_THS_6D()
		{
			uint8_t AValue = ( Tap().Threshold().GetValue() * 0b00011111 ) + 0.5;

			if( Orientation().Mode2D().GetValue() )
				AValue |= 0b10000000;

			if( Orientation().Threshold() < 60 )
				AValue |= 0b01100000;

			else if( Orientation().Threshold() < 70 )
				AValue |= 0b01000000;

			else if( Orientation().Threshold() < 80 )
				AValue |= 0b00100000;

//			Serial.println( "TAP_THS_6D" );
//			Serial.println( AValue, BIN );
//			AValue = 0b1000;
//			Serial.println( "TAP_THS_6D" );
//			Serial.println( AValue, BIN );
			T_INTERFACE::WriteRegister8( LSM6DS3::TAP_THS_6D, AValue );
		}

		void Update_CTRL10_CInt( bool AReset )
		{
			uint8_t AValue = ( Pedometer().GetIsEnabled() ) ? 0b00000100 : 0;
			if( Gyroscope().Enabled() )
			{
				if( Gyroscope().OutputPins_X().GetPinIsConnected() )
					AValue |= 0b00001000;

				if( Gyroscope().OutputPins_Y().GetPinIsConnected() )
					AValue |= 0b00010000;

				if( Gyroscope().OutputPins_Z().GetPinIsConnected() )
					AValue |= 0b00100000;

			}

			if( AReset )
				AValue |= 0b00000010;

			T_INTERFACE::WriteRegister8( LSM6DS3::CTRL10_C, AValue );
		}

		inline void Update_CTRL10_C()
		{
			Update_CTRL10_CInt( false );
		}

		inline void Update_TAP_CFG__CTRL10_C()
		{
			Update_TAP_CFG();
			Update_CTRL10_C();
			Update_CTRL9_XL();
		}

		void EnableEmbedded()
		{
			T_INTERFACE::WriteRegister8( LSM6DS3::FUNC_CFG_ACCESS, 0b10000000 );
		}

		void DisableEmbedded()
		{
			T_INTERFACE::WriteRegister8( LSM6DS3::FUNC_CFG_ACCESS, 0 );
		}

		void Update_Direct_PEDO_THS_REG()
		{
			uint8_t AValue;
			if( Pedometer().MinThreshold() > 512 )
				AValue |= 0b10000000 | ( Pedometer().MinThreshold() / 32 );

			else
				AValue = Pedometer().MinThreshold() / 16;

			T_INTERFACE::WriteRegister8( LSM6DS3::PEDO_THS_REG, AValue );
		}

		void Update_PEDO_THS_REG()
		{
			EnableEmbedded();

			Update_Direct_PEDO_THS_REG();

			DisableEmbedded();
		}

		void Update_Direct_PEDO_DEB_REG()
		{
			uint8_t AValue = Pedometer().Threshold() | ( ( Pedometer().Time() / 80 ) << 3 );

			T_INTERFACE::WriteRegister8( LSM6DS3::PEDO_DEB_REG, AValue );
		}

		void Update_PEDO_DEB_REG()
		{
			EnableEmbedded();

			Update_Direct_PEDO_DEB_REG();

			DisableEmbedded();
		}

	protected:
		float AccelerationFromValue( float AValue )
		{
			AValue /= 32768.0;
			if( Accelerometer().FullScaleRange() >= 16 )
				return AValue * 16.0;

			if( Accelerometer().FullScaleRange() >= 8 )
				return AValue * 8.0;

			if( Accelerometer().FullScaleRange() >= 4 )
				return AValue * 4.0;

//			Serial.println( "TEST" );
//			delay( 1000 );

			return AValue * 2.0;
		}

		float AngleFromValue( float AValue )
		{
			AValue /= 32768.0;

			if( Gyroscope().FullScaleRange() >= 2000.0 )
				return AValue * 2000.0;

			if( Gyroscope().FullScaleRange() >= 1000.0 )
				return AValue * 1000.0;

			if( Gyroscope().FullScaleRange() >= 500.0 )
				return AValue * 500.0;

			if( Gyroscope().FullScaleRange() >= 250.0 )
				return AValue * 250.0;

			return AValue * 125.0;
		}

		void ReadSensors( bool AChangeOnly, bool AForceRead )
		{
			if( ! Enabled().GetValue() )
				return;

			bool ATemperatureData = AForceRead;
			bool AGyroscopeData = AForceRead; 
			bool AAccelerometerData = AForceRead; 
			if( ! AForceRead )
			{
				uint8_t AValue = T_INTERFACE::ReadRegister8( LSM6DS3::STATUS_REG );
//				Serial.println( AValue, BIN );
				uint8_t AMask = ( Thermometer().GetIsEnabled() ) ? 0b00000100 : 0;

				if( Gyroscope().GetIsEnabled() )
					AMask |= 0b00000010;

				if( Accelerometer().GetIsEnabled() )
					AMask |= 0b00000001;

				if( ( AValue & AMask ) != AMask )
					return;
			}

			if( Thermometer().GetIsEnabled() )
			{
				float AValue = int16_t( T_INTERFACE::ReadRegister16( LSM6DS3::OUT_TEMP_L ));
				AValue /= 16.0;
				AValue += 25.0;

				Thermometer().OutputPin().SetPinValue( AValue, AChangeOnly );
			}

			if( Accelerometer().GetIsEnabled() )
			{
				if( Accelerometer().OutputPins_X().GetPinIsConnected() )
				{
					uint8_t ACount;
					if( Accelerometer().OutputPins_Z().GetPinIsConnected() )
						ACount = 3;

					else if( Accelerometer().OutputPins_Y().GetPinIsConnected() )
						ACount = 2;

					else
						ACount = 1;

					T_INTERFACE::StartReadRegisters( LSM6DS3::OUTX_L_XL, ACount * 2 );
					int16_t AValues[ 3 ];

					for( int i = 0; i < ACount; ++i )
						AValues[ i ] = int16_t( T_INTERFACE::ReadUInt16());

					T_INTERFACE::EndReadRegisters();

					Accelerometer().OutputPins_X().SetPinValue( AccelerationFromValue( AValues[ 0 ] ));
					if( Accelerometer().OutputPins_Y().GetPinIsConnected() )
					{
						Accelerometer().OutputPins_Y().SetPinValue( AccelerationFromValue( AValues[ 1 ] ));
						if( Accelerometer().OutputPins_Z().GetPinIsConnected() )
							Accelerometer().OutputPins_Z().SetPinValue( AccelerationFromValue( AValues[ 2 ] ));
					}

					else
					{
						if( Accelerometer().OutputPins_Z().GetPinIsConnected() )
							Accelerometer().OutputPins_Z().SetPinValue( AccelerationFromValue( AValues[ 2 ] ));

					}
				}

				else if( Accelerometer().OutputPins_Y().GetPinIsConnected() )
				{
					uint8_t ACount;
					if( Accelerometer().OutputPins_Z().GetPinIsConnected() )
						ACount = 2;

					else
						ACount = 1;

					T_INTERFACE::StartReadRegisters( LSM6DS3::OUTY_L_XL, ACount * 2 );
					int16_t AValues[ 2 ];

					for( int i = 0; i < ACount; ++i )
						AValues[ i ] = int16_t( T_INTERFACE::ReadUInt16());

					T_INTERFACE::EndReadRegisters();

					Accelerometer().OutputPins_Y().SetPinValue( AccelerationFromValue( AValues[ 0 ] ));
					if( Accelerometer().OutputPins_Z().GetPinIsConnected() )
						Accelerometer().OutputPins_Z().SetPinValue( AccelerationFromValue( AValues[ 1 ] ));

				}

				else
					Accelerometer().OutputPins_Z().SetPinValue( AccelerationFromValue( int16_t( T_INTERFACE::ReadRegister16( LSM6DS3::OUTZ_L_XL ))));

			}

			if( Gyroscope().GetIsEnabled() )
			{
				if( Gyroscope().OutputPins_X().GetPinIsConnected() )
				{
					uint8_t ACount;
					if( Gyroscope().OutputPins_Z().GetPinIsConnected() )
						ACount = 3;

					else if( Gyroscope().OutputPins_Y().GetPinIsConnected() )
						ACount = 2;

					else
						ACount = 1;

					T_INTERFACE::StartReadRegisters( LSM6DS3::OUTX_L_G, ACount * 2 );
					int16_t AValues[ 3 ];

					for( int i = 0; i < ACount; ++i )
						AValues[ i ] = int16_t( T_INTERFACE::ReadUInt16());

					T_INTERFACE::EndReadRegisters();

					Gyroscope().OutputPins_X().SetPinValue( AngleFromValue( AValues[ 0 ] ));
					if( Gyroscope().OutputPins_Y().GetPinIsConnected() )
					{
						Gyroscope().OutputPins_Y().SetPinValue( AngleFromValue( AValues[ 1 ] ));
						if( Gyroscope().OutputPins_Z().GetPinIsConnected() )
							Gyroscope().OutputPins_Z().SetPinValue( AngleFromValue( AValues[ 2 ] ));
					}

					else
					{
						if( Gyroscope().OutputPins_Z().GetPinIsConnected() )
							Gyroscope().OutputPins_Z().SetPinValue( AngleFromValue( AValues[ 2 ] ));

					}

					T_INTERFACE::EndReadRegisters();
				}

				else if( Gyroscope().OutputPins_Y().GetPinIsConnected() )
				{
					uint8_t ACount;
					if( Gyroscope().OutputPins_Z().GetPinIsConnected() )
						ACount = 2;

					else
						ACount = 1;

					T_INTERFACE::StartReadRegisters( LSM6DS3::OUTY_L_G, ACount * 2 );
					int16_t AValues[ 2 ];

					for( int i = 0; i < ACount; ++i )
						AValues[ i ] = int16_t( T_INTERFACE::ReadUInt16());

					T_INTERFACE::EndReadRegisters();

					Gyroscope().OutputPins_Y().SetPinValue( AngleFromValue( AValues[ 0 ] ));
					if( Gyroscope().OutputPins_Z().GetPinIsConnected() )
						Gyroscope().OutputPins_Z().SetPinValue( AngleFromValue( AValues[ 1 ] ));

					T_INTERFACE::EndReadRegisters();
				}

				else
					Gyroscope().OutputPins_Z().SetPinValue( AngleFromValue( int16_t( T_INTERFACE::ReadRegister16( LSM6DS3::OUTZ_L_G ))));

			}

		}

		void ReadFunctions( bool AChangeOnly )
		{
			if( Pedometer().GetIsEnabled() )
			{
				if( Pedometer().CountOutputPin().GetPinIsConnected() )
				{
					uint32_t ACount = T_INTERFACE::ReadRegister16( LSM6DS3::STEP_COUNTER_L );
					Pedometer().CountOutputPin().SetPinValue( ACount, AChangeOnly );
				}
			}

			if( ( Pedometer().GetIsEnabled() && ( Pedometer().StepOutputPin().GetPinIsConnected() || Pedometer().OverflowOutputPin().GetPinIsConnected() ) ) /* || Tili().GetIsEnabled() */ )
			{
				uint8_t AValue = T_INTERFACE::ReadRegister8( LSM6DS3::FUNC_SRC );
				Pedometer().StepOutputPin().SetPinValue( ( AValue & 0b00010000 ) != 0 );
				Pedometer().OverflowOutputPin().SetPinValue( ( AValue & 0b00001000 ) != 0 );
			}

			if( Tap().GetIsEnabled() )
			{
				uint8_t AValue = T_INTERFACE::ReadRegister8( LSM6DS3::TAP_SRC );


//				if( AValue ) Serial.println( AValue, BIN );


				if( AValue & 0b01000000 )
					Tap().ProcessTaps( AValue );

			}

			if( Sleep().GetIsEnabled() || FreeFall().GetIsEnabled() )
			{
				uint8_t AValue = T_INTERFACE::ReadRegister8( LSM6DS3::WAKE_UP_SRC );

//				if( AValue ) Serial.println( AValue, BIN );

				FreeFall().OutputPin().SetPinValue( ( AValue & 0b00100000 ) != 0 );					

				if( AValue & 0b00010000 )
					Sleep().OutputPin().SetPinValue( true );

				if( AValue & 0b00001000 )
					Sleep().OutputPin().SetPinValue( false );

				if( AValue & 0b00000100 )
					Sleep().WakeUpOutputPins_X().ClockPin();

				if( AValue & 0b00000010 )
					Sleep().WakeUpOutputPins_Y().ClockPin();

				if( AValue & 0b00000001 )
					Sleep().WakeUpOutputPins_Z().ClockPin();

			}

			if( Orientation().GetIsEnabled() )
			{
				uint8_t AValue = T_INTERFACE::ReadRegister8( LSM6DS3::D6D_SRC );
				TArduino3DOrientation::TArduino3DOrientation AOrientation;
				if( AValue & 0b00100000 )
					AOrientation = TArduino3DOrientation::Front;

				else if( AValue & 0b00010000 )
					AOrientation = TArduino3DOrientation::Back;

				else if( AValue & 0b00001000 )
					AOrientation = TArduino3DOrientation::Up;

				else if( AValue & 0b00000100 )
					AOrientation = TArduino3DOrientation::Down;

				else if( AValue & 0b00000010 )
					AOrientation = TArduino3DOrientation::Left;

				else if( AValue & 0b00000001 )
					AOrientation = TArduino3DOrientation::Right;

				Orientation().OutputPin().SetPinValue( AOrientation );
			}
		}

	public:
		inline void GyroscopeHighPassFilter_ResetInputPin_o_Receive( void * )
		{
			Update_CTRL7_G();
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Update_CTRL3_CInt( true, false );
		}

		inline void RebootMemoryInputPin_o_Receive( void *_Data )
		{
			Update_CTRL3_CInt( false, true );
		}

		inline void Pedometer_ResetInputPin_o_Receive( void *_Data )
		{
			Update_CTRL10_CInt( true );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false, true );
		}

	public:
		inline void SystemInit()
		{
			T_INTERFACE::Init();

			delay( 20 ); // Boot time

			EnableEmbedded();

			Update_Direct_PEDO_THS_REG();
			Update_Direct_PEDO_DEB_REG();

			DisableEmbedded();

			Update_ORIENT_CFG_G();
			Update_CTRL1_XL();
			Update_CTRL2_G();
			Update_CTRL3_C();
			Update_CTRL4_C();
			Update_CTRL6_C();
			Update_CTRL7_G();
			Update_CTRL8_XL();
			Update_CTRL9_XL();
			Update_TAP_THS_6D();
			Update_TAP_CFG();
			Update_WAKE_UP_THS();
			Update_WAKE_UP_DUR();
			Update_FREE_FALL();
			Update_INT_DUR2();
			Update_CTRL10_CInt( true );
		}

		inline void SystemStart()
		{
//			Serial.println( T_INTERFACE::ReadRegister8( LSM6DS3::WHO_AM_I ), HEX );
			ReadSensors( false, false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true, false );
		}

		inline void SystemStartFunctions()
		{
			Sleep().OutputPin().SetPinValue( true );
			ReadFunctions( false );
		}

		inline void SystemLoopBeginFunctions()
		{
			ReadFunctions( true );
		}

	};
//---------------------------------------------------------------------------
}

