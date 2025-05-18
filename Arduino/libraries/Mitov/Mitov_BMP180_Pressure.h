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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//	const uint8_t BMP180_Address = 0x77; // 7-bit address
	const uint8_t BMP180_REG_CONTROL = 0xF4;
	const uint8_t BMP180_REG_RESULT = 0xF6;

	const uint8_t BMP180_COMMAND_TEMPERATURE = 0x2E;
	const uint8_t BMP180_COMMAND_PRESSURE0 = 0x34;
	const uint8_t BMP180_COMMAND_PRESSURE1 = 0x74;
	const uint8_t BMP180_COMMAND_PRESSURE2 = 0xB4;
	const uint8_t BMP180_COMMAND_PRESSURE3 = 0xF4;
//---------------------------------------------------------------------------
	enum TPressureBMP180Resolution { prLow, prStandard, prHigh, prHighest };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_ErrorOutputPin,
		typename T_FClocked,
		typename T_FState,
		typename T_InFahrenheit,
		typename T_PressureOutputPin,
		typename T_Resolution,
		typename T_TemperatureOutputPin
	> class BMP180_Pressure :
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_ErrorOutputPin,
		public T_FClocked,
		public T_FState,
		public T_InFahrenheit,
		public T_PressureOutputPin,
		public T_Resolution,
		public T_TemperatureOutputPin
	{
	public:
		_V_PIN_( PressureOutputPin )
		_V_PIN_( TemperatureOutputPin )
		_V_PIN_( ErrorOutputPin )

	protected:
		enum TState { stCoeff, stIdle, stTemperature, stPressure };

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Resolution )
		_V_PROP_( InFahrenheit )

	protected:
		_V_PROP_( FClocked )
		_V_PROP_( FState )

		uint32_t	FStartTime;
		uint32_t	FInterval;

		int16_t		AC1,AC2,AC3,VB1,VB2,MB,MC,MD;
		uint16_t	AC4,AC5,AC6;
		double c5,c6,mc,md,x0,x1,x2,y0,y1,y2,p0,p1,p2;
		float T;

	protected:
		bool WriteBytes( uint8_t *values, char length )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write(values,length);
			bool AError = ( C_I2C.endTransmission() != 0 );
			T_ErrorOutputPin::SetPinValue( AError, true );

			return ( ! AError );
		}

		bool ReadBytes( uint8_t *values, uint8_t length )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write(values[0]);
			bool AError = ( C_I2C.endTransmission() != 0 );
//			Serial.println( AError );
			T_ErrorOutputPin::SetPinValue( AError, true );
			if ( ! AError )
			{
//				Serial.println( "READ" );
				C_I2C.requestFrom( uint8_t( Address().GetValue() ), length );
				while(C_I2C.available() != length) ; // wait until bytes are ready

				while( length-- )
					*values++ = C_I2C.read();

/*
				values += length;
				while( length-- )
					*--values = C_I2C.read();
*/
			}

			return ( ! AError );
		}

		uint32_t ReadBytes( uint8_t length )
		{
/*
			union T24BitData
			{
				uint8_t		Bytes[ 4 ];
				uint32_t	Value;
			};

			T24BitData AData;
			AData.Value = 0;

			ReadBytes( AData.Bytes + 3 - length, length );

			return AData.Value;
*/
			uint8_t		ABytes[ 4 ];
			ABytes[0] = BMP180_REG_RESULT;

			ReadBytes( ABytes, length );

			uint32_t	AValue = 0;
			for( int i = 0; i < length; ++i )
			{
				AValue <<= 8;
				AValue |= ABytes[ i ];
			}

			return AValue;
		}

		void StartReading( uint8_t AType, TState AState, uint32_t AInterval )
		{
			uint8_t data[ 2 ] =
			{
				BMP180_REG_CONTROL
			};

			data[ 1 ] = AType;

//			Serial.println( "StartReading" );
			if( WriteBytes(data, 2))
			{
//				Serial.println( "STATE" );

				FState() = AState;
				FInterval = AInterval;
				FStartTime = millis();
			}
		}

		bool	ReadUnsignedInt(char address, uint16_t &value)
		{
			unsigned char data[2];

			data[0] = address;
			if (ReadBytes(data,2))
			{
				value = (((uint16_t)data[0]<<8)|(uint16_t)data[1]);
				return( true );
			}

			value = 0;
			return( false );
		}

		bool	ReadInt(char address, int16_t &value)
		{
			return ReadUnsignedInt( address, *(uint16_t *)&value );
		}

		void StartReadingTemp()
		{
			if( FState() == stCoeff )
			{
//				Serial.println( "TEST0" );
				if( !
					(
						ReadInt(0xAA,AC1) &&
						ReadInt(0xAC,AC2) &&
						ReadInt(0xAE,AC3) &&
						ReadUnsignedInt(0xB0,AC4) &&
						ReadUnsignedInt(0xB2,AC5) &&
						ReadUnsignedInt(0xB4,AC6) &&
						ReadInt(0xB6,VB1) &&
						ReadInt(0xB8,VB2) &&
						ReadInt(0xBA,MB) &&
						ReadInt(0xBC,MC) &&
						ReadInt(0xBE,MD)
					)
				)
					return;

				// All reads completed successfully!

				// If you need to check your math using known numbers,
				// you can uncomment one of these examples.
				// (The correct results are commented in the below functions.)

				// Example from Bosch datasheet
				// AC1 = 408; AC2 = -72; AC3 = -14383; AC4 = 32741; AC5 = 32757; AC6 = 23153;
				// B1 = 6190; B2 = 4; MB = -32768; MC = -8711; MD = 2868;

				// Example from http://wmrx00.sourceforge.net/Arduino/BMP180-Calcs.pdf
				// AC1 = 7911; AC2 = -934; AC3 = -14306; AC4 = 31567; AC5 = 25671; AC6 = 18974;
				// VB1 = 5498; VB2 = 46; MB = -32768; MC = -11075; MD = 2432;

				// Compute floating-point polynominals:

				double c3 = 160.0 * pow(2,-15) * AC3;
				double c4 = pow(10,-3) * pow(2,-15) * AC4;
				double b1 = pow(160,2) * pow(2,-30) * VB1;
				c5 = (pow(2,-15) / 160) * AC5;
				c6 = AC6;
				mc = (pow(2,11) / pow(160,2)) * MC;
				md = MD / 160.0;
				x0 = AC1;
				x1 = 160.0 * pow(2,-13) * AC2;
				x2 = pow(160,2) * pow(2,-25) * VB2;
				y0 = c4 * pow(2,15);
				y1 = c4 * c3;
				y2 = c4 * b1;
				p0 = (3791.0 - 8.0) / 1600.0;
				p1 = 1.0 - 7357.0 * pow(2,-20);
				p2 = 3038.0 * 100.0 * pow(2,-36);

//				Serial.println( "TEST1" );
				FState() = stIdle;
			}

			StartReading( BMP180_COMMAND_TEMPERATURE, stTemperature, 5 );
		}

		void StartReadingPressure()
		{
			switch( Resolution() )
			{
				case prLow:			StartReading( BMP180_COMMAND_PRESSURE0, stPressure, 5 ); break;
				case prStandard:	StartReading( BMP180_COMMAND_PRESSURE1, stPressure, 8 ); break;
				case prHigh:		StartReading( BMP180_COMMAND_PRESSURE2, stPressure, 14 ); break;
				case prHighest:		StartReading( BMP180_COMMAND_PRESSURE3, stPressure, 26 ); break;
			}
		}

		bool IsIdle() { return (( FState() == stCoeff ) || ( FState() == stIdle )); }

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( IsIdle() )
				StartReadingTemp();

			else
				FClocked() = true;
		}

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				StartReadingTemp();

		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
			{
				if( ! IsIdle() )
					if( FState() == stTemperature )
						ReadBytes( 2 );

					else
						ReadBytes( 3 );

				FState() = stIdle;
			}

			else
			{
				if( IsIdle() )
				{
					if( FClocked() || ( ! ClockInputPin_o_IsConnected() ))
						StartReadingTemp();
				}

				else
				{
					unsigned long ACurrentMillis = millis();
//					Serial.println( "TEST2" );
					if( ACurrentMillis - FStartTime >= FInterval )
						if( FState() == stTemperature )
						{
							if( T_TemperatureOutputPin::GetPinIsConnected() )
							{
								float tu = ReadBytes( 2 );
	//							Serial.print( "T1: " ); Serial.println( tu );
								//example from Bosch datasheet
								//tu = 27898;

								//example from http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf
								//tu = 0x69EC;

								float a = c5 * (tu - c6);
								T = a + (mc / (a + md));

								if( InFahrenheit() )
									T = T * ( 9.0/5.0 ) + 32.0;

								T_TemperatureOutputPin::SetPinValue( T );
								StartReadingPressure();
							}
						}

						else
						{
							if( T_PressureOutputPin::GetPinIsConnected() )
							{
								float pu = ReadBytes( 3 );
	//							Serial.print( "P1: " ); Serial.println( pu );
								pu /= 256.0;
	//							pu = (data[0] * 256.0) + data[1] + (data[2]/256.0);

								//example from Bosch datasheet
								//pu = 23843;

								//example from http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf, pu = 0x982FC0;
								//pu = (0x98 * 256.0) + 0x2F + (0xC0/256.0);

								float s = T - 25.0;
								float x = (x2 * pow(s,2)) + (x1 * s) + x0;
								float y = (y2 * pow(s,2)) + (y1 * s) + y0;
								float z = (pu - x) / y;
								float P = (p2 * pow(z,2)) + (p1 * z) + p0;

								T_PressureOutputPin::SetPinValue( P );

	//							Serial.println( "VVVVVV" );
								if( ClockInputPin_o_IsConnected() )
								{
	//								Serial.println( "PPPPP" );
									FState() = stIdle;
									if( FClocked() )
									{
	//									Serial.println( "TTTTTTT" );
										FClocked() = false;
										StartReadingTemp();
									}
								}

								else
									StartReadingTemp();

							}
						}

				}
			}

/*
			if( ! Enabled )
				if( FClocked || ( ! ClockInputPin_o_IsConnected() ))
					ReadCompass();
*/
		}

	public:
		inline BMP180_Pressure()
//			ClockInputPin_o_IsConnected( false ),
//			Enabled( true ),
//			FClocked( false ),
//			Resolution( prStandard ),
//			FState( stCoeff ),
//			InFahrenheit( false )
		{
			FClocked() = false;
			FState() = stCoeff;
		}
	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
