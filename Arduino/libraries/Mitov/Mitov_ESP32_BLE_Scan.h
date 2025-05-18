////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <BLEDevice.h>
#ifndef SEEEDUINO_WIO_TERMINAL
    #include <BLEUtils.h>
#endif
#include <BLEScan.h>

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __ESP32_BLE_SCAN__DEBUG__


namespace Mitov
{
//---------------------------------------------------------------------------
	namespace ESP32ModuleScanBLE
	{
		bool InScanning = false;
	}
//---------------------------------------------------------------------------
	struct TESP32BLEDeviceInfo
	{
		String		Name;
		BLEAddress	Address;
		BLEUUID		ServiceUUID;
		String		ManufacturerData;
		int			RSSI;
		int8_t		TXPower;
#ifndef SEEEDUINO_WIO_TERMINAL
		String		ServiceData;
		Mitov::TValueArray<uint8_t> Payload;
#endif

	public:
#ifdef SEEEDUINO_WIO_TERMINAL
		inline TESP32BLEDeviceInfo( BLEAddress	AAddress ) :
#else
		inline TESP32BLEDeviceInfo( BLEAddress	AAddress, size_t APayloadSize, uint8_t *APayload ) :
			Payload( APayloadSize, APayload ),
#endif
			Address( AAddress )
		{
		}
	};

//---------------------------------------------------------------------------
//	typedef Mitov::SimpleObjectDeleteList<TRequestElement *>	TESP32BLEDeviceInfos;
//---------------------------------------------------------------------------
	template <
		typename T_Active,
		typename T_ActiveScanWindow,
		typename T_ClearStored,
		typename T_CountDevicesOutputPin,
		typename T_DeviceNameOutputPin,
		typename T_Duration,
		typename T_FScanRequest,
		typename T_FoundDeviceOutputPin,
		typename T_Interval,
		typename T_MACAddressOutputPin,
		typename T_ManufacturerDataOutputPin,
#ifndef SEEEDUINO_WIO_TERMINAL
		typename T_PayloadOutputPin,
#endif
		typename T_ScanningOutputPin,
#ifndef SEEEDUINO_WIO_TERMINAL
		typename T_ServiceDataOutputPin,
#endif
		typename T_ServiceUUIDOutputPin,
		typename T_SignalStrengthOutputPin,
		typename T_TransmitPowerOutputPin
	> class ESP32ModuleScanBLEOperation :
	    public BLEAdvertisedDeviceCallbacks,
		public T_Active,
		public T_ActiveScanWindow,
		public T_ClearStored,
		public T_CountDevicesOutputPin,
		public T_DeviceNameOutputPin,
		public T_Duration,
		public T_FScanRequest,
		public T_FoundDeviceOutputPin,
		public T_Interval,
		public T_MACAddressOutputPin,
		public T_ManufacturerDataOutputPin,
#ifndef SEEEDUINO_WIO_TERMINAL
		public T_PayloadOutputPin,
		public T_ServiceDataOutputPin,
#endif
		public T_ScanningOutputPin,
		public T_ServiceUUIDOutputPin,
		public T_SignalStrengthOutputPin,
		public T_TransmitPowerOutputPin
	{
	public:
		_V_PIN_( ScanningOutputPin )
		_V_PIN_( CountDevicesOutputPin )
		_V_PIN_( DeviceNameOutputPin )
		_V_PIN_( MACAddressOutputPin )
#ifndef SEEEDUINO_WIO_TERMINAL
		_V_PIN_( PayloadOutputPin )
		_V_PIN_( ServiceDataOutputPin )
#endif
		_V_PIN_( ServiceUUIDOutputPin )
		_V_PIN_( ManufacturerDataOutputPin )
		_V_PIN_( SignalStrengthOutputPin )
		_V_PIN_( TransmitPowerOutputPin )
		_V_PIN_( FoundDeviceOutputPin )

	public:
		_V_PROP_( Duration )
		_V_PROP_( ActiveScanWindow )
		_V_PROP_( Active )
		_V_PROP_( ClearStored )
		_V_PROP_( Interval );

	protected:
		_V_PROP_( FScanRequest )

//		static	void *FInstance;

		static	bool FFinished;

		Mitov::SimpleObjectDeleteList<TESP32BLEDeviceInfo *>	FESP32BLEDeviceInfos;

	protected:
		static void SharedScanCompleteCB( BLEScanResults foundDevices )
		{
//			return;
#ifdef __NEXTION__DEBUG__
			Serial.println( "SharedScanCompleteCB" );
#endif // __NEXTION__DEBUG__
//			((ESP32ModuleScanBLEOperation *)FInstance )->ScanCompleteCB( foundDevices );
//			ESP32ModuleScanBLE::InScanning = false;
			FFinished = true;
//			((ESP32ModuleScanBLEOperation *)FInstance )->FFinished = true;
			return;

			int ACount = foundDevices.getCount();
#ifdef __NEXTION__DEBUG__
			Serial.println( ACount );
#endif // __NEXTION__DEBUG__

//			BLEDevice::getScan()->clearResults();
//*/
//			((ESP32ModuleScanBLEOperation *)FInstance )->CountDevicesOutputPin().SetPinValue( ACount );
//			((ESP32ModuleScanBLEOperation *)FInstance )->ScanningOutputPin().SetPinValue( false, true );
		}

	    void onResult( BLEAdvertisedDevice ADevice ) 
		{
//			return;
//			Serial.printf("Advertised Device: %s \n", ADevice.toString().c_str());
			for( int i = 0; i < FESP32BLEDeviceInfos.size(); ++ i )
			{
				TESP32BLEDeviceInfo *AInfo = FESP32BLEDeviceInfos[ i ];
				if( AInfo->Address.equals( ADevice.getAddress() ) )
				{
#ifdef SEEEDUINO_WIO_TERMINAL
                    if(	ADevice.m_haveName )
#else
					if(	ADevice.haveName() )
#endif
						AInfo->Name = ADevice.getName().c_str();

#ifndef SEEEDUINO_WIO_TERMINAL
					if(	ADevice.haveServiceData() )
						AInfo->ServiceData = ADevice.getServiceData().c_str();
#endif

					if(	ADevice.haveManufacturerData() )
#ifdef SEEEDUINO_WIO_TERMINAL
						AInfo->ManufacturerData = (char *)ADevice.getManufacturerData();
#else
						AInfo->ManufacturerData = ADevice.getManufacturerData().c_str();
#endif

					if(	ADevice.haveServiceUUID() )
						AInfo->ServiceUUID = ADevice.getServiceUUID();

					if( ADevice.haveRSSI() )
						AInfo->RSSI = ADevice.getRSSI();
					
					if( ADevice.haveTXPower() )
						AInfo->RSSI = ADevice.getTXPower();

#ifndef SEEEDUINO_WIO_TERMINAL
					if( ADevice.getPayloadLength() > AInfo->Payload.Size )
						AInfo->Payload.Assign( ADevice.getPayload(), ADevice.getPayloadLength() );
//						AInfo->Payload = Mitov::TValueArray<uint8_t>( ADevice.getPayloadLength(), ADevice.getPayload() );
#endif
					
					return;
				}
			}

#ifdef SEEEDUINO_WIO_TERMINAL
			TESP32BLEDeviceInfo *AInfo = new TESP32BLEDeviceInfo( ADevice.getAddress() );
#else
			TESP32BLEDeviceInfo *AInfo = new TESP32BLEDeviceInfo( ADevice.getAddress(), ADevice.getPayloadLength(), ADevice.getPayload() );
#endif
			AInfo->Name = ADevice.getName().c_str();
#ifndef SEEEDUINO_WIO_TERMINAL
			AInfo->ServiceData = ADevice.getServiceData().c_str();
#endif
			AInfo->ServiceUUID = ADevice.getServiceUUID();
#ifdef SEEEDUINO_WIO_TERMINAL
			AInfo->ManufacturerData = (char *)ADevice.getManufacturerData();
#else
			AInfo->ManufacturerData = ADevice.getManufacturerData().c_str();
#endif
			AInfo->RSSI = ADevice.getRSSI();
			AInfo->TXPower = ADevice.getTXPower();
			
			FESP32BLEDeviceInfos.push_back( AInfo );

#ifdef __NEXTION__DEBUG__
			Serial.println( "---ADD DEVICE---" );
#endif // __NEXTION__DEBUG__

		}

		void ScanCompleteCB( BLEScanResults &foundDevices )
		{
//			return;
#ifdef __ESP32_BLE_SCAN__DEBUG__
			Serial.println( "ScanCompleteCB" );
#endif // __ESP32_BLE_SCAN__DEBUG__
			int ACount = foundDevices.getCount();
///*
#ifdef __ESP32_BLE_SCAN__DEBUG__
			Serial.println( ACount );
#endif // __ESP32_BLE_SCAN__DEBUG__
//			BLEDevice::getScan()->clearResults();
			T_CountDevicesOutputPin::SetPinValue( ACount );
			T_ScanningOutputPin::SetPinValue( false, true );
//			return;
			for( int i = 0; i < ACount; ++i )
			{
				BLEAdvertisedDevice ADevice = foundDevices.getDevice( i );
//				foundDevices.getDevice( i );
#ifdef __ESP32_BLE_SCAN__DEBUG__
				Serial.printf("Advertised Device: %s \n", ADevice.toString().c_str());
#endif // __ESP32_BLE_SCAN__DEBUG__
				continue;
				break;

//				BLEAdvertisedDevice ADevice = foundDevices.getDevice( i );
				T_DeviceNameOutputPin::SetPinValue( ADevice.getName().c_str() );
				T_MACAddressOutputPin::SetPinValue( ADevice.getAddress().toString().c_str() );
#ifndef SEEEDUINO_WIO_TERMINAL
				T_ServiceDataOutputPin::SetPinValue( ADevice.getServiceData().c_str() );
#endif
				T_ServiceUUIDOutputPin::SetPinValue( ADevice.getServiceUUID().toString().c_str() );
				T_ManufacturerDataOutputPin::SetPinValue( ADevice.getManufacturerData() );
				T_SignalStrengthOutputPin::SetPinValue( ADevice.getRSSI() );
				T_TransmitPowerOutputPin::SetPinValue( ADevice.getTXPower() );
#ifndef SEEEDUINO_WIO_TERMINAL
				T_PayloadOutputPin::SendPinValue( Mitov::TDataBlock( ADevice.getPayloadLength(), ADevice.getPayload() ));
#endif
				T_FoundDeviceOutputPin::ClockPin();
			}

			BLEDevice::getScan()->clearResults();
//*/
			T_CountDevicesOutputPin::SetPinValue( ACount );
			T_ScanningOutputPin::SetPinValue( false, true );
		}

	public:
		inline void SystemStart()
		{
			SystemStartNoScan();
//			BLEDevice::getScan()->setAdvertisedDeviceCallbacks( this );
//			T_ScanningOutputPin::SetPinValue( false, false );
			ScanInputPin_o_Receive( nullptr );
		}

		inline void SystemStartNoScan()
		{
#ifdef SEEEDUINO_WIO_TERMINAL
			BLEDevice::getScan()->start( 1, false ); // Force the BLE to start due to framework bug!
#endif
			BLEDevice::getScan()->setAdvertisedDeviceCallbacks( this, true );
			T_ScanningOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( FFinished )
			{
				int ACount = FESP32BLEDeviceInfos.size();
#ifdef __NEXTION__DEBUG__
				Serial.println( ACount );
#endif // __NEXTION__DEBUG__
				for( int i = 0; i < ACount; ++ i )
				{
					TESP32BLEDeviceInfo *AInfo = FESP32BLEDeviceInfos[ i ];
#ifdef __NEXTION__DEBUG__
					Serial.println( AInfo->Name );
#endif // __NEXTION__DEBUG__
					T_DeviceNameOutputPin::SetPinValue( AInfo->Name );
					T_MACAddressOutputPin::SetPinValue( AInfo->Address.toString().c_str() );
#ifndef SEEEDUINO_WIO_TERMINAL
					T_ServiceDataOutputPin::SetPinValue( AInfo->ServiceData );
#endif
					T_ServiceUUIDOutputPin::SetPinValue( AInfo->ServiceUUID.toString().c_str() );
					T_ManufacturerDataOutputPin::SetPinValue( AInfo->ManufacturerData );
					T_SignalStrengthOutputPin::SetPinValue( AInfo->RSSI );
					T_TransmitPowerOutputPin::SetPinValue( AInfo->TXPower );
#ifndef SEEEDUINO_WIO_TERMINAL
					T_PayloadOutputPin::SendPinValue( Mitov::TDataBlock( AInfo->Payload.Size, AInfo->Payload.Data ));
#endif
					T_FoundDeviceOutputPin::ClockPin();
				}

				T_CountDevicesOutputPin::SetPinValue( ACount );
				T_ScanningOutputPin::SetPinValue( false, true );

				FESP32BLEDeviceInfos.clear();
//				BLEDevice::getScan()->stop();

/*
				BLEScanResults foundDevices = BLEDevice::getScan()->getResults();
				int ACount = foundDevices.getCount();
///*
				Serial.println( ACount );
//			BLEDevice::getScan()->clearResults();
				T_CountDevicesOutputPin::SetPinValue( ACount );
				T_ScanningOutputPin::SetPinValue( false, true );
				for( int i = 0; i < ACount; ++i )
				{
					BLEAdvertisedDevice ADevice = foundDevices.getDevice( i );
	//				foundDevices.getDevice( i );
					Serial.printf("Advertised Device: %s \n", ADevice.toString().c_str());
				}
*/
				FFinished = false;
				ESP32ModuleScanBLE::InScanning = false;
			}

			if( FScanRequest() )
				ScanInputPin_o_Receive( nullptr );
		}

	public:
//		inline // inline causes problems due to compiler bugs
		void ScanInputPin_o_Receive( void *_Data )
		{
			if( ESP32ModuleScanBLE::InScanning )
			{
				FScanRequest() = true;
				return;
			}

#ifdef __NEXTION__DEBUG__
			Serial.println( "START" );
#endif // __NEXTION__DEBUG__

			FScanRequest() = false;
			ESP32ModuleScanBLE::InScanning = true;
			T_ScanningOutputPin::SetPinValue( true, true );

//			FInstance = this;
			BLEScan* AScan = BLEDevice::getScan();
			AScan->clearResults();
			AScan->setActiveScan( Active() );


//			AScan->setActiveScan(true); //active scan uses more power, but get results faster
//			AScan->setInterval( Duration );
			AScan->setWindow( MitovMin<uint32_t>( Duration() * 1000, ActiveScanWindow() ));
			AScan->setInterval( Interval() );
			AScan->setWindow( MitovMin<uint32_t>( ActiveScanWindow().GetValue(), Interval().GetValue() ));  // less or equal setInterval value

//  AScan->setInterval(100);
//  AScan->setWindow(99);  // less or equal setInterval value

			AScan->start( Duration(), SharedScanCompleteCB, false );
//			AScan->start( 8, SharedScanCompleteCB, false );
//			AScan->start( 3, nullptr, false );
//			AScan->start( 3, false );
//			AScan->start( 3, SharedScanCompleteCB, false );
//			ESP32ModuleScanBLE::InScanning = false;




//			AScan->start( Duration(), SharedScanCompleteCB, false );
//			AScan->start( 10, SharedScanCompleteCB, false );
		}
		
	};
//---------------------------------------------------------------------------
/*
template <
		typename T_Active,
		typename T_ActiveScanWindow,
		typename T_ClearStored,
		typename T_CountDevicesOutputPin,
		typename T_DeviceNameOutputPin,
		typename T_Duration,
		typename T_FScanRequest,
		typename T_FoundDeviceOutputPin,
		typename T_Interval,
		typename T_MACAddressOutputPin,
		typename T_ManufacturerDataOutputPin,
		typename T_PayloadOutputPin,
		typename T_ScanningOutputPin,
		typename T_ServiceDataOutputPin,
		typename T_ServiceUUIDOutputPin,
		typename T_SignalStrengthOutputPin,
		typename T_TransmitPowerOutputPin
	> void * ESP32ModuleScanBLEOperation<
		T_Active,
		T_ActiveScanWindow,
		T_ClearStored,
		T_CountDevicesOutputPin,
		T_DeviceNameOutputPin,
		T_Duration,
		T_FScanRequest,
		T_FoundDeviceOutputPin,
		T_Interval,
		T_MACAddressOutputPin,
		T_ManufacturerDataOutputPin,
		T_PayloadOutputPin,
		T_ScanningOutputPin,
		T_ServiceDataOutputPin,
		T_ServiceUUIDOutputPin,
		T_SignalStrengthOutputPin,
		T_TransmitPowerOutputPin
	>::FInstance;
*/
template <
		typename T_Active,
		typename T_ActiveScanWindow,
		typename T_ClearStored,
		typename T_CountDevicesOutputPin,
		typename T_DeviceNameOutputPin,
		typename T_Duration,
		typename T_FScanRequest,
		typename T_FoundDeviceOutputPin,
		typename T_Interval,
		typename T_MACAddressOutputPin,
		typename T_ManufacturerDataOutputPin,
#ifndef SEEEDUINO_WIO_TERMINAL
		typename T_PayloadOutputPin,
#endif
		typename T_ScanningOutputPin,
#ifndef SEEEDUINO_WIO_TERMINAL
		typename T_ServiceDataOutputPin,
#endif
		typename T_ServiceUUIDOutputPin,
		typename T_SignalStrengthOutputPin,
		typename T_TransmitPowerOutputPin
	> bool ESP32ModuleScanBLEOperation<
		T_Active,
		T_ActiveScanWindow,
		T_ClearStored,
		T_CountDevicesOutputPin,
		T_DeviceNameOutputPin,
		T_Duration,
		T_FScanRequest,
		T_FoundDeviceOutputPin,
		T_Interval,
		T_MACAddressOutputPin,
		T_ManufacturerDataOutputPin,
#ifndef SEEEDUINO_WIO_TERMINAL
		T_PayloadOutputPin,
#endif
		T_ScanningOutputPin,
#ifndef SEEEDUINO_WIO_TERMINAL
		T_ServiceDataOutputPin,
#endif
		T_ServiceUUIDOutputPin,
		T_SignalStrengthOutputPin,
		T_TransmitPowerOutputPin
	>::FFinished = false;
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif