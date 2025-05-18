////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SPI.h>
#include <Mitov.h>
#include <Mitov_Memory.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace ArduinoFlash_SPI_Consts
	{
		const uint32_t	SECTOR_SIZE = 4 * 1024;

		const uint8_t OPCODE_READ				= 0x03;	// Read Memory
		const uint8_t OPCODE_WRITE				= 0x02;	// Write Memory
		const uint8_t OPCODE_WRITE_ENABLE		= 0x06;
		const uint8_t OPCODE_READ_STATUS_1		= 0x05;
		const uint8_t OPCODE_READ_STATUS_2		= 0x35;
		const uint8_t OPCODE_WRITE_STATUS		= 0x01;
		const uint8_t OPCODE_ERASE_SECTOR_4K	= 0x20;
		const uint8_t OPCODE_ERASE_BLOCK_32K	= 0x52;
		const uint8_t OPCODE_POWER_DOWN			= 0xB9;
		const uint8_t OPCODE_POWER_UP_ID		= 0xAB; // Power Up and ID
		const uint8_t OPCODE_ERASE_BLOCK_64K	= 0xD8;
		const uint8_t OPCODE_CHIP_ERASE			= 0xC7; // (0x60)
	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Reverse,
		typename T_Size,
		typename T_Upper
	> class TArduinoFlashWriteProtect :
		public T_Enabled,
		public T_Reverse,
		public T_Size,
		public T_Upper
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reverse )
		_V_PROP_( Size )
		_V_PROP_( Upper )

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_AddressBytes,
		typename T_ChipSelectOutputPin,
		typename T_Enabled,
		typename T_PowerDown,
		typename T_SPISpeed,
		typename T_WriteProtect,
		typename T_WriteProtectedOutputPin
	> class ArduinoGenericFlash_SPI :
		public T_AddressBytes,
		public T_ChipSelectOutputPin,
		public T_Enabled,
		public T_PowerDown,
		public T_SPISpeed,
		public T_WriteProtect,
		public T_WriteProtectedOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( WriteProtectedOutputPin )

	public:
		_V_PROP_( AddressBytes )
		_V_PROP_( Enabled )
		_V_PROP_( PowerDown )
		_V_PROP_( SPISpeed )
		_V_PROP_( WriteProtect )

	public:
		void UpdatePowerDown()
		{
			if( PowerDown() )
				SendCommand( ArduinoFlash_SPI_Consts::OPCODE_POWER_DOWN );

			else
			{
				SendCommand( ArduinoFlash_SPI_Consts::OPCODE_POWER_UP_ID );
				delayMicroseconds( 3 );
			}
		}

		void UpdateWriteProtectReverse()
		{
			if( WriteProtect().Reverse() )
				SetRegister( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_2, 0b01000000 );

			else
				SetRegister( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_2, 0b00000000 );

		}

		void UpdateWriteProtect()
		{
			if( ! WriteProtect().Enabled() )
				SetRegister( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_1, 0 );

			else
			{
//				uint8_t AValue = ( WriteProtect().Reverse() ) ? 0 : 0b100000;
				uint8_t AValue = ( WriteProtect().Upper() ) ? 0 : 0b100000;

				if( WriteProtect().Size() < 8 )
					AValue |= 0b01000100;

				else if( WriteProtect().Size() < 16 )
					AValue |= 0b01001000;

				else if( WriteProtect().Size() < 32 )
					AValue |= 0b01001100;

				else if( WriteProtect().Size() < 128 )
					AValue |= 0b01010000;

				else if( WriteProtect().Size() < 256 )
					AValue |= 0b00000100;

				else if( WriteProtect().Size() < 512 )
					AValue |= 0b00001000;

				else if( WriteProtect().Size() < 1024 )
					AValue |= 0b00001100;

				else if( WriteProtect().Size() < 2048 )
					AValue |= 0b00010000;

				else if( WriteProtect().Size() < 4096 )
					AValue |= 0b00010100;

				else if( WriteProtect().Size() < 8192 )
					AValue |= 0b00011000;

				else
					AValue |= 0b00011100;

				SetRegister( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_1, AValue );
			}
		}

	protected:
		uint8_t ReadByte( uint8_t ACommand )
		{
//			Serial.print( "ReadByte: " );
//			Serial.print( ACommand, HEX );
//			Serial.print( " - " );

			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );

			C_SPI.transfer( ACommand );
			uint8_t AResult = C_SPI.transfer(0);

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();

//			Serial.println( AResult, BIN );
			return AResult;
		}

		void WaitBusy()
		{
//			Serial.println( "WaitBusy" );
//			Serial.println( ReadByte( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_1 ), BIN );

			// Wait for BUSY flag to clear!
			while( ReadByte( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_1 ) & 1 )
				;
		}

		void WriteEnableWait()
		{

			uint8_t AValue;
			// Wait for BUSY flag to clear!
			do
			{
				AValue = ReadByte( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_1 );
			}
			while( AValue & 1 );

			if( ( AValue & 0b10 ) == 0 )
			{
//				Serial.println( "OPCODE_WRITE_ENABLE" );
				SendCommand( ArduinoFlash_SPI_Consts::OPCODE_WRITE_ENABLE );
			}

		}

		void PerformAddressWriteOperation( uint8_t ACommand, uint32_t AAddress )
		{
			WriteEnableWait();

			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );
			C_SPI.transfer( ACommand );

			for( int i = AddressBytes().GetValue(); i--; )
				C_SPI.transfer( uint8_t(AAddress >> ( i * 8 )) );

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void SetRegister( uint8_t ACommand, uint8_t AData )
		{
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );
			C_SPI.transfer( ACommand );
			C_SPI.transfer( AData );

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void SendCommand( uint8_t ACommand )
		{
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );
			C_SPI.transfer( ACommand );

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}


	public:
		bool ReadData( uint32_t AAddress, size_t ASize, uint8_t *AData )
		{
//			Serial.print( "ReadData: " );
//			Serial.print( AAddress );
//			Serial.print( " " );
//			Serial.println( ASize );
			WaitBusy();

			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );

			C_SPI.transfer( ArduinoFlash_SPI_Consts::OPCODE_READ );

			for( int i = AddressBytes().GetValue(); i--; )
				C_SPI.transfer( uint8_t(AAddress >> ( i * 8 )) );

			for( int i = 0; i < ASize; ++i )
				*AData ++ = C_SPI.transfer(0);

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();

			return true;
		}

		bool WriteData( uint32_t AAddress, size_t ASize, uint8_t *AData )
		{
/*
			Serial.println( "" );
			Serial.println( "-------" );
			float ATest = *(float *)AData;
			Serial.println( ATest );

			Serial.println( "???????????" );

			ReadData( AAddress, 4, (uint8_t *)&ATest );

			Serial.println( ATest );
*/
//			Serial.print( "WriteData: " );
//			Serial.print( AAddress );
//			Serial.print( " " );
//			Serial.println( ASize );

//			Serial.println( ReadByte( ArduinoFlash_SPI_Consts::OPCODE_READ_STATUS_1 ), BIN );

			WriteEnableWait();
//			return true;

			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0) );
			C_SPI.transfer( ArduinoFlash_SPI_Consts::OPCODE_WRITE );

			for( int i = AddressBytes().GetValue(); i--; )
				C_SPI.transfer( uint8_t(AAddress >> ( i * 8 )) );

			for( int i = 0; i < ASize; ++i )
			{
//				Serial.println( *AData, HEX );
				C_SPI.transfer( *AData ++ );
			}

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();



//			ReadData( AAddress, 4, (uint8_t *)&ATest );

//			Serial.println( ATest );
//			Serial.println( "******" );
//			Serial.println( "" );

			return true;
		}

		bool CheckBlankToEndOfSector( uint32_t AAddress )
		{
//			Serial.println( "CheckBlankToEndOfSector" );
			WaitBusy();

//			Serial.println( "WaitBusy 1" );

			uint32_t ABaseAddress = ( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE ) * ArduinoFlash_SPI_Consts::SECTOR_SIZE;
			uint32_t AEndAddress = ABaseAddress + ArduinoFlash_SPI_Consts::SECTOR_SIZE;

//			Serial.print( "CheckBlankToEndOfSector: " );
//			Serial.print( ABaseAddress );
//			Serial.print( " " );
//			Serial.println( AEndAddress );

			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );

			C_SPI.transfer( ArduinoFlash_SPI_Consts::OPCODE_READ );

			for( int i = AddressBytes().GetValue(); i--; )
				C_SPI.transfer( uint8_t( AAddress >> ( i * 8 )) );

			bool ANeedsErase = false;
			for( uint32_t i = AAddress; i < AEndAddress; ++i )
			{
				uint8_t AValue = C_SPI.transfer(0);
//				Serial.println( AValue, HEX );
				if( AValue != 0xFF )
				{
					ANeedsErase = true;
					break;
				}
			}

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();

			if( ANeedsErase )
				EraseSector_4K( ABaseAddress );

			return ANeedsErase;
		}

		inline void EraseSector_4K( uint32_t AAddress )
		{
//			Serial.print( "EraseSector_4K: " );
//			Serial.println( AAddress );

//			return;
			PerformAddressWriteOperation( ArduinoFlash_SPI_Consts::OPCODE_ERASE_SECTOR_4K, ( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE ) * ArduinoFlash_SPI_Consts::SECTOR_SIZE );
		}

		inline void EraseBlock_32K( uint32_t AAddress )
		{
			PerformAddressWriteOperation( ArduinoFlash_SPI_Consts::OPCODE_ERASE_BLOCK_32K, ( AAddress / ( 32 * 1024 ) ) * ( 32 * 1024 ) );
		}

		inline void EraseBlock_64K( uint32_t AAddress )
		{
			PerformAddressWriteOperation( ArduinoFlash_SPI_Consts::OPCODE_ERASE_BLOCK_64K, ( AAddress / ( 64 * 1024 ) ) * ( 64 * 1024 ) );
		}

	public:
		inline void SystemStart()
		{
			UpdatePowerDown();
			UpdateWriteProtectReverse();
			UpdateWriteProtect();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address,
		typename T_Size
	> class ArduinoEraseFlashBlock :
		public T_Address,
		public T_Size
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Size )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( Size().GetValue() < 32 )
				C_OWNER.EraseSector_4K( Address().GetValue() );

			else if( Size().GetValue() < 64 )
				C_OWNER.EraseBlock_32K( Address().GetValue() );

			else
				C_OWNER.EraseBlock_64K( Address().GetValue() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FWriteMoved,
		typename T_FirstBlock,
		typename T_IgnoreReadSize,
		typename T_Loop,
		typename T_NumberBlocks,
		typename T_ReadPosition,
		typename T_ReadPositionOutputPin,
		typename T_RemainingReadSize,
		typename T_RemainingReadSizeOutputPin,
		typename T_StartReadPosition,
		typename T_StartReadPositionOutputPin,
		typename T_StoreStateOutputPin,
		typename T_StoredSize,
		typename T_StoredSizeOutputPin,
//		typename T_SwapBlocks,
		typename T_WritePosition,
		typename T_WritePositionOutputPin
	> class ArduinoFlashDataStorageBlock :
		public T_FWriteMoved,
		public T_FirstBlock,
		public T_IgnoreReadSize,
		public T_Loop,
		public T_NumberBlocks,
		public T_ReadPosition,
		public T_ReadPositionOutputPin,
		public T_RemainingReadSize,
		public T_RemainingReadSizeOutputPin,
		public T_StartReadPosition,
		public T_StartReadPositionOutputPin,
		public T_StoreStateOutputPin,
		public T_StoredSize,
		public T_StoredSizeOutputPin,
//		public T_SwapBlocks,
		public T_WritePosition,
		public T_WritePositionOutputPin
	{
		typedef ArduinoFlashDataStorageBlock T_SELF;

	public:
		_V_PIN_( StoreStateOutputPin )

		_V_PIN_( StartReadPositionOutputPin )
		_V_PIN_( ReadPositionOutputPin )
		_V_PIN_( WritePositionOutputPin )
		_V_PIN_( StoredSizeOutputPin )
		_V_PIN_( RemainingReadSizeOutputPin )

	public:
		_V_PROP_( IgnoreReadSize )
		_V_PROP_( Loop )
		_V_PROP_( FirstBlock )
		_V_PROP_( NumberBlocks )
		_V_PROP_( StoredSize )
		_V_PROP_( RemainingReadSize )
//		_V_PROP_( SwapBlocks )
		_V_PROP_( ReadPosition )
		_V_PROP_( StartReadPosition )
		_V_PROP_( WritePosition )

	protected:
		_V_PROP_( FWriteMoved )

	protected:
		void UpdateReadPosition( uint32_t AValue )
		{
//			Serial.println( "UpdateReadPosition" );
//			Serial.println( AValue );

			TStorageStateAccess AStateAccess( true, TStorageStateAccess::eReadPosition, AValue );
			T_StoreStateOutputPin::PinNotify( &AStateAccess );

			ReadPosition() = AValue;
			ReadPositionOutputPin().SetPinValue( AValue, false );
		}

		void UpdateStartReadPosition( uint32_t AValue )
		{
//			Serial.println( "UpdateStartReadPosition" );
//			Serial.println( AValue );

			TStorageStateAccess AStateAccess( true, TStorageStateAccess::eStartReadPosition, AValue );
			T_StoreStateOutputPin::PinNotify( &AStateAccess );

			StartReadPosition() = AValue;
			StartReadPositionOutputPin().SetPinValue( AValue, false );
		}

		void UpdateWritePosition( uint32_t AValue )
		{
//			Serial.println( "UpdateWritePosition" );
//			Serial.println( AValue );

//			StoredSize() = StoredSize().GetValue() + AValue;
			TStorageStateAccess AStateAccess( true, TStorageStateAccess::eWritePosition, AValue );
			T_StoreStateOutputPin::PinNotify( &AStateAccess );

			WritePosition() = AValue;
			WritePositionOutputPin().SetPinValue( AValue, false );
		}

		void UpdateStoredSize( uint32_t AValue )
		{
//			Serial.println( "UpdateStoredSize" );
//			Serial.println( AValue );

			TStorageStateAccess AStateAccess( true, TStorageStateAccess::eSize, AValue );
			T_StoreStateOutputPin::PinNotify( &AStateAccess );

			StoredSize() = AValue;
			StoredSizeOutputPin().SetPinValue( AValue, false );
		}

		void UpdateRemainingReadSize( uint32_t AValue )
		{
//			Serial.println( "UpdateRemainingReadSize" );
//			Serial.println( AValue );

			TStorageStateAccess AStateAccess( true, TStorageStateAccess::eRemainingReadSize, AValue );
			T_StoreStateOutputPin::PinNotify( &AStateAccess );

			RemainingReadSize() = AValue;
			RemainingReadSizeOutputPin().SetPinValue( AValue, false );
		}

	public:
		inline void UpdateStoredSizeOutput()
		{
			StoredSizeOutputPin().SetPinValue( StoredSize(), false );
		}

		inline void UpdateReadPositionOutput()
		{
			ReadPositionOutputPin().SetPinValue( ReadPosition(), false );
		}

		inline void UpdateStartReadPositionOutput()
		{
			StartReadPositionOutputPin().SetPinValue( ReadPosition(), false );
		}

		inline void UpdateWritePositionOutput()
		{
			WritePositionOutputPin().SetPinValue( WritePosition(), false );
		}

		inline void UpdateRemainingReadSizeOutput()
		{
			RemainingReadSizeOutputPin().SetPinValue( WritePosition(), false );
		}

	protected:
		inline uint32_t GetByteSize()
		{
			return NumberBlocks().GetValue() * 4 * 1024;
		}

		inline uint32_t GetStartAddress()
		{
			return FirstBlock().GetValue() * 4 * 1024;
		}

		inline bool ReadDataDirect( uint32_t AAddress, size_t ASize, uint8_t *AData )
		{
			return C_OWNER.ReadData( AAddress, ASize, AData );
		}

		void CheckBlankToEndOfSector( uint32_t AAddress )
		{
//			Serial.println( "CheckBlankToEndOfSector" );

//			Serial.println( StoredSize().GetValue() );
			if( C_OWNER.CheckBlankToEndOfSector( AAddress ))
			{
				uint32_t AAddressOffset = AAddress - GetStartAddress();

//				Serial.println( "TEST1" );
				if( StoredSize().GetValue() > 0 )
				{
//					Serial.println( "TEST2" );
/*
					if( ( StartReadPosition().GetValue() >= AAddressOffset ) && ( StartReadPosition().GetValue() < AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE ) )
					{
//						Serial.println( "TEST3" );
						uint32_t ACorrectionSize = ( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE ) - StartReadPosition().GetValue();
						if( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE == ( FirstBlock().GetValue() + NumberBlocks().GetValue() ) * ArduinoFlash_SPI_Consts::SECTOR_SIZE )
							UpdateStartReadPosition( 0 );

						else
							UpdateStartReadPosition( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE );

						UpdateStoredSize( TotalStoredSize().GetValue() - ACorrectionSize );
					}
*/
					if( ( StartReadPosition().GetValue() >= AAddressOffset ) && ( StartReadPosition().GetValue() < AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE ) )
					{
//						Serial.println( "TEST3_3" );
						uint32_t ACorrectionSize = ( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE ) - StartReadPosition().GetValue();
						if( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE == ( FirstBlock().GetValue() + NumberBlocks().GetValue() ) * ArduinoFlash_SPI_Consts::SECTOR_SIZE )
							UpdateStartReadPosition( 0 );

						else
							UpdateStartReadPosition( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE );

						UpdateStoredSize( StoredSize().GetValue() - ACorrectionSize );
					}

				}

				if( RemainingReadSize().GetValue() > 0 )
				{
					if( ( ReadPosition().GetValue() >= AAddressOffset ) && ( ReadPosition().GetValue() < AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE ) )
					{
//						Serial.println( "TEST3" );

						uint32_t ACorrectionSize = ( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE ) - ReadPosition().GetValue();
						if( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE == ( FirstBlock().GetValue() + NumberBlocks().GetValue() ) * ArduinoFlash_SPI_Consts::SECTOR_SIZE )
							UpdateReadPosition( 0 );

						else
							UpdateReadPosition( AAddressOffset + ArduinoFlash_SPI_Consts::SECTOR_SIZE );

						UpdateRemainingReadSize( RemainingReadSize().GetValue() - ACorrectionSize );
					}
				}
			}
		}

		inline bool WriteDataDirect( uint32_t AAddress, size_t ASize, uint8_t *AData )
		{
//			Serial.println( "WriteDataDirect" );
//			Serial.println( ASize );
			if( FWriteMoved() )
				CheckBlankToEndOfSector( AAddress );

			while( AAddress + ASize > ( ( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE) * ArduinoFlash_SPI_Consts::SECTOR_SIZE ) + ArduinoFlash_SPI_Consts::SECTOR_SIZE )
			{
//				Serial.println( ( ( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE) * ArduinoFlash_SPI_Consts::SECTOR_SIZE ) + ArduinoFlash_SPI_Consts::SECTOR_SIZE );
				if( AAddress == ( ( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE) * ArduinoFlash_SPI_Consts::SECTOR_SIZE ))
					CheckBlankToEndOfSector( AAddress );

				uint32_t ANewAddress = (( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE) * ArduinoFlash_SPI_Consts::SECTOR_SIZE ) + ArduinoFlash_SPI_Consts::SECTOR_SIZE;

//				Serial.print( "TT3 " );
//				Serial.print( ANewAddress );
//				Serial.print( " - " );
//				Serial.println( AAddress );

				uint32_t ADiffernceSize = ANewAddress - AAddress;
				C_OWNER.WriteData( AAddress, ADiffernceSize, AData );
				ASize -= ADiffernceSize;
				AData += ADiffernceSize;
				AAddress = ANewAddress;
			}

			if( ASize )
			{
				if( AAddress == ( ( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE) * ArduinoFlash_SPI_Consts::SECTOR_SIZE ) )
				{
//					Serial.print( "TEST1: " );
//					Serial.print( ( AAddress / ArduinoFlash_SPI_Consts::SECTOR_SIZE) * ArduinoFlash_SPI_Consts::SECTOR_SIZE );
//					Serial.print( " - " );
//					Serial.println( AAddress );

					CheckBlankToEndOfSector( AAddress );
				}

				C_OWNER.WriteData( AAddress, ASize, AData );
			}

			return true;
		}

	protected:
		inline size_t ReadBytesInternal( uint8_t *AData, size_t ASize, bool AUpdatePosition )
		{
//			Serial.println( "ReadBytesInternal" );
//			Serial.println( RemainingReadSize().GetValue() );

			if( ! IgnoreReadSize().GetValue() )
				if( RemainingReadSize().GetValue() < ASize )
					return 0;

			if( ( ReadPosition() + ASize ) > GetByteSize() )
				if( ASize < GetByteSize() )
				{
					if( AUpdatePosition )
					{
						if( Loop() )
							UpdateReadPosition( 0 );

					}

					else
						return 0;

				}

			if( ( ReadPosition() + ASize ) <= GetByteSize() )
				if( ReadDataDirect( GetStartAddress() + ReadPosition().GetValue(), ASize, AData ))
				{
//					memcpy( AData, FBuffer + ReadPosition().GetValue(), ASize );
					if( AUpdatePosition )
					{
						UpdateRemainingReadSize( RemainingReadSize().GetValue() - ASize );
						UpdateReadPosition( ReadPosition() + ASize );
					}

					return ASize;
				}

			return 0;
		}

	public:
		inline void BeginRead() {} // Placeholder!
		inline bool TryOpen() { return true; }
		inline bool DataAvailable()
		{
			if( ! IgnoreReadSize().GetValue() )
//				if( ! StoredSize().GetValue() )
				if( ! RemainingReadSize().GetValue() )
					return false;

			return true;
		}

		inline void SeekRead( uint32_t AValue )
		{
			ReadPosition() = AValue;
//			FReadMoved() = true;
		}

		inline void SeekWrite( uint32_t AValue )
		{
			if( WritePosition() == AValue )
				return;

			FWriteMoved() = true;
			WritePosition() = AValue;
//			FWriteMoved() = true;
		}

	public:
		inline int Read()
		{
			if( ! IgnoreReadSize().GetValue() )
				if( ! RemainingReadSize().GetValue() )
					return -1;

//			if( ! TryOpen() )
//				return -1;

//			Serial.println( "Read" );

			uint8_t AChar;
			if( ReadBytes( &AChar, 1 ) < 1 )
				return -1;

			return AChar;
		}

		inline int Peek()
		{
			if( ! IgnoreReadSize().GetValue() )
				if( ! RemainingReadSize().GetValue() )
					return -1;

//			if( ! TryOpen() )
//				return -1;

//			Serial.println( "Peek" );

			uint8_t AChar;
			if( ! ReadBytesInternal( &AChar, 1, false ))
				return -1;

			return AChar;
		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
//			Serial.println( "write" );
//			Serial.println( AValue );
			AImplementation.Write( &AValue, 1 );
			return 1;
		}

		template <typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template <typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_SELF::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template <typename T> inline size_t print( T AValue )
		{
			return print<T_SELF, T>( this, AValue );
		}

		template <typename T> inline size_t println( T AValue )
		{
			return println<T_SELF, T>( this, AValue );
		}

	public:
		void Write( void *AData, size_t ASize )
		{
//			Serial.println( "Write" );
//			Serial.println( WritePosition().GetValue() );

			if( ( WritePosition() + ASize ) > GetByteSize() )
				if( ASize < GetByteSize() )
					if( Loop() )
						UpdateWritePosition( 0 );

			if( ( WritePosition() + ASize ) <= GetByteSize() )
			{
//				Serial.println( "STEP 1" );
//				memcpy( FBuffer + WritePosition().GetValue(), AData, ASize );
//				C_OWNER.WriteData( C_Address + WritePosition().GetValue(), ASize, (uint8_t *)AData );
				if( WriteDataDirect( GetStartAddress() + WritePosition().GetValue(), ASize, (uint8_t *)AData ))
				{
//					Serial.println( "STEP 2" );
					UpdateStoredSize( StoredSize().GetValue() + ASize );
					UpdateRemainingReadSize( RemainingReadSize().GetValue() + ASize );
					UpdateWritePosition( WritePosition() + ASize );
				}

//				Serial.println( "STEP EXIT" );
			}
		}

		inline size_t ReadBytes( uint8_t *AData, size_t ASize )
		{
			return ReadBytesInternal( AData, ASize, true );
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "ResetInputPin_o_Receive" );

			UpdateReadPosition( 0 );
			UpdateWritePosition( 0 );
			UpdateStoredSize( 0 );
			UpdateStartReadPosition( 0 );
			UpdateRemainingReadSize( 0 );
		}

		inline void RestartReadInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "RestartReadInputPin_o_Receive" );
//			Serial.println( StoredSize().GetValue() );
//			Serial.println( StartReadPosition().GetValue() );

			UpdateRemainingReadSize( StoredSize().GetValue() );
			UpdateReadPosition( StartReadPosition().GetValue() );
		}

	public:
		inline void SystemInit()
		{
//			Serial.println( "SystemInit" );

			{
				TStorageStateAccess AStateAccess( false, TStorageStateAccess::eStartReadPosition, ReadPosition().GetValue() );
				T_StoreStateOutputPin::PinNotify( &AStateAccess );
				StartReadPosition() = AStateAccess.Value;

//				Serial.print( "eStartReadPosition: " );
//				Serial.println( AStateAccess.Value );
			}

			{
				TStorageStateAccess AStateAccess( false, TStorageStateAccess::eReadPosition, ReadPosition().GetValue() );
				T_StoreStateOutputPin::PinNotify( &AStateAccess );
				ReadPosition() = AStateAccess.Value;

//				Serial.print( "eReadPosition: " );
//				Serial.println( AStateAccess.Value );
			}

			{
				TStorageStateAccess AStateAccess( false, TStorageStateAccess::eWritePosition, WritePosition().GetValue() );
				T_StoreStateOutputPin::PinNotify( &AStateAccess );
				WritePosition() = AStateAccess.Value;

//				Serial.print( "eWritePosition: " );
//				Serial.println( AStateAccess.Value );
			}

			{
				TStorageStateAccess AStateAccess( false, TStorageStateAccess::eSize, StoredSize().GetValue() );
				T_StoreStateOutputPin::PinNotify( &AStateAccess );
				StoredSize() = AStateAccess.Value;

//				Serial.print( "eSize: " );
//				Serial.println( AStateAccess.Value );
			}

			{
				TStorageStateAccess AStateAccess( false, TStorageStateAccess::eRemainingReadSize, RemainingReadSize().GetValue() );
				T_StoreStateOutputPin::PinNotify( &AStateAccess );
				RemainingReadSize() = AStateAccess.Value;

//				Serial.print( "eRemainingReadSize: " );
//				Serial.println( AStateAccess.Value );
			}
		}

		inline void SystemStart()
		{
//			delay( 1000 );
//			Serial.println( "SystemStart" );

			UpdateStartReadPositionOutput();
			UpdateReadPositionOutput();
			UpdateWritePositionOutput();
			UpdateStoredSizeOutput();
			UpdateRemainingReadSizeOutput();

			UpdateStoredSizeOutput();
			UpdateReadPositionOutput();
			UpdateWritePositionOutput();
		}

	public:
		inline ArduinoFlashDataStorageBlock()
		{
			FWriteMoved() = false;

//			ReadPosition() = 0;
//			WritePosition() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
