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
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_Value
	> class TArduinoMicrochipThermometerMCP9808_BasicAlert :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	template <
		typename T_CryticalOnly,
		typename T_Enabled,
		typename T_InterruptMode,
		typename T_Positive
	> class TArduinoMicrochipThermometerMCP9808_AlertSignal :
		public T_CryticalOnly,
		public T_Enabled,
		public T_InterruptMode,
		public T_Positive
	{
	public:
		_V_PROP_( CryticalOnly )
		_V_PROP_( Enabled )
		_V_PROP_( InterruptMode )
		_V_PROP_( Positive )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Locked,
		typename T_OutputPin,
		typename T_Value
	> class TArduinoMicrochipThermometerMCP9808_CriticalAlert :
		public T_Locked,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Locked )
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Hysteresis,
		typename T_Locked,
		typename T_Lower,
		typename T_Upper
	> class TArduinoMicrochipThermometerMCP9808_LimitsAlert :
		public T_Hysteresis,
		public T_Locked,
		public T_Lower,
		public T_Upper
	{
	public:
		_V_PROP_( Hysteresis )
		_V_PROP_( Locked )
		_V_PROP_( Lower )
		_V_PROP_( Upper )

	};
//---------------------------------------------------------------------------
	template <
		typename T_AlertPin,
		typename T_Critical,
		typename T_Limits
	> class TArduinoMicrochipThermometerMCP9808_Alerts :
		public T_AlertPin,
		public T_Critical,
		public T_Limits
	{
	public:
		_V_PROP_( AlertPin )
		_V_PROP_( Critical )
		_V_PROP_( Limits )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Alerts,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin,
		typename T_ShutDown
	> class TArduinoMicrochipThermometerMCP9808_I2C :
		public T_Address,
		public T_Alerts,
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin,
		public T_ShutDown
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Alerts )
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )
		_V_PROP_( ShutDown )

	protected:
/*
		static const uint8_t THYSTERESIS	= 0x0600;
		static const uint8_t SHUTDOWN       = 0x0100;
		static const uint8_t CRIT_LOCK      = 0x0080;
		static const uint8_t WIN_LOCK       = 0x0040;
		static const uint8_t INT_CLEAR      = 0x0020;
		static const uint8_t ALERT_STATUS   = 0x0010;
		static const uint8_t ALERT_CTRL     = 0x0008;
		static const uint8_t ALERT_SELECT   = 0x0004;
		static const uint8_t ALERT_POLAR    = 0x0002;
		static const uint8_t ALERT_MODE     = 0x0001;
*/
		static const uint8_t RFU	= 0x00;
		static const uint8_t CONFIG = 0x01;
		static const uint8_t TUPPER = 0x02;
		static const uint8_t TLOWER = 0x03;
		static const uint8_t TCRIT  = 0x04;
		static const uint8_t TA     = 0x05;
		static const uint8_t MID    = 0x06;
		static const uint8_t DID    = 0x07;
		static const uint8_t RES    = 0x08;

	public:
		inline void UpdateUpperAlert()
		{
			writeFloat( TUPPER, Alerts().Limits().Upper().Value() );
		}

		inline void UpdateLowerAlert()
		{
			writeFloat( TLOWER, Alerts().Limits().Lower().Value() );
		}

		inline void UpdateCriticalAlert()
		{
			writeFloat( TCRIT, Alerts().Critical().Value() );
		}

		inline void UpdateConfig()
		{
			UpdateConfigInt( false );
		}

	protected:
		void UpdateConfigInt( bool AClearAlert )
		{
			uint16_t AValue = ( ShutDown().GetValue() ) ? 0b100000000 : 0;

			if( Alerts().Limits().Hysteresis() >= 6.0 )
				AValue |= 0b11000000000;

			else if( Alerts().Limits().Hysteresis() >= 3.0 )
				AValue |= 0b10000000000;

			else if( Alerts().Limits().Hysteresis() >= 1.5 )
				AValue |= 0b01000000000;

			if( Alerts().Critical().Locked().GetValue() )
				AValue |= 0b10000000;

			if( Alerts().Limits().Locked().GetValue() )
				AValue |= 0b01000000;

			if( AClearAlert )
				AValue |= 0b00100000;

			if( Alerts().AlertPin().Enabled() )
				AValue |= 0b1000;

			if( Alerts().AlertPin().CryticalOnly() )
				AValue |= 0b100;

			if( Alerts().AlertPin().Positive() )
				AValue |= 0b10;

			if( Alerts().AlertPin().InterruptMode() )
				AValue |= 0b01;

			writeReg16( CONFIG, AValue );
		}

		void writeFloat( uint8_t ARegister, float AFloatValue )
		{
			bool ANegative = ( AFloatValue < 0.0);
			if (ANegative) 
				AFloatValue = -AFloatValue;

			uint16_t AValue = uint16_t( AFloatValue * 4 + 0.5) * 4;
			if (ANegative) 
				AValue |= 0x1000;

			writeReg16( ARegister, AValue );
		}

		uint16_t readReg16( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) );
			uint16_t AValue = uint16_t( C_I2C.read() ) << 8;
			AValue |= C_I2C.read();
			return AValue;
		}

		void writeReg16( uint8_t ARegister, uint16_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.write( AValue >> 8 );   // hi byte
			C_I2C.write( AValue & 0xFF ); // lo byte
			C_I2C.endTransmission();
		}

		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

			uint16_t AValue = readReg16( TA );
//			if (reg == MCP9808_TA)
//				_status = (val & 0xE000) >> 13;

			Alerts().Critical().OutputPin().SetPinValue( ( AValue & 0b1000000000000000 ) != 0 );
			Alerts().Limits().Upper().OutputPin().SetPinValue( ( AValue & 0b0100000000000000 ) != 0 );
			Alerts().Limits().Lower().OutputPin().SetPinValue( ( AValue & 0b0010000000000000 ) != 0 );

			if( T_OutputPin::GetPinIsConnected() )
			{
				float AOutValue;
				if( AValue & 0x1000 )  // negative value
					AOutValue = 256 - ( AValue & 0x0FFF ) * 0.0625;

				else
					AOutValue = ( AValue & 0x0FFF ) * 0.0625;

				T_OutputPin::SetPinValue( AOutValue );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

		inline void AlertSignal_ClearInputPin_o_Receive( void *_Data )
		{
			UpdateConfigInt( true );
		}

	public:
		inline void SystemStartClocked()
		{
			UpdateUpperAlert();
			UpdateLowerAlert();
			UpdateCriticalAlert();
			UpdateConfigInt( true ); // Config last due to optional locking!
		}

		inline void SystemStart()
		{
			SystemStartClocked();
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

