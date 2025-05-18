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

// #define __HTS221__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ChipSelectOutputPin,
		typename T_SPISpeed
	> class TArduinoLSM9DS1_SPI_Settings :
		public T_ChipSelectOutputPin,
		public T_SPISpeed
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( SPISpeed )

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_AccelerometerGyroscopeSPI,
		typename T_CompassSPI
	> class Interface_LSM9DS1_SPI :
		public T_AccelerometerGyroscopeSPI,
		public T_CompassSPI
	{
	public:
		_V_PROP_( AccelerometerGyroscopeSPI )
		_V_PROP_( CompassSPI )

	public:
		inline void Init()
		{
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueHigh();
		}

		void WriteRegister8( uint8_t ARegister, uint8_t value )
		{
			C_SPI.beginTransaction( SPISettings( AccelerometerGyroscopeSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( value );
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();
		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( AccelerometerGyroscopeSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low
			uint8_t AValue = C_SPI.transfer( 0 );
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();

			return AValue;
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( AccelerometerGyroscopeSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueLow();

			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low

			uint16_t AResult = C_SPI.transfer( 0 );
			AResult |= uint16_t( C_SPI.transfer( 0 ) ) << 8;

			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_SPI.beginTransaction( SPISettings( AccelerometerGyroscopeSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 | 0x40 ); // write, bit 7 low
		}

		void WriteCompassRegister8( uint8_t ARegister, uint8_t value )
		{
			C_SPI.beginTransaction( SPISettings( CompassSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			CompassSPI().ChipSelectOutputPin().SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( value );
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			CompassSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();
		}

		uint8_t ReadCompassRegister8( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( CompassSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			CompassSPI().ChipSelectOutputPin().SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low
			uint8_t AValue = C_SPI.transfer( 0 );
			CompassSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();

			return AValue;
		}

		uint16_t ReadCompassRegister16( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( CompassSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			CompassSPI().ChipSelectOutputPin().SetPinValueLow();

			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low

			uint16_t AResult = C_SPI.transfer( 0 );
			AResult |= uint16_t( C_SPI.transfer( 0 ) ) << 8;

			CompassSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();

			return AResult;
		}

		void StartReadCompassRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_SPI.beginTransaction( SPISettings( CompassSPI().SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			CompassSPI().ChipSelectOutputPin().SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 | 0x40 ); // write, bit 7 low
		}

		inline uint8_t ReadUInt8()
		{
			return C_SPI.transfer( 0xFF );
		}

		void EndReadRegisters()
		{
			AccelerometerGyroscopeSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void EndReadCompassRegisters()
		{
			CompassSPI().ChipSelectOutputPin().SetPinValueHigh();
			C_SPI.endTransaction();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

