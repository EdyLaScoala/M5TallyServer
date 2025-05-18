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

// #define __TCP_HTTP_CLIENT__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ContentOutputPin,
		typename T_OutputPin,
		typename T_StatusCodeOutputPin,
		typename T_TimeoutOutputPin
	> class TArduinoNetworkProtocolsHTTPClientRequestResponse :
		public T_ContentOutputPin,
		public T_OutputPin,
		public T_StatusCodeOutputPin,
		public T_TimeoutOutputPin
	{
	public:
		_V_PIN_( ContentOutputPin )
		_V_PIN_( OutputPin )
		_V_PIN_( StatusCodeOutputPin )
		_V_PIN_( TimeoutOutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Name,
		typename T_Value
	> class MitovHTTPClientHaderCustomField :
		public T_Name,
		public T_Value
	{
	public:
		_V_PROP_( Name )
		_V_PROP_( Value )

	public:
		inline void AddFields( String & AText )
		{
			AText += Name().GetValue() + ": " + Value().GetValue() + "\r\n";
		}

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestGET
	{
	public:
		static String GetCommand() { return "GET"; }

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestPOST
	{
	public:
		static String GetCommand() { return "POST"; }

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestHEAD
	{
	public:
		static String GetCommand() { return "HEAD"; }

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestDELETE
	{
	public:
		static String GetCommand() { return "DELETE"; }

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestCONNECT
	{
	public:
		static String GetCommand() { return "CONNECT"; }

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestOPTIONS
	{
	public:
		static String GetCommand() { return "OPTIONS"; }

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestTRACE
	{
	public:
		static String GetCommand() { return "TRACE"; }

	};
//---------------------------------------------------------------------------
	class TArduinoNetworkProtocolsHTTPClientRequestPUT
	{
	public:
		static String GetCommand() { return "PUT"; }

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CacheControl,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Connection,
		typename T_Content,
		typename T_ContentLength,
		typename T_ContentType,
		typename T_FClocked,
//		typename T_FWaitingForReply,
		typename T_Fields_AddFields,
		typename T_IMPLEMENTATION,
		uint32_t C_INDEX,
		typename T_Response,
		typename T_Timeout,
		typename T_URL
	> class MitovHTTPClientRequest :
		public T_CacheControl,
		public T_ClockInputPin_o_IsConnected,
		public T_Connection,
		public T_Content,
		public T_ContentLength,
		public T_ContentType,
		public T_FClocked,
//		public T_FWaitingForReply,
		public T_Response,
		public T_Timeout,
		public T_URL
	{
	public:
		_V_PROP_( CacheControl )
		_V_PROP_( Connection )
		_V_PROP_( Content )
		_V_PROP_( ContentLength )
		_V_PROP_( ContentType )
		_V_PROP_( Response )
		_V_PROP_( Timeout )
		_V_PROP_( URL )

	protected:
		_V_PROP_( FClocked )

//		uint32_t FTestSounter = 0;
//		_V_PROP_( FWaitingForReply )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )


	public:
		void SetContent( Mitov::String &AOldValue, Mitov::String &AValue )
		{
			Content() = AValue;
			if( ClockInputPin_o_IsConnected() )
				return;

			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline void ProcessData( void *AData )
		{
			Response().OutputPin().PinNotify( AData );

			if( C_OWNER.FInProcessingContent() )
				Response().ContentOutputPin().PinNotify( AData );

/*
			else
			{
				++ FTestSounter;
				Serial.println( FTestSounter );
			}
*/
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)AData;
			uint8_t *ABytes = ADataBlock.Data;
			for( uint32_t i = 0; i < ADataBlock.Size; ++ i )
				ProcessByte( *ABytes ++ );

		}

	protected:
		inline void ProcessByte( uint8_t AByte )
		{
//			Serial.print( "+" );
			if( AByte == 10 )
			{
				C_OWNER.FDetectedNL() = true;
				C_OWNER.FCollectBuffer() = false;
			}

			else if( AByte == 13 )
			{
				C_OWNER.FDetectedCR() = true;
				C_OWNER.FCollectBuffer() = false;
			}

			if( C_OWNER.FCollectBuffer() )
			{
//				Serial.print( "." );
//				Serial.print( char( AByte ));
				if( AByte == ' ' )
				{
					if( C_OWNER.FDetectedCR() )
						C_OWNER.FDetectedNL() = true;

					else
						C_OWNER.FDetectedCR() = true;
				}

				else
				{
					if( C_OWNER.FDetectedNL() && C_OWNER.FDetectedCR() )
					{
						C_OWNER.FCollectBuffer() = false;
						Response().StatusCodeOutputPin().SetPinValue( C_OWNER.FStatusCode, false );
//						Response().GetValue().StatusCodeOutputPin().SetPinValue( C_OWNER.FStatusCode(), false );
//						ProcessBuffer();
						C_OWNER.FStatusCode = 0;
	//					C_OWNER.FBuffer.clear();
						C_OWNER.FDetectedNL() = false;
						C_OWNER.FDetectedCR() = false;
					}

					else if( C_OWNER.FDetectedCR() )
					{
						if( isdigit( char( AByte ) ) )
						{
							C_OWNER.FStatusCode *= 10;
							C_OWNER.FStatusCode += char( AByte ) - '0';
//							Serial.print( char( AByte ));
						}
					}
				}

//				else if( C_OWNER.FDetectedNL() )
//					C_OWNER.FBuffer.push_back( AByte );

			}

			else if( ! C_OWNER.FInProcessingContent().GetValue() )
			{
				if( C_OWNER.FDetectedNL() && C_OWNER.FDetectedCR() )
				{
//					Serial.println( "TEST0" );
					C_OWNER.FDetectedNL() = false;
					C_OWNER.FDetectedCR() = false;

					if( C_OWNER.FNewLineDetected() )
					{
//						Serial.println( "TEST1" );
						C_OWNER.FInProcessingContent() = true;
					}

					else
						C_OWNER.FNewLineDetected() = true;

				}

				else if( ! ( C_OWNER.FDetectedNL().GetValue() || C_OWNER.FDetectedCR().GetValue() ))
					C_OWNER.FNewLineDetected() = false;
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.GetReadyToConnect() )
			{
				FClocked() = true;
				return;
			}

			String AText = T_IMPLEMENTATION::GetCommand() + " " + URL().GetValue() + " " + C_OWNER.Version().GetValue() + "\r\nHost: " + C_OWNER.Host().GetValue() + "\r\n";
			if( ContentType().GetValue() != "" )
				AText += "Content-Type: " + ContentType().GetValue() + "\r\n";

			if( CacheControl().GetValue() != "" )
				AText += "cache-control: " + CacheControl().GetValue() + "\r\n";

			if( Connection().GetValue() != "" )
				AText += "Connection: " + Connection().GetValue() + "\r\n";

			T_Fields_AddFields::Call( AText );

			if( ContentLength() )
				AText += "Content-Length: " + String( Content().GetValue().length()) + "\r\n";

			AText += "\r\n" + Content().GetValue();

//			Serial.println( AText );
//			FTestSounter = 0;

			C_OWNER.FStatusCode = 0;
//			C_OWNER.FBuffer.clear();
			if( C_OWNER.SendPacket( AText ))
			{
//				Serial.println( "C_OWNER.SendPacket" );

				FClocked() = false;
				C_OWNER.FRequestIndex() = C_INDEX;
//				FWaitingForReply() = true;
				C_OWNER.FEndTime() = millis();
				C_OWNER.FCollectBuffer() = true;
				C_OWNER.FDetectedNL() = false;
				C_OWNER.FDetectedCR() = false;
				C_OWNER.FNewLineDetected() = false;
				C_OWNER.FInProcessingContent() = false;
			}

			else
				FClocked() = true;

//			Serial.println( AText );
		}

	public:
		inline void SystemLoopBegin()
		{
//            Serial.println( FClocked() );
			if( Timeout().GetValue() > 0 )
			{
				if( millis() - C_OWNER.FEndTime().GetValue() > Timeout().GetValue() )
				{
#ifdef __TCP_HTTP_CLIENT__DEBUG__
					Serial.println( "Disconnect....... 1" );
#endif
					C_OWNER.Disconnect();
					Response().TimeoutOutputPin().ClockPin();
				}
			}

//			if( ! FWaitingForReply().GetValue() )
			if( C_OWNER.FRequestIndex() != C_INDEX )
				if( ! FClocked().GetValue() )
					return;

			if( C_OWNER.GetIsConnected() )
			{
				if( FClocked() )
					ClockInputPin_o_Receive( nullptr );

				return;
			}

			if( FClocked() )
				ClockInputPin_o_Receive( nullptr );

			if( C_OWNER.FRequestIndex() == C_INDEX )
			{
				C_OWNER.ResetRequestIndex();
//				FWaitingForReply() = false;
//				ProcessResponse( C_OWNER.FBuffer.AsPointer(), C_OWNER.FBuffer.size() );
//				C_OWNER.FBuffer.clear();
			}

		}

//	public:
//		inline void ProcessReady( bool & AHandled )
//		{
//		}

	public:
		inline MitovHTTPClientRequest()
		{
			FClocked() = ! ClockInputPin_o_IsConnected().GetValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SOCKET, T_SOCKET &C_SOCKET,
		uint32_t C_COUNT_Requests,
		typename T_ConfigureSocket,
		typename T_FCollectBuffer,
		typename T_FDetectedCR,
		typename T_FDetectedNL,
		typename T_FEndTime,
		typename T_FInProcessingContent,
		typename T_FNewLineDetected,
		typename T_FRequestIndex,
//		typename T_FStatusLine,
		typename T_Host,
//		typename T_Requests_ProcessReady,
		typename T_Requests_ProcessData,
		typename T_UserAgent,
		typename T_Version
	> class MitovHTTPClient :
		public T_ConfigureSocket,
		public T_FDetectedCR,
		public T_FDetectedNL,
		public T_Version,
		public T_FCollectBuffer,
		public T_FEndTime,
		public T_FInProcessingContent,
		public T_FNewLineDetected,
		public T_FRequestIndex,
//		public T_FStatusLine,
		public T_Host,
		public T_UserAgent
	{
	public:
		_V_PROP_( ConfigureSocket )
		_V_PROP_( Host )
		_V_PROP_( UserAgent )
		_V_PROP_( Version )

	public:
//		_V_PROP_( FStatusLine )
		_V_PROP_( FCollectBuffer )
		_V_PROP_( FDetectedCR )
		_V_PROP_( FDetectedNL )
		_V_PROP_( FEndTime )
		_V_PROP_( FInProcessingContent )
		_V_PROP_( FNewLineDetected )
		_V_PROP_( FRequestIndex )

	public:
		uint16_t	FStatusCode = 0;

	public:
		bool SendPacket( String &AText )
		{
//            Serial.println( "SendPacket" );
#ifdef __TCP_HTTP_CLIENT__DEBUG__
			Serial.println( "SendPacket 0 :" );
			Serial.println( C_SOCKET.Enabled().GetValue() );
#endif
			if( C_SOCKET.CanSend() )
			{
//				Serial.println( "C_SOCKET.CanSend" );
#ifdef __TCP_HTTP_CLIENT__DEBUG__
				Serial.println( "C_SOCKET.CanSend" );
#endif
				C_SOCKET.BeginPacket();
				C_SOCKET.GetPrint().write( ( uint8_t *) AText.c_str(), AText.length() );
				C_SOCKET.EndPacket();
				return true;
			}

			if( ConfigureSocket() )
			{
				if( Host().GetValue() != "" )
					C_SOCKET.Host() = Host().GetValue();

				if( C_SOCKET.GetIsSecure() )
					C_SOCKET.Port() = 443;

				else
					C_SOCKET.Port() = 80;
			}

#ifdef __TCP_HTTP_CLIENT__DEBUG__
			Serial.println( "SendPacket 1 :" );
			Serial.println( C_SOCKET.Enabled().GetValue() );
#endif

			if( ! C_SOCKET.CanSend() ) //C_SOCKET.Enabled().GetValue() )
			{
//    			Serial.println( "C_SOCKET.CanSend" );

//				if( ! C_SOCKET.Enabled().GetValue() )
				{
//					Serial.println( "C_SOCKET.Enabled() = true" );
#ifdef __TCP_HTTP_CLIENT__DEBUG__
					Serial.println( "C_SOCKET.Enabled() = true" );
#endif
					C_SOCKET.ForceStart();
//					C_SOCKET.Enabled() = true;
//					C_SOCKET.TryStartSocket();
					FEndTime() = millis();
				}
			}

			return false;
		}

		inline void Disconnect()
		{
//			Serial.println( "Disconnect......." );
			C_SOCKET.StopSocket();
		}

		inline bool GetIsConnected()
		{
			return C_SOCKET.CanSend();
		}

		inline bool GetReadyToConnect()
		{
			return C_SOCKET.GetReadyToConnect();
		}

		inline void ResetRequestIndex()
		{
			FRequestIndex() = C_COUNT_Requests;
		}

	public:
		inline void _DirectPinReceive( void *_Data )
		{
//			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;

//			Serial.print( "o" );

			T_Requests_ProcessData::Call( FRequestIndex(), _Data );

//			Serial.print( *(char *)ADataBlock.Data );

//			Serial.println();
//			Serial.println();
//			Serial.println( ADataBlock.Size );
//			Serial.println();

//			FBuffer.append( ADataBlock.Data, ADataBlock.Size );
		}

	public:
		inline MitovHTTPClient()
		{
			FDetectedCR() = false;
			FDetectedNL() = false;
			FCollectBuffer() = false;
			FNewLineDetected() = false;
			FInProcessingContent() = false;
			ResetRequestIndex();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
