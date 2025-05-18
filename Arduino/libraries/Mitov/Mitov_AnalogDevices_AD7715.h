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
	namespace AD7715Const
	{
		const uint8_t REG_COMM         = 0b00000000;
		const uint8_t REG_SETUP        = 0b00010000;
		const uint8_t REG_TEST         = 0b00100000;
		const uint8_t REG_DATA         = 0b00110000;
	}
//---------------------------------------------------------------------------
	template <
		typename T_FInCalibration
	> class TArduinoAD7715Calibration :
		public T_FInCalibration
	{
	public:
		_V_PROP_( FInCalibration )

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_Bipolar,
		typename T_Buffer,
		typename T_Calibration,
		typename T_ChipSelectOutputPin,
		typename T_Enabled,
		typename T_Gain,
		typename T_OutputPin,
		typename T_ResetOutputPin,
		typename T_SPISpeed,
		typename T_SampleRate,
		typename T_Standby
	> class AnalogDevicesAD7715 :
		public T_ChipSelectOutputPin,
		public T_Bipolar,
		public T_Buffer,
		public T_Calibration,
		public T_Enabled,
		public T_Gain,
		public T_ResetOutputPin,
		public T_OutputPin,
		public T_SPISpeed,
		public T_SampleRate,
		public T_Standby
	{
	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( OutputPin )
		_V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( Bipolar )
		_V_PROP_( Buffer )
		_V_PROP_( Calibration )
		_V_PROP_( Enabled )
		_V_PROP_( Gain )
		_V_PROP_( SPISpeed )
		_V_PROP_( SampleRate )
		_V_PROP_( Standby )

	protected:
		void StartSetupRegister( uint8_t ARegisterSelectMask, bool AReadOperation ) 
		{
			if( AReadOperation )
				ARegisterSelectMask |= 0b00001000;

			if( Standby() )
				ARegisterSelectMask |= 0b00000100;

			if( Gain().GetValue() > 32 )
				ARegisterSelectMask |= 0b00000011;

			else if( Gain().GetValue() > 2 )
				ARegisterSelectMask |= 0b00000010;

			else if( Gain().GetValue() > 1 )
				ARegisterSelectMask |= 0b00000001;

//			Serial.print( "RegisterMask: " );
//			Serial.println( ARegisterSelectMask, BIN );

			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegisterSelectMask );
//			C_SPI.transfer( value );
//			T_ChipSelectOutputPin::SetPinValueHigh();
//			C_SPI.endTransaction();
		}

		inline void EndTransaction()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		inline void WriteRegister( uint8_t ARegisterSelectMask, uint8_t AValue )
		{
			StartSetupRegister( ARegisterSelectMask, false );

			C_SPI.transfer( AValue );

			EndTransaction();
		}

		inline uint8_t ReadRegister( uint8_t ARegisterSelectMask )
		{
			StartSetupRegister( ARegisterSelectMask, true );

			uint8_t AResult = SPI.transfer( 0xFF );

			EndTransaction();

			return AResult;
		}

	public:
		void Update_REG_SETUP()
		{
			SetMode( 0b00000000 );
		}

	protected:
		void Reset()
		{
//			Serial.println( "Reset" );
			if( T_ResetOutputPin::GetPinIsConnected() )
			{
				T_ResetOutputPin::SetPinValueLow();
				delayMicroseconds( 1 );
				T_ResetOutputPin::SetPinValueHigh();
			}


/*
    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
//    SetCS(LOW);
    digitalWrite( 2, LOW );
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    digitalWrite( 2, HIGH );
    SPI.endTransaction();

    delay(5);

    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
    digitalWrite( 2, LOW );
    SPI.transfer( 0b00011001 );
    uint8_t ATest1 = SPI.transfer(0xFF);
    digitalWrite( 2, HIGH );
    SPI.endTransaction();
    
    Serial.println( ATest1 );
*/
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();

			SPI.transfer(0xFF);
			SPI.transfer(0xFF);
			SPI.transfer(0xFF);
			SPI.transfer(0xFF);

			EndTransaction();

			delay( 5 );

/*
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
			SPI.transfer( 0b00011001 );
			uint8_t ATest2 = SPI.transfer(0xFF);
			EndTransaction();
*/
//			Serial.print( "READ DATA : " );
//			Serial.println( ATest2 );

			Update_REG_SETUP();
		}

		void SetMode( uint8_t AMode )
		{
			if( Buffer() )
				AMode |= 0b00000010;

			if( ! Bipolar().GetValue() )
				AMode |= 0b00000100;

			if( SampleRate().GetValue() > 250 )
				AMode |= 0b00111000;

			else if( SampleRate().GetValue() > 200 )
				AMode |= 0b00110000;

			else if( SampleRate().GetValue() > 100 )
				AMode |= 0b00011000;

			else if( SampleRate().GetValue() > 60 )
				AMode |= 0b00010000;

			else if( SampleRate().GetValue() > 50 )
				AMode |= 0b00101000;

			else if( SampleRate().GetValue() > 25 )
				AMode |= 0b00100000;

			else if( SampleRate().GetValue() > 20 )
				AMode |= 0b00001000;

//			Serial.print( "Mode: " );
//			Serial.println( AMode, BIN );
			WriteRegister( AD7715Const::REG_SETUP, AMode );
		}

		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			if( Calibration().FInCalibration() )
				return;

			if( ! ( ReadRegister( AD7715Const::REG_COMM ) & 0b10000000 ))
			{
				StartSetupRegister( AD7715Const::REG_DATA, true );

//				C_SPI.transfer( AValue );
				float AValue = float( SPI.transfer16( 0xFFFF ) ) / 0xFFFF;

				T_OutputPin::SetPinValue( AValue, AChangeOnly );

//				Serial.println( AValue );

				EndTransaction();
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	public:
		inline void Calibration_SelfInputPin_o_Receive( void *_Data )
		{
			SetMode( 0b01000000 );
			Calibration().FInCalibration() = true;
		}
		
		inline void Calibration_ZeroScaleInputPin_o_Receive( void *_Data )
		{
			SetMode( 0b10000000 );
			Calibration().FInCalibration() = true;
		}

		inline void Calibration_FullScaleInputPin_o_Receive( void *_Data )
		{
			SetMode( 0b11000000 );
			Calibration().FInCalibration() = true;
		}

	public:
		inline void SystemStart()
		{
			Reset();
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

		inline void SystemLoopBeginCalibration()
		{
			if( Calibration().FInCalibration() )
				if( ! ( ReadRegister( AD7715Const::REG_SETUP ) & 0b11000000 ))
					Calibration().FInCalibration() = false;
		}

	public:
		inline AnalogDevicesAD7715()
		{
			Calibration().FInCalibration() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

