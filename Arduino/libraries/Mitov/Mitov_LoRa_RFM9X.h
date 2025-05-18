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

//#define ___MITOV_LORA_RFM9X_DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TRFM9XGainControl { gcAuto, gcG1, gcG2, gcG3, gcG4, gcG5, gcG6 };
	enum TRFM9XCodingRate { cr4_5, cr4_6, cr4_7, cr4_8 };
//---------------------------------------------------------------------------
/*
	class MitovBasicLoRaRFM9X
	{
	public:
		#include <Mitov_LoRa_RFM9X_Functions.h>

	public:
		MitovBasicLoRaRFM9X() :
			SymbolTimeout( 0x64 ),
			LoRaTMMode( true ),
			MobileNode( false ),
			LowNoiseAmplifierHighFrequencyBoost( false ),
			Gain( gcAuto ),
			CodingRate( cr4_5 ),
			SpreadingFactorOrder( 7 ),
			FReady( false ),
			FModeChange( false ),
			FMode( RHModeInitialising )
		{
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_CodingRate,
		typename T_FMode,
		typename T_FModeChange,
		typename T_FReady,
		typename T_Frequency,
		typename T_Gain,
		uint16_t C_InterruptInputPin,
		typename T_LoRaTMMode,
		typename T_LowNoiseAmplifierHighFrequencyBoost,
		typename T_MaxSendQueue,
		typename T_MobileNode,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_ResetOutputPin,
		typename T_SignalBandwidth,
		typename T_SignalStrengthOutputPin,
		typename T_SpreadingFactorOrder,
		typename T_SymbolTimeout
	> class MitovLoRaRFM9X_Interrupt : // public OpenWire::InterruptObject,
		public T_ChipSelectOutputPin,
		public T_CodingRate,
		public T_FMode,
		public T_FModeChange,
		public T_FReady,
		public T_Frequency,
		public T_Gain,
		public T_LoRaTMMode,
		public T_LowNoiseAmplifierHighFrequencyBoost,
		public T_MaxSendQueue,
		public T_MobileNode,
		public T_OutputPin,
		public T_Power,
		public T_PreambleLength,
		public T_ResetOutputPin,
		public T_SignalBandwidth,
		public T_SignalStrengthOutputPin,
		public T_SpreadingFactorOrder,
		public T_SymbolTimeout
	{
		#include <Mitov_LoRa_RFM9X_Functions.h>

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( ResetOutputPin )

	protected:
		void Reset()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();

			if( T_ResetOutputPin::GetPinIsConnected() )
			{
//				Serial.println( "RESET" );
				T_ResetOutputPin::SetPinValueLow();
				delay( 100 );
				T_ResetOutputPin::SetPinValueHigh();
				delay( 100 );
			}
		}

		uint8_t spiRead(uint8_t reg)
		{
			uint8_t val;
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(reg & ~0x80); // Send the address with the write mask off
			val = C_SPI.transfer(0); // The written value is ignored, reg value is read
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
//Serial.print( "spiRead: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			return val;
		}

		uint8_t spiWrite(uint8_t reg, uint8_t val)
		{
//Serial.print( "spiWrite: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			uint8_t status = C_SPI.transfer(reg | 0x80); // Send the address with the write mask on
			C_SPI.transfer(val); // New value follows
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return status;
		}

	protected:
		void readFifo( uint8_t ALength )
		{
//			Serial.print( "LN: " ); Serial.println( ALength );
			// Reset the fifo read ptr to the beginning of the packet
			spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));

			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer(RH_RF95_REG_00_FIFO); // Send the start address with the write mask off

			uint8_t *ABuffer = new uint8_t[ 1 + ALength ];
			FReceivedPackets.push_back( ABuffer );
			*ABuffer++ = ALength; // Skip the ARxHeaderTo

			for ( int i = 0; i < ALength; ++i )
				*ABuffer++ = C_SPI.transfer(0);

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void TransferPacket( const uint8_t* data, uint8_t len )
		{
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( RH_RF95_REG_00_FIFO | 0x80 ); // Send the start address with the write mask on
//			C_SPI.transfer( len + 1 ); // Include length of headers
			// First the 4 headers
//			C_SPI.transfer( ATxHeaderTo );
//			C_SPI.transfer( Address );
//			C_SPI.transfer(_txHeaderId);
//			C_SPI.transfer(_txHeaderFlags);
			// Now the payload
			while( len-- )
				C_SPI.transfer( *data++ );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

	public:
//#ifdef _MITOV_CONNECTED_LORA_RFM9X_INTERRUPT_PIN_
		// C++ level interrupt handler for this instance
		// RH_RF69 is unusual in that it has several interrupt lines, and not a single, combined one.
		// On Moteino, only one of the several interrupt lines (DI0) from the RH_RF69 is connnected to the processor.
		// We use this to get PACKETSDENT and PAYLOADRADY interrupts.
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool )
		{
			// Read the interrupt register
			uint8_t irq_flags = spiRead(RH_RF95_REG_12_IRQ_FLAGS);
			if ( FMode() == RHModeRx && irq_flags & (RH_RF95_RX_TIMEOUT | RH_RF95_PAYLOAD_CRC_ERROR))
				; //_rxBad++;

			else if ( FMode() == RHModeRx && irq_flags & RH_RF95_RX_DONE)
			{
				// Have received a packet
				uint8_t len = spiRead(RH_RF95_REG_13_RX_NB_BYTES);

				// Reset the fifo read ptr to the beginning of the packet
//				spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
//				spiBurstRead(RH_RF95_REG_00_FIFO, _buf, len);
//				_bufLen = len;
//				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags

				// Remember the RSSI of this packet
				// this is according to the doc, but is it really correct?
				// weakest receiveable signals are reported RSSI at about -66
				FLastRssi = spiRead(RH_RF95_REG_1A_PKT_RSSI_VALUE) - 137;

				readFifo( len );

				// We have received a message.
//				validateRxBuf();
//				if (_rxBufValid)
					setModeIdle(); // Got one
			}

			else if ( FMode() == RHModeTx && irq_flags & RH_RF95_TX_DONE)
			{
//				_txGood++;
				setModeIdle();
			}

			spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
		}
//#endif // _MITOV_CONNECTED_LORA_RFM9X_INTERRUPT_PIN_

	protected:
		void ProcessInterrupt()
		{
		}

	public:
		inline void SystemInit()
		{
			// Do in startup for M0 compatibility! (Fails in constructor!)
			int AInterruptNumber = digitalPinToInterrupt( C_InterruptInputPin );
			C_SPI.usingInterrupt( AInterruptNumber );
			IntSystemInit();
		}

	public:
		inline MitovLoRaRFM9X_Interrupt()
		{
			FReady() = false;
			FModeChange() = false;
			FMode() = RHModeInitialising;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_CodingRate,
		typename T_FMode,
		typename T_FModeChange,
		typename T_FReady,
		typename T_Frequency,
		typename T_Gain,
		typename T_LoRaTMMode,
		typename T_LowNoiseAmplifierHighFrequencyBoost,
		typename T_MaxSendQueue,
		typename T_MobileNode,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_ResetOutputPin,
		typename T_SignalBandwidth,
		typename T_SignalStrengthOutputPin,
		typename T_SpreadingFactorOrder,
		typename T_SymbolTimeout
	> class MitovLoRaRFM9X :
		public T_ChipSelectOutputPin,
		public T_CodingRate,
		public T_FMode,
		public T_FModeChange,
		public T_FReady,
		public T_Frequency,
		public T_Gain,
		public T_LoRaTMMode,
		public T_LowNoiseAmplifierHighFrequencyBoost,
		public T_MaxSendQueue,
		public T_MobileNode,
		public T_OutputPin,
		public T_Power,
		public T_PreambleLength,
		public T_ResetOutputPin,
		public T_SignalBandwidth,
		public T_SignalStrengthOutputPin,
		public T_SpreadingFactorOrder,
		public T_SymbolTimeout
	{
		#include <Mitov_LoRa_RFM9X_Functions.h>

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( ResetOutputPin )

	protected:
		void Reset()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();

			if( T_ResetOutputPin::GetPinIsConnected() )
			{
//				Serial.println( "RESET" );
				T_ResetOutputPin::SetPinValueLow();
				delay( 100 );
				T_ResetOutputPin::SetPinValueHigh();
				delay( 100 );
			}
		}

		uint8_t spiRead(uint8_t reg)
		{
			uint8_t val;
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(reg & ~0x80); // Send the address with the write mask off
			val = C_SPI.transfer(0); // The written value is ignored, reg value is read
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
//Serial.print( "spiRead: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			return val;
		}

		uint8_t spiWrite(uint8_t reg, uint8_t val)
		{
//Serial.print( "spiWrite: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			uint8_t status = C_SPI.transfer(reg | 0x80); // Send the address with the write mask on
			C_SPI.transfer(val); // New value follows
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return status;
		}

	protected:
		void readFifo( uint8_t ALength )
		{
//			Serial.print( "LN: " ); Serial.println( ALength );
			// Reset the fifo read ptr to the beginning of the packet
			spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));

			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer(RH_RF95_REG_00_FIFO); // Send the start address with the write mask off

			uint8_t *ABuffer = new uint8_t[ 1 + ALength ];
			FReceivedPackets.push_back( ABuffer );
			*ABuffer++ = ALength; // Skip the ARxHeaderTo

			for ( int i = 0; i < ALength; ++i )
				*ABuffer++ = C_SPI.transfer(0);

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void TransferPacket( const uint8_t* data, uint8_t len )
		{
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( RH_RF95_REG_00_FIFO | 0x80 ); // Send the start address with the write mask on
//			C_SPI.transfer( len + 1 ); // Include length of headers
			// First the 4 headers
//			C_SPI.transfer( ATxHeaderTo );
//			C_SPI.transfer( Address );
//			C_SPI.transfer(_txHeaderId);
//			C_SPI.transfer(_txHeaderFlags);
			// Now the payload
			while( len-- )
				C_SPI.transfer( *data++ );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

   	protected:
		void ProcessInterrupt()
		{
			// Read the interrupt register
			uint8_t irq_flags = spiRead(RH_RF95_REG_12_IRQ_FLAGS);

			if ( FMode() == RHModeRx && irq_flags & (RH_RF95_RX_TIMEOUT | RH_RF95_PAYLOAD_CRC_ERROR))
			{
				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
				; //_rxBad++;
			}

			else if ( FMode() == RHModeRx && irq_flags & RH_RF95_RX_DONE)
			{
				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
				// Have received a packet
				uint8_t len = spiRead(RH_RF95_REG_13_RX_NB_BYTES);

				// Reset the fifo read ptr to the beginning of the packet
//				spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
//				spiBurstRead(RH_RF95_REG_00_FIFO, _buf, len);
//				_bufLen = len;
//				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags

				// Remember the RSSI of this packet
				// this is according to the doc, but is it really correct?
				// weakest receiveable signals are reported RSSI at about -66
				FLastRssi = spiRead(RH_RF95_REG_1A_PKT_RSSI_VALUE) - 137;

				readFifo( len );

				// We have received a message.
//				validateRxBuf();
//				if (_rxBufValid)
					setModeIdle(); // Got one
			}

			else if ( FMode() == RHModeTx && irq_flags & RH_RF95_TX_DONE)
			{
				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
//				_txGood++;
				setModeIdle();
			}
		}

	public:
		inline void SystemInit()
		{
			IntSystemInit();
		}

	public:
		inline MitovLoRaRFM9X()
		{
			FReady() = false;
			FModeChange() = false;
			FMode() = RHModeInitialising;
		}

	};
//---------------------------------------------------------------------------
	template <
		int PIN_CS, int PIN_RST, int PIN_INT,
		typename T_CodingRate,
		typename T_FMode,
		typename T_FModeChange,
		typename T_FReady,
		typename T_Frequency,
		typename T_Gain,
		typename T_LoRaTMMode,
		typename T_LowNoiseAmplifierHighFrequencyBoost,
		typename T_MaxSendQueue,
		typename T_MobileNode,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_SignalBandwidth,
		typename T_SignalStrengthOutputPin,
		typename T_SpreadingFactorOrder,
		typename T_SymbolTimeout
	> class MitovLoRaRFM9XShield_Interrupt :
		public T_CodingRate,
		public T_FMode,
		public T_FModeChange,
		public T_FReady,
		public T_Frequency,
		public T_Gain,
		public T_LoRaTMMode,
		public T_LowNoiseAmplifierHighFrequencyBoost,
		public T_MaxSendQueue,
		public T_MobileNode,
		public T_OutputPin,
		public T_Power,
		public T_PreambleLength,
		public T_SignalBandwidth,
		public T_SignalStrengthOutputPin,
		public T_SpreadingFactorOrder,
		public T_SymbolTimeout
	{
		#include <Mitov_LoRa_RFM9X_Functions.h>

	protected:
		static MitovLoRaRFM9XShield_Interrupt<
				PIN_CS, PIN_RST, PIN_INT,
				T_CodingRate,
				T_FMode,
				T_FModeChange,
				T_FReady,
				T_Frequency,
				T_Gain,
				T_LoRaTMMode,
				T_LowNoiseAmplifierHighFrequencyBoost,
				T_MaxSendQueue,
				T_MobileNode,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_SignalBandwidth,
				T_SignalStrengthOutputPin,
				T_SpreadingFactorOrder,
				T_SymbolTimeout
			>	*FInstnce;

	protected:
		static void StaticInterruptHandler()
		{
			FInstnce->InterruptHandler( true );
		}

	protected:
		void Reset()
		{
			digitalWrite( PIN_CS, HIGH );

//				Serial.println( "RESET" );
			digitalWrite( PIN_RST, LOW );
			delay( 100 );
			digitalWrite( PIN_RST, HIGH );
			delay( 100 );
		}

		void readFifo( uint8_t ALength )
		{
//			Serial.print( "LN: " ); Serial.println( ALength );

			// Reset the fifo read ptr to the beginning of the packet
			spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));

			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );

			SPI.transfer(RH_RF95_REG_00_FIFO); // Send the start address with the write mask off

			uint8_t *ABuffer = new uint8_t[ 1 + ALength ];
			FReceivedPackets.push_back( ABuffer );
			*ABuffer++ = ALength; // Skip the ARxHeaderTo

			for ( int i = 0; i < ALength; ++i )
				*ABuffer++ = SPI.transfer(0);

			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
		}

		void TransferPacket( const uint8_t* data, uint8_t len )
		{
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			SPI.transfer( RH_RF95_REG_00_FIFO | 0x80 ); // Send the start address with the write mask on
//			SPI.transfer( len + 1 ); // Include length of headers
			// First the 4 headers
//			SPI.transfer( ATxHeaderTo );
//			SPI.transfer( Address );
//			SPI.transfer(_txHeaderId);
//			SPI.transfer(_txHeaderFlags);
			// Now the payload
			while( len-- )
				SPI.transfer( *data++ );

			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
		}

		uint8_t spiRead(uint8_t reg)
		{
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			SPI.transfer(reg & ~0x80); // Send the address with the write mask off
			uint8_t val = SPI.transfer(0); // The written value is ignored, reg value is read
			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
//Serial.print( "spiRead: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			return val;
		}

		uint8_t spiWrite(uint8_t reg, uint8_t val)
		{
//Serial.print( "spiWrite: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			uint8_t status = SPI.transfer(reg | 0x80); // Send the address with the write mask on
			SPI.transfer(val); // New value follows
			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
			return status;
		}

	protected:
		void ProcessInterrupt()
		{
		}

	public:
//#ifdef _MITOV_CONNECTED_LORA_RFM9X_INTERRUPT_PIN_
		// C++ level interrupt handler for this instance
		// RH_RF69 is unusual in that it has several interrupt lines, and not a single, combined one.
		// On Moteino, only one of the several interrupt lines (DI0) from the RH_RF69 is connnected to the processor.
		// We use this to get PACKETSDENT and PAYLOADRADY interrupts.
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool )
		{
			// Read the interrupt register
			uint8_t irq_flags = spiRead(RH_RF95_REG_12_IRQ_FLAGS);
			if ( FMode() == RHModeRx && irq_flags & (RH_RF95_RX_TIMEOUT | RH_RF95_PAYLOAD_CRC_ERROR))
				; //_rxBad++;

			else if ( FMode() == RHModeRx && irq_flags & RH_RF95_RX_DONE)
			{
				// Have received a packet
				uint8_t len = spiRead(RH_RF95_REG_13_RX_NB_BYTES);

				// Reset the fifo read ptr to the beginning of the packet
//				spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
//				spiBurstRead(RH_RF95_REG_00_FIFO, _buf, len);
//				_bufLen = len;
//				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags

				// Remember the RSSI of this packet
				// this is according to the doc, but is it really correct?
				// weakest receiveable signals are reported RSSI at about -66
				FLastRssi = spiRead(RH_RF95_REG_1A_PKT_RSSI_VALUE) - 137;

				readFifo( len );

				// We have received a message.
//				validateRxBuf();
//				if (_rxBufValid)
					setModeIdle(); // Got one
			}
			else if ( FMode() == RHModeTx && irq_flags & RH_RF95_TX_DONE)
			{
//				_txGood++;
				setModeIdle();
			}

			spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
		}
//#endif // _MITOV_CONNECTED_LORA_RFM9X_INTERRUPT_PIN_

	public:
		inline void SystemInit()
		{
			SPI.begin();
			int AInterruptNumber = digitalPinToInterrupt( PIN_INT );
			attachInterrupt( AInterruptNumber, StaticInterruptHandler, RISING );
#ifndef VISUINO_ANY_ESP32
			SPI.usingInterrupt( AInterruptNumber );
#endif
			IntSystemInit();
		}

	public:
		inline MitovLoRaRFM9XShield_Interrupt()
		{
			FReady() = false;
			FModeChange() = false;
			FMode() = RHModeInitialising;

			FInstnce = this;
			pinMode( PIN_RST, OUTPUT );
			pinMode( PIN_CS, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template <
		int PIN_CS, int PIN_RST, int PIN_INT,
		typename T_CodingRate,
		typename T_FMode,
		typename T_FModeChange,
		typename T_FReady,
		typename T_Frequency,
		typename T_Gain,
		typename T_LoRaTMMode,
		typename T_LowNoiseAmplifierHighFrequencyBoost,
		typename T_MaxSendQueue,
		typename T_MobileNode,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_SignalBandwidth,
		typename T_SignalStrengthOutputPin,
		typename T_SpreadingFactorOrder,
		typename T_SymbolTimeout
	> MitovLoRaRFM9XShield_Interrupt<
			PIN_CS, PIN_RST, PIN_INT,
			T_CodingRate,
			T_FMode,
			T_FModeChange,
			T_FReady,
			T_Frequency,
			T_Gain,
			T_LoRaTMMode,
			T_LowNoiseAmplifierHighFrequencyBoost,
			T_MaxSendQueue,
			T_MobileNode,
			T_OutputPin,
			T_Power,
			T_PreambleLength,
			T_SignalBandwidth,
			T_SignalStrengthOutputPin,
			T_SpreadingFactorOrder,
			T_SymbolTimeout
		>	*MitovLoRaRFM9XShield_Interrupt<
			PIN_CS, PIN_RST, PIN_INT,
			T_CodingRate,
			T_FMode,
			T_FModeChange,
			T_FReady,
			T_Frequency,
			T_Gain,
			T_LoRaTMMode,
			T_LowNoiseAmplifierHighFrequencyBoost,
			T_MaxSendQueue,
			T_MobileNode,
			T_OutputPin,
			T_Power,
			T_PreambleLength,
			T_SignalBandwidth,
			T_SignalStrengthOutputPin,
			T_SpreadingFactorOrder,
			T_SymbolTimeout
		>::FInstnce = nullptr;
//---------------------------------------------------------------------------
	template <
		int PIN_CS,
		int PIN_RST,
		typename T_CodingRate,
		typename T_FMode,
		typename T_FModeChange,
		typename T_FReady,
		typename T_Frequency,
		typename T_Gain,
		typename T_LoRaTMMode,
		typename T_LowNoiseAmplifierHighFrequencyBoost,
		typename T_MaxSendQueue,
		typename T_MobileNode,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_SignalBandwidth,
		typename T_SignalStrengthOutputPin,
		typename T_SpreadingFactorOrder,
		typename T_SymbolTimeout
	> class MitovLoRaRFM9XShield :
		public T_CodingRate,
		public T_FMode,
		public T_FModeChange,
		public T_FReady,
		public T_Frequency,
		public T_Gain,
		public T_LoRaTMMode,
		public T_LowNoiseAmplifierHighFrequencyBoost,
		public T_MaxSendQueue,
		public T_MobileNode,
		public T_OutputPin,
		public T_Power,
		public T_PreambleLength,
		public T_SignalBandwidth,
		public T_SignalStrengthOutputPin,
		public T_SpreadingFactorOrder,
		public T_SymbolTimeout
	{
		#include <Mitov_LoRa_RFM9X_Functions.h>

	protected:
		void Reset()
		{
			digitalWrite( PIN_CS, HIGH );

//				Serial.println( "RESET" );
			digitalWrite( PIN_RST, LOW );
			delay( 100 );
			digitalWrite( PIN_RST, HIGH );
			delay( 100 );
		}

		void readFifo( uint8_t ALength )
		{
//			Serial.print( "LN: " ); Serial.println( ALength );

			// Reset the fifo read ptr to the beginning of the packet
			spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));

			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );

			SPI.transfer(RH_RF95_REG_00_FIFO); // Send the start address with the write mask off

			uint8_t *ABuffer = new uint8_t[ 1 + ALength ];
			FReceivedPackets.push_back( ABuffer );
			*ABuffer++ = ALength; // Skip the ARxHeaderTo

			for ( int i = 0; i < ALength; ++i )
				*ABuffer++ = SPI.transfer(0);

			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
		}

		void TransferPacket( const uint8_t* data, uint8_t len )
		{
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			SPI.transfer( RH_RF95_REG_00_FIFO | 0x80 ); // Send the start address with the write mask on
//			SPI.transfer( len + 1 ); // Include length of headers
			// First the 4 headers
//			SPI.transfer( ATxHeaderTo );
//			SPI.transfer( Address );
//			SPI.transfer(_txHeaderId);
//			SPI.transfer(_txHeaderFlags);
			// Now the payload
			while( len-- )
				SPI.transfer( *data++ );

			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
		}

		uint8_t spiRead(uint8_t reg)
		{
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			SPI.transfer(reg & ~0x80); // Send the address with the write mask off
			uint8_t val = SPI.transfer(0); // The written value is ignored, reg value is read
			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
//Serial.print( "spiRead: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			return val;
		}

		uint8_t spiWrite(uint8_t reg, uint8_t val)
		{
//Serial.print( "spiWrite: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			uint8_t status = SPI.transfer(reg | 0x80); // Send the address with the write mask on
			SPI.transfer(val); // New value follows
			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
			return status;
		}

	protected:
		void ProcessInterrupt()
		{
			// Read the interrupt register
			uint8_t irq_flags = spiRead(RH_RF95_REG_12_IRQ_FLAGS);

			if ( FMode() == RHModeRx && irq_flags & (RH_RF95_RX_TIMEOUT | RH_RF95_PAYLOAD_CRC_ERROR))
			{
				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
				; //_rxBad++;
			}

			else if ( FMode() == RHModeRx && irq_flags & RH_RF95_RX_DONE)
			{
				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
				// Have received a packet
				uint8_t len = spiRead(RH_RF95_REG_13_RX_NB_BYTES);

				// Reset the fifo read ptr to the beginning of the packet
//				spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
//				spiBurstRead(RH_RF95_REG_00_FIFO, _buf, len);
//				_bufLen = len;
//				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags

				// Remember the RSSI of this packet
				// this is according to the doc, but is it really correct?
				// weakest receiveable signals are reported RSSI at about -66
				FLastRssi = spiRead(RH_RF95_REG_1A_PKT_RSSI_VALUE) - 137;

				readFifo( len );

				// We have received a message.
//				validateRxBuf();
//				if (_rxBufValid)
					setModeIdle(); // Got one
			}

			else if ( FMode().GetValue() == RHModeTx && irq_flags & RH_RF95_TX_DONE)
			{
				spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
//				_txGood++;
				setModeIdle();
			}
		}

	public:
		inline void SystemInit()
		{
			SPI.begin();
			IntSystemInit();
		}

	public:
		inline MitovLoRaRFM9XShield()
		{
			FReady() = false;
			FModeChange() = false;
			FMode() = RHModeInitialising;

			pinMode( PIN_RST, OUTPUT );
			pinMode( PIN_CS, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif