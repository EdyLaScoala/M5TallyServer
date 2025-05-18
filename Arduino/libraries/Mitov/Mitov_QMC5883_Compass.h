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

namespace Mitov
{
	namespace QMC5883L
	{
		const uint8_t OUT_X_L						= 0x00;
		const uint8_t OUT_X_H						= 0x01;
		const uint8_t OUT_Y_L						= 0x02;
		const uint8_t OUT_Y_H						= 0x03;
		const uint8_t OUT_Z_L						= 0x04;
		const uint8_t OUT_Z_H						= 0x05;
		const uint8_t StatusRegister				= 0x06;
		const uint8_t T_OUT_Z_L						= 0x07;
		const uint8_t T_OUT_Z_H						= 0x08;
		const uint8_t ControlRegister1				= 0x09;
		const uint8_t ControlRegister2				= 0x0A;
		const uint8_t SET_RESET_Period_Register		= 0x0B;

	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_Offset,
		typename T_OutputPin
	> class TCompassQMC5883LThermometer :
		public T_Enabled,
		public T_InFahrenheit,
		public T_Offset,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )
		_V_PROP_( Offset )

	public:
		inline bool GetIsEnabled()
		{
			return ( Enabled().GetValue() && T_OutputPin::GetPinIsConnected());
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_FClocked,
		typename T_FullScaleRange,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_OverSampleRatio,
		typename T_OverflowOutputPin,
		typename T_SampleRate,
		typename T_Thermometer
	> class QMC5883L_Compass :
		public T_Address,
		public T_Enabled,
		public T_FClocked,
		public T_FullScaleRange,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_OverSampleRatio,
		public T_OverflowOutputPin,
		public T_SampleRate,
		public T_Thermometer
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )
		_V_PIN_( OverflowOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( FullScaleRange )
		_V_PROP_( OverSampleRatio )
		_V_PROP_( SampleRate )
		_V_PROP_( Thermometer )

	protected:
		_V_PROP_( FClocked )

	public:
		void UpdateControlRegister1()
		{
			uint8_t AValue = ( FullScaleRange().GetValue() > 5 ) ? 0b00010001 : 0b00000001;

			if( SampleRate().GetValue() >= 200.0 )
				AValue |= 0b00001100;

			else if( SampleRate().GetValue() >= 100.0 )
				AValue |= 0b00001000;

			else if( SampleRate().GetValue() >= 50.0 )
				AValue |= 0b00000100;

			if( OverSampleRatio().GetValue() < 128 )
				AValue |= 0b11000000;

			else if( OverSampleRatio().GetValue() < 256 )
				AValue |= 0b10000000;

			else if( OverSampleRatio().GetValue() < 512 )
				AValue |= 0b01000000;

			WriteTo( QMC5883L::ControlRegister1, AValue );
		}

	protected:
		bool DataAvailable()
		{
			StartReadFrom( QMC5883L::StatusRegister, 1 );

			uint8_t	AStatus = C_I2C.read();
			return( AStatus & 1 );
		}

		void ReadCompass()
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! DataAvailable() )
				return;

			FClocked() = false;

			StartReadFrom( QMC5883L::OUT_X_L, 6 );

			int16_t AValues[ 3 ];

			for( int i = 0; i < 3; ++i )
			{
				AValues[ i ] = C_I2C.read();
				AValues[ i ] = ( int16_t( C_I2C.read() ) << 8 ) | AValues[ i ];
			}

//			Serial.println( AValues[ 0 ] );
//			Serial.println( AScale );
//return;
			float	AScale = ( FullScaleRange().GetValue() > 5 ) ? ( 8.0 * 1000 ) / 32768.0 : ( 2.0 * 1000 ) / 32768.0;
			

			T_OutputPins_X::SetPinValue( AValues[ 0 ] * AScale );
			T_OutputPins_Y::SetPinValue( AValues[ 1 ] * AScale );
			T_OutputPins_Z::SetPinValue( AValues[ 2 ] * AScale );

//			FChangeOnly() = true;
		}

		inline void Reset()
		{
			WriteTo( QMC5883L::ControlRegister2, 0b11000001 );
		}

	protected:
		void WriteTo( uint8_t ARegister, uint8_t AData )
		{
//			Serial.print( "WriteTo :" ); Serial.print( ARegister ); Serial.print( " " ); Serial.println( AData );
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( AData );
			C_I2C.endTransmission();
		}

		void StartReadFrom( uint8_t ARegister, uint8_t ALength )
		{
//			Serial.print( "StartReadFrom :" ); Serial.print( ARegister ); Serial.print( " " ); Serial.println( ALength );
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue()), ALength );
			while (C_I2C.available() < ALength);
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
			ReadCompass();
			SystemBeginLoopTemperature();
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	public:
		inline void SystemStart()
		{
			Reset();
//			uint8_t	AValue;

//			WriteTo( HMC5883L_ModeRegister, 0 );

			WriteTo( QMC5883L::SET_RESET_Period_Register, 0x01 );

			UpdateControlRegister1();

//			AValue = ( Bias().GetValue() & 0b11 ) + (( AveragedSamples().GetValue() & 0b11 ) << 5 );
//			WriteTo( HMC5883_REGISTER_MAG_CRA_REG_M, AValue );

//			AValue = ( Gain().GetValue() & 0b111 ) << 5;
//			WriteTo( HMC5883_REGISTER_MAG_CRB_REG_M, AValue );
//			Serial.println( AValue );
//			delay( 10000 );

//			UpdateIdle();
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue())
				return;

			if( FClocked() )
				ReadCompass();
		}

		inline void SystemStartTemperature()
		{
			SystemStart();
			SystemBeginLoopTemperature();
		}
		
		inline void SystemBeginLoopTemperature()
		{
			SystemLoopBegin();
			if( ! T_Thermometer::OutputPin().GetPinIsConnected() )
				return;

			StartReadFrom( QMC5883L::T_OUT_Z_L, 4 );

			int16_t AValue = C_I2C.read();
			AValue |= ( int16_t( C_I2C.read() ) << 8 );

			T_Thermometer::OutputPin().SetPinValue( Thermometer().Offset().GetValue() + ( float( AValue ) / 100.0 ));
		}

	public:
		inline QMC5883L_Compass()
		{
			FClocked() = false;
		}
	};
}

