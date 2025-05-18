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
#include <IPAddress.h>
#include <Mitov_Sockets.h>
#if defined( VISUINO_ESP8266 )
	#include <WiFiClientSecure.h>
#endif
//#include <Ethernet.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#define Console UNGUARDED DEBUG PRINT!!!
#endif

// #define __TCP_SERVER__DEBUG__
// #define __TCP_CLIENT__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	inline Mitov::String MACAdressToString( const uint8_t *AMACAddress )
	{
#ifdef VISUINO_ARDUINO_PORTENTA
		char AMACString[ 6 * 3 + 15 ];
#else
		char AMACString[ 6 * 3 + 1 ];
#endif
		sprintf( AMACString, "%02X-%02X-%02X-%02X-%02X-%02X", AMACAddress[ 0 ], AMACAddress[ 1 ], AMACAddress[ 2 ], AMACAddress[ 3 ], AMACAddress[ 4 ], AMACAddress[ 5 ] );
		return Mitov::String( AMACString );
//		MACOutputPin.Notify( AMACString );
	}
//---------------------------------------------------------------------------
	inline Mitov::String IPAdressToString( ::IPAddress AIPAddress )
	{
#ifdef VISUINO_ARDUINO_PORTENTA
		char AIPString[ 4 * 4 + 15 ];
#else
		char AIPString[ 4 * 4 + 1 ];
#endif
		sprintf( AIPString, "%u.%u.%u.%u", AIPAddress[ 0 ], AIPAddress[ 1 ], AIPAddress[ 2 ], AIPAddress[ 3 ] );
		return AIPString;
	}
//---------------------------------------------------------------------------
/*
	class BasicShieldIPAddress
	{
	public:
		bool		Enabled = false;
		::IPAddress	IP;

	};
//---------------------------------------------------------------------------
	class ShieldGatewayAddress : public BasicShieldIPAddress
	{
	public:
		BasicShieldIPAddress	Subnet;
	};
//---------------------------------------------------------------------------
	class ShieldDNSAddress : public BasicShieldIPAddress
	{
	public:
		ShieldGatewayAddress	Gateway;
	};
//---------------------------------------------------------------------------
	class ShieldIPAddress : public BasicShieldIPAddress
	{
	public:
		ShieldDNSAddress	DNS;
	};
//---------------------------------------------------------------------------
	class ShieldIPDNS2Address : public ShieldIPAddress
	{
	public:
		BasicShieldIPAddress	DNS2;
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Octet1,
		typename T_Octet2,
		typename T_Octet3,
		typename T_Octet4
	> class TIPAddress :
		public T_Octet1,
		public T_Octet2,
		public T_Octet3,
		public T_Octet4
	{
	public:
		_V_PROP_( Octet1 )
		_V_PROP_( Octet2 )
		_V_PROP_( Octet3 )
		_V_PROP_( Octet4 )

	public:
		void operator = ( uint32_t AValue )
		{
			Octet4() = AValue;
			Octet3() = AValue >> 8;
			Octet2() = AValue >> 16;
			Octet1() = AValue >> 24;
		}

		inline uint32_t GetUInt32Value()
		{
			return ( uint32_t( Octet1() ) << 24 ) | ( uint32_t( Octet2() ) << 16 ) | ( uint32_t( Octet3() ) << 8 ) | Octet4();
		}

		String AsText()
		{
			return String( Octet1() ) + "." + String( Octet2() ) + "." + String( Octet3() ) + "." + String( Octet4() );
		}
/*
		bool operator == ( uint32_t AValue )
		{
			if( Octet4 != AValue & 0xFF )
				return false;

			if( Octet3 != ( AValue >> 8 ) & 0xFF )
				return false;

			if( Octet2 != ( AValue >> 16 ) & 0xFF )
				return false;

			if( Octet1 != ( AValue >> 24 ) & 0xFF )
				return false;

			return true;
		}
*/
	public:
		inline ::IPAddress GetIPAddress()
		{
			return ::IPAddress( Octet1(), Octet2(), Octet3(), Octet4() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IP
	> class TArduinoBasicIPV4Address :
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IP )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IP,
		typename T_Subnet
	> class TArduinoIPV4GatewayDNSAddress :
		public T_Enabled,
		public T_IP,
		public T_Subnet
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IP )
		_V_PROP_( Subnet )
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Gateway,
		typename T_IP
	> class TArduinoIPV4DNSAddress :
		public T_Enabled,
		public T_Gateway,
		public T_IP
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Gateway )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IP
	> class TArduinoSubnetIPV4Address :
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
	template <
		typename T_DNS,
		typename T_Enabled,
		typename T_IP
	> class TArduinoIPV4DNS1Address :
		public T_DNS,
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( DNS )
		_V_PROP_( Enabled )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
	template <
		typename T_DNS,
		typename T_DNS2,
		typename T_Enabled,
		typename T_IP
	> class TArduinoIPV4DNS2Address :
		public T_DNS,
		public T_DNS2,
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( DNS )
		_V_PROP_( DNS2 )
		_V_PROP_( Enabled )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
	template <
		typename T_CLIENT
	> class BasicTCPClientSocket_Implementation
	{
	protected:
		T_CLIENT FClient;

	public:
		inline void ApplyStartParameters() {}

		inline bool GetIsSecure() { return false; }


	};
//---------------------------------------------------------------------------
	template <
		typename T_CLIENT,
		typename T_CACert,
		typename T_DisableSNI
	> class SSLTCPClientSocket_Implementation :
		public T_CACert,
		public T_DisableSNI
	{
	protected:
		T_CLIENT FClient;

	public:
		_V_PROP_( CACert )
		_V_PROP_( DisableSNI )

		inline void ApplyStartParameters()
		{
			if( CACert().GetValue() != "" )
				FClient.appendCustomCACert( CACert().c_str() );

			else
				FClient.appendCustomCACert( nullptr );

			FClient.disableSNI( DisableSNI().GetValue() );
		}

		inline bool GetIsSecure() { return true; }

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FIsConnected,
		typename T_Host,
		typename T_IPAddress,
		typename T_OutputPin,
		typename T_Port,
		typename T_ReconnectDelay
	> class TCPClientSocket :
		public T_IMPLEMENTATION,
		public T_ConnectedOutputPin,
		public T_Enabled,
		public T_FIsConnected,
		public T_Host,
		public T_IPAddress,
		public T_OutputPin,
		public T_Port,
		public T_ReconnectDelay
	{
		typedef T_IMPLEMENTATION inherited;

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )
		_V_PROP_( ReconnectDelay )

	protected:
		_V_PROP_( FIsConnected )

	public:
		_V_PROP_( Host )
		_V_PROP_( IPAddress )

	protected:
		unsigned long FLastConnectTime;

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
                ForceStartSocket();

			else
				StopSocket();

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
                ForceStartSocket();

		}

        void ForceStartSocket()
        {
            FLastConnectTime = millis() + ReconnectDelay().GetValue();
            StartSocket();
        }

		void ForceStart() // For direct socket access components
		{
  			FLastConnectTime = millis() + ReconnectDelay().GetValue();
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
		inline void FlushInputPin_o_Receive( void *_Data )
		{
			inherited::FClient.flush();
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

		inline Print &GetPrint()
		{
			return inherited::FClient;
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			inherited::FClient.flush();
			inherited::FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
			Enabled() = false;
//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

	public:
		void StartSocket()
		{
			if( ! C_OWNER.GetIsStarted() )
				return;

//#ifdef __TCP_CLIENT__DEBUG__
//			Serial.println( "StartSocket1" );
//#endif
			if( FIsConnected() )
				return;

			if( ! Enabled().GetValue() )
				return;

			if( millis() - FLastConnectTime < ReconnectDelay().GetValue() )
				return;

			inherited::ApplyStartParameters();

#ifdef __TCP_CLIENT__DEBUG__
			Serial.println( "StartSocket" );
#endif
			if( Host().GetValue().length() )
				FIsConnected() = inherited::FClient.connect( Host().c_str(), Port() );

			else if( IPAddress().GetUInt32Value() != 0 )
			{
//				IPAddress.printTo( Serial );
				FIsConnected() = inherited::FClient.connect( IPAddress().GetIPAddress(), Port() );
			}

            FLastConnectTime = millis();
		}

		void StopSocket()
		{
			if( ! FIsConnected() )
				return;

#ifdef __TCP_CLIENT__DEBUG__
			Serial.println( "StopSocket" );
#endif
			FIsConnected() = false;

			inherited::FClient.flush();
			inherited::FClient.stop();
			Enabled() = false;
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
		}

	public:
		inline void SystemLoopBegin()
		{
			StartSocket();
			for( int i = 0; i < 20; ++i )
			{
				if( ! inherited::FClient.available() )
					break;

				unsigned char AByte = inherited::FClient.read();
				T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
			}

			bool AIsConnected = inherited::FClient.connected();

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			if( ! AIsConnected )
			{
				while( inherited::FClient.available() )
				{
					unsigned char AByte = inherited::FClient.read();
					T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
				}

//				FClient.stop(); // Do we need this?
				T_ConnectedOutputPin::SetPinValue( false, true );

				if( FIsConnected() )
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
					FIsConnected() = false;
				}

			}
		}

	public:
		inline TCPClientSocket()
		{
			FLastConnectTime = millis() + ReconnectDelay().GetValue();
			FIsConnected() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CLIENT,
		typename T_Certificate,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
//		typename T_FConnecting,
		typename T_FIsConnected,
//		typename T_FRunning,
		typename T_Host,
		typename T_IPAddress,
		typename T_OutputPin,
		typename T_Port,
		typename T_PrivateKey,
		typename T_ReconnectDelay
	> class TCPSecureClientSocket :
		public T_Certificate,
		public T_ConnectedOutputPin,
		public T_Enabled,
//		public T_FConnecting,
		public T_FIsConnected,
//		public T_FRunning,
		public T_Host,
		public T_IPAddress,
		public T_OutputPin,
		public T_Port,
		public T_PrivateKey,
		public T_ReconnectDelay
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )
		_V_PROP_( ReconnectDelay )

	protected:
//		_V_PROP_( FRunning )
//		_V_PROP_( FConnecting )
		_V_PROP_( FIsConnected )
//		bool			FResolved : 1;

	public:
		_V_PROP_( Host )
		_V_PROP_( IPAddress )

	public:
		_V_PROP_( Certificate )
		_V_PROP_( PrivateKey )

	protected:
		T_CLIENT FClient;
		unsigned long FLastConnectTime;

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
                ForceStartSocket();

			else
				StopSocket();

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
			FClient.flush();
		}

	public:
		inline bool GetIsSecure() { return true; }

		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				ForceStartSocket();
		}

		void ForceStart() // For direct socket access components
		{
            FLastConnectTime = millis() + ReconnectDelay().GetValue();
			Enabled() = true;
			TryStartSocket();
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{

//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
/*
			Serial.print( "CanSend: Enabled() = " );
			Serial.print( Enabled().GetValue() );
			Serial.print( " C_OWNER.Enabled() = " );
			Serial.print( C_OWNER.Enabled().GetValue() );
			Serial.print( " FIsConnected() = " );
			Serial.print( FIsConnected().GetValue() );
			Serial.print( " C_OWNER.GetIsStarted() = " );
			Serial.print( C_OWNER.GetIsStarted() );
			Serial.println();
*/
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

		inline bool GetReadyToConnect()
		{
			return C_OWNER.Enabled() && C_OWNER.GetIsStarted();
		}

		inline Print &GetPrint()
		{
			return FClient;
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
			Enabled() = false;

//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

	public:
		void StopSocket()
		{
			if( ! FIsConnected() )
				return;

			FIsConnected() = false;

			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
		}

	public:
		inline void SystemLoopBegin()
		{
			StartSocket();
			for( int i = 0; i < 20; ++i )
			{
				if( ! FClient.available() )
					break;

				unsigned char AByte = FClient.read();
				T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
			}

			bool AIsConnected = FClient.connected();

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			if ( ! AIsConnected )
			{
				while ( FClient.available() )
				{
					unsigned char AByte = FClient.read();
					T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
				}

//				FClient.stop(); // Do we need this?
				T_ConnectedOutputPin::SetPinValue( false, true );

				if( FIsConnected() )
				{
					if( C_OWNER.Enabled() )
						Enabled() = false;

					FIsConnected() = false;
				}
			}
		}

	protected:
        void ForceStartSocket()
        {
            FLastConnectTime = millis() + ReconnectDelay().GetValue();
            StartSocket();
        }

		inline void StartSocket()
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			if( FIsConnected() )
				return;

			if( millis() - FLastConnectTime < ReconnectDelay().GetValue() )
				return;

			if( Certificate().GetValue() != "" )
			{
#if defined( VISUINO_ESP8266 )
				FIsConnected() = FClient.setCertificate( (uint8_t*)Certificate().c_str(), Certificate().length() );
#else
				FIsConnected() = FClient.setCertificate( Certificate().c_str() );
#endif
				return;
			}

			if( PrivateKey != "" )
			{
#if defined( VISUINO_ESP8266 )
				FIsConnected() = FClient.setPrivateKey( (uint8_t*)PrivateKey().c_str(), PrivateKey().length() );
#else
				FIsConnected() = FClient.setPrivateKey( PrivateKey().c_str() );
#endif
				return;
			}

//			Serial.println( "StartSocket1" );
			if( FIsConnected() )
				return;

//			Serial.println( "StartSocket" );
			if( Host().length() )
				FIsConnected() = FClient.connect( Host().c_str(), Port() );

			else
			{
//				IPAddress.printTo( Serial );
				FIsConnected() = FClient.connect( IPAddress().GetIPAddress(), Port() );
			}

            FLastConnectTime = millis();
		}

	public:
		inline TCPSecureClientSocket()
		{
			FLastConnectTime = millis() + ReconnectDelay().GetValue();
//			FRunning() = false;
//			FConnecting() = false;
			FIsConnected() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
#if defined( VISUINO_ESP8266 )
		typename T_AllowSelfSigned,
#endif
		typename T_CACert,
		typename T_CLIENT,
#if ! defined( VISUINO_ESP8266 )
		typename T_Certificate,
#endif
		typename T_ConnectedOutputPin,
		typename T_Enabled,
//		typename T_FConnecting,
		typename T_FIsConnected,
//		typename T_FRunning,
#if defined( VISUINO_ESP8266 )
		typename T_Fingerprint,
#endif
		typename T_Host,
		typename T_IPAddress,
		typename T_Insecure,
		typename T_OutputPin,
		typename T_Port,
#if ! defined( VISUINO_ESP8266 )
		typename T_PrivateKey,
#endif
		typename T_ReconnectDelay
	> class TCPCACertSecureClientSocket :
#if defined( VISUINO_ESP8266 )
		public T_AllowSelfSigned,
#endif
		public T_CACert,
#if ! defined( VISUINO_ESP8266 )
		public T_Certificate,
#endif
		public T_ConnectedOutputPin,
		public T_Enabled,
//		public T_FConnecting,
		public T_FIsConnected,
//		public T_FRunning,
#if defined( VISUINO_ESP8266 )
		public T_Fingerprint,
#endif
		public T_Host,
		public T_IPAddress,
		public T_Insecure,
		public T_OutputPin,
		public T_Port,
#if ! defined( VISUINO_ESP8266 )
		public T_PrivateKey,
#endif
		public T_ReconnectDelay
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )
		_V_PROP_( Insecure )
		_V_PROP_( ReconnectDelay )

	protected:
//		_V_PROP_( FRunning )
//		_V_PROP_( FConnecting )
		_V_PROP_( FIsConnected )
//		bool			FResolved : 1;

	public:
		_V_PROP_( Host )
		_V_PROP_( IPAddress )

	public:
#if defined( VISUINO_ESP8266 )
		_V_PROP_( AllowSelfSigned )
		_V_PROP_( Fingerprint )
#else
		_V_PROP_( Certificate )
		_V_PROP_( PrivateKey )
#endif

		_V_PROP_( CACert )

	protected:
		T_CLIENT FClient;
#if defined( VISUINO_ESP8266 )
		X509List *FX509List = nullptr;
#endif
		unsigned long FLastConnectTime;

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
				ForceStartSocket();

			else
				StopSocket();

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
		inline bool GetIsSecure() { return true; }

		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				ForceStartSocket();
		}

        void ForceStartSocket()
        {
//            Serial.println( "ForceStartSocket" );
            FLastConnectTime = millis() + ReconnectDelay().GetValue();
            StartSocket();
        }

		void ForceStart() // For direct socket access components
		{
//            Serial.println( "ForceStart" );
			Enabled() = true;
            FLastConnectTime = millis() + ReconnectDelay().GetValue();
			TryStartSocket();
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

	public:
		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
/*
            static bool ALastEnabled = false;
            static bool ALastOwnerEnabled = false;
            static bool AFIsConnected = false;
            static bool AGetIsStarted = false;
            if( ( ALastEnabled != Enabled().GetValue() ) || ( ALastOwnerEnabled != C_OWNER.Enabled().GetValue() ) || ( AFIsConnected != FIsConnected().GetValue() ) || ( AGetIsStarted != C_OWNER.GetIsStarted() ))
            {
                Serial.print( "CanSend: Enabled() = " );
                Serial.print( Enabled().GetValue() );
                Serial.print( " C_OWNER.Enabled() = " );
                Serial.print( C_OWNER.Enabled().GetValue() );
                Serial.print( " FIsConnected() = " );
                Serial.print( FIsConnected().GetValue() );
                Serial.print( " C_OWNER.GetIsStarted() = " );
                Serial.print( C_OWNER.GetIsStarted() );
                Serial.println();
            }

            ALastEnabled = Enabled().GetValue();
            ALastOwnerEnabled = C_OWNER.Enabled().GetValue();
            AFIsConnected = FIsConnected().GetValue();
            AGetIsStarted = C_OWNER.GetIsStarted();
*/
/*
			Serial.print( "CanSend: Enabled() = " );
			Serial.print( Enabled().GetValue() );
			Serial.print( " C_OWNER.Enabled() = " );
			Serial.print( C_OWNER.Enabled().GetValue() );
			Serial.print( " FIsConnected() = " );
			Serial.print( FIsConnected().GetValue() );
			Serial.print( " C_OWNER.GetIsStarted() = " );
			Serial.print( C_OWNER.GetIsStarted() );
			Serial.println();
*/
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

		inline bool GetReadyToConnect()
		{
			return C_OWNER.Enabled() && C_OWNER.GetIsStarted();
		}

		inline Print &GetPrint()
		{
			return FClient;
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
			Enabled() = false;

//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

	public:
		void StopSocket()
		{
			if( ! FIsConnected() )
				return;

			FIsConnected() = false;

			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
#if defined( VISUINO_ESP8266 )
			if( FX509List )
			{
				delete FX509List;
				FX509List = nullptr;
			}
#endif
//			inherited::Enabled = false;
		}

	public:
		inline void SystemLoopBegin()
		{
			StartSocket();
			if ( FClient.available() )
			{
				unsigned char AByte = FClient.read();
				T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.Notify( &AByte );
			}

			bool AIsConnected = FClient.connected();

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );
			if ( ! AIsConnected )
			{
				while ( FClient.available() )
				{
					unsigned char AByte = FClient.read();
					T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
				}

//				FClient.stop(); // Do we need this?
//				T_ConnectedOutputPin::SetPinValue( false, true );

				if( FIsConnected() )
				{
#if defined( VISUINO_ESP8266 )
					if( FX509List )
					{
						delete FX509List;
						FX509List = nullptr;
					}
#endif

//					Serial.println( "DISCONNECT" );

					if( C_OWNER.Enabled() )
						Enabled() = false;

					FIsConnected() = false;
				}
			}
		}

	protected:
		inline void StartSocket()
		{
			if( ! Enabled().GetValue() )
				return;

//			Serial.println( "StartSocket 1" );
			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "StartSocket 2" );
			if( FIsConnected() )
				return;

//			Serial.println( "StartSocket 3" );
			if( millis() - FLastConnectTime < ReconnectDelay().GetValue() )
				return;

//			Serial.println( "StartSocket 4" );
			if( CACert().GetValue() != "" )
#if defined( VISUINO_ESP8266 )
//				FClient.setCACert( (uint8_t *)CACert().c_str(), CACert().GetValue().length() );
				if( FX509List )
					delete FX509List;

				FX509List = new X509List( (char *)CACert().c_str() );
//				Serial.println( CACert().c_str() );
				FClient.setTrustAnchors( FX509List );
#else
				FClient.setCACert( (char *)CACert().c_str() );
#endif

#if defined( VISUINO_ESP8266 )
			if( Fingerprint().Enabled() )
			{
//				if ( Fingerprint().Key().GetCount() )
//				{
//				uint8_t *ABuffer = new uint8_t[ 16 ];
//				memcpy_P( ABuffer, EncryptionKey, 16 );
//				spiBurstWrite( RH_RF69_REG_3E_AESKEY1, ABuffer, 16 );
//				delete [] ABuffer;
                uint8_t *AKey = Fingerprint().Key().Allocate();

				uint8_t ALocalKey[ 20 ];
				memcpy( ALocalKey, AKey, 20 );
                Fingerprint().Key().Release( AKey );

				FClient.setFingerprint( ALocalKey );
//				}
			}

			if( AllowSelfSigned() )
				FClient.allowSelfSignedCerts();

#else
			if( Certificate().GetValue() != "" )
			{
//#if defined( VISUINO_ESP8266 )
////				FClient.setCertificate( (uint8_t*)Certificate().c_str(), Certificate().GetValue().length() );
//#else
//				FIsConnected() = FClient.setCertificate( Certificate().c_str() );
				FClient.setCertificate( Certificate().c_str() );
//#endif
			}

			if( PrivateKey().GetValue() != "" )
			{
//#if defined( VISUINO_ESP8266 )
//				FClient.setPrivateKey( (uint8_t*)Certificate().c_str(), Certificate().GetValue().length() );
//#else
//				FIsConnected() = FClient.setPrivateKey( PrivateKey().c_str() );
				FClient.setPrivateKey( PrivateKey().c_str() );
//#endif
			}
#endif

			if( Insecure() )
				FClient.setInsecure();

			else
#if defined( VISUINO_ESP8266 )
//				FClient._ctx->_use_insecure = false;
#else
				FClient._use_insecure = false;
#endif


//			Serial.println( "StartSocket1" );
//			if( FIsConnected() )
//				return;

//			Serial.println( "StartSocket" );
			if( Host().GetValue().length() )
				FIsConnected() = FClient.connect( Host().c_str(), Port() );

			else if( IPAddress().GetUInt32Value() != 0 )
			{
//				IPAddress.printTo( Serial );
				FIsConnected() = FClient.connect( IPAddress().GetIPAddress(), Port() );
			}

            FLastConnectTime = millis();
		}

	public:
		inline TCPCACertSecureClientSocket()
		{
			FLastConnectTime = millis() + ReconnectDelay().GetValue();
//			FRunning() = false;
//			FConnecting() = false;
			FIsConnected() = false;
		}


	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CLIENT,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
//		typename T_FConnecting,
		typename T_FIsConnected,
//		typename T_FResolved,
//		typename T_FRunning,
#if defined( ESP8266 ) || defined( ESP32 )
		typename T_NoDelay,
#endif
		typename T_OpenedOutputPin,
		typename T_OutputPin,
		typename T_Port,
		typename T_SERVER
	> class TCPServerSocket :
		public T_ConnectedOutputPin,
		public T_Enabled,
//		public T_FConnecting,
		public T_FIsConnected,
//		public T_FResolved,
//		public T_FRunning,
#if defined( ESP8266 ) || defined( ESP32 )
		public T_NoDelay,
#endif
		public T_OpenedOutputPin,
		public T_OutputPin,
		public T_Port
	{
	public:
		_V_PIN_( OpenedOutputPin )
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )
#if defined( ESP8266 ) || defined( ESP32 )
		_V_PROP_( NoDelay )
#endif

	protected:
//		_V_PROP_( FRunning )
//		_V_PROP_( FConnecting )
		_V_PROP_( FIsConnected )
//		_V_PROP_( FResolved )

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
		inline void UpdateNoDelay()
		{
#if defined( ESP8266 ) || defined( ESP32 )
			FServer->setNoDelay( NoDelay().GetValue() );
#endif
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

		}

	protected:
		T_CLIENT	FClient;
		T_SERVER	*FServer = nullptr;

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
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
		void StartSocket()
		{
//			Serial.println( "StartSockect 1" );
			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "StartSockect 2" );
			if( FIsConnected() )
				return;

//			Serial.println( "StartSockect" );
			if( FServer )
			{
				if( ! *FServer )
				{
//					Console.println( "Try Restart" );
					FServer->begin();
				}

				return;
			}

#ifdef __TCP_SERVER__DEBUG__
			Serial.println( "TCPServer - StartSockect" );
			Serial.println( Port() );
#endif //__TCP_SERVER__DEBUG__
			FServer = new T_SERVER( Port() );
			FServer->begin();
			UpdateNoDelay();
//			FIsConnected() = true;
//			Serial.println( "Start Server Sockect" );
//			Serial.println( inherited::Port );
		}

	public:
		inline void SystemLoopBegin()
		{
			StartSocket();
			if( FServer )
			{
				if( T_OpenedOutputPin::GetPinIsConnected() )
					T_OpenedOutputPin::SetPinValue( ( *FServer ) != false, true );

				if( ! FClient )
				{
#ifdef __TCP_SERVER__DEBUG__
//					Serial.println( "TRY CLIENT" );
//					Serial.println( Port() );
#endif // __TCP_SERVER__DEBUG__
//					FClient = FServer->available();
					FClient = FServer->accept();

#ifdef __TCP_SERVER__DEBUG__
					if( FClient )
						Serial.println( "CLIENT" );
#endif // __TCP_SERVER__DEBUG__

//					Serial.println( "TRY CLIENT OUT" );
				}

/*
				if( inherited::FClient )
 					if (! inherited::FClient.connected())
					{
						Serial.println( "STOP" );
						inherited::FClient.stop(); // Do we need this?
					}
*/
//				if( FClient )
//				{
//					Serial.println( "CLIENT" );
					if( FClient.available() )
					{
						unsigned char AByte = FClient.read();
//				Serial.println( "RECEIVED" );
						T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//						OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
					}

 					if( ! FClient.connected() )
					{
						while( FClient.available() )
						{
							unsigned char AByte = FClient.read();
#ifdef __TCP_SERVER__DEBUG__
							Serial.println( "RECEIVED" );
#endif // __TCP_SERVER__DEBUG__
							T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
						}

//						Serial.println( "STOP" );
						FClient.stop(); // This is needed for the Arduino EthernetShield!
						T_ConnectedOutputPin::SetPinValue( false, true );
					}
//				}
			}

			else
				T_OpenedOutputPin::SetPinValue( false, true );

//			Serial.println( inherited::FClient.connected() );

			bool AIsConnected = FClient.connected();

//			Serial.println( FClient.status() );

#ifdef __TCP_SERVER__DEBUG__
			if( FIsConnected() != AIsConnected )
				Serial.println( AIsConnected );

#endif // __TCP_SERVER__DEBUG__

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			FIsConnected() = AIsConnected;

//			if( ! AIsConnected )
//				if( C_OWNER.Enabled )
//					inherited::Enabled = false;

//			Serial.println( "SYSTEM_LOOP" );
//			Serial.println( "SYSTEM_LOOP_OUT" );
		}

	public:
		inline bool IsConnected() { return FClient; }

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
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
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
			FClient.stop();
		}

		inline void StopSocket()
		{
#ifdef __TCP_SERVER__DEBUG__
			Serial.println( "TCPServer - StopSocket" );
#endif // __TCP_SERVER__DEBUG__
			FClient.stop();
			FIsConnected() = false;

			if( FServer )
			{
				delete FServer;
				FServer = nullptr;
			}
		}

	public:
		inline Print &GetPrint()
		{
//			Serial.println( "GetPrint" );
			if( FClient )
				return FClient;

//			Serial.println( "SERVER!!!" );
			return *FServer;
		}

/*
		void EndPacket()
		{
			Serial.println( "EndPacket" );
//			inherited::FClient.flush();
//			delay(1000);
//			inherited::FClient.stop();
		}
*/
	public:
		inline TCPServerSocket()
		{
//			FRunning() = false;
//			FConnecting() = false;
			FIsConnected() = false;
//			FResolved() = false;
		}


/*
		virtual ~TCPServerSocket()
		{
			if( FServer )
				delete FServer;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_UDP,
		typename T_Enabled,
		typename T_FIsConnected,
//		typename T_FResolved,
		typename T_OutputPin,
		typename T_Port,
		typename T_RemoteHost,
		typename T_RemoteIPAddress,
		typename T_RemoteIPOutputPin,
		typename T_RemotePort,
		typename T_RemotePortOutputPin
	> class UDPSocket :
		public T_Enabled,
		public T_FIsConnected,
//		public T_FResolved,
		public T_OutputPin,
		public T_Port,
		public T_RemoteHost,
		public T_RemoteIPAddress,
		public T_RemoteIPOutputPin,
		public T_RemotePort,
		public T_RemotePortOutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( RemoteIPOutputPin )
		_V_PIN_( RemotePortOutputPin )

	public:
		_V_PROP_( RemotePort )
		_V_PROP_( Port )
		_V_PROP_( RemoteHost )
		_V_PROP_( RemoteIPAddress )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FIsConnected )
//		_V_PROP_( FResolved )

	protected:
		T_UDP FSocket;
//		Mitov::String FHostName;

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
			FIsConnected() = FSocket.begin( Port() );
		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
			RemotePort() = APort;
		}

		inline void BeginPacket()
		{
			if( ! FIsConnected() )
				return;

//			Serial.print( "BeginPacket: " );
//			Serial.print( RemoteHost );
//			Serial.print( RemoteIPAddress );
//			Serial.print( " " );
//			Serial.println( RemotePort );
			if( RemoteHost().GetValue().length() )
				FSocket.beginPacket( RemoteHost().c_str(), RemotePort() );

			else if( RemoteIPAddress().GetUInt32Value() != 0 )
				FSocket.beginPacket( RemoteIPAddress().GetIPAddress(), RemotePort() );

////  FSocket.beginPacket( RemoteIPAddress, 8888 );
//  FSocket.println( "Hello1" );
////  FSocket.endPacket();
		}

		inline void EndPacket()
		{
			if( ! FIsConnected() )
				return;

//			Serial.println( "EndPacket" );
//			FSocket.println( "Hello" );
			FSocket.endPacket();
//			delay( 1000 );
		}

		inline void StopSocket()
		{
			FIsConnected() = false;

//			Serial.println( "StopSocket" );
			FSocket.stop();
		}

		inline Print &GetPrint()
		{
//  FSocket.println( "Hello2" );
//			return &Serial;
//			Serial.println( "GetPrint" );
			return FSocket;
		}

	public:
		inline void SystemLoopBegin()
		{
			StartSocket();
			if( ! FIsConnected().GetValue() )
				return;

			int APacketSize = FSocket.parsePacket();
			if( APacketSize )
			{
//				Serial.println( APacketSize );
				uint8_t *Adata = new uint8_t[ APacketSize ];
				FSocket.read( Adata, APacketSize );

				T_OutputPin::SendPinValue( Mitov::TDataBlock( APacketSize, Adata ));
//				OutputPin.SendValue( Mitov::TDataBlock( APacketSize, Adata ));
				T_RemoteIPOutputPin::SetPinValue( IPAdressToString( FSocket.remoteIP() ));
//				RemoteIPOutputPin.SendValue( IPAdressToString( FSocket.remoteIP() ));
				T_RemotePortOutputPin::SetPinValue( FSocket.remotePort() );
//				RemotePortOutputPin.SendValue( FSocket.remotePort() );

				delete []Adata;
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
		inline UDPSocket()
		{
			FIsConnected() = false;
//			FResolved() = true;
		}
/*
		UDPSocket( ::IPAddress ARemoteIPAddress ) :
			Enabled( true ),
			FIsConnected( false ),
			FResolved( true ),
			RemoteIPAddress( ARemoteIPAddress )

		{
		}

		UDPSocket( Mitov::String AHostName ) :
			Enabled( true ),
			FIsConnected( false ),
			FResolved( false ),
			FHostName( AHostName )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_Channel,
		typename T_Enabled
	> class ESPWiFiModuleOptionalChannel :
		public T_Channel,
		public T_Enabled
	{
	public:
		_V_PROP_( Channel )
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Gateway,
		typename T_IP,
		typename T_Subnet
	> class ESPWiFiModuleAccessPointConfig :
		public T_Enabled,
		public T_Gateway,
		public T_IP,
		public T_Subnet
	{
	public:
		_V_PROP_( Enabled )

		_V_PROP_( IP )
		_V_PROP_( Gateway )
		_V_PROP_( Subnet )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DNS1,
		typename T_DNS2,
		typename T_Enabled,
		typename T_Gateway,
		typename T_IP,
		typename T_Subnet
	> class ESPWiFiModuleRemoteConfig  :
		public T_DNS1,
		public T_DNS2,
		public T_Enabled,
		public T_Gateway,
		public T_IP,
		public T_Subnet
	{
	public:
		_V_PROP_( Enabled )

		_V_PROP_( IP )
		_V_PROP_( Gateway )
		_V_PROP_( Subnet )

		 _V_PROP_( DNS1 )
		 _V_PROP_( DNS2 )
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_MacAddress
	> class ESPWiFiModuleOptionalMacAddress :
		public T_Enabled,
		public T_MacAddress
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( MacAddress )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddressOutputPin,
		typename T_Enabled,
		typename T_FailedOutputPin,
		typename T_Host
	> class TArduinoNetworkGetHostIPOperation :
		public T_AddressOutputPin,
		public T_Enabled,
		public T_FailedOutputPin,
		public T_Host
	{
	public:
		_V_PIN_( AddressOutputPin )
		_V_PIN_( FailedOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Host )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue())
				return;

			IPAddress aResult;
			if( C_OWNER.GetIPFromHostName( Host().c_str(), aResult ))
				T_AddressOutputPin::SetPinValue( IPAdressToString( aResult ) );

			else
			{
				T_AddressOutputPin::SetPinValue( "0.0.0.0" );
				T_FailedOutputPin::ClockPin();
			}

		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#undef Console
#endif

