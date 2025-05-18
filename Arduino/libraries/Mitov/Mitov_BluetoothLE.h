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

#ifdef VISUINO_ANY_ESP32
	#define private public
		#include <BLECharacteristic.h>
	#undef private

	#include <BLEDevice.h>
	#include <BLEUtils.h>
#else // VISUINO_ANY_ESP32
	#ifdef VISUINO_ARDUINO_101
		#include <CurieBLE.h>
	#else // VISUINO_ARDUINO_101
		#ifdef VISUINO_BLE_NINA
			#include <ArduinoBLE.h>
		#else
			#include <BLEPeripheral.h>
		#endif
	#endif // VISUINO_ARDUINO_101
#endif // VISUINO_ANY_ESP32

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
#ifdef VISUINO_BLE_NINA
	typedef BLEDevice MitovBLECentral;
#else
	typedef BLECentral MitovBLECentral;
#endif
//---------------------------------------------------------------------------
	template <
		typename T_ConnectedOutputPin,
		typename T_DeviceName,
		typename T_Enabled,
		typename T_FConnected,
		typename T_LocalName,
		typename T_MACAddressOutputPin
	>	class ArduinoBluetoothLE :
		public T_ConnectedOutputPin,
		public T_DeviceName,
		public T_Enabled,
		public T_FConnected,
		public T_LocalName,
		public T_MACAddressOutputPin
	{
	public:
		_V_PIN_( ConnectedOutputPin )
		_V_PIN_( MACAddressOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( LocalName )
		_V_PROP_( DeviceName )

	protected:
#ifndef VISUINO_BLE_NINA
		BLEPeripheral	FPeripheral;
#endif

	public:
#ifdef VISUINO_BLE_NINA
		inline BLELocalDevice	&GetPeripheral() { return BLE; }
#else
		inline BLEPeripheral	&GetPeripheral() { return FPeripheral; }
#endif

	protected:
		_V_PROP_( FConnected ) // = false;

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
//				Serial.println( "GetPeripheral().begin" );
				GetPeripheral().begin();
				if( LocalName().GetValue() != "" )
					GetPeripheral().setLocalName( LocalName().c_str() );

				if( DeviceName().GetValue() != "" )
					GetPeripheral().setDeviceName( DeviceName().c_str() );

#ifdef VISUINO_BLE_NINA
				BLE.advertise();
#endif
			}

			else
				GetPeripheral().end();
		}

	public:
		inline bool IsConnected()
		{
			return FConnected();
		}

	public:
		void AddveriseService( BLEService *AService )
		{
			GetPeripheral().setAdvertisedServiceUuid( AService->uuid());
		}

		BLEService *AddStandardService( uint16_t ASubUUID )
		{
			Mitov::String AText( ASubUUID, HEX );
			while( AText.length() < 4 )
				AText = '0' + AText;

			BLEService *AService = new BLEService( AText.c_str() ); // Add -0000-1000-8000-00805F9B34FB for standard
#ifdef VISUINO_BLE_NINA
			GetPeripheral().addService( *AService );
#else // VISUINO_BLE_NINA
			GetPeripheral().addAttribute( *AService );
#endif // VISUINO_BLE_NINA
			return AService;
		}

		template <typename T_CHARACTERISTIC> T_CHARACTERISTIC *AddStandardCharacteristic( BLEService *AService, uint16_t ASubUUID, uint8_t AProperties )
		{
			Mitov::String AText( ASubUUID, HEX );
			while( AText.length() < 4 )
				AText = '0' + AText;

			T_CHARACTERISTIC *ACharacteristic = new BLEUnsignedCharCharacteristic( AText.c_str(), AProperties );
#ifdef VISUINO_BLE_NINA
			AService->addCharacteristic( *ACharacteristic );
#else // VISUINO_BLE_NINA
			GetPeripheral().addAttribute( *ACharacteristic );
#endif // VISUINO_BLE_NINA
			return ACharacteristic;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			MitovBLECentral ACentral = GetPeripheral().central();
			if( FConnected() )
			{
				if( ! ACentral )
				{
					FConnected() = false;
					T_ConnectedOutputPin::SetPinValue( FConnected(), false );
				}
			}
			else
			{
				if( ACentral )
				{
					FConnected() = true;
					T_ConnectedOutputPin::SetPinValue( FConnected(), false );
					Mitov::String AAddress = ACentral.address();
					T_MACAddressOutputPin::SetPinValue( AAddress, false );
				}
			}

/*
			if( FConnected )
			{
				for( int i = 0; i < FElements.size(); ++ i )
					FElements[ i ]->UpdateValues();
			}
*/
		}

		inline void SystemInit()
		{
/*
			if( LocalName().GetValue() != "" )
				GetPeripheral().setLocalName( LocalName().c_str() );

			if( DeviceName().GetValue() != "" )
				GetPeripheral().setDeviceName( DeviceName().c_str() );
*/
		}

		inline void SystemStart()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
			UpdateEnabled();

//			GetPeripheral().begin();
		}

	public:
		inline ArduinoBluetoothLE()
		{
			FConnected() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif