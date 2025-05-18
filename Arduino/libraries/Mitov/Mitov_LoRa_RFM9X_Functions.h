////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( SignalStrengthOutputPin )

	public:
		#include <Mitov_LoRa_RFM9X_Registers.h>

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
		_V_PROP_( PreambleLength )
		_V_PROP_( Power )
		_V_PROP_( Frequency )
		_V_PROP_( SignalBandwidth )
		_V_PROP_( MaxSendQueue )

	public:
		_V_PROP_( SymbolTimeout )
		_V_PROP_( LoRaTMMode )
		_V_PROP_( MobileNode )
		_V_PROP_( LowNoiseAmplifierHighFrequencyBoost )
		_V_PROP_( Gain )
		_V_PROP_( CodingRate )
		_V_PROP_( SpreadingFactorOrder )

	protected:
		_V_PROP_( FReady )
		_V_PROP_( FModeChange )
		_V_PROP_( FMode )

		/// The value of the last received RSSI value, in some transport specific units
		volatile int8_t     FLastRssi = 0;

		Mitov::SimpleList<uint8_t *>	FReceivedPackets;
		Mitov::SimpleList<uint8_t *>	FSendQueue;

	public:
		void Print( Mitov::String AValue )
		{
			AValue += "\r\n";
			send( (uint8_t *)AValue.c_str(), AValue.length() );
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
			send( (uint8_t*)&AValue, 1 );
		}

		inline void PrintChar( uint8_t AValue )
		{
			send( &AValue, 1 );
		}

		inline void Write( uint8_t *AData, uint32_t ASize )
		{
			send( AData, ASize );
		}

	public:
		void send( const uint8_t* data, uint8_t len )
		{
			if( ! FReady() )
				return;

#ifdef ___MITOV_LORA_RFM9X_DEBUG__
			Serial.println( "TRY SEND" );
			Serial.println( len );
#endif
//				Serial.println( FTestCount );
//				Serial.println( int( FMode ));
//			if (len > RH_RF69_MAX_MESSAGE_LEN)
//				return;

//			waitPacketSent(); // Make sure we dont interrupt an outgoing message // _mode == RHModeTx

			if( FMode() == RHModeTx )
			{
				while( FSendQueue.size() > MaxSendQueue() )
					;

				uint8_t *ABuffer = new uint8_t[ len + 1 ];
				ABuffer[ 0 ] = len;
				memcpy( ABuffer + 1, data, len );
				FSendQueue.push_back( ABuffer );
				return;
			}

			setModeIdle(); // Prevent RX while filling the fifo
#ifdef ___MITOV_LORA_RFM9X_DEBUG__
			Serial.println( "SEND" );
#endif

			// Position at the beginning of the FIFO
			spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);

			TransferPacket( data, len );

			spiWrite( RH_RF95_REG_22_PAYLOAD_LENGTH, len );

			setModeTx(); // Start the transmitter
		}

	protected:
		void IntSystemInit()
		{

//			ChipSelectOutputPin.SendValue( true );

			Reset();
//			Serial.println( "START1" );

			// Set sleep mode, so we can also set LORA mode:
			spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE);
			delay(10); // Wait for sleep mode to take over from say, CAD

			// Check we are in sleep mode, with LORA set
			if (spiRead(RH_RF95_REG_01_OP_MODE) != (RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE))
			{
		//	Serial.println(spiRead(RH_RF95_REG_01_OP_MODE), HEX);
				return; // No device present?
			}

			FReady() = true;

//			Serial.println( spiRead( RH_RF95_REG_1F_SYMB_TIMEOUT_LSB ), HEX );

//			Serial.println( "START2" );
			// Set up FIFO
			// We configure so that we can use the entire 256 byte FIFO for either receive
			// or transmit, but not both at the same time
			spiWrite(RH_RF95_REG_0E_FIFO_TX_BASE_ADDR, 0);
			spiWrite(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 0);

			// Packet format is preamble + explicit-header + payload + crc
			// Explicit Header Mode
			// payload is TO + FROM + ID + FLAGS + message data
			// RX mode is implmented with RXCONTINUOUS
			// max message data length is 255 - 4 = 251 octets

			setModeIdle();

/*
			// Set up default configuration
			// No Sync Words in LORA mode.
			setModemConfig(Bw125Cr45Sf128); // Radio default
		//    setModemConfig(Bw125Cr48Sf4096); // slow and reliable?
			setPreambleLength(8); // Default is 8
			// An innocuous ISM frequency, same as RF22's
			setFrequency(434.0);
			// Lowish power
			setTxPower(13);
		//    setTxPower(20);
*/
			UpdateModemRegisters();
			UpdatePreambleLength();
			UpdateFrequency();
			UpdateTransmitPower();
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! FReady() )
				return;

			if( FModeChange() )
			{
				if( ! LoRaTMMode() )
					if( !(spiRead(RH_RF95_REG_3E_IRQFLAGS1) & RH_RF95_IRQFLAGS1_MODEREADY) )
						return;

				FModeChange() = false;
			}

			ProcessInterrupt();

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
//				uint8_t ASentTo = APacket[ 0 ];
				uint8_t ASize = APacket[ 0 ];
#ifdef ___MITOV_LORA_RFM9X_DEBUG__
				Serial.println( "RECEIVED" );
				Serial.println( ASize );
#endif
				T_OutputPin::SetPinValue( Mitov::TDataBlock( ASize, APacket + 1 ));
				delete [] APacket;
			}

			if( FMode() != RHModeTx )
			{
				if( FSendQueue.size() )
				{
					uint8_t *ABuffer = FSendQueue[ 0 ];
					FSendQueue.pop_front();
					send( ABuffer + 1, ABuffer[ 0 ] );
					delete [] ABuffer;
				}
			}

			if( FMode() == RHModeIdle )
				if( T_OutputPin::GetPinIsConnected() )
					setModeRx();

			T_SignalStrengthOutputPin::SetPinValue( FLastRssi );
		}

	protected:
		/// If current mode is Rx or Tx changes it to Idle. If the transmitter or receiver is running,
		/// disables them.
		void setModeIdle()
		{
			if( FMode() != RHModeIdle )
			{
				setOpMode( RH_RF95_MODE_STDBY );
				FMode() = RHModeIdle;
			}
		}

		void setModeRx()
		{
			if( FMode() != RHModeRx )
			{
				spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_RXCONTINUOUS);
				spiWrite(RH_RF95_REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone
				FMode() = RHModeRx;
			}
		}

		void setModeTx()
		{
			if( FMode() != RHModeTx )
			{
				spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_TX);
				spiWrite(RH_RF95_REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone
				FMode() = RHModeTx;
			}
		}

		void setOpMode(uint8_t mode)
		{
			uint8_t opmode = spiRead(RH_RF95_REG_01_OP_MODE);
			opmode &= ~RH_RF95_MODE;
			opmode |= (mode & RH_RF95_MODE);
			spiWrite(RH_RF95_REG_01_OP_MODE, opmode);

			FModeChange() = true;

			// Wait for mode to change.
//			while (!(spiRead(RH_RF69_REG_27_IRQFLAGS1) & RH_RF69_IRQFLAGS1_MODEREADY))
//				;
		}

	public:
		void UpdatePreambleLength()
		{
//			spiWrite(RH_RF69_REG_2C_PREAMBLEMSB, PreambleLength >> 8);
//			spiWrite(RH_RF69_REG_2D_PREAMBLELSB, PreambleLength & 0xff);

			spiWrite(RH_RF95_REG_20_PREAMBLE_MSB, PreambleLength() >> 8);
		    spiWrite(RH_RF95_REG_21_PREAMBLE_LSB, PreambleLength() & 0xff);
		}

		void UpdateFrequency()
		{
			// Frf = FRF / FSTEP
/*
			uint32_t frf = (Frequency * 1000000.0) / RH_RF69_FSTEP;
//			Serial.println( frf );
			spiWrite(RH_RF69_REG_07_FRFMSB, (frf >> 16) & 0xff);
			spiWrite(RH_RF69_REG_08_FRFMID, (frf >> 8) & 0xff);
			spiWrite(RH_RF69_REG_09_FRFLSB, frf & 0xff);
*/
			uint32_t frf = (Frequency() * 1000000.0) / RH_RF95_FSTEP;
			spiWrite(RH_RF95_REG_06_FRF_MSB, (frf >> 16) & 0xff);
			spiWrite(RH_RF95_REG_07_FRF_MID, (frf >> 8) & 0xff);
			spiWrite(RH_RF95_REG_08_FRF_LSB, frf & 0xff);
		}

		void UpdateTransmitPower()
		{
			int8_t aPower = Power();
			if (aPower > 20)
				aPower = 20;

			if (aPower < 5)
				aPower = 5;

			// RFM95/96/97/98 does not have RFO pins connected to anything. ONly PA_BOOST
			// pin is connected, so must use PA_BOOST
			// Pout = 2 + OutputPower.
			// The documentation is pretty confusing on this topic: PaSelect says the max poer is 20dBm,
			// but OutputPower claims it would be 17dBm.
			// My measurements show 20dBm is correct
			spiWrite(RH_RF95_REG_09_PA_CONFIG, RH_RF95_PA_SELECT | (aPower-5));
		//    spiWrite(RH_RF95_REG_09_PA_CONFIG, 0); // no power
		}

		// Sets registers from a canned modem configuration structure
		void UpdateModemRegisters()
		{
//    { 0x72,   0x74,    0x00}, // Bw125Cr45Sf128 (the chip default)

//			spiWrite(RH_RF95_REG_1D_MODEM_CONFIG1,       config->reg_1d);
//			spiWrite(RH_RF95_REG_1E_MODEM_CONFIG2,       config->reg_1e);

			uint8_t	AValue;

			if( Gain() == gcAuto )
				AValue = 0b11000000;

			else
				AValue = Gain() << 5;

			AValue |= (( LowNoiseAmplifierHighFrequencyBoost() ) ? 0b11 : 0 );

			spiWrite( RH_RF95_REG_0C_LNA, AValue );

			static const float CSignalBandwidths[] =
			{
				7.8,
				10.4,
				15.6,
				20.8,
				31.25,
				41.7,
				62.5,
				125.0,
				250.0,
				500.0
			};

			AValue = 9;
			for( int i = 0; i < sizeof( CSignalBandwidths ) / sizeof( CSignalBandwidths[ 0 ] ); ++i )
				if( SignalBandwidth() <= CSignalBandwidths[ i ] )
				{
					AValue = i;
					break;
				}

//			Serial.println( AValue );

			AValue <<= 4;
			AValue |= ( CodingRate() + 1 ) << 1;

//			Serial.println( AValue, HEX );

//			spiWrite(RH_RF95_REG_1D_MODEM_CONFIG1,       0x72 );
			spiWrite(RH_RF95_REG_1D_MODEM_CONFIG1,       AValue );

			AValue = ( SpreadingFactorOrder() << 4 ) |
				     0b100 | // Header indicates CRC on
					 ( SymbolTimeout() >> 8 );

//			Serial.println( AValue, HEX );

//			spiWrite(RH_RF95_REG_1E_MODEM_CONFIG2,       0x74 );
			spiWrite(RH_RF95_REG_1E_MODEM_CONFIG2,       AValue );

			AValue =	(( Gain() == gcAuto ) ? 0 : 0b100 ) |
								(( MobileNode() ) ? 0b1000 : 0 );
			spiWrite(RH_RF95_REG_26_MODEM_CONFIG3,       AValue );

			spiWrite( RH_RF95_REG_1F_SYMB_TIMEOUT_LSB, uint8_t( SymbolTimeout() ));

		}


