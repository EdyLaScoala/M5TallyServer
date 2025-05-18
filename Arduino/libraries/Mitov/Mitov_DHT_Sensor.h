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
	namespace DHTSensorGlobals
	{
		uint32_t Maxcycles;
	};

	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FLastResult,
		typename T_HumidityOutputPin,
		typename T_InFahrenheit,
		int C_SensorOutputPin,
		typename T_TemperatureOutputPin
	> class DHTSensor :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FLastResult,
		public T_HumidityOutputPin,
		public T_InFahrenheit,
		public T_TemperatureOutputPin
	{
	public:
		_V_PIN_( TemperatureOutputPin )
		_V_PIN_( HumidityOutputPin )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		_V_PROP_( FLastResult )

		uint8_t data[5];
		unsigned long FLastReadTime;

//#ifdef __AVR
		// Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
		// for the digital pin connected to the DHT.  Other platforms will use digitalRead.
//		uint8_t _bit, _port;
//#endif

//		DHT	*FSensor;

	public:
		inline void SystemInit()
		{
			FLastReadTime = millis() - 2000;
			pinMode( C_SensorOutputPin, INPUT_PULLUP );
//			FSensor = new DHT( C_SensorOutputPin, SENSOR_TYPE );
//			FSensor->begin();
		}

		inline void SystemStart() {} // Placeholder

		inline void SystemLoopBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				ReadSensor();

		}

	protected:
		void ReadSensor()
		{
			if( ! TryRead())
				return;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
			{
				float   AValue = data[2];
				if( InFahrenheit() )
					AValue = Func::ConvertCtoF( AValue );

				T_TemperatureOutputPin::SetPinValue( AValue );
			}

			T_HumidityOutputPin::SetPinValue( data[0] );
		}

		uint32_t expectPulse(bool level)
		{
			uint32_t count = 0;
			// On AVR platforms use direct GPIO port access as it's much faster and better
			// for catching pulses that are 10's of microseconds in length:
#ifdef __AVR
			uint8_t _bit = digitalPinToBitMask( C_SensorOutputPin );
			uint8_t portState = level ? _bit : 0;
			uint8_t _port = digitalPinToPort( C_SensorOutputPin );
			while ((*portInputRegister(_port) & _bit) == portState)
			// Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266
			// right now, perhaps bugs in direct port access functions?).
#else
			while (digitalRead(C_SensorOutputPin) == level)
#endif
			{
				if (count++ >= DHTSensorGlobals::Maxcycles )
					return 0; // Exceeded timeout, fail.
			}

			return count;
		}

		bool TryRead()
		{
			if( ! Enabled().GetValue() )
				return false;

			uint32_t currenttime = millis();
			if ( (currenttime - FLastReadTime) < 2000 )
				return FLastResult(); // return last correct measurement

			FLastReadTime = currenttime;

			// Reset 40 bits of received data to zero.
			data[0] = data[1] = data[2] = data[3] = data[4] = 0;

			// Send start signal.  See DHT datasheet for full signal diagram:
			//   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

			// Go into high impedence state to let pull-up raise data line level and
			// start the reading process.
			digitalWrite( C_SensorOutputPin, HIGH);
			delay(250);

			// First set data line low for 20 milliseconds.
			pinMode( C_SensorOutputPin, OUTPUT);
			digitalWrite( C_SensorOutputPin, LOW);
			delay(20);

			uint32_t cycles[80];
			{
				// Turn off interrupts temporarily because the next sections are timing critical
				// and we don't want any interruptions.
				InterruptLock lock;

				// End the start signal by setting data line high for 40 microseconds.
				digitalWrite( C_SensorOutputPin, HIGH);
				delayMicroseconds(40);

				// Now start reading the data line to get the value from the DHT sensor.
				pinMode( C_SensorOutputPin, INPUT_PULLUP );
				delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.

				// First expect a low signal for ~80 microseconds followed by a high signal
				// for ~80 microseconds again.
				if (expectPulse(LOW) == 0)
				{
//					DEBUG_PRINTLN(F("Timeout waiting for start signal low pulse."));
					FLastResult() = false;
					return FLastResult();
				}
				if (expectPulse(HIGH) == 0)
				{
//					DEBUG_PRINTLN(F("Timeout waiting for start signal high pulse."));
					FLastResult() = false;
					return FLastResult();
				}

				// Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
				// microsecond low pulse followed by a variable length high pulse.  If the
				// high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
				// then it's a 1.  We measure the cycle count of the initial 50us low pulse
				// and use that to compare to the cycle count of the high pulse to determine
				// if the bit is a 0 (high state cycle count < low state cycle count), or a
				// 1 (high state cycle count > low state cycle count). Note that for speed all
				// the pulses are read into a array and then examined in a later step.
				for (int i=0; i<80; i+=2)
				{
					cycles[i]   = expectPulse(LOW);
					cycles[i+1] = expectPulse(HIGH);
				}

			} // Timing critical code is now complete.

			// Inspect pulses and determine which ones are 0 (high state cycle count < low
			// state cycle count), or 1 (high state cycle count > low state cycle count).
			for (int i=0; i<40; ++i)
			{
				uint32_t lowCycles  = cycles[2*i];
				uint32_t highCycles = cycles[2*i+1];
				if ((lowCycles == 0) || (highCycles == 0))
				{
//					DEBUG_PRINTLN(F("Timeout waiting for pulse."));
					FLastResult() = false;
					return FLastResult();
				}

				data[i/8] <<= 1;
				// Now compare the low and high cycle times to see if the bit is a 0 or 1.
				if (highCycles > lowCycles)
					// High cycles are greater than 50us low cycle count, must be a 1.
					data[i/8] |= 1;

				// Else high cycles are less than (or equal to, a weird case) the 50us low
				// cycle count so this must be a zero.  Nothing needs to be changed in the
				// stored data.
			}

/*
  DEBUG_PRINTLN(F("Received:"));
  DEBUG_PRINT(data[0], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[1], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[2], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[3], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[4], HEX); DEBUG_PRINT(F(" =? "));
  DEBUG_PRINTLN((data[0] + data[1] + data[2] + data[3]) & 0xFF, HEX);
*/
			// Check we read 40 bits and that the checksum matches.
			if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
			{
				FLastResult() = true;
				return FLastResult();
			}

			else
			{
	//			DEBUG_PRINTLN(F("Checksum failure!"));
				FLastResult() = false;
				return FLastResult();
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		DHTSensor()
		{
			 FLastResult() = false;
//  #ifdef __AVR
//			_bit = digitalPinToBitMask( C_SensorOutputPin );
//			_port = digitalPinToPort( C_SensorOutputPin );
//  #endif
			DHTSensorGlobals::Maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FLastResult,
		typename T_HumidityOutputPin,
		typename T_InFahrenheit,
		int C_SensorOutputPin,
		typename T_TemperatureOutputPin
	> class DHTSensor_DHT22 :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FLastResult,
		public T_HumidityOutputPin,
		public T_InFahrenheit,
		public T_TemperatureOutputPin
	{
	public:
		_V_PIN_( TemperatureOutputPin )
		_V_PIN_( HumidityOutputPin )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		_V_PROP_( FLastResult )

		uint8_t data[5];
		unsigned long FLastReadTime;

//#ifdef __AVR
		// Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
		// for the digital pin connected to the DHT.  Other platforms will use digitalRead.
//		uint8_t _bit, _port;
//#endif

//		DHT	*FSensor;

	public:
		inline void SystemInit()
		{
			FLastReadTime = millis() - 2000;
			pinMode( C_SensorOutputPin, INPUT_PULLUP );
//			FSensor = new DHT( C_SensorOutputPin, SENSOR_TYPE );
//			FSensor->begin();
		}

		inline void SystemStart() {} // Placeholder

		inline void SystemLoopBegin()
		{
			if( ! ClockInputPin_o_IsConnected() )
				ReadSensor();

		}

	protected:
		void ReadSensor()
		{
			if( ! TryRead())
				return;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
			{
				float   AValue = data[2] & 0x7F;
				AValue *= 256;
				AValue += data[3];
				AValue *= 0.1;
				if( data[2] & 0x80 )
					AValue *= -1;

				if( InFahrenheit() )
					AValue = Func::ConvertCtoF( AValue );

				T_TemperatureOutputPin::SetPinValue( AValue );
			}

			if( T_HumidityOutputPin::GetPinIsConnected() )
			{
				float   AValue = data[0];
				AValue *= 256;
				AValue += data[1];
				AValue *= 0.1;
				T_HumidityOutputPin::SetPinValue( AValue );
			}
		}

		uint32_t expectPulse(bool level)
		{
			uint32_t count = 0;
			// On AVR platforms use direct GPIO port access as it's much faster and better
			// for catching pulses that are 10's of microseconds in length:
#ifdef __AVR
			uint8_t _bit = digitalPinToBitMask( C_SensorOutputPin );
			uint8_t portState = level ? _bit : 0;
			uint8_t _port = digitalPinToPort( C_SensorOutputPin );
			while ((*portInputRegister(_port) & _bit) == portState)
			// Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266
			// right now, perhaps bugs in direct port access functions?).
#else
			while (digitalRead(C_SensorOutputPin) == level)
#endif
			{
				if (count++ >= DHTSensorGlobals::Maxcycles )
					return 0; // Exceeded timeout, fail.
			}

			return count;
		}

		bool TryRead()
		{
			if( ! Enabled().GetValue() )
				return false;

			uint32_t currenttime = millis();
			if ( (currenttime - FLastReadTime) < 2000 )
				return FLastResult(); // return last correct measurement

			FLastReadTime = currenttime;

			// Reset 40 bits of received data to zero.
			data[0] = data[1] = data[2] = data[3] = data[4] = 0;

			// Send start signal.  See DHT datasheet for full signal diagram:
			//   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

			// Go into high impedence state to let pull-up raise data line level and
			// start the reading process.
			digitalWrite( C_SensorOutputPin, HIGH);
			delay(250);

			// First set data line low for 20 milliseconds.
			pinMode( C_SensorOutputPin, OUTPUT);
			digitalWrite( C_SensorOutputPin, LOW);
			delay(20);

			uint32_t cycles[80];
			{
				// Turn off interrupts temporarily because the next sections are timing critical
				// and we don't want any interruptions.
				InterruptLock lock;

				// End the start signal by setting data line high for 40 microseconds.
				digitalWrite( C_SensorOutputPin, HIGH);
				delayMicroseconds(40);

				// Now start reading the data line to get the value from the DHT sensor.
				pinMode( C_SensorOutputPin, INPUT_PULLUP );
				delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.

				// First expect a low signal for ~80 microseconds followed by a high signal
				// for ~80 microseconds again.
				if (expectPulse(LOW) == 0)
				{
//					DEBUG_PRINTLN(F("Timeout waiting for start signal low pulse."));
					FLastResult() = false;
					return FLastResult();
				}
				if (expectPulse(HIGH) == 0)
				{
//					DEBUG_PRINTLN(F("Timeout waiting for start signal high pulse."));
					FLastResult() = false;
					return FLastResult();
				}

				// Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
				// microsecond low pulse followed by a variable length high pulse.  If the
				// high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
				// then it's a 1.  We measure the cycle count of the initial 50us low pulse
				// and use that to compare to the cycle count of the high pulse to determine
				// if the bit is a 0 (high state cycle count < low state cycle count), or a
				// 1 (high state cycle count > low state cycle count). Note that for speed all
				// the pulses are read into a array and then examined in a later step.
				for (int i=0; i<80; i+=2)
				{
					cycles[i]   = expectPulse(LOW);
					cycles[i+1] = expectPulse(HIGH);
				}

			} // Timing critical code is now complete.

			// Inspect pulses and determine which ones are 0 (high state cycle count < low
			// state cycle count), or 1 (high state cycle count > low state cycle count).
			for (int i=0; i<40; ++i)
			{
				uint32_t lowCycles  = cycles[2*i];
				uint32_t highCycles = cycles[2*i+1];
				if ((lowCycles == 0) || (highCycles == 0))
				{
//					DEBUG_PRINTLN(F("Timeout waiting for pulse."));
					FLastResult() = false;
					return FLastResult();
				}

				data[i/8] <<= 1;
				// Now compare the low and high cycle times to see if the bit is a 0 or 1.
				if (highCycles > lowCycles)
					// High cycles are greater than 50us low cycle count, must be a 1.
					data[i/8] |= 1;

				// Else high cycles are less than (or equal to, a weird case) the 50us low
				// cycle count so this must be a zero.  Nothing needs to be changed in the
				// stored data.
			}

/*
  DEBUG_PRINTLN(F("Received:"));
  DEBUG_PRINT(data[0], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[1], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[2], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[3], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[4], HEX); DEBUG_PRINT(F(" =? "));
  DEBUG_PRINTLN((data[0] + data[1] + data[2] + data[3]) & 0xFF, HEX);
*/
			// Check we read 40 bits and that the checksum matches.
			if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
			{
				FLastResult() = true;
				return FLastResult();
			}

			else
			{
	//			DEBUG_PRINTLN(F("Checksum failure!"));
				FLastResult() = false;
				return FLastResult();
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		DHTSensor_DHT22()
		{
			 FLastResult() = false;
//  #ifdef __AVR
//			_bit = digitalPinToBitMask( C_SensorOutputPin );
//			_port = digitalPinToPort( C_SensorOutputPin );
//  #endif
			DHTSensorGlobals::Maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
		}
	};
//---------------------------------------------------------------------------
    template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FBitIndex,
		typename T_FClocked,
		typename T_FIndex,
		typename T_FState,
		typename T_FStatus,
		typename T_HumidityOutputPin,
		void (*INTERRUPT_HANDLER)(),
		typename T_InFahrenheit,
		int C_SensorOutputPin,
		typename T_TemperatureOutputPin
	> class DHTSensor_Interrupt :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FBitIndex,
		public T_FClocked,
		public T_FIndex,
		public T_FState,
		public T_FStatus,
		public T_HumidityOutputPin,
		public T_InFahrenheit,
		public T_TemperatureOutputPin
	{
	public:
		_V_PIN_( TemperatureOutputPin )
		_V_PIN_( HumidityOutputPin )

	protected:
		// error codes
		static const uint8_t IDDHTLIB_ERROR_CHECKSUM	= 1;
		static const uint8_t IDDHTLIB_ERROR_TIMEOUT		= 2;
		static const uint8_t IDDHTLIB_ERROR_ACQUIRING	= 3;
		static const uint8_t IDDHTLIB_ERROR_DELTA		= 4;
		static const uint8_t IDDHTLIB_ERROR_NOTSTARTED	= 5;

	protected:
		enum TStates { RESPONSE, DATA, ACQUIRED, STOPPED, ACQUIRING, RAW_DATA_READY, START_SIGNAL };

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		_V_PROP_( FClocked )
		_V_PROP_( FState )
		_V_PROP_( FStatus )
	    _V_PROP_( FIndex )
	    _V_PROP_( FBitIndex )

		volatile uint8_t data[5];
		unsigned long FLastReadTime;
		volatile unsigned long us;
//		int FCount = 0;
//		int32_t cycles[80];
//		volatile uint8_t bits[5];
//		uint32_t FInterruptCount = 0;

//#ifdef __AVR
		// Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
		// for the digital pin connected to the DHT.  Other platforms will use digitalRead.
//		uint8_t _bit, _port;
//#endif

//		DHT	*FSensor;

	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool AValue )
		{
//			FInterruptCount ++;
//			Serial.println( "TEST" );
			unsigned long newUs = micros();
			unsigned long delta = newUs - us;
			if ( delta > 255 )
			{
//				Serial.print( "TIMEOUT: " );
//				Serial.println( delta );

				FStatus() = IDDHTLIB_ERROR_TIMEOUT;
				FState() = STOPPED;
				detachInterrupt( digitalPinToInterrupt( C_SensorOutputPin ));
				return;
			}

/*
			if( AValue )
				cycles[ FCount++ ] = delta;

			else
				cycles[ FCount++ ] = -delta;
*/
			us = newUs;

			switch( FState() )
			{
				case RESPONSE:
//					if( AValue )
//						return;

//					Serial.print( "RESPONSE: " );
//					Serial.println( delta );
					if (delta < 25)
					{
						us -= delta;
						break; //do nothing, it started the response signal
					}

					if (125 < delta && delta < 190)
					{
//						Serial.println( "DATA 2" );
						FState() = DATA;
					}

					else
					{
//						Serial.print( "REPONSE: " );
//						Serial.println( delta );
//						Serial.println( AValue );

						detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
						FStatus() = IDDHTLIB_ERROR_TIMEOUT;
						FState() = STOPPED;
					}
					break;

				case DATA:
//					Serial.println( "DATA 1" );
//					Serial.println( delta );
					if (60 < delta && delta < 145)
					{ //valid in timing
						data[ FIndex() ] <<= 1; //shift the data
						if (delta > 100) //is a one
							data[ FIndex() ] |= 1;

						if (FBitIndex() == 0)
						{  // when we have fulfilled the byte, go to the next
							FBitIndex() = 7;    // restart at MSB
							FIndex() = FIndex() + 1;
							if ( FIndex() == 5 )
							{ // go to next byte; when we have got 5 bytes, stop.
								detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
/*
								Serial.println( "DATA" );
								for( int i = 0; i < 5; ++i )
								{
									Serial.print( data[i] ); Serial.print( " " );
								}
								Serial.println( "" );

								for( int i = 0; i < FCount; ++i )
								{
									Serial.print( cycles[i] ); Serial.print( " " );
								}
								Serial.println( "" );
*/
								FState() = RAW_DATA_READY;
								break;
							}
						}

						else
							FBitIndex() = FBitIndex() - 1;

					}

					else if (delta < 10)
					{
//						Serial.println( "IDDHTLIB_ERROR_DELTA" );
//						Serial.println( delta );
						detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
						FStatus() = IDDHTLIB_ERROR_DELTA;
						FState() = STOPPED;
					}

					else
					{
//						Serial.println( "IDDHTLIB_ERROR_TIMEOUT" );
//						Serial.println( delta );
						detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
						FStatus() = IDDHTLIB_ERROR_TIMEOUT;
						FState() = STOPPED;
					}
					break;

				default:
					break;
			}

		}

	public:
		inline void SystemInit() {} // Placeholder

		inline void SystemStart()
		{
			FLastReadTime = millis() - 2000;
			detachInterrupt( digitalPinToInterrupt( C_SensorOutputPin ) );
			pinMode( C_SensorOutputPin, INPUT_PULLUP );
//			FSensor = new DHT( C_SensorOutputPin, SENSOR_TYPE );
//			FSensor->begin();
		}

		inline void SystemLoopBegin()
		{
//			Serial.println( FInterruptCount );
			ReadResult();

			if ( ! ( FState() == STOPPED || FState() == ACQUIRED ))
			{
				unsigned long delta = millis() - FLastReadTime;
				if( delta > 1000 )
				{
//					Serial.print( "TIMEOUT: " );
//					Serial.println( delta );

					FStatus() = IDDHTLIB_ERROR_TIMEOUT;
					FState() = STOPPED;
					detachInterrupt( digitalPinToInterrupt( C_SensorOutputPin ));
					digitalWrite( C_SensorOutputPin, HIGH );
					FLastReadTime = millis();
					return;
				}
			}

			if( FClocked() )
			{
				ReadSensor();
				return;
			}

			if( ! ClockInputPin_o_IsConnected() )
				ReadSensor();

		}

	protected:
		void ReadSensor()
		{
			uint32_t currenttime = millis();
			if ( (currenttime - FLastReadTime) < 2000 )
				return;

			FLastReadTime = currenttime;

			FClocked() = ( ! TryRead());
		}

		void ReadResult()
		{
			if( FState() != RAW_DATA_READY )
				return;

//			Serial.println( "ReadResult" );

			if( ! Enabled().GetValue() )
				return;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
			{
				float   AValue = data[2];
				if( InFahrenheit() )
					AValue = Func::ConvertCtoF( AValue );

				T_TemperatureOutputPin::SetPinValue( AValue );
			}

			T_HumidityOutputPin::SetPinValue( data[0] );

			FState() = STOPPED;
		}

		bool TryRead()
		{
//			Serial.println( "TryRead" );
			if ( FState() == STOPPED || FState() == ACQUIRED )
			{
				if( ! Enabled().GetValue() )
					return true;

				detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
				//set the state machine for interruptions analisis of the signal
//				FState = useDelay ? RESPONSE : START_SIGNAL;
				FState() = RESPONSE;

				// EMPTY BUFFER and vars
				memset( (void *)data, 0, sizeof( data ));
//				for (uint8_t i = 0; i < 5; i++)
//					bits[i] = 0;

				FBitIndex() = 7;
				FIndex() = 0;
				FStatus() = 0;
//				FCount = 0;
//				hum = 0; // This will be the Humidity result!
//				temp = 0; // This will be the Temperature result!

				// REQUEST SAMPLE
				pinMode( C_SensorOutputPin, OUTPUT);
/*
				// Go into high impedence state to let pull-up raise data line level and
				// start the reading process.
				digitalWrite( C_SensorOutputPin, HIGH);
				delay(250);
*/
				digitalWrite( C_SensorOutputPin, LOW);

//				if (useDelay)
				{
					delay(18);
					digitalWrite( C_SensorOutputPin, HIGH);
					delayMicroseconds(25);
					pinMode( C_SensorOutputPin, INPUT_PULLUP );
				}

				us = micros();
				// Analize the data in an interrupt
//				if (useDelay)
//					attachInterrupt(intNumber, pCallbackArray[intNumber], FALLING);
					attachInterrupt( digitalPinToInterrupt( C_SensorOutputPin ), INTERRUPT_HANDLER, FALLING );
//					INTERRUPT_HANDLER.Reattach();

//				Serial.println( "START" );
				return true;
			}

			return false;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		DHTSensor_Interrupt()
		{
			FClocked() = false;
			FState() = STOPPED;
			FStatus() = IDDHTLIB_ERROR_NOTSTARTED;
//  #ifdef __AVR
//			_bit = digitalPinToBitMask( C_SensorOutputPin );
//			_port = digitalPinToPort( C_SensorOutputPin );
//  #endif
//			DHTSensorGlobals::Maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
		}
	};
//---------------------------------------------------------------------------
    template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FBitIndex,
		typename T_FClocked,
		typename T_FIndex,
		typename T_FState,
		typename T_FStatus,
		typename T_HumidityOutputPin,
		void (*INTERRUPT_HANDLER)(),
		typename T_InFahrenheit,
		int C_SensorOutputPin,
		typename T_TemperatureOutputPin
	> class DHTSensor_Interrupt_DHT22 :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FBitIndex,
		public T_FClocked,
		public T_FIndex,
		public T_FState,
		public T_FStatus,
		public T_HumidityOutputPin,
		public T_InFahrenheit,
		public T_TemperatureOutputPin
	{
	public:
		_V_PIN_( TemperatureOutputPin )
		_V_PIN_( HumidityOutputPin )

	protected:
		// error codes
		static const uint8_t IDDHTLIB_ERROR_CHECKSUM	= 1;
		static const uint8_t IDDHTLIB_ERROR_TIMEOUT		= 2;
		static const uint8_t IDDHTLIB_ERROR_ACQUIRING	= 3;
		static const uint8_t IDDHTLIB_ERROR_DELTA		= 4;
		static const uint8_t IDDHTLIB_ERROR_NOTSTARTED	= 5;

	protected:
		enum TStates { RESPONSE, DATA, ACQUIRED, STOPPED, ACQUIRING, RAW_DATA_READY, START_SIGNAL };

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		_V_PROP_( FClocked )
		_V_PROP_( FState )
		_V_PROP_( FStatus )
	    _V_PROP_( FIndex )
	    _V_PROP_( FBitIndex )

		volatile uint8_t data[5];
		unsigned long FLastReadTime;
		volatile unsigned long us;
//		int FCount = 0;
//		int32_t cycles[80];
//		volatile uint8_t bits[5];
//		uint32_t FInterruptCount = 0;

//#ifdef __AVR
		// Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
		// for the digital pin connected to the DHT.  Other platforms will use digitalRead.
//		uint8_t _bit, _port;
//#endif

//		DHT	*FSensor;

	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool AValue )
		{
//			Serial.print( "^" );
//			FInterruptCount ++;
			unsigned long newUs = micros();
			unsigned long delta = newUs - us;
			if( delta > 255 )
			{
//				Serial.print( "TIMEOUT: " );
//				Serial.println( delta );

				FStatus() = IDDHTLIB_ERROR_TIMEOUT;
				FState() = STOPPED;
				detachInterrupt( digitalPinToInterrupt( C_SensorOutputPin ));
				return;
			}

/*
			if( AValue )
				cycles[ FCount++ ] = delta;

			else
				cycles[ FCount++ ] = -delta;
*/
			us = newUs;

			switch( FState() )
			{
				case RESPONSE:
//					if( AValue )
//						return;

//					Serial.print( "RESPONSE: " );
//					Serial.println( delta );
					if( delta < 25 )
					{
						us -= delta;
						break; //do nothing, it started the response signal
					}

					if( 125 < delta && delta < 190 )
					{
//						Serial.println( "DATA 2" );
						FState() = DATA;
					}

					else
					{
//						Serial.print( "REPONSE: " );
//						Serial.println( delta );
//						Serial.println( AValue );

						detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
						FStatus() = IDDHTLIB_ERROR_TIMEOUT;
						FState() = STOPPED;
					}
					break;

				case DATA:
//					Serial.println( "DATA 1" );
//					Serial.println( delta );
					if( 60 < delta && delta < 145 )
					{ //valid in timing
						data[ FIndex() ] <<= 1; //shift the data
						if( delta > 100 ) //is a one
							data[ FIndex() ] |= 1;

						if( FBitIndex() == 0 )
						{  // when we have fulfilled the byte, go to the next
							FBitIndex() = 7;    // restart at MSB
							FIndex() = FIndex() + 1;
							if( FIndex() == 5)
							{ // go to next byte; when we have got 5 bytes, stop.
								detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
/*
								Serial.println( "DATA" );
								for( int i = 0; i < 5; ++i )
								{
									Serial.print( data[i] ); Serial.print( " " );
								}
								Serial.println( "" );

								for( int i = 0; i < FCount; ++i )
								{
									Serial.print( cycles[i] ); Serial.print( " " );
								}
								Serial.println( "" );
*/
								FState() = RAW_DATA_READY;
								break;
							}
						}

						else
							FBitIndex() = FBitIndex() - 1;

					}

					else if( delta < 10 )
					{
//						Serial.println( "IDDHTLIB_ERROR_DELTA" );
//						Serial.println( delta );
						detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
						FStatus() = IDDHTLIB_ERROR_DELTA;
						FState() = STOPPED;
					}

					else
					{
//						Serial.println( "IDDHTLIB_ERROR_TIMEOUT" );
//						Serial.println( delta );
						detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
						FStatus() = IDDHTLIB_ERROR_TIMEOUT;
						FState() = STOPPED;
					}

					break;

				default:
					break;
			}

		}

	public:
		inline void SystemInit() {} // Placeholder

		inline void SystemStart()
		{
			FLastReadTime = millis() - 2000;
			detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
			pinMode( C_SensorOutputPin, INPUT_PULLUP );
//			FSensor = new DHT( C_SensorOutputPin, SENSOR_TYPE );
//			FSensor->begin();
		}

		inline void SystemLoopBegin()
		{
//			Serial.println( FInterruptCount );
			ReadResult();

			if ( ! ( FState() == STOPPED || FState() == ACQUIRED ))
			{
				unsigned long delta = millis() - FLastReadTime;
				if( delta > 1000 )
				{
//				Serial.print( "TIMEOUT: " );
//				Serial.println( delta );

					FStatus() = IDDHTLIB_ERROR_TIMEOUT;
					FState() = STOPPED;
					detachInterrupt( digitalPinToInterrupt( C_SensorOutputPin ));
					digitalWrite( C_SensorOutputPin, HIGH );
					FLastReadTime = millis();
					return;
				}
			}

			if( FClocked() )
			{
//				Serial.println( "EXIT1" );
				ReadSensor();
				return;
			}

			if( ! ClockInputPin_o_IsConnected() )
				ReadSensor();

		}

	protected:
		void ReadSensor()
		{
			uint32_t currenttime = millis();
			if( (currenttime - FLastReadTime) < 2000 )
				return; // return last correct measurement

//			Serial.println( "ReadSensor" );
			FLastReadTime = currenttime;

			FClocked() = ( ! TryRead());
		}

		void ReadResult()
		{
			if( FState() != RAW_DATA_READY )
				return;

			if( ! Enabled().GetValue() )
				return;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
			{
				float   AValue = data[2] & 0x7F;
				AValue *= 256;
				AValue += data[3];
				AValue *= 0.1;
				if( data[2] & 0x80 )
					AValue *= -1;

				if( InFahrenheit() )
					AValue = Func::ConvertCtoF( AValue );

				T_TemperatureOutputPin::SetPinValue( AValue );
			}

			if( T_HumidityOutputPin::GetPinIsConnected() )
			{
				float   AValue = data[0];
				AValue *= 256;
				AValue += data[1];
				AValue *= 0.1;
				T_HumidityOutputPin::SetPinValue( AValue );
			}

			FState() = STOPPED;
		}

		bool TryRead()
		{
//			Serial.println( FState().GetValue() );
			if( FState() == STOPPED || FState() == ACQUIRED )
			{
				if( ! Enabled().GetValue() )
					return true;

//				Serial.print( "-" );
				detachInterrupt(digitalPinToInterrupt( C_SensorOutputPin ));
				//set the state machine for interruptions analisis of the signal
//				FState = useDelay ? RESPONSE : START_SIGNAL;
				FState() = RESPONSE;

				// EMPTY BUFFER and vars
				memset( (void *)data, 0, sizeof( data ));
//				for (uint8_t i = 0; i < 5; i++)
//					bits[i] = 0;

				FBitIndex() = 7;
				FIndex() = 0;
				FStatus() = 0;
//				FCount = 0;
//				hum = 0; // This will be the Humidity result!
//				temp = 0; // This will be the Temperature result!

				// REQUEST SAMPLE
				pinMode( C_SensorOutputPin, OUTPUT);
/*
				// Go into high impedence state to let pull-up raise data line level and
				// start the reading process.
				digitalWrite( C_SensorOutputPin, HIGH);
				delay(250);
*/
				digitalWrite( C_SensorOutputPin, LOW);

//				if (useDelay)
				{
					delay(18);
					digitalWrite( C_SensorOutputPin, HIGH);
					delayMicroseconds(25);
					pinMode( C_SensorOutputPin, INPUT_PULLUP );
				}

				us = micros();
				// Analize the data in an interrupt
//				if (useDelay)
//					attachInterrupt(intNumber, pCallbackArray[intNumber], FALLING);
				attachInterrupt( digitalPinToInterrupt( C_SensorOutputPin ), INTERRUPT_HANDLER, FALLING );
//					INTERRUPT_HANDLER.Reattach();
//				Serial.print( "+" );

//				Serial.println( "START" );
				return true;
			}

			return false;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline DHTSensor_Interrupt_DHT22()
		{
			FClocked() = false;
			FState() = STOPPED;
			FStatus() = IDDHTLIB_ERROR_NOTSTARTED;
//  #ifdef __AVR
//			_bit = digitalPinToBitMask( C_SensorOutputPin );
//			_port = digitalPinToPort( C_SensorOutputPin );
//  #endif
//			DHTSensorGlobals::Maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_HumidityOutputPin,
		typename T_InFahrenheit,
		typename T_TemperatureOutputPin
	> class DHT12SensorI2C :
		public T_Address,
		public T_Enabled,
		public T_HumidityOutputPin,
		public T_InFahrenheit,
		public T_TemperatureOutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Address )
		_V_PROP_( InFahrenheit )

	public:
		_V_PIN_( HumidityOutputPin )
		_V_PIN_( TemperatureOutputPin )

	protected:
		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

			uint8_t ADatos[ 5 ];

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( 0 );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address() ), uint8_t( 5 ));

			for( int i=0; i < 5; ++ i )
				ADatos[ i ] = C_I2C.read();

//			Serial.println( "TEST: " );
//			for( int i=0; i < 5; ++ i )
//			{
//				Serial.print( ADatos[ i ], HEX ); Serial.print( " " );
//			}

//			Serial.println( "" );

			if( ADatos[4] != ( ADatos[0] + ADatos[1] + ADatos[2] + ADatos[3] ))
				return; // Error

//			Serial.println( "TEST1" );
			float ATemperature = ( ADatos[2] + float( ADatos[ 3 ] ) / 10 );
			if( InFahrenheit() )
				ATemperature = Func::ConvertCtoF( ATemperature );

			T_TemperatureOutputPin::SetPinValue( ATemperature );

			float AHumidity = ( ADatos[ 0 ] + float( ADatos[ 1 ] ) / 10 );
			T_HumidityOutputPin::SetPinValue( AHumidity );
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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

