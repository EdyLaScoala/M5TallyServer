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

namespace Mitov
{
#if ! defined( VISUINO_STMDUINO )
	static const	USARTClass::USARTModes CUSARTSerialInits[] = 
	{
		SERIAL_5N1,
		SERIAL_6N1,
		SERIAL_7N1,
		SERIAL_8N2,

		SERIAL_5N2,
		SERIAL_6N2,
		SERIAL_7N2,
		SERIAL_8N2,

		SERIAL_5E1,
		SERIAL_6E1,
		SERIAL_7E1,
		SERIAL_8E2,

		SERIAL_5E2,
		SERIAL_6E2,
		SERIAL_7E2,
		SERIAL_8E2,

		SERIAL_5O1,
		SERIAL_6O1,
		SERIAL_7O1,
		SERIAL_8O2,

		SERIAL_5O2,
		SERIAL_6O2,
		SERIAL_7O2,
		SERIAL_8O2,

		SERIAL_5M1,
		SERIAL_6M1,
		SERIAL_7M1,
		SERIAL_8M2,

		SERIAL_5M2,
		SERIAL_6M2,
		SERIAL_7M2,
		SERIAL_8M2,

		SERIAL_5S1,
		SERIAL_6S1,
		SERIAL_7S1,
		SERIAL_8S2,

		SERIAL_5S2,
		SERIAL_6S2,
		SERIAL_7S2,
		SERIAL_8S2
	};
#endif

//---------------------------------------------------------------------------
/*
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL
	> class USARTSerialPort : public Mitov::SpeedSerialPort<T_SERIAL_TYPE, C_SERIAL>
	{
		typedef	Mitov::SpeedSerialPort<T_SERIAL_TYPE, C_SERIAL> inherited;

	public:
		TArduinoSerialParity	Parity : 3;
		unsigned int			StopBits : 2;
		unsigned int			DataBits : 4;

#ifdef _MITOV_SERIAL_SENDING_PIN_
	protected:
		uint32_t GetByteSpeed()
		{ 
			return Speed / ( StopBits + DataBits + 1 ); 
		}
#endif

	protected:
		inline void UpdateSending( bool ASending ) {} // Placeholder

		void StartPort()
		{
#if defined( VISUINO_STMDUINO )
			C_SERIAL.begin( inherited::Speed );
#else
			int AIndex = ((int)Parity) * 8 + ( StopBits - 1 ) * 4 + ( DataBits - 5);
			C_SERIAL.begin( inherited::Speed, CUSARTSerialInits[ AIndex ] );
#endif
		}

	public:
		USARTSerialPort() :
			Parity( spNone ),
			StopBits( 2 ),
			DataBits( 8 )
		{
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_DataBits,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_Parity,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits
	> class USARTSerialPort : public BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin>,
		public T_AfterSendingDelay,
		public T_DataBits,
		public T_FEndTime,
		public T_FSending,
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
		_V_PROP_( AfterSendingDelay )

//	public:
//		bool	FTransmit : 1; // In base class to save memory
//		bool	FReceive : 1; // In base class to save memory

	protected:
		_V_PROP_( FSending )

	protected:
		_V_PROP_( FEndTime )
//#ifdef _MITOV_SERIAL_SENDING_PIN_
//		uint32_t	FEndTime;
//#endif

	public:
//#ifndef VISUINO_CHIPKIT
		_V_PROP_( Parity )
		_V_PROP_( StopBits )
		_V_PROP_( DataBits )
//#endif // VISUINO_CHIPKIT

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
#if defined( VISUINO_STMDUINO )
			C_SERIAL.begin( Speed() );
#else // VISUINO_STMDUINO
			int AIndex = ((int)Parity().GetValue() ) * 8 + ( StopBits().GetValue() - 1 ) * 4 + ( DataBits().GetValue() - 5);
			C_SERIAL.begin( Speed(), CUSARTSerialInits[ AIndex ] );
#endif // VISUINO_STMDUINO
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

	public:
/*
		inline USARTSerialPort()
		{
#ifdef _MITOV_SERIAL_SENDING_PIN_
			FSending() = false;
#endif
		}
*/
	};
//---------------------------------------------------------------------------
} // Mitov
