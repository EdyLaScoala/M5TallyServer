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
#include <Mitov_BasicEthernet.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_SSID
	> class WiFiRemoteAccessPoint :
		public T_Enabled,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( SSID )

	public:
	    inline void UpdateEnabled()
	    {
            C_OWNER.WiFi_Disconnect_Update();
	    }

	public:
		inline void Connect( bool &AValue )
		{
			if( AValue )
				return;

			if( ! Enabled() )
				return;

//			if( SSID == "" )
//				return ( WiFi.begin() == WL_CONNECTED );

			AValue = ( WiFi.begin( (char *)SSID().c_str() ) == WL_CONNECTED );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Password,
		typename T_SSID
	> class WiFiRemoteAccessPointWPA :
		public T_Enabled,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Password )
		_V_PROP_( SSID )

	public:
	    inline void UpdateEnabled()
	    {
            C_OWNER.WiFi_Disconnect_Update();
	    }

	public:
		inline void Connect( bool &AValue )
		{
			if( AValue )
				return;

			if( ! Enabled() )
				return;

//			if( SSID == "" )
//				return ( WiFi.begin() == WL_CONNECTED );

			AValue = ( WiFi.begin( (char *)SSID().c_str(), (char *)Password().c_str() ) == WL_CONNECTED );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Key,
		typename T_KeyIndex,
		typename T_SSID
	> class WiFiRemoteAccessPointWEP :
		public T_Enabled,
		public T_Key,
		public T_KeyIndex,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Key )
		_V_PROP_( KeyIndex )
		_V_PROP_( SSID )


	public:
	    inline void UpdateEnabled()
	    {
            C_OWNER.WiFi_Disconnect_Update();
	    }

	public:
		inline void Connect( bool &AValue )
		{
			if( AValue )
				return;

			if( ! Enabled() )
				return;

//			if( SSID == "" )
//				return ( WiFi.begin() == WL_CONNECTED );

			AValue = ( WiFi.begin( (char *)SSID().c_str(), KeyIndex(), (char *)Key().c_str() ) == WL_CONNECTED );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AccessPoints_Connect,
		typename T_AddressOutputPin,
		typename T_BSSIDOutputPin,
		typename T_Enabled,
		typename T_FoundSSIDOutputPin,
		typename T_FoundSignalStrengthOutputPin,
		typename T_GatewayIPOutputPin,
		typename T_IPAddress,
		typename T_IsStarted,
		typename T_MACOutputPin,
		typename T_SignalStrengthOutputPin,
		typename T_Sockets_StopSocket,
		typename T_SubnetMaskIPOutputPin
	> class WiFiShield :
		public T_AccessPoints_Connect,
		public T_AddressOutputPin,
		public T_BSSIDOutputPin,
		public T_Enabled,
		public T_FoundSSIDOutputPin,
		public T_FoundSignalStrengthOutputPin,
		public T_GatewayIPOutputPin,
		public T_IPAddress,
		public T_IsStarted,
		public T_MACOutputPin,
		public T_SignalStrengthOutputPin,
		public T_Sockets_StopSocket,
		public T_SubnetMaskIPOutputPin
	{
	public:
		_V_PIN_( FoundSignalStrengthOutputPin )
		_V_PIN_( FoundSSIDOutputPin )

		_V_PIN_( AddressOutputPin )
		_V_PIN_( MACOutputPin )

		_V_PIN_( SignalStrengthOutputPin )
		_V_PIN_( BSSIDOutputPin )
		_V_PIN_( GatewayIPOutputPin )
		_V_PIN_( SubnetMaskIPOutputPin )

	public:
		_V_PROP_( IPAddress )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( IsStarted )

	public:
		inline bool GetIsStarted() { return IsStarted(); }

        void WiFi_Disconnect_Update()
        {
            StopEthernet();
        }

	public:
		bool GetIPFromHostName( Mitov::String AHostName, ::IPAddress &AAdress )
		{
			bool AResult = ( WiFi.hostByName( AHostName.c_str(), AAdress ) == 1 );
			if( ! AResult )
				AAdress = INADDR_NONE;

			return AResult;
		}

	protected:
		void StartEthernet()
		{
//			int AIndex = ((int)Parity) * 2 * 4 + ( StopBits - 1 ) + ( DataBits - 5);
//			C_SERIAL.begin( Speed );

//			Serial.println( "StartEthernet" );
			bool AIsConnected = false;
			T_AccessPoints_Connect::Call( AIsConnected );

//			for( int i = 0; i < C_ACESS_POINTS_COUNT; ++i )
//				if( AccessPoints[ i ]->Connect() )
			if( AIsConnected )
			{
				IsStarted() = true;
				if( IPAddress().Enabled() )
				{
					if( ! IPAddress().DNS().Enabled() )
						WiFi.config( IPAddress().IP().GetIPAddress() );

					else
					{
						if( ! IPAddress().DNS().Gateway().Enabled() )
							WiFi.config( IPAddress().IP().GetIPAddress(), IPAddress().DNS().IP().GetIPAddress() );

						else
						{
							if( ! IPAddress().DNS().Gateway().Subnet().Enabled() )
								WiFi.config( IPAddress().IP().GetIPAddress(), IPAddress().DNS().IP().GetIPAddress(), IPAddress().DNS().Gateway().IP().GetIPAddress() );

							else
								WiFi.config( IPAddress().IP().GetIPAddress(), IPAddress().DNS().IP().GetIPAddress(), IPAddress().DNS().Gateway().IP().GetIPAddress(), IPAddress().DNS().Gateway().Subnet().IP().GetIPAddress() );
						}

						if( IPAddress().DNS2().Enabled() )
							WiFi.setDNS( IPAddress().DNS().IP().GetIPAddress(), IPAddress().DNS2().IP().GetIPAddress() );

					}
				}

				if( T_MACOutputPin::GetPinIsConnected() )
				{
					uint8_t AMACAddress[6] = {0};

					WiFi.macAddress( AMACAddress );

//					char AMACString[ 6 * 3 + 1 ];
//					sprintf( AMACString, "%02X:%02X:%02X:%02X:%02X:%02X", AMACString[ 0 ], AMACAddress[ 1 ], AMACAddress[ 2 ], AMACAddress[ 3 ], AMACAddress[ 4 ], AMACAddress[ 5 ] );
					T_MACOutputPin::SetPinValue( MACAdressToString( AMACAddress ) );
				}

				if( T_AddressOutputPin::GetPinIsConnected() )
				{
					Mitov::String AIPString = IPAdressToString( WiFi.localIP());
					T_AddressOutputPin::SetPinValue( AIPString );
				}

				if( T_SignalStrengthOutputPin::GetPinIsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					T_SignalStrengthOutputPin::SetPinValue( ASetrength );
				}

				if( T_BSSIDOutputPin::GetPinIsConnected() )
				{
					uint8_t AMACAddress[6] = {0};

					WiFi.BSSID( AMACAddress );

					T_BSSIDOutputPin::SetPinValue( MACAdressToString( AMACAddress ) );
				}

				if( T_GatewayIPOutputPin::GetPinIsConnected() )
				{
					Mitov::String AIPString = IPAdressToString( WiFi.gatewayIP());
					T_AddressOutputPin::SetPinValue( AIPString );
				}

				if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
				{
					Mitov::String AIPString = IPAdressToString( WiFi.subnetMask());
					T_SubnetMaskIPOutputPin::SetPinValue( AIPString );
				}

//				break;
			}

//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->TryStartSocket();
		}

		void StopEthernet()
		{
			T_Sockets_StopSocket::Call();

			WiFi.disconnect();
			IsStarted() = false;
		}

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

	public:
        inline void RestartEthernet()
		{
			if( ! Enabled() )
				return;

//			C_SERIAL.end();
//			Ethernet.end();
			StartEthernet();
		}

	public:
		void ScanNetworksInputPin_o_Receive( void *_Data )
		{
			int nearbyAccessPointCount = WiFi.scanNetworks();
			for( int i = 0; i < nearbyAccessPointCount; i ++ )
			{
				T_FoundSSIDOutputPin::SetPinValue( WiFi.SSID( i ));

				int32_t ASetrength = WiFi.RSSI( i );
				T_FoundSignalStrengthOutputPin::SetPinValue( ASetrength );
			}
		}

		void CheckSignalStrengthInputPin_o_Receive( void *_Data )
		{
			if( IsStarted() )
				if( T_SignalStrengthOutputPin::GetPinIsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					T_SignalStrengthOutputPin::SetPinValue( ASetrength );
				}
		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartEthernet();

		}

	public:
		inline WiFiShield()
		{
			IsStarted() = false;
		}

/*
		WiFiShield( ::IPAddress local_ip ) :
			Enabled( true ),
			IsStarted( false )
		{
			IPAddress.Enabled = true;
			IPAddress.IP = local_ip;
		}

		WiFiShield( ::IPAddress local_ip, ::IPAddress dns_server) :
			WiFiShield( local_ip )
		{

			IPAddress.DNS.Enabled = true;
			IPAddress.DNS.IP = dns_server;
		}

		WiFiShield( ::IPAddress local_ip, ::IPAddress dns_server, ::IPAddress gateway) :
			WiFiShield( local_ip, dns_server )
		{
			IPAddress.DNS.Gateway.Enabled = true;
			IPAddress.DNS.Gateway.IP = gateway;
		}

		WiFiShield( ::IPAddress local_ip, ::IPAddress dns_server, ::IPAddress gateway, ::IPAddress subnet) :
			WiFiShield( local_ip, dns_server, gateway )
		{
			IPAddress.DNS.Gateway.Subnet.Enabled = true;
			IPAddress.DNS.Gateway.Subnet.IP = subnet;
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
