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
#include <Mitov_StandardSerial.h>

namespace Mitov
{
	const	long CTeensySerialInits[] = 
	{
		SERIAL_7E1,
		SERIAL_7E1,
		SERIAL_7O1,
		SERIAL_8N1,
		SERIAL_8E1,
		SERIAL_8O1,

		SERIAL_8N2,
		SERIAL_8N2,
		SERIAL_8N2,
		SERIAL_8N2,
		SERIAL_8N2,
		SERIAL_8N2
/*
		SERIAL_7E1_RXINV,
		SERIAL_7E1_RXINV,
		SERIAL_7O1_RXINV,
		SERIAL_8N1_RXINV,
		SERIAL_8E1_RXINV,
		SERIAL_8O1_RXINV,

		SERIAL_8N2_RXINV,
		SERIAL_8N2_RXINV,
		SERIAL_8N2_RXINV,
		SERIAL_8N2_RXINV,
		SERIAL_8N2_RXINV,
		SERIAL_8N2_RXINV,

		SERIAL_7E1_TXINV,
		SERIAL_7E1_TXINV,
		SERIAL_7O1_TXINV,
		SERIAL_8N1_TXINV,
		SERIAL_8E1_TXINV,
		SERIAL_8O1_TXINV,

		SERIAL_8N2_TXINV,
		SERIAL_8N2_TXINV,
		SERIAL_8N2_TXINV,
		SERIAL_8N2_TXINV,
		SERIAL_8N2_TXINV,
		SERIAL_8N2_TXINV,

		SERIAL_7E1_RXINV_TXINV,
		SERIAL_7E1_RXINV_TXINV,
		SERIAL_7O1_RXINV_TXINV,
		SERIAL_8N1_RXINV_TXINV,
		SERIAL_8E1_RXINV_TXINV,
		SERIAL_8O1_RXINV_TXINV,

		SERIAL_8N2_RXINV_TXINV,
		SERIAL_8N2_RXINV_TXINV,
		SERIAL_8N2_RXINV_TXINV,
		SERIAL_8N2_RXINV_TXINV,
		SERIAL_8N2_RXINV_TXINV,
		SERIAL_8N2_RXINV_TXINV
*/
/*
#ifdef SERIAL_9BIT_SUPPORT
#define SERIAL_9N1 0x84
#define SERIAL_9E1 0x8E
#define SERIAL_9O1 0x8F
#define SERIAL_9N1_RXINV 0x94
#define SERIAL_9E1_RXINV 0x9E
#define SERIAL_9O1_RXINV 0x9F
#define SERIAL_9N1_TXINV 0xA4
#define SERIAL_9E1_TXINV 0xAE
#define SERIAL_9O1_TXINV 0xAF
#define SERIAL_9N1_RXINV_TXINV 0xB4
#define SERIAL_9E1_RXINV_TXINV 0xBE
#define SERIAL_9O1_RXINV_TXINV 0xBF
#endif
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_DataBits,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_InvertedRX,
		typename T_InvertedTX,
		typename T_OutputPin,
		typename T_Parity,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits
	> class TeensySerialPort : public BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin>,
		public T_AfterSendingDelay,
		public T_DataBits,
		public T_FEndTime,
		public T_FSending,
		public T_InvertedRX,
		public T_InvertedTX,
		public T_Parity,
		public T_SendingOutputPin,
		public T_Speed,
		public T_StopBits
	{
		typedef BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin> inherited;

	public:
		_V_PIN_( SendingOutputPin )

	public:
		_V_PROP_( Speed )

	public:
//		_V_PROP_( Enabled )
		_V_PROP_( AfterSendingDelay )

//	public:
//		bool	FTransmit : 1; // In base class to save memory
//		bool	FReceive : 1; // In base class to save memory

	public:
//#ifndef VISUINO_CHIPKIT
		_V_PROP_( Parity )
		_V_PROP_( StopBits )
		_V_PROP_( DataBits )
//#endif // VISUINO_CHIPKIT
		_V_PROP_( InvertedRX )
		_V_PROP_( InvertedTX )

	protected:
		_V_PROP_( FSending )

/*
#ifdef VISUINO_ESP8266
		bool	ReceiveEnabled : 1;
		bool	TransmitEnabled : 1;
		bool	UseAlternativePins : 1;

#endif // VISUINO_ESP8266
*/
#ifdef _MITOV_SERIAL_SENDING_PIN_
	#ifndef VISUINO_CHIPKIT
		uint32_t GetByteSpeed()
		{ 
			return Speed().GetValue() / ( StopBits().GetValue() + DataBits().GetValue() + 1 ); 
		}
	#endif // VISUINO_CHIPKIT
#endif

	public:
		void ResetSerial()
		{
			C_SERIAL.end();
			StartPort();
		}

	protected:
		inline void SerialEnd() {} // Placeholder

		void StartPort()
		{
//			int AIndex = ((int)Parity) * 8 + ( StopBits - 1 ) * 4 + ( DataBits - 5);
			int AIndex = ( StopBits().GetValue() - 1 ) * 6 + ( DataBits().GetValue() - 5) * 3 + ((int)Parity().GetValue() );
			long AValue = CTeensySerialInits[ AIndex ];
			if( InvertedRX() )
				AValue |= 0x10;

			if( InvertedTX() )
				AValue |= 0x20;

			C_SERIAL.begin( Speed(), AValue );
		}


/*
#ifdef _MITOV_SERIAL_SENDING_PIN_
		virtual uint32_t GetByteSpeed()
		{ 
			return 9600 / 11;
		}
#endif
*/
	protected:
		_V_PROP_( FEndTime )
//#ifdef _MITOV_SERIAL_SENDING_PIN_
//		uint32_t	FEndTime;
//#endif

	protected:
//	#if ( defined( VISUINO_CONTROLLINO_MAXI ) || defined( VISUINO_CONTROLLINO_MEGA ) || defined( VISUINO_M_DUINO_ETHERNET_58 ))
//		virtual
//	#endif
		void UpdateSending( bool ASending )
		{
#ifdef _MITOV_SERIAL_SENDING_PIN_
//			T_PARENT::UpdateSending( ASending );
			T_SendingOutputPin::SetPinValue( ASending );
#endif // _MITOV_SERIAL_SENDING_PIN_
		}

#ifdef _MITOV_SERIAL_SENDING_PIN_
		void SendingCount( uint32_t ACount )
		{
//			Serial.println( ACount );

			ACount = 1; // Most conrtrollers can send only one character at a time.
#ifdef __AVR
			ACount = 1; // The AVR can send only one character at a time.
#endif
			if( ! FSending() )
			{
				FSending() = true;
				UpdateSending( FSending() );
				uint32_t ATime = micros();
				FEndTime() = ATime + AfterSendingDelay() + ACount * ( 10000000 / GetByteSpeed() );
//				Serial.println( 10000000 / GetByteSpeed() );
//				Serial.println( int32_t( FEndTime - ATime ));
			}

			else
			{
				UpdateSending( FSending() );
				uint32_t ATime = micros();
				if( int32_t( FEndTime().GetValue() - ATime ) >= AfterSendingDelay() )
					FEndTime() = FEndTime().GetValue() + ACount * ( 10000000 / GetByteSpeed() );

				else
					FEndTime() = ATime + AfterSendingDelay() + ACount * ( 10000000 / GetByteSpeed() );

//					Serial.println( 10000000 / GetByteSpeed() );
//					Serial.println( int32_t( FEndTime - ATime ));
			}

//			Serial.println( 10000000 / GetByteSpeed() );
//			Serial.println( int32_t( FEndTime - ATime ));
		}
#endif

	public:
		template <typename T> void Print( T AValue )
		{
			if( inherited::Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( 1 );
#endif
				C_SERIAL.println( AValue );
#ifdef __AVR
//				SendingCount( 1 );
#endif
			}
		}

#ifdef _MITOV_SERIAL_SENDING_PIN_
		void Print( Mitov::String AValue )
		{
			if( inherited::Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( AValue.length() );
#endif
				C_SERIAL.println( AValue );
#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//				SendingCount( 1 );
#endif
#endif
			}
		}

		void Print( char *AValue )
		{
			if( inherited::Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( strlen( AValue ) );
#endif
				C_SERIAL.println( AValue );
#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//				SendingCount( 1 );
#endif
#endif
			}
		}
#endif

		void PrintChar( char AValue )
		{
			if( inherited::Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( 1 );
#endif
				C_SERIAL.print( AValue );
			}
		}

		size_t Write( uint8_t *AData, uint32_t ASize )
		{
			if( ! inherited::Enabled().GetValue() )
				return 0;

#ifdef _MITOV_SERIAL_SENDING_PIN_
			SendingCount( ASize );
#endif
			return C_SERIAL.write( AData, ASize );
#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//			SendingCount( 1 );
#endif
#endif
		}

	    size_t Write(uint8_t byte)
		{
			if( ! inherited::Enabled().GetValue() )
				return 0;

#ifdef _MITOV_SERIAL_SENDING_PIN_
			SendingCount( 1 );
#endif
			return C_SERIAL.write( byte );
		}

		inline int AvailableForWrite()
		{
			return C_SERIAL.availableForWrite();
		}

	public:
		inline void SystemLoopBegin()
		{
#ifdef _MITOV_SERIAL_SENDING_PIN_
			if( FSending() )
			{
				unsigned long ACurrentMicros = micros();
				if( int32_t( FEndTime().GetValue() - ACurrentMicros ) < 0 )
				{
//					Serial.print( "STOP: " );
//					Serial.println( int32_t( FEndTime - ACurrentMicros ) );
					FSending() = false;
					UpdateSending( FSending() );
				}
			}

#endif
		}

		inline void SystemInit()
		{
			if( inherited::Enabled() )
				StartPort();

		}

	public:
        void RestartPort() // Used by setters
		{
			if( ! inherited::Enabled() )
				return;

			C_SERIAL.end();
			StartPort();
		}

	public:
		void UpdateEnabled()
		{
			if( inherited::Enabled() )
				StartPort();

			else
			{
				C_SERIAL.end();
				SerialEnd();
			}
		}

	};
//---------------------------------------------------------------------------
} // Mitov
