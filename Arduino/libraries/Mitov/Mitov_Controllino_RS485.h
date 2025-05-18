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
#include <Controllino.h>
#include <Mitov_StandardSerial.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_DataBits,
		typename T_Enabled,
//		typename T_FReceive,
		typename T_FEndTime,
		typename T_FSending,
		typename T_FTransmit,
		typename T_OutputPin,
		typename T_Parity,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits
	> class ControllinoRS48SerialPort : public BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin>,
		public T_AfterSendingDelay,
		public T_DataBits,
//		public T_FReceive,
		public T_FEndTime,
		public T_FSending,
		public T_FTransmit,
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

	protected:
//		_V_PROP_( FReceive )
		_V_PROP_( FSending )
		_V_PROP_( FTransmit )

	protected:
		_V_PROP_( FEndTime )
//#ifdef _MITOV_SERIAL_SENDING_PIN_
//		uint32_t	FEndTime;
//#endif

	public:
		_V_PROP_( Parity )
		_V_PROP_( StopBits )
		_V_PROP_( DataBits )

	protected:
//#ifdef _MITOV_SERIAL_SENDING_PIN_
		inline uint32_t GetByteSpeed()
		{ 
			return Speed().GetValue() / ( StopBits().GetValue() + DataBits().GetValue() + 1 ); 
		}
//#endif

		inline void UpdateSending( bool ASending )
		{
//			Serial.print( "UpdateSending: " );
//			Serial.println( ASending );
#if ((!defined _MITOV_CONTROLLINO_RS485_TRANSMIT_PIN_ ) || (!defined _MITOV_CONTROLLINO_RS485_RECEIVE_PIN_ ))
//			Serial.print( "UpdateSending: " );
//			Serial.println( inherited::FSending );
#ifndef _MITOV_CONTROLLINO_RS485_TRANSMIT_PIN_
//			Serial.println( "Controllino_SwitchRS485DE" );
			UpdateSendingDirection( ASending );

//			Controllino_SwitchRS485DE( ASending );
#endif

//#ifndef _MITOV_CONTROLLINO_RS485_RECEIVE_PIN_
//			Serial.println( "Controllino_SwitchRS485RE" );
//			Controllino_SwitchRS485RE( ASending );
//#endif
//			Controllino_SwitchRS485DE( 1 );
//			Controllino_SwitchRS485RE( 1 );

//			inherited::UpdateSending();
#endif
//#ifdef _MITOV_SERIAL_SENDING_PIN_
//			T_PARENT::UpdateSending( ASending );
			T_SendingOutputPin::SetPinValue( ASending );
//#endif // _MITOV_SERIAL_SENDING_PIN_
		}

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
			int AIndex = ((int)Parity()) * 8 + ( StopBits() - 1 ) * 4 + ( DataBits() - 5);
			C_SERIAL.begin( Speed(), CSerialInits[ AIndex ] );
		}


	protected:
//#ifdef _MITOV_SERIAL_SENDING_PIN_
		void SendingCount( uint32_t ACount )
		{
//			Serial.println( ACount );

			ACount = 1; // Most conrtrollers can send only one character at a time.
#ifdef __AVR
			ACount = 1; // The AVR can send only one character at a time.
#endif
			if( ! FSending().GetValue() )
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
//#endif

	public:
		template <typename T> void Print( T AValue )
		{
			if( inherited::Enabled() )
			{
//#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( 1 );
//#endif
				C_SERIAL.println( AValue );
#ifdef __AVR
//				SendingCount( 1 );
#endif
			}
		}

//#ifdef _MITOV_SERIAL_SENDING_PIN_
		void Print( Mitov::String AValue )
		{
			if( inherited::Enabled() )
			{
//#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( AValue.length() );
//#endif
				C_SERIAL.println( AValue );
//#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//				SendingCount( 1 );
#endif
//#endif
			}
		}

		void Print( char *AValue )
		{
			if( inherited::Enabled() )
			{
//#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( strlen( AValue ) );
//#endif
				C_SERIAL.println( AValue );
//#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//				SendingCount( 1 );
#endif
//#endif
			}
		}
//#endif

		void PrintChar( char AValue )
		{
			if( inherited::Enabled() )
			{
//#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( 1 );
//#endif
				C_SERIAL.print( AValue );
			}
		}

		size_t Write( uint8_t *AData, uint32_t ASize )
		{
			if( ! inherited::Enabled().GetValue() )
				return 0;

//#ifdef _MITOV_SERIAL_SENDING_PIN_
			SendingCount( ASize );
//#endif
			return C_SERIAL.write( AData, ASize );
//#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//			SendingCount( 1 );
#endif
//#endif
		}

	    size_t Write(uint8_t byte)
		{
			if( ! inherited::Enabled().GetValue() )
				return 0;

//#ifdef _MITOV_SERIAL_SENDING_PIN_
			SendingCount( 1 );
//#endif
			return C_SERIAL.write( byte );
//#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//			SendingCount( 1 );
#endif
//#endif
		}

		inline int AvailableForWrite()
		{
			return C_SERIAL.availableForWrite();
		}

	public:
		inline void SystemLoopBegin()
		{
//#ifdef _MITOV_SERIAL_SENDING_PIN_
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

//#endif
		}

		inline void SystemInit()
		{
			if( inherited::Enabled() )
			{
				StartModule();
				StartPort();
			}
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
			{
				StartModule();
				StartPort();
			}

			else
			{
				C_SERIAL.end();
				SerialEnd();
				StopModule();
			}
		}

	protected:
		void UpdateSendingDirection( bool AValue )
		{
			if( AValue )
			{
				digitalWrite(CONTROLLINO_RS485_nRE, HIGH );
				digitalWrite(CONTROLLINO_RS485_DE, HIGH );
			}

			else
			{
				digitalWrite(CONTROLLINO_RS485_DE, LOW );
				digitalWrite(CONTROLLINO_RS485_nRE, LOW );
			}
		}

		void StopModule()
		{
		}

		void StartModule()
		{
//			Serial.println( "START MODULE" );

			pinMode( CONTROLLINO_RS485_DE, OUTPUT );
			pinMode( CONTROLLINO_RS485_nRE, OUTPUT );

//			Controllino_RS485Init( Speed() );
#ifdef _MITOV_CONTROLLINO_RS485_TRANSMIT_PIN_
//			Controllino_SwitchRS485DE( FTransmit().GetValue() );
//			Serial.println( "FTransmit" );
//			Serial.println( FTransmit().GetValue() );
			UpdateSendingDirection( FTransmit().GetValue() );
#endif
//#ifdef _MITOV_CONTROLLINO_RS485_RECEIVE_PIN_
//			Controllino_SwitchRS485RE( ! FReceive().GetValue() );
//#endif
		}

	public:
		void TransmitInputPin_o_Receive( void *_Data )
		{
			FTransmit() = *(bool *)_Data;
			if( ! Enabled().GetValue() )
				return;

//			Serial.println( "FTransmit" );
//			Serial.println( FTransmit().GetValue() );
			UpdateSendingDirection( FTransmit().GetValue() );
//			Controllino_SwitchRS485DE( FTransmit().GetValue() );
		}

/*
		void ReceiveInputPin_o_Receive( void *_Data )
		{
			FReceive() = *(bool *)_Data;
			if( ! Enabled().GetValue() )
				return;

			Controllino_SwitchRS485RE( ! FReceive().GetValue() );
		}
*/
	public:
		inline ControllinoRS48SerialPort()
		{
//			FReceive() = false;
			FTransmit() = false;
//			FSending() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
