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

//#define __SMTP__DEBUG__

namespace Mitov
{
	class SMTPEMailBasic;
	class SMTPClientBasic;
	class SMTPEMailAccountBasic;
	class SMTPEMailBasicFunction;
//---------------------------------------------------------------------------
	namespace SMTPEMail_
	{
		struct TQueryElement
		{
		public:
			Mitov::String Command;
			SMTPEMailBasicFunction *ResponseElement;

		public:
			TQueryElement() :
				ResponseElement( nullptr )
			{
			}

			TQueryElement( Mitov::String ACommand, SMTPEMailBasicFunction *AResponseElement ) :
				Command( ACommand ),
				ResponseElement( AResponseElement )
			{
			}

		};
//---------------------------------------------------------------------------
		struct TEMailTransaction
		{
		public:
			uint8_t		FRefCount = 0;

		public:
			SMTPEMailAccountBasic	*SenderAccount;
			SMTPEMailBasic	*Mail;

			Mitov::String ClientID;
			Mitov::String UserName;
			Mitov::String Password;
			Mitov::String FromAddress;

		public:
			void Acquire()
			{
				++FRefCount;
			}

			void Release()
			{
				--FRefCount;
				if( ! FRefCount )
					delete this;
			}

		public:
			TEMailTransaction( SMTPEMailBasic *AMail, Mitov::String AFromAddress ) :
				Mail( AMail ),
				FromAddress( AFromAddress )
			{
			}
		};
	}
//---------------------------------------------------------------------------
	class SMTPClientBasic
	{
	public:
		virtual void SendTextLineShared( bool ANoDot, Mitov::String AText ) = 0;
		virtual void TrySendQuery( SMTPEMail_::TQueryElement *AElement ) = 0;

	public:
		inline void SendTextLine( Mitov::String AText )
		{
			SendTextLineShared( false, AText );
		}

		inline void SendLinesNoDot( Mitov::String AText )
		{
			SendTextLineShared( true, AText );
		}

	};
//---------------------------------------------------------------------------
	class SMTPEMailBasic
	{
	public:
		virtual void SendToOperation( SMTPClientBasic *AClient, SMTPEMail_::TEMailTransaction * ATransaction, uint8_t AOperation ) = 0;

	};
//---------------------------------------------------------------------------
	class SMTPEMailAccountBasic
	{
	public:
		virtual uint32_t GetTimeout() = 0;
/*
	public:
		Mitov::String	ClientID;
		Mitov::String	UserName;
		Mitov::String	Password;
		uint16_t	Timeout = 5000;
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClientID,
		typename T_Password,
		typename T_Timeout,
		typename T_UserName
	> class SMTPEMailAccount : public SMTPEMailAccountBasic,
		public T_ClientID,
		public T_Password,
		public T_Timeout,
		public T_UserName
	{
	public:
		_V_PROP_( ClientID )
		_V_PROP_( UserName )
		_V_PROP_( Password )
		_V_PROP_( Timeout )

	public:
		virtual uint32_t GetTimeout() { return Timeout(); }

	public:
		inline void MessageInputPin_o_Receive( void *_Data )
		{
			SMTPEMail_::TEMailTransaction *ATransaction = (SMTPEMail_::TEMailTransaction *)_Data;
	//		ATransaction->Acquire();
			ATransaction->SenderAccount = this;

			ATransaction->ClientID = ClientID();
			ATransaction->UserName = UserName();
			ATransaction->Password = Password();


			C_OWNER.ProcessTransaction( ATransaction );
		}
	};
//---------------------------------------------------------------------------
	class SMTPEMailBasicFunction
	{
	public:
		bool	Disconnect : 1;
		uint8_t RetryCount : 4;
		uint32_t Timeout;
		uint32_t Delay;
		SMTPEMail_::TEMailTransaction *Transaction;

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &AResponseCompleted, bool &ASuccess ) { return false; }
		virtual void TimedOut() {}

	public:
		SMTPEMailBasicFunction( SMTPEMail_::TEMailTransaction *ATransaction, bool ADisconnect, uint8_t ARetryCount, uint32_t ATimeout, uint32_t ADelay ) :
			Disconnect( ADisconnect ),
			RetryCount( ARetryCount ),
			Timeout( ATimeout ),
			Delay( ADelay ),
			Transaction( ATransaction )
		{
		}

		virtual ~SMTPEMailBasicFunction() {}

	};
//---------------------------------------------------------------------------
/*
	class SMTPEMailDisconnectFunction : public SMTPEMailBasicFunction
	{
		typedef SMTPEMailBasicFunction inherited;

	public:
		SMTPEMailDisconnectFunction() :
			inherited( true, 0, 2000, 0 )
		{
		}
	}
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER
	> class SMTPEMailConnectFunction : public SMTPEMailBasicFunction
	{
		typedef SMTPEMailBasicFunction inherited;

	protected:
		T_OWNER *FOwner;

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &AResponseCompleted, bool &ASuccess ) override
		{ 
			if( ALine.startsWith( "220 " ) )
			{
#ifdef __SMTP__DEBUG__
				Serial.println( "STARTED" );
#endif
				AResponseCompleted = true;
				ASuccess = true;
				return true;
			}

			return false; 
		}

		virtual void TimedOut() override
		{
			FOwner->Disconnect();
		}

	public:
		SMTPEMailConnectFunction( T_OWNER *AOwner, SMTPEMail_::TEMailTransaction *ATransaction ) :
			inherited( ATransaction, false, 5, 2000, 0 ),
			FOwner( AOwner )
		{
		}
	};
//---------------------------------------------------------------------------
	class SMTPEMailSendWaitResponse : public SMTPEMailBasicFunction
	{
		typedef SMTPEMailBasicFunction inherited;

	protected:
		int FResponse;

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &AResponseCompleted, bool &ASuccess ) override
		{ 
//			if( ALine.startsWith( "250 " ) )
			if( ALine.startsWith( Mitov::String( FResponse ) + " " ) )
			{
#ifdef __SMTP__DEBUG__
				Serial.print( "RES: " );
				Serial.println( FResponse );
#endif
				AResponseCompleted = true;
				ASuccess = true;
				return true;
			}

			return false; 
		}

	public:
		SMTPEMailSendWaitResponse( SMTPEMail_::TEMailTransaction *ATransaction, int AResponse ) :
			inherited( ATransaction, false, 5, 2000, 0 ),
			FResponse( AResponse )
		{
		}

		SMTPEMailSendWaitResponse( SMTPEMail_::TEMailTransaction *ATransaction, int AResponse, int ACountReply ) :
			inherited( ATransaction, false, ACountReply, 2000, 0 ),
			FResponse( AResponse )
		{
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER
	> class SMTPEMailSendDataResponse : public SMTPEMailBasicFunction
	{
		typedef SMTPEMailBasicFunction inherited;

	protected:
		T_OWNER *FOwner;

	public:
		virtual bool TryProcessRequestedInput( Mitov::String ALine, bool &AResponseCompleted, bool &ASuccess ) override
		{ 
//				if( ALine.startsWith( "250 " ) )
			if( ALine.startsWith( "354 " ) )
			{
#ifdef __SMTP__DEBUG__
				Serial.println( "DATA!" );
#endif
				FOwner->SendData( Transaction );
				AResponseCompleted = true;
				ASuccess = true;
				return true;
			}

			return false; 
		}

	public:
		SMTPEMailSendDataResponse( T_OWNER *AOwner, SMTPEMail_::TEMailTransaction *ATransaction ) :
			inherited( ATransaction, false, 5, 2000, 0 ),
			FOwner( AOwner )
		{
			Transaction->Acquire();
		}

		virtual ~SMTPEMailSendDataResponse()
		{
			Transaction->Release();
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_OWNER, T_OWNER C_OWNER,
		typename T_Address,
		typename T_Name
	> class TArduinoSMTPEMailAddress :
		public T_Address,
		public T_Name
	{
	public:
		_V_PROP_( Name )
		_V_PROP_( Address )

	public:
		inline Mitov::String GetText()
		{
			if( Name() == "" )
				return "<" + Address().GetValue() + ">";

			return Name().GetValue() + " <" + Address().GetValue() + ">";
		}

	public:
		inline void ProcessTransaction( SMTPClientBasic *AClient, SMTPEMail_::TEMailTransaction *ATransaction )
		{
			AClient->TrySendQuery( new SMTPEMail_::TQueryElement( "RCPT TO: <" + Address().GetValue() + ">", new SMTPEMailSendWaitResponse( ATransaction, 250 )));
		}

		inline void PrepareAddressList( Mitov::String &ALine )
		{
			if( ALine != "" )
				ALine += ", ";

			ALine += GetText();
		}
	};
//---------------------------------------------------------------------------	
	class SMTPEMailMessageBasic
	{
	public:
		virtual void SendLines( SMTPClientBasic *AClient ) = 0;

	};
//---------------------------------------------------------------------------	
	template <
		typename T_Text
	> class SMTPEMailMessageText : public SMTPEMailMessageBasic,
		public T_Text
	{
	public:
		_V_PROP_( Text )

	public:
		virtual void SendLines( SMTPClientBasic *AClient ) override
		{
			AClient->SendTextLine( "Content-Type: text/plain; charset=\"UTF-8\"" );
			AClient->SendTextLine( "Content-Transfer-Encoding: 7bit" );
			AClient->SendTextLine( "" );
			AClient->SendLinesNoDot( Text() );
		}

	};
//---------------------------------------------------------------------------	
	template <
		typename T_Text
	> class SMTPEMailMessageHTML : public SMTPEMailMessageBasic,
		public T_Text
	{
	public:
		_V_PROP_( Text )

	public:
		virtual void SendLines( SMTPClientBasic *AClient ) override
		{
			AClient->SendTextLine( "Content-Type: text/html; charset=\"UTF-8\"" );
			AClient->SendTextLine( "Content-Transfer-Encoding: 7bit" );
			AClient->SendTextLine( "" );
			AClient->SendLinesNoDot( Text() );
		}

	};
//---------------------------------------------------------------------------	
	template <
		typename T_Boundary,
		typename T_PlainText,
		typename T_HTMLText
	> class SMTPEMailMessageTextTHML : public SMTPEMailMessageBasic,
		public T_Boundary,
		public T_PlainText,
		public T_HTMLText
	{
	public:
		_V_PROP_( Boundary )
		_V_PROP_( PlainText )
		_V_PROP_( HTMLText )

	public:
		virtual void SendLines( SMTPClientBasic *AClient ) override
		{
			AClient->SendTextLine( "Content-Type: multipart/alternative; boundary=" + Boundary().GetValue() );
			AClient->SendTextLine( "\r\n--" + Boundary().GetValue() );
			AClient->SendTextLine( "Content-Type: text/plain; charset=\"UTF-8\"" );
			AClient->SendTextLine( "Content-Transfer-Encoding: 7bit" );
			AClient->SendTextLine( "" );
			AClient->SendLinesNoDot( PlainText().GetValue() );

			AClient->SendTextLine( "--" + Boundary().GetValue() );
			AClient->SendTextLine( "Content-Type: text/html; charset=\"UTF-8\"" );
			AClient->SendTextLine( "Content-Transfer-Encoding: 7bit" );
			AClient->SendTextLine( "" );
			AClient->SendLinesNoDot( HTMLText().GetValue() );
			AClient->SendTextLine( "--" + Boundary().GetValue() + "--" );

		}

	};
//---------------------------------------------------------------------------	
	template <
		typename T_BCC_PrepareAddressList,
		typename T_BCC_ProcessTransaction,
		typename T_CC_PrepareAddressList,
		typename T_CC_ProcessTransaction,
		uint32_t COUNT_BCC,
		uint32_t COUNT_CC,
		uint32_t COUNT_To,
		typename T_From,
		typename T_Message,
		typename T_MessageOutputPin,
		typename T_Subject,
		typename T_To_PrepareAddressList,
		typename T_To_ProcessTransaction
	> class SMTPEMail : public SMTPEMailBasic,
		public T_BCC_ProcessTransaction,
		public T_CC_ProcessTransaction,
		public T_From,
		public T_MessageOutputPin,
		public T_Message,
		public T_Subject,
		public T_To_ProcessTransaction
	{
	public:
		_V_PIN_( MessageOutputPin )

	public:
		_V_PROP_( From )
//		Mitov::SimpleList<TArduinoSMTPEMailAddress *>	To;
//		Mitov::SimpleList<TArduinoSMTPEMailAddress *>	CC;
//		Mitov::SimpleList<TArduinoSMTPEMailAddress *>	BCC;
		_V_PROP_( Subject )
		_V_PROP_( Message )

	public:
		virtual void SendToOperation( SMTPClientBasic *AClient, SMTPEMail_::TEMailTransaction * ATransaction, uint8_t AOperation ) override
		{
			if( AOperation == 0 )
			{
				T_To_ProcessTransaction::Call( AClient, ATransaction );
				T_CC_ProcessTransaction::Call( AClient, ATransaction );
				T_BCC_ProcessTransaction::Call( AClient, ATransaction );
			}

			else
				SendLines( AClient, ATransaction );
		}

	public:
		inline void SendLines( SMTPClientBasic *AClient, SMTPEMail_::TEMailTransaction * ATransaction )
		{
			AClient->SendTextLine( "From: " + From().GetText() );
//			AClient->SendTextLineShared( false, "From: " + From.GetText() );

			{ // AToLine Scope
				Mitov::String AToLine;
				T_To_PrepareAddressList::Call( AToLine );
/*
				for( int i = 0; i < To.size(); ++i )
				{
					if( i )
						AToLine += ", ";

					AToLine += To[ i ]->GetText();
				}
*/
				AClient->SendTextLine( "To: " + AToLine );

				if( COUNT_CC )
				{
					AToLine = "";
					T_CC_PrepareAddressList::Call( AToLine );
				}

				if( COUNT_BCC )
				{
					AToLine = "";
					T_BCC_PrepareAddressList::Call( AToLine );
				}
/*
				if( CC.size() )
				{
					AToLine = "";
					for( int i = 0; i < CC.size(); ++i )
					{
						if( i )
							AToLine += ", ";

						AToLine += CC[ i ]->GetText();
					}

					AClient->SendTextLine( "CC: " + AToLine );
				}

				if( BCC.size() )
				{
					AToLine = "";
					for( int i = 0; i < BCC.size(); ++i )
					{
						if( i )
							AToLine += ", ";

						AToLine += BCC[ i ]->GetText();
					}

					AClient->SendTextLine( "BCC: " + AToLine );
				}
*/
			}

			AClient->SendTextLine( "Subject: " + Subject().GetValue() );
			AClient->SendTextLine( "Mime-Version: 1.0" );
			Message().SendLines( AClient );
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ( COUNT_To == 0 ) && ( COUNT_CC == 0 ) && ( COUNT_BCC == 0 ) )
				return;

			SMTPEMail_::TEMailTransaction *ATransaction = new SMTPEMail_::TEMailTransaction( this, From().Address() );

			ATransaction->Acquire();
			T_MessageOutputPin::Notify( ATransaction );
			ATransaction->Release();
		}

	};
//---------------------------------------------------------------------------	
	template <
		typename T_ConnectOutputPin,
		typename T_OutputPin
	> class SMTPClient : public SMTPClientBasic,
		public T_ConnectOutputPin,
		public T_OutputPin
	{
		typedef SMTPClient<
			T_ConnectOutputPin,
			T_OutputPin
		> SELF;

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectOutputPin )

	protected:
		char		FBuffer[ 256 ];
		uint8_t		FIndex = 0;

	protected:
		Mitov::SimpleList<SMTPEMail_::TQueryElement *>	FQueryQueue;
		SMTPEMail_::TQueryElement *FCurrentElement = nullptr;
		uint32_t	FDelay = 0;
		unsigned long FCurrentMillis = 0;

	protected:
		SMTPEMailAccountBasic	*FActiveAccount = nullptr;

	public:
		virtual void SendTextLineShared( bool ANoDot, Mitov::String AText ) override
		{
			if( ANoDot )
				SendLinesNoDot( AText );

			else
				SendTextLine( AText );

		}

		void SendTextLine( Mitov::String AText )
		{
#ifdef __SMTP__DEBUG__
			Serial.print( "SEND: " );
			Serial.println( AText );
#endif

			AText += "\r\n";
			Mitov::TDataBlock ABlock( AText.length(), AText.c_str() );
			T_OutputPin::SetPinValue( ABlock );

			FCurrentMillis = millis();
		}

		void SendTextLineNoDot( Mitov::String AText )
		{
			if( AText.startsWith( "." ))
				AText = "." + AText;

			SendTextLine( AText );
		}

		void SendLinesNoDot( Mitov::String ALines )
		{
			int AStart = 0;
			for( int i = 0; i < ALines.length(); ++i )
			{
				if( ALines[ i ] == '\n' )
				{
					if( AStart != i )
					{
						Mitov::String ALine = ALines.substring( AStart, i );
//						Serial.println( i - AStart );
						SendTextLineNoDot( ALine );
					}

					AStart = i + 1;
				}

				if( ALines[ i ] == '\r' )
				{
					Mitov::String ALine = ALines.substring( AStart, i );
//					Serial.println( i - AStart );
					SendTextLineNoDot( ALine );
					AStart = i + 1;
				}

				else if ( i == ALines.length() - 1 )
				{
					Mitov::String ALine = ALines.substring( AStart );
					SendTextLineNoDot( ALine );
				}
			}
		}

		inline void Disconnect()
		{
			T_ConnectOutputPin::SetPinValueLow();
		}

		void QuitConnection()
		{
			TrySendQuery( new SMTPEMail_::TQueryElement( "QUIT", new SMTPEMailSendWaitResponse( nullptr, 221 ) ));
			TrySendQuery( new SMTPEMail_::TQueryElement( "", new SMTPEMailBasicFunction( nullptr, true, 0, 0, 0 )));
			FActiveAccount = nullptr;
		}

		void ProcessTransaction( SMTPEMail_::TEMailTransaction *ATransaction )
		{
			if( FActiveAccount != ATransaction->SenderAccount )
			{
				if( FActiveAccount )
					QuitConnection();

				TrySendQuery( new SMTPEMail_::TQueryElement( "", new SMTPEMailConnectFunction<SELF>( this, ATransaction )));
//				ConnectOutputPin.SendValue( true );

				TrySendQuery( new SMTPEMail_::TQueryElement( "EHLO " + ATransaction->ClientID, new SMTPEMailSendWaitResponse( ATransaction, 250 )));
				if( ATransaction->UserName != "" )
				{
					TrySendQuery( new SMTPEMail_::TQueryElement( "AUTH LOGIN", new SMTPEMailSendWaitResponse( ATransaction, 334 )));
					TrySendQuery( new SMTPEMail_::TQueryElement( Func::Base64Encode( ATransaction->UserName ), new SMTPEMailSendWaitResponse( ATransaction, 334 )));
					TrySendQuery( new SMTPEMail_::TQueryElement( Func::Base64Encode( ATransaction->Password ), new SMTPEMailSendWaitResponse( ATransaction, 235 )));
					TrySendQuery( new SMTPEMail_::TQueryElement( "MAIL FROM: <" + ATransaction->FromAddress + ">", new SMTPEMailSendWaitResponse( ATransaction, 250 )));
				}
			}

			ATransaction->Mail->SendToOperation( this, ATransaction, 0 ); // Send Address
/*
			for( int i = 0; i < ATransaction->Mail->To.size(); ++i )
				TrySendQuery( new SMTPEMail_::TQueryElement( "RCPT TO: <" + ATransaction->Mail->To[ i ]->Address + ">", new SMTPEMailSendWaitResponse( ATransaction, 250 )));

			for( int i = 0; i < ATransaction->Mail->CC.size(); ++i )
				TrySendQuery( new SMTPEMail_::TQueryElement( "RCPT TO: <" + ATransaction->Mail->CC[ i ]->Address + ">", new SMTPEMailSendWaitResponse( ATransaction, 250 )));

			for( int i = 0; i < ATransaction->Mail->BCC.size(); ++i )
				TrySendQuery( new SMTPEMail_::TQueryElement( "RCPT TO: <" + ATransaction->Mail->BCC[ i ]->Address + ">", new SMTPEMailSendWaitResponse( ATransaction, 250 )));
*/
			TrySendQuery( new SMTPEMail_::TQueryElement( "DATA", new SMTPEMailSendDataResponse<SELF>( this, ATransaction )));
			FActiveAccount = ATransaction->SenderAccount;

//			TrySendQuery( new SMTPEMailSendMessage
		}

	public:
		void SendData( SMTPEMail_::TEMailTransaction *ATransaction )
		{
//			ATransaction->Mail->SendLines( this );
			ATransaction->Mail->SendToOperation( this, ATransaction, 1 ); // Send Lines

			// Send .
			FQueryQueue.push_front( new SMTPEMail_::TQueryElement( ".", new SMTPEMailSendWaitResponse( ATransaction, 250, 1 )) );
		}

	protected:
		void SendQuery( SMTPEMail_::TQueryElement *AElement )
		{
#ifdef __SMTP__DEBUG__
			Serial.print( "QUERY : \"" );	Serial.print( AElement->Command ); Serial.println( "\"" );
#endif
			if( AElement->Command == "" )
			{
				if( AElement->ResponseElement->Disconnect )
				{
					Disconnect();
					delete AElement;
				}

				else
				{
					FCurrentMillis = millis();
					T_ConnectOutputPin::SetPinValueHigh();
					FCurrentElement = AElement;
				}

				return;
			}

			FCurrentMillis = millis();

//			FStream.println( AQuery );
//			FStream.write( AElement->Command, AElement->Command.size() );
			SendTextLine( AElement->Command );

			FCurrentElement = AElement;
		}

		void ProcessNextCommand()
		{
			if( FCurrentElement )
			{
				FDelay = FCurrentElement->ResponseElement->Delay;
#ifdef __SMTP__DEBUG__
				Serial.print( "DELAY: " ); Serial.println( FDelay );
#endif
				delete FCurrentElement;
				FCurrentElement = nullptr;
			}

			if( FDelay )
				return;

#ifdef __SMTP__DEBUG__
			Serial.println( "ProcessNextCommand" );
#endif
//				Serial.print( "RESP_QUEUE: " ); Serial.println( FResponseHandlersQueue.size() );
			if( FQueryQueue.size() )
			{
//					Serial.print( "SEND_QUERY: " ); Serial.println( FQueryQueue.size() );
				SMTPEMail_::TQueryElement *AElement = FQueryQueue[ 0 ];
//					Serial.print( "ESTRACT_QUERY: " ); Serial.println( ACommand );
				FQueryQueue.pop_front();

#ifdef __SMTP__DEBUG__
				Serial.println( "QUEUE>>" );
				for( int i = 0; i < FQueryQueue.size(); ++i )
					Serial.println( FQueryQueue[ i ]->Command );

				Serial.println( "<<QUEUE" );
#endif

				SendQuery( AElement );
//				Serial.print( "SEND_QUERY: " ); Serial.println( FQueryQueue.size() );
			}
		}

	public:
		virtual void TrySendQuery( SMTPEMail_::TQueryElement *AElement ) override
		{
//			if( ( PowerOn || ( AQuery == "AT" ) ) && ( FResponseHandlersQueue.size() == 0 ) && ( !FInPowerSwitch ))
//			if( FResponseHandlersQueue.size() == 0 )
			if( ! FCurrentElement )
				SendQuery( AElement );

			else
			{
//				while( FQueryQueue.size() > 10 )
//					SystemLoopBegin( micros());

#ifdef __SMTP__DEBUG__
				Serial.print( "ADD TO QUERY : \"" );	Serial.print( AElement->Command ); Serial.println( "\"" );
#endif
				FQueryQueue.push_back( AElement );

#ifdef __SMTP__DEBUG__
				Serial.println( "QUEUE>>" );
				for( int i = 0; i < FQueryQueue.size(); ++i )
					Serial.println( FQueryQueue[ i ]->Command );

				Serial.println( "<<QUEUE" );
#endif
			}
		}

	protected:
		void DeleteTransactionElements( SMTPEMail_::TEMailTransaction *ATransaction )
		{
			if( ! ATransaction )
				return;

#ifdef __SMTP__DEBUG__
			Serial.print( "DELETE TRANSACTION: " );
			Serial.print( FQueryQueue.size() );
#endif
			for( int i = FQueryQueue.size(); i--; )
				if( FQueryQueue[ i ]->ResponseElement->Transaction == ATransaction )
					FQueryQueue.Delete( i );

#ifdef __SMTP__DEBUG__
			Serial.print( " -> " );
			Serial.println( FQueryQueue.size() );
#endif
		}

	public:
		void ConnectedInputPin_o_Receive( void *_Data )
		{
			if( ((bool *)_Data ))
				return;

			FActiveAccount = nullptr;
		}

		void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;
			if( ! ABlock.Size )
				return;

			for( int i = 0; i < ABlock.Size; ++i )
			{
				char AChar = ABlock.Data[ i ];
				if( AChar == 13 )
					return;

				if( AChar != 10 )
				{
					FBuffer[ FIndex ++ ] = AChar;
					if( FIndex < 255 )
						return;
				}

				FBuffer[ FIndex ] = '\0';
				FIndex = 0;

				Mitov::String AString = FBuffer;

#ifdef __SMTP__DEBUG__
				Serial.println( AString );
#endif

/*
				if( AString.startsWith( "220 " ) )
				{
					Serial.println( "STARTED" );
				}
*/
				bool	AResponseCompleted = false;
				if( FCurrentElement )
				{
					bool ASuccess = false;
					if( FCurrentElement->ResponseElement->TryProcessRequestedInput( AString, AResponseCompleted, ASuccess ))
					{
						if( AResponseCompleted )
						{

							if( ( ! ASuccess ) && FCurrentElement->Command && FCurrentElement->ResponseElement->RetryCount )
							{
#ifdef __SMTP__DEBUG__
								Serial.println( "RETRY2" );
								Serial.println( FCurrentElement->Command );
#endif
								-- FCurrentElement->ResponseElement->RetryCount;
								SendQuery( FCurrentElement );
							}

							else
							{
								if( ! ASuccess )
									DeleteTransactionElements( FCurrentElement->ResponseElement->Transaction );

#ifdef __SMTP__DEBUG__
								Serial.println( "Queue Delete 2" );
								Serial.print( "DELETING: " ); Serial.println( FCurrentElement->Command );
		//							Serial.print( "RESP_QUEUE: " ); Serial.println( FResponseHandlersQueue.size() );
#endif
		//							Serial.println( "ProcessNextCommand 2" );
								ProcessNextCommand();
							}
						}
					}
				}
			}
		}


	public:
		inline void SystemLoopBegin()
		{
			if( FCurrentElement )
			{
//				Serial.println( "test2" );
				unsigned long AMillis = millis();
				if( ( AMillis - FCurrentMillis ) > FCurrentElement->ResponseElement->Timeout )
				{
#ifdef __SMTP__DEBUG__
					Serial.print( "TIMEOUT: " ); Serial.println( FCurrentElement->Command );
					Serial.println( FCurrentElement->ResponseElement->Timeout );
					Serial.println( AMillis );
					Serial.println( FCurrentMillis );
#endif
//					FLockRequestedInputIndex = 0;

					if( FCurrentElement->Command && FCurrentElement->ResponseElement->RetryCount )
					{
#ifdef __SMTP__DEBUG__
						Serial.println( "RETRY3" );
						Serial.println( FCurrentElement->Command );
#endif
						-- FCurrentElement->ResponseElement->RetryCount;
						SendQuery( FCurrentElement );
					}

					else
					{			
						FCurrentElement->ResponseElement->TimedOut();
						DeleteTransactionElements( FCurrentElement->ResponseElement->Transaction );
	//					Serial.println( "ProcessNextCommand 3" );
						ProcessNextCommand();
					}

				}
			}

			else if( FActiveAccount )
			{

//				Serial.println( "test1" );
				unsigned long AMillis = millis();
				if( ( AMillis - FCurrentMillis ) > FActiveAccount->GetTimeout() )
				{
#ifdef __SMTP__DEBUG__
					Serial.println( "CLOSING!!!" );
#endif
					QuitConnection();
				}
			}

			if( FDelay )
			{
				unsigned long AMillis = millis();
				if( ( AMillis - FCurrentMillis ) > FDelay )
				{
#ifdef __SMTP__DEBUG__
					Serial.println( "DELAY COMPLETED" );
#endif
					FDelay = 0;
					ProcessNextCommand();
				}
			}
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