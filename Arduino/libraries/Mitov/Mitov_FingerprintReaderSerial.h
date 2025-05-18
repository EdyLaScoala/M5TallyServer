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

// #define __FINGERPRINT_DEBUG__
// #define __FINGERPRINT_STATE_DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
    namespace TArduinoFingerprintReader
    {
        // Internal status codes
        static const uint8_t FINGERPRINT_OK                     = 0x00; //!< Command execution is complete
        static const uint8_t FINGERPRINT_PACKETRECIEVEERR       = 0x01; //!< Error when receiving data package
        static const uint8_t FINGERPRINT_NOFINGER               = 0x02; //!< No finger on the sensor
        static const uint8_t FINGERPRINT_IMAGEFAIL              = 0x03; //!< Failed to enroll the finger
        static const uint8_t FINGERPRINT_IMAGEMESS              = 0x06; //!< Failed to generate character file due to overly disorderly fingerprint image
        static const uint8_t FINGERPRINT_FEATUREFAIL            = 0x07; //!< Failed to generate character file due to the lack of character point or small fingerprint image
        static const uint8_t FINGERPRINT_NOMATCH                = 0x08; //!< Finger doesn't match
        static const uint8_t FINGERPRINT_NOTFOUND               = 0x09; //!< Failed to find matching finger
        static const uint8_t FINGERPRINT_ENROLLMISMATCH         = 0x0A; //!< Failed to combine the character files
        static const uint8_t FINGERPRINT_BADLOCATION            = 0x0B; //!< Addressed PageID is beyond the finger library
        static const uint8_t FINGERPRINT_DBRANGEFAIL            = 0x0C; //!< Error when reading template from library or invalid template
        static const uint8_t FINGERPRINT_UPLOADFEATUREFAIL      = 0x0D; //!< Error when uploading template
        static const uint8_t FINGERPRINT_PACKETRESPONSEFAIL     = 0x0E; //!< Module failed to receive the following data packages
        static const uint8_t FINGERPRINT_UPLOADFAIL             = 0x0F; //!< Error when uploading image
        static const uint8_t FINGERPRINT_DELETEFAIL             = 0x10; //!< Failed to delete the template
        static const uint8_t FINGERPRINT_DBCLEARFAIL            = 0x11; //!< Failed to clear finger library
        static const uint8_t FINGERPRINT_PASSFAIL               = 0x13; //!< Find whether the fingerprint passed or failed
        static const uint8_t FINGERPRINT_INVALIDIMAGE           = 0x15; //!< Failed to generate image because of lac of valid primary image
        static const uint8_t FINGERPRINT_FLASHERR               = 0x18; //!< Error when writing flash
        static const uint8_t FINGERPRINT_INVALIDREG             = 0x1A; //!< Invalid register number
        static const uint8_t FINGERPRINT_ADDRCODE               = 0x20; //!< Address code
        static const uint8_t FINGERPRINT_PASSVERIFY             = 0x21; //!< Verify the fingerprint passed

        // Additional status codes
//        static const uint8_t FINGERPRINT_STATUS_NO_ERROR        = 0;
        static const uint8_t FINGERPRINT_STATUS_PROCESSING      = 0x80;
        static const uint8_t FINGERPRINT_STATUS_CHECKSUM_ERROR  = 0x81;
        static const uint8_t FINGERPRINT_STATUS_TIMEOUT_ERROR   = 0x82;

		static const uint8_t FINGERPRINT_COMMANDPACKET	        = 0x01; //!< Command packet
		static const uint8_t FINGERPRINT_DATAPACKET	            = 0x02; //!< Data packet
        static const uint8_t FINGERPRINT_ACKPACKET              = 0x07; //!< Acknowledge packet
		static const uint8_t FINGERPRINT_END_OF_DATAPACKET      = 0x08; //!< End Of Data packet

		static const uint8_t FINGERPRINT_GETIMAGE		        = 0x01; //!< Collect finger image
		static const uint8_t FINGERPRINT_IMAGE2TZ		        = 0x02; //!< Generate character file from image
		static const uint8_t FINGERPRINT_SEARCH			        = 0x04; //!< Search for fingerprint in slot
		static const uint8_t FINGERPRINT_REGMODEL		        = 0x05; //!< Combine character files and generate template
		static const uint8_t FINGERPRINT_STORE			        = 0x06; //!< Store template
		static const uint8_t FINGERPRINT_LOAD			        = 0x07; //!< Read/load template
		static const uint8_t FINGERPRINT_UPLOAD_TEMPLATE        = 0x08; //!< Upload template
		static const uint8_t FINGERPRINT_DOWNLOAD_TEMPLATE      = 0x09; //!< Download template
		static const uint8_t FINGERPRINT_UPLOAD_IMAGE           = 0x0A; //!< Upload template
		static const uint8_t FINGERPRINT_DOWNLOAD_IMAGE         = 0x0B; //!< Download template
		static const uint8_t FINGERPRINT_DELETE			        = 0x0C; //!< Delete templates
		static const uint8_t FINGERPRINT_EMPTY			        = 0x0D; //!< Empty library
		static const uint8_t FINGERPRINT_SETSYSPARAM	        = 0x0E; //!< Set system parameter
		static const uint8_t FINGERPRINT_READSYSPARAM	        = 0x0F; //!< Read system parameters
		static const uint8_t FINGERPRINT_SETPASSWORD	        = 0x12; //!< Sets passwords
		static const uint8_t FINGERPRINT_VERIFYPASSWORD         = 0x13; //!< Verifies the password
		static const uint8_t FINGERPRINT_GETRANDOMCODE          = 0x14; //!< Generate Random Code
		static const uint8_t FINGERPRINT_SETADDRESS 	        = 0x15; //!< Sets Address
		static const uint8_t FINGERPRINT_NOTEPAD_WRITE          = 0x18; //!< Writes to Notepad
		static const uint8_t FINGERPRINT_NOTEPAD_READ           = 0x19; //!< Reads from Notepad
		static const uint8_t FINGERPRINT_HISPEEDSEARCH          = 0x1B; //!< Asks the sensor to search for a matching fingerprint template to the last model generated
		static const uint8_t FINGERPRINT_TEMPLATECOUNT	        = 0x1D; //!< Read finger template numbers
		static const uint8_t FINGERPRINT_LIST_TEMPLATES	        = 0x1F; //!< Read template index table
		static const uint8_t FINGERPRINT_AURALEDCONFIG	        = 0x35; //!< Aura LED control
		static const uint8_t FINGERPRINT_RESET      	        = 0x3D; //!< Reset
		static const uint8_t FINGERPRINT_LEDON			        = 0x50; //!< Turn on the onboard LED
		static const uint8_t FINGERPRINT_LEDOFF			        = 0x51; //!< Turn off the onboard LED
		static const uint8_t FINGERPRINT_HANDSHAKE		        = 0x53; //!< Handshake
		static const uint16_t FINGERPRINT_STARTCODE		        = 0xEF01; //!< Fixed falue of EF01H; High byte transferred first

//		const uint8_t C_PACKET_HEADER[] = { FINGERPRINT_STARTCODE >> 8, FINGERPRINT_STARTCODE, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x03, 0x00, 0x00, 0x0A };
//		const uint8_t C_ACK_PASSWORD[] = { FINGERPRINT_ACKPACKET, 0x00, 0x03, 0x00, 0x00, 0x0A };
        static const uint8_t DATA_INDEX = 10;

        static const uint16_t IMAGE_WIDTH = 256;
        static const uint16_t IMAGE_HEIGHT = 288;
        static const uint16_t IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT / 2;
        static const uint16_t FEATURES_SIZE = 6 * 128;
        static const uint8_t  FEATURES_LIST_SIZE = 32;
        static const uint8_t  NOTEPAD_PAGE_SIZE = 32;
    }
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_ConfidenceOutputPin,
        typename T_DetectedOutputPin,
		typename T_Enabled,
		typename T_ErrorOutputPin,
//        typename T_FIsLastPacket,
        typename T_FClocked,
        typename T_FImageData,
        typename T_FState,
        typename T_FeaturesOutputPin,
        typename T_FingerprintIndexOutputPin,
        typename T_ImageOutputPin,
		typename T_RecognizedOutputPin,
        typename T_UnknownOutputPin
	> class TArduinoFingerprintReaderScan :
        public T_ConfidenceOutputPin,
        public T_DetectedOutputPin,
		public T_Enabled,
		public T_ErrorOutputPin,
//        public T_FIsLastPacket,
        public T_FClocked,
        public T_FImageData,
        public T_FState,
        public T_FeaturesOutputPin,
        public T_FingerprintIndexOutputPin,
        public T_ImageOutputPin,
		public T_RecognizedOutputPin,
        public T_UnknownOutputPin
	{
        enum TState
        {
            eStandby,
            eGetImage,
            eUploadImage,
            eConvert,
            eUploadFeatures,
            eSearch,
            eDownloadImage
        };

	public:
		_V_PIN_( ConfidenceOutputPin )
        _V_PIN_( DetectedOutputPin )
		_V_PIN_( ErrorOutputPin )
        _V_PIN_( FeaturesOutputPin )
		_V_PIN_( FingerprintIndexOutputPin )
		_V_PIN_( RecognizedOutputPin )
		_V_PIN_( UnknownOutputPin )

	public:
		_V_PROP_( Enabled )

    protected:
        _V_PROP_( FClocked )
        _V_PROP_( FImageData )
        _V_PROP_( FState )
//        _V_PROP_( FIsLastPacket )

    protected:
        void StartSearch()
        {
            SetState( eSearch );

            uint8_t AData[ 5 ];
            AData[ 0 ] = 1;
            AData[ 1 ] = 0x00;
            AData[ 2 ] = 0x00;
            AData[ 3 ] = C_OWNER.FCapacity().GetValue() >> 8;
            AData[ 4 ] = C_OWNER.FCapacity().GetValue();
            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_SEARCH, AData, 5 );
        }

        void StartConvert()
        {
            SetState( eConvert );
            C_OWNER.StartConvert( 1 );
        }

        void CompleteProcessing()
        {
            C_OWNER.FDataIndex = 0;
            SetState( eStandby );
            C_OWNER.ReleaseBuffer();
            C_OWNER.Processed();
        }

        void DownloadImage()
        {
            SetState( eDownloadImage );
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_DOWNLOAD_IMAGE );
        }

        bool CanConvert()
        {
            return
                C_OWNER.GetNeedsRecognition() ||
                T_RecognizedOutputPin::GetPinIsConnected() ||
                T_UnknownOutputPin::GetPinIsConnected() ||
                T_FingerprintIndexOutputPin::GetPinIsConnected() ||
                T_ConfidenceOutputPin::GetPinIsConnected() ||
                T_ErrorOutputPin::GetPinIsConnected() ||
                T_FeaturesOutputPin::GetPinIsConnected();
        }

        inline void SetState( TState AState )
        {
#ifdef __FINGERPRINT_STATE_DEBUG__
//            if( FState().GetValue() > 1 )
            {
                Serial.print( "SCAN STATE: " );
                Serial.println( AState );
            }
//            Serial.print( " STAGE: " );
//            Serial.println( FSecondStage().GetValue() );
#endif // __FINGERPRINT_STATE_DEBUG__
            FState() = AState;
        }

	public:
        inline void InvalidPacket()
        {
            CompleteProcessing();
        }

        inline void TimeoutProcess()
        {
            SetState( eStandby );
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
//uint32_t AIndex = C_OWNER.FDataIndex;

//uint32_t AState = FState();

            switch ( FState() )
            {
                case eGetImage :
                {
                    if( APacketEnd )
                    {
//                        Serial.println( "STEP 1" );
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
                            T_DetectedOutputPin::ClockPin();
                            if( T_ImageOutputPin::GetPinIsConnected() )
                            {
                                SetState( eUploadImage );
                                C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_UPLOAD_IMAGE );
                            }

                            else
                            {
                                if( CanConvert() )
                                    StartConvert();

                                else
                                    CompleteProcessing();

                            }
                        }
                    }

                    break;
                }

                case eUploadImage :
                {
                    if( C_OWNER.FReceivingData() )
                    {
                        if( APacketEnd )
                        {

    //                        Serial.println( FCount );

    /*
                            if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                                CompleteProcessing();

                            else
    */
    //                        if( FIsLastPacket().GetValue() )
//                            if( FState().GetValue() == eUploadImageLastData )
                            if( C_OWNER.FReceivingLastData() )
                            {
                                C_OWNER.ReleaseBuffer();

                                if( T_RecognizedOutputPin::GetPinIsConnected() || T_UnknownOutputPin::GetPinIsConnected() || T_FeaturesOutputPin::GetPinIsConnected() )
                                    StartConvert();

                                else
                                    CompleteProcessing();

                            }
                        }

                        else
                        {
                            if( C_OWNER.FData )
                            {
                                if( C_OWNER.FCount < TArduinoFingerprintReader::IMAGE_SIZE )
                                {
                                    C_OWNER.FData[ C_OWNER.FCount ++ ] = AValue;
                                    if( C_OWNER.FCount == TArduinoFingerprintReader::IMAGE_SIZE )
                                    {
                                        TImageBuffer AImage( TImageFormat::Gray4, TArduinoFingerprintReader::IMAGE_WIDTH, TArduinoFingerprintReader::IMAGE_HEIGHT, C_OWNER.FData );

                                        T_ImageOutputPin::SetPinValue( AImage );

                                        C_OWNER.ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
//                            SetState( eUploadImageData );
                            C_OWNER.StartReadBuffer( TArduinoFingerprintReader::IMAGE_SIZE );
//                            FDataPtr = FData;
                        }

                    }

                    break;
                }

                case eConvert :
                {
                    if( APacketEnd )
                    {
//                        Serial.println( "eConvert" );
//                        Serial.println( C_OWNER.FConfirmationCode );
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
                            if( T_FeaturesOutputPin::GetPinIsConnected() )
                            {
                                SetState( eUploadFeatures );
                                C_OWNER.UploadTemplate();
                            }

                            else
                                StartSearch();

                        }
                    }

                    break;
                }

                case eUploadFeatures :
                {
                    if( C_OWNER.FReceivingData() )
                    {
                        if( APacketEnd )
                        {
    //                        if( FIsLastPacket().GetValue() )
//                            if( FState().GetValue() == eUploadFeaturesLastData )
                            if( C_OWNER.FReceivingLastData() )
                            {
                                C_OWNER.ReleaseBuffer();

    //                        Serial.println( FCount );

                                if( T_RecognizedOutputPin::GetPinIsConnected() || T_UnknownOutputPin::GetPinIsConnected() )
                                    StartSearch();

                                else
                                    CompleteProcessing();

                            }
                        }

                        else
                        {
    //                        Serial.println( AValue, HEX );
                            if( C_OWNER.FData )
                            {
                                if( C_OWNER.FCount < TArduinoFingerprintReader::FEATURES_SIZE + 1 )
                                {
                                    C_OWNER.FData[ C_OWNER.FCount ++ ] = AValue;
                                    if( C_OWNER.FCount == TArduinoFingerprintReader::FEATURES_SIZE )
                                    {
                                        T_FeaturesOutputPin::SetPinValue( Mitov::TDataBlock( C_OWNER.FCount, C_OWNER.FData ));

                                        C_OWNER.ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
//                            SetState( eUploadFeaturesData );
                            C_OWNER.StartReadBuffer( TArduinoFingerprintReader::FEATURES_SIZE );
//                            FDataPtr = FData;
                        }

                    }

                    break;
                }

                case eSearch :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            T_UnknownOutputPin::ClockPin();

                        else
                            T_RecognizedOutputPin::ClockPin();

                        CompleteProcessing();
                    }

                    switch ( C_OWNER.FDataIndex )
                    {
                        case 10:
                        case 12:
                            C_OWNER.FFirstByte = AValue;
                            break;

                        case 11:
                        {
                            uint16_t AOutValue = ( uint16_t( C_OWNER.FFirstByte ) << 8 ) | AValue;
                            T_FingerprintIndexOutputPin::SetPinValue( AOutValue );
                            C_OWNER.Recognized( AOutValue );
                            break;
                        }

                        case 13:
                        {
                            uint16_t AOutValue = ( uint16_t( C_OWNER.FFirstByte ) << 8 ) | AValue;
                            T_ConfidenceOutputPin::SetPinValue( AOutValue );
                            break;
                        }

                    }

//                    FFirstByte
                    break;
                }

                case eDownloadImage :
                {
                    if( APacketEnd )
                    {
                        uint8_t *ABuffer = ( uint8_t *)FImageData().GetValue();
                        if( ABuffer )
                        {
                            if( ASuccess )
                            {
                                C_OWNER.writeData( ABuffer, TArduinoFingerprintReader::IMAGE_SIZE );

//                                Serial.println( "DOWNLOADED" );
                                delete [] ABuffer;
                                FImageData() = nullptr;

//                                Serial.println( "START: eStoreTemplate" );
                                StartConvert();
                            }

                            else
                                CompleteProcessing();

                        }

                        else
                            CompleteProcessing();

                    }

                    break;
                }

            }
//            if(
//            ++ C_OWNER.FDataIndex;
		}

    protected:
        bool RequestImage()
        {
//            if( FState() != eStandby )
//                if( C_OWNER.GetInProcessing() )
//                    return false;

            if( ! C_OWNER.GetIsReady() )
                return false;

            if( C_OWNER.GetInProcessing() )
                return false;

            SetState( eGetImage );
            FClocked() = false;
            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
            return true;
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( ! RequestImage())
                FClocked() = true;
        }

		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! CanConvert() )
                return;

            if( FState() == eDownloadImage )
                return;

            TImageBuffer &AImage = *(TImageBuffer *)_Data;
            uint32_t AImageSize = AImage.GetSize();

            if( FImageData().GetValue() )
                delete [] FImageData().GetValue();

            FImageData() = new uint8_t[ TArduinoFingerprintReader::IMAGE_SIZE ];
            uint32_t ACopySize = MitovMin<uint32_t>( TArduinoFingerprintReader::IMAGE_SIZE, AImageSize );
            memcpy( FImageData().GetValue(), AImage.Data, ACopySize );
            memset( (( uint8_t *) FImageData().GetValue() ) + ACopySize, 0, TArduinoFingerprintReader::IMAGE_SIZE - ACopySize );
//            eDownloadImage
        }

	public:
		inline void SystemLoopBegin()
        {
            if( FImageData().GetValue() )
            {
//                Serial.println( "Try Download" );
                if( ! Enabled().GetValue() )
                    return;

                if( FState().GetValue() != eStandby )
                    return;

                if( C_OWNER.GetInProcessing() )
                    return;

                if( ! C_OWNER.GetIsReady() )
                    return;

//                Serial.println( "Download" );
                DownloadImage();
            }
        }

		inline void SystemLoopEnd()
		{
            if( FClocked() )
                RequestImage();
		}

	public:
        inline TArduinoFingerprintReaderScan()
        {
            FClocked() = false;
            FState() = eStandby;
//            FIsLastPacket() = false;
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked
    > class TArduinoFingerprintReaderDeleteAllTemplates :
        public T_Enabled,
        public T_FClocked
    {
	public:
		_V_PROP_( Enabled )

    protected:
		_V_PROP_( FClocked )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
        }

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_EMPTY );
            C_OWNER.RequestCount();
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderDeleteAllTemplates()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked
    > class TArduinoFingerprintReaderResetElement :
        public T_Enabled,
        public T_FClocked
    {
	public:
		_V_PROP_( Enabled )

    protected:
		_V_PROP_( FClocked )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
        }

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
//            Serial.println( "FINGERPRINT_RESET" );
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_RESET );
            C_OWNER.UpdateEnabled();
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderResetElement()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_AddressOutputPin,
        typename T_CapacityOutputPin,
        typename T_Enabled,
        typename T_FClocked,
        typename T_FProcessing,
        typename T_PacketLenOutputPin,
        typename T_SecurityLevelOutputPin,
        typename T_SerialSpeedOutputPin,
        typename T_StatusOutputPin,
        typename T_SystemIDOutputPin
    > class TArduinoFingerprintReaderGetInfo :
        public T_AddressOutputPin,
        public T_CapacityOutputPin,
        public T_Enabled,
        public T_FClocked,
        public T_FProcessing,
        public T_PacketLenOutputPin,
        public T_SecurityLevelOutputPin,
        public T_SerialSpeedOutputPin,
        public T_StatusOutputPin,
        public T_SystemIDOutputPin
    {
	public:
		_V_PIN_( AddressOutputPin )
		_V_PIN_( CapacityOutputPin )
		_V_PIN_( PacketLenOutputPin )
		_V_PIN_( SecurityLevelOutputPin )
		_V_PIN_( SerialSpeedOutputPin )
		_V_PIN_( StatusOutputPin )
		_V_PIN_( SystemIDOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
        _V_PROP_( FClocked )
		_V_PROP_( FProcessing )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}

        inline void TimeoutProcess()
        {
            FProcessing() = false;
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
            if( ! FProcessing().GetValue() )
                return;

            uint32_t AIndex = C_OWNER.FDataIndex;
            if( C_OWNER.FDataIndex == TArduinoFingerprintReader::DATA_INDEX )
                C_OWNER.AllocateBuffer( 16 );

            if( APacketEnd )
            {
                if( ASuccess )
                {
                    if( C_OWNER.FData )
                    {
                        T_StatusOutputPin::SetPinValue( ( uint16_t( C_OWNER.FData[ 0 ] ) << 8 ) | C_OWNER.FData[ 1 ] );
                        T_SystemIDOutputPin::SetPinValue( ( uint16_t( C_OWNER.FData[ 2 ] ) << 8 ) | C_OWNER.FData[ 3 ] );
                        T_CapacityOutputPin::SetPinValue( ( uint16_t( C_OWNER.FData[ 4 ] ) << 8 ) | C_OWNER.FData[ 5 ] );
                        T_SecurityLevelOutputPin::SetPinValue( ( uint16_t( C_OWNER.FData[ 6 ] ) << 8 ) | C_OWNER.FData[ 7 ] );
                        T_AddressOutputPin::SetPinValue( ( ( uint32_t( C_OWNER.FData[ 8 ] ) << 24 ) | ( uint32_t( C_OWNER.FData[ 9 ] ) << 16 ) | ( uint32_t( C_OWNER.FData[ 10 ] ) << 8 ) | C_OWNER.FData[ 11 ] ));
                        T_PacketLenOutputPin::SetPinValue( 0b100000 << ( ( uint16_t( C_OWNER.FData[ 12 ] ) << 8 ) | C_OWNER.FData[ 13 ] ));
                        T_SerialSpeedOutputPin::SetPinValue( (( uint16_t( C_OWNER.FData[ 14 ] ) << 8 ) | C_OWNER.FData[ 15 ] ) * 9600 );
                    }
                }

                C_OWNER.ReleaseBuffer();
                C_OWNER.Processed();
                return;
            }

            if( C_OWNER.FDataIndex >= TArduinoFingerprintReader::DATA_INDEX )
            {
                C_OWNER.FData[ C_OWNER.FDataIndex - TArduinoFingerprintReader::DATA_INDEX ] = AValue;
            }

        }

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_READSYSPARAM );
            FProcessing() = true;
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        TArduinoFingerprintReaderGetInfo()
        {
            FClocked() = false;
            FProcessing() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_ClockInputPin_o_IsConnected,
        typename T_CompletedOutputPin,
        typename T_Count,
        typename T_Enabled,
        typename T_FClocked,
        typename T_FState,
        typename T_FTableIndex,
        typename T_FeaturesOutputPin,
        typename T_IndexOutputPin,
        typename T_PopulatedOutputPin,
        typename T_StartIndex,
        typename T_TemplateProcessedOutputPin
    > class TArduinoFingerprintReaderListTemplates :
        public T_ClockInputPin_o_IsConnected,
        public T_CompletedOutputPin,
        public T_Count,
        public T_Enabled,
        public T_FClocked,
        public T_FState,
        public T_FTableIndex,
        public T_FeaturesOutputPin,
        public T_IndexOutputPin,
        public T_PopulatedOutputPin,
        public T_StartIndex,
        public T_TemplateProcessedOutputPin
    {
	public:
		_V_PROP_( Count )
		_V_PROP_( Enabled )
		_V_PROP_( StartIndex )

	protected:
        _V_PROP_( ClockInputPin_o_IsConnected )

        _V_PROP_( FClocked )
		_V_PROP_( FState )
		_V_PROP_( FTableIndex )

        uint8_t FRecordIndex = 0;
        uint8_t *FTableData = nullptr;

        enum TState
        {
            eStandby,
            eGetTable,
            eExtractTemplate,
            eUploadFeatures
//            eUploadFeaturesData,
//            eUploadFeaturesLastData
        };

	protected:
        inline void SetState( TState AState )
        {
#ifdef __FINGERPRINT_STATE_DEBUG__
//            if( FState().GetValue() > 1 )
            {
                Serial.print( "LIST STATE: " );
                Serial.println( AState );
            }
//            Serial.print( " STAGE: " );
//            Serial.println( FSecondStage().GetValue() );
#endif // __FINGERPRINT_STATE_DEBUG__
            FState() = AState;
        }

        void ExecuteNextStep()
        {
//            switch (
//              case
            uint16_t ALastIndex = StartIndex().GetValue() + Count().GetValue();
            uint16_t AStartIndex = FTableIndex() * 256;
            ALastIndex -= AStartIndex;
            uint16_t ACapacity = C_OWNER.FCapacity().GetValue();
            ACapacity -= AStartIndex;

            if( ALastIndex > 256 )
                ALastIndex = 256;

            if( ALastIndex > ACapacity )
                ALastIndex = ACapacity;

/*
            uint16_t ATableStartIndex;
            if( StartIndex().GetValue() < AStartIndex )
                ATableStartIndex = 0;

            else
                ATableStartIndex = FRecordIndex - AStartIndex;
*/
            if( FRecordIndex < FTableIndex() * 256 )
                FRecordIndex = 0;

            else
                FRecordIndex -= FTableIndex() * 256;

            for( int i = FRecordIndex; i < ALastIndex; ++ i )
            {
                uint8_t AIndex = i >> 3;
                uint8_t AOffset = i & 0b111;
                uint8_t AMask = ( 1 << AOffset );
                bool APopulated = (( FTableData[ AIndex ] & AMask ) != 0 );
                T_IndexOutputPin::SetPinValue( AStartIndex + i );
                T_PopulatedOutputPin::SetPinValue( APopulated );
                if( APopulated )
                {
                    FTableData[ AIndex ] &= ~AMask;

                    uint16_t AID = ( FTableIndex().GetValue() << 8 ) | i;
//                    C_OWNER.LoadFingerprintFromTable( AID );

                    if( T_FeaturesOutputPin::GetPinIsConnected() )
                    {
                        FRecordIndex = AID + 1;
                        SetState( eExtractTemplate );
                        C_OWNER.LoadFingerprintFromTable( AID );
                        return;
                    }
                }

                T_TemplateProcessedOutputPin::ClockPin();
            }

            delete [] FTableData;
            FTableData = nullptr;

//            uint16_t ACapacity = C_OWNER.FCapacity().GetValue();
            uint8_t AMaxTablesIndex = ( C_OWNER.FCapacity().GetValue() >> 8 );

            if( FTableIndex() < AMaxTablesIndex )
            {
                FTableIndex() = FTableIndex() + 1;
                RequestTable();
            }

            else
                Reset();
        }

        void Reset()
        {
            T_CompletedOutputPin::ClockPin();
            SetState( eStandby );
            C_OWNER.Processed();
            FTableIndex() = 0;
        }

        void RequestTable()
        {
			uint8_t AData = FTableIndex();
            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_LIST_TEMPLATES, &AData, 1 );
            SetState( eGetTable );
        }

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}

        inline void TimeoutProcess()
        {
            Reset();
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
            if( FState().GetValue() == eStandby )
                return;

            switch( FState().GetValue() )
            {
                case eStandby :
                    return;

                case eGetTable :
                {
                    uint32_t AIndex = C_OWNER.FDataIndex;
                    if( C_OWNER.FDataIndex == TArduinoFingerprintReader::DATA_INDEX )
                        C_OWNER.AllocateBuffer( TArduinoFingerprintReader::FEATURES_LIST_SIZE );

                    if( APacketEnd )
                    {
                        if( ASuccess )
                        {
                            if( C_OWNER.FData )
                            {
                                FTableData = C_OWNER.FData;
                                C_OWNER.FData = nullptr;
                                ExecuteNextStep();
                            }
                        }

                        else
                            Reset();
    
                        return;
                    }

                    if( C_OWNER.FDataIndex >= TArduinoFingerprintReader::DATA_INDEX )
                    {
                        C_OWNER.FData[ C_OWNER.FDataIndex - TArduinoFingerprintReader::DATA_INDEX ] = AValue;
                    }

                    break;
                }

                case eExtractTemplate :
                {
                    if( APacketEnd )
                    {
                        if( ASuccess )
                        {
                            SetState( eUploadFeatures );
                            C_OWNER.UploadTemplate();
//                            ExecuteNextStep();
                        }

                        else
                            Reset();

                    }

                    break;
                }

                case eUploadFeatures :
                {
                    if( C_OWNER.FReceivingData() )
                    {
                        if( APacketEnd )
                        {
//                            if( FState().GetValue() == eUploadFeaturesLastData )
                            if( C_OWNER.FReceivingLastData() )
                            {
                                C_OWNER.ReleaseBuffer();

                                ExecuteNextStep();
    //                            NextTable();
    //                        Serial.println( FCount );

    /*
                                if( T_RecognizedOutputPin::GetPinIsConnected() || T_UnknownOutputPin::GetPinIsConnected() )
                                    StartSearch();

                                else
                                    CompleteProcessing();

    */
                            }
                        }

                        else
                        {
    //                        Serial.println( AValue, HEX );
                            if( C_OWNER.FData )
                            {
                                if( C_OWNER.FCount < TArduinoFingerprintReader::FEATURES_SIZE + 1 )
                                {
                                    C_OWNER.FData[ C_OWNER.FCount ] = AValue;
                                    ++ C_OWNER.FCount;
                                    if( C_OWNER.FCount == TArduinoFingerprintReader::FEATURES_SIZE )
                                    {
                                        T_FeaturesOutputPin::SetPinValue( Mitov::TDataBlock( C_OWNER.FCount, C_OWNER.FData ));

                                        T_TemplateProcessedOutputPin::ClockPin();
                                        C_OWNER.ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            Reset();

                        else
                        {
//                            SetState( eUploadFeaturesData );
                            C_OWNER.StartReadBuffer( TArduinoFingerprintReader::FEATURES_SIZE );
//                            FDataPtr = FData;
                        }

                    }

                    break;
                }
            }

        }

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
            FTableIndex() = StartIndex().GetValue() / 256;
            FRecordIndex = StartIndex().GetValue(); // & 0xFF is applied automatically

            if( FTableIndex() > 3 )
                return;

            RequestTable();
/*
			uint8_t AData = FTableIndex();
            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_LIST_TEMPLATES, &AData, 1 );
            SetState( eGetTable );
*/
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        TArduinoFingerprintReaderListTemplates()
        {
            FClocked() = ! ClockInputPin_o_IsConnected().GetValue();
            FState() = eStandby;
            FTableIndex() = 0;
        }

    };
//---------------------------------------------------------------------------
	template <
//		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_FClocked,
        typename T_FImageData,
        typename T_OutputPin
    > class TArduinoFingerprintReaderTemplateEnrollImage :
        public T_FClocked,
        public T_FImageData,
        public T_OutputPin
    {
    public:
        _V_PIN_( OutputPin )

    public:
        _V_PROP_( FClocked )
        _V_PROP_( FImageData )

/*
    public:
        inline void SendImage( Mitov::TImageBuffer &AImage )
        {
            T_OutputPin::SetPinValue( AImage );
        }

        inline void IsOutputConnected( bool &AValue )
        {
            AValue |= T_OutputPin::GetPinIsConnected();
        }

    public:
		inline void StartInputPin_o_Receive( void *_Data )
		{
            C_OWNER
        }
*/
    public:
        inline TArduinoFingerprintReaderTemplateEnrollImage()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_FClocked,
        typename T_FLastTime,
        typename T_FSecondStage,
        typename T_FState,
        typename T_FailedOutputPin,
        typename T_FingerprintsCountOutputPin,
//        typename T_Image_IsOutputConnected,
//        typename T_Image_SendImage,
        typename T_Image,
        typename T_LearnedOutputPin,
        typename T_PlaceFingerOutputPin,
        typename T_RemoveFingerOutputPin,
        typename T_Timeout,
        typename T_WaitingFingerprintOutputPin
    > class TArduinoFingerprintReaderTemplateEnroll :
        public T_FClocked,
        public T_FLastTime,
        public T_FSecondStage,
        public T_FState,
        public T_FailedOutputPin,
        public T_FingerprintsCountOutputPin,
        public T_Image,
        public T_LearnedOutputPin,
        public T_PlaceFingerOutputPin,
        public T_RemoveFingerOutputPin,
        public T_Timeout,
        public T_WaitingFingerprintOutputPin
    {
    public:
        _V_PIN_( FailedOutputPin )
        _V_PIN_( FingerprintsCountOutputPin )

    public:
        _V_PROP_( Image )
        _V_PROP_( Timeout )

    protected:
        _V_PROP_( FClocked )
        _V_PROP_( FSecondStage )
        _V_PROP_( FLastTime )
        _V_PROP_( FState )

    protected:
        enum TState
        {
            eStandby,
            eTakeImage,
            eUploadImage,
            eConvert,
            eRemoveFinger,
            eCreateModel,
            eUploadFeatures,
            eStoreModel,
            eDownloadImage
        };

    protected:
        void ProcessClock()
        {
            if( ! C_OWNER.Enabled().GetValue() )
                return;

//            Serial.print( " <.> " );
            if( FState().GetValue() != eStandby )
            {
                FClocked() = true;
                return;
            }

//            Serial.print( " <+> " );
            if( C_OWNER.GetOwner().GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

//            Serial.print( " <*> " );
            if( ! C_OWNER.GetOwner().GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
            FSecondStage() = false;
            SetState( eTakeImage );
            T_WaitingFingerprintOutputPin::SetPinValue( true );
            T_RemoveFingerOutputPin::SetPinValue( false );
            T_PlaceFingerOutputPin::SetPinValue( true );
            T_FingerprintsCountOutputPin::SetPinValue( 0 );
            FLastTime() = millis();
            C_OWNER.GetOwner().writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
        }

        void CompleteProcessing()
        {
            C_OWNER.GetOwner().FDataIndex = 0;
            SetState( eStandby );
            C_OWNER.GetOwner().ReleaseBuffer();
            C_OWNER.GetOwner().Processed();
        }

        void Failure()
        {
            CompleteProcessing();
            T_FailedOutputPin::ClockPin();
        }

        inline void SetState( TState AState )
        {
#ifdef __FINGERPRINT_STATE_DEBUG__
            Serial.print( "STATE: " );
            Serial.print( AState );
            Serial.print( " STAGE: " );
            Serial.println( FSecondStage().GetValue() );
#endif // __FINGERPRINT_STATE_DEBUG__
            FState() = AState;
        }

        void StartConvert()
        {
            if( FSecondStage().GetValue() )
                T_WaitingFingerprintOutputPin::SetPinValue( false );

            T_FingerprintsCountOutputPin::SetPinValue( FSecondStage().GetValue() ? 2 : 1 );
            SetState( eConvert );
            C_OWNER.GetOwner().StartConvert( FSecondStage().GetValue() ? 2 : 1 );
        }

        void ProcessDownload()
        {
//                Serial.println( "Try Download" );
            if( ! C_OWNER.Enabled().GetValue() )
                return;

            if( FState().GetValue() != eStandby )
            {
                Image().FClocked() = true;
                return;
            }

            if( C_OWNER.GetInProcessing() )
            {
                Image().FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                Image().FClocked() = true;
                return;
            }

            Image().FClocked() = false;
//                Serial.println( "Download" );
            DownloadImage();
        }

        void DownloadImage()
        {
            SetState( eDownloadImage );
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_DOWNLOAD_IMAGE );
        }

    public:
		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
//            Serial.print( "." );
            switch ( FState() )
            {
                case eTakeImage :
                {
//                    Serial.print( "." );
                    if( APacketEnd )
                    {
//                        Serial.println( "STEP_1 1" );
//                        Serial.println( C_OWNER.GetOwner().FConfirmationCode );
                        if( C_OWNER.GetOwner().FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            C_OWNER.GetOwner().writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );

                        else
                        {
                            T_RemoveFingerOutputPin::SetPinValue( false );
                            T_PlaceFingerOutputPin::SetPinValue( false );
//                            bool AIsConnected = false;
//                            T_Image_IsOutputConnected::Call( AIsConnected );
//                            if( AIsConnected ) //Image().OutputPin().GetPinIsConnected() )
                            if( Image().OutputPin().GetPinIsConnected() )
                            {
                                SetState( eUploadImage );
                                C_OWNER.GetOwner().writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_UPLOAD_IMAGE );
                            }

                            else
                            {
                                StartConvert();
                            }
                        }
                    }

                    break;
                }

                case eRemoveFinger :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.GetOwner().FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            T_RemoveFingerOutputPin::SetPinValue( false );
                            T_PlaceFingerOutputPin::SetPinValue( true );
                            SetState( eTakeImage );
                        }

                        C_OWNER.GetOwner().writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
                    }

                    break;
                }

                case eUploadImage :
                {
                    if( C_OWNER.GetOwner().FReceivingData() )
                    {
                        if( APacketEnd )
                        {
                            if( C_OWNER.GetOwner().FReceivingLastData() )
                            {
                                C_OWNER.GetOwner().ReleaseBuffer();
                                StartConvert();
                            }
                        }

                        else
                        {
                            if( C_OWNER.GetOwner().FData )
                            {
                                if( C_OWNER.GetOwner().FCount < TArduinoFingerprintReader::IMAGE_SIZE )
                                {
                                    C_OWNER.GetOwner().FData[ C_OWNER.GetOwner().FCount ++ ] = AValue;
                                    if( C_OWNER.GetOwner().FCount == TArduinoFingerprintReader::IMAGE_SIZE )
                                    {
                                        TImageBuffer AImage( TImageFormat::Gray4, TArduinoFingerprintReader::IMAGE_WIDTH, TArduinoFingerprintReader::IMAGE_HEIGHT, C_OWNER.GetOwner().FData );

//                                        T_Image_SendImage::Call( AImage );
                                        Image().OutputPin().SetPinValue( AImage );

                                        C_OWNER.GetOwner().ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.GetOwner().FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
//                            SetState( eUploadImageData );
                            C_OWNER.GetOwner().StartReadBuffer( TArduinoFingerprintReader::IMAGE_SIZE );
//                            FDataPtr = FData;
                        }

                    }

                    break;
                }

                case eConvert :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.GetOwner().FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            StartConvert();
                            break;
                        }

                        if( FSecondStage().GetValue() )
                        {
                            T_WaitingFingerprintOutputPin::SetPinValue( true );
                            FSecondStage() = false;
                            SetState( eCreateModel );
                            C_OWNER.GetOwner().writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_REGMODEL );
                            break;
                        }

                        FSecondStage() = true;
                        T_PlaceFingerOutputPin::SetPinValue( false );
                        T_RemoveFingerOutputPin::SetPinValue( false );

/*
                        if( T_FeaturesOutputPin::GetPinIsConnected() )
                        {
                            SetState( eUploadFeatures );
                            C_OWNER.UploadTemplate();
                        }

                        else
                            StartSearch();
*/
                        SetState( eRemoveFinger );
                        T_PlaceFingerOutputPin::SetPinValue( false );
                        T_RemoveFingerOutputPin::SetPinValue( true );
                        C_OWNER.GetOwner().writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
                    }

                    break;
                }

                case eCreateModel :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.GetOwner().FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            Failure();
                        }

                        else
                        {
                            if( C_OWNER.OutputPin().GetPinIsConnected() )
                            {
                                SetState( eUploadFeatures );
                                C_OWNER.GetOwner().UploadTemplate();
                            }

                            else
                            {
                                SetState( eStoreModel );
                                C_OWNER.GetOwner().StoreInTable( 1, C_OWNER.Index() );
                            }
                        }
                    }

                    break;
                }

                case eUploadFeatures :
                {
                    if( C_OWNER.GetOwner().FReceivingData() )
                    {
                        if( APacketEnd )
                        {
    //                        if( FIsLastPacket().GetValue() )
//                            if( FState().GetValue() == eUploadFeaturesLastData )
                            if( C_OWNER.GetOwner().FReceivingLastData() )
                            {
                                C_OWNER.GetOwner().ReleaseBuffer();

    //uint32_t ACount = C_OWNER.FCount;
    //                        Serial.println( FCount );

                                SetState( eStoreModel );
                                C_OWNER.GetOwner().StoreInTable( 1, C_OWNER.Index() );
//                                CompleteProcessing();

                            }
                        }

                        else
                        {
    //                        Serial.println( AValue, HEX );
                            if( C_OWNER.GetOwner().FData )
                            {
                                if( C_OWNER.GetOwner().FCount < TArduinoFingerprintReader::FEATURES_SIZE + 1 )
                                {
                                    C_OWNER.GetOwner().FData[ C_OWNER.GetOwner().FCount ] = AValue;
    //uint32_t ACount = C_OWNER.FCount;
                                    ++ C_OWNER.GetOwner().FCount;
                                    if( C_OWNER.GetOwner().FCount == TArduinoFingerprintReader::FEATURES_SIZE )
                                    {
                                        C_OWNER.OutputPin().SetPinValue( Mitov::TDataBlock( C_OWNER.GetOwner().FCount, C_OWNER.GetOwner().FData ));

                                        C_OWNER.GetOwner().ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.GetOwner().FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
//                            FState() = eUploadFeaturesData;
                            C_OWNER.GetOwner().StartReadBuffer( TArduinoFingerprintReader::FEATURES_SIZE );
//                            FDataPtr = FData;
                        }

                    }

                    break;
                }

                case eStoreModel :
                {
//                    Serial.println( "eStoreModel" );
                    if( APacketEnd )
                    {
                        if( C_OWNER.GetOwner().FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            Failure();
                        }

                        else
                        {
                            T_LearnedOutputPin::ClockPin();
                            T_WaitingFingerprintOutputPin::SetPinValue( true );
                            T_RemoveFingerOutputPin::SetPinValue( false );
                            T_PlaceFingerOutputPin::SetPinValue( true );
                            T_FingerprintsCountOutputPin::SetPinValue( 0 );
                            CompleteProcessing();
//                            SetState( eStoreModel );
//                            C_OWNER.GetOwner().StoreInTable( 1, C_OWNER.Index() );
                        }
                    }

                    break;
                }

                case eDownloadImage :
                {
                    if( APacketEnd )
                    {
                        uint8_t *ABuffer = ( uint8_t *)Image().FImageData().GetValue();
                        if( ABuffer )
                        {
                            if( ASuccess )
                            {
                                C_OWNER.writeData( ABuffer, TArduinoFingerprintReader::IMAGE_SIZE );

//                                Serial.println( "DOWNLOADED" );
                                delete [] ABuffer;
                                Image().FImageData() = nullptr;

//                                Serial.println( "START: eStoreTemplate" );
                                StartConvert();
                            }

                            else
                                CompleteProcessing();

                        }

                        else
                            CompleteProcessing();

                    }

                    break;
                }
            }
        }

    public:
		inline void StartInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

        inline void Image_InputPin_o_Receive( void *_Data )
        {
            TImageBuffer &AImage = *(TImageBuffer *)_Data;
            uint32_t AImageSize = AImage.GetSize();

            if( Image().FImageData().GetValue() )
                delete [] Image().FImageData().GetValue();

            Image().FImageData() = new uint8_t[ TArduinoFingerprintReader::IMAGE_SIZE ];
            uint32_t ACopySize = MitovMin<uint32_t>( TArduinoFingerprintReader::IMAGE_SIZE, AImageSize );
            memcpy( Image().FImageData().GetValue(), AImage.Data, ACopySize );
            memset( (( uint8_t *) Image().FImageData().GetValue() ) + ACopySize, 0, TArduinoFingerprintReader::IMAGE_SIZE - ACopySize );

            if( Image().FClocked() )
                return;

            ProcessDownload();
        }

	public:
        inline void SystemStart()
        {
            T_PlaceFingerOutputPin::SetPinValue( false );
            T_RemoveFingerOutputPin::SetPinValue( false );
            T_WaitingFingerprintOutputPin::SetPinValue( false );
        }

        inline void SystemLoopBegin()
        {
            if( Timeout().GetValue() )
                if( millis() - FLastTime().GetValue() > Timeout() )
                    Failure();

            if( FClocked() )
                ProcessClock();

        }

    public:
        inline TArduinoFingerprintReaderTemplateEnroll()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Count,
        typename T_Enabled,
        typename T_FClocked,
        typename T_Index
    > class TArduinoFingerprintDeleteTemplate :
        public T_Count,
        public T_Enabled,
        public T_FClocked,
        public T_Index
    {
	public:
        _V_PROP_( Count )
        _V_PROP_( Enabled )
        _V_PROP_( Index )

	protected:
        _V_PROP_( FClocked )

	protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
//            Serial.println( "DELETE" );
            C_OWNER.DeleteFromTable( Index().GetValue(), Count().GetValue() );
        }

    public:
        inline void Recognized( uint16_t AValue ) {}

        inline void TimeoutProcess() {}
        inline void InvalidPacket() {}
        inline void NeedsRecognition( bool & AValue ) {}

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        TArduinoFingerprintDeleteTemplate()
        {
            FClocked() = true;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_DeleteInputPin_o_IsConnected,
        typename T_Enabled,
        typename T_Enroll_ProcessByte,
        typename T_FDeleteClocked,
//        typename T_FLearnClocked,
        typename T_FReadClocked,
        typename T_FState,
        typename T_FTemplateData,
        typename T_Index,
//        typename T_LearnInputPin_o_IsConnected,
        typename T_OutputPin,
        typename T_ReadInputPin_o_IsConnected,
        typename T_RecognizedOutputPin
    > class TArduinoFingerprintReaderTemplate :
        public T_DeleteInputPin_o_IsConnected,
        public T_Enabled,
        public T_FDeleteClocked,
//        public T_FLearnClocked,
        public T_FReadClocked,
        public T_FState,
        public T_FTemplateData,
        public T_Index,
//        public T_LearnInputPin_o_IsConnected,
        public T_OutputPin,
        public T_ReadInputPin_o_IsConnected,
        public T_RecognizedOutputPin
    {
	public:
        _V_PIN_( OutputPin )

	public:
        _V_PROP_( Enabled )
        _V_PROP_( Index )

    protected:
        _V_PROP_( FDeleteClocked )
//        _V_PROP_( FLearnClocked )
        _V_PROP_( FReadClocked )
        _V_PROP_( FState )
        _V_PROP_( FTemplateData )

        _V_PROP_( DeleteInputPin_o_IsConnected )
//        _V_PROP_( LearnInputPin_o_IsConnected )
        _V_PROP_( ReadInputPin_o_IsConnected )

    protected:
        enum
        {
            eStandby,
//            eReset,
            eExtractTemplate,
            eUploadFeatures,
            eDownloadFeatures,
            eStoreTemplate
//            eUploadFeaturesData,
//            eUploadFeaturesLastData

        };

    protected:
        void CompleteProcessing()
        {
            C_OWNER.FDataIndex = 0;
            FState() = eStandby;
            C_OWNER.ReleaseBuffer();
            C_OWNER.Processed();
        }

        inline void Failure()
        {
            CompleteProcessing();
        }

        void DownloadFeatures()
        {
            FState() = eDownloadFeatures;
            C_OWNER.writeCommandPacketByte( true, TArduinoFingerprintReader::FINGERPRINT_DOWNLOAD_TEMPLATE, 1 );
        }

	public:
        inline T_OWNER &GetOwner()
        {
            return C_OWNER;
        }

	public:
        inline void NeedsRecognition( bool & AValue )
        {
            if( T_RecognizedOutputPin::GetPinIsConnected() )
                AValue = true;

        }

        inline void Recognized( uint16_t AValue )
        {
            if( Index().GetValue() == AValue )
                T_RecognizedOutputPin::ClockPin();

        }

        inline void InvalidPacket()
        {
            CompleteProcessing();
        }

        inline void TimeoutProcess()
        {
            CompleteProcessing();
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
            switch ( FState() )
            {
/*
                case eReset :
                {
                    if( APacketEnd )
                    {
                        if( ASuccess )
                        {
                            FState() = eExtractTemplate;
                            C_OWNER.LoadFingerprintFromTable( Index().GetValue() );
                        }

                        else
                            CompleteProcessing();

                    }

                    break;
                }
*/
                case eExtractTemplate :
                {
                    if( APacketEnd )
                    {
//                        Serial.println( "STEP 2" );

                        if( ASuccess )
                        {
                            FState() = eUploadFeatures;
                            C_OWNER.UploadTemplate();
//                            ExecuteNextStep();
                        }

                        else
                            CompleteProcessing();

                    }

                    break;
                }

                case eUploadFeatures :
                {
                    if( C_OWNER.FReceivingData() )
                    {
                        if( APacketEnd )
                        {
    //                        if( FIsLastPacket().GetValue() )
//                            if( FState().GetValue() == eUploadFeaturesLastData )
                            if( C_OWNER.FReceivingLastData() )
                            {
                                C_OWNER.ReleaseBuffer();

    //uint32_t ACount = C_OWNER.FCount;
    //                        Serial.println( FCount );

                                CompleteProcessing();

                            }
                        }

                        else
                        {
    //                        Serial.println( AValue, HEX );
                            if( C_OWNER.FData )
                            {
                                if( C_OWNER.FCount < TArduinoFingerprintReader::FEATURES_SIZE + 1 )
                                {
                                    C_OWNER.FData[ C_OWNER.FCount ] = AValue;
    //uint32_t ACount = C_OWNER.FCount;
                                    ++ C_OWNER.FCount;
                                    if( C_OWNER.FCount == TArduinoFingerprintReader::FEATURES_SIZE )
                                    {
                                        T_OutputPin::SetPinValue( Mitov::TDataBlock( C_OWNER.FCount, C_OWNER.FData ));

                                        C_OWNER.ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
//                            FState() = eUploadFeaturesData;
                            C_OWNER.StartReadBuffer( TArduinoFingerprintReader::FEATURES_SIZE );
//                            FDataPtr = FData;
                        }

                    }

                    break;
                }

                case eDownloadFeatures :
                {
                    if( APacketEnd )
                    {
                        if( FTemplateData().GetValue() )
                        {
                            uint8_t *ABuffer = ( uint8_t *)FTemplateData().GetValue();
                            if( ASuccess )
                            {
                                C_OWNER.writeData( ABuffer, TArduinoFingerprintReader::FEATURES_SIZE );

                                delete [] ABuffer;
                                FTemplateData() = nullptr;

//                                Serial.println( "START: eStoreTemplate" );
                                FState() = eStoreTemplate;
                                C_OWNER.StoreInTable( 1, Index() );
                            }

                            else
                                CompleteProcessing();

                        }

                        else
                            CompleteProcessing();

                    }

                    break;
                }

                case eStoreTemplate :
                {
//                    Serial.println( "eStoreTemplate" );
                    if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            Failure();
                        }

                        else
                        {
//                            Serial.println( "eStoreTemplate" );

                            CompleteProcessing();

                            if( T_OutputPin::GetPinIsConnected() )
                            {
                                FState() = eExtractTemplate;
                                C_OWNER.LoadFingerprintFromTable( Index().GetValue() );
                            }

                        }
                    }

                    break;
                }

            default:
                T_Enroll_ProcessByte::Call( AValue, APacketEnd, ASuccess );

            }

        }

    protected:
        void ProcessRead()
        {
//            Serial.println( "START READ!!!" );
            if( ! Enabled().GetValue() )
                return;

            if( FState().GetValue() != eStandby )
            {
                FReadClocked() = true;
                return;
            }

            if( C_OWNER.GetInProcessing() )
            {
                FReadClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FReadClocked() = true;
                return;
            }

//            Serial.println( "START" );
//            delay( 1000 );
            C_OWNER.FCount = 0;
//            FState() = eReset;
            FReadClocked() = false;
//            C_OWNER.SendPassword();
//            C_OWNER.UpdateEnabled(); // Force reset!
//            FReadClocked() = false;
            FState() = eExtractTemplate;
            C_OWNER.LoadFingerprintFromTable( Index().GetValue() );
//            C_OWNER.WriteByte( 0x00 ); // Dummy wakeup byte!
        }

        void ProcessDelete()
        {
            if( ! Enabled().GetValue() )
                return;

            if( FState().GetValue() != eStandby )
            {
                FDeleteClocked() = true;
                return;
            }

            if( C_OWNER.GetInProcessing() )
            {
                FDeleteClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FDeleteClocked() = true;
                return;
            }

            FDeleteClocked() = false;
            C_OWNER.DeleteFromTable( Index().GetValue(), 1 );
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FReadClocked() )
                ProcessRead();

            if( FDeleteClocked() )
                ProcessDelete();

            if( FTemplateData().GetValue() )
            {
                if( ! Enabled().GetValue() )
                    return;

                if( FState().GetValue() != eStandby )
                    return;

                if( C_OWNER.GetInProcessing() )
                    return;

                if( ! C_OWNER.GetIsReady() )
                    return;

                DownloadFeatures();
            }
        }

	public:
        inline void ReadInputPin_o_Receive( void *_Data )
        {
            if( FReadClocked() )
                return;

            ProcessRead();
        }

        inline void DeleteInputPin_o_Receive( void *_Data )
        {
            if( FDeleteClocked() )
                return;

            ProcessDelete();
        }

        inline void InputPin_o_Receive( void *_Data )
        {
			TArray<uint8_t> &AValue = *(TArray<uint8_t>*)_Data;
            uint32_t ASize = MitovMax<uint32_t>( TArduinoFingerprintReader::FEATURES_SIZE, AValue.Size );

            if( FTemplateData().GetValue() )
                delete [] FTemplateData().GetValue();

//            FTemplateData() = new uint8_t[ ASize ];
//            memcpy( FTemplateData().GetValue(), AValue.Data, MitovMin<uint32_t>( ASize, AValue.Size ));

            FTemplateData() = new uint8_t[ TArduinoFingerprintReader::IMAGE_SIZE ];
            uint32_t ACopySize = MitovMin<uint32_t>( TArduinoFingerprintReader::FEATURES_SIZE, ASize );
            memcpy( FTemplateData().GetValue(), AValue.Data, ACopySize );
            memset( (( uint8_t *) FTemplateData().GetValue() ) + ACopySize, 0, TArduinoFingerprintReader::FEATURES_SIZE - ACopySize );

//            Serial.println( "DATA" );
        }

	public:
        inline TArduinoFingerprintReaderTemplate()
        {
            FDeleteClocked() = false;
//            FLearnClocked() = false;
            FReadClocked() = false;
            FState() = eStandby;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked,
        typename T_FLastTime,
        typename T_FSecondStage,
        typename T_FState,
        typename T_FTableIndex,
        typename T_FailedOutputPin,
        typename T_FeaturesOutputPin,
        typename T_FingerprintsCountOutputPin,
//        typename T_Image_IsOutputConnected,
//        typename T_Image_SendImage,
        typename T_Image,
        typename T_IndexOutputPin,
        typename T_PlaceFingerOutputPin,
        typename T_RemoveFingerOutputPin,
        typename T_SuccededOutputPin,
        typename T_Timeout,
        typename T_WaitingFingerprintOutputPin
    > class TArduinoFingerprintReaderEnroll :
        public T_Enabled,
        public T_FClocked,
        public T_FLastTime,
        public T_FSecondStage,
        public T_FState,
        public T_FTableIndex,
        public T_FailedOutputPin,
        public T_FeaturesOutputPin,
        public T_FingerprintsCountOutputPin,
        public T_Image,
        public T_IndexOutputPin,
        public T_PlaceFingerOutputPin,
        public T_RemoveFingerOutputPin,
        public T_SuccededOutputPin,
        public T_Timeout,
        public T_WaitingFingerprintOutputPin
    {
	public:
		_V_PIN_( FailedOutputPin )
        _V_PIN_( FeaturesOutputPin )
        _V_PIN_( FingerprintsCountOutputPin )
		_V_PIN_( IndexOutputPin )
        _V_PIN_( PlaceFingerOutputPin )
        _V_PIN_( RemoveFingerOutputPin )
		_V_PIN_( SuccededOutputPin )
        _V_PIN_( WaitingFingerprintOutputPin )

	public:
		_V_PROP_( Enabled )
        _V_PROP_( Image )
        _V_PROP_( Timeout )

    protected:
        enum TState
        {
            eStandby,
            eGetTable,
            eTakeImage,
            eUploadImage,
            eConvert,
            eRemoveFinger,
            eCreateModel,
            eUploadFeatures,
            eStoreModel,
            eDownloadImage
        };

    protected:
        _V_PROP_( FClocked )
        _V_PROP_( FSecondStage )
        _V_PROP_( FLastTime )
        _V_PROP_( FState )
        _V_PROP_( FTableIndex )

        uint16_t FTemplateIndex;

    protected:
        inline void SetState( TState AState )
        {
#ifdef __FINGERPRINT_STATE_DEBUG__
//            if( FState().GetValue() > 1 )
            {
                Serial.print( "ENROLL STATE: " );
                Serial.println( AState );
            }
//            Serial.print( " STAGE: " );
//            Serial.println( FSecondStage().GetValue() );
#endif // __FINGERPRINT_STATE_DEBUG__
            FState() = AState;
        }

        void CompleteProcessing()
        {
            C_OWNER.FDataIndex = 0;
            SetState( eStandby );
            C_OWNER.ReleaseBuffer();
            C_OWNER.Processed();
        }

        void Failure()
        {
            CompleteProcessing();
            T_FailedOutputPin::ClockPin();
        }

        void RequestTable()
        {
			uint8_t AData = FTableIndex();
            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_LIST_TEMPLATES, &AData, 1 );
            FState() = eGetTable;
        }

        void DownloadImage()
        {
            SetState( eDownloadImage );
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_DOWNLOAD_IMAGE );
        }

        void ProcessClock()
        {
            if( ! C_OWNER.Enabled().GetValue() )
                return;

//            Serial.print( " <.> " );
            if( FState().GetValue() != eStandby )
            {
                FClocked() = true;
                return;
            }

//            Serial.print( " <+> " );
            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

//            Serial.print( " <*> " );
            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
            FTableIndex() = 0;
            RequestTable();

/*
            FSecondStage() = false;
            SetState( eTakeImage );
            T_WaitingFingerprintOutputPin::SetPinValue( true );
            T_RemoveFingerOutputPin::SetPinValue( false );
            T_PlaceFingerOutputPin::SetPinValue( true );
            T_FingerprintsCountOutputPin::SetPinValue( 0 );
            FLastTime() = millis();
            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
*/
        }

        void ProcessDownload()
        {
//                Serial.println( "Try Download" );
            if( ! C_OWNER.Enabled().GetValue() )
                return;

            if( FState().GetValue() != eStandby )
            {
                Image().FClocked() = true;
                return;
            }

            if( C_OWNER.GetInProcessing() )
            {
                Image().FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                Image().FClocked() = true;
                return;
            }

            Image().FClocked() = false;
//                Serial.println( "Download" );
            DownloadImage();
        }

        void StartConvert()
        {
            if( FSecondStage().GetValue() )
                T_WaitingFingerprintOutputPin::SetPinValue( false );

            T_FingerprintsCountOutputPin::SetPinValue( FSecondStage().GetValue() ? 2 : 1 );
            SetState( eConvert );
            C_OWNER.StartConvert( FSecondStage().GetValue() ? 2 : 1 );
        }

        void ExecuteNextTableStep()
        {
//            switch (
//              case
//            uint16_t ALastIndex = StartIndex().GetValue() + Count().GetValue();
            uint16_t AStartIndex = FTableIndex() * 256;
//            ALastIndex -= FTableIndex() * 256;
            uint16_t ACapacity = C_OWNER.FCapacity().GetValue();
            ACapacity -= AStartIndex;

//            if( ALastIndex > 256 )
//                ALastIndex = 256;

//            ALastIndex =
            uint16_t ALastIndex;
            if( 256 <= ACapacity )
                ALastIndex = 256;

            else
                ALastIndex = ACapacity;

            for( int i = 0; i < ALastIndex; ++ i )
            {
                uint8_t AIndex = i >> 3;
                uint8_t AOffset = i & 0b111;
                uint8_t AMask = ( 1 << AOffset );
                bool APopulated = (( C_OWNER.FData[ AIndex ] & AMask ) != 0 );
//                T_IndexOutputPin::SetPinValue( AStartIndex + i );
//                T_PopulatedOutputPin::SetPinValue( APopulated );
                if( ! APopulated )
                {
                    FTemplateIndex = AStartIndex | i;
                    C_OWNER.ReleaseBuffer();
                    FSecondStage() = false;
                    SetState( eTakeImage );
                    T_WaitingFingerprintOutputPin::SetPinValue( true );
                    T_RemoveFingerOutputPin::SetPinValue( false );
                    T_PlaceFingerOutputPin::SetPinValue( true );
                    T_FingerprintsCountOutputPin::SetPinValue( 0 );
                    FLastTime() = millis();
                    C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
                    return;

//                    C_OWNER.FData[ AIndex ] &= ~AMask;

/*
                    uint16_t AID = ( FTableIndex().GetValue() << 8 ) | i;
                    C_OWNER.LoadFingerprintFromTable( AID );

                    if( T_FeaturesOutputPin::GetPinIsConnected() )
                    {
                        FState() = eExtractTemplate;
                        return;
                    }
*/
                }

//                T_TemplateProcessedOutputPin::ClockPin();
            }

            C_OWNER.ReleaseBuffer();

//            uint16_t ACapacity = C_OWNER.FCapacity().GetValue();
            uint8_t AMaxTablesIndex = ( C_OWNER.FCapacity().GetValue() >> 8 );
            if( FTableIndex() < AMaxTablesIndex )
            {
                FTableIndex() = FTableIndex() + 1;
                RequestTable();
            }

            else
               Failure();

        }

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}

        inline void InvalidPacket() {}

        inline void TimeoutProcess()
        {
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
            switch ( FState() )
            {
                case eGetTable :
                {
                    uint32_t AIndex = C_OWNER.FDataIndex;
                    if( C_OWNER.FDataIndex == TArduinoFingerprintReader::DATA_INDEX )
                        C_OWNER.AllocateBuffer( TArduinoFingerprintReader::FEATURES_LIST_SIZE );

                    if( APacketEnd )
                    {
                        if( ASuccess )
                        {
                            if( C_OWNER.FData )
                            {
//                                FTableData = C_OWNER.FData;
//                                C_OWNER.FData = nullptr;
                                ExecuteNextTableStep();
                            }
                        }

                        else
                            Failure();

                        return;
                    }

                    if( C_OWNER.FDataIndex >= TArduinoFingerprintReader::DATA_INDEX )
                    {
                        C_OWNER.FData[ C_OWNER.FDataIndex - TArduinoFingerprintReader::DATA_INDEX ] = AValue;
                    }

                    break;
                }

                case eTakeImage :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );

                        else
                        {
                            T_RemoveFingerOutputPin::SetPinValue( false );
                            T_PlaceFingerOutputPin::SetPinValue( false );
//                            bool AIsConnected = false;
//                            T_Image_IsOutputConnected::Call( AIsConnected );
//                            if( AIsConnected ) //Image().OutputPin().GetPinIsConnected() )
                            if( Image().OutputPin().GetPinIsConnected() )
                            {
                                SetState( eUploadImage );
                                C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_UPLOAD_IMAGE );
                            }

                            else
                            {
                                StartConvert();
                            }
                        }
                    }

                    break;
                }

                case eRemoveFinger :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            T_RemoveFingerOutputPin::SetPinValue( false );
                            T_PlaceFingerOutputPin::SetPinValue( true );
                            SetState( eTakeImage );
                        }

                        C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
                    }

                    break;
                }

                case eUploadImage :
                {
                    if( C_OWNER.FReceivingData() )
                    {
                        if( APacketEnd )
                        {
                            if( C_OWNER.FReceivingLastData() )
                            {
                                C_OWNER.ReleaseBuffer();
                                StartConvert();
                            }
                        }

                        else
                        {
                            if( C_OWNER.FData )
                            {
                                if( C_OWNER.FCount < TArduinoFingerprintReader::IMAGE_SIZE )
                                {
                                    C_OWNER.FData[ C_OWNER.FCount ++ ] = AValue;
                                    if( C_OWNER.FCount == TArduinoFingerprintReader::IMAGE_SIZE )
                                    {
                                        TImageBuffer AImage( TImageFormat::Gray4, TArduinoFingerprintReader::IMAGE_WIDTH, TArduinoFingerprintReader::IMAGE_HEIGHT, C_OWNER.FData );

//                                        T_Image_SendImage::Call( AImage );
                                        Image().OutputPin().SetPinValue( AImage );

                                        C_OWNER.ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
//                            SetState( eUploadImageData );
                            C_OWNER.StartReadBuffer( TArduinoFingerprintReader::IMAGE_SIZE );
//                            FDataPtr = FData;
                        }

                    }

                    break;
                }

                case eConvert :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            StartConvert();
                            break;
                        }

                        if( FSecondStage().GetValue() )
                        {
                            T_WaitingFingerprintOutputPin::SetPinValue( true );
                            FSecondStage() = false;
                            SetState( eCreateModel );
                            C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_REGMODEL );
                            break;
                        }

                        FSecondStage() = true;
                        T_PlaceFingerOutputPin::SetPinValue( false );
                        T_RemoveFingerOutputPin::SetPinValue( false );

/*
                        if( T_FeaturesOutputPin::GetPinIsConnected() )
                        {
                            SetState( eUploadFeatures );
                            C_OWNER.UploadTemplate();
                        }

                        else
                            StartSearch();
*/
                        SetState( eRemoveFinger );
                        T_PlaceFingerOutputPin::SetPinValue( false );
                        T_RemoveFingerOutputPin::SetPinValue( true );
                        C_OWNER.writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_GETIMAGE );
                    }

                    break;
                }

                case eCreateModel :
                {
                    if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            Failure();
                        }

                        else
                        {
                            if( T_FeaturesOutputPin::GetPinIsConnected() )
                            {
                                SetState( eUploadFeatures );
                                C_OWNER.UploadTemplate();
                            }

                            else
                            {
                                SetState( eStoreModel );

//                                Serial.print( "StoreInTable: " );
//                                Serial.println( FTemplateIndex );

                                C_OWNER.StoreInTable( 1, FTemplateIndex );
                            }
                        }
                    }

                    break;
                }

                case eUploadFeatures :
                {
                    if( C_OWNER.FReceivingData() )
                    {
                        if( APacketEnd )
                        {
    //                        if( FIsLastPacket().GetValue() )
//                            if( FState().GetValue() == eUploadFeaturesLastData )
                            if( C_OWNER.FReceivingLastData() )
                            {
                                C_OWNER.ReleaseBuffer();

    //uint32_t ACount = C_OWNER.FCount;
    //                        Serial.println( FCount );

                                SetState( eStoreModel );

//                                Serial.print( "StoreInTable: " );
//                                Serial.println( FTemplateIndex );

                                C_OWNER.StoreInTable( 1, FTemplateIndex );
//                                CompleteProcessing();

                            }
                        }

                        else
                        {
    //                        Serial.println( AValue, HEX );
                            if( C_OWNER.FData )
                            {
                                if( C_OWNER.FCount < TArduinoFingerprintReader::FEATURES_SIZE + 1 )
                                {
                                    C_OWNER.FData[ C_OWNER.FCount ] = AValue;
    //uint32_t ACount = C_OWNER.FCount;
                                    ++ C_OWNER.FCount;
                                    if( C_OWNER.FCount == TArduinoFingerprintReader::FEATURES_SIZE )
                                    {
                                        T_FeaturesOutputPin::SetPinValue( Mitov::TDataBlock( C_OWNER.FCount, C_OWNER.FData ));

                                        C_OWNER.ReleaseBuffer();
                                    }
                                }
                            }

    //                        Serial.println( FCount );
                        }
                    }

                    else if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                            CompleteProcessing();

                        else
                        {
//                            FState() = eUploadFeaturesData;
                            C_OWNER.StartReadBuffer( TArduinoFingerprintReader::FEATURES_SIZE );
//                            FDataPtr = FData;
                        }
                    }

                    break;
                }

                case eStoreModel :
                {
//                    Serial.println( "eStoreModel" );
                    if( APacketEnd )
                    {
                        if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                        {
                            Failure();
                        }

                        else
                        {
                            T_IndexOutputPin::SetPinValue( FTemplateIndex );
                            T_SuccededOutputPin::ClockPin();
                            T_WaitingFingerprintOutputPin::SetPinValue( true );
                            T_RemoveFingerOutputPin::SetPinValue( false );
                            T_PlaceFingerOutputPin::SetPinValue( true );
                            T_FingerprintsCountOutputPin::SetPinValue( 0 );
                            CompleteProcessing();
//                            SetState( eStoreModel );
//                            C_OWNER.StoreInTable( 1, C_OWNER.Index() );
                        }
                    }

                    break;
                }

                case eDownloadImage :
                {
                    if( APacketEnd )
                    {
                        uint8_t *ABuffer = ( uint8_t *)Image().FImageData().GetValue();
                        if( ABuffer )
                        {
                            if( ASuccess )
                            {
                                C_OWNER.writeData( ABuffer, TArduinoFingerprintReader::IMAGE_SIZE );

//                                Serial.println( "DOWNLOADED" );
                                delete [] ABuffer;
                                Image().FImageData() = nullptr;

//                                Serial.println( "START: eStoreTemplate" );
                                StartConvert();
                            }

                            else
                                CompleteProcessing();

                        }

                        else
                            CompleteProcessing();

                    }

                    break;
                }
            }
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

        inline void Image_InputPin_o_Receive( void *_Data )
        {
            TImageBuffer &AImage = *(TImageBuffer *)_Data;
            uint32_t AImageSize = AImage.GetSize();

            if( Image().FImageData().GetValue() )
                delete [] Image().FImageData().GetValue();

            Image().FImageData() = new uint8_t[ TArduinoFingerprintReader::IMAGE_SIZE ];
            uint32_t ACopySize = MitovMin<uint32_t>( TArduinoFingerprintReader::IMAGE_SIZE, AImageSize );
            memcpy( Image().FImageData().GetValue(), AImage.Data, ACopySize );
            memset( (( uint8_t *) Image().FImageData().GetValue() ) + ACopySize, 0, TArduinoFingerprintReader::IMAGE_SIZE - ACopySize );

            if( Image().FClocked() )
                return;

            ProcessDownload();
        }

    public:
        inline void SystemLoopBegin()
        {
            if( Timeout().GetValue() )
                if( millis() - FLastTime().GetValue() > Timeout() )
                    Failure();

            if( FClocked() )
                ProcessClock();

            if( Image().FClocked() )
                ProcessDownload();

        }

    public:
        inline TArduinoFingerprintReaderEnroll()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked,
        typename T_PasswordCode
    > class TArduinoFingerprintReaderSetPasswordElement :
        public T_Enabled,
        public T_FClocked,
        public T_PasswordCode
    {
	public:
		_V_PROP_( Enabled )
		_V_PROP_( PasswordCode )

    protected:
        _V_PROP_( FClocked )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}
		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess ) {}

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;

            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_SETPASSWORD, PasswordCode().GetValue() );
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderSetPasswordElement()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Address,
        typename T_Enabled,
        typename T_FClocked
    > class TArduinoFingerprintReaderSetAddressElement :
        public T_Address,
        public T_Enabled,
        public T_FClocked
    {
	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

    protected:
        _V_PROP_( FClocked )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}
		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess ) {}

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;

            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_SETADDRESS, Address().GetValue() );
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderSetAddressElement()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked,
        typename T_Speed
    > class TArduinoFingerprintReaderSetSerialSpeedElement :
        public T_Enabled,
        public T_FClocked,
        public T_Speed
    {
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Speed )

    protected:
        _V_PROP_( FClocked )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}
		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess ) {}

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;

            uint8_t AData[ 2 ];
            AData[ 0 ] = 4;

            uint8_t AValue = Speed() / 9600;
            if( AValue < 0 )
                AValue = 1;

            else if( AValue > 12 )
                AValue = 12;

            AData[ 1 ] = AValue;
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_SETSYSPARAM, AData, 2 );
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderSetSerialSpeedElement()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked,
        typename T_Level
    > class TArduinoFingerprintReaderSetSecurityLevelElement :
        public T_Enabled,
        public T_FClocked,
        public T_Level
    {
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Level )

    protected:
        _V_PROP_( FClocked )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}
		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess ) {}

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;

            uint8_t AData[ 2 ];
            AData[ 0 ] = 5;
            AData[ 1 ] = Level().GetValue();
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_SETSYSPARAM, AData, 2 );
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderSetSecurityLevelElement()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked,
        typename T_Length
    > class TArduinoFingerprintReaderSetPacketLengthElement :
        public T_Enabled,
        public T_FClocked,
        public T_Length
    {
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Length )

    protected:
        _V_PROP_( FClocked )

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}
		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess ) {}

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;

            uint8_t AData[ 2 ];
            AData[ 0 ] = 6;
            AData[ 1 ] = Length().GetValue() >> 6 ;
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_SETSYSPARAM, AData, 2 );
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderSetPacketLengthElement()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
  	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked,
        typename T_FProcessing,
        typename T_FailedOutputPin,
        typename T_SuccededOutputPin
    > class TArduinoFingerprintReaderHandshakeElement :
        public T_Enabled,
        public T_FClocked,
        public T_FProcessing,
        public T_FailedOutputPin,
        public T_SuccededOutputPin
    {
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SuccededOutputPin )

	public:
		_V_PROP_( Enabled )

    protected:
        _V_PROP_( FClocked )
        _V_PROP_( FProcessing )

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
            FProcessing() = true;
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_HANDSHAKE );
        }

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess()
        {
            FProcessing() = false;
            T_FailedOutputPin::ClockPin();
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
        {
            if( ! FProcessing().GetValue() )
                return;

            if( APacketEnd )
                if( C_OWNER.FConfirmationCode = 0x55 )
                    T_SuccededOutputPin::ClockPin();

                else
                    T_FailedOutputPin::ClockPin();


        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderHandshakeElement()
        {
            FClocked() = false;
            FProcessing() = false;
        }

    };
//---------------------------------------------------------------------------
    namespace TArduinoFingerprintReaderSetArealLEDColor
    {
        enum TArduinoFingerprintReaderSetArealLEDColor
        {
            Red = 1,
            Blue = 2,
            Purple = 3
        };
    }
//---------------------------------------------------------------------------
  namespace TArduinoFingerprintReaderSetArealLEDMode
  {
        enum TArduinoFingerprintReaderSetArealLEDMode
        {
            Breathing = 1,
            Flashing = 2,
            On = 3,
            Off = 4,
            GraduallyOn = 5,
            GraduallyOff = 6
        };
  }
//---------------------------------------------------------------------------
  	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Color,
        typename T_Cycles,
        typename T_Enabled,
        typename T_FClocked,
        typename T_Mode,
        typename T_Speed
    > class TArduinoFingerprintReaderSetArealLEDElement :
        public T_Color,
        public T_Cycles,
        public T_Enabled,
        public T_FClocked,
        public T_Mode,
        public T_Speed
    {
	public:
		_V_PROP_( Color )
		_V_PROP_( Cycles )
		_V_PROP_( Mode )
		_V_PROP_( Speed )
		_V_PROP_( Enabled )

    protected:
        _V_PROP_( FClocked )

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
			uint8_t AData[ 4 ];
            AData[ 0 ] = Mode().GetValue();
            AData[ 1 ] = ( Speed().GetValue() * 255 ) + 0.5;
            AData[ 2 ] = Color().GetValue();
            AData[ 3 ] = Cycles().GetValue();
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_AURALEDCONFIG, AData, 4 );
        }

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}
        inline void TimeoutProcess() {}
		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess ) {}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderSetArealLEDElement()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
  	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FClocked,
        typename T_FProcessing,
        typename T_OutputPin
    > class TArduinoFingerprintReaderGetRandomNumberElement :
        public T_Enabled,
        public T_FClocked,
        public T_FProcessing,
        public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

    protected:
        _V_PROP_( FClocked )
        _V_PROP_( FProcessing )

    protected:
        uint32_t    FValue;

    protected:
        void ProcessClock()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FClocked() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FClocked() = true;
                return;
            }

            FClocked() = false;
            FProcessing() = true;
            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_GETRANDOMCODE );
        }

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}
        inline void InvalidPacket() {}

        inline void TimeoutProcess()
        {
            FProcessing() = false;
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
            if( ! FProcessing().GetValue() )
                return;

            if( APacketEnd )
            {
                if( ASuccess )
                    T_OutputPin::SetPinValue( FValue );

                FProcessing() = false;
                return;
            }

            switch( C_OWNER.FDataIndex - TArduinoFingerprintReader::DATA_INDEX )
            {
                case 0 :
                    FValue = uint32_t( AValue ) << 24;
                    break;

                case 1 :
                    FValue |= uint32_t( AValue ) << 16;
                    break;

                case 2 :
                    FValue |= uint32_t( AValue ) << 8;
                    break;

                case 3 :
                    FValue |= AValue;
                    break;

            }

        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            if( FClocked() )
                return;

            ProcessClock();
        }

	public:
        inline void SystemLoopBegin()
        {
            if( FClocked() )
                ProcessClock();
        }

	public:
        inline TArduinoFingerprintReaderGetRandomNumberElement()
        {
            FClocked() = false;
            FProcessing() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Enabled,
        typename T_FInputData,
        typename T_FReading,
        typename T_FReadingRequest,
        typename T_OutputPin,
        typename T_Page
    > class TArduinoFingerprintReaderNotepadElement :
        public T_Enabled,
        public T_FInputData,
        public T_FReading,
        public T_FReadingRequest,
        public T_OutputPin,
        public T_Page
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
        _V_PROP_( Page )

	protected:
        _V_PROP_( FInputData )
        _V_PROP_( FReading )
        _V_PROP_( FReadingRequest )

	protected:
        inline void CompleteProcessing()
        {
            C_OWNER.ReleaseBuffer();
            C_OWNER.Processed();
            FReading() = false;
        }

        void ProcessRead()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
            {
                FReadingRequest() = true;
                return;
            }

            if( ! C_OWNER.GetIsReady() )
            {
                FReadingRequest() = true;
                return;
            }

/*
            if( FInputData() )
            {
                FReadingRequest() = true;
                return;
            }
*/
            FReadingRequest() = false;

            FReading() = true;

            C_OWNER.StartReadBuffer( TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE );

            C_OWNER.writeCommandPacketByte( false, TArduinoFingerprintReader::FINGERPRINT_NOTEPAD_READ, Page().GetValue() );
        }

        void ProcessWrite()
        {
            if( ! Enabled().GetValue() )
                return;

            if( C_OWNER.GetInProcessing() )
                return;

            if( ! C_OWNER.GetIsReady() )
                return;

            C_OWNER.writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_NOTEPAD_WRITE, (uint8_t *)FInputData().GetValue(), TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE + 1 );
            delete [] FInputData().GetValue();
            FInputData() = nullptr;
        }

	public:
        inline void NeedsRecognition( bool & AValue ) {}
        inline void Recognized( uint16_t AValue ) {}

        inline void InvalidPacket() {}

        inline void TimeoutProcess()
        {
            FReading() = false;
        }

		inline void ProcessByte( uint8_t AValue, bool APacketEnd, bool ASuccess )
		{
            if( ! FReading().GetValue() )
                return;

            if( APacketEnd )
            {
//                if( C_OWNER.FConfirmationCode != TArduinoFingerprintReader::FINGERPRINT_OK )
                 CompleteProcessing();
                 FReading() = false;

            }

            else
            {
//                        Serial.println( AValue, HEX );
                if( C_OWNER.FData && ( C_OWNER.FDataIndex > 8 ))
                {
                    if( C_OWNER.FCount < TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE + 1 )
                    {
                        C_OWNER.FData[ C_OWNER.FCount ++ ] = AValue;
                        if( C_OWNER.FCount == TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE )
                        {
                            T_OutputPin::SetPinValue( Mitov::TDataBlock( C_OWNER.FCount, C_OWNER.FData ));

                            C_OWNER.ReleaseBuffer();
                        }
                    }
                }

//                        Serial.println( FCount );
            }
        }

	public:
		inline void ReadInputPin_o_Receive( void *_Data )
		{
            if( FReadingRequest() )
                return;

            ProcessRead();
        }

        inline void InputPin_o_Receive( void *_Data )
        {
//            uint8_t AData[ TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE + 1 ];
//            AData[ 0 ] = Page().GetValue();

			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
            if( FInputData().GetValue() )
                delete [] FInputData().GetValue();

            FInputData() = new uint32_t[ TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE + 1 ];
            uint8_t *AData = (uint8_t *)FInputData().GetValue();
            AData[ 0 ] = Page().GetValue();
            uint32_t ASize = MitovMin<uint32_t>( ADataBlock.Size, TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE );
			for( uint8_t i = 0; i < ASize; ++ i )
                AData[ i + 1 ] = ADataBlock.Data[ i ];

            memset( AData + ASize + 1, 0, TArduinoFingerprintReader::NOTEPAD_PAGE_SIZE - ASize );
            if( ! FReadingRequest() )
                ProcessWrite();

        }

    public:
        inline void SystemLoopBegin()
        {
            if( FReading() )
                return;

            if( FInputData().GetValue() )
                ProcessWrite();

            else if( FReadingRequest() )
                ProcessRead();

        }

    public:
        inline TArduinoFingerprintReaderNotepadElement()
        {
            FReading() = false;
            FReadingRequest() = false;
//            FWriting() = false;
            FInputData() = nullptr;
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
        typename T_Address,
        typename T_Backlight,
        typename T_CapacityOutputPin,
        typename T_CountTemplatesOutputPin,
        typename T_Elements_InvalidPacket,
        typename T_Elements_NeedsRecognition,
        typename T_Elements_ProcessByte,
        typename T_Elements_Recognized,
        typename T_Elements_TimeoutProcess,
		typename T_Enabled,
//        typename T_FDataIndex,
        typename T_FCapacity,
        typename T_FCheckOK,
        typename T_FCheckingCapacity,
        typename T_FHasCount,
        typename T_FInProcessing,
        typename T_FLEDRequest,
        typename T_FPacketLen,
        typename T_FPasswordRequest,
        typename T_FReady,
        typename T_FReceivingData,
        typename T_FReceivingLastData,
		typename T_PasswordCode,
        typename T_Scan_InvalidPacket,
		typename T_Scan_ProcessByte,
        typename T_Scan_TimeoutProcess,
        typename T_StatusOutputPin,
        typename T_Timeout
	> class TArduinoFingerprintReaderSerial :
        public T_Address,
        public T_Backlight,
        public T_CapacityOutputPin,
        public T_CountTemplatesOutputPin,
		public T_Enabled,
//        public T_FDataIndex,
        public T_FCapacity,
        public T_FCheckOK,
        public T_FCheckingCapacity,
        public T_FHasCount,
        public T_FInProcessing,
        public T_FLEDRequest,
        public T_FPacketLen,
        public T_FPasswordRequest,
        public T_FReady,
        public T_FReceivingData,
        public T_FReceivingLastData,
		public T_PasswordCode,
        public T_StatusOutputPin,
        public T_Timeout
	{
	public:
        _V_PIN_( CapacityOutputPin )
        _V_PIN_( CountTemplatesOutputPin )
        _V_PIN_( StatusOutputPin )

	public:
        _V_PROP_( Address )
        _V_PROP_( Backlight )
		_V_PROP_( Enabled )
		_V_PROP_( PasswordCode )
        _V_PROP_( Timeout )

	public:
        _V_PROP_( FPacketLen )
        _V_PROP_( FCapacity )

	protected:
//		_V_PROP_( FDataIndex )
        _V_PROP_( FCheckOK )
        _V_PROP_( FCheckingCapacity )
        _V_PROP_( FHasCount )
        _V_PROP_( FReady )
        _V_PROP_( FInProcessing )
        _V_PROP_( FLEDRequest )
        _V_PROP_( FPasswordRequest )

	public:
        _V_PROP_( FReceivingData )
        _V_PROP_( FReceivingLastData )

	public:
        uint8_t* FData = nullptr;
        uint8_t  FFirstByte;
        uint8_t  FConfirmationCode;
        uint16_t FDataIndex;
//        uint16_t FCapacity = 64; // Needs to read it!
        uint16_t FPacketLength;

	public:
        uint16_t FCount = 0;

	protected:
        uint16_t        FChecksum;
		unsigned long   FLastTime;

	public:
        inline bool GetIsReady()
        {
            return FReady().GetValue() && FHasCount().GetValue() && ( ! FCheckingCapacity().GetValue() );
        }

        inline bool GetInProcessing()
        {
            return FInProcessing().GetValue();
        }

	public:
        void StartReadBuffer( uint32_t ASize )
        {
            FReceivingData() = true;
            FCount = 0;
            AllocateBuffer( ASize );
        }

        void AllocateBuffer( uint32_t ASize )
        {
            if( FData )
                delete [] FData;

            FData = new uint8_t[ ASize ];
        }

        void ReleaseBuffer()
        {
            if( FData )
            {
                delete [] FData;
                FData = nullptr;
            }
        }

        inline void StartConvert( uint8_t ABufferIndex )
        {
            writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_IMAGE2TZ, &ABufferIndex, 1 );
        }

        void StoreInTable( uint8_t ABuffer, uint16_t ALocation )
        {
			uint8_t AData[ 3 ];
            AData[ 0 ] = ABuffer;
            AData[ 1 ] = ALocation >> 8;
            AData[ 2 ] = ALocation;
            writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_STORE, AData, 3 );
        }

        void DeleteFromTable( uint16_t ALocation, uint16_t ACount )
        {
			uint8_t AData[ 4 ];
            AData[ 0 ] = ALocation >> 8;
            AData[ 1 ] = ALocation;
            AData[ 2 ] = ACount >> 8;
            AData[ 3 ] = ACount;
            writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_DELETE, AData, 4 );
        }

	public:
        inline void Recognized( uint16_t AValue )
        {
            T_Elements_Recognized::Call( AValue );
        }

        inline bool GetNeedsRecognition()
        {
            bool AResult = false;
            T_Elements_NeedsRecognition::Call( AResult );
            return AResult;
        }

	public:
		void UpdateEnabled()
		{
			if( ! Enabled().GetValue() )
				return;

            FReady() = false;
            FCheckOK() = true;

            SendPassword();
		}

        void SendPassword()
        {
            if( ! Enabled().GetValue() )
                return;

            if( GetInProcessing() )
            {
                FPasswordRequest() = true;
                return;
            }

/*
            if( ! GetIsReady() )
            {
                FPasswordRequest() = true;
                return;
            }
*/
            FPasswordRequest() = false;
            FLEDRequest() = true;
			writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_VERIFYPASSWORD, PasswordCode().GetValue() );
        }

        void SendLEDControl()
        {
            if( ! Enabled().GetValue() )
                return;

            if( GetInProcessing() )
            {
                FLEDRequest() = true;
                return;
            }

            if( ! GetIsReady() )
            {
                FLEDRequest() = true;
                return;
            }

            FLEDRequest() = false;
            if( Backlight() )
                writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_LEDON );

            else
                writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_LEDOFF );

//			writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_VERIFYPASSWORD, PasswordCode().GetValue() );
        }

        void UploadTemplate()
        {
//            FReceivingData() = true;
//            AllocateBuffer( TArduinoFingerprintReader::FEATURES_SIZE );
            static const uint8_t AData[] = { 1 };
            writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_UPLOAD_TEMPLATE, AData, 1 );
        }

        void LoadFingerprintFromTable( uint16_t AID )
        {
            uint8_t AData[ 3 ];
            AData[ 0 ] = 1;
            AData[ 1 ] = AID >> 8;
            AData[ 2 ] = AID;

            writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_LOAD, AData, 3 );
        }

	public:
		inline void _DirectPinReceive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			for( int i = 0; i < ADataBlock.Size; ++i )
			{
		#ifdef __FINGERPRINT_DEBUG__
    			Serial.print(" 0x");
				Serial.print( ADataBlock.Data[ i ], HEX );
				Serial.print( "," );
		#endif
                ProcessByte( ADataBlock.Data[ i ] );
			}

//		#ifdef __FINGERPRINT_DEBUG__
//    		Serial.println();
//		#endif

		}

        inline void ProcessByte( uint8_t AValue )
        {
//            Serial.print( FDataIndex );
//            Serial.print( " -> " );
//            Serial.println( AValue );

//		const uint8_t C_PACKET_HEADER[] = { FINGERPRINT_STARTCODE >> 8, FINGERPRINT_STARTCODE, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x03, 0x00, 0x00, 0x0A };
            uint8_t AExpected;
            if( FDataIndex < 6 )
            {
                switch ( FDataIndex )
                {
                    case 0  : AExpected = TArduinoFingerprintReader::FINGERPRINT_STARTCODE >> 8; break;
                    case 1  : AExpected = TArduinoFingerprintReader::FINGERPRINT_STARTCODE; break;
                    case 2  : AExpected = Address().GetValue() >> 24; break;
                    case 3  : AExpected = Address().GetValue() >> 16; break;
                    case 4  : AExpected = Address().GetValue() >> 8; break;
                    default : AExpected = Address().GetValue(); FChecksum = 0; break;
//                    default : AExpected = TArduinoFingerprintReader::FINGERPRINT_ACKPACKET; FChecksum = TArduinoFingerprintReader::FINGERPRINT_ACKPACKET; break;
                }

                if( AValue != AExpected )
                {
                    FDataIndex = 0;
                    return;
                }

                ++ FDataIndex;
                FLastTime = millis();
                return;
            }

            FChecksum += AValue;

//          Serial.print( " <+> " );

          switch ( FDataIndex )
            {
                case 6 :
                {
                    if( FReceivingData().GetValue() )
                    {
                        bool AIsLastPacket = ( AValue == TArduinoFingerprintReader::FINGERPRINT_END_OF_DATAPACKET );
                        if( AIsLastPacket )
                            FReceivingLastData() = true;

//bool AIsLastPacket = FIsLastPacket();

                        if( AValue != TArduinoFingerprintReader::FINGERPRINT_DATAPACKET )
                            if( ! AIsLastPacket )
                            {
                                FReceivingData() = false;
                                FReceivingLastData() = false;
                                T_Scan_InvalidPacket::Call();
                                T_Elements_InvalidPacket::Call();
                            }

                        ++ FDataIndex;
                        return;
                    }

                    break;
                }

                case 7 : FPacketLength = uint16_t( AValue ) << 8; ++ FDataIndex; return;
                case 8 : FPacketLength |= AValue; ++ FDataIndex; return;
                case 9 :
                {
////                    Serial.print( "FPacketLength = " );
////                    Serial.println( FPacketLength );

                    if( ! ( FReceivingData().GetValue() || FReceivingLastData().GetValue() ))
                    {
                        FConfirmationCode = AValue;
                        ++ FDataIndex;
                        return;
                    }
                }
            }

//            Serial.print( " <+> " );

            bool ASuccess = false;
            bool APacketEnd = false;
            if( FDataIndex > 9 )
            {
                if( FDataIndex > FPacketLength + 8 - 2 ) // Checksum
                {
                    FChecksum -= AValue;

                    if( FDataIndex == FPacketLength + 8 - 1 )
                    {
                        if( AValue != uint8_t( FChecksum >> 8 ))
                            T_StatusOutputPin::SetPinValue( TArduinoFingerprintReader::FINGERPRINT_STATUS_CHECKSUM_ERROR );

                        ++ FDataIndex;
                        return;
                    }

                    else
                    {
                        APacketEnd = true;
                        if( AValue != uint8_t( FChecksum & 0xFF ) )
                            T_StatusOutputPin::SetPinValue( TArduinoFingerprintReader::FINGERPRINT_STATUS_CHECKSUM_ERROR );

                        else
                        {
                            T_StatusOutputPin::SetPinValue( FConfirmationCode );
                            ASuccess = ( FConfirmationCode == TArduinoFingerprintReader::FINGERPRINT_OK );
                            if( FCheckOK().GetValue() )
                            {
                                if( ! FReady().GetValue() )
                                    FReady() = true;

                                Processed();
                            }
                        }

                        FDataIndex = 0;
                    }
//                    if( AValue
//                    FDataIndex = FDataIndex;
                }
            }

//            Serial.print( " <*> " );
            if( T_CountTemplatesOutputPin::GetPinIsConnected() )
            {
                if( ! FHasCount().GetValue() )
                {
                    switch ( FDataIndex )
                    {
                        case 6 :
                        {
                            if( AValue != TArduinoFingerprintReader::FINGERPRINT_ACKPACKET )
                            {
                                FDataIndex = 0;
                                return;
                            }

                            ++ FDataIndex;
                            return;
                        }
                    }

                    if( ASuccess )
                    {
                        FHasCount() = true;
                        Processed();
                        return;
                    }

                    switch ( FDataIndex )
                    {
                        case 10 : FFirstByte = AValue; break;
                        case 11 :
                        {
                            uint32_t ACountValue = ( uint32_t( FFirstByte ) << 8 ) | AValue;
                            T_CountTemplatesOutputPin::SetPinValue( ACountValue );
                            break;
                        }
                    }
                }
            }

            if( FCheckingCapacity() )
            {
                switch( FDataIndex )
                {
                    case TArduinoFingerprintReader::DATA_INDEX + 4:
                    {
                        FFirstByte = AValue;
                        break;
                    }

                    case TArduinoFingerprintReader::DATA_INDEX + 5:
                    {
                        FCapacity() = ( uint16_t( FFirstByte ) << 8 ) | AValue;
                        break;
                    }

                    case TArduinoFingerprintReader::DATA_INDEX + 12:
                    {
                        FFirstByte = AValue;
                        break;
                    }

                    case TArduinoFingerprintReader::DATA_INDEX + 13:
                    {
                        FPacketLen() = ( uint16_t( FFirstByte ) << 8 ) | AValue;
//                        T_PacketLenOutputPin::SetPinValue( 0b100000 << ( ( uint16_t( C_OWNER.FData[ 12 ] ) << 8 ) | C_OWNER.FData[ 13 ] ));
                        break;
                    }
                }

                if( APacketEnd )
                {
                    FCheckingCapacity() = false;
                    if( ASuccess )
                        T_CapacityOutputPin::SetPinValue( FCapacity() );

                    else
                    {
                        FCapacity() = 0x7FF;
                        FPacketLen() = 0;
                    }
                }
            }

			T_Scan_ProcessByte::Call( AValue, APacketEnd, ASuccess );
            T_Elements_ProcessByte::Call( AValue, APacketEnd, ASuccess );

            if( FDataIndex > 0 )
                ++ FDataIndex;
        }

    public:
		inline void WriteByte( uint8_t AValue )
		{
//            delay( 5 );
			C_SERIAL.Write( AValue );
		#ifdef __FINGERPRINT_DEBUG__
			Serial.print(" 0x");
			Serial.print( AValue, HEX );
			Serial.print(",");
		#endif
		}

    public:
        inline void RequestCount()
        {
            FHasCount() = ! T_CountTemplatesOutputPin::GetPinIsConnected();
        }

        void Processed()
        {
            FDataIndex = 0;
            FInProcessing() = false;
            FCheckOK() = false;
        }

		inline void writeCommandPacket( bool ACheckOK, uint8_t ACommand )
		{
            writeCommandPacket( ACheckOK, ACommand, nullptr, 0 );
        }

		inline void writeCommandPacket( bool ACheckOK, uint8_t ACommand, uint32_t AValue )
		{
			uint8_t AData[ 4 ];
			AData[ 0 ] = AValue >> 24;
			AData[ 1 ] = AValue >> 16;
			AData[ 2 ] = AValue >> 8;
			AData[ 3 ] = AValue;
            writeCommandPacket( ACheckOK, ACommand, AData, 4 );
        }

		inline void writeCommandPacketByte( bool ACheckOK, uint8_t ACommand, uint32_t AValue )
		{
			uint8_t AData[ 1 ];
			AData[ 0 ] = AValue;
            writeCommandPacket( ACheckOK, ACommand, AData, 1 );
        }

		void writeCommandPacket( bool ACheckOK, uint8_t ACommand, const uint8_t *AData, uint16_t ASize )
		{
		#ifdef __FINGERPRINT_DEBUG__
			Serial.println();
			Serial.print(" ->");
		#endif
            FCheckOK() = ACheckOK;
			WriteByte( uint8_t( TArduinoFingerprintReader::FINGERPRINT_STARTCODE >> 8 ));
			WriteByte( uint8_t( TArduinoFingerprintReader::FINGERPRINT_STARTCODE & 0xFF ));
			WriteByte( Address().GetValue() >> 24 ); // Address FFFFFFFF
			WriteByte( Address().GetValue() >> 16 );
			WriteByte( Address().GetValue() >> 8 );
			WriteByte( Address().GetValue() );
			WriteByte( TArduinoFingerprintReader::FINGERPRINT_COMMANDPACKET ); // Packet Type
			uint16_t AWireLength = ASize + 3;
			WriteByte( uint8_t( AWireLength >> 8 ) );
			WriteByte( uint8_t( AWireLength & 0xFF ) );
			uint16_t sum = ( AWireLength >> 8 ) + ( AWireLength & 0xFF ) + TArduinoFingerprintReader::FINGERPRINT_COMMANDPACKET; //packet.type;

			WriteByte( ACommand );
			sum += ACommand;

			for( uint8_t i = 0; i < ASize; ++i )
			{
				WriteByte( AData[ i ] );
				sum += AData[ i ];
			}

			WriteByte( uint8_t( sum >> 8 ));
			WriteByte( uint8_t( sum & 0xFF ));
		#ifdef __FINGERPRINT_DEBUG__
			Serial.println();
		#endif
            FLastTime = millis();
            FInProcessing() = true;
            FDataIndex = 0;
            FReceivingData() = false;
            FReceivingLastData() = false;
            T_StatusOutputPin::SetPinValue( TArduinoFingerprintReader::FINGERPRINT_STATUS_PROCESSING );
		}

        void writeData( const uint8_t *AData, uint32_t ASize )
        {
            uint16_t APacketSize = 0b100000 << FPacketLen().GetValue();
            while( ASize )
            {
                uint32_t ASendSize = MitovMin<uint32_t>( APacketSize, ASize );
                ASize -= ASendSize;
                writeDataPacket( ASize == 0, AData, ASendSize );
                AData += ASendSize;
            }
        }

		void writeDataPacket( bool AIsLastPacket, const uint8_t *AData, uint16_t ASize )
		{
		#ifdef __FINGERPRINT_DEBUG__
			Serial.println();
			Serial.print(" ->");
		#endif
			WriteByte( uint8_t( TArduinoFingerprintReader::FINGERPRINT_STARTCODE >> 8 ));
			WriteByte( uint8_t( TArduinoFingerprintReader::FINGERPRINT_STARTCODE & 0xFF ));
			WriteByte( Address().GetValue() >> 24 ); // Address FFFFFFFF
			WriteByte( Address().GetValue() >> 16 );
			WriteByte( Address().GetValue() >> 8 );
			WriteByte( Address().GetValue() );

            uint8_t APacketType = ( AIsLastPacket ) ? TArduinoFingerprintReader::FINGERPRINT_END_OF_DATAPACKET : TArduinoFingerprintReader::FINGERPRINT_DATAPACKET;
            WriteByte( APacketType );

			uint16_t AWireLength = ASize + 2;
			WriteByte( uint8_t( AWireLength >> 8 ) );
			WriteByte( uint8_t( AWireLength & 0xFF ) );
			uint16_t sum = ( AWireLength >> 8 ) + ( AWireLength & 0xFF ) + APacketType; //packet.type;

//			WriteByte( ACommand );
//			sum += ACommand;

			for( uint8_t i = 0; i < ASize; ++i )
			{
				WriteByte( AData[ i ] );
				sum += AData[ i ];
			}

			WriteByte( uint8_t( sum >> 8 ));
			WriteByte( uint8_t( sum & 0xFF ));
		#ifdef __FINGERPRINT_DEBUG__
			Serial.println();
		#endif
		}

	public:
		inline void SystemStart()
		{
            FCapacity() = 0x7FF;
            FReady() = false;
            FReceivingData() = false;
            FPasswordRequest() = false;
            FReceivingLastData() = false;
            FCheckingCapacity() = false;
            FInProcessing() = false;
            FLEDRequest() = false;
            FCheckOK() = true;
            FHasCount() = ! T_CountTemplatesOutputPin::GetPinIsConnected();
            FDataIndex = 0;
            delay( 1000 );
			UpdateEnabled();
		}

        inline void SystemLoopBegin()
        {
            if( FInProcessing() )
            {
                if( millis() - FLastTime > Timeout() )
                {
                    T_Scan_TimeoutProcess::Call();
                    T_Elements_TimeoutProcess::Call();
                    FDataIndex = 0;
                    FInProcessing() = false;
                    ReleaseBuffer();
                    T_StatusOutputPin::SetPinValue( TArduinoFingerprintReader::FINGERPRINT_STATUS_TIMEOUT_ERROR );
                }

                return;
            }

            if( ! FReady() )
                UpdateEnabled();

            if( FReady() )
            {
                if( FPasswordRequest() )
                    SendPassword();

                else if( FLEDRequest() )
                    SendLEDControl();

                else if( FCapacity() == 0x7FF )
                {
                    if( ! FCheckingCapacity().GetValue() )
                    {
                        FCheckingCapacity() = true;
                        writeCommandPacket( true, TArduinoFingerprintReader::FINGERPRINT_READSYSPARAM );
                    }
                }

                else if( T_CountTemplatesOutputPin::GetPinIsConnected() )
                {
                    if( ! FHasCount() )
                        writeCommandPacket( false, TArduinoFingerprintReader::FINGERPRINT_TEMPLATECOUNT );

                }
            }
        }

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

