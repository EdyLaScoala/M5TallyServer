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
#ifdef VISUINO_ESP8266
  #include <espnow.h>
#else
  #include <esp_now.h>
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

#ifdef VISUINO_ESP8266
    #define ESP_NOW_KEY_LEN 16
    #define ESP_NOW_SEND_SUCCESS 0
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
namespace Arduino_ESPNow
{
    uint8_t MAX_PEERS = 20;
}
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
    > class TArduino_ESPNow_BasicDeviceElement_DeviceExists :
		public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			uint8_t AMacAddress[ 6 ];
			C_OWNER.GetMacAddress( AMacAddress );

		    T_OutputPin::SetPinValue( esp_now_is_peer_exist( AMacAddress ));
		}

    };
*/
	template <
	    typename T_CanDisconnect,
	    typename T_Enabled,
	    typename T_Key,
	    typename T_Key_GetValue,
	    typename T_Key_ApplyValues
    > class TArduino_ESPNow_Encryption :
	    public T_CanDisconnect,
	    public T_Enabled,
	    public T_Key,
	    public T_Key_GetValue,
	    public T_Key_ApplyValues
    {
	public:
		_V_PROP_( CanDisconnect )
		_V_PROP_( Enabled )
		_V_PROP_( Key )

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FPairedCount,
		typename T_FailedOutputPin,
		typename T_PeeredOutputPin,
		typename T_SuccededOutputPin
    > class TArduino_ESPNow_DeviceSendiing :
		public T_Enabled,
		public T_FPairedCount,
		public T_FailedOutputPin,
		public T_PeeredOutputPin,
		public T_SuccededOutputPin
    {
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( PeeredOutputPin )
		_V_PIN_( SuccededOutputPin )

	public:
		_V_PROP_( Enabled )

    protected:
		_V_PROP_( FPairedCount )

	public:
        void UpdateEnabled()
        {
            if( Enabled() )
                return;

            C_OWNER.Disconnect();
        }

/*
	public:
        inline void GetMacAddress( uint8_t *AMacAddress )
        {
            C_OWNER.MACAddress().GetMacAddress( AMacAddress );
        }
*/
    protected:
		void Disconnect()
		{
            if( ! FPairedCount().GetValue() )
                return;

			uint8_t AMacAddress[ 6 ];
			C_OWNER.MACAddress().GetMacAddress( AMacAddress );
            esp_now_del_peer( AMacAddress );
            T_PeeredOutputPin::SetPinValue( false );
            FPairedCount() = 0;
		}

	    void Peer()
	    {
            if( FPairedCount().GetValue() )
                return;

            C_OWNER.ReleasePair();
//		        Serial.println( "C_OWNER.ReleasePair" );

#ifdef VISUINO_ESP8266
			uint8_t AMacAddress[ 6 ];
            C_OWNER.MACAddress().GetMacAddress( AMacAddress );

			uint8_t AKey[ ESP_NOW_KEY_LEN ];
            uint8_t *AKeyPtr;
            uint8_t AKeySize;

            if( C_OWNER.Encryption().Enabled() )
            {
                AKeySize = ESP_NOW_KEY_LEN;
                C_OWNER.Encryption().Key().CopyData( AKey, ESP_NOW_KEY_LEN );
                AKeyPtr = AKey;
            }

            else
            {
                AKeySize = 0;
                AKeyPtr = nullptr;
            }

            esp_now_add_peer( AMacAddress, ESP_NOW_ROLE_COMBO, 1, AKeyPtr, AKeySize );

//            u8 AChannel;
#else
            esp_now_peer_info_t peerInfo;
            memset( &peerInfo, 0, sizeof( peerInfo ));

            C_OWNER.MACAddress().GetMacAddress( peerInfo.peer_addr );
            peerInfo.channel = 0;
//                peerInfo.encrypt = C_OWNER.Encryption().Enabled();
            if( C_OWNER.Encryption().Enabled() )
            {
                peerInfo.encrypt = true;

                C_OWNER.Encryption().Key().CopyData( peerInfo.lmk, ESP_NOW_KEY_LEN );
            }

            else
                peerInfo.encrypt = false;

//                Serial.println( "esp_now_add_peer" );
            esp_now_add_peer( &peerInfo );
#endif
            T_PeeredOutputPin::SetPinValue( true );
            FPairedCount() = Arduino_ESPNow::MAX_PEERS;
	    }

	public:
        inline void UnpairAll(void)
        {
            Disconnect();
        }

	public:
		void Print( Mitov::String AValue )
		{
			AValue += "\r\n";
			send( (uint8_t *)AValue.c_str(), AValue.length() );
		}

		void Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		inline void PrintChar( char AValue )
		{
			send( (uint8_t*)&AValue, 1 );
		}

		inline void PrintChar( uint8_t AValue )
		{
			send( &AValue, 1 );
		}

		inline void Write( uint8_t *AData, uint32_t ASize )
		{
			send( AData, ASize );
		}

		void send( const uint8_t* AData, uint8_t ASize )
		{
		    if( ! Enabled().GetValue() )
		        return;

            Peer();

            C_OWNER.DecreasePairConts_Internal( this );

			uint8_t AMacAddress[ 6 ];
			C_OWNER.MACAddress().GetMacAddress( AMacAddress );

//            esp_err_t result =
            esp_now_send( AMacAddress, (uint8_t*)AData, ASize );
//            Serial.println( "esp_now_send" );

//            if (result == ESP_OK)
//                T_SuccededOutputPin::ClockPin();

//            else
//                T_FailedOutputPin::ClockPin();

            FPairedCount() = Arduino_ESPNow::MAX_PEERS;
        }

        inline void DataSent( const uint8_t * AMac, bool ASuccess )
        {
			uint8_t AMacAddress[ 6 ];
			C_OWNER.MACAddress().GetMacAddress( AMacAddress );
			if( memcmp( AMac, AMacAddress, 6 ))
			    return;

//            Serial.println( ASuccess );
            if ( ASuccess )
                T_SuccededOutputPin::ClockPin();

            else
                T_FailedOutputPin::ClockPin();
        }

        inline void GetPeeredCount( uint8_t & ACount )
        {
            if( FPairedCount() )
                ++ ACount;

        }

		inline void GetMinCount( uint8_t & ACount )
		{
		    if( FPairedCount() )
		        if( FPairedCount() < ACount )
		            FPairedCount() = ACount;

		}

		inline void DecreasePairConts( void * AExceptInstance )
		{
            if( AExceptInstance == this )
                return;

            if( FPairedCount() > 1 )
                FPairedCount() = FPairedCount() - 1;

		}

		inline void ReleaseFirstCount( uint8_t & ACount, bool AIncludeEncrypted )
		{
		    if( ACount )
		        if( FPairedCount() == ACount )
		        {
		            if( C_OWNER.Encryption().Enabled() )
		            {
		                if( ! AIncludeEncrypted )
		                    return;

                        if( ! C_OWNER.Encryption().CanDisconnect().GetValue() )
		                    return;

		            }

//		            if( AIncludeEncrypted ||  )
		            Disconnect();
		            ACount = 0;
                }

		}

        inline void UpdateEncryption(void)
        {
            if( C_OWNER.Encryption().Enabled() )
                Peer();

            else
                Disconnect();

        }

        inline void UpdateEncryptionProp(void)
        {
            Disconnect();
            if( C_OWNER.Encryption().Enabled() )
                Peer();

        }

	public:
		inline void SystemStart(void)
        {
            T_PeeredOutputPin::SetPinValue( FPairedCount().GetValue() != 0 );
            if( C_OWNER.Encryption().Enabled() )
                Peer();
        }

	public:
        inline TArduino_ESPNow_DeviceSendiing()
        {
            FPairedCount() = 0;
        }

    };
//---------------------------------------------------------------------------
    namespace ESPNow_Broadcast_Const
    {
        const uint8_t MacAddress[ 6 ] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    };
    
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Encryption,
		typename T_FPairedCount,
		typename T_FailedOutputPin,
		typename T_PeeredOutputPin,
		typename T_SuccededOutputPin
    > class TArduino_ESPNow_Broadcast :
		public T_Enabled,
		public T_Encryption,
		public T_FPairedCount,
		public T_FailedOutputPin,
		public T_PeeredOutputPin,
		public T_SuccededOutputPin
    {
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( PeeredOutputPin )
		_V_PIN_( SuccededOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Encryption )

    protected:
		_V_PROP_( FPairedCount )

	public:
        void UpdateEnabled()
        {
            if( Enabled() )
                return;

            C_OWNER.Disconnect();
        }


    protected:
		void Disconnect()
		{
            if( ! FPairedCount().GetValue() )
                return;

            esp_now_del_peer( (uint8_t *)ESPNow_Broadcast_Const::MacAddress );
            T_PeeredOutputPin::SetPinValue( false );
            FPairedCount() = 0;
		}

	    void Peer()
	    {
            if( FPairedCount().GetValue() )
                return;

            C_OWNER.ReleasePair();
//		        Serial.println( "C_OWNER.ReleasePair" );

#ifdef VISUINO_ESP8266
			uint8_t AKey[ ESP_NOW_KEY_LEN ];
            uint8_t *AKeyPtr;
            uint8_t AKeySize;

            if( Encryption().Enabled() )
            {
                AKeySize = ESP_NOW_KEY_LEN;
                Encryption().Key().CopyData( AKey, ESP_NOW_KEY_LEN );
                AKeyPtr = AKey;
            }

            else
            {
                AKeySize = 0;
                AKeyPtr = nullptr;
            }

            esp_now_add_peer( (uint8_t *)ESPNow_Broadcast_Const::MacAddress, ESP_NOW_ROLE_COMBO, 1, AKeyPtr, AKeySize );

//            u8 AChannel;
#else
            esp_now_peer_info_t peerInfo;
            memset( &peerInfo, 0, sizeof( peerInfo ));
            memcpy( peerInfo.peer_addr, ESPNow_Broadcast_Const::MacAddress, 6 );

//            C_OWNER.MACAddress().GetMacAddress( peerInfo.peer_addr );
            peerInfo.channel = 0;
//                peerInfo.encrypt = Encryption().Enabled();
            if( Encryption().Enabled() )
            {
                peerInfo.encrypt = true;

                Encryption().Key().CopyData( peerInfo.lmk, ESP_NOW_KEY_LEN );
            }

            else
                peerInfo.encrypt = false;

//                Serial.println( "esp_now_add_peer" );
            esp_now_add_peer( &peerInfo );
#endif
            T_PeeredOutputPin::SetPinValue( true );
            FPairedCount() = Arduino_ESPNow::MAX_PEERS;
	    }

	public:
        inline void UnpairAll(void)
        {
            Disconnect();
        }

	public:
		void Print( Mitov::String AValue )
		{
			AValue += "\r\n";
			send( (uint8_t *)AValue.c_str(), AValue.length() );
		}

		void Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		inline void PrintChar( char AValue )
		{
			send( (uint8_t*)&AValue, 1 );
		}

		inline void PrintChar( uint8_t AValue )
		{
			send( &AValue, 1 );
		}

		inline void Write( uint8_t *AData, uint32_t ASize )
		{
			send( AData, ASize );
		}

		void send( const uint8_t* AData, uint8_t ASize )
		{
		    if( ! Enabled().GetValue() )
		        return;

            Peer();

            C_OWNER.DecreasePairConts_Internal( this );

//            esp_err_t result =
            esp_now_send( (uint8_t *)ESPNow_Broadcast_Const::MacAddress, (uint8_t*)AData, ASize );
//            Serial.println( "esp_now_send" );

//            if (result == ESP_OK)
//                T_SuccededOutputPin::ClockPin();

//            else
//                T_FailedOutputPin::ClockPin();

            FPairedCount() = Arduino_ESPNow::MAX_PEERS;
        }

        inline void DataSent( const uint8_t * AMac, bool ASuccess )
        {
			if( memcmp( AMac, ESPNow_Broadcast_Const::MacAddress, 6 ))
			    return;

//            Serial.println( ASuccess );
            if ( ASuccess )
                T_SuccededOutputPin::ClockPin();

            else
                T_FailedOutputPin::ClockPin();
        }

        inline void GetPeeredCount( uint8_t & ACount )
        {
            if( FPairedCount() )
                ++ ACount;

        }

		inline void GetMinCount( uint8_t & ACount )
		{
		    if( FPairedCount() )
		        if( FPairedCount() < ACount )
		            FPairedCount() = ACount;

		}

		inline void DecreasePairConts( void * AExceptInstance )
		{
            if( AExceptInstance == this )
                return;

            if( FPairedCount() > 1 )
                FPairedCount() = FPairedCount() - 1;

		}

		inline void ReleaseFirstCount( uint8_t & ACount, bool AIncludeEncrypted )
		{
		    if( ACount )
		        if( FPairedCount() == ACount )
		        {
		            if( Encryption().Enabled() )
		            {
		                if( ! AIncludeEncrypted )
		                    return;

                        if( ! Encryption().CanDisconnect().GetValue() )
		                    return;

		            }

//		            if( AIncludeEncrypted ||  )
		            Disconnect();
		            ACount = 0;
                }

		}

/*
        inline void UpdateEncryption(void)
        {
            if( C_OWNER.Encryption().Enabled() )
                Peer();

            else
                Disconnect();

        }

        inline void UpdateEncryptionProp(void)
        {
            Disconnect();
            if( C_OWNER.Encryption().Enabled() )
                Peer();

        }
*/
	public:
		inline void SystemStart(void)
        {
            T_PeeredOutputPin::SetPinValue( FPairedCount().GetValue() != 0 );
            if( Encryption().Enabled() )
                Peer();
        }

	public:
        inline TArduino_ESPNow_Broadcast()
        {
            FPairedCount() = 0;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Encryption,
		typename T_MACAddress,
		typename T_OutputPin,
		typename T_PairedOutputPin,
		typename T_Sendiing_DataSent,
		typename T_Sendiing_DecreasePairConts,
		typename T_Sendiing_GetMinCount,
		typename T_Sendiing_GetPeeredCount,
		typename T_Sendiing_ReleaseFirstCount,
		typename T_Sendiing_UnpairAll,
		typename T_Sendiing_UpdateEncryption
    > class TArduino_ESPNow_Device :
		public T_Encryption,
		public T_MACAddress,
		public T_OutputPin,
		public T_PairedOutputPin
    {
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( PairedOutputPin )

	public:
		_V_PROP_( Encryption )
		_V_PROP_( MACAddress )

    public:
        inline void UpdateEncryption(void)
        {
            T_Sendiing_UpdateEncryption::Call();
        }

    public:
        inline void ReleasePair(void)
        {
            C_OWNER.ReleasePair();
        }

        inline void DecreasePairConts_Internal( void *ADevice )
        {
            C_OWNER.DecreasePairConts_Internal( ADevice );
        }

        inline void UnpairAll(void)
        {
            T_Sendiing_UnpairAll::Call();
        }

	public:
		inline void ReceivedMessage( const uint8_t * AMac, const uint8_t *AIncomingData, int ALen )
		{
			uint8_t AMacAddress[ 6 ];
			MACAddress().GetMacAddress( AMacAddress );

//            Serial.println( MACAdressToString( AMac ) );
//            Serial.println( MACAdressToString( AMacAddress ) );

			if( memcmp( AMac, AMacAddress, 6 ))
			    return;

            T_OutputPin::SetPinValue( Mitov::TDataBlock( ALen, AIncomingData ));
		}

        inline void DataSent( const uint8_t * AMac, bool ASuccess )
        {
            T_Sendiing_DataSent::Call( AMac, ASuccess );
        }

        inline void GetPeeredCount( uint8_t & ACount )
        {
            T_Sendiing_GetPeeredCount::Call( ACount );
        }

		inline void GetMinCount( uint8_t & ACount )
		{
            T_Sendiing_GetMinCount::Call( ACount );
		}

		inline void DecreasePairConts( void * AExceptInstance )
		{
            T_Sendiing_DecreasePairConts::Call( AExceptInstance );
		}

		inline void ReleaseFirstCount( uint8_t & ACount, bool AIncludeEncrypted )
		{
            T_Sendiing_ReleaseFirstCount::Call( ACount, AIncludeEncrypted );
		}

    };
//---------------------------------------------------------------------------
	template <
	    typename T_CompletedOutputPin,
	    typename T_MACAddressOutputPin,
		typename T_OutputPin
    > class TArduino_ESPNow_Received :
	    public T_CompletedOutputPin,
	    public T_MACAddressOutputPin,
		public T_OutputPin
    {
	public:
		_V_PIN_( CompletedOutputPin )
		_V_PIN_( MACAddressOutputPin )
		_V_PIN_( OutputPin )

	public:
        void OnDataReceive( const uint8_t * AMac, const uint8_t *AIncomingData, int ALen )
        {
            T_MACAddressOutputPin::SetPinValue( MACAdressToString( AMac ) );
            T_OutputPin::SetPinValue( Mitov::TDataBlock( ALen, AIncomingData ));
            T_CompletedOutputPin::ClockPin();
        }

    };
//---------------------------------------------------------------------------
	template <
	    typename T_Elements_DataSent,
	    typename T_Elements_DecreasePairConts,
	    typename T_Elements_GetMinCount,
	    typename T_Elements_GetPeeredCount,
	    typename T_Elements_ReceivedMessage,
	    typename T_Elements_ReleaseFirstCount,
	    typename T_Elements_UnpairAll,
		typename T_Enabled,
		typename T_PrimaryKey,
		typename T_PrimaryKey_ApplyValues,
		typename T_PrimaryKey_GetValue,
		typename T_Received,
		typename T_VersionOutputPin,
		typename T_WakeWindow
	> class TArduino_ESPNow :
		public T_Enabled,
		public T_PrimaryKey,
		public T_Received,
		public T_VersionOutputPin,
		public T_WakeWindow
	{
	public:
		_V_PIN_( VersionOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( PrimaryKey )
		_V_PROP_( Received )
		_V_PROP_( WakeWindow )

    protected:
		static void *FInstance;

    protected:
#ifdef VISUINO_ESP8266
        static void OnDataSent_Static( uint8_t *AMac, uint8_t status )
#else
        static void OnDataSent_Static( const uint8_t *AMac, esp_now_send_status_t status )
#endif
        {
			((TArduino_ESPNow *)FInstance )->OnDataSent( AMac, status );
        }

#ifdef VISUINO_ESP8266
        static void OnDataReceive_Static( uint8_t * AMac, uint8_t *AIncomingData, uint8_t ALen )
#else
        static void OnDataReceive_Static( const uint8_t * AMac, const uint8_t *AIncomingData, int ALen )
#endif
        {
			((TArduino_ESPNow *)FInstance )->OnDataReceive( AMac, AIncomingData, ALen );
        }

	public:
        void ReleasePair(void)
        {
            uint8_t ACount = 0;

            T_Elements_GetPeeredCount::Call( ACount );
            if( ACount < Arduino_ESPNow::MAX_PEERS )
                return;

            ACount = Arduino_ESPNow::MAX_PEERS;

            T_Elements_GetMinCount::Call( ACount );

            if( ACount )
            {
                T_Elements_ReleaseFirstCount::Call( ACount, false );

                if( ACount )
                    T_Elements_ReleaseFirstCount::Call( ACount, true );
            }

        }

        void DecreasePairConts_Internal( void *ADevice )
        {
            T_Elements_DecreasePairConts::Call( ADevice );
        }

	public:
        inline void OnDataReceive( const uint8_t * AMac, const uint8_t *AIncomingData, int ALen )
        {
			T_Received::OnDataReceive( AMac, AIncomingData, ALen );

			T_Elements_ReceivedMessage::Call( AMac, AIncomingData, ALen );
        }

#ifdef VISUINO_ESP8266
        inline void OnDataSent( const uint8_t *mac_addr, uint8_t status )
#else
        inline void OnDataSent( const uint8_t *mac_addr, esp_now_send_status_t status )
#endif
        {
            T_Elements_DataSent::Call( mac_addr, status == ESP_NOW_SEND_SUCCESS );
        }

	public:
        void UpdateEnabled(void)
        {
            if( Enabled() )
            {
//                Serial.println( "esp_now_init" );
                esp_now_init();
#ifdef VISUINO_ESP8266
                esp_now_set_self_role( ESP_NOW_ROLE_COMBO );
#endif
                esp_now_register_send_cb( OnDataSent_Static );
                esp_now_register_recv_cb( OnDataReceive_Static );

#ifndef VISUINO_ESP8266
                esp_now_set_wake_window( WakeWindow() );
#endif
            }

            else
            {
                T_Elements_UnpairAll::Call();
                esp_now_unregister_send_cb();
                esp_now_unregister_recv_cb();
                esp_now_deinit();
            }
        }

        void UpdatePrimaryKey(void)
        {
            if( ! Enabled().GetValue() )
                return;

//            if( ! PrimaryEncryption().Enabled().GetValue() )
//                return;

            uint8_t AKey[ ESP_NOW_KEY_LEN ];
            PrimaryKey().CopyData( AKey, ESP_NOW_KEY_LEN );

#ifdef VISUINO_ESP8266
            esp_now_set_kok( AKey, ESP_NOW_KEY_LEN );
#else

            esp_now_set_pmk( AKey );
#endif
        }

        void UpdateWakeWindow(void)
        {
            if( ! Enabled().GetValue() )
                return;

            esp_now_set_wake_window( WakeWindow() );
        }

	public:
		inline void SystemInit(void)
		{
            UpdateEnabled();
		}

		inline void SystemStart(void)
		{
//		    delay( 1000 );
//		    Serial.println();
#ifndef VISUINO_ESP8266
		    uint32_t AVersion = 0;
		    esp_now_get_version( &AVersion );

		    T_VersionOutputPin::SetPinValue( AVersion );
#endif
		}

    public:
        inline TArduino_ESPNow(void)
        {
            FInstance = this;
        }

	};
//---------------------------------------------------------------------------
    template <
	    typename T_Elements_DataSent,
	    typename T_Elements_DecreasePairConts,
	    typename T_Elements_GetMinCount,
	  	typename T_Elements_GetPeeredCount,
        typename T_Elements_ReceivedMessage,
	    typename T_Elements_ReleaseFirstCount,
	    typename T_Elements_UnpairAll,
		typename T_Enabled,
		typename T_PrimaryKey,
		typename T_PrimaryKey_ApplyValues,
		typename T_PrimaryKey_GetValue,
		typename T_Received,
		typename T_VersionOutputPin,
		typename T_WakeWindow
	> void * TArduino_ESPNow<
	    T_Elements_DataSent,
	    T_Elements_DecreasePairConts,
	    T_Elements_GetMinCount,
	    T_Elements_GetPeeredCount,
	    T_Elements_ReceivedMessage,
	    T_Elements_ReleaseFirstCount,
	    T_Elements_UnpairAll,
		T_Enabled,
		T_PrimaryKey,
		T_PrimaryKey_ApplyValues,
		T_PrimaryKey_GetValue,
		T_Received,
		T_VersionOutputPin,
		T_WakeWindow
		>::FInstance = nullptr;
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif