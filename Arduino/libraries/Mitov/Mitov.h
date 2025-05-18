////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#define VISUINO_BASE_YEAR 2000

#if ( defined( VISUINO_ESP32 ) || defined( SEEEDUINO_WIO_TERMINAL ))
	#define VISUINO_ESP32_BLE
#endif

#if ( defined( VISUINO_NANO33BLE ) || defined( VISUINO_RPI_PICO ))
	#include <api/deprecated-avr-comp/avr/dtostrf.h>
	#include <api/itoa.h>
#endif // VISUINO_NANO33BLE

#include <OpenWire.h>

#if defined(VISUINO_CIRCUIT_PLAYGROUND_EXPRESS) || defined(VISUINO_MACCHINA_M2)
//	#include <stdlib.h>
	#include "avr/dtostrf.h"
#endif

#if ! defined( SERIAL_TX_BUFFER_SIZE )
	#define SERIAL_TX_BUFFER_SIZE SERIAL_BUFFER_SIZE
#endif

#ifdef VISUINO_TRINKET_TWO_WIRE
	#define TinyM_USI_TWI TwoWire
#endif

#define _VISUINO_MAX_PIN_NO_ 0xFFFF

#ifdef _MITOV_SOFTWARE_SERIAL_SENDING_PIN_
	#define _MITOV_SERIAL_SENDING_PIN_
#endif

#ifdef FSPI
  #undef FSPI
#endif

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
  #define Mitov_ATmega_168_168P_328P
#endif

#ifdef VISUINO_ANY_ESP32
	typedef const char *MITOV_FILE_MODE;
	#define __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
#else
	typedef uint8_t MITOV_FILE_MODE;
#endif


#ifndef INPUT_PULLDOWN
	#ifndef VISUINO_ESP32
		#define INPUT_PULLDOWN INPUT
	#endif
#endif

#if defined( VISUINO_CHIPKIT_MAX32 ) || defined( VISUINO_CHIPKIT_UC32 ) || defined( VISUINO_CHIPKIT_DP32 ) || defined( VISUINO_CHIPKIT_LENNY )
	#define VISUINO_CHIPKIT
#endif

#if defined( VISUINO_FEATHER_M0_BASIC ) || defined( VISUINO_TRINKET_M0 ) || defined( VISUINO_MKR_ZERO )
	#define VISUINO_M0
	#define VISUINO_MEMORY_ALLIGN
	#define VISUINO_FEATHER_M0
#endif

#if defined( VISUINO_WEMOS_D1_R1 ) || defined( VISUINO_WEMOS_D1_R2 ) || defined( VISUINO_WEMOS_D1_MINI ) || defined( VISUINO_FEATHER_HUZZAH_ESP8266 )
	#define VISUINO_ESP8266
#endif

#if defined( VISUINO_STMDUINO )

#if defined( _WIRISH_WPROGRAM_H_ ) // For the olr Mapple compatibility
  #define PIN_MODE WiringPinMode
  #define VISUINO_STMDUINO_MAPLE_OLD
#endif

  #ifndef OUTPUT_OPEN_DRAIN
	#define OUTPUT_OPEN_DRAIN	OUTPUT
  #endif // OUTPUT_OPEN_DRAIN

#else
  #ifndef OUTPUT_OPEN_DRAIN
    #define OUTPUT_OPEN_DRAIN	OUTPUT
  #endif // OUTPUT_OPEN_DRAIN

#endif

#ifndef PWMRANGE
  #ifdef VISUINO_STMDUINO_MAPLE_OLD
    #define PWMRANGE 0xFFFF
  #else // VISUINO_STMDUINO
    #define PWMRANGE 0xFF
  #endif // VISUINO_STMDUINO

#endif // PWMRANGE


#ifdef VISUINO_ESP32
	#define VISUINO_ANALOG_IN_RANGE 4096
	#define VISUINO_ANALOG_IN_THRESHOLD 2000
#else
	#ifndef VISUINO_ANALOG_IN_RANGE
		#define VISUINO_ANALOG_IN_RANGE 1023
	#endif

	#define VISUINO_ANALOG_IN_THRESHOLD 409
#endif

#if defined(ESP8266)
  #define FEATHER_PIN_9  0
  #define FEATHER_PIN_6 16
  #define FEATHER_PIN_5  2
#elif defined(ESP32)
  #define FEATHER_PIN_9 15
  #define FEATHER_PIN_6 32
  #define FEATHER_PIN_5 14
#elif defined(ARDUINO_STM32_FEATHER)
  #define FEATHER_PIN_9 PA15
  #define FEATHER_PIN_6 PC7
  #define FEATHER_PIN_5 PC5
#elif defined(TEENSYDUINO)
  #define FEATHER_PIN_9  4
  #define FEATHER_PIN_6  3
  #define FEATHER_PIN_5  8
#elif defined(ARDUINO_FEATHER52832)
  #define FEATHER_PIN_9 31
  #define FEATHER_PIN_6 30
  #define FEATHER_PIN_5 27
#else // 32u4, M0, M4, nrf52840 and 328p
  #define FEATHER_PIN_9  9
  #define FEATHER_PIN_6  6
  #define FEATHER_PIN_5  5
#endif

#if defined(VISUINO_NODE_MCU)
	static const uint8_t MCU_D0   = 16;
	static const uint8_t MCU_D1   = 5;
	static const uint8_t MCU_D2   = 4;
	static const uint8_t MCU_D3   = 0;
	static const uint8_t MCU_D4   = 2;
	static const uint8_t MCU_D5   = 14;
	static const uint8_t MCU_D6   = 12;
	static const uint8_t MCU_D7   = 13;
	static const uint8_t MCU_D8   = 15;
	static const uint8_t MCU_D9   = 3;
	static const uint8_t MCU_D10  = 1;
	static const uint8_t MCU_D11  = 9;
	static const uint8_t MCU_D12  = 10;
	static const uint8_t MCU_D13  = 8;
	static const uint8_t MCU_D14  = 11;
	static const uint8_t MCU_D15  = 7;
	static const uint8_t MCU_D16  = 6;
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define mitov_pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define mitov_pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

//#define _VPROP_(TYPE,NAME) inline typename TYPE::T_TYPE		NAME() { return TYPE::GetValue(); }
//#define _VPROP_(TYPE,NAME) inline static TYPE		&NAME() { return TYPE::GetInstance(); }
#define _V_PROP_(NAME) inline T_##NAME		&NAME() { return T_##NAME::GetInstance(); }
//#define _VPROP_(TYPE,NAME) inline TYPE		&NAME() { TYPE *AResult; TYPE::GetInstance( AResult ); return *AResult; }
//#define _V_PROP_(NAME) inline T_##NAME		&NAME() { T_##NAME *AResult; T_##NAME::GetInstance( AResult ); return *AResult; }

//#define _VPIN_(TYPE,NAME) TYPE	&NAME() { return TYPE::GetInstance(); }
#define _V_PIN_(NAME) T_##NAME	&NAME() { return T_##NAME::GetInstance(); }

namespace Mitov
{
#ifdef VISUINO_ESP32
	const	uint8_t MicroBitPin_0	= 25;
	const	uint8_t MicroBitPin_1	= 32;
	const	uint8_t MicroBitPin_2	= 33;
	const	uint8_t MicroBitPin_3	= 13;
	const	uint8_t MicroBitPin_4	= 16;
	const	uint8_t MicroBitPin_5	= 35;
	const	uint8_t MicroBitPin_6	= 12;
	const	uint8_t MicroBitPin_7	= 14;
	const	uint8_t MicroBitPin_8	= 16;
	const	uint8_t MicroBitPin_9	= 17;
	const	uint8_t MicroBitPin_10	= 26;
	const	uint8_t MicroBitPin_11	= 27;
	const	uint8_t MicroBitPin_12	= 2;
	const	uint8_t MicroBitPin_13	= 18;
	const	uint8_t MicroBitPin_14	= 19;
	const	uint8_t MicroBitPin_15	= 23;
	const	uint8_t MicroBitPin_16	= 5;
	const	uint8_t MicroBitPin_19	= 22;
	const	uint8_t MicroBitPin_20	= 21;
#else
	const	uint8_t MicroBitPin_0	= 0;
	const	uint8_t MicroBitPin_1	= 1;
	const	uint8_t MicroBitPin_2	= 2;
	const	uint8_t MicroBitPin_3	= 3;
	const	uint8_t MicroBitPin_4	= 4;
	const	uint8_t MicroBitPin_5	= 5;
	const	uint8_t MicroBitPin_6	= 6;
	const	uint8_t MicroBitPin_7	= 7;
	const	uint8_t MicroBitPin_8	= 8;
	const	uint8_t MicroBitPin_9	= 9;
	const	uint8_t MicroBitPin_10	= 10;
	const	uint8_t MicroBitPin_11	= 11;
	const	uint8_t MicroBitPin_12	= 12;
	const	uint8_t MicroBitPin_13	= 13;
	const	uint8_t MicroBitPin_14	= 14;
	const	uint8_t MicroBitPin_15	= 15;
	const	uint8_t MicroBitPin_16	= 16;
	const	uint8_t MicroBitPin_19	= 19;
	const	uint8_t MicroBitPin_20	= 20;
#endif
}

#ifdef VISUINO_ANY_ESP32
//  #define PWMRANGE 255

namespace Mitov
{
#ifndef __BORLANDC__
	HardwareSerial Serial1(1);
	HardwareSerial Serial2(2);
#else // __BORLANDC__
	HardwareSerial &Serial1 = ::Serial1;
	HardwareSerial Serial2 = ::Serial2;
#endif // __BORLANDC__
}

namespace Mitov
{
//---------------------------------------------------------------------------
	uint8_t	CESP32PWMTimers[ 2 ] = {0};
	uint8_t	CESP32PWMPinMap[ 40 ] = {0};
//---------------------------------------------------------------------------
	void ServoAttach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] == 0 )
		{
			uint8_t AChannel = 0;
			for( AChannel = 0; AChannel < 16; ++AChannel )
				if( ! ( Mitov::CESP32PWMTimers[ AChannel >> 3 ] & ( 1 << ( AChannel & 0b111 ) ) ))
					break;

			if( AChannel == 16 )
				return; // Can't allocate channel

			Mitov::CESP32PWMPinMap[ APin ] = AChannel + 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] |= ( 1 << ( AChannel & 0b111 ) );
			ledcSetup( AChannel, 50, 16 ); // 50Hz 16 bit
//			ledcWrite( AChannel, AValue );
			ledcAttachPin( APin, AChannel );
		}
	}
//---------------------------------------------------------------------------
	void ServoDetach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			ledcDetachPin( APin );
			uint8_t AChannel = Mitov::CESP32PWMPinMap[ APin ] - 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] &= ~ ( 1 << ( AChannel & 0b111 ) );
			Mitov::CESP32PWMPinMap[ APin ] = 0;
		}
	}
//---------------------------------------------------------------------------
	void ServoWrite( uint8_t APin, float AValue, uint16_t APulseMin, uint16_t APulseMax )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			APulseMin *= 0xFFFF / 20000;
			APulseMax *= 0xFFFF / 20000;
			APulseMin += 512; //255;
			APulseMax += 512; //255;
			uint16_t AIntValue = ( AValue / 180 ) * ( APulseMax - APulseMin ) + 0.5;
			AIntValue += APulseMin;
			ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AIntValue );
		}
	}
//---------------------------------------------------------------------------
	void ToneAttach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] == 0 )
		{
			uint8_t AChannel = 0;
			for( AChannel = 0; AChannel < 16; ++AChannel )
				if( ! ( Mitov::CESP32PWMTimers[ AChannel >> 3 ] & ( 1 << ( AChannel & 0b111 ) ) ))
					break;

			if( AChannel == 16 )
				return; // Can't allocate channel

			Mitov::CESP32PWMPinMap[ APin ] = AChannel + 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] |= ( 1 << ( AChannel & 0b111 ) );
			ledcSetup( AChannel, 50, 16 ); // 50Hz 16 bit
//			ledcWrite( AChannel, AValue );
			ledcAttachPin( APin, AChannel );
		}
	}
//---------------------------------------------------------------------------
	void ToneDetach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			ledcDetachPin( APin );
			uint8_t AChannel = Mitov::CESP32PWMPinMap[ APin ] - 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] &= ~ ( 1 << ( AChannel & 0b111 ) );
			Mitov::CESP32PWMPinMap[ APin ] = 0;
		}
	}
//---------------------------------------------------------------------------
	void PlayTone( uint8_t APin, double freq )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			ledcWriteTone( Mitov::CESP32PWMPinMap[ APin ] - 1, freq );
		}
	}
//---------------------------------------------------------------------------
}

#endif // VISUINO_ESP32

#if defined( VISUINO_FREESOC2 )
  #include <itoa.h>
#endif

#if defined( VISUINO_ARDUINO_PORTENTA )
  #include <api/itoa.h>
#endif

#if defined( VISUINO_ARDUINO_DUE ) || defined( VISUINO_NRF52 ) || defined( VISUINO_BBC_MICRO_BIT ) || defined( VISUINO_FEATHER_M0 ) || defined( VISUINO_ARDUINO_PORTENTA )
  #include <avr/dtostrf.h>
#endif

//#if defined( VISUINO_TEENSY_3_0 ) || defined( VISUINO_TEENSY_3_1 ) || defined( VISUINO_TEENSY_LC ) || defined( VISUINO_TEENSY_3_5 ) || defined( VISUINO_TEENSY_3_6 ) || defined( VISUINO_TEENSY_4_0 ) || defined( VISUINO_TEENSY_4_1 )
//  #define VISUINO_TEENSY
//#endif

#if defined( VISUINO_ARDUINO_DUE ) || defined( VISUINO_LINKIT_ONE ) || defined( VISUINO_TEENSY ) || defined( VISUINO_TEENSY_2_0 ) || defined( VISUINO_TEENSY_2_0_PP ) || defined( VISUINO_ARDUINO_RASPBERRY_PI )
  #define NO_SERIAL_FORMAT
#endif

#ifdef VISUINO_TEENSY
namespace Mitov
{
	float GAnalogInDivider = 1023.0;
}
#endif

#if defined( VISUINO_ESP8266 )
  #define V_FMOD(a,b) (a - b * floor(a / b))
  #define sscanf os_sprintf
#else
  #define V_FMOD(a,b) fmod(a,b)
#endif

float posmod( float a, float b )
{
	a = V_FMOD(a,b);
	if( a < 0 )
		a += b;

	return a;
}

#define MAKE_CALLBACK_3_1(T1,T2,A,P) (T2)( void (T1::*) ( P ) )&A
#define MAKE_CALLBACK_3(T1,T2,A,P) (T1 *)this, (T2)( void (T1::*) ( P ) )&A
#define MAKE_CALLBACK_2(T1,T2,A) (T1 *)this, (T2)( void (T1::*) ( void *_Data ) )&A

#define MITOV_ARRAY_SIZE(A) ( sizeof(A) / sizeof(A[0]) )
#define ARRAY_PARAM(A) A,MITOV_ARRAY_SIZE(A)

namespace Mitov
{
    class TAnalogClass
    {
#ifdef VISUINO_ESP32
        protected:
        	static void analogWrite( uint8_t APin, uint16_t AValue )
        	{
        //		Serial.print( "analogWrite : " ); Serial.println( AValue );

        		if( AValue == 0 || AValue == 255 )
        		{
        			if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
        			{
        				ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AValue );
        //				Serial.println( "ledcDetachPin" );
        				digitalWrite( APin, (AValue == 255 ) ? HIGH : LOW );
        				ledcDetachPin( APin );
        				uint8_t AChannel = Mitov::CESP32PWMPinMap[ APin ] - 1;
        				Mitov::CESP32PWMTimers[ AChannel >> 3 ] &= ~ ( 1 << ( AChannel & 0b111 ) );
        				Mitov::CESP32PWMPinMap[ APin ] = 0;
        			}

        			pinMode( APin, OUTPUT );
        			digitalWrite( APin, (AValue == 255 ) ? HIGH : LOW );
        			return;
        		}

        		if( Mitov::CESP32PWMPinMap[ APin ] == 0 )
        		{
        			uint8_t AChannel = 0;
        			for( AChannel = 0; AChannel < 16; ++AChannel )
        				if( ! ( Mitov::CESP32PWMTimers[ AChannel >> 3 ] & ( 1 << ( AChannel & 0b111 ) ) ))
        					break;

        			if( AChannel == 16 )
        				return; // Can't allocate channel

        			Mitov::CESP32PWMPinMap[ APin ] = AChannel + 1;
        			Mitov::CESP32PWMTimers[ AChannel >> 3 ] |= ( 1 << ( AChannel & 0b111 ) );
        			ledcSetup( AChannel, 1000, 8 ); // 1KHz 8 bit
        			ledcWrite( AChannel, AValue );
        			ledcAttachPin( APin, AChannel );
        //			Serial.print( "ledcAttachPin: " ); Serial.print( APin ); Serial.print( " " ); Serial.println( AChannel );
        			return;
        		}

        		ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AValue );
        //		Serial.print( "ledcWrite: " ); Serial.print( Mitov::CESP32PWMPinMap[ APin ] - 1 ); Serial.print( " " ); Serial.println( AValue );
        	}

#endif // VISUINO_ESP32
        public:
            inline static void Write( uint32_t APinNumber, float AValue )
            {
                analogWrite( APinNumber, ( AValue * PWMRANGE ) + 0.5 );
            }

            inline static void WriteRaw( uint32_t APinNumber, uint16_t AValue )
            {
                analogWrite( APinNumber, AValue );
            }

    };
//---------------------------------------------------------------------------
    TAnalogClass Analog;
//---------------------------------------------------------------------------
	template <typename T> inline T PROGMEM_getAnything(const T * ASource )
	{
		T AResult;
		memcpy_P( &AResult, ASource, sizeof(T));
		return AResult;
	}
//---------------------------------------------------------------------------
	uint32_t Reverse32Bits( register uint32_t x )
	{
		x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
		x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
		x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
		x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
		return((x >> 16) | (x << 16));

	}
//---------------------------------------------------------------------------
	Mitov::String ByteToHEX( uint8_t AValue )
	{
		Mitov::String AResult( AValue, uint8_t( HEX ));
		while( AResult.length() < 2 )
			AResult = "0" + AResult;

		return AResult;
	}
//---------------------------------------------------------------------------
	struct FloatPoint2D
	{
		float X;
		float Y;
	};
//---------------------------------------------------------------------------
	struct FloatRect2D // : public FloatPoint2D
	{
		float X;
		float Y;
		float Width;
		float Height;
	};
//---------------------------------------------------------------------------
	template <typename T> class TAsBytes
	{
		union
		{
			T Value;
			uint8_t  Bytes[sizeof( T )];
		}	Value;

	public:
		inline TAsBytes<T>& operator=( const uint32_t AValue )
		{
			Value.Value = AValue;
			return *this;
		}

		inline uint8_t &operator[]( const uint32_t AIndex ) { return Value.Bytes[ AIndex ]; }

		inline operator T() { return Value.Value; }

	public:
		inline TAsBytes() {}
		inline TAsBytes( const uint32_t AValue )
		{
			Value.Value = AValue;
		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T> T log2( T log2_input)
	{
		return log(float( log2_input)) / log(float( 2 ));
	}
*/
	uint8_t MitovIlog2( uint32_t AValue )
	{
		for( int i = 0; i < 32; ++i )
		{
			if( !( AValue >> i ))
				return i;
		}

		return 32;
	}
//---------------------------------------------------------------------------
	template <typename T> T Constrain16( uint32_t AValue )
	{
		uint32_t AValue1 = MitovMin( AValue, uint32_t( 0xFFFF ));
		return AValue1;
	}
//---------------------------------------------------------------------------
	template<> int16_t Constrain16<int16_t>( uint32_t AValue )
	{
		int32_t AValue1 = constrain( (*(int32_t *)&AValue ), -0x8000l, 0x7FFFl );
		return AValue1;
	}
//---------------------------------------------------------------------------
	template <typename T> T Constrain16( int32_t AValue )
	{
		int32_t AValue1 = constrain( AValue, -0x8000l, 0x7FFFl );
		return AValue1;
	}
//---------------------------------------------------------------------------
	enum TArduinoGraphicsOrientation { goUp, goRight, goDown, goLeft }; // Use the same order as TArduino3DOrientation !
//---------------------------------------------------------------------------
	class TArduinoGraphicsOrientationObject
	{
	public:
		TArduinoGraphicsOrientation	Value;

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
			switch( Value )
			{
				case goUp : return "Up" ;
				case goDown : return "Down" ;
				case goLeft : return "Left" ;
				default :
					return "Right" ;
			}
		}

	public:
		TArduinoGraphicsOrientationObject( TArduinoGraphicsOrientation AValue ) :
			Value( AValue )
		{
		}
	};
//---------------------------------------------------------------------------
	namespace TArduino3DOrientation
	{
		enum TArduino3DOrientation { Up, Right, Down, Left, Front, Back }; // Use the same order as TArduinoGraphicsOrientation !
	}
//---------------------------------------------------------------------------
	class TArduino3DOrientationObject
	{
	public:
		TArduino3DOrientation::TArduino3DOrientation	Value;

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
			switch( Value )
			{
				case TArduino3DOrientation::Up : return "Up" ;
				case TArduino3DOrientation::Down : return "Down" ;
				case TArduino3DOrientation::Left : return "Left" ;
				case TArduino3DOrientation::Right : return "Right" ;
				case TArduino3DOrientation::Front : return "Front" ;
				default :
					return "Back" ;
			}
		}

	public:
		TArduino3DOrientationObject( TArduino3DOrientation::TArduino3DOrientation AValue ) :
			Value( AValue )
		{
		}
	};
//---------------------------------------------------------------------------
	enum TAngleUnits { auDegree, auRadians, auNormalized };
	typedef TAngleUnits TArduinoAngleUnits;
//---------------------------------------------------------------------------
	const uint16_t MonthDays[2][12] =
	{
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};
//---------------------------------------------------------------------------
	static const double HoursPerDay			= 24;
	static const double MinsPerHour			= 60;
	static const double SecsPerMin			= 60;
	static const double MSecsPerSec			= 1000;
	static const double MinsPerDay			= HoursPerDay * MinsPerHour;
	static const double SecsPerDay			= MinsPerDay * SecsPerMin;
	static const double SecsPerHour			= SecsPerMin * MinsPerHour;
	static const double MSecsPerDay			= SecsPerDay * MSecsPerSec;
	static const int32_t	IMSecsPerDay	= MSecsPerDay;
// Days between 1/1/0001 and 12/31/1899
//	const double DateDelta			= 693594;
//---------------------------------------------------------------------------
	template <
		typename T_Days,
		typename T_Hours,
		typename T_MilliSeconds,
		typename T_Minutes,
		typename T_Months,
		typename T_Seconds,
		typename T_WeekDays,
		typename T_Years
	> class TDateTimeCompareItems :
		public T_Days,
		public T_Hours,
		public T_MilliSeconds,
		public T_Minutes,
		public T_Months,
		public T_Seconds,
		public T_WeekDays,
		public T_Years
	{
	public:
		_V_PROP_( Years )
		_V_PROP_( Months )
		_V_PROP_( Days )
		_V_PROP_( WeekDays )
		_V_PROP_( Hours )
		_V_PROP_( Minutes )
		_V_PROP_( Seconds )
		_V_PROP_( MilliSeconds )

	public:
		inline bool AllTrue()
		{
			return Years().GetValue() && Months().GetValue() && Days().GetValue() && Hours().GetValue() && Minutes().GetValue() && Seconds().GetValue() && MilliSeconds().GetValue();
		}

		inline bool AllDateTrue()
		{
			return Years().GetValue() && Months().GetValue() && Days().GetValue();
		}

		inline bool AllDateFalse()
		{
			return ( ! Years().GetValue() ) && ( ! Months().GetValue() ) && ( ! Days().GetValue() );
		}

		inline bool AllTimeTrue()
		{
			return Hours().GetValue() && Minutes().GetValue() && Seconds().GetValue() && MilliSeconds().GetValue();
		}

		inline bool AllTimeFalse()
		{
			return ( ! Hours().GetValue() ) && ( ! Minutes().GetValue() ) && ( ! Seconds().GetValue() ) && ( ! MilliSeconds().GetValue() );
		}

		inline bool CompareWeekDays()
		{
			return AllDateFalse() && WeekDays().GetValue();
		}
	};
//---------------------------------------------------------------------------
	class TComplex
	{
	public:
		float Real;
		float Imaginary;

	public:
        Mitov::String ToString() const
        {
			return "[" + Mitov::String( Real, uint8_t( 10 )) + "," + Mitov::String( Imaginary, uint8_t( 10 )) + "]";
        }

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		bool operator == ( const TComplex &AOther ) const
		{
			if( Real != AOther.Real )
				return false;

			return ( Imaginary == AOther.Imaginary );
		}

		bool operator != ( const TComplex &AOther ) const
		{
			if( Real != AOther.Real )
				return true;

			return ( Imaginary != AOther.Imaginary );
		}

	public:
		TComplex() :
			Real( 0 ),
			Imaginary( 0 )
		{
		}

		TComplex( float AReal ) :
			Real( AReal ),
			Imaginary( 0 )
		{
		}

		TComplex( float AReal, float AImaginary ) :
			Real( AReal ),
			Imaginary( AImaginary )
		{
		}
	};
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TCompactColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;

	public:
		inline void SetValue( const int32_t AValue )
		{
#if defined( VISUINO_MEMORY_ALLIGN )
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
#else
			*((int32_t *)this) = AValue & 0xFFFFFF;
#endif
		}

		inline void operator = ( const long AValue )
		{
			SetValue( AValue );
		}

	public:
		inline TCompactColor()
		{
		}

		inline TCompactColor( const long AValue )
		{
			SetValue( AValue );
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		uint8_t Alpha = 0;

	public:
		inline void operator = ( const TCompactColor AValue )
		{
			Red = AValue.Red;
			Green = AValue.Green;
			Blue = AValue.Blue;
		}

		inline TColor( const TCompactColor AValue )
		{
			Red = AValue.Red;
			Green = AValue.Green;
			Blue = AValue.Blue;
		}

	public:
		inline void SetValue( const int32_t AValue )
		{
//#if defined( VISUINO_MEMORY_ALLIGN )
			Blue = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Red = AValue & 0xFF;
//#else
//			*((int32_t *)this) = AValue & 0xFFFFFF;
//#endif

//			Blue = ( AValue >> 16 ) & 0xFF;
//			Green = ( AValue >> 8 ) & 0xFF;
//			Red = AValue & 0xFF;
//			Alpha = 0;
		}

		inline void operator = ( const long AValue )
		{
			SetValue( AValue );
		}

		inline bool operator == ( const TColor other )
		{
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( Red == other.Red ) && ( Green == other.Green ) && ( Blue == other.Blue ) && ( Alpha == other.Alpha ));
#else
			return *((long *)this) == *((long *)&other);
#endif
		}

		inline bool operator != ( const TColor other )
		{
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( Red != other.Red ) || ( Green != other.Green ) || ( Blue != other.Blue ) || ( Alpha != other.Alpha ));
#else
			return *((long *)this) != *((long *)&other);
#endif
		}

		inline operator long() const
		{
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( uint32_t( Blue ) << 16 ) | ( uint32_t( Green ) << 8 ) | Red );
#else
			return *((long *)this);
#endif
		}

		inline uint32_t AsRGBWord() const
		{
			return (( uint32_t( Red ) << 16 ) | ( uint32_t( Green ) << 8 ) | Blue );
		}

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
			return	Mitov::String( "(R:" ) +
					Mitov::String((int)Red ) + ",G:" +
					Mitov::String((int)Green ) + ",B:" +
					Mitov::String((int)Blue ) + ")";
		}

		uint16_t To565Color()
		{
			return (((31*(Red+4))/255)<<11) |
               (((63*(Green+2))/255)<<5) |
               ((31*(Blue+4))/255);
		}

		uint16_t BGRTo565Color()
		{
			return (((31*(Blue+4))/255)<<11) |
               (((63*(Green+2))/255)<<5) |
               ((31*(Red+4))/255);
		}
/*
		uint32_t GetGBR()
		{
			return ( uint32_t( Red ) << 16 ) | ( uint32_t( Green ) << 8 ) | Blue;
		}
*/
	public:
		TColor( long AValue = 0 )
		{
			SetValue( AValue );
		}

		TColor( uint8_t ARed, uint8_t AGreen, uint8_t ABlue ) :
			Red( ARed ),
			Green( AGreen ),
			Blue( ABlue )
		{
		}

		TColor( long AValue, bool ) // For Windows BGR support
		{
			Blue = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Red = AValue & 0xFF;
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TRGBWColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		uint8_t White;

	public:
		void SetValue( const uint32_t AValue )
		{
//			*((uint32_t *)this) = AValue;
			White = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}

		inline void operator = ( const long AValue )
		{
			SetValue( AValue );
		}

		inline bool operator ==( const TRGBWColor other )
		{
//			Serial.println( "==" );
//			Serial.println( other.ToString() );
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( Red == other.Red ) && ( Green == other.Green ) && ( Blue == other.Blue ) && ( White == other.White ));

#else
			return *((uint32_t *)this) == *((uint32_t *)&other);
#endif
		}

		inline bool operator != ( const TRGBWColor other )
		{
//			Serial.println( "==" );
//			Serial.println( other.ToString() );
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( Red != other.Red ) || ( Green != other.Green ) || ( Blue != other.Blue ) || ( White != other.White ));

#else
			return *((uint32_t *)this) != *((uint32_t *)&other);
#endif
		}

		inline operator long() const
		{
			return long( White ) << 24 |
							long( Red ) << 16 |
							long( Green ) << 8 |
							Blue;
//			White = ( AValue >> 24 ) & 0xFF;
//			Red = ( AValue >> 16 ) & 0xFF;
//			Green = ( AValue >> 8 ) & 0xFF;
//			Blue = AValue & 0xFF;
//			return *((long *)this);
		}

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
			return	Mitov::String( "(R:" ) +
					Mitov::String((int)Red ) + ",G:" +
					Mitov::String((int)Green ) + ",B:" +
					Mitov::String((int)Blue ) + ",W:" +
					Mitov::String((int)White ) + ")";
		}

		inline uint16_t To565Color()
		{
			return (((31*(Red+4))/255)<<11) |
               (((63*(Green+2))/255)<<5) |
               ((31*(Blue+4))/255);
		}

	public:
		TRGBWColor( uint32_t AValue = 0 )
		{
			SetValue( AValue );
		}

		TRGBWColor( TColor &AColor )
		{
//			Serial.println( AColor.Red );

			Red = AColor.Red;
			Green = AColor.Green;
			Blue = AColor.Blue;
			White = 0;
		}

		TRGBWColor( unsigned char ARed, unsigned char AGreen, unsigned char ABlue, unsigned char AWhite ) :
			Red( ARed ),
			Green( AGreen ),
			Blue( ABlue ),
			White( AWhite )
		{
		}

		TRGBWColor( uint32_t AValue, bool ) // For Windows BGR support
		{
			White = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TAlphaColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		uint8_t Alpha;

	public:
		void SetValue( const uint32_t AValue )
		{
//			*((uint32_t *)this) = AValue;
			Alpha = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}

		inline void operator = ( const long AValue )
		{
			SetValue( AValue );
		}

		inline bool operator == ( const TAlphaColor other )
		{
//			Serial.println( "==" );
//			Serial.println( other.ToString() );
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( Red == other.Red ) && ( Green == other.Green ) && ( Blue == other.Blue ) && ( Alpha == other.Alpha ));

#else
			return *((uint32_t *)this) == *((uint32_t *)&other);
#endif
		}

		inline bool operator != ( const TAlphaColor other )
		{
//			Serial.println( "==" );
//			Serial.println( other.ToString() );
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( Red != other.Red ) || ( Green != other.Green ) || ( Blue != other.Blue ) || ( Alpha != other.Alpha ));

#else
			return *((uint32_t *)this) != *((uint32_t *)&other);
#endif
		}

		inline operator long() const
		{
			return long( Alpha ) << 24 |
							long( Red ) << 16 |
							long( Green ) << 8 |
							Blue;
//			Alpha = ( AValue >> 24 ) & 0xFF;
//			Red = ( AValue >> 16 ) & 0xFF;
//			Green = ( AValue >> 8 ) & 0xFF;
//			Blue = AValue & 0xFF;
//			return *((long *)this);
		}

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
			return	Mitov::String( "(R:" ) +
					Mitov::String((int)Red ) + ",G:" +
					Mitov::String((int)Green ) + ",B:" +
					Mitov::String((int)Blue ) + ",A:" +
					Mitov::String((int)Alpha ) + ")";
		}

		inline uint16_t To565Color()
		{
			return (((31*(Red+4))/255)<<11) |
               (((63*(Green+2))/255)<<5) |
               ((31*(Blue+4))/255);
		}

	public:
		TAlphaColor( uint32_t AValue = 0 )
		{
			SetValue( AValue );
		}

		TAlphaColor( TColor &AColor )
		{
//			Serial.println( AColor.Red );

			Red = AColor.Red;
			Green = AColor.Green;
			Blue = AColor.Blue;
			Alpha = 0xFF;
		}

		TAlphaColor( unsigned char ARed, unsigned char AGreen, unsigned char ABlue, unsigned char AAlpha ) :
			Red( ARed ),
			Green( AGreen ),
			Blue( ABlue ),
			Alpha( AAlpha )
		{
		}

		TAlphaColor( uint32_t AValue, bool ) // For Windows BGR support
		{
			Alpha = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TRGBWAlphaColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		uint8_t White;
		uint8_t Alpha;

	public:
		void SetValue( const uint64_t AValue )
		{
//			*((uint32_t *)this) = AValue;
			Alpha = ( AValue >> 32 ) & 0xFF;
			White = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}

		inline void operator = ( const uint64_t AValue )
		{
			SetValue( AValue );
		}

		inline bool operator ==( const TRGBWAlphaColor other )
		{
//			Serial.println( "==" );
//			Serial.println( other.ToString() );
			return (( Red == other.Red ) && ( Green == other.Green ) && ( Blue == other.Blue ) && ( White == other.White ) && ( Alpha == other.Alpha ));
		}

		inline bool operator != ( const TRGBWAlphaColor other )
		{
//			Serial.println( "==" );
//			Serial.println( other.ToString() );
			return (( Red != other.Red ) || ( Green != other.Green ) || ( Blue != other.Blue ) || ( White != other.White ) || ( Alpha != other.Alpha ));
		}

		inline operator int64_t() const
		{
			return int64_t( Alpha ) << 32 |
							int64_t( White ) << 24 |
							int64_t( Red ) << 16 |
							int64_t( Green ) << 8 |
							Blue;
//			White = ( AValue >> 24 ) & 0xFF;
//			Red = ( AValue >> 16 ) & 0xFF;
//			Green = ( AValue >> 8 ) & 0xFF;
//			Blue = AValue & 0xFF;
//			return *((long *)this);
		}

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
			return	Mitov::String( "(R:" ) +
					Mitov::String((int)Red ) + ",G:" +
					Mitov::String((int)Green ) + ",B:" +
					Mitov::String((int)Blue ) + ",W:" +
					Mitov::String((int)White ) + ",A:" +
					Mitov::String((int)Alpha ) + ")";
		}

		inline uint16_t To565Color()
		{
			return (((31*(Red+4))/255)<<11) |
               (((63*(Green+2))/255)<<5) |
               ((31*(Blue+4))/255);
		}

	public:
		TRGBWAlphaColor( uint64_t AValue = 0 )
		{
			SetValue( AValue );
		}

		TRGBWAlphaColor( TRGBWColor &AColor )
		{
//			Serial.println( AColor.Red );

			Red = AColor.Red;
			Green = AColor.Green;
			Blue = AColor.Blue;
			White = AColor.White;
			Alpha = 0;
		}

		TRGBWAlphaColor( unsigned char ARed, unsigned char AGreen, unsigned char ABlue, unsigned char AWhite, unsigned char AAlpha ) :
			Red( ARed ),
			Green( AGreen ),
			Blue( ABlue ),
			White( AWhite ),
			Alpha( AAlpha )
		{
		}

		TRGBWAlphaColor( uint64_t AValue, bool ) // For Windows BGR support
		{
			Alpha = ( AValue >> 32 ) & 0xFF;
			White = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	class TDateTime
	{
	public:
		// Do not change the order! Date Must be after Time for pcket communication!
		int32_t Time; // Number of milliseconds since midnight
		int32_t Date; // One plus number of days since 1/1/0001

	protected:
		inline void DivMod( int32_t Dividend, uint16_t Divisor, uint16_t &Result, uint16_t &Remainder ) const
		{
			Result = Dividend / Divisor;
			Remainder = Dividend % Divisor;
		}

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		void DecodeTime( uint16_t &AHour, uint16_t &AMin, uint16_t &ASec, uint16_t &AMSec ) const
		{
			uint16_t	MinCount, MSecCount;

			DivMod( Time, SecsPerMin * MSecsPerSec, MinCount, MSecCount );
			DivMod( MinCount, MinsPerHour, AHour, AMin );
			DivMod( MSecCount, MSecsPerSec, ASec, AMSec );
		}

		bool IsLeapYear( uint16_t Year ) const
		{
			return (Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0));
		}

		bool DecodeDateFully( uint16_t &Year, uint16_t &Month, uint16_t &Day, uint16_t &DOW ) const
		{
			const uint32_t TD1 = 365;
			const uint32_t TD4 = TD1 * 4 + 1;
			const uint32_t D100 = TD4 * 25 - 1;
			const uint32_t D400 = D100 * 4 + 1;

			int32_t T = Date;
			if( T <= 0 )
			{
				Year = 0;
				Month = 0;
				Day = 0;
				DOW = 0;
				return( false );
			}

			else
			{
				DOW = T % 7 + 1;
				--T;
				uint16_t Y = 1;
				while( T >= (int32_t)D400 )
				{
					T -= D400;
					Y += 400;
				}

				uint16_t D, I;
				DivMod( T, D100, I, D );
				if( I == 4 )
				{
					-- I;
					D -= D100;
				}

				Y += I * 100;
				DivMod(D, TD4, I, D);

				Y += I * 4;
				DivMod(D, TD1, I, D);

				if( I == 4 )
				{
					--I;
					D += TD1;
				}

				Y += I;
				bool Result = IsLeapYear(Y);

				const uint16_t *DayTable = MonthDays[Result];
				uint16_t M = 0; // The C++ Day table is zero indexed!
				for(;;)
				{
					I = DayTable[M];
					if( D < I )
						break;

					D -= I;
					++ M;
				}

				Year = Y;
				Month = M + 1;
				Day = D + 1;

                return Result;
			}
		}

		bool TryEncodeDate( uint16_t Year, uint16_t Month, uint16_t Day )
		{
			const uint16_t *DayTable = MonthDays[IsLeapYear(Year)];
			if( (Year >= 1) && (Year <= 9999) && (Month >= 1) && (Month <= 12) && (Day >= 1))
				if( Day <= DayTable[Month - 1] )
				{

					for( uint32_t i = 1; i < Month; i ++ )
						Day += DayTable[ i - 1 ];

					int I = Year - 1;
					Date = ((uint32_t)I) * 365 + (int)( I / 4 ) - (int)( I / 100 ) + (int)( I / 400 ) + (uint32_t)Day;
					return true;
				}

			return false;
		}

		bool TryEncodeTime( uint16_t Hour, uint16_t AMin, uint16_t Sec, uint16_t MSec )
		{
			if ((Hour < HoursPerDay) && (AMin < MinsPerHour) && (Sec < SecsPerMin) && (MSec < MSecsPerSec))
			{
				Time =  (Hour * (MinsPerHour * SecsPerMin * MSecsPerSec))
					  + (AMin * SecsPerMin * MSecsPerSec)
					  + (Sec * MSecsPerSec)
					  +  MSec;
//				Date = DateDelta; // This is the "zero" day for a TTimeStamp, days between 1/1/0001 and 12/30/1899 including the latter date
				Date = 0; // This is the "zero" day for a TTimeStamp, days between 1/1/0001 and 12/30/1899 including the latter date
				return true;
			}

			return false;
		}

		bool TryEncodeDateTime( uint16_t AYear, uint16_t AMonth, uint16_t ADay, uint16_t AHour, uint16_t AMinute, uint16_t ASecond, uint16_t AMilliSecond )
		{
			bool Result = TryEncodeDate( AYear, AMonth, ADay );
			if( Result )
			{
				TDateTime LTime;
				Result = LTime.TryEncodeTime( AHour, AMinute, ASecond, AMilliSecond );
				if( Result )
					Time = LTime.Time;

			}

            return Result;
		}

		void DecodeDateTime( uint16_t &AYear, uint16_t &AMonth, uint16_t &ADay, uint16_t &AHour, uint16_t &AMinute, uint16_t &ASecond, uint16_t &AMilliSecond ) const
		{
			uint16_t AWeekDay;
			DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );
		}

		void DecodeDateTime( uint16_t &AYear, uint16_t &AMonth, uint16_t &ADay, uint16_t &AWeekDay, uint16_t &AHour, uint16_t &AMinute, uint16_t &ASecond, uint16_t &AMilliSecond ) const
		{
			DecodeDateFully( AYear, AMonth, ADay, AWeekDay );
			DecodeTime( AHour, AMinute, ASecond, AMilliSecond );
		}

		Mitov::String ToString() const
		{
			uint16_t AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond;
			DecodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond );
#ifdef VISUINO_K210
			char ABuffer[ 6 + 4 + 5 * 2 + 3 + 2 + 1 + 20 ]; // Added some 20 padding for Maixduino
#else
			char ABuffer[ 6 + 4 + 5 * 2 + 3 + 2 + 1 + 5 ]; // Added some 5 padding for Maixduino
#endif
			sprintf( ABuffer, "(%04d.%02d.%02d %02d:%02d:%02d.%03d)", AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond );
			return ABuffer;
		}

		uint32_t JulianDate()
		{
			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t DOW;

			DecodeDateFully( AYear, AMonth, ADay, DOW );
//			Serial.println( AYear );
			if( AMonth <= 2 )
			{
				AYear--;
				AMonth += 12;
			}

			int A = AYear / 100;
			int B = 2 - A + A / 4;
			return uint32_t((365.25*( AYear+4716))+(int)(30.6001*(AMonth+1))+ ADay+B-1524 );
		}

		void AddMilliSeconds( int64_t AValue )
		{
			AllignTime( AValue + Time );
//			int64_t
//			if( int64_t( AValue ) + >= MSecsPerDay )
//			int32_t AOldTime
		}

		void AddSeconds( int64_t AValue )
		{
//			Serial.println( uint32_t( AValue ));
			AllignTime( int64_t( AValue * MSecsPerSec ) + Time );
		}

		void AddMinutes( int64_t AValue )
		{
			AllignTime( int64_t( AValue * MSecsPerSec * SecsPerMin ) + Time );
		}

		void AddHours( int64_t AValue )
		{
			AllignTime( int64_t( AValue * MSecsPerSec * SecsPerMin * MinsPerHour ) + Time );
		}

		void AddDays( int32_t AValue )
		{
			Date += AValue;
		}

		void AddMonths( int32_t AValue )
		{
			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t DOW;
			DecodeDateFully( AYear, AMonth, ADay, DOW );

			IncAMonth( AYear, AMonth, ADay, AValue );

			TryEncodeDate( AYear, AMonth, ADay );
//			Date += AValue;
		}

		void AddYears( int32_t AValue )
		{
			AddMonths( AValue * 12 );
		}

	protected:
		void AllignTime( int64_t AValue )
		{
//			Serial.println( "AllignTime" );
//			Serial.println( int32_t( AValue ));
//			Serial.println( Time );
			Date += AValue / IMSecsPerDay;
			Time = AValue % IMSecsPerDay;
			if( Time < 0 )
			{
				Time += IMSecsPerDay;
				Date --;
			}

//			Serial.println( Time );
		}

		void IncAMonth( uint16_t &AYear, uint16_t &AMonth, uint16_t &ADay, int32_t ANumberOfMonths )
		{
			int Sign;

			if( ANumberOfMonths >= 0 )
				Sign = 1;

			else
				Sign = -1;

			AYear += ANumberOfMonths / 12;
			ANumberOfMonths %= 12;
			int64_t ABigMonth = AMonth;
			ABigMonth += ANumberOfMonths;
			if( uint64_t( ABigMonth - 1 ) > 11 )    // if Month <= 0, word(Month-1) > 11)
			{
				AYear += Sign;
				ABigMonth += -12 * Sign;
			}

			AMonth = ABigMonth;

			const uint16_t *DayTable = MonthDays[IsLeapYear(AYear)];
			if( ADay > DayTable[ AMonth - 1 ] )
				ADay = DayTable[ AMonth - 1 ];

		}


	public:
		template <typename T_ITEMS> bool IsEqual( const TDateTime &AOther, T_ITEMS &AItems ) const
		{
			if( AItems.AllTrue() )
				return ( *this == AOther );

			if( AItems.AllDateTrue() && AItems.AllTimeFalse() )
				return ( Date == AOther.Date );

			if( AItems.AllTimeTrue() && AItems.AllDateFalse() )
				return ( Time == AOther.Time );

			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;

			DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

			uint16_t AOtherYear;
			uint16_t AOtherMonth;
			uint16_t AOtherDay;
			uint16_t AOtherWeekDay;
			uint16_t AOtherHour;
			uint16_t AOtherMinute;
			uint16_t AOtherSecond;
			uint16_t AOtherMilliSecond;

			AOther.DecodeDateTime( AOtherYear, AOtherMonth, AOtherDay, AOtherWeekDay, AOtherHour, AOtherMinute, AOtherSecond, AOtherMilliSecond );

			if( AItems.CompareWeekDays() )
				if( AWeekDay != AOtherWeekDay )
					return false;

			if( AItems.Years() )
				if( AYear != AOtherYear )
					return false;

			if( AItems.Months() )
				if( AMonth != AOtherMonth )
					return false;

			if( AItems.Days() )
				if( ADay != AOtherDay )
					return false;

			if( AItems.Hours() )
				if( AHour != AOtherHour )
					return false;

			if( AItems.Minutes() )
				if( AMinute != AOtherMinute )
					return false;

			if( AItems.Seconds() )
				if( ASecond != AOtherSecond )
					return false;

			if( AItems.MilliSeconds() )
				if( AMilliSecond != AOtherMilliSecond )
					return false;

			return true;
		}

		template <typename T_ITEMS> bool IsBigger( const TDateTime &AOther, T_ITEMS &AItems ) const
		{
			if( AItems.AllTrue() )
				return ( *this > AOther );

			if( AItems.AllDateTrue() && AItems.AllTimeFalse() )
				return ( Date > AOther.Date );

			if( AItems.AllTimeTrue() && AItems.AllDateFalse() )
				return ( Time > AOther.Time );

			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;

			DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

			uint16_t AOtherYear;
			uint16_t AOtherMonth;
			uint16_t AOtherDay;
			uint16_t AOtherWeekDay;
			uint16_t AOtherHour;
			uint16_t AOtherMinute;
			uint16_t AOtherSecond;
			uint16_t AOtherMilliSecond;

			AOther.DecodeDateTime( AOtherYear, AOtherMonth, AOtherDay, AOtherWeekDay, AOtherHour, AOtherMinute, AOtherSecond, AOtherMilliSecond );

			if( AItems.CompareWeekDays() )
			{
				if( AWeekDay < AOtherWeekDay )
					return false;

				if( AWeekDay > AOtherWeekDay )
					return true;
			}

			if( AItems.Years().GetValue() )
			{
				if( AYear < AOtherYear )
					return false;

				if( AYear > AOtherYear )
					return true;
			}

			if( AItems.Months().GetValue() )
			{
				if( AMonth < AOtherMonth )
					return false;

				if( AMonth > AOtherMonth )
					return true;
			}

			if( AItems.Days().GetValue() )
			{
				if( ADay < AOtherDay )
					return false;

				if( ADay > AOtherDay )
					return true;
			}

			if( AItems.Hours().GetValue() )
			{
				if( AHour < AOtherHour )
					return false;

				if( AHour > AOtherHour )
					return true;
			}

			if( AItems.Minutes().GetValue() )
			{
				if( AMinute < AOtherMinute )
					return false;

				if( AMinute > AOtherMinute )
					return true;
			}

			if( AItems.Seconds().GetValue() )
			{
				if( ASecond < AOtherSecond )
					return false;

				if( ASecond > AOtherSecond )
					return true;
			}

			if( AItems.MilliSeconds().GetValue() )
			{
				if( AMilliSecond < AOtherMilliSecond )
					return false;

				if( AMilliSecond > AOtherMilliSecond )
					return true;
			}

			return true;
		}

		template <typename T_ITEMS> bool IsBiggerOrEqual( const TDateTime &AOther, T_ITEMS &AItems ) const
		{
			if( AItems.AllTrue() )
				return ( *this >= AOther );

			return( IsEqual( AOther, AItems ) || IsBigger( AOther, AItems ));
		}

	public:
		bool operator == ( const TDateTime &AOther ) const
		{
			if( Date != AOther.Date )
				return false;

			return ( Time == AOther.Time );
		}

		bool operator != ( const TDateTime &AOther ) const
		{
			if( Date != AOther.Date )
				return true;

			return ( Time != AOther.Time );
		}

		bool operator <= ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return false;

			if( Date < AOther.Date )
				return true;

			return ( Time <= AOther.Time );
		}

		bool operator >= ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return true;

			if( Date < AOther.Date )
				return false;

			return ( Time >= AOther.Time );
		}

		bool operator < ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return false;

			if( Date < AOther.Date )
				return true;

			return ( Time < AOther.Time );
		}

		bool operator > ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return true;

			if( Date < AOther.Date )
				return false;

			return ( Time > AOther.Time );
		}

	public:
		inline TDateTime() :
			Time( 0 ),
			Date( 693594 )
		{
		}

		inline TDateTime( int32_t ADate, int32_t ATime ) :
			Time( ATime ),
			Date( ADate )
		{
		}

		inline TDateTime( uint32_t ADummy ) :
			Time( 0 ),
			Date( 693594 )
		{
		}

	};
//---------------------------------------------------------------------------
    template <
        typename T_OWNER, T_OWNER &C_OWNER,
        uint32_t C_INDEX,
        typename T_Imaginary,
        typename T_Real
    > class TArduinoBasicComplexValueElement :
        public T_Imaginary,
        public T_Real
    {
    public:
        _V_PROP_( Imaginary )
        _V_PROP_( Real )

    public:
		inline void UpdateValue()
        {
            C_OWNER.SetIndexedValue( C_INDEX );
        }

    public:
        inline void GetValue( Mitov::TComplex & AValue )
        {
            AValue.Real = Real().GetValue();
            AValue.Imaginary = Imaginary().GetValue();
        }

        inline void ApplyValues( Mitov::TComplex * ABuffer )
        {
            ABuffer[ C_INDEX ].Real = Real().GetValue();
            ABuffer[ C_INDEX ].Imaginary = Imaginary().GetValue();
        }

    };
//---------------------------------------------------------------------------
    template <
        typename T_OWNER, T_OWNER &C_OWNER,
        uint32_t C_INDEX,
        typename T_TYPE,
        typename T_Value
    > class TArduinoTypedValueElement :
        public T_Value
    {
    public:
        _V_PROP_( Value )

    public:
        inline void UpdateValue()
        {
            C_OWNER.SetIndexedValue( C_INDEX );
        }

    public:
		inline void SetIndexedValue( uint32_t AIndex )
        {
            C_OWNER.SetIndexedValue( C_INDEX );
        }

    public:
        inline void GetValue( T_TYPE & AValue )
        {
            AValue = Value().GetValue();
        }

        inline void ApplyValues( T_TYPE * ABuffer )
        {
            ABuffer[ C_INDEX ] = Value().GetValue();
        }

    };
//---------------------------------------------------------------------------
/*
    template <
        typename T_OWNER, T_OWNER &C_OWNER,
        uint32_t C_INDEX
    > class TArduinoDigitalValueElement
    {
    public:
        inline void SetValue( bool AValue )
        {
            C_OWNER.SetIndexedValue( C_INDEX, AValue );
//            C_OWNER.UpdateArrayValueIndex( C_INDEX );
        }

    };
*/
//---------------------------------------------------------------------------
	class TQuaternion
	{
	public:
		float Values[ 4 ];

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
			return Mitov::String( "(" ) +
					Mitov::String( Values[ 0 ], uint8_t( 10 ) ) + "," +
					Mitov::String( Values[ 1 ], uint8_t( 10 ) ) + "," +
					Mitov::String( Values[ 2 ], uint8_t( 10 ) ) + "," +
					Mitov::String( Values[ 3 ], uint8_t( 10 ) ) + ")";
		}

	public:
		inline float Imaginary_X() { return Values[ 0 ]; }
		inline float Imaginary_Y() { return Values[ 1 ]; }
		inline float Imaginary_Z() { return Values[ 2 ]; }
		inline float Real() { return Values[ 3 ]; }

    public:
        inline void SetX( float AValue )
        {
            Values[ 0 ] = AValue;
        }

        inline void SetY( float AValue )
        {
            Values[ 1 ] = AValue;
        }

        inline void SetZ( float AValue )
        {
            Values[ 2 ] = AValue;
        }

        inline void SetReal( float AValue )
        {
            Values[ 3 ] = AValue;
        }

//    public:
//        inline void UpdateArrayValue()
//        {
//            C_OWNER.UpdateArrayValue();
//        }

	public:
		bool operator == ( const TQuaternion &AOther ) const
		{
			return( memcmp( Values, AOther.Values, sizeof( Values ) ) == 0 );
		}

		bool operator != ( const TQuaternion &AOther ) const
		{
			return( memcmp( Values, AOther.Values, sizeof( Values ) ) != 0 );
		}

	public:
		TQuaternion()
		{
			memset( Values, 0, sizeof( Values ));
		}

		TQuaternion( float AReal )
		{
			Values[ 0 ] = 0.0f;
			Values[ 1 ] = 0.0f;
			Values[ 2 ] = 0.0f;
			Values[ 3 ] = AReal;
		}

		TQuaternion( float A, float B, float C, float D )
		{
			Values[ 0 ] = A;
			Values[ 1 ] = B;
			Values[ 2 ] = C;
			Values[ 3 ] = D;
		}
	};
//---------------------------------------------------------------------------
    template <
        typename T_OWNER, T_OWNER &C_OWNER,
        typename T_X,
        typename T_Y,
        typename T_Z
    > class TArduinoQuaternion3DPoint :
        public T_X,
        public T_Y,
        public T_Z
    {
    public:
        _V_PROP_( X )
        _V_PROP_( Y )
        _V_PROP_( Z )

    public:
        inline void UpdateValue()
        {
            C_OWNER.UpdateValue();
        }

    public:
        inline void GetValue( Mitov::TQuaternion & AValue )
        {
            AValue.SetX( X().GetValue());
            AValue.SetY( Y().GetValue());
            AValue.SetZ( Z().GetValue());
        }

		inline void ApplyValues( Mitov::TQuaternion * ABuffer ) {}

    };
//---------------------------------------------------------------------------
    template <
        typename T_OWNER, T_OWNER &C_OWNER,
        uint32_t C_INDEX,
        typename T_Imaginary_ApplyValues,
        typename T_Imaginary_GetValue,
        typename T_Real
    > class TArduinoBasicQuaternionValueElement :
        public T_Real
    {
    public:
        _V_PROP_( Real )

    public:
        inline void UpdateValue()
        {
            C_OWNER.SetIndexedValue( C_INDEX );
        }

    public:
        inline void GetValue( Mitov::TQuaternion & AValue )
        {
            AValue.SetReal( Real().GetValue());
            T_Imaginary_GetValue::Call( AValue );
//            AValue.
        }

        inline void ApplyValues( Mitov::TQuaternion * ABuffer )
        {
        }

    };
//---------------------------------------------------------------------------
/*
	class TAudioBuffer
	{
	public:
		Mitov::String ToString( uint32_t AMaxSize = 32 )
		{
		}

	public:
		TAudioBuffer( uint32_t ASize, const uint8_t *AData ) :
			Size( ASize),
			Data( (T *)AData )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_RollOver,
		typename T_Value
	> class GenericValueLimit :
		public T_RollOver,
		public T_Value
	{
	public:
		_V_PROP_( RollOver )
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class EmptyProperty
	{
	public:
		inline EmptyProperty &GetInstance() { return *this; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class PointerVariable
    {
    protected:
        void *FData = nullptr;

	public:
		inline PointerVariable &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( void * AValue ) { FData = AValue; }
		inline operator void *() const { return FData; }
		inline void * GetValue() const { return FData; }
//		inline T_TYPE &GetReference() const { return T_NESTED::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

    };
//---------------------------------------------------------------------------
    template <uint32_t C_UNIQUE> class NullPointer
    {
	public:
		inline NullPointer &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( void * AValue ) {}
		inline operator void *() const { return nullptr; }
		inline void * GetValue() const { return nullptr; }
//		inline T_TYPE &GetReference() const { return T_NESTED::GetReference(); }
		static inline constexpr bool GetIsConstant() { return true; }

    };
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_TYPE, typename T_NESTED> class TypedVariable : public T_NESTED
	{
	public:
		inline TypedVariable &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T_TYPE AValue ) { T_NESTED::SetValue( AValue ); }
		inline operator T_TYPE() const { return T_NESTED::GetValue(); }
		inline T_TYPE GetValue() const { return T_NESTED::GetValue(); }
//		inline T_TYPE &GetReference() const { return T_NESTED::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_NESTED::SetValue( *( T_TYPE* )_Data );
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_TYPE, typename T_NESTED, T_TYPE C_INITIAL_VALUE> class TypedVariableValue : public T_NESTED
	{
	public:
		inline TypedVariableValue &GetInstance() { return *this; }
		inline void operator = ( T_TYPE AValue ) { T_NESTED::SetValue( AValue ); }
		inline operator T_TYPE() const { return T_NESTED::GetValue(); }
		inline T_TYPE GetValue() const { return T_NESTED::GetValue(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_NESTED::SetValue( *( T_TYPE* )_Data );
		}

	public:
		TypedVariableValue()
		{
			T_NESTED::SetValue( C_INITIAL_VALUE );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_TYPE, typename T_NESTED, T_TYPE C_INITIAL_VALUE, T_TYPE C_MAX> class TypedVariableValueMax : public T_NESTED
	{
	public:
		inline TypedVariableValueMax &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T_TYPE AValue )
		{
			if( AValue > C_MAX )
				AValue = C_MAX;

			T_NESTED::SetValue( AValue );
		}

		inline operator T_TYPE() const { return T_NESTED::GetValue(); }
		inline T_TYPE GetValue() const { return T_NESTED::GetValue(); }
//		inline T_TYPE &GetReference() const { return T_NESTED::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_NESTED::SetValue( *( T_TYPE* )_Data );
		}

	public:
		inline TypedVariableValueMax()
		{
			T_NESTED::SetValue( C_INITIAL_VALUE );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_TYPE, typename T_NESTED, T_TYPE C_MIN, T_TYPE C_MAX> class TypedVariableMinMax : public T_NESTED
    {
	public:
		inline TypedVariableMinMax &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T_TYPE AValue )
		{
			if( AValue < C_MIN )
				AValue = C_MIN;

			else if( AValue > C_MAX )
				AValue = C_MAX;

			T_NESTED::SetValue( AValue - C_MIN );
		}

		inline operator T_TYPE() const { return T_TYPE( T_NESTED::GetValue() ) + C_MIN; }
		inline T_TYPE GetValue() const { return T_TYPE( T_NESTED::GetValue() ) + C_MIN; }
//		inline T_TYPE &GetReference() const { return T_NESTED::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_NESTED::SetValue( *( T_TYPE* )_Data );
		}
/*
	public:
		inline TypedVariableMinMax()
		{
			T_NESTED::SetValue( C_INITIAL_VALUE - C_MIN );
		}
*/
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_NESTED> class NestedProperty : public T_NESTED
	{
	public:
		inline NestedProperty &GetInstance() { return *this; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE> class VariableProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
		inline VariableProperty &GetInstance() { return *this; }
		inline void operator = ( T AValue ) { FValue = AValue; }
		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FValue = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, typename T_SHARED_VARIABLE> class SharedVariableProperty
	{
	public:
		typedef T T_TYPE;

	public:
		inline SharedVariableProperty &GetInstance() { return *this; }
		inline void operator = ( T AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator T() const { return T_SHARED_VARIABLE::GetVariableValue( C_VALUE ); }
		inline T GetValue() const { return T_SHARED_VARIABLE::GetVariableValue( C_VALUE ); }
		inline T &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_SHARED_VARIABLE::SetVariableValue( *( T* )_Data );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, T C_MIN> class VariableMinProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
		inline VariableMinProperty &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			if( AValue < C_MIN )
				AValue = C_MIN;

			FValue = AValue;
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, typename T_NESTED, T C_MIN> class VariableMinPropertySharedBits
	{
	public:
		typedef T T_TYPE;

	public:
		inline VariableMinPropertySharedBits &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{

			if( AValue < C_MIN )
				AValue = C_MIN;

			T_NESTED::SetValue( AValue - C_MIN );
		}

		inline operator T() const { return T_NESTED::GetValue() + C_MIN; }
		inline T GetValue() const { return T_NESTED::GetValue() + C_MIN; }
//		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}

	public:
		inline VariableMinPropertySharedBits()
		{
			T_NESTED::SetValue( C_VALUE );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, typename T_NESTED, T C_MIN> class SharedVariableMinPropertySharedBits
	{
	public:
		typedef T T_TYPE;

	public:
		inline SharedVariableMinPropertySharedBits &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			T_NESTED::SetVariableValue( AValue );
		}

		inline operator T() const { return T_NESTED::GetVariableValue( C_VALUE ); }
		inline T GetValue() const
        {
            T AValue = T_NESTED::GetVariableValue( C_VALUE );
			if( AValue < C_MIN )
				AValue = C_MIN;

            return AValue;
        }
//		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, T C_MAX> class VariableMaxProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
		inline VariableMaxProperty &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			if( AValue > C_MAX )
				AValue = C_MAX;

			FValue = AValue;
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, typename T_NESTED, T C_MAX> class VariableMaxPropertySharedBits
	{
	public:
		typedef T T_TYPE;

	public:
		inline VariableMaxPropertySharedBits &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			if( AValue > C_MAX )
				AValue = C_MAX;

			T_NESTED::SetValue( AValue );
		}

		inline operator T() const { return T_NESTED::GetValue(); }
		inline T GetValue() const { return T_NESTED::GetValue(); }
//		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}

	public:
		inline VariableMaxPropertySharedBits()
		{
			T_NESTED::SetValue( C_VALUE );
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, typename T_NESTED, T C_MAX> class SharedVariableMaxPropertySharedBits
	{
	public:
		typedef T T_TYPE;

	public:
		inline SharedVariableMaxPropertySharedBits &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			T_NESTED::SetVariableValue( AValue );
		}

		inline operator T() const { return T_NESTED::GetVariableValue( C_VALUE ); }
		inline T GetValue() const
        {
            T AValue = T_NESTED::GetVariableValue( C_VALUE );
			if( AValue > C_MAX )
				AValue = C_MAX;

            return AValue;
        }
//		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, T C_MIN, T C_MAX> class VariableMinMaxProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
		inline VariableMinMaxProperty &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			if( AValue < C_MIN )
				AValue = C_MIN;

			else if( AValue > C_MAX )
				AValue = C_MAX;

			FValue = AValue;
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, typename T_NESTED, T C_MIN, T C_MAX> class VariableMinMaxPropertySharedBits
	{
	public:
		typedef T T_TYPE;

	public:
		inline VariableMinMaxPropertySharedBits &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
//            Serial.print( AValue );
//            Serial.print( " * " );

			if( AValue < C_MIN )
				AValue = C_MIN;

			else if( AValue > C_MAX )
				AValue = C_MAX;

//            Serial.print( C_MIN );
//            Serial.print( " - " );
//            Serial.print( C_MAX );
//            Serial.print( " - " );
//            Serial.print( AValue );
//            Serial.print( " = " );

			T_NESTED::SetValue( AValue - C_MIN );
//            Serial.println( T_NESTED::GetValue() );
		}

		inline operator T() const { return T_NESTED::GetValue() + C_MIN; }
		inline T GetValue() const { return T_NESTED::GetValue() + C_MIN; }
//		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}

	public:
		inline VariableMinMaxPropertySharedBits()
		{
			T_NESTED::SetValue( C_VALUE );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE, typename T_NESTED, T C_MIN, T C_MAX> class SharedVariableMinMaxPropertySharedBits
	{
	public:
		typedef T T_TYPE;

	public:
		inline SharedVariableMinMaxPropertySharedBits &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			T_NESTED::SetVariableValue( AValue );
		}

		inline operator T() const { return T_NESTED::GetVariableValue( C_VALUE ); }
		inline T GetValue() const
        {
            T AValue = T_NESTED::GetVariableValue( C_VALUE );
			if( AValue < C_MIN )
				AValue = C_MIN;

			else if( AValue > C_MAX )
				AValue = C_MAX;

            return AValue;
        }
//		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T C_VALUE> class ConstantProperty
	{
	public:
		inline ConstantProperty &GetInstance() { return *this; }
		inline void operator = ( T AValue ) {}
		inline operator T() const { return C_VALUE; }
		inline T GetValue() const { return C_VALUE; }
		static inline constexpr bool GetIsConstant() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE> class VariablePropertyFloat
	{
	protected:
		T	FValue = T_VALUE::GetValue();

	public:
		inline VariablePropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue ) { FValue = AValue; }
		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_SHARED_VARIABLE> class SharedVariablePropertyFloat
	{
	public:
		inline SharedVariablePropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator T() const { return T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() ); }
		inline T GetValue() const { return T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() ); }
		inline T &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class VariableFieldFloat
	{
//	public:
//		typedef T T_TYPE;

	protected:
		float	FValue;

	public:
//		inline void GetInstance( VariablePropertyFloat * &AInstance ) { AInstance = this; }
		inline VariableFieldFloat &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( float AValue ) { FValue = AValue; }
		inline operator float() const { return FValue; }
		inline float GetValue() const { return FValue; }
		inline float &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

//	public:
//		VariableProperty() {}

//		VariablePropertyFloat( T AValue ) :
//			FValue( AValue )
//		{
//		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MIN> class VariableMinPropertyFloat
	{
	protected:
		T	FValue = T_VALUE::GetValue();

	public:
		inline VariableMinPropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			if( AValue < T_MIN::GetValue() )
				AValue = T_MIN::GetValue();

			FValue = AValue;
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MIN, typename T_SHARED_VARIABLE> class SharedVariableMinPropertyFloat
	{
	public:
		inline SharedVariableMinPropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator T() const { return GetValue(); }
		inline T GetValue() const
        {
            T AValue = T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() );

			if( AValue < T_MIN::GetValue() )
				return T_MIN::GetValue();

            return AValue;
        }

		inline T &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MAX> class VariableMaxPropertyFloat
	{
	protected:
		T	FValue = T_VALUE::GetValue();

	public:
		inline VariableMaxPropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			if( AValue > T_MAX::GetValue() )
				AValue = T_MAX::GetValue();

			FValue = AValue;
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MAX, typename T_SHARED_VARIABLE> class SharedVariableMaxPropertyFloat
	{
	public:
		inline SharedVariableMaxPropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator T() const { return GetValue(); }
		inline T GetValue() const
        {
            T AValue = T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() );

			if( AValue > T_MAX::GetValue() )
				return T_MAX::GetValue();

            return AValue;
        }

		inline T &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MIN, typename T_MAX> class VariableMinMaxPropertyFloat
	{
	protected:
		T	FValue = T_VALUE::GetValue();

	public:
		inline VariableMinMaxPropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue )
		{
			if( AValue < T_MIN::GetValue() )
				AValue = T_MIN::GetValue();

			else if( AValue > T_MAX::GetValue() )
				AValue = T_MAX::GetValue();

			FValue = AValue;
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MIN, typename T_MAX, typename T_SHARED_VARIABLE> class SharedVariableMinMaxPropertyFloat
	{
	public:
		inline SharedVariableMinMaxPropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator T() const { return GetValue(); }
		inline T GetValue() const
        {
            T AValue = T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() );

			if( AValue < T_MIN::GetValue() )
				return T_MIN::GetValue();

			if( AValue > T_MAX::GetValue() )
				return T_MAX::GetValue();

            return AValue;
        }

		inline T &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_VALUE> class VariableFieldFloatValue
	{
	protected:
		float	FValue = T_VALUE::GetValue();

	public:
		inline VariableFieldFloatValue &GetInstance() { return *this; }
		inline void operator = ( float AValue ) { FValue = AValue; }
		inline operator float() const { return FValue; }
		inline float GetValue() const { return FValue; }
		inline float &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, typename C_VALUE> class ConstantPropertyFloat
	{
	public:
		inline ConstantPropertyFloat &GetInstance() { return *this; }
		inline void operator = ( T AValue ) {}
		inline operator T() const { return C_VALUE::GetValue(); }
		inline T GetValue() const { return C_VALUE::GetValue(); }
		static inline constexpr bool GetIsConstant() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_VALUE> class VariablePropertyString
	{
	protected:
		Mitov::String	FValue;

	public:
		inline VariablePropertyString &GetInstance() { return *this; }
		inline void operator = ( const char *AValue ) { FValue = AValue; }
		inline void operator = ( Mitov::String AValue ) { FValue = AValue; }
		inline operator Mitov::String () const { return FValue; }
		inline const char *c_str() const { return FValue.c_str(); }
		inline Mitov::String GetValue() const { return FValue; }
		inline Mitov::String &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = ( char * )_Data;
		}

	public:
		inline VariablePropertyString()
		{
			FValue = T_VALUE::GetValue();
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_VALUE, typename T_SHARED_VARIABLE> class SharedVariablePropertyString
	{
	public:
		inline SharedVariablePropertyString &GetInstance() { return *this; }
		inline void operator = ( const char *AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline void operator = ( Mitov::String AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator Mitov::String () const { return T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() ); }
		inline const char *c_str() const { return T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() ).c_str(); }
		inline Mitov::String GetValue() const { return T_SHARED_VARIABLE::GetVariableValue( T_VALUE::GetValue() ); }
		inline Mitov::String &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_SHARED_VARIABLE::SetVariableValue( ( char * )_Data );
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_VALUE> class ConstantPropertyString
	{
	public:
		inline ConstantPropertyString &GetInstance() { return *this; }
		inline void operator = ( const char *AValue ) {}
		inline void operator = ( Mitov::String AValue ) {}
		inline bool operator == ( const char * AOther ) { return strcmp( T_VALUE::GetValue(), AOther ) == 0; }
		inline bool operator != ( const char * AOther ) { return strcmp( T_VALUE::GetValue(), AOther ) != 0; }
		inline operator Mitov::String () const { return T_VALUE::GetValue(); }
		inline const char *c_str() const { return T_VALUE::GetValue(); }
		inline Mitov::String GetValue() const { return T_VALUE::GetValue(); }
		static inline constexpr bool GetIsConstant() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_COLOR, typename T_CONST, T_CONST C_VALUE> class VariablePropertyColor
	{
	protected:
		T_COLOR	FValue;

	public:
		inline VariablePropertyColor &GetInstance() { return *this; }
		inline void operator = ( T_COLOR AValue ) { FValue = AValue; }
		inline void operator = ( uint32_t AValue ) { FValue = AValue; }
		inline operator T_COLOR () const { return FValue; }
		inline long GetValue() const { return FValue; }
		inline T_COLOR &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T_COLOR* )_Data;
		}

	public:
		inline VariablePropertyColor() :
			FValue( C_VALUE, true )
		{
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_COLOR, typename T_CONST, T_CONST C_VALUE, typename T_SHARED_VARIABLE> class SharedVariablePropertyColor
	{
	public:
		inline SharedVariablePropertyColor &GetInstance() { return *this; }
		inline void operator = ( T_COLOR AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline void operator = ( uint32_t AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator T_COLOR () const { return T_SHARED_VARIABLE::GetVariableValue( C_VALUE ); }
		inline long GetValue() const { return T_SHARED_VARIABLE::GetVariableValue( C_VALUE ); }
		inline T_COLOR &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_SHARED_VARIABLE::SetVariableValue( *( T_COLOR* )_Data );
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_COLOR, typename T_CONST, T_CONST C_VALUE> class ConstantPropertyColor
	{
	public:
		inline ConstantPropertyColor &GetInstance() { return *this; }
		inline void operator = ( T_COLOR AValue ) {}
		inline operator T_COLOR () const { return T_COLOR( C_VALUE, true ); }
		inline T_COLOR GetValue() const { return T_COLOR( C_VALUE, true ); }
		inline T_COLOR GetReference() { return T_COLOR( C_VALUE, true ); }
		static inline constexpr bool GetIsConstant() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, int32_t C_DATE, int32_t C_TIME> class VariablePropertyDateTime
	{
	protected:
		TDateTime	FValue;

	public:
		inline VariablePropertyDateTime &GetInstance() { return *this; }
		inline void operator = ( TDateTime AValue ) { FValue = AValue; }
		inline operator TDateTime () const { return FValue; }
		inline TDateTime GetValue() const { return FValue; }
		inline TDateTime &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( TDateTime* )_Data;
		}

	public:
		VariablePropertyDateTime () :
			FValue( C_DATE, C_TIME )
		{
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, int32_t C_DATE, int32_t C_TIME, typename T_SHARED_VARIABLE> class SharedVariablePropertyDateTime
	{
	public:
		inline SharedVariablePropertyDateTime &GetInstance() { return *this; }
		inline void operator = ( TDateTime AValue ) { T_SHARED_VARIABLE::SetVariableValue( AValue ); }
		inline operator TDateTime() const { return T_SHARED_VARIABLE::GetVariableValue( C_DATE, C_TIME ); }
		inline TDateTime GetValue() const { return T_SHARED_VARIABLE::GetVariableValue( C_DATE, C_TIME ); }
		inline TDateTime &GetReference() { return T_SHARED_VARIABLE::GetReference(); }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_SHARED_VARIABLE::SetVariableValue( *( TDateTime* )_Data );
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, int32_t C_DATE, int32_t C_TIME> class ConstantPropertyDateTime
	{
	public:
		inline ConstantPropertyDateTime &GetInstance() { return *this; }
		inline void operator = ( TDateTime AValue ) {}
		inline operator TDateTime() const { return TDateTime( C_DATE, C_TIME ); }
		inline TDateTime GetValue() const { return TDateTime( C_DATE, C_TIME ); }
		static inline constexpr bool GetIsConstant() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_REAL, typename T_IMAGINARY> class VariablePropertyComplex
	{
	protected:
		TComplex	FValue;

	public:
		inline VariablePropertyComplex &GetInstance() { return *this; }
		inline void operator = ( TComplex AValue ) { FValue = AValue; }
		inline operator TComplex () const { return FValue; }
		inline TComplex GetValue() const { return FValue; }
		inline TComplex &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( TComplex* )_Data;
		}

	public:
		VariablePropertyComplex () :
			FValue( T_REAL::GetValue(), T_IMAGINARY::GetValue() )
		{
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_REAL, typename T_IMAGINARY> class ConstantPropertyComplex
	{
	public:
		inline ConstantPropertyComplex &GetInstance() { return *this; }
		inline void operator = ( TComplex AValue ) {}
		inline operator TComplex () const { return TComplex( T_REAL::GetValue(), T_IMAGINARY::GetValue() ); }
		inline TComplex GetValue() const { return TComplex( T_REAL::GetValue(), T_IMAGINARY::GetValue() ); }
		static inline constexpr bool GetIsConstant() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_REAL, typename T_IMAGINARY_X, typename T_IMAGINARY_Y, typename T_IMAGINARY_Z> class VariablePropertyQuaternion
	{
	protected:
		TQuaternion	FValue;

	public:
		inline VariablePropertyQuaternion &GetInstance() { return *this; }
		inline void operator = ( TQuaternion AValue ) { FValue = AValue; }
		inline operator TQuaternion () const { return FValue; }
		inline TQuaternion GetValue() const { return FValue; }
		inline TQuaternion &GetReference() { return FValue; }
		static inline constexpr bool GetIsConstant() { return false; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( TQuaternion* )_Data;
		}

	public:
		VariablePropertyQuaternion () :
			FValue( T_IMAGINARY_X::GetValue(), T_IMAGINARY_Y::GetValue(), T_IMAGINARY_Z::GetValue(), T_REAL::GetValue() )
		{
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_REAL, typename T_IMAGINARY_X, typename T_IMAGINARY_Y, typename T_IMAGINARY_Z> class ConstantPropertyQuaternion
	{
	public:
		inline ConstantPropertyQuaternion &GetInstance() { return *this; }
		inline void operator = ( TQuaternion AValue ) {}
		inline operator TQuaternion () const { return TQuaternion( T_IMAGINARY_X::GetValue(), T_IMAGINARY_Y::GetValue(), T_IMAGINARY_Z::GetValue(), T_REAL::GetValue() ); }
		inline TQuaternion GetValue() const { return TQuaternion( T_IMAGINARY_X::GetValue(), T_IMAGINARY_Y::GetValue(), T_IMAGINARY_Z::GetValue(), T_REAL::GetValue() ); }
		static inline constexpr bool GetIsConstant() { return true; }

	};
//---------------------------------------------------------------------------
    template <typename T> inline Mitov::String ValueToString( const T &AValue ) { return Mitov::String( AValue, uint8_t( 10 ) ); }
//---------------------------------------------------------------------------
    template<> inline Mitov::String ValueToString( const Mitov::TComplex &AValue ) { return AValue.ToString(); }
//---------------------------------------------------------------------------
	template <typename T> struct TArray
	{
	public:
		uint32_t	Size;
		T *Data;

	public:
		bool operator == ( const TArray &other )
		{
			if( Size != other.Size )
				return false;

			return( memcmp( Data, other.Data, Size * sizeof( T ) ) == 0 );
		}

		bool operator != ( const TArray &other )
		{
			if( Size != other.Size )
				return true;

			return( memcmp( Data, other.Data, Size * sizeof( T ) ) != 0 );
		}

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString( uint32_t AMaxSize = 32 ) const
		{
//			Serial.println( "ToString" );
			Mitov::String AResult = "(";
			for( uint32_t i = 0; i < MitovMin( Size, uint32_t( AMaxSize ) ); ++i )
			{
				if( i < Size - 1 )
					AResult += ValueToString<T>( Data[ i ] ) + ",";

				else
					AResult += ValueToString<T>( Data[ i ] );

			}

			if( Size > AMaxSize )
				AResult += "...";

			return	AResult + ")";
		}

	public:
		TArray( uint32_t ASize, const T *AData ) :
			Size( ASize),
			Data( (T *)AData )
		{
		}

		TArray( const T &AData ) :
			Size( 1 ),
			Data( (T *)&AData )
		{
		}

	};
//---------------------------------------------------------------------------
	struct TDataBlock : public TArray<uint8_t>
	{
		typedef TArray<uint8_t> inherited;

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		inline Mitov::String ToString( uint32_t AMaxSize = 32 ) const
		{
			return inherited::ToString( AMaxSize );
		}

		Mitov::String ToString( uint32_t AMaxSize, uint8_t ABase ) const
		{
//			Serial.println( "ToString" );
			Mitov::String AResult = "(";
			for( uint32_t i = 0; i < MitovMin( Size, uint32_t( AMaxSize ) ); ++i )
			{
				if( i < Size - 1 )
					AResult += Mitov::String( Data[ i ], ABase ) + ",";

				else
					AResult += Mitov::String( Data[ i ], ABase );

			}

			if( Size > AMaxSize )
				AResult += "...";

			return	AResult + ")";
		}

		Mitov::String ToString( uint32_t AMaxSize, uint8_t ABase, Mitov::String ABegin, Mitov::String AEnd, Mitov::String ASepator, Mitov::String AElementPrefix ) const
		{
//			Serial.println( "ToString" );
			for( uint32_t i = 0; i < MitovMin( Size, uint32_t( AMaxSize ) ); ++i )
			{
				if( i < Size - 1 )
					ABegin += AElementPrefix + Mitov::String( Data[ i ], ABase ) + ASepator;

				else
					ABegin += AElementPrefix + Mitov::String( Data[ i ], ABase );

			}

			if( Size > AMaxSize )
				ABegin += "...";

			return	ABegin + AEnd;
		}

	public:
		TDataBlock( uint32_t ASize, const void *AData ) :
			inherited( ASize, (uint8_t *)AData )
		{
		}

		TDataBlock() :
			inherited( 0, nullptr )
		{
		}

	};
//---------------------------------------------------------------------------
	struct TCanBusPacket : public TArray<uint8_t>
	{
		typedef TArray<uint8_t> inherited;

	public:
		inline bool IsRequest() const { return Data[ 4 ] & 0b10; }
		inline bool IsExtended() const { return Data[ 4 ] & 0b01; }
		inline uint32_t	GetID() const { return *(uint32_t *)Data; }
		inline uint8_t *GetBuffer() const { return Data + sizeof( uint32_t ) + 1; }
		inline uint8_t GetBufferSize() const { return Size - ( sizeof( uint32_t ) + 1 ); }

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString() const
		{
//			Serial.println( "ToString" );
			Mitov::String AResult = "(id:";
			AResult += Mitov::String( *(uint32_t *)Data, HEX ) + " ";
			if( IsRequest() )
				AResult += "R";

			if( IsExtended() )
				AResult += "X";

			AResult += ",";
			for( uint32_t i = 5; i < Size; ++i )
			{
				if( i < Size - 1 )
					AResult += Mitov::String( Data[ i ], uint8_t( HEX )) + ",";

				else
					AResult += Mitov::String( Data[ i ], uint8_t( HEX ));

			}

			return	AResult + ")";
		}

	public:
		TCanBusPacket( uint32_t ASize, const void *AData ) :
			inherited( ASize, (uint8_t *)AData )
		{
		}

		TCanBusPacket() :
			inherited( 0, nullptr )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T> struct TValueArray : public TArray<T>
	{
		typedef TArray<T> inherited;

	public:
		TValueArray<T> & operator = (const TArray<T>& other)
		{
//            Serial.println( "TEST1" );
			if( inherited::Data )
				delete [] inherited::Data;

			inherited::Size = other.Size;
			inherited::Data = new T[ inherited::Size ];

			for( int i = 0; i < inherited::Size; ++ i )
				inherited::Data[ i ] = other.Data[ i ];

/*
			for( int i = 0; i < inherited::Size; ++ i )
            {
                Serial.print( inherited::Data[ i ] );
                Serial.print( "," );
            }
*/
			return *this;
		}

		TValueArray<T> & operator = (const TValueArray<T>& other)
		{
			if( inherited::Data )
				delete [] inherited::Data;

			inherited::Size = other.Size;
			inherited::Data = new T[ inherited::Size ];

			for( int i = 0; i < inherited::Size; ++ i )
				inherited::Data[ i ] = other.Data[ i ];

			return *this;
		}

	public:
		void Assign( const T *AData, uint32_t ASize )
		{
			delete [] inherited::Data;
			inherited::Data = new T[ ASize ];

			inherited::Size = ASize;

			for( int i = 0; i < ASize; ++i )
				inherited::Data[ i ] = AData[ i ];
		}

		void Copy( const T *AData, uint32_t ASize, uint32_t AOffset )
		{
			for( int i = 0; i < MitovMin( ASize, inherited::Size - AOffset ); ++i )
				inherited::Data[ i + AOffset ] = AData[ i ];

		}

		void ResizeFront( uint32_t ASize )
		{
			if( ASize == inherited::Size )
				return;

			T *AOldData = inherited::Data;
			inherited::Data = new T[ ASize ];

			if( ASize > inherited::Size )
			{
				uint32_t AOffset = ASize - inherited::Size;
				for( int i = 0; i < inherited::Size; ++i )
					inherited::Data[ i + AOffset ] = AOldData[ i ];
			}

			else
			{
				uint32_t AOffset = inherited::Size - ASize;
				for( int i = 0; i < inherited::Size; ++i )
					inherited::Data[ i ] = AOldData[ i + AOffset ];
			}

			delete [] AOldData;

			inherited::Size = ASize;
		}

		void ResizeBack( uint32_t ASize, bool APreserveData )
		{
			if( ASize == inherited::Size )
				return;

			T *AOldData = inherited::Data;
			inherited::Data = new T[ ASize ];

			for( int i = 0; i < MitovMin( ASize, inherited::Size ); ++i )
				inherited::Data[ i ] = AOldData[ i ];

			delete [] AOldData;

			inherited::Size = ASize;
		}

		void Resize( uint32_t ASize )
		{
			if( ASize == inherited::Size )
				return;

            if( inherited::Data )
    			delete [] inherited::Data;

			inherited::Data = new T[ ASize ];

			inherited::Size = ASize;
		}

	public:
		TValueArray( T AData, bool ) :
			inherited( 1, new T[ 1 ] )

		{
			*inherited::Data = AData;
		}

		TValueArray( bool, uint32_t ASize ) :
			inherited( ASize, new T[ ASize ] )
		{
		}

		TValueArray( uint32_t ASize, const T *AData ) :
			inherited( ASize, new T[ ASize ] )
		{
			for( int i = 0; i < ASize; ++ i )
				inherited::Data[ i ] = AData[ i ];
		}

		TValueArray( const TArray<T> &AOther ) :
			TValueArray( AOther.Size, AOther.Data )
		{
		}

		TValueArray( const TValueArray<T> &AOther ) :
			TValueArray( AOther.Size, new T[ AOther.Size ] )
		{
			for( int i = 0; i < AOther.Size; ++ i )
				inherited::Data[ i ] = AOther.Data[ i ];
		}

		TValueArray() :
			inherited( 0, nullptr )
		{
		}

		~TValueArray()
		{
//            Serial.println( "~TValueArray" );
			if( inherited::Data )
				delete [] inherited::Data;

            inherited::Data = nullptr;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA, typename T_STORAGE> class NullPropertyArray
	{
	public:
		typedef T_DATA          T_TYPE;
        typedef TArray<T_DATA>  T_ARRAY;

//	public:
//		inline constexpr T *GetInstance() { return nullptr; }

	public:
		inline static constexpr bool GetIsConstant() { return true; }
		inline static constexpr uint32_t GetCount() { return 0; }
        inline static constexpr T_DATA *GetArray() { return nullptr; }
		static constexpr const T_DATA *GetValue() { return nullptr; }
        inline static void SetValue( T_DATA *AData, uint32_t ASize ) {}
        inline static void CopyData( T_DATA *ADestination ) {}
        inline static void CopyData( T_DATA *ADestination, uint32_t ASize )
        {
            memset( ADestination, 0, ASize * sizeof( T_DATA ) );
        }

        inline static T_ARRAY *GetDataArray() { return new TArray<T_DATA>( 0, nullptr ); }
        inline static T_ARRAY *GetMofifyDataArray() { return new TArray<T_DATA>( 0, nullptr ); }
        inline static T_TYPE *Allocate() { return nullptr; }
        inline static void Release( const T_TYPE *ABuffer ) {}
//        inline static T_ARRAY GetDataArrayLocal() { return new TArray<T_DATA>( 0, nullptr ); }

        inline static int Compare( T_TYPE *AData ) { return 0; }
        inline static uint8_t GetByteAt( uint32_t AIndex ) { return 0; }

        template <typename T_PIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            TArray<T_DATA> ABuffer( 0, nullptr );
            APin.SetPinValue( ABuffer, AChangeOnly );
        }

        template <typename T_PIN, typename T_CALL_CHAIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            TArray<T_DATA> ABuffer( 0, nullptr );
            APin.SetPinValue( ABuffer, AChangeOnly );
        }

	public:
		inline NullPropertyArray &GetInstance() { return *this; }

	public:
		inline static constexpr const T_DATA GetValue( uint32_t AIndex ) { return *(T_DATA *)nullptr; }
        inline static void SetValue( uint32_t AIndex, T_DATA AValue ) {}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA, typename T_STORAGE, T_STORAGE const *C_VALUE, uint32_t C_COUNT> class ConstantPropertyArray
	{
	public:
		typedef T_DATA              T_TYPE;
        typedef TValueArray<T_DATA> T_ARRAY;

//	public:
//		inline constexpr T *GetInstance() { return C_VALUE; }

	public:
		inline static constexpr bool GetIsConstant() { return true; }
		inline static constexpr uint32_t GetCount() { return C_COUNT; }
        inline static constexpr T_DATA *GetArray() { return nullptr; }
        inline static constexpr const T_DATA *GetValue() { return (T_DATA *)C_VALUE; }
        inline static void SetValue( T_DATA *AData, uint32_t ASize ) {}
//		inline static constexpr const T_DATA *GetValue() { return (T_DATA *)pgm_read_ptr( C_VALUE ); }
        inline static void CopyData( T_DATA *ADestination )
        {
            memcpy_P( ADestination, C_VALUE, C_COUNT * sizeof( T_DATA ) );
        }

        inline static void CopyData( T_DATA *ADestination, uint32_t ASize )
        {
            memcpy_P( ADestination, C_VALUE, MitovMin( C_COUNT, ASize ) * sizeof( T_DATA ) );
            if( C_COUNT >= ASize )
                return;

            memset( ADestination + C_COUNT, 0, ( ASize - C_COUNT ) * sizeof( T_DATA ) );
        }

        inline static T_ARRAY *GetDataArray()
        {
            TValueArray<T_DATA> *ABuffer = new TValueArray<T_DATA>( true, C_COUNT );
            CopyData( ABuffer->Data );
            return ABuffer;
        }

        inline static T_ARRAY *GetMofifyDataArray()
        {
            TValueArray<T_DATA> *ABuffer = new TValueArray<T_DATA>( true, C_COUNT );
            CopyData( ABuffer->Data );
            return ABuffer;
        }

        inline static T_TYPE *Allocate()
        {
            T_TYPE *ABuffer = new T_TYPE[ C_COUNT ];
            CopyData( ABuffer );
            return ABuffer;
        }

        inline static void Release( const T_TYPE *ABuffer ) { delete [] ABuffer; }

        inline static int Compare( T_TYPE *AData )
        {
            return memcmp_P( AData, C_VALUE, C_COUNT * sizeof( T_DATA ) );
        }

        inline static uint8_t GetByteAt( uint32_t AIndex )
        {
            return pgm_read_byte( C_VALUE + AIndex );
        }

        template <typename T_PIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            TValueArray<T_DATA> ABuffer( true, GetCount() );
            CopyData( ABuffer.Data );
            APin.SetPinValue( ABuffer, AChangeOnly );
        }

        template <typename T_PIN, typename T_CALL_CHAIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            TValueArray<T_DATA> ABuffer( true, GetCount() );
            CopyData( ABuffer.Data );
            T_CALL_CHAIN::Call( ABuffer.Data );
            APin.SetPinValue( ABuffer, AChangeOnly );
        }
/*
        inline static T_ARRAY GetDataArrayLocal()
        {
            TValueArray<T_DATA> ABuffer( C_COUNT );
            CopyData( ABuffer.Data );
            return ABuffer;
        }
*/
	public:
		inline ConstantPropertyArray &GetInstance() { return *this; }

	public:
//		inline T_DATA operator[] (uint32_t AIndex) const { return *((T_DATA *)pgm_read_ptr( C_VALUE + AIndex )); }
		inline static constexpr const T_DATA GetValue( uint32_t AIndex ) { return Mitov::PROGMEM_getAnything<T_DATA>( ((T_DATA *)C_VALUE ) + AIndex ); }
        inline static void SetValue( uint32_t AIndex, T_DATA AValue ) {}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA, typename T_STORAGE, T_STORAGE const *C_VALUE, uint32_t C_COUNT> class VariablePropertyArray
	{
	public:
		typedef T_DATA T_TYPE;
        typedef TArray<T_DATA>  T_ARRAY;

//	public:
//		inline constexpr T *GetInstance() { return C_VALUE; }

	public:
		inline static constexpr bool GetIsConstant() { return false; }
		inline static constexpr uint32_t GetCount() { return C_COUNT; }
        inline static constexpr T_DATA *GetArray() { return (T_DATA *)C_VALUE; }
		inline static constexpr T_DATA *GetValue() { return (T_DATA *)C_VALUE; }
        inline static void SetValue( T_DATA *AData, uint32_t ASize ) {}
        inline static void CopyData( T_DATA *ADestination )
        {
            memcpy( ADestination, C_VALUE, C_COUNT * sizeof( T_DATA ) );
        }

        inline static void CopyData( T_DATA *ADestination, uint32_t ASize )
        {
            memcpy( ADestination, C_VALUE, MitovMin( C_COUNT, ASize ) * sizeof( T_DATA ) );
        }

        inline static T_ARRAY *GetDataArray()
        {
            return new TArray<T_DATA>( C_COUNT, C_VALUE );
        }

        inline static T_ARRAY *GetMofifyDataArray()
        {
            TValueArray<T_DATA> *ABuffer = new TValueArray<T_DATA>( true, C_COUNT );
            CopyData( ABuffer->Data );
            return ABuffer;
        }

        inline static T_TYPE *Allocate() { return (T_DATA *)C_VALUE; }
        inline static void Release( const T_TYPE *ABuffer ) {}

        inline static int Compare( T_TYPE *AData )
        {
            return memcmp( AData, C_VALUE, C_COUNT * sizeof( T_DATA ) );
        }

        inline static uint8_t GetByteAt( uint32_t AIndex )
        {
            return C_VALUE[ AIndex ];
        }

        template <typename T_PIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            TArray<T_DATA> ABuffer( GetCount(), GetArray() );
	    	APin.SetPinValue( ABuffer, AChangeOnly );
        }

        template <typename T_PIN, typename T_CALL_CHAIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            if( T_CALL_CHAIN::Count() == 0 )
            {
                TArray<T_DATA> ABuffer( GetCount(), GetArray() );
	        	APin.SetPinValue( ABuffer, AChangeOnly );
            }

            else
            {
                TValueArray<T_DATA> ABuffer( true, GetCount() );
                CopyData( ABuffer.Data );
                T_CALL_CHAIN::Call( ABuffer.Data );
                APin.SetPinValue( ABuffer, AChangeOnly );
            }
        }
/*
        inline static T_ARRAY GetDataArrayLocal()
        {
            return TArray<T_DATA>( C_COUNT, C_VALUE );
        }
*/
//		inline operator T() const { return C_VALUE; }
	public:
		inline VariablePropertyArray &GetInstance() { return *this; }

	public:
		inline static constexpr const T_DATA GetValue( uint32_t AIndex ) { return ( (T_DATA *)C_VALUE )[ AIndex ]; }
        inline static void SetValue( uint32_t AIndex, T_DATA AValue ) { ( (T_DATA *)C_VALUE )[ AIndex ] = AValue; }

	};
//---------------------------------------------------------------------------
    template <uint32_t C_UNIQUE, typename T_DATA, typename T_STORAGE, T_STORAGE const *C_VALUE, uint32_t C_COUNT> class VariableLengthPropertyArray
	{
	public:
		typedef T_DATA T_TYPE;
        typedef TValueArray<T_DATA>  T_ARRAY;

    protected:
        static TValueArray<T_DATA> FData;

//	public:
//		inline constexpr T *GetInstance() { return C_VALUE; }

	public:
		inline static constexpr bool GetIsConstant() { return false; }
		inline static constexpr uint32_t GetCount() { return FData.Size; }
        inline static constexpr T_DATA *GetArray() { return (T_DATA *)C_VALUE; }
		inline static constexpr T_DATA *GetValue() { return (T_DATA *)C_VALUE; }

        inline static void SetValue( T_DATA *AData, uint32_t ASize )
        {
            FData.Assign( AData, ASize );
        }

        inline static void CopyData( T_DATA *ADestination )
        {
            memcpy( ADestination, C_VALUE, C_COUNT * sizeof( T_DATA ) );
        }

        inline static void CopyData( T_DATA *ADestination, uint32_t ASize )
        {
            memcpy( ADestination, C_VALUE, MitovMin( C_COUNT, ASize ) * sizeof( T_DATA ) );
        }

        inline static T_ARRAY *GetDataArray()
        {
            return new TArray<T_DATA>( C_COUNT, C_VALUE );
        }

        inline static T_ARRAY *GetMofifyDataArray()
        {
            TValueArray<T_DATA> *ABuffer = new TValueArray<T_DATA>( true, C_COUNT );
            CopyData( ABuffer->Data );
            return ABuffer;
        }

        inline static T_TYPE *Allocate() { return (T_DATA *)C_VALUE; }
        inline static void Release( const T_TYPE *ABuffer ) {}

        inline static int Compare( T_TYPE *AData )
        {
            return memcmp( AData, C_VALUE, C_COUNT * sizeof( T_DATA ) );
        }

        inline static uint8_t GetByteAt( uint32_t AIndex )
        {
            return C_VALUE[ AIndex ];
        }

        template <typename T_PIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            TArray<T_DATA> ABuffer( GetCount(), GetArray() );
	    	APin.SetPinValue( ABuffer, AChangeOnly );
        }

        template <typename T_PIN, typename T_CALL_CHAIN> inline static void PinSend( T_PIN &APin, bool AChangeOnly = false )
        {
            if( T_CALL_CHAIN::Count() == 0 )
            {
                TArray<T_DATA> ABuffer( GetCount(), GetArray() );
                APin.SetPinValue( ABuffer, AChangeOnly );
            }

            else
            {
                TValueArray<T_DATA> ABuffer( true, GetCount() );
                CopyData( ABuffer.Data );
                T_CALL_CHAIN::Call( ABuffer.Data );
                APin.SetPinValue( ABuffer, AChangeOnly );
            }

        }
/*
        inline static T_ARRAY GetDataArrayLocal()
        {
            return TArray<T_DATA>( C_COUNT, C_VALUE );
        }
*/
//		inline operator T() const { return C_VALUE; }
	public:
		inline VariableLengthPropertyArray &GetInstance() { return *this; }

	public:
		inline static constexpr const T_DATA GetValue( uint32_t AIndex ) { return ( (T_DATA *)C_VALUE )[ AIndex ]; }
        inline static void SetValue( uint32_t AIndex, T_DATA AValue ) { ( (T_DATA *)C_VALUE )[ AIndex ] = AValue; }

	};
//---------------------------------------------------------------------------
    template <
			uint32_t C_UNIQUE,
            typename T_DATA,
            typename T_STORAGE,
            T_STORAGE const *C_VALUE,
            uint32_t C_COUNT
	> TValueArray<T_DATA> VariableLengthPropertyArray<
			C_UNIQUE,
            T_DATA,
            T_STORAGE,
            C_VALUE,
            C_COUNT
		>::FData;
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, uint8_t const *C_VALUE, uint32_t C_COUNT> class VariablePropertyDigitalArray
	{
	public:
		typedef bool T_TYPE;

//	public:
//		inline constexpr T *GetInstance() { return C_VALUE; }

	public:
		class TByteAccess
		{
		protected:
			VariablePropertyDigitalArray    *FOwner;
			int					            FIndex;

		public:
			inline void operator = ( bool AValue )
			{
				if( AValue )
					FOwner->GetArray()[ FIndex / 8 ] |= 1 << ( FIndex & 0b111 );

				else
					FOwner->GetArray()[ FIndex / 8 ] &= ~( 1 << ( FIndex & 0b111 ));

			}

			inline bool operator == ( bool AValue )
			{
				return ( FOwner->GetArray()[ FIndex / 8 ] & ( 1 << ( FIndex & 0b111 )) == AValue );
			}

			inline bool operator != ( bool AValue )
			{
				return ( FOwner->GetArray()[ FIndex / 8 ] & ( 1 << ( FIndex & 0b111 )) != AValue );
			}

		public:
			inline TByteAccess( VariablePropertyDigitalArray *AOwner, int AIndex ) :
				FOwner( AOwner ),
				FIndex( AIndex )
			{
			}
		};

		inline TByteAccess operator [](int AIndex) { return TByteAccess( this, AIndex ); }

	public:
		inline static constexpr bool GetIsConstant() { return false; }
		inline static constexpr uint32_t GetCount() { return C_COUNT; }
		inline static constexpr uint8_t GetValue( int AIndex ) { return ((uint8_t *) C_VALUE )[ AIndex ]; }
		inline static constexpr uint8_t *GetArray() { return (uint8_t *) C_VALUE; }

//		inline operator T() const { return C_VALUE; }
	public:
		inline VariablePropertyDigitalArray &GetInstance() { return *this; }

//	public:
//		T_DATA &operator[] (uint32_t AIndex) const { return ( (T_DATA *)C_VALUE )[ AIndex ]; }

	};
//---------------------------------------------------------------------------
	namespace TImageFormat
	{
		enum TImageFormat { RGB24, RGBA32, YUY2, UYVY, YV12, IYUV, RGB565, RGB555, GrayScale, Gray16, Gray4 };

        const TImageFormat MaxFormat = Gray4;

		Mitov::String ToString( TImageFormat AValue )
		{
			switch( AValue )
			{
				case Gray4 : return "Gray 4";
				case GrayScale : return "Gray 8";
				case RGB24 : return "RGB 8";
				default : return "Unknown";
			}

			return "Unknown";
		}

		uint32_t GetSize( TImageFormat AValue, uint32_t AWidth, uint32_t AHeight )
		{
			switch( AValue )
			{
				case Gray4 : return (( AWidth * AHeight ) / 2 ) + 0.5;
				case GrayScale : return AWidth * AHeight;
				case RGB24 : return AWidth * AHeight * 3;
				default : return AWidth * AHeight;
			}

			return AWidth * AHeight;
		}
	}
//---------------------------------------------------------------------------
	struct TImageBuffer
	{
	public:
		TImageFormat::TImageFormat	PixelFormat;
		uint32_t	Width;
		uint32_t	Height;
		uint8_t	   *Data;

	public:
		inline uint32_t GetSize() { return TImageFormat::GetSize( PixelFormat, Width, Height ); }

	public:
		bool operator == ( const TImageBuffer &other )
		{
			if( PixelFormat != other.PixelFormat )
				return false;

			if( Width != other.Width )
				return false;

			if( Height != other.Height )
				return false;

			return( memcmp( Data, other.Data, GetSize() ) == 0 );
		}

		bool operator != ( const TImageBuffer &other )
		{
			if( PixelFormat != other.PixelFormat )
				return true;

			if( Width != other.Width )
				return true;

			if( Height != other.Height )
				return true;

			return( memcmp( Data, other.Data, GetSize() ) != 0 );
		}

	public:
		inline operator Mitov::String() const
		{
            return ToString();
		}

	public:
		Mitov::String ToString( uint32_t AMaxSize = 32 ) const
		{
			Mitov::String AResult = TImageFormat::ToString( PixelFormat ) + " " + Mitov::String( Width ) + "x" + Mitov::String( Height ) + " (";
			for( uint32_t i = 0; i < MitovMin<uint32_t>( Width * Height, uint32_t( AMaxSize ) ); ++i )
			{
				if( i < Width * Height - 1 )
					AResult += Mitov::String( Data[ i ], uint8_t( 10 ) ) + ",";

				else
					AResult += Mitov::String( Data[ i ], uint8_t( 10 ) );

			}

			if( Width * Height > AMaxSize )
				AResult += "...";

			AResult += ")";
			return	AResult;
		}

	public:
		TImageBuffer( TImageFormat::TImageFormat APixelFormat, uint32_t AWidth, uint32_t AHeight, const uint8_t *AData ) :
			PixelFormat( APixelFormat ),
			Width( AWidth ),
			Height( AHeight ),
			Data( (uint8_t *)AData )
		{
		}

	};
//---------------------------------------------------------------------------
	struct TStoredImageBuffer : public TImageBuffer
	{
		typedef TImageBuffer inherited;

	public:
		TStoredImageBuffer& operator = (const inherited& AOther)
		{
			if( inherited::Data )
				delete [] inherited::Data;

			inherited::PixelFormat = AOther.PixelFormat;
			inherited::Width = AOther.Width;
			inherited::Height = AOther.Height;
			inherited::Data = new uint8_t[ inherited::GetSize() ];

			memcpy( inherited::Data, AOther.Data, inherited::GetSize() );

			return *this;
		}

/*
	public:
		void Assign( T *AData, uint32_t ASize )
		{
			delete [] inherited::Data;
			inherited::Data = new T[ ASize ];

			memcpy( inherited::Data, AData, inherited::GetSize() );
		}

		void Copy( T *AData, uint32_t ASize, uint32_t AOffset )
		{
			memcpy( inherited::Data, AData, inherited::GetSize() );
		}
*/
	public:
		TStoredImageBuffer() :
			TStoredImageBuffer( TImageFormat::GrayScale, 1, 1 )
		{
		}

		TStoredImageBuffer( const inherited &AOther ) :
			TStoredImageBuffer( AOther.PixelFormat, AOther.Width, AOther.Height )
		{
			memcpy( inherited::Data, AOther.Data, inherited::GetSize() );
		}

		TStoredImageBuffer( TImageFormat::TImageFormat APixelFormat, uint32_t AWidth, uint32_t AHeight ) :
			inherited( APixelFormat, AWidth, AHeight, new uint8_t[ TImageFormat::GetSize( APixelFormat, AWidth, AHeight ) ] )
		{
		}

		TStoredImageBuffer( TImageFormat::TImageFormat APixelFormat, uint32_t AWidth, uint32_t AHeight, const uint8_t *AData ) :
			inherited( APixelFormat, AWidth, AHeight, new uint8_t[ TImageFormat::GetSize( APixelFormat, AWidth, AHeight ) ] )
		{
			memcpy( inherited::Data, AData, inherited::GetSize() );
		}

		~TStoredImageBuffer()
		{
			if( inherited::Data )
				delete [] inherited::Data;

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Octet1,
		typename T_Octet2,
		typename T_Octet3,
		typename T_Octet4,
		typename T_Octet5,
		typename T_Octet6
	> class TMACAddress :
		public T_Octet1,
		public T_Octet2,
		public T_Octet3,
		public T_Octet4,
		public T_Octet5,
		public T_Octet6
	{
	public:
		_V_PROP_( Octet1 )
		_V_PROP_( Octet2 )
		_V_PROP_( Octet3 )
		_V_PROP_( Octet4 )
		_V_PROP_( Octet5 )
		_V_PROP_( Octet6 )

	public:
		void GetMacAddress( uint8_t *AMacAddress )
		{
			AMacAddress[ 0 ] = Octet1();
			AMacAddress[ 1 ] = Octet2();
			AMacAddress[ 2 ] = Octet3();
			AMacAddress[ 3 ] = Octet4();
			AMacAddress[ 4 ] = Octet5();
			AMacAddress[ 5 ] = Octet6();
		}

	public:
		bool IsZero()
		{
			if( Octet1() )
				return false;

			if( Octet2() )
				return false;

			if( Octet3() )
				return false;

			if( Octet4() )
				return false;

			if( Octet5() )
				return false;

			if( Octet6() )
				return false;

			return true;
		}

	public:
		String AsText()
		{
			return ByteToHEX( Octet1() ) + ":" + ByteToHEX( Octet2() ) + ":" + ByteToHEX( Octet3() ) + ":" + ByteToHEX( Octet4() ) + ":" + ByteToHEX( Octet5() ) + ":" + ByteToHEX( Octet6() );
		}

	};
//---------------------------------------------------------------------------
	template <typename T> inline T GetDefaultValue() { return 0; }
	template<> inline bool GetDefaultValue() { return false; }
	template<> inline Mitov::String GetDefaultValue() { return ""; }
	template<> inline TDateTime GetDefaultValue() { return TDateTime(); }
	template<> inline TColor GetDefaultValue() { return TColor(); }
	template<> inline TRGBWColor GetDefaultValue() { return TRGBWColor(); }
//---------------------------------------------------------------------------
	template <typename T> inline T ExtractValue( void *_Data ) { return *( (T *)_Data ); }
	template<> inline Mitov::String ExtractValue( void *_Data ) { return ( (char *)_Data ); }
//---------------------------------------------------------------------------
	template <typename T> void swap ( T& a, T& b )
	{
		T c(a); a=b; b=c;
	}
//---------------------------------------------------------------------------
	template <
        uint32_t COUNT_Ramp_Elements,
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSource :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline T GetCurrentValue() { return Value(); }

	public:
		inline void UpdateValue()
		{
			T_OutputPin::SetPinValue( Value() );
		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalAddValue( T AValue )
		{
			Value() = Value() + AValue;
			T_OutputPin::SetPinValue( Value() );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
        uint32_t COUNT_Ramp_Elements,
		typename T_Elements_TryActive,
		typename T_FActiveElement,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSourceRamp :
		public T_OutputPin,
		public T_Value,
		public T_FActiveElement
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FActiveElement )

	public:
		inline bool GetCurrentValue() { return Value(); }

	public:
		void SetValue( T AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FActiveElement() == COUNT_Ramp_Elements )
				T_OutputPin::SetPinValue( AValue );

		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( AIndex != FActiveElement() )
				FActiveElement() = COUNT_Ramp_Elements;

//			inherited::ExternalSetValue( AIndex, AValue );
			if( Value() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalAddValue( T AValue )
		{
			FActiveElement() = COUNT_Ramp_Elements;
			Value() = Value() + AValue;
			T_OutputPin::SetPinValue( Value() );
		}

	public:
		void RegisterForTimeUpdate( int32_t AIndex, bool ARegister )
		{
			if( ARegister )
				FActiveElement() = AIndex;

			else if( FActiveElement() == AIndex )
				FActiveElement() = COUNT_Ramp_Elements;

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	public:
		inline ValueSourceRamp()
		{
			FActiveElement() = COUNT_Ramp_Elements;
		}
	};
//---------------------------------------------------------------------------
	template <
        uint32_t COUNT_Ramp_Elements,
		typename T_Elements_TryActive,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSourceState :
		public T_OutputPin,
		public T_Value,
		public T_FStateActive
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	protected:
		T FCurrentValue;

	public:
		inline T GetCurrentValue() { return FCurrentValue; }

	public:
		void SetValue( T AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FStateActive() )
				T_OutputPin::SetPinValue( AValue );

		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ExternalAddValue( T AValue )
		{
			Value() = Value() + AValue;
			T_OutputPin::SetPinValue( Value() );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline ValueSourceState()
		{
			FStateActive() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
        uint32_t COUNT_Ramp_Elements,
		typename T_Elements_TryActive,
		typename T_FActiveElement,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSourceRampState :
		public T_OutputPin,
		public T_Value,
		public T_FStateActive,
		public T_FActiveElement
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	public:
		_V_PROP_( FActiveElement )

	protected:
		T FCurrentValue;

	public:
		inline T GetCurrentValue() { return FCurrentValue; }

	public:
		void UpdateValue()
		{
			if( ! FStateActive() )
				return;

			if( FActiveElement() != COUNT_Ramp_Elements )
				return;

			T_OutputPin::SetPinValue( Value() );
		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( AIndex != FActiveElement() )
				FActiveElement() = COUNT_Ramp_Elements;

			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ExternalAddValue( T AValue )
		{
			FActiveElement() = COUNT_Ramp_Elements;
			FCurrentValue = FCurrentValue + AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		void RegisterForTimeUpdate( int32_t AIndex, bool ARegister )
		{
			if( ARegister )
				FActiveElement() = AIndex;

			else if( FActiveElement() == AIndex )
				FActiveElement() = COUNT_Ramp_Elements;

		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		ValueSourceRampState()
		{
			FActiveElement() = COUNT_Ramp_Elements;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T_Value
	> class ValueSourceDigital :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline bool GetCurrentValue() { return Value(); }

	public:
		inline void UpdateValue()
		{
			T_OutputPin::SetPinValue( Value() );
		}

		void ExternalSetValue( int32_t AIndex, bool AValue )
		{
			if( Value() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_FCurrentValue,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T_Value
	> class ValueSourceDigitalState :
		public T_OutputPin,
		public T_Value,
		public T_FStateActive,
		public T_FCurrentValue
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	protected:
		_V_PROP_( FCurrentValue )

	public:
		inline bool GetCurrentValue() { return FCurrentValue(); }

	public:
		void SetValue( bool AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FStateActive() )
				T_OutputPin::SetPinValue( AValue );

		}

		void ExternalSetValue( int32_t AIndex, bool AValue )
		{
			if( FCurrentValue() == AValue )
				return;

			FCurrentValue() = AValue;
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

			FCurrentValue() = Value();
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue() = Value();
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

	public:
		inline ValueSourceDigitalState()
		{
			FStateActive() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin,
        typename T_Value,
        typename T_Value_ApplyValues,
        typename T_Value_GetValue
	> class BinaryValueSource :
		public T_OutputPin,
        public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( Value )

    protected:
		void SetValue()
		{
//			if( ! T_Value::GetCount() )
//				return;

            T_Value::PinSend( OutputPin() );

//			OutputPin.Notify( &ABuffer );
		}

	public:
		void ExternalSetValue( const TArray<uint8_t> &ABuffer )
		{
//			TArray<T> AValue( ACount, AValues );
//			TArray<T> AValue( T_ValueElements::GetCount(), T_ValueElements::GetValue() );
//            typename T_Value::T_ARRAY *AData = T_Value::GetDataArray();
//            bool AEquals = ( *AData == ABuffer );
//            delete AData;
//			if( AEquals )
//				return;

            if( ABuffer.Size == T_Value::GetCount() )
                if( T_Value::Compare( ABuffer.Data ) == 0 )
    				return;

//			_Values = AValues;
//			_Count = ACount;
			T_OutputPin::SetPinValue( ABuffer );
		}
/*
		void ExternalSetValue( Mitov::Bytes &AValue )
		{
			if( Value == AValue )
				return;

			Value = AValue;
			T_OutputPin::SendPinValue( Mitov::TDataBlock( Value._BytesSize, Value._Bytes ));
		}
*/
	public:
		inline void SystemStart()
		{
            SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            SetValue();
		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_OWNER> class ArduinoBinaryStateSetValueElementBasic
	{
	protected:
		bool	FInputValue = false;

	public:
		Mitov::Bytes	Value;

	public:
		inline Mitov::Bytes GetCurrentValue() { return Value; }

	public:
		bool TryActive( T_OWNER *AOwner )
		{
			if( FInputValue )
			{
				C_OWNER.ExternalSetValue( Value );
				return true;
			}

			return false;
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FInputValue,
        typename T_Value,
        typename T_Value_ApplyValues,
        typename T_Value_GetValue
	> class ArduinoBinaryStateSetValueElement :
		public T_FInputValue,
        public T_Value
	{
	public:
		_V_PROP_( Value )

	protected:
		_V_PROP_( FInputValue )

//	public:
//		inline Mitov::Bytes GetCurrentValue() { return Value; }

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

			if( FInputValue() )
			{
                typename T_Value::T_ARRAY *ABuffer = T_Value::GetDataArray();
                C_OWNER.ExternalSetValue( *ABuffer );
                delete ABuffer;
//				C_OWNER.ExternalSetValue( Value );
				AResult = true;
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue() == *(bool *)_Data )
				return;

			FInputValue() = *(bool *)_Data;
			C_OWNER.ResetValue();
		}

//	public:
//		ArduinoBinaryStateSetValueElement()
//		{
//			C_OWNER.RegisterElement( this );
//		}

	public:
		ArduinoBinaryStateSetValueElement()
		{
			FInputValue() = false;
		}
	};
//---------------------------------------------------------------------------
    template <
		typename T_Elements_TryActive,
		typename T_OutputPin,
        typename T_Value,
        typename T_Value_ApplyValues,
        typename T_Value_GetValue
	> class BinaryValueSourceState :
		public T_OutputPin,
        public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( Value )

	protected:
		TValueArray<uint8_t> FCurrentValue;

	public:
		void ExternalSetValue( const TArray<uint8_t> &AValue )
		{
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

            FCurrentValue.Resize( T_Value::GetCount() );
            T_Value::CopyData( FCurrentValue.Data );
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
            FCurrentValue.Resize( T_Value::GetCount() );
            T_Value::CopyData( FCurrentValue.Data );
			if( T_Value::GetCount() )
    			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class DigitalInput_Flexible :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode(C_PIN_NUMBER, INPUT);
		}

		inline void SystemLoopBegin()
		{
			T_OutputPin::SetPinValue( digitalRead( C_PIN_NUMBER ) );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class DigitalInput_Inverted_Flexible :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode(C_PIN_NUMBER, INPUT);
		}

		inline void SystemLoopBegin()
		{
			T_OutputPin::SetPinValue( digitalRead( C_PIN_NUMBER ) != HIGH );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER ) != HIGH;
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? LOW : HIGH );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class DigitalInputInvertedPullUp_Flexible :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPin::SetPinValue( ! digitalRead( C_PIN_NUMBER ) );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return ! digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? LOW : HIGH );
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class DigitalInput :
//		public T_IsOutput,
		public T_IsPullDown,
		public T_IsPullUp,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsPullDown )
		_V_PROP_( IsPullUp )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT );
		}

		inline void SystemLoopBegin()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class DigitalPullUpInput :
//		public T_IsOutput,
		public T_IsPullDown,
		public T_IsPullUp,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsPullDown )
		_V_PROP_( IsPullUp )

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class SimpleDigitalInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT );
		}

		inline void SystemLoopBegin()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class SimpleDigitalPullUpInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_IsOutput,
//		typename T_IsPullDown,
//		typename T_IsPullUp,
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class InvertedDigitalPullUpInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = ! digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class InvertedDigitalInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit()
		{
//			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = ( digitalRead( C_PIN_NUMBER ) != HIGH );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		int C_VALUE,
		typename T_OutputPin
	> class InvertedAnalogAsDigitalInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit()
		{
//			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = ( analogRead( C_PIN_NUMBER ) < C_VALUE );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class AnalogAsDigitalInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void SetMode( int AMode ) {} // Placeholder
		void SetInitialMode( int AMode ) {} // Placeholder

		bool DigitalRead()
		{
			int AValue = analogRead( C_PIN_NUMBER );
			return ( AValue > VISUINO_ANALOG_IN_THRESHOLD ); // > 2.0 V High < 0.8 V Low
		}

	public:
		inline void SystemLoopBegin()
		{
			bool AValue = DigitalRead();
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemInit() {} // Placeholder
		inline void SystemStart() {} // Placeholder

	};
//---------------------------------------------------------------------------
	template <
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class ArduinoAnalogInputChannel :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
//		float			FLastOutput = 0.0f;

#ifdef VISUINO_ESP8266
		unsigned long	FLastTime;
#endif // VISUINO_ESP8266

/*
#ifdef VISUINO_TEENSY
	protected:
//		unsigned int	FResolution = 0;
//		float			FDivider = 1023.0;

	public:
		void SetResolution( unsigned int AResolution )
		{
			if( AResolution == FResolution )
				return;

			FResolution = AResolution;
			analogReadRes( FResolution );
			CalculateMultiplier();
		}

	protected:
		void CalculateMultiplier()
		{
			FDivider = pow( 2, FResolution ) - 1;
		}

#endif // VISUINO_TEENSY
*/

	public:
		inline void SystemInit() {} // Placeholder

		inline void SystemLoopUpdateHardware() {} // Placeholder

		inline void SystemStart()
		{
#ifdef VISUINO_TEENSY
//			analogReadRes( FResolution );
			float ALastOutput = analogRead( C_PIN_NUMBER ) / float( Mitov::GAnalogInDivider );
#else // VISUINO_TEENSY
			float ALastOutput = analogRead( C_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE );
#endif // VISUINO_TEENSY

#ifdef VISUINO_ESP8266
			FLastTime = micros();
#endif // VISUINO_ESP8266
	    	T_OutputPin::SetPinValue( ALastOutput, false );
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ESP8266
			unsigned long ACurrentMicros = micros();
			if( ACurrentMicros - FLastTime < 10000 )
				return;

			FLastTime = ACurrentMicros;
#endif // VISUINO_ESP8266
#ifdef VISUINO_TEENSY
			float AValue = analogRead( C_PIN_NUMBER ) / float( Mitov::GAnalogInDivider );
#else // VISUINO_TEENSY
			float AValue = analogRead( C_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE );
#endif // VISUINO_TEENSY
//			if( AValue == FLastOutput )
//				return;

//			FLastOutput = AValue;
			T_OutputPin::SetPinValue( AValue, true );
		}

/*
	public:
		ArduinoAnalogInputChannel()
#ifdef VISUINO_TEENSY
			: FResolution( 1023 )
#endif // VISUINO_TEENSY
		{
		}

#ifdef VISUINO_TEENSY
		ArduinoAnalogInputChannel( unsigned int AResolution ) :
			FResolution( AResolution )
		{
			CalculateMultiplier();
		}

#endif // VISUINO_TEENSY
*/
	};
//---------------------------------------------------------------------------
#if ( defined( VISUINO_TEENSY ) || defined( VISUINO_ESP32 ))
//---------------------------------------------------------------------------
	template <
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoTouchAnalogInputChannel :
		public T_IsOutput,
		public T_IsPullDown,
		public T_IsPullUp,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsOutput )
		_V_PROP_( IsPullDown )
		_V_PROP_( IsPullUp )

	public:
		inline void SystemInit() {} // Form compatibility!

		inline void SystemStart()
		{
			float AValue = touchRead( C_PIN_NUMBER );
	    	T_OutputPin::SetPinValue( AValue, false );
		}

		inline void SystemLoopBegin()
		{
			float AValue = touchRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue, true );
		}

	};
//---------------------------------------------------------------------------
/*
	template <int V_PIN> void SetArduinoAnalogInputResolutionChannel( unsigned int AValue )
	{
		analogReadRes( AValue );
	}
*/
//---------------------------------------------------------------------------
#endif // VISUINO_TEENSY
//---------------------------------------------------------------------------
	template <int C_PIN_NUMBER> class AnalogOutput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float*)_Data;
			Analog.Write( C_PIN_NUMBER, AValue );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, OUTPUT);
		}

	};
//---------------------------------------------------------------------------
    template <
		typename T_InitialValue,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoDigitalInputChannel :
		public T_IsPullDown,
		public T_IsPullUp,
		public T_InitialValue,
		public T_IsOpenDrain
	{
	public:
		_V_PROP_( IsOpenDrain )
		_V_PROP_( InitialValue )
		_V_PROP_( IsPullDown )
		_V_PROP_( IsPullUp )

	public:
		inline void SystemInit() {} // Placeholder
		inline void SystemStart() {} // Placeholder
		inline void SystemLoopBegin() {} // Placeholder

	public:
		inline void UpdatePinOutputValue()
		{
			digitalWrite( C_PIN_NUMBER, InitialValue().GetValue() ? HIGH : LOW );
		}

	public:
		inline void DigitalInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
			InitialValue() = AValue;
		}

	public:
		inline ArduinoDigitalInputChannel()
		{
			pinMode( C_PIN_NUMBER, ( IsOpenDrain() ) ? OUTPUT_OPEN_DRAIN : OUTPUT );
			digitalWrite( InitialValue().GetValue(), InitialValue().GetValue() ? HIGH : LOW );
		}
	};
//---------------------------------------------------------------------------
    template <
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_IsRawInput,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoDigitalOutputChannel :
		public T_IsAnalog,
		public T_IsOpenDrain,
		public T_IsOutput,
		public T_IsPullUp,
		public T_IsPullDown,
		public T_IsRawInput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsAnalog )
		_V_PROP_( IsOpenDrain )
		_V_PROP_( IsOutput )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsPullDown )
		_V_PROP_( IsRawInput )
//		_V_PROP_( InitialValue )
//		bool FIsPullDown : 1;
//		bool FLastOutput : 1;

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, IsPullUp() ? INPUT_PULLUP : ( IsPullDown() ? INPUT_PULLDOWN : INPUT ) );
		}

		inline void SystemStart()
		{
			bool ALastOutput = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    	T_OutputPin::SetPinValue( ALastOutput, false );
		}

		inline void SystemLoopBegin()
        {
    		bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//			if( AValue == FLastOutput )
//				return;

//			FLastOutput = AValue;
	    	T_OutputPin::SetPinValue( AValue, true );
        }

	public:
		inline void UpdatePinOutputValue()
		{
			digitalWrite( C_PIN_NUMBER, HIGH );
		}

	public:
        inline void SetIsOutput( bool AOldValue, bool AValue ) {} // Placeholder
        inline void SetIsAnalog( bool AOldValue, bool AValue ) {} // Placeholder
        inline void SetIsOpenDrain( bool AOldValue, bool AValue ) {} // Placeholder

        void SetIsPullUp( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullUp() = AValue;
            SystemInit();
        }

        void SetIsPullDown( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullDown() = AValue;
            SystemInit();
        }

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER ) != LOW;
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

/*
	public:
		inline ArduinoDigitalOutputChannel( bool AIsPullUp, bool AIsPullDown )
		{
			IsPullUp() = AIsPullUp;
			IsPullDown() = AIsPullDown;
		}
*/
	};
//---------------------------------------------------------------------------
    template <
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_IsRawInput,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoInvertedDigitalOutputChannel :
		public T_IsAnalog,
		public T_IsCombinedInOut,
		public T_IsOpenDrain,
		public T_IsOutput,
		public T_IsPullUp,
		public T_IsPullDown,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsAnalog )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOpenDrain )
		_V_PROP_( IsOutput )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsPullDown )
//		_V_PROP_( InitialValue )
//		bool FIsPullDown : 1;
//		bool FLastOutput : 1;

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, IsPullUp() ? INPUT_PULLUP : ( IsPullDown() ? INPUT_PULLDOWN : INPUT ) );
		}

		inline void SystemStart()
		{
			bool ALastOutput = ( digitalRead( C_PIN_NUMBER ) == LOW );
	    	T_OutputPin::SetPinValue( ALastOutput, false );
		}

		inline void SystemLoopBegin()
        {
    		bool AValue = ( digitalRead( C_PIN_NUMBER ) == LOW );
//			if( AValue == FLastOutput )
//				return;

//			FLastOutput = AValue;
	    	T_OutputPin::SetPinValue( AValue, true );
        }

	public:
		inline void UpdatePinOutputValue()
		{
			digitalWrite( C_PIN_NUMBER, LOW );
		}

	public:
        inline void SetIsOutput( bool AOldValue, bool AValue ) {} // Placeholder
        inline void SetIsAnalog( bool AOldValue, bool AValue ) {} // Placeholder
        inline void SetIsOpenDrain( bool AOldValue, bool AValue ) {} // Placeholder

        void SetIsPullUp( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullUp() = AValue;
            SystemInit();
        }

        void SetIsPullDown( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullDown() = AValue;
            SystemInit();
        }

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER ) != HIGH;
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? LOW : HIGH );
		}

/*
	public:
		inline ArduinoDigitalOutputChannel( bool AIsPullUp, bool AIsPullDown )
		{
			IsPullUp() = AIsPullUp;
			IsPullDown() = AIsPullDown;
		}
*/
	};
//---------------------------------------------------------------------------
    template <
		typename T_DigitalOutputPin,
		int C_PIN_NUMBER
	> class ArduinoAnalogAsDigitalOutputChannel :
		public T_DigitalOutputPin
	{
	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			bool ALastOutput = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    	T_DigitalOutputPin::SetPinValue( ALastOutput, false );
		}

		inline void SystemLoopBegin()
        {
    		bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//			if( AValue == FLastOutput )
//				return;

//			FLastOutput = AValue;
	    	T_DigitalOutputPin::SetPinValue( AValue, true );
        }

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialValue,
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_IsRawInput,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoDigitalChannel :
		public T_InitialValue,
		public T_IsAnalog,
		public T_IsCombinedInOut,
		public T_IsOpenDrain,
		public T_OutputPin,
		public T_IsPullDown,
		public T_IsPullUp,
		public T_IsRawInput,
		public T_IsOutput
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsAnalog )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOpenDrain )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsPullDown )
		_V_PROP_( IsRawInput )
		_V_PROP_( IsOutput )
		_V_PROP_( InitialValue )

	protected:
        void PinDirectionsInit()
        {
//			if( IsRawInput() )
//				return;

            if( IsOutput() )
			{
				pinMode( C_PIN_NUMBER, ( IsOpenDrain().GetValue() ) ? OUTPUT_OPEN_DRAIN : OUTPUT );
				if( ! IsAnalog().GetValue() )
					digitalWrite( C_PIN_NUMBER, InitialValue().GetValue() ? HIGH : LOW );
			}

            else
				pinMode( C_PIN_NUMBER, ( IsPullUp().GetValue() ) ? INPUT_PULLUP : ( IsPullDown() ? INPUT_PULLDOWN : INPUT ) );
        }

	public:
		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

	public:
		inline void SystemInit()
		{
            PinDirectionsInit();
		}

		inline void SystemStart()
		{
//			if( IsRawInput() )
//				return;
            if( ! IsOutput().GetValue() )
				if( T_OutputPin::GetPinIsConnected() )
				{
					bool ALastOutput = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    			T_OutputPin::SetPinValue( ALastOutput, false );
				}

		}

		inline void SystemLoopBegin()
        {
            if( ! IsOutput().GetValue() )
            {
    			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//				if( AValue == FLastOutput )
//					return;

//				FLastOutput = AValue;
//	Serial.println( AData.Value );
	    		T_OutputPin::SetPinValue( AValue, true );
            }
        }

	public:
		inline void UpdatePinOutputValue()
		{
			digitalWrite( C_PIN_NUMBER, InitialValue().GetValue() ? HIGH : LOW );
		}

	public:
		inline void AnalogInputPin_o_Receive( void *_Data )
        {
            if( IsOutput().GetValue() && IsAnalog().GetValue() )
            {
                float AValue = *(float*)_Data;
                Analog.Write( C_PIN_NUMBER, AValue );
            }
        }

		inline void DigitalInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			InitialValue() = AValue;
			if( IsRawInput() )
			{
				digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
//				*((int*)_Data) = C_PIN_NUMBER;
				return;
			}

            if( ( IsCombinedInOut().GetValue() || IsOutput().GetValue() ) && (! IsAnalog().GetValue() ))
            {
				if( AValue )
				  if( IsCombinedInOut() )
					  pinMode( C_PIN_NUMBER, OUTPUT );

                digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );

				if( ! AValue )
					if( IsCombinedInOut() )
						pinMode( C_PIN_NUMBER, IsPullUp() ? INPUT_PULLUP : ( IsPullDown() ? INPUT_PULLDOWN : INPUT ) );

            }
		}

	public:
        inline void SetIsOutput( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsOutput() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsPullUp( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullUp() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsPullDown( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullDown() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsAnalog( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsAnalog() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsOpenDrain( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsOpenDrain() = AValue;
            PinDirectionsInit();
        }

/*
	public:
		ArduinoDigitalChannel( bool AIsOutput, bool AIsPullUp, bool AIsPullDown, bool AIsOpenDrain, bool AIsAnalog, bool ACombinedInOut, bool ARawInput ) // :
//            FIsOutput( AIsOutput ),
//            FIsPullUp( AIsPullUp ),
//			FIsPullDown( AIsPullDown ),
//			FIsOpenDrain( AIsOpenDrain ),
//            FIsAnalog( AIsAnalog ),
//			FRawInput( ARawInput ),
//			FCombinedInOut( ACombinedInOut )
		{
			IsPullUp() = AIsPullUp;
			IsOutput() = AIsOutput;
			IsPullDown() = AIsPullDown;
			IsOpenDrain() = AIsOpenDrain;
			IsAnalog() = AIsAnalog;
			IsRawInput() = ARawInput;
			IsCombinedInOut() = ACombinedInOut;
		}
*/
	};
//---------------------------------------------------------------------------
    template <int C_PIN_NUMBER> class ArduinoAnalogOutputChannel
    {
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            float AValue = *(float*)_Data;
            Analog.Write( C_PIN_NUMBER, AValue );
		}

	};
//---------------------------------------------------------------------------
	template <int PIN_NUMBER> class DirectOuputPin
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			digitalWrite( PIN_NUMBER, ( *(bool*)_Data ) ? HIGH : LOW );
		}

	public:
		inline DirectOuputPin( bool AInitialValue = false )
		{
			pinMode( PIN_NUMBER, OUTPUT );
			digitalWrite( PIN_NUMBER, ( AInitialValue ) ? HIGH : LOW );
		}
	};
//---------------------------------------------------------------------------
	template <
		int PIN_0_NUMBER,
		int PIN_1_NUMBER
	> class DirectGroveOuputPin0
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			digitalWrite( PIN_0_NUMBER, ( *(bool*)_Data ) ? HIGH : LOW );
		}

	public:
		inline DirectGroveOuputPin0( bool AInitialValue = false )
		{
			pinMode( PIN_0_NUMBER, OUTPUT );
			digitalWrite( PIN_0_NUMBER, ( AInitialValue ) ? HIGH : LOW );
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO_MAPLE_OLD )
			pinMode( PIN_0_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( PIN_0_NUMBER, AMode );
#endif
		}

		inline void SetInitialMode( int AMode )
		{
			SetMode( AMode );
		}

		inline bool DigitalRead()
		{
			return digitalRead( PIN_0_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( PIN_0_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template <
		int PIN_1,
		int PIN_2,
		int PIN_3,
		int PIN_4,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3
	> class InvertedDigtitalInputPins4 :
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )

	public:
		inline void SystemInit()
		{
//			pinMode( PIN_1, INPUT );
//			pinMode( PIN_2, INPUT );
//			pinMode( PIN_3, INPUT );
//			pinMode( PIN_4, INPUT );
		}

		inline void SystemStart()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) != HIGH , false );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) != HIGH , false );
			T_OutputPins_2::SetPinValue( digitalRead( PIN_3 ) != HIGH , false );
			T_OutputPins_3::SetPinValue( digitalRead( PIN_4 ) != HIGH , false );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) != HIGH , true );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) != HIGH , true );
			T_OutputPins_2::SetPinValue( digitalRead( PIN_3 ) != HIGH , true );
			T_OutputPins_3::SetPinValue( digitalRead( PIN_4 ) != HIGH , true );
		}
	};
//---------------------------------------------------------------------------
	template <int C_PIN_NUMBER>	class TPinAccess
	{
	public:
		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

	};
//---------------------------------------------------------------------------
	template <int C_PIN> class TDefinedDigitalPinBasic
	{
	protected:
		typedef TPinAccess<C_PIN> T_PIN_TYPE;

	};
//---------------------------------------------------------------------------
	template <int C_PIN> class TDefinedDigitalInputPin : public TDefinedDigitalPinBasic<C_PIN>
	{
	protected:
		inline void Init()
		{
			pinMode( C_PIN, INPUT );
		}

		inline bool ReadValue()
		{
			return( digitalRead( C_PIN ) == HIGH );
		}
	};
//---------------------------------------------------------------------------
	template <int C_PIN> class TDefinedDigitalInputPinPullUp : public TDefinedDigitalInputPin<C_PIN>
	{
	protected:
		inline void Init()
		{
			pinMode( C_PIN, INPUT_PULLUP );
		}

	};
//---------------------------------------------------------------------------
	template <int C_PIN> class TDefinedDigitalInputPinInverted : public TDefinedDigitalPinBasic<C_PIN>
	{
	protected:
		inline void Init()
		{
			pinMode( C_PIN, INPUT );
		}

		inline bool ReadValue()
		{
			return( digitalRead( C_PIN ) != HIGH );
		}
	};
//---------------------------------------------------------------------------
	template <int C_PIN> class TDefinedDigitalInputPinPullUpInverted : public TDefinedDigitalInputPinInverted<C_PIN>
	{
	protected:
		inline void Init()
		{
			pinMode( C_PIN, INPUT_PULLUP );
		}

	};
//---------------------------------------------------------------------------
	template <int C_PIN> class TDefinedAnalogInputPin
	{
	protected:
		typedef TPinAccess<C_PIN> T_PIN_TYPE;

	protected:
		inline void Init()
		{
//			pinMode( C_PIN, INPUT );
		}

		inline bool ReadValue()
		{
			return( analogRead( C_PIN ) / float( VISUINO_ANALOG_IN_RANGE ) );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_PIN_1,
		typename T_PIN_2,
		typename T_OutputPins0,
		typename T_OutputPins1
	> class TypedInputPins2 :
		public T_PIN_1,
		public T_PIN_2,
		public T_OutputPins0,
		public T_OutputPins1
	{
	public:
		typedef typename T_PIN_1::T_PIN_TYPE T_PIN_TYPE_0;
		typedef typename T_PIN_2::T_PIN_TYPE T_PIN_TYPE_1;

	public:
		static T_PIN_TYPE_0 PinObject0;
		static T_PIN_TYPE_1 PinObject1;

	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )

	protected:
		inline void ReadValues( bool AChangeOnly )
		{
			T_OutputPins0::SetPinValue( T_PIN_1::ReadValue(), AChangeOnly );
			T_OutputPins1::SetPinValue( T_PIN_2::ReadValue(), AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValues( false );
		}

	public:
		inline void SystemInit()
		{
			T_PIN_1::Init();
			T_PIN_2::Init();
		}

		inline void SystemStart()
		{
			ReadValues( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValues( true );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_PIN_1,
		typename T_PIN_2,
		typename T_PIN_3,
		typename T_OutputPins0,
		typename T_OutputPins1,
		typename T_OutputPins2
	> class TypedInputPins3 :
		public T_PIN_1,
		public T_PIN_2,
		public T_PIN_3,
		public T_OutputPins0,
		public T_OutputPins1,
		public T_OutputPins2
	{
	public:
		typedef typename T_PIN_1::T_PIN_TYPE T_PIN_TYPE_0;
		typedef typename T_PIN_2::T_PIN_TYPE T_PIN_TYPE_1;
		typedef typename T_PIN_3::T_PIN_TYPE T_PIN_TYPE_2;

	public:
		static T_PIN_TYPE_0 PinObject0;
		static T_PIN_TYPE_1 PinObject1;
		static T_PIN_TYPE_2 PinObject2;

	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )
		_V_PIN_( OutputPins2 )

	protected:
		inline void ReadValues( bool AChangeOnly )
		{
			T_OutputPins0::SetPinValue( T_PIN_1::ReadValue(), AChangeOnly );
			T_OutputPins1::SetPinValue( T_PIN_2::ReadValue(), AChangeOnly );
			T_OutputPins2::SetPinValue( T_PIN_3::ReadValue(), AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValues( false );
		}

	public:
		inline void SystemInit()
		{
			T_PIN_1::Init();
			T_PIN_2::Init();
			T_PIN_3::Init();
		}

		inline void SystemStart()
		{
			ReadValues( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValues( true );
		}
	};
//---------------------------------------------------------------------------
	template <
	    typename T_Enabled,
		typename T_OutputPin,
		int IN_PIN
	> class AnalogInputPinSensor :
	    public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		inline void ReadValue()
		{
		    if( Enabled() )
			    T_OutputPin::SetPinValue( analogRead( IN_PIN ) / float( VISUINO_ANALOG_IN_RANGE ) );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue();
		}

	public:
		inline void SystemInit()
		{
//			pinMode( PIN, INPUT );
		}

		inline void SystemStart()
		{
			ReadValue();
		}

		inline void SystemLoopBegin()
		{
			ReadValue();
		}

	};
//---------------------------------------------------------------------------
	template <
	    typename T_Enabled,
		typename T_OutputPin,
		int IN_PIN
	> class InvertedAnalogInputPinSensor :
	    public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		inline void ReadValue()
		{
		    if( Enabled() )
    			T_OutputPin::SetPinValue( 1.0f - ( analogRead( IN_PIN ) / float( VISUINO_ANALOG_IN_RANGE )) );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue();
		}

	public:
		inline void SystemInit()
		{
//			pinMode( PIN, INPUT );
		}

		inline void SystemStart()
		{
			ReadValue();
		}

		inline void SystemLoopBegin()
		{
			ReadValue();
		}

	};
//---------------------------------------------------------------------------
#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( VISUINO_Mx ))
	#define SERIAL_TYPE Serial_

#else // VISUINO_CIRCUIT_PLAYGROUND_EXPRESS
	#if defined( VISUINO_NRF52 ) || defined( VISUINO_BBC_MICRO_BIT )
		#define SERIAL_TYPE Uart

	#else // VISUINO_NRF52
		#ifdef VISUINO_FREESOC2
			#define SERIAL_TYPE UARTClass

		#else // VISUINO_FREESOC2
			#ifdef VISUINO_FEATHER_M0
				#define SERIAL_TYPE Serial_

			#else // VISUINO_FEATHER_M0
				#ifdef VISUINO_FEMTO_USB
					#define SERIAL_TYPE Serial_
					#define Serial SerialUSB

				#else // VISUINO_FEMTO_USB
					#ifdef HAVE_CDCSERIAL
						#define SERIAL_TYPE Serial_
					#else // HAVE_CDCSERIAL
						#ifdef VISUINO_ARDUINO_101
							#define SERIAL_TYPE CDCSerialClass
						#else // VISUINO_ARDUINO_101
							#ifdef VISUINO_INTEL_EDISON
								#define SERIAL_TYPE TTYUARTClass
							#else // VISUINO_INTEL_EDISON
								#ifdef VISUINO_ARDUINO_AVR4809
									#define SERIAL_TYPE UartClass
								#else // VISUINO_ARDUINO_AVR4809
									#ifdef VISUINO_ARDUINO_PORTENTA
										#define SERIAL_TYPE arduino::UART
									#else // VISUINO_ARDUINO_PORTENTA
										#define SERIAL_TYPE HardwareSerial
									#endif // VISUINO_ARDUINO_PORTENTA
								#endif // VISUINO_ARDUINO_NANO_EVERY
							#endif // VISUINO_INTEL_EDISON
						#endif // VISUINO_ARDUINO_101
					#endif // HAVE_CDCSERIAL
				#endif // VISUINO_FEMTO_USB
			#endif // VISUINO_FEATHER_M0
		#endif // VISUINO_FREESOC2
	#endif // VISUINO_NRF52
#endif // VISUINO_CIRCUIT_PLAYGROUND_EXPRESS
//---------------------------------------------------------------------------
	enum TArduinoSerialParity { spNone, spEven, slOdd, slMark, slSpace };
	typedef TArduinoSerialParity TArduinoStandardSerialParity;
//---------------------------------------------------------------------------
	namespace Func
	{
		const char PROGMEM b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";
//---------------------------------------------------------------------------
		inline void a3_to_a4(unsigned char * a4, unsigned char * a3)
		{
			a4[0] = (a3[0] & 0xfc) >> 2;
			a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
			a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
			a4[3] = (a3[2] & 0x3f);
		}
//---------------------------------------------------------------------------
		inline void a4_to_a3(unsigned char * a3, unsigned char * a4)
		{
			a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
			a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
			a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
		}
//---------------------------------------------------------------------------
		inline unsigned char b64_lookup(char c)
		{
			if(c >='A' && c <='Z')
				return c - 'A';

			if(c >='a' && c <='z')
				return c - 71;

			if(c >='0' && c <='9')
				return c + 4;

			if(c == '+')
				return 62;

			if(c == '/')
				return 63;

			return -1;
		}
//---------------------------------------------------------------------------
		int Base64Encode(char *output, const char *input, int inputLen)
		{
			int i = 0, j = 0;
			int encLen = 0;
			unsigned char a3[3];
			unsigned char a4[4];

			while(inputLen--)
			{
				a3[i++] = *(input++);
				if(i == 3)
				{
					a3_to_a4(a4, a3);

					for(i = 0; i < 4; i++)
						output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);

					i = 0;
				}
			}

			if(i)
			{
				for(j = i; j < 3; j++)
					a3[j] = '\0';

				a3_to_a4(a4, a3);

				for(j = 0; j < i + 1; j++)
					output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);

				while((i++ < 3))
					output[encLen++] = '=';

			}

			output[encLen] = '\0';
			return encLen;
		}
//---------------------------------------------------------------------------
		int Base64Decode(char * output, const char * input, int inputLen)
		{
			int i = 0, j = 0;
			int decLen = 0;
			unsigned char a3[3] = {};
			unsigned char a4[4];


			while (inputLen--)
			{
				if(*input == '=')
					break;

				a4[i++] = *(input++);
				if (i == 4)
				{
					for (i = 0; i <4; i++)
						a4[i] = b64_lookup(a4[i]);

					a4_to_a3(a3,a4);

					for (i = 0; i < 3; i++)
						output[decLen++] = a3[i];

					i = 0;
				}
			}

			if (i)
			{
				for (j = i; j < 4; j++)
					a4[j] = '\0';

				for (j = 0; j <4; j++)
					a4[j] = b64_lookup(a4[j]);

				a4_to_a3(a3,a4);

				for (j = 0; j < i - 1; j++)
					output[decLen++] = a3[j];

			}

			output[decLen] = '\0';
			return decLen;
		}
//---------------------------------------------------------------------------
		inline int Base64EncodeLength(int plainLen)
		{
			int n = plainLen;
			return (n + 2 - ((n + 2) % 3)) / 3 * 4;
		}
//---------------------------------------------------------------------------
		int Base64DecodeLength(char * input, int inputLen)
		{
			int i = 0;
			int numEq = 0;
			for(i = inputLen - 1; input[i] == '='; i--)
				numEq++;

			return ((6 * inputLen) / 8) - numEq;
		}
//---------------------------------------------------------------------------
		Mitov::String Base64Encode( Mitov::String input )
		{
			int ALength = Base64EncodeLength( input.length() );
			Mitov::String AResult;
			AResult.reserve( ALength );
			for( int i = 0; i < ALength; ++i )
				AResult += " ";

			Base64Encode( (char *)AResult.c_str(), input.c_str(), input.length() );

			return AResult;
		}
//---------------------------------------------------------------------------
		Mitov::String ByteListToString( Mitov::SimpleList<uint8_t, uint16_t> &AValue )
		{
			Mitov::String Result;
			Result.reserve( AValue.size());
			for( uint32_t i = 0; i < AValue.size(); ++i )
				Result += (char)AValue[ i ];

			return Result;
		}
//---------------------------------------------------------------------------
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
		inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)
		{
			return (uint16_t(r & 0xF8) << 8) | (uint16_t(g & 0xFC) << 3) | (b >> 3);
		}
//---------------------------------------------------------------------------
		inline uint16_t Color565(uint32_t AColor )
		{
			uint8_t	r = AColor >> 16;
			uint8_t	g = AColor >> 8;
			uint8_t	b = AColor;
			return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
		}
//---------------------------------------------------------------------------
		inline uint32_t ColorFrom565( uint16_t AColor )
		{
			return	0b000001110000001100000111 |
					(( uint32_t( AColor ) & 0xf800 ) << 8 ) |
					(( uint32_t( AColor ) & 0x07e0 ) << 5 ) |
					(( uint32_t( AColor ) & 0x001f ) << 3 );

		}
//---------------------------------------------------------------------------
		inline uint32_t ColorFrom555( uint16_t AColor )
		{
			return	0b000001110000011100000111 |
					(( uint32_t( AColor ) & 0xf800 ) << 9 ) |
					(( uint32_t( AColor ) & 0x07e0 ) << 6 ) |
					(( uint32_t( AColor ) & 0x001f ) << 3 );

		}
//---------------------------------------------------------------------------
		void insert_sort(int array[], uint8_t size)
		{
			uint8_t j;
			int save;

			for (int i = 1; i < size; i++)
			{
				save = array[i];
				for (j = i; j >= 1 && save < array[j - 1]; j--)
					array[j] = array[j - 1];

				array[j] = save;
			}
		}
//---------------------------------------------------------------------------
		inline uint8_t FromBcdToDec(const uint8_t bcd)
		{
			return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
		}
//---------------------------------------------------------------------------
		inline uint8_t FromDecToBcd(const uint8_t dec)
		{
			const uint8_t tens = dec / 10;
			const uint8_t ones = dec % 10;
			return (tens << 4) | ones;
		}
//---------------------------------------------------------------------------
		uint8_t FromCharsToInt8( const char *AValues, uint8_t ALength )
		{
			uint8_t AResult = 0;
			for( int i = 0; i < ALength; ++ i )
				AResult = AResult * 10 + ( AValues[ i ] - '0' );

			return AResult;
		}
//---------------------------------------------------------------------------
		uint16_t FromCharsToInt16( const char *AValues, uint8_t ALength )
		{
			uint16_t AResult = 0;
			for( int i = 0; i < ALength; ++ i )
				AResult = AResult * 10 + ( AValues[ i ] - '0' );

			return AResult;
		}
//---------------------------------------------------------------------------
		uint64_t StringToUInt64( const char *AValues )
		{
			uint64_t AResult = 0;
            while( *AValues == ' ' )
                ++ AValues;

            while( *AValues )
            {
                if( *AValues < '0' || *AValues > '9' )
                    break;

				AResult = AResult * 10 + ( *AValues ++ - '0' );
            }

			return AResult;
		}
//---------------------------------------------------------------------------
		inline int64_t StringToInt64( const char *AValues )
		{
            if( *AValues == '-' )
                return - StringToUInt64( AValues + 1 );

            else if( *AValues == '+' )
                return StringToUInt64( AValues + 1 );

            return StringToUInt64( AValues );
		}
//---------------------------------------------------------------------------
		uint32_t StringToUInt32( const char *AValues )
		{
			uint32_t AResult = 0;
            while( *AValues == ' ' )
                ++ AValues;

            while( *AValues )
            {
                if( *AValues < '0' || *AValues > '9' )
                    break;

				AResult = AResult * 10 + ( *AValues ++ - '0' );
            }

			return AResult;
		}
//---------------------------------------------------------------------------
        Mitov::String UInt64ToString( uint64_t AValue )
        {
            char AText[ 21 ];
            char *AOutPtr = AText + 20;
            *AOutPtr = '\0';
            while( AValue )
            {
                * --AOutPtr = ( AValue % 10 ) + '0';
                AValue /= 10;
            }

            return AOutPtr;
        }
//---------------------------------------------------------------------------
        Mitov::String Int64ToString( int64_t AValue )
        {
            bool AIsNegative = ( AValue < 0 );
            if( AIsNegative )
            {
                AValue = -AValue;
                AIsNegative = true;
            }

            char AText[ 22 ];
            char *AOutPtr = AText + 21;
            *AOutPtr = '\0';
            while( AValue )
            {
                * --AOutPtr = ( AValue % 10 ) + '0';
                AValue /= 10;
            }

            if( AIsNegative )
                * --AOutPtr = '-';

            return AOutPtr;
        }
//---------------------------------------------------------------------------
		inline float ConvertCtoF(float c)
		{
			return c * 1.8 + 32;
		}
//---------------------------------------------------------------------------
		inline float ConvertFtoC(float f)
		{
			return (f - 32) * 0.55555;
		}
//---------------------------------------------------------------------------
		template <typename T>T MapRange( T x, T in_min, T in_max, T out_min, T out_max )
		{
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}
//---------------------------------------------------------------------------
		bool ExtractOptionallyQuotedCommaText( Mitov::String &AText, Mitov::String &AResult )
		{
			if( AText.length() == 0 )
				return false;

			bool	AStartsWithQuote;
//			int		AStartIndex;

			if( AText[ 0 ] == '"' )
			{
				AStartsWithQuote = true;
//				AStartIndex = 1;
			}

			else
			{
				AStartsWithQuote = false;
//				AStartIndex = 0;
			}

			if( ! AStartsWithQuote )
			{
				int APos = AText.indexOf( "," );
				if( APos < 0 )
				{
					AResult = AText;
					AText = "";
					return true;
				}

				AResult = AText.substring( 0, APos );
				AText.remove( 0, APos + 1 );
				return true;
			}

			AResult = "";
			int ASubStartsWithQuote = 0;
			uint32_t ALength = 0;
			while( ALength < AText.length() )
			{
				if( AText[ ASubStartsWithQuote ] == ',' )
					break;

				if( AText[ ASubStartsWithQuote ] != '"' )
					return false;

				++ASubStartsWithQuote;

	//			Serial.print( "ASubStartsWithQuote: " ); Serial.println( ASubStartsWithQuote );
				int APos = AText.indexOf( "\"", ASubStartsWithQuote );
				if( APos < 0 )
					return false;

	//			Serial.println( APos );
				AResult += AText.substring( ASubStartsWithQuote, APos );

				ASubStartsWithQuote = APos + 1;
				ALength = APos + 2;
			}

			AText.remove( 0, ALength );

			return true;
		}
//---------------------------------------------------------------------------
		bool ContainsTextAtPos( Mitov::String ALine, int APos, Mitov::String ASubText )
		{
			if( ALine.length() < APos + ASubText.length() )
				return false;

			for( int i = 0; i < int( ASubText.length()); ++i )
				if( ALine[ APos + i ] != ASubText[ i ] )
					return false;

			return true;
		}
//---------------------------------------------------------------------------
/*
		uint8_t Exponent(uint16_t value)
		{
			// Calculates the base 2 logarithm of a value
			uint8_t result = 0;
			while (((value >> result) & 1) != 1)
				result++;
			return(result);
		}
*/
//---------------------------------------------------------------------------
		bool SetBitField( uint8_t *AValuesArray, int AIndex, bool AValue )
		{
			uint8_t AMask = 1 << ( AIndex & 0b111 ); // same as % 8
			uint8_t	AArrayIndex = AIndex / 8;
			if( AValue == (( AValuesArray[ AArrayIndex ] & AMask ) != 0 ))
				return false;

			if( AValue )
				AValuesArray[ AArrayIndex ] |= AMask;

			else
				AValuesArray[ AArrayIndex ] &= ~AMask;

			return true;
		}
//---------------------------------------------------------------------------
		inline float ConvertAngle( TAngleUnits AUints, float AValue )
		{
			switch( AUints )
			{
				case auRadians :	return AValue * PI / 180.0f;
				case auNormalized :	return AValue / 360;
				default : return AValue;

			}

//			return AValue;
		}
//---------------------------------------------------------------------------
		inline uint32_t SwapRB( uint32_t AColor )
		{
			return ( ( AColor & 0xFF ) << 16 ) | ( AColor & 0x00FF00 ) | ( ( AColor >> 16 ) & 0xFF );
		}
//---------------------------------------------------------------------------
		uint8_t GetVariableSize( uint32_t ALength )
		{
            uint8_t AResult = 0;
			do
			{
				++ AResult;
				ALength >>= 7;
			}
			while( ALength );

            return AResult;
		}
//---------------------------------------------------------------------------
		void GetAddVariableSizeValue( uint8_t * ABuffer, uint32_t &ADataIndex, uint32_t AValue )
		{
			uint8_t ACount = 0;
			do
			{
				++ ACount;
//				C_OWNER.CheckSize( ADataIndex + AValue + ACount );
				ABuffer[ ADataIndex ] = AValue & 0x7F;

				AValue >>= 7;
				if( AValue )
					ABuffer[ ADataIndex ] |= 0x80;

				ADataIndex ++;
			}
			while( AValue );
		}
//---------------------------------------------------------------------------
		uint32_t GetVariableSizeValue( uint8_t * ABuffer, uint32_t ABufferSize, uint32_t &ADataIndex )
		{
			uint32_t AResult = 0;

			uint8_t	AByteSize;
            uint8_t AOffset = 0;
			do
			{
                if( ADataIndex >= ABufferSize )
                    AByteSize = 0;

                else
                    AByteSize = ABuffer[ ADataIndex ];

				AResult += ( AByteSize & 0x7F ) << ( AOffset * 7 );
				++ AOffset;
                ++ ADataIndex;
			}
			while( AByteSize & 0x80 );

            return AResult;
		}
//---------------------------------------------------------------------------
	}
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL, typename T> class ArduinoSerialInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( *(T*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialInput<T_INSTANCE, C_SERIAL, uint64_t>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            C_SERIAL.Print( Func::UInt64ToString( *(uint64_t*)_Data ));
		}

	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialInput<T_INSTANCE, C_SERIAL, int64_t>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            C_SERIAL.Print( Func::Int64ToString( *(int64_t*)_Data ));
		}

	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialInput<T_INSTANCE, C_SERIAL, bool>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( *(bool*)_Data ? "true" : "false" );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialInput<T_INSTANCE, C_SERIAL, char>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( *(char*)_Data == '\n' )
				return;

			if( *(char*)_Data == '\r' )
				C_SERIAL.Print( "" );

			else
				C_SERIAL.PrintChar( *(char*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL, typename T> class ArduinoSerialInput_Binary
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			C_SERIAL.Write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoEnabledSerialBinaryInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
  			C_SERIAL.Write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
		}

	};
*/
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL, typename T> class ArduinoSerialInput_String
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( (char*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialInput_Clock
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( "(Clock)" );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE, T_INSTANCE &C_SERIAL, typename T_OBJECT> class ArduinoSerialInput_Object
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( ((T_OBJECT *)_Data)->ToString().c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_AccessPointName,
		typename T_Enabled,
		typename T_NumberRetries,
		typename T_Password,
		typename T_RetryInterval,
		typename T_UserName
	> class GPRSAccessPoint :
		public T_AccessPointName,
		public T_Enabled,
		public T_NumberRetries,
		public T_Password,
		public T_RetryInterval,
		public T_UserName
	{
	public:
		_V_PROP_( Enabled )

		_V_PROP_( AccessPointName )
		_V_PROP_( UserName )
		_V_PROP_( Password )

		_V_PROP_( NumberRetries )
		_V_PROP_( RetryInterval )

	public:
		int32_t	FRetryCount = 0;
		unsigned long FLastTime = 0;

	public:
		inline void ResetRetry()
		{
			FRetryCount = 0;
		}

	public:
		void TryConnect( bool AFromStart, bool &ATryNextConnection, bool &AProccessed )
		{
			if( ! Enabled() )
				return;

			bool ACanRetry = true;
			if( ! AFromStart )
				if( ! CanRetry( ACanRetry ) )
				{
					ATryNextConnection |= ACanRetry;
					return;
				}

			ATryNextConnection |= ACanRetry;
			if( C_OWNER.TryConnectToPoint( AccessPointName(), UserName(), Password() ))
			{
				AProccessed = true;
				return;
			}

			FLastTime = micros();
			++ FRetryCount;
		}

	public:
		bool CanRetry( bool &ACanRetry )
		{
			ACanRetry = ( FRetryCount <= NumberRetries() );
//			Serial.print( "ACanRetry" ); Serial.println( ACanRetry );
			if( FRetryCount == 0 )
				return true;

			if( ! ACanRetry )
				return false;

			if(( micros() - FLastTime ) < RetryInterval() * 1000 )
				return false;

			return true;
		}

	};
//---------------------------------------------------------------------------
	class Convert_AnalogToComplexArray
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TComplex	AComplex( *(float *)_Data );
			TArray<Mitov::TComplex> AComplexBuffer( 1, &AComplex );
			ACallback( &AComplexBuffer );
		}

	};
//---------------------------------------------------------------------------
	class Convert_BinaryBlockToChar
	{
	public:
		static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
//			Serial.println( "TEST" );
			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;
			char *APtr = (char *)ABlock.Data;
			int ASize = ABlock.Size;
//			Serial.println( ASize );
			while( ASize-- )
			{
//  				Serial.println( *APtr );
				ACallback( APtr++ );
//				if( OutputPin.IsConnected() )
//					Serial.println( "TEST2" );
			}
		}

	};
//---------------------------------------------------------------------------
	class Convert_BinaryBlockToText
	{
	public:
		static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
//			Serial.println( "TEST" );
			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;
			char *APtr = (char *)ABlock.Data;
			int ASize = ABlock.Size;
			Mitov::String AText;
			AText.reserve( ASize );
//			Serial.println( ASize );
			while( ASize-- )
				AText += *APtr++;

			ACallback( (char *)AText.c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_FLastValue> class Convert_BinaryToClock : public T_FLastValue
	{
	protected:
		_V_PROP_( FLastValue )

	public:
		inline void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			bool AValue = *(bool *)_Data;
			if( AValue == FLastValue() )
				return;

			FLastValue() = AValue;
			if( AValue )
				ACallback( nullptr );

		}

	public:
		Convert_BinaryToClock()
		{
			FLastValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_FROM, typename T_TO> class TypedConvert
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			T_TO AValue = T_TO( *(T_FROM *)_Data );
			ACallback( &AValue );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_FROM, typename T_TO> class ForcedTypedConvert
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			T_TO AValue = *(T_TO *)_Data;
			ACallback( &AValue );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_FROM> class TypedConvert<T_FROM, Mitov::String>
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			Mitov::String AValue = Mitov::String( *(T_FROM *)_Data );
			ACallback( (void *)AValue.c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_TO> class TypedDigitalConvert
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
		    T_TO AValue = ( *(bool*)_Data ) ? 1 : 0;
			ACallback( &AValue );
		}

	};
//---------------------------------------------------------------------------
	class Convert_DigitalToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			if(*(bool*)_Data )
				ACallback( (void *)"true" );

			else
				ACallback( (void *)"false" );

		}

	};
//---------------------------------------------------------------------------
	class Convert_TextToBytes
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			char *AText = ( char * )_Data;
			Mitov::TDataBlock AOutData( strlen( AText ), AText );
			ACallback( &AOutData );
		}

	};
//---------------------------------------------------------------------------
	class Convert_DateTimeToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TDateTime &ADate = *(TDateTime *)_Data;
			ACallback( (void *)ADate.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	class Convert_TextToChar
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			for( char *AData = (char *)_Data; *AData; ++ AData )
				ACallback( AData );
			char AValue = 13;
			ACallback( &AValue );

			AValue = 10;
			ACallback( &AValue );
		}

	};
//---------------------------------------------------------------------------
	class Convert_ColorToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TColor &AColor = *(TColor *)_Data;
			ACallback( (void *)AColor.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	class Convert_ColorToAlphaColor
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			uint32_t AColor = 0xFF000000 | ( *(uint32_t *)_Data );
			ACallback( &AColor );
		}

	};
//---------------------------------------------------------------------------
	class Convert_RGBWColorToRGBWAlphaColor
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			uint64_t AColor = 0xFF00000000 | uint64_t( *(uint32_t *)_Data );
			ACallback( &AColor );
		}

	};
//---------------------------------------------------------------------------
	class Convert_RGBWColorToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TRGBWColor &AColor = *(TRGBWColor *)_Data;
			ACallback( (void *)AColor.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	class Convert_RGBWAlphaColorToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TRGBWAlphaColor &AColor = *(TRGBWAlphaColor *)_Data;
			ACallback( (void *)AColor.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	class Convert_ImageToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
            TImageBuffer &AImage = *(TImageBuffer *)_Data;
			ACallback( (void *)AImage.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
    class Convert_ImageToGrayscale4
    {
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
            TImageBuffer &AImage = *(TImageBuffer *)_Data;
            switch( AImage.PixelFormat )
            {
                case TImageFormat::RGB24 :
                {
                    TStoredImageBuffer AOutBufffer( TImageFormat::Gray4, AImage.Width, AImage.Height );
                    uint8_t *AInPtr = AImage.Data;
                    uint8_t *AOutPtr = AOutBufffer.Data;
                    for( int32_t i = 0; i < int32_t( AImage.Width * AImage.Height ); ++ i )
                    {
                        // R G B -> Gray
                        uint8_t AGrayPixel = uint8_t( 0.21 * float( AInPtr[ 0 ] ) + 0.72 * float( AInPtr[ 1 ] ) + 0.07 * float( AInPtr[ 2 ] ) + 0.5 );
                        if( i & 1 )
                            *AOutPtr ++ |= AGrayPixel & 0xF0;

                        else
                            *AOutPtr = AGrayPixel >> 4;

                        ++ AOutPtr;
                        AInPtr += 3;
                    }

//			return 0xFF00 | uint8_t( 0.21 * float( r ) + 0.72 * float( g ) + 0.07 * float( b ) + 0.5 );

                    ACallback( &AOutBufffer );
                    break;
                }

                case TImageFormat::RGBA32 :
                {
                    TStoredImageBuffer AOutBufffer( TImageFormat::Gray4, AImage.Width, AImage.Height );
                    uint8_t *AInPtr = AImage.Data;
                    uint8_t *AOutPtr = AOutBufffer.Data;
                    for( int32_t i = 0; i < int32_t( AImage.Width * AImage.Height ); ++ i )
                    {
                        // A R G B -> Gray
                        uint8_t AGrayPixel = uint8_t( 0.21 * float( AInPtr[ 1 ] ) + 0.72 * float( AInPtr[ 2 ] ) + 0.07 * float( AInPtr[ 3 ] ) + 0.5 );
                        if( i & 1 )
                            *AOutPtr ++ |= AGrayPixel & 0xF0;

                        else
                            *AOutPtr = AGrayPixel >> 4;

                        ++ AOutPtr;
                        AInPtr += 4;
                    }
                    ACallback( &AOutBufffer );
                    break;
                }

//                YUY2,
//                UYVY,
//                YV12,
//                IYUV,
                case TImageFormat::RGB565 :
                {
                    TStoredImageBuffer AOutBufffer( TImageFormat::Gray4, AImage.Width, AImage.Height );
                    uint8_t *AInPtr = AImage.Data;
                    uint8_t *AOutPtr = AOutBufffer.Data;
                    for( int32_t i = 0; i < int32_t( AImage.Width * AImage.Height ); ++ i )
                    {
            		    uint32_t AColor = Func::ColorFrom565( ( uint16_t( AInPtr[ 1 ] ) << 8 ) | AInPtr[ 0 ] );

                        // R G B -> Gray
                        uint8_t AGrayPixel = uint8_t( 0.21 * float( AColor >> 16 ) + 0.72 * float( ( AColor >> 8 ) & 0xFF ) + 0.07 * float( AColor & 0xFF ) + 0.5 );
                        if( i & 1 )
                            *AOutPtr ++ |= AGrayPixel & 0xF0;

                        else
                            *AOutPtr = AGrayPixel >> 4;

                        ++ AOutPtr;
                        AInPtr += 3;
                    }

                    ACallback( &AOutBufffer );
                    break;
                }

                case TImageFormat::RGB555 :
                {
                    TStoredImageBuffer AOutBufffer( TImageFormat::Gray4, AImage.Width, AImage.Height );
                    uint8_t *AInPtr = AImage.Data;
                    uint8_t *AOutPtr = AOutBufffer.Data;
                    for( int32_t i = 0; i < int32_t( AImage.Width * AImage.Height ); ++ i )
                    {
            		    uint32_t AColor = Func::ColorFrom555( ( uint16_t( AInPtr[ 1 ] ) << 8 ) | AInPtr[ 0 ] );

                        // R G B -> Gray
                        uint8_t AGrayPixel = uint8_t( 0.21 * float( AColor >> 16 ) + 0.72 * float( ( AColor >> 8 ) & 0xFF ) + 0.07 * float( AColor & 0xFF ) + 0.5 );
                        if( i & 1 )
                            *AOutPtr ++ |= AGrayPixel & 0xF0;

                        else
                            *AOutPtr = AGrayPixel >> 4;

                        ++ AOutPtr;
                        AInPtr += 3;
                    }

                    ACallback( &AOutBufffer );
                    break;
                }

                case TImageFormat::GrayScale :
                {
                    TStoredImageBuffer AOutBufffer( TImageFormat::Gray4, AImage.Width, AImage.Height );
                    uint8_t *AOutPtr = AOutBufffer.Data;
                    for( int32_t i = 0; i < int32_t( AImage.Width * AImage.Height ); ++ i )
                    {
                        if( i & 1 )
                            *AOutPtr ++ |= AImage.Data[ i ] & 0xF0;

                        else
                            *AOutPtr = AImage.Data[ i ] >> 4;

                    }

                    ACallback( &AOutBufffer );
                    break;
                }

                case TImageFormat::Gray16 :
                {
                    TStoredImageBuffer AOutBufffer( TImageFormat::Gray4, AImage.Width, AImage.Height );
                    uint8_t *AInPtr = AImage.Data;
                    uint8_t *AOutPtr = AOutBufffer.Data;
                    for( int32_t i = 0; i < int32_t( AImage.Width * AImage.Height ); ++ i )
                    {
                        if( i & 1 )
                            *AOutPtr ++ |= *AInPtr & 0xF0;

                        else
                            *AOutPtr = *AInPtr >> 4;

                        ++ AOutPtr;
                        AInPtr += 2;
                    }

                    ACallback( &AOutBufffer );
                    break;
                }

                case TImageFormat::Gray4 :
                {
                    ACallback( _Data );
                    break;
                }

                case TImageFormat::YUY2 :
                case TImageFormat::UYVY :
                case TImageFormat::YV12 :
                case TImageFormat::IYUV :
                {
                    break;
                }

            }

//			TRGBWColor &AColor= *(TRGBWColor *)_Data;
//			ACallback( (void *)AColor.ToString().c_str());
		}

    };
//---------------------------------------------------------------------------
	typedef Convert_RGBWColorToText Convert_AlphaColorToText;
//---------------------------------------------------------------------------
	template <typename T_OBJECT> class Convert_ObjectToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			T_OBJECT &AColor= *(T_OBJECT *)_Data;
			ACallback( (void *)AColor.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	template <typename T> T ConstrainToList( T AValue, const T AList[], int ASize )
	{
		int AIndex = -1;
		for( int i = 0; i < ASize; ++i )
			if( AValue <= AList[ i ] )
			{
				AIndex = i;
				break;
			}

		if( AIndex == -1 )
			return AList[ ASize - 1 ];

		return AList[ AIndex ];
	}
//---------------------------------------------------------------------------
	class InterruptLock
	{
	public:
		InterruptLock()
		{
#ifndef VISUINO_FREESOC2
			noInterrupts();
#endif
		}

		~InterruptLock()
		{
#ifndef VISUINO_FREESOC2
			interrupts();
#endif
		}
	};
//---------------------------------------------------------------------------
#if ( ! defined VISUINO_DIGISPARK )
	class StringEx : public Mitov::String
	{
		typedef	Mitov::String	inherited;

	public:
		using inherited::inherited;

		inline StringEx( const char *AStr, int ALength )
		{
			copy( AStr, ALength );
		}

	};
#endif
//---------------------------------------------------------------------------
/*
	template <
		typename T_BASE,
		typename T_Enabled,
		typename T_OutputPin,
		typename T
	> class UnaryOperation :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            T_OutputPin::SetPinValue( T_BASE::CalculateOutput( *(T*)_Data ) );
        }

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_BASE,
		typename T_Enabled,
		typename T_OutputPin,
		typename T
	> class UnaryOperation :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            T_OutputPin::SetPinValue( T_BASE::CalculateOutput( *(T*)_Data ) );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION_CLASS,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_TYPE,
		typename T_Value
	> class UnaryOperation_Param1 :
		public T_Enabled,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
        _V_PROP_( Value )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            T_OutputPin::SetPinValue( T_IMPLEMENTATION_CLASS::CalculateOutput( *(T_TYPE *)_Data, Value() ) );
        }
	};
//---------------------------------------------------------------------------
	template <typename T_BASE> class NullCallChain : public T_BASE
	{
	public:
		static inline bool ChainHasItems() { return false; }

	};
//---------------------------------------------------------------------------
	template <typename T_BASE> class EmbeddedCallChain : public T_BASE
	{
	public:
		static inline bool ChainHasItems() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class ClockPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void ClockPin()
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline ClockPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class ClockPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
	public:
		inline void PinNotify( void *_data )
		{
			T_BASE::Notify( nullptr );
		}

		inline void ClockPin()
		{
			T_BASE::Notify( nullptr );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline ClockPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class GenericPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline void SetPinValueLow()
		{
		}

		inline void SetPinValueHigh()
		{
		}

		template <typename T> inline void SendPinValue( T AValue )
		{
		}

		template <typename T> inline void SetPinValue( T AValue, bool AChangeOnly = false )
		{
		}

		inline GenericPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class GenericPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		template <typename T> inline void SendPinValue( T AValue )
		{
			PinNotify( &AValue );
		}

		inline void SetPinValueLow()
		{
			SetPinValue( false );
		}

		inline void SetPinValueHigh()
		{
			SetPinValue( true );
		}

		template <typename T> inline void SetPinValue( T AValue, bool AChangeOnly = false )
		{
			PinNotify( &AValue );
		}

		inline GenericPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA> class TypedPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		template <typename T> inline void SendPinValue( T AValue )
		{
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline TypedPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE, typename T_DATA> class TypedPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SendValue( T_DATA AValue )
		{
			inherited:: template SendValue<T_DATA>( AValue );
		}

		inline void SendPinValue( T_DATA AValue )
		{
			PinNotify( &AValue );
//			inherited:: template SendValue<T_DATA>( AValue );
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
			inherited:: template SendValue<T_DATA>( AValue );
		}

/*
		inline void SetPinValueLow()
		{
//			SetPinValue( false );
		}

		inline void SetPinValueHigh()
		{
//			SetPinValue( true );
		}
*/
		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline TypedPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
/*
	template <uint32_t C_UNIQUE, typename T_BASE> void TypedPin_EmbeddedPinImplementation<C_UNIQUE, T_BASE, bool>::SetPinValueLow()
    {
		SetPinValue( false );
    }
*/
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA> class TypedPin_NoImplementation_ChangeOnly : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SendPinValue( T_DATA AValue )
		{
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline TypedPin_NoImplementation_ChangeOnly & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE, typename T_DATA> class TypedPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		T_DATA	Value;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		void SetValue( T_DATA AValue, bool AChangeOnly )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::Notify( &Value );
		}

		inline void SendPinValue( T_DATA AValue )
		{
			PinNotify( &AValue );
//			SetPinValue( AValue );
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline TypedPin_EmbeddedPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class TextPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline TextPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class TextPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
			inherited::SendValue( AValue );
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//			inherited::Notify( (char *)AValue.c_str() );
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
			inherited::Notify( (char *)AValue.c_str() );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline TextPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class TextPin_NoImplementation_ChangeOnly : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline TextPin_NoImplementation_ChangeOnly &GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class TextPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		Mitov::String	Value;

	public:
		void SetValue( String &AValue, bool AChangeOnly )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::Notify( (char *)Value.c_str() );
		}

	public:
		inline void PinNotify( void *_data )
		{
			SetValue( (const char *)_data, false );
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//			SetValue( AValue, AChangeOnly );
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline TextPin_EmbeddedPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class DigitalPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
		}

		inline void SetPinValueHigh()
		{
		}

		inline void SetPinValueLow()
		{
		}

		inline bool GetPinValue()
		{
			return false;
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline DigitalPin_NoImplementation & GetInstance()
		{
			return *this;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class DigitalPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			OpenWire::StreamPin<T_BASE>::SendValue( AValue );
		}

		inline void SetPinValueHigh()
		{
			OpenWire::StreamPin<T_BASE>::SendValue( true );
		}

		inline void SetPinValueLow()
		{
			OpenWire::StreamPin<T_BASE>::SendValue( false );
		}

		inline bool GetPinValue()
		{
			return false;
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE, typename T_Value> class DigitalPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>, public T_Value
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	protected:
		_V_PROP_( Value )

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			if( AChangeOnly )
				if( AValue == Value() )
					return;

			Value() = AValue;
			OpenWire::StreamPin<T_BASE>::SendValue( AValue );
		}

		inline void SetPinValueHigh()
		{
			SetPinValue( true );
		}

		inline void SetPinValueLow()
		{
			SetPinValue( false );
		}

		inline bool GetPinValue()
		{
			return Value();
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_EmbeddedPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_PIN, T_PIN &C_PIN> class DigitalPin_DirectPinImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(bool *)_data, false );
		}

		inline void SetPinValueHigh()
		{
			C_PIN.DigitalWrite( true );
		}

		inline void SetPinValueLow()
		{
			C_PIN.DigitalWrite( false );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			C_PIN.DigitalWrite( AValue );
		}

		inline bool GetPinValue()
		{
			return C_PIN.DigitalRead();
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_DirectPinImplementation & GetInstance()
		{
			return *this;
		}

	public:
		inline DigitalPin_DirectPinImplementation()
		{
			C_PIN.SetInitialMode( OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template <uint8_t C_PIN_NUMBER> class DigitalPin_DirectBoardPinImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(bool *)_data, false );
		}

		inline void SetPinValueHigh()
		{
//			Serial.print( C_PIN_NUMBER ); Serial.println( " = True" );
			digitalWrite( C_PIN_NUMBER, HIGH );
		}

		inline void SetPinValueLow()
		{
//			Serial.print( C_PIN_NUMBER ); Serial.println( " = False" );
			digitalWrite( C_PIN_NUMBER, LOW );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
//			Serial.print( C_PIN_NUMBER ); Serial.print( " = " ); Serial.println( AValue );
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

		inline bool GetPinValue()
		{
			pinMode( C_PIN_NUMBER, INPUT );
			bool AValue = ( digitalRead( C_PIN_NUMBER ) != LOW );
			pinMode( C_PIN_NUMBER, OUTPUT );

			return AValue;
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_DirectBoardPinImplementation & GetInstance()
		{
			return *this;
		}

	public:
		inline DigitalPin_DirectBoardPinImplementation()
		{
			pinMode( C_PIN_NUMBER, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class AnalogPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline AnalogPin_NoImplementation & GetInstance()
		{
			return *this;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class AnalogPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			inherited::SendValue( AValue );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class AnalogPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	protected:
		float	Value;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::SendValue( AValue );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_EmbeddedPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <typename T_PIN, T_PIN &C_PIN> class AnalogPin_DirectPinImplementation
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(float *)_data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			C_PIN.AnalogWrite( AValue );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
/*
	public:
		AnalogPin_DirectPinImplementation()
		{
			C_PIN.SetInitialMode( OUTPUT );
		}
*/
	};
//---------------------------------------------------------------------------
	template <uint8_t C_PIN_NUMBER> class AnalogPin_DirectBoardPinImplementation
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(float *)_data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			Analog.Write( C_PIN_NUMBER, AValue );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectBoardPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}

	public:
		AnalogPin_DirectBoardPinImplementation()
		{
			pinMode( C_PIN_NUMBER, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class AnalogPin_NoImplementation_ChangeOnly : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return false;
		}

		inline AnalogPin_NoImplementation_ChangeOnly & GetInstance()
		{
			return *this;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_PIN, T_PIN &C_PIN> class AnalogPin_DirectPinImplementation_ChangeOnly
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			C_PIN.AnalogWrite( AValue );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}

/*
	public:
		AnalogPin_DirectPinImplementation()
		{
			C_PIN.SetInitialMode( OUTPUT );
		}
*/
	};
//---------------------------------------------------------------------------
	template <uint8_t C_PIN_NUMBER> class AnalogPin_DirectBoardPinImplementation_ChangeOnly
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(bool *)_data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			Analog.Write( C_PIN_NUMBER, AValue );
		}

		static inline constexpr bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectBoardPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}

	public:
		AnalogPin_DirectBoardPinImplementation_ChangeOnly()
		{
			pinMode( C_PIN_NUMBER, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	class ResetPinImplementation
	{
	public:
		template <typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelay )
		{
			AInstance->SetResetValueLow();
			delay( ADelay );
			AInstance->SetResetValueHigh();
		}

		template <typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow )
		{
			AInstance->SetResetValueHigh();
			delayMicroseconds( ADelayHigh );
			AInstance->SetResetValueLow();
			delay( ADelayLow );
			AInstance->SetResetValueHigh();
		}

		template <typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow, uint16_t APostDelay )
		{
			AInstance->SetResetValueHigh();
			delayMicroseconds( ADelayHigh );
			AInstance->SetResetValueLow();
			delay( ADelayLow );
			AInstance->SetResetValueHigh();
			delay( APostDelay );
		}

/*
	public:
		template <typename T> static void ExecuteMicrosReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow )
		{
			delayMicroseconds( ADelayHigh );
			AInstance->SetResetValueLow();
			delayMicroseconds( ADelayLow );
			AInstance->SetResetValueHigh();
		}
*/
	};
//---------------------------------------------------------------------------
	class NoResetImplementation
	{
	public:
		template <typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelay )
		{
		}

		template <typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow )
		{
		}

		template <typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow, uint16_t APostDelay )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_BLUE_PIN,
		uint8_t C_GREEN_PIN,
		uint8_t C_RED_PIN,
		typename T_InitialValue
	> class ArduinoRGBLEDModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
			Analog.WriteRaw( C_RED_PIN, int( InitialValue().GetReference().Red ) * PWMRANGE / 255 );
			Analog.WriteRaw( C_GREEN_PIN, int( InitialValue().GetReference().Green ) * PWMRANGE / 255 );
			Analog.WriteRaw( C_BLUE_PIN, int( InitialValue().GetReference().Blue ) * PWMRANGE / 255 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
//			Serial.print( InitialValue().GetValue().ToString()); Serial.print( " " ); Serial.println( AColor.ToString() );
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;

//			Serial.print( InitialValue().GetValue().ToString()); Serial.print( " " ); Serial.println( AColor.ToString() );

			UpdateColor();
		}

	public:
		inline void SystemStart()
		{
//			pinMode( C_RED_PIN, OUTPUT );
//			pinMode( C_GREEN_PIN, OUTPUT );
//			pinMode( C_BLUE_PIN, OUTPUT );
			UpdateColor();
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN
	> class ArduinoInvertedDigitalPWMModule
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Analog.WriteRaw( C_PIN, ( *(bool *)_Data ) ? 0 : PWMRANGE );
//			digitalWrite( C_PIN, ( *(bool *)_Data ) ? LOW : HIGH );
		}

		inline void AnalogInputPin_o_Receive( void *_Data )
		{
			Analog.Write( C_PIN, 1.0 - MitovConstrain( *(float *)_Data, 0.0f, 1.0f ) );
		}

	public:
		inline ArduinoInvertedDigitalPWMModule( float AValue )
		{
			pinMode( C_PIN, OUTPUT );
			Analog.Write( C_PIN, 1.0 - AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN
	> class ArduinoDigitalPWMModule
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Analog.WriteRaw( C_PIN, ( *(bool *)_Data ) ? PWMRANGE : 0 );
//			digitalWrite( C_PIN, ( *(bool *)_Data ) ? HIGH : LOW );
		}

		inline void AnalogInputPin_o_Receive( void *_Data )
		{
			Analog.Write( C_PIN, MitovConstrain( *(float *)_Data, 0.0f, 1.0f ) );
		}

	public:
		inline ArduinoDigitalPWMModule( float AValue )
		{
			pinMode( C_PIN, OUTPUT );
			Analog.Write( C_PIN, AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		typename T_Enabled,
		typename T_InitialValue
	> class ArduinoDigitalModuleEnableInitialValue :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void UpdateOutput()
		{
			digitalWrite( C_PIN, ( Enabled().GetValue() && InitialValue().GetValue() ) ? HIGH : LOW );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( InitialValue() == *(bool *)_Data )
				return;

			InitialValue() = *(bool *)_Data;
			UpdateOutput();
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN, OUTPUT );
			UpdateOutput();
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		typename T_Enabled,
		typename T_InitialValue
	> class ArduinoInvertedDigitalModuleEnableInitialValue :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void UpdateOutput()
		{
			digitalWrite( C_PIN, ( Enabled().GetValue() && InitialValue().GetValue() ) ? LOW : HIGH );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( InitialValue() == *(bool *)_Data )
				return;

			InitialValue() = *(bool *)_Data;
			UpdateOutput();
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN, OUTPUT );
			UpdateOutput();
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_BLUE_PIN,
		uint8_t C_GREEN_PIN,
		uint8_t C_RED_PIN,
		typename T_InitialValue
	> class ArduinoRGBLEDInvertedModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
//			Serial.println( InitialValue().GetValue().ToString());
			Analog.WriteRaw( C_RED_PIN, int( 255 - InitialValue().GetReference().Red ) * PWMRANGE / 255 );
			Analog.WriteRaw( C_GREEN_PIN, int( 255 - InitialValue().GetReference().Green ) * PWMRANGE / 255 );
			Analog.WriteRaw( C_BLUE_PIN, int( 255 - InitialValue().GetReference().Blue ) * PWMRANGE / 255 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

//			Serial.print( InitialValue().GetValue().ToString()); Serial.print( " " ); Serial.println( AColor.ToString() );
			InitialValue() = AColor;
			UpdateColor();
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_False
	{
	public:
		inline float Convert( float AValue )
		{
			return AValue;
		}
	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_True
	{
	public:
		inline float Convert( float AValue )
		{
			return 1 - AValue;
		}
	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_Dynamic_False
	{
	public:
		bool	Inverted = false;

	public:
		float Convert( float AValue )
		{
			if( Inverted )
				return 1 - AValue;

			return AValue;
		}
	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_Dynamic_True
	{
	public:
		bool	Inverted = true;

	public:
		float Convert( float AValue )
		{
			if( Inverted )
				return 1 - AValue;

			return AValue;
		}
	};
//---------------------------------------------------------------------------
	class TElapsedMicros
	{
	protected:
		unsigned long FLastTime;

	public:
		inline void SystemStarted()
		{
			FLastTime = micros();
		}

		inline unsigned long GetNewElapsedTime()
		{
			unsigned long AOldTime = FLastTime;
			FLastTime = micros();
			return ( FLastTime - AOldTime );
		}
	};
//---------------------------------------------------------------------------
	class TElapsedMillis
	{
	protected:
		unsigned long FLastTime;

	public:
		inline void SystemStarted()
		{
			FLastTime = millis();
		}

		inline unsigned long GetNewElapsedTime()
		{
			unsigned long AOldTime = FLastTime;
			FLastTime = millis();
			return ( FLastTime - AOldTime );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_AllPixels,
		typename T_Reversed
	> class MaximLedPixelsReversedProperty :
		public T_AllPixels,
		public T_Reversed
	{
	public:
		_V_PIN_( AllPixels )
		_V_PIN_( Reversed )

//	public:
//		bool FCurrentReversed : 1;

/*
	public:
		MaximLedPixelsReversedProperty() :
			Reversed( false ),
			AllPixels( false ),
			FCurrentReversed( false )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_X,
		typename T_Y
	> class TArduinoXYSize :
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	};
//---------------------------------------------------------------------------
	template <
		typename T_X,
		typename T_Y,
		typename T_Z
	> class TArduinoXYZOffsets :
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class TArduinoBasicEnabledThermometer :
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	public:
		inline bool GetIsEnabled()
		{
			return ( Enabled().GetValue() && T_OutputPin::GetPinIsConnected());
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Value
	> class TArduinoXYZOptionalOffset :
		public T_Enabled,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	class TAnalogTransform_Normal
	{
	public:
		static inline float Transform( float AValue ) { return AValue; }

	};
//---------------------------------------------------------------------------
	class TAnalogTransform_Invert
	{
	public:
		static inline float Transform( float AValue ) { return 1 - AValue; }

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		uint16_t C_RANGE,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class DirectMotorDriverForwardChannel :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
			UpdateOutputs();
		}

	public:
		inline void UpdateOutputs()
		{
			if( Enabled() )
//			{
//				Serial.print( C_PIN ); Serial.print( " - " ); Serial.println( ( InitialValue() * 200 ) + 0.5 );
				Analog.WriteRaw( C_PIN, ( InitialValue() * C_RANGE ) + 0.5 );
//			}
//				Analog.Write( C_PIN, InitialValue() );

			else
				Analog.WriteRaw( C_PIN, 0 );

		}

	public:
		inline void SystemStart()
		{
			pinMode( C_PIN, OUTPUT );
			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for *_Slopped compatibility
	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		uint16_t C_RANGE,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class DirectMotorDriverForwardChannel_Slopped :
		public T_Enabled,
		public T_InitialValue,
		public T_SlopeRun,
		public T_SlopeStop
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )
		_V_PROP_( SlopeRun )
		_V_PROP_( SlopeStop )

	protected:
		float	FCurrentValue = 0.0f;
		unsigned long	FLastTime = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
//			UpdateOutputs();
		}

	public:
		inline void UpdateOutputs()
		{
//			Analog.WriteRaw( C_PIN, ( FCurrentValue * PWMRANGE ) + 0.5 );
			Analog.Write( C_PIN, ( FCurrentValue * C_RANGE ) + 0.5 );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();

			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros )
		{
			float ATargetValue = ( Enabled() ) ? InitialValue() : 0.5;
			if( FCurrentValue != ATargetValue )
			{
				float ASlope = SlopeRun();
				if( FCurrentValue > ATargetValue )
				{
					if( ATargetValue > 0.5 )
						ASlope = SlopeStop();
				}

				else
				{
					if( ATargetValue < 0.5 )
						ASlope = SlopeStop();
				}

				if( ASlope == 0 )
					FCurrentValue = ATargetValue;

				else
				{
					float ARamp = abs( AElapsedMicros * ASlope / 1000000 );
					if( FCurrentValue < ATargetValue )
					{
						FCurrentValue += ARamp;
						if( FCurrentValue > ATargetValue )
							FCurrentValue = ATargetValue;

					}
					else
					{
						FCurrentValue -= ARamp;
						if( FCurrentValue < ATargetValue )
							FCurrentValue = ATargetValue;

					}
				}

				UpdateOutputs();
			}
		}

	public:
		inline DirectMotorDriverForwardChannel_Slopped()
		{
			pinMode( C_PIN, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address
	> class InterfaceI2C :
		public T_Address
	{
	public:
		_V_PROP_( Address )

	protected:
		inline void Init()
		{
		}

		void WriteRegister8( uint8_t ARegister, uint8_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

		void WriteRegister16( uint8_t ARegister, uint16_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( uint8_t( AValue ));
			C_I2C.write( uint8_t( AValue >> 8 ));
			C_I2C.endTransmission();
		}

		void WriteRegister16_HighFirst( uint8_t ARegister, uint16_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( uint8_t( AValue >> 8 ));
			C_I2C.write( uint8_t( AValue ));
			C_I2C.endTransmission();
		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister | 0x80 );
			C_I2C.endTransmission();

#ifdef VISUINO_ARDUINO_AVR4809
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), size_t( 1 ) );
#else
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ) );
#endif // VISUINO_ARDUINO_AVR4809
			return C_I2C.read();
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister | 0x80 );
			C_I2C.endTransmission();

#ifdef VISUINO_ARDUINO_AVR4809
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), size_t( 2 ) );
#else
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) );
#endif // VISUINO_ARDUINO_AVR4809

			uint16_t AResult = C_I2C.read();

			AResult |= uint16_t( C_I2C.read() ) << 8;

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister | 0x80 );
			C_I2C.endTransmission();
#ifdef VISUINO_ARDUINO_AVR4809
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), size_t( ACount ));
#else
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), ACount );
#endif // VISUINO_ARDUINO_AVR4809
		}

		inline uint8_t ReadUInt8()
		{
			return C_I2C.read();
		}

		inline uint16_t ReadUInt16()
		{
			uint16_t AResult = C_I2C.read();
			return ( AResult | uint16_t( C_I2C.read() ) << 8 );
		}

		inline uint16_t ReadUInt16_HighFirst()
		{
			uint16_t AResult = uint16_t( C_I2C.read() ) << 8;
			return AResult | C_I2C.read();
		}

		inline void EndReadRegisters()
		{
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address
	> class InterfaceI2CNoSetHighBit :
		public T_Address
	{
	public:
		_V_PROP_( Address )

	protected:
		inline void Init()
		{
		}

		void WriteRegister8( uint8_t ARegister, uint8_t value )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( value );
			C_I2C.endTransmission();
		}

		void WriteRegister16( uint8_t ARegister, uint16_t value )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( uint8_t( value ));
			C_I2C.write( uint8_t( value >> 8 ));
			C_I2C.endTransmission();
		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

#ifdef VISUINO_ARDUINO_AVR4809
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), size_t( 1 ) );
#else
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ) );
#endif // VISUINO_ARDUINO_AVR4809

			return C_I2C.read();
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

#ifdef VISUINO_ARDUINO_AVR4809
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), size_t( 2 ) );
#else
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) );
#endif // VISUINO_ARDUINO_AVR4809

			uint16_t AResult = C_I2C.read();

			AResult |= uint16_t( C_I2C.read() ) << 8;

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
#ifdef VISUINO_ARDUINO_AVR4809
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), size_t( ACount ));
#else
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), ACount );
#endif // VISUINO_ARDUINO_AVR4809
		}

		inline uint8_t ReadUInt8()
		{
			return C_I2C.read();
		}

		inline uint16_t ReadUInt16()
		{
			uint16_t AResult = C_I2C.read();
			return ( AResult | uint16_t( C_I2C.read() ) << 8 );
		}

		inline uint16_t ReadUInt16_HighFirst()
		{
			uint16_t AResult = uint16_t( C_I2C.read() ) << 8;
			return AResult | C_I2C.read();
		}

		inline void EndReadRegisters()
		{
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX
	> class I2CSplitterChannel
	{
	public:
		inline void beginTransmission( uint8_t AAddress )
		{
			C_OWNER.SetIndex( C_INDEX );
			C_OWNER.GetI2C().beginTransmission( AAddress );
		}

		inline uint8_t endTransmission(void)
		{
			return C_OWNER.GetI2C().endTransmission();
		}

		inline uint8_t endTransmission( uint8_t AValue )
		{
			return C_OWNER.GetI2C().endTransmission( AValue );
		}

		inline uint8_t requestFrom( uint8_t AAddress, uint8_t ACount )
		{
			C_OWNER.SetIndex( C_INDEX );
			return C_OWNER.GetI2C().requestFrom( AAddress, ACount );
		}

		inline size_t write( uint8_t AValue )
		{
			return C_OWNER.GetI2C().write( AValue );
		}

		inline size_t write(const uint8_t *AValue, size_t ASize )
		{
			return C_OWNER.GetI2C().write( AValue, ASize );
		}

		inline int available(void)
		{
			return C_OWNER.GetI2C().available();
		}

		inline int read(void)
		{
			return C_OWNER.GetI2C().read();
		}

		inline int peek(void)
		{
			return C_OWNER.GetI2C().peek();
		}

		inline void flush(void)
		{
			C_OWNER.GetI2C().flush();
		}

		inline size_t write(unsigned long n)
		{
			return C_OWNER.GetI2C().write( n );
		}

		inline size_t write(long n)
		{
			return C_OWNER.GetI2C().write( n );
		}

		inline size_t write(unsigned int n)
		{
			return C_OWNER.GetI2C().write( n );
		}

		inline size_t write(int n)
		{
			return C_OWNER.GetI2C().write( n );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class I2CDelegatedChannel
	{
	public:
		inline void beginTransmission( uint8_t AAddress )
		{
			C_OWNER.Embedded_I2C_beginTransmission( AAddress );
		}

		inline uint8_t endTransmission(void)
		{
			return C_OWNER.Embedded_I2C_endTransmission();
		}

		inline uint8_t endTransmission( uint8_t AValue )
		{
			return C_OWNER.Embedded_I2C_endTransmission( AValue );
		}

		inline uint8_t requestFrom( uint8_t AAddress, uint8_t ACount )
		{
			return C_OWNER.Embedded_I2C_requestFrom( AAddress, ACount );
		}

		inline size_t write( uint8_t AValue )
		{
			return C_OWNER.Embedded_I2C_write( AValue );
		}

		inline size_t write(const uint8_t *AValue, size_t ASize )
		{
			return C_OWNER.Embedded_I2C_write( AValue, ASize );
		}

		inline int available(void)
		{
			return C_OWNER.Embedded_I2C_available();
		}

		inline int read(void)
		{
			return C_OWNER.Embedded_I2C_read();
		}

		inline int peek(void)
		{
			return C_OWNER.Embedded_I2C_peek();
		}

		inline void flush(void)
		{
			C_OWNER.Embedded_I2C_flush();
		}

		inline size_t write(unsigned long n)
		{
			return C_OWNER.Embedded_I2C_write( n );
		}

		inline size_t write(long n)
		{
			return C_OWNER.Embedded_I2C_write( n );
		}

		inline size_t write(unsigned int n)
		{
			return C_OWNER.Embedded_I2C_write( n );
		}

		inline size_t write(int n)
		{
			return C_OWNER.Embedded_I2C_write( n );
		}
	};
//---------------------------------------------------------------------------
	template <typename T, typename T_CONFIG> class GraphicsPrint : public Print
	{
	public:
		typedef size_t (*TCallback)( T &AImplementation, T_CONFIG&AConfig, uint8_t AChar );

	protected:
		T			*FOwner;
		T_CONFIG	*FConfig;
		TCallback	 FCallback;

	public:
		virtual size_t write( uint8_t AChar ) override
		{
			return FCallback( *FOwner, *FConfig, AChar );
		}

	public:
		inline GraphicsPrint( T *AOwner, T_CONFIG *AConfig, TCallback ACallback ) :
			FOwner( AOwner ),
			FConfig( AConfig ),
			FCallback( ACallback )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T> class CallbackPrint : public Print
	{
	public:
		typedef size_t (*TCallback)( T &AImplementation, const uint8_t *buf, size_t size );

	protected:
		T			*FOwner;
		TCallback	 FCallback;

	public:
		virtual size_t write( uint8_t AChar ) override
		{
			return write( &AChar, 1 );
		}

		virtual size_t write( const uint8_t *buf, size_t size ) override
		{
			return FCallback( *FOwner, buf, size );
		}

	public:
		inline CallbackPrint( T *AOwner, TCallback ACallback ) :
			FOwner( AOwner ),
			FCallback( ACallback )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T> class VirtualStreamOut : public Stream
	{
	public:
		typedef size_t (*TCallback)( T *AOwner, uint8_t AChar );

	protected:
		T			*FOwner;
		TCallback	 FCallback;

	public:
		virtual size_t write( uint8_t AChar ) override
		{
//			Serial.println( "write" );
			return FCallback( FOwner, AChar );
		}

	public:
		int available() override
		{
			return false;
		}

		int peek() override
		{
			return -1;
		}

		int read() override
		{
			return -1;
		}

		void flush()
#ifndef VISUINO_K210
			override
#endif // !VISUINO_K210
		{
		}

	public:
		inline VirtualStreamOut( T *AOwner, TCallback ACallback ) :
			FOwner( AOwner ),
			FCallback( ACallback )
		{
		}

	};
//---------------------------------------------------------------------------
	class Buffered_Stream :
		public Stream
	{
	protected:
		Mitov::SimpleList<uint8_t, uint8_t> FBuffer;

	public:
		typedef Stream & T_STREAM;

	public:
		inline Stream &GetStream()
		{
			return *this;
		}

	public:
		inline void SendToBuffer( uint8_t *AData, uint32_t ALength, uint32_t ABufferSize )
		{
			for( uint32_t i = 0; i < ALength; ++i )
			{
				FBuffer.push_back( AData[ i ] );
				if( FBuffer.size() > ABufferSize )
					FBuffer.pop_front();
			}
		}

	public:
		int available() override
		{
			return FBuffer.size();
		}

		int peek() override
		{
			if( ! FBuffer.size() )
				return -1;

			return * FBuffer.AsPointer();
		}

		int read() override
		{
			if( ! FBuffer.size() )
				return -1;

			int AResult = * FBuffer.AsPointer();
			FBuffer.pop_front();
			return AResult;
		}

		void flush()
#ifndef VISUINO_K210
			override
#endif // !VISUINO_K210
		{
			FBuffer.clear();
		}

	};
//---------------------------------------------------------------------------
	template <typename T_SERVER, typename T_CLIENT> class AcceptWrapper : public T_SERVER
	{
	public:
		T_CLIENT accept() { return T_SERVER::available(); }

	public:
		using T_SERVER::T_SERVER;
	};
//---------------------------------------------------------------------------
	template <typename T_SERVER, typename T_CLIENT> class AvailableAcceptWrapper : public T_SERVER
	{
	public:
		T_CLIENT accept() { return T_SERVER::available(); }

	public:
	    inline operator bool() { return true; }

	public:
		using T_SERVER::T_SERVER;

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Value
	> class TArduinoRTCAlarmElement :
		public T_Enabled,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	class CompareType_Implementation_Equal
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			return ( ACurrentValue == AValue );
		}
	};
//---------------------------------------------------------------------------
	typedef CompareType_Implementation_Equal CompareType_Implementation_ctEqual;
//---------------------------------------------------------------------------
	class CompareType_Implementation_NotEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			return ( ACurrentValue != AValue );
		}
	};
//---------------------------------------------------------------------------
	typedef CompareType_Implementation_NotEqual CompareType_Implementation_ctNotEqual;
//---------------------------------------------------------------------------
	class CompareType_Implementation_Bigger
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			return ( ACurrentValue > AValue );
		}
	};
//---------------------------------------------------------------------------
	typedef CompareType_Implementation_Bigger CompareType_Implementation_ctBigger;
//---------------------------------------------------------------------------
	class CompareType_Implementation_Smaller
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			return ( ACurrentValue < AValue );
		}
	};
//---------------------------------------------------------------------------
	typedef CompareType_Implementation_Smaller CompareType_Implementation_ctSmaller;
//---------------------------------------------------------------------------
	class CompareType_Implementation_BiggerOrEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			return ( ACurrentValue >= AValue );
		}
	};
//---------------------------------------------------------------------------
	typedef CompareType_Implementation_BiggerOrEqual CompareType_Implementation_ctBiggerOrEqual;
//---------------------------------------------------------------------------
	class CompareType_Implementation_ctSmallerOrEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			return ( ACurrentValue <= AValue );
		}
	};
//---------------------------------------------------------------------------
	typedef CompareType_Implementation_Bigger CompareType_Implementation_ctBigger;
//---------------------------------------------------------------------------
	class DisplayOrientationUp
	{
	public:
		inline static TArduinoGraphicsOrientation ConvertOrientation( const TArduinoGraphicsOrientation AValue ) { return AValue; }

	};
//---------------------------------------------------------------------------
	class DisplayOrientationDown
	{
	public:
		inline static TArduinoGraphicsOrientation ConvertOrientation( const TArduinoGraphicsOrientation AValue ) { return TArduinoGraphicsOrientation( ( uint8_t( AValue ) + 2 ) & 0b11 ); }

	};
//---------------------------------------------------------------------------
	class DisplayOrientationLeft
	{
	public:
		inline static TArduinoGraphicsOrientation ConvertOrientation( const TArduinoGraphicsOrientation AValue ) { return TArduinoGraphicsOrientation( ( uint8_t( AValue ) + 3 ) & 0b11 ); }

	};
//---------------------------------------------------------------------------
	class DisplayOrientationRight
	{
	public:
		inline static TArduinoGraphicsOrientation ConvertOrientation( const TArduinoGraphicsOrientation AValue ) { return TArduinoGraphicsOrientation( ( uint8_t( AValue ) + 1 ) & 0b11 ); }

	};
//---------------------------------------------------------------------------
	class NullDisplayOverridePixelOffset
	{
	public:
		inline bool Enabled() { return false; }
		inline uint8_t X() { return 0; }
		inline uint8_t Y() { return 0; }

	};
//---------------------------------------------------------------------------
} // Mitov

namespace ComponentsHardware
{
	extern void SystemUpdateHardware();
} // ComponentsHardware
//---------------------------------------------------------------------------


