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
namespace TArduinoMMA845XQAccelerometerFilterFrequency
{
	enum TArduinoMMA845XQAccelerometerFilterFrequency { Low, MediulLow, MediulHigh, High };
}
//---------------------------------------------------------------------------
	template <
		typename T_AutoSleep,
		typename T_Mode,
		typename T_SampleFrequency
	> class TArduinoMMA845XQAccelerometerSleepMode :
		public T_AutoSleep,
		public T_Mode,
		public T_SampleFrequency
	{
	public:
		_V_PROP_( AutoSleep )
		_V_PROP_( Mode )
		_V_PROP_( SampleFrequency )

	};
//---------------------------------------------------------------------------
	template <
		typename T_HighResolution,
		typename T_LowPower
	> class TArduinoMMA845XQAccelerometerMode :
		public T_HighResolution,
		public T_LowPower
	{
	public:
		_V_PROP_( HighResolution )
		_V_PROP_( LowPower )

	public:
		inline uint8_t GetRegisterValue()
		{
			uint8_t AResult = ( HighResolution() ) ? 0b10 : 0;

			if( LowPower() )
				AResult |= 0b01;

			return AResult;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_DoubleOutputPins_Up,
		typename T_DoubleOutputPins_Down,
		uint8_t C_INDEX,
		typename T_SingleOutputPins_Up,
		typename T_SingleOutputPins_Down,
		typename T_Threshold
	> class TArduinoMMA845XQAccelerometerTapChannel :
		public T_DoubleOutputPins_Up,
		public T_DoubleOutputPins_Down,
		public T_SingleOutputPins_Up,
		public T_SingleOutputPins_Down,
		public T_Threshold
	{
	public:
		_V_PIN_( DoubleOutputPins_Up );
		_V_PIN_( DoubleOutputPins_Down );
		_V_PIN_( SingleOutputPins_Up );
		_V_PIN_( SingleOutputPins_Down );

	public:
		_V_PROP_( Threshold )

	public:
		void GetTapsEnabled( uint8_t &ATapsEnabled )
		{
			if( T_DoubleOutputPins_Up::GetPinIsConnected() || T_DoubleOutputPins_Down::GetPinIsConnected() )
				ATapsEnabled |= 0b10 << ( 2 * C_INDEX );

			if( T_SingleOutputPins_Up::GetPinIsConnected() || T_SingleOutputPins_Down::GetPinIsConnected() )
				ATapsEnabled |= 0b01 << ( 2 * C_INDEX );

		}

		void GetThreshold( uint8_t &AThreshold )
		{
			AThreshold = Threshold().GetValue() * 127 + 0.5;
		}

		void ProcessOutputs( uint8_t ARegisterValue )
		{
			if( ARegisterValue & ( 0b00010001 << C_INDEX ))
			{
				if( ARegisterValue & 0b00001000 )
					T_DoubleOutputPins_Down::ClockPin();

				else
					T_SingleOutputPins_Down::ClockPin();

			}

			else if( ARegisterValue & ( 0b00010000 << C_INDEX ))
			{
				if( ARegisterValue & 0b00001000 )
					T_DoubleOutputPins_Up::ClockPin();

				else
					T_SingleOutputPins_Up::ClockPin();

			}


		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Axes_GetTapsEnabled,
		typename T_Axes_GetThreshold,
		typename T_Axes_ProcessOutputs,
		typename T_IgnoreInterval,
		typename T_MaxDoubleClickInterval,
		typename T_MaxTapDuration
	> class TArduinoMMA845XQAccelerometerTap :
		public T_IgnoreInterval,
		public T_MaxDoubleClickInterval,
		public T_MaxTapDuration
	{
	public:
		_V_PROP_( IgnoreInterval )
		_V_PROP_( MaxDoubleClickInterval )
		_V_PROP_( MaxTapDuration )

	public:
		inline void Axes_GetTapsEnabled_ChainCall( uint8_t &ATapsEnabled )
		{
			T_Axes_GetTapsEnabled::Call( ATapsEnabled );
		}

		inline void Axes_GetThreshold_ChainCall( int32_t AIndex, uint8_t & AThreshold )
		{
			T_Axes_GetThreshold::Call( AIndex, AThreshold );
		}

		inline void Axes_ProcessOutput_ChainCall( uint8_t ARegisterValue )
		{
			T_Axes_ProcessOutputs::Call( ARegisterValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Bypass,
		typename T_Enabled
	> class TArduinoMMA845XQAccelerometerPulseProcessing :
		public T_Bypass,
		public T_Enabled
	{
	public:
		_V_PROP_( Bypass )
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DoubleTap,
		typename T_Interval
	> class TArduinoMMA845XQAccelerometerTapIgnoreInterval :
		public T_DoubleTap,
		public T_Interval
	{
	public:
		_V_PROP_( DoubleTap )
		_V_PROP_( Interval )
	};
//---------------------------------------------------------------------------
	template <
		typename T_CutoffFrequency,
		typename T_Enabled,
		typename T_PulseProcessing
	> class TArduinoMMA845XQAccelerometerFilter :
		public T_CutoffFrequency,
		public T_Enabled,
		public T_PulseProcessing
	{
	public:
		_V_PROP_( CutoffFrequency )
		_V_PROP_( Enabled )
		_V_PROP_( PulseProcessing )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FrontDownOutputPin,
		typename T_LockoutOutputPin,
		typename T_OutputPin,
		typename T_ResetCounter
	> class TArduinoMMA845XQAccelerometerOrientationDetection :
		public T_Enabled,
		public T_FrontDownOutputPin,
		public T_LockoutOutputPin,
		public T_OutputPin,
		public T_ResetCounter
	{
	public:
		_V_PIN_( FrontDownOutputPin )
		_V_PIN_( LockoutOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( ResetCounter )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_Filter,
		typename T_FullScaleRange,
		typename T_Mode,
		typename T_Offsets,
		typename T_Orientation,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_ReducedNoise,
		typename T_SampleFrequency,
		typename T_SleepMode,
		typename T_Standby,
		typename T_Tap
//		typename T_Tap_GetParameters
	> class AccelerometerMMA845X :
		public T_Address,
		public T_Enabled,
		public T_Filter,
		public T_FullScaleRange,
		public T_Mode,
		public T_Offsets,
		public T_Orientation,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_ReducedNoise,
		public T_SampleFrequency,
		public T_SleepMode,
		public T_Standby,
		public T_Tap
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Filter )
		_V_PROP_( FullScaleRange )
		_V_PROP_( Mode )
		_V_PROP_( Offsets )
		_V_PROP_( Orientation )
		_V_PROP_( ReducedNoise )
		_V_PROP_( SampleFrequency )
		_V_PROP_( SleepMode )
		_V_PROP_( Standby )
		_V_PROP_( Tap )

	protected:
		enum MMA8452Q_Register
		{
			STATUS = 0x00,
			OUT_X_MSB = 0x01,
			OUT_X_LSB = 0x02,
			OUT_Y_MSB = 0x03,
			OUT_Y_LSB = 0x04,
			OUT_Z_MSB = 0x05,
			OUT_Z_LSB = 0x06,
			SYSMOD = 0x0B,
			INT_SOURCE = 0x0C,
			WHO_AM_I = 0x0D,
			XYZ_DATA_CFG = 0x0E,
			HP_FILTER_CUTOFF = 0x0F,
			PL_STATUS = 0x10,
			PL_CFG = 0x11,
			PL_COUNT = 0x12,
			PL_BF_ZCOMP = 0x13,
			P_L_THS_REG = 0x14,
			FF_MT_CFG = 0x15,
			FF_MT_SRC = 0x16,
			FF_MT_THS = 0x17,
			FF_MT_COUNT = 0x18,
			TRANSIENT_CFG = 0x1D,
			TRANSIENT_SRC = 0x1E,
			TRANSIENT_THS = 0x1F,
			TRANSIENT_COUNT = 0x20,
			PULSE_CFG = 0x21,
			PULSE_SRC = 0x22,
			PULSE_THSX = 0x23,
			PULSE_THSY = 0x24,
			PULSE_THSZ = 0x25,
			PULSE_TMLT = 0x26,
			PULSE_LTCY = 0x27,
			PULSE_WIND = 0x28,
			ASLP_COUNT = 0x29,
			CTRL_REG1 = 0x2A,
			CTRL_REG2 = 0x2B,
			CTRL_REG3 = 0x2C,
			CTRL_REG4 = 0x2D,
			CTRL_REG5 = 0x2E,
			OFF_X = 0x2F,
			OFF_Y = 0x30,
			OFF_Z = 0x31
		};

	public:
		inline void UpdateConfigReg1()
		{
			SetToStandby( true );

			SetToStandby( true ); // Just to make sure the settings take place!

			SetToStandby( false );
		}

		inline void Update_XYZ_DATA_CFG()
		{
			SetToStandby( true );

			Update_XYZ_DATA_CFG_Internal();

			SetToStandby( false );
		}

		inline void Update_PL_CFG()
		{
			SetToStandby( true );

			Update_PL_CFG_Internal();

			SetToStandby( false );
		}

		inline void Update_PULSE_THSX()
		{
			SetToStandby( true );

			Update_PULSE_THSX_Internal();

			SetToStandby( false );
		}

		inline void Update_PULSE_THSY()
		{
			SetToStandby( true );

			Update_PULSE_THSY_Internal();

			SetToStandby( false );
		}

		inline void Update_PULSE_THSZ()
		{
			SetToStandby( true );

			Update_PULSE_THSZ_Internal();

			SetToStandby( false );
		}

		inline void Update_PULSE_LTCY()
		{
			SetToStandby( true );

			Update_PULSE_LTCY_Internal();

			SetToStandby( false );
		}

		inline void Update_PULSE_CFG()
		{
			SetToStandby( true );

			Update_PULSE_CFG_Internal();

			SetToStandby( false );
		}

		inline void Update_PULSE_WIND()
		{
			SetToStandby( true );

			Update_PULSE_WIND_Internal();

			SetToStandby( false );
		}

		inline void Update_PULSE_TMLT()
		{
			SetToStandby( true );

			Update_PULSE_TMLT_Internal();

			SetToStandby( false );
		}

		inline void Update_CTRL_REG2()
		{
			SetToStandby( true );

			Update_CTRL_REG2_Internal( false );

			SetToStandby( false );
		}

		inline void Update_OFF_X()
		{
			Write8bit( OFF_X, uint8_t( int8_t( MitovConstrain<float>( Offsets().X(), -256, 255 ) / 2 )));
		}

		inline void Update_OFF_Y()
		{
			Write8bit( OFF_Y, uint8_t( int8_t( MitovConstrain<float>( Offsets().X(), -256, 255 ) / 2 )));
		}

		inline void Update_OFF_Z()
		{
			Write8bit( OFF_Z, uint8_t( int8_t( MitovConstrain<float>( Offsets().X(), -256, 255 ) / 2 )));
		}

		inline void Update_HP_FILTER_CUTOFF()
		{
			Update_HP_FILTER_CUTOFF_Internal();
		}

	protected:
		uint8_t Read8bit( uint8_t ARegisterAddress )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegisterAddress );  
			C_I2C.endTransmission( false );
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ));
			return C_I2C.read();
		}

		void Write8bit( uint8_t ARegisterAddress, uint8_t AValue )
		{
/*
	Serial.print( "Write " );
	Serial.print( ARegisterAddress, HEX );
	Serial.print( " = " );
	Serial.println( AValue, HEX );
*/
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegisterAddress );  
			C_I2C.write( AValue ); 
			C_I2C.endTransmission();
		}

		void BeginRead( uint8_t ARegisterAddress, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegisterAddress );  
			C_I2C.endTransmission( false );
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), ACount );
		}

		uint16_t ReadSingle16bit()
		{
			uint16_t AResult = uint16_t( C_I2C.read() ) << 8;
			AResult |= C_I2C.read();
			return AResult;
		}

	protected:
		void SetToStandby( bool AValue )
		{
			uint8_t ARegisterValue = ( AValue ) ? 0 : 1;

//			if( FastRead() )
//				ARegisterValue != 0b00000010;

			if( ReducedNoise() )
				ARegisterValue != 0b00000100;


			if( SampleFrequency() < 6.25 )
				ARegisterValue != 0b00111000;

			else if( SampleFrequency() < 12.5 )
				ARegisterValue != 0b00110000;

			else if( SampleFrequency() < 50.0 )
				ARegisterValue != 0b00101000;

			else if( SampleFrequency() < 100.0 )
				ARegisterValue != 0b00100000;

			else if( SampleFrequency() < 200.0 )
				ARegisterValue != 0b00011000;

			else if( SampleFrequency() < 400.0 )
				ARegisterValue != 0b00010000;

			else if( SampleFrequency() < 800.0 )
				ARegisterValue != 0b00001000;


			if( SleepMode().SampleFrequency() < 6.25 )
				ARegisterValue != 0b11000000;

			else if( SleepMode().SampleFrequency() < 12.5 )
				ARegisterValue != 0b10000000;

			else if( SleepMode().SampleFrequency() < 50 )
				ARegisterValue != 0b01000000;

			Write8bit( CTRL_REG1, ARegisterValue );
		}

		void Update_CTRL_REG2_Internal( bool AReset )
		{
			uint8_t ARegisterValue = ( AReset ) ? 0b01000000 : 0;

			if( SleepMode().AutoSleep() )
				ARegisterValue |= 0b00000100;

			ARegisterValue |= Mode().GetRegisterValue();
			ARegisterValue |= ( SleepMode().Mode().GetRegisterValue() << 3 );

			Write8bit( CTRL_REG2, ARegisterValue );
		}

		void Update_HP_FILTER_CUTOFF_Internal()
		{
			uint8_t ARegisterValue = ( Filter().PulseProcessing().Bypass() ) ? 0b00100000 : 0;

			if( Filter().PulseProcessing().Enabled() )
				ARegisterValue |= 0b00010000;

			switch( Filter().CutoffFrequency().GetValue() )
			{
				case TArduinoMMA845XQAccelerometerFilterFrequency::Low :		ARegisterValue |= 0b00000011; break;
				case TArduinoMMA845XQAccelerometerFilterFrequency::MediulLow :	ARegisterValue |= 0b00000010; break;
				case TArduinoMMA845XQAccelerometerFilterFrequency::MediulHigh :	ARegisterValue |= 0b00000001; break;
			}

			Write8bit( XYZ_DATA_CFG, ARegisterValue );
		}

		void Update_PL_CFG_Internal()
		{
			uint8_t ARegisterValue = ( Orientation().Enabled() ) ? 0b01000000 : 0;

			if( Orientation().ResetCounter() )
				ARegisterValue |= 0b10000000;

			Write8bit( PL_CFG, ARegisterValue );
		}

		void Update_PULSE_THSX_Internal()
		{
			uint8_t ARegisterValue = 0;
			Tap().Axes_GetThreshold_ChainCall( 0, ARegisterValue );

			Write8bit( PULSE_THSX, ARegisterValue );
		}

		void Update_PULSE_THSY_Internal()
		{
			uint8_t ARegisterValue = 0;
			Tap().Axes_GetThreshold_ChainCall( 1, ARegisterValue );

			Write8bit( PULSE_THSY, ARegisterValue );
		}

		void Update_PULSE_THSZ_Internal()
		{
			uint8_t ARegisterValue = 0;
			Tap().Axes_GetThreshold_ChainCall( 2, ARegisterValue );

			Write8bit( PULSE_THSZ, ARegisterValue );
		}

		void Update_PULSE_LTCY_Internal()
		{
			uint8_t ARegisterValue;
			if( Filter().PulseProcessing().Enabled() )
			{
				if( SampleFrequency() < 12.5 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 81.6f ) / 81.6f ) * 255 + 0.5;

				else if( SampleFrequency() < 50.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 40.8 ) / 40.8 ) * 255 + 0.5;

				else if( SampleFrequency() < 100.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 10.2 ) / 10.2 ) * 255 + 0.5;

				else if( SampleFrequency() < 200.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 5.1 ) / 5.1 ) * 255 + 0.5;

				else if( SampleFrequency() < 400.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 2.56 ) / 2.56 ) * 255 + 0.5;

				else if( SampleFrequency() < 800.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 1.276 ) / 1.276 ) * 255 + 0.5;

				else
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 0.638 ) / 0.638 ) * 255 + 0.5;

			}

			else
			{
				if( SampleFrequency() < 50.0 )
					ARegisterValue = ( MitovConstrain<float>(Tap().IgnoreInterval().Interval(), 0, 20.4 ) / 20.4 ) * 255 + 0.5;

				else if( SampleFrequency() < 100.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 5.1 ) / 5.1 ) * 255 + 0.5;

				else if( SampleFrequency() < 200.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 2.56 ) / 2.56 ) * 255 + 0.5;

				else if( SampleFrequency() < 400.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 1.276 ) / 1.276 ) * 255 + 0.5;

				else if( SampleFrequency() < 800.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 0.638 ) / 0.638 ) * 255 + 0.5;

				else
					ARegisterValue = ( MitovConstrain<float>( Tap().IgnoreInterval().Interval(), 0, 0.318 ) / 0.318 ) * 255 + 0.5;

			}

			Write8bit( PULSE_LTCY, ARegisterValue );
		}

		void Update_PULSE_WIND_Internal()
		{
			uint8_t ARegisterValue;
			if( Filter().PulseProcessing().Enabled() )
			{
				if( SampleFrequency() < 12.5 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 81.6f ) / 81.6f ) * 255 + 0.5;

				else if( SampleFrequency() < 50.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 40.8 ) / 40.8 ) * 255 + 0.5;

				else if( SampleFrequency() < 100.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 10.2 ) / 10.2 ) * 255 + 0.5;

				else if( SampleFrequency() < 200.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 5.1 ) / 5.1 ) * 255 + 0.5;

				else if( SampleFrequency() < 400.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 2.56 ) / 2.56 ) * 255 + 0.5;

				else if( SampleFrequency() < 800.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 1.276 ) / 1.276 ) * 255 + 0.5;

				else
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 0.638 ) / 0.638 ) * 255 + 0.5;

			}

			else
			{
				if( SampleFrequency() < 50.0 )
					ARegisterValue = ( MitovConstrain<float>(Tap().MaxDoubleClickInterval(), 0, 20.4 ) / 20.4 ) * 255 + 0.5;

				else if( SampleFrequency() < 100.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 5.1 ) / 5.1 ) * 255 + 0.5;

				else if( SampleFrequency() < 200.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 2.56 ) / 2.56 ) * 255 + 0.5;

				else if( SampleFrequency() < 400.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 1.276 ) / 1.276 ) * 255 + 0.5;

				else if( SampleFrequency() < 800.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 0.638 ) / 0.638 ) * 255 + 0.5;

				else
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxDoubleClickInterval(), 0, 0.318 ) / 0.318 ) * 255 + 0.5;

			}

			Write8bit( PULSE_WIND, ARegisterValue );
		}

		void Update_PULSE_TMLT_Internal()
		{
			uint8_t ARegisterValue;
			if( Filter().PulseProcessing().Enabled() )
			{
				if( SampleFrequency() < 12.5 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 81.6f ) / 81.6f ) * 255 + 0.5;

				else if( SampleFrequency() < 50.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 40.8 ) / 40.8 ) * 255 + 0.5;

				else if( SampleFrequency() < 100.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 10.2 ) / 10.2 ) * 255 + 0.5;

				else if( SampleFrequency() < 200.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 5.1 ) / 5.1 ) * 255 + 0.5;

				else if( SampleFrequency() < 400.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 2.56 ) / 2.56 ) * 255 + 0.5;

				else if( SampleFrequency() < 800.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 1.276 ) / 1.276 ) * 255 + 0.5;

				else
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 0.638 ) / 0.638 ) * 255 + 0.5;

			}

			else
			{
				if( SampleFrequency() < 50.0 )
					ARegisterValue = ( MitovConstrain<float>(Tap().MaxTapDuration(), 0, 20.4 ) / 20.4 ) * 255 + 0.5;

				else if( SampleFrequency() < 100.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 5.1 ) / 5.1 ) * 255 + 0.5;

				else if( SampleFrequency() < 200.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 2.56 ) / 2.56 ) * 255 + 0.5;

				else if( SampleFrequency() < 400.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 1.276 ) / 1.276 ) * 255 + 0.5;

				else if( SampleFrequency() < 800.0 )
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 0.638 ) / 0.638 ) * 255 + 0.5;

				else
					ARegisterValue = ( MitovConstrain<float>( Tap().MaxTapDuration(), 0, 0.318 ) / 0.318 ) * 255 + 0.5;

			}

			Write8bit( PULSE_TMLT, ARegisterValue );
		}

		void Update_PULSE_CFG_Internal()
		{
			uint8_t ARegisterValue = ( Tap().IgnoreInterval().DoubleTap() ) ? 0b01000000 : 0b11000000;
			Tap().Axes_GetTapsEnabled_ChainCall( ARegisterValue );
			Write8bit( PULSE_CFG, ARegisterValue );
		}

		void Update_XYZ_DATA_CFG_Internal()
		{
			uint8_t ARegisterValue = ( Filter().Enabled() ) ? 0b00010000 : 0;

			if( FullScaleRange() > 4 )
				ARegisterValue |= 0b10;

			else if( FullScaleRange() > 2 )
				ARegisterValue |= 0b01;

			Write8bit( XYZ_DATA_CFG, ARegisterValue );
		}

		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			float ACoefficient;
			if( FullScaleRange() < 4 )
				ACoefficient = 2.0 / 32768.0;

			else if( FullScaleRange() < 8 )
				ACoefficient = 4.0 / 32768.0;

			else
				ACoefficient = 8.0 / 32768.0;

			BeginRead( OUT_X_MSB, 6 );
			float AValueX = int16_t( ReadSingle16bit() ) * ACoefficient;
			float AValueY = int16_t( ReadSingle16bit() ) * ACoefficient;
			float AValueZ = int16_t( ReadSingle16bit() ) * ACoefficient;

			OutputPinsX().SetPinValue( AValueX, AChangeOnly );
			OutputPinsY().SetPinValue( AValueY, AChangeOnly );
			OutputPinsZ().SetPinValue( AValueZ, AChangeOnly );

			uint8_t ATapRegister = Read8bit( PULSE_SRC );
			if( ATapRegister & 0b10000000 )
				Tap().Axes_ProcessOutput_ChainCall( ATapRegister );

			uint8_t AOrientationStatus = Read8bit( PL_STATUS );
			Orientation().FrontDownOutputPin().SetPinValue( (( AOrientationStatus & 0b00000001 ) != 0 ), AChangeOnly );
			Orientation().LockoutOutputPin().SetPinValue( (( AOrientationStatus & 0b01000000 ) != 0 ), AChangeOnly );
//			Serial.println( AOrientationStatus, BIN );
			switch( AOrientationStatus & 0b00000110 )
			{
				case 0b00000000 : Orientation().OutputPin().SetPinValue( goUp, AChangeOnly ); break;
				case 0b00000010 : Orientation().OutputPin().SetPinValue( goDown, AChangeOnly ); break;
				case 0b00000100 : Orientation().OutputPin().SetPinValue( goRight, AChangeOnly ); break;
				case 0b00000110 : Orientation().OutputPin().SetPinValue( goLeft, AChangeOnly ); break;
			}

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Update_CTRL_REG2_Internal( true );
		}

	public:
		inline void SystemStartInit()
		{
			SetToStandby( true );

			SetToStandby( true ); // Just to make sure the settings take place!

			Update_CTRL_REG2_Internal( false );

			Update_OFF_X();
			Update_OFF_Y();
			Update_OFF_Z();

			Update_XYZ_DATA_CFG_Internal();
			Update_PL_CFG_Internal();
			Update_PULSE_THSX_Internal();
			Update_PULSE_THSY_Internal();
			Update_PULSE_THSZ_Internal();

			Update_PULSE_CFG_Internal();
			Update_PULSE_TMLT_Internal();
			Update_PULSE_LTCY_Internal();
			Update_PULSE_WIND_Internal();

			SetToStandby( false );
		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
//			if( DataAvailable() )
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			if( Enabled().GetValue() )
				if( Read8bit( STATUS ) && 0b00001000 )
					ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
}


