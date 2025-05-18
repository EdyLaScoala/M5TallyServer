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

//#define __SIMGSM__DEBUG__

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class MitovGSMSerialSMSModule;
//---------------------------------------------------------------------------
	namespace GSMFinc
	{
		bool ExtractTimeStamp( Mitov::String ATimeStamp, TDateTime &ADateTime, int32_t &ATimeZone )
		{
			if( ATimeStamp.length() < 17 )
				return false;

			Mitov::String AText = ATimeStamp.substring( 0, 2 );
			int16_t AYear = 2000 + AText.toInt();

			AText = ATimeStamp.substring( 3, 5 );
			int16_t AMonth = AText.toInt();

			AText = ATimeStamp.substring( 6, 8 );
			int16_t ADay = AText.toInt();

			AText = ATimeStamp.substring( 9, 11 );
			int16_t AHour = AText.toInt();

			AText = ATimeStamp.substring( 12, 14 );
			int16_t AMinute = AText.toInt();

			AText = ATimeStamp.substring( 15, 17 );
			int16_t ASecond = AText.toInt();

			AText = ATimeStamp.substring( 17, 20 );
			ATimeZone = AText.toInt();

			return ADateTime.TryEncodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, 0 );
		}
	}
//---------------------------------------------------------------------------
	class MitovGSMSerialBasicFunction;
//---------------------------------------------------------------------------
	class BasicMitovGSMSerial : public OpenWire::Object
	{
	protected:
		Mitov::SimpleList<MitovGSMSerialBasicFunction *>	FFunctions;

	public:
		void AddFunction( MitovGSMSerialBasicFunction *AFunction )
		{
			FFunctions.push_back( AFunction );
		}

		void RemoveFunction( MitovGSMSerialBasicFunction *AFunction )
		{
			FFunctions.erase( AFunction );
		}

	};
//---------------------------------------------------------------------------
	class MitovGSMSerialBasicFunction : public OpenWire::Object
	{
	public:
		BasicMitovGSMSerial &FOwner;

	public:
		virtual void ElementSystemStart() {} // Make sure it is different than SystemStart
		virtual bool TryProcessPromptInput( bool &ASuccess ) { ASuccess = false; return false; }
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) { return false; }
		virtual bool TryProcessInput( Mitov::String ALine, bool &ALockInput ) { return false; }
		virtual bool ProcessCharacter( char AChar, bool &ASuccess ) { return false; }
		virtual void TimedOut() {}

	public:
		MitovGSMSerialBasicFunction( BasicMitovGSMSerial &AOwner ) :
			FOwner( AOwner )
		{
			AOwner.AddFunction( this );
		}

		~MitovGSMSerialBasicFunction()
		{
			FOwner.RemoveFunction( this );
		}

	};
//---------------------------------------------------------------------------
	class MitovGSMSerialBasicExpectOKFunction : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		bool	FEmptyLineDetected = false;

	public:
		void Reset()
		{
			FEmptyLineDetected = false;
		}

/*
	protected:
		virtual void OKReceived()
		{
		}
*/
	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
//			Serial.println( "TryProcessRequestedInput" );
//			Serial.println( ALine );
			if( FEmptyLineDetected )
			{
				ALine.trim();
				if( ALine != "" )
				{
					ASuccess = ( ALine == "OK" );
/*
					if( ASuccess )
					{
//						Serial.println( "OK Processed" );
		//					Serial.println( "ALockInput = false" );
						OKReceived();
					}
*/
					AResponseCompleted = true;
					FEmptyLineDetected = false;
					ALockInput = false;
					return true;
				}
			}

			else if( ALine == "" )
			{
//				Serial.println( "FEmptyLineDetected" );
				FEmptyLineDetected = true;
				return true;
			}

			return false;
		}

	public:
		using inherited::inherited;


	};
//---------------------------------------------------------------------------
	typedef void (OpenWire::Object::*TOnMitovGSMSerialFunctionResult) ( bool AResult );
	typedef bool (OpenWire::Object::*TOnMitovGSMSerialFunctionLine) ( Mitov::String ALine );
//---------------------------------------------------------------------------
	class TMitovGSMSerialFunctionExpectOk : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		OpenWire::Object *FOwnerFunction;
		TOnMitovGSMSerialFunctionResult FCallback;

	protected:
		bool	FLocked = false;
		bool	FEmptyLineDetected = false;
		Mitov::String	FLockStringStart;

	public:
		void Reset()
		{
			FEmptyLineDetected = false;
		}

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
//			Serial.println( "TryProcessRequestedInput" );
//			Serial.println( ALine );
			if( FEmptyLineDetected )
			{
				ALine.trim();
#ifdef __SIMGSM__DEBUG__
				Serial.print( "RES1: " ); Serial.println( ALine );
#endif
				ASuccess = ( ALine == "OK" );
				if( ASuccess || ( ALine == "ERROR" ))
				{
					( FOwnerFunction->*FCallback )( ALine == "OK" );

					AResponseCompleted = true;
					FEmptyLineDetected = false;
					ALockInput = false;
					return true;
				}
			}

			else if( ALine == "" )
			{
//				Serial.println( "FEmptyLineDetected" );
				FEmptyLineDetected = true;
				return true;
			}

			else
			{
				ALine.trim();
	//			Serial.print( "SHORT_LINE: " ); Serial.println( AShortLine );
				if( ALine.startsWith( FLockStringStart ))
				{
					ALockInput = true;
					FLocked = true;
	//				Serial.println( "+CMGR: DETECTED!!!"  );
				}
			}

			return false;
		}

	public:
		TMitovGSMSerialFunctionExpectOk( BasicMitovGSMSerial &AOwner, OpenWire::Object *AOwnerFunction, TOnMitovGSMSerialFunctionResult ACallback, Mitov::String ALockStringStart ) :
			inherited( AOwner ),
			FOwnerFunction( AOwnerFunction ),
			FCallback( ACallback )
		{
		}

	};
	//---------------------------------------------------------------------------
	class TMitovGSMSerialFunctionStatus : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		OpenWire::Object *FOwnerFunction;
		TOnMitovGSMSerialFunctionResult FCallback;

	protected:
		bool	FEmptyLineDetected = false;

	public:
		void Reset()
		{
			FEmptyLineDetected = false;
		}

	public:
		virtual void TimedOut() override
		{
			if( FCallback )
				( FOwnerFunction->*FCallback )( false );
		}

		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
//			Serial.println( "TryProcessRequestedInput" );
//			Serial.println( ALine );
			if( FEmptyLineDetected )
			{
				ALine.trim();
#ifdef __SIMGSM__DEBUG__
				Serial.print( "RES1: " ); Serial.println( ALine );
#endif
				ASuccess = ( ALine == "OK" );
				if( ASuccess || ( ALine == "ERROR" ))
				{
					if( FCallback )
						( FOwnerFunction->*FCallback )( ALine == "OK" );

					AResponseCompleted = true;
					FEmptyLineDetected = false;
					ALockInput = false;
					return true;
				}
			}

			else if( ALine == "" )
			{
//				Serial.println( "FEmptyLineDetected" );
				FEmptyLineDetected = true;
				return true;
			}

			return false;
		}

	public:
/*
		TMitovGSMSerialFunctionStatus( MitovGSMSerialBasicFunction *AOwnerFunction, TOnMitovGSMSerialFunctionResult ACallback ) :
			inherited( AOwnerFunction->FOwner ),
			FOwnerFunction( AOwnerFunction ),
			FCallback( ACallback )
		{
		}
*/
		TMitovGSMSerialFunctionStatus( BasicMitovGSMSerial &AOwner, OpenWire::Object *AOwnerFunction, TOnMitovGSMSerialFunctionResult ACallback ) :
			inherited( AOwner ),
			FOwnerFunction( AOwnerFunction ),
			FCallback( ACallback )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_PowerPulse,
		typename T_Shutdown,
		typename T_Startup
	> class TArduinoGSMSerialDelays :
		public T_PowerPulse,
		public T_Shutdown,
		public T_Startup
	{
	public:
		_V_PROP_( PowerPulse )
		_V_PROP_( Shutdown )
		_V_PROP_( Startup )
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Delays,
		typename T_FInPowerCheckWait,
		typename T_FInPowerSwitch,
		typename T_FInPowerSwitchDelay,
		typename T_FInStartupWait,
		typename T_FPowerChecked,
		typename T_IsStarted,
//		typename T_Modules_StartModule,
//		typename T_Modules_StopModule,
		typename T_PowerOn,
		typename T_PowerOutputPin,
		typename T_ReadyOutputPin
	> class MitovGSMSerial :
		public BasicMitovGSMSerial,
		public T_Delays,
		public T_FInPowerCheckWait,
		public T_FInPowerSwitch,
		public T_FInPowerSwitchDelay,
		public T_FInStartupWait,
		public T_FPowerChecked,
		public T_IsStarted,
//		public T_Modules_StartModule,
//		public T_Modules_StopModule,
		public T_PowerOn,
		public T_PowerOutputPin,
		public T_ReadyOutputPin
	{
		typedef MitovGSMSerial<
				T_SERIAL, C_SERIAL,
				T_Delays,
				T_FInPowerCheckWait,
				T_FInPowerSwitch,
				T_FInPowerSwitchDelay,
				T_FInStartupWait,
				T_FPowerChecked,
				T_IsStarted,
//				T_Modules_StartModule,
//				T_Modules_StopModule,
				T_PowerOn,
				T_PowerOutputPin,
				T_ReadyOutputPin
			> SELF;

	public:
		inline BasicMitovGSMSerial &GetRootOwner() { return *this; }

	protected:
		struct TResponseElement
		{
		public:
			MitovGSMSerialBasicFunction *Handler;
			bool Delete;
			uint32_t Timeout;
			Mitov::String Command;
			uint8_t RetryCount;

		public:
			TResponseElement() :
				Handler( nullptr ),
				Delete( false ),
				Timeout( 0 )
			{
			}

			TResponseElement( MitovGSMSerialBasicFunction *AHandler, bool ADelete, uint32_t ATimeout, Mitov::String ACommand, uint8_t ARetryCount ) :
				Handler( AHandler ),
				Delete( ADelete ),
				Timeout( ATimeout ),
				Command( ACommand ),
				RetryCount( ARetryCount )
			{
#ifdef __SIMGSM__DEBUG__
				Serial.print( "TResponseElement: " ); Serial.println( Command );
#endif
			}

			~TResponseElement()
			{
				if( Delete )
					delete Handler;
			}

		};

		struct TQueryElement
		{
		public:
			Mitov::String Command;
			TResponseElement *ResponseElement;

		public:
			TQueryElement() :
				ResponseElement( nullptr )
			{
			}

			TQueryElement( Mitov::String ACommand, TResponseElement *AResponseElement ) :
				Command( ACommand ),
				ResponseElement( AResponseElement )
			{
			}

		};

	public:
		_V_PIN_( PowerOutputPin )
		_V_PIN_( ReadyOutputPin )

	public:
		_V_PROP_( Delays )

		_V_PROP_( PowerOn )

	public:
		_V_PROP_( IsStarted )

	protected:
		_V_PROP_( FPowerChecked )
		_V_PROP_( FInPowerCheckWait )
		_V_PROP_( FInPowerSwitch )
		_V_PROP_( FInPowerSwitchDelay )
		_V_PROP_( FInStartupWait )
		TResponseElement *FCurrentElement = nullptr;
		unsigned long FLastTime = 0;
		unsigned long FCurrentMillis = 0;

	public:
		void UpdatePowerOn()
		{
			if( ! PowerOn() )
				IsStarted() = false;

//			Serial.println( "SetPowerOn" );
//			Serial.println( PowerOn );
			QueryPowerOn();
		}

	protected:
		Mitov::SimpleList<TResponseElement *>	FResponseHandlersQueue;
		Mitov::SimpleList<TQueryElement>		FQueryQueue;

	public:
		void	*FSocketConnections[ 7 ];

	public:
		typedef Stream & T_STREAM;

	public:
		inline Stream &GetStream() { C_SERIAL.GetStream(); }

	public:
		void SendDirect( void *AData, int ASize )
		{
#ifdef __SIMGSM__DEBUG__
			Serial.print( "SENDING : " ); Serial.println( ASize );

			char *APtr = (char *)AData;
			for( int i = 0; i < ASize; ++i )
				Serial.print( *APtr++ );
#endif

			C_SERIAL.GetStream().write( (uint8_t *)AData, ASize );
		}

		void SendQuery( Mitov::String AQuery, TResponseElement *AElement = nullptr )
		{
#ifdef __SIMGSM__DEBUG__
			Serial.print( "QUERY : \"" );	Serial.print( AQuery ); Serial.println( "\"" );
#endif
			FCurrentMillis = millis();

			C_SERIAL.GetStream().println( AQuery );
			FCurrentElement = AElement;
		}

		void TrySendQuery( Mitov::String AQuery, TResponseElement *AElement )
		{
			if( ( PowerOn() || ( AQuery == "AT" ) ) && ( FResponseHandlersQueue.size() == 0 ) && ( ! FInPowerSwitch() ))
				SendQuery( AQuery, AElement );

			else
			{
#ifdef __SIMGSM__DEBUG__
				Serial.print( "ADD TO QUERY : \"" );	Serial.print( AQuery ); Serial.println( "\"" );
#endif
				FQueryQueue.push_back( TQueryElement( AQuery, AElement ));

#ifdef __SIMGSM__DEBUG__
				Serial.println( "QUEUE>>" );
				for( int i = 0; i < FQueryQueue.size(); ++i )
					Serial.println( FQueryQueue[ i ].Command );

				Serial.println( "<<QUEUE" );
#endif
			}
		}

		void SendQueryRegisterResponse( MitovGSMSerialBasicFunction *ASender, Mitov::String AQuery, bool ADelete = false, uint8_t ARetryCount = 0, uint32_t ATimeout = 10000 )
		{
#ifdef __SIMGSM__DEBUG__
			Serial.print( "PUSHING: " ); Serial.println( AQuery );
#endif
			TResponseElement *AElement = new TResponseElement( ASender, ADelete, ATimeout, ( ARetryCount ) ? AQuery : "", ARetryCount );
			TrySendQuery( AQuery, AElement );
			FResponseHandlersQueue.push_back( AElement );

//			SendQuery( AQuery );
//			Serial.print( "PUSHING: " ); Serial.println( AQuery );
//			Serial.println( "PUSHED" );
		}

		void AbortResponseHandler( MitovGSMSerialBasicFunction *ASender )
		{
			if( FLockRequestedInputIndex )
				if( FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ]->Handler == ASender )
					FLockRequestedInputIndex = 0;

#ifdef __SIMGSM__DEBUG__
			Serial.println( "FResponseHandlersQueue.erase" );
#endif
			for( int i = 0; i < FResponseHandlersQueue.size(); ++i )
				if( FResponseHandlersQueue[ i ]->Handler == ASender )
				{
					if( FCurrentElement == FResponseHandlersQueue[ i ] )
						FCurrentElement =nullptr;

					delete FResponseHandlersQueue[ i ];
					FResponseHandlersQueue.Delete( i );
					break;
				}

//			FResponseHandlersQueue.erase( ASender );
		}

	public:
		TMitovGSMSerialFunctionStatus	FPowerOnFunction;

	protected:
		char		FBuffer[ 256 ];
		uint8_t		FIndex = 0;
		uint8_t		FLockInputIndex = 0;
		uint8_t		FLockRequestedInputIndex = 0;

	protected:
		void ProcessNextCommand()
		{
//			Serial.println( "ProcessNextCommand" );
//				Serial.print( "RESP_QUEUE: " ); Serial.println( FResponseHandlersQueue.size() );
			if( FQueryQueue.size() )
			{
//					Serial.print( "SEND_QUERY: " ); Serial.println( FQueryQueue.size() );
				TQueryElement AElement = FQueryQueue[ 0 ];
//					Serial.print( "ESTRACT_QUERY: " ); Serial.println( ACommand );
				FQueryQueue.pop_front();

#ifdef __SIMGSM__DEBUG__
				Serial.println( "QUEUE>>" );
				for( int i = 0; i < FQueryQueue.size(); ++i )
					Serial.println( FQueryQueue[ i ].Command );
				Serial.println( "<<QUEUE" );
#endif

				SendQuery( AElement.Command, AElement.ResponseElement );
//				Serial.print( "SEND_QUERY: " ); Serial.println( FQueryQueue.size() );
			}
		}

		void ReadSerial()
		{
//			if( FInPowerSwitch )
//				return;

			int AChar = C_SERIAL.GetStream().read();
//			Serial.print( AChar );
			if( AChar < 0 )
				return;

			if( FLockInputIndex )
			{
				bool ASuccess = false;
				if( FFunctions[ FLockInputIndex - 1 ]->ProcessCharacter( AChar, ASuccess ))
				{
					if( ASuccess )
						FLockInputIndex = 0;

					return;
				}
			}

//			Serial.print( (char)AChar );
//			if( AChar < ' ' )
//				Serial.println( AChar );

			if( AChar == 13 )
				return;

			if( AChar != 10 )
			{
				if( FIndex == 0 )
					if( AChar == '>' )
						for( int i = 0; i < FResponseHandlersQueue.size(); ++i )
						{
							bool ASuccess = false;
							if( FResponseHandlersQueue[ i ]->Handler->TryProcessPromptInput( ASuccess ))
							{
								if( FCurrentElement == FResponseHandlersQueue[ i ] )
									FCurrentElement = nullptr;

								if( ( ! ASuccess ) && FResponseHandlersQueue[ i ]->Command && FResponseHandlersQueue[ i ]->RetryCount )
								{
#ifdef __SIMGSM__DEBUG__
									Serial.println( "RETRY5" );
									Serial.println( FResponseHandlersQueue[ i ]->Command );
#endif
									-- FResponseHandlersQueue[ i ]->RetryCount;
									SendQuery( FResponseHandlersQueue[ i ]->Command, FResponseHandlersQueue[ i ] );
								}

								else
								{
#ifdef __SIMGSM__DEBUG__
									Serial.println( "Queue Delete 5" );
		//							Serial.println( ALockInput );
		//							Serial.print( "RESP_QUEUE: " ); Serial.println( FResponseHandlersQueue.size() );
#endif
									delete FResponseHandlersQueue[ i ];
									FResponseHandlersQueue.Delete( i );
		//							Serial.println( "ProcessNextCommand 1" );
									ProcessNextCommand();
								}
							}
						}

				FBuffer[ FIndex ++ ] = AChar;
				if( FIndex < 255 )
					return;
			}

//			Serial.println( "TEST!!!" );
//			Serial.println( "" );
//			Serial.println( FIndex );

			FBuffer[ FIndex ] = '\0';
			FIndex = 0;

			Mitov::String AString = FBuffer;

#ifdef __SIMGSM__DEBUG__
			Serial.print( "LINE: " ); Serial.println( AString );

//			Serial.print( "QUEUE: " ); Serial.println( FResponseHandlersQueue.size() );
#endif

			bool	ALockInput;
			bool	AResponseCompleted = false;

//			Serial.print( "FLockRequestedInputIndex : " ); Serial.println( FLockRequestedInputIndex );
//			Serial.print( "FLockInputIndex : " ); Serial.println( FLockInputIndex );
			if( FLockRequestedInputIndex )
			{
				ALockInput = true;
				bool ASuccess = false;
				if( FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ]->Handler->TryProcessRequestedInput( AString, ALockInput, AResponseCompleted, ASuccess ))
				{
					if( AResponseCompleted )
					{
						if( FCurrentElement == FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ] )
							FCurrentElement = nullptr;

						if( ( ! ASuccess ) && FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ]->Command && FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ]->RetryCount )
						{
#ifdef __SIMGSM__DEBUG__
							Serial.println( "RETRY1" );
							Serial.println( FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ]->Command );
#endif
							-- FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ]->RetryCount;
							SendQuery( FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ]->Command, FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ] );
						}

						else
						{
#ifdef __SIMGSM__DEBUG__
							Serial.println( "Queue Delete 1" );
//							Serial.println( ALockInput );
//							Serial.print( "RESP_QUEUE: " ); Serial.println( FResponseHandlersQueue.size() );
#endif
							delete FResponseHandlersQueue[ FLockRequestedInputIndex - 1 ];
							FResponseHandlersQueue.Delete( FLockRequestedInputIndex - 1 );
//							Serial.println( "ProcessNextCommand 1" );
							ProcessNextCommand();
						}
					}

					if( ! ALockInput )
						FLockRequestedInputIndex = 0;
				}

				return;
			}

			ALockInput = false;
			AResponseCompleted = false;
			for( int i = 0; i < FResponseHandlersQueue.size(); ++i )
			{
				bool ASuccess = false;
				if( FResponseHandlersQueue[ i ]->Handler->TryProcessRequestedInput( AString, ALockInput, AResponseCompleted, ASuccess ))
				{
					if( ALockInput )
						FLockRequestedInputIndex = i + 1;

					if( AResponseCompleted )
					{
						if( FCurrentElement == FResponseHandlersQueue[ i ] )
							FCurrentElement = nullptr;

						if( ( ! ASuccess ) && FResponseHandlersQueue[ i ]->Command && FResponseHandlersQueue[ i ]->RetryCount )
						{
#ifdef __SIMGSM__DEBUG__
							Serial.println( "RETRY2" );
							Serial.println( FResponseHandlersQueue[ i ]->Command );
#endif
							-- FResponseHandlersQueue[ i ]->RetryCount;
							SendQuery( FResponseHandlersQueue[ i ]->Command, FResponseHandlersQueue[ i ] );
						}

						else
						{
#ifdef __SIMGSM__DEBUG__
							Serial.println( "Queue Delete 2" );
							Serial.print( "DELETING: " ); Serial.println( FResponseHandlersQueue[ i ]->Command );
//							Serial.print( "RESP_QUEUE: " ); Serial.println( FResponseHandlersQueue.size() );
#endif
							delete FResponseHandlersQueue[ i ];
							FResponseHandlersQueue.Delete( i );
//							Serial.println( "ProcessNextCommand 2" );
							ProcessNextCommand();
						}
					}

					return;
				}
			}

			if( FLockInputIndex )
			{
//				Serial.println( "FLockInputIndex" );
				ALockInput = true;
				FFunctions[ FLockInputIndex - 1 ]->TryProcessInput( AString, ALockInput );
				if( ! ALockInput )
					FLockInputIndex = 0;

				return;
			}

//			Serial.println( "*****" );
			ALockInput = false;
			for( int i = 0; i < FFunctions.size(); ++i )
				if( FFunctions[ i ]->TryProcessInput( AString, ALockInput ))
				{
					if( ALockInput )
					{
						FLockInputIndex = i + 1;
#ifdef __SIMGSM__DEBUG__
						Serial.print( "FLockInputIndex = " ); Serial.println( FLockInputIndex );
#endif
					}

					return;
				}
/*
			ALine.trim();
			if( ALine == "RDY" )
			{
				return true;
			}

			if( ALine == "NORMAL POWER DOWN" )
			{
				return true;
			}

			if( ALine == "Call Ready" )
			{
			}

*/
		}

	public:
		void PulsePower()
		{
#ifdef __SIMGSM__DEBUG__
			Serial.println( "PULSE" );
#endif
			FInPowerSwitch() = true;
			T_PowerOutputPin::SetPinValueHigh();
			FLastTime = micros();
		}

		void PulsePowerDelay()
		{
#ifdef __SIMGSM__DEBUG__
			Serial.println( "PULSE DELAY" );
#endif
			FInPowerSwitchDelay() = true;
			FLastTime = micros();
		}

		void DoPowerResult( bool ASucess )
		{
			if( ! ASucess )
				return;

//			Serial.println( "PowerOKReceived()" );
			FInPowerCheckWait() = false;
			if( PowerOn() && FPowerChecked() )
				StartStartupWait();

			else
			{
#ifdef __SIMGSM__DEBUG__
				Serial.println( "FPowerChecked = true" );
#endif
				if( PowerOn() )
					PulsePower();

				else
					FPowerChecked() = true;
			}

		}

	protected:
		void QueryPowerOn()
		{
//			Serial.print( "FInPowerSwitch = " ); Serial.println( FInPowerSwitch );

			if( FInPowerSwitch() )
				return;

			if( ! T_PowerOutputPin::GetPinIsConnected() )
			{
				ModuleReady();
				return;
			}

//			Serial.println( "AT..." );
			SendQueryRegisterResponse( &FPowerOnFunction, "AT" );
			FLastTime = micros();
			FInPowerCheckWait() = true;
		}

		void ModuleReady()
		{
			IsStarted() = PowerOn();
			T_ReadyOutputPin::ClockPin();

			for( int i = 0; i < FFunctions.size(); ++i )
				FFunctions[ i ]->ElementSystemStart();
		}

		void StartStartupWait()
		{
#ifdef __SIMGSM__DEBUG__
			Serial.println( "STARTUP_WAIT" );
#endif
			FInStartupWait() = true;
			FLastTime = micros();
		}

	public:
		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( FInStartupWait() )
			{
				if( ACurrentMicros - FLastTime >= Delays().Startup() ) // 20000000 )
				{
#ifdef __SIMGSM__DEBUG__
					Serial.println( "FInStartupWait" );
#endif
					FInStartupWait() = false;
					ModuleReady();
				}
			}

			else if( FInPowerCheckWait() )
			{
				if( ACurrentMicros - FLastTime >= 2000000 )
				{
					FInPowerCheckWait() = false;
					FPowerOnFunction.Reset();
					AbortResponseHandler( &FPowerOnFunction );
#ifdef __SIMGSM__DEBUG__
					Serial.println( "FInPowerCheckWait" );
#endif
					if( PowerOn() )
					{
						FPowerChecked() = true;
						PulsePower();
					}

					else
						StartStartupWait();

//					PowerOutputPin.SendValue( false );
				}
			}

			else if( FInPowerSwitchDelay() )
			{
				if( ACurrentMicros - FLastTime >= Delays().Shutdown() ) //10000000 )
				{
#ifdef __SIMGSM__DEBUG__
					Serial.println( "SECOND_PULSE" );
#endif
					FInPowerSwitchDelay() = false;
					PulsePower();
				}
			}

			else if( FInPowerSwitch() )
			{
				if( ACurrentMicros - FLastTime >= Delays().PowerPulse() ) //2000000 )
				{
					T_PowerOutputPin::SetPinValueLow();
					if( FPowerChecked() )
					{
#ifdef __SIMGSM__DEBUG__
						Serial.println( "FInPowerSwitch" );
#endif
						FInPowerSwitch() = false;
						StartStartupWait();
					}

					else
					{
#ifdef __SIMGSM__DEBUG__
						Serial.println( "FPowerChecked = true" );
#endif
						FPowerChecked() = true;
						PulsePowerDelay();
					}
				}
			}

			ReadSerial();
//			Serial.println( "TEST6" );
			if( FCurrentElement )
			{
				unsigned long AMillis = millis();
				if( ( AMillis - FCurrentMillis ) > FCurrentElement->Timeout )
				{
#ifdef __SIMGSM__DEBUG__
					Serial.println( "TIMEOUT" );
#endif
					FLockRequestedInputIndex = 0;

					if( FCurrentElement->Command && FCurrentElement->RetryCount )
					{
#ifdef __SIMGSM__DEBUG__
						Serial.println( "RETRY3" );
						Serial.println( FCurrentElement->Command );
#endif
						-- FCurrentElement->RetryCount;
						SendQuery( FCurrentElement->Command, FCurrentElement );
					}

					else
					{
						FCurrentElement->Handler->TimedOut();
						FResponseHandlersQueue.erase( FCurrentElement );
						delete FCurrentElement;
	//					Serial.println( "ProcessNextCommand 3" );
						ProcessNextCommand();
					}

					FCurrentElement = nullptr;
				}
			}
/*
			if( FResponseHandlersQueue.size() > 0 )
			{
				int AIndex = ( FLockRequestedInputIndex ) ? ( FLockRequestedInputIndex - 1 ) : 0;
				unsigned long AMillis = millis();
//				Serial.print( "TEST7: " ); Serial.println( AIndex );
//				Serial.print( "SIZE: " ); Serial.println( FResponseHandlersQueue.size() );
				if( ( AMillis - FCurrentMillis ) > FResponseHandlersQueue[ AIndex ]->Timeout )
				{
					Serial.print( "TIMEOUT: " ); Serial.println( AIndex );
					FLockRequestedInputIndex = 0;

					if( FResponseHandlersQueue[ AIndex ]->Command && FResponseHandlersQueue[ AIndex ]->RetryCount )
					{
						Serial.println( "RETRY3" );
						Serial.println( FResponseHandlersQueue[ AIndex ]->Command );
						-- FResponseHandlersQueue[ AIndex ]->RetryCount;
						TrySendQuery( FResponseHandlersQueue[ AIndex ]->Command );
					}

					else
					{
						FResponseHandlersQueue[ AIndex ]->Handler->TimedOut();
						delete FResponseHandlersQueue[ AIndex ];
						FResponseHandlersQueue.Delete( AIndex );
	//					Serial.println( "ProcessNextCommand 3" );
						ProcessNextCommand();
					}
				}
			}
*/
			if( FResponseHandlersQueue.size() == 0 )
			{
//				Serial.println( "FResponseHandlersQueue.size() == 0" );
				ProcessNextCommand();
			}
		}

		inline void SystemStart()
		{
//			Serial.println( "SYSTEM_START" );
			T_PowerOutputPin::SetPinValueLow();
			QueryPowerOn();
		}

	public:
		MitovGSMSerial() :
			FPowerOnFunction( *this, this, (TOnMitovGSMSerialFunctionResult)&MitovGSMSerial::DoPowerResult )
		{
			IsStarted() = false;
			FPowerChecked() = false;
			FInPowerCheckWait() = false;
			FInPowerSwitch() = false;
			FInPowerSwitchDelay() = false;
			FInStartupWait() = false;

			memset( FSocketConnections, 0, sizeof( FSocketConnections ) );
		}

	};
//---------------------------------------------------------------------------
    template <
		typename T_Enabled,
		typename T_NumberRings
	> class TArduinoGSMReceivingVoiceCallAutoAnswer :
		public T_Enabled,
		public T_NumberRings
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( NumberRings )

	};
//---------------------------------------------------------------------------
    template <
//		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddressTypeOutputPin,
		typename T_AutoAnswer,
		typename T_CallingOutputPin,
		typename T_NumberOutputPin,
		typename T_PhoneBookAddressOutputPin,
		typename T_SubAddressOutputPin,
		typename T_SubAddressTypeOutputPin
	> class MitovGSMSerialReceivingVoiceCall :
		public T_AddressTypeOutputPin,
		public T_AutoAnswer,
		public T_CallingOutputPin,
		public T_NumberOutputPin,
		public T_PhoneBookAddressOutputPin,
		public T_SubAddressOutputPin,
		public T_SubAddressTypeOutputPin
	{
	public:
		_V_PIN_( CallingOutputPin )
		_V_PIN_( NumberOutputPin )
		_V_PIN_( AddressTypeOutputPin )
		_V_PIN_( SubAddressOutputPin )
		_V_PIN_( SubAddressTypeOutputPin )
		_V_PIN_( PhoneBookAddressOutputPin )

	public:
		_V_PROP_( AutoAnswer )

	public:
		uint8_t	FRingCount = 0;

	public:
/*
		void Ringing()
		{
	//		Serial.println( "RINGING!!!" );
			CallingOutputPin.Notify( nullptr );
			if( AutoAnswer().Enabled() )
			{
				++FRingCount;
				if( FRingCount >= AutoAnswer().NumberRings() )
					C_OWNER.DoReceiveAnswer();
			}
		}
*/
		inline void ClearRingCount()
		{
			FRingCount = 0;
		}

/*
	public:
		inline void AnswerInputPin_o_Receive( void *_Data )
		{
			C_OWNER.DoReceiveAnswer();
		}
*/
/*
	public:
		MitovGSMSerialReceivingVoiceCall()
		{
			C_OWNER.FReceivingCall = this;
		}
*/
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class TMitovGSMSerialFunctionResponseLineAndStatus : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		OpenWire::Object *FOwnerFunction;
		TOnMitovGSMSerialFunctionResult FCallback;
		TOnMitovGSMSerialFunctionLine	FLineCallback;

	protected:
		bool	FEmptyLineDetected : 1; // = false;
		bool	FLocked : 1; // = false;

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
//			Serial.println( "TryProcessRequestedInput" );
//			Serial.println( ALine );
			if( FLocked )
			{
				if( FEmptyLineDetected )
				{
					ALine.trim();
#ifdef __SIMGSM__DEBUG__
					Serial.print( "RES1: " ); Serial.println( ALine );
#endif
					ASuccess = ( ALine == "OK" );
					if( ASuccess || ( ALine == "ERROR" ))
					{
	//					Serial.print( "RES2: " ); Serial.println( ALine );
						if( FCallback )
							( FOwnerFunction->*FCallback )( ALine == "OK" );

						AResponseCompleted = true;
						FEmptyLineDetected = false;
						ALockInput = false;
						FLocked = false;
						return true;
					}
				}

				else if( ALine == "" )
				{
	//				Serial.println( "FEmptyLineDetected" );
					FEmptyLineDetected = true;
					return true;
				}
			}

			else if(( FOwnerFunction->*FLineCallback )( ALine ) )
			{
#ifdef __SIMGSM__DEBUG__
				Serial.println( "READY!!!" );
#endif
				ALockInput = true;
				FLocked = true;
			}

			return false;
		}

	public:
		TMitovGSMSerialFunctionResponseLineAndStatus( OpenWire::Object *AOwnerFunction, TOnMitovGSMSerialFunctionResult ACallback, TOnMitovGSMSerialFunctionLine ALineCallback ) :
			inherited( C_OWNER ),
			FOwnerFunction( AOwnerFunction ),
			FCallback( ACallback ),
			FLineCallback( ALineCallback ),
			FEmptyLineDetected( false ),
			FLocked( false )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class MitovGSMSerialVoiceCallAnswerFunction : public OpenWire::Object
	{
	protected:
//		bool	FEmptyLineDetected = false;
		TMitovGSMSerialFunctionStatus	FQuery;

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		void Send()
		{
			C_OWNER.SendQueryRegisterResponse( &FQuery, "ATA" );
		}

	public:
//		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override;

	public:
		MitovGSMSerialVoiceCallAnswerFunction() :
			FQuery( C_OWNER, this, nullptr )
		{
		}

	};
//---------------------------------------------------------------------------
	enum TArduinoGSMSerialVoiceModuleExistingCallMode {ccmDrop, ccmHold};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BusyOutputPin,
		typename T_InUseOutputPin,
		typename T_ReceivingCall
	> class MitovGSMSerialVoiceCallFunction :
		public MitovGSMSerialBasicFunction,
		public T_BusyOutputPin,
		public T_InUseOutputPin,
		public T_ReceivingCall
	{
		typedef MitovGSMSerialBasicFunction inherited;
/*
		typedef MitovGSMSerialVoiceCallFunction<
				T_OWNER,
				C_OWNER,
				T_BusyOutputPin,
				T_InUseOutputPin
			> SELF;
*/
	public:
		typedef	T_OWNER T_MODULE_OWNER;

	public:
		_V_PIN_( BusyOutputPin )
		_V_PIN_( InUseOutputPin )

	protected:
		_V_PROP_( ReceivingCall )

	protected:
		enum TState { sIdle, sCalling, sReceivingCall, sTalking };

//	public:
//		MitovGSMSerialReceivingVoiceCall<SELF> *FReceivingCall = nullptr;

	public:
		inline T_OWNER &GetOwner() { return C_OWNER; }

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	protected:
		TState	FState = sIdle;

		MitovGSMSerialVoiceCallAnswerFunction<T_OWNER, C_OWNER>	FVoiceCallAnswerFunction;

	protected:
		inline void SetState( TState AValue )
		{
			FState = AValue;
//			Serial.print( "STATE: " ); Serial.println( AValue );
		}

	public:
		inline void ReceivingCall_AnswerInputPin_o_Receive( void *_Data )
		{
			DoReceiveAnswer();
		}

	public:
		inline void DoReceiveAnswer()
		{
			FVoiceCallAnswerFunction.Send();
		}

	public:
		void CallAnswered()
		{
			T_InUseOutputPin::SetPinValueHigh();
			SetState( sTalking );
		}

		inline void MakingCall()
		{
			SetState( sCalling );
		}

		void DropCall()
		{
			if( sIdle )
				return;

			C_OWNER.SendQueryRegisterResponse( new MitovGSMSerialBasicExpectOKFunction( C_OWNER ), "ATH", true );
		}

		void PlaceOnHold()
		{
			if( sIdle )
				return;

			C_OWNER.SendQueryRegisterResponse( new MitovGSMSerialBasicExpectOKFunction( C_OWNER ), "AT+CHLD=2", true ); // Place on Hold
		}

	public:
		virtual bool TryProcessInput( Mitov::String ALine, bool &ALockInput ) override
		{
//			Serial.println( "????" );
//			Serial.println( ALine );
			ALine.trim();
//			Serial.println( "TEST3333" );
			if( ALine.startsWith( "+CLIP:" )) //"RDY" )
			{
				ALine.remove( 0, 6 );
				ALine.trim();
//				Serial.println( "TEST111" );
//				ProcessLine( ALine, false );
//				if( FReceivingCall )
				{
					Mitov::String ANumber;
					if( Func::ExtractOptionallyQuotedCommaText( ALine, ANumber ))
					{
						Mitov::String AAddressType;
						if( Func::ExtractOptionallyQuotedCommaText( ALine, AAddressType ))
						{
							ReceivingCall().NumberOutputPin().SetPinValue( ANumber );
							ReceivingCall().AddressTypeOutputPin().SetPinValue( AAddressType.toInt() );
							ALine.trim();
							Mitov::String ASubAddress;
							if( Func::ExtractOptionallyQuotedCommaText( ALine, ASubAddress ))
							{
								Mitov::String ASubAddressType;
								if( Func::ExtractOptionallyQuotedCommaText( ALine, ASubAddressType ))
								{
									ReceivingCall().SubAddressOutputPin().SetPinValue( ASubAddress );
									ReceivingCall().SubAddressTypeOutputPin().SetPinValue( ASubAddressType.toInt() );
									ALine.trim();

									Mitov::String APhoneBookAddress;
									if( Func::ExtractOptionallyQuotedCommaText( ALine, APhoneBookAddress ))
										ReceivingCall().PhoneBookAddressOutputPin().SetPinValue( APhoneBookAddress );

									else
										ReceivingCall().PhoneBookAddressOutputPin().SetPinValue( "" );
								}
							}
							else
							{
								ReceivingCall().SubAddressOutputPin().SetPinValue( "" );
								ReceivingCall().SubAddressTypeOutputPin().SetPinValue( 0 );
								ReceivingCall().PhoneBookAddressOutputPin().SetPinValue( "" );
							}

						}
					}
				}

				T_InUseOutputPin::SetPinValueHigh();
				SetState( sReceivingCall );
				return true;
			}

			if( ALine == "RING" )
			{
//				Serial.println( "RRRRR" );
//				if( FReceivingCall )
//					ReceivingCall().Ringing();

				ReceivingCall().CallingOutputPin().ClockPin();
				if( ReceivingCall().AutoAnswer().Enabled() )
				{
					++ReceivingCall().FRingCount;
					if( ReceivingCall().FRingCount >= ReceivingCall().AutoAnswer().NumberRings() )
						DoReceiveAnswer();
				}


//					FReceivingCall->CallingOutputPin.Notify( nullptr );

				T_InUseOutputPin::SetPinValueHigh();
				SetState( sReceivingCall );
				return true;
			}

			if( ALine == "BUSY" )
			{
				T_BusyOutputPin::ClockPin();
				T_InUseOutputPin::SetPinValueLow();
				SetState( sIdle );
//				if( FReceivingCall )
					ReceivingCall().ClearRingCount();

				return true;
			}

			if( ALine == "NO CARRIER" )
			{
				T_InUseOutputPin::SetPinValueLow();
				SetState( sIdle );
//				if( FReceivingCall )
					ReceivingCall().ClearRingCount();

				return true;
			}

			if( ALine.startsWith( "+COLP:" ))
			{
				T_InUseOutputPin::SetPinValueHigh();
				SetState( sTalking );
				return true;
			}

			return false;
		}

		virtual void ElementSystemStart() override
		{
			T_InUseOutputPin::SetPinValueLow();
		}

	public:
		MitovGSMSerialVoiceCallFunction() :
			inherited( C_OWNER )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ExistingCallMode,
		typename T_Number
	> class MitovVoiceModuleCallFunction :
		public MitovGSMSerialBasicExpectOKFunction,
		public T_ClockInputPin_o_IsConnected,
		public T_ExistingCallMode,
		public T_Number
	{
		typedef MitovGSMSerialBasicExpectOKFunction inherited;
		typedef MitovVoiceModuleCallFunction<
				T_OWNER,
				C_OWNER,
				T_ClockInputPin_o_IsConnected,
				T_ExistingCallMode,
				T_Number
			>	SELF;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( ExistingCallMode )
		_V_PROP_( Number )

//	protected:
//		SELF &FOwnerFunction;

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ExistingCallMode() == ccmDrop )
				C_OWNER.DropCall();

			else
				C_OWNER.PlaceOnHold();

			C_OWNER.GetOwner().SendQueryRegisterResponse( this, Mitov::String( "ATD" ) + Number() + ";" );
			C_OWNER.MakingCall();
		}

	public:
		virtual void ElementSystemStart() override
		{
			if( ! ClockInputPin_o_IsConnected() )
				ClockInputPin_o_Receive( nullptr );
		}

	public:
		MitovVoiceModuleCallFunction() :
			inherited( C_OWNER.GetOwner() )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Channel,
		typename T_ClockInputPin_o_IsConnected
	> class MitovGSMSerialVoiceSelectAudioChannelFunction :
		public MitovGSMSerialBasicExpectOKFunction,
		public T_Channel,
		public T_ClockInputPin_o_IsConnected
	{
		typedef MitovGSMSerialBasicExpectOKFunction inherited;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Channel )

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.GetOwner().SendQueryRegisterResponse( this, Mitov::String( "AT+CHFA=" ) + Channel() );
		}

	public:
		virtual void ElementSystemStart() override
		{
			if( ! ClockInputPin_o_IsConnected() )
				ClockInputPin_o_Receive( nullptr );
		}

	public:
		MitovGSMSerialVoiceSelectAudioChannelFunction() :
			inherited( C_OWNER.GetOwner() )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Channel,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Volume
	> class MitovGSMSerialVoiceSetVolumeFunction :
		public MitovGSMSerialBasicExpectOKFunction,
		public T_Channel,
		public T_ClockInputPin_o_IsConnected,
		public T_Volume
	{
		typedef MitovGSMSerialBasicExpectOKFunction inherited;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Channel )
		_V_PROP_( Volume )

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			int AVolume = ( Volume() * 15 ) + 0.5;
			C_OWNER.GetOwner().SendQueryRegisterResponse( this, Mitov::String( "AT+CMIC=" ) + Channel() + "," + AVolume );
		}

	public:
		virtual void ElementSystemStart() override
		{
			if( ! ClockInputPin_o_IsConnected() )
				ClockInputPin_o_Receive( nullptr );
		}

	public:
		MitovGSMSerialVoiceSetVolumeFunction() :
			inherited( C_OWNER.GetOwner() )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_FEmptyLineDetected,
		typename T_FLocked,
		typename T_FTextSent,
		typename T_FailedOutputPin,
		typename T_ReferenceOutputPin,
		typename T_Text
	> class MitovGSMSerialSMSSendMessageFunction :
		public MitovGSMSerialBasicFunction,
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_FEmptyLineDetected,
		public T_FLocked,
		public T_FTextSent,
		public T_FailedOutputPin,
		public T_ReferenceOutputPin,
		public T_Text
	{
		typedef MitovGSMSerialBasicFunction inherited;

	public:
		_V_PIN_( ReferenceOutputPin )
		_V_PIN_( FailedOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Text )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FEmptyLineDetected )
		_V_PROP_( FTextSent )
		_V_PROP_( FLocked )

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		void UpdateText()
		{
			if( ! ClockInputPin_o_IsConnected() )
				TrySendValue();

		}

	protected:
		void TrySendValue()
		{
			if( Address().GetValue() == "" )
				return;

			if( Text().GetValue() == "" )
				return;

//			Serial.println( "TEST555" );
			FTextSent() = false;

//			C_OWNER.SendQueryRegisterResponse( this, Mitov::String( "AT+CMGS=\"" ) + Address + "\"\r\n" + Text + "\x1a"  );
			C_OWNER.GetOwner().SendQueryRegisterResponse( this, Mitov::String( "AT+CMGS=\"" ) + Address() + "\"" );
//			C_OWNER.GetStream().print( Text );
//			C_OWNER.GetStream().print((char)26);	//the ASCII code of the ctrl+z is 26
		}

	public:
		virtual bool TryProcessPromptInput( bool &ASuccess ) override
		{
			if( FTextSent() )
				return true;

//			Serial.println( "SEND TEXT!!!" );
			C_OWNER.GetStream().print( Text() );
			C_OWNER.GetStream().print((char)26);	//the ASCII code of the ctrl+z is 26
			FTextSent() = true;
			ASuccess = true;
			return true;
		}

		virtual void TimedOut() override
		{
			T_FailedOutputPin::ClockPin();
		}

		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
			if( FLocked() )
			{
				if( FEmptyLineDetected() )
				{
					ALine.trim();
					if( ALine != "" )
					{
//						Serial.println( "COMPLETE!!!" );
						ASuccess = ( ALine == "OK" );

						if( ! ASuccess )
							T_FailedOutputPin::ClockPin();
/*
						if( ALine == "OK" )
						{
			//				Serial.println( "OK Processed" );
			//					Serial.println( "ALockInput = false" );
						}
*/
						AResponseCompleted = true;
						FEmptyLineDetected() = false;
						FLocked() = false;
						ALockInput = false;
						return true;
					}
				}

				else if( ALine == "" )
				{
//					Serial.println( "FEmptyLineDetected = true" );
					FEmptyLineDetected() = true;
					return true;
				}

				return false;
			}

			ALine.trim();
			if( ALine.startsWith( "+CMGS:" ) )
			{
//				Serial.println( "+CMGS: DETECTED!!!" );
				ALine.remove( 0, 6 );
				ALine.trim();
				uint32_t AReference = ALine.toInt();

				T_ReferenceOutputPin::SetPinValue( AReference );

				ALockInput = true;
				FLocked() = true;
				return true;
			}

/*
			else
			{
				if( ALine == ">" ) )
				{
				}
			}
*/

			return false;
		}

	protected:
		void DoInputChange( void *_Data )
		{
			if( ClockInputPin_o_IsConnected() )
				return;

			TrySendValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			TrySendValue();
		}

	public:
		MitovGSMSerialSMSSendMessageFunction() :
			inherited( C_OWNER )
		{
			FEmptyLineDetected() = false;
			FTextSent() = false;
			FLocked() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER
	> class MitovGSMSerialSMSBasicMessageFunction : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		bool	FLocked = false;
//		bool	FEmptyLineDetected = false;
		Mitov::String	FLines;

	protected:
		virtual void ProcessLine( Mitov::String ALine, bool AIsSecondLine ) {}

	public:
		virtual void ReceivedMessageInfo( Mitov::String AMessageStatus, Mitov::String AAddress, Mitov::String AName, Mitov::TDateTime ADateTime, int32_t ATimeZone ) {}
		virtual void ReceivedMessageText( Mitov::String AText ) {}

	public:
		virtual bool TryProcessInput( Mitov::String ALine, bool &ALockInput ) override
		{
			if( FLocked )
			{
/*
				if( FEmptyLineDetected )
				{
//				Serial.println( ALine );
					FEmptyLineDetected = false;
					ProcessLine( FLines, true );
					ALockInput = false;
					FLocked = false;
//					ReceivedMessageText( FLines );

					Serial.print( "LINES: " ); Serial.println( FLines );
					FLines = "";

					return true;
				}

				else if( ALine == "" )
				{
					Serial.println( "FEmptyLineDetected" );
					FEmptyLineDetected = true;
					return true;
				}
*/
				if( ALine == "" )
				{
//					Serial.println( "FEmptyLineDetected" );
//					FEmptyLineDetected = false;
					ProcessLine( FLines, true );
					ALockInput = false;
					FLocked = false;
//					ReceivedMessageText( FLines );

//					Serial.print( "LINES: " ); Serial.println( FLines );
					FLines = "";

					return true;
				}
			}

			Mitov::String AShortLine = ALine;
			AShortLine.trim();
//			Serial.println( "TEST3333" );
			if( AShortLine.startsWith( "+CMT:" )) //"RDY" )
			{
//				Serial.println( "+CMT: DETECTED!" );
				ProcessLine( ALine, false );

				FLocked = true;
				ALockInput = true;
				return true;
			}

			else if( FLocked )
				FLines += ALine + "\r\n";

			return false;
		}

	public:
		MitovGSMSerialSMSBasicMessageFunction( T_OWNER &AOwner ) :
			inherited( AOwner.GetOwner() )
		{
			AOwner.FMessageInfo = this;
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class MitovGSMSerialSMSModule : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	public:
		typedef	T_OWNER T_MODULE_OWNER;

	public:
		inline T_OWNER &GetOwner() const { return C_OWNER; }

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		MitovGSMSerialSMSBasicMessageFunction< MitovGSMSerialSMSModule<T_OWNER, C_OWNER> > *FMessageInfo = nullptr;

	public:
		MitovGSMSerialSMSModule() :
			inherited( C_OWNER.GetRootOwner() )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddressOutputPin,
		typename T_FIsPDU,
		typename T_NameOutputPin,
		typename T_OutputPin,
		typename T_TimeOutputPin,
		typename T_TimeZoneOutputPin
	> class MitovGSMSerialSMSMessageFunction :
		public MitovGSMSerialSMSBasicMessageFunction<T_OWNER>,
		public T_AddressOutputPin,
		public T_FIsPDU,
		public T_NameOutputPin,
		public T_OutputPin,
		public T_TimeOutputPin,
		public T_TimeZoneOutputPin
	{
		typedef MitovGSMSerialSMSBasicMessageFunction<T_OWNER> inherited;

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( AddressOutputPin )
		_V_PIN_( NameOutputPin )
		_V_PIN_( TimeOutputPin )
		_V_PIN_( TimeZoneOutputPin )

	protected:
		_V_PROP_( FIsPDU )

	public:
		inline T_OWNER &GetOwner() const { return C_OWNER; }
		inline BasicMitovGSMSerial &GetRootOwner() const { return C_OWNER.GetRootOwner(); }

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		virtual void ReceivedMessageInfo( Mitov::String AMessageStatus, Mitov::String AAddress, Mitov::String AName, Mitov::TDateTime ADateTime, int32_t ATimeZone ) override
		{
			T_AddressOutputPin::SetPinValue( AAddress );
			T_NameOutputPin::SetPinValue( AName );
			T_TimeOutputPin::SetPinValue( ADateTime );
			T_TimeZoneOutputPin::SetPinValue( ATimeZone );
		}

		virtual void ReceivedMessageText( Mitov::String AText ) override
		{
			T_OutputPin::SetPinValue( AText );
		}

	public:
		virtual void ProcessLine( Mitov::String ALine, bool AIsSecondLine ) override
		{
			if( AIsSecondLine )
			{
				if( FIsPDU() )
				{
//					Serial.println( ALine );
					// DODO: Decode!
					// http://soft.laogu.com/download/sms_pdu-mode.pdf
					// https://www.diafaan.com/sms-tutorials/gsm-modem-tutorial/online-sms-submit-pdu-decoder/
					// http://jazi.staff.ugm.ac.id/Mobile%20and%20Wireless%20Documents/s_gsm0705pdu.pdf
				}

				else
					T_OutputPin::SetPinValue( ALine );
			}

			else
			{
				FIsPDU() = false;
				ALine.remove( 0, 5 );
				ALine.trim();
				Mitov::String AAddressOrNameOrLength;
				if( Func::ExtractOptionallyQuotedCommaText( ALine, AAddressOrNameOrLength ))
				{
//					Serial.println( "TTT1" );
//					Serial.println( AAddressOrName );
					Mitov::String ANameOrLength;
					if( Func::ExtractOptionallyQuotedCommaText( ALine, ANameOrLength ))
					{
//						Serial.println( "TTT2" );
						Mitov::String ATimeStamp;
						if( Func::ExtractOptionallyQuotedCommaText( ALine, ATimeStamp ))
						{
							// Text Mode
							T_AddressOutputPin::SetPinValue( AAddressOrNameOrLength );
							T_NameOutputPin::SetPinValue( ANameOrLength );

							Mitov::TDateTime ADateTime;
							int32_t ATimeZone;
							if( GSMFinc::ExtractTimeStamp( ATimeStamp, ADateTime, ATimeZone ))
							{
								T_TimeOutputPin::SetPinValue( ADateTime );
								T_TimeZoneOutputPin::SetPinValue( ATimeZone );
							}
						}

						else
						{
//							Serial.println( "YYYYYYYYY" );
							FIsPDU() = true;
//							int ALength = ANameOrLength.toInt();
							T_NameOutputPin::SetPinValue( AAddressOrNameOrLength );
						}
					}

					else
					{
//						Serial.println( "YYYYYYYYY" );
						FIsPDU() = true;
	//					int ALength = ANameOrLength.toInt();
						T_NameOutputPin::SetPinValue( "" );
					}
				}
			}
		}

	public:
		inline MitovGSMSerialSMSMessageFunction() :
			inherited( C_OWNER )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class MitovArduinoGSMSerialDetectDefinedTextFunction :
		public MitovGSMSerialBasicFunction,
		public T_OutputPin
	{
		typedef MitovGSMSerialBasicFunction inherited;

	public:
		_V_PIN_( OutputPin )

	protected:
		const char *FText;

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		virtual bool TryProcessInput( Mitov::String ALine, bool &ALockInput ) override
		{
			ALine.trim();
			if( ALine == FText ) //"RDY" )
			{
				T_OutputPin::ClockPin();
				return true;
			}

			return false;
		}

	public:
		MitovArduinoGSMSerialDetectDefinedTextFunction( const char *AText ) :
			inherited( C_OWNER ),
			FText( AText )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, typename T_OWNER_FUNCTION> class MitovGSMSerialSMSMessageReceivedFunctionRequestSMSDetails : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		T_OWNER_FUNCTION &FOwnerFunction;

	protected:
		bool	FLocked : 1; // = false;
		bool	FEmptyLineDetected : 1; // = false;
		Mitov::String	FLines;

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
	//		Serial.println( "MitovGSMSerialSMSMessageReceivedFunctionRequestSMSDetails::TryProcessRequestedInput" );
	//		Serial.println( ALine );
			if( FEmptyLineDetected )
			{
	//			Serial.println( "FEmptyLineDetected" );
				ALine.trim();
				if( ALine != "" )
				{
					ASuccess = ( ALine == "OK" );
	/*
					if( ALine == "OK" )
					{
	//					Serial.println( "OK Processed" );
	//						Serial.println( "ALockInput = false" );
					}
	*/
					AResponseCompleted = true;
					FEmptyLineDetected = false;
					FLocked = false;
	//				Serial.print( "LINES: " ); Serial.println( FLines );
					if( FOwnerFunction.GetOwner().FMessageInfo )
						FOwnerFunction.GetOwner().FMessageInfo->ReceivedMessageText( FLines );

					FLines = "";
					ALockInput = false;
					return true;
				}
			}

			else if( FLocked && ( ALine == "" ))
			{
	//			Serial.println( "FEmptyLineDetected = true" );
				FEmptyLineDetected = true;
				return true;
			}

			else
			{
				Mitov::String AShortLine = ALine;
				AShortLine.trim();
	//			Serial.print( "SHORT_LINE: " ); Serial.println( AShortLine );
				if( ! AShortLine.startsWith( "+CMGR:" ) )
				{
					if( FLocked )
						FLines += ALine + "\r\n";
				}

				else
				{
					FLocked = true;
	//				Serial.println( "+CMGR: DETECTED!!!"  );
					ALine.remove( 0, 6 );
					ALine.trim();
					Mitov::String AMessageStatus;
					if( Func::ExtractOptionallyQuotedCommaText( ALine, AMessageStatus ))
					{
	//					Serial.println( AMessageStatus );
						Mitov::String AAddress;
						if( Func::ExtractOptionallyQuotedCommaText( ALine, AAddress ))
						{
	//						Serial.println( AAddress );
							Mitov::String AName;
							if( Func::ExtractOptionallyQuotedCommaText( ALine, AName ))
							{
	//							Serial.println( AName );
								Mitov::String ATimeStamp;
								if( Func::ExtractOptionallyQuotedCommaText( ALine, ATimeStamp ))
								{
									Mitov::TDateTime ADateTime;
									int32_t ATimeZone;
									if( GSMFinc::ExtractTimeStamp( ATimeStamp, ADateTime, ATimeZone ))
									{
										if( FOwnerFunction.GetOwner().FMessageInfo )
											FOwnerFunction.GetOwner().FMessageInfo->ReceivedMessageInfo( AMessageStatus, AAddress, AName, ADateTime, ATimeZone );
	//									TimeOutputPin.Notify( &ADateTime );
	//									TimeZoneOutputPin.Notify( &ATimeZone );
									}
								}
							}
						}
					}
				}
			}

			return false;
		}

	public:
		MitovGSMSerialSMSMessageReceivedFunctionRequestSMSDetails( T_OWNER_FUNCTION &AOwnerFunction ) :
			inherited( AOwnerFunction.FOwner ),
			FOwnerFunction( AOwnerFunction ),
			FLocked( false ),
			FEmptyLineDetected( false )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_IndexOutputPin,
		typename T_ReceivedOutputPin,
		typename T_StorageOutputPin
	> class MitovGSMSerialSMSMessageReceivedFunction :
		public MitovGSMSerialBasicFunction,
		public T_IndexOutputPin,
		public T_ReceivedOutputPin,
		public T_StorageOutputPin
	{
		typedef MitovGSMSerialBasicFunction inherited;
		typedef MitovGSMSerialSMSMessageReceivedFunction<
				T_OWNER,
				C_OWNER,
				T_IndexOutputPin,
				T_ReceivedOutputPin,
				T_StorageOutputPin
			> SELF;

	public:
		_V_PIN_( StorageOutputPin )
		_V_PIN_( IndexOutputPin )
		_V_PIN_( ReceivedOutputPin )

	protected:
		void RequestDetails( int32_t AIndex )
		{
			C_OWNER.FOwner.SendQueryRegisterResponse( new MitovGSMSerialSMSMessageReceivedFunctionRequestSMSDetails<typename T_OWNER::T_MODULE_OWNER, SELF>( *this ), Mitov::String( "AT+CMGR=" ) + AIndex + Mitov::String( ",1" ), true );
		}

	public:
		inline T_OWNER &GetOwner() const { return C_OWNER; }

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		virtual bool TryProcessInput( Mitov::String ALine, bool &ALockInput ) override
		{
//			Serial.println( "+CMTI: TEST!!!" );
			ALine.trim();
			if( ALine.startsWith( "+CMTI:" ) )
			{
//				Serial.println( "+CMTI: DETECTED!!!" );
//				Serial.println( "ALine.startsWith" );
//				Serial.println( ALine );
				Mitov::String AStorageTypeText;
				if( Func::ExtractOptionallyQuotedCommaText( ALine, AStorageTypeText ))
				{
					Mitov::String AIndexText;
					if( Func::ExtractOptionallyQuotedCommaText( ALine, AIndexText ))
					{
						int32_t	AStorageType = AStorageTypeText.toInt();
						T_StorageOutputPin::SetPinValue( AStorageType );

						int32_t	AIndex = AIndexText.toInt();
						T_IndexOutputPin::SetPinValue( AIndex );

						RequestDetails( AIndex );

//						ReceivedOutputPin.Notify( nullptr );
					}
				}

				return true;
			}

			return false;
		}

		void ReportMessageDetails()
		{
			T_ReceivedOutputPin::ClockPin();
		}

	public:
		MitovGSMSerialSMSMessageReceivedFunction() :
			inherited( C_OWNER.FOwner )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class GSMSim900InputOutputModule
	{
	public:
		typedef T_OWNER _T_OWNER;

		typedef TMitovGSMSerialFunctionResponseLineAndStatus<T_OWNER, C_OWNER> _TMitovGSMSerialFunctionResponseLineAndStatus;

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		static constexpr inline T_OWNER &GetOwner() { return C_OWNER; }

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ErrorOutputPin,
		typename T_FErrorStarted,
		typename T_FStarted,
		typename T_OutputPin
	> class MitovSIM900ReadADCFunction :
		public T_ErrorOutputPin,
		public T_FErrorStarted,
		public T_FStarted,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ErrorOutputPin )

	protected:
		_V_PROP_( FStarted )
		_V_PROP_( FErrorStarted )

		typename T_OWNER::_TMitovGSMSerialFunctionResponseLineAndStatus FQuery;

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	protected:
		bool DoProcessLine( Mitov::String ALine )
		{
			ALine.trim();
#ifdef __SIMGSM__DEBUG__
			Serial.print( "TESTING: " ); Serial.println( ALine );
#endif
			if( ! ALine.startsWith( "+CADC:" ))
				return false;

			int APos = ALine.indexOf( ",", 6 );
			if( APos >= 0 )
			{
				Mitov::String ALeft = ALine.substring( 6, APos );
				Mitov::String ARight = ALine.substring( APos + 1 );
				ALeft.trim();
				ARight.trim();
				int ASuccess = ALeft.toInt();
				float AValue = ARight.toInt();

//					Serial.println( ASuccess );
//					Serial.println( AValue );

				T_ErrorOutputPin::SetPinValue( ASuccess != 0, FErrorStarted() );
				FErrorStarted() = true;

				if( ASuccess )
				{
					AValue /= 2800;
					T_OutputPin::SetPinValue( AValue, FStarted() );
					FStarted() = true;
				}
			}

			return true;
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.GetOwner().IsStarted() )
				return;

			C_OWNER.GetOwner().AbortResponseHandler( &FQuery );
			C_OWNER.GetOwner().SendQueryRegisterResponse( &FQuery, "AT+CADC?" );
		}

	public:
		inline MitovSIM900ReadADCFunction() :
			FQuery( (OpenWire::Object *)this, nullptr, (TOnMitovGSMSerialFunctionLine)&MitovSIM900ReadADCFunction::DoProcessLine )
		{
			FStarted() = false;
			FErrorStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BitErrorRateOutputPin,
		typename T_BitErrorRateUnknownOutputPin,
		typename T_FStarted,
		typename T_SignalStrengthOutputPin,
		typename T_SignalStrengthUnknownOutputPin
	> class MitovGSMSerialGetSignalStrengthFunction :
		public MitovGSMSerialBasicFunction,
		public T_BitErrorRateOutputPin,
		public T_BitErrorRateUnknownOutputPin,
		public T_FStarted,
		public T_SignalStrengthOutputPin,
		public T_SignalStrengthUnknownOutputPin
	{
		typedef MitovGSMSerialBasicFunction inherited;

	public:
		_V_PIN_( SignalStrengthOutputPin )
		_V_PIN_( SignalStrengthUnknownOutputPin )
		_V_PIN_( BitErrorRateOutputPin )
		_V_PIN_( BitErrorRateUnknownOutputPin )

	protected:
		_V_PROP_( FStarted )
		TMitovGSMSerialFunctionResponseLineAndStatus<T_OWNER, C_OWNER> FQuery;

	public:
		inline Stream &GetStream() { C_OWNER.GetStream(); }

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "DoClockReceive" );
			C_OWNER.AbortResponseHandler( &FQuery );
			C_OWNER.SendQueryRegisterResponse( &FQuery, "AT+CSQ" );
		}

	protected:
		bool DoProcessLine( Mitov::String ALine )
		{
			ALine.trim();
			if( ! ALine.startsWith( "+CSQ:" ) )
				return false;
//				Serial.println( "ALine.startsWith" );
//				Serial.println( ALine );

			int APos = ALine.indexOf( ",", 6 );
			if( APos >= 0 )
			{
				Mitov::String ALeft = ALine.substring( 6, APos );
				Mitov::String ARight = ALine.substring( APos + 1 );
				ALeft.trim();
				ARight.trim();
				int AStrength = ALeft.toInt();
				int ABitErrorRate = ARight.toInt();

//					Serial.println( AStrength );
//					Serial.println( ABitErrorRate );

				if( AStrength > 31 )
				{
					T_SignalStrengthOutputPin::SetPinValue( 0, FStarted() );
					T_SignalStrengthUnknownOutputPin::SetPinValue( true, FStarted() );
				}

				else
				{
					T_SignalStrengthOutputPin::SetPinValue( float( AStrength ) / 31, FStarted() );
					T_SignalStrengthUnknownOutputPin::SetPinValue( false, FStarted() );
				}

				if( ABitErrorRate > 31 )
				{
					T_BitErrorRateOutputPin::SetPinValue( 0, FStarted() );
					T_BitErrorRateUnknownOutputPin::SetPinValue( true, FStarted() );
				}

				else
				{
					T_BitErrorRateOutputPin::SetPinValue( float( ABitErrorRate ) / 7, FStarted() );
					T_BitErrorRateUnknownOutputPin::SetPinValue( false, FStarted() );
				}

				FStarted() = true;
			}

			return true;
		}

	public:
		virtual void ElementSystemStart() override
		{
			ClockInputPin_o_Receive( nullptr );
		}

	public:
		MitovGSMSerialGetSignalStrengthFunction() :
			inherited( C_OWNER ),
			FQuery( (OpenWire::Object *)this, nullptr, MAKE_CALLBACK_3_1( OpenWire::Object, TOnMitovGSMSerialFunctionLine, MitovGSMSerialGetSignalStrengthFunction::DoProcessLine, Mitov::String ))
		{
		}

	};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

