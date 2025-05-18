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
//---------------------------------------------------------------------------
	enum TArduinoLoRaRFM69Modulation
	{
		mFrequencyShiftKeying_Gaussian_1_0,
		mFrequencyShiftKeying_Gaussian_0_5,
		mFrequencyShiftKeying_Gaussian_0_3,
		mOnOffKeying_NoShaping,
		mOnOffKeying_Cutoff_BitRate,
		mOnOffKeying_Cutoff_2_BitRate
	};
//---------------------------------------------------------------------------
	enum TArduinoLoRaRFM69DCCCutoffFrequency
	{
		rfmDCCCutoff16,
		rfmDCCCutoff8,
		rfmDCCCutoff4,
		rfmDCCCutoff2,
		rfmDCCCutoff1,
		rfmDCCCutoff0_5,
		rfmDCCCutoff0_25,
		rfmDCCCutoff0_125
	};
//---------------------------------------------------------------------------
	enum TArduinoLoRaRFM69ChannelFilterBandwidth
	{
		bfFSK_2_6_OOK_1_3,
		bfFSK_3_1_OOK_1_6,
		bfFSK_3_9_OOK_2_0,
		bfFSK_5_2_OOK_2_6,
		bfFSK_6_3_OOK_3_1,
		bfFSK_7_8_OOK_3_9,
		bfFSK_10_4_OOK_5_2,
		bfFSK_12_5_OOK_6_3,
		bfFSK_15_6_OOK_7_8,
		bfFSK_20_8_OOK_10_4,
		bfFSK_25_0_OOK_12_5,
		bfFSK_31_3_OOK_15_6,
		bfFSK_41_7_OOK_20_8,
		bfFSK_50_0_OOK_25_0,
		bfFSK_62_5_OOK_31_3,
		bfFSK_83_3_OOK_41_7,
		bfFSK_100_0_OOK_50_0,
		bfFSK_125_0_OOK_62_5,
		bfFSK_166_7_OOK_83_3,
		bfFSK_200_0_OOK_100_0,
		bfFSK_250_0_OOK_125_0,
		bfFSK_333_3_OOK_166_7,
		bfFSK_400_0_OOK_200_0,
		bfFSK_500_0_OOK_250_0
	};
//---------------------------------------------------------------------------
	enum TArduinoLoRaRFM69DCFreeEncoding
	{
		dcfeNone,
		dcfeManchester,
		dcfeWhitening
	};
//---------------------------------------------------------------------------
	template <
		typename T_BitRate,
		typename T_CRCEnabled,
		typename T_ChannelFilterBandwidth,
		typename T_DCCCutoffFrequency,
		typename T_DCFreeEncoding,
		typename T_FrequencyDeviation,
		typename T_Modulation
	> class TArduinoLoRaRFM69ModemSettings :
		public T_BitRate,
		public T_CRCEnabled,
		public T_ChannelFilterBandwidth,
		public T_DCCCutoffFrequency,
		public T_DCFreeEncoding,
		public T_FrequencyDeviation,
		public T_Modulation
	{
	public:
		_V_PROP_( BitRate )
		_V_PROP_( CRCEnabled )
		_V_PROP_( ChannelFilterBandwidth )
		_V_PROP_( DCCCutoffFrequency )
		_V_PROP_( DCFreeEncoding )
		_V_PROP_( FrequencyDeviation )
		_V_PROP_( Modulation )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Address,
		typename T_BroadcastAddress,
        typename T_EncryptionKey,
        typename T_EncryptionKey_ApplyValues,
        typename T_EncryptionKey_GetValue,
		typename T_FMode,
		typename T_FModeChange,
		typename T_Frequency,
		typename T_MaxSendQueue,
		typename T_ModemSettings,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_Promiscuous,
		typename T_SentToOutputPin,
		typename T_SignalStrengthOutputPin,
        typename T_SyncWords,
        typename T_SyncWords_ApplyValues,
        typename T_SyncWords_GetValue
	> class MitovBasicLoRaRFM69 : // public OpenWire::InterruptObject,
		public T_Address,
		public T_BroadcastAddress,
        public T_EncryptionKey,
		public T_FMode,
		public T_FModeChange,
		public T_Frequency,
		public T_MaxSendQueue,
		public T_ModemSettings,
		public T_OutputPin,
		public T_Power,
		public T_PreambleLength,
		public T_Promiscuous,
		public T_SentToOutputPin,
		public T_SignalStrengthOutputPin,
        public T_SyncWords
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( SentToOutputPin )
		_V_PIN_( SignalStrengthOutputPin )

	public:
        _V_PROP_( SyncWords )
        _V_PROP_( EncryptionKey )

	public:
		#include <Mitov_LoRa_RFM69_Registers.h>

	protected:
		/// \brief Defines different operating modes for the transport hardware
		///
		/// These are the different values that can be adopted by the _mode variable and
		/// returned by the mode() member function,
		enum RHMode
		{
			RHModeInitialising,			///< Transport is initialising. Initial default value until init() is called..
			RHModeIdle,					///< Transport is idle.
			RHModeFrequencySynthesizer, ///< Frequency Synthesizer.
			RHModeTx,					///< Transport is in the process of transmitting a message.
			RHModeRx					///< Transport is in the process of receiving a message.
		};

	public:
		/// Sets the length of the preamble
		/// in bytes.
		/// Caution: this should be set to the same
		/// value on all nodes in your network. Default is 4.
		/// Sets the message preamble length in REG_0?_PREAMBLE?SB
		/// \param[in] bytes Preamble length in bytes.
		_V_PROP_( PreambleLength )

		/// The selected output power in dBm
		// +13dBm, same as power-on default
		_V_PROP_( Power )
		_V_PROP_( Frequency )
		_V_PROP_( Address )
		_V_PROP_( BroadcastAddress )
		_V_PROP_( MaxSendQueue )

//		ConstBytes	SyncWords;
//		uint8_t* SyncWords = nullptr;
//		uint8_t	SyncWords_Length = 0;

//		uint8_t* EncryptionKey = nullptr;

//		uint8_t* ModemConfig;

/*
		struct
		{
			uint16_t	BitRate = 0x80;
			float		FrequencyDeviation = 0.25;

			TArduinoLoRaRFM69Modulation	Modulation : 3;
			TArduinoLoRaRFM69DCCCutoffFrequency DCCCutoffFrequency : 3;
			TArduinoLoRaRFM69ChannelFilterBandwidth ChannelFilterBandwidth : 5;
			TArduinoLoRaRFM69DCFreeEncoding DCFreeEncoding : 2;
			bool	CRCEnabled : 1;

		} ModemSettings;
*/
		_V_PROP_( ModemSettings )
		_V_PROP_( Promiscuous )

	protected:
		_V_PROP_( FModeChange )
		_V_PROP_( FMode )

//		int	FInterruptPin;

		/// The value of the last received RSSI value, in some transport specific units
		volatile int8_t     FLastRssi = 0;

		Mitov::SimpleList<uint8_t *>	FReceivedPackets;
		Mitov::SimpleList<uint8_t *>	FSendQueue;

	public:
		// set the frequency (in Hz)
/*
		void SynthesizeFrequency(uint32_t freqHz)
		{
//			uint8_t oldMode = FMode;
//			if (oldMode == RH_RF69_OPMODE_MODE_TX )
//				setModeRx();

			freqHz /= RF69_FSTEP; // divide down by FSTEP to get FRF
			writeReg(REG_FRFMSB, freqHz >> 16);
			writeReg(REG_FRFMID, freqHz >> 8);
			writeReg(REG_FRFLSB, freqHz);
//			if (oldMode == RF69_MODE_RX)
			setModeFrequencySynthesizer();

//			setOpMode(oldMode);
		}
*/
	protected:
		/// If current mode is Rx or Tx changes it to Idle. If the transmitter or receiver is running,
		/// disables them.
		void setModeIdle()
		{
			if ( FMode().GetValue() != RHModeIdle)
			{
//				Serial.println( "setModeIdle" );
				if ( Power() >= 18)
				{
					// If high power boost, return power amp to receive mode
					spiWrite(RH_RF69_REG_5A_TESTPA1, RH_RF69_TESTPA1_NORMAL);
					spiWrite(RH_RF69_REG_5C_TESTPA2, RH_RF69_TESTPA2_NORMAL);
				}

				setOpMode( RH_RF69_OPMODE_MODE_STDBY );
				FMode() = RHModeIdle;
			}
		}

/*
		void setModeFrequencySynthesizer()
		{
			if ( FMode != RHModeFrequencySynthesizer)
			{
				if ( Power >= 18)
				{
					// Set high power boost mode
					// Note that OCP defaults to ON so no need to change that.
					spiWrite(RH_RF69_REG_5A_TESTPA1, RH_RF69_TESTPA1_BOOST);
					spiWrite(RH_RF69_REG_5C_TESTPA2, RH_RF69_TESTPA2_BOOST);
				}

				setOpMode( RH_RF69_OPMODE_MODE_FS );
				FMode = RHModeFrequencySynthesizer;
			}
		}
*/
		void setModeRx()
		{
			if ( FMode().GetValue() != RHModeRx)
			{
//				Serial.println( "setModeRx" );
				if ( Power() >= 18)
				{
					// If high power boost, return power amp to receive mode
					spiWrite(RH_RF69_REG_5A_TESTPA1, RH_RF69_TESTPA1_NORMAL);
					spiWrite(RH_RF69_REG_5C_TESTPA2, RH_RF69_TESTPA2_NORMAL);
				}

				spiWrite(RH_RF69_REG_25_DIOMAPPING1, RH_RF69_DIOMAPPING1_DIO0MAPPING_01); // Set interrupt line 0 PayloadReady
				setOpMode(RH_RF69_OPMODE_MODE_RX); // Clears FIFO
				FMode() = RHModeRx;
			}
		}

		void setModeTx()
		{
			if ( FMode().GetValue() != RHModeTx)
			{
//				Serial.println( "setModeTx" );
				if ( Power() >= 18)
				{
					// Set high power boost mode
					// Note that OCP defaults to ON so no need to change that.
					spiWrite(RH_RF69_REG_5A_TESTPA1, RH_RF69_TESTPA1_BOOST);
					spiWrite(RH_RF69_REG_5C_TESTPA2, RH_RF69_TESTPA2_BOOST);
				}

				spiWrite(RH_RF69_REG_25_DIOMAPPING1, RH_RF69_DIOMAPPING1_DIO0MAPPING_00); // Set interrupt line 0 PacketSent
				setOpMode(RH_RF69_OPMODE_MODE_TX); // Clears FIFO
				FMode() = RHModeTx;
			}
		}

		virtual uint8_t spiRead(uint8_t reg) = 0;

		virtual uint8_t spiWrite(uint8_t reg, uint8_t val) = 0;

/*
		uint8_t spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len)
		{
			uint8_t status = 0;
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			status = C_SPI.transfer(reg & ~0x80); // Send the start address with the write mask off
			while (len--)
				*dest++ = C_SPI.transfer(0);

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return status;
		}
*/
		virtual uint8_t spiBurstWrite( uint8_t reg, const uint8_t* src, uint8_t len, uint8_t ATotalLength ) = 0;

		void setOpMode(uint8_t mode)
		{
			uint8_t opmode = spiRead(RH_RF69_REG_01_OPMODE);
			opmode &= ~RH_RF69_OPMODE_MODE;
			opmode |= (mode & RH_RF69_OPMODE_MODE);
			spiWrite(RH_RF69_REG_01_OPMODE, opmode);

			FModeChange() = true;

			// Wait for mode to change.
//			while (!(spiRead(RH_RF69_REG_27_IRQFLAGS1) & RH_RF69_IRQFLAGS1_MODEREADY))
//				;
		}

	public:
		void UpdateSyncWords()
		{
			uint8_t syncconfig = spiRead(RH_RF69_REG_2E_SYNCCONFIG);
			if ( T_SyncWords::GetCount() )
			{
//				uint8_t *ABuffer = new uint8_t[ SyncWords._BytesSize ];
//				memcpy_P( ABuffer, SyncWords._Bytes, SyncWords._BytesSize );
//				spiBurstWrite(RH_RF69_REG_2F_SYNCVALUE1, ABuffer, SyncWords._BytesSize );
//				delete [] ABuffer;
                uint8_t * ASyncWords = T_SyncWords::Allocate();
				spiBurstWrite( RH_RF69_REG_2F_SYNCVALUE1, ASyncWords, T_SyncWords::GetCount(), T_SyncWords::GetCount() );
                T_SyncWords::Release( ASyncWords );

				syncconfig |= RH_RF69_SYNCCONFIG_SYNCON;
			}

			else
				syncconfig &= ~RH_RF69_SYNCCONFIG_SYNCON;

			syncconfig &= ~RH_RF69_SYNCCONFIG_SYNCSIZE;
			if( T_SyncWords::GetCount() )
				syncconfig |= ( T_SyncWords::GetCount() - 1 ) << 3;

			spiWrite(RH_RF69_REG_2E_SYNCCONFIG, syncconfig);
		}

		void UpdatePreambleLength()
		{
			spiWrite(RH_RF69_REG_2C_PREAMBLEMSB, PreambleLength().GetValue() >> 8);
			spiWrite(RH_RF69_REG_2D_PREAMBLELSB, PreambleLength().GetValue() & 0xff);
		}

		void UpdateFrequency()
		{
			// Frf = FRF / FSTEP
			uint32_t frf = (Frequency() * 1000000.0) / RH_RF69_FSTEP;
//			Serial.println( frf );
			spiWrite(RH_RF69_REG_07_FRFMSB, (frf >> 16) & 0xff);
			spiWrite(RH_RF69_REG_08_FRFMID, (frf >> 8) & 0xff);
			spiWrite(RH_RF69_REG_09_FRFLSB, frf & 0xff);
		}

		void UpdateEncryptionKey()
		{
			if ( T_EncryptionKey::GetCount() )
			{
//				uint8_t *ABuffer = new uint8_t[ 16 ];
//				memcpy_P( ABuffer, EncryptionKey, 16 );
//				spiBurstWrite( RH_RF69_REG_3E_AESKEY1, ABuffer, 16 );
//				delete [] ABuffer;
                uint8_t *AEncryptionKey = T_EncryptionKey::Allocate();
				spiBurstWrite( RH_RF69_REG_3E_AESKEY1, AEncryptionKey, T_EncryptionKey::GetCount(), 16 );
                T_EncryptionKey::Release( AEncryptionKey );
				spiWrite(RH_RF69_REG_3D_PACKETCONFIG2, spiRead(RH_RF69_REG_3D_PACKETCONFIG2) | RH_RF69_PACKETCONFIG2_AESON);
			}

			else
				spiWrite(RH_RF69_REG_3D_PACKETCONFIG2, spiRead(RH_RF69_REG_3D_PACKETCONFIG2) & ~RH_RF69_PACKETCONFIG2_AESON);
		}

		void UpdateTransmitPower()
		{
			uint8_t palevel;
			if ( Power().GetValue() < -18)
				Power() = -18;

			// See http://www.hoperf.com/upload/rfchip/RF69-V1.2.pdf section 3.3.6
			// for power formulas
			if (Power() <= 13)
			{
				// -18dBm to +13dBm
				palevel = RH_RF69_PALEVEL_PA0ON | ((Power() + 18) & RH_RF69_PALEVEL_OUTPUTPOWER);
			}
			else if (Power() >= 18)
			{
				// +18dBm to +20dBm
				// Need PA1+PA2
				// Also need PA boost settings change when tx is turned on and off, see setModeTx()
				palevel = RH_RF69_PALEVEL_PA1ON | RH_RF69_PALEVEL_PA2ON | ((Power() + 11) & RH_RF69_PALEVEL_OUTPUTPOWER);
			}
			else
			{
				// +14dBm to +17dBm
				// Need PA1+PA2
				palevel = RH_RF69_PALEVEL_PA1ON | RH_RF69_PALEVEL_PA2ON | ((Power() + 14) & RH_RF69_PALEVEL_OUTPUTPOWER);
			}

			spiWrite(RH_RF69_REG_11_PALEVEL, palevel);
		}

		void UpdateAddress()
		{
			spiWrite( RH_RF69_REG_39_NODEADRS, Address() );
			spiWrite( RH_RF69_REG_3A_BROADCASTADRS, BroadcastAddress() );
		}

		// Sets registers from a canned modem configuration structure
		void UpdateModemRegisters()
		{
			const uint8_t CModulations[] =
			{
				RH_RF69_DATAMODUL_DATAMODE_PACKET | RH_RF69_DATAMODUL_MODULATIONTYPE_FSK | RH_RF69_DATAMODUL_MODULATIONSHAPING_FSK_BT1_0,
				RH_RF69_DATAMODUL_DATAMODE_PACKET | RH_RF69_DATAMODUL_MODULATIONTYPE_FSK | RH_RF69_DATAMODUL_MODULATIONSHAPING_FSK_BT0_5,
				RH_RF69_DATAMODUL_DATAMODE_PACKET | RH_RF69_DATAMODUL_MODULATIONTYPE_FSK | RH_RF69_DATAMODUL_MODULATIONSHAPING_FSK_BT0_3,

				RH_RF69_DATAMODUL_DATAMODE_PACKET | RH_RF69_DATAMODUL_MODULATIONTYPE_OOK | RH_RF69_DATAMODUL_MODULATIONSHAPING_OOK_NONE,
				RH_RF69_DATAMODUL_DATAMODE_PACKET | RH_RF69_DATAMODUL_MODULATIONTYPE_OOK | RH_RF69_DATAMODUL_MODULATIONSHAPING_OOK_BR,
				RH_RF69_DATAMODUL_DATAMODE_PACKET | RH_RF69_DATAMODUL_MODULATIONTYPE_OOK | RH_RF69_DATAMODUL_MODULATIONSHAPING_OOK_2BR
			};

			const uint8_t ChannelFilterBandwidths[] =
			{
				0b10000 | 7,
				0b01000 | 7,
				0b00000 | 7,
				0b10000 | 6,
				0b01000 | 6,
				0b00000 | 6,
				0b10000 | 5,
				0b01000 | 5,
				0b00000 | 5,
				0b10000 | 4,
				0b01000 | 4,
				0b00000 | 4,
				0b10000 | 3,
				0b01000 | 3,
				0b00000 | 3,
				0b10000 | 2,
				0b01000 | 2,
				0b00000 | 2,
				0b10000 | 1,
				0b01000 | 1,
				0b00000 | 1,
				0b10000 | 0,
				0b01000 | 0,
				0b00000 | 0
			};

			spiWrite( RH_RF69_REG_02_DATAMODUL, CModulations[ ModemSettings().Modulation() ] );
			spiWrite( RH_RF69_REG_03_BITRATEMSB, ModemSettings().BitRate().GetValue() >> 8 );
			spiWrite( RH_RF69_REG_04_BITRATELSB, ModemSettings().BitRate() );

			uint32_t frf = ( ModemSettings().FrequencyDeviation() * 1000000.0) / RH_RF69_FSTEP;
			spiWrite( RH_RF69_REG_05_FDEVMSB, frf >> 8 );
			spiWrite( RH_RF69_REG_06_FDEVLSB, frf );

			uint8_t AValue = ModemSettings().DCCCutoffFrequency().GetValue() << 5 | ChannelFilterBandwidths[ ModemSettings().ChannelFilterBandwidth() ];
			spiWrite( RH_RF69_REG_19_RXBW, AValue );

			AValue = RH_RF69_PACKETCONFIG1_PACKETFORMAT_VARIABLE | uint8_t( ModemSettings().DCFreeEncoding().GetValue() ) << 5 | ( ModemSettings().CRCEnabled() ? RH_RF69_PACKETCONFIG1_CRC_ON : 0 );
			if( ! Promiscuous() )
				AValue |= RH_RF69_PACKETCONFIG1_ADDRESSFILTERING_NODE_BC;

			spiWrite( RH_RF69_REG_37_PACKETCONFIG1, AValue );
		}

	protected:
		// Low level function reads the FIFO and checks the address
		// Caution: since we put our headers in what the RH_RF69 considers to be the payload, if encryption is enabled
		// we have to suffer the cost of decryption before we can determine whether the address is acceptable.
		// Performance issue?
		virtual void readFifo() = 0;

		virtual void Reset() = 0;

	public:
		virtual void send( uint8_t ATxHeaderTo, const uint8_t* data, uint8_t len ) = 0;

	public:
		inline void SystemInit()
		{
			Reset();

			setModeIdle();

			// Configure important RH_RF69 registers
			// Here we set up the standard packet format for use by the RH_RF69 library:
			// 4 bytes preamble
			// 2 SYNC words 2d, d4
			// 2 CRC CCITT octets computed on the header, length and data (this in the modem config data)
			// 0 to 60 bytes data
			// RSSI Threshold -114dBm
			// We dont use the RH_RF69s address filtering: instead we prepend our own headers to the beginning
			// of the RH_RF69 payload
			spiWrite(RH_RF69_REG_3C_FIFOTHRESH, RH_RF69_FIFOTHRESH_TXSTARTCONDITION_NOTEMPTY | 0x0f); // thresh 15 is default
			// RSSITHRESH is default
		//    spiWrite(RH_RF69_REG_29_RSSITHRESH, 220); // -110 dbM
			// SYNCCONFIG is default. SyncSize is set later by setSyncWords()
		//    spiWrite(RH_RF69_REG_2E_SYNCCONFIG, RH_RF69_SYNCCONFIG_SYNCON); // auto, tolerance 0
			// PAYLOADLENGTH is default
		//    spiWrite(RH_RF69_REG_38_PAYLOADLENGTH, RH_RF69_FIFO_SIZE); // max size only for RX
			// PACKETCONFIG 2 is default
			spiWrite(RH_RF69_REG_6F_TESTDAGC, RH_RF69_TESTDAGC_CONTINUOUSDAGC_IMPROVED_LOWBETAOFF);
			// If high power boost set previously, disable it
			spiWrite(RH_RF69_REG_5A_TESTPA1, RH_RF69_TESTPA1_NORMAL);
			spiWrite(RH_RF69_REG_5C_TESTPA2, RH_RF69_TESTPA2_NORMAL);

			UpdateSyncWords();
			UpdateModemRegisters();
//			setModemConfig(GFSK_Rb250Fd250);
			UpdatePreambleLength();
			UpdateFrequency();
			UpdateEncryptionKey();
			UpdateTransmitPower();
			UpdateAddress();

//			Serial.println( "START1" );

/*
			// The following can be changed later by the user if necessary.
			// Set up default configuration
			uint8_t syncwords[] = { 0x2d, 0xd4 };
			setSyncWords(syncwords, sizeof(syncwords)); // Same as RF22's
			// Reasnably fast and reliable default speed and modulation
			setModemConfig(GFSK_Rb250Fd250);
			// 3 would be sufficient, but this is the same as RF22's
			setPreambleLength(4);
			// An innocuous ISM frequency, same as RF22's
			setFrequency(434.0);
			// No encryption
			setEncryptionKey( nullptr );
			// +13dBm, same as power-on default
			setTxPower(13);
*/
			if( T_OutputPin::GetPinIsConnected() )
				setModeRx();
		}

		inline void SystemLoopBegin()
		{
			if( FModeChange() )
			{
				if( !(spiRead(RH_RF69_REG_27_IRQFLAGS1) & RH_RF69_IRQFLAGS1_MODEREADY) )
					return;

				FModeChange() = false;
			}


			// Wait for mode to change.
//			while (!(spiRead(RH_RF69_REG_27_IRQFLAGS1) & RH_RF69_IRQFLAGS1_MODEREADY))
//				;

			uint8_t *APacket = nullptr;
			noInterrupts(); // Protect the FReceivedPackets from being corrupted
			if( FReceivedPackets.size() )
			{
				APacket = FReceivedPackets[ 0 ];
				FReceivedPackets.pop_front();
			}

			interrupts();

			if( APacket )
			{
				uint8_t ASentTo = APacket[ 0 ];
				uint8_t ASize = APacket[ 1 ];
//				Serial.println( "RECEIVED" );
//				Serial.println( ASize );
				T_SentToOutputPin::SetPinValue( ASentTo );
				T_OutputPin::SetPinValue( Mitov::TDataBlock( ASize, APacket + 2 ));
				delete [] APacket;
			}

			if( FMode() != RHModeTx )
			{
				if( FSendQueue.size() )
				{
					uint8_t *ABuffer = FSendQueue[ 0 ];
					FSendQueue.pop_front();
					send( ABuffer[ 0 ], ABuffer + 2, ABuffer[ 1 ] );
					delete [] ABuffer;
				}
			}

			if( FMode().GetValue() == RHModeIdle )
				if( T_OutputPin::GetPinIsConnected() )
					setModeRx();

			T_SignalStrengthOutputPin::SetPinValue( FLastRssi );
//			Serial.print( "FLastRssi: " );	Serial.println( FLastRssi );
		}

	public:
/*
		MitovBasicLoRaRFM69( BasicSPI &ASPI, int AInterruptPin, void (*AInterruptRoutine)() ) :
			C_SPI( ASPI ),
//			FInterruptPin( AInterruptPin ),
//			Modulation( mFrequencyShiftKeying_Gaussian_1_0 ),
			Promiscuous( false ),
			FModeChange( false ),
			FMode( RHModeInitialising )
		{
			ModemSettings.Modulation = mFrequencyShiftKeying_Gaussian_1_0;
			ModemSettings.DCCCutoffFrequency = rfmDCCCutoff4;
			ModemSettings.ChannelFilterBandwidth = bfFSK_500_0_OOK_250_0;
			ModemSettings.DCFreeEncoding = dcfeNone;
			ModemSettings.CRCEnabled = true;

			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			// On all other platforms, its innocuous, belt and braces
			pinMode( AInterruptPin, INPUT );

			int AInterruptNumber = digitalPinToInterrupt( AInterruptPin );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, RISING );

			C_SPI.usingInterrupt( AInterruptNumber );

			T_ChipSelectOutputPin::SetPinValueHigh();
		}
*/
		inline MitovBasicLoRaRFM69()
		{
			FModeChange() = false;
			FMode() = RHModeInitialising;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address
	> class MitovLoRaSendPacket :
		public T_Address
	{
	public:
		_V_PROP_( Address )

	public:
		void Print( Mitov::String AValue )
		{
//			Serial.println( "Print" );
			AValue += "\r\n";
			C_OWNER.send( Address(), (uint8_t *)AValue.c_str(), AValue.length() );
		}

		void Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		inline void PrintChar( char AValue )
		{
			C_OWNER.send( Address(), (uint8_t*)&AValue, 1 );
		}

		inline void PrintChar( uint8_t AValue )
		{
			C_OWNER.send( Address(), &AValue, 1 );
		}

		inline void Write( uint8_t *AData, uint32_t ASize )
		{
			C_OWNER.send( Address(), AData, ASize );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_Address,
		typename T_BroadcastAddress,
		typename T_ChipSelectOutputPin,
        typename T_EncryptionKey,
        typename T_EncryptionKey_ApplyValues,
        typename T_EncryptionKey_GetValue,
		typename T_FMode,
		typename T_FModeChange,
		typename T_Frequency,
		uint16_t C_InterruptInputPin,
		typename T_MaxSendQueue,
		typename T_ModemSettings,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_Promiscuous,
		typename T_ResetOutputPin,
		typename T_SentToOutputPin,
		typename T_SignalStrengthOutputPin,
        typename T_SyncWords,
        typename T_SyncWords_ApplyValues,
        typename T_SyncWords_GetValue
	> class MitovLoRaRFM69_Interrupt : public MitovBasicLoRaRFM69 <
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			>,
		public T_ChipSelectOutputPin,
		public T_ResetOutputPin
	{
		typedef	MitovBasicLoRaRFM69 <
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			> inherited;

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( ResetOutputPin )

	protected:
		virtual uint8_t spiRead(uint8_t reg) override
		{
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(reg & ~0x80); // Send the address with the write mask off
			uint8_t val = C_SPI.transfer(0); // The written value is ignored, reg value is read
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return val;
		}

		virtual uint8_t spiWrite(uint8_t reg, uint8_t val) override
		{
//Serial.print( "spiWrite: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			uint8_t status = 0;
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			status = C_SPI.transfer(reg | 0x80); // Send the address with the write mask on
			C_SPI.transfer(val); // New value follows
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return status;
		}

		virtual uint8_t spiBurstWrite( uint8_t reg, const uint8_t* src, uint8_t len, uint8_t ATotalLength ) override
		{
/*
			Serial.print( "spiBurstWrite: " ); Serial.print( reg, HEX ); Serial.print( " =" );
			for( int i = 0; i < len; ++i )
			{
				Serial.print( " " );
				Serial.print( src[ i ], HEX );
			}

			Serial.println( "" );
*/
			uint8_t status = 0;
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			status = C_SPI.transfer( reg | 0x80 ); // Send the start address with the write mask on
            if( len > ATotalLength )
                len = ATotalLength;

			while( len -- )
            {
				C_SPI.transfer( *src ++ );
                -- ATotalLength;
            }

            while( ATotalLength -- )
				C_SPI.transfer( 0 );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return status;
		}

		// Low level function reads the FIFO and checks the address
		// Caution: since we put our headers in what the RH_RF69 considers to be the payload, if encryption is enabled
		// we have to suffer the cost of decryption before we can determine whether the address is acceptable.
		// Performance issue?
		virtual void readFifo() override
		{
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(inherited::RH_RF69_REG_00_FIFO); // Send the start address with the write mask off
			uint8_t payloadlen = C_SPI.transfer(0); // First byte is payload len (counting the headers)
//			if ( ( payloadlen <= RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN ) && ( payloadlen >= RH_RF69_HEADER_LEN ) )
			if ((( inherited::EncryptionKey().GetCount() == 0 ) && ( payloadlen <= inherited::RH_RF69_MAX_PAYLOAD_LEN ) ) || ( payloadlen <= inherited::RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN ))
			{
				uint8_t ARxHeaderTo = C_SPI.transfer(0);
				// Check addressing
//				if ( Promiscuous ||
//					ARxHeaderTo == Address ||
//					ARxHeaderTo == BroadcastAddress )
				{
					uint8_t *ABuffer = new uint8_t[ 2 + payloadlen ];
					inherited::FReceivedPackets.push_back( ABuffer );
					*ABuffer++ = ARxHeaderTo;
					*ABuffer++ = payloadlen - 1; // Skip the ARxHeaderTo
					for ( int i = 0; i < payloadlen - 1; ++i )
						*ABuffer++ = C_SPI.transfer(0);
/*
					uint8_t *ABuffer = new uint8_t[ 1 + (payloadlen - RH_RF69_HEADER_LEN) + 3 ];
					FReceivedPackets.push_back( ABuffer );
					*ABuffer++ = payloadlen;
					for ( int i = 0; i < (payloadlen - RH_RF69_HEADER_LEN) + 3; ++i )
						*ABuffer++ =C_SPI.transfer(0);
*/
					/*
					// Get the rest of the headers
					_rxHeaderFrom  = C_SPI.transfer(0);
					_rxHeaderId    = C_SPI.transfer(0);
					_rxHeaderFlags = C_SPI.transfer(0);
					// And now the real payload
					for (_bufLen = 0; _bufLen < (payloadlen - RH_RF69_HEADER_LEN); _bufLen++)
						_buf[_bufLen] = C_SPI.transfer(0);
*/
//					_rxGood++;
//					_rxBufValid = true;
				}
			}

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			// Any junk remaining in the FIFO will be cleared next time we go to receive mode.
		}

		virtual void Reset() override
		{
			T_ChipSelectOutputPin::SetPinValueHigh();

			if( T_ResetOutputPin::GetPinIsConnected() )
			{
//				Serial.println( "RESET" );
				T_ResetOutputPin::SetPinValueHigh();
				delay( 100 );
				T_ResetOutputPin::SetPinValueLow();
				delay( 100 );
			}
		}

	public:
		// C++ level interrupt handler for this instance
		// RH_RF69 is unusual in that it has several interrupt lines, and not a single, combined one.
		// On Moteino, only one of the several interrupt lines (DI0) from the RH_RF69 is connnected to the processor.
		// We use this to get PACKETSDENT and PAYLOADRADY interrupts.
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool )
		{
//			Serial.println( "INT" );
			// Get the interrupt cause
			uint8_t irqflags2 = spiRead(inherited::RH_RF69_REG_28_IRQFLAGS2);

//			Serial.println( irqflags2, BIN );

			if ( inherited::FMode() == inherited::RHModeTx && (irqflags2 & inherited::RH_RF69_IRQFLAGS2_PACKETSENT))
			{
				// A transmitter message has been fully sent
/*
				if( OutputPin.IsConnected() )
					setModeRx(); // Clears FIFO

				else
*/
				inherited::setModeIdle(); // Clears FIFO

//				Serial.println("PACKET SENT");
			}

			// Must look for PAYLOADREADY, not CRCOK, since only PAYLOADREADY occurs _after_ AES decryption
			// has been done
			if ( inherited::FMode() == inherited::RHModeRx && (irqflags2 & inherited::RH_RF69_IRQFLAGS2_PAYLOADREADY))
			{
//				Serial.println("PACKET RECEIVED");
				// A complete message has been received with good CRC
				inherited::FLastRssi = -((int8_t)(spiRead(inherited::RH_RF69_REG_24_RSSIVALUE) >> 1));
//				_lastPreambleTime = millis();

				inherited::setModeIdle();
				// Save it in our buffer
				readFifo();
//				setModeRx();
			}
		}

	public:
		virtual void send( uint8_t ATxHeaderTo, const uint8_t* data, uint8_t len ) override
		{
//				Serial.println( "TRY SEND" );
//				Serial.println( len );
//				Serial.println( FTestCount );
//				Serial.println( int( FMode ));
//			if (len > RH_RF69_MAX_MESSAGE_LEN)
//				return;

//			waitPacketSent(); // Make sure we dont interrupt an outgoing message // _mode == RHModeTx

			if( inherited::FMode() == inherited::RHModeTx )
			{
				while( inherited::FSendQueue.size() > inherited::MaxSendQueue() )
					;

				uint8_t *ABuffer = new uint8_t[ len + 2 ];
				ABuffer[ 0 ] = ATxHeaderTo;
				ABuffer[ 1 ] = len;
				memcpy( ABuffer + 2, data, len );
				inherited::FSendQueue.push_back( ABuffer );
				return;
			}

			inherited::setModeIdle(); // Prevent RX while filling the fifo
//			Serial.println( "SEND" );

			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( inherited::RH_RF69_REG_00_FIFO | 0x80 ); // Send the start address with the write mask on
			C_SPI.transfer( len + 1 ); // Include length of headers
			// First the 4 headers
			C_SPI.transfer( ATxHeaderTo );
//			C_SPI.transfer( Address );
//			C_SPI.transfer(_txHeaderId);
//			C_SPI.transfer(_txHeaderFlags);
			// Now the payload
			while( len-- )
				C_SPI.transfer( *data++ );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			inherited::setModeTx(); // Start the transmitter
		}

	public:
		inline void SystemInit()
		{
//			Serial.println( "SystemInit" );
			int AInterruptNumber = digitalPinToInterrupt( C_InterruptInputPin );
			C_SPI.usingInterrupt( AInterruptNumber );

			T_ChipSelectOutputPin::SetPinValueHigh();

			inherited::SystemInit();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_Address,
		typename T_BroadcastAddress,
		typename T_ChipSelectOutputPin,
        typename T_EncryptionKey,
        typename T_EncryptionKey_ApplyValues,
        typename T_EncryptionKey_GetValue,
		typename T_FMode,
		typename T_FModeChange,
		typename T_Frequency,
		typename T_MaxSendQueue,
		typename T_ModemSettings,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_Promiscuous,
		typename T_ResetOutputPin,
		typename T_SentToOutputPin,
		typename T_SignalStrengthOutputPin,
        typename T_SyncWords,
        typename T_SyncWords_ApplyValues,
        typename T_SyncWords_GetValue
	> class MitovLoRaRFM69 : public MitovBasicLoRaRFM69 <
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			>,
		public T_ChipSelectOutputPin,
		public T_ResetOutputPin
	{
		typedef	MitovBasicLoRaRFM69 <
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			> inherited;

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( ResetOutputPin )

	protected:
		virtual uint8_t spiRead(uint8_t reg) override
		{
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(reg & ~0x80); // Send the address with the write mask off
			uint8_t val = C_SPI.transfer(0); // The written value is ignored, reg value is read
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return val;
		}

		virtual uint8_t spiWrite(uint8_t reg, uint8_t val) override
		{
//Serial.print( "spiWrite: " ); Serial.print( reg, HEX ); Serial.print( " = " ); Serial.println( val, HEX );
			uint8_t status = 0;
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			status = C_SPI.transfer(reg | 0x80); // Send the address with the write mask on
			C_SPI.transfer(val); // New value follows
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return status;
		}

		virtual uint8_t spiBurstWrite( uint8_t reg, const uint8_t* src, uint8_t len, uint8_t ATotalLength ) override
		{
/*
			Serial.print( "spiBurstWrite: " ); Serial.print( reg, HEX ); Serial.print( " =" );
			for( int i = 0; i < len; ++i )
			{
				Serial.print( " " );
				Serial.print( src[ i ], HEX );
			}

			Serial.println( "" );
*/
			uint8_t status = 0;
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			status = C_SPI.transfer(reg | 0x80); // Send the start address with the write mask on

            if( len > ATotalLength )
                len = ATotalLength;

			while (len--)
				C_SPI.transfer(*src++);

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			return status;
		}

		// Low level function reads the FIFO and checks the address
		// Caution: since we put our headers in what the RH_RF69 considers to be the payload, if encryption is enabled
		// we have to suffer the cost of decryption before we can determine whether the address is acceptable.
		// Performance issue?
		virtual void readFifo() override
		{
			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(inherited::RH_RF69_REG_00_FIFO); // Send the start address with the write mask off
			uint8_t payloadlen = C_SPI.transfer(0); // First byte is payload len (counting the headers)
//			if ( ( payloadlen <= RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN ) && ( payloadlen >= RH_RF69_HEADER_LEN ) )
			if ((( inherited::EncryptionKey().GetCount() == 0 ) && ( payloadlen <= inherited::RH_RF69_MAX_PAYLOAD_LEN ) ) || ( payloadlen <= inherited::RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN ))
			{
				uint8_t ARxHeaderTo = C_SPI.transfer(0);
				// Check addressing
//				if ( Promiscuous ||
//					ARxHeaderTo == Address ||
//					ARxHeaderTo == BroadcastAddress )
				{
					uint8_t *ABuffer = new uint8_t[ 2 + payloadlen ];
					inherited::FReceivedPackets.push_back( ABuffer );
					*ABuffer++ = ARxHeaderTo;
					*ABuffer++ = payloadlen - 1; // Skip the ARxHeaderTo
					for ( int i = 0; i < payloadlen - 1; ++i )
						*ABuffer++ = C_SPI.transfer(0);
/*
					uint8_t *ABuffer = new uint8_t[ 1 + (payloadlen - RH_RF69_HEADER_LEN) + 3 ];
					FReceivedPackets.push_back( ABuffer );
					*ABuffer++ = payloadlen;
					for ( int i = 0; i < (payloadlen - RH_RF69_HEADER_LEN) + 3; ++i )
						*ABuffer++ =C_SPI.transfer(0);
*/
					/*
					// Get the rest of the headers
					_rxHeaderFrom  = C_SPI.transfer(0);
					_rxHeaderId    = C_SPI.transfer(0);
					_rxHeaderFlags = C_SPI.transfer(0);
					// And now the real payload
					for (_bufLen = 0; _bufLen < (payloadlen - RH_RF69_HEADER_LEN); _bufLen++)
						_buf[_bufLen] = C_SPI.transfer(0);
*/
//					_rxGood++;
//					_rxBufValid = true;
				}
			}

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
			// Any junk remaining in the FIFO will be cleared next time we go to receive mode.
		}

		virtual void Reset() override
		{
			T_ChipSelectOutputPin::SetPinValueHigh();

			if( T_ResetOutputPin::GetPinIsConnected() )
			{
//				Serial.println( "RESET" );
				T_ResetOutputPin::SetPinValueHigh();
				delay( 100 );
				T_ResetOutputPin::SetPinValueLow();
				delay( 100 );
			}
		}

	public:
		virtual void send( uint8_t ATxHeaderTo, const uint8_t* data, uint8_t len ) override
		{
//				Serial.println( "TRY SEND" );
//				Serial.println( len );
//				Serial.println( FTestCount );
//				Serial.println( int( FMode ));
//			if (len > RH_RF69_MAX_MESSAGE_LEN)
//				return;

//			waitPacketSent(); // Make sure we dont interrupt an outgoing message // _mode == RHModeTx

			if( inherited::FMode() == inherited::RHModeTx )
			{
				while( inherited::FSendQueue.size() > inherited::MaxSendQueue() )
					;

				uint8_t *ABuffer = new uint8_t[ len + 2 ];
				ABuffer[ 0 ] = ATxHeaderTo;
				ABuffer[ 1 ] = len;
				memcpy( ABuffer + 2, data, len );
				inherited::FSendQueue.push_back( ABuffer );
				return;
			}

			inherited::setModeIdle(); // Prevent RX while filling the fifo
//			Serial.println( "SEND" );

			C_SPI.beginTransaction( SPISettings() );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( inherited::RH_RF69_REG_00_FIFO | 0x80 ); // Send the start address with the write mask on
			C_SPI.transfer( len + 1 ); // Include length of headers
			// First the 4 headers
			C_SPI.transfer( ATxHeaderTo );
//			C_SPI.transfer( Address );
//			C_SPI.transfer(_txHeaderId);
//			C_SPI.transfer(_txHeaderFlags);
			// Now the payload
			while( len-- )
				C_SPI.transfer( *data++ );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			inherited::setModeTx(); // Start the transmitter
		}

	public:
		inline void SystemLoopBegin()
		{
			uint8_t irqflags2 = spiRead( inherited::RH_RF69_REG_28_IRQFLAGS2 );
			if ( inherited::FMode() == inherited::RHModeTx && (irqflags2 & inherited::RH_RF69_IRQFLAGS2_PACKETSENT))
			{
				// A transmitter message has been fully sent
/*
				if( OutputPin.IsConnected() )
					setModeRx(); // Clears FIFO

				else
*/
				inherited::setModeIdle(); // Clears FIFO

				//	Serial.println("PACKETSENT");
			}

			// Must look for PAYLOADREADY, not CRCOK, since only PAYLOADREADY occurs _after_ AES decryption
			// has been done
			if ( inherited::FMode() == inherited::RHModeRx && (irqflags2 & inherited::RH_RF69_IRQFLAGS2_PAYLOADREADY))
			{
				// A complete message has been received with good CRC
				inherited::FLastRssi = -((int8_t)(spiRead(inherited::RH_RF69_REG_24_RSSIVALUE) >> 1));
//				_lastPreambleTime = millis();

				inherited::setModeIdle();
				// Save it in our buffer
				readFifo();
//				setModeRx();
			}

			inherited::SystemLoopBegin();
		}

	};
//---------------------------------------------------------------------------
	template <
		int PIN_CS, int PIN_RST,
		int PIN_INT,
		typename T_Address,
		typename T_BroadcastAddress,
        typename T_EncryptionKey,
        typename T_EncryptionKey_ApplyValues,
        typename T_EncryptionKey_GetValue,
		typename T_FMode,
		typename T_FModeChange,
		typename T_Frequency,
		typename T_MaxSendQueue,
		typename T_ModemSettings,
		typename T_OutputPin,
		typename T_Power,
		typename T_PreambleLength,
		typename T_Promiscuous,
		typename T_SentToOutputPin,
		typename T_SignalStrengthOutputPin,
        typename T_SyncWords,
        typename T_SyncWords_ApplyValues,
        typename T_SyncWords_GetValue
	> class MitovLoRaRFM69Shield : public MitovBasicLoRaRFM69 <
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			>
	{
		typedef	MitovBasicLoRaRFM69 <
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			> inherited;

	protected:
		static MitovLoRaRFM69Shield <
				PIN_CS, PIN_RST, PIN_INT,
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			>	*FInstnce;

	protected:
		virtual uint8_t spiRead(uint8_t reg) override
		{
			uint8_t val;
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			SPI.transfer(reg & ~0x80); // Send the address with the write mask off
			val = SPI.transfer(0); // The written value is ignored, reg value is read
			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
			return val;
		}

		virtual uint8_t spiWrite(uint8_t reg, uint8_t val) override
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

		virtual uint8_t spiBurstWrite( uint8_t reg, const uint8_t* src, uint8_t len, uint8_t ATotalLength ) override
		{
/*
			Serial.print( "spiBurstWrite: " ); Serial.print( reg, HEX ); Serial.print( " =" );
			for( int i = 0; i < len; ++i )
			{
				Serial.print( " " );
				Serial.print( src[ i ], HEX );
			}

			Serial.println( "" );
*/
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			uint8_t status = SPI.transfer( reg | 0x80 ); // Send the start address with the write mask on

            if( len > ATotalLength )
                len = ATotalLength;

			while( len -- )
				SPI.transfer( *src ++ );

			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
			return status;
		}

		// Low level function reads the FIFO and checks the address
		// Caution: since we put our headers in what the RH_RF69 considers to be the payload, if encryption is enabled
		// we have to suffer the cost of decryption before we can determine whether the address is acceptable.
		// Performance issue?
		virtual void readFifo() override
		{
			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			SPI.transfer( inherited::RH_RF69_REG_00_FIFO ); // Send the start address with the write mask off
			uint8_t payloadlen = SPI.transfer(0); // First byte is payload len (counting the headers)
//			if ( ( payloadlen <= RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN ) && ( payloadlen >= RH_RF69_HEADER_LEN ) )
			if ((( inherited::EncryptionKey().GetCount() == 0 ) && ( payloadlen <= inherited::RH_RF69_MAX_PAYLOAD_LEN ) ) || ( payloadlen <= inherited::RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN ))
			{
				uint8_t ARxHeaderTo = SPI.transfer(0);
				// Check addressing
//				if ( Promiscuous ||
//					ARxHeaderTo == Address ||
//					ARxHeaderTo == BroadcastAddress )
				{
					uint8_t *ABuffer = new uint8_t[ 2 + payloadlen ];
					inherited::FReceivedPackets.push_back( ABuffer );
					*ABuffer++ = ARxHeaderTo;
					*ABuffer++ = payloadlen - 1; // Skip the ARxHeaderTo
					for ( int i = 0; i < payloadlen - 1; ++i )
						*ABuffer++ = SPI.transfer(0);
/*
					uint8_t *ABuffer = new uint8_t[ 1 + (payloadlen - RH_RF69_HEADER_LEN) + 3 ];
					FReceivedPackets.push_back( ABuffer );
					*ABuffer++ = payloadlen;
					for ( int i = 0; i < (payloadlen - RH_RF69_HEADER_LEN) + 3; ++i )
						*ABuffer++ =SPI.transfer(0);
*/
					/*
					// Get the rest of the headers
					_rxHeaderFrom  = SPI.transfer(0);
					_rxHeaderId    = SPI.transfer(0);
					_rxHeaderFlags = SPI.transfer(0);
					// And now the real payload
					for (_bufLen = 0; _bufLen < (payloadlen - RH_RF69_HEADER_LEN); _bufLen++)
						_buf[_bufLen] = SPI.transfer(0);
*/
//					_rxGood++;
//					_rxBufValid = true;
				}
			}

			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();
			// Any junk remaining in the FIFO will be cleared next time we go to receive mode.
		}

		virtual void Reset() override
		{
			digitalWrite( PIN_CS, HIGH );

//				Serial.println( "RESET" );
			digitalWrite( PIN_RST, LOW );
			delay( 100 );
			digitalWrite( PIN_RST, HIGH );
			delay( 100 );
		}

	public:
		// C++ level interrupt handler for this instance
		// RH_RF69 is unusual in that it has several interrupt lines, and not a single, combined one.
		// On Moteino, only one of the several interrupt lines (DI0) from the RH_RF69 is connnected to the processor.
		// We use this to get PACKETSDENT and PAYLOADRADY interrupts.
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool )
		{
//			Serial.println( "INT" );
			// Get the interrupt cause
			uint8_t irqflags2 = spiRead( inherited::RH_RF69_REG_28_IRQFLAGS2 );
			if ( inherited::FMode() == inherited::RHModeTx && (irqflags2 & inherited::RH_RF69_IRQFLAGS2_PACKETSENT ))
			{
				// A transmitter message has been fully sent
/*
				if( OutputPin.IsConnected() )
					setModeRx(); // Clears FIFO

				else
*/
				inherited::setModeIdle(); // Clears FIFO

				//	Serial.println("PACKETSENT");
			}

			// Must look for PAYLOADREADY, not CRCOK, since only PAYLOADREADY occurs _after_ AES decryption
			// has been done
			if ( inherited::FMode() == inherited::RHModeRx && ( irqflags2 & inherited::RH_RF69_IRQFLAGS2_PAYLOADREADY ))
			{
				// A complete message has been received with good CRC
				inherited::FLastRssi = -((int8_t)(spiRead( inherited::RH_RF69_REG_24_RSSIVALUE ) >> 1));
//				_lastPreambleTime = millis();

				inherited::setModeIdle();
				// Save it in our buffer
				readFifo();
//				inherited::setModeRx();
			}
		}

	public:
		virtual void send( uint8_t ATxHeaderTo, const uint8_t* data, uint8_t len ) override
		{
//				Serial.println( "TRY SEND" );
//				Serial.println( len );
//				Serial.println( FTestCount );
//				Serial.println( int( FMode ));
//			if (len > RH_RF69_MAX_MESSAGE_LEN)
//				return;

//			waitPacketSent(); // Make sure we dont interrupt an outgoing message // _mode == RHModeTx

			if( inherited::FMode() == inherited::RHModeTx )
			{
				while( inherited::FSendQueue.size() > inherited::MaxSendQueue() )
					;

				uint8_t *ABuffer = new uint8_t[ len + 2 ];
				ABuffer[ 0 ] = ATxHeaderTo;
				ABuffer[ 1 ] = len;
				memcpy( ABuffer + 2, data, len );
				inherited::FSendQueue.push_back( ABuffer );
				return;
			}

			inherited::setModeIdle(); // Prevent RX while filling the fifo
//			Serial.println( "SEND" );

			SPI.beginTransaction( SPISettings() );
			digitalWrite( PIN_CS, LOW );
			SPI.transfer( inherited::RH_RF69_REG_00_FIFO | 0x80 ); // Send the start address with the write mask on
			SPI.transfer( len + 1 ); // Include length of headers
			// First the 4 headers
			SPI.transfer( ATxHeaderTo );
//			SPI.transfer( Address );
//			SPI.transfer(_txHeaderId);
//			SPI.transfer(_txHeaderFlags);
			// Now the payload
			while( len-- )
				SPI.transfer( *data++ );

			digitalWrite( PIN_CS, HIGH );
			SPI.endTransaction();

			inherited::setModeTx(); // Start the transmitter
		}

	protected:
		static __ICACHE_RAM_ATTR__ void StaticInterruptHandler()
		{
			FInstnce->InterruptHandler( true );
		}

	public:
		inline void SystemInit()
		{
			SPI.begin();
			int AInterruptNumber = digitalPinToInterrupt( PIN_INT );
			attachInterrupt( AInterruptNumber, StaticInterruptHandler, RISING );
#ifndef VISUINO_ANY_ESP32
			SPI.usingInterrupt( AInterruptNumber );
#endif
		}

	public:
		inline MitovLoRaRFM69Shield()
		{
			FInstnce = this;
			pinMode( PIN_RST, OUTPUT );
			pinMode( PIN_CS, OUTPUT );
		}

	};
//---------------------------------------------------------------------------
	template <
			int PIN_CS, int PIN_RST, int PIN_INT,
			typename T_Address,
			typename T_BroadcastAddress,
            typename T_EncryptionKey,
            typename T_EncryptionKey_ApplyValues,
            typename T_EncryptionKey_GetValue,
			typename T_FMode,
			typename T_FModeChange,
			typename T_Frequency,
			typename T_MaxSendQueue,
			typename T_ModemSettings,
			typename T_OutputPin,
			typename T_Power,
			typename T_PreambleLength,
			typename T_Promiscuous,
			typename T_SentToOutputPin,
			typename T_SignalStrengthOutputPin,
            typename T_SyncWords,
            typename T_SyncWords_ApplyValues,
            typename T_SyncWords_GetValue
		> MitovLoRaRFM69Shield <
				PIN_CS,PIN_RST,
				PIN_INT,
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			>	*MitovLoRaRFM69Shield <
				PIN_CS,PIN_RST,
				PIN_INT,
				T_Address,
				T_BroadcastAddress,
                T_EncryptionKey,
                T_EncryptionKey_ApplyValues,
                T_EncryptionKey_GetValue,
				T_FMode,
				T_FModeChange,
				T_Frequency,
				T_MaxSendQueue,
				T_ModemSettings,
				T_OutputPin,
				T_Power,
				T_PreambleLength,
				T_Promiscuous,
				T_SentToOutputPin,
				T_SignalStrengthOutputPin,
                T_SyncWords,
                T_SyncWords_ApplyValues,
                T_SyncWords_GetValue
			>::FInstnce = nullptr;
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif