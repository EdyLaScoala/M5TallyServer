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
	typedef enum
	{
		cmSingleMeasurement,
		cm8Hz,
		cm100Hz,
		cmExternalTrigger
	} TArduinoCompassAK8963Mode;
//---------------------------------------------------------------------------
	typedef TArduinoCompassAK8963Mode TArduinoMPU9250CompassMode;
//---------------------------------------------------------------------------
	namespace CompassAK8963Const
	{
		static const uint8_t CompassModes[] =
		{
			0b00000001,
			0b00000010,
			0b00000110,
			0b00000100
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_HighResolution,
		typename T_Mode,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_OverflowOutputPin,
		typename T_SelfTest
	> class CompassAK8963 :
		public T_Address,
		public T_Enabled,
		public T_HighResolution,
		public T_Mode,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_OverflowOutputPin,
		public T_SelfTest
	{
	protected:
		static const uint8_t MPU9150_RA_MAG_ST1		= 0x02;
		static const uint8_t MPU9150_RA_MAG_XOUT_L		= 0x03;
		static const uint8_t MPU9150_RA_MAG_XOUT_H		= 0x04;
		static const uint8_t MPU9150_RA_MAG_YOUT_L		= 0x05;
		static const uint8_t MPU9150_RA_MAG_YOUT_H		= 0x06;
		static const uint8_t MPU9150_RA_MAG_ZOUT_L		= 0x07;
		static const uint8_t MPU9150_RA_MAG_ZOUT_H		= 0x08;
		static const uint8_t MPU9150_RA_MAG_ST2		= 0x09;
		static const uint8_t MPU9150_RA_MAG_CNTL1		= 0x0A;
		static const uint8_t MPU9150_RA_MAG_CNTL2		= 0x0B;
		static const uint8_t MPU9150_RA_MAG_ASTC		= 0x0C;

		static const uint8_t MPU9150_RA_MAG_ASAX		= 0x10;
		static const uint8_t MPU9150_RA_MAG_ASAY		= 0x11;
		static const uint8_t MPU9150_RA_MAG_ASAZ		= 0x12;

	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

		_V_PIN_( OverflowOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Mode )
		_V_PROP_( Enabled )
		_V_PROP_( SelfTest ) // Added to save space as bitfield
		_V_PROP_( HighResolution )

	public:
		void UpdateCompassSelfTestReg()
		{
			uint8_t AValue = ( SelfTest()	? 0b01000000 : 0 );
			WriteByte( MPU9150_RA_MAG_ASTC, AValue );
		}

		void UpdateCompassControlReg()
		{
			uint8_t AValue;
			if( Enabled() )
				AValue = CompassAK8963Const::CompassModes[ Mode() ];

			else
				AValue = 0;

			AValue |= ( HighResolution()	? 0b00010000 : 0 );

			WriteByte( MPU9150_RA_MAG_CNTL1, AValue );
		}

	protected:
		float CompassAdjustmentValues[ 3 ];

	protected:
		void PerformRead()
		{
			static const float CompassCoefficients[] =
			{
				10.0f *4219.0f / 8190.0f,
				10.0f *4219.0f / 32760.0f
			};

			uint8_t AIntValues[ 7 ];
			if( ReadBytes( MPU9150_RA_MAG_XOUT_L, 7, AIntValues ))
				for( int i = 0; i < 3; ++i )
				{
					int16_t AIntValue = ( int16_t( int8_t( AIntValues[ i * 2 + 1 ] )) << 8 ) | AIntValues[ i * 2 ];
					float AValue = AIntValue * CompassCoefficients[ HighResolution() & 1 ] * CompassAdjustmentValues[ i ];
					switch( i )
					{
						case 0: T_OutputPinsX::SetPinValue( AValue ); break;
						case 1: T_OutputPinsY::SetPinValue( AValue ); break;
						case 2: T_OutputPinsZ::SetPinValue( AValue ); break;
//					OutputPins[ i ].Notify( &AValue );
					}
				}

		}

		void ReadSensors()
		{
			uint8_t AIntValue;

			if( Mode() == cmSingleMeasurement )
			{
				UpdateCompassControlReg();
				delay(10);
				PerformRead();
			}

			if( ReadBytes( MPU9150_RA_MAG_ST1, 1, &AIntValue ))
			{
				if( AIntValue & 0b00000010 )
					T_OverflowOutputPin::ClockPin();

				if( AIntValue & 0b00000001 )
					PerformRead();
			}

		}

		void ReagCompassAdjustmentValues()
		{
			uint8_t AValues[ 3 ];

			ReadBytes( MPU9150_RA_MAG_ASAX, sizeof( AValues ), AValues );
			for( int i = 0; i < 3; ++i )
				CompassAdjustmentValues[ i ] = (((float) AValues[ i ] ) - 128.0f) / 256.0f + 1.0f;

		}

	protected:
		inline void WriteByte( uint8_t ARegister, uint8_t AData )
		{
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue() ), ARegister, AData );
		}

		bool ReadBytes( uint8_t regAddr, uint8_t length, void *data )
		{
			return I2C::ReadBytes( C_I2C, uint8_t( Address().GetValue()), regAddr, length, data );
		}

	public:
		inline void SystemStart()
		{
			UpdateCompassControlReg();
			UpdateCompassSelfTestReg();
			ReagCompassAdjustmentValues();
		}

		inline void SystemLoopBegin()
		{

			if( Enabled() )
				ReadSensors();

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			WriteByte( MPU9150_RA_MAG_CNTL2, 0b00000001 );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif