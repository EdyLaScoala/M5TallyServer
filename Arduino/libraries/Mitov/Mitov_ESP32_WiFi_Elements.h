////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//  #define __ESP32_WIFI__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Channel,
		typename T_Config,
		typename T_Enabled,
		typename T_MacAddress,
		typename T_Password,
		typename T_SSID
	> class ESP32ModuleRemoteAccessPoint :
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

	public:
		void Connect()
		{
			if( ! Enabled() )
				return;

			if( SSID().GetValue() == "" )
				return;

			if( Config().Enabled() )
				C_OWNER.WiFi_config( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress(), Config().DNS1().GetIPAddress(), Config().DNS2().GetIPAddress() );

//			Serial.println( SSID );
//			Serial.println( APassword );

			Begin();

//			if( C_OWNER.HostName != "" )
//				WiFi.setHostname( (char *)C_OWNER.HostName.c_str() );

		}

		void Begin()
		{
//		    Serial.println( "TEST1" );
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

#ifdef __ESP32_WIFI__DEBUG__
			Serial.print( "WiFi.begin " );
			Serial.print( SSID().c_str() );
			Serial.print( " : " );
			Serial.println( APassword );

#endif // __ESP32_WIFI__DEBUG__

			C_OWNER.WiFi_disconnect();
			C_OWNER.WiFi_begin( (char *)SSID().c_str(), APassword, channel, bssid, false );

			if( C_OWNER.HostName().GetValue() != "" )
				C_OWNER.WiFi_setHostname( (char *)C_OWNER.HostName().c_str() );

			C_OWNER.WiFi_reconnect();
		}

	public:
		inline void IsEnabled( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID().GetValue() == "" ))
				return;

//			Serial.println( "IsEnabled" );

			AIsEnabled = true;
		}

		inline void TryConnect( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID().GetValue() == "" ))
				return;

//			Serial.println( "TryConnect" );
			AIsEnabled = true;
			Connect();
		}

		inline void AssignDNS()
		{
//				Serial.println( Config.DNS.IP.toString() );
			if( Config().DNS1().GetUInt32Value() == 0 )
			{
				IPAddress AIP = C_OWNER.WiFi_gatewayIP();
				C_OWNER.WiFi_disconnect();

//					delay( 10000 );

//					Serial.println( AIP.toString() );
				if( Config().Enabled() )
					C_OWNER.WiFi_config( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress(), AIP, Config().DNS2().GetIPAddress() );

				else
					C_OWNER.WiFi_config( ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 255, 255, 255, 0 ), AIP, ::IPAddress( 0, 0, 0, 0 ) );

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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

