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
#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) ) // IRremote
	#include <utility/IRLib_HashRaw.h>
	#include <Adafruit_Circuit_Playground.h>
//	#include <IRLib_HashRaw.h>
#else
//	#if ( defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_ESP8266 ) )
	#if defined( VISUINO_ESP8266 )
	  #include <IRremoteESP8266.h>  // The IRremote Arduino Library https://github.com/z3t0/Arduino-IRremote needs to be installed trough the Arduino IDE for this Scetch to compile!
	  #include <IRrecv.h>
	  #include <IRsend.h>

	#else
	  #if defined( SEEEDUINO_WIO_TERMINAL )
//		#include <IRLibCombo.h>
//	    #include <IRLibProtocols.h>
//		#include <IRLibRecv.h>
		#include <IRLibAll.h>
	  #else
		#include <IRremote.h>
	  #endif

	#endif
#endif

#include <Mitov_InfraredRemote_Structure.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( VISUINO_ESP8266 ) || defined( SEEEDUINO_WIO_TERMINAL ) )

	#define _OLD_IR_REMOTE_VALUE_FORMAT_

// Decoded value for NEC when a repeat code is received
//
//	const uint8_t USECPERTICK	= 50;
	#if( ! defined( VISUINO_ESP8266 ))
		#define PANASONIC PANASONIC_OLD

//	typedef IRdecodeBase TDecodeResults;
		typedef uint8_t decode_type_t;

		#if ( ! defined( SEEEDUINO_WIO_TERMINAL ))
			typedef IRrecvPCI	IRrecv;
		#endif

	#endif

	#if ! defined(RAW_BUFFER_LENGTH)
		#define RAW_BUFFER_LENGTH  100  ///< Maximum length of raw duration buffer. Must be even. 100 supports up to 48 bit codings inclusive 1 start and 1 stop bit.
	#endif

	struct irparams_struct {
		// The fields are ordered to reduce memory over caused by struct-padding

	#if RAW_BUFFER_LENGTH <= 255        // saves around 75 bytes program space and speeds up ISR
		uint8_t rawlen;                 ///< counter of entries in rawbuf
	#else
		unsigned int rawlen;            ///< counter of entries in rawbuf
	#endif

		uint16_t *rawbuf; // [RAW_BUFFER_LENGTH]; ///< raw data / tick counts per mark/space, first entry is the length of the gap between previous and current command
	};

	struct IRData {
		decode_type_t protocol;     ///< UNKNOWN, NEC, SONY, RC5, ...
		uint16_t address;           ///< Decoded address
		uint16_t command;           ///< Decoded command
		uint16_t extra;             ///< Used by MagiQuest and for Kaseikyo unknown vendor ID
		uint8_t numberOfBits;       ///< Number of bits received for data (address + command + parity) - to determine protocol length if different length are possible.
		uint8_t flags;              ///< See IRDATA_FLAGS_* definitions above
		uint32_t decodedRawData;    ///< Up to 32 bit decoded raw data, used for sendRaw functions.
		irparams_struct *rawDataPtr; ///< Pointer of the raw timing data to be decoded. Mainly the data buffer filled by receiving ISR.
	};

#endif

namespace Mitov
{
	const uint32_t _IR_REPEAT		= 0xFFFFFFFF;

	typedef IRData TDecodeResults;
/*
	class TDecodeResults
	{
		public:
			decode_type_t          protocol;  // UNKNOWN, NEC, SONY, RC5, ...
			unsigned int           address;      // Used by Panasonic & Sharp [16-bits]
			unsigned long          value;        // Decoded value [max 32-bits]
			int                    bits;         // Number of bits in decoded value
			volatile uint16_t	  *rawbuf;      // Raw intervals in 50uS ticks
			int                    rawlen;       // Number of records in rawbuf
			int                    overflow;     // true iff IR raw code too long
	};
*/
//---------------------------------------------------------------------------
	template <
		int C_TYPE,
		typename T_OutputPin,
		typename T_Value
	> class InfraredRemoteReceiverTypedCommandElement :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		void TryDecode( TDecodeResults &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.protocol != C_TYPE )
				return;

			if( AResult.decodedRawData != Value() )
				return;

			T_OutputPin::ClockPin();
			ASuccess = true;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_FLastValue,
		typename T_OutputPin,
		typename T_RepeatInterval,
		typename T_RepeatOutputPin,
		typename T_Value
	> class InfraredRemoteReceiverNECCommandElement :
		public T_FLastValue,
		public T_OutputPin,
		public T_RepeatInterval,
		public T_RepeatOutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( RepeatOutputPin )

	public:
		_V_PROP_( Value )
		_V_PROP_( RepeatInterval )

	protected:
		_V_PROP_( FLastValue )

		unsigned long	FLastTime = 0;

	public:
		void TryDecode( TDecodeResults &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.protocol != NEC )
				return;

			if(( AResult.decodedRawData == _IR_REPEAT ) && ( ALastCode == Value() ))
			{
				FLastTime = millis();
				T_OutputPin::SetPinValue( true, true );
				FLastValue() = true;
				T_RepeatOutputPin::ClockPin();
				ASuccess = true;
				return;
			}

			if( AResult.decodedRawData == Value() )
			{
				FLastTime = millis();
				T_OutputPin::SetPinValue( false, true );
				T_OutputPin::SetPinValue( true, true );
				FLastValue() = true;
				ASuccess = true;
				return;
			}
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FLastValue() )
				if( millis() - FLastTime > RepeatInterval() )
				{
					T_OutputPin::SetPinValue( false, true );
					FLastValue() = false;
				}

		}

		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( false, false );
		}

	public:
		InfraredRemoteReceiverNECCommandElement()
		{
			FLastValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		uint8_t COUNT_OutputPins,
		typename T_FIndex,
		typename T_OutputPins_SetPinValue,
		typename T_RepeatInterval,
		typename T_RepeatOutputPin
	> class InfraredRemoteReceiverNECButtonsBasicCommandsElement :
		public T_FIndex,
		public T_RepeatInterval,
		public T_RepeatOutputPin
	{
	public:
		_V_PIN_( RepeatOutputPin )

	public:
		_V_PROP_( RepeatInterval )

	protected:
		_V_PROP_( FIndex )

	protected:
		uint32_t FLastValue = 0;

	protected:
		unsigned long	FLastTime = 0;

	protected:
		inline void SetOutputPinValue( uint8_t AIndex, bool AValue )
		{
			T_OutputPins_SetPinValue::Call( AIndex, AValue );
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FIndex() < COUNT_OutputPins )
				if( millis() - FLastTime > RepeatInterval() )
				{
					SetOutputPinValue( FIndex(), false );
					FIndex() = COUNT_OutputPins;
				}
		}

		inline void SystemStart()
		{
			for( int i = 0; i < COUNT_OutputPins; ++i )
				T_OutputPins_SetPinValue::Call( i, false );

		}

	public:
		inline InfraredRemoteReceiverNECButtonsBasicCommandsElement()
		{
			FIndex() = COUNT_OutputPins;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Values_Array,
		const T_Values_Array *C_Values_Array,
		uint8_t COUNT_OutputPins,
		typename T_FIndex,
		typename T_OutputPins_SetPinValue,
		typename T_RepeatInterval,
		typename T_RepeatOutputPin
	> class InfraredRemoteReceiverNECButtonsCommandsElement :
		public InfraredRemoteReceiverNECButtonsBasicCommandsElement <
				COUNT_OutputPins,
				T_FIndex,
				T_OutputPins_SetPinValue,
				T_RepeatInterval,
				T_RepeatOutputPin
			>
	{
		typedef InfraredRemoteReceiverNECButtonsBasicCommandsElement <
				COUNT_OutputPins,
				T_FIndex,
				T_OutputPins_SetPinValue,
				T_RepeatInterval,
				T_RepeatOutputPin
			> inherited;

	protected:
		bool TrySendValue( uint32_t AValue )
		{
			uint8_t AOldIndex = inherited::FIndex();
			inherited::FIndex() = COUNT_OutputPins;
			bool AMatched = false;
			T_Values_Array *APtr = C_Values_Array;
			for( uint8_t i = 0; i < COUNT_OutputPins; ++ i )
				if( PROGMEM_getAnything( APtr ++ ) == AValue )
				{
					inherited::FIndex() = i;
					AMatched = true;
					break;
				}


			if( ! AMatched )
			{
				if( AOldIndex < COUNT_OutputPins )
					inherited::SetOutputPinValue( AOldIndex, false );

				return false;
			}

			inherited::FLastTime = millis();
			if( AOldIndex == inherited::FIndex() )
				return true;

			if( AOldIndex < COUNT_OutputPins )
				inherited::SetOutputPinValue( AOldIndex, false );

			inherited::SetOutputPinValue( inherited::FIndex(), true );
			return true;
		}

	public:
		void TryDecode( TDecodeResults &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.protocol != NEC )
				return;

			if(( AResult.decodedRawData == _IR_REPEAT ) && ( ALastCode == inherited::FLastValue ))
			{
				if( inherited::FLastValue )
				{
					TrySendValue( inherited::FLastValue );
					T_RepeatOutputPin::ClockPin();
					ASuccess = true;
					return;
				}
			}

			if( TrySendValue( AResult.decodedRawData ))
				inherited::FLastValue = AResult.decodedRawData;

			ASuccess = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Values_Array,
		const T_Values_Array *C_Values_Array,
		uint8_t COUNT_OutputPins,
		typename T_FIndex,
		typename T_OutputPin,
		typename T_OutputPins_SetPinValue,
		typename T_RepeatInterval,
		typename T_RepeatOutputPin
	> class InfraredRemoteReceiverNECButtonsOtputCommandsElement :
		public InfraredRemoteReceiverNECButtonsBasicCommandsElement <
				COUNT_OutputPins,
				T_FIndex,
				T_OutputPins_SetPinValue,
				T_RepeatInterval,
				T_RepeatOutputPin
			>,
		public T_OutputPin
	{
		typedef InfraredRemoteReceiverNECButtonsBasicCommandsElement <
				COUNT_OutputPins,
				T_FIndex,
				T_OutputPins_SetPinValue,
				T_RepeatInterval,
				T_RepeatOutputPin
			> inherited;

	public:
		_V_PIN_( OutputPin )

		bool TrySendValue( uint32_t AValue )
		{
			uint8_t AOldIndex = inherited::FIndex();
			inherited::FIndex() = COUNT_OutputPins;
			bool AMatched = false;
			T_Values_Array *APtr = C_Values_Array;
			for( uint8_t i = 0; i < COUNT_OutputPins; ++ i )
			{
				T_Values_Array AItem = PROGMEM_getAnything( APtr ++ );
				if( AItem.Code == AValue )
				{
					inherited::FIndex() = i;
					char ACharValue = AItem.Value;
					if( ACharValue )
						T_OutputPin::SetPinValue( ACharValue, false );

					AMatched = true;
					break;
				}
			}

			if( ! AMatched )
			{
				if( AOldIndex < COUNT_OutputPins )
					inherited::SetOutputPinValue( AOldIndex, false );

				return false;
			}

			inherited::FLastTime = millis();
			if( AOldIndex == inherited::FIndex() )
				return true;

			if( AOldIndex < COUNT_OutputPins )
				inherited::SetOutputPinValue( AOldIndex, false );

			inherited::SetOutputPinValue( inherited::FIndex(), true );
			return true;
		}

	public:
		void TryDecode( TDecodeResults &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.protocol != NEC )
				return;

			if(( AResult.decodedRawData == _IR_REPEAT ) && ( ALastCode == inherited::FLastValue ))
				if( inherited::FLastValue )
				{
					TrySendValue( inherited::FLastValue );
					T_RepeatOutputPin::ClockPin();
					ASuccess = true;
					return;
				}

			if( TrySendValue( AResult.decodedRawData ))
				inherited::FLastValue = AResult.decodedRawData;

			ASuccess = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Address,
		typename T_OutputPin,
		typename T_Value
	> class InfraredRemoteReceiverPanasonicCommandElement :
		public T_Address,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )
		_V_PROP_( Address )

	public:
		void TryDecode( TDecodeResults &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.protocol != PANASONIC )
				return;

			if( AResult.decodedRawData != Value() )
				return;

			if( AResult.address != Address() )
				return;

			T_OutputPin::ClockPin();
			ASuccess = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Code,
		typename T_OutputPin
	> class InfraredRemoteReceiverAiwaRCT501CommandElement :
		public T_Code,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Code )

	public:
		void TryDecode( TDecodeResults &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.protocol != PANASONIC )
				return;

			if( AResult.decodedRawData != Code() )
				return;

			T_OutputPin::ClockPin();
			ASuccess = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AddressOutputPin,
		typename T_ClearInterval,
		typename T_DataTypeOutputPin,
		typename T_Elements_TryDecode,
		typename T_Enabled,
		typename T_G_Infrared_Resume,
		typename T_OutputPin,
		typename T_RawOutputPin,
		typename T_ReplaceNECRepeatCode,
		uint8_t	C_SensorInputPin
	> class InfraredRemoteReceiver :
		public T_AddressOutputPin,
		public T_ClearInterval,
		public T_DataTypeOutputPin,
		public T_Elements_TryDecode,
		public T_Enabled,
		public T_G_Infrared_Resume,
		public T_OutputPin,
		public T_RawOutputPin,
		public T_ReplaceNECRepeatCode
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( AddressOutputPin )
		_V_PIN_( DataTypeOutputPin )
		_V_PIN_( RawOutputPin )

	public:
		_V_PROP_( ClearInterval )
		_V_PROP_( Enabled )
		_V_PROP_( ReplaceNECRepeatCode )

	protected:
		_V_PROP_( G_Infrared_Resume )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				FSensor.enableIRIn();

		}

	public:
		inline void ResumeSensor()
		{
			if( Enabled() )
				FSensor.enableIRIn();
		}

	protected:
		IRrecv		FSensor;
#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( SEEEDUINO_WIO_TERMINAL ) )
		IRdecode	FDecoder;
#endif
		uint32_t	FLastCode = 0;

	protected:
		unsigned long	FLastTime = 0;

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				FSensor.enableIRIn();

		}

		inline void SystemLoopBegin()
		{
			if( FLastCode )
				if( millis() - FLastTime > ClearInterval().GetValue() )
					FLastCode = 0;

			if( Enabled() )
			{
				if( G_Infrared_Resume() )
				{
					G_Infrared_Resume() = false;
//					Serial.println( "G_Infrared_Resume" );
#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( SEEEDUINO_WIO_TERMINAL ) )
					FSensor.enableIRIn();
#else
					FSensor.enableIRIn();
					FSensor.resume();
#endif
				}

				irparams_struct ARawData;
				TDecodeResults AResults;
				AResults.rawDataPtr = &ARawData;

#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( SEEEDUINO_WIO_TERMINAL ) )
				if ( FDecoder.decode())
				{
					AResults.protocol = FDecoder.protocolNum;
					AResults.address = FDecoder.address;
					AResults.decodedRawData = FDecoder.value;
					AResults.numberOfBits = FDecoder.bits;
					AResults.rawDataPtr->rawbuf = (uint16_t*)recvGlobal.recvBuffer;
					AResults.rawDataPtr->rawlen = recvGlobal.recvLength;
//					AResults.overflow = 0;
#else // VISUINO_CIRCUIT_PLAYGROUND_EXPRESS
	#if ( defined( VISUINO_ESP8266 ) )
				decode_results AESP8266Results;
				if ( FSensor.decode( &AESP8266Results ))
				{
					AResults.protocol = AESP8266Results.decode_type;
					AResults.address = AESP8266Results.address;
					AResults.decodedRawData = AESP8266Results.value;
					AResults.numberOfBits = AESP8266Results.bits;
					AResults.rawDataPtr->rawbuf = (uint16_t*)AESP8266Results.rawbuf;
					AResults.rawDataPtr->rawlen = AESP8266Results.rawlen;
	#else // VISUINO_ESP8266
				if ( FSensor.decode())
				{
					AResults = FSensor.decodedIRData;

					switch( AResults.protocol )
					{
						case NEC:
						{
							if( AResults.decodedRawData == 0 )
							{
								AResults.decodedRawData = _IR_REPEAT;
								break;
							}
						}

						case PANASONIC:
						case SONY:
						case SAMSUNG:
						case JVC:
							AResults.decodedRawData = Reverse32Bits( AResults.decodedRawData );

					}

	#endif // VISUINO_ESP8266
#endif // VISUINO_CIRCUIT_PLAYGROUND_EXPRESS
					uint32_t *ARawData;
					uint32_t ARawDataLength = 0;
					if( T_RawOutputPin::GetPinIsConnected() )
						{
							ARawDataLength = AResults.rawDataPtr->rawlen;
							if( ARawDataLength )
							{
								ARawData = new uint32_t[ ARawDataLength ];
								for( int i = 0; i < ARawDataLength; ++ i )
									ARawData[ i ] = AResults.rawDataPtr->rawbuf[ i ];
							}
						}

//					Serial.print( "Len: " ); Serial.println( AResults.rawlen );
#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( SEEEDUINO_WIO_TERMINAL ) )
					FSensor.enableIRIn();
#else
					FSensor.resume();
#endif

					bool ASuccess = false;
					T_Elements_TryDecode::Call( AResults, FLastCode, ASuccess );
/*
					for( int i = 0; i < FElements.size(); ++i )
						if( FElements[ i ]->TryDecode( AResults, FLastCode ))
							break;
*/
					uint32_t AValue = AResults.decodedRawData;

					if( AResults.protocol == NEC )
					{
						if( ReplaceNECRepeatCode() )
							if( AValue == _IR_REPEAT )
							{
								AValue = FLastCode;
								if( FLastCode != 0 )
									FLastTime = millis();
							}

						if( AValue != _IR_REPEAT )
						{
							FLastCode = AValue;
							FLastTime = millis();
						}
					}

					T_DataTypeOutputPin::SetPinValue( AResults.protocol );
					T_AddressOutputPin::SetPinValue( AResults.address );
					if( T_RawOutputPin::GetPinIsConnected() )
						if( ARawDataLength )
						{
							T_RawOutputPin::SetPinValue( TArray<uint32_t>( ARawDataLength, ARawData ));
							delete [] ARawData;
						}

					T_OutputPin::SetPinValue( AValue );
				}
			}
		}

	public:
		inline InfraredRemoteReceiver() :
			FSensor( C_SensorInputPin )
		{
			G_Infrared_Resume() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FSentOneJVC,
		typename T_G_Infrared_Resume,
		uint8_t	C_SensorOutputPin
	> class InfraredRemoteTransmitter :
		public T_Enabled,
		public T_FSentOneJVC,
		public T_G_Infrared_Resume
	{
	public:
		IRsend	FSensor;

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FSentOneJVC )
		_V_PROP_( G_Infrared_Resume )

		uint32_t	FLastValueJVC = 0;

	public:
		inline void ResumeSensor()
		{
			G_Infrared_Resume() = true;
//			if( Enabled() )
//				FSensor.enableIROut();
		}

	public:
		bool RepeatedJVCCommand( uint32_t AValue )
		{
			bool AResult = ( FLastValueJVC == AValue );
			FLastValueJVC = AValue;
			FSentOneJVC() = true;

			return AResult;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
			{
				TArray<uint32_t> *AData = (TArray<uint32_t>*)_Data;
				uint16_t * code = new uint16_t[ AData->Size ];
				for( int i = 0; i < AData->Size; ++i )
					code[ i ] = AData->Data[ i ];

//#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( SEEEDUINO_WIO_TERMINAL ) ) // IRremote
#if ( defined( VISUINO_CIRCUIT_PLAYGROUND_EXPRESS ) || defined( SEEEDUINO_WIO_TERMINAL ) ) // IRremote
//				FSensor.send( code, AData->Size, 38 ); // 38KHz
#else // VISUINO_CIRCUIT_PLAYGROUND_EXPRESS
				FSensor.sendRaw( code, AData->Size, 38 ); // 38KHz
#endif // VISUINO_CIRCUIT_PLAYGROUND_EXPRESS
				delete [] code;
			}
		}

	public:
		inline void SystemInit()
		{
//#ifdef VISUINO_ANY_ESP32
//			FSensor.begin();
//#endif
		}

	public:
		inline InfraredRemoteTransmitter()
#if( ! ( defined( VISUINO_ANY_ESP32 ) || defined( SEEEDUINO_WIO_TERMINAL )) )
			: FSensor( C_SensorOutputPin )
#endif
		{
			FSentOneJVC() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendRC5CommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendRC5( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendRC6CommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendRC6( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendNECCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//	#if defined( VISUINO_ESP8266 )
	#if defined( SEEEDUINO_WIO_TERMINAL )
			C_OWNER.FSensor.send( NEC, Value(), NumberBits(), 0 );
    #else
			C_OWNER.FSensor.sendNEC( Value(), NumberBits() );
	#endif
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendSonyCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSony( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address,
		typename T_Value
	> class InfraredRemoteTransmitterSendPanasonicCommandElement :
		public T_Address,
		public T_Value
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendPanasonic( Address(), Value() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_UseRepeat,
		typename T_Value
	> class InfraredRemoteTransmitterSendJVCCommandElement :
		public T_NumberBits,
		public T_UseRepeat,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )
		_V_PROP_( UseRepeat )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			bool ARepeat = C_OWNER.RepeatedJVCCommand( Value() );
			if( UseRepeat() && ARepeat )
				C_OWNER.FSensor.sendJVC( Value(), NumberBits(), true );

			else
				C_OWNER.FSensor.sendJVC( Value(), NumberBits(), false );

			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendSAMSUNGCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSAMSUNG( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendWhynterCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendWhynter( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Code
	> class InfraredRemoteTransmitterSendAiwaRCT501CommandElement :
		public T_Code
	{
	public:
		_V_PROP_( Code )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendAiwaRCT501( Code() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendLGCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendLG( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendDISHCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendDISH( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendSharpRawCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSharpRaw( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address,
		typename T_Command
	> class InfraredRemoteTransmitterSendSharpCommandElement :
		public T_Address,
		public T_Command
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Command )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSharp( Address(), Command() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendDenonCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendDenon( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Code,
//		typename T_Fallback,
		typename T_Repeat
	> class InfraredRemoteTransmitterSendProntoCommandElement :
		public T_Code,
//		public T_Fallback,
		public T_Repeat
	{
	public:
		_V_PROP_( Code )
		_V_PROP_( Repeat )
//		_V_PROP_( Fallback )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendPronto( (char *)Code().GetValue().c_str(), Repeat().GetValue() ); //, Fallback().GetValue() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendLEGOPowerFunctionsCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendLegoPowerFunctions( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Data,
        typename T_Data_ApplyValues
	> class InfraredRemoteTransmitterSendRawCommandElement :
		public T_Data
	{
	public:
		_V_PROP_( Data )

    protected:
        void SendOutput()
        {
            uint16_t *ABuffer = T_Data::Allocate();

            T_Data::CopyData( ABuffer );

            T_Data_ApplyValues::Call( ABuffer );

            C_OWNER.FSensor.sendRaw( ABuffer, T_Data::GetCount(), 38 ); // 38KHz

            T_Data::Release( ABuffer );
        }

	public:
		void SetIndexedValue( uint32_t AIndex )
        {
//            T_Elements::SetValue( AIndex, AValue );
              SendOutput();
        }

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( T_Data::GetIsConstant() )
                SendOutput();

			else
				C_OWNER.FSensor.sendRaw( T_Data::GetArray(), T_Data::GetCount(), 38 ); // 38KHz

			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class InfraredRemoteTransmitterSendConstRawCommandElement
	{
	public:
		const /*PROGMEM*/ uint16_t *_Values;
		uint32_t _Count;

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "TEST" );
			uint16_t *AValues = new uint16_t[ _Count ];
			for( int i = 0; i < _Count; ++i )
			{
#if ( defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_ESP8266 ) )
				AValues[ i ] = pgm_read_word( _Values + i ) * 50;
#else
				AValues[ i ] = pgm_read_word( _Values + i ) * 50; //USECPERTICK;
#endif
//				Serial.println( AValues[ i ] );
			}

			C_OWNER.FSensor.sendRaw( AValues, _Count, 38 ); // 38KHz

			delete [] _Values;

			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
