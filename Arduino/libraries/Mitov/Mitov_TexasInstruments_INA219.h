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
	enum TINA219ResolutionAveragingMode
	{
      ram9_Bit,
      ram10_Bit,
      ram11_Bit,
      ram12_Bit,
      ram2_Samples,
      ram4_Samples,
      ram8_Samples,
      ram16_Samples,
      ram32_Samples,
      ram64_Samples,
      ram128_Samples
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Bus32VoltsRange,
		typename T_BusVoltageMode,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_OutputPins_Power,
		typename T_OutputPins_BusVoltage,
		typename T_OutputPins_Current,
		typename T_OutputPins_ShuntVoltage,
        typename T_OverflowOutputPin,
        typename T_PowerDown,
		typename T_ShuntResistor,
		typename T_ShuntVoltageGainOrder,
		typename T_ShuntVoltageMode,
		typename T_SingleShot
	> class TexasInstrumentsINA219 :
		public T_Address,
		public T_Bus32VoltsRange,
		public T_BusVoltageMode,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_OutputPins_Power,
		public T_OutputPins_BusVoltage,
		public T_OutputPins_Current,
		public T_OutputPins_ShuntVoltage,
        public T_OverflowOutputPin,
        public T_PowerDown,
		public T_ShuntResistor,
		public T_ShuntVoltageGainOrder,
		public T_ShuntVoltageMode,
		public T_SingleShot
	{
	public:
		_V_PIN_( OutputPins_Power )
		_V_PIN_( OutputPins_BusVoltage )
		_V_PIN_( OutputPins_Current )
		_V_PIN_( OutputPins_ShuntVoltage )
        _V_PIN_( OverflowOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( ShuntResistor )
        _V_PROP_( PowerDown )
//		uint16_t	FCalibrationValue;

		_V_PROP_( BusVoltageMode )
		_V_PROP_( ShuntVoltageMode )

		_V_PROP_( Enabled )
		_V_PROP_( Bus32VoltsRange )
		_V_PROP_( SingleShot )
		_V_PROP_( ShuntVoltageGainOrder )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		static const uint8_t INA219_REG_CONFIG			= 0x00;
		static const uint8_t INA219_REG_SHUNTVOLTAGE	= 0x01;
		static const uint8_t INA219_REG_BUSVOLTAGE		= 0x02;
		static const uint8_t INA219_REG_POWER			= 0x03;
		static const uint8_t INA219_REG_CURRENT			= 0x04;
		static const uint8_t INA219_REG_CALIBRATION		= 0x05;

	protected:
		void IntUpdateConfig( bool AReset )
		{
			uint8_t mode;
			bool ANeedsShunt = T_OutputPins_Power::GetPinIsConnected() || T_OutputPins_ShuntVoltage::GetPinIsConnected() || T_OutputPins_Current::GetPinIsConnected();
			bool ANeedsBus = T_OutputPins_Power::GetPinIsConnected() || T_OutputPins_BusVoltage::GetPinIsConnected();

            if( PowerDown() )
                mode = 0b000;

			if( SingleShot() && ClockInputPin_o_IsConnected() )
			{
				if( ANeedsShunt && ANeedsBus )
					mode = 0b011;

				else if( ANeedsShunt )
					mode = 0b001;

				else
					mode = 0b010;
			}

			else
			{
				if( ANeedsShunt && ANeedsBus )
					mode = 0b111;

				else if( ANeedsShunt )
					mode = 0b101;

				else
					mode = 0b110;
			}

//			mode = 0b111; // For now continuous

			uint16_t config =	(( AReset )				? 0b10000000 : 0 ) |
								(( Bus32VoltsRange() )	? 0b00100000 : 0 ) |
								( uint16_t( ShuntVoltageGainOrder() ) << 11 ) |
								( ResolutionToBits( BusVoltageMode() ) << 7 ) |
								( ResolutionToBits( ShuntVoltageMode() ) << 3 ) |
								mode;

//			Serial.println( "" );
//			Serial.println( config, BIN );
			WriteRegister( INA219_REG_CONFIG, config );
/*
			// Compute Callibration Register
			float VSHUNT_MAX = 0.04 * ( 1 << ShuntVoltageGainOrder );

//			Serial.println( "" );
//			Serial.println( "VSHUNT_MAX" );
//			Serial.println( VSHUNT_MAX );

			float MaxPossible_I = VSHUNT_MAX / ShuntResistor;

//			Serial.println( "MaxPossible_I" );
//			Serial.println( MaxPossible_I );

			float MinimumLSB = MaxPossible_I / 32767;
			float MaximumLSB = MaxPossible_I / 4096;

//			Serial.println( "MinimumLSB" );
//			Serial.println( MinimumLSB, 8 );

//			Serial.println( "MaximumLSB" );
//			Serial.println( MaximumLSB, 8 );

//			float ATest = MinimumLSB * 10000;
//			Serial.println( "ATest" );
//			Serial.println( ATest, 8 );

//			float ATest1 = MinimumLSB / 0.0001;
//			Serial.println( "ATest1" );
//			Serial.println( ATest1, 8 );

// 4. Choose an LSB between the min and max values
//    (Preferrably a roundish number close to MinLSB)
			float AMultiplier = 1;
			float CurrentLSB = MinimumLSB;
			while( CurrentLSB * AMultiplier < 1.0 )
				AMultiplier *= 10;

//			Serial.println( "AMultiplier" );
//			Serial.println( AMultiplier );

			CurrentLSB = trunc( CurrentLSB * AMultiplier );
//			Serial.println( "CurrentLSB" );
//			Serial.println( CurrentLSB, 8 );

			while( CurrentLSB / AMultiplier < MinimumLSB )
				CurrentLSB += 1;

//			Serial.println( "CurrentLSB" );
//			Serial.println( CurrentLSB, 8 );

			CurrentLSB /= AMultiplier;

// 5. Compute the calibration register
			FCalibrationValue = trunc(0.04096 / (CurrentLSB * ShuntResistor ));

//			Serial.println( "FCalibrationValue" );
//			Serial.println( FCalibrationValue );

// 6. Calculate the power LSB
			float PowerLSB = 20 * CurrentLSB;

			wireWriteRegister(INA219_REG_CALIBRATION, FCalibrationValue );
*/
		}

		uint16_t	ResolutionToBits( TINA219ResolutionAveragingMode AMode )
		{
			static const uint8_t AResults[] =
			{
				0b0000, // ram9_Bit
				0b0001, // ram10_Bit
				0b0010, // ram11_Bit
				0b0011, // ram12_Bit
				0b1001, // ram2_Samples
				0b1010, // ram4_Samples
				0b1011, // ram8_Samples
				0b1100, // ram16_Samples
				0b1101, // ram32_Samples
				0b1110, // ram64_Samples
				0b1111  // ram128_Samples
			};

			return AResults[ AMode ];
		}

	public:
		inline void UpdateConfig() // Used by setters!
		{
			IntUpdateConfig( false );
		}

	protected:
		uint16_t ReadRegister( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(ARegister);                       // Register
			C_I2C.endTransmission();

//			delay(1); // Max 12-bit conversion time is 586us per sample

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) );
			// Shift values to create properly formed integer
			uint16_t AValue = C_I2C.read();
			return (( AValue << 8 ) | C_I2C.read());
		}

		void WriteRegister( uint8_t ARegister, uint16_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(ARegister);                       // Register
			C_I2C.write((AValue >> 8) & 0xFF);       // Upper 8-bits
			C_I2C.write(AValue & 0xFF);              // Lower 8-bits
			C_I2C.endTransmission();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( true );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			IntUpdateConfig( true );
		}

	protected:
		void ReadData( bool ASingleShot )
		{
			if( ! Enabled() )
				return;

			if( ASingleShot && SingleShot().GetValue() )
				IntUpdateConfig( false ); // Force reading!

			bool APowerConnected = T_OutputPins_Power::GetPinIsConnected();
			bool ACurrentConnected = T_OutputPins_Current::GetPinIsConnected();
			bool AShuntVoltageConnected = T_OutputPins_ShuntVoltage::GetPinIsConnected();

			bool ABusVoltageConnected = T_OutputPins_BusVoltage::GetPinIsConnected() || T_OverflowOutputPin::GetPinIsConnected();
			bool ANeedsShuntVoltage = AShuntVoltageConnected || ACurrentConnected || APowerConnected;
			bool ANeedsCurrent = ACurrentConnected || APowerConnected;
			bool ANeedsBusVoltage = APowerConnected || ABusVoltageConnected || ASingleShot;

/*
			if( APowerConnected || ACurrentConnected )
			{
				// Sometimes a sharp load will reset the INA219, which will
				// reset the cal register, meaning CURRENT and POWER will
				// not be available ... avoid this by always setting a cal
				// value even if it's an unfortunate extra step
				WriteRegister(INA219_REG_CALIBRATION, FCalibrationValue );
			}
*/
			uint16_t ARawBusValue = 0;
			float ABusVoltage = 0;
			if( ANeedsBusVoltage )
			{
				do
				{
					ARawBusValue = ReadRegister( INA219_REG_BUSVOLTAGE );
				}
				while( ASingleShot && (( ARawBusValue & 0b10 ) == 0 ) );

                bool AOveflow = (( ARawBusValue & 0b1 ) != 0 );
                T_OverflowOutputPin::SetPinValue( AOveflow );

				ABusVoltage = ( ARawBusValue >> 1 ) & 0b1111111111111100;
				ABusVoltage *= 0.001;
			}

			float AShuntVoltage;
			float ACurrent;
			if( ANeedsShuntVoltage )
			{
				AShuntVoltage = (int16_t)ReadRegister( INA219_REG_SHUNTVOLTAGE );
				AShuntVoltage *= 0.01;

				if( ANeedsCurrent )
					ACurrent = ( AShuntVoltage / ShuntResistor() );
			}

			if( APowerConnected ) // Power
			{
//				float AValue = ReadRegister( INA219_REG_POWER );
				float AValue = ACurrent * ABusVoltage;
				T_OutputPins_Power::SetPinValue( AValue );
			}

    		T_OutputPins_BusVoltage::SetPinValue( ABusVoltage );
			T_OutputPins_Current::SetPinValue( ACurrent );
			T_OutputPins_ShuntVoltage::SetPinValue( AShuntVoltage );
		}

	public:
		inline void SystemStart()
		{
			UpdateConfig();
		}

		inline void SystemLoopBegin()
		{
			ReadData( false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif