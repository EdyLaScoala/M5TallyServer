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
#include <Wire.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace SGP30Constants
	{
		const uint16_t COMMAND_INIT = 0x2003;
		const uint16_t COMMAND_MEASURE = 0x2008;
		const uint16_t COMMAND_MEASURE_RAW = 0x2050;
		const uint16_t COMMAND_SET_HUMIDITY = 0x2061;
		const uint16_t COMMAND_SELF_TEST = 0x2032;
		const uint16_t COMMAND_GET_BASELINE = 0x2015;
		const uint16_t COMMAND_SET_BASELINE = 0x201E;
	}
//---------------------------------------------------------------------------
	template <
		typename T_Ethanol,
		typename T_Hydrogen
	> class TArduinoAirQualitySensorSGP30Reference :
		public T_Ethanol,
		public T_Hydrogen
	{
	public:
		_V_PROP_( Ethanol )
		_V_PROP_( Hydrogen )

	};
//---------------------------------------------------------------------------
    template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CO2OutputPin,
		typename T_ClockInputPin_o_IsConnected,
		typename T_FClocked,
		typename T_TVOCOutputPin
	> class TArduinoAirQualitySensorSGP30Element_GetBaseline :
		public T_CO2OutputPin,
		public T_ClockInputPin_o_IsConnected,
		public T_FClocked,
		public T_TVOCOutputPin
	{
	public:
		_V_PIN_( CO2OutputPin )
		_V_PIN_( TVOCOutputPin )
		
	protected:
		_V_PROP_( FClocked )
		_V_PROP_( ClockInputPin_o_IsConnected )
	
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
		}

	public:
		inline void Start()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				SystemLoopBegin();
				
		}
		
		inline void GetInProcessing( bool & AValue )
		{
			if( FClocked() )
				AValue = true;

		}
		
	public:
		inline void SystemStart()
		{
			FClocked() = true;			
		}
		
		inline void SystemLoopBegin()
		{
			if( FClocked() )
				if( ! C_OWNER.IsBusy() )
				{
					FClocked() = false;
					C_OWNER._command( SGP30Constants::COMMAND_GET_BASELINE );
					C_OWNER.FInProcessing() = false;
					
					if (C_OWNER.Wire().requestFrom(uint8_t( C_OWNER.Address() ), uint8_t( 6 )) != 6)
					{
						FClocked() = false;
						return;
					}
					
					uint16_t CO2 =  C_OWNER.Wire().read() << 8;
					CO2  += C_OWNER.Wire().read();
					uint8_t crc = C_OWNER.Wire().read();
					if ( C_OWNER._CRC8( CO2 ) != crc)
					{
						FClocked() = false;
						return;
					}
					
					uint16_t TVOC =  C_OWNER.Wire().read() << 8;
					TVOC += C_OWNER.Wire().read();
					crc = C_OWNER.Wire().read();
					if ( C_OWNER._CRC8( TVOC ) != crc)
					{
						FClocked() = false;
						return;
					}
					
					T_CO2OutputPin::SetPinValue( CO2 );
					T_TVOCOutputPin::SetPinValue( TVOC );
				}
				
		}
		
	public:
		inline TArduinoAirQualitySensorSGP30Element_GetBaseline()
		{
			FClocked() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CO2,
		typename T_ClockInputPin_o_IsConnected,
		typename T_FClocked,
		typename T_TVOC
	> class TArduinoAirQualitySensorSGP30Element_SetBaseline :
		public T_CO2,
		public T_ClockInputPin_o_IsConnected,
		public T_FClocked,
		public T_TVOC
	{
	protected:
		_V_PROP_( CO2 )
		_V_PROP_( TVOC )

	protected:
		_V_PROP_( FClocked )
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
		}

	public:
		inline void Start()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				SystemLoopBegin();
				
		}
		
		inline void GetInProcessing( bool & AValue )
		{
			if( FClocked() )
				AValue = true;

		}

	public:
		inline void SystemStart()
		{
			FClocked() = true;			
		}
		
		inline void SystemLoopBegin()
		{
			if( FClocked() )
				if( ! C_OWNER.IsBusy() )
				{
					FClocked() = false;
					C_OWNER._command( SGP30Constants::COMMAND_SET_BASELINE, CO2().GetValue() + 0.5, TVOC().GetValue() + 0.5 );
					C_OWNER.FInProcessing() = false;
				}
				
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_FClocked,
		typename T_FRunning,
		typename T_FailedOutputPin,
		typename T_SuccessOutputPin,
		typename T_TestingOutputPin	
	> class TArduinoAirQualitySensorSGP30Element_SelfTest :
		public T_ClockInputPin_o_IsConnected,
		public T_FClocked,
		public T_FRunning,
		public T_FailedOutputPin,
		public T_SuccessOutputPin,
		public T_TestingOutputPin	
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SuccessOutputPin )
		_V_PIN_( TestingOutputPin )
		
	protected:
		_V_PROP_( FRunning )
		_V_PROP_( FClocked )
		_V_PROP_( ClockInputPin_o_IsConnected )
		
	protected:
		void StartTest()
		{
			T_TestingOutputPin::SetPinValueHigh();
			FClocked() = false;
			FRunning() = true;
			C_OWNER._command( SGP30Constants::COMMAND_SELF_TEST );
			C_OWNER.FLastTime = millis();
		}
		
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
		}
		
	public:
		inline void Start()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				StartTest();
				
		}
		
		inline void GetInProcessing( bool & AValue )
		{
			if( FRunning() || FClocked() )
				AValue = true;

		}
		
	public:
		inline void SystemLoopBegin()
		{
			if( FRunning() )
			{
				if( millis() - C_OWNER.FLastTime > 220 )
				{
					C_OWNER.Wire().requestFrom( uint8_t( C_OWNER.Address()), uint8_t( 3 ));
					
					uint16_t AResult = C_OWNER.Wire().read() << 8;
					AResult  += C_OWNER.Wire().read();
					uint8_t crc = C_OWNER.Wire().read();
					if( C_OWNER._CRC8( AResult ) != crc )
						T_FailedOutputPin::ClockPin();
					
					else
					{
						if( AResult == 0xD400 )
							T_SuccessOutputPin::ClockPin();
						
						else
							T_FailedOutputPin::ClockPin();
						
					}
					
					T_TestingOutputPin::SetPinValueLow();
					C_OWNER.FInProcessing() = false;
					FRunning() = false;
				}
			}
			
			if( FClocked() )
				if( ! C_OWNER.IsBusy() )
					StartTest();
				
		}
		
	public:
		inline TArduinoAirQualitySensorSGP30Element_SelfTest()
		{
			FRunning() = false;
			FClocked() = true;
		}		
	
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_AbsoluteHumidity,
		typename T_Address,
		typename T_CO2OutputPin,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Elements_GetInProcessing,
		typename T_Elements_Start,
		typename T_Enabled,
		typename T_EthanolOutputPin,
		typename T_FAllStarted,
		typename T_FClocked,
		typename T_FInProcessing,
		typename T_FInRawMeasure,
		typename T_FInitialized,
//		typename T_FStarted,
		typename T_HydrogenOutputPin,
//		typename T_RawEthanolOutputPin,
//		typename T_RawHydrogenOutputPin,
		typename T_Reference,
		typename T_TVOCOutputPin
	> class TArduinoAirQualitySensorSGP30 :
		public T_AbsoluteHumidity,
		public T_Address,
		public T_CO2OutputPin,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_EthanolOutputPin,
		public T_FAllStarted,
		public T_FClocked,
		public T_FInProcessing,
		public T_FInRawMeasure,
		public T_FInitialized,
//		public T_FStarted,
		public T_HydrogenOutputPin,
//		public T_RawEthanolOutputPin,
//		public T_RawHydrogenOutputPin,
		public T_Reference,
		public T_TVOCOutputPin
	{
	public:
		_V_PIN_( CO2OutputPin ) 
		_V_PIN_( EthanolOutputPin ) 
		_V_PIN_( HydrogenOutputPin ) 
		_V_PIN_( TVOCOutputPin ) 
//		_V_PIN_( RawEthanolOutputPin )
//		_V_PIN_( RawHydrogenOutputPin )
		
	public:
		_V_PROP_( AbsoluteHumidity )
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Reference )
		
	public:
		_V_PROP_( FInProcessing )
		
	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )
		_V_PROP_( FInRawMeasure )
		_V_PROP_( FClocked )
		_V_PROP_( FAllStarted )
		_V_PROP_( FInitialized )
	
	public:
		void Update_Humidity()
		{
			uint16_t AValue = AbsoluteHumidity().GetValue();
			uint8_t tmp = (( AbsoluteHumidity().GetValue() - AValue ) * 256 + 0.5 );
			AValue = (AValue << 8) | tmp;
			
			_command( SGP30Constants::COMMAND_SET_HUMIDITY, AValue );
			FInProcessing() = false;
		}
		
	public:
		unsigned long	FLastTime;
		
	public:
		inline bool IsBusy()
		{
			return FInProcessing().GetValue();
		}
		
		inline T_I2C &Wire()
		{
			return C_I2C;
		}
		
	public:
		void _command( uint16_t cmd )
		{
			C_I2C.beginTransmission( uint8_t( Address()) );
			C_I2C.write(cmd >> 8);
			C_I2C.write(cmd & 0xFF);
			C_I2C.endTransmission();
			FInProcessing() = true;
		}
		
		void _command(uint16_t cmd, uint16_t v1)
		{
			C_I2C.beginTransmission( uint8_t( Address()) );
			C_I2C.write(cmd >> 8);
			C_I2C.write(cmd & 0xFF);
			C_I2C.write(v1 >> 8);
			C_I2C.write(v1 & 0xFF);
			C_I2C.write(_CRC8(v1));
			C_I2C.endTransmission();
			FInProcessing() = true;
		}

		void _command(uint16_t cmd, uint16_t v1, uint16_t v2)
		{
			C_I2C.beginTransmission( uint8_t( Address()) );
			C_I2C.write(cmd >> 8);
			C_I2C.write(cmd & 0xFF);
			C_I2C.write(v1 >> 8);
			C_I2C.write(v1 & 0xFF);
			C_I2C.write(_CRC8(v1));
			C_I2C.write(v2 >> 8);
			C_I2C.write(v2 & 0xFF);
			C_I2C.write(_CRC8(v2));
			C_I2C.endTransmission();
			FInProcessing() = true;
		}

		// for sending command - CRC lib.
		// always 2 bytes
		uint8_t _CRC8(uint16_t data)
		{
			uint8_t val[2];
			val[0] = data >> 8;
			val[1] = data & 0xFF;

			uint8_t crc = 0xFF;             // start value
			for(uint8_t i = 0; i < 2; i++) 
			{
				crc ^= val[i];
				for (uint8_t b = 8; b > 0; b--)
				{
					if (crc & 0x80)
						crc = (crc << 1) ^ 0x31;  // polynomial
					
					else
						crc <<= 1;
				}
			}
			
			return crc;
		};		
		
		inline uint16_t CalculateInverse( float AValue )
		{
			return log( 2 * AValue ) / 1.953125e-3;
		}

	protected:
		void OutputResults()
		{
			if( ! FAllStarted() )
				return;
			
			if (C_I2C.requestFrom( uint8_t( Address()), uint8_t( 6 )) != 6 )
				return;
				
			FInProcessing() = false;
			if( FInRawMeasure() )
			{
				uint16_t Ah2      =  C_I2C.read() << 8;
				Ah2      += C_I2C.read();
				uint8_t crc = C_I2C.read();
				if( _CRC8( Ah2 ) != crc )
					return;
				
				uint16_t Aethanol =  C_I2C.read() << 8;
				Aethanol += C_I2C.read();
				crc = C_I2C.read();
				if( _CRC8( Aethanol ) != crc )
					return;
				
//				T_RawHydrogenOutputPin::SetPinValue( Ah2 );
//				T_RawEthanolOutputPin::SetPinValue( Aethanol );
				
//				Y = 0.5 * exp(( X ) * 1.953125e-3 );
// X = ? Y

				float AResult = 0.5 * exp(( CalculateInverse( Reference().Hydrogen().GetValue() ) - Ah2 ) * 1.953125e-3 );
				T_HydrogenOutputPin::SetPinValue( AResult );
				
				AResult = 0.4 * exp(( CalculateInverse( Reference().Ethanol().GetValue() ) - Aethanol ) * 1.953125e-3 );
				T_EthanolOutputPin::SetPinValue( AResult );
			}
			
			else
			{
//				Serial.println( "READ" );
				uint16_t Aco2  =  C_I2C.read() << 8;
				Aco2  += C_I2C.read();
				uint8_t crc = C_I2C.read();
				if( _CRC8( Aco2 ) != crc )
					return;

				uint16_t Atvoc =  C_I2C.read() << 8;
				Atvoc += C_I2C.read();
				crc = C_I2C.read();
				if ( _CRC8( Atvoc ) != crc )			
					return;
				
				T_CO2OutputPin::SetPinValue( Aco2 );
				T_TVOCOutputPin::SetPinValue( Atvoc );
			}			
		}
		
		inline bool NeedsRead()
		{
			return T_CO2OutputPin::GetPinIsConnected() || T_TVOCOutputPin::GetPinIsConnected();
		}
		
		inline bool NeedsRawRead()
		{
//			return T_HydrogenOutputPin::GetPinIsConnected() || T_EthanolOutputPin::GetPinIsConnected() || T_RawHydrogenOutputPin::GetPinIsConnected() || T_RawEthanolOutputPin::GetPinIsConnected();
			return T_HydrogenOutputPin::GetPinIsConnected() || T_EthanolOutputPin::GetPinIsConnected();
		}
		
		void TryRequest()
		{
			if( NeedsRead() )
			{
//				Serial.println( "-------" );
//				Serial.println( NeedsRawRead() );
//				Serial.println( FInRawMeasure().GetValue() );
				if( FInRawMeasure() || ( ! NeedsRawRead() ))
				{
//					Serial.println( "START READ 1" );
					if( millis() - FLastTime < (( FInRawMeasure() ) ? 25 : 12 ))
						return;
					
//					Serial.println( "START READ" );
					OutputResults();
					FInRawMeasure() = false;

					bool AIsElementInProcessing = false;
					T_Elements_GetInProcessing::Call( AIsElementInProcessing );
					if( AIsElementInProcessing )
						FAllStarted() = false;

					else
					{
						_command( SGP30Constants::COMMAND_MEASURE );
						FAllStarted() = true;
						FLastTime = millis();
						if( ! NeedsRawRead() )
							FClocked() = false;
					
					}

					return;
				}
				
				
//				if( FRawMeasure
			}
			
			if( NeedsRawRead() )
			{
				if( ( ! FInRawMeasure().GetValue() ) || ( ! NeedsRead() ))
				{
					if( millis() - FLastTime < (( FInRawMeasure() ) ? 25 : 12 ))
						return;
					
//					Serial.println( "START READ RAW" );
					OutputResults();
					FInRawMeasure() = true;
					bool AIsElementInProcessing = false;
					T_Elements_GetInProcessing::Call( AIsElementInProcessing );
					if( AIsElementInProcessing )
						FAllStarted() = false;

					else
					{
						_command( SGP30Constants::COMMAND_MEASURE_RAW );
						FAllStarted() = true;
						FLastTime = millis();
						FClocked() = false;
					}

					return;
				}
			}
		}
		
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
			TryRequest();
		}
		
	public:
		inline void SystemStart()
		{
			_command( SGP30Constants::COMMAND_INIT );
			FInProcessing() = false;

			T_Elements_Start::Call();
//			_command( SGP30Constants::
		}
		
		inline void SystemLoopBegin()
		{
			if( FInitialized() )
			{
				if( FClocked() )
					TryRequest();
			}
				
			else
			{
				FInitialized() = true;
				Update_Humidity();
//					Serial.println( "START" );
				FLastTime = millis() - 26;
				TryRequest();
			}
		}
		
	public:
		inline TArduinoAirQualitySensorSGP30()
		{
			FClocked() = false;
			FAllStarted() = false;
			FInRawMeasure() = false;
			FInitialized() = false;
			FInProcessing() = false;
//				FStarted() = false;
		}
		
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
