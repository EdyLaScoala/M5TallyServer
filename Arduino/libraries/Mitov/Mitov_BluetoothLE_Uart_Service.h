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
#include <Mitov_BluetoothLE_Services.h>
#ifdef VISUINO_ESP32_BLE
	#include <BLE2902.h>
#endif

namespace Mitov
{
	namespace ArduinoBluetoothLEUart
	{
		const char *SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
		const char *RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
		const char *TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

		const char *HC08_SERVICE_UUID = "0000FFE0-0000-1000-8000-00805F9B34FB";
		const char *HC08_COM_UUID = "0000FFE1-0000-1000-8000-00805F9B34FB";
	}
//---------------------------------------------------------------------------
	class ArduinoBluetoothLEUartServiceImpl_Nordic
#ifdef VISUINO_ESP32_BLE
		: public BLECharacteristicCallbacks
#endif //  VISUINO_ESP32_BLE
	{
	protected:
		BLEService			*FService;
		BLECharacteristic	*FTXCharacteristic;
		BLECharacteristic	*FRXCharacteristic;

	protected:
		inline void WriteData( uint8_t *AData, uint32_t ASize )
		{
			FTXCharacteristic->setValue( AData, ASize );
#ifdef VISUINO_ESP32_BLE
			FTXCharacteristic->notify();
#endif
		}

		template <typename T_OWNER> inline void InitImpl( T_OWNER &AOwner )
		{
#ifdef VISUINO_ESP32_BLE
			FService = AOwner.GetServer()->createService( ArduinoBluetoothLEUart::SERVICE_UUID );
#else // VISUINO_ESP32_BLE
			FService = new BLEService( ArduinoBluetoothLEUart::SERVICE_UUID );
	#ifndef VISUINO_BLE_NINA
			AOwner.GetPeripheral().addAttribute( *FService );
	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE

			AOwner.AddveriseService( FService );

#ifdef VISUINO_ESP32_BLE
// Create a BLE Characteristic
			FTXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify );

			FService->addCharacteristic( FTXCharacteristic );
//			FTXCharacteristic->setCallbacks( this );
			FTXCharacteristic->addDescriptor(new BLE2902());

			FRXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse );
//			FService->addCharacteristic( FRXCharacteristic );
			FRXCharacteristic->setCallbacks( this );

#else // VISUINO_ESP32_BLE
			FTXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify, 20 );
			FService->addCharacteristic( *FTXCharacteristic );

			FRXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse, 20 );
			FService->addCharacteristic( *FRXCharacteristic );
#endif // VISUINO_ESP32_BLE
		}

		inline BLECharacteristic *GetRXCharacteristic()
		{
			return FRXCharacteristic;
		}

	};
//---------------------------------------------------------------------------
	class ArduinoBluetoothLEUartServiceImpl_HC08
#ifdef VISUINO_ESP32_BLE
		: public BLECharacteristicCallbacks
#endif //  VISUINO_ESP32_BLE
	{
	protected:
		BLEService			*FService;
		BLECharacteristic	*FCharacteristic;

	protected:
		inline void WriteData( uint8_t *AData, uint32_t ASize )
		{
//			delay( 100 );
//			Serial.println( ASize );
//			( *AData ) ++;
//			Serial.println( uint32_t( *AData ), HEX );
#ifdef VISUINO_BLE_NINA
			FCharacteristic->writeValue( AData, ASize, true );
//			FCharacteristic->writeValue( AData, ASize, true );
//			FCharacteristic->broadcast();
#else // VISUINO_ESP32_BLE
			FCharacteristic->setValue( AData, ASize );
#endif

#ifdef VISUINO_ESP32_BLE
			FCharacteristic->notify();
#endif
		}

		template <typename T_OWNER> inline void InitImpl( T_OWNER &AOwner )
		{
#ifdef VISUINO_ESP32_BLE
			FService = AOwner.GetServer()->createService( ArduinoBluetoothLEUart::HC08_SERVICE_UUID );
#else // VISUINO_ESP32_BLE
			FService = new BLEService( ArduinoBluetoothLEUart::HC08_SERVICE_UUID );
	#ifndef VISUINO_BLE_NINA
			AOwner.GetPeripheral().addAttribute( *FService );
	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE

			AOwner.AddveriseService( FService );

#ifdef VISUINO_ESP32_BLE
// Create a BLE Characteristic
			FCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::HC08_COM_UUID, BLEWriteWithoutResponse | BLERead | BLENotify );
			FService->addCharacteristic( FCharacteristic );
			FCharacteristic->addDescriptor(new BLE2902());

			FCharacteristic->setCallbacks( this );

#else // VISUINO_ESP32_BLE
			FCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::HC08_COM_UUID, BLEWriteWithoutResponse | BLERead | BLENotify, 20 );
			FService->addCharacteristic( *FCharacteristic );
//			FCharacteristic->addDescriptor(new BLE2902());
//	#ifdef VISUINO_BLE_NINA
//			FCharacteristic->broadcast();
//	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE
		}

		inline BLECharacteristic *GetRXCharacteristic()
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
	> class ArduinoBluetoothLEUartService :
		public T_BASE,
		public T_BufferSize,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( BufferSize )
		_V_PROP_( Enabled )

#ifdef VISUINO_BLE_NINA
	protected:
		static	void *FInstance;
#endif // VISUINO_BLE_NINA

	protected:
	    static size_t write_Impl( ArduinoBluetoothLEUartService * AInstance, uint8_t AByte )
		{
			return AInstance->write( AByte );
		}

	public:
		inline VirtualStreamOut<ArduinoBluetoothLEUartService> GetStream()
		{
			return VirtualStreamOut<ArduinoBluetoothLEUartService>( this, (typename VirtualStreamOut<ArduinoBluetoothLEUartService>::TCallback) &ArduinoBluetoothLEUartService::write_Impl );
		}

	public:
		size_t write( uint8_t AData )
		{
			T_BASE::WriteData( &AData, 1 );
			return 1;
		}

		size_t write( uint8_t *AData, uint32_t ASize )
		{
			if( ! C_OWNER.Enabled().GetValue() )
				return 0;

			if( ! Enabled() )
				return 0;

			size_t AResultSize = ASize;
			while( ASize )
			{
				uint8_t ASendSize = MitovMin<uint32_t>( ASize, 20 );

				T_BASE::WriteData( AData, ASendSize );

				AData += ASendSize;
				ASize -= ASendSize;
			}

			return AResultSize;
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

		template <typename T> void PrintChar( T AValue )
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

	protected:
		inline void SendToOutput( uint8_t *AData, uint32_t ALength )
		{
			if( ! C_OWNER.Enabled().GetValue() )
				return;

			if( ! Enabled().GetValue() )
				return;

			T_OutputPin::SendPinValue( Mitov::TDataBlock( ALength, AData ));
		}

#ifdef VISUINO_ESP32_BLE
	public:
		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
//			Serial.println( "RECEIVE" );
			SendToOutput( pCharacteristic->getData(), pCharacteristic->m_value.getLength() );
		}

#else // VISUINO_ESP32_BLE
	#ifdef VISUINO_BLE_NINA
		static void DoCharacteristicWritten( BLEDevice central, BLECharacteristic characteristic )
		{
//			Serial.println( "RECEIVE" );
			((ArduinoBluetoothLEUartService *)FInstance )->SendToOutput( (uint8_t *)characteristic.value(), characteristic.valueLength() );
		}

	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE

	protected:
		inline void InternalInit()
		{
			T_BASE::InitImpl( C_OWNER );

#ifdef VISUINO_BLE_NINA
//			Serial.println( "INIT" );
			T_BASE::GetRXCharacteristic()->setEventHandler( BLEWritten, DoCharacteristicWritten );
#endif // VISUINO_BLE_NINA


/*
			if( UserDescription != "" )
			{
#ifdef VISUINO_ESP32_BLE
//				FUserDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
//				FUserDescriptor->setValue( UserDescription.c_str() );
//				FCharacteristic->addDescriptor( FUserDescriptor );
#else
//				FUserDescriptor = new BLEDescriptor( "2901", UserDescription.c_str() );
//				FCharacteristic->addDescriptor( *FUserDescriptor );
#endif
			}
*/
//			FCharacteristic->setValueLE( InitialValue * 100 + 0.5 );
#ifdef VISUINO_ESP32_BLE
			T_BASE::FService->start();
#endif
/*
			FService = new BLEService( "180F" ); // Add -0000-1000-8000-00805F9B34FB for standard
			C_OWNER.GetPeripheral().addAttribute( *FService );
			C_OWNER.GetPeripheral().setAdvertisedServiceUuid( FService->uuid());
			FCharacteristic = new BLEUnsignedCharCharacteristic( "2A19", BLERead | BLENotify );
			C_OWNER.GetPeripheral().addAttribute( *FCharacteristic );
			FCharacteristic->setValue( InitialValue * 100 + 0.5 );
*/
#ifdef VISUINO_BLE_NINA
			C_OWNER.GetPeripheral().addService( *T_BASE::FService );
			C_OWNER.GetPeripheral().setAdvertisedService( *T_BASE::FService );
#endif // VISUINO_BLE_NINA
		}

	public:
#ifdef VISUINO_BLE_NINA
		inline void SystemInit() {}
		inline void SystemStart() { InternalInit(); }

#else // VISUINO_BLE_NINA
		inline void SystemInit() { InternalInit(); }
		inline void SystemStart() {}
#endif // VISUINO_BLE_NINA

		inline void SystemLoopBegin()
		{
//			Serial.println( T_BASE::GetRXCharacteristic()->valueLength() );
#if( ! ( defined( VISUINO_ESP32_BLE ) || defined( VISUINO_BLE_NINA ) ) )
			if( C_OWNER.IsConnected() )
				if( T_BASE::GetRXCharacteristic()->written() )
					T_OutputPin::SendPinValue( Mitov::TDataBlock( FRXCharacteristic->valueLength(), FRXCharacteristic->value() ));
#endif
		}

#ifdef VISUINO_BLE_NINA
	public:
		ArduinoBluetoothLEUartService()
		{
			FInstance = this;
		}
#endif // VISUINO_BLE_NINA

	};
//---------------------------------------------------------------------------
#ifdef VISUINO_BLE_NINA
template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BASE,
		typename T_BufferSize,
		typename T_Enabled,
		typename T_OutputPin
	> void * ArduinoBluetoothLEUartService<
			T_OWNER, C_OWNER,
			T_BASE,
			T_BufferSize,
			T_Enabled,
			T_OutputPin
		>::FInstance;
#endif // VISUINO_BLE_NINA
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BASE,
		typename T_BufferSize,
		typename T_Enabled,
		typename T_OutputPin
	> class ArduinoBluetoothLEUartService_Stream :
		public Buffered_Stream,
		public T_BASE,
		public T_BufferSize,
		public T_Enabled,
		public T_OutputPin
	{
		typedef Buffered_Stream inherited;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( BufferSize )
		_V_PROP_( Enabled )

#ifdef VISUINO_BLE_NINA
	protected:
		static	void *FInstance;
#endif // VISUINO_BLE_NINA

	public:
	    virtual size_t write( uint8_t AData ) override
		{
			T_BASE::WriteData( &AData, 1 );
			return 1;
		}

		void write( uint8_t *AData, uint32_t ASize )
		{
			if( ! C_OWNER.Enabled().GetValue() )
				return;

			if( ! Enabled() )
				return;

			while( ASize )
			{
				uint8_t ASendSize = MitovMin<uint32_t>( ASize, 20 );

				T_BASE::WriteData( AData, ASendSize );

				AData += ASendSize;
				ASize -= ASendSize;
			}
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

		template <typename T> void PrintChar( T AValue )
		{
			write( AValue );
		}

		inline size_t Write( uint8_t *AData, uint32_t ASize )
		{
			return write( AData, ASize );
		}

	    inline size_t Write( uint8_t AByte )
		{
			return write( AByte );
		}

	protected:
		inline void SendToOutput( uint8_t *AData, uint32_t ALength )
		{
			if( ! C_OWNER.Enabled().GetValue() )
				return;

			if( ! Enabled().GetValue() )
				return;

			inherited::SendToBuffer( AData, ALength, BufferSize().GetValue() );
			T_OutputPin::SendPinValue( Mitov::TDataBlock( ALength, AData ));
		}

#ifdef VISUINO_ESP32_BLE
	public:
		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			SendToOutput( pCharacteristic->getData(), pCharacteristic->m_value.getLength() );
		}

#else // VISUINO_ESP32_BLE
	#ifdef VISUINO_BLE_NINA
		static void DoCharacteristicWritten( BLEDevice central, BLECharacteristic characteristic )
		{
			((ArduinoBluetoothLEUartService_Stream *)FInstance )->SendToOutput( (uint8_t *)characteristic.value(), characteristic.valueLength() );
		}

	#endif // VISUINO_BLE_NINA
#endif // VISUINO_ESP32_BLE

	protected:
		inline void InternalInit()
		{
			T_BASE::InitImpl( C_OWNER );

#ifdef VISUINO_BLE_NINA
//			Serial.println( "INIT" );
			T_BASE::GetRXCharacteristic()->setEventHandler( BLEWritten, DoCharacteristicWritten );
#endif // VISUINO_BLE_NINA
/*
			if( UserDescription != "" )
			{
#ifdef VISUINO_ESP32_BLE
//				FUserDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
//				FUserDescriptor->setValue( UserDescription.c_str() );
//				FCharacteristic->addDescriptor( FUserDescriptor );
#else
//				FUserDescriptor = new BLEDescriptor( "2901", UserDescription.c_str() );
//				FCharacteristic->addDescriptor( *FUserDescriptor );
#endif
			}
*/
//			FCharacteristic->setValueLE( InitialValue * 100 + 0.5 );
#ifdef VISUINO_ESP32_BLE
			T_BASE::FService->start();
#endif
/*
			FService = new BLEService( "180F" ); // Add -0000-1000-8000-00805F9B34FB for standard
			C_OWNER.GetPeripheral().addAttribute( *FService );
			C_OWNER.GetPeripheral().setAdvertisedServiceUuid( FService->uuid());
			FCharacteristic = new BLEUnsignedCharCharacteristic( "2A19", BLERead | BLENotify );
			C_OWNER.GetPeripheral().addAttribute( *FCharacteristic );
			FCharacteristic->setValue( InitialValue * 100 + 0.5 );
*/
#ifdef VISUINO_BLE_NINA
			C_OWNER.GetPeripheral().addService( *T_BASE::FService );
			C_OWNER.GetPeripheral().setAdvertisedService( *T_BASE::FService );
#endif // VISUINO_BLE_NINA
		}

	public:
#ifdef VISUINO_BLE_NINA
		inline void SystemInit() {}
		inline void SystemStart() { InternalInit(); }

#else // VISUINO_BLE_NINA
		inline void SystemInit() { InternalInit(); }
		inline void SystemStart() {}
#endif // VISUINO_BLE_NINA

		inline void SystemLoopBegin()
		{
#if( ! ( defined( VISUINO_ESP32_BLE ) || defined( VISUINO_BLE_NINA ) ) )
			if( C_OWNER.IsConnected() )
				if( T_BASE::GetRXCharacteristic()->written() )
					T_OutputPin::SendPinValue( Mitov::TDataBlock( FRXCharacteristic->valueLength(), FRXCharacteristic->value() ));
#endif
		}

#ifdef VISUINO_BLE_NINA
	public:
		ArduinoBluetoothLEUartService_Stream()
		{
			FInstance = this;
		}
#endif // VISUINO_BLE_NINA

	};
//---------------------------------------------------------------------------
#ifdef VISUINO_BLE_NINA
template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BASE,
		typename T_BufferSize,
		typename T_Enabled,
		typename T_OutputPin
	> void * ArduinoBluetoothLEUartService_Stream<
			T_OWNER, C_OWNER,
			T_BASE,
			T_BufferSize,
			T_Enabled,
			T_OutputPin
		>::FInstance;
#endif // VISUINO_BLE_NINA
//---------------------------------------------------------------------------
}

