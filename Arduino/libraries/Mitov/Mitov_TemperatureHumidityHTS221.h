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

// #define __HTS221__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace HTS221
	{
		const uint8_t HTS221_WHO_AM_I_REG		= 0x0F;
		const uint8_t HTS221_AV_CONF			= 0x10;
		const uint8_t HTS221_CTRL1_REG			= 0x20;
		const uint8_t HTS221_CTRL2_REG			= 0x21;
		const uint8_t HTS221_CTRL3_REG			= 0x22;
		const uint8_t HTS221_STATUS_REG			= 0x27;
		const uint8_t HTS221_HUMIDITY_OUT_L_REG	= 0x28;
		const uint8_t HTS221_TEMP_OUT_L_REG		= 0x2A;
		const uint8_t HTS221_H0_rH_x2_REG		= 0x30;
		const uint8_t HTS221_H1_rH_x2_REG		= 0x31;
		const uint8_t HTS221_T0_degC_x8_REG		= 0x32;
		const uint8_t HTS221_T1_degC_x8_REG		= 0x33;
		const uint8_t HTS221_T1_T0_MSB_REG		= 0x35;
		const uint8_t HTS221_H0_T0_OUT_REG		= 0x36;
		const uint8_t HTS221_H1_T0_OUT_REG		= 0x3A;
		const uint8_t HTS221_T0_OUT_REG			= 0x3C;
		const uint8_t HTS221_T1_OUT_REG			= 0x3E;
	}
//---------------------------------------------------------------------------
	template <
		typename T_Humidity,
		typename T_Temperature
	> class TArduinoTemperatureHumidityHTS221Average :
		public T_Humidity,
		public T_Temperature
	{
	public:
		_V_PROP_( Humidity )
		_V_PROP_( Temperature )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ActiveLow,
		typename T_Enabled,
		typename T_OpenDrain
	> class TArduinoTemperatureHumidityHTS221DataReadySignal :
		public T_ActiveLow,
		public T_Enabled,
		public T_OpenDrain
	{
	public:
		_V_PROP_( ActiveLow )
		_V_PROP_( Enabled )
		_V_PROP_( OpenDrain )

	};
//---------------------------------------------------------------------------
	template <
//		typename T_I2C, T_I2C &C_I2C,
//		typename T_Address,
		typename T_IMPLEMENTATION,
		typename T_Average,
		typename T_DataReadySignal,
		typename T_Enabled,
		typename T_Heater,
		typename T_HumidityOutputPin,
		typename T_InFahrenheit,
		typename T_OneShot,
		typename T_PowerDown,
		typename T_SampleRate,
		typename T_TemperatureOutputPin
	> class TemperatureHumidityHTS221 : 
		public T_IMPLEMENTATION,
		public T_Average,
		public T_DataReadySignal,
		public T_Enabled,
		public T_Heater,
		public T_HumidityOutputPin,
		public T_InFahrenheit,
		public T_OneShot,
		public T_PowerDown,
		public T_SampleRate,
		public T_TemperatureOutputPin
	{
	public:
//		_V_PROP_( Address )
		_V_PROP_( Average )
		_V_PROP_( DataReadySignal )
		_V_PROP_( Enabled )
		_V_PROP_( Heater )
		_V_PROP_( InFahrenheit )
		_V_PROP_( OneShot )
		_V_PROP_( PowerDown )
		_V_PROP_( SampleRate )

	public:
		_V_PIN_( HumidityOutputPin )
		_V_PIN_( TemperatureOutputPin )


	protected:
		float FHumiditySlope;
		float FHumidityZero;

		float FTemperatureSlope;
		float FTemperatureZero;

	public:
		void UpdateControl1()
		{
			uint8_t AValue = ( PowerDown() ) ? 0b00000100 : 0b10000100;
//			AValue = 0b10000000;

			if( ! OneShot().GetValue() )
			{
				if( SampleRate().GetValue() <= 1.0 )
					AValue |= 0b00000001;

				else if( SampleRate().GetValue() <= 7.0 )
					AValue |= 0b00000010;

				else
					AValue |= 0b00000011;
			}

//			Serial.print( AValue, BIN );
			Write8( HTS221::HTS221_CTRL1_REG, AValue );
		}

		void UpdateControl2()
		{
//			uint8_t AValue = Read8( HTS221::HTS221_CTRL2_REG );

			uint8_t AValue = ( Heater() ) ? 0b00000010 : 0b00000000;
//			if( Heater() )
//				AValue |= 0b00000010;

//			else
//				AValue &= 0b11111101;

			Write8( HTS221::HTS221_CTRL2_REG, AValue );
		}

		void UpdateControl3()
		{
			uint8_t AValue = ( DataReadySignal().Enabled() ) ? 0b00000100 : 0b00000000;

			if( DataReadySignal().ActiveLow() )
				AValue |= 0b10000000;

			if( DataReadySignal().OpenDrain() )
				AValue |= 0b01000000;

			Write8( HTS221::HTS221_CTRL3_REG, AValue );
		}

		void UpdateConfig()
		{
			uint8_t AValue = 0;

			if( Average().Temperature().GetValue() > 256 )
				AValue |= 0b00111000;

			else if( Average().Temperature().GetValue() > 128 )
				AValue |= 0b00110000;

			else if( Average().Temperature().GetValue() > 64 )
				AValue |= 0b00101000;

			else if( Average().Temperature().GetValue() > 32 )
				AValue |= 0b00100000;

			else if( Average().Temperature().GetValue() > 16 )
				AValue |= 0b00011000;

			else if( Average().Temperature().GetValue() > 8 )
				AValue |= 0b00010000;

			else if( Average().Temperature().GetValue() > 4 )
				AValue |= 0b00001000;


			if( Average().Humidity().GetValue() > 128 )
				AValue |= 0b00000111;

			else if( Average().Humidity().GetValue() > 64 )
				AValue |= 0b00000110;

			else if( Average().Humidity().GetValue() > 32 )
				AValue |= 0b00000101;

			else if( Average().Humidity().GetValue() > 16 )
				AValue |= 0b00000100;

			else if( Average().Humidity().GetValue() > 8 )
				AValue |= 0b00000011;

			else if( Average().Humidity().GetValue() > 4 )
				AValue |= 0b00000010;

			else if( Average().Humidity().GetValue() > 2 )
				AValue |= 0b00000001;

			Write8( HTS221::HTS221_AV_CONF, AValue );
		}

	protected:
		void Write8( uint8_t ARegister, uint8_t AValue )
		{
//			return;
#ifdef __HTS221__DEBUG__
			Serial.print( ARegister, HEX ); Serial.print( " W : " ); Serial.println( AValue, BIN );
#endif
			T_IMPLEMENTATION::WriteRegister8( ARegister, AValue );
/*
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( AValue );
			C_I2C.endTransmission();
*/
		}

		inline uint8_t Read8( uint8_t ARegister )
		{
//			return 0;
#ifdef __HTS221__DEBUG__
			Serial.print( ARegister, HEX ); Serial.print( " R : " );
#endif
			uint8_t AResult = T_IMPLEMENTATION::ReadRegister8( ARegister );
#ifdef __HTS221__DEBUG__
			Serial.println( AResult, BIN );
#endif
			return AResult;
/*
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ));

			uint8_t AValue = C_I2C.read();

//			Serial.println( AValue, BIN );
			return AValue;
*/
		}

		inline uint16_t Read16( uint8_t ARegister )
		{
//			return 0;
#ifdef __HTS221__DEBUG__
			Serial.print( ARegister, HEX ); Serial.print( " R : " );
#endif
			uint16_t AResult = T_IMPLEMENTATION::ReadRegister16( ARegister );;
#ifdef __HTS221__DEBUG__
			Serial.println( AResult, BIN );
#endif
			return AResult;
/*
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister | 0x80 );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ));

			uint16_t AResult = C_I2C.read();

			AResult |= uint16_t( C_I2C.read() ) << 8;

//			Serial.println( AResult, BIN );

			return AResult;

//			return (Read8(reg) | (Read8(reg + 1) << 8));
*/
		}

	protected:
		void ReadSensor( bool AChangeOnly, bool AStartNext )
		{
			if( ! Enabled().GetValue() )
				return;

//			return;

			uint8_t AStatus = Read8( HTS221::HTS221_STATUS_REG );
//			Serial.println( AStatus, BIN );
			if( AStatus == 0 )
				return;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
				if( AStatus & 0b01 )
				{
					int16_t AValue = Read16( HTS221::HTS221_TEMP_OUT_L_REG );
					float ATemperature = ( AValue * FTemperatureSlope + FTemperatureZero );
					if( InFahrenheit() )
						ATemperature = Func::ConvertCtoF( ATemperature );

					T_TemperatureOutputPin::SetPinValue( ATemperature, AChangeOnly );
				}

			if( T_HumidityOutputPin::GetPinIsConnected() )
				if( AStatus & 0b10 )
				{
					int16_t AValue = Read16( HTS221::HTS221_HUMIDITY_OUT_L_REG );
					float AHumidity = ( AValue * FHumiditySlope + FHumidityZero );

					T_HumidityOutputPin::SetPinValue( AHumidity, AChangeOnly );
				}

			if( AStartNext )
				if( OneShot() )
					Write8( HTS221::HTS221_CTRL2_REG, 0x01 );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( OneShot() )
				Write8( HTS221::HTS221_CTRL2_REG, 0x01 );

			ReadSensor( false, false );
		}

	public:
		inline void SystemStartInit()
		{
			T_IMPLEMENTATION::Init();

			UpdateControl1();
			UpdateControl2();
			UpdateControl3();
			UpdateConfig();

			uint8_t h0rH = Read8(HTS221::HTS221_H0_rH_x2_REG);
			uint8_t h1rH = Read8(HTS221::HTS221_H1_rH_x2_REG);

			uint16_t t0degC = Read8(HTS221::HTS221_T0_degC_x8_REG) | ((Read8( HTS221::HTS221_T1_T0_MSB_REG) & 0x03) << 8);
			uint16_t t1degC = Read8(HTS221::HTS221_T1_degC_x8_REG) | ((Read8( HTS221::HTS221_T1_T0_MSB_REG) & 0x0C) << 6);

			int16_t h0t0Out = Read16(HTS221::HTS221_H0_T0_OUT_REG);
			int16_t h1t0Out = Read16(HTS221::HTS221_H1_T0_OUT_REG);

			int16_t t0Out = Read16(HTS221::HTS221_T0_OUT_REG);
			int16_t t1Out = Read16(HTS221::HTS221_T1_OUT_REG);

			// calculate slopes and 0 offset from calibration values,
			// for future calculations: value = a * X + b

			FHumiditySlope = (h1rH - h0rH) / (2.0 * (h1t0Out - h0t0Out));
			FHumidityZero = (h0rH / 2.0) - FHumiditySlope * h0t0Out;

			FTemperatureSlope = (t1degC - t0degC) / (8.0 * (t1Out - t0Out));
			FTemperatureZero = (t0degC / 8.0) - FTemperatureSlope * t0Out;
		}

		inline void SystemStart()
		{
			SystemStartInit();
			if( OneShot() )
				Write8( HTS221::HTS221_CTRL2_REG, 0x01 );

			ReadSensor( false, true );
		}

		inline void SystemLoopBegin()
		{
//			Serial.println( "TEST" );
			ReadSensor( true, true );
		}

		inline void SystemLoopBeginClock()
		{
			if( OneShot() )
				ReadSensor( true, false );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

