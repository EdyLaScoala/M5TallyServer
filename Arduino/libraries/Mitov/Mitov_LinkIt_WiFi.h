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
#undef min
#undef max
#include <LWiFi.h>
#include <LWiFiServer.h>
#include <LWiFiClient.h>
#include <LWiFiUDP.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoLinkItWiFiEncryption { liweAuto, liweWEP, liweWPA };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Encryption,
		typename T_Password,
		typename T_SSID
	> class LinkItRemoteAccessPoint :
		public T_Enabled,
		public T_Encryption,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Encryption )
		_V_PROP_( SSID )
	    _V_PROP_( Password )

	public:
	    inline void UpdateEnabled()
	    {
            C_OWNER.WiFi_Disconnect_Update();
	    }

	public:
		inline void TryConnect( bool & AResult )
		{
			if( ! Enabled() )
				return;

			if( SSID().GetValue() == "" )
				return;

			if( Password().GetValue() == "" )
			{
				AResult = ( LWiFi.connect( SSID().c_str() ) > 0 );
				return;
			}


			switch( Encryption() )
			{
				case liweAuto:
				{
					if( LWiFi.connectWPA( SSID().c_str(), Password().c_str() ) > 0 )
					{
						AResult = true;
						return;
					}

					AResult = ( LWiFi.connectWEP( SSID().c_str(), Password().c_str() ) > 0 );
					return;
				}

				case liweWEP:
					AResult = ( LWiFi.connectWEP( SSID().c_str(), Password().c_str() ) > 0 );
					return;

				case liweWPA:
					AResult = ( LWiFi.connectWPA( SSID().c_str(), Password().c_str() ) > 0 );
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AccessPoints_TryConnect,
		typename T_AddressOutputPin,
		typename T_BSSIDOutputPin,
		uint32_t C_COUNT_AccessPoints,
		uint32_t C_COUNT_Sockets,
		typename T_Enabled,
		typename T_FoundSSIDOutputPin,
		typename T_FoundSignalStrengthOutputPin,
		typename T_GatewayIPOutputPin,
		typename T_IsStarted,
		typename T_MACOutputPin,
		typename T_SignalStrengthOutputPin,
		typename T_Sockets_StartSocket,
		typename T_Sockets_StopSocket,
		typename T_SubnetMaskIPOutputPin
	> class LinkItWiFiModule :
		public T_AddressOutputPin,
		public T_BSSIDOutputPin,
		public T_Enabled,
		public T_FoundSSIDOutputPin,
		public T_FoundSignalStrengthOutputPin,
		public T_GatewayIPOutputPin,
		public T_IsStarted,
		public T_MACOutputPin,
		public T_SignalStrengthOutputPin,
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
//		Mitov::SimpleObjectList<LinkItRemoteAccessPoint*>	AccessPoints;
//		Mitov::SimpleList<BasicEthernetSocket<LinkItWiFiModule>*>	Sockets;

		_V_PROP_( Enabled )

	public:
		_V_PROP_( IsStarted )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

	public:
	    void WiFi_Disconnect_Update()
	    {
	        StopEthernet();
//	        IsStarted() = false;
	    }

		inline bool GetIsStarted() { return IsStarted(); }

	public:
		virtual bool GetIPFromHostName( Mitov::String AHostName, ::IPAddress &AAdress )
		{
			bool AResult = ( LWiFi.hostByName( AHostName.c_str(), AAdress ) == 1 );
			if( ! AResult )
				AAdress = INADDR_NONE;

			return AResult;
		}

	protected:
		void StopEthernet()
		{
			T_Sockets_StopSocket::Call();
//			for( int i = 0; i < Sockets.size(); ++i )
//				Sockets[ i ]->StopSocket();

			LWiFi.end();
			IsStarted() = false;
		}

		void StartEthernet()
		{
/*
			if( ! AccessPoints.length() )
				return;

			if( ! Sockets.length() )
				return;
*/
			LWiFi.begin();
			for( int i = 0; i < C_COUNT_AccessPoints; ++i )
//				if( AccessPoints[ i ]->Connect() )
			{
				bool AConnected = false;
				T_AccessPoints_TryConnect::Call( i, AConnected );
				if( AConnected )
				{
					IsStarted() = true;
					if( T_MACOutputPin::GetPinIsConnected() )
					{
						uint8_t AMACAddress[VM_WLAN_WNDRV_MAC_ADDRESS_LEN] = {0};

						LWiFi.macAddress( AMACAddress );

						T_MACOutputPin::SetPinValue( MACAdressToString( AMACAddress ) );
					}

					if( T_AddressOutputPin::GetPinIsConnected() )
					{
						IPAddress ALocalIPAddress = LWiFi.localIP();
						T_AddressOutputPin::SetPinValue( IPAdressToString( ALocalIPAddress ) );
					}

					if( T_SignalStrengthOutputPin::GetPinIsConnected() )
					{
						int32_t ASetrength = LWiFi.RSSI();
						T_SignalStrengthOutputPin::SetPinValue( ASetrength );
					}

					if( T_BSSIDOutputPin::GetPinIsConnected() )
					{
						uint8_t AMACAddress[VM_WLAN_WNDRV_MAC_ADDRESS_LEN] = {0};

						LWiFi.BSSID( AMACAddress );

						T_BSSIDOutputPin::SetPinValue( MACAdressToString( AMACAddress ) );
					}

					if( T_GatewayIPOutputPin::GetPinIsConnected() )
					{
						IPAddress ALocalIPAddress = LWiFi.gatewayIP();
						T_AddressOutputPin::SetPinValue( IPAdressToString( ALocalIPAddress ) );
					}

					if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
					{
						IPAddress ALocalIPAddress = LWiFi.subnetMask();
						T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( ALocalIPAddress ) );
					}

					break;
				}
			}

			T_Sockets_StartSocket::Call();
//			for( int i = 0; i < Sockets.size(); ++i )
//				Sockets[ i ]->TryStartSocket();
		}

	public:
		void ScanNetworksInputPin_o_Receive( void *_Data )
		{
			int nearbyAccessPointCount = LWiFi.scanNetworks();
			for( int i = 0; i < nearbyAccessPointCount; i ++ )
			{
				T_FoundSSIDOutputPin::SetPinValue( LWiFi.SSID( i ));

				int32_t ASetrength = LWiFi.RSSI( i );
				T_FoundSignalStrengthOutputPin::SetPinValue( ASetrength );
			}
		}

		void CheckSignalStrengthInputPin_o_Receive( void *_Data )
		{
			if( IsStarted() )
				if( T_SignalStrengthOutputPin::IsPinConnected() )
				{
					int32_t ASetrength = LWiFi.RSSI();
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
		inline LinkItWiFiModule()
		{
			IsStarted() = false;
		}

	};
//---------------------------------------------------------------------------
/*
	template <Mitov::LinkItWiFiModule &C_OWNER> class LinkItWiFiTCPClientSocket : public TCPClientSocket<Mitov::LinkItWiFiModule,C_OWNER,LWiFiClient>
	{
		typedef TCPClientSocket<Mitov::LinkItWiFiModule,C_OWNER,LWiFiClient> inherited;

	public:
		virtual bool CanSend() override
		{
			return inherited::Enabled && C_OWNER.Enabled && inherited::FClient;
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
	template <Mitov::LinkItWiFiModule &C_OWNER> class LinkItWiFiTCPServerSocket : public TCPServerSocket<Mitov::LinkItWiFiModule,C_OWNER,LWiFiServer,LWiFiClient>
	{
		typedef TCPServerSocket<Mitov::LinkItWiFiModule,C_OWNER,LWiFiServer,LWiFiClient> inherited;

	public:
		virtual bool CanSend() override
		{
			return inherited::Enabled && C_OWNER.Enabled; // && inherited::FClient;
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
	template <Mitov::LinkItWiFiModule &C_OWNER> class LinkItWiFiUDPSocket : public UDPSocket<Mitov::LinkItWiFiModule,C_OWNER,LWiFiUDP>
	{
		typedef UDPSocket<Mitov::LinkItWiFiModule,C_OWNER,LWiFiUDP> inherited;

	public:
		using inherited::inherited;

	};
*/
//---------------------------------------------------------------------------
}
