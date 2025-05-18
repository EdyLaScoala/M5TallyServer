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
	#include <BLEDevice.h>
	#include <BLEUtils.h>
#else // VISUINO_ANY_ESP32
	#ifdef VISUINO_ARDUINO_101
		#include <CurieBLE.h>
	#else // VISUINO_ARDUINO_101
		#include <BLEPeripheral.h>
	#endif // VISUINO_ARDUINO_101
#endif // VISUINO_ANY_ESP32

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//---------------------------------------------------------------------------
namespace CallChains
{
	void NotifyRemoteCharacteristicsCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify );
}
//---------------------------------------------------------------------------
namespace Mitov
{
//	class BLEFloatRemoteCharacteristic
//	{
//	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Characteristics_TryStart,
		typename T_UUID
	> class ArduinoBluetoothLERemoteService :
		public T_UUID
	{
	public:
		_V_PROP_( UUID )

	public:
		BLERemoteService		*FService = nullptr;

	public:
		inline void SystemInit()
		{
		}

		inline void SystemStart()
		{
		}

	public:
		inline void TryStart( bool AValue, bool &AResult )
		{
			if( AValue )
			{
#ifdef VISUINO_ANY_ESP32
//				Serial.println( "TEST55" );
				FService = C_OWNER.GetClient()->getService( UUID().c_str() );
//				Serial.println( "TEST56" );
#else // VISUINO_ANY_ESP32
#endif // VISUINO_ANY_ESP32
			}
			else
				FService = nullptr;

			ArduinoBluetoothLERemoteService::Call( AValue );
		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_OWNER> class ArduinoBluetoothLEBasicRemoteCharacteristic
	{
	public:
		Mitov::String	UUID;

	protected:
//		static	void *FInstance;

	public:
		inline void SystemInit()
		{
		}

	};
*/
//---------------------------------------------------------------------------
//	template <typename T_OWNER, T_OWNER &C_OWNER> void * ArduinoBluetoothLEBasicRemoteCharacteristic<T_OWNER, C_OWNER>::FInstance;
//---------------------------------------------------------------------------
//	template <typename T_OWNER, T_OWNER &C_OWNER, typename T, typename T_CHARACT, typename T_OUTPUT_PIN> class ArduinoBluetoothLETypedRemoteCharacteristic : public ArduinoBluetoothLEBasicRemoteCharacteristic<T_OWNER, C_OWNER>, public T_OUTPUT_PIN
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BigEndian,
		typename T_FAssigned,
		typename T_OutputPin,
		typename T,
		typename T_UUID,
		typename T_WriteInputPin_o_IsConnected
	> class ArduinoBluetoothLETypedRemoteCharacteristic :
		public T_BigEndian,
		public T_FAssigned,
		public T_OutputPin,
		public T_UUID,
		public T_WriteInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( UUID )

	public:
		_V_PROP_( BigEndian )

	public:
		_V_PROP_( WriteInputPin_o_IsConnected )

	protected:
		_V_PROP_( FAssigned ) //= false;

		T	FCurrentValue;

	protected:
		BLERemoteCharacteristic	*FCharacteristic = nullptr;

	public:
		inline void ReadInputPin_o_Receive( void *_Data )
		{
			if( FCharacteristic )
				FCharacteristic->readValue();

		}

		inline void WriteInputPin_o_Receive( void *_Data )
		{
			WriteOutput();
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			FCurrentValue = *(T*)_Data;
			FAssigned() = true;

			if( ! WriteInputPin_o_IsConnected() )
				WriteOutput();

		}

	protected:
		void WriteOutput()
		{
			if( ! FAssigned() )
				return;

			if( ! FCharacteristic )
				return;

/*
			if( BigEndian )
				FCharacteristic->setValueBE( AValue );

			else
				FCharacteristic->setValueLE( AValue );
*/
			uint8_t temp[ sizeof( T ) ];
			if( BigEndian() )
			{
				for( int i = 0; i < sizeof( T ); ++ i )
					temp[ sizeof( T ) - i - 1 ] = FCurrentValue >> ( i * 8 );
			}

			else
			{
				for( int i = 0; i < sizeof( T ); ++ i )
					temp[ i ] = FCurrentValue >> ( i * 8 );
			}

			FCharacteristic->writeValue( temp, sizeof( T ) );
		}

	public:
		inline void NotifyRemoteCharacteristicsCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify )
		{
			if( pBLERemoteCharacteristic != FCharacteristic )
				return;

			if( length != sizeof( T ) )
				return;

			T AValue = 0;
			if( BigEndian() )
			{
				for( int i = 0; i < sizeof( T ); ++ i )
					AValue |= uint32_t( pData[ sizeof( T ) - i - 1 ] ) << ( i * 8 );

			}

			else
			{
				for( int i = 0; i < sizeof( T ); ++ i )
					AValue |= uint32_t( pData[ i ] ) << ( i * 8 );
			}

			T_OutputPin::SetPinValue( AValue, false );
//			T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( length, pData ));
		}

	public:
		inline void TryStart( bool AValue, bool &AResult )
		{
			if( AValue )
			{
#ifdef VISUINO_ANY_ESP32
//				Serial.println( "Test88" );
				FCharacteristic = C_OWNER.FService->getCharacteristic( UUID().c_str() );
//				Serial.println( "Test89" );
				if( FCharacteristic )
					FCharacteristic->registerForNotify( CallChains::NotifyRemoteCharacteristicsCallback );

//				Serial.println( "Test90" );
#else // VISUINO_ANY_ESP32
#endif // VISUINO_ANY_ESP32
			}

			else
			{
				FCharacteristic = nullptr;
			}

		}

	public:
		inline void SystemInit()
		{
		}

	public:
		inline ArduinoBluetoothLETypedRemoteCharacteristic()
		{
			FAssigned() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BigEndian,
		typename T_FAssigned,
		typename T_OutputPin,
		typename T_UUID,
		typename T_WriteInputPin_o_IsConnected
	> class ArduinoBluetoothLEAnalogRemoteCharacteristic :
		public T_BigEndian,
		public T_FAssigned,
		public T_OutputPin,
		public T_UUID,
		public T_WriteInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( UUID )

	public:
		_V_PROP_( BigEndian )

	public:
		_V_PROP_( WriteInputPin_o_IsConnected )

	protected:
		_V_PROP_( FAssigned ) //= false;

		uint32_t	FCurrentValue;

	protected:
		BLERemoteCharacteristic	*FCharacteristic;

	public:
		inline void ReadInputPin_o_Receive( void *_Data )
		{
			FCharacteristic->readValue();
		}

		inline void WriteInputPin_o_Receive( void *_Data )
		{
			WriteOutput();
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			// Convert to uint32_t for easy shift operations!
			FCurrentValue = *(uint32_t*)_Data;
			FAssigned() = true;

			if( ! WriteInputPin_o_IsConnected() )
				WriteOutput();

		}

	protected:
		void WriteOutput()
		{
			if( ! FAssigned() )
				return;

			if( ! FCharacteristic )
				return;

//			Serial.println( "WriteOutput" );

			uint8_t temp[ sizeof( float ) ];

			if( BigEndian() )
			{
				for( int i = 0; i < sizeof( float ); ++ i )
					temp[ sizeof( float ) - i - 1 ] = FCurrentValue >> ( i * 8 );
			}

			else
			{
				for( int i = 0; i < sizeof( float ); ++ i )
					temp[ i ] = FCurrentValue >> ( i * 8 );
			}

			FCharacteristic->writeValue( temp, sizeof( float ) );

/*
			if( BigEndian )
				FCharacteristic->setValueBE( AValue );

			else
				FCharacteristic->setValueLE( AValue );
*/
		}

	public:
		inline void NotifyRemoteCharacteristicsCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify )
		{
			if( pBLERemoteCharacteristic != FCharacteristic )
				return;

			if( length != sizeof( float ) )
				return;

			uint32_t AValue = 0;
			if( BigEndian() )
			{
				for( int i = 0; i < sizeof( float ); ++ i )
					AValue |= uint32_t( pData[ sizeof( float ) - i - 1 ] ) << ( i * 8 );

			}

			else
			{
				for( int i = 0; i < sizeof( float ); ++ i )
					AValue |= uint32_t( pData[ i ] ) << ( i * 8 );
			}

			float AFloatValue = *( float * )&AValue;

			T_OutputPin::SetPinValue( AFloatValue, false );
//			T_OutputPin::SendPinValue( Mitov::TDataBlock( length, pData ));
		}

	public:
		inline void TryStart( bool AValue, bool &AResult )
		{
			if( AValue )
			{
#ifdef VISUINO_ANY_ESP32
//				Serial.println( "Test88" );
				FCharacteristic = C_OWNER.FService->getCharacteristic( UUID().c_str() );
//				Serial.println( "Test89" );
				if( FCharacteristic )
					FCharacteristic->registerForNotify( CallChains::NotifyRemoteCharacteristicsCallback );

//				Serial.println( "Test90" );

//				FCharacteristic = C_OWNER.FService->getCharacteristic( inherited::UUID.c_str() );
//				FCharacteristic->registerForNotify( CallChains::NotifyRemoteCharacteristicsCallback );
#else // VISUINO_ANY_ESP32
#endif // VISUINO_ANY_ESP32
			}

			else
			{
				FCharacteristic = nullptr;
			}

		}

	public:
		inline void SystemInit()
		{
		}

	public:
		inline ArduinoBluetoothLEAnalogRemoteCharacteristic()
		{
			FAssigned() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BigEndian,
		typename T_FAssigned,
		typename T_OutputPin,
		typename T_UUID,
		typename T_WriteInputPin_o_IsConnected
	> class ArduinoBluetoothLEBinaryRemoteCharacteristic :
		public T_BigEndian,
		public T_FAssigned,
		public T_OutputPin,
		public T_UUID,
		public T_WriteInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( UUID )

	public:
		_V_PROP_( BigEndian )

	public:
		_V_PROP_( WriteInputPin_o_IsConnected )

	protected:
		_V_PROP_( FAssigned ) //= false;

		Mitov::TDataBlock	FCurrentValue;

	protected:
		BLERemoteCharacteristic	*FCharacteristic;

	public:
		inline void ReadInputPin_o_Receive( void *_Data )
		{
			FCharacteristic->readValue();
		}

		inline void WriteInputPin_o_Receive( void *_Data )
		{
			WriteOutput();
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			FCurrentValue = *(Mitov::TDataBlock *)_Data;
			FAssigned() = true;

			if( ! WriteInputPin_o_IsConnected() )
				WriteOutput();

		}

	protected:
		void WriteOutput()
		{
//			Serial.println( "WriteOutput" );

			if( ! FCharacteristic )
				return;

			if( FAssigned() )
				FCharacteristic->writeValue( FCurrentValue.Data, FCurrentValue.Size );

		}

	public:
		inline void NotifyRemoteCharacteristicsCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify )
		{
			if( pBLERemoteCharacteristic != FCharacteristic )
				return;

			T_OutputPin::SendPinValue( Mitov::TDataBlock( length, pData ));
		}

	public:
		inline void TryStart( bool AValue, bool &AResult )
		{
			if( AValue )
			{
#ifdef VISUINO_ANY_ESP32
//				Serial.println( "Test88" );
				FCharacteristic = C_OWNER.FService->getCharacteristic( UUID().c_str() );
//				Serial.println( "Test89" );
				if( FCharacteristic )
					FCharacteristic->registerForNotify( CallChains::NotifyRemoteCharacteristicsCallback );

//				Serial.println( "Test90" );

//				FCharacteristic = C_OWNER.FService->getCharacteristic( inherited::UUID.c_str() );
//				FCharacteristic->registerForNotify( CallChains::NotifyRemoteCharacteristicsCallback );
#else // VISUINO_ANY_ESP32
#endif // VISUINO_ANY_ESP32
			}

			else
			{
				FCharacteristic = nullptr;
			}

		}

	public:
		inline void SystemInit()
		{
		}

	public:
		inline ArduinoBluetoothLEBinaryRemoteCharacteristic()
		{
			FAssigned() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BigEndian,
		typename T_FAssigned,
		typename T_OutputPin,
		typename T_UUID,
		typename T_WriteInputPin_o_IsConnected
	> class ArduinoBluetoothLETextRemoteCharacteristic :
		public T_BigEndian,
		public T_FAssigned,
		public T_OutputPin,
		public T_UUID,
		public T_WriteInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( UUID )

	public:
		_V_PROP_( BigEndian )

	public:
		_V_PROP_( WriteInputPin_o_IsConnected )

	protected:
		_V_PROP_( FAssigned ) //= false;

		Mitov::String	FCurrentValue;

	protected:
		BLERemoteCharacteristic	*FCharacteristic;

	public:
		inline void ReadInputPin_o_Receive( void *_Data )
		{
			FCharacteristic->readValue();
		}

		inline void WriteInputPin_o_Receive( void *_Data )
		{
			WriteOutput();
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			FCurrentValue = (char *)_Data;
			FAssigned() = true;

			if( ! WriteInputPin_o_IsConnected() )
				WriteOutput();

		}

	protected:
		void WriteOutput()
		{
//			Serial.println( "WriteOutput" );

			if( ! FCharacteristic )
				return;

			if( FAssigned() )
				FCharacteristic->writeValue( (uint8_t *)FCurrentValue.c_str(), FCurrentValue.length() );

		}

	public:
		inline void NotifyRemoteCharacteristicsCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify )
		{
			if( pBLERemoteCharacteristic != FCharacteristic )
				return;

			T_OutputPin::SetPinValue( StringEx( (const char*)pData, length ), false );
		}

	public:
		inline void TryStart( bool AValue, bool &AResult )
		{
			if( AValue )
			{
#ifdef VISUINO_ANY_ESP32
//				Serial.println( "Test88" );
				FCharacteristic = C_OWNER.FService->getCharacteristic( UUID().c_str() );
//				Serial.println( "Test89" );
				if( FCharacteristic )
					FCharacteristic->registerForNotify( CallChains::NotifyRemoteCharacteristicsCallback );

//				Serial.println( "Test90" );

//				FCharacteristic = C_OWNER.FService->getCharacteristic( inherited::UUID.c_str() );
//				FCharacteristic->registerForNotify( CallChains::NotifyRemoteCharacteristicsCallback );
#else // VISUINO_ANY_ESP32
#endif // VISUINO_ANY_ESP32
			}

			else
			{
				FCharacteristic = nullptr;
			}

		}

	public:
		inline void SystemInit()
		{
		}

	public:
		inline ArduinoBluetoothLETextRemoteCharacteristic()
		{
			FAssigned() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

