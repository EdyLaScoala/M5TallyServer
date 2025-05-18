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
#include <Mitov_GSM_Serial.h>
#include <Mitov_StringPrint.h>

//#define __SIMGPRS__DEBUG__

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//	class SIMComGPRSModule;
//	class SIMComGPRSTCPClientSocket;
//---------------------------------------------------------------------------
	template <
		typename T_MODULE, T_MODULE &C_OWNER,
		typename T_Enabled,
		typename T_FIsConnected,
		typename T_FRunning,
		typename T_Port
	> class BasicSIMComGPRSSocket : 
		public MitovGSMSerialBasicFunction,
		public T_Enabled,
		public T_FIsConnected,
		public T_FRunning,
		public T_Port
	{
	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FRunning )
		_V_PROP_( FIsConnected )

	protected:
		virtual void StartSocket() = 0;

	public:
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

		virtual void EndPacket()
		{
		}

		virtual void StopSocket() = 0;

		inline void SystemLoopBegin() 
		{
			StartSocket();
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

		virtual void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

	protected:
		BufferPrint	FStringPrint;

	public:
		inline void DisconnectInputPin_o_Receive( void * )
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.println( "DoDisconnect" );
#endif
			if( Enabled() )
			{
				Enabled() = false;
				UpdateEnabled();
			}
		}

	public:
		virtual bool IsEnabled()
		{
			return Enabled(); // && inherited::FRunning(); //C_OWNER.Enabled;
		}

		virtual Print &GetPrint()
		{
			return FStringPrint;
		}

		inline bool CanSend()
		{
			return Enabled().GetValue() && FRunning().GetValue();
		}

		inline bool GetReadyToConnect()
		{
			return C_OWNER.Enabled() && C_OWNER.GetIsStarted();
		}

		virtual void BeginPacket()
		{
			FStringPrint.Value.clear();
		}

//		virtual void DataReceived( int connectionId, unsigned char AData ) = 0;

	public:
		BasicSIMComGPRSSocket() :
			MitovGSMSerialBasicFunction( C_OWNER.GetOwner() )
		{
			FRunning() = false;
			FIsConnected() = false;
//			C_OWNER.Sockets.push_back( this );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_MODULE> class TMitovGSMSerialGPRSGetIPFunctionStatus : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		T_MODULE *FOwnerModule;

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
	//			Serial.println( "TryProcessRequestedInput" );
	//			Serial.println( ALine );
	//			delay( 100 );
			if( ALine != "" )
			{
				ALine.trim();
#ifdef __SIMGPRS__DEBUG__
				Serial.print( "RES IP: " ); Serial.println( ALine );
#endif

				bool AIsValid = true;
				int ACountDots = 0;
				for( int i = 0; i < ALine.length(); ++i )
				{
					if( ALine[ i ] == '.' )
						++ACountDots;

					else if( ! isdigit( ALine[ i ] ))
					{
						AIsValid = false;
						break;
					}
				}

#ifdef __SIMGPRS__DEBUG__
				Serial.println( AIsValid );
				Serial.println( ACountDots );
#endif

				ASuccess = ( ( ACountDots == 3 ) && AIsValid );
				if( ASuccess)
					FOwnerModule->GetIPConfigResult( ALine ); //AddressOutputPin.SendValue( ALine, false );

				AResponseCompleted = true;
				ALockInput = false;
				return true;
			}

			return false;
		}

	public:
		TMitovGSMSerialGPRSGetIPFunctionStatus( T_MODULE *AOwnerModule ) :
			inherited( AOwnerModule->GetOwner() ),
			FOwnerModule( AOwnerModule )
		{
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AccessPoints_ResetRetry,
		typename T_AccessPoints_TryConnect,
		typename T_AddressOutputPin,
		typename T_Enabled,
		typename T_FConnected,
		typename T_FFromStart,
		typename T_FInConnecting,
		typename T_FMultiConnection,
		typename T_FTryNextConnection,
		uint32_t C_APNContext,
		typename T_Sockets_StopSocket
	> class SIMComGPRSModule :
		public MitovGSMSerialBasicFunction,
		public T_AccessPoints_ResetRetry,
		public T_AccessPoints_TryConnect,
		public T_AddressOutputPin,
		public T_Enabled,
		public T_FConnected,
		public T_FFromStart,
		public T_FInConnecting,
		public T_FMultiConnection,
		public T_FTryNextConnection,
		public T_Sockets_StopSocket
	{
		typedef SIMComGPRSModule<
				T_OWNER, 
				C_OWNER,
				T_AccessPoints_ResetRetry,
				T_AccessPoints_TryConnect,
				T_AddressOutputPin,
				T_Enabled,
				T_FConnected,
				T_FFromStart,
				T_FInConnecting,
				T_FMultiConnection,
				T_FTryNextConnection,
				C_APNContext,
				T_Sockets_StopSocket
			> SELF;

	public:
		_V_PIN_( AddressOutputPin )

	public:
		typedef	T_OWNER	TOwner;

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FConnected )
		_V_PROP_( FFromStart )
		_V_PROP_( FInConnecting )
		_V_PROP_( FMultiConnection )
		_V_PROP_( FTryNextConnection )
//		uint8_t	FAPNContext;

	public:
		inline T_OWNER	&GetOwner() { return C_OWNER; }

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

	public:
/*
		bool AssignServerID( BasicSIMComGPRSSocket *ASocket )
		{
			bool AResult = false;
			for( int i = 0; i < sizeof( FOwner.FSocketConnections ) / sizeof( FOwner.FSocketConnections[ 0 ] ); ++i )
				if( ! FOwner.FSocketConnections[ i ] )
				{
					FOwner.FSocketConnections[ i ] = ASocket;
//					FServerInstance = ASocket;
					AResult = true;
				}

			return AResult;
		}
*/
		void AddServerConnection( void *ASocket, int AIndex )
		{
			if( ! C_OWNER.FSocketConnections[ AIndex ] )
				C_OWNER.FSocketConnections[ AIndex ] = ASocket;

		}

		bool RemoveServerConnection( void *ASocket, int AIndex )
		{
			if( C_OWNER.FSocketConnections[ AIndex ] == ASocket )
			{
				C_OWNER.FSocketConnections[ AIndex ] = nullptr;
				return true;
			}

			return false;
		}

		bool IsServerConnection( void *ASocket, int AIndex )
		{
			return( C_OWNER.FSocketConnections[ AIndex ] == ASocket );
		}

		void ReleaseServerID( void *ASocket )
		{
//			if( FServerInstance == ASocket )
//				FServerInstance = nullptr;

			for( int i = 0; i < sizeof( C_OWNER.FSocketConnections ) / sizeof( C_OWNER.FSocketConnections[ 0 ] ); ++i )
				if( C_OWNER.FSocketConnections[ i ] == ASocket )
					C_OWNER.FSocketConnections[ i ] = nullptr;
		}

		bool AssignConnectionID( void *ASocket, int &AID )
		{
			for( int i = 0; i < sizeof( C_OWNER.FSocketConnections ) / sizeof( C_OWNER.FSocketConnections[ 0 ] ); ++i )
				if( ! C_OWNER.FSocketConnections[ i ] )
				{
					C_OWNER.FSocketConnections[ i ] = ASocket;
					AID = i;
					return true;
				}

			return false;
		}

		void ReleaseConnectionID( int AID )
		{
//			FOwner.FSocketConnections[ AID ] = FServerInstance;
		}

	public:
		bool GetIsStarted() { return FConnected(); }

	public:
		void ConnectResult( bool AValue )
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.print( "STATUS: " ); Serial.println( AValue );
#endif
			if( AValue )
			{
				FConnected() = true;
				T_AccessPoints_ResetRetry::Call();
//				for( int i = 0; i < AccessPoints.size(); ++ i )
//					AccessPoints[ i ]->FRetryCount = 0;

				StartConnection();
			}

			else
				TryNextConnection();
		}

		void GetIPConfigResult( Mitov::String AIPAddress )
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.println( "TEST5" );
#endif
			T_AddressOutputPin::SetPinValue( AIPAddress );

//			for( int i = 0; i < Sockets.size(); ++i )
//				Sockets[ i ]->TryStartSocket();
		}

	protected:
		void SetMultiConnectionResult( bool AValue )
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.println( "TEST3_1" );
#endif
			delay( 200 );
			if( AValue )
				SetContext();

#ifdef __SIMGPRS__DEBUG__
			Serial.println( "TEST3_2" );
#endif
			delay( 200 );
		}

		void SetContextResult( bool AValue )
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.println( "TEST3" );
#endif
			delay( 200 );
			if( AValue )
				TryNextConnection();
		}

		void StartConnectionResult( bool AValue )
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.println( "TEST4" );
#endif
			if( AValue )
				GetIPConfig();

		}

		inline void StartConnection()
		{
//			FOwner.SendQueryRegisterResponse( new TMitovGSMSerialFunctionStatus( this, (TOnMitovGSMSerialFunctionResult)&SIMComGPRSModule::StartConnectionResult ), Mitov::String( "AT+CIICR" ), true, 5 );
			C_OWNER.SendQueryRegisterResponse( new TMitovGSMSerialFunctionStatus( C_OWNER, MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSModule::StartConnectionResult, bool )), Mitov::String( "AT+CIICR" ), true, 5 );
		}

		inline void GetIPConfig()
		{
			C_OWNER.SendQueryRegisterResponse( new TMitovGSMSerialGPRSGetIPFunctionStatus<SELF>( this ), Mitov::String( "AT+CIFSR" ), true, 5 );
		}

	public:
		inline bool TryConnectToPoint( String AAccessPointName, String AUserName, String APassword )
		{
			MitovGSMSerialBasicFunction *AFunction = new TMitovGSMSerialFunctionStatus( C_OWNER, MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSModule::ConnectResult, bool ));

			if( AUserName == "" )
				C_OWNER.SendQueryRegisterResponse( AFunction, Mitov::String( "AT+CSTT=\"" ) + AAccessPointName + "\"", true );

			else
				C_OWNER.SendQueryRegisterResponse( AFunction, Mitov::String( "AT+CSTT=\"" ) + AAccessPointName + "\",\"" + AUserName + "\",\"" + APassword + "\"", true );

			return false;
//			 return FGprs.attachGPRS( (char *)AAccessPointName.c_str(), (char *)AUserName.c_str(), (char *)APassword.c_str() );
		}

	protected:
		void TryNextConnection()
		{
			FTryNextConnection() = false;
			unsigned long ACurrentMicros = micros();

			bool ATryNextConnection = false;
			bool AProccessed = false;
			T_AccessPoints_TryConnect::Call( FFromStart(), ATryNextConnection, AProccessed );

/*
			for( int i = 0; i < AccessPoints.size(); ++ i )
			{
				if( AccessPoints[ i ]->Enabled )
				{
//					Serial.println( "TryNextConnection" );
					bool ACanRetry = true;
					if( ! FFromStart )
						if( ! AccessPoints[ i ]->CanRetry( ACanRetry ) )
						{
							ATryNextConnection |= ACanRetry;
							continue;
						}

					ATryNextConnection |= ACanRetry;
					Mitov::String AUserName = AccessPoints[ i ]->UserName;

					MitovGSMSerialBasicFunction *AFunction = new TMitovGSMSerialFunctionStatus( C_OWNER, MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSModule::ConnectResult, bool ));

					if( AUserName == "" )
						C_OWNER.SendQueryRegisterResponse( AFunction, Mitov::String( "AT+CSTT=\"" ) + AccessPoints[ i ]->AccessPointName + "\"", true );

					else
						C_OWNER.SendQueryRegisterResponse( AFunction, Mitov::String( "AT+CSTT=\"" ) + AccessPoints[ i ]->AccessPointName + "\",\"" + AccessPoints[ i ]->UserName + "\",\"" + AccessPoints[ i ]->Password + "\"", true );

					AccessPoints[ i ]->FLastTime = ACurrentMicros;
					++ AccessPoints[ i ]->FRetryCount;
					return;
				}
			}
*/
			FTryNextConnection() = ATryNextConnection;
//			Serial.print( "FTryNextConnection: " ); Serial.println( FTryNextConnection );
		}

		void SetMultiConnection()
		{
			if( FMultiConnection() )
				TryNextConnection();

			else
			{
#ifdef __SIMGPRS__DEBUG__
				Serial.println( "TEST1" );
#endif
//				FOwner.SendQueryRegisterResponse( this, Mitov::String( "AT+CIPMUX=1" ), true );
				C_OWNER.SendQueryRegisterResponse( new TMitovGSMSerialFunctionStatus( C_OWNER, MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSModule::SetMultiConnectionResult, bool )), Mitov::String( "AT+CIPMUX=1" ), true, 5 );
			}
		}

		void SetContext()
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.println( "TEST2" );
			Serial.print( "FAPNContext: " ); Serial.println( FAPNContext );
#endif
			C_OWNER.SendQueryRegisterResponse( new TMitovGSMSerialFunctionStatus( C_OWNER, MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSModule::SetContextResult, bool )), Mitov::String( "AT+CIPSGTXT=" ) + C_APNContext, true, 5 );
		}

		void TryConnect( bool FromStart )
		{
			if( FInConnecting() )
				return;

			FInConnecting() = true;
			FFromStart() = FromStart;
			SetMultiConnection();


/*
			for( int i = 0; i < AccessPoints.size(); i ++ )
				if( AccessPoints[ i ]->Enabled )
				{
					bool ACanRetry = false;
					if( ! FromStart )
						if( ! AccessPoints[ i ]->CanRetry( ACanRetry ) )
							continue;

					Mitov::String AUserName = AccessPoints[ i ]->UserName;

					if( AUserName == "" )
						FOwner.SendQueryRegisterResponse( &FRequestConnectAccessPoint, Mitov::String( "AT+CSTT=\"" ) + AccessPoints[ i ]->AccessPointName + "\"" );

					else
						FOwner.SendQueryRegisterResponse( &FRequestConnectAccessPoint, Mitov::String( "AT+CSTT=\"" ) + AccessPoints[ i ]->AccessPointName + "\",\"" + AccessPoints[ i ]->UserName + "\",\"" + AccessPoints[ i ]->Password + "\"" );
/ *
					FConnected = FGprs.attachGPRS( (char *)AccessPoints[ i ]->AccessPointName.c_str(), (char *)AccessPoints[ i ]->UserName.c_str(), (char *)AccessPoints[ i ]->Password.c_str() );
					if( FConnected )
						break;

					AccessPoints[ i ]->FLastTime = ACurrentMicros;
					++ AccessPoints[ i ]->FRetryCount;
* /
				}
*/
		}

		void StopEthernet()
		{
			T_Sockets_StopSocket::Call();

//			LWiFi.end();
//			IsStarted = false;
			FConnected() = false;
		}

		void StartEthernet()
		{
			if( ! C_OWNER.IsStarted() )
				return;
/*
			if( ! AccessPoints.length() )
				return;

			if( ! Sockets.length() )
				return;
*/
			TryConnect( true );
//			if( AccessPointName != "" )
//				IsStarted = FGprs.attachGPRS( (char *)AccessPointName.c_str(), (char *)UserName.c_str(), (char *)Password.c_str() );

//			else
//				AConnected = FGprs.attachGPRS();

/*
			if( AConnected )
			{
			}
*/
		}

	public:
		inline void SystemLoopBegin()
		{
			if( Enabled() )
				if( ! FConnected() )
					if( C_OWNER.IsStarted() )
						TryConnect( false );

			if( FTryNextConnection() )
				TryNextConnection();

		}

	public:
		virtual void ElementSystemStart() override
		{
#ifdef __SIMGPRS__DEBUG__
			Serial.print( "FAPNContext: " ); Serial.println( APNContext );
#endif

			if( Enabled() )
				if( ! FConnected() )
					if( C_OWNER.IsStarted() )
						TryConnect( false );

		}

	public:
		SIMComGPRSModule() :
			MitovGSMSerialBasicFunction( C_OWNER )
		{
			FFromStart() = false;
			FInConnecting() = false;
			FMultiConnection() = false;
			FTryNextConnection() = false;
			FConnected() = false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER> class TSIMComGPRSTCPSocketSendPacket : public MitovGSMSerialBasicFunction
	{
		typedef MitovGSMSerialBasicFunction inherited;

	protected:
		T_OWNER* FOwnerSocket;
		BufferPrint	FStringPrint;

	public:
		virtual bool TryProcessPromptInput( bool &ASuccess ) override
		{
			FOwnerSocket->GetOwner().GetOwner().SendDirect( FStringPrint.Value, FStringPrint.Value.size() );
			ASuccess = true;
			return true;
		}

/*
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &ALockInput, bool &AResponseCompleted, bool &ASuccess ) override
		{
			if( ALine.startsWith( ">" ) )
			{
				C_OWNER.GetOwner().SendDirect( FStringPrint.Value, FStringPrint.Value.size() );
				ALockInput = false;
				ASuccess = true;
				return true;
			}

			return false;
		}
*/
	public:
		TSIMComGPRSTCPSocketSendPacket( T_OWNER *AOwnerSocket, BufferPrint AStringPrint ) :
			inherited( AOwnerSocket->GetOwner().GetOwner() ),
			FOwnerSocket( AOwnerSocket ),
			FStringPrint( AStringPrint )
		{
	/*
	#ifdef __SIMGPRS__DEBUG__
			Serial.println( "TCPSocketSendPacket" );

			for( int i = 0; i < FStringPrint.Value.size(); ++i )
				Serial.println( FStringPrint.Value[ i ] );

			Serial.println( "" );

	#endif
	*/
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_MODULE, T_MODULE &C_OWNER, 
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FInOpening,
		typename T_FIsConnected,
		typename T_FRunning,
		typename T_ListeningOutputPin,
		typename T_OutputPin,
		typename T_Port
	> class SIMComGPRSTCPServerSocket : 
		public BasicSIMComGPRSSocket<
				T_MODULE, C_OWNER,
				T_Enabled,
				T_FIsConnected,
				T_FRunning,
				T_Port
			>,
		public T_ConnectedOutputPin, 
		public T_FInOpening,
		public T_ListeningOutputPin, 
		public T_OutputPin
	{
		typedef BasicSIMComGPRSSocket<
				T_MODULE, 
				C_OWNER,
				T_Enabled,
				T_FIsConnected,
				T_FRunning,
				T_Port
			> inherited;

		typedef SIMComGPRSTCPServerSocket<
				T_MODULE, 
				C_OWNER, 
				T_ConnectedOutputPin,
				T_Enabled,
				T_FInOpening,
				T_FIsConnected,
				T_FRunning,
				T_ListeningOutputPin,
				T_OutputPin,
				T_Port
			> SELF;

	public:
		typedef T_MODULE TOwner;

		inline T_MODULE &GetOwner() { return C_OWNER; }

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )
		_V_PIN_( ListeningOutputPin )

	protected:
		int	FClientCurrentID = -1;

	protected:
		_V_PROP_( FInOpening )
		int		FReadSize = 0;
		int		FReadCounter;
		uint8_t	*FBuffer = nullptr;
		uint8_t	*FBufferPtr;

	public:
		virtual bool ProcessCharacter( char AChar, bool &ASuccess ) override
		{ 
			if( FReadCounter )
			{
				*FBufferPtr++ = *(uint8_t*)&AChar;
				--FReadCounter;
//				Serial.println( FReadSize );
//				Serial.println( AChar );
				if( !FReadCounter )
				{
#ifdef __SIMGPRS__DEBUG__
					Serial.println( "SERVER RECEIVE COMPLETED" );
#endif
					ASuccess = true;
					T_OutputPin::SendPinValue( Mitov::TDataBlock( FReadSize, FBuffer ));
					FClientCurrentID = -1;
//					Serial.println( (char *)FBuffer );
					delete [] FBuffer;
					FBuffer = nullptr;
				}

				return true;
			}

			return false; 
		}

		virtual bool TryProcessInput( Mitov::String ALine, bool &ALockInput ) override
		{ 
			ALine.trim();
			if( FInOpening() )
			{
				if( ALine == "SERVER OK" )
				{
#ifdef __SIMGPRS__DEBUG__
					Serial.println( "ServerSocketListening" );
#endif
					T_ListeningOutputPin::SetPinValue( true, false );
					FInOpening() = false;
					inherited::FRunning() = true;
					return true;
				}
			}

			else if( Func::ContainsTextAtPos( ALine, 1, ", REMOTE IP:" ))
			{
				int AIndex = ALine[ 0 ] - '0';
				if( ( AIndex >= 0 ) && ( AIndex <= 7 ))
				{
					C_OWNER.AddServerConnection( this, AIndex );
#ifdef __SIMGPRS__DEBUG__
					Serial.print( "ServerSocket: Client Connected: " ); Serial.println( AIndex );
#endif
				}

				return true;
			}

			else if( Func::ContainsTextAtPos( ALine, 1, ", CLOSE OK" ) || Func::ContainsTextAtPos( ALine, 1, ", CLOSED" ))
			{
				int AIndex = ALine[ 0 ] - '0';
				if( ( AIndex >= 0 ) && ( AIndex <= 7 ))
				{
					if( C_OWNER.RemoveServerConnection( this, AIndex ))
					{
#ifdef __SIMGPRS__DEBUG__
						Serial.print( "ServerSocket: Client Close: " ); Serial.println( AIndex );
#endif
						return true;
					}
				}
			}

			else if( ALine.startsWith( "+RECEIVE," ))
			{
				int AIndex = ALine[ 9 ] - '0';
				if( ( AIndex >= 0 ) && ( AIndex <= 7 ))
				{
					if( C_OWNER.IsServerConnection( this, AIndex ))
					{
						FClientCurrentID = AIndex;
						ALine.remove( 0, 11 );
						FReadSize = ALine.toInt();
#ifdef __SIMGPRS__DEBUG__
						Serial.println( "SERVER RECEIVE " + Mitov::String( AIndex ) + " -> " + FReadSize );
#endif
						if( FReadSize )
						{
							FReadCounter = FReadSize;
							if( FBuffer )
								delete [] FBuffer;

							FBuffer = new uint8_t[ FReadSize ];
							FBufferPtr = FBuffer;
							ALockInput = true;
							return true;
						}
					}
				}
			}

			return false; 
		}

	protected:
		void OpenedResult( bool AResult )
		{
			if( AResult )
			{
				FInOpening() = true;
#ifdef __SIMGPRS__DEBUG__
				Serial.println( "ServerSocketConnected" );
#endif
			}
		}

	public:
		inline void SystemStart()
		{
			T_ListeningOutputPin::SetPinValue( false, false );
		}

	protected:
		virtual void StartSocket() override
		{
			if( inherited::FIsConnected() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			inherited::FIsConnected() = true;

#ifdef __SIMGPRS__DEBUG__
				Serial.println( "SERVER::StartSocket" );
#endif
//			if( C_OWNER.AssignConnectionID( this ) )
//			if( C_OWNER.AssignServerID( this ) )
//			{
////				C_OWNER.SendData( "AT+CIPSERVER=1," + Mitov::String( Port ), 2000 );
				C_OWNER.GetOwner().SendQueryRegisterResponse( new TMitovGSMSerialFunctionExpectOk( C_OWNER.GetOwner(), MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSTCPServerSocket::OpenedResult, bool ), "AT+CIPSERVER=1," + Mitov::String( inherited::Port() )), "AT+CIPSERVER=1," + Mitov::String( inherited::Port() ), true, 5 );
//			}

		}

/*
		virtual void DataReceived( int connectionId, unsigned char AData )
		{
#ifdef __SIMGPRS__DEBUG__
				Serial.print( "RECEIVED: " );
				Serial.println( connectionId );
#endif
			FClientCurrentID = connectionId;
			OutputPin.Notify( &AData );
//			FClientCurrentID = -1;
		}
*/
	public:
		inline bool IsConnected() { return inherited::FRunning().GetValue(); }

		virtual void EndPacket()
		{
			if( inherited::FStringPrint.Value.size() == 0 )
				return;

			if( FClientCurrentID >= 0 )
			{
				C_OWNER.GetOwner().SendQueryRegisterResponse( new TSIMComGPRSTCPSocketSendPacket<SELF>( this, inherited::FStringPrint ), "AT+CIPSEND=" + Mitov::String( FClientCurrentID ) +"," + Mitov::String( inherited::FStringPrint.Value.size()), true, 5, 60000 );
//				if( C_OWNER.SendData( "AT+CIPSEND=" + Mitov::String( FClientCurrentID ) + "," + Mitov::String( FStringPrint.Value.size()), "\r\n> ", 2000 ))
//					C_OWNER.SendDirect( (uint8_t *)FStringPrint.Value, FStringPrint.Value.size() );

//				Serial.println( FStringPrint.Value );
			}

			else
			{
				for( int i = 0; i < 7; ++i )
					if( C_OWNER.IsServerConnection( this, i ))
						C_OWNER.GetOwner().SendQueryRegisterResponse( new TSIMComGPRSTCPSocketSendPacket<SELF>( this, inherited::FStringPrint ), "AT+CIPSEND=" + Mitov::String( i ) +"," + Mitov::String( inherited::FStringPrint.Value.size()), true, 5, 60000 );
			}

//			FStringPrint.Value.clear();
		}

	public:
		virtual void StopSocket()
		{
			FInOpening() = false;
			if( inherited::FRunning() )
			{
				T_ListeningOutputPin::SetPinValue( false, true );
//				C_OWNER.SendData( "AT+CIPSERVER=0," + Mitov::String( Port ), 2000 );
				C_OWNER.GetOwner().SendQueryRegisterResponse( new TMitovGSMSerialFunctionExpectOk( C_OWNER.GetOwner(), MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSTCPServerSocket::OpenedResult, bool ), "AT+CIPSERVER=0"), "AT+CIPSERVER=0", true, 5 );
				C_OWNER.ReleaseServerID( this );
				inherited::FRunning() = false;
			}
		}

	public:
		inline SIMComGPRSTCPServerSocket()
		{
			FInOpening() = false;
		}


	};
//---------------------------------------------------------------------------
	template <typename T_MODULE, T_MODULE &C_OWNER, 
		typename T_ConnectedOutputPin, 
		typename T_Enabled,
		typename T_FConnectingRunning,
		typename T_FIsConnected,
		typename T_FRunning,
		typename T_Host,
		typename T_OutputPin,
		typename T_Port
	> class SIMComGPRSTCPClientSocket : 
		public BasicSIMComGPRSSocket<
			T_MODULE, C_OWNER,
			T_Enabled,
			T_FIsConnected,
			T_FRunning,
			T_Port
			>, 
		public T_FConnectingRunning,
		public T_Host,
		public T_ConnectedOutputPin, 
		public T_OutputPin
	{
		typedef BasicSIMComGPRSSocket<
				T_MODULE, 
				C_OWNER,
				T_Enabled,
				T_FIsConnected,
				T_FRunning,
				T_Port
			> inherited;

		typedef SIMComGPRSTCPClientSocket<
				T_MODULE, 
				C_OWNER, 
				T_ConnectedOutputPin, 
				T_Enabled,
				T_FConnectingRunning,
				T_FIsConnected,
				T_FRunning,
				T_Host,
				T_OutputPin,
				T_Port
			> SELF;

	public:
		typedef T_MODULE TOwner;

		inline T_MODULE &GetOwner() { return C_OWNER; }

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Host )
		int				FID;

	protected:
		int		FReadSize;
		int		FReadCounter;
		uint8_t	*FBuffer = nullptr;
		uint8_t	*FBufferPtr;
		_V_PROP_( FConnectingRunning )

	public:
		virtual bool ProcessCharacter( char AChar, bool &ASuccess ) override
		{ 
			if( FReadCounter )
			{
				*FBufferPtr++ = *(uint8_t*)&AChar;
				--FReadCounter;
//				Serial.println( FReadSize );
//				Serial.println( AChar );
//				OutputPin.SendValue( Mitov::TDataBlock( 1, &AChar ));
				if( !FReadCounter )
				{
					ASuccess = true;
					T_OutputPin::SendPinValue( Mitov::TDataBlock( FReadSize, FBuffer ));
//					Serial.println( (char *)FBuffer );
					delete [] FBuffer;
					FBuffer = nullptr;
				}

				return true;
			}

			return false; 
		}

		virtual bool TryProcessInput( Mitov::String ALine, bool &ALockInput ) override
		{ 
			if( ! FConnectingRunning() )
				return false;

			ALine.trim();
			if( ALine == Mitov::String( FID ) + ", CONNECT OK" )
			{
#ifdef __SIMGPRS__DEBUG__
				Serial.println( "CLIENT::CONNECT OK " + Mitov::String( FID ) );
#endif
				inherited::FRunning() = true;
				T_ConnectedOutputPin::SetPinValue( true, true );
				return true;
			}

			if( ALine == Mitov::String( FID ) + ", CLOSED" )
			{
				FConnectingRunning() = false;
#ifdef __SIMGPRS__DEBUG__
				Serial.println( "CLIENT::CLOSED " + Mitov::String( FID ) );
#endif
				StopSocket();
				return true;
			}

			if( ALine == Mitov::String( FID ) + ", CLOSE OK" )
			{
				FConnectingRunning() = false;
#ifdef __SIMGPRS__DEBUG__
				Serial.println( "CLIENT::CLOSE OK " + Mitov::String( FID ) );
#endif
				StopSocket();
				return true;
			}

			if( ALine == Mitov::String( FID ) + ", SEND OK" )
			{
#ifdef __SIMGPRS__DEBUG__
				Serial.println( "CLIENT::SEND OK " + Mitov::String( FID ) );
#endif
				return true;
			}

			if( ALine.startsWith( "+RECEIVE," + Mitov::String( FID ) + "," ))
			{
				ALine.remove( 0, 11 );
				FReadSize = ALine.toInt();
#ifdef __SIMGPRS__DEBUG__
				Serial.println( "RECEIVE " + Mitov::String( FID ) + " -> " + FReadSize );
#endif
				if( FReadSize )
				{
					FReadCounter = FReadSize;
					if( FBuffer )
						delete [] FBuffer;

					FBuffer = new uint8_t[ FReadSize ];
					FBufferPtr = FBuffer;
					ALockInput = true;
				}

				return true;
			}

			return false; 
		}

	protected:
		void OpenedResult( bool AResult )
		{
#ifdef __SIMGPRS__DEBUG__
			if( AResult )
				Serial.println( "SocketConnected" );
#endif
		}

	public:
		inline void SystemStart()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		virtual void StartSocket() override
		{
			if( inherited::FIsConnected() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			inherited::FIsConnected() = true;

#ifdef __SIMGPRS__DEBUG__
				Serial.println( "CLIENT::StartSocket" );
#endif
			if( C_OWNER.AssignConnectionID( this, FID ) )
			{
#ifdef __SIMGPRS__DEBUG__
				Serial.print( "CLIENT::StartSocket ID: " ); Serial.println( FID );
#endif
				FConnectingRunning() = true;
				C_OWNER.GetOwner().SendQueryRegisterResponse( new TMitovGSMSerialFunctionStatus( C_OWNER.GetOwner(), MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSTCPClientSocket::OpenedResult, bool )), "AT+CIPSTART=" + Mitov::String( FID ) +",\"TCP\",\"" + Host() + "\"," + Mitov::String( inherited::Port() ), true, 5 );
//				C_OWNER.SendData( "AT+CIPSTART=" + Mitov::String( FID ) +",\"TCP\",\"" + Host + "\"," + Mitov::String( Port ), 2000 );
			}

//			Serial.println( "StartSocket" );
		}

		virtual void StopSocket() override
		{
//			FConnectingRunning() = false;
			if( inherited::FRunning() )
			{
#ifdef __SIMGPRS__DEBUG__
				Serial.print( "CLIENT::CloseSocket ID: " ); Serial.println( FID );
#endif
				T_ConnectedOutputPin::SetPinValue( false, true );
				C_OWNER.ReleaseConnectionID( FID );
				C_OWNER.GetOwner().TrySendQuery( "AT+CIPCLOSE=" + Mitov::String( FID ), nullptr );
//				C_OWNER.SendData( "AT+CIPCLOSE=" + Mitov::String( FID ), 2000 );
				inherited::FRunning() = false;
			}
//			FClient.stop();
		}

/*
		virtual void DataReceived( int connectionId, unsigned char AData )
		{
			OutputPin.Notify( &AData );
		}
*/

	public:
		virtual void EndPacket()
		{
			if( inherited::FStringPrint.Value.size() == 0 )
				return;

			C_OWNER.GetOwner().SendQueryRegisterResponse( new TSIMComGPRSTCPSocketSendPacket<SELF>( this, inherited::FStringPrint ), "AT+CIPSEND=" + Mitov::String( FID ) +"," + Mitov::String( inherited::FStringPrint.Value.size()), true, 5, 60000 );

//			if( C_OWNER.SendData( "AT+CIPSEND=" + Mitov::String( FID ) + "," + Mitov::String( inherited::FStringPrint.Value.size()), "\r\n> ", 2000 ))
//				C_OWNER.SendDirect( (uint8_t *)inherited::FStringPrint.Value, inherited::FStringPrint.Value.size() );

//			inherited::FStringPrint.Value.clear();
		}

	public:
		inline SIMComGPRSTCPClientSocket()
		{
			FConnectingRunning() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_MODULE, T_MODULE &C_OWNER,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FConnectingRunning,
		typename T_FIsConnected,
		typename T_FRunning,
		typename T_Host,
		typename T_OutputPin,
		typename T_Port
	> class SIMComGPRSUDPClientSocket : 
		public SIMComGPRSTCPClientSocket<
				T_MODULE, C_OWNER, 
				T_ConnectedOutputPin, 
				T_Enabled,
				T_FConnectingRunning,
				T_FIsConnected,
				T_FRunning,
				T_Host,
				T_OutputPin,
				T_Port
			>
	{
		typedef SIMComGPRSTCPClientSocket<
				T_MODULE, C_OWNER, 
				T_ConnectedOutputPin, 
				T_Enabled,
				T_FConnectingRunning,
				T_FIsConnected,
				T_FRunning,
				T_Host,
				T_OutputPin,
				T_Port
			> inherited;

	protected:
		virtual void StartSocket() override
		{
			if( inherited::FIsConnected() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			inherited::FIsConnected() = true;

#ifdef __SIMGPRS__DEBUG__
			Serial.println( "UDP CLIENT::StartSocket" );
#endif
			if( C_OWNER.AssignConnectionID( this, inherited::FID ) )
			{
#ifdef __SIMGPRS__DEBUG__
				Serial.print( "UDP CLIENT::StartSocket ID: " ); Serial.println( inherited::FID );
#endif
				inherited::FConnectingRunning() = true;
				C_OWNER.GetOwner().SendQueryRegisterResponse( new TMitovGSMSerialFunctionStatus( C_OWNER.GetOwner(), MAKE_CALLBACK_3( OpenWire::Object, TOnMitovGSMSerialFunctionResult, SIMComGPRSUDPClientSocket::OpenedResult, bool )), "AT+CIPSTART=" + Mitov::String( inherited::FID ) +",\"UDP\",\"" + inherited::Host() + "\"," + Mitov::String( inherited::Port() ), true, 5 );
//				C_OWNER.SendData( "AT+CIPSTART=" + Mitov::String( FID ) +",\"TCP\",\"" + Host + "\"," + Mitov::String( Port ), 2000 );
			}

//			Serial.println( "StartSocket" );
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif