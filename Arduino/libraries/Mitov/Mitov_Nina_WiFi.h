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
#include <SPI.h>

#include <Mitov_ESP32_WiFi_Elements.h>
#include <IPAddress.h>
//#include "IPAddress.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//  #define __ESP32_WIFI__DEBUG__
//  #define __TCP_CLIENT__DEBUG__

//---------------------------------------------------------------------------
#define __NINA_DEBUG_PRINT_FILE_LINE() { 						\
		Serial.print("[");Serial.print(__FILE__);		\
		Serial.print("::");Serial.print(__LINE__);Serial.print("]");\
}

#ifdef _NINA_HW_DEBUG_

#define __NINA_DEBUG_INFO(format, args...) { \
	char buf[250];	\
	sprintf(buf, format, args); \
	Serial.println(buf); \
}

#define __NINA_DEBUG_INFO1(x) { __NINA_DEBUG_PRINT_FILE_LINE() Serial.print("-I-");\
		Serial.println(x);    			\
}


#define __NINA_DEBUG_INFO2(x,y) { __NINA_DEBUG_PRINT_FILE_LINE() Serial.print("-I-");\
		Serial.print(x,16);Serial.print(",");Serial.println(y,16); \
}


#else
#define __NINA_DEBUG_INFO1(x) ;
#define __NINA_DEBUG_INFO2(x,y) ;
#define __NINA_DEBUG_INFO(format, args...) ;
#endif

#ifdef _NINA_HW_DEBUG_
	#define __NINA_DEBUG_WARN(args) { __NINA_DEBUG_PRINT_FILE_LINE()			\
			Serial.print("-W-"); Serial.println(args);	\
	}
#else
	#define __NINA_DEBUG_WARN(args) ;
#endif

#ifdef _NINA_HW_DEBUG_SPI_
	#define __NINA_DEBUG_DBG_PIN2 5
	#define __NINA_DEBUG_DBG_PIN 4

	#define __NINA_DEBUG_START()         digitalWrite(DBG_PIN2, HIGH);
	#define __NINA_DEBUG_END()           digitalWrite(DBG_PIN2, LOW);
	#define __NINA_DEBUG_SET_TRIGGER()   digitalWrite(DBG_PIN, HIGH);
	#define __NINA_DEBUG_RST_TRIGGER()   digitalWrite(DBG_PIN, LOW);

	#define __NINA_DEBUG_INIT_TRIGGER()  { pinMode(DBG_PIN, OUTPUT); \
							pinMode(DBG_PIN2, OUTPUT); \
							 __NINA_DEBUG_RST_TRIGGER() }

	#define __NINA_DEBUG_TOGGLE_TRIGGER()  { __NINA_DEBUG_SET_TRIGGER() \
							   delayMicroseconds(2);    \
									__NINA_DEBUG_RST_TRIGGER() }

#else
	#define __NINA_DEBUG_START()
	#define __NINA_DEBUG_END()
	#define __NINA_DEBUG_SET_TRIGGER()
	#define __NINA_DEBUG_RST_TRIGGER()
	#define __NINA_DEBUG_INIT_TRIGGER()
	#define __NINA_DEBUG_TOGGLE_TRIGGER()
#endif
//---------------------------------------------------------------------------
namespace Mitov
{
namespace NinaESP32
{
//---------------------------------------------------------------------------
	enum wl_tcp_state
	{
	  CLOSED      = 0,
	  LISTEN      = 1,
	  SYN_SENT    = 2,
	  SYN_RCVD    = 3,
	  ESTABLISHED = 4,
	  FIN_WAIT_1  = 5,
	  FIN_WAIT_2  = 6,
	  CLOSE_WAIT  = 7,
	  CLOSING     = 8,
	  LAST_ACK    = 9,
	  TIME_WAIT   = 10
	};

	enum wl_ping_result_t
	{
		WL_PING_DEST_UNREACHABLE = -1,
		WL_PING_TIMEOUT = -2,
		WL_PING_UNKNOWN_HOST = -3,
		WL_PING_ERROR = -4
	};

	enum wl_status_t
	{
		WL_NO_SHIELD = 255,
		WL_NO_MODULE = WL_NO_SHIELD,
		WL_IDLE_STATUS = 0,
		WL_NO_SSID_AVAIL,
		WL_SCAN_COMPLETED,
		WL_CONNECTED,
		WL_CONNECT_FAILED,
		WL_CONNECTION_LOST,
		WL_DISCONNECTED,
		WL_AP_LISTENING,
		WL_AP_CONNECTED,
		WL_AP_FAILED
	};

	enum wl_error_code_t
	{
		WL_FAILURE = -1,
		WL_SUCCESS = 1,
	};

	enum eProtMode
	{
		TCP_MODE,
		UDP_MODE,
		TLS_MODE,
		UDP_MULTICAST_MODE
	};

	// Encryption modes
	enum wl_enc_type // Values map to 802.11 encryption suites...
	{
		ENC_TYPE_WEP  = 5,
		ENC_TYPE_TKIP = 2,
		ENC_TYPE_CCMP = 4,
		// ... except these two, 7 and 8 are reserved in 802.11-2007
		ENC_TYPE_NONE = 7,
		ENC_TYPE_AUTO = 8,

		ENC_TYPE_UNKNOWN = 255
	};

	//#define	MAX_SOCK_NUM		4	/**< Maxmium number of socket  */
	const uint8_t NO_SOCKET_AVAIL     = 255;

		const uint8_t START_CMD   = 0xE0;
		const uint8_t END_CMD     = 0xEE;
		const uint8_t ERR_CMD	 = 0xEF;

		const uint8_t DUMMY_DATA  = 0xFF;

		const uint8_t REPLY_FLAG      = ( 1 << 7 );

		const uint8_t NO_LAST_PARAM   = 0;
		const uint8_t LAST_PARAM      = 1;

		const uint16_t TIMEOUT_CHAR    = 1000;

		// Maximum size of a SSID
		const uint8_t WL_SSID_MAX_LENGTH = 32;

		const uint8_t WIFI_SPI_ACK        = 1;
		const uint8_t WIFI_SPI_ERR        = 0xFF;

		// Size of a MAC-address or BSSID
		const uint8_t WL_IPV4_LENGTH = 4;

		// 5 secs of delay to have the connection established
		const uint16_t WL_DELAY_START_CONNECTION = 5000;

		//Maximum number of attempts to establish wifi connection
		const uint8_t WL_MAX_ATTEMPT_CONNECTION	= 10;

		// Maxmium number of socket
		const uint8_t WIFI_MAX_SOCK_NUM	= 10;

#ifdef __AVR__
		const uint8_t WIFI_SOCKET_BUFFER_SIZE = 64;
#else
		const uint16_t WIFI_SOCKET_BUFFER_SIZE = 1500;
#endif

		const uint8_t WL_FW_VER_LENGTH = 6;

		enum numParams
		{
			PARAM_NUMS_0,
			PARAM_NUMS_1,
			PARAM_NUMS_2,
			PARAM_NUMS_3,
			PARAM_NUMS_4,
			PARAM_NUMS_5,
			MAX_PARAM_NUMS,
			MAX_PARAMS = MAX_PARAM_NUMS - 1
		};

		enum
		{
			SET_NET_CMD 				= 0x10,
			SET_PASSPHRASE_CMD			= 0x11,
			SET_KEY_CMD					= 0x12,
		//	TEST_CMD					= 0x13,
			SET_IP_CONFIG_CMD			= 0x14,
			SET_DNS_CONFIG_CMD			= 0x15,
			SET_HOSTNAME_CMD			= 0x16,
			SET_POWER_MODE_CMD			= 0x17,
			SET_AP_NET_CMD				= 0x18,
			SET_AP_PASSPHRASE_CMD		= 0x19,
			SET_DEBUG_CMD				= 0x1A,
			GET_TEMPERATURE_CMD			= 0x1B,

			GET_CONN_STATUS_CMD			= 0x20,
			GET_IPADDR_CMD				= 0x21,
			GET_MACADDR_CMD				= 0x22,
			GET_CURR_SSID_CMD			= 0x23,
			GET_CURR_BSSID_CMD			= 0x24,
			GET_CURR_RSSI_CMD			= 0x25,
			GET_CURR_ENCT_CMD			= 0x26,
			SCAN_NETWORKS				= 0x27,
			START_SERVER_TCP_CMD		= 0x28,
			GET_STATE_TCP_CMD			= 0x29,
			DATA_SENT_TCP_CMD			= 0x2A,
			AVAIL_DATA_TCP_CMD			= 0x2B,
			GET_DATA_TCP_CMD			= 0x2C,
			START_CLIENT_TCP_CMD		= 0x2D,
			STOP_CLIENT_TCP_CMD			= 0x2E,
			GET_CLIENT_STATE_TCP_CMD	= 0x2F,
			DISCONNECT_CMD				= 0x30,
		//	GET_IDX_SSID_CMD			= 0x31,
			GET_IDX_RSSI_CMD			= 0x32,
			GET_IDX_ENCT_CMD			= 0x33,
			REQ_HOST_BY_NAME_CMD		= 0x34,
			GET_HOST_BY_NAME_CMD		= 0x35,
			START_SCAN_NETWORKS			= 0x36,
			GET_FW_VERSION_CMD			= 0x37,
		//	GET_TEST_CMD				= 0x38,
			SEND_DATA_UDP_CMD			= 0x39,
			GET_REMOTE_DATA_CMD			= 0x3A,
			GET_TIME_CMD				= 0x3B,
			GET_IDX_BSSID				= 0x3C,
			GET_IDX_CHANNEL_CMD			= 0x3D,
			PING_CMD					= 0x3E,
			GET_SOCKET_CMD				= 0x3F,

			// All command with DATA_FLAG 0x40 send a 16bit Len

			SEND_DATA_TCP_CMD		= 0x44,
			GET_DATABUF_TCP_CMD		= 0x45,
			INSERT_DATABUF_CMD		= 0x46,

			// regular format commands
			SET_PIN_MODE		= 0x50,
			SET_DIGITAL_WRITE	= 0x51,
			SET_ANALOG_WRITE	= 0x52
		};

}
//---------------------------------------------------------------------------
	class TArduinoAdafruitAirLiftWiFi_TCP_Impl
	{
	public:
		constexpr inline bool GetIsSecure() { return false; }

	protected:
		template <typename T_INSTANCE> inline void Client_connect( T_INSTANCE *AInstance, ::IPAddress &ip, uint16_t port )
		{
			AInstance->Client_TCP_connect( ip, port );
		}

		template <typename T_INSTANCE> inline void Client_connect( T_INSTANCE *AInstance, const char *host, uint16_t port )
		{
			AInstance->Client_TCP_connect( host, port );
		}
	};
//---------------------------------------------------------------------------
	class TArduinoAdafruitAirLiftWiFi_SSL_Impl
	{
	public:
		constexpr inline bool GetIsSecure() { return true; }

	protected:
		template <typename T_INSTANCE> inline void Client_connect( T_INSTANCE *AInstance, ::IPAddress &ip, uint16_t port )
		{
			AInstance->Client_SSL_connect( ip, port );
		}

		template <typename T_INSTANCE> inline void Client_connect( T_INSTANCE *AInstance, const char *host, uint16_t port )
		{
			AInstance->Client_SSL_connect( host, port );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_IMPLEMENTATION,
		typename T_FClientConnecting,
		typename T_FClientIsClosing,
		typename T_FClientIsConnected
	> class TArduinoAdafruitAirLiftWiFiBasicSocket :
		public T_IMPLEMENTATION,
		public T_FClientConnecting,
		public T_FClientIsClosing,
		public T_FClientIsConnected
	{
	protected:
		_V_PROP_( FClientConnecting )
		_V_PROP_( FClientIsClosing )
		_V_PROP_( FClientIsConnected )

	protected:
		uint8_t FClientSocketNo = NinaESP32::NO_SOCKET_AVAIL;

	protected:
		unsigned long FLastConnectTime;

	protected:
		uint8_t Client_status()
		{
			if( FClientSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				return NinaESP32::CLOSED;

			return C_OWNER.getClientState( FClientSocketNo );
		}

		inline void Client_flush()
		{
		}

		inline bool Client_available()
		{
			if( FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
				return C_OWNER.SocketBuffer_available( FClientSocketNo );

			return 0;
		}

		inline int Client_read()
		{
			if( ! Client_available() )
				return -1;

			uint8_t b;

			C_OWNER.SocketBuffer_read( FClientSocketNo, &b, sizeof(b));

			return b;
		}

		inline void Client_connect( ::IPAddress ip, uint16_t port )
		{
			T_IMPLEMENTATION::Client_connect( this, ip, port );
		}

		inline void Client_connect( const char *host, uint16_t port )
		{
			T_IMPLEMENTATION::Client_connect( this, host, port );
//			Serial.println( "Client_connect" );
		}

		inline bool Client_connected()
		{
			if( FClientSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				return false;

			if( Client_available())
				return true;

			uint8_t s = Client_status();

//			Serial.print( "Client_status: " );
//			Serial.println( s );
//			delay( 100 );

			bool result =  !(s == NinaESP32::LISTEN || s == NinaESP32::CLOSED || s == NinaESP32::FIN_WAIT_1 ||
								s == NinaESP32::FIN_WAIT_2 || s == NinaESP32::TIME_WAIT ||
								s == NinaESP32::SYN_SENT || s== NinaESP32::SYN_RCVD ||
								(s == NinaESP32::CLOSE_WAIT));

			if( ! result )
			{
				C_OWNER.SocketBuffer_close( FClientSocketNo );
				FClientSocketNo = NinaESP32::NO_SOCKET_AVAIL;
			}

			return result;
		}

		inline void Client_stop()
		{
			if( FClientSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				return;

			C_OWNER.stopClient( FClientSocketNo );

			FClientIsClosing() = true;
			FClientIsConnected() = false;
			FLastConnectTime = millis();

//			int count = 0;
			// wait maximum 5 secs for the connection to close
//			while( Client_status() != NinaESP32::CLOSED && ++count < 50 )
//				delay(100);

		}

	public:
		inline void Client_TCP_connect( ::IPAddress &ip, uint16_t port )
		{
			if( FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
				return;
//				Client_stop();

			FClientSocketNo = C_OWNER.getSocket();
			if( FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
			{
    			C_OWNER.startClient(uint32_t(ip), port, FClientSocketNo );

				FClientConnecting() = true;
				FLastConnectTime = millis();
			}
		}

		inline void Client_TCP_connect( const char *host, uint16_t port )
		{
//			Serial.println( "Client_connect" );
			::IPAddress remote_addr;
			if( C_OWNER.GetIPFromHostName( host, remote_addr ))
				Client_TCP_connect(remote_addr, port);

		}

		void Client_SSL_connect( ::IPAddress ip, uint16_t port )
		{
			if( FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
				return;
//				Client_stop();

			FClientSocketNo = C_OWNER.getSocket();
			if( FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
			{
				C_OWNER.startClient(uint32_t(ip), port, FClientSocketNo, NinaESP32::TLS_MODE );
				FClientConnecting() = true;
			}
		}

		void Client_SSL_connect( const char *host, uint16_t port )
		{
//			Serial.println( "Client_SSL_connect" );
//			Serial.println( host );
//			Serial.println( port );
			if( FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
				return;
//				Client_stop();

//			Serial.println( "Step 1" );
			FClientSocketNo = C_OWNER.getSocket();
			if( FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
			{
//				Serial.println( "Step 2" );
//				Serial.println( host );
//				Serial.println( port );
				C_OWNER.startClient(host, strlen(host), uint32_t(0), port, FClientSocketNo, NinaESP32::TLS_MODE );
				FClientConnecting() = true;
			}
		}

	public:
		inline TArduinoAdafruitAirLiftWiFiBasicSocket()
		{
//			FLastConnectTime = millis() + ReconnectDelay().GetValue();
//			FRunning() = false;
			FClientIsClosing() = false;
			FClientIsConnected() = false;
			FClientConnecting() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_IMPLEMENTATION,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FConnecting,
		typename T_FIsClosing,
		typename T_FIsConnected,
		typename T_Host,
		typename T_IPAddress,
		typename T_OutputPin,
		typename T_Port,
		typename T_ReconnectDelay
	> class TArduinoAdafruitAirLiftWiFiTCPClientSocket :
		public TArduinoAdafruitAirLiftWiFiBasicSocket<T_OWNER, C_OWNER, T_IMPLEMENTATION, T_FConnecting, T_FIsClosing, T_FIsConnected>,
//		public T_IMPLEMENTATION,
//		public Print,
		public T_ConnectedOutputPin,
		public T_Enabled,
//		public T_FIsClosing,
//		public T_FConnecting,
//		public T_FIsConnected,
		public T_Host,
		public T_IPAddress,
		public T_OutputPin,
		public T_Port,
		public T_ReconnectDelay
	{
		typedef TArduinoAdafruitAirLiftWiFiBasicSocket<T_OWNER, C_OWNER, T_IMPLEMENTATION, T_FConnecting, T_FIsClosing, T_FIsConnected>	inherited;
		typedef TArduinoAdafruitAirLiftWiFiTCPClientSocket T_SELF;

	public:
		_V_PIN_( ConnectedOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Host )
		_V_PROP_( IPAddress )
		_V_PROP_( Port )
		_V_PROP_( ReconnectDelay )

	protected:
//		_V_PROP_( FIsClosing )
//		_V_PROP_( FConnecting )
//		_V_PROP_( FIsConnected )

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			UpdateEnabled();
		}

		void UpdateEnabled()
		{
			if( IsEnabled() )
				StartSocket( true );

			else
				StopSocket();

		}

//	public:
//		using Print::setWriteError;

	public:
/*
		virtual size_t write( uint8_t b ) override
		{
			return write(&b, 1);
		}
*/
		size_t write( const uint8_t *buf, size_t size )
		{
			return C_OWNER.Socket_write( this, inherited::FClientSocketNo, buf, size );
		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
		}

		inline void BeginPacket()
		{
		}

		inline void EndPacket()
		{
		}

	public:
		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				StartSocket();
		}

		void ForceStart() // For direct socket access components
		{
			Enabled() = true;
			StartSocket( true );
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

/*
	public:
		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				if( Enabled )
					StartSocket();

		}
*/
	public:
		inline void FlushInputPin_o_Receive( void *_Data )
		{
			inherited::Client_flush();
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
			return Enabled() && C_OWNER.Enabled() && inherited::FClientIsConnected() && C_OWNER.GetIsStarted();
		}

		inline bool GetReadyToConnect()
		{
			return C_OWNER.Enabled() && C_OWNER.GetIsStarted();
		}

		inline CallbackPrint<T_SELF> GetPrint()
		{
			return CallbackPrint<T_SELF>( this, (typename CallbackPrint<T_SELF>::TCallback) &T_SELF::write );
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			inherited::Client_flush();
			inherited::Client_stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
			Enabled() = false;
//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

	public:
		void StartSocket( bool AForceStart )
		{
			if( ! C_OWNER.GetIsStarted() )
				return;

//#ifdef __TCP_CLIENT__DEBUG__
//			Serial.println( "StartSocket1" );
//#endif
			if( inherited::FClientIsClosing() )
				return;

			if( inherited::FClientIsConnected() )
				return;

//			Serial.println( "StartSocket2" );
//			delay( 2000 );
//			Serial.println( "StartSocket1" );
//			Serial.println( FConnecting().GetValue() );
//			Serial.println( Enabled().GetValue() );
//			Serial.println( FClientSocketNo );
//			delay( 200 );

			if( ! Enabled().GetValue() )
				return;

            if( inherited::FClientConnecting() )
			{
				if( inherited::Client_connected() )
				{
//					Serial.println( "StartSocket.Client_connected" );
//					delay( 2000 );

					inherited::FClientIsConnected() = true;
					inherited::FClientConnecting() = false;
				}

				else
				{
					if( millis() - inherited::FLastConnectTime < 10000 )
						return;

					if( inherited::FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
					{
						C_OWNER.SocketBuffer_close( inherited::FClientSocketNo );
						inherited::FClientSocketNo = NinaESP32::NO_SOCKET_AVAIL;
					}

//					Serial.println( "FConnecting() = false" );
					inherited::FClientConnecting() = false;
					inherited::FLastConnectTime = millis();
				}

				return;
			}

//			Serial.println( "StartSocket2" );

			if( ! AForceStart )
				if( millis() - inherited::FLastConnectTime < ReconnectDelay().GetValue() )
					return;

//			Serial.println( "StartSocket2" );
//			delay( 2000 );

#ifdef __TCP_CLIENT__DEBUG__
			Serial.print( "StartSocket " );
			Serial.println( Host().GetValue() );
#endif
			if( Host().GetValue().length() )
				inherited::Client_connect( Host().c_str(), Port() );
//				FIsConnected() = Client_connect( Host().c_str(), Port() );

			else if( IPAddress().GetUInt32Value() != 0 )
			{
//				IPAddress.printTo( Serial );
				inherited::Client_connect( IPAddress().GetIPAddress(), Port() );
//				FIsConnected() = Client_connect( IPAddress().GetIPAddress(), Port() );
			}

#ifdef __TCP_CLIENT__DEBUG__
			Serial.println( inherited::FClientIsConnected().GetValue() );
#endif
		}

		void StartSocket()
		{
			StartSocket( false );
		}

		void StopSocket()
		{
			inherited::FClientConnecting() = false;
//			Serial.println( "StopSocket.FConnecting() = false" );
			if( ! inherited::FClientIsConnected() )
				return;

#ifdef __TCP_CLIENT__DEBUG__
			Serial.println( "StopSocket" );
#endif
			inherited::FClientIsConnected() = false;

			inherited::Client_flush();
			inherited::Client_stop();
			Enabled() = false;
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( inherited::FClientIsClosing() )
			{
				if( ! inherited::Client_connected() )
					inherited::FClientIsClosing() = false;

				else
				{
					if( millis() - inherited::FLastConnectTime < 5000 )
						return;

					C_OWNER.SocketBuffer_close( inherited::FClientSocketNo );
					inherited::FClientSocketNo = NinaESP32::NO_SOCKET_AVAIL;
				}

			}

			bool AIsConnected = inherited::Client_connected(); // Has to be here for ESP32 WiFi Nina chips compatibility!

			StartSocket();
			for( int i = 0; i < 20; ++i )
			{
				if( ! inherited::Client_available() )
					break;

				unsigned char AByte = inherited::Client_read();
				T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
			}

		    T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			if( ! AIsConnected )
			{
				while( inherited::Client_available() )
				{
					unsigned char AByte = inherited::Client_read();
					T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
				}

//				FClient.stop(); // Do we need this?
				T_ConnectedOutputPin::SetPinValue( false, true );

				if( inherited::FClientIsConnected() )
				{
#ifdef __TCP_CLIENT__DEBUG__
					Serial.println( "DISCONNECT" );
#endif
					if( C_OWNER.Enabled() )
						Enabled() = false;

#ifdef __TCP_CLIENT__DEBUG__
					Serial.print( "Enabled() = " );
					Serial.println( Enabled() );
#endif
					inherited::Client_flush();
					inherited::Client_stop();

					inherited::FClientIsConnected() = false;
					inherited::FClientConnecting() = false;
				}
			}
		}

	public:
		inline TArduinoAdafruitAirLiftWiFiTCPClientSocket()
		{
			inherited::FLastConnectTime = millis() + ReconnectDelay().GetValue();
//			FRunning() = false;
//			FIsClosing() = false;

//			FIsConnected() = false;
//			FResolved() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FClientConnecting,
		typename T_FClientIsClosing,
		typename T_FClientIsConnected,
		typename T_FConnecting,
//		typename T_FIsClosing,
		typename T_FIsConnected,
		typename T_OpenedOutputPin,
		typename T_OutputPin,
		typename T_Port,
		typename T_RetryOpenDelay
	> class TArduinoAdafruitAirLiftWiFiTCPServerSocket :
		public TArduinoAdafruitAirLiftWiFiBasicSocket<T_OWNER, C_OWNER, TArduinoAdafruitAirLiftWiFi_TCP_Impl, T_FClientConnecting, T_FClientIsClosing, T_FClientIsConnected>,
//		public Print,
		public T_ConnectedOutputPin,
		public T_Enabled,
		public T_FConnecting,
//		public T_FIsClosing,
		public T_FIsConnected,
		public T_OpenedOutputPin,
		public T_OutputPin,
		public T_Port,
		public T_RetryOpenDelay
	{
		typedef TArduinoAdafruitAirLiftWiFiBasicSocket<T_OWNER, C_OWNER, TArduinoAdafruitAirLiftWiFi_TCP_Impl, T_FClientConnecting, T_FClientIsClosing, T_FClientIsConnected>	inherited;
		typedef TArduinoAdafruitAirLiftWiFiTCPServerSocket T_SELF;

	public:
		_V_PIN_( ConnectedOutputPin )
		_V_PIN_( OpenedOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Port )
		_V_PROP_( RetryOpenDelay )

	protected:
		_V_PROP_( FConnecting )
//		_V_PROP_( FIsClosing )
		_V_PROP_( FIsConnected )

		uint8_t FServerSocketNo = NinaESP32::NO_SOCKET_AVAIL;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			UpdateEnabled();
		}

		void UpdateEnabled()
		{
			if( IsEnabled() )
				StartSocket( true );

			else
				StopSocket();

		}

//	public:
//		using Print::setWriteError;

	protected:
		void Server_begin()
		{
			FServerSocketNo = C_OWNER.getSocket();
			if( FServerSocketNo != NinaESP32::NO_SOCKET_AVAIL )
			{
				C_OWNER.startServer( Port().GetValue(), FServerSocketNo );
				FConnecting() = true;
				inherited::FLastConnectTime = millis();
			}
		}

		void Server_end()
		{
			if( FServerSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				return;

			C_OWNER.SocketBuffer_close( FServerSocketNo );
			FServerSocketNo = NinaESP32::NO_SOCKET_AVAIL;
		}

		uint8_t Server_status()
		{
			if( FServerSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				return NinaESP32::CLOSED;

			return C_OWNER.getServerState( FServerSocketNo );
		}

		inline bool Server_opened()
		{
			if( FServerSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				return false;

			uint8_t s = Server_status();
//			Serial.print( "Server_status: " );
//			Serial.println( s );
//			delay( 100 );

/*
			bool result =  !(s == NinaESP32::LISTEN || s == NinaESP32::CLOSED || s == NinaESP32::FIN_WAIT_1 ||
								s == NinaESP32::FIN_WAIT_2 || s == NinaESP32::TIME_WAIT ||
								s == NinaESP32::SYN_SENT || s== NinaESP32::SYN_RCVD ||
								(s == NinaESP32::CLOSE_WAIT));
*/
			bool result =  !( s == NinaESP32::CLOSED || s == NinaESP32::CLOSE_WAIT );

			if( ! result )
			{
				C_OWNER.SocketBuffer_close( FServerSocketNo );
				FServerSocketNo = NinaESP32::NO_SOCKET_AVAIL;
			}

			return result;
		}

		uint8_t Server_accept()
		{
			if( FServerSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				return NinaESP32::NO_SOCKET_AVAIL;

			return C_OWNER.availServer( FServerSocketNo );
		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
		}

		inline void BeginPacket()
		{
		}

		inline void EndPacket()
		{
		}

	public:
//		inline bool GetIsSecure() { return false; }

		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				StartSocket();
		}

		void ForceStart() // For direct socket access components
		{
			Enabled() = true;
			TryStartSocket();
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			inherited::Client_flush();
			inherited::Client_stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

		inline void FlushInputPin_o_Receive( void *_Data )
		{
			inherited::Client_flush();
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	protected:
		void StartSocket( bool AForceStart )
		{
//			Serial.println( "StartSockect 1" );
			if( ! C_OWNER.GetIsStarted() )
			{
				inherited::FLastConnectTime = millis() + RetryOpenDelay().GetValue();
				return;
			}

//			if( FServerSocketNo != NinaESP32::NO_SOCKET_AVAIL )
//				return;

//			Serial.println( "StartSockect 2" );
			if( FIsConnected() )
				return;

			if( ! Enabled().GetValue() )
				return;

            if( FConnecting() )
			{
//				Serial.println( "Server.FConnecting" );
				if( Server_opened() )
				{
//					Serial.println( "StartSocket.Client_connected" );

					FIsConnected() = true;
					FConnecting() = false;
				}

				else
				{
					if( millis() - inherited::FLastConnectTime < 10000 )
						return;

					if( FServerSocketNo != NinaESP32::NO_SOCKET_AVAIL )
					{
						C_OWNER.SocketBuffer_close( FServerSocketNo );
						FServerSocketNo = NinaESP32::NO_SOCKET_AVAIL;
					}

//					Serial.println( "FConnecting() = false" );
					FConnecting() = false;
					inherited::FLastConnectTime = millis();
				}

				return;
			}

//			Serial.println( "StartSockect 1" );
//			delay( 100 );

			if( ! AForceStart )
				if( millis() - inherited::FLastConnectTime < RetryOpenDelay().GetValue() )
					return;

//			Serial.println( "StartSockect" );
/*
			if( FServerSocketNo == NinaESP32::NO_SOCKET_AVAIL )
			{
//				if( ! *FServer )
//				{
//					Console.println( "Try Restart" );
					Server_begin();
//				}

				return;
			}
*/
#ifdef __TCP_SERVER__DEBUG__
			Serial.println( "TCPServer - StartSockect" );
			Serial.println( Port() );
#endif //__TCP_SERVER__DEBUG__
//			FServer = new T_SERVER( Port() );
			Server_begin();
//			UpdateNoDelay();
//			FIsConnected() = true;
//			Serial.println( "Start Server Sockect" );
//			Serial.println( inherited::Port );
		}

	public:
/*
		virtual size_t write( uint8_t b ) override
		{
			return write(&b, 1);
		}
*/
		size_t write( const uint8_t *buf, size_t size )
		{
			if( inherited::FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
				return C_OWNER.Socket_write( this, inherited::FClientSocketNo, buf, size );

			return C_OWNER.Socket_write( this, FServerSocketNo, buf, size );
		}

	public:
		inline void SystemLoopBegin()
		{
			StartSocket( false );
			if( FServerSocketNo != NinaESP32::NO_SOCKET_AVAIL )
			{
				if( T_OpenedOutputPin::GetPinIsConnected() )
					T_OpenedOutputPin::SetPinValue( FIsConnected(), true );

				if( inherited::FClientSocketNo == NinaESP32::NO_SOCKET_AVAIL )
					if( FIsConnected() )
					{
#ifdef __TCP_SERVER__DEBUG__
//						Serial.println( "TRY CLIENT" );
//						Serial.println( Port() );
#endif // __TCP_SERVER__DEBUG__
//						FClient = FServer->available();
//						FClient = Server_accept();
						inherited::FClientSocketNo = Server_accept();

#ifdef __TCP_SERVER__DEBUG__
//						Serial.println( inherited::FClientSocketNo );
//						delay( 100 );

						if( inherited::FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
							Serial.println( "CLIENT" );
#endif // __TCP_SERVER__DEBUG__

//						Serial.println( "TRY CLIENT OUT" );
					}

/*
				if( inherited::FClient )
 					if (! inherited::FClient.connected())
					{
						Serial.println( "STOP" );
						inherited::FClient.stop(); // Do we need this?
					}
*/
//				if( FIsConnected() )
				if( inherited::FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL )
				{
//					Serial.println( "CLIENT" );
					if( inherited::Client_available() )
					{
						unsigned char AByte = inherited::Client_read();
//				Serial.println( "RECEIVED" );
						T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//						OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
					}

 					if( ! inherited::Client_connected() )
					{
						while( inherited::Client_available() )
						{
							unsigned char AByte = inherited::Client_read();
#ifdef __TCP_SERVER__DEBUG__
							Serial.println( "RECEIVED" );
#endif // __TCP_SERVER__DEBUG__
							T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
						}

//						Serial.println( "STOP!!!" );
						inherited::Client_stop(); // This is needed for the Arduino EthernetShield!
						T_ConnectedOutputPin::SetPinValue( false, true );
					}
				}
			}

			else
				T_OpenedOutputPin::SetPinValue( false, true );

//			Serial.println( inherited::FClient.connected() );

			bool AIsConnected = inherited::Client_connected();

//			Serial.println( FClient.status() );

#ifdef __TCP_SERVER__DEBUG__
			if( inherited::FClientIsConnected() != AIsConnected )
			{
				Serial.print( "inherited::FClientIsConnected() = " );
				Serial.println( AIsConnected );
			}

#endif // __TCP_SERVER__DEBUG__

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			inherited::FClientIsConnected() = AIsConnected;

//			if( ! AIsConnected )
//				if( C_OWNER.Enabled )
//					inherited::Enabled = false;

//			Serial.println( "SYSTEM_LOOP" );
//			Serial.println( "SYSTEM_LOOP_OUT" );
		}

	public:
		inline bool IsConnected() { return inherited::FClientSocketNo != NinaESP32::NO_SOCKET_AVAIL; }

		inline bool CanSend()
		{
/*
			Serial.print( Enabled() ); Serial.print( " " );
			Serial.print( C_OWNER.Enabled() ); Serial.print( " " );
			Serial.print( FIsConnected().GetValue() ); Serial.print( " " );
			Serial.println( C_OWNER.GetIsStarted() );

			Serial.print( "IS_STARTED : " );
			Serial.println( C_OWNER.IsStarted().GetValue() );
*/
//			return inherited::Enabled && C_OWNER.Enabled && inherited::FClient;
//			return Enabled() && C_OWNER.Enabled() && C_OWNER.GetIsStarted();
			return Enabled() && C_OWNER.Enabled() && inherited::FClientIsConnected() && C_OWNER.GetIsStarted();
		}

		inline bool GetReadyToConnect()
		{
			return C_OWNER.Enabled() && C_OWNER.GetIsStarted();
		}

		inline void Disconnect()
		{
#ifdef __TCP_SERVER__DEBUG__
			Serial.println( "TCPServer - Disconnect" );
#endif // __TCP_SERVER__DEBUG__
			inherited::Client_stop();
		}

		inline void StopSocket()
		{
#ifdef __TCP_SERVER__DEBUG__
			Serial.println( "TCPServer - StopSocket" );
#endif // __TCP_SERVER__DEBUG__
			inherited::Client_stop();
			inherited::FClientIsConnected() = false;

			Server_end();
		}

	public:
		inline CallbackPrint<T_SELF> GetPrint()
		{
			return CallbackPrint<T_SELF>( this, (typename CallbackPrint<T_SELF>::TCallback) &T_SELF::write );
		}

	public:
		inline TArduinoAdafruitAirLiftWiFiTCPServerSocket()
		{
			inherited::FLastConnectTime = millis() + RetryOpenDelay().GetValue();

			FConnecting() = false;
//			FIsClosing() = false;
			FIsConnected() = false;
//			FIsConnected() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FIsConnected,
		typename T_OutputPin,
		typename T_Port,
		typename T_RemoteHost,
		typename T_RemoteIPAddress,
		typename T_RemoteIPOutputPin,
		typename T_RemotePort,
		typename T_RemotePortOutputPin
	> class TArduinoAdafruitAirLiftWiFiUDPSocket :
//		public Print,
		public T_Enabled,
		public T_FIsConnected,
		public T_OutputPin,
		public T_Port,
		public T_RemoteHost,
		public T_RemoteIPAddress,
		public T_RemoteIPOutputPin,
		public T_RemotePort,
		public T_RemotePortOutputPin
	{
		typedef TArduinoAdafruitAirLiftWiFiUDPSocket T_SELF;

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( RemoteIPOutputPin )
		_V_PIN_( RemotePortOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Port )
		_V_PROP_( RemoteHost )
		_V_PROP_( RemoteIPAddress )
		_V_PROP_( RemotePort )

	protected:
		_V_PROP_( FIsConnected )

	protected:
		uint8_t FUDPSocketNo = NinaESP32::NO_SOCKET_AVAIL;

//		uint16_t _port; // local port to listen on
		int _parsed;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			UpdateEnabled();
		}

		void UpdateEnabled()
		{
			if( IsEnabled() )
				StartSocket();

			else
				StopSocket();

		}

	protected:
		bool Socket_begin(uint16_t port)	// initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
		{
			if( FUDPSocketNo != NinaESP32::NO_SOCKET_AVAIL )
				Socket_stop();

//			if( port == 0 )
//				return true;

			FUDPSocketNo = C_OWNER.getSocket();
			if( FUDPSocketNo != NinaESP32::NO_SOCKET_AVAIL )
			{
				C_OWNER.startServer( port, FUDPSocketNo, NinaESP32::UDP_MODE );
//				_port = port;

//				Serial.print( "port: " );
//				Serial.println( port );
//				Serial.println( FUDPSocketNo );

				_parsed = 0;
				return true;
			}

			return false;
		}

		// Finish with the UDP socket
		void Socket_stop()
		{
			if ( FUDPSocketNo == NinaESP32::NO_SOCKET_AVAIL)
				return;

			C_OWNER.stopClient( FUDPSocketNo );

			C_OWNER.SocketBuffer_close( FUDPSocketNo );
			FUDPSocketNo = NinaESP32::NO_SOCKET_AVAIL;
		}

/*
	size_t Socket_write( const uint8_t *buf, size_t size )
		{
			if( FUDPSocketNo == NinaESP32::NO_SOCKET_AVAIL )
			{
				setWriteError();
				return 0;
			}

			if (size==0)
			{
				setWriteError();
				return 0;
			}

			size_t written = C_OWNER.sendData( FUDPSocketNo, buf, size );
			if ( ! written )
			{
				setWriteError();
				return 0;
			}
/ *
			if (! C_OWNER.checkDataSent( FClientSocketNo ))
			{
				setWriteError();
				return 0;
			}
* /
			return written;
		}
*/

		// Start building up a packet to send to the remote host specific in ip and port
		// Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
		bool Socket_beginPacket(IPAddress ip, uint16_t port)
		{
//			Serial.print( "Socket_beginPacket IP: " );
//			Serial.print( ip );
//			Serial.print( " : " );
//			Serial.println( port );

			if ( FUDPSocketNo == NinaESP32::NO_SOCKET_AVAIL )
				FUDPSocketNo = C_OWNER.getSocket();

			if ( FUDPSocketNo != NinaESP32::NO_SOCKET_AVAIL)
			{
				C_OWNER.startClient(uint32_t(ip), port, FUDPSocketNo, NinaESP32::UDP_MODE );
				return true;
			}

			return false;
		}

		// Start building up a packet to send to the remote host specific in host and port
		// Returns 1 if successful, 0 if there was a problem resolving the hostname or port
		bool Socket_beginPacket(const char *host, uint16_t port)
		{
			// Look up the host first
			bool ret = false;
			IPAddress remote_addr;
			if ( C_OWNER.GetIPFromHostName(host, remote_addr))
				return Socket_beginPacket(remote_addr, port);

			return ret;
		}

		// Finish off this packet and send it
		// Returns 1 if the packet was sent successfully, 0 if there was an error
		int Socket_endPacket()
		{
			return C_OWNER.sendUdpData( FUDPSocketNo );
		}

		// Return the port of the host who sent the current incoming packet
		uint16_t Socket_remotePort()
		{
			uint8_t _remoteIp[4] = {0};
			uint8_t _remotePort[2] = {0};

			C_OWNER.getRemoteData( FUDPSocketNo, _remoteIp, _remotePort );
			uint16_t port = (_remotePort[0]<<8)+_remotePort[1];
			return port;
		}

		// Return the IP address of the host who sent the current incoming packet
		IPAddress Socket_remoteIP()
		{
			uint8_t _remoteIp[4] = {0};
			uint8_t _remotePort[2] = {0};

			C_OWNER.getRemoteData( FUDPSocketNo, _remoteIp, _remotePort );
			IPAddress ip(_remoteIp);
			return ip;
		}

		// Read up to len bytes from the current packet and place them into buffer
		// Returns the number of bytes read, or 0 if none are available
		int Socket_read(unsigned char* buffer, size_t len)
		{
			if (_parsed < 1)
				return 0;

			int result = C_OWNER.SocketBuffer_read( FUDPSocketNo, buffer, len );

			if (result > 0)
				_parsed -= result;

			return result;
		}

		// Read up to len characters from the current packet and place them into buffer
		// Returns the number of characters read, or 0 if none are available
		int Socket_read(char* buffer, size_t len)
		{
			return Socket_read((unsigned char*)buffer, len);
		}

		// Start processing the next available incoming packet
		// Returns the size of the packet in bytes, or 0 if no packets are available
		int Socket_parsePacket()
		{
			while( _parsed -- )
			{
				// discard previously parsed packet data
				uint8_t b;

				C_OWNER.SocketBuffer_read( FUDPSocketNo, &b, sizeof(b));
			}

			_parsed = C_OWNER.availData( FUDPSocketNo );
/*
			if( _parsed )
			{
				Serial.print( "_parsed = " );
				Serial.println( _parsed );
			}
*/
			return _parsed;
		}

//	public:
//		using Print::setWriteError;

	public:
/*
		virtual size_t write( uint8_t b ) override
		{
			return write(&b, 1);
		}
*/
		size_t write( const uint8_t *buf, size_t size )
		{
//			Serial.print( "write: " );
//			Serial.println( size );
			C_OWNER.insertDataBuf( FUDPSocketNo, buf, size );
			return size;
//			return C_OWNER.Socket_write( this, FUDPSocketNo, buf, size );
		}

	public:
		inline bool GetIsSecure() { return false; }

		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				StartSocket();
		}

		void ForceStart() // For direct socket access components
		{
			Enabled() = true;
			TryStartSocket();
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();
//				if( Enabled )
//					StartSocket();

		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

		inline bool GetReadyToConnect()
		{
			return C_OWNER.Enabled() && C_OWNER.GetIsStarted();
		}

	protected:
		void StartSocket()
		{
//			Serial.println( "StartSocket" );
			if( FIsConnected() )
				return;

//			Serial.println( "StartSocket 1" );
			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "TEST11111" );
/*
			if( FHostName != "" )
			{
//				Serial.println( "TEST1" );
				FResolved = C_OWNER.GetIPFromHostName( FHostName, RemoteIPAddress );
			}
*/
//			Serial.print( "StartSocket: " );
//			Serial.println( inherited::Port );
			FIsConnected() = Socket_begin( Port() );
		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
			RemotePort() = APort;
		}

		inline void BeginPacket()
		{
//			if( ! FIsConnected() )
//				return;

//			Serial.print( "BeginPacket: " );
//			Serial.print( RemoteHost().GetValue() );
//			Serial.print( " " );
//			Serial.print( RemoteIPAddress().AsText() );
//			Serial.print( " " );
//			Serial.println( RemotePort().GetValue() );

			if( RemoteHost().GetValue().length() )
				Socket_beginPacket( RemoteHost().c_str(), RemotePort() );

			else if( RemoteIPAddress().GetUInt32Value() != 0 )
				Socket_beginPacket( RemoteIPAddress().GetIPAddress(), RemotePort() );

////  FSocket.beginPacket( RemoteIPAddress, 8888 );
//  FSocket.println( "Hello1" );
////  FSocket.endPacket();
		}

		inline void EndPacket()
		{
//			if( ! FIsConnected() )
//				return;

//			Serial.println( "EndPacket" );
//			FSocket.println( "Hello" );
			Socket_endPacket();
//			delay( 1000 );
		}

		inline void StopSocket()
		{
			FIsConnected() = false;

//			Serial.println( "StopSocket" );
			Socket_stop();
		}

		inline CallbackPrint<T_SELF> GetPrint()
		{
			return CallbackPrint<T_SELF>( this, (typename CallbackPrint<T_SELF>::TCallback) &T_SELF::write );
		}

	public:
		inline void SystemLoopBegin()
		{
			StartSocket();
			if( ! FIsConnected().GetValue() )
				return;

//			Serial.print( "Socket_parsePacket: " );
//			Serial.println( FUDPSocketNo );
//			delay( 100 );

			int APacketSize = Socket_parsePacket();
			if( APacketSize )
			{
//				Serial.print( "APacketSize = " );
//				Serial.println( APacketSize );

				uint8_t *Adata = new uint8_t[ APacketSize ];
				Socket_read( Adata, APacketSize );

				T_OutputPin::SendPinValue( Mitov::TDataBlock( APacketSize, Adata ));
//				OutputPin.SendValue( Mitov::TDataBlock( APacketSize, Adata ));
				T_RemoteIPOutputPin::SetPinValue( IPAdressToString( Socket_remoteIP() ));
//				RemoteIPOutputPin.SendValue( IPAdressToString( FSocket.remoteIP() ));
				T_RemotePortOutputPin::SetPinValue( Socket_remotePort() );
//				RemotePortOutputPin.SendValue( FSocket.remotePort() );

				delete [] Adata;
			}
/*
			if ( FSocket.available() )
			{
				unsigned char AByte = FSocket.read();
				inherited::OutputPin.Notify( &AByte );
			}
*/
		}

	public:
		inline TArduinoAdafruitAirLiftWiFiUDPSocket()
		{
			FIsConnected() = false;
//			FResolved() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		uint8_t C_BusyInputPin,
		typename T_ChipSelectOutputPin,
		int8_t C_GPIO0OutputPin,
		typename T_ResetOutputPin,
		typename T_ResetValue,
		typename T_SPISpeed
	> class AirLiftSPIInterface :
		public T_ChipSelectOutputPin,
		public T_ResetOutputPin,
		public T_ResetValue,
		public T_SPISpeed
	{
	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( ResetValue )
		_V_PROP_( SPISpeed )

	public:
		void Begin()
		{
			pinMode( C_BusyInputPin, INPUT );

			if( C_GPIO0OutputPin >= 0 )
			{
				pinMode( C_GPIO0OutputPin, OUTPUT );
				digitalWrite( C_GPIO0OutputPin, HIGH );
			}

			T_ChipSelectOutputPin::SetPinValue( true );
			if( ResetOutputPin().GetPinIsConnected() )
			{
				T_ResetOutputPin::SetPinValue( ResetValue().GetValue() );
				delay( 10 );
				T_ResetOutputPin::SetPinValue( ! ResetValue().GetValue() );
				delay( 750 );
			}

			if( C_GPIO0OutputPin >= 0 )
				pinMode( C_GPIO0OutputPin, INPUT_PULLUP);

		}

		void End()
		{
			T_ResetOutputPin::SetPinValue( ResetValue().GetValue() );
		}

		inline void SPI_BeginTransfer()
		{
			C_SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			T_ChipSelectOutputPin::SetPinValue( false );
//			digitalWrite( WIFININA_SLAVESELECT, LOW );

			// wait for up to 5 ms for the NINA to indicate it is not ready for transfer
			// the timeout is only needed for the case when the shield or module is not present
			for (unsigned long start = millis(); (digitalRead( C_BusyInputPin ) != HIGH) && (millis() - start) < 5;);
		}

		inline void SPI_EndTransfer()
		{
			T_ChipSelectOutputPin::SetPinValue( true );
//			digitalWrite( WIFININA_SLAVESELECT, HIGH );
			C_SPI.endTransaction();
		}

		inline char SPI_Transfer(volatile char data) // { return SpiDrv::SpiDrv_spiTransfer( data ); }
		{
			return C_SPI.transfer(data);
		}

		inline bool isSlaveReady()
		{
			return ( digitalRead( C_BusyInputPin ) == LOW );
		}

		inline bool SpiDrv_available()
		{
			if ( C_GPIO0OutputPin >= 0 )
				return (digitalRead( C_GPIO0OutputPin ) == HIGH);

			return true;
		}

	};
//---------------------------------------------------------------------------
	class AirLiftM4ModuleInterface
	{
		static const uint8_t WIFININA_SLAVESELECT = 36;
		static const uint8_t WIFININA_SLAVEREADY = 37;
		static const uint8_t WIFININA_SLAVERESET = 38;
		static const uint8_t WIFININA_SLAVEGPIO0 = 35;

	public:
		void Begin()
		{
			pinMode(WIFININA_SLAVESELECT, OUTPUT);
			pinMode(WIFININA_SLAVEREADY, INPUT);
			pinMode(WIFININA_SLAVERESET, OUTPUT);

			pinMode(WIFININA_SLAVEGPIO0, OUTPUT);
			digitalWrite(WIFININA_SLAVEGPIO0, HIGH);

			digitalWrite(WIFININA_SLAVESELECT, HIGH);
			digitalWrite(WIFININA_SLAVERESET, LOW );
			delay(10);
			digitalWrite(WIFININA_SLAVERESET, HIGH );
			delay(750);

			pinMode(WIFININA_SLAVEGPIO0, INPUT_PULLUP);
		}

		inline void End()
		{
			digitalWrite( WIFININA_SLAVERESET, LOW );
			pinMode(WIFININA_SLAVESELECT, INPUT);
		}

		inline void SPI_BeginTransfer()
		{
			SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			digitalWrite( WIFININA_SLAVESELECT, LOW );

			// wait for up to 5 ms for the NINA to indicate it is not ready for transfer
			// the timeout is only needed for the case when the shield or module is not present
			for (unsigned long start = millis(); (digitalRead(WIFININA_SLAVEREADY) != HIGH) && (millis() - start) < 5;);
		}

		inline void SPI_EndTransfer()
		{
			digitalWrite( WIFININA_SLAVESELECT, HIGH );
			SPI.endTransaction();
		}

		inline char SPI_Transfer(volatile char data) // { return SpiDrv::SpiDrv_spiTransfer( data ); }
		{
			return SPI.transfer(data);
		}

		inline bool isSlaveReady()
		{
			return digitalRead(WIFININA_SLAVEREADY) == LOW;
		}

		inline bool SpiDrv_available()
		{
			return ( digitalRead( WIFININA_SLAVEGPIO0 ) == HIGH );
		}

	};
//---------------------------------------------------------------------------
	template <
#ifdef VISUINO_MKR_ZERO
		SPIClassSAMD &C_SPI,
#else
		SPIClass &C_SPI,
#endif
		uint8_t C_BUSY,
		uint8_t C_CHIP_SELECT,
		uint8_t C_GPIO0,
		uint8_t C_RESET,
		bool C_INVERT_RESET,
		typename T_UseGPIO0
	> class AirLiftShieldInterface :
		public T_UseGPIO0
	{
	public:
		_V_PROP_( UseGPIO0 )

//		static const uint8_t WIFININA_SLAVESELECT = C_CHIP_SELECT;
//		static const uint8_t WIFININA_SLAVEREADY = C_BUSY;
//		static const uint8_t WIFININA_SLAVERESET = C_RESET;
//		static const uint8_t WIFININA_SLAVEGPIO0 = C_GPIO0;

	public:
		void Begin()
		{
			pinMode( C_CHIP_SELECT, OUTPUT );
			pinMode( C_BUSY, INPUT );
			pinMode( C_RESET, OUTPUT );

			if( UseGPIO0() )
			{
				pinMode( C_GPIO0, OUTPUT );
				digitalWrite( C_GPIO0, HIGH );
			}

			digitalWrite( C_CHIP_SELECT, HIGH );
			digitalWrite( C_RESET, C_INVERT_RESET ? HIGH : LOW );
			delay(10);
			digitalWrite( C_RESET, C_INVERT_RESET ? LOW : HIGH );
			delay(750);

			if( UseGPIO0() )
				pinMode( C_GPIO0, INPUT_PULLUP );

		}

		inline void End()
		{
			digitalWrite( C_RESET, C_INVERT_RESET ? HIGH : LOW );
			pinMode( C_CHIP_SELECT, INPUT );
		}

		inline void SPI_BeginTransfer()
		{
			C_SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
			digitalWrite( C_CHIP_SELECT, LOW );

			// wait for up to 5 ms for the NINA to indicate it is not ready for transfer
			// the timeout is only needed for the case when the shield or module is not present
			for (unsigned long start = millis(); (digitalRead(C_BUSY) != HIGH) && (millis() - start) < 5;);
		}

		inline void SPI_EndTransfer()
		{
			digitalWrite( C_CHIP_SELECT, HIGH );
			C_SPI.endTransaction();
		}

		inline char SPI_Transfer(volatile char data) // { return SpiDrv::SpiDrv_spiTransfer( data ); }
		{
			return C_SPI.transfer(data);
		}

		inline bool isSlaveReady()
		{
			return digitalRead( C_BUSY ) == LOW;
		}

		inline bool SpiDrv_available()
		{
			if( UseGPIO0() )
				return ( digitalRead( C_GPIO0 ) == HIGH );

			return true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
//		uint8_t C_INDEX,
		typename T_ChannelOutputPin,
		typename T_CountOutputPin,
		typename T_Duration,
		typename T_EncryptionOutputPin,
		typename T_FScanning,
		typename T_FoundNetworkOutputPin,
		typename T_MACAddressOutputPin,
		typename T_SSIDOutputPin,
		typename T_ScanningOutputPin,
		typename T_SignalStrengthOutputPin
	> class AdafruitAirLiftScanNetworksOperation :
		public T_ChannelOutputPin,
		public T_CountOutputPin,
		public T_Duration,
		public T_EncryptionOutputPin,
		public T_FScanning,
		public T_FoundNetworkOutputPin,
		public T_MACAddressOutputPin,
		public T_SSIDOutputPin,
		public T_ScanningOutputPin,
		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( ChannelOutputPin )
		_V_PIN_( CountOutputPin )
		_V_PIN_( EncryptionOutputPin )
		_V_PIN_( FoundNetworkOutputPin )
		_V_PIN_( MACAddressOutputPin )
		_V_PIN_( SSIDOutputPin )
		_V_PIN_( ScanningOutputPin )
		_V_PIN_( SignalStrengthOutputPin )

	public:
		_V_PROP_( Duration )

	protected:
		_V_PROP_( FScanning )

	public:
		inline void ScanInputPin_o_Receive( void *_Data )
		{
			if( C_OWNER.StartScan( ChannelOutputPin().GetPinIsConnected(), EncryptionOutputPin().GetPinIsConnected(), MACAddressOutputPin().GetPinIsConnected(), SignalStrengthOutputPin().GetPinIsConnected() ))
			{
				FScanning() = true;
				T_ScanningOutputPin::SetPinValueHigh();
			}

		}

	public:
		inline void SystemStart()
		{
			T_ScanningOutputPin::SetPinValueLow();
			ScanInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
		}

	public:
		inline void ScanResultCountChannels( uint8_t AValue )
		{
			T_CountOutputPin::SetPinValue( AValue );
		}

		inline void ScanResulFound()
		{
			T_FoundNetworkOutputPin::ClockPin();
		}

		inline void ScanResultChannel( uint8_t AValue )
		{
			T_ChannelOutputPin::SetPinValue( AValue );
		}

		inline void ScanResultEncription( uint8_t AValue )
		{
			T_EncryptionOutputPin::SetPinValue( AValue );
		}

		inline void ScanResultSignalStrength( int32_t AValue )
		{
			T_SignalStrengthOutputPin::SetPinValue( AValue );
		}

		inline void ScanResultMACAddress( uint8_t * AValue )
		{
//			char AMACString[ 6 * 3 + 1 ];
//			sprintf( AMACString, "%02X:%02X:%02X:%02X:%02X:%02X", AValue[ 5 ], AValue[ 4 ], AValue[ 3 ], AValue[ 2 ], AValue[ 1 ], AValue[ 0 ] );
			T_MACAddressOutputPin::SetPinValue( MACAdressToString( AValue ) );
		}

		inline void ScanResultSSID( String & AValue )
		{
			T_SSIDOutputPin::SetPinValue( AValue );
		}

		inline void ScanResulCompleted()
		{
			T_ScanningOutputPin::SetPinValueLow();
		}

		inline void ScanDurationCompleted( unsigned long APeriod, bool & AValue )
		{
			if( FScanning() )
				if( APeriod > Duration().GetValue() )
					AValue = true;

		}

	public:
		inline AdafruitAirLiftScanNetworksOperation()
		{
			FScanning() = false;
		}

	};
//---------------------------------------------------------------------------
/*
	class TArduinoAdafruitAirLiftBasicOperation
	{
	public:
		inline void ScanResultCountChannels( uint8_t AValue ) {}
		inline void ScanResultSSID( String & AValue ) {}
		inline void ScanResultChannel( uint8_t AValue ) {}
		inline void ScanResultEncription( uint8_t AValue ) {}
		inline void ScanResultSignalStrength( int32_t AValue ) {}
		inline void ScanResultMACAddress( uint8_t * AValue ) {}
		inline void ScanResulFound() {}
		inline void ScanResulCompleted() {}

	};
*/
		//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class TArduinoAdafruitAirLiftThermometer :
//		public TArduinoAdafruitAirLiftBasicOperation,
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		inline void ReadSensor()
		{
			if( Enabled().GetValue())
				T_OutputPin::SetPinValue( C_OWNER.getTemperature() );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FLastTime,
		typename T_OutputPin,
		typename T_TimeZoneOffset
	> class TArduinoAdafruitAirLiftTimeOperation :
//		public TArduinoAdafruitAirLiftBasicOperation,
		public T_Enabled,
		public T_FLastTime,
		public T_OutputPin,
		public T_TimeZoneOffset
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( TimeZoneOffset )

	protected:
		_V_PROP_( FLastTime )

	protected:
		inline void ReadSensor( bool AForceSend )
		{
			if( ! Enabled().GetValue())
				return;

//			Serial.println(  );

			TDateTime ADateTime( 719163, 0 ); // = 2.0; 1-1-1970

			uint32_t	ATimeValue = C_OWNER.getTime();
//			Serial.println( ATimeValue );
			uint32_t	ATimeValue1 = ATimeValue / ( 60 * 60 );
			ADateTime.AddHours( ATimeValue1 );
			ADateTime.AddSeconds( ATimeValue % ( 60 * 60 ));
//			ADateTime.AddMilliSeconds( ADelay );

			int32_t	AIntOffset = TimeZoneOffset();

//				Serial.println( AIntOffset );

			ADateTime.AddHours( AIntOffset );
			float AFraction = V_FMOD( TimeZoneOffset(), 1 );
//				Serial.println( AFraction );

			AIntOffset = AFraction * 60;

//				Serial.println( AIntOffset );
			ADateTime.AddMinutes( AIntOffset );

//			Serial.println( ADateTime.ToString() );
			if( AForceSend || FLastTime().GetValue() != ATimeValue )
				T_OutputPin::SetPinValue( ADateTime );

			FLastTime() = ATimeValue;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor( true );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( true );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_ErrorOutputPin,
		typename T_Host,
		typename T_IPAddress,
		typename T_SuccessOutputPin,
		typename T_TimeOfLive,
		typename T_TimeoutOutputPin,
		typename T_UnknownHostOutputPin,
		typename T_UnreachableOutputPin
	> class TArduinoAdafruitAirLiftPingOperation :
//		public TArduinoAdafruitAirLiftBasicOperation,
		public T_Enabled,
		public T_ErrorOutputPin,
		public T_Host,
		public T_IPAddress,
		public T_SuccessOutputPin,
		public T_TimeOfLive,
		public T_TimeoutOutputPin,
		public T_UnknownHostOutputPin,
		public T_UnreachableOutputPin
	{
	public:
		_V_PIN_( ErrorOutputPin )
		_V_PIN_( SuccessOutputPin )
		_V_PIN_( TimeoutOutputPin )
		_V_PIN_( UnknownHostOutputPin )
		_V_PIN_( UnreachableOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Host )
		_V_PROP_( IPAddress )
		_V_PROP_( TimeOfLive )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue())
				return;

			T_ErrorOutputPin::SetPinValue( false );
			T_SuccessOutputPin::SetPinValue( false );
			T_TimeoutOutputPin::SetPinValue( false );
			T_UnknownHostOutputPin::SetPinValue( false );
			T_UnreachableOutputPin::SetPinValue( false );

			int AResult;
			if( Host().GetValue().length() )
				AResult = C_OWNER.ping( Host().c_str(), TimeOfLive() );

			else if( IPAddress().GetUInt32Value() != 0 )
				AResult = C_OWNER.ping( IPAddress().GetIPAddress(), TimeOfLive() );

			else
				return;

			switch( AResult )
			{
				case NinaESP32::WL_PING_DEST_UNREACHABLE : T_UnreachableOutputPin::SetPinValue( true ); break;
				case NinaESP32::WL_PING_TIMEOUT : T_TimeoutOutputPin::SetPinValue( true ); break;
				case NinaESP32::WL_PING_UNKNOWN_HOST : T_UnknownHostOutputPin::SetPinValue( true ); break;
				case NinaESP32::WL_PING_ERROR : T_ErrorOutputPin::SetPinValue( true ); break;
				default: T_SuccessOutputPin::SetPinValue( true ); break;
			}
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_InitialValue
	> class TArduinoAdafruitAirLiftRGBLed :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
			setLEDs(uint8_t red, uint8_t green, uint8_t blue);
			Analog.Write( C_RED_PIN, InitialValue().GetReference().Red / 255.0 );
			Analog.Write( C_GREEN_PIN, InitialValue().GetReference().Green / 255.0 );
			Analog.Write( C_BLUE_PIN, InitialValue().GetReference().Blue / 255.0 );
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class TArduinoAdafruitAirLiftLeds
	{
	public:
		inline void Nina_pinMode( uint8_t pin, uint8_t mode )
		{
			C_OWNER.Nina_pinMode( pin, mode );
		}

		inline void Nina_analogWrite( uint8_t pin, uint8_t value )
		{
			C_OWNER.Nina_analogWrite( pin, value );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_InitialValue
	> class TArduinoAdafruitAirLiftLed :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
//			Serial.println( "UpdateColor" );
			C_OWNER.Nina_analogWrite( 26, InitialValue().GetReference().Red );
			C_OWNER.Nina_analogWrite( 25, InitialValue().GetReference().Green );
			C_OWNER.Nina_analogWrite( 27, InitialValue().GetReference().Blue );
		}

	public:
		inline void SystemInit()
		{
			C_OWNER.Nina_pinMode( 25, OUTPUT );
			C_OWNER.Nina_pinMode( 26, OUTPUT );
			C_OWNER.Nina_pinMode( 27, OUTPUT );
		}

		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_PIN,
		typename T_InitialValue
	> class TArduinoAdafruitAirLiftRGBLed :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateValue()
		{
			C_OWNER.Nina_analogWrite( C_PIN, MitovConstrain<float>( InitialValue(), 0.0f, 1.0f ) * 255 + 0.5 );
		}

		void SetValue( float AValue )
		{
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			UpdateValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			SetValue( ( *(bool *)_Data ) ? 1.0 : 0.0 );
		}

		inline void AnalogInputPin_o_Receive( void *_Data )
		{
			SetValue( *(float *)_Data );
		}

	public:
		inline void SystemInit()
		{
			C_OWNER.Nina_pinMode( C_PIN, OUTPUT );
		}

		inline void SystemStart()
		{
			UpdateValue();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Channel,
		typename T_Config,
		typename T_Enabled,
//		typename T_IsHidden,
		typename T_Password,
		typename T_SSID
	> class TArduinoAdafruitAirLiftWiFiModuleAccessPoint :
		public T_Channel,
		public T_Config,
		public T_Enabled,
//		public T_IsHidden,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
//		_V_PROP_( IsHidden )
		_V_PROP_( Channel )

		_V_PROP_( SSID )
		_V_PROP_( Password )

		_V_PROP_( Config )

/*
	public:
		inline void TryStart()
		{
//			if( AOwner->Enabled )
				if( Enabled() )
					if( SSID().GetValue() != "" )
					{
	//Serial.println( "TEST1" );
	//Serial.println( SSID );
	//Serial.println( IsHidden );
						if( Config().Enabled() )
							WiFi.softAPConfig( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress() );

						if( Password().GetValue() != "" )
							WiFi.softAP( SSID().c_str(), Password().c_str(), Channel(), IsHidden() );

						else
	//						WiFi.softAP( SSID().c_str() );
							WiFi.softAP( SSID().c_str(), nullptr, Channel(), IsHidden() );

//						if( AOwner->HostName != "" )
//							WiFi.setHostname( (char *)AOwner->HostName.c_str() );

						WiFi.mode( WIFI_AP );
						WiFi.softAPIP();
					}

		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION
	> class Nina_Module :
		public T_IMPLEMENTATION
	{
	public:
		inline void DELAY_TRANSFER() {}
		inline int WAIT_START_CMD() { return SpiDrv_waitSpiChar(NinaESP32::START_CMD); }

		void SpiDrv_waitForSlaveReady() // { SpiDrv::waitForSlaveReady(); }
		{
			while ( ! T_IMPLEMENTATION::isSlaveReady() );
		}

		void SpiDrv_spiSlaveSelect() // { SpiDrv::spiSlaveSelect(); }
		{
			T_IMPLEMENTATION::SPI_BeginTransfer();
//			WIFININA_SPIWIFI->beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
//			digitalWrite(WIFININA_SLAVESELECT, LOW);

			// wait for up to 5 ms for the NINA to indicate it is not ready for transfer
			// the timeout is only needed for the case when the shield or module is not present
//			for (unsigned long start = millis(); (digitalRead(WIFININA_SLAVEREADY) != HIGH) && (millis() - start) < 5;);
		}

		inline void SpiDrv_spiSlaveDeselect() // { SpiDrv::spiSlaveDeselect(); }
		{
			T_IMPLEMENTATION::SPI_EndTransfer();
//			digitalWrite(WIFININA_SLAVESELECT,HIGH);
//			WIFININA_SPIWIFI->endTransaction();
		}

		inline void WAIT_FOR_SLAVE_SELECT()
		{
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();
		}

		int SpiDrv_waitResponseCmd(uint8_t cmd, uint8_t numParam, uint8_t* param, uint8_t* param_len) // { return SpiDrv::waitResponseCmd( cmd, numParam, param, param_len ); }
		{
			char _data = 0;
			int ii = 0;

			if( IF_CHECK_START_CMD())
			{
				CHECK_DATA( cmd | NinaESP32::REPLY_FLAG, _data);

				if( CHECK_DATA(numParam, _data))
				{
					SpiDrv_readParamLen8(param_len);
					for (ii=0; ii<(*param_len); ++ii)
					{
						// Get Params data
						//param[ii] = SpiDrv_spiTransfer(DUMMY_DATA);
						SpiDrv_getParam(&param[ii]);
					}
				}

				SpiDrv_readAndCheckChar( NinaESP32::END_CMD, &_data);
			}

			return 1;
		}

		inline char SpiDrv_spiTransfer( volatile char data = NinaESP32::DUMMY_DATA ) // { return SpiDrv::SpiDrv_spiTransfer( data ); }
		{
//			char result = WIFININA_SPIWIFI->transfer(data);
			char result = T_IMPLEMENTATION::SPI_Transfer( data );
			DELAY_TRANSFER();

			return result;                    // return the received byte
		}

		inline void SpiDrv_sendParamLen8(uint8_t param_len) // { SpiDrv::sendParamLen8( param_len ); }
		{
			// Send Spi paramLen
			SpiDrv_spiTransfer(param_len);
		}

		uint8_t SpiDrv_readParamLen8( uint8_t* param_len = nullptr ) // { return SpiDrv::readParamLen8( param_len ); }
		{
			uint8_t _param_len = SpiDrv_spiTransfer();
			if ( param_len )
				*param_len = _param_len;

			return _param_len;
		}

		inline int SpiDrv_readAndCheckChar(char checkChar, char* readChar) // { return SpiDrv::SpiDrv_readAndCheckChar( checkChar, readChar ); }
		{
			SpiDrv_getParam((uint8_t*)readChar);

			return  (*readChar == checkChar);
		}

		inline bool IF_CHECK_START_CMD()
		{
			if( ! WAIT_START_CMD() )
			{
				__NINA_DEBUG_TOGGLE_TRIGGER()
				__NINA_DEBUG_WARN("Error waiting START_CMD");
				return false;
			}

			return true;
		}

		int SpiDrv_waitSpiChar(unsigned char waitChar) // { return SpiDrv::SpiDrv_waitSpiChar( waitChar ); }
		{
			int timeout = NinaESP32::TIMEOUT_CHAR;
			unsigned char _readChar = 0;

			do
			{
				_readChar = SpiDrv_readChar(); //get data byte
				if (_readChar == NinaESP32::ERR_CMD)
				{
        			__NINA_DEBUG_WARN("Err cmd received\n");
        			return -1;
				}
			}
			while((timeout-- > 0) && (_readChar != waitChar));

			return  (_readChar == waitChar);
		}

		inline bool CHECK_DATA( uint8_t check, char &x )
		{
			if (!SpiDrv_readAndCheckChar( check, &x ))
			{
        		__NINA_DEBUG_TOGGLE_TRIGGER()
				__NINA_DEBUG_WARN("Reply error");
				__NINA_DEBUG_INFO2(check, (uint8_t)x);
				return false;
			}

			return true;
		}

// Cmd Struct Message
// _________________________________________________________________________________
//| START CMD | C/R  | CMD  |[TOT LEN]| N.PARAM | PARAM LEN | PARAM  | .. | END CMD |
//|___________|______|______|_________|_________|___________|________|____|_________|
//|   8 bit   | 1bit | 7bit |  8bit   |  8bit   |   8bit    | nbytes | .. |   8bit  |
//|___________|______|______|_________|_________|___________|________|____|_________|

		void SpiDrv_sendCmd(uint8_t cmd, uint8_t numParam) // { SpiDrv::sendCmd( cmd, numParam); }
		{
			// Send Spi START CMD
			SpiDrv_spiTransfer(NinaESP32::START_CMD);

			// Send Spi C + cmd
			SpiDrv_spiTransfer(cmd & ~(NinaESP32::REPLY_FLAG));

			// Send Spi totLen
			//SpiDrv_spiTransfer(totLen);

			// Send Spi numParam
			SpiDrv_spiTransfer(numParam);

			// If numParam == 0 send END CMD
			if (numParam == 0)
				SpiDrv_spiTransfer( NinaESP32::END_CMD );

		}

		void SpiDrv_sendParam(uint8_t* param, uint8_t param_len, uint8_t lastParam = NinaESP32::NO_LAST_PARAM ) // { SpiDrv::sendParam( param, param_len, lastParam ); }
		{
			int i = 0;
			// Send Spi paramLen
			SpiDrv_sendParamLen8(param_len);

			// Send Spi param data
			for (i=0; i<param_len; ++i)
				SpiDrv_spiTransfer(param[i]);

			// if lastParam==1 Send Spi END CMD
			if (lastParam == 1)
				SpiDrv_spiTransfer( NinaESP32::END_CMD );
		}

	public:
		float getTemperature()
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_TEMPERATURE_CMD, NinaESP32::PARAM_NUMS_0 );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			float _data = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::GET_TEMPERATURE_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&_data, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
			return _data;
		}

		inline void SpiDrv_getParam(uint8_t* param) // { SpiDrv::getParam( param ); }
		{
			// Get Params data
			*param = SpiDrv_spiTransfer();
			DELAY_TRANSFER();
		}

		inline char SpiDrv_readChar() // { return SpiDrv::SpiDrv_readChar(); }
		{
			uint8_t readChar = 0;
			SpiDrv_getParam( &readChar );
			return readChar;
		}

	public:
		void Nina_pinMode( uint8_t pin, uint8_t mode )
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_PIN_MODE, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendParam((uint8_t*)&pin, 1, NinaESP32::NO_LAST_PARAM);
			SpiDrv_sendParam((uint8_t*)&mode, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_PIN_MODE, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}

			SpiDrv_spiSlaveDeselect();
		}

		void Nina_digitalWrite(uint8_t pin, uint8_t value)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_DIGITAL_WRITE, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendParam((uint8_t*)&pin, 1, NinaESP32::NO_LAST_PARAM);
			SpiDrv_sendParam((uint8_t*)&value, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_DIGITAL_WRITE, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}
			SpiDrv_spiSlaveDeselect();
		}

		void Nina_analogWrite(uint8_t pin, uint8_t value)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_ANALOG_WRITE, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendParam((uint8_t*)&pin, 1, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)&value, 1, NinaESP32::LAST_PARAM );

			// pad to multiple of 4
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_ANALOG_WRITE, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}

			SpiDrv_spiSlaveDeselect();
		}

	public:
		inline void SystemInit()
		{
			T_IMPLEMENTATION::Begin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AccessPoint,
		typename T_AccessPoints_AssignDNS,
		typename T_AccessPoints_IsEnabled,
		typename T_AccessPoints_TryConnect,
		typename T_AddressOutputPin,
		typename T_BSSIDOutputPin,
		uint32_t C_ACESS_POINTS_COUNT,
		uint32_t C_SOCKETS_COUNT,
		typename T_Enabled,
		typename T_FAccessPointIndex,
		typename T_FConnected,
		typename T_FDNSUpdated,
		typename T_FGetChannel,
		typename T_FGetEncriptions,
		typename T_FGetMACAddress,
		typename T_FGetSignalStrengths,
		typename T_FIsStarted,
		typename T_FRemoteConnected,
		typename T_FRemoteStarted,
		typename T_FScanning,
		typename T_FirmwareVersionOutputPin,
		typename T_GatewayIPOutputPin,
		typename T_HostName,
		typename T_LowPower,
		typename T_MACOutputPin,
		typename T_Operations_ScanDurationCompleted,
		typename T_Operations_ScanResulCompleted,
		typename T_Operations_ScanResulFound,
		typename T_Operations_ScanResultChannel,
		typename T_Operations_ScanResultCountChannels,
		typename T_Operations_ScanResultEncription,
		typename T_Operations_ScanResultMACAddress,
		typename T_Operations_ScanResultSSID,
		typename T_Operations_ScanResultSignalStrength,
		typename T_RemoteConnectedOutputPin,
		typename T_Sockets_StopSocket,
		typename T_SubnetMaskIPOutputPin
	> class Nina_WiFi :
		public T_AccessPoint,
		public T_AddressOutputPin,
		public T_BSSIDOutputPin,
		public T_Enabled,
		public T_FAccessPointIndex,
		public T_FConnected,
		public T_FDNSUpdated,
		public T_FGetChannel,
		public T_FGetEncriptions,
		public T_FGetMACAddress,
		public T_FGetSignalStrengths,
		public T_FIsStarted,
		public T_FRemoteConnected,
		public T_FRemoteStarted,
		public T_FScanning,
		public T_FirmwareVersionOutputPin,
		public T_GatewayIPOutputPin,
		public T_HostName,
		public T_LowPower,
		public T_MACOutputPin,
		public T_RemoteConnectedOutputPin,
		public T_SubnetMaskIPOutputPin
	{
	private:
		struct __attribute__((__packed__)) tParam
		{
			uint8_t     paramLen;
			char*	    param;
		};

	struct {
		uint8_t* data;
		uint8_t* head;
		int length;
	} _buffers[ NinaESP32::WIFI_MAX_SOCK_NUM ];

	static const uint8_t WIFI_SOCKET_NUM_BUFFERS = (sizeof(_buffers) / sizeof(_buffers[0]));

	public:
		_V_PIN_( AddressOutputPin )
		_V_PIN_( BSSIDOutputPin )
		_V_PIN_( FirmwareVersionOutputPin )
		_V_PIN_( GatewayIPOutputPin )
		_V_PIN_( SubnetMaskIPOutputPin )
		_V_PIN_( MACOutputPin )
		_V_PIN_( RemoteConnectedOutputPin )

	public:
		_V_PROP_( AccessPoint )
		_V_PROP_( Enabled )
		_V_PROP_( HostName )
		_V_PROP_( LowPower )

	protected:
		_V_PROP_( FAccessPointIndex )
		_V_PROP_( FConnected )
		_V_PROP_( FDNSUpdated )
		_V_PROP_( FGetChannel )
		_V_PROP_( FGetSignalStrengths )
		_V_PROP_( FGetMACAddress )
		_V_PROP_( FGetEncriptions )
		_V_PROP_( FIsStarted )
		_V_PROP_( FRemoteConnected )
		_V_PROP_( FRemoteStarted )
		_V_PROP_( FScanning )

	protected:
		unsigned long	FLastTime;

	public:
		float getTemperature()
		{
			return C_OWNER.getTemperature();
		}

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

		void UpdateLowPower()
		{
			WiFiDrv_setPowerMode( LowPower().GetValue() ? 1 : 0 );
		}

		void UpdateAccessPoint()
		{
			if( AccessPoint().Enabled() )
			{
				if( Enabled() )
				{
					if( AccessPoint().SSID().GetValue() != "" )
					{
//	Serial.println( "TEST1" );
//	Serial.println( AccessPoint().SSID().GetValue() );
//	Serial.println( IsHidden );
						if( AccessPoint().Password().GetValue() != "" )
							wifiSetApPassphrase( AccessPoint().SSID().c_str(), AccessPoint().SSID().GetValue().length(), AccessPoint().Password().c_str(), AccessPoint().Password().GetValue().length(), AccessPoint().Channel() );

						else
	//						WiFi.softAP( SSID().c_str() );
							wifiSetApNetwork( AccessPoint().SSID().c_str(), AccessPoint().SSID().GetValue().length(), AccessPoint().Channel() );

						if( AccessPoint().Config().Enabled() )
							config( AccessPoint().Config().IP().GetIPAddress(), AccessPoint().Config().Gateway().GetIPAddress(), AccessPoint().Config().Subnet().GetIPAddress() );
//							WiFiDrv_config(3, (uint32_t)AccessPoint().Config().IP().GetIPAddress(), (uint32_t)AccessPoint().Config().Gateway().GetIPAddress(), (uint32_t)AccessPoint().Config().Subnet().GetIPAddress());

//						if( AOwner->HostName != "" )
//							WiFi.setHostname( (char *)AOwner->HostName.c_str() );

//						WiFi.mode( WIFI_AP );
//						WiFi.softAPIP();
					}
				}

			}

			else
				disconnect();

		}

	protected:
		void StopEthernet()
		{
			T_Sockets_StopSocket::Call();

//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->StopSocket();

#ifdef __ESP32_WIFI__DEBUG__
			Serial.println( "StopEthernet" );
#endif
			disconnect(); // true, true );
			FIsStarted() = false;
			FRemoteStarted() = false;
			FDNSUpdated() = false;
		}

		void StartEthernet()
		{
			if( HostName().GetValue() != "" )
				setHostname( (char *)HostName().c_str() );

////			begin();
			disconnect(); // true, true );
//			esp_wifi_disconnect();

//			WiFi.setAutoReconnect( AutoReconnect().GetValue() );

			if( HostName().GetValue() != "" )
				setHostname( (char *)HostName().c_str() );

////			WiFi.mode( WIFI_OFF );
			if( Enabled() )
				UpdateAccessPoint();
//				AccessPoint().TryStart();

			if( T_BSSIDOutputPin::GetPinIsConnected() )
			{
				uint8_t mac[6];
				BSSID( mac );

//				char AMACString[ 6 * 3 + 1 ];
//				sprintf( AMACString, "%02X:%02X:%02X:%02X:%02X:%02X", mac[ 5 ], mac[ 4 ], mac[ 3 ], mac[ 2 ], mac[ 1 ], mac[ 0 ] );
				T_BSSIDOutputPin::SetPinValue( MACAdressToString( mac ) );
			}

			if( T_AddressOutputPin::GetPinIsConnected() || T_GatewayIPOutputPin::GetPinIsConnected() || T_SubnetMaskIPOutputPin::GetPinIsConnected() )
			{
				uint8_t  AlocalIp[ NinaESP32::WL_IPV4_LENGTH ];
				uint8_t  AsubnetMask[ NinaESP32::WL_IPV4_LENGTH ];
				uint8_t  AgatewayIp[ NinaESP32::WL_IPV4_LENGTH ];
				getNetworkData( AlocalIp, AsubnetMask, AgatewayIp );

				if( T_AddressOutputPin::GetPinIsConnected() )
					T_AddressOutputPin::SetPinValue( IPAdressToString( AlocalIp ) );

				if( T_GatewayIPOutputPin::GetPinIsConnected() )
					T_GatewayIPOutputPin::SetPinValue( IPAdressToString( AgatewayIp ) );

				if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
					T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( AsubnetMask ) );

			}
//			if( T_BSSIDOutputPin::GetPinIsConnected() )
//				T_BSSIDOutputPin::SetPinValue( WiFi.BSSIDstr().c_str() );

//			if( T_GatewayIPOutputPin::GetPinIsConnected() )
//				T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

//			if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
//				T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

		}

	protected:
		inline void DELAY_TRANSFER() { C_OWNER.DELAY_TRANSFER(); }

		inline int WAIT_START_CMD() { return C_OWNER.WAIT_START_CMD(); }

		inline bool IF_CHECK_START_CMD()
		{
			return C_OWNER.IF_CHECK_START_CMD();
		}

		inline bool CHECK_DATA( uint8_t check, char &x )
		{
			return C_OWNER.CHECK_DATA( check, x );
		}

		void SpiDrv_spiSlaveSelect() // { SpiDrv::spiSlaveSelect(); }
		{
			C_OWNER.SpiDrv_spiSlaveSelect();
		}

		inline void SpiDrv_spiSlaveDeselect() // { SpiDrv::spiSlaveDeselect(); }
		{
			C_OWNER.SpiDrv_spiSlaveDeselect();
		}

		inline char SpiDrv_spiTransfer( volatile char data = NinaESP32::DUMMY_DATA ) // { return SpiDrv::SpiDrv_spiTransfer( data ); }
		{
			return C_OWNER.SpiDrv_spiTransfer( data );
		}

		inline void SpiDrv_waitForSlaveReady() // { SpiDrv::waitForSlaveReady(); }
		{
			C_OWNER.SpiDrv_waitForSlaveReady();
		}

		inline void WAIT_FOR_SLAVE_SELECT()
		{
			C_OWNER.WAIT_FOR_SLAVE_SELECT();
		}

		inline int SpiDrv_waitSpiChar(unsigned char waitChar) // { return SpiDrv::SpiDrv_waitSpiChar( waitChar ); }
		{
			return C_OWNER.SpiDrv_waitSpiChar( waitChar );
		}

		inline int SpiDrv_waitResponseCmd(uint8_t cmd, uint8_t numParam, uint8_t* param, uint8_t* param_len) // { return SpiDrv::waitResponseCmd( cmd, numParam, param, param_len ); }
		{
			return C_OWNER.SpiDrv_waitResponseCmd( cmd, numParam, param, param_len );
		}

		int SpiDrv_waitResponseData8(uint8_t cmd, uint8_t* param, uint8_t* param_len) // { return SpiDrv::waitResponseData8( cmd, param, param_len ); }
		{
			char _data = 0;
			int ii = 0;

			if( IF_CHECK_START_CMD())
			{
				CHECK_DATA(cmd | NinaESP32::REPLY_FLAG, _data);

				uint8_t numParam = SpiDrv_readChar();
				if (numParam != 0)
				{
					SpiDrv_readParamLen8(param_len);
					for (ii=0; ii<(*param_len); ++ii)
					{
						// Get Params data
						param[ii] = SpiDrv_spiTransfer();
					}
				}

				SpiDrv_readAndCheckChar( NinaESP32::END_CMD, &_data);
			}

			return 1;
		}

		int SpiDrv_waitResponseData16(uint8_t cmd, uint8_t* param, uint16_t* param_len) // { return SpiDrv::waitResponseData16( cmd, param, param_len ); }
		{
			char _data = 0;
			uint16_t ii = 0;

			if( IF_CHECK_START_CMD())
			{
				CHECK_DATA(cmd | NinaESP32::REPLY_FLAG, _data);

				uint8_t numParam = SpiDrv_readChar();
				if (numParam != 0)
				{
					SpiDrv_readParamLen16(param_len);
					for (ii=0; ii<(*param_len); ++ii)
					{
						// Get Params data
						param[ii] = SpiDrv_spiTransfer();
					}
				}

				SpiDrv_readAndCheckChar( NinaESP32::END_CMD, &_data);
			}

			return 1;
		}

		inline void SpiDrv_sendParamLen8(uint8_t param_len) // { SpiDrv::sendParamLen8( param_len ); }
		{
			C_OWNER.SpiDrv_sendParamLen8( param_len );
		}

		inline uint8_t SpiDrv_readParamLen8(uint8_t* param_len = nullptr) // { return SpiDrv::readParamLen8( param_len ); }
		{
			return C_OWNER.SpiDrv_readParamLen8( param_len );
		}

		uint16_t SpiDrv_readParamLen16(uint16_t* param_len = NULL) // { return SpiDrv::readParamLen16( param_len ); }
		{
			uint16_t _param_len = SpiDrv_spiTransfer()<<8 | (SpiDrv_spiTransfer()& 0xff);
			if (param_len != NULL)
				*param_len = _param_len;

			return _param_len;
		}

		inline void SpiDrv_sendParamLen16(uint16_t param_len) // { SpiDrv::sendParamLen16( param_len ); }
		{
			// Send Spi paramLen
			SpiDrv_spiTransfer((uint8_t)((param_len & 0xff00)>>8));
			SpiDrv_spiTransfer((uint8_t)(param_len & 0xff));
		}

		inline void SpiDrv_sendCmd(uint8_t cmd, uint8_t numParam) // { SpiDrv::sendCmd( cmd, numParam); }
		{
			C_OWNER.SpiDrv_sendCmd( cmd, numParam );
		}

		void SpiDrv_sendParam(uint8_t* param, uint8_t param_len, uint8_t lastParam = NinaESP32::NO_LAST_PARAM) // { SpiDrv::sendParam( param, param_len, lastParam ); }
		{
			C_OWNER.SpiDrv_sendParam( param, param_len, lastParam );
		}

		void SpiDrv_sendParam(uint16_t param, uint8_t lastParam = NinaESP32::NO_LAST_PARAM) // { SpiDrv::sendParam( param, lastParam ); }
		{
			// Send Spi paramLen
			SpiDrv_sendParamLen8(2);

			SpiDrv_spiTransfer((uint8_t)((param & 0xff00)>>8));
			SpiDrv_spiTransfer((uint8_t)(param & 0xff));

			// if lastParam==1 Send Spi END CMD
			if (lastParam == 1)
				SpiDrv_spiTransfer( NinaESP32::END_CMD);
		}

		inline void SpiDrv_getParam(uint8_t* param) // { SpiDrv::getParam( param ); }
		{
			C_OWNER.SpiDrv_getParam( param );
		}

		inline int SpiDrv_readAndCheckChar(char checkChar, char* readChar) // { return SpiDrv::SpiDrv_readAndCheckChar( checkChar, readChar ); }
		{
			return C_OWNER.SpiDrv_readAndCheckChar( checkChar, readChar );
		}

		inline char SpiDrv_readChar() // { return SpiDrv::SpiDrv_readChar(); }
		{
			return C_OWNER.SpiDrv_readChar();
		}


/*
		int SpiDrv_available() // { return SpiDrv::available(); }
		{
			if (WIFININA_SLAVEGPIO0 >= 0)
				return (digitalRead(WIFININA_SLAVEGPIO0) == HIGH);

			return true;
		}
*/

		int SpiDrv_waitResponseParams(uint8_t cmd, uint8_t numParam, tParam* params) // { return SpiDrv::waitResponseParams( cmd, numParam, params ); }
		{
			char _data = 0;
			int i =0, ii = 0;

			if( IF_CHECK_START_CMD())
			{
				CHECK_DATA(cmd | NinaESP32::REPLY_FLAG, _data);

				uint8_t _numParam = SpiDrv_readChar();
				if (_numParam != 0)
				{
					for (i=0; i<_numParam; ++i)
					{
						params[i].paramLen = SpiDrv_readParamLen8();
						for (ii=0; ii<params[i].paramLen; ++ii)
						{
							// Get Params data
							params[i].param[ii] = SpiDrv_spiTransfer();
						}
					}
				}

				else
				{
					__NINA_DEBUG_WARN("Error numParam == 0");
					return 0;
				}

				if (numParam != _numParam)
				{
					__NINA_DEBUG_WARN("Mismatch numParam");
					return 0;
				}

				SpiDrv_readAndCheckChar( NinaESP32::END_CMD, &_data);
			}
			return 1;
		}

		// Set ip configuration disabling dhcp client
		//
		//    param validParams: set the number of parameters that we want to change
		//    					 i.e. validParams = 1 means that we'll change only ip address
		//    					 	  validParams = 3 means that we'll change ip address, gateway and netmask
		//    param local_ip: 	Static ip configuration
		//    param gateway: 	Static gateway configuration
		//    param subnet: 	Static subnet mask configuration
		//
		void WiFiDrv_config(uint8_t validParams, uint32_t local_ip, uint32_t gateway, uint32_t subnet)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_IP_CONFIG_CMD, NinaESP32::PARAM_NUMS_4 );
			SpiDrv_sendParam((uint8_t*)&validParams, 1, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)&local_ip, 4, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)&gateway, 4, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)&subnet, 4, NinaESP32::LAST_PARAM );

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_IP_CONFIG_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}

			SpiDrv_spiSlaveDeselect();
		}

		// Set DNS ip configuration
		//
		//       param validParams: set the number of parameters that we want to change
		//       					 i.e. validParams = 1 means that we'll change only dns_server1
		//       					 	  validParams = 2 means that we'll change dns_server1 and dns_server2
		//       param dns_server1: Static DNS server1 configuration
		//       param dns_server2: Static DNS server2 configuration
		//
		void WiFiDrv_setDNS(uint8_t validParams, uint32_t dns_server1, uint32_t dns_server2)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_DNS_CONFIG_CMD, NinaESP32::PARAM_NUMS_3 );
			SpiDrv_sendParam((uint8_t*)&validParams, 1, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)&dns_server1, 4, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)&dns_server2, 4, NinaESP32::LAST_PARAM );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_DNS_CONFIG_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}

			SpiDrv_spiSlaveDeselect();
		}

		void WiFiDrv_setPowerMode( uint8_t mode )
		{
			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_POWER_MODE_CMD, NinaESP32::PARAM_NUMS_1 );

			SpiDrv_sendParam(&mode, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t dataLen = 0;
			uint8_t data = 0;
			SpiDrv_waitResponseCmd( NinaESP32::SET_POWER_MODE_CMD, NinaESP32::PARAM_NUMS_1, &data, &dataLen);

			SpiDrv_spiSlaveDeselect();
		}

		uint8_t WiFiDrv_reqHostByName(const char* aHostname)
		{
			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::REQ_HOST_BY_NAME_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam((uint8_t*)aHostname, strlen(aHostname), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			int commandSize = 5 + strlen(aHostname);
			while (commandSize % 4)
			{
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			uint8_t result = SpiDrv_waitResponseCmd( NinaESP32::REQ_HOST_BY_NAME_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen );

			SpiDrv_spiSlaveDeselect();

			if (result)
				result = (_data == 1);

			return result;
		}

		int16_t WiFiDrv_ping(uint32_t ipAddress, uint8_t ttl)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::PING_CMD, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendParam((uint8_t*)&ipAddress, sizeof(ipAddress), NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)&ttl, sizeof(ttl), NinaESP32::LAST_PARAM );

			// pad to multiple of 4
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint16_t _data;
			uint8_t _dataLen = 0;
			if( ! SpiDrv_waitResponseCmd( NinaESP32::PING_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_PING_ERROR;
			}

			SpiDrv_spiSlaveDeselect();
			return _data;
		}

		int8_t wifiSetApNetwork(const char* ssid, uint8_t ssid_len, uint8_t channel)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_AP_NET_CMD, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendParam((uint8_t*)ssid, ssid_len);
			SpiDrv_sendParam(&channel, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			int commandSize = 3 + ssid_len;
			while (commandSize % 4) {
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_AP_NET_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}
			SpiDrv_spiSlaveDeselect();

			return(_data == NinaESP32::WIFI_SPI_ACK) ? NinaESP32::WL_SUCCESS : NinaESP32::WL_FAILURE;
		}

		int8_t wifiSetApPassphrase(const char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len, uint8_t channel)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_AP_PASSPHRASE_CMD, NinaESP32::PARAM_NUMS_3 );
			SpiDrv_sendParam((uint8_t*)ssid, ssid_len, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)passphrase, len, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam(&channel, 1, NinaESP32::LAST_PARAM );

			// pad to multiple of 4
			int commandSize = 4 + ssid_len + len;
			while (commandSize % 4)
			{
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_AP_PASSPHRASE_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}

			SpiDrv_spiSlaveDeselect();
			return _data;
		}

		// Start Wifi connection with passphrase
		//   the most secure supported mode will be automatically selected

		//   param ssid: Pointer to the SSID string.
		//   param ssid_len: Lenght of ssid string.
		//   param passphrase: Passphrase. Valid characters in a passphrase
		//          must be between ASCII 32-126 (decimal).
		//   param len: Lenght of passphrase string.
		//   return: WL_SUCCESS or WL_FAILURE
		int8_t wifiSetPassphrase(const char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_PASSPHRASE_CMD, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendParam((uint8_t*)ssid, ssid_len, NinaESP32::NO_LAST_PARAM );
			SpiDrv_sendParam((uint8_t*)passphrase, len, NinaESP32::LAST_PARAM );

			// pad to multiple of 4
			int commandSize = 6 + ssid_len + len;
			while (commandSize % 4)
			{
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_PASSPHRASE_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}
			SpiDrv_spiSlaveDeselect();
			return _data;
		}

		// Change Ip configuration settings disabling the dhcp client
		//
		//    param local_ip: 	Static ip configuration
		//
		void config(IPAddress local_ip)
		{
			WiFiDrv_config(1, (uint32_t)local_ip, 0, 0);
		}

		// Change Ip configuration settings disabling the dhcp client
		//
		//    param local_ip: 	Static ip configuration
		//    param dns_server:     IP configuration for DNS server 1
		//
		void config(IPAddress local_ip, IPAddress dns_server)
		{
			WiFiDrv_config(1, (uint32_t)local_ip, 0, 0);
			WiFiDrv_setDNS(1, (uint32_t)dns_server, 0);
		}

		// Change Ip configuration settings disabling the dhcp client
		//
		//    param local_ip: 	Static ip configuration
		//	  param dns_server:     IP configuration for DNS server 1
		//    param gateway : 	Static gateway configuration
		//
		void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
		{
			WiFiDrv_config(2, (uint32_t)local_ip, (uint32_t)gateway, 0);
			WiFiDrv_setDNS(1, (uint32_t)dns_server, 0);
		}

		// Change Ip configuration settings disabling the dhcp client
		//
		//    param local_ip: 	Static ip configuration
		//    param dns_server:     IP configuration for DNS server 1
		//    param gateway: 	Static gateway configuration
		//    param subnet:		Static Subnet mask
		//
		void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
		{
			WiFiDrv_config(3, (uint32_t)local_ip, (uint32_t)gateway, (uint32_t)subnet);
			WiFiDrv_setDNS(1, (uint32_t)dns_server, 0);
		}

		//
		// Disconnect from the network
		//
		// return: one value of wl_status_t enum
		//
		int disconnect(void)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::DISCONNECT_CMD, NinaESP32::PARAM_NUMS_1 );

			uint8_t _dummy = NinaESP32::DUMMY_DATA;
			SpiDrv_sendParam(&_dummy, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			int8_t result = SpiDrv_waitResponseCmd( NinaESP32::DISCONNECT_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen);

			SpiDrv_spiSlaveDeselect();

			return result;
		}

		// Get network Data information
		void getNetworkData(uint8_t *ip, uint8_t *mask, uint8_t *gwip)
		{
			tParam params[NinaESP32::PARAM_NUMS_3] = { {0, (char*)ip}, {0, (char*)mask}, {0, (char*)gwip}};

			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_IPADDR_CMD, NinaESP32::PARAM_NUMS_1 );

			uint8_t _dummy = NinaESP32::DUMMY_DATA;
			SpiDrv_sendParam(&_dummy, sizeof(_dummy), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			SpiDrv_waitResponseParams( NinaESP32::GET_IPADDR_CMD, NinaESP32::PARAM_NUMS_3, params);

			SpiDrv_spiSlaveDeselect();
		}

		// Set the hostname used for DHCP requests
		//  param name: hostname to set
		void setHostname(const char* hostname)
		{
#ifdef __ESP32_WIFI__DEBUG__
			Serial.print( "setHostname: " );
			Serial.println( hostname );
#endif
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SET_HOSTNAME_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam((uint8_t*)hostname, strlen(hostname), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			int commandSize = 5 + strlen(hostname);
			while (commandSize % 4)
			{
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::SET_HOSTNAME_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen ))
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}

			SpiDrv_spiSlaveDeselect();
		}

		String firmwareVersion()
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_FW_VERSION_CMD, NinaESP32::PARAM_NUMS_0 );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			static char 	fwVersion[ NinaESP32::WL_FW_VER_LENGTH ];
			if (!SpiDrv_waitResponseCmd( NinaESP32::GET_FW_VERSION_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)fwVersion, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
			return fwVersion;
		}

		uint8_t beginAP(const char *ssid, const char* passphrase, uint8_t channel)
		{
			uint8_t status = NinaESP32::WL_IDLE_STATUS;
			uint8_t attempts = NinaESP32::WL_MAX_ATTEMPT_CONNECTION;

			// set passphrase
			if ( wifiSetApPassphrase(ssid, strlen(ssid), passphrase, strlen(passphrase), channel)!= NinaESP32::WL_FAILURE)
			{
 			   do
 			   {
 				   delay( NinaESP32::WL_DELAY_START_CONNECTION );
 				   status = getConnectionStatus();
 			   }
			   while ((( status == NinaESP32::WL_IDLE_STATUS)||(status == NinaESP32::WL_SCAN_COMPLETED))&&(--attempts>0));
			}

			else
    			status = NinaESP32::WL_AP_FAILED;

			return status;
		}

		int begin(const char* ssid, const char *passphrase)
		{
			uint8_t status = NinaESP32::WL_IDLE_STATUS;
			uint8_t attempts = NinaESP32::WL_MAX_ATTEMPT_CONNECTION;

			// set passphrase
			if ( wifiSetPassphrase(ssid, strlen(ssid), passphrase, strlen(passphrase))!= NinaESP32::WL_FAILURE)
			{
 			   do
 			   {
 				   delay( NinaESP32::WL_DELAY_START_CONNECTION );
 				   status = getConnectionStatus();
 			   }
			   while ((( status == NinaESP32::WL_IDLE_STATUS)||(status == NinaESP32::WL_NO_SSID_AVAIL)||(status == NinaESP32::WL_SCAN_COMPLETED))&&(--attempts>0));
			}
			else
    			status = NinaESP32::WL_CONNECT_FAILED;

			return status;
		}

		void SpiDrv_sendBuffer(uint8_t* param, uint16_t param_len, uint8_t lastParam = NinaESP32::NO_LAST_PARAM ) // { SpiDrv::sendBuffer( param, param_len, lastParam ); }
		{
			uint16_t i = 0;

			// Send Spi paramLen
			SpiDrv_sendParamLen16(param_len);

			// Send Spi param data
			for (i=0; i<param_len; ++i)
				SpiDrv_spiTransfer(param[i]);

			// if lastParam==1 Send Spi END CMD
			if (lastParam == 1)
				SpiDrv_spiTransfer( NinaESP32::END_CMD);
		}

		// Resolve the given hostname to an IP address.
		//   param aHostname: Name to be resolved
		//   param aResult: IPAddress structure to store the returned IP address
		//   result: 1 if aIPAddrString was successfully converted to an IP address,
		//          else error code
		bool WiFiDrv_getHostByName(IPAddress& aResult)
		{
			uint8_t  _ipAddr[ NinaESP32::WL_IPV4_LENGTH ];
			IPAddress dummy(0xFF,0xFF,0xFF,0xFF);
			bool result = false;

			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_HOST_BY_NAME_CMD, NinaESP32::PARAM_NUMS_0 );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::GET_HOST_BY_NAME_CMD, NinaESP32::PARAM_NUMS_1, _ipAddr, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse")

			else
			{
    			aResult = _ipAddr;
    			result = (aResult != dummy);
			}

			SpiDrv_spiSlaveDeselect();
			return result;
		}

		bool getDataBuf(uint8_t sock, uint8_t *_data, uint16_t *_dataLen)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_DATABUF_TCP_CMD, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendBuffer(&sock, sizeof(sock));
			SpiDrv_sendBuffer((uint8_t *)_dataLen, sizeof(*_dataLen), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			if (!SpiDrv_waitResponseData16( NinaESP32::GET_DATABUF_TCP_CMD, _data, _dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
			}
			SpiDrv_spiSlaveDeselect();
			if (*_dataLen!=0)
			{
				return true;
			}
			return false;
		}

	public:
		template < typename T_INSTANCE > size_t Socket_write( T_INSTANCE * AInstance, uint8_t ASocketNo, const uint8_t *buf, size_t size )
		{
			if( ASocketNo == NinaESP32::NO_SOCKET_AVAIL )
			{
//				AInstance->setWriteError();
				return 0;
			}

			if (size==0)
			{
//				AInstance->setWriteError();
				return 0;
			}

			size_t written = sendData( ASocketNo, buf, size);
			if ( ! written )
			{
//				AInstance->setWriteError();
				return 0;
			}

/*
			if (! C_OWNER.checkDataSent( FClientSocketNo ))
			{
				AInstance->setWriteError();
				return 0;
			}
*/
			return written;
		}

	public:
		unsigned long getTime()
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_TIME_CMD, NinaESP32::PARAM_NUMS_0 );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			uint32_t _data = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::GET_TIME_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&_data, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
			return _data;
		//	return WiFiDrv::getTime();
		}

		int ping( const char* hostname, uint8_t ttl )
		{
			IPAddress ip;

			if (! GetIPFromHostName(hostname, ip))
				return NinaESP32::WL_PING_UNKNOWN_HOST;

			return ping( ip, ttl );
		}

		int ping( IPAddress host, uint8_t ttl )
		{
			return WiFiDrv_ping( host, ttl );
		}

		// Resolve the given hostname to an IP address.
		//   param aHostname: Name to be resolved
		//   param aResult: IPAddress structure to store the returned IP address
		//   result: 1 if aIPAddrString was successfully converted to an IP address,
		//           else error code
		bool GetIPFromHostName(const char* aHostname, IPAddress& aResult)
		{
			if( WiFiDrv_reqHostByName(aHostname))
				return WiFiDrv_getHostByName(aResult);

			return false;
		}

		void SocketBuffer_close( int socket )
		{
			if (_buffers[socket].data)
			{
				free(_buffers[socket].data);
				_buffers[socket].data = _buffers[socket].head = NULL;
				_buffers[socket].length = 0;
			}
		}

		int SocketBuffer_available( int socket )
		{
		  if (_buffers[socket].length == 0)
		  {
			if (_buffers[socket].data == NULL)
			{
			  _buffers[socket].data = _buffers[socket].head = (uint8_t*)malloc( NinaESP32::WIFI_SOCKET_BUFFER_SIZE );
			  _buffers[socket].length = 0;
			}

			// sizeof(size_t) is architecture dependent
			// but we need a 16 bit data type here
			uint16_t size = NinaESP32::WIFI_SOCKET_BUFFER_SIZE;
			if ( getDataBuf(socket, _buffers[socket].data, &size))
			{
			  _buffers[socket].head = _buffers[socket].data;
			  _buffers[socket].length = size;
			}
		  }

		  return _buffers[socket].length;
		}

	protected:
		//
		// Start scan WiFi networks available
		//
		// return: Number of discovered networks
		//
		int8_t WiFiDrv_startScanNetworks()
		{
			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::START_SCAN_NETWORKS, NinaESP32::PARAM_NUMS_0 );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;

			if ( ! SpiDrv_waitResponseCmd( NinaESP32::START_SCAN_NETWORKS, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen ) )
			{
				__NINA_DEBUG_WARN("error waitResponse");
				_data = NinaESP32::WL_FAILURE;
			}

			SpiDrv_spiSlaveDeselect();

			return ((int8_t)_data == NinaESP32::WL_FAILURE) ? _data : (int8_t)NinaESP32::WL_SUCCESS;
		}

	// Maximum size of a SSID list
//	static const uint8_t WL_NETWORKS_LIST_MAXNUM	= 10;
//	char 	_networkSsid[WL_NETWORKS_LIST_MAXNUM][WL_SSID_MAX_LENGTH];

		//
		// Get the networks available
		//
		// return: Number of discovered networks
		//
		/*uint8_t*/ void WiFiDrv_getScanNetworks()
		{
//			WAIT_FOR_SLAVE_SELECT();
			if( ! C_OWNER.isSlaveReady() )
				return;

//			Serial.println( "Step 1" );
			SpiDrv_spiSlaveSelect();

//			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::SCAN_NETWORKS, NinaESP32::PARAM_NUMS_0 );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
//			Serial.println( "Step 2" );
			SpiDrv_waitForSlaveReady();
//			Serial.println( "Step 3" );
			SpiDrv_spiSlaveSelect();

			// Wait for reply
//			uint8_t ssidListNum = 0;
//			SpiDrv_waitResponse( SCAN_NETWORKS, &ssidListNum, (uint8_t**)_networkSsid, WL_NETWORKS_LIST_MAXNUM );

			if( IF_CHECK_START_CMD() )
			{
				char _data = 0;
				CHECK_DATA( NinaESP32::SCAN_NETWORKS | NinaESP32::REPLY_FLAG, _data );

				uint8_t numParam = SpiDrv_readChar();

/*
				Serial.println();
				Serial.print( "numParam : " );
				Serial.println( numParam );
				Serial.println();
*/
				String *ANames = nullptr;

				if( numParam )
				{
					ANames = new String[ numParam ];

					for ( int i = 0; i < numParam; ++ i )
					{
//						Serial.print( "STEP1" );
            			uint8_t paramLen = SpiDrv_readParamLen8();
//						Serial.print( "paramLen call" );
//						delay( 500 );
						T_Operations_ScanResultCountChannels::Call( paramLen );
//						Serial.print( "paramLen : " );
//						Serial.println( paramLen );

						String AName;
						AName.reserve( paramLen );

						for (int ii=0; ii<paramLen; ++ii)
							AName += (char)SpiDrv_spiTransfer();
	//						Serial.print( (char)SpiDrv_spiTransfer() );

						ANames[ i ] = AName;
//						Serial.println( AName );
					}

//					Serial.println();
	//				if( numParam > 0 )
				}

				SpiDrv_readAndCheckChar( NinaESP32::END_CMD, &_data );
				SpiDrv_spiSlaveDeselect();

//				Serial.println( numParam );

				for( uint8_t networkItem = 0; networkItem < numParam; ++ networkItem )
				{
					T_Operations_ScanResultSSID::Call( ANames[ networkItem ] );

					if( FGetChannel() )
					{
						WAIT_FOR_SLAVE_SELECT();

						// Send Command
						SpiDrv_sendCmd( NinaESP32::GET_IDX_CHANNEL_CMD, NinaESP32::PARAM_NUMS_1 );

						SpiDrv_sendParam(&networkItem, 1, NinaESP32::LAST_PARAM);

						// pad to multiple of 4
						SpiDrv_readChar();
						SpiDrv_readChar();

						SpiDrv_spiSlaveDeselect();
						//Wait the reply elaboration
						SpiDrv_waitForSlaveReady();
						SpiDrv_spiSlaveSelect();

						// Wait for reply
						uint8_t dataLen = 0;
						uint8_t channel = 0;
						SpiDrv_waitResponseCmd( NinaESP32::GET_IDX_CHANNEL_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&channel, &dataLen);

						SpiDrv_spiSlaveDeselect();

						T_Operations_ScanResultChannel::Call( channel );
					}

					if( FGetEncriptions() )
					{
						WAIT_FOR_SLAVE_SELECT();

						// Send Command
						SpiDrv_sendCmd( NinaESP32::GET_IDX_ENCT_CMD, NinaESP32::PARAM_NUMS_1 );

						SpiDrv_sendParam(&networkItem, 1, NinaESP32::LAST_PARAM);

						// pad to multiple of 4
						SpiDrv_readChar();
						SpiDrv_readChar();

						SpiDrv_spiSlaveDeselect();
						//Wait the reply elaboration
						SpiDrv_waitForSlaveReady();
						SpiDrv_spiSlaveSelect();

						// Wait for reply
						uint8_t dataLen = 0;
						uint8_t encType = 0;
						SpiDrv_waitResponseCmd( NinaESP32::GET_IDX_ENCT_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&encType, &dataLen);

						SpiDrv_spiSlaveDeselect();

						T_Operations_ScanResultEncription::Call( encType );
					}

					if( FGetMACAddress() )
					{
						WAIT_FOR_SLAVE_SELECT();

						// Send Command
						SpiDrv_sendCmd( NinaESP32::GET_IDX_BSSID, NinaESP32::PARAM_NUMS_1 );

						SpiDrv_sendParam(&networkItem, 1, NinaESP32::LAST_PARAM);

						// pad to multiple of 4
						SpiDrv_readChar();
						SpiDrv_readChar();

						SpiDrv_spiSlaveDeselect();
						//Wait the reply elaboration
						SpiDrv_waitForSlaveReady();
						SpiDrv_spiSlaveSelect();

						// Wait for reply
						uint8_t dataLen = 0;
						uint8_t bssid[6];
						SpiDrv_waitResponseCmd( NinaESP32::GET_IDX_BSSID, NinaESP32::PARAM_NUMS_1, bssid, &dataLen);

						SpiDrv_spiSlaveDeselect();

						T_Operations_ScanResultMACAddress::Call( bssid );
					}

					if( FGetSignalStrengths() )
					{
						WAIT_FOR_SLAVE_SELECT();

						// Send Command
						SpiDrv_sendCmd( NinaESP32::GET_IDX_RSSI_CMD, NinaESP32::PARAM_NUMS_1 );

						SpiDrv_sendParam(&networkItem, 1, NinaESP32::LAST_PARAM );

						// pad to multiple of 4
						SpiDrv_readChar();
						SpiDrv_readChar();

						SpiDrv_spiSlaveDeselect();
						//Wait the reply elaboration
						SpiDrv_waitForSlaveReady();
						SpiDrv_spiSlaveSelect();

						// Wait for reply
						uint8_t dataLen = 0;
						int32_t	networkRssi = 0;
						SpiDrv_waitResponseCmd( NinaESP32::GET_IDX_RSSI_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&networkRssi, &dataLen);

						SpiDrv_spiSlaveDeselect();

						T_Operations_ScanResultSignalStrength::Call( networkRssi );

//						return rssi;
					}

					T_Operations_ScanResulFound::Call();
				}

				if( ANames )
					delete [] ANames;

				FScanning() = false;
				FGetChannel() = false;
				FGetMACAddress() = false;
				FGetSignalStrengths() = false;
				FGetEncriptions() = false;
				return;
			}

			SpiDrv_spiSlaveDeselect();

//			return ssidListNum;
		}

		inline void macAddress(uint8_t* mac)
		{
		//	Serial.println( "macAddress" );
			WAIT_FOR_SLAVE_SELECT();

		//	Serial.println( "macAddress 1" );
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_MACADDR_CMD, NinaESP32::PARAM_NUMS_1 );

		//	Serial.println( "macAddress 2" );
			uint8_t _dummy = NinaESP32::DUMMY_DATA;
			SpiDrv_sendParam(&_dummy, 1, NinaESP32::LAST_PARAM);

		//	Serial.println( "macAddress 3" );
			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

		//	Serial.println( "macAddress 4" );
			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			SpiDrv_waitResponseCmd( NinaESP32::GET_MACADDR_CMD, NinaESP32::PARAM_NUMS_1, mac, &_dataLen);

		//	Serial.println( "macAddress 5" );
			SpiDrv_spiSlaveDeselect();
		}

		//
		// Disconnect from the network
		//
		// return: one value of wl_status_t enum
		//
		uint8_t getConnectionStatus()
		{
	//		Serial.println( "getConnectionStatus 0" );

			WAIT_FOR_SLAVE_SELECT();

	//		Serial.println( "getConnectionStatus 1" );

			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_CONN_STATUS_CMD, NinaESP32::PARAM_NUMS_0 );

	//		Serial.println( "getConnectionStatus 2" );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

	//		Serial.println( "getConnectionStatus 3" );

			// Wait for reply
			uint8_t _data = -1;
			uint8_t _dataLen = 0;
			SpiDrv_waitResponseCmd( NinaESP32::GET_CONN_STATUS_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen);

			SpiDrv_spiSlaveDeselect();

			return _data;
		}

		void BSSID( uint8_t * bssid )
		{
			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_CURR_BSSID_CMD, NinaESP32::PARAM_NUMS_1 );

			uint8_t _dummy = NinaESP32::DUMMY_DATA;
			SpiDrv_sendParam(&_dummy, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			SpiDrv_waitResponseCmd( NinaESP32::GET_CURR_BSSID_CMD, NinaESP32::PARAM_NUMS_1, bssid, &_dataLen);

			SpiDrv_spiSlaveDeselect();

		//	uint8_t* _bssid = WiFiDrv::getCurrentBSSID();
		//	memcpy(bssid, _bssid, WL_MAC_ADDR_LENGTH);
		//    return bssid;
		}

		bool WiFi_isConnected()
		{
			return ( getConnectionStatus() == NinaESP32::WL_CONNECTED );
		}

		//
		// Get the gateway ip address.
		//  return: copy the gateway ip address value in IPAddress object
		void WiFiDrv_getGatewayIP(IPAddress& ip)
		{
			uint8_t  _localIp[ NinaESP32::WL_IPV4_LENGTH ];
			uint8_t  _subnetMask[ NinaESP32::WL_IPV4_LENGTH ];
			uint8_t  _gatewayIp[ NinaESP32::WL_IPV4_LENGTH ];
			getNetworkData(_localIp, _subnetMask, _gatewayIp);
			ip = _gatewayIp;
		}

	public:
		bool StartScan( bool AGetChannel, bool AGetEncryption, bool AGetMACAddress, bool AGetSignalStrength )
		{
			if( AGetChannel )
				FGetChannel() = true;

			if( AGetMACAddress )
				FGetMACAddress() = true;

			if( AGetSignalStrength )
				FGetSignalStrengths() = true;

			if( AGetEncryption )
				FGetEncriptions() = true;

			if( FScanning() )
				return true;

//			Serial.println( "Start scan" );

			if( WiFiDrv_startScanNetworks() == NinaESP32::WL_SUCCESS )
			{
				FScanning() = true;
				FLastTime = millis();
			}

			return FScanning();
		}

		uint8_t getSocket()
		{
			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_SOCKET_CMD, NinaESP32::PARAM_NUMS_0 );

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = -1;
			uint8_t _dataLen = 0;
			SpiDrv_waitResponseCmd( NinaESP32::GET_SOCKET_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen);

			SpiDrv_spiSlaveDeselect();

			return _data;
		}

		void startClient(uint32_t ipAddress, uint16_t port, uint8_t sock, uint8_t protMode=NinaESP32::TCP_MODE)
		{
	#ifdef __ESP32_WIFI__DEBUG__
			Serial.println( "startClient 1" );
	#endif // __ESP32_WIFI__DEBUG__

			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::START_CLIENT_TCP_CMD, NinaESP32::PARAM_NUMS_4 );
			SpiDrv_sendParam((uint8_t*)&ipAddress, sizeof(ipAddress));
			SpiDrv_sendParam(port);
			SpiDrv_sendParam(&sock, 1);
			SpiDrv_sendParam(&protMode, 1, NinaESP32::LAST_PARAM);

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::START_CLIENT_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
		}

		void startClient(const char* host, uint8_t host_len, uint32_t ipAddress, uint16_t port, uint8_t sock, uint8_t protMode=NinaESP32::TCP_MODE)
		{
	#ifdef __ESP32_WIFI__DEBUG__
			Serial.print( "startClient " );
			Serial.print( host );
			Serial.print( " " );
			Serial.println( host_len );

			Serial.print( "ipAddress " );
			Serial.println( ipAddress );

			Serial.print( "sock " );
			Serial.println( sock );

			Serial.print( "protMode " );
			Serial.println( protMode );
	#endif // __ESP32_WIFI__DEBUG__

			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::START_CLIENT_TCP_CMD, NinaESP32::PARAM_NUMS_5 );
			SpiDrv_sendParam((uint8_t*)host, host_len);
			SpiDrv_sendParam((uint8_t*)&ipAddress, sizeof(ipAddress));
			SpiDrv_sendParam(port);
			SpiDrv_sendParam(&sock, 1);
			SpiDrv_sendParam(&protMode, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			int commandSize = 17 + host_len;
			while (commandSize % 4)
			{
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if ( ! SpiDrv_waitResponseCmd( NinaESP32::START_CLIENT_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen ) )
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
		}

		void stopClient( uint8_t sock )
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::STOP_CLIENT_TCP_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam(&sock, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if ( ! SpiDrv_waitResponseCmd( NinaESP32::STOP_CLIENT_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen ) )
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
		}

		uint8_t getServerState( uint8_t sock )
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_STATE_TCP_CMD, NinaESP32::PARAM_NUMS_1);
			SpiDrv_sendParam(&sock, sizeof(sock), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::GET_STATE_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
			{
				__NINA_DEBUG_WARN("error waitResponse");
			}
			SpiDrv_spiSlaveDeselect();
		   return _data;
		}

		uint8_t getClientState( uint8_t sock )
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_CLIENT_STATE_TCP_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam(&sock, sizeof(sock), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::GET_CLIENT_STATE_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
		   return _data;
		}

		uint8_t checkDataSent(uint8_t sock)
		{
			const uint16_t TIMEOUT_DATA_SENT = 25;
			uint16_t timeout = 0;
			uint8_t _data = 0;
			uint8_t _dataLen = 0;

			do
			{
				WAIT_FOR_SLAVE_SELECT();
				// Send Command
				SpiDrv_sendCmd( NinaESP32::DATA_SENT_TCP_CMD, NinaESP32::PARAM_NUMS_1 );
				SpiDrv_sendParam( &sock, sizeof(sock), NinaESP32::LAST_PARAM );

				// pad to multiple of 4
				SpiDrv_readChar();
				SpiDrv_readChar();

				SpiDrv_spiSlaveDeselect();
				//Wait the reply elaboration
				SpiDrv_waitForSlaveReady();
				SpiDrv_spiSlaveSelect();

				// Wait for reply
				if (!SpiDrv_waitResponseCmd( NinaESP32::DATA_SENT_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen ))
					__NINA_DEBUG_WARN("error waitResponse isDataSent");

				SpiDrv_spiSlaveDeselect();

				if (_data)
					timeout = 0;

				else
				{
					++timeout;
					delay(100);
				}

			}
			while((_data==0)&&(timeout<TIMEOUT_DATA_SENT));

			return (timeout==TIMEOUT_DATA_SENT)? 0 : 1;
		}

		int SocketBuffer_read( int socket, uint8_t* data, size_t length )
		{
			int avail = SocketBuffer_available(socket);

			if (!avail)
				return 0;

			if (avail < (int)length)
				length = avail;

			memcpy(data, _buffers[socket].head, length);
			_buffers[socket].head += length;
			_buffers[socket].length -= length;

			return length;
		}

		bool insertDataBuf(uint8_t sock, const uint8_t *data, uint16_t _len)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::INSERT_DATABUF_CMD, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendBuffer(&sock, sizeof(sock));
			SpiDrv_sendBuffer((uint8_t *)data, _len, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			int commandSize = 9 + _len;
			while (commandSize % 4)
			{
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseData8( NinaESP32::INSERT_DATABUF_CMD, &_data, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
			if (_dataLen!=0)
				return (_data == 1);

			return false;
		}

		uint16_t sendData(uint8_t sock, const uint8_t *data, uint16_t len)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SEND_DATA_TCP_CMD, NinaESP32::PARAM_NUMS_2 );
			SpiDrv_sendBuffer(&sock, sizeof(sock));
			SpiDrv_sendBuffer((uint8_t *)data, len, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			int commandSize = 9 + len;
			while (commandSize % 4)
			{
				SpiDrv_readChar();
				commandSize++;
			}

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint16_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseData8( NinaESP32::SEND_DATA_TCP_CMD, (uint8_t*)&_data, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();

			return _data;
		}

		bool sendUdpData(uint8_t sock)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::SEND_DATA_UDP_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam(&sock, sizeof(sock), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseData8( NinaESP32::SEND_DATA_UDP_CMD, &_data, &_dataLen))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
			if (_dataLen!=0)
				return (_data == 1);

			return false;
		}

		// Get remote Data information on UDP socket
		void getRemoteData(uint8_t sock, uint8_t *ip, uint8_t *port)
		{
			tParam params[ NinaESP32::PARAM_NUMS_2 ] = { {0, (char*)ip}, {0, (char*)port} };

			WAIT_FOR_SLAVE_SELECT();

			// Send Command
			SpiDrv_sendCmd( NinaESP32::GET_REMOTE_DATA_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam(&sock, sizeof(sock), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			SpiDrv_waitResponseParams( NinaESP32::GET_REMOTE_DATA_CMD, NinaESP32::PARAM_NUMS_2, params);

			SpiDrv_spiSlaveDeselect();
		}

		// Start server TCP on port specified
		void startServer( uint16_t port, uint8_t sock, uint8_t protMode=NinaESP32::TCP_MODE )
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::START_SERVER_TCP_CMD, NinaESP32::PARAM_NUMS_3 );
			SpiDrv_sendParam(port);
			SpiDrv_sendParam(&sock, 1);
			SpiDrv_sendParam(&protMode, 1, NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;

			if (!SpiDrv_waitResponseCmd( NinaESP32::START_SERVER_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen ))
			  __NINA_DEBUG_WARN("No response to server start");

			SpiDrv_spiSlaveDeselect();
		}

		void startServer(uint32_t ipAddress, uint16_t port, uint8_t sock, uint8_t protMode=NinaESP32::TCP_MODE)
		{
			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::START_SERVER_TCP_CMD, NinaESP32::PARAM_NUMS_4 );
			SpiDrv_sendParam((uint8_t*)&ipAddress, sizeof(ipAddress));
			SpiDrv_sendParam(port);
			SpiDrv_sendParam(&sock, 1);
			SpiDrv_sendParam(&protMode, 1, NinaESP32::LAST_PARAM);

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _data = 0;
			uint8_t _dataLen = 0;
			if (!SpiDrv_waitResponseCmd( NinaESP32::START_SERVER_TCP_CMD, NinaESP32::PARAM_NUMS_1, &_data, &_dataLen ))
				__NINA_DEBUG_WARN("error waitResponse");

			SpiDrv_spiSlaveDeselect();
		}

		uint16_t availData(uint8_t sock)
		{
			if ( ! C_OWNER.SpiDrv_available() )
				return 0;

			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::AVAIL_DATA_TCP_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam(&sock, sizeof(sock), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			uint16_t len = 0;

			SpiDrv_waitResponseCmd( NinaESP32::AVAIL_DATA_TCP_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&len,  &_dataLen );

			SpiDrv_spiSlaveDeselect();

			return len;
		}

		uint8_t availServer(uint8_t sock)
		{
			if (! C_OWNER.SpiDrv_available() )
				return NinaESP32::NO_SOCKET_AVAIL;

			WAIT_FOR_SLAVE_SELECT();
			// Send Command
			SpiDrv_sendCmd( NinaESP32::AVAIL_DATA_TCP_CMD, NinaESP32::PARAM_NUMS_1 );
			SpiDrv_sendParam(&sock, sizeof(sock), NinaESP32::LAST_PARAM);

			// pad to multiple of 4
			SpiDrv_readChar();
			SpiDrv_readChar();

			SpiDrv_spiSlaveDeselect();
			//Wait the reply elaboration
			SpiDrv_waitForSlaveReady();
			SpiDrv_spiSlaveSelect();

			// Wait for reply
			uint8_t _dataLen = 0;
			uint16_t socket = 0;

			SpiDrv_waitResponseCmd( NinaESP32::AVAIL_DATA_TCP_CMD, NinaESP32::PARAM_NUMS_1, (uint8_t*)&socket,  &_dataLen );

			SpiDrv_spiSlaveDeselect();

			return socket;
		}

		void SocketBuffer_Init()
		{
			memset(&_buffers, 0x00, sizeof(_buffers));
		}

//	public:
//		inline void BusyInputPin_o_Receive( void *_Data )
//		{
//		}

	public:
/*
		inline void SystemInit()
		{
		}
*/
		inline void SystemStart()
		{
			SocketBuffer_Init();
			FLastTime = millis() + 30000;

			T_RemoteConnectedOutputPin::SetPinValue( false, false );
			UpdateLowPower();
			UpdateEnabled();

			if( T_FirmwareVersionOutputPin::GetPinIsConnected() )
				T_FirmwareVersionOutputPin::SetPinValue( firmwareVersion() );


//			if( Enabled() )
//				StartEthernet();

		}

		inline void SystemLoopBegin()
		{
//			Serial.print( "STATUS: " );
//			Serial.println( getConnectionStatus() );

			if( FScanning() )
			{
//				Serial.println( "SCANNING" );
				bool AScanCompleted = false;
				T_Operations_ScanDurationCompleted::Call( millis() - FLastTime, AScanCompleted );
//				if( millis() - FLastTime > 10000 )
				if( AScanCompleted )
				{
//					Serial.println( "GET RESULTS" );
					WiFiDrv_getScanNetworks();
				}
			}

			else if( C_OWNER.isSlaveReady() )
			{
				bool AIsEnabled = false;

				uint8_t ARes = getConnectionStatus();
//				if( ! WiFi_isConnected() )
				if(	ARes != NinaESP32::WL_CONNECTED )
				{
					if( FRemoteStarted() )
						FLastTime = millis();

//					Serial.println( "! WiFi.isConnected()" );
	//				FIsStarted() = false;
					FRemoteStarted() = false;
//					FDNSUpdated() = false;
				}

				if( C_ACESS_POINTS_COUNT > 0 )
					if( ! FRemoteStarted() )
					{
//						Serial.println( ARes );
//						if( ( ARes != NinaESP32::WL_CONNECTED ) && ( ARes != NinaESP32::WL_DISCONNECTED ) && ( ARes != NinaESP32::WL_IDLE_STATUS ) )
						if( ( ARes != NinaESP32::WL_CONNECTED ) && ( ARes != NinaESP32::WL_DISCONNECTED ) )
//						if( ( ARes != NinaESP32::WL_CONNECTED ) && ( ARes != NinaESP32::WL_AP_LISTENING ) )
						{
//					Serial.println( "TEST1" );
							unsigned long ATime = millis();
							if( ( ATime - FLastTime ) > 30000 )
							{
	#ifdef __ESP32_WIFI__DEBUG__
								Serial.print( "STATUS: " );
								Serial.println( ARes );
								Serial.println( FAccessPointIndex().GetValue() );
	#endif // __ESP32_WIFI__DEBUG__
								 FAccessPointIndex() = FAccessPointIndex() + 1;
								if( FAccessPointIndex() > C_ACESS_POINTS_COUNT )
									FAccessPointIndex() = 1;

								T_AccessPoints_TryConnect::Call( FAccessPointIndex().GetValue() - 1, AIsEnabled );
								FLastTime = millis();
							}
						}

						else
							T_AccessPoints_IsEnabled::Call( FAccessPointIndex().GetValue() - 1, AIsEnabled );

					}

				T_RemoteConnectedOutputPin::SetPinValue( WiFi_isConnected(), true );

//				Serial.println( AIsEnabled );

				if( ! AIsEnabled )
					FIsStarted() = true;

				else if( WiFi_isConnected() )
					if( ! FRemoteStarted() )
					{
//						Serial.println( "! FRemoteStarted()" );

						if( ! FDNSUpdated() )
						{
	#ifdef __ESP32_WIFI__DEBUG__
							Serial.println( "DNS Updated" );
	#endif // __ESP32_WIFI__DEBUG__
							FDNSUpdated() = true;
	//							bool AIsEnabled = false;
////							T_AccessPoints_AssignDNS::Call( FAccessPointIndex() - 1 );

							return;
						}

						else
						{
							FIsStarted() = true;
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
						if( T_AddressOutputPin::GetPinIsConnected() || T_GatewayIPOutputPin::GetPinIsConnected() || T_SubnetMaskIPOutputPin::GetPinIsConnected() )
						{
							uint8_t  AlocalIp[ NinaESP32::WL_IPV4_LENGTH ];
							uint8_t  AsubnetMask[ NinaESP32::WL_IPV4_LENGTH ];
							uint8_t  AgatewayIp[ NinaESP32::WL_IPV4_LENGTH ];
							getNetworkData( AlocalIp, AsubnetMask, AgatewayIp );

							if( T_AddressOutputPin::GetPinIsConnected() )
								T_AddressOutputPin::SetPinValue( IPAdressToString( AlocalIp ));

							if( T_GatewayIPOutputPin::GetPinIsConnected() )
								T_GatewayIPOutputPin::SetPinValue( IPAdressToString( AgatewayIp ) );

							if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
								T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( AsubnetMask ) );

						}

						if( MACOutputPin().GetPinIsConnected() )
						{
							uint8_t mac[6];
							macAddress(mac);
							T_MACOutputPin::SetPinValue( MACAdressToString( mac ) );
						}

						if( T_BSSIDOutputPin::GetPinIsConnected() )
						{
							uint8_t mac[6];
							BSSID( mac );

							T_BSSIDOutputPin::SetPinValue( MACAdressToString( mac ) );
						}

					}

			}

/*
			if( FConnected() )
			{
				if( millis() - FLastTime > 5000 )
				{
					if( C_OWNER.isSlaveReady() )
					{
						uint8_t AStatus = getConnectionStatus();
						switch( AStatus )
						{
							case WL_CONNECT_FAILED:
							case WL_CONNECTION_LOST:
							case WL_DISCONNECTED:
								FConnected() = false;
								break;

							case WL_CONNECTED:
							case WL_AP_CONNECTED
								FConnected() = true;
								break;

						}
					}
				}

			}

			else
			{
				if( FRemoteStarted() )
					FLastReconnectTime = millis();

//				Serial.println( "WiFi.isConnected()" );
//				FIsStarted() = false;
				FRemoteStarted() = false;
				FDNSUpdated() = false;

			}

//			int ARes = WiFi.status();

//			Serial.print( "STATUS: " );
//			Serial.println( ARes );

//			bool AIsEnabled = false; //( C_ACESS_POINTS_COUNT == 0 );
*/
		}

	public:
		inline bool GetIsStarted() { return FIsStarted(); }

	public:
		inline void WiFi_disconnect()
		{
			disconnect();
//			WiFi.disconnect();
		}

		inline void WiFi_reconnect()
		{
//			WiFi.reconnect();
		}

		inline void WiFi_config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2)
		{
			WiFiDrv_config(3, (uint32_t)local_ip, (uint32_t)gateway, (uint32_t)subnet);
			WiFiDrv_setDNS(2, (uint32_t)dns1, (uint32_t)dns2);
//			WiFi.config(local_ip, gateway, subnet, dns1, dns2);
		}

		inline void WiFi_setHostname(const char * hostname)
		{
			setHostname( hostname );
//			WiFi.setHostname( hostname );
		}

		inline IPAddress WiFi_gatewayIP()
		{
			IPAddress ret;
			WiFiDrv_getGatewayIP(ret);
			return ret;
//			return WiFi.gatewayIP();
		}

		inline void WiFi_begin(const char* ssid, const char *passphrase, int32_t channel, const uint8_t* bssid, bool connect)
		{
			begin( ssid, passphrase );
//			beginAP( ssid, passphrase, channel ); //, connect );
//			WiFi.begin( ssid, passphrase, channel, bssid, connect );
		}

	public:
		inline Nina_WiFi()
		{
			FAccessPointIndex() = C_ACESS_POINTS_COUNT;
			FConnected() = false;
			FScanning() = false;
			FGetChannel() = false;
			FGetMACAddress() = false;
			FGetSignalStrengths() = false;
			FGetEncriptions() = false;
			FRemoteStarted() = false;
			FDNSUpdated() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

