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
#include <SPI.h>
#include <Mitov_BasicMPU.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __ADXL345O__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
namespace ADXL345
{
	const uint8_t DEVID				= 0x00;
//	const uint8_t RESERVED1			= 0x01;
	const uint8_t THRESH_TAP		= 0x1d;
	const uint8_t OFSX				= 0x1e;
	const uint8_t OFSY				= 0x1f;
	const uint8_t OFSZ				= 0x20;
	const uint8_t DUR				= 0x21;
	const uint8_t LATENT			= 0x22;
	const uint8_t WINDOW			= 0x23;
	const uint8_t THRESH_ACT		= 0x24;
	const uint8_t THRESH_INACT		= 0x25;
	const uint8_t TIME_INACT		= 0x26;
	const uint8_t ACT_INACT_CTL		= 0x27;
	const uint8_t THRESH_FF			= 0x28;
	const uint8_t TIME_FF			= 0x29;
	const uint8_t TAP_AXES			= 0x2a;
	const uint8_t ACT_TAP_STATUS	= 0x2b;
	const uint8_t BW_RATE			= 0x2c;
	const uint8_t POWER_CTL			= 0x2d;
	const uint8_t INT_ENABLE		= 0x2e;
	const uint8_t INT_MAP			= 0x2f;
	const uint8_t INT_SOURCE		= 0x30;
	const uint8_t DATA_FORMAT		= 0x31;
	const uint8_t DATAX0			= 0x32;
	const uint8_t DATAX1			= 0x33;
	const uint8_t DATAY0			= 0x34;
	const uint8_t DATAY1			= 0x35;
	const uint8_t DATAZ0			= 0x36;
	const uint8_t DATAZ1			= 0x37;
	const uint8_t FIFO_CTL			= 0x38;
	const uint8_t FIFO_STATUS		= 0x39;
}
//---------------------------------------------------------------------------
/*
namespace TArduinoAccelerometerLIS3DHDataRate
{
	enum TArduinoAccelerometerLIS3DHDataRate
	{
		Normal_1_Hz,
		Normal_10_Hz,
		Normal_25_Hz,
		Normal_50_Hz,
		Normal_100_Hz,
		Normal_200_Hz,
		Normal_400_Hz_LowPower_1_60_kHz,
		Normal_1_344_kHz_LowPower_5_376_kHz
	};
}
*/
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_Threshold,
		typename T_Time
	> class TArduinoAccelerometerADXL345FreeFall :
		public T_OutputPin,
		public T_Threshold,
		public T_Time
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Threshold )
		_V_PROP_( Time )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class TArduinoAccelerometerADXL345Offsets :
		public T_Enabled,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	};
//---------------------------------------------------------------------------
	template <
		typename T_AutoSleep,
		typename T_OutputPin,
		typename T_SamplingRate,
		typename T_Sleep
	> class TArduinoAccelerometerADXL345Sleep :
		public T_AutoSleep,
		public T_OutputPin,
		public T_SamplingRate,
		public T_Sleep
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( AutoSleep )
		_V_PROP_( SamplingRate )
		_V_PROP_( Sleep )
	};
//---------------------------------------------------------------------------
	template <
		typename T_Latentcy,
		typename T_Window
	> class TArduinoAccelerometerADXL345DoubleTapTiming :
		public T_Latentcy,
		public T_Window
	{
	public:
		_V_PROP_( Latentcy )
		_V_PROP_( Window )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DoubleTap,
		typename T_Duration
	> class TArduinoAccelerometerADXL345TapTiming :
		public T_DoubleTap,
		public T_Duration
	{
	public:
		_V_PROP_( DoubleTap )
		_V_PROP_( Duration )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DoubleOutputPins_X,
		typename T_DoubleOutputPins_Y,
		typename T_DoubleOutputPins_Z,
		typename T_SingleOutputPins_X,
		typename T_SingleOutputPins_Y,
		typename T_SingleOutputPins_Z,
		typename T_Suppress,
		typename T_Threshold,
		typename T_Timing,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class TArduinoAccelerometerADXL345Tap :
		public T_DoubleOutputPins_X,
		public T_DoubleOutputPins_Y,
		public T_DoubleOutputPins_Z,
		public T_SingleOutputPins_X,
		public T_SingleOutputPins_Y,
		public T_SingleOutputPins_Z,
		public T_Suppress,
		public T_Threshold,
		public T_Timing,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PIN_( DoubleOutputPins_X )
		_V_PIN_( DoubleOutputPins_Y )
		_V_PIN_( DoubleOutputPins_Z )

		_V_PIN_( SingleOutputPins_X )
		_V_PIN_( SingleOutputPins_Y )
		_V_PIN_( SingleOutputPins_Z )

	public:
		_V_PROP_( Suppress )
		_V_PROP_( Threshold )
		_V_PROP_( Timing )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ACMode,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_Threshold,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class TArduinoAccelerometerADXL345Active :
		public T_ACMode,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_Threshold,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	public:
		_V_PROP_( ACMode )
		_V_PROP_( Threshold )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ACMode,
		typename T_OutputPin,
		typename T_Threshold,
		typename T_Time,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class TArduinoAccelerometerADXL345Inactive :
		public T_ACMode,
		public T_OutputPin,
		public T_Threshold,
		public T_Time,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ACMode )
		_V_PROP_( Threshold )
		_V_PROP_( Time )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Active,
//		typename T_AutoSleep,
		typename T_Inactive,
		typename T_Linked
	> class TArduinoAccelerometerADXL345Activity :
		public T_Active,
//		public T_AutoSleep,
		public T_Inactive,
		public T_Linked
	{
	public:
		_V_PROP_( Active )
//		_V_PROP_( AutoSleep )
		_V_PROP_( Inactive )
		_V_PROP_( Linked )

	};
//---------------------------------------------------------------------------
	template <
//		typename T_I2C, T_I2C &C_I2C,
		typename T_INTERFACE,
		typename T_Activity,
		typename T_Enabled,
		typename T_FreeFall,
		typename T_FullScaleRange,
		typename T_HighResolution,
		typename T_LowPower,
		typename T_Offsets,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_SamplingRate,
		typename T_Sleep,
//		typename T_SleepOutputPin,
		typename T_Standby,
		typename T_Tap
	> class AnalogDevicesADXL345 :
		public T_INTERFACE,
		public T_Activity,
		public T_Enabled,
		public T_FreeFall,
		public T_FullScaleRange,
		public T_HighResolution,
		public T_LowPower,
		public T_Offsets,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_SamplingRate,
		public T_Sleep,
//		public T_SleepOutputPin,
		public T_Standby,
		public T_Tap
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )
//		_V_PIN_( SleepOutputPin )

	public:
		_V_PROP_( Activity )
		_V_PROP_( Enabled )
		_V_PROP_( FreeFall )
		_V_PROP_( FullScaleRange )
		_V_PROP_( HighResolution )
		_V_PROP_( LowPower )
		_V_PROP_( Offsets )
		_V_PROP_( SamplingRate )
		_V_PROP_( Sleep )
		_V_PROP_( Standby )
		_V_PROP_( Tap )

	protected:
		float ConvertAcceleration( uint16_t AIntValue )
		{
			if( ! HighResolution().GetValue() )
				AIntValue <<= 6;  // 10 bit

			else
			switch( FullScaleRange().GetValue() )
			{
				case ar2g	: AIntValue <<= 6; break; // 10 bit
				case ar4g	: AIntValue <<= 5; break; // 11 bit
				case ar8g	: AIntValue <<= 4; break; // 12 bit
				default		: AIntValue <<= 3; break; // 13 bit
			}				

			float AValue = float( int16_t( AIntValue ) ) / 0x7FFF;
//			Serial.println( AValue );

			switch( FullScaleRange().GetValue() )
			{
				case ar2g : AValue *= 2.0f; break;
				case ar4g : AValue *= 4.0f; break;
				case ar8g : AValue *= 8.0f; break;
				default : AValue *= 16.0f; break;
			}

			return AValue;
		}

		void ReadActivity( bool AChangeOnly )
		{
			uint8_t AValue = T_INTERFACE::ReadRegister8( ADXL345::ACT_TAP_STATUS );
			uint8_t AValueInt = T_INTERFACE::ReadRegister8( ADXL345::INT_SOURCE );
#ifdef __ADXL345O__DEBUG__
			if( AValue & 0b00000111 )
			{
				Serial.println( AValue, BIN );
				Serial.println( AValueInt, BIN );
			}
#endif
//			Serial.println( AValue, BIN );
//				Serial.println( AValueInt, BIN );

			if( Tap().SingleOutputPins_X().GetPinIsConnected() )
				if( AValue & 0b00000100 )
					if( AValueInt & 0b01000000 )
						Tap().SingleOutputPins_X().ClockPin();

			if( Tap().DoubleOutputPins_X().GetPinIsConnected() )
				if( AValue & 0b00000100 )
					if( AValueInt & 0b00100000 )
						Tap().DoubleOutputPins_X().ClockPin();

			if( Tap().SingleOutputPins_Y().GetPinIsConnected() )
				if( AValue & 0b00000010 )
					if( AValueInt & 0b01000000 )
						Tap().SingleOutputPins_Y().ClockPin();

			if( Tap().DoubleOutputPins_Y().GetPinIsConnected() )
				if( AValue & 0b00000010 )
					if( AValueInt & 0b00100000 )
						Tap().DoubleOutputPins_Y().ClockPin();

			if( Tap().SingleOutputPins_Z().GetPinIsConnected() )
				if( AValue & 0b00000001 )
					if( AValueInt & 0b01000000 )
						Tap().SingleOutputPins_Z().ClockPin();

			if( Tap().DoubleOutputPins_Z().GetPinIsConnected() )
				if( AValue & 0b00000001 )
					if( AValueInt & 0b00100000 )
						Tap().DoubleOutputPins_Z().ClockPin();

			if( Activity().Inactive().OutputPin().GetPinIsConnected() )
				Activity().Inactive().OutputPin().SetPinValue( ( AValueInt & 0b00001000 ) != 0, AChangeOnly );

			if( Activity().Active().OutputPins_X().GetPinIsConnected() )
				if( AValue & 0b01000000 )
					if( AValueInt & 0b00010000 )
						Activity().Active().OutputPins_X().ClockPin();

			if( Activity().Active().OutputPins_Y().GetPinIsConnected() )
				if( AValue & 0b00100000 )
					if( AValueInt & 0b00010000 )
						Activity().Active().OutputPins_Y().ClockPin();

			if( Activity().Active().OutputPins_Z().GetPinIsConnected() )
				if( AValue & 0b00010000 )
					if( AValueInt & 0b00010000 )
						Activity().Active().OutputPins_Z().ClockPin();

			if( Sleep().OutputPin().GetPinIsConnected() )
				Sleep().OutputPin().SetPinValue( ( AValue & 0b00001000 ) != 0, AChangeOnly );


			if( FreeFall().OutputPin().GetPinIsConnected() )
				FreeFall().OutputPin().SetPinValue( ( AValueInt & 0b00000100 ) != 0, AChangeOnly );

/*
			AValue &= 0b00000111;
			if( AValue )
			{
				Serial.println( AValue, BIN );
				T_INTERFACE::WriteRegister8( ADXL345::TAP_AXES, T_INTERFACE::ReadRegister8( ADXL345::TAP_AXES ) & ( ~AValue ));
				UpdateTapAxes();
			}
*/
		}

		void ReadSensors( bool AChangeOnly, bool AForceRead )
		{
			if( ! Enabled().GetValue() )
				return;

/*
			if( ! AForceRead )
			{
				uint8_t AValue = T_INTERFACE::ReadRegister8( LIS3DH::REG_STATUS1 );
//				Serial.println( AValue, BIN );
				if( !( AValue & 0b00001000 ))
					return;
			}
*/
			if( OutputPins_X().GetPinIsConnected() && OutputPins_Y().GetPinIsConnected() && OutputPins_Z().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( ADXL345::DATAX0, 6 );

				uint16_t AValue1 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue1 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue3 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue3 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				OutputPins_X().SetPinValue( ConvertAcceleration( AValue1 ), AChangeOnly );
				OutputPins_Y().SetPinValue( ConvertAcceleration( AValue2 ), AChangeOnly );
				OutputPins_Z().SetPinValue( ConvertAcceleration( AValue3 ), AChangeOnly );
			}

			else if( OutputPins_X().GetPinIsConnected() && OutputPins_Y().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( ADXL345::DATAX0, 4 );

				uint16_t AValue1 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue1 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				OutputPins_X().SetPinValue( ConvertAcceleration( AValue1 ), AChangeOnly );
				OutputPins_Y().SetPinValue( ConvertAcceleration( AValue2 ), AChangeOnly );
			}

			else if( OutputPins_Y().GetPinIsConnected() && OutputPins_Z().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( ADXL345::DATAY0, 4 );

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue3 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue3 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				OutputPins_Y().SetPinValue( ConvertAcceleration( AValue2 ), AChangeOnly );
				OutputPins_Z().SetPinValue( ConvertAcceleration( AValue3 ), AChangeOnly );
			}

			else
			{
				if( OutputPins_X().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( ADXL345::DATAX0, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits
//					Serial.println( AValue );
//					return;

					T_INTERFACE::EndReadRegisters();

					OutputPins_X().SetPinValue( ConvertAcceleration( AValue ), AChangeOnly );
				}

				if( OutputPins_Y().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( ADXL345::DATAY0, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

					T_INTERFACE::EndReadRegisters();

					OutputPins_Y().SetPinValue( ConvertAcceleration( AValue ), AChangeOnly );
				}

				if( OutputPins_Z().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( ADXL345::DATAZ0, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

					T_INTERFACE::EndReadRegisters();

					OutputPins_Z().SetPinValue( ConvertAcceleration( AValue ), AChangeOnly );
				}
			}

//			T_INTERFACE::ReadRegister8( LIS3DH::REG_CTRL4 );

		}
	public:
		inline void UpdateTapThreshold()
		{
//			Serial.print( "Tap().Threshold().GetValue()" );
//			Serial.println( Tap().Threshold().GetValue() );
			uint8_t AValue = ( Tap().Threshold().GetValue() / 16.0f ) * 255 + 0.5;
#ifdef __ADXL345O__DEBUG__
			Serial.print( "THRESH_TAP: " );
			Serial.println( AValue );
#endif
			T_INTERFACE::WriteRegister8( ADXL345::THRESH_TAP, AValue );
		}

		inline void UpdateTapDuration()
		{
			uint8_t AValue = ( Tap().Timing().Duration().GetValue() / 159.375f ) * 255 + 0.5;
#ifdef __ADXL345O__DEBUG__
			Serial.print( "DUR: " );
			Serial.println( AValue );
#endif
			T_INTERFACE::WriteRegister8( ADXL345::DUR, AValue );
		}

		inline void UpdateTapLatent()
		{
			uint8_t AValue = ( Tap().Timing().Latentcy().GetValue() / 318.75f ) * 255 + 0.5;
#ifdef __ADXL345O__DEBUG__
			Serial.print( "LATENT: " );
			Serial.println( AValue );
#endif
			T_INTERFACE::WriteRegister8( ADXL345::LATENT, AValue );
		}

		inline void UpdateTapWindow()
		{
			uint8_t AValue = ( Tap().Timing().Window().GetValue() / 318.75f ) * 255 + 0.5;
#ifdef __ADXL345O__DEBUG__
			Serial.print( "WINDOW: " );
			Serial.println( AValue );
#endif
			T_INTERFACE::WriteRegister8( ADXL345::WINDOW, AValue );
		}

		inline void UpdateTapAxes()
		{
			uint8_t AValue = 0;
			if( Tap().Suppress() )
				AValue |= 0b00001000;

			if( Tap().X() )
				AValue |= 0b00000100;

			if( Tap().Y() )
				AValue |= 0b00000010;

			if( Tap().Z() )
				AValue |= 0b00000001;

#ifdef __ADXL345O__DEBUG__
			Serial.print( "TAP_AXES: " );
			Serial.println( AValue, BIN );
#endif

			T_INTERFACE::WriteRegister8( ADXL345::TAP_AXES, AValue );
		}

		inline void UpdateActiveThreshold()
		{
			uint8_t AValue = ( Activity().Active().Threshold().GetValue() / 16.0f ) * 255 + 0.5;
			T_INTERFACE::WriteRegister8( ADXL345::THRESH_ACT, AValue );
		}

		inline void UpdateInactiveThreshold()
		{
			uint8_t AValue = ( Activity().Inactive().Threshold().GetValue() / 16.0f ) * 255 + 0.5;
			T_INTERFACE::WriteRegister8( ADXL345::THRESH_INACT, AValue );
		}

		inline void UpdateInactiveTime()
		{
			uint8_t AValue = Activity().Inactive().Time().GetValue();
			T_INTERFACE::WriteRegister8( ADXL345::TIME_INACT, AValue );
		}

		inline void UpdateFreeFallThreshold()
		{
			uint8_t AValue = ( FreeFall().Threshold().GetValue() / 16.0f ) * 255 + 0.5;
			T_INTERFACE::WriteRegister8( ADXL345::THRESH_FF, AValue );
		}

		inline void UpdateFreeFallTime()
		{
			uint8_t AValue = ( FreeFall().Time().GetValue() * 255 ) / 1275;
			T_INTERFACE::WriteRegister8( ADXL345::TIME_FF, AValue );
		}

		inline void UpdateActiveAxis()
		{
			uint8_t AValue = 0;
			if( Activity().Active().ACMode() )
				AValue |= 0b10000000;

			if( Activity().Active().X() )
				AValue |= 0b01000000;

			if( Activity().Active().Y() )
				AValue |= 0b00100000;

			if( Activity().Active().Y() )
				AValue |= 0b00010000;

			if( Activity().Inactive().ACMode() )
				AValue |= 0b00001000;

			if( Activity().Active().X() )
				AValue |= 0b00000100;

			if( Activity().Active().Y() )
				AValue |= 0b00000010;

			if( Activity().Active().Z() )
				AValue |= 0b00000001;

			T_INTERFACE::WriteRegister8( ADXL345::ACT_INACT_CTL, AValue );
		}

		inline void UpdatePowerControl()
		{
			uint8_t AValue = 0;
			if( Sleep().Sleep() )
				AValue = 0b00000100;

			if( Sleep().AutoSleep() )
				AValue |= 0b00010000;

			if( ! Standby().GetValue() )
				AValue |= 0b00001000;

			if( Activity().Linked() )
				AValue |= 0b00100000;

			if( Sleep().SamplingRate() < 2 )
				AValue |= 0b00000011;

			else if( Sleep().SamplingRate() < 4 )
				AValue |= 0b00000010;

			else if( Sleep().SamplingRate() < 8 )
				AValue |= 0b00000001;

			T_INTERFACE::WriteRegister8( ADXL345::POWER_CTL, AValue );
		}

		inline void UpdateOffsets()
		{
			if( Offsets().Enabled() )
			{
				uint8_t AValue = ( Offsets().X().GetValue() / 2.0f ) * 255 + 0.5;
				T_INTERFACE::WriteRegister8( ADXL345::OFSX, AValue );

				AValue = ( Offsets().Y().GetValue() / 2.0f ) * 255 + 0.5;
				T_INTERFACE::WriteRegister8( ADXL345::OFSY, AValue );

				AValue = ( Offsets().Z().GetValue() / 2.0f ) * 255 + 0.5;
				T_INTERFACE::WriteRegister8( ADXL345::OFSZ, AValue );
			}
		}

		inline void UpdateDataFormat()
		{
			uint8_t AValue = 0;
			if( HighResolution() )
				AValue = 0b00001000;

//			Set Bit 6 for 3 wire SPI !!!

			switch( FullScaleRange().GetValue() )
			{
//				case ar2g	:
				case ar4g	: AValue |= 0b00000001; break;
				case ar8g	: AValue |= 0b00000010; break;
				case ar16g	: AValue |= 0b00000011; break;
			}

//			delay( 5000 );
//			Serial.println( "" );
//			Serial.println( AValue, BIN );
//			Serial.println( "" );
//			delay( 5000 );

			T_INTERFACE::WriteRegister8( ADXL345::DATA_FORMAT, AValue );
		}

		inline void UpdateRate()
		{
			uint8_t AValue = 0;
			if( LowPower() )
				AValue = 0b00010000;

			if( SamplingRate() > 0.1f )
			{
				if( SamplingRate() < 0.39f )
					AValue |= 0b00000001;

				else if( SamplingRate() < 0.78f )
					AValue |= 0b00000010;

				else if( SamplingRate() < 1.56f )
					AValue |= 0b00000011;

				else if( SamplingRate() < 3.13f )
					AValue |= 0b00000100;

				else if( SamplingRate() < 6.25f )
					AValue |= 0b00000101;

				else if( SamplingRate() < 12.5f )
					AValue |= 0b00000110;

				else if( SamplingRate() < 25.0f )
					AValue |= 0b00000111;

				else if( SamplingRate() < 50.0f )
					AValue |= 0b00001000;

				else if( SamplingRate() < 100.0f )
					AValue |= 0b00001001;

				else if( SamplingRate() < 200.0f )
					AValue |= 0b00001010;

				else if( SamplingRate() < 400.0f )
					AValue |= 0b00001011;

				else if( SamplingRate() < 800.0f )
					AValue |= 0b00001100;

				else if( SamplingRate() < 1600.0f )
					AValue |= 0b00001101;

				else if( SamplingRate() < 3200.0f )
					AValue |= 0b00001110;

				else
					AValue |= 0b00001111;

			}

			T_INTERFACE::WriteRegister8( ADXL345::BW_RATE, AValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false, true );
		}

	public:
		inline void SystemLoopBeginActivity()
		{
			ReadActivity( true );
		}

		inline void SystemLoopBeginRead()
		{
			ReadSensors( true, false );
		}

		inline void SystemStartActivity()
		{
			T_INTERFACE::WriteRegister8( ADXL345::FIFO_CTL, 0 );  // Disable FIFO

			T_INTERFACE::WriteRegister8( ADXL345::INT_ENABLE, 0b11111111 );

			UpdateTapThreshold();
			UpdateTapDuration();
			UpdateTapLatent();
			UpdateTapWindow();
			UpdateTapAxes();

			UpdateActiveThreshold();
			UpdateInactiveThreshold();
			UpdateInactiveTime();

			UpdateFreeFallThreshold();
			UpdateFreeFallTime();

			UpdateDataFormat();
			UpdateRate();

			UpdatePowerControl();

			UpdateOffsets();

			ReadActivity( false );
		}

		inline void SystemStartRead()
		{
			ReadSensors( false, false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

