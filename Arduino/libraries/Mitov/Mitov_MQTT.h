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

// #define __MQTT_DEBUG__

namespace Mitov
{
#ifdef __MQTT_DEBUG__
    uint32_t GDebugPacketCount = 0;
#endif
	enum TArduinoMQTTDelivery { dcOnceOrLess, dcOnceOrMore, dcOnceOnly };
	namespace TArduinoMQTTErrorTypes
	{
		enum TArduinoMQTTErrorTypes 
		{ 
			Timeout,
			ConnectionRefused,
			IncorrectLength
		};
	};
//---------------------------------------------------------------------------
	template <
		typename T_Delivery,
		typename T_Message,
		typename T_Topic
	> class TCLMQTTClientWill :
		public T_Delivery,
		public T_Message,
		public T_Topic
	{
	public:
		_V_PROP_( Topic )
		_V_PROP_( Message )
		_V_PROP_( Delivery )
	};
//---------------------------------------------------------------------------
	static const uint8_t PROGMEM MQIsdp_V[] =
	{
		0, 6, 
		'M', 'Q', 'I', 's', 'd', 'p',
		3
	};
//---------------------------------------------------------------------------
	static const uint8_t PROGMEM MQTT_V[] =
	{
		0, 4,
		'M', 'Q', 'T', 'T',
		4
	};
//---------------------------------------------------------------------------
/*
	class ArduinoMQTTClientTextBasicTopic
	{
	public:
		Mitov::String	Topic;
		Mitov::String	InitialValue;

		TArduinoMQTTDelivery SendDelivery : 2;
		TArduinoMQTTDelivery ReceiveDelivery : 2;

		bool	Enabled	: 1;
		bool	Retain	: 1;

		bool	FSubscribed : 1; // In base class to save memory!

	protected:
		bool	FInitialValueSent : 1;
		bool	FModified : 1; // In base class to save memory!

	public:
		inline void ConnectionClosed()
		{
			FSubscribed = false;
		}

		virtual void Process()
		{
		}

		virtual bool NeedsSubscription()
		{
			return false;
		}

		virtual bool NeedsUnsubscription()
		{
			return false;
		}

		virtual void SendPayload( Mitov::String APayload )
		{
		}

	public:
		ArduinoMQTTClientTextBasicTopic() :
			SendDelivery( dcOnceOrLess ),
			ReceiveDelivery( dcOnceOrLess ),
			Enabled( true ),
			Retain( true ),
			FSubscribed( false ),
			FInitialValueSent( false ),
			FModified( true )
		{
		}


	};
*/
//---------------------------------------------------------------------------
	namespace TMQTTMessageType
	{
		enum TMQTTMessageType
		{
//          Reserved0,    //  0  Reserved
          BROKERCONNECT,  //  0  Broker request to connect to Broker
          CONNECT,        //  1  Client request to connect to Broker
          CONNACK,        //  2  Connect Acknowledgment
          PUBLISH,        //  3  Publish message
          PUBACK,         //  4  Publish Acknowledgment
          PUBREC,         //  5  Publish Received (assured delivery part 1)
          PUBREL,         //  6  Publish Release (assured delivery part 2)
          PUBCOMP,        //  7  Publish Complete (assured delivery part 3)
          SUBSCRIBE,      //  8  Client Subscribe request
          SUBACK,         //  9  Subscribe Acknowledgment
          UNSUBSCRIBE,    // 10  Client Unsubscribe request
          UNSUBACK,       // 11  Unsubscribe Acknowledgment
          PINGREQ,        // 12  PING Request
          PINGRESP,       // 13  PING Response
          DISCONNECT,     // 14  Client is Disconnecting
          Reserved15      // 15
		};

		static void AddLengthStringPayload( SimpleList<uint8_t, uint32_t>	&ABytes, Mitov::String AText )
		{
			uint16_t ALength = AText.length();
/*
			Serial.println( AText );
			Serial.println( "AddLengthStringPayload" );
			Serial.println( ALength );
			return;
*/
			ABytes.push_back( ALength >> 8 );
			ABytes.push_back( ALength );

			ABytes.append( (uint8_t *)AText.c_str(), ALength );
		}

		static void IntToMSBLSB( SimpleList<uint8_t, uint32_t>	&ABytes, uint16_t AValue )
		{
			ABytes.push_back( AValue >> 8 );
			ABytes.push_back( AValue );
		}

		static uint16_t MSBLSBToInt( uint8_t *ABytes )
		{
			return ( uint16_t( ABytes[ 0 ] ) << 8 ) | ABytes[ 1 ];
		}

		static Mitov::String ExtractUTF8String( uint8_t *&ABytes, uint32_t &ALenght )
		{
			uint16_t AStrLength = MSBLSBToInt( ABytes );

			if( AStrLength > 2000 )
				return( "" );

//			Serial.print( "AStrLength: " ); Serial.println( int( AStrLength ));

			char *Array = new char[ AStrLength + 1 ];
			memcpy( Array, ABytes + 2, AStrLength );

			Array[ AStrLength ] ='\0';

//			Serial.print( "Array: " ); Serial.println( Array );

			Mitov::String AResult = Array;

//			Serial.print( "AResult: " ); Serial.println( AResult );
			delete [] Array;

//			Mitov::String AResult;
//			AResult.reserve( AStrLength );

			ABytes += AStrLength + 2;
			ALenght -= AStrLength + 2;

			return AResult;
		}
	}
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTPacket
	{
	public:
		SimpleList< uint8_t, uint32_t>	FBytes;

	public:
/*
	protected:
		inline void SetRetain( bool AValue )
		{
		}

		inline void SetQoSLevel( uint8_t AValue )
		{
		}

		inline void SetDuplicate( bool AValue )
		{
		}
*/
	public:
		virtual bool CanPost( void *ATopic ) { return true; }
		virtual bool IsProcessed( T_OWNER *AOwner, TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength ) { return false; }
		virtual void SystemLoop( T_OWNER *AOwner ) {}

	protected:
		void AddRemainingLength( uint32_t ALength )
		{
			if( ALength == 0 )
			{
				FBytes.push_back( 0 );
				return;
			}
  
			while( ALength )
			{
				uint8_t	ADigit = ALength % 128;
				ALength /= 128;
				if( ALength > 0 )
					ADigit |= 0x80;
      
				FBytes.push_back( ADigit );
			}
		}

	public:
		TMQTTPacket( TMQTTMessageType::TMQTTMessageType AMessageType )
		{
			FBytes.push_back( AMessageType << 4 );
#ifdef __MQTT_DEBUG__
            ++ GDebugPacketCount;
            Serial.print( "Count TMQTTPacket + : " );
            Serial.println( GDebugPacketCount );
#endif
		}

#ifdef __MQTT_DEBUG__
        ~TMQTTPacket()
        {
            -- GDebugPacketCount;
            Serial.print( "Count TMQTTPacket - : " );
            Serial.println( GDebugPacketCount );
        }
#endif
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTConnectPacket : public TMQTTPacket<T_OWNER>
	{
		typedef TMQTTPacket<T_OWNER> inherited;

	public:
		virtual bool IsProcessed( T_OWNER *AOwner, TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength ) override
		{
			return( AMessageType = TMQTTMessageType::CONNACK );
		}

	public:
		template <typename T_WILL> TMQTTConnectPacket( Mitov::String AClientID, bool AIs3_1_1, bool ACleanSession, uint16_t AKeepAliveInterval, T_WILL &AWill, Mitov::String AUserName, Mitov::String APassword ) :
			inherited( TMQTTMessageType::CONNECT )
		{
			SimpleList<uint8_t, uint32_t>	ABytes;
			if( AIs3_1_1 )
			{
				for( int i = 0; i < sizeof( MQTT_V ); ++ i )
					ABytes.push_back( pgm_read_byte( MQTT_V + i ));
			}

			else
			{
				for( int i = 0; i < sizeof( MQIsdp_V ); ++ i )
					ABytes.push_back( pgm_read_byte( MQIsdp_V + i ));
			}

			uint8_t AFlagsOffset = ABytes.size();

			ABytes.push_back( 0 ); // Connection Flags
			TMQTTMessageType::IntToMSBLSB( ABytes, AKeepAliveInterval );
			uint8_t &AConnectFlags = ((uint8_t *)ABytes )[ AFlagsOffset ];

			if( ACleanSession )
				AConnectFlags |= 0b00000010;

			TMQTTMessageType::AddLengthStringPayload( ABytes, AClientID );
			if( AWill.Topic().GetValue() != "" )
			{
				AConnectFlags |= 0b00000100;
				AConnectFlags |= ( AWill.Delivery().GetValue() << 3 );

				TMQTTMessageType::AddLengthStringPayload( ABytes, AWill.Topic() );
				TMQTTMessageType::AddLengthStringPayload( ABytes, AWill.Message() );
			}

			if( AUserName != "" )
			{
				uint8_t &AConnectFlags = ((uint8_t *)ABytes )[ AFlagsOffset ];
				AConnectFlags |= 0b10000000;
				TMQTTMessageType::AddLengthStringPayload( ABytes, AUserName );
				if( APassword != "" )
				{
					uint8_t &AConnectFlags = ((uint8_t *)ABytes )[ AFlagsOffset ];
					AConnectFlags |= 0b01000000;
					TMQTTMessageType::AddLengthStringPayload( ABytes, APassword );
				}
			}

			inherited::AddRemainingLength( ABytes.size() );
			inherited::FBytes.append( ABytes, ABytes.size() );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTBasicRetryPacket : public TMQTTPacket<T_OWNER>
	{
		typedef TMQTTPacket<T_OWNER> inherited;

	protected:
		unsigned long	FLastTime;
		uint32_t		FRetryInterval;

	public:
		virtual void SystemLoop( T_OWNER *AOwner ) override
		{
			unsigned long ATime = millis();
			if( ATime - FLastTime > FRetryInterval * 1000 )
			{
	/*
				Serial.println( "TIME DIFF:" );
				Serial.println( ATime - FLastTime );
				Serial.println( FRetryInterval );
	*/
				inherited::FBytes[ 0 ] |= 0b1000; // Set the Dublicate flag
				AOwner->ResendPacket( this );
				FLastTime = ATime;
			}
		}

	public:
		TMQTTBasicRetryPacket( TMQTTMessageType::TMQTTMessageType AMessageType, uint32_t ARetryInterval ) :
			inherited( AMessageType ),
			FRetryInterval( ARetryInterval )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTReleasePacket : public TMQTTBasicRetryPacket<T_OWNER>
	{
		typedef TMQTTBasicRetryPacket<T_OWNER> inherited;

	protected:
		uint16_t FPacketID;
		void	*FTopic;

	public:
		virtual bool CanPost( void *ATopic ) override
		{
			return ( FTopic != ATopic );
		}

		virtual bool IsProcessed( T_OWNER *AOwner, TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength ) override
		{
//			Serial.println( "PUBCOMP Test" );
			if( AMessageType != TMQTTMessageType::PUBCOMP )
				return( false );

			return( TMQTTMessageType::MSBLSBToInt( AData ) == FPacketID );
		}

	public:
		TMQTTReleasePacket( void *ATopicObject, uint16_t APacketID, uint32_t ARetryInterval ) :
			inherited( TMQTTMessageType::PUBREL, ARetryInterval ),
			FPacketID( APacketID )
		{
			FTopic = ATopicObject;
			inherited::FBytes[ 0 ] |= 0b0010; // Set required bit!
			inherited::FBytes.push_back( 2 );
			TMQTTMessageType::IntToMSBLSB( inherited::FBytes, APacketID );

			inherited::FLastTime = millis();
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTPublishPacket : public TMQTTBasicRetryPacket<T_OWNER>
	{
		typedef TMQTTBasicRetryPacket<T_OWNER> inherited;

	protected:
		uint16_t	FPacketID;
		void	*FTopic;

	public:
		virtual bool CanPost( void *ATopic ) override
		{
			return ( FTopic != ATopic );
		}

		virtual bool IsProcessed( T_OWNER *AOwner, TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength ) override
		{
//			Serial.println( "Test PUBREC1" );
	//		Serial.println( FBytes[ 0 ] & 0b110 );

			if( ( inherited::FBytes[ 0 ] & 0b110 ) == 0b100 ) // QoS Level 2 - dcOnceOnly
			{
	//			Serial.println( "Test PUBREC" );
				if( AMessageType != TMQTTMessageType::PUBREC )
					return( false );

	//			Serial.print( "My ID: " ); Serial.print( FPacketID ); Serial.print( " ? " ); Serial.println( MSBLSBToInt( AData ) );
				if( TMQTTMessageType::MSBLSBToInt( AData ) == FPacketID )
				{
	//				Serial.println( "Send Response" );
					AOwner->SendResponsePacket( new TMQTTReleasePacket<T_OWNER>( FTopic, FPacketID, inherited::FRetryInterval ));
					return true;
				}
    
	//				FOnRelease( FPacketID );
    
				return false;
			}
    
			if( AMessageType != TMQTTMessageType::PUBACK )
				return( false );
    
			return( TMQTTMessageType::MSBLSBToInt( AData ) == FPacketID );
		}

	public:
		TMQTTPublishPacket( void *ATopicObject, TArduinoMQTTDelivery ATopicObjectSendDelivery, bool ATopicObjectRetain, String ATopicObjectTopic, String ATopicObjectInitialValue, uint16_t &APacketID, uint32_t ARetryInterval ) :
			inherited( TMQTTMessageType::PUBLISH, ARetryInterval )
		{
			FTopic = ATopicObject;
			inherited::FBytes[ 0 ] |= ( ATopicObjectSendDelivery << 1 ) | ( ATopicObjectRetain ? 1 : 0 );

//			Serial.println( FBytes[ 0 ], BIN );

			SimpleList<uint8_t, uint32_t>	ABytes;
			TMQTTMessageType::AddLengthStringPayload( ABytes, ATopicObjectTopic );
			if( ATopicObjectSendDelivery != dcOnceOrLess )
			{
				FPacketID = APacketID;
				TMQTTMessageType::IntToMSBLSB( ABytes, FPacketID );
				if( APacketID == 0xFFFF )
					APacketID = 1;

				else
					++ APacketID;

				inherited::FLastTime = millis();
			}

			ABytes.append( (uint8_t *)ATopicObjectInitialValue.c_str(), ATopicObjectInitialValue.length() );

			inherited::AddRemainingLength( ABytes.size() );
			inherited::FBytes.append( ABytes, ABytes.size() );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTPublishAckPacket : public TMQTTPacket<T_OWNER>
	{
		typedef TMQTTPacket<T_OWNER> inherited;

	public:
		TMQTTPublishAckPacket( uint16_t APacketID ) :
			inherited( TMQTTMessageType::PUBACK )
		{
			inherited::FBytes.push_back( 2 );
			TMQTTMessageType::IntToMSBLSB( inherited::FBytes, APacketID );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTPublishReceivedPacket : public TMQTTBasicRetryPacket<T_OWNER>
	{
		typedef TMQTTBasicRetryPacket<T_OWNER> inherited;

	protected:
		uint16_t	FPacketID;

	public:
		virtual bool IsProcessed( T_OWNER *AOwner, TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength ) override
		{
			if( AMessageType != TMQTTMessageType::PUBREL )
				return false;

			return( TMQTTMessageType::MSBLSBToInt( AData ) == FPacketID );
		}

	public:
		TMQTTPublishReceivedPacket( uint16_t APacketID, uint32_t ARetryInterval ) :
			inherited( TMQTTMessageType::PUBREC, ARetryInterval )
		{
			inherited::FBytes.push_back( 2 );
			TMQTTMessageType::IntToMSBLSB( inherited::FBytes, APacketID );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTSubscribePacket : public TMQTTBasicRetryPacket<T_OWNER>
	{
		typedef TMQTTBasicRetryPacket<T_OWNER> inherited;

	protected:
		uint16_t	FPacketID;

	public:
		virtual bool IsProcessed( T_OWNER *AOwner, TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength ) override
		{
			if( AMessageType != TMQTTMessageType::SUBACK )
				return false;

			return( TMQTTMessageType::MSBLSBToInt( AData ) == FPacketID );
		}

	public:
		TMQTTSubscribePacket( T_OWNER *AOwner, uint16_t &APacketID, uint32_t ARetryInterval ) :
			inherited( TMQTTMessageType::SUBSCRIBE, ARetryInterval )
		{
			inherited::FBytes[ 0 ] |= 0b0010; // Set required bit!

			SimpleList<uint8_t, uint32_t>	ABytes;

			FPacketID = APacketID;
			TMQTTMessageType::IntToMSBLSB( ABytes, FPacketID );
			if( APacketID == 0xFFFF )
				APacketID = 1;

			else
				++ APacketID;

			AOwner->AddSubscriptionBytes( ABytes );

/*
			for( int i = 0; i < AList.size(); ++i )
			{
//				TMQTTMessageType::AddLengthStringPayload( ABytes, AList[ i ]->Topic );
//				ABytes.push_back( AList[ i ]->ReceiveDelivery );
				TMQTTMessageType::AddLengthStringPayload( ABytes, AOwner->GetTopic( AList[ i ] ));
				ABytes.push_back( AOwner->GetReceiveDelivery( AList[ i ] ) );
			}
*/
			inherited::AddRemainingLength( ABytes.size() );
			inherited::FBytes.append( ABytes, ABytes.size() );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTUnsubscribePacket : public TMQTTBasicRetryPacket<T_OWNER>
	{
		typedef TMQTTBasicRetryPacket<T_OWNER> inherited;

	protected:
		uint16_t	FPacketID;

	public:
		virtual bool IsProcessed( T_OWNER *AOwner, TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength ) override
		{
			if( AMessageType != TMQTTMessageType::UNSUBACK )
				return false;

			return( TMQTTMessageType::MSBLSBToInt( AData ) == FPacketID );
		}

	public:
		TMQTTUnsubscribePacket( T_OWNER *AOwner, uint16_t &APacketID, uint32_t ARetryInterval ) :
			inherited( TMQTTMessageType::UNSUBSCRIBE, ARetryInterval )
		{
			inherited::FBytes[ 0 ] |= 0b0010; // Set required bit!

			SimpleList<uint8_t, uint32_t>	ABytes;

			FPacketID = APacketID;
			TMQTTMessageType::IntToMSBLSB( ABytes, FPacketID );
			if( APacketID == 0xFFFF )
				APacketID = 1;

			else
				++ APacketID;

			AOwner->AddLengthStringPayload( ABytes );

//			for( int i = 0; i < AList.size(); ++i )
//				TMQTTMessageType::AddLengthStringPayload( ABytes, AOwner->GetTopic( AList[ i ] ) );
//				TMQTTMessageType::AddLengthStringPayload( ABytes, AList[ i ]->Topic );

			inherited::AddRemainingLength( ABytes.size() );
			inherited::FBytes.append( ABytes, ABytes.size() );
		}
	};
//---------------------------------------------------------------------------
/*
	class TMQTTPingRequestPacket : public TMQTTPacket
	{
		typedef TMQTTPacket inherited;

	public:
		TMQTTPingRequestPacket() :
			inherited( TMQTTMessageType::PINGREQ )
		{
			FBytes.push_back( 0 );
		}
	};
*/
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TMQTTSimplePacket : public TMQTTPacket<T_OWNER>
	{
		typedef TMQTTPacket<T_OWNER> inherited;

	public:
		TMQTTSimplePacket( TMQTTMessageType::TMQTTMessageType AMessageType ) :
			inherited( AMessageType )
		{
			inherited::FBytes.push_back( 0 );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_CleanSession,
		typename T_ClientID,
		typename T_ConnectedInputPin_o_IsConnected,
		typename T_DisconnectOutputPin,
		typename T_Enabled,
		typename T_FConnected,
		bool C_FIs3_1_1,
		typename T_FSocketConnected,
		typename T_KeepAliveInterval,
		typename T_OutputPin,
		typename T_Password,
		typename T_RetryInterval,
		typename T_SessionPresentOutputPin,
		uint16_t C_COUNT_TOPICS,
		typename T_Topics_AddLengthStringPayload,
		typename T_Topics_AddSubscriptionBytes,
		typename T_Topics_ConnectionClosed,
		typename T_Topics_NeedsSubscription,
		typename T_Topics_NeedsUnsubscription,
		typename T_Topics_Process,
		typename T_Topics_SendPayload,
		typename T_UserName,
		typename T_Will
	> class MitovMQTTClient :
		public T_CleanSession,
		public T_ClientID,
		public T_ConnectedInputPin_o_IsConnected,
		public T_DisconnectOutputPin,
		public T_Enabled,
		public T_FConnected,
		public T_FSocketConnected,
		public T_KeepAliveInterval,
		public T_OutputPin,
		public T_Password,
		public T_RetryInterval,
		public T_SessionPresentOutputPin,
		public T_UserName,
		public T_Will
	{
		typedef MitovMQTTClient<
				T_CleanSession,
				T_ClientID,
				T_ConnectedInputPin_o_IsConnected,
				T_DisconnectOutputPin,
				T_Enabled,
				T_FConnected,
				C_FIs3_1_1,
				T_FSocketConnected,
				T_KeepAliveInterval,
				T_OutputPin,
				T_Password,
				T_RetryInterval,
				T_SessionPresentOutputPin,
				C_COUNT_TOPICS,
				T_Topics_AddLengthStringPayload,
				T_Topics_AddSubscriptionBytes,
				T_Topics_ConnectionClosed,
				T_Topics_NeedsSubscription,
				T_Topics_NeedsUnsubscription,
				T_Topics_Process,
				T_Topics_SendPayload,
				T_UserName,
				T_Will
			> SELF;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PIN_( DisconnectOutputPin )
		_V_PIN_( SessionPresentOutputPin )

	public:
		_V_PROP_( ClientID )

		_V_PROP_( UserName )
		_V_PROP_( Password )

		_V_PROP_( Will )

		_V_PROP_( KeepAliveInterval )

		_V_PROP_( RetryInterval )

		_V_PROP_( Enabled )
		_V_PROP_( CleanSession )

	public:
		_V_PROP_( ConnectedInputPin_o_IsConnected )

	protected:
		_V_PROP_( FSocketConnected )
		_V_PROP_( FConnected )

	protected:
		SimpleObjectDeleteList<TMQTTPacket<SELF> *>	FRequests;
		SimpleList<uint8_t, uint32_t>	FReceivedData;

		unsigned long	FCurrentTime;
		unsigned long	FLastComTime;

		uint16_t	FCurrentPacketID = 1;

//	public:
//		ArduinoMQTTClientTextBasicTopic * FTopics[ C_COUNT_TOPICS ];
//		SimpleList<ArduinoMQTTClientTextBasicTopic *>	FTopics;

	public:
		void SendResponsePacket( TMQTTPacket<SELF> *APacket )
		{
			FRequests.push_back( APacket );

#ifdef __MQTT_DEBUG__
			Serial.println( "SendResponsePacket" );
			Serial.println( APacket->FBytes.size() );
			Serial.println( Mitov::TDataBlock( APacket->FBytes.size(), APacket->FBytes ).ToString( 1000 ));
#endif

			T_OutputPin::SetPinValue( Mitov::TDataBlock( APacket->FBytes.size(), APacket->FBytes ));
		}

		void SendPacket( TMQTTPacket<SELF> *APacket )
		{
#ifdef __MQTT_DEBUG__
			Serial.println( "SendPacket" );
			Serial.println( APacket->FBytes.size() );
			Serial.println( Mitov::TDataBlock( APacket->FBytes.size(), APacket->FBytes ).ToString( 1000 ));
#endif
			T_OutputPin::SetPinValue( Mitov::TDataBlock( APacket->FBytes.size(), APacket->FBytes ));
			delete APacket;
		}

		void ResendPacket( TMQTTPacket<SELF> *APacket )
		{
//			Serial.println( "ResendPacket" );
//			Serial.println( APacket->FBytes.size() );
//			Serial.println( Mitov::TDataBlock( APacket->FBytes.size(), APacket->FBytes ).ToString());
			T_OutputPin::SetPinValue( Mitov::TDataBlock( APacket->FBytes.size(), APacket->FBytes ));
		}

	protected:
		void Connect()
		{
#ifdef __MQTT_DEBUG__
			Serial.println( "Connect()" );
#endif
//			return;
			if( FConnected() )
				return;

			if( FRequests.size() )
				return;

			FLastComTime = millis();
			SendResponsePacket( new TMQTTConnectPacket<SELF>( ClientID(), C_FIs3_1_1, CleanSession(), KeepAliveInterval(), Will(), UserName(), Password() ));
		}

		void Disconnect()
		{
//			Serial.println( "Disconnect()" );
			if( ! FConnected() )
				return;

			FConnected() = false;
			SendPacket( new TMQTTSimplePacket<SELF>( TMQTTMessageType::DISCONNECT ));
//			for( int i = 0; i < C_COUNT_TOPICS; ++ i )
//				FTopics[ i ]->ConnectionClosed();

			T_Topics_ConnectionClosed::Call();
		}

	public:
		void Publish( void *ATopicObject, TArduinoMQTTDelivery ATopicObjectSendDelivery, bool ATopicObjectRetain, String ATopicObjectTopic, String ATopicObjectInitialValue )
		{
			if( FRequests.size() > 4 * C_COUNT_TOPICS )
				return;

			for( int i = 0; i < FRequests.size(); ++i )
				if( ! FRequests[ i ]->CanPost( ATopicObject ))
					return;

#ifdef __MQTT_DEBUG__
			Serial.println( "Publish" );
#endif
			TMQTTPacket<SELF> *APacket = new TMQTTPublishPacket<SELF>( ATopicObject, ATopicObjectSendDelivery, ATopicObjectRetain, ATopicObjectTopic, ATopicObjectInitialValue, FCurrentPacketID, RetryInterval() );
//			TMQTTPacket<SELF> *APacket = new TMQTTPublishPacket<SELF>( ATopicObject, FCurrentPacketID, RetryInterval() );
			if( ATopicObjectSendDelivery != dcOnceOrLess )
				SendResponsePacket( APacket );

			else
				SendPacket( APacket );

		}

		void Subscribe()
		{
#ifdef __MQTT_DEBUG__
			Serial.println( "Subscribe" );
#endif
			SendResponsePacket( new TMQTTSubscribePacket<SELF>( this, FCurrentPacketID, RetryInterval() ));
		}

		void Unsubscribe()
		{
//			Serial.println( "Unsubscribe" );
			SendResponsePacket( new TMQTTUnsubscribePacket<SELF>( this, FCurrentPacketID, RetryInterval() ));
		}

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
				if( ! ConnectedInputPin_o_IsConnected().GetValue() )
					Connect();

			}

			else
				Disconnect();

		}

	public:
		inline void SystemStart()
		{
			T_SessionPresentOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( FConnected() )
			{
				unsigned long	ACurrentTime = millis();
				if( ACurrentTime - FLastComTime > KeepAliveInterval() * 5 * 1000 + 2000 )
				{
					Error( TArduinoMQTTErrorTypes::Timeout ); // Timedout!
					return;
				}

				if( ACurrentTime - FCurrentTime > KeepAliveInterval() * 800 )
				{
//					Serial.println( "PING" );
					FCurrentTime = ACurrentTime;
					SendPacket( new TMQTTSimplePacket<SELF>( TMQTTMessageType::PINGREQ ));
				}

				ProcessTopics();
				StartSubscriptions();
				EndsSubscriptions();

				for( int i = FRequests.size(); i--; )
					FRequests[ i ]->SystemLoop( this );

			}
		}

	protected:
		inline void Error( uint8_t AErrorCode )
		{
#ifdef __MQTT_DEBUG__
			Serial.println( "Error()" );
			Serial.println( AErrorCode );
#endif

			T_DisconnectOutputPin::ClockPin();
//			DisconnectOutputPin.Notify( nullptr );
		}

		void ProcessTopics()
		{
			if( ! FConnected() )
				return;

			T_Topics_Process::Call();
		}

		void StartSubscriptions()
		{
			bool ANeedsSubscription = false;
//#ifdef __MQTT_DEBUG__
//			Serial.println( "StartSubscriptions Step 1" );
//#endif
			T_Topics_NeedsSubscription::Call( ANeedsSubscription );
//#ifdef __MQTT_DEBUG__
//			Serial.println( "StartSubscriptions Step 2" );
//#endif
			if( ANeedsSubscription )
				Subscribe();

/*
			bool ANeedsSubscribe = false;
			for( int i = 0; i < C_COUNT_TOPICS; ++ i )
				if( FTopics[ i ]->NeedsSubscription())
				{
					ANeedsSubscribe = true;
					break;
				}

			if( ANeedsSubscribe )
				Subscribe();
*/
		}

		void EndsSubscriptions()
		{
			bool ANeedsUnsubscription = false;
			T_Topics_NeedsUnsubscription::Call( ANeedsUnsubscription );
			if( ANeedsUnsubscription )
				Unsubscribe();
/*
			SimpleList<uint8_t> AList;
			for( int i = 0; i < C_COUNT_TOPICS; ++ i )
				if( FTopics[ i ]->NeedsUnsubscription())
					AList.push_back( i );

			if( AList.size() )
				Unsubscribe( AList );
*/
		}

	public:
/*
		Mitov::String GetTopic( uint8_t AIndex )
		{
			return FTopics[ AIndex ]->Topic;
		}

		TArduinoMQTTDelivery GetReceiveDelivery( uint8_t AIndex )
		{
			return FTopics[ AIndex ]->ReceiveDelivery;
		}
*/
		void AddSubscriptionBytes( SimpleList<uint8_t, uint32_t> &ABytes )
		{
			T_Topics_AddSubscriptionBytes::Call( ABytes );
/*
			for( int i = 0; i < C_COUNT_TOPICS; ++ i )
				if( FTopics[ i ]->NeedsSubscription())
				{
					TMQTTMessageType::AddLengthStringPayload( ABytes, GetTopic( i ));
					ABytes.push_back( GetReceiveDelivery( i ) );
				}
*/
		}

		void AddLengthStringPayload( SimpleList<uint8_t, uint32_t> &ABytes )
		{
			T_Topics_AddLengthStringPayload::Call( ABytes );
		}

	public:
		void ConnectedInputPin_o_Receive( void *_Data )
		{
			if( FSocketConnected() == *((bool *)_Data))
				return;

			FSocketConnected() = *((bool *)_Data);

//			Serial.print( "FSocketConnected: " );
//			Serial.println( FSocketConnected().GetValue() );

			if( FSocketConnected() )
				Connect();

			else
			{
				FConnected() = false;
				FRequests.clear();
				FReceivedData.clear();
//				for( int i = 0; i < C_COUNT_TOPICS; ++ i )
//					FTopics[ i ]->ConnectionClosed();

				T_Topics_ConnectionClosed::Call();
			}
		}

		void InputPin_o_Receive( void *_Data )
		{
			FLastComTime = millis();

			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;

#ifdef __MQTT_DEBUG__
			Serial.println( "Received" );
			Serial.println( ABlock.ToString() );
#endif

			FReceivedData.append( ABlock.Data, ABlock.Size );

			if( FReceivedData.size() < 2 )
				return;

//			Serial.println( "FReceivedData" );
//			Serial.println( APacket->FBytes.size() );
//			Serial.println( Mitov::TDataBlock( FReceivedData.size(), FReceivedData ).ToString());

//			Serial.print( "ReceivedSize = " ); Serial.println( FReceivedData.size() );
//			Serial.println( "Received - 2" );
			uint8_t	ADataOffset = 1;
			uint32_t	ALength = 0;

			uint32_t	AReceivedLength = FReceivedData.size();

			uint32_t	AMultiplier = 1;

			for( int i = 1; i <= 4; ++ i )
			{
				++ ADataOffset;
				ALength += ( FReceivedData[ i ] & 0x7F ) * AMultiplier;
//				Serial.print( "i = " ); Serial.println( i );
//				Serial.print( "ALength = " ); Serial.println( ALength );
				if( !( FReceivedData[ i ] & 0x80 ))
					break;

				if( AReceivedLength < i + 2 )
					return;

				AMultiplier *= 128;
				if( i == 4 )
				{
					Error( TArduinoMQTTErrorTypes::IncorrectLength );
					return;
				}
			}

//			Serial.println( "TEST1" );
//			Serial.println( AReceivedLength );
//			Serial.println( ADataOffset );
//			Serial.println( ALength );
			if( AReceivedLength < ADataOffset + ALength )
				return;

//			Serial.println( "TEST2" );
//			Serial.println( ALength );
//			Serial.println( FReceivedData[ 0 ] );

			TMQTTMessageType::TMQTTMessageType AMessageType = TMQTTMessageType::TMQTTMessageType( FReceivedData[ 0 ] >> 4 );
			switch( AMessageType )
			{
				case TMQTTMessageType::CONNACK :
#ifdef __MQTT_DEBUG__
					Serial.println( "CONNACK" );
#endif
				    if( ALength != 2 )
					{
						Error( TArduinoMQTTErrorTypes::IncorrectLength );
						return;
					}

					if( FReceivedData[ 3 ] )
					{
						Error( TArduinoMQTTErrorTypes::ConnectionRefused ); // Connection refused
						break;
					}

					T_SessionPresentOutputPin::SetPinValue( FReceivedData[ 2 ] & 1, true );

#ifdef __MQTT_DEBUG__
					Serial.print( "CONNACK: " ); Serial.println( FReceivedData[ 3 ] );
#endif

				    FConnected() = true;
					FCurrentTime = millis();
					ProcessRequests( AMessageType, FReceivedData + ADataOffset, ALength );
#ifdef __MQTT_DEBUG__
					Serial.println( "CONNACK: Step 1" );
#endif
					ProcessTopics();
#ifdef __MQTT_DEBUG__
					Serial.println( "CONNACK: Step 2" );
#endif
					StartSubscriptions();
#ifdef __MQTT_DEBUG__
					Serial.println( "CONNACK: Step 3" );
#endif

//					FCurrentTime = millis();
//					SendPacket( new TMQTTSimplePacket<SELF>( TMQTTMessageType::PINGREQ ));
					break;

				case TMQTTMessageType::PINGRESP :
#ifdef __MQTT_DEBUG__
					Serial.println( "PINGRESP" );
#endif
					break;

				case TMQTTMessageType::PUBACK : // Acknowleges a message in QoS 1
#ifdef __MQTT_DEBUG__
					Serial.println( "PUBACK" );
#endif
					ProcessRequests( AMessageType, FReceivedData + ADataOffset, ALength );
					break;

				case TMQTTMessageType::PUBREC : // Acknowleges a message in QoS 2
#ifdef __MQTT_DEBUG__
					Serial.println( "PUBREC" );
#endif
					ProcessRequests( AMessageType, FReceivedData + ADataOffset, ALength );
					break;

				case TMQTTMessageType::PUBCOMP : // Complete a message in QoS 2
#ifdef __MQTT_DEBUG__
					Serial.println( "PUBCOMP" );
#endif
					ProcessRequests( AMessageType, FReceivedData + ADataOffset, ALength );
					break;

				case TMQTTMessageType::SUBACK :
#ifdef __MQTT_DEBUG__
					Serial.println( "SUBACK" );
#endif
					ProcessRequests( AMessageType, FReceivedData + ADataOffset, ALength );
					break;

				case TMQTTMessageType::UNSUBACK :
#ifdef __MQTT_DEBUG__
					Serial.println( "UNSUBACK" );
#endif
					ProcessRequests( AMessageType, FReceivedData + ADataOffset, ALength );
					break;

				case TMQTTMessageType::PUBLISH :
#ifdef __MQTT_DEBUG__
					Serial.println( "PUBLISH" );
#endif
					ProcessPublish( FReceivedData[ 0 ], FReceivedData + ADataOffset, ALength );
					break;
			}

			FReceivedData.Delete( 0, ADataOffset + ALength );

//			Serial.print( "FReceivedData.Delete = " ); Serial.println( ADataOffset + ALength );
//			Serial.println( APacket->FBytes.size() );
//			Serial.println( Mitov::TDataBlock( FReceivedData.size(), FReceivedData ).ToString());
		}

	protected:
		void ProcessPublish( uint8_t AHeader, uint8_t *AData, uint32_t ALength )
		{
//			Serial.print( "AHeader: " ); Serial.println( AHeader, BIN );

			Mitov::String ATopic = TMQTTMessageType::ExtractUTF8String( AData, ALength );
			if( ALength > 2000 )
				return;

			TArduinoMQTTDelivery AQoS = TArduinoMQTTDelivery( ( AHeader >> 1 ) & 0b11 );
			uint16_t AMessageID;
			if( AQoS != dcOnceOrLess )
			{
				AMessageID = TMQTTMessageType::MSBLSBToInt( AData );
				AData += 2;
				ALength -= 2;
			}

//			Serial.print( "ATopic: " ); Serial.println( ATopic );

			char *Array = new char[ ALength + 1 ];
			memcpy( Array, AData, ALength );

			Array[ ALength ] ='\0';
			Mitov::String APayload = Array;

			delete [] Array;

//			Serial.print( "APayload: " ); Serial.println( APayload );
			switch( AQoS )
			{
				case dcOnceOrMore:
					SendPacket( new TMQTTPublishAckPacket<SELF>( AMessageID ));
					break;

				case dcOnceOnly:
					SendResponsePacket( new TMQTTPublishReceivedPacket<SELF>( AMessageID, RetryInterval() ));
					break;

			}

			T_Topics_SendPayload::Call( ATopic, APayload );

//			for( int i = 0; i < C_COUNT_TOPICS; ++ i )
//				if( FTopics[ i ]->Topic == ATopic )
//					FTopics[ i ]->SendPayload( APayload );
		}

		void ProcessRequests( TMQTTMessageType::TMQTTMessageType AMessageType, uint8_t *AData, uint32_t ALength )
		{
#ifdef __MQTT_DEBUG__
            Serial.print( "MQTT FRequests.size: " );
			Serial.println( FRequests.size() );
#endif
//			Serial.println( FRequests.size() );
			for( int i = FRequests.size(); i--; )
				if( FRequests[ i ]->IsProcessed( this, AMessageType, AData, ALength ))
				{
//					Serial.println( "IsProcessed DELETE" );
//					Serial.println( FRequests.size() );
					FRequests.Delete( i );
//					Serial.println( "IsProcessed after delete" );
//					Serial.println( FRequests.size() );
					break;
				}
#ifdef __MQTT_DEBUG__
            Serial.print( "ProcessRequests Completed FRequests.size: " );
			Serial.println( FRequests.size() );
#endif
		}

	public:
		inline MitovMQTTClient()
		{
			FSocketConnected() = false;
			FConnected() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FInitialValueSent,
		typename T_FModified,
		typename T_FSubscribed,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_ReceiveDelivery,
		typename T_Retain,
		typename T_SendDelivery,
		typename T_Topic
	> class ArduinoMQTTClientTextTopic :
		public T_Enabled,
		public T_FInitialValueSent,
		public T_FModified,
		public T_FSubscribed,
		public T_InitialValue,
		public T_OutputPin,
		public T_ReceiveDelivery,
		public T_Retain,
		public T_SendDelivery,
		public T_Topic
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( ReceiveDelivery )
		_V_PROP_( Retain )
		_V_PROP_( SendDelivery )
		_V_PROP_( Topic )

	protected:
		_V_PROP_( FInitialValueSent )
		_V_PROP_( FModified )
		_V_PROP_( FSubscribed )

	public:
		inline void Process()
		{
			if( Enabled() )
				if( FModified() )
				{
					FModified() = false;
					if( FInitialValueSent() )
					    C_OWNER.Publish( this, SendDelivery(), Retain(), Topic(), InitialValue() );

					else
					{
						FInitialValueSent() = true;

						if( InitialValue().GetValue() != "" )
							C_OWNER.Publish( this, SendDelivery(), Retain(), Topic(), InitialValue() );

					}
				}

		}

		inline void ConnectionClosed()
		{
			FSubscribed() = false;
		}

		inline void NeedsSubscription( bool & AResult )
		{
//#ifdef __MQTT_DEBUG__
//			Serial.println( "NeedsSubscription Step 1" );
//#endif
			if( T_OutputPin::GetPinIsConnected() )
				if( Enabled() )
					if( ! FSubscribed().GetValue() )
						AResult = true;

//#ifdef __MQTT_DEBUG__
//			Serial.println( "NeedsSubscription Step 2" );
//#endif
		}

		inline void NeedsUnsubscription( bool & AResult )
		{
			if( ( ! Enabled() ) && FSubscribed() )
			{
				AResult = true;
				FSubscribed() = false;
			}

		}

		void AddSubscriptionBytes( SimpleList<uint8_t, uint32_t> &ABytes )
		{
			if( T_OutputPin::GetPinIsConnected() )
				if( Enabled() )
					if( ! FSubscribed().GetValue() )
					{
						FSubscribed() = true;
						if( ! FInitialValueSent().GetValue() )
						{
							FInitialValueSent() = true;
							if( InitialValue().GetValue() != "" )
								T_OutputPin::SetPinValue( InitialValue() );
						}

						TMQTTMessageType::AddLengthStringPayload( ABytes, Topic());
						ABytes.push_back( ReceiveDelivery() );
					}

		}

		void AddLengthStringPayload( SimpleList<uint8_t, uint32_t> &ABytes )
		{
			TMQTTMessageType::AddLengthStringPayload( ABytes, Topic() );
			ABytes.push_back( ReceiveDelivery() );
		}

		inline void SendPayload( Mitov::String ATopic, Mitov::String APayload )
		{
			if( T_OutputPin::GetPinIsConnected() )
				if( ATopic == Topic().GetValue() )
					T_OutputPin::SetPinValue( APayload );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = ( char * )_Data;
			FModified() = true;
		}

	public:
		inline ArduinoMQTTClientTextTopic()
		{
			FSubscribed() = false;
			FInitialValueSent() = false;
			FModified() = true;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif