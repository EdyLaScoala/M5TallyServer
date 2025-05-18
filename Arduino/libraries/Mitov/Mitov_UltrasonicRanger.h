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
#include <Mitov_MultiSensorStart.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	enum ArduinoRangerUnits_Def { rdTime, rdCm, rdInch };

	typedef ArduinoRangerUnits_Def TArduinoRangerUnits;
//---------------------------------------------------------------------------
namespace UltrasonicRangerTypes
{
	enum State { sStartUp, sStartDown, sListeningEdgeUp, sListeningEdgeDown, sEchoDetected, sPause };
}
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FClocked,
		typename T_FState,
		typename T_OutputPin,
		typename T_PauseTime,
		typename T_PingOutputPin,
		typename T_PingTime,
		typename T_Timeout, 
		typename T_TimeoutOutputPin,
		typename T_TimeoutValue,
		typename T_Units
	> class UltrasonicRanger : 
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FClocked,
		public T_FState,
		public T_OutputPin,
		public T_PauseTime,
		public T_PingOutputPin,
		public T_PingTime,
		public T_Timeout, 
		public T_TimeoutOutputPin,
		public T_TimeoutValue,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( PingOutputPin )
		_V_PIN_( TimeoutOutputPin )

	public:
		_V_PROP_( Timeout )
		_V_PROP_( PingTime )
		_V_PROP_( PauseTime )
		_V_PROP_( TimeoutValue )
		_V_PROP_( Units )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
//		UltrasonicRangerTypes::State	FState : 3;
		_V_PROP_( FState )
		_V_PROP_( FClocked )

		unsigned long	FStartTime;
		unsigned long	FEchoStartTime;
		unsigned long	FEndTime;

	public:
		void EchoInputPin_o_Receive( void *_Data )
		{
			if( ( FState() != UltrasonicRangerTypes::sListeningEdgeUp ) && ( FState() != UltrasonicRangerTypes::sListeningEdgeDown ) )
				return;

			bool AValue = *(bool *)_Data;
			if( FState() == UltrasonicRangerTypes::sListeningEdgeDown )
			{
				if( AValue )
					return;

				FEndTime = micros();
				FState() = UltrasonicRangerTypes::sEchoDetected;
			}

			else
			{
			  if( ! AValue )
				  return;

			  FState() = UltrasonicRangerTypes::sListeningEdgeDown;
			  FEchoStartTime = micros();
			}
		}

	public:
		inline void SystemStart()
		{
			T_PingOutputPin::SetPinValueLow();
			T_TimeoutOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			switch ( FState() )
			{
				case UltrasonicRangerTypes::sPause:
				{
					if( ( micros() - FStartTime ) < ((unsigned long)PauseTime() ) * 1000 )
						return;

#ifdef _MITOV_ULTRASONIC_RANGE_CLOCK_PIN_
					if( ClockInputPin_o_IsConnected() )
						if( ! FClocked() )
							return;

					FClocked() = false;
#endif
				}

				case UltrasonicRangerTypes::sStartUp:
				{
//					Serial.println( "start" );
					T_PingOutputPin::SetPinValueHigh();
//					PingOutputPin.SendValue<bool>( true );
					FStartTime = micros();
					FState() = UltrasonicRangerTypes::sStartDown;
					break;
				}

				case UltrasonicRangerTypes::sStartDown:
				{
					unsigned long ACurrentMicros = micros();

					if( ( ACurrentMicros - FStartTime ) < PingTime() )
						return;

					T_PingOutputPin::SetPinValueLow();
//					PingOutputPin.SendValue<bool>( false );
					FStartTime = ACurrentMicros;
					FState() = UltrasonicRangerTypes::sListeningEdgeUp;
					break;
				}

				case UltrasonicRangerTypes::sListeningEdgeUp:
				case UltrasonicRangerTypes::sListeningEdgeDown:
				{
					if( ( micros() - FStartTime ) < ((unsigned long)Timeout() ) * 1000 )
						return;

					T_OutputPin::SetPinValue( TimeoutValue() );
//					OutputPin.Notify( &TimeoutValue );
					T_TimeoutOutputPin::SetPinValue( true, true );
					FState() = UltrasonicRangerTypes::sPause;
					break;
				}

				case UltrasonicRangerTypes::sEchoDetected:
				{
					unsigned long APeriod = FEndTime - FEchoStartTime;
					float AValue;
					switch( Units() )
					{
						case rdTime:
							AValue = APeriod;
							break;

						case rdCm:
							AValue = ((float)APeriod) / 29 / 2; // / 58.2;
							break;

						case rdInch:
							AValue = ((float)APeriod) / 74 / 2;
							break;
					}

					T_OutputPin::SetPinValue( AValue );
//					OutputPin.Notify( &AValue );
					T_TimeoutOutputPin::SetPinValue( false, true );
					FState() = UltrasonicRangerTypes::sPause;
					break;
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
		}

	public:
		UltrasonicRanger()
		{
			FState() = UltrasonicRangerTypes::sStartUp;
			FClocked() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FClocked,
		typename T_FState,
		typename T_OutputPin,
		typename T_PauseTime,
		typename T_PingOutputPin,
		typename T_PingTime,
		typename T_Timeout, 
		typename T_TimeoutOutputPin,
		typename T_TimeoutValue,
		typename T_Units
	> class UltrasonicRanger_Interrupt : 
//		public OpenWire::InterruptObject, 
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FClocked,
		public T_FState,
		public T_OutputPin,
		public T_PauseTime,
		public T_PingOutputPin,
		public T_PingTime,
		public T_Timeout, 
		public T_TimeoutOutputPin,
		public T_TimeoutValue,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( PingOutputPin )
		_V_PIN_( TimeoutOutputPin )

	public:
		_V_PROP_( Timeout )
		_V_PROP_( PingTime )
		_V_PROP_( PauseTime )
		_V_PROP_( TimeoutValue )
		_V_PROP_( Units )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FState )
		_V_PROP_( FClocked )

		unsigned long	FStartTime;
		unsigned long	FEchoStartTime;
		unsigned long	FEndTime;

	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool AValue )
		{
//			Serial.println( "+" );
			if( ( FState() != UltrasonicRangerTypes::sListeningEdgeUp ) && ( FState() != UltrasonicRangerTypes::sListeningEdgeDown ) )
				return;

			if( FState() == UltrasonicRangerTypes::sListeningEdgeDown )
			{
				if( AValue )
					return;

				FEndTime = micros();
				FState() = UltrasonicRangerTypes::sEchoDetected;
			}

			else
			{
			  if( ! AValue )
				  return;

			  FState() = UltrasonicRangerTypes::sListeningEdgeDown;
			  FEchoStartTime = micros();
			}
		}

	public:
		inline void SystemStart()
		{
			T_PingOutputPin::SetPinValueLow();
			T_TimeoutOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			switch ( FState() )
			{
				case UltrasonicRangerTypes::sPause:
				{
					if( ( micros() - FStartTime ) < ((unsigned long)PauseTime() ) * 1000 )
						return;

#ifdef _MITOV_ULTRASONIC_RANGE_CLOCK_PIN_
					if( ClockInputPin_o_IsConnected() )
						if( ! FClocked() )
							return;

					FClocked() = false;
#endif
				}

				case UltrasonicRangerTypes::sStartUp:
				{
//					Serial.println( "start" );
					T_PingOutputPin::SetPinValueHigh();
					FStartTime = micros();
					FState() = UltrasonicRangerTypes::sStartDown;
					break;
				}

				case UltrasonicRangerTypes::sStartDown:
				{
					unsigned long ACurrentMicros = micros();

					if( ( ACurrentMicros - FStartTime ) < PingTime() )
						return;

//					Serial.println( "start 1" );
					T_PingOutputPin::SetPinValueLow();
					FStartTime = ACurrentMicros;
					FState() = UltrasonicRangerTypes::sListeningEdgeUp;
					break;
				}

				case UltrasonicRangerTypes::sListeningEdgeUp:
				case UltrasonicRangerTypes::sListeningEdgeDown:
				{
					if( ( micros() - FStartTime ) < ((unsigned long)Timeout() ) * 1000 )
						return;

					T_OutputPin::SetPinValue( TimeoutValue() );
//					OutputPin.Notify( &TimeoutValue );
					T_TimeoutOutputPin::SetPinValue( true, true );
					FState() = UltrasonicRangerTypes::sPause;
					break;
				}

				case UltrasonicRangerTypes::sEchoDetected:
				{
					unsigned long APeriod = FEndTime - FEchoStartTime;
					float AValue;
					switch( Units() )
					{
						case rdTime:
							AValue = APeriod;
							break;

						case rdCm:
							AValue = ((float)APeriod) / 29 / 2; // / 58.2;
							break;

						case rdInch:
							AValue = ((float)APeriod) / 74 / 2;
							break;
					}

					T_OutputPin::SetPinValue( AValue );
//					OutputPin.Notify( &AValue );
					T_TimeoutOutputPin::SetPinValue( false, true );
					FState() = UltrasonicRangerTypes::sPause;
					break;
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
		}

	public:
		UltrasonicRanger_Interrupt()
		{
			FState() = UltrasonicRangerTypes::sStartUp;
			FClocked() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FState,
		typename T_OutputPin,
		typename T_Timeout,
		typename T_TimeoutOutputPin, 
		typename T_TimeoutValue,
		typename T_Units
	> class UltrasonicRangerElement : 
		public T_Enabled, 
		public T_FState,
		public T_OutputPin, 
		public T_Timeout,
		public T_TimeoutOutputPin, 
		public T_TimeoutValue,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( TimeoutOutputPin )

	public:
		_V_PROP_( Timeout )
		_V_PROP_( TimeoutValue )

		_V_PROP_( Units )

		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FState )

		unsigned long	FStartTime;
		unsigned long	FEchoStartTime;
		unsigned long	FEndTime;

	public:
		void EchoInputPin_o_Receive( void *_Data )
		{
//			Serial.print( "TEST: " ); Serial.println( *(bool *)_Data );
//			Serial.println( FState );
			if( ( FState() != UltrasonicRangerTypes::sListeningEdgeUp ) && ( FState() != UltrasonicRangerTypes::sListeningEdgeDown ) )
				return;

			bool AValue = *(bool *)_Data;
			if( FState() == UltrasonicRangerTypes::sListeningEdgeDown )
			{
				if( AValue )
					return;

				FEndTime = micros();
				FState() = UltrasonicRangerTypes::sEchoDetected;
//				Serial.println( "DETECTED" );
			}

			else
			{
			  if( ! AValue )
				  return;

			  FState() = UltrasonicRangerTypes::sListeningEdgeDown;
			  FEchoStartTime = micros();
			}
		}

	public:
		void StartElement( uint32_t &ACountStarted )
		{
			if( ! Enabled() )
				return;

//			Serial.println( "START" );
			FStartTime = micros();
			FState() = UltrasonicRangerTypes::sListeningEdgeUp;

			++ ACountStarted;
//			return true;
		}

	public:
		inline void SystemStart()
		{
			T_TimeoutOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			switch ( FState() )
			{
/*
				case UltrasonicRangerTypes::sStartUp:
				{
//					Serial.println( "start" );
					FStartTime = micros();
					FState = UltrasonicRangerTypes::sStartDown;
					break;
				}

				case UltrasonicRangerTypes::sStartDown:
				{
					if( ( ACurrentMicros - FStartTime ) < PingTime )
						return;

					bool AValue = false;
					PingOutputPin.Notify( &AValue );
					FStartTime = ACurrentMicros;
					FState = UltrasonicRangerTypes::sListeningEdgeUp;
					break;
				}
*/
				case UltrasonicRangerTypes::sListeningEdgeUp:
				case UltrasonicRangerTypes::sListeningEdgeDown:
				{
					unsigned long ACurrentMicros = micros();
					if( ( ACurrentMicros - FStartTime ) < ((unsigned long)Timeout() ) * 1000 )
						return;

					FState() = UltrasonicRangerTypes::sPause;
					T_OutputPin::SetPinValue( TimeoutValue() );
					T_TimeoutOutputPin::SetPinValue( true, true );
//					Serial.println( "TIMEOUT" );
//					Serial.println( FStartTime );
//					Serial.println( ACurrentMicros - FStartTime );
					C_OWNER.ElementReady();
					break;
				}

				case UltrasonicRangerTypes::sEchoDetected:
				{
					unsigned long APeriod = FEndTime - FEchoStartTime;
					float AValue;
					switch( Units() )
					{
						case rdTime:
							AValue = APeriod;
							break;

						case rdCm:
							AValue = ((float)APeriod) / 29 / 2; // / 58.2;
							break;

						case rdInch:
							AValue = ((float)APeriod) / 74 / 2;
							break;
					}

					FState() = UltrasonicRangerTypes::sPause;
					T_OutputPin::SetPinValue( AValue );
					T_TimeoutOutputPin::SetPinValue( false, true );
//					Serial.println( "VALUE" );
					C_OWNER.ElementReady();
					break;
				}
			}
		}

	public:
		UltrasonicRangerElement()
		{
			FState() = UltrasonicRangerTypes::sStartUp;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FState,
		typename T_OutputPin,
		typename T_Timeout,
		typename T_TimeoutOutputPin, 
		typename T_TimeoutValue,
		typename T_Units
	> class UltrasonicRangerElement_Interrupt : 
		public T_Enabled, 
		public T_FState,
		public T_OutputPin, 
		public T_Timeout,
		public T_TimeoutOutputPin, 
		public T_TimeoutValue,
		public T_Units
	{
		_V_PIN_( OutputPin )
		_V_PIN_( TimeoutOutputPin )

	public:
		_V_PROP_( Timeout )
		_V_PROP_( TimeoutValue )

		_V_PROP_( Units )

		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FState )
		unsigned long	FStartTime;
		unsigned long	FEchoStartTime;
		unsigned long	FEndTime;

	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool AValue )
		{
			if( ( FState() != UltrasonicRangerTypes::sListeningEdgeUp ) && ( FState() != UltrasonicRangerTypes::sListeningEdgeDown ) )
				return;

			if( FState() == UltrasonicRangerTypes::sListeningEdgeDown )
			{
				if( AValue )
					return;

				FEndTime = micros();
				FState() = UltrasonicRangerTypes::sEchoDetected;
			}

			else
			{
			  if( ! AValue )
				  return;

			  FState() = UltrasonicRangerTypes::sListeningEdgeDown;
			  FEchoStartTime = micros();
			}
		}

	public:
		void StartElement( uint32_t &ACountStarted )
		{
//			Serial.println( "START" );
			if( ! Enabled() )
				return;

			FStartTime = micros();
			FState() = UltrasonicRangerTypes::sListeningEdgeUp;
			++ ACountStarted;
		}

	public:
		inline void SystemStart()
		{
			T_TimeoutOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			switch ( FState() )
			{
/*
				case UltrasonicRangerTypes::sStartUp:
				{
//					Serial.println( "start" );
					PingOutputPin.SendValue<bool>( true );
					FStartTime = ACurrentMicros;
					FState = UltrasonicRangerTypes::sStartDown;
					break;
				}

				case UltrasonicRangerTypes::sStartDown:
				{
					if( ( ACurrentMicros - FStartTime ) < PingTime )
						return;

					PingOutputPin.SendValue<bool>( false );
					FStartTime = ACurrentMicros;
					FState = UltrasonicRangerTypes::sListeningEdgeUp;
					break;
				}
*/
				case UltrasonicRangerTypes::sListeningEdgeUp:
				case UltrasonicRangerTypes::sListeningEdgeDown:
				{
					if( ( micros() - FStartTime ) < ((unsigned long)Timeout() ) * 1000 )
						return;

					FState() = UltrasonicRangerTypes::sPause;
					T_OutputPin::SetPinValue( TimeoutValue() );
					T_TimeoutOutputPin::SetPinValue( true, true );
//					Serial.println( "TIMEOUT" );
//					Serial.println( FStartTime );
//					Serial.println( ACurrentMicros - FStartTime );
//					FState = UltrasonicRangerTypes::sPause;
					C_OWNER.ElementReady();
					break;
				}

				case UltrasonicRangerTypes::sEchoDetected:
				{
					unsigned long APeriod = FEndTime - FEchoStartTime;
					float AValue;
					switch( Units() )
					{
						case rdTime:
							AValue = APeriod;
							break;

						case rdCm:
							AValue = ((float)APeriod) / 29 / 2; // / 58.2;
							break;

						case rdInch:
							AValue = ((float)APeriod) / 74 / 2;
							break;
					}

					FState() = UltrasonicRangerTypes::sPause;
					T_OutputPin::SetPinValue( AValue );
					T_TimeoutOutputPin::SetPinValue( false, true );
//					FState = UltrasonicRangerTypes::sPause;
					C_OWNER.ElementReady();
					break;
				}
			}
		}

	public:
		UltrasonicRangerElement_Interrupt()
		{
			FState() = UltrasonicRangerTypes::sStartUp;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FClocked,
		typename T_FState,
		typename T_OutputPin,
		int PIN_ECHO,
		int PIN_PING,
		typename T_PauseTime,
		typename T_PingTime,
		typename T_Timeout, 
		typename T_TimeoutOutputPin,
		typename T_TimeoutValue,
		typename T_Units
	> class UltrasonicRangerShieldElement :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FClocked,
		public T_FState,
		public T_OutputPin,
		public T_PauseTime,
		public T_PingTime,
		public T_Timeout, 
		public T_TimeoutOutputPin,
		public T_TimeoutValue,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( TimeoutOutputPin )

	public:
		_V_PROP_( Timeout )
		_V_PROP_( PingTime )
		_V_PROP_( PauseTime )
		_V_PROP_( TimeoutValue )
		_V_PROP_( Units )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FState )
		_V_PROP_( FClocked )

		unsigned long	FStartTime;
		unsigned long	FEchoStartTime;
		unsigned long	FEndTime;

	protected:
		void DoReceive( bool AValue )
		{
//			Serial.println( AValue );
			if( ( FState() != UltrasonicRangerTypes::sListeningEdgeUp ) && ( FState() != UltrasonicRangerTypes::sListeningEdgeDown ) )
				return;

			if( FState() == UltrasonicRangerTypes::sListeningEdgeDown )
			{
				if( AValue )
					return;

				FEndTime = micros();
				FState() = UltrasonicRangerTypes::sEchoDetected;
			}

			else
			{
			  if( ! AValue )
				  return;

			  FState() = UltrasonicRangerTypes::sListeningEdgeDown;
			  FEchoStartTime = micros();
			}
		}

	public:
		inline void SystemStart()
		{
			T_TimeoutOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			bool AValue = ( digitalRead( PIN_ECHO ) == HIGH ); // Echo
			DoReceive( AValue );

			switch ( FState() )
			{
				case UltrasonicRangerTypes::sPause:
				{
					if( ( micros() - FStartTime ) < ((unsigned long)PauseTime() ) * 1000 )
						return;

#ifdef _MITOV_SHIELD_ELEMENT_ULTRASONIC_RANGE_CLOCK_PIN_
					if( ClockInputPin_o_IsConnected() )
						if( ! FClocked() )
							return;

					FClocked() = false;
#endif
				}

				case UltrasonicRangerTypes::sStartUp:
				{
//					Serial.println( "start" );
					digitalWrite( PIN_PING, HIGH ); // Ping
//					PingOutputPin.SendValue<bool>( true );
					FStartTime = micros();
					FState() = UltrasonicRangerTypes::sStartDown;
					break;
				}

				case UltrasonicRangerTypes::sStartDown:
				{
					unsigned long ACurrentMicros = micros();

					if( ( ACurrentMicros - FStartTime ) < PingTime() )
						return;

					digitalWrite( PIN_PING, LOW ); // Ping
//					PingOutputPin.SendValue<bool>( false );
					FStartTime = ACurrentMicros;
					FState() = UltrasonicRangerTypes::sListeningEdgeUp;
					break;
				}

				case UltrasonicRangerTypes::sListeningEdgeUp:
				case UltrasonicRangerTypes::sListeningEdgeDown:
				{
					if( ( micros() - FStartTime ) < ((unsigned long)Timeout() ) * 1000 )
						return;

					T_OutputPin::SetPinValue( TimeoutValue() );
					T_TimeoutOutputPin::SetPinValue( true, true );
					FState() = UltrasonicRangerTypes::sPause;
					break;
				}

				case UltrasonicRangerTypes::sEchoDetected:
				{
					unsigned long APeriod = FEndTime - FEchoStartTime;
					float AValue;
					switch( Units() )
					{
						case rdTime:
							AValue = APeriod;
							break;

						case rdCm:
							AValue = ((float)APeriod) / 29 / 2; // / 58.2;
							break;

						case rdInch:
							AValue = ((float)APeriod) / 74 / 2;
							break;
					}

					T_OutputPin::SetPinValue( AValue );
					T_TimeoutOutputPin::SetPinValue( false, true );
					FState() = UltrasonicRangerTypes::sPause;
					break;
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
		}

	public:
		UltrasonicRangerShieldElement()
		{
			FState() = UltrasonicRangerTypes::sStartUp;
			FClocked() = false;
			pinMode( PIN_PING, OUTPUT ); // Ping
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif