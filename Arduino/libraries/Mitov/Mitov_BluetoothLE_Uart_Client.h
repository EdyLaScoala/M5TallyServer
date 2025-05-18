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
#include <Mitov_BluetoothLE_Uart_Service.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
#ifdef VISUINO_BLE_NINA
	typedef BLEService MitovBLEClientService;
	typedef BLECharacteristic MitovBLEClientCharacteristic;
#else
	typedef BLERemoteService MitovBLEClientService;
	typedef BLERemoteCharacteristic MitovBLEClientCharacteristic;
#endif
//---------------------------------------------------------------------------
	class ArduinoBluetoothLEUartClientImpl_Nordic
	{
	protected:
		MitovBLEClientService			*FService;
		MitovBLEClientCharacteristic	*FTXCharacteristic;
		MitovBLEClientCharacteristic	*FRXCharacteristic;

	protected:
		template <typename T_OWNER> inline void TryStartImpl( T_OWNER &AOwner, bool AValue, bool &AResult )
		{
			if( AValue )
			{
#ifdef VISUINO_ESP32_BLE
				FService = AOwner.GetClient()->getService( ArduinoBluetoothLEUart::SERVICE_UUID );
				FTXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::RX_UUID ); // TX and RX are reversed!

				FRXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::TX_UUID ); // TX and RX are reversed!
//				if( FRXCharacteristic )
//					FRXCharacteristic->registerForNotify( NotifyCallback );

#else // VISUINO_ESP32_BLE
	#ifdef VISUINO_BLE_NINA
//				C_OWNER.GetClient()->discoverAttributes();
//				Serial.println( C_OWNER.GetClient()->discoverService
//				Serial.println( "TryStart" );
//				Serial.println( C_OWNER.GetClient()->serviceCount() );
//				Serial.println( C_OWNER.GetClient()->service( 0 ).uuid() );
//				FService = new BLEService( C_OWNER.GetClient()->service( Mitov::ToLowerCaseString( ArduinoBluetoothLEUart::SERVICE_UUID ).c_str() ));
				FService = new BLEService( AOwner.GetClient()->service( ArduinoBluetoothLEUart::SERVICE_UUID ));
				if( ! ( *FService ))
				{
//					Serial.println( "Missing Service" );
					delete FService;
					FService = nullptr;
					AResult = false;
					return;
				}

//				Serial.println( "STEP1" );
//				Serial.println( FService->uuid() );
				FTXCharacteristic = new BLECharacteristic( FService->characteristic( ArduinoBluetoothLEUart::RX_UUID )); // TX and RX are reversed!
				if( ! ( *FTXCharacteristic ))
				{
//					Serial.println( "Missing TX Characteristic" );
					delete FService;
					FService = nullptr;
					delete FTXCharacteristic;
					FTXCharacteristic = nullptr;
					AResult = false;
				}

//				Serial.println( "STEP2" );
//				Serial.println( FTXCharacteristic->uuid() );

				FRXCharacteristic = new BLECharacteristic( FService->characteristic( ArduinoBluetoothLEUart::TX_UUID )); // TX and RX are reversed!
				if( ! ( *FRXCharacteristic ))
				{
//					Serial.println( "Missing TX Characteristic" );
					delete FService;
					FService = nullptr;
					delete FTXCharacteristic;
					FTXCharacteristic = nullptr;
					delete FRXCharacteristic;
					FRXCharacteristic = nullptr;
					AResult = false;
				}

//				Serial.println( "STEP3" );
//				Serial.println( FRXCharacteristic->uuid() );

//				FRXCharacteristic->setEventHandler( BLEWritten, DoCharacteristicWritten );

//				FRXCharacteristic->subscribe();
//				if( FRXCharacteristic )
//					FRXCharacteristic->
	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE
			}

			else
			{
#ifdef VISUINO_BLE_NINA
				delete( FRXCharacteristic );
				delete( FTXCharacteristic );
				delete( FService );
#endif // VISUINO_BLE_NINA
				FRXCharacteristic = nullptr;
				FTXCharacteristic = nullptr;
				FService = nullptr;
			}
		}

		inline void WriteData( uint8_t *AData, uint32_t ASize )
		{
			FTXCharacteristic->writeValue( AData, ASize );
		}

		inline MitovBLEClientCharacteristic *GetRXCharacteristic()
		{
			return FRXCharacteristic;
		}

		inline MitovBLEClientCharacteristic *GetTXCharacteristic()
		{
			return FTXCharacteristic;
		}

	};
//---------------------------------------------------------------------------
	class ArduinoBluetoothLEUartClientImpl_HC08
	{
	protected:
		MitovBLEClientService			*FService;
		MitovBLEClientCharacteristic	*FCharacteristic;

	protected:
		template <typename T_OWNER> inline void TryStartImpl( T_OWNER &AOwner, bool AValue, bool &AResult )
		{
			if( AValue )
			{
#ifdef VISUINO_ESP32_BLE
				FService = AOwner.GetClient()->getService( ArduinoBluetoothLEUart::HC08_SERVICE_UUID );
				FCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::HC08_COM_UUID ); // TX and RX are reversed!

//				if( FCharacteristic )
//					FCharacteristic->registerForNotify( NotifyCallback );

#else // VISUINO_ESP32_BLE
	#ifdef VISUINO_BLE_NINA
//				C_OWNER.GetClient()->discoverAttributes();
//				Serial.println( C_OWNER.GetClient()->discoverService
//				Serial.println( "TryStart" );
//				Serial.println( C_OWNER.GetClient()->serviceCount() );
//				Serial.println( C_OWNER.GetClient()->service( 0 ).uuid() );
//				FService = new BLEService( C_OWNER.GetClient()->service( Mitov::ToLowerCaseString( ArduinoBluetoothLEUart::SERVICE_UUID ).c_str() ));
				FService = new BLEService( AOwner.GetClient()->service( ArduinoBluetoothLEUart::HC08_SERVICE_UUID ));
				if( ! ( *FService ))
				{
//					Serial.println( "Missing Service" );
					delete FService;
					FService = nullptr;
					AResult = false;
					return;
				}

//				Serial.println( "STEP1" );
//				Serial.println( FService->uuid() );
				FCharacteristic = new BLECharacteristic( FService->characteristic( ArduinoBluetoothLEUart::HC08_COM_UUID ));
				if( ! ( *FCharacteristic ))
				{
//					Serial.println( "Missing TX Characteristic" );
					delete FService;
					FService = nullptr;
					delete FCharacteristic;
					FCharacteristic = nullptr;
					AResult = false;
				}

//				Serial.println( "STEP3" );
//				Serial.println( FRXCharacteristic->uuid() );

//				FRXCharacteristic->setEventHandler( BLEWritten, DoCharacteristicWritten );

//				FRXCharacteristic->subscribe();
//				if( FRXCharacteristic )
//					FRXCharacteristic->
	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE
			}

			else
			{
#ifdef VISUINO_BLE_NINA
				delete( FCharacteristic );
				delete( FService );
#endif // VISUINO_BLE_NINA
				FCharacteristic = nullptr;
				FService = nullptr;
			}
		}

		inline void WriteData( uint8_t *AData, uint32_t ASize )
		{
			FCharacteristic->writeValue( AData, ASize );
		}

		inline MitovBLEClientCharacteristic *GetRXCharacteristic()
		{
			return FCharacteristic;
		}

		inline MitovBLEClientCharacteristic *GetTXCharacteristic()
		{
			return FCharacteristic;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BASE,
		typename T_BufferSize,
		typename T_Enabled,
		typename T_OutputPin
	> class ArduinoBluetoothLEUartClient :
		public T_BASE,
		public T_BufferSize,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( BufferSize )

	protected:
		static	void *FInstance;

	protected:
#ifdef VISUINO_ESP32_BLE
		static void NotifyCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify )
		{
			((ArduinoBluetoothLEUartClient *)FInstance )->InstanceNotifyCallback( pData, length );
		}

		inline void InstanceNotifyCallback( uint8_t* pData, size_t length )
		{
			if( ! Enabled() )
				return;

			T_OutputPin::SendPinValue( Mitov::TDataBlock( length, pData ));
		}
#else // VISUINO_ESP32_BLE
	#ifdef VISUINO_BLE_NINA
		static void DoCharacteristicWritten( BLEDevice central, BLECharacteristic characteristic )
		{
			if( ! ((ArduinoBluetoothLEUartClient *)FInstance )->Enabled().GetValue() )
				return;

//			if( ! FInstance->FRXCharacteristic )
//				return;

//			Serial.print( "Characteristic event, written: " );
			int ALength = characteristic.valueLength();
//						uint8_t *AData = new uint8_t[ ALength ];
//						Serial.println( ALength );
//						FRXCharacteristic->readValue( AData, ALength );
			const uint8_t *AData = characteristic.value();
//						for( int i = 0; i < ALength; ++i )
//							Serial.println( AData[ i ], HEX );


			((ArduinoBluetoothLEUartClient *)FInstance )->OutputPin().SendPinValue( Mitov::TDataBlock( ALength, AData ));
		}

	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE
/*
	public:
		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( pCharacteristic->m_value.getLength(), pCharacteristic->getData() ));
		}
*/
	protected:
	    static size_t write_Impl( ArduinoBluetoothLEUartClient * AInstance, uint8_t AByte )
		{
			return AInstance->write( AByte );
		}

	public:
		inline VirtualStreamOut<ArduinoBluetoothLEUartClient> GetStream()
		{
			return VirtualStreamOut<ArduinoBluetoothLEUartClient>( this, (typename VirtualStreamOut<ArduinoBluetoothLEUartClient>::TCallback) &ArduinoBluetoothLEUartClient::write_Impl );
		}

	public:
		size_t write( uint8_t AData )
		{
			T_BASE::WriteData( &AData, 1 );
			return 1;
		}

		size_t write( uint8_t *AData, uint32_t ASize )
		{
			if( ! Enabled() )
				return 0;

			if( ! T_BASE::GetTXCharacteristic() )
				return 0;

//			if( ! C_OWNER.IsConnected() )
//				return;

            uint32_t ASizeToSend = ASize;
			while( ASizeToSend )
			{
				uint8_t ASendSize = MitovMin<uint32_t>( ASizeToSend, 20 );
				T_BASE::WriteData( AData, ASendSize );
//				FTXCharacteristic->writeValue( AData, ASendSize );
//				FTXCharacteristic->setValue( AData, ASendSize );
//				FTXCharacteristic->notify();
				AData += ASendSize;
				ASizeToSend -= ASendSize;
			}

            return ASize;
		}

		void Print( const Mitov::String AValue )
		{
			if( ! Enabled() )
				return;

			write( (uint8_t *)AValue.c_str(), AValue.length());
			write( (uint8_t *)"\r\n", 2 );
		}

		void Print( float AValue )
		{
			if( ! Enabled() )
				return;

			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			if( ! Enabled() )
				return;

			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			if( ! Enabled() )
				return;

			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		template <typename T> inline void PrintChar( T AValue )
		{
			write( AValue );
		}

		inline size_t Write( uint8_t *AData, uint32_t ASize )
		{
			return write( AData, ASize );
		}

	    inline size_t Write(uint8_t AByte)
		{
			return write( AByte );
		}

	public:
		inline void TryStart( bool AValue, bool &AResult )
		{
			T_BASE::TryStartImpl( C_OWNER, AValue, AResult );
			if( AValue )
				if( T_BASE::GetRXCharacteristic() )
#ifdef VISUINO_ESP32_BLE
					T_BASE::GetRXCharacteristic()->registerForNotify( NotifyCallback );
#endif // VISUINO_ESP32_BLE

#ifdef VISUINO_BLE_NINA
					T_BASE::GetRXCharacteristic()->setEventHandler( BLEWritten, DoCharacteristicWritten );
#endif // VISUINO_BLE_NINA

		}

	public:
		inline void SystemInit()
		{
/*
#ifdef VISUINO_ESP32_BLE
			FService = C_OWNER.GetClient()->getService( ArduinoBluetoothLEUart::SERVICE_UUID );
			FTXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::RX_UUID ); // TX and RX are reversed!

			FRXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::TX_UUID ); // TX and RX are reversed!
			FRXCharacteristic->registerForNotify( NotifyCallback );

#else // VISUINO_ESP32_BLE
#endif // VISUINO_ESP32_BLE
*/


/*
#ifdef VISUINO_ESP32_BLE
			FService = C_OWNER.FServer->createService( ArduinoBluetoothLEUart::SERVICE_UUID );
#else // VISUINO_ESP32_BLE
			FService = new BLEService( ArduinoBluetoothLEUart::SERVICE_UUID );
			C_OWNER.GetPeripheral().addAttribute( *FService );
#endif // VISUINO_ESP32_BLE

			C_OWNER.AddveriseService( FService );

#ifdef VISUINO_ESP32_BLE
// Create a BLE Characteristic
			FTXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify );

			FService->addCharacteristic( FTXCharacteristic );
//			FTXCharacteristic->setCallbacks( this );
			FTXCharacteristic->addDescriptor(new BLE2902());

			FRXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse );
//			FService->addCharacteristic( FRXCharacteristic );
			FRXCharacteristic->setCallbacks( this );

#else
			FTXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify, 20 );
			FService->addCharacteristic( *FTXCharacteristic );

			FRXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse, 20 );
			FService->addCharacteristic( *FRXCharacteristic );
#endif

#ifdef VISUINO_ESP32_BLE
			FService->start();
#endif
*/
		}

		inline void SystemLoopBegin()
		{
//			Serial.println( "SystemLoopBegin" );
/*
			if( FService )
				if( ! ( *FService ))
					Serial.println( "#" );

			if( FRXCharacteristic )
				if( ! ( *FRXCharacteristic ))
					Serial.println( "*" );
*/
/*
#ifndef VISUINO_ESP32_BLE
			if( C_OWNER.IsConnected() )
				if( FRXCharacteristic->written() )
					T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( FRXCharacteristic->valueLength(), FRXCharacteristic->value() ));
#endif
*/
		}

	public:
		ArduinoBluetoothLEUartClient()
		{
			FInstance = this;
		}

	};
//---------------------------------------------------------------------------
template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BASE,
		typename T_BufferSize,
		typename T_Enabled,
		typename T_OutputPin
	> void * ArduinoBluetoothLEUartClient<
			T_OWNER, C_OWNER,
			T_BASE,
			T_BufferSize,
			T_Enabled,
			T_OutputPin
		>::FInstance;
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BASE,
		typename T_BufferSize,
		typename T_Enabled,
		typename T_OutputPin
	> class ArduinoBluetoothLEUartClient_Stream :
		public Buffered_Stream,
		public T_BASE,
		public T_BufferSize,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( BufferSize )

	protected:
		static	void *FInstance;

	protected:
#ifdef VISUINO_ESP32_BLE
		static void NotifyCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify )
		{
			((ArduinoBluetoothLEUartClient_Stream *)FInstance )->InstanceNotifyCallback( pData, length );
		}

		inline void InstanceNotifyCallback( uint8_t* pData, size_t length )
		{
			if( ! Enabled() )
				return;

			T_OutputPin::SendPinValue( Mitov::TDataBlock( length, pData ));
		}
#else // VISUINO_ESP32_BLE
	#ifdef VISUINO_BLE_NINA
		static void DoCharacteristicWritten( BLEDevice central, BLECharacteristic characteristic )
		{
			if( ! ((ArduinoBluetoothLEUartClient_Stream *)FInstance )->Enabled().GetValue() )
				return;

//			if( ! FInstance->FRXCharacteristic )
//				return;

//			Serial.print( "Characteristic event, written: " );
			int ALength = characteristic.valueLength();
//						uint8_t *AData = new uint8_t[ ALength ];
//						Serial.println( ALength );
//						FRXCharacteristic->readValue( AData, ALength );
			const uint8_t *AData = characteristic.value();
//						for( int i = 0; i < ALength; ++i )
//							Serial.println( AData[ i ], HEX );


			((ArduinoBluetoothLEUartClient_Stream *)FInstance )->OutputPin().SendPinValue( Mitov::TDataBlock( ALength, AData ));
		}

	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE
/*
	public:
		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( pCharacteristic->m_value.getLength(), pCharacteristic->getData() ));
		}
*/
	public:
		size_t write( uint8_t AData )
		{
			T_BASE::WriteData( &AData, 1 );
			return 1;
		}

		size_t write( uint8_t *AData, uint32_t ASize )
		{
			if( ! Enabled() )
				return 0;

			if( ! T_BASE::GetTXCharacteristic() )
				return 0;

//			if( ! C_OWNER.IsConnected() )
//				return;

            uint32_t ASizeToSend = ASize;
			while( ASizeToSend )
			{
				uint8_t ASendSize = MitovMin<uint32_t>( ASizeToSend, 20 );
				T_BASE::WriteData( AData, ASendSize );
//				FTXCharacteristic->writeValue( AData, ASendSize );
//				FTXCharacteristic->setValue( AData, ASendSize );
//				FTXCharacteristic->notify();
				AData += ASendSize;
				ASizeToSend -= ASendSize;
			}

            return ASize;
		}

		void Print( const Mitov::String AValue )
		{
			if( ! Enabled() )
				return;

			write( (uint8_t *)AValue.c_str(), AValue.length());
			write( (uint8_t *)"\r\n", 2 );
		}

		void Print( float AValue )
		{
			if( ! Enabled() )
				return;

			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			if( ! Enabled() )
				return;

			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			if( ! Enabled() )
				return;

			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		template <typename T> inline void PrintChar( T AValue )
		{
			write( AValue );
		}

		inline size_t Write( uint8_t *AData, uint32_t ASize )
		{
			return write( AData, ASize );
		}

	    inline size_t Write(uint8_t AByte)
		{
			return write( AByte );
		}

	public:
		inline void TryStart( bool AValue, bool &AResult )
		{
			T_BASE::TryStartImpl( C_OWNER, AValue, AResult );
			if( AValue )
				if( T_BASE::GetRXCharacteristic() )
#ifdef VISUINO_ESP32_BLE
					T_BASE::GetRXCharacteristic()->registerForNotify( NotifyCallback );
#endif // VISUINO_ESP32_BLE

#ifdef VISUINO_BLE_NINA
					T_BASE::GetRXCharacteristic()->setEventHandler( BLEWritten, DoCharacteristicWritten );
#endif // VISUINO_BLE_NINA

		}

	public:
		inline void SystemInit()
		{
/*
#ifdef VISUINO_ESP32_BLE
			FService = C_OWNER.GetClient()->getService( ArduinoBluetoothLEUart::SERVICE_UUID );
			FTXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::RX_UUID ); // TX and RX are reversed!

			FRXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::TX_UUID ); // TX and RX are reversed!
			FRXCharacteristic->registerForNotify( NotifyCallback );

#else // VISUINO_ESP32_BLE
#endif // VISUINO_ESP32_BLE
*/


/*
#ifdef VISUINO_ESP32_BLE
			FService = C_OWNER.FServer->createService( ArduinoBluetoothLEUart::SERVICE_UUID );
#else // VISUINO_ESP32_BLE
			FService = new BLEService( ArduinoBluetoothLEUart::SERVICE_UUID );
			C_OWNER.GetPeripheral().addAttribute( *FService );
#endif // VISUINO_ESP32_BLE

			C_OWNER.AddveriseService( FService );

#ifdef VISUINO_ESP32_BLE
// Create a BLE Characteristic
			FTXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify );

			FService->addCharacteristic( FTXCharacteristic );
//			FTXCharacteristic->setCallbacks( this );
			FTXCharacteristic->addDescriptor(new BLE2902());

			FRXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse );
//			FService->addCharacteristic( FRXCharacteristic );
			FRXCharacteristic->setCallbacks( this );

#else
			FTXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify, 20 );
			FService->addCharacteristic( *FTXCharacteristic );

			FRXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse, 20 );
			FService->addCharacteristic( *FRXCharacteristic );
#endif

#ifdef VISUINO_ESP32_BLE
			FService->start();
#endif
*/
		}

		inline void SystemLoopBegin()
		{
//			Serial.println( "SystemLoopBegin" );
/*
			if( FService )
				if( ! ( *FService ))
					Serial.println( "#" );

			if( FRXCharacteristic )
				if( ! ( *FRXCharacteristic ))
					Serial.println( "*" );
*/
/*
#ifndef VISUINO_ESP32_BLE
			if( C_OWNER.IsConnected() )
				if( FRXCharacteristic->written() )
					T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( FRXCharacteristic->valueLength(), FRXCharacteristic->value() ));
#endif
*/
		}

	public:
		ArduinoBluetoothLEUartClient_Stream()
		{
			FInstance = this;
		}

	};
//---------------------------------------------------------------------------
template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BASE,
		typename T_BufferSize,
		typename T_Enabled,
		typename T_OutputPin
	> void * ArduinoBluetoothLEUartClient_Stream<
			T_OWNER, C_OWNER,
			T_BASE,
			T_BufferSize,
			T_Enabled,
			T_OutputPin
		>::FInstance;
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif