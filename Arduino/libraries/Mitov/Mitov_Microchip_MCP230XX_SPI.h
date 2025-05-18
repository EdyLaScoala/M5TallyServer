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
		typename T_SPI, T_SPI &C_SPI,
		typename T_Address,
		typename T_ChipSelectOutputPin,
		typename T_SPISpeed
	> class InterfaceSPISpeed_MCP230XX :
		public T_Address,
		public T_ChipSelectOutputPin,
		public T_SPISpeed
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( Address )
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
			C_SPI.transfer( uint8_t( Address().GetValue() ) << 1 );
			C_SPI.transfer( ARegister );
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
			C_SPI.transfer( uint8_t( Address().GetValue() ) << 1 );
			C_SPI.transfer( ARegister );
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
			C_SPI.transfer( ( uint8_t( Address().GetValue() ) << 1 ) | 1 );
			C_SPI.transfer( ARegister );
			uint8_t AValue = C_SPI.transfer( 0 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AValue;
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ( uint8_t( Address().GetValue() ) << 1 ) | 1 );
			C_SPI.transfer( ARegister );

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
			C_SPI.transfer( ( uint8_t( Address().GetValue() ) << 1 ) | 1 );
			C_SPI.transfer( ARegister );
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
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif