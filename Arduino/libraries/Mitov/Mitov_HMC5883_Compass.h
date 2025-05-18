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

//	const uint8_t HMC5883L_Address = 0x1E;
	const uint8_t HMC5883L_StatusRegister = 9;
	const uint8_t HMC5883L_ModeRegister = 2;
	const uint8_t HMC5883_REGISTER_MAG_CRA_REG_M = 0x00;
	const uint8_t HMC5883_REGISTER_MAG_CRB_REG_M = 0x01;
	const uint8_t HMC5883_REGISTER_MAG_OUT_X_H_M = 0x03;
//---------------------------------------------------------------------------
	enum TCompassHMC5883LAveragedSamples { cas1, cas2, cas4, cas8 };
	enum TCompassHMC5883LBias { cbNone, cbPositive, cbNegative };
	enum TCompassHMC5883LGain { cg1370, cg1090, cg820, cg660, cg440, cg390, cg330, cg230 };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_AveragedSamples,
		typename T_Bias,
		typename T_Enabled,
//		typename T_FChangeOnly,
		typename T_FClocked,
		typename T_Gain,
		typename T_Idle,
		typename T_XOutputPin,
		typename T_YOutputPin,
		typename T_ZOutputPin
	> class HMC5883L_Compass :
		public T_Address,
		public T_AveragedSamples,
		public T_Bias,
		public T_Enabled,
//		public T_FChangeOnly,
		public T_FClocked,
		public T_Gain,
		public T_Idle,
		public T_XOutputPin,
		public T_YOutputPin,
		public T_ZOutputPin
	{
	public:
		_V_PIN_( XOutputPin )
		_V_PIN_( YOutputPin )
		_V_PIN_( ZOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( AveragedSamples )
		_V_PROP_( Bias )
		_V_PROP_( Gain )
		_V_PROP_( Idle )

	protected:
		_V_PROP_( FClocked )
//		_V_PROP_( FChangeOnly )

//	protected:
//		float	FScale;

	public:
		void UpdateIdle()
		{
			if( Idle() )
				WriteTo( uint8_t( Address().GetValue()), 0b11 ); // Idle

			else
				WriteTo( uint8_t( Address().GetValue()), 0b00 ); // Single measurement mode				
//				WriteTo( uint8_t( Address().GetValue()), 0b01 ); // Single measurement mode				
		}

	protected:
		bool DataAvailable()
		{
			StartReadFrom( HMC5883L_StatusRegister, 1 );

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

			StartReadFrom( HMC5883_REGISTER_MAG_OUT_X_H_M, 6 );

			int16_t AValues[ 3 ];

			{ // Free Stack Block
				uint8_t	ABytes[ 2 ];
				for( int i = 0; i < 3; ++i )
				{
					ABytes[ 0 ] = C_I2C.read();
					ABytes[ 1 ] = C_I2C.read();
					AValues[ i ] = ( int16_t( ABytes[ 0 ] ) << 8 ) | ABytes[ 1 ];
				}
			}

			float	AScale;
			switch( Gain() )
			{
				case cg1370: AScale = 0.73; break;
				case cg1090: AScale = 0.92; break;
				case cg820: AScale = 1.22; break;
				case cg660: AScale = 1.52; break;
				case cg440: AScale = 2.27; break;
				case cg390: AScale = 2.56; break;
				case cg330: AScale = 3.03; break;
				case cg230: AScale = 4.35; break;
			}

			T_XOutputPin::SetPinValue( AValues[ 0 ] * AScale ); //, FChangeOnly() );
			T_YOutputPin::SetPinValue( AValues[ 2 ] * AScale ); //, FChangeOnly() ); // Y is actually last!
			T_ZOutputPin::SetPinValue( AValues[ 1 ] * AScale ); //, FChangeOnly() ); // Z is second

//			FChangeOnly() = true;
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
		}

	public:
		inline void SystemStart()
		{
			uint8_t	AValue;

			WriteTo( HMC5883L_ModeRegister, 0 );

			AValue = ( Bias().GetValue() & 0b11 ) + (( AveragedSamples().GetValue() & 0b11 ) << 5 );
			WriteTo( HMC5883_REGISTER_MAG_CRA_REG_M, AValue );

			AValue = ( Gain().GetValue() & 0b111 ) << 5;
			WriteTo( HMC5883_REGISTER_MAG_CRB_REG_M, AValue );
//			Serial.println( AValue );
//			delay( 10000 );

			UpdateIdle();
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() && FClocked() )
				ReadCompass();
		}

	public:
		inline HMC5883L_Compass()
		{
			FClocked() = false;
//			FChangeOnly() = false;
		}
	};
}

