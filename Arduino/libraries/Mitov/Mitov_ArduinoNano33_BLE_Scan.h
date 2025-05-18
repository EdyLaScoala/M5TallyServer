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

// #define __NANO33_BLE_SCAN__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_AppearanceOutputPin,
		typename T_DeviceNameOutputPin,
		typename T_Enabled,
		typename T_LocalNameOutputPin,
		typename T_MACAddressOutputPin,
		typename T_SignalStrengthOutputPin
//		typename T_WithDuplicates
	> class ArduinoNano33ModuleScanBLEOperation :
		public T_AppearanceOutputPin,
		public T_DeviceNameOutputPin,
		public T_Enabled,
		public T_LocalNameOutputPin,
		public T_MACAddressOutputPin,
		public T_SignalStrengthOutputPin
//		public T_WithDuplicates
	{
	public:
		_V_PIN_( AppearanceOutputPin )
		_V_PIN_( DeviceNameOutputPin )
		_V_PIN_( LocalNameOutputPin )
		_V_PIN_( MACAddressOutputPin )

	public:
		_V_PROP_( Enabled )
//		_V_PROP_( WithDuplicates )

	public:
/*
inline void UpdateEnabled()
		{
			if( Enabled() )
				BLE.scan( WithDuplicates().GetValue() );

			else
				BLE.stopScan();
		}

		inline void UpdateWithDuplicates()
		{
			if( Enabled() )
			{
				BLE.stopScan();
				BLE.scan( WithDuplicates().GetValue() );
			}
		}
*/
/*
	protected:
		static void DoCentralDiscoverHandler( BLEDevice peripheral )
		{
			Serial.print("Address: ");
			Serial.println(peripheral.address());
		}
*/
	public:
		inline void NeedsScan( bool &AValue )
		{
			AValue |= Enabled().GetValue();
		}

		inline void DeviceFound( BLEDevice & ADevice )
		{
			T_MACAddressOutputPin::SetPinValue( ADevice.address() );
			T_DeviceNameOutputPin::SetPinValue( ADevice.deviceName() );
			T_LocalNameOutputPin::SetPinValue( ADevice.localName() );
			T_AppearanceOutputPin::SetPinValue( int32_t( ADevice.appearance() ));
			T_SignalStrengthOutputPin::SetPinValue( int32_t( ADevice.rssi() ));
//			Serial.print("Address: ");
//			Serial.println( ADevice.address() );
		}

	public:
/*
		inline void SystemInit()
		{
			Serial.println( "Scan.SystemInit" );
//			BLE.setEventHandler( BLEDiscovered, DoCentralDiscoverHandler );
		}

		inline void SystemStart()
		{
//			Serial.println( "Scan.SystemStart" );
//			UpdateEnabled();
		}
*/
/*
		inline void SystemLoopBegin()
		{
			if( Enabled() )
			{
				BLEDevice APeripheral = BLE.available();
				if( APeripheral )
				{
					T_MACAddressOutputPin::SetPinValue( APeripheral.address() );
					T_DeviceNameOutputPin::SetPinValue( APeripheral.deviceName() );
					T_LocalNameOutputPin::SetPinValue( APeripheral.localName() );
					T_AppearanceOutputPin::SetPinValue( int32_t( APeripheral.appearance() ));
					T_SignalStrengthOutputPin::SetPinValue( int32_t( APeripheral.rssi() ));
//					Serial.print("Address: ");
//					Serial.println( APeripheral.address() );
				}

//				BLE.poll();
			}
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

