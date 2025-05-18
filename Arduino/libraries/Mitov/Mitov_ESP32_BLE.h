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

#ifdef __TEST_FOR_DEBUG_PRINTS__
    #define Serial UNGUARDED DEBUG PRINT!!!
#endif

#define private public
	#include <BLECharacteristic.h>
#undef private

#include <BLEDevice.h>
#ifdef SEEEDUINO_WIO_TERMINAL
    #include <BLEServer.h>
#else
    #include <BLEUtils.h>
  
    #include <esp_bt.h>            // ESP32 BLE
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace TArduinoESP32BluetoothLESecurity
	{
		enum TArduinoESP32BluetoothLESecurity {	Default, Encrypt, Encrypt_No_MITM, Encrypt_MITM };
	}
//---------------------------------------------------------------------------
	class ESP32ServerImp_NoServer
	{
	public:
		BLEServer *GetServer() { return nullptr; }

	};
//---------------------------------------------------------------------------
	class ESP32ServerImp
	{
	public:
		BLEServer *FServer;

	public:
		BLEServer *GetServer() { return FServer; }

	};
//---------------------------------------------------------------------------
	template <
		typename T_ConnectedCountOutputPin,
		typename T_DeviceName,
		typename T_MACAddressOutputPin,
		typename T_MaximumTransmissionUnit,
#ifndef SEEEDUINO_WIO_TERMINAL
		typename T_Security,
#endif
		typename T_Server
#ifndef SEEEDUINO_WIO_TERMINAL
        ,
		typename T_TransmitPower
#endif
	> class ESP32BluetoothLE :
		public T_ConnectedCountOutputPin,
		public T_DeviceName,
		public T_MACAddressOutputPin,
		public T_MaximumTransmissionUnit,
#ifndef SEEEDUINO_WIO_TERMINAL
		public T_Security,
		public T_TransmitPower,
#endif
		public T_Server
	{
	public:
		_V_PIN_( ConnectedCountOutputPin )
		_V_PIN_( MACAddressOutputPin )
		
	public:
		_V_PROP_( DeviceName )
		_V_PROP_( MaximumTransmissionUnit )
#ifndef SEEEDUINO_WIO_TERMINAL
		_V_PROP_( TransmitPower )
		_V_PROP_( Security )
#endif

	public:
		inline void SystemInit()
		{
			BLEDevice::init( DeviceName().c_str() );
		}

		inline void SystemStart()
		{
			T_ConnectedCountOutputPin::SetPinValue( 0, false );

//			Serial.println( "TEST" );
//			BLEDevice::init( DeviceName.c_str() );

//			Serial.println( "TEST1" );
			BLEDevice::setMTU( MaximumTransmissionUnit() );
#ifndef SEEEDUINO_WIO_TERMINAL
			BLEDevice::setPower( esp_power_level_t( TransmitPower().GetValue() ));
			BLEDevice::setEncryptionLevel( esp_ble_sec_act_t( Security().GetValue() ));
#endif
			T_MACAddressOutputPin::SetPinValue( BLEDevice::getAddress().toString().c_str() );
		}

		inline void SystemLoopBeginConnectionCount()
		{
			T_ConnectedCountOutputPin::SetPinValue( T_Server::GetServer()->getConnectedCount(), true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif