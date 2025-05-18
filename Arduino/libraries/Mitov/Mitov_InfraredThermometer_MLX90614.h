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
		typename T_OWNER,
		T_OWNER &C_OWNER,
		uint8_t	C_REGISTER,
		typename T_ErrorOutputPin,
		typename T_OutputPin
	> class TArduinoInfraredThermometerMLX90614Channel :
		public T_ErrorOutputPin,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ErrorOutputPin )

	public:
		inline void ReadSensor()
		{
			if( T_OutputPin::GetPinIsConnected() || T_ErrorOutputPin::GetPinIsConnected() )
			{
				int16_t ARawValue;
				if ( C_OWNER.I2CReadWord( C_REGISTER, ARawValue ))
				{
					// If the read succeeded
					if (ARawValue & 0x8000) // If there was a flag error
					{
						T_ErrorOutputPin::SetPinValueHigh();
						return;
					}

					T_ErrorOutputPin::SetPinValueLow();
					T_OutputPin::SetPinValue( C_OWNER.GetTemperature( ARawValue ));
				}
			}
		}
	};
//---------------------------------------------------------------------------
	namespace MLX90614Constants
	{
		const uint16_t	I2C_READ_TIMEOUT = 1000;

		const uint16_t MLX90614_REGISTER_TA			= 0x06;
		const uint16_t MLX90614_REGISTER_TOBJ1		= 0x07;
		const uint16_t MLX90614_REGISTER_TOBJ2		= 0x08;
		const uint16_t MLX90614_REGISTER_TOMAX		= 0x20;
		const uint16_t MLX90614_REGISTER_TOMIN		= 0x21;
		const uint16_t MLX90614_REGISTER_PWMCTRL	= 0x22;
		const uint16_t MLX90614_REGISTER_TARANGE	= 0x23;
		const uint16_t MLX90614_REGISTER_KE			= 0x24;
		const uint16_t MLX90614_REGISTER_CONFIG		= 0x25;
		const uint16_t MLX90614_REGISTER_ADDRESS	= 0x2E;
		const uint16_t MLX90614_REGISTER_ID0		= 0x3C;
		const uint16_t MLX90614_REGISTER_ID1		= 0x3D;
		const uint16_t MLX90614_REGISTER_ID2		= 0x3E;
		const uint16_t MLX90614_REGISTER_ID3		= 0x3F;
		const uint16_t MLX90614_COMMAND_SLEEP		= 0xFF;
	}
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_AmbientOutputPin,
		typename T_Channels_ReadSensor,
		typename T_EmissivityOutputPin,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_RangeOutputPins_Min,
		typename T_RangeOutputPins_Max
	> class InfraredThermometerMLX90614 :
		public T_Address,
		public T_AmbientOutputPin,
		public T_EmissivityOutputPin,
		public T_Enabled,
		public T_InFahrenheit,
		public T_RangeOutputPins_Min,
		public T_RangeOutputPins_Max
	{
	public:
		_V_PIN_( AmbientOutputPin )
		_V_PIN_( EmissivityOutputPin )
		_V_PIN_( RangeOutputPins_Min )
		_V_PIN_( RangeOutputPins_Max )

	public:
		_V_PROP_( Address )
		_V_PROP_( InFahrenheit )

	public:
		bool I2CReadWord( uint8_t reg, int16_t &dest )
		{
			int timeout = MLX90614Constants::I2C_READ_TIMEOUT;

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(reg);

			C_I2C.endTransmission(false); // Send restart
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), (uint8_t) 3);

			while ((C_I2C.available() < 3) && (timeout-- > 0))
				delay(1);

			if (timeout <= 0)
				return false;

			uint8_t lsb = C_I2C.read();
			uint8_t msb = C_I2C.read();
			uint8_t pec = C_I2C.read();

			uint8_t crc = crc8(0, ( uint8_t( Address().GetValue() ) << 1));
			crc = crc8(crc, reg);
			crc = crc8(crc, ( uint8_t( Address().GetValue() ) << 1) + 1);
			crc = crc8(crc, lsb);
			crc = crc8(crc, msb);

			if (crc != pec)
				return false;

			dest = (msb << 8) | lsb;
			return true;
		}

		uint8_t I2CWriteWord( uint8_t reg, int16_t data)
		{
			uint8_t crc;
			uint8_t lsb = data & 0x00FF;
			uint8_t msb = (data >> 8);

			crc = crc8(0, ( uint8_t( Address().GetValue() ) << 1));
			crc = crc8(crc, reg);
			crc = crc8(crc, lsb);
			crc = crc8(crc, msb);

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(reg);
			C_I2C.write(lsb);
			C_I2C.write(msb);
			C_I2C.write(crc);
			return C_I2C.endTransmission(true);
		}

		bool writeEEPROM( uint8_t reg, int16_t data )
		{
			// Clear out EEPROM first:
			if (I2CWriteWord(reg, 0) != 0)
				return false; // If the write failed, return 0

			delay(5); // Delay tErase

			uint8_t i2cRet = I2CWriteWord(reg, data);
			delay(5); // Delay tWrite

			return (i2cRet == 0);
		}

		bool writeTemperatureEEPROM( uint8_t reg, float AValue )
		{
			if( InFahrenheit().GetValue() )
				AValue = (AValue - 32.0) * 5.0 / 9.0;

			AValue = ( AValue + 273.15 ) * 50;
			int16_t rawTemp = (int16_t)AValue;

			writeEEPROM( reg, rawTemp );
		}

		void setAddress(uint8_t newAdd)
		{
			// Make sure the address is within the proper range:
			if ((newAdd >= 0x80) || (newAdd == 0x00))
				return; // Return fail if out of range

			int16_t tempAdd;

			// Read from the I2C address address first:
			if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_ADDRESS, tempAdd))
			{
				tempAdd &= 0xFF00; // Mask out the address (MSB is junk?)
				tempAdd |= newAdd; // Add the new address

				// Write the new addres back to EEPROM:
				 writeEEPROM( MLX90614Constants::MLX90614_REGISTER_ADDRESS, tempAdd );
			}
		}

/*
		void setConfig( TArduinoInfraredThermometerMLX90614SetConfigBasic *AConfig )
		{
			static const uint8_t CIIRFilterValues[] = { 0, 0b111, 0b110, 0b101, 0b100 };
			static const float CGainCiefficients[] =
			{
				1.0,
				3.0,
				6.0,
				12.5,
				25.0,
				50.0,
				100.0
			};

			uint16_t ACoeffOrder = MITOV_ARRAY_SIZE( CGainCiefficients ) - 1;
			for( int i = MITOV_ARRAY_SIZE( CGainCiefficients ); i--; )
			{
				if( AConfig->Gain > CGainCiefficients[ i ] )
					break;

				ACoeffOrder = i;

				if( AConfig->Gain == CGainCiefficients[ i ] )
					break;

			}

			uint16_t AValue =	CIIRFilterValues[ AConfig->IIRCoeficients ] |
								( AConfig->UsePositiveTemperatureCoefficientSensor ? 0b1000 : 0 ) |
								( uint16_t( AConfig->FIRCoeficientsOrder ) << 8 ) |
								( ACoeffOrder << 11 )
								( AConfig->NegativeThermoshockCompensation ? 0x8000 : 0 );

			uint16_t ARegisterValue;
			if( ! I2CReadWord( MLX90614Constants::MLX90614_REGISTER_CONFIG, ARegisterValue ))
				return;

			ARegisterValue &=	( uint16_t( 1 ) << 14 ) |
								( uint16_t( 1 ) << 7 ) |
								( uint16_t( 1 ) << 6 ) |
								( uint16_t( 1 ) << 5 ) |
								( uint16_t( 1 ) << 4 );

			ARegisterValue |= AValue;
//			writeEEPROM( MLX90614Constants::MLX90614_REGISTER_CONFIG, ARegisterValue );
		}
*/
	public:
		float GetTemperature( int16_t ARawValue )
		{
			float AResult = float(ARawValue) * 0.02 - 273.15;
			if( InFahrenheit() )
				return AResult * 9.0 / 5.0 + 32;

			return AResult;
		}

	protected:
		uint8_t crc8 (uint8_t inCrc, uint8_t inData)
		{
			uint8_t data = inCrc ^ inData;
			for ( uint8_t i = 0; i < 8; i++ )
			{
				if (( data & 0x80 ) != 0 )
				{
					data <<= 1;
					data ^= 0x07;
				}
				else
					data <<= 1;
			}

			return data;
		}

		void ReadSensor()
		{
			int16_t ARawValue;

			T_Channels_ReadSensor::Call();

/*
//			if( Channels[ 0 ].OutputPin.IsConnected() || Channels[ 0 ].ErrorOutputPin.IsConnected() )
			{
				if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_TOBJ1, ARawValue ))
				{
					// If the read succeeded
					if (ARawValue & 0x8000) // If there was a flag error
					{
						Channels[ 0 ].ErrorOutputPin.SendValue( true );
						return;
					}

//					Serial.println( ARawValue );
					Channels[ 0 ].ErrorOutputPin.SendValue( false );
					Channels[ 0 ].OutputPin.SetValue( GetTemperature( ARawValue ));
				}
			}

			if( Channels[ 1 ].OutputPin.IsConnected() || Channels[ 1 ].ErrorOutputPin.IsConnected() )
			{
				if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_TOBJ2, ARawValue ))
				{
					// If the read succeeded
					if (ARawValue & 0x8000) // If there was a flag error
					{
						Channels[ 1 ].ErrorOutputPin.SendValue( true );
						return;
					}

					Channels[ 1 ].ErrorOutputPin.SendValue( false );
					Channels[ 1 ].OutputPin.SetValue( GetTemperature( ARawValue ));
				}
			}
*/
//			if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_CONFIG, ARawValue ))
//				Serial.println( ARawValue, HEX );


//#ifdef _MITOV_MLX90614_AMBIENT_PIN_
			if( T_AmbientOutputPin::GetPinIsConnected() )
				if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_TA, ARawValue ))
					T_AmbientOutputPin::SetPinValue( GetTemperature( ARawValue ));
//#endif

//#ifdef _MITOV_MLX90614_EMISSIVITY_PIN_
			if( T_EmissivityOutputPin::GetPinIsConnected() )
				if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_KE, ARawValue ))
				{
					float AValue = (((float)((uint16_t)ARawValue)) / 65535.0);
					T_EmissivityOutputPin::SetPinValue( AValue );
				}
//#endif

//#ifdef _MITOV_MLX90614_RANGE_PINS_
			if( T_RangeOutputPins_Min::GetPinIsConnected() )
				if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_TOMIN, ARawValue ))
					T_RangeOutputPins_Min::SetPinValue( GetTemperature( ARawValue ));

			if( T_RangeOutputPins_Max::GetPinIsConnected() )
				if (I2CReadWord( MLX90614Constants::MLX90614_REGISTER_TOMAX, ARawValue ))
					T_RangeOutputPins_Max::SetPinValue( GetTemperature( ARawValue ));
//#endif
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_Value
	> class TArduinoInfraredThermometerMLX90614SetEmissivityElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void SystemStart()
		{
			// Calculate the raw 16-bit value:
			uint16_t ke = uint16_t( 65535.0 * Value().GetValue() );
			ke = constrain( ke, 0x2000, 0xFFFF );

			// Write that value to the ke register
			C_INSTANCE.writeEEPROM( MLX90614Constants::MLX90614_REGISTER_KE, (int16_t)ke);
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SystemStart();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_Value
	> class TArduinoInfraredThermometerMLX90614SetRangeMinElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void SystemStart()
		{
			C_INSTANCE.writeTemperatureEEPROM( MLX90614Constants::MLX90614_REGISTER_TOMIN, Value() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SystemStart();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_Value
	> class TArduinoInfraredThermometerMLX90614SetRangeMaxElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void SystemStart()
		{
			C_INSTANCE.writeTemperatureEEPROM( MLX90614Constants::MLX90614_REGISTER_TOMAX, Value() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SystemStart();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_Value
	> class TArduinoInfraredThermometerMLX90614SetAddressElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void SystemStart()
		{
			C_INSTANCE.setAddress( Value() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SystemStart();
		}

	};
//---------------------------------------------------------------------------
	enum TArduinoInfraredThermometerMLX90614IIRCoefficients { iir_a_0_5__b_0_5, iir_a_0_571__b_0_429, iir_a_0_667__b_0_333, iir_a_0_8__b_0_2, iir_Off };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_INSTANCE,
		typename T_FIRCoeficientsOrder,
		typename T_Gain,
		typename T_IIRCoeficients,
		typename T_NegativeThermoshockCompensation,
		typename T_UsePositiveTemperatureCoefficientSensor
	> class TArduinoInfraredThermometerMLX90614SetConfigElement :
		public T_FIRCoeficientsOrder,
		public T_Gain,
		public T_IIRCoeficients,
		public T_NegativeThermoshockCompensation,
		public T_UsePositiveTemperatureCoefficientSensor
	{
	public:
		_V_PROP_( IIRCoeficients )
		_V_PROP_( UsePositiveTemperatureCoefficientSensor )
		_V_PROP_( NegativeThermoshockCompensation )
		_V_PROP_( FIRCoeficientsOrder )
		_V_PROP_( Gain )

	protected:
		void setConfig()
		{
			static const uint8_t CIIRFilterValues[] = { 0, 0b111, 0b110, 0b101, 0b100 };
			static const float CGainCiefficients[] =
			{
				1.0,
				3.0,
				6.0,
				12.5,
				25.0,
				50.0,
				100.0
			};

			uint16_t ACoeffOrder = MITOV_ARRAY_SIZE( CGainCiefficients ) - 1;
			for( int i = MITOV_ARRAY_SIZE( CGainCiefficients ); i--; )
			{
				if( Gain().GetValue() > CGainCiefficients[ i ] )
					break;

				ACoeffOrder = i;

				if( Gain().GetValue() == CGainCiefficients[ i ] )
					break;

			}

			uint16_t AValue =	CIIRFilterValues[ IIRCoeficients().GetValue() ] |
								( UsePositiveTemperatureCoefficientSensor().GetValue() ? 0b1000 : 0 ) |
								( uint16_t( FIRCoeficientsOrder().GetValue() - 3 ) << 8 ) |
								( ACoeffOrder << 11 ) |
								( NegativeThermoshockCompensation().GetValue() ? 0x8000 : 0 );

			int16_t ARegisterValue;
			if( ! C_INSTANCE.I2CReadWord( MLX90614Constants::MLX90614_REGISTER_CONFIG, ARegisterValue ))
				return;

			uint16_t AMask =	( uint16_t( 1 ) << 14 ) |
								( uint16_t( 1 ) << 7 ) |
								( uint16_t( 1 ) << 6 ) |
								( uint16_t( 1 ) << 5 ) |
								( uint16_t( 1 ) << 4 );

//			Serial.println( ARegisterValue, BIN );
//			Serial.println( AMask, BIN );
//			Serial.println( AValue, BIN );

			ARegisterValue &= AMask;
			ARegisterValue |= AValue;

//			Serial.println( ARegisterValue, BIN );

			C_INSTANCE.writeEEPROM( MLX90614Constants::MLX90614_REGISTER_CONFIG, ARegisterValue );
		}

	public:
		inline void SystemStart()
		{
			setConfig();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SystemStart();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif