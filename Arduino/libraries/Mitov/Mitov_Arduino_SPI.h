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
	template <
		typename T_SPI,
		T_SPI &C_SPI
	> class ArduinoBasicSPI
	{
	public:
		inline uint16_t transfer16(uint16_t data)
		{
			return C_SPI.transfer16( data );
		}

		inline uint8_t transfer(uint8_t data)
		{
//			Serial.println( data, HEX );
			return C_SPI.transfer( data );
		}

		inline void transfer(void *buf, size_t count)
		{
#if defined(VISUINO_ESP8266) || defined( VISUINO_ANY_ESP32 )
			C_SPI.writeBytes( (uint8_t*)buf, count );
#else // VISUINO_ESP8266
			C_SPI.transfer( buf, count );
#endif // VISUINO_ESP8266
		}

		inline void beginTransaction(SPISettings settings)
		{
		#if defined(SPI_HAS_TRANSACTION)
			C_SPI.beginTransaction( settings );
		#endif
		}

		inline void endTransaction()
		{
		#if defined(SPI_HAS_TRANSACTION)
			C_SPI.endTransaction();
		#endif
		}

#if ( defined (__AVR__) || defined (__SAM3X8E__))
		inline void setDataMode( uint8_t dataMode )
		{
			C_SPI.setDataMode( dataMode );
		}

/*
		inline void setBitOrder( uint8_t bitOrder )
		{
			C_SPI.setBitOrder( bitOrder );
		}
*/
		inline void setClockDivider( uint8_t clockDiv )
		{
			C_SPI.setClockDivider( clockDiv );
		}
#endif
		inline void usingInterrupt( uint8_t interruptNumber )
		{
#if ! ( defined(VISUINO_ESP8266) || defined( VISUINO_ANY_ESP32 ) || defined(VISUINO_STMDUINO) )
			C_SPI.usingInterrupt( interruptNumber );
#endif
		}

	public:
		inline void SystemInit()
		{
			C_SPI.begin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin
	> class InterfaceSPI :
		public T_ChipSelectOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	protected:
		inline void Init()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void WriteRegister8( uint8_t ARegister, uint8_t value )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( value );
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister16( uint8_t ARegister, uint16_t value )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( uint8_t( value ));
			C_SPI.transfer( uint8_t( value >> 8 ));
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low
			uint8_t AValue = C_SPI.transfer( 0 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AValue;
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low

			uint16_t AResult = C_SPI.transfer( 0 );
			AResult |= uint16_t( C_SPI.transfer( 0 ) ) << 8;

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 | 0x40 ); // write, bit 7 low
		}

		inline uint8_t ReadUInt8()
		{
			return C_SPI.transfer( 0xFF );
		}

		void EndReadRegisters()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_SPISpeed
	> class InterfaceSPISpeed :
		public T_ChipSelectOutputPin,
		public T_SPISpeed
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( SPISpeed )

	protected:
		inline void Init()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void WriteRegister8( uint8_t ARegister, uint8_t value )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( value );
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister16( uint8_t ARegister, uint16_t value )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( uint8_t( value ));
			C_SPI.transfer( uint8_t( value >> 8 ));
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low
			uint8_t AValue = C_SPI.transfer( 0 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AValue;
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low

			uint16_t AResult = C_SPI.transfer( 0 );
			AResult |= uint16_t( C_SPI.transfer( 0 ) ) << 8;

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 /*| 0x40 */ ); // read, bit 7 high
		}

		inline uint8_t ReadUInt8()
		{
			return C_SPI.transfer( 0xFF );
		}

		inline uint16_t ReadUInt16()
		{
			uint16_t AResult = C_SPI.transfer( 0xFF );

			return ( AResult | uint16_t( C_SPI.transfer( 0xFF ) ) << 8 );
		}

		void EndReadRegisters()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}
	};
//---------------------------------------------------------------------------
	template <SPIClass &C_SPI> class ArduinoSPI :
		public ArduinoBasicSPI<SPIClass, C_SPI>
	{
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
