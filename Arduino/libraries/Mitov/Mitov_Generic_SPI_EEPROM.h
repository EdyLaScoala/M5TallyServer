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
#include <Mitov_Memory.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace ArduinoGenericEEPROM_SPI_Consts
	{
		const uint8_t OPCODE_WREN   = 0b0110;     // Write Enable Latch
		const uint8_t OPCODE_WRDI   = 0b0100;     // Reset Write Enable Latch
		const uint8_t OPCODE_RDSR   = 0b0101;     // Read Status Register
		const uint8_t OPCODE_WRSR   = 0b0001;     // Write Status Register
		const uint8_t OPCODE_READ   = 0b0011;     // Read Memory
		const uint8_t OPCODE_WRITE  = 0b0010;     // Write Memory
//		const uint8_t OPCODE_RDID   = 0b10011111;  // Read Device ID
	};
//---------------------------------------------------------------------------
	enum ArduinoEEPROMProtectedBlocks { pbNone, pbUppeQuarter, pbUppeHalf, pbAll };
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_AddressBytes,
		typename T_ChipSelectOutputPin,
		typename T_Elements_ProcessWrite,
		typename T_Enabled,
		typename T_FCached,
		typename T_ProtectedBlocks,
		typename T_SPISpeed,
		typename T_WriteEnable,
		typename T_WriteProtectedOutputPin
	> class ArduinoGenericEEPROM_SPI :
		public T_AddressBytes,
		public T_ChipSelectOutputPin,
		public T_Elements_ProcessWrite,
		public T_Enabled,
		public T_FCached,
		public T_ProtectedBlocks,
		public T_SPISpeed,
		public T_WriteEnable,
		public T_WriteProtectedOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( AddressBytes )
		_V_PROP_( Enabled )
		_V_PROP_( SPISpeed )
		_V_PROP_( WriteEnable )
		_V_PROP_( ProtectedBlocks )

	protected:
		_V_PROP_( FCached )

		uint32_t	FCachedAddress = 0;
		uint8_t		FCachedValue = 0;

	public:
		void UpdateWriteProtect()
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );
			C_SPI.transfer( WriteEnable() ? ArduinoGenericEEPROM_SPI_Consts::OPCODE_WREN : ArduinoGenericEEPROM_SPI_Consts::OPCODE_WRDI );
			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void UpdateProtectedBlocks()
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );

			C_SPI.transfer( ArduinoGenericEEPROM_SPI_Consts::OPCODE_WREN );
			T_ChipSelectOutputPin::SetPinValueHigh();

			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ArduinoGenericEEPROM_SPI_Consts::OPCODE_WRSR );
			C_SPI.transfer( uint8_t( ProtectedBlocks() ) << 2 );

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	public:
		bool ReadData( uint32_t AAddress, uint32_t ASize, uint8_t *AData )
		{
//			Serial.print( "ReadData : " );
//			Serial.print( AAddress );
//			Serial.print( " = " );
//			Serial.println( ASize );

			if( ! Enabled() )
				return false;

			if( FCached() )
				if( AAddress == FCachedAddress )
				{
					*AData ++ = FCachedValue;
					--ASize;
					++ AAddress;
				}

			if( ASize == 0 )
				return	true;

			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );

			C_SPI.transfer( ArduinoGenericEEPROM_SPI_Consts::OPCODE_READ );

			for( int i = AddressBytes().GetValue(); i--; )
				C_SPI.transfer( uint8_t(AAddress >> ( i * 8 )) );

//			C_SPI.transfer( uint8_t(AAddress >> 8) );
//			C_SPI.transfer( uint8_t(AAddress & 0xFF) );

			for( int i = 0; i < ASize; ++i )
				*AData ++ = C_SPI.transfer(0);

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();

			FCached() = true;
			FCachedAddress = AAddress + ASize - 1;
			FCachedValue = AData[ -1 ];

			return true;
		}

		bool WriteData( uint32_t AAddress, uint32_t ASize, uint8_t *AData )
		{
			if( ! Enabled() )
				return false;

			FCached() = false;

			UpdateWriteProtect();

			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );
			C_SPI.transfer( ArduinoGenericEEPROM_SPI_Consts::OPCODE_WRITE );

			for( int i = AddressBytes().GetValue(); i--; )
				C_SPI.transfer( uint8_t(AAddress >> ( i * 8 )) );

//			C_SPI.transfer( uint8_t(AAddress >> 8));
//			C_SPI.transfer( uint8_t(AAddress & 0xFF));

			for( int i = 0; i < ASize; ++i )
				C_SPI.transfer( *AData ++ );

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
			return true;
		}

	public:
		inline void SystemInit()
		{
			UpdateWriteProtect();
			UpdateProtectedBlocks();
		}

		inline void SystemLoopUpdateHardware()
		{
			if( WriteEnable() )
				T_Elements_ProcessWrite::Call();

		}

		inline void SystemLoopBegin()
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );

			C_SPI.transfer( ArduinoGenericEEPROM_SPI_Consts::OPCODE_RDSR );
			uint8_t Avalue = C_SPI.transfer( 0 );

			T_WriteProtectedOutputPin::SetPinValue( ( Avalue & 0x80 ) != 0 );

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	public:
		inline ArduinoGenericEEPROM_SPI()
		{
			FCached() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif