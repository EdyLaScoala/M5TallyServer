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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ContactInputPin_o_IsConnected,
		typename T_EnergyExpandedInputPin_o_IsConnected,
		typename T_FContactValue,
		typename T_FModified,
		typename T_InitialEnergyExpanded,
		typename T_InitialValue,
		typename T_Location,
		typename T_ResetEnergyExpendedOutputPin,
		typename T_TwoBytes
	> class ArduinoBluetoothLEHearthRateService :
#ifdef _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_
	#ifdef VISUINO_ANY_ESP32
		public BLECharacteristicCallbacks,
	#endif //  VISUINO_ANY_ESP32
#endif // _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_
		public T_ContactInputPin_o_IsConnected,
		public T_EnergyExpandedInputPin_o_IsConnected,
		public T_FContactValue,
		public T_FModified,
		public T_InitialEnergyExpanded,
		public T_InitialValue,
		public T_Location,
		public T_ResetEnergyExpendedOutputPin,
		public T_TwoBytes
	{
	public:
		_V_PIN_( ResetEnergyExpendedOutputPin )

	public:
		_V_PROP_( InitialValue )
//#ifdef _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_
		_V_PROP_( InitialEnergyExpanded )
//#endif

		_V_PROP_( Location )
		_V_PROP_( TwoBytes )

	protected:
		_V_PROP_( FContactValue )
		_V_PROP_( FModified )

	protected:
		BLEService						*FService;
		BLECharacteristic				*FCharacteristic;
		BLEUnsignedCharCharacteristic	*FLocationCharacteristic;
#ifdef _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_
		BLEUnsignedCharCharacteristic	*FControlPointCharacteristic;
#endif

#ifdef _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_
	#ifdef VISUINO_ANY_ESP32
	public:
		virtual void onRead(BLECharacteristic* pCharacteristic) override
		{
		}

		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			if( pCharacteristic != FControlPointCharacteristic )
				return;

			uint32_t ALength = FControlPointCharacteristic->m_value.getLength();
			if( ALength != 1 )
				return;

			if( FControlPointCharacteristic->valueLE() != 1 )
				return;

			T_ResetEnergyExpendedOutputPin::ClockPin();
//			T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( FCharacteristic->m_value.getLength(), FCharacteristic->getData() ));
		}
	#endif //  VISUINO_ANY_ESP32
#endif // _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue < 0 )
				AValue = 0;

			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			SendData();
		}

		inline void ContactInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( FContactValue() == AValue )
				return;

			FContactValue() = AValue;
			SendData();
		}

//#ifdef _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_
		inline void EnergyExpandedInputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( InitialEnergyExpanded() == AValue )
				return;

			InitialEnergyExpanded() = AValue;
			SendData();
		}
//#endif // _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_

	protected:
		void SendData()
		{
//			const uint8_t heartRateCharArray[2] = { 0, (uint8_t)( InitialValue + 0.5 ) };
//			FCharacteristic->setValue( heartRateCharArray, 2 );
			uint8_t AFlags = 0;
//#ifdef _MITOV_BLE_HEARTHRATE_CONTACT_PIN_
			if( FContactValue() )
				AFlags |= 0b110; // Contact

			else
				AFlags |= 0b100; // No Contact
//#endif // _MITOV_BLE_HEARTHRATE_CONTACT_PIN_

//#ifdef _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_
			if( T_EnergyExpandedInputPin_o_IsConnected::GetValue() )
				AFlags |= 0b1000;

//#endif // _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_

			if( TwoBytes() )
			{
				uint16_t AValue16 =  InitialValue() + 0.5;

//#ifdef _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_
				if( T_EnergyExpandedInputPin_o_IsConnected::GetValue() )
				{
					uint16_t AEnergy16 =  InitialEnergyExpanded() + 0.5;

					uint8_t heartRateCharArray[5] = { uint8_t( AFlags | 1 ), uint8_t( AValue16 ), uint8_t( AValue16 >> 8 ), uint8_t( AEnergy16 ), uint8_t( AEnergy16 >> 8 ) };
					FCharacteristic->setValue( heartRateCharArray, 5 );
				}
//#else
				else
				{
					uint8_t heartRateCharArray[3] = { uint8_t( AFlags | 1 ), uint8_t( AValue16 ), uint8_t( AValue16 >> 8 ) };
					FCharacteristic->setValue( heartRateCharArray, 3 );
				}
//#endif
			}
			else
			{
//#ifdef _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_
				if( T_EnergyExpandedInputPin_o_IsConnected::GetValue() )
				{
					uint16_t AEnergy16 =  InitialEnergyExpanded() + 0.5;

					uint8_t heartRateCharArray[4] = { AFlags, uint8_t( InitialValue() + 0.5 ), uint8_t( AEnergy16 ), uint8_t( AEnergy16 >> 8 ) };
					FCharacteristic->setValue( heartRateCharArray, 4 );
				}
//#else
				else
				{
					uint8_t heartRateCharArray[2] = { AFlags, uint8_t( InitialValue() + 0.5 ) };
					FCharacteristic->setValue( heartRateCharArray, 2 );
				}
//#endif
			}
		}

	public:
		inline void SystemLoopBegin()
		{
#ifdef _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_
	#if( ! ( defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_BLE_NINA ) ) )
			if( FControlPointCharacteristic->written() )
				if( FControlPointCharacteristic->value() == 1 )
					T_ResetEnergyExpendedOutputPin::ClockPin();
	#endif // VISUINO_ANY_ESP32
#endif // _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
			{
				SendData();
				FModified() = false;
			}
		}

#ifdef VISUINO_BLE_NINA
		inline void SystemInit() {}
		inline void SystemStart() { InternalInit(); }

#else // VISUINO_BLE_NINA
		inline void SystemInit() { InternalInit(); }
		inline void SystemStart() {}
#endif // VISUINO_BLE_NINA

	protected:
		inline void InternalInit()
		{
			FService = C_OWNER.AddStandardService( 0x180D );
//			C_OWNER.GetPeripheral().addAttribute( *FService );
#ifdef VISUINO_BLE_NINA
			C_OWNER.AddveriseService( FService );
#endif // VISUINO_BLE_NINA
//			C_OWNER.GetPeripheral().setAdvertisedServiceUuid( FService->uuid());

#ifdef VISUINO_ANY_ESP32
			FCharacteristic = C_OWNER.template AddStandardCharacteristic<BLECharacteristic>( FService, 0x2A37, BLERead | BLENotify );
#else //  VISUINO_ANY_ESP32
//	#ifdef _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_
			if( T_EnergyExpandedInputPin_o_IsConnected::GetValue() )
			{
				if( TwoBytes() )
					FCharacteristic = new BLECharacteristic( "2A37", BLERead | BLENotify, 5 );

				else
					FCharacteristic = new BLECharacteristic( "2A37", BLERead | BLENotify, 4 );

			}
//	#else // _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_
			else
			{
				if( TwoBytes() )
					FCharacteristic = new BLECharacteristic( "2A37", BLERead | BLENotify, 3 );

				else
					FCharacteristic = new BLECharacteristic( "2A37", BLERead | BLENotify, 2 );
			}
//	#endif // _MITOV_BLE_HEARTHRATE_ENERGY_EXPANDED_PIN_

	#ifdef VISUINO_BLE_NINA
			FService->addCharacteristic( *FCharacteristic );
	#else //  VISUINO_BLE_NINA
			C_OWNER.GetPeripheral().addAttribute( *FCharacteristic );
	#endif //  VISUINO_BLE_NINA

#endif //  VISUINO_ANY_ESP32

			if( Location() != slNone )
			{
				FLocationCharacteristic = C_OWNER.template AddStandardCharacteristic<BLEUnsignedCharCharacteristic>( FService, 0x2A38, BLERead );
//				FLocationCharacteristic = new BLEUnsignedCharCharacteristic( "2A38", BLERead );
				FLocationCharacteristic->setValueLE( uint8_t( Location().GetValue() ) - 1 );
//				C_OWNER.GetPeripheral().addAttribute( *FLocationCharacteristic );
			}

#ifdef _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_
			FControlPointCharacteristic = C_OWNER.template AddStandardCharacteristic<BLEUnsignedCharCharacteristic>( FService, 0x2A39, BLEWrite );
//			FControlPointCharacteristic = new BLEUnsignedCharCharacteristic( "2A39", BLEWrite );
//			C_OWNER.GetPeripheral().addAttribute( *FControlPointCharacteristic );
#endif // _MITOV_BLE_HEARTHRATE_RESET_ENERGY_EXPANDED_PIN_

//			SendData();
#ifdef VISUINO_ANY_ESP32
			FService->start();
#endif

#ifdef VISUINO_BLE_NINA
			C_OWNER.GetPeripheral().addService( *FService );
			C_OWNER.GetPeripheral().setAdvertisedService( *FService );
#endif // VISUINO_BLE_NINA
		}

	public:
		inline ArduinoBluetoothLEHearthRateService()
		{
			FContactValue() = false;
			FModified() = true; // Force sending initial value
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
