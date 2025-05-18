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
#include <ESP8266WiFi.h>
//#include "ip_addr.h"
//#include "espconn.h"
#include <sys/time.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!L
#endif

//  #define __ESP8266_WIFI__DEBUG__

namespace Mitov
{
/*
	class ESP8266WiFiModuleOptionalIPConfig
	{
	public:
		bool	Enabled = false;
		::IPAddress	IP;

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Channel,
		typename T_Config,
		typename T_Enabled,
		typename T_MacAddress,
		typename T_Password,
		typename T_SSID
	> class ESP8266ModuleRemoteAccessPoint :
		public T_Channel,
		public T_Config,
		public T_Enabled,
		public T_MacAddress,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( SSID )
	    _V_PROP_( Password )
		_V_PROP_( Channel )
		_V_PROP_( MacAddress )
		_V_PROP_( Config )

	public:
        inline void UpdateEnabled()
        {
            C_OWNER.WiFi_Disconnect_Update();
        }

	protected:
		void Begin()
		{
			const char *APassword;
			if( Password().GetValue() == "" )
				APassword = nullptr;

			else
				APassword = Password().c_str();

			int32_t channel;

			if( Channel().Enabled() )
				channel = Channel().Channel();

			else
				channel = 0;

			uint8_t AMacAddress[ 6 ];
			const uint8_t* bssid;

			if( MacAddress().Enabled() )
			{
				MacAddress().MacAddress().GetMacAddress( AMacAddress );
				bssid = AMacAddress;
			}

			else
				bssid = nullptr;

//			Serial.println( SSID );
//			Serial.println( APassword );
//			wifi_set_macaddr( SOFTAP_IF, (uint8_t*)bssid );
//			WiFi.begin( (char *)SSID().c_str(), APassword, channel, nullptr );
			WiFi.begin( (char *)SSID().c_str(), APassword, channel, bssid );

			if( C_OWNER.HostName().GetValue() != "" )
				WiFi.hostname( C_OWNER.HostName() );

			WiFi.reconnect();
		}

	public:
		void Connect()
		{
			if( ! Enabled() )
				return;

			if( SSID().GetValue() == "" )
				return;

//			Serial.println( SSID );
//			Serial.println( Password );

			if( Config().Enabled() )
				WiFi.config( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress(), Config().DNS1().GetIPAddress(), Config().DNS2().GetIPAddress() );

			Begin();

/*
			for(;;)
			{
//				Serial.print( "." );
				int ARes = WiFi.status();
//				Serial.print( ";" );
//				Serial.println( ARes );
				if( ARes == WL_CONNECTED )
					return true;

//				Serial.println( "+" );
				if( ARes == WL_CONNECT_FAILED )
					return false;

//				Serial.println( "++" );
				if( ARes == WL_NO_SSID_AVAIL )
					return false;

//				Serial.println( "+++" );
				delay(500);
			}
*/
/*
			while (WiFi.status() != WL_CONNECTED) {
			WL_CONNECT_FAILED

			return ( ARes == WL_CONNECTED );
*/
//			return ( WiFi.begin( (char *)SSID.c_str(), APassword, channel, bssid ) == WL_CONNECTED );


//			return ( WiFi.begin( (char *)SSID.c_str() ) == WL_CONNECTED );


/*
			if( Password == "" )
				return ( LWiFi.connect( SSID.c_str() ) > 0 );

			switch( Encription )
			{
				case liweAuto:
				{
					if( LWiFi.connectWPA( SSID.c_str(), Password.c_str() ) > 0 )
						return true;

					return( LWiFi.connectWEP( SSID.c_str(), Password.c_str() ) > 0 );
				}

				case liweWEP:
					return( LWiFi.connectWEP( SSID.c_str(), Password.c_str() ) > 0 );

				case liweWPA:
					return( LWiFi.connectWPA( SSID.c_str(), Password.c_str() ) > 0 );
			}
*/
//			return false;
		}

	public:
		inline void IsEnabled( bool &AIsEnabled )
		{
			if( ( ! Enabled().GetValue() ) || ( SSID().GetValue() == "" ))
				return;

			AIsEnabled = true;
		}

		inline void TryConnect( bool &AIsEnabled )
		{
			if( ( ! Enabled().GetValue() ) || ( SSID().GetValue() == "" ))
				return;

			AIsEnabled = true;
			Connect();
		}

		inline void AssignDNS()
		{
			if( Config().DNS1().GetUInt32Value() == 0 )
			{
				IPAddress AIP = WiFi.gatewayIP();
				WiFi.disconnect();

//					delay( 10000 );

//					Serial.println( AIP.toString() );
				if( Config().Enabled() )
					WiFi.config( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress(), AIP, Config().DNS2().GetIPAddress() );

				else
					WiFi.config( ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 255, 255, 255, 0 ), AIP );

/*
				ip_addr_t d;
				d.type = IPADDR_TYPE_V4;

				// Set DNS1-Server
				d.u_addr.ip4.addr = static_cast<uint32_t>( AIP );

				dns_setserver( 0, &d );
*/
				Begin();
//					WiFi.reconnect();
//					if(esp_wifi_disconnect() == ESP_OK)
//						esp_wifi_connect();

			}
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Channel,
		typename T_Config,
		typename T_Enabled,
		typename T_IsHidden,
		typename T_MacAddress,
		typename T_Password,
		typename T_SSID
	> class ESP8266WiFiModuleAccessPoint :
		public T_Channel,
		public T_Config,
		public T_Enabled,
		public T_IsHidden,
		public T_MacAddress,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IsHidden )
		_V_PROP_( MacAddress )
		_V_PROP_( Channel )
		_V_PROP_( SSID )
		_V_PROP_( Password )
		_V_PROP_( Config )

	public:
		inline void TryStart()
		{
//Serial.println( "TEST1" );
//Serial.println( SSID().GetValue() );
//Serial.println( IsHidden().GetValue() );
//			if( AOwner->Enabled )
				if( Enabled() )
					if( SSID().GetValue() != "" )
					{
						if( MacAddress().Enabled() )
						{
							uint8_t AMacAddress[ 6 ];
							MacAddress().MacAddress().GetMacAddress( AMacAddress );
							wifi_set_macaddr( SOFTAP_IF, AMacAddress );
						}

						if( Config().Enabled() )
						{
//							Serial.print( "IP : " );
//							Serial.println( IPAdressToString( Config().IP().GetIPAddress() ) );
							WiFi.softAPConfig( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress() );
						}

						if( Password().GetValue() != "" )
							WiFi.softAP( SSID().c_str(), Password().c_str(), Channel(), IsHidden() );

						else
//								WiFi.softAP( SSID.c_str() );
							WiFi.softAP( SSID().c_str(), nullptr, Channel(), IsHidden() );

						WiFi.softAPIP();
					}

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ChannelOutputPin,
		typename T_CountOutputPin,
		typename T_EncryptionOutputPin,
		typename T_FScanRequest,
		typename T_FailedOutputPin,
		typename T_FoundNetworkOutputPin,
		typename T_IsHiddenOutputPin,
		typename T_MACAddressOutputPin,
		typename T_SSIDOutputPin,
		typename T_ScanningOutputPin,
		typename T_SignalStrengthOutputPin
	> class ESP8266ModuleScanNetworksOperation :
		public T_ChannelOutputPin,
		public T_CountOutputPin,
		public T_EncryptionOutputPin,
		public T_FScanRequest,
		public T_FailedOutputPin,
		public T_FoundNetworkOutputPin,
		public T_IsHiddenOutputPin,
		public T_MACAddressOutputPin,
		public T_SSIDOutputPin,
		public T_ScanningOutputPin,
		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( ScanningOutputPin )
		_V_PIN_( FailedOutputPin )
		_V_PIN_( CountOutputPin )
		_V_PIN_( SignalStrengthOutputPin )
		_V_PIN_( SSIDOutputPin )
		_V_PIN_( MACAddressOutputPin )
		_V_PIN_( EncryptionOutputPin )
		_V_PIN_( ChannelOutputPin )
		_V_PIN_( IsHiddenOutputPin )
		_V_PIN_( FoundNetworkOutputPin )

	protected:
		_V_PROP_( FScanRequest )

	public:
		inline void SystemStart()
		{
			T_ScanningOutputPin::SetPinValue( FScanRequest(), false );
			ScanInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
			T_ScanningOutputPin::SetPinValue( FScanRequest(), true );
			if( FScanRequest() )
			{
				int8_t AComplete = WiFi.scanComplete();
				if( AComplete == WIFI_SCAN_FAILED )
				{
					T_FailedOutputPin::ClockPin();
					T_CountOutputPin::SetPinValue( 0 );
					FScanRequest() = false;
				}

				else if( AComplete >= 0 )
				{
					T_CountOutputPin::SetPinValue( AComplete );
					for( int i = 0; i < AComplete; i ++ )
					{
						Mitov::String ssid;
						uint8_t encryptionType;
						int32_t ASetrength;
						uint8_t* BSSID;
						int32_t channel;
						bool isHidden;
						if( WiFi.getNetworkInfo( i, ssid, encryptionType, ASetrength, BSSID, channel, isHidden ))
						{
							Mitov::String BSSIDStr = WiFi.BSSIDstr( i );

							T_SSIDOutputPin::SetPinValue( ssid.c_str() );
							T_SignalStrengthOutputPin::SetPinValue( ASetrength );
							T_EncryptionOutputPin::SetPinValue( encryptionType );
							T_MACAddressOutputPin::SetPinValue( BSSIDStr.c_str() );
							T_ChannelOutputPin::SetPinValue( channel );
							T_IsHiddenOutputPin::SetPinValue( isHidden );
						}

						T_FoundNetworkOutputPin::ClockPin();
					}

					FScanRequest() = false;
				}
			}
		}

	public:
		inline void TryStart() {}

	public:
		void ScanInputPin_o_Receive( void *_Data )
		{
			if( WiFi.scanComplete() != WIFI_SCAN_RUNNING )
			{
				WiFi.scanNetworks( true );
				FScanRequest() = true;
			}
/*
			for( int i = 0; i < nearbyAccessPointCount; i ++ )
			{
				FoundSSIDOutputPin.Notify( LWiFi.SSID( i ));

				int32_t ASetrength = LWiFi.RSSI( i );
				FoundSignalStrengthOutputPin.Notify( &ASetrength );
			}
*/
		}

	public:
		inline ESP8266ModuleScanNetworksOperation()
		{
			FScanRequest() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Key,
		typename T_Key_ApplyValues,
		typename T_Key_GetValue
	> class TArduinoESP8266WiFiFingerprint :
		public T_Enabled,
		public T_Key
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Key )

	};
//---------------------------------------------------------------------------
	enum TArduinoESP8266PhysicalMode
	{
		Mode11B,
		Mode11G,
		Mode11N
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Host
	> class TArduinoESP8266WiFiNetworkTimeServer :
		public T_Enabled,
		public T_Host
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Host )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DaylightOffset,
		typename T_Enabled,
		typename T_Servers_0,
		typename T_Servers_1,
		typename T_Servers_2,
		typename T_TimeZoneOffset
	> class TArduinoESP8266WiFiNetworkTime :
		public T_DaylightOffset,
		public T_Enabled,
		public T_Servers_0,
		public T_Servers_1,
		public T_Servers_2,
		public T_TimeZoneOffset
	{
	public:
		typedef T_Servers_0 T_SERVER_0;
		typedef T_Servers_1 T_SERVER_1;
		typedef T_Servers_2 T_SERVER_2;

	public:
		_V_PROP_( DaylightOffset )
		_V_PROP_( Enabled )
		_V_PROP_( Servers_0 )
		_V_PROP_( Servers_1 )
		_V_PROP_( Servers_2 )
		_V_PROP_( TimeZoneOffset )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_NetworkTime,
		typename T_OutputPin
	> class TArduinoESP8266WiFiTime :
		public T_Enabled,
		public T_InitialValue,
		public T_NetworkTime,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( NetworkTime )

	protected:
		template <typename T, typename T1> void ApplyServer( T &AServer, T1 &AServers, uint8_t &AIndex )
		{
//			Serial.println( "STEP1" );
			if( AServer.Enabled() )
				if( AServer.Host().GetValue() != "" )
				{
//					Serial.println( AIndex );
//					Serial.println( AServer.Host().GetValue() );
					AServers[ AIndex ++ ] = (char *)AServer.Host().c_str();
				}

		}

	public:
		void UpdateSettings()
		{
//			Serial.println( "UpdateSettings" );

			if( NetworkTime().Enabled() )
			{
				char *AServers[ 3 ] = { 0 };
				uint8_t AIndex = 0;

				ApplyServer<typename T_NetworkTime::T_SERVER_0>( NetworkTime().Servers_0(), AServers, AIndex );
				ApplyServer<typename T_NetworkTime::T_SERVER_1>( NetworkTime().Servers_1(), AServers, AIndex );
				ApplyServer<typename T_NetworkTime::T_SERVER_2>( NetworkTime().Servers_2(), AServers, AIndex );

//				for( int i = 0; i < 3; ++i )
//					Serial.println( AServers[ i ] );

				configTime( NetworkTime().TimeZoneOffset().GetValue() * 3600, ( NetworkTime().DaylightOffset().Enabled().GetValue() ) ? NetworkTime().DaylightOffset().Offset().GetValue() * 3600 : 0, AServers[ 0 ], AServers[ 1 ], AServers[ 2 ] );
			}

			else
				configTime( 0, 0, nullptr, nullptr, nullptr );

		}

	protected:
		inline void ReadTime( bool AChangeOnly )
		{
			if( Enabled() )
			{
				if( T_OutputPin::GetPinIsConnected() )
				{
					time_t ANow = time(nullptr);
					struct tm ATimeinfo;
					gmtime_r( &ANow, &ATimeinfo );

					TDateTime ADateTime;

					if( ADateTime.TryEncodeDateTime( ATimeinfo.tm_year + 1900, ATimeinfo.tm_mon + 1, ATimeinfo.tm_mday, ATimeinfo.tm_hour, ATimeinfo.tm_min, ATimeinfo.tm_sec, 0 ))
					{
						int32_t AOffset = NetworkTime().TimeZoneOffset().GetValue() * 3600;
						if( NetworkTime().DaylightOffset().Enabled().GetValue() )
							AOffset += NetworkTime().DaylightOffset().Offset().GetValue() * 3600;

						ADateTime.AddSeconds( AOffset );
						T_OutputPin::SetPinValue( ADateTime, AChangeOnly );
					}

				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( false );
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadTime( true );
		}

		inline void SystemStart()
		{
			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;

			InitialValue().GetValue().DecodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond );
			struct tm t = {0};        // Initalize to all 0's
			t.tm_year = AYear - 1900;    // This is year-1900, so 121 = 2021
			t.tm_mon = AMonth - 1;
			t.tm_mday = ADay;
			t.tm_hour = AHour;
			t.tm_min = AMinute;
			t.tm_sec = ASecond;
			time_t timeSinceEpoch = mktime(&t);

			struct timeval tv;
			tv.tv_sec = timeSinceEpoch;  // epoch time (seconds)
			tv.tv_usec = 1;    // microseconds
			settimeofday( &tv, nullptr );

			UpdateSettings();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AccessPoint,
		typename T_AccessPoints_AssignDNS,
		typename T_AccessPoints_IsEnabled,
		typename T_AccessPoints_TryConnect,
		typename T_AddressOutputPin,
		typename T_AutoReconnect,
		typename T_BSSIDOutputPin,
		uint32_t C_COUNT_AccessPoints,
		uint32_t C_COUNT_Scan_Elements,
		uint32_t C_COUNT_Sockets,
		bool C_ESPNow,
		typename T_Enabled,
		typename T_FAccessPointIndex,
		typename T_FDNSUpdated,
		typename T_FIsStarted,
		typename T_FRemoteStarted,
		typename T_GatewayIPOutputPin,
		typename T_HostName,
		typename T_MACOutputPin,
//		typename T_NoDelay,
		typename T_MacAddress,
		typename T_Operations_TryStart,
		typename T_PhysicalMode,
		typename T_RemoteConnectedOutputPin,
		typename T_Sockets_StopSocket,
		typename T_SubnetMaskIPOutputPin,
//		typename T_Time,
		typename T_TransmitPower
	> class ESP8266WiFiModule :
		public T_AccessPoint,
		public T_AddressOutputPin,
		public T_AutoReconnect,
		public T_BSSIDOutputPin,
		public T_Enabled,
		public T_FAccessPointIndex,
		public T_FDNSUpdated,
		public T_FIsStarted,
		public T_FRemoteStarted,
		public T_GatewayIPOutputPin,
		public T_HostName,
		public T_MACOutputPin,
//		public T_NoDelay,
		public T_MacAddress,
		public T_PhysicalMode,
		public T_RemoteConnectedOutputPin,
		public T_Sockets_StopSocket,
		public T_SubnetMaskIPOutputPin,
//		public T_Time,
		public T_TransmitPower
	{
	public:
		_V_PIN_( AddressOutputPin )
		_V_PIN_( MACOutputPin )
		_V_PIN_( BSSIDOutputPin )
		_V_PIN_( GatewayIPOutputPin )
		_V_PIN_( SubnetMaskIPOutputPin )
		_V_PIN_( RemoteConnectedOutputPin )

	public:
		_V_PROP_( AccessPoint )

	public:
		_V_PROP_( MacAddress )
		_V_PROP_( PhysicalMode )
		_V_PROP_( HostName )
		_V_PROP_( Enabled )
		_V_PROP_( AutoReconnect )
//		_V_PROP_( Time )
		_V_PROP_( TransmitPower )
//		_V_PROP_( NoDelay )

	public:
		_V_PROP_( FIsStarted )
		_V_PROP_( FRemoteStarted )
		_V_PROP_( FAccessPointIndex )
		_V_PROP_( FDNSUpdated )

	protected:
		unsigned int	FLastTime;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

            Enabled() = AValue;
			UpdateEnabled();
		}

		void UpdateEnabled()
		{
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

	public:
		inline void UpdateAutoReconnect()
		{
			WiFi.setAutoReconnect( AutoReconnect() );
		}

	public:
		void Update_TransmitPower()
		{
			WiFi.setOutputPower( TransmitPower().GetValue() );
		}

		void Update_PhysicalMode()
		{
			WiFi.setPhyMode( WiFiPhyMode_t( PhysicalMode().GetValue() ));
		}

		void UpdateAccessPoint()
		{
			if( AccessPoint().Enabled() )
			{
				if( Enabled() )
					AccessPoint().TryStart();
			}

			else
				WiFi.softAPdisconnect();

		}

	public:
		void WiFi_Disconnect_Update()
		{
//			T_Sockets_StopSocket::Call();

//			for( int i = 0; i < C_COUNT_Sockets; ++i )
//				Sockets[ i ]->StopSocket();

#ifdef __ESP32_WIFI__DEBUG__
			Serial.println( "WiFi_Disconnect_Update" );
#endif
			WiFi.disconnect( true );
			FIsStarted() = false;
			FRemoteStarted() = false;
			FDNSUpdated() = false;
		}

	public:
		inline void TryStart() {}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartEthernet();

		}

	public:
		bool GetIPFromHostName( Mitov::String AHostName, ::IPAddress &AAdress )
		{
			bool AResult = ( WiFi.hostByName( AHostName.c_str(), AAdress ) == 1 );
			if( ! AResult )
				AAdress = INADDR_NONE;

			return AResult;
		}

	public:
        void RestartEthernet()
		{
			if( ! Enabled() )
				return;

//			C_SERIAL.end();
//			Ethernet.end();
			StartEthernet();
		}

	public:
		bool GetIsStarted()
		{
//			Serial.print( "IS_STARTED : " );
//			Serial.println( FIsStarted().GetValue() );
//			Serial.println( uint32_t( this ));
			return FIsStarted().GetValue();
		}

/*
		bool GetIsStarted1()
		{

			Serial.print( "IS_STARTED1 : " );
			Serial.println( FIsStarted().GetValue() );
			Serial.println( uint32_t( this ));
			return FIsStarted().GetValue();
		}
*/
	protected:
		void StopEthernet()
		{
//			Serial.println( "StopEthernet" );
			T_Sockets_StopSocket::Call();

//			for( int i = 0; i < C_COUNT_Sockets; ++i )
//				Sockets[ i ]->StopSocket();

			WiFi.disconnect( true );
			FIsStarted() = false;
			FRemoteStarted() = false;
			FDNSUpdated() = false;
		}

		void StartEthernet()
		{
/*
			if( ! AccessPoints.length() )
				return;

			if( ! Sockets.length() )
				return;
*/
//			Serial.println( "TRY CONNECT" );
/*
			if( HostName != "" )
			{
				WiFi.hostname( HostName );
//				espconn_mdns_set_hostname( (char *) HostName.c_str() );
			}

//			Serial.println( "TRY CONNECT1" );

			bool AConnected = false;
			for( int i = 0; i < C_COUNT_AccessPoints; ++i )
				if( AccessPoints[ i ]->Connect() )
				{
					FRemoteStarted = true;
					AConnected = true;
//					Serial.println( "CONNECT" );

					if( AddressOutputPin.IsConnected() )
					{
						Mitov::String IPAddress = IPAdressToString( WiFi.localIP());
						AddressOutputPin.Notify( (void *)IPAddress.c_str() );
					}

					if( MACOutputPin.IsConnected() )
					{
						Mitov::String AAddress = WiFi.macAddress();
						MACOutputPin.Notify( (void *)AAddress.c_str() );
					}

					FIsStarted = true;
					break;
				}

			if( ! AConnected )
				WiFi.begin();
*/
//			WiFi.begin();
//			WiFi.disconnect( true );

			if( HostName().GetValue() != "" )
			{
				WiFi.hostname( HostName() );
//				espconn_mdns_set_hostname( (char *) HostName.c_str() );
			}

			if( MacAddress().Enabled() )
			{
				uint8_t AMacAddress[ 6 ];
				MacAddress().MacAddress().GetMacAddress( AMacAddress );
				wifi_set_macaddr( STATION_IF, AMacAddress );
			}

//			RemoteConnectedOutputPin.SetValue( AConnected, true );

			WiFi.setAutoReconnect( AutoReconnect() );

			WiFi.mode( WIFI_OFF );

            if( ( AccessPoint().Enabled() || ( ! AccessPoint().Enabled().GetIsConstant() )) &&
                ( ( AccessPoint().SSID().GetValue() != "" ) || ( ! AccessPoint().SSID().GetIsConstant() ))
              )
            {
                if( C_ESPNow || C_COUNT_AccessPoints )
                {
        	    	WiFi.mode( WIFI_AP_STA );
#ifdef __ESP8266_WIFI__DEBUG__
    			    Serial.println( "WIFI_AP_STA" );
#endif
        	    }

                else
        	    {
        	        WiFi.mode( WIFI_AP );
#ifdef __ESP8266_WIFI__DEBUG__
    			    Serial.println( "WIFI_AP" );
#endif
        	    }

            }

            else if( C_COUNT_AccessPoints > 0 )
            {
            	WiFi.mode( WIFI_STA );
#ifdef __ESP8266_WIFI__DEBUG__
    			    Serial.println( "WIFI_STA" );
#endif
            }

            else
            {
                if( C_ESPNow || C_COUNT_Scan_Elements )
                {
                    WiFi.mode( WIFI_STA );
#ifdef __ESP8266_WIFI__DEBUG__
    			    Serial.println( "WIFI_STA" );
#endif
                }

                else
                {
        		    WiFi.mode( WIFI_OFF );
#ifdef __ESP8266_WIFI__DEBUG__
    			    Serial.println( "WIFI_OFF" );
#endif
        		}
            }

			if( Enabled() )
				AccessPoint().TryStart();

			if( T_BSSIDOutputPin::GetPinIsConnected() )
				T_BSSIDOutputPin::SetPinValue( WiFi.BSSIDstr().c_str() );

			if( T_GatewayIPOutputPin::GetPinIsConnected() )
				T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

			if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
				T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

//			for( int i = 0; i < C_COUNT_Sockets; ++i )
//				Sockets[ i ]->TryStartSocket();

//			WiFi.setNoDelay( NoDelay() );
		}

	public:
		inline void SystemStart()
		{
//			Serial.println( "SystemStart" );
			T_RemoteConnectedOutputPin::SetPinValue( WiFi.isConnected(), false );

			FLastTime = micros() + 10000000;

			Update_PhysicalMode();
			Update_TransmitPower();

/*
			if( FRemoteStarted )
			{
				if( AddressOutputPin.IsConnected() )
				{
					Mitov::String IPAddress = IPAdressToString( WiFi.localIP());
					AddressOutputPin.Notify( (void *)IPAddress.c_str() );
				}

				if( MACOutputPin.IsConnected() )
				{
					Mitov::String AAddress = WiFi.macAddress();
					MACOutputPin.Notify( (void *)AAddress.c_str() );
				}
			}

			if( BSSIDOutputPin.IsConnected() )
				BSSIDOutputPin.SendValue( WiFi.BSSIDstr() );

			if( GatewayIPOutputPin.IsConnected() )
				GatewayIPOutputPin.SendValue( IPAdressToString( WiFi.gatewayIP() ));

			if( SubnetMaskIPOutputPin.IsConnected() )
				SubnetMaskIPOutputPin.SendValue( IPAdressToString( WiFi.subnetMask() ));
*/
			if( T_MACOutputPin::GetPinIsConnected() )
				T_MACOutputPin::SetPinValue( WiFi.macAddress().c_str() );

		}

		inline void SystemLoopBegin() // Called only for RemoteConnectedOutputPin.IsConnected
		{
//			return;
			int ARes = WiFi.status();

//			Serial.print( "STATUS: " );
//			Serial.println( ARes );

			if( ! WiFi.isConnected() )
			{
//				Serial.println( "WiFi.isConnected()" );
//				Serial.println( "DISCONNECTED" );

//				FIsStarted() = false;
				FRemoteStarted() = false;
				FDNSUpdated() = false;
			}

			bool AIsEnabled = false;
			if( C_COUNT_AccessPoints > 0 )
			{
//				Serial.print( "STATUS: " );
//				Serial.println( FRemoteStarted );

				if( ! FRemoteStarted() )
				{
					unsigned long ACurrentMicros = micros();
					if( (( ACurrentMicros - FLastTime ) > 10000000 ) || ( ARes != WL_CONNECTED ) && ( ARes != WL_DISCONNECTED ) && ( ARes != WL_IDLE_STATUS ) )
					{
//			WiFi.printDiag( Serial );
//			Serial.print( "STATUS: " );
//				Serial.println( ARes );
						FLastTime = ACurrentMicros;
//						AccessPoints[ FAccessPointIndex ]->Connect( this );
						FAccessPointIndex() = FAccessPointIndex() + 1;
						if( FAccessPointIndex().GetValue() > C_COUNT_AccessPoints )
							FAccessPointIndex() = 1;

//							Serial.println( FAccessPointIndex );
						T_AccessPoints_TryConnect::Call( FAccessPointIndex().GetValue() - 1, AIsEnabled );
					}

					else
						T_AccessPoints_IsEnabled::Call( FAccessPointIndex().GetValue() - 1, AIsEnabled );

				}
				else
					T_AccessPoints_IsEnabled::Call( FAccessPointIndex().GetValue() - 1, AIsEnabled );

			}

			T_RemoteConnectedOutputPin::SetPinValue( WiFi.isConnected(), true );

//			Serial.println( AIsEnabled );

			if( ! AIsEnabled )
			{
//				WiFi.begin();
//				Serial.println( "FIsStarted" );
				FIsStarted() = true;
			}

			else if( WiFi.isConnected() )
			{
				if( ! FRemoteStarted() )
				{
					if( ! FDNSUpdated() )
					{
						T_Operations_TryStart::Call();

//			WiFi.printDiag( Serial );
//						Serial.println( "TEST11" );
						FDNSUpdated() = true;
/*
						if( AccessPoints_CallChain )
						{
							bool AIsEnabled = false;
							AccessPoints_CallChain( FAccessPointIndex, 2, AIsEnabled );
						}
*/
						return;
					}

					else
					{
						FIsStarted() = true;
//						Serial.println( "FRemoteStarted = true" );
						FRemoteStarted() = true;
					}

/*
					if( HostName != "" )
					{
						WiFi.setHostname( (char *)HostName.c_str() );
//						WiFi.disconnect();
//						WiFi.reconnect();
//						ESP.restart();
//						Serial.println( HostName );
					}
*/
//					WiFi.setNoDelay( NoDelay() );

					if( T_AddressOutputPin::GetPinIsConnected() )
						T_AddressOutputPin::SetPinValue( IPAdressToString( WiFi.localIP()).c_str() );

					if( T_MACOutputPin::GetPinIsConnected() )
						T_MACOutputPin::SetPinValue( WiFi.macAddress().c_str() );

					if( T_BSSIDOutputPin::GetPinIsConnected() )
						T_BSSIDOutputPin::SetPinValue( WiFi.BSSIDstr().c_str() );

					if( T_GatewayIPOutputPin::GetPinIsConnected() )
						T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

					if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
						T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

				}
			}

//			for( int i = 0; i < C_COUNT_Sockets; ++i )
//				Sockets[ i ]->TryStartSocket();

//			WiFi.setHostname( (char *)HostName.c_str() );
//			Serial.print( "IS_STARTED : " );
//			Serial.println( FIsStarted().GetValue() );
		}

/*
	void DoCheckSignalStrength( void *_Data )
		{
			if( FIsStarted )
				if( SignalStrengthOutputPin.IsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					SignalStrengthOutputPin.Notify( &ASetrength );
				}

		}
*/
	public:
		inline ESP8266WiFiModule()
		{
			FAccessPointIndex() = C_COUNT_AccessPoints;
			FIsStarted() = false;
			FRemoteStarted() = false;
			FDNSUpdated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_SignalStrengthOutputPin
	> class ESP8266ModuleSignalStrengthOperation :
		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( SignalStrengthOutputPin )

	public:
		inline void TryStart() {}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( C_OWNER.FIsStarted() )
				if( T_SignalStrengthOutputPin::GetPinIsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					T_SignalStrengthOutputPin::SetPinValue( ASetrength );
				}

		}

	};
//---------------------------------------------------------------------------
	class ESP8266ModuleReconnectOperation
	{
	public:
		inline void TryStart() {}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			WiFi.reconnect();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

