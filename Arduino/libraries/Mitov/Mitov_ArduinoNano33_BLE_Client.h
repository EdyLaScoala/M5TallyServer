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
#include <Mitov_ArduinoNano33_BLE_Scan.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_MACAddress,
		typename T_Name,
		typename T_ServiceUUID
	> class ArduinoNano33BluetoothLERemoteDeviceID :
		public T_MACAddress,
		public T_Name,
		public T_ServiceUUID
	{
	public:
		_V_PROP_( Name )
		_V_PROP_( ServiceUUID )
		_V_PROP_( MACAddress )

	public:
		inline void DeviceFound( BLEDevice & ADevice )
		{
//			if( C_OWNER.IsConnected() || C_OWNER.IsConnecting() )
			if( C_OWNER.IsConnected() )
			{
				Serial.println( "IsConnected" );
				return;
			}

			if( ( Name().GetValue() == "" ) || ( Name().GetValue() == ADevice.deviceName().c_str() ) || ( Name().GetValue() == ADevice.localName().c_str() ))
				if( ServiceUUID().GetValue() == "" || ServiceUUID().GetValue() == ADevice.advertisedServiceUuid().c_str() )
				{
					String AMacAddress = MACAddress().AsText();
					AMacAddress.toLowerCase();

					if( MACAddress().IsZero() || AMacAddress == ADevice.address() )
					{
						C_OWNER.Connect( ADevice );
						return;
					}
				}


		}

	};
//---------------------------------------------------------------------------
	template <
		uint32_t C_COUNT_DeviceIDs,
		typename T_DeviceIDs_DeviceFound,
		typename T_DeviceNameOutputPin,
		typename T_MACAddressOutputPin,
		typename T_Serial_TryStart
	> class ArduinoNano33BluetoothLERemoteDevice :
		public T_DeviceNameOutputPin,
		public T_MACAddressOutputPin
	{
	public:
		_V_PIN_( DeviceNameOutputPin )
		_V_PIN_( MACAddressOutputPin )

	protected:
		BLEDevice *FConnectDevice = nullptr;

	public:
/*
		inline bool IsConnecting()
		{
			return ( FConnectDevice != nullptr );
		}
*/
		inline bool IsConnected()
		{
			if( ! FConnectDevice )
				return false;

			return FConnectDevice->connected();
		}

		void Connect( BLEDevice &ADevice )
		{			
			if( FConnectDevice )
				delete FConnectDevice;

			Serial.println( "CONNECT" );
			FConnectDevice = new BLEDevice( ADevice );
			if( FConnectDevice->connect())
			{
				Serial.println( "CONNECTED..." );
				ADevice.discoverAttributes();
//				Serial.println( C_OWNER.GetClient()->discoverService
				Serial.println( "TryStart" );
				Serial.println( ADevice.serviceCount() );
				Serial.println( ADevice.service( 0 ).uuid() );

				delay( 1000 );
				Serial.println( "-------------" );


				bool AResult = true;
				T_Serial_TryStart::Call( true, AResult );

				if( ! AResult )
					FConnectDevice->disconnect();
			}
		}

	public:
		inline BLEDevice *GetClient()
		{
			return FConnectDevice;
		}

	public:
		inline void DeviceFound( BLEDevice & ADevice )
		{
			T_DeviceIDs_DeviceFound::Call( ADevice );
		}

		inline void NeedsScan( bool &AValue )
		{
			AValue = (( C_COUNT_DeviceIDs > 0 ) && ( ! FConnectDevice ));
		}

	public:
		inline void SystemInit()
		{
		}

		inline void SystemLoopBegin()
		{
			if( FConnectDevice )
				if( ! FConnectDevice->connected() )
				{
					Serial.println( "DISCONNECT" );
					delete FConnectDevice;
					FConnectDevice = nullptr;
				}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
