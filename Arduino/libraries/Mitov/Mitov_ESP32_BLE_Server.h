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
#include <BLEDevice.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled
	> class ESP32BluetoothLEServer :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		BLEServer *GetServer() { return C_OWNER.FServer; }

	public:
		void AddveriseService( BLEService *AService )
		{
			BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
			pAdvertising->addServiceUUID( AService->getUUID() );
		}

		BLEService *AddStandardService( uint16_t ASubUUID )
		{
			return C_OWNER.FServer->createService( BLEUUID(ASubUUID) );
		}

		template <typename T_CHARACTERISTIC> T_CHARACTERISTIC *AddStandardCharacteristic( BLEService *AService, uint16_t ASubUUID, uint8_t AProperties )
		{
			T_CHARACTERISTIC *ACharacteristic = new T_CHARACTERISTIC( BLEUUID(ASubUUID), AProperties );
			AService->addCharacteristic( ACharacteristic );
			return ACharacteristic;
		}

	public:
		inline void SystemInit()
		{
			C_OWNER.FServer = BLEDevice::createServer();
		}

		inline void SystemStart()
		{
//			T_CONNECTED_COUNT_PIN::SetPinValue( 0, false );

			BLEAdvertising *AAdvertising = C_OWNER.FServer->getAdvertising();
			AAdvertising->start();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

