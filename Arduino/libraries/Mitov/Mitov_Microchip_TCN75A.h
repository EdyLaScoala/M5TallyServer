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
		typename T_ActiveLow,
		typename T_Hysteresis,
		typename T_InterruptMode,
		typename T_QueueSize,
		typename T_Temperature
	> class TArduinoMicrochipTCN75AAlert :
		public T_ActiveLow,
		public T_Hysteresis,
		public T_InterruptMode,
		public T_QueueSize,
		public T_Temperature
	{
	public:
		_V_PROP_( QueueSize )
		_V_PROP_( InterruptMode )
		_V_PROP_( ActiveLow )
		_V_PROP_( Temperature )
		_V_PROP_( Hysteresis )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Alert,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin,
		typename T_Resolution,
		typename T_ShutDown
	> class MicrochipTCN75A :
		public T_Address,
		public T_Alert,
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin,
		public T_Resolution,
		public T_ShutDown
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Resolution )
		_V_PROP_( InFahrenheit )
		_V_PROP_( ShutDown )
		_V_PROP_( Alert )

	protected:
		 static const uint8_t TEMPERATURE_REG = 0x00;
		 static const uint8_t CONFIG_REG = 0x01;
		 static const uint8_t HYSTERESIS_REG = 0x02;
		 static const uint8_t LIMIT_REG = 0x03;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData();
		}

	public:
		void	UpdateLimits()
		{
			WriteRegister16Temp( LIMIT_REG, Alert.Temperature );
			UpdateHysteresis();
		}

		void	UpdateHysteresis()
		{
			float AMinValue = Alert.Temperature - Alert.Hysteresis;
			if( AMinValue < -273.15 )
				AMinValue = -273.15;

			WriteRegister16Temp( HYSTERESIS_REG, AMinValue );
		}

		void	UpdateConfig()
		{
			uint8_t	AValue =	(( Resolution() - 9 ) & 0b11 << 5 );
			switch( Alert().QueueSize() )
			{
				case 2:		AValue |= 0b01000; break;
				case 4:		AValue |= 0b10000; break;
				default:	AValue |= 0b11000;
			}

			if( ! Alert().ActiveLow() )
				AValue |= 0b100;

			if( Alert().InterruptMode() )
				AValue |= 0b10;

			if( ShutDown() )
				AValue |= 0b1;

//			Serial.println( AValue, BIN );

			WriteRegister8( CONFIG_REG, AValue );
		}

	public:
		void ReadData()
		{
			if( ! Enabled() )
				 return;

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( TEMPERATURE_REG );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ));
			if ( C_I2C.available() != 2 )
				return;

			int32_t AValue = C_I2C.read();
			AValue <<= 8;
			AValue |= C_I2C.read();

//			AValue = 0x7FFFFFFF;
//			AValue = 0x80000000;

//			AValue /= ( 16 << ( 3 - ( Resolution - 9 )));
			AValue /= 16;
//			Serial.println( AValue );

			float ATemp = AValue * 0.0625;
			if( InFahrenheit() )
				ATemp = ATemp * ( 9.0 / 5.0 ) + 32.0;

//			AValue = ( ATemp / 0.0625 ) + 0.5;
//			AValue << 8;
//			Serial.println( AValue );
//			Serial.println( "" );

			T_OutputPin::SetPinValue( ATemp );
		}

	protected:
		void WriteRegister8( uint8_t ARegister, uint8_t AValue )
		{
			 C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			 C_I2C.write( ARegister );
			 C_I2C.write( AValue );
			 C_I2C.endTransmission();
		}

		void WriteRegister16Temp( uint8_t ARegister, float ATemp )
		{
			int16_t AValue = ( ATemp / 0.0625 ) + 0.5;
			AValue << 8;
			AValue &= 0xFF80;
			WriteRegister16( ARegister, AValue );
		}

		void WriteRegister16( uint8_t ARegister, uint16_t AValue )
		{
			 C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			 C_I2C.write( ARegister );
			 C_I2C.write( uint8_t( AValue >> 8 ) );
			 C_I2C.write( uint8_t( AValue ) );
			 C_I2C.endTransmission();
		}

	public:
		inline void SystemStart()
		{
			UpdateConfig();
		}

		inline void SystemLoopBegin()
		{
			ReadData();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif