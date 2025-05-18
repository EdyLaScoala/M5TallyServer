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
	namespace JY901
	{
		const uint8_t JY_SAVE         = 0x00;
		const uint8_t JY_CALSW        = 0x01;
		const uint8_t JY_RSW          = 0x02;
		const uint8_t JY_RRATE        = 0x03;
		const uint8_t JY_BAUD         = 0x04;
		const uint8_t JY_AXOFFSET     = 0x05;
		const uint8_t JY_AYOFFSET     = 0x06;
		const uint8_t JY_AZOFFSET     = 0x07;
		const uint8_t JY_GXOFFSET     = 0x08;
		const uint8_t JY_GYOFFSET     = 0x09;
		const uint8_t JY_GZOFFSET     = 0x0a;
		const uint8_t JY_HXOFFSET     = 0x0b;
		const uint8_t JY_HYOFFSET     = 0x0c;
		const uint8_t JY_HZOFFSET     = 0x0d;
		const uint8_t JY_D0MODE       = 0x0e;
		const uint8_t JY_D1MODE       = 0x0f;
		const uint8_t JY_D2MODE       = 0x10;
		const uint8_t JY_D3MODE       = 0x11;
		const uint8_t JY_D0PWMH       = 0x12;
		const uint8_t JY_D1PWMH       = 0x13;
		const uint8_t JY_D2PWMH       = 0x14;
		const uint8_t JY_D3PWMH       = 0x15;
		const uint8_t JY_D0PWMT       = 0x16;
		const uint8_t JY_D1PWMT       = 0x17;
		const uint8_t JY_D2PWMT       = 0x18;
		const uint8_t JY_D3PWMT       = 0x19;
		const uint8_t JY_IICADDR      = 0x1a;
		const uint8_t JY_LEDOFF       = 0x1b;
		const uint8_t JY_GPSBAUD      = 0x1c;

		const uint8_t JY_SLEEP_WAKE   = 0x22;
		const uint8_t JY_DIRECTION	  = 0x23;
		const uint8_t JY_ALG		  = 0x24;

		const uint8_t JY_YYMM       = 0x30;
		const uint8_t JY_DDHH       = 0x31;
		const uint8_t JY_MMSS       = 0x32;
		const uint8_t JY_MS         = 0x33;
		const uint8_t JY_AX         = 0x34;
		const uint8_t JY_AY         = 0x35;
		const uint8_t JY_AZ         = 0x36;
		const uint8_t JY_GX         = 0x37;
		const uint8_t JY_GY         = 0x38;
		const uint8_t JY_GZ         = 0x39;
		const uint8_t JY_HX         = 0x3a;
		const uint8_t JY_HY         = 0x3b;
		const uint8_t JY_HZ         = 0x3c;
		const uint8_t JY_Roll       = 0x3d;
		const uint8_t JY_Pitch      = 0x3e;
		const uint8_t JY_Yaw        = 0x3f;
		const uint8_t JY_TEMP       = 0x40;
		const uint8_t JY_D0Status   = 0x41;
		const uint8_t JY_D1Status   = 0x42;
		const uint8_t JY_D2Status   = 0x43;
		const uint8_t JY_D3Status   = 0x44;
		const uint8_t JY_PressureL  = 0x45;
		const uint8_t JY_PressureH  = 0x46;
		const uint8_t JY_HeightL    = 0x47;
		const uint8_t JY_HeightH    = 0x48;
		const uint8_t JY_LonL       = 0x49;
		const uint8_t JY_LonH       = 0x4a;
		const uint8_t JY_LatL       = 0x4b;
		const uint8_t JY_LatH       = 0x4c;
		const uint8_t JY_GPSHeight  = 0x4d;
		const uint8_t JY_GPSYAW     = 0x4e;
		const uint8_t JY_GPSVL      = 0x4f;
		const uint8_t JY_GPSVH      = 0x50;

		const uint8_t JY_Q0			= 0x51;
		const uint8_t JY_Q1			= 0x52;
		const uint8_t JY_Q2			= 0x53;
		const uint8_t JY_Q3			= 0x54;

		const uint8_t JY_D0VALUE	= 0x55;
		const uint8_t JY_D1VALUE	= 0x56;
		const uint8_t JY_D2VALUE	= 0x57;
		const uint8_t JY_D3VALUE	= 0x58;

		const uint8_t JY_AUTO_GYRO	= 0x63;
	}
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Value
	> class AttitudeAngleJY901SetAddressElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{		
			SystemStart();
		}

	public:
		inline void SystemStart()
		{
			C_OWNER.WriteRegister16( JY901::JY_IICADDR, Value().GetValue() & 0x7F );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class AttitudeAngleJY901SaveSettingsElement
	{
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{		
			SystemStart();
		}

	public:
		inline void SystemStart()
		{
			C_OWNER.WriteRegister16( JY901::JY_SAVE, 0 );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class AttitudeAngleJY901RestoreDefaultSettingsElement
	{
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{		
			SystemStart();
		}

	public:
		inline void SystemStart()
		{		
			C_OWNER.WriteRegister16( JY901::JY_SLEEP_WAKE, 1 );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class AttitudeAngleJY901SleepWakeElement
	{
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{		
			SystemStart();
		}

	public:
		inline void SystemStart()
		{		
			C_OWNER.WriteRegister16( JY901::JY_SAVE, 1 );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_PARAM
	> class AttitudeAngleJY901CalibrateElement
	{
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{		
			SystemStart();
		}

	public:
		inline void SystemStart()
		{		
			C_OWNER.WriteRegister16( JY901::JY_CALSW, C_PARAM );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class AttitudeAngleJY901_Thermometer :
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			float AValue = float( int16_t( C_OWNER.ReadRegister16( JY901::JY_TEMP ))) / 100.0;
			if( InFahrenheit() )
				AValue = Func::ConvertCtoF( AValue );

			T_OutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_OutputPin
	> class AttitudeAngleJY901_Time : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadTime( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			uint8_t ATimeData[ 8 ];
			C_OWNER.ReadRegisters( JY901::JY_YYMM, 8, ATimeData );

//			Serial.println( ATimeData[ 1 ] );
//			Serial.println( ATimeData[ 5 ] );

//			Serial.println( (( uint16_t( ATimeData[ 7 ] ) << 8 ) | ATimeData[ 6 ] ));


			Mitov::TDateTime ADateTime;
			if( ADateTime.TryEncodeDateTime( 2010 + ATimeData[ 0 ], ATimeData[ 1 ] + 1, ATimeData[ 2 ] + 1, ATimeData[ 3 ], ATimeData[ 4 ], ATimeData[ 5 ], ( uint16_t( ATimeData[ 7 ] ) << 8 ) | ATimeData[ 6 ] ))
				T_OutputPin::SetPinValue( ADateTime, AChangeOnly );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadTime( false );
		}

	public:
		inline void SystemStart()
		{
			ReadTime( false );
		}

		inline void SystemLoopBegin()
		{
			ReadTime( true );

//			Serial.println( C_OWNER.ReadRegister16( JY901::JY_TEMP ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_REGISTER_X,
		uint8_t C_REGISTER_Y,
		uint8_t C_REGISTER_Z,
		typename T_Enabled,
		typename T_FullScaleRange,
		typename T_Offset,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ
	> class AttitudeAngleJY901_XYZ_Accelerometer :
		public T_Enabled,
		public T_FullScaleRange,
		public T_Offset,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( FullScaleRange )
		_V_PROP_( Offset )

	protected:
		inline float GetCoefficient()
		{
			if( FullScaleRange().GetValue() < 4 )
				return 2.0f;

			if( FullScaleRange().GetValue() < 8 )
				return 4.0f;

			if( FullScaleRange().GetValue() < 16 )
				return 8.0f;

			return 16.0f;
		}

	public:
		void UpdateOffset()
		{
			if( Offset().X().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_AXOFFSET, Offset().X().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

			if( Offset().Y().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_AYOFFSET, Offset().Y().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

			if( Offset().Z().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_AZOFFSET, Offset().Z().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

		}

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			float ACoeff = GetCoefficient();

			if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 6 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else
			{
				if( OutputPinsX().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_X ))) / (32768.0f / ACoeff );
					T_OutputPinsX::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsY().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_Y ))) / (32768.0f / ACoeff );
					T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsZ().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_Z ))) / (32768.0f / ACoeff );
					T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
				}
			}

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemStartInit()
		{
			UpdateOffset();
		}

		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AutoCalibrate,
		uint8_t C_REGISTER_X,
		uint8_t C_REGISTER_Y,
		uint8_t C_REGISTER_Z,
		typename T_Enabled,
		typename T_FullScaleRange,
		typename T_Offset,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Units
	> class AttitudeAngleJY901_XYZ_Gyroscope :
		public T_AutoCalibrate,
		public T_Enabled,
		public T_FullScaleRange,
		public T_Offset,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Units
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( AutoCalibrate )
		_V_PROP_( Enabled )
		_V_PROP_( FullScaleRange )
		_V_PROP_( Offset )
		_V_PROP_( Units )

	public:
		void UpdateOffset()
		{
			if( Offset().X().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_GXOFFSET, Offset().X().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

			if( Offset().Y().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_GYOFFSET, Offset().Y().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

			if( Offset().Z().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_GZOFFSET, Offset().Z().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

		}

	protected:
		inline float GetCoefficient()
		{
			float ACoeff = Func::ConvertAngle( Units().GetValue(), 1.0f );
			if( FullScaleRange().GetValue() < 500 )
				return ACoeff * 250.0;

			if( FullScaleRange().GetValue() < 1000 )
				return ACoeff * 500.0;

			if( FullScaleRange().GetValue() < 2000 )
				return ACoeff * 1000.0;

			return ACoeff * 2000.0;
		}

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			float ACoeff = GetCoefficient();

			if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 6 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) / (32768.0f / ACoeff );
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else
			{
				if( OutputPinsX().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_X ))) / (32768.0f / ACoeff );
					T_OutputPinsX::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsY().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_Y ))) / (32768.0f / ACoeff );
					T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsZ().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_Z ))) / (32768.0f / ACoeff );
					T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
				}
			}

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemStartInit()
		{
			if( AutoCalibrate() )
				C_OWNER.WriteRegister16( JY901::JY_AUTO_GYRO, 0 );

			else
				C_OWNER.WriteRegister16( JY901::JY_AUTO_GYRO, 1 );

			UpdateOffset();
		}

		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
//		uint32_t C_COEFF,
		uint8_t C_REGISTER_X,
		uint8_t C_REGISTER_Y,
		uint8_t C_REGISTER_Z,
		typename T_Enabled,
		typename T_Offset,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Units
	> class AttitudeAngleJY901_XYZ_Compass :
		public T_Enabled,
		public T_Offset,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Units
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )
		_V_PROP_( Offset )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Units );

	public:
		void UpdateOffset()
		{
			if( Offset().X().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_HXOFFSET, Offset().X().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

			if( Offset().Y().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_HYOFFSET, Offset().Y().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

			if( Offset().Z().Enabled() )				
				C_OWNER.WriteRegister16( JY901::JY_HZOFFSET, Offset().Z().Value().GetValue() / ( GetCoefficient() / 32768.0f ) + 0.5 );

		}

	protected:
		inline float GetCoefficient()
		{
			return Func::ConvertAngle( Units().GetValue(), 1.0f );
		}

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			float ACoeff = GetCoefficient();

			if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 6 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) * ACoeff;
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) * ACoeff;
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) * ACoeff;
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) * ACoeff;
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) * ACoeff;
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) * ACoeff;
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = float( int16_t( C_OWNER.ContinueReadRegister16())) * ACoeff;
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else
			{
				if( OutputPinsX().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_X ))) * ACoeff;
					T_OutputPinsX::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsY().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_Y ))) * ACoeff;
					T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsZ().GetPinIsConnected() )
				{
					float AValue = float( int16_t( C_OWNER.ReadRegister16( C_REGISTER_Z ))) * ACoeff;
					T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemStartInit()
		{
			UpdateOffset();
		}

		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_REGISTER_X,
		uint8_t C_REGISTER_Y,
		uint8_t C_REGISTER_Z,
		typename T_Enabled,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PositiveOnly,
		typename T_Units
	> class AttitudeAngleJY901_XYZ_Sensor :
		public T_Enabled,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_PositiveOnly,
		public T_Units
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Units );
		_V_PROP_( PositiveOnly );

	protected:
		inline float RegisterToAngle( float ACoeff, int16_t AValue )
		{
			float AResult = ACoeff * float( AValue ) / 32768.0f;

			if( PositiveOnly() )
				if( AResult < 0 )
					AResult += ACoeff * 2;

			return AResult;
		}

		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

//			float ACoeff = Func::ConvertAngle( Units().GetValue(), 1.0f );

			float ACoeff;
			switch( Units().GetValue() )
			{
				case auRadians :	ACoeff = PI / 90.0f; break;
				case auNormalized :	ACoeff = 0.5f; break;
				default: ACoeff =  180.f;
			}

			if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 6 );

				float AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ContinueReadRegister16() ));
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ContinueReadRegister16() ));
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ContinueReadRegister16() ));
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsX().GetPinIsConnected() && OutputPinsY().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ContinueReadRegister16() ));
				T_OutputPinsX::SetPinValue( AValue, AChangeOnly );

				AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ContinueReadRegister16() ));
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
			}

			else if( OutputPinsY().GetPinIsConnected() && OutputPinsZ().GetPinIsConnected() )
			{
				C_OWNER.BeginRead( C_REGISTER_X, 4 );

				float AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ContinueReadRegister16() ));
				T_OutputPinsY::SetPinValue( AValue, AChangeOnly );

				AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ContinueReadRegister16() ));
				T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
			}

			else
			{
				if( OutputPinsX().GetPinIsConnected() )
				{
					float AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ReadRegister16( C_REGISTER_X )));
					T_OutputPinsX::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsY().GetPinIsConnected() )
				{
					float AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ReadRegister16( C_REGISTER_Y )));
					T_OutputPinsY::SetPinValue( AValue, AChangeOnly );
				}

				if( OutputPinsZ().GetPinIsConnected() )
				{
					float AValue = RegisterToAngle( ACoeff, int16_t( C_OWNER.ReadRegister16( C_REGISTER_Z )));
					T_OutputPinsZ::SetPinValue( AValue, AChangeOnly );
				}
			}

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PositiveOnly,
		typename T_QuaternionOutputPin,
		typename T_Units
	> class AttitudeAngleJY901_XYZ_Rotation :
		public AttitudeAngleJY901_XYZ_Sensor <
				T_OWNER, C_OWNER,
				Mitov::JY901::JY_Roll, // C_REGISTER_X
				Mitov::JY901::JY_Pitch, // C_REGISTER_Y
				Mitov::JY901::JY_Yaw, // C_REGISTER_Z

				T_Enabled,
				T_OutputPinsX,
				T_OutputPinsY,
				T_OutputPinsZ,
				T_PositiveOnly,
				T_Units
			>,
		public T_QuaternionOutputPin
	{
		typedef AttitudeAngleJY901_XYZ_Sensor <
				T_OWNER, C_OWNER,
				Mitov::JY901::JY_Roll, // C_REGISTER_X
				Mitov::JY901::JY_Pitch, // C_REGISTER_Y
				Mitov::JY901::JY_Yaw, // C_REGISTER_Z
				T_Enabled,
				T_OutputPinsX,
				T_OutputPinsY,
				T_OutputPinsZ,
				T_PositiveOnly,
				T_Units
			> inherited;

	public:
		_V_PIN_( QuaternionOutputPin )

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! inherited::Enabled().GetValue() )
				return;

			inherited::ReadSensor( AChangeOnly );

			if( T_QuaternionOutputPin::GetPinIsConnected() )
			{
				C_OWNER.BeginRead( JY901::JY_Q0, 8 );

/*
				Serial.println( C_OWNER.ContinueReadRegister16() );
				Serial.println( C_OWNER.ContinueReadRegister16() );
				Serial.println( C_OWNER.ContinueReadRegister16() );
				Serial.println( C_OWNER.ContinueReadRegister16() );

				return;
*/
				TQuaternion AQuaternion;
				AQuaternion.Values[ 0 ] = float( int16_t( C_OWNER.ContinueReadRegister16())) / 32768.0f;
				for( int i = 1; i < 4; ++i )
					AQuaternion.Values[ i ] = float( int16_t( C_OWNER.ContinueReadRegister16())) / 32768.0f;

//				Serial.println( AQuaternion.ToString() );

				T_QuaternionOutputPin::SetPinValue( AQuaternion, AChangeOnly );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AnalogOutputPin,
		typename T_DigitalOutputPin,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_IsAnalog,
		typename T_IsOutput,
		typename T_PWMFrequency
	> class GPIO_JY901_Channel :
		public T_AnalogOutputPin,
		public T_DigitalOutputPin,
		public T_InitialValue,
		public T_IsAnalog,
		public T_IsOutput,
		public T_PWMFrequency
	{
	public:
		_V_PIN_( AnalogOutputPin )
		_V_PIN_( DigitalOutputPin )

	public:
		_V_PROP_( InitialValue )
		_V_PROP_( IsAnalog )
		_V_PROP_( IsOutput )
		_V_PROP_( PWMFrequency )

	protected:
		void ReadValues( bool AChangeOnly )
		{
			if( T_AnalogOutputPin::GetPinIsConnected() )
			{
				uint16_t AAnalog16 = C_OWNER.AnalogRead( C_INDEX );
				T_AnalogOutputPin::SetPinValue( float( AAnalog16 ) / 1024.0f, AChangeOnly );
				if( T_DigitalOutputPin::GetPinIsConnected() )
					T_DigitalOutputPin::SetPinValue( AAnalog16 > 512, AChangeOnly );

			}
			else if( T_DigitalOutputPin::GetPinIsConnected() )
				T_DigitalOutputPin::SetPinValue( C_OWNER.DigitalRead( C_INDEX ) > 0, AChangeOnly );

		}

	public:
        void PinDirectionsInit()
        {
//			Serial.println( "PinDirectionsInit" );
            if( IsOutput() )
			{
				SetMode( OUTPUT );
				if( ! IsAnalog().GetValue() )
					DigitalWrite( InitialValue().GetValue() );
			}

            else
				SetMode( INPUT );
		}

		inline void SetMode( int AMode )
		{
			C_OWNER.SetMode( C_INDEX, AMode, InitialValue().GetValue(), IsAnalog().GetValue() || T_AnalogOutputPin::GetPinIsConnected(), false );
//			pinMode( C_PIN_NUMBER, AMode );
		}

		inline void SetInitialMode( int AMode ) // Placeholder for direct pin control!
		{
			C_OWNER.SetInitialMode( C_INDEX, AMode, InitialValue().GetValue(), IsAnalog().GetValue() || T_AnalogOutputPin::GetPinIsConnected(), false );
//			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			if( T_AnalogOutputPin::GetPinIsConnected() )
				return ( C_OWNER.AnalogRead( C_INDEX ) > 512 );

			else
				return C_OWNER.DigitalRead( C_INDEX );
//			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
//			Serial.println( "DigitalWrite" );
//			return;
			C_OWNER.DigitalWrite( C_INDEX, AValue );
//			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

		inline void AnalogWrite( float AValue )
		{
			C_OWNER.AnalogWrite( C_INDEX, AValue );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
            if( IsOutput() )
				DigitalWrite( *( bool * )_Data );

		}

		inline void AnalogInputPin_o_Receive( void * _Data )
		{
            if( IsOutput() )
				AnalogWrite( *( float * )_Data );

		}

	public:
		inline void SystemInit()
		{
            PinDirectionsInit();
		}

		inline void SystemStart()
		{
			ReadValues( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValues( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AnalogOutputPin,
		typename T_DigitalOutputPin,
		typename T_GPSEnabled,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_IsAnalog,
		typename T_IsOutput,
		typename T_PWMFrequency
	> class GPIO_JY901_GPS_Channel :
		public GPIO_JY901_Channel<
			T_OWNER, C_OWNER,
			T_AnalogOutputPin,
			T_DigitalOutputPin,
			C_INDEX,
			T_InitialValue,
			T_IsAnalog,
			T_IsOutput,
			T_PWMFrequency
		>,
		public T_GPSEnabled
	{
		typedef GPIO_JY901_Channel<
			T_OWNER, C_OWNER,
			T_AnalogOutputPin,
			T_DigitalOutputPin,
			C_INDEX,
			T_InitialValue,
			T_IsAnalog,
			T_IsOutput,
			T_PWMFrequency
		> inherited;

	public:
		_V_PROP_( GPSEnabled )

	public:
        void PinDirectionsInit()
        {
//			Serial.println( "PinDirectionsInit" );
            if( inherited::IsOutput() )
			{
				SetMode( OUTPUT );
				if( ! inherited::IsAnalog().GetValue() )
					inherited::DigitalWrite( inherited::InitialValue().GetValue() );
			}

            else
				SetMode( INPUT );
		}

		inline void SetMode( int AMode )
		{
//			Serial.println( "TEST1" );
			C_OWNER.SetMode( C_INDEX, AMode, inherited::InitialValue().GetValue(), inherited::IsAnalog().GetValue() || T_AnalogOutputPin::GetPinIsConnected(), GPSEnabled().GetValue() );
//			pinMode( C_PIN_NUMBER, AMode );
		}

		inline void SetInitialMode( int AMode ) // Placeholder for direct pin control!
		{
			C_OWNER.SetInitialMode( C_INDEX, AMode, inherited::InitialValue().GetValue(), inherited::IsAnalog().GetValue() || T_AnalogOutputPin::GetPinIsConnected(), GPSEnabled().GetValue() );
//			SetMode( AMode );
		}

	public:
		inline void SystemInit()
		{
            PinDirectionsInit();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FDirections,
		typename T_FGPS
	> class AttitudeAngleJY901_GPIO :
		public T_FDirections,
		public T_FGPS
	{
	protected:
		enum TMode { mAnalogIn, mDigitalIn, mDigitalOutHigh, mDigitalOutLow, mAnalogOut, mGPS };

	protected:
		_V_PROP_( FDirections )
		_V_PROP_( FGPS )

	public:
		inline void SetMode( uint8_t APinIndex, int AMode, bool AInitialDigitalValue, bool AIsAnalog, bool AGPS )
		{
			uint16_t AValue = SetInitialMode( APinIndex, AMode, AInitialDigitalValue, AIsAnalog, AGPS );

//			Serial.println( AGPS );

//			Serial.print( JY901::JY_D0MODE + APinIndex );
//			Serial.print( " : " );
//			Serial.println( AValue );

			C_OWNER.WriteRegister16( JY901::JY_D0MODE + APinIndex, AValue );
		}

		inline TMode SetInitialMode( uint8_t APinIndex, int AMode, bool AInitialDigitalValue, bool AIsAnalog, bool AGPS )
		{
			TMode AValue;
			if( AMode == OUTPUT )
			{
				if( AIsAnalog )
					AValue = mAnalogOut;

				else
					AValue = AInitialDigitalValue ? mDigitalOutHigh : mDigitalOutLow;

			}

			else
			{
				if( AIsAnalog )
					AValue = mAnalogIn;

				else
					AValue = mDigitalIn;

			}

			FDirections() = FDirections().GetValue() & ( uint16_t( 0b111 ) << ( APinIndex * 3 ) );
			FDirections() = FDirections().GetValue() | ( uint16_t( AValue ) << ( APinIndex * 3 ) );
			if( APinIndex == 1 )
			{
				FGPS() = AGPS;
				if( AGPS )
					AValue = mGPS;
			}

			return AValue;
		}

		inline bool DigitalRead( uint8_t APinIndex )
		{
//			return ( C_OWNER.ReadRegister16( JY901::JY_D0VALUE + APinIndex ) > 0 );
			return ( C_OWNER.ReadRegister16( JY901::JY_D0Status + APinIndex ) > 0 );
		}

		inline uint16_t AnalogRead( uint8_t APinIndex )
		{
//			return C_OWNER.ReadRegister16( JY901::JY_D0VALUE + APinIndex );
			return C_OWNER.ReadRegister16( JY901::JY_D0Status + APinIndex );
		}

		inline void DigitalWrite( uint8_t APinIndex, bool AValue )
		{
			TMode ANewValue = TMode( FDirections().GetValue() >> ( APinIndex * 3 ));
			if( ( ANewValue == mDigitalOutHigh ) || ( ANewValue == mDigitalOutLow ) )
				SetMode( APinIndex, OUTPUT, AValue, false, FGPS() );
		}

		inline void AnalogWrite( uint8_t APinIndex, float AValue )
		{
			C_OWNER.WriteRegister16( JY901::JY_D0PWMT + APinIndex, AValue * 0xFFFF );
		}

	public:
		inline AttitudeAngleJY901_GPIO()
		{
			FDirections() = 0;
			FGPS() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CourseOutputPins_Speed,
		typename T_CourseOutputPins_Course,
		typename T_Enabled,
		typename T_LocationOutputPins_Latitude,
		typename T_LocationOutputPins_Longitude,
		typename T_LocationOutputPins_Altitude,
		typename T_SerialSpeed
	> class AttitudeAngleJY901_GPS :
		public T_CourseOutputPins_Speed,
		public T_CourseOutputPins_Course,
		public T_Enabled,
		public T_LocationOutputPins_Latitude,
		public T_LocationOutputPins_Longitude,
		public T_LocationOutputPins_Altitude,
		public T_SerialSpeed
	{
	public:
		_V_PIN_( CourseOutputPins_Speed )
		_V_PIN_( CourseOutputPins_Course )

		_V_PIN_( LocationOutputPins_Latitude )
		_V_PIN_( LocationOutputPins_Longitude )
		_V_PIN_( LocationOutputPins_Altitude )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( SerialSpeed )

	public:
		void UpdateSerialSpeed()
		{
			uint8_t AValue;
			if( SerialSpeed().GetValue() < 4800 )
				AValue = 0; //2400;

			else if( SerialSpeed().GetValue() < 9600 )
				AValue = 1; //4800;

			else if( SerialSpeed().GetValue() < 19200 )
				AValue = 2; //9600;

			else if( SerialSpeed().GetValue() < 38400 )
				AValue = 3; //19200;

			else if( SerialSpeed().GetValue() < 57600 )
				AValue = 4; //38400;

			else if( SerialSpeed().GetValue() < 115200 )
				AValue = 5; //57600;

			else if( SerialSpeed().GetValue() < 230400 )
				AValue = 6; //115200;

			else if( SerialSpeed().GetValue() < 460800 )
				AValue = 7; //230400;

			else if( SerialSpeed().GetValue() < 921600 )
				AValue = 8; //460800;

			else
				AValue = 9; //921600;

//			Serial.print( JY901::JY_GPSBAUD, HEX );
//			Serial.print( " : " );
//			Serial.println( AValue );

			C_OWNER.WriteRegister16( JY901::JY_GPSBAUD, AValue );
		}

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

//			C_OWNER.BeginRead( JY901::JY_LatL, 4 );
//			float AValue = float( int32_t( C_OWNER.ContinueReadRegister32()) % 10000000)) / 1e5;
//			Serial.println( AValue );

			if( LocationOutputPins_Latitude().GetPinIsConnected() || LocationOutputPins_Longitude().GetPinIsConnected() )
			{
				if( LocationOutputPins_Latitude().GetPinIsConnected() && LocationOutputPins_Longitude().GetPinIsConnected() )
				{
					C_OWNER.BeginRead( JY901::JY_LonL, 8 );
 
					float AValue = float( int32_t( C_OWNER.ContinueReadRegister32()) % 10000000);
					LocationOutputPins_Longitude().SetPinValue( AValue, AChangeOnly );

					AValue = float( int32_t( C_OWNER.ContinueReadRegister32()) % 10000000);
					LocationOutputPins_Latitude().SetPinValue( AValue, AChangeOnly );
				}

				else if( LocationOutputPins_Longitude().GetPinIsConnected() )
				{
					C_OWNER.BeginRead( JY901::JY_LonL, 4 );
					float AValue = float( int32_t( C_OWNER.ContinueReadRegister32()) % 10000000);
					LocationOutputPins_Longitude().SetPinValue( AValue, AChangeOnly );
				}

				else
				{
					C_OWNER.BeginRead( JY901::JY_LatL, 4 );
					float AValue = float( int32_t( C_OWNER.ContinueReadRegister32()) % 10000000);
					LocationOutputPins_Latitude().SetPinValue( AValue, AChangeOnly );
				}
			}

			if( LocationOutputPins_Altitude().GetPinIsConnected() )
			{
				float AValue = float( int16_t( C_OWNER.ReadRegister16( JY901::JY_GPSHeight ))) / 10;
				LocationOutputPins_Altitude().SetPinValue( AValue, AChangeOnly );
			}

			if( CourseOutputPins_Course().GetPinIsConnected() )
			{
				float AValue = float( int16_t( C_OWNER.ReadRegister16( JY901::JY_GPSYAW )));
				CourseOutputPins_Course().SetPinValue( AValue, AChangeOnly );
			}
			
			if( CourseOutputPins_Speed().GetPinIsConnected() )
			{				
				float AValue = float( int32_t( C_OWNER.ReadRegister32( JY901::JY_GPSVL )));
				CourseOutputPins_Speed().SetPinValue( AValue, AChangeOnly );
			}


/*
			if( ! ( CourseOutputPins_Speed().GetPinIsConnected() || CourseOutputPins_Course().GetPinIsConnected() || LocationOutputPins_Latitude().GetPinIsConnected() || LocationOutputPins_Longitude().GetPinIsConnected() || LocationOutputPins_Altitude().GetPinIsConnected() ))
				return;

			if( 
					( CourseOutputPins_Speed().GetPinIsConnected() || CourseOutputPins_Course().GetPinIsConnected() ) &&
					( LocationOutputPins_Latitude().GetPinIsConnected() || LocationOutputPins_Longitude().GetPinIsConnected() || LocationOutputPins_Altitude().GetPinIsConnected() )
				)
				C_OWNER.BeginRead( JY901::JY_LonL, 8 );

			else if ( CourseOutputPins_Speed().GetPinIsConnected() || CourseOutputPins_Course().GetPinIsConnected() )
				C_OWNER.BeginRead( JY901::JY_LonL, 4 );

			float AValue = float( C_OWNER.ContinueReadRegister32());
*/
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemInit()
		{
			UpdateSerialSpeed();
		}

		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_InstalledVertical,
		typename T_Led,
		typename T_Use6AxisAlgorithm
	> class AttitudeAngleJY901 :
		public T_Address,
		public T_InstalledVertical,
		public T_Led,
		public T_Use6AxisAlgorithm
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( InstalledVertical )
		_V_PROP_( Led )
		_V_PROP_( Use6AxisAlgorithm )

	public:
		void UpdateLED()
		{
			if( Led() )
				WriteRegister16( JY901::JY_LEDOFF, 1 );

			else
				WriteRegister16( JY901::JY_LEDOFF, 0 );

		}

		void UpdateInstalledVertical()
		{
			if( InstalledVertical() )
				WriteRegister16( JY901::JY_DIRECTION, 1 );

			else
				WriteRegister16( JY901::JY_DIRECTION, 0 );

		}

		void UpdateUse6AxisAlgorithm()
		{
			if( Use6AxisAlgorithm() )
				WriteRegister16( JY901::JY_ALG, 1 );

			else
				WriteRegister16( JY901::JY_ALG, 0 );

		}

	public:
		void BeginRead( uint8_t addressToRead, uint8_t bytesToRead )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( addressToRead );
			C_I2C.endTransmission(false); //endTransmission but keep the connection active

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), bytesToRead ); //Ask for bytes, once done, bus is released by default
		}

		uint16_t ContinueReadRegister16()
		{
			uint8_t AResult = C_I2C.read();
			return ( AResult | ( uint16_t( C_I2C.read() ) << 8 ));
		}

		uint32_t ContinueReadRegister32()
		{
			uint32_t AResult = C_I2C.read();
			AResult |= uint32_t( C_I2C.read() ) << 8;
			AResult |= uint32_t( C_I2C.read() ) << 16;
			AResult |= uint32_t( C_I2C.read() ) << 24;
			return AResult;
		}

		void ReadRegisters( uint8_t addressToRead, uint8_t bytesToRead, uint8_t *dest )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( addressToRead );
			C_I2C.endTransmission(false); //endTransmission but keep the connection active

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), bytesToRead ); //Ask for bytes, once done, bus is released by default

			for (int x = 0; x < bytesToRead; x++)
				dest[x] = C_I2C.read();

		}

		uint16_t ReadRegister16( uint8_t addressToRead )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( addressToRead );
			C_I2C.endTransmission(false); //endTransmission but keep the connection active

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) ); //Ask for bytes, once done, bus is released by default

			uint8_t AResult = C_I2C.read();
			return ( AResult | ( uint16_t( C_I2C.read() ) << 8 ));
		}

		uint32_t ReadRegister32( uint8_t addressToRead )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( addressToRead );
			C_I2C.endTransmission(false); //endTransmission but keep the connection active

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) ); //Ask for bytes, once done, bus is released by default

			uint32_t AResult = C_I2C.read();
			AResult |= uint32_t( C_I2C.read() ) << 8;
			AResult |= uint32_t( C_I2C.read() ) << 16;
			AResult |= uint32_t( C_I2C.read() ) << 24;
			return AResult;
		}

		void WriteRegister16( uint8_t addressToRead, uint16_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( JY901::JY_LEDOFF );
			C_I2C.write( uint8_t( AValue ));
			C_I2C.write( uint8_t( AValue >> 8 ));
			C_I2C.endTransmission(); //endTransmission but keep the connection active
		}

	public:
		inline void SystemStart()
		{
			UpdateUse6AxisAlgorithm();
			UpdateInstalledVertical();
			UpdateLED();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


