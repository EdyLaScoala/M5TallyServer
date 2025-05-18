////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <ArduinoBLE.h>

#include <Mitov.h>

#pragma once

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __NANO33_BLE__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_DeviceName,
		typename T_FScanning,
		typename T_LocalName,
		typename T_MACAddressOutputPin,
		typename T_Operations_DeviceFound,
		typename T_Operations_NeedsScan,
		typename T_RemoteDevices_DeviceFound,
		typename T_RemoteDevices_NeedsScan
//		typename T_SignalStrengthOutputPin
	> class ArduinoNano33BluetoothLE :
		public T_DeviceName,
		public T_FScanning,
		public T_LocalName,
		public T_MACAddressOutputPin
//		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( MACAddressOutputPin )
//		_V_PIN_( SignalStrengthOutputPin )

	public:
		_V_PROP_( LocalName )
		_V_PROP_( DeviceName )

	protected:
		_V_PROP_( FScanning ) // = false;

	protected:
		unsigned long	FLastTime;

/*
	protected:
		static void DoCentralDiscoverHandler( BLEDevice peripheral )
		{
		}
*/
	public:
		inline void SystemInit()
		{
//			Serial.println( "BLE.begin" );
			BLE.begin();

//			BLE.debug( Serial );
		}

		inline void SystemStart()
		{
			if( LocalName().GetValue() != "" )
			{
//				Serial.println( "BLE.setLocalName" );
				BLE.setLocalName( LocalName().c_str() );
			}

			if( DeviceName().GetValue() != "" )
			{
//				Serial.println( "BLE.setDeviceName" );
				BLE.setDeviceName( DeviceName().c_str() );
			}

			BLE.advertise();

			if( T_MACAddressOutputPin::GetPinIsConnected() )
			{
				T_MACAddressOutputPin::SetPinValue( BLE.address() );
			}

		}

		inline void SystemLoopBegin()
		{
			BLE.poll();
//			BLE.setEventHandler( BLEDisconnected, DoCentralDisconnectedHandler );

			if( FScanning() )
			{
				unsigned long ATime = millis();
				if( ATime > FLastTime + 10000 )
				{
					FLastTime = ATime;
//					Serial.println( "FORCED STOP SCAN" );
					BLE.stopScan();
//					BLE.end();
//					BLE.begin();
					FScanning() = false;
				}
			}

			bool AValue = false; //FScanning;
			if( ! AValue )
			{
				T_Operations_NeedsScan::Call( AValue );
				if( ! AValue )
					T_RemoteDevices_NeedsScan::Call( AValue );

			}

			if( AValue )
			{
				if( ! FScanning().GetValue() )
				{
//					Serial.println( "SCAN" );
					BLE.scan( true );
					FScanning() = true;
				}

				BLEDevice APeripheral = BLE.available();
				if( APeripheral )
				{
//					Serial.println( APeripheral.localName() );
					T_Operations_DeviceFound::Call( APeripheral );
					T_RemoteDevices_DeviceFound::Call( APeripheral );
//					Serial.println( "FINISHED" );
				}

			}

			else if( FScanning() )
			{
//				Serial.println( "STOP SCAN" );
				BLE.stopScan();
				FScanning() = false;
			}

//			if( T_SignalStrengthOutputPin::GetPinIsConnected() )
//				T_SignalStrengthOutputPin::SetPinValue( BLE.rssi() );

//			BLE.rssi();
		}

	public:
		ArduinoNano33BluetoothLE()
		{
			FScanning() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

