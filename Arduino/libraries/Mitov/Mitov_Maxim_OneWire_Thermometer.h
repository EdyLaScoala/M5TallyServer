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

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Octet1,
		typename T_Octet2,
		typename T_Octet3,
		typename T_Octet4,
		typename T_Octet5,
		typename T_Octet6,
		typename T_Octet7,
		typename T_Octet8
	> class TOneWireAddress :
		public T_Octet1,
		public T_Octet2,
		public T_Octet3,
		public T_Octet4,
		public T_Octet5,
		public T_Octet6,
		public T_Octet7,
		public T_Octet8
	{
	public:
		_V_PROP_( Octet1 )
		_V_PROP_( Octet2 )
		_V_PROP_( Octet3 )
		_V_PROP_( Octet4 )
		_V_PROP_( Octet5 )
		_V_PROP_( Octet6 )
		_V_PROP_( Octet7 )
		_V_PROP_( Octet8 )

	public:
		void GetOneWireAddress( uint8_t *AOneWireAddress )
		{
			AOneWireAddress[ 0 ] = Octet1();
			AOneWireAddress[ 1 ] = Octet2();
			AOneWireAddress[ 2 ] = Octet3();
			AOneWireAddress[ 3 ] = Octet4();
			AOneWireAddress[ 4 ] = Octet5();
			AOneWireAddress[ 5 ] = Octet6();
			AOneWireAddress[ 6 ] = Octet7();
			AOneWireAddress[ 7 ] = Octet8();
		}

/*
	public:
		bool IsZero()
		{
			if( Octet1() )
				return false;

			if( Octet2() )
				return false;

			if( Octet3() )
				return false;

			if( Octet4() )
				return false;

			if( Octet5() )
				return false;

			if( Octet6() )
				return false;

			if( Octet7() )
				return false;

			if( Octet8() )
				return false;

			return true;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_Address,
		typename T_AddressOutputPin,
		typename T_AlarmHighTemperature,
		typename T_AlarmLowTemperature,
		typename T_AlarmOutputPin,
		typename T_AutoDiscover,
		typename T_ClockInputPin_o_IsConnected,
		int C_DEVICE_INDEX,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OneWireOutputPin,
		typename T_OutputPin,
		int C_PIN_NUMBER,
		typename T_Resolution
	> class OneWireThermometer :
		public T_Address,
		public T_AddressOutputPin,
		public T_AlarmHighTemperature,
		public T_AlarmLowTemperature,
		public T_AlarmOutputPin,
		public T_AutoDiscover,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_InFahrenheit,
		public T_OneWireOutputPin,
		public T_OutputPin,
		public T_Resolution
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( OneWireOutputPin )
		_V_PIN_( AddressOutputPin )
		_V_PIN_( AlarmOutputPin )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address )
		_V_PROP_( AutoDiscover )
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )
		_V_PROP_( Resolution )

		_V_PROP_( AlarmHighTemperature )
		_V_PROP_( AlarmLowTemperature )

	public:
		void SetResolution( uint8_t AValue )
		{
			if( Resolution() == AValue )
				return;

			Resolution() = AValue;
			if( FThermometer )
				FThermometer->setResolution( FAddress, Resolution );
		}

	protected:
//		bool	FIsChained;
//		int		FDigitalPin;
//		int     FIndex;
//		float   FCurrentValue;

	protected:
		OneWire				*FOneWire = nullptr;
		// Migrate to direct OpenWire with DallasTemperature rewrite!
		DallasTemperature	*FThermometer = nullptr;
		DeviceAddress		FAddress;

	public:
		inline void Update_Address()
		{
			Address().GetOneWireAddress( FAddress );
		}

	public:
		inline void SystemInit()
		{
			Update_Address();
			if( C_DEVICE_INDEX )
			{
//				OneWireOutputPin.Notify( &FOneWire );
				// Migrate to OpenWire with DallasTemperature rewrite!
				T_OneWireOutputPin::Notify( &FThermometer );
			}

			else
			{
				GetOneWire();

				FThermometer = new DallasTemperature( FOneWire );
				FThermometer->begin();
			}

			if( AutoDiscover() )
				FThermometer->getAddress( FAddress, C_DEVICE_INDEX );

			FThermometer->setResolution( FAddress, Resolution() );


/*
			if( C_DEVICE_INDEX )
			{
				Serial.println( FThermometer->getDeviceCount() );
				Serial.println( C_DEVICE_INDEX );
				for (uint8_t i = 0; i < 8; i++)
					Serial.println( FAddress[ i ] );
			}
*/
		}

		inline void SystemStart()
		{
			if( C_DEVICE_INDEX == 0 )
			{
				FThermometer->setCheckForConversion( true );
				FThermometer->requestTemperatures();
				FThermometer->setCheckForConversion( false );
			}

			if( T_AddressOutputPin::GetPinIsConnected() )
			{
/*
				Mitov::String AAddress = Mitov::String( FAddress[ 0 ], HEX );
				for( int i = 1; i < 8; ++i )
					AAddress += '-' + Mitov::String( FAddress[ i ], HEX );

				AddressOutputPin.Notify( (void *)AAddress.c_str() );
*/
				char format[ 24 ];
				sprintf( format, "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X", FAddress[ 0 ], FAddress[ 1 ], FAddress[ 2 ], FAddress[ 3 ], FAddress[ 4 ], FAddress[ 5 ], FAddress[ 6 ], FAddress[ 7 ] );
				T_AddressOutputPin::SetPinValue( format );
			}

			if( T_AlarmOutputPin::GetPinIsConnected() )
			{
				FThermometer->setHighAlarmTemp( FAddress, (char)( AlarmHighTemperature() + 0.5 ));
				FThermometer->setLowAlarmTemp( FAddress, (char)( AlarmLowTemperature() + 0.5 ));

//				float AValue = ( AlarmHighTemperature - (-55) ) / ( 125 - -55 ) * 255;
//				FThermometer->setHighAlarmTemp( FAddress, (char)( AValue + 0.5 ));

//				AValue = ( AlarmLowTemperature - (-55) ) / ( 125 - -55 ) * 255;
//				FThermometer->setLowAlarmTemp( FAddress, (char)( AValue + 0.5 ));
			}
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! ClockInputPin_o_IsConnected() )
				ReadTemperature();

			// Needs to be changed to handle when the clock is different for the thermometers!
			if( C_DEVICE_INDEX == 0 )
				FThermometer->requestTemperatures();

		}

	protected:
		void GetOneWire()
		{
			if( ! FOneWire )
				FOneWire = new OneWire( C_PIN_NUMBER );
		}

		void ReadTemperature()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_AlarmOutputPin::GetPinIsConnected() )
			{
				bool AValue = FThermometer->hasAlarm( FAddress );
				T_AlarmOutputPin::SetPinValue( AValue );
			}

			float   AValue = (InFahrenheit() )
				? FThermometer->getTempF( FAddress )
				: FThermometer->getTempC( FAddress );

			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void OneWireInputPin_o_Receive( void *_Data )
		{
			if( C_DEVICE_INDEX )
				T_OneWireOutputPin::PinNotify( _Data );

			else
			{
				GetOneWire();
//				*(OneWire**)_Data = FOneWire;
				// Migrate to OpenWire with DallasTemperature rewrite!
				*(DallasTemperature**)_Data = FThermometer;
			}
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTemperature();
		}

	public:
		OneWireThermometer()
		{
			memset( FAddress, 0, 8 );
		}

/*
		OneWireThermometer( uint8_t AByte0, uint8_t AByte1, uint8_t AByte2, uint8_t AByte3, uint8_t AByte4, uint8_t AByte5, uint8_t AByte6, uint8_t AByte7 )
		{
			FAddress[ 0 ] = AByte0;
			FAddress[ 1 ] = AByte1;
			FAddress[ 2 ] = AByte2;
			FAddress[ 3 ] = AByte3;
			FAddress[ 4 ] = AByte4;
			FAddress[ 5 ] = AByte5;
			FAddress[ 6 ] = AByte6;
			FAddress[ 7 ] = AByte7;
		}
*/
/*
		virtual ~OneWireThermometer()
		{
			if( C_DEVICE_INDEX == 0 )
			{
				if( FThermometer )
					delete FThermometer;

				if( FOneWire )
					delete FOneWire;
			}
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif