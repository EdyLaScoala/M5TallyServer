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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class TArduinoEVIVEADCTemperature :
		public T_ClockInputPin_o_IsConnected,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InFahrenheit )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_OutputPin
	> class TArduinoEVIVEVoltageProbe :
		public T_ClockInputPin_o_IsConnected,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_OutputPin,
		typename T_Voltage
	> class TArduinoEVIVECurrentVoltageProbe :
		public T_ClockInputPin_o_IsConnected,
		public T_OutputPin,
		public T_Voltage
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Voltage )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	};
//---------------------------------------------------------------------------
	template <
		typename T_CurrentVoltage,
		typename T_LowBandwith,
		typename T_PoweredUp,
		typename T_SampleFrequency,
		typename T_Temperature,
		typename T_Voltage
	> class ArduinoEVIVEAnalogMeasurement :
		public T_CurrentVoltage,
		public T_LowBandwith,
		public T_PoweredUp,
		public T_SampleFrequency,
		public T_Temperature,
		public T_Voltage
	{
	protected:
		static const uint8_t SPI_ADC_SS = 35;    // SPI ADC SS
		static const uint8_t ADC_RDY = 32;      // ADC Ready pin

		static const uint8_t ADE791X_REG_IWV = 0x00;    // Instantaneous value of Current I
		static const uint8_t ADE791X_REG_V1WV = 0x01;   // Instantaneous value of Voltage V1
		static const uint8_t ADE791X_REG_V2WV = 0x02;   // Instantaneous value of Voltage V2

		static const uint8_t ADE791X_REG_CONFIG = 0x08; // Configuration register. See Table 15 for details

		static const uint8_t ADE791X_REG_TEMPOS = 0x18;	// Temperature sensor offset

		// Command Definitions
		static const uint8_t ADE791X_READ = 0x04;
		static const uint8_t ADE791X_WRITE = 0x00;

		static const int32_t ADE791X_OFFSET_V1WV = 362378;	//387161;   //Adjust
		static const int32_t ADE791X_OFFSET_VIMWV = 369226; //Adjust  
		static const int32_t ADE791X_OFFSET_IWV = 250000;	//369226;   //Adjust

		//For V1WV 5,320,000 reading = 34.5V  (Multiplier = 0.006485) mV
		static constexpr float ADE791X_MUL_V1WV = 0.006485;
		//For V2WV 5,320,000Num reading = 6.3315V (Multiplier = 0.0011901) mV
		static constexpr float ADE791X_MUL_VIMWV = 0.0011901;
		//For IWV 5,320,000 reading = 3.15A (Multiplier = 0.0005921) mA
		static constexpr float ADE791X_MUL_IWV = 0.0005921;

	public:
		_V_PROP_( Voltage )
		_V_PROP_( CurrentVoltage )
		_V_PROP_( Temperature )

	public:
		_V_PROP_( SampleFrequency )
		_V_PROP_( PoweredUp )
		_V_PROP_( LowBandwith )

	public:
		void SetSampleFrequency( uint32_t AOldValue, uint32_t AValue )
		{
			static const uint8_t CLevels[] =
			{
				8,
				4,
				2,
				1
			};

			int ASetValue = 0;
			for( int i = 0; i < sizeof( CLevels ) / sizeof( CLevels[ 0 ] ); ++i )
				if( AValue >= CLevels[ i ] )
				{
					ASetValue = i;
					break;
				}

			if( CLevels[ ASetValue ] == AOldValue )
				return;

			SampleFrequency() = CLevels[ ASetValue ];
			UpdateConfig( false );
		}

		void UpdateConfig( bool AReset )
		{
			static const uint8_t CLevels[] =
			{
				8,
				4,
				2,
				1
			};

			int AIndex;
			for( AIndex = 0; AIndex < 4; ++AIndex )
				if( CLevels[ AIndex ] == SampleFrequency() )
					break;

//			Serial.println( "--------" );

			uint8_t	AValue = AIndex << 4;
			if( ! PoweredUp().GetValue() )
				AValue |= 0b00000100;

			if( AReset )
				AValue |= 0b01000000;

			if( LowBandwith() )
				AValue |= 0b10000000;

//			Serial.println( SampleFrequency );
//			Serial.println( AIndex );
//			Serial.println( AValue, BIN );
//			delay( 1000 );

			WriteRegister( ADE791X_REG_CONFIG, AValue );
		}

	protected:
		float	ReadValue( uint8_t ARegister )
		{
			ARegister = ARegister << 3;  // left shift address by 3 bits
			uint8_t opcode = (ARegister | ADE791X_READ);     // forms opcode byte

			SPI.beginTransaction( SPISettings( 8000000, MSBFIRST, SPI_MODE3 ));

			// take the SS pin low to select the chip:
			PORTC &= 0b11111011;
			SPI.transfer(opcode);        // send out opcode

			uint8_t	tempValue1 = SPI.transfer(0xFF);  // read MS Byte
			uint8_t	tempValue2 = SPI.transfer(0xFF);  // read mid Byte
			uint8_t	tempValue3 = SPI.transfer(0xFF);  // LS Byte

			// take the SS pin high to de-select the chip:
			PORTC |= 0b00000100;
			SPI.endTransaction();

			uint32_t AResult = ( uint32_t( tempValue1 ) << 16 ) | ( uint32_t( tempValue2 ) << 8 ) | tempValue3;

			AResult <<= 8;        // sign extends value to 32 bit
			return float( AResult ) / 0x100;    // converts back value to 24 bit but now sign extended
		}

		uint8_t	ReadRegisted( uint8_t ARegister )
		{
			ARegister = ARegister << 3;  // left shift address by 3 bits
			uint8_t opcode = (ARegister | ADE791X_READ);     // forms opcode byte

			SPI.beginTransaction( SPISettings( 8000000, MSBFIRST, SPI_MODE3 ));

			// take the SS pin low to select the chip:
			PORTC &= 0b11111011;
			SPI.transfer(opcode);        // send out opcode

			uint8_t	AResult = SPI.transfer(0xFF);  // read MS Byte

			// take the SS pin high to de-select the chip:
			PORTC |= 0b00000100;
			SPI.endTransaction();

			return AResult;
		}

		void WriteRegister( uint8_t ARegister, uint8_t AValue )
		{
			ARegister = ARegister << 3;  // left shift address by 3 bits
			uint8_t opcode = (ARegister | ADE791X_WRITE);     // forms opcode byte

			SPI.beginTransaction( SPISettings( 8000000, MSBFIRST, SPI_MODE3 ));

			// take the SS pin low to select the chip:
			PORTC &= 0b11111011;
			SPI.transfer(opcode);        // send out opcode
			SPI.transfer( AValue );

			// take the SS pin high to de-select the chip:
			PORTC |= 0b00000100;
			SPI.endTransaction();
		}

	public:
//#ifdef _MITOV_EVIVE_TEMPERATURE_CLOCK_PIN_
		void Temperature_ClockInputPin_o_Receive( void *_Data )
		{
			float ACoeff = ReadRegisted( ADE791X_REG_TEMPOS );
//			Serial.println( ACoeff );
			float AValue = ReadValue( ADE791X_REG_V2WV );

			float AGain;
			if( LowBandwith() )
				AGain = 8.21015;

			else
				AGain = 8.72101;

			AValue = AGain * AValue * 0.00001 + 8.72101 * 0.00001 * ACoeff * 2048 - 306.47;

			Temperature().OutputPin().SetPinValue( AValue );
		}
//#endif

//#ifdef _MITOV_EVIVE_VOLTAGE_CLOCK_PIN_
		void Voltage_ClockInputPin_o_Receive( void *_Data )
		{
			float AValue = ReadValue( ADE791X_REG_V1WV );

			AValue = ( AValue - ADE791X_OFFSET_V1WV ) * ADE791X_MUL_V1WV;
			AValue /= 1000;
			Voltage().OutputPin().SetPinValue( AValue );
		}
//#endif

//#ifdef _MITOV_EVIVE_VOLTAGE_CURRENT_CLOCK_PIN_
		void CurrentVoltage_ClockInputPin_o_Receive( void *_Data )
		{
			float AValue = ReadValue( ADE791X_REG_IWV );
			if( CurrentVoltage().Voltage() )
				AValue = ( AValue - ADE791X_OFFSET_VIMWV ) * ADE791X_MUL_VIMWV;

			else
				AValue = ( AValue - ADE791X_OFFSET_IWV ) * ADE791X_MUL_IWV;

			AValue /= 1000;
			CurrentVoltage().OutputPin().SetPinValue( AValue );
		}
//#endif

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			UpdateConfig( true );
		}

	public:
		inline void SystemStart()
		{
			pinMode( SPI_ADC_SS, OUTPUT );

			// take the SS pin high to de-select the chip:
			digitalWrite(SPI_ADC_SS, HIGH);

			// initialize SPI:
			SPI.begin();

			UpdateConfig( false );
		}

		inline void SystemLoopUpdateHardware()
		{
			if( ! PoweredUp() )
				return;

			float AValue;

//#ifdef _MITOV_EVIVE_VOLTAGE_OUTPUT_PIN_
//#ifndef _MITOV_EVIVE_VOLTAGE_CLOCK_PIN_
			if( ! Voltage().ClockInputPin_o_IsConnected() )
				if( Voltage().OutputPin().GetPinIsConnected() )
				{
					AValue = ReadValue( ADE791X_REG_V1WV );

					AValue = ( AValue - ADE791X_OFFSET_V1WV ) * ADE791X_MUL_V1WV;
					AValue /= 1000;
					Voltage().OutputPin().SetPinValue( AValue );
				}
//#endif // _MITOV_EVIVE_VOLTAGE_CLOCK_PIN_
//#endif // _MITOV_EVIVE_VOLTAGE_OUTPUT_PIN_

//#ifdef _MITOV_EVIVE_VOLTAGE_CURRENT_OUTPUT_PIN_
//#ifndef _MITOV_EVIVE_VOLTAGE_CURRENT_CLOCK_PIN_
			if( ! CurrentVoltage().ClockInputPin_o_IsConnected() )
				if( CurrentVoltage().OutputPin().GetPinIsConnected() )
				{
					AValue = ReadValue( ADE791X_REG_IWV );
					if( CurrentVoltage().Voltage() )
						AValue = ( AValue - ADE791X_OFFSET_VIMWV ) * ADE791X_MUL_VIMWV;

					else
						AValue = ( AValue - ADE791X_OFFSET_IWV ) * ADE791X_MUL_IWV;

					AValue /= 1000;
					CurrentVoltage().OutputPin().SetPinValue( AValue );
				}
//#endif // _MITOV_EVIVE_VOLTAGE_CURRENT_CLOCK_PIN_
//#endif // _MITOV_EVIVE_VOLTAGE_CURRENT_OUTPUT_PIN_


//#ifdef _MITOV_EVIVE_TEMPERATURE_OUTPUT_PIN_
//#ifndef _MITOV_EVIVE_TEMPERATURE_CLOCK_PIN_
			if( ! Temperature().ClockInputPin_o_IsConnected() )
				if( Temperature().OutputPin().GetPinIsConnected() )
				{
					float ACoeff = ReadRegisted( ADE791X_REG_TEMPOS );
//					Serial.println( ACoeff );
					AValue = ReadValue( ADE791X_REG_V2WV );

					float AGain;
					if( LowBandwith() )
						AGain = 8.21015;

					else
						AGain = 8.72101;

					AValue = AGain * AValue * 0.00001 + 8.72101 * 0.00001 * ACoeff * 2048 - 306.47;

					Temperature().OutputPin().SetPinValue( AValue );
				}
//#endif // _MITOV_EVIVE_TEMPERATURE_CLOCK_PIN_
//#endif // _MITOV_EVIVE_TEMPERATURE_OUTPUT_PIN_

//			Serial.println( AValue );
//			UpdateAll();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

