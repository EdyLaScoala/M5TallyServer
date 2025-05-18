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

namespace Mitov
{
//---------------------------------------------------------------------------
//	enum PCF8563ClockFrequency { rtc1307fDisabledLow, rtc1307fDisabledHigh, rtc1307f1Hz, rtc1307f4096Hz, rtc1307f8192Hz, rtc1307f32768Hz };
//---------------------------------------------------------------------------
	template <
		typename T_Day,
		typename T_Hour,
		typename T_InterruptEnabled,
		typename T_Minute,
		typename T_WeekDay
	> class TArduinoRTCAlarm :
		public T_InterruptEnabled,
		public T_Minute,
		public T_Hour,
		public T_Day,
		public T_WeekDay
	{
	public:
		_V_PROP_( Minute )
		_V_PROP_( Hour )
		_V_PROP_( Day )
		_V_PROP_( WeekDay )

		_V_PROP_( InterruptEnabled )

	};
//---------------------------------------------------------------------------
	enum TArduinoPCF8563TimerClockFrequency { pcft4096Hz, pcft64Hz, pcft1Hz, pcft1_60th_Hz, pcftAuto };
//---------------------------------------------------------------------------
	template <
		typename T_ClockFrequency,
		typename T_Enabled,
		typename T_InterruptEnabled,
		typename T_Period,
		typename T_PulseInterruptEnabled
	> class TArduinoPCF8563TimerElement :
		public T_ClockFrequency,
		public T_Enabled,
		public T_InterruptEnabled,
		public T_Period,
		public T_PulseInterruptEnabled
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InterruptEnabled )
		_V_PROP_( PulseInterruptEnabled )
		_V_PROP_( ClockFrequency )
		_V_PROP_( Period )

	};
//---------------------------------------------------------------------------
	enum TArduinoPCF8563ClockOutFrequency { pcfco32768Hz, pcfco1024Hz, pcfco32Hz, pcfco1Hz };
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Frequency
	> class TArduinoPCF8563ClockOutElement :
		public T_Enabled,
		public T_Frequency
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Frequency )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Alarm,
		typename T_AlarmOutputPin,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ClockOut,
		typename T_Enabled,
		typename T_LowVoltageOutputPin,
		typename T_OutputPin,
		typename T_Timer,
		typename T_TimerOutputPin
	> class RTC_PCF8563 :
		public T_Address,
		public T_Alarm,
		public T_AlarmOutputPin,
		public T_ClockInputPin_o_IsConnected,
		public T_ClockOut,
		public T_Enabled,
		public T_LowVoltageOutputPin,
		public T_OutputPin,
		public T_Timer,
		public T_TimerOutputPin
	{
	protected:
//		const uint8_t	PCF_ADDRESS = ( uint8_t( 0xA3 ) >> 1 );
//		const uint8_t	RTCC_WRITE = 0xA2;

		// register addresses in the rtc
		const uint8_t	RTCC_STAT1_ADDR			= 0x00;
		const uint8_t	RTCC_STAT2_ADDR			= 0x01;
		const uint8_t	RTCC_SEC_ADDR			= 0x02;
		const uint8_t	RTCC_MIN_ADDR			= 0x03;
		const uint8_t	RTCC_HOUR_ADDR			= 0x04;
		const uint8_t	RTCC_DAY_ADDR			= 0x05;
		const uint8_t	RTCC_WEEKDAY_ADDR		= 0x06;
		const uint8_t	RTCC_MONTH_ADDR			= 0x07;
		const uint8_t	RTCC_YEAR_ADDR			= 0x08;
		const uint8_t	RTCC_ALRM_MIN_ADDR		= 0x09;
		const uint8_t	RTCC_ALRM_HOUR_ADDR		= 0x0A;
		const uint8_t	RTCC_ALRM_DAY_ADDR		= 0x0B;
		const uint8_t	RTCC_ALRM_WEEKDAY_ADDR	= 0x0C;
		const uint8_t	RTCC_SQW_ADDR			= 0x0D;
		const uint8_t	RTCC_TIMER1_ADDR		= 0x0E;
		const uint8_t	RTCC_TIMER2_ADDR		= 0x0F;

	public:
		_V_PIN_( OutputPin )

//#ifdef _MITOV_PCF8563_RTC_LOW_VOLTAGE_PIN_
		_V_PIN_( LowVoltageOutputPin )
//#endif

//#ifdef _MITOV_PCF8563_RTC_ALARM_PIN_
		_V_PIN_( AlarmOutputPin )
//#endif

//#ifdef _MITOV_PCF8563_RTC_TIMER_PIN_
		_V_PIN_( TimerOutputPin )
//#endif

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( Alarm )
		_V_PROP_( Timer )
		_V_PROP_( ClockOut )

	public:
		void UpdateMinuteAlarm()
		{
			if( Alarm().Minute().Enabled() )
				writeRegister( RTCC_ALRM_MIN_ADDR, 0x80 | Func::FromDecToBcd( Alarm().Minute().Value() ) );

			else
				writeRegister( RTCC_ALRM_MIN_ADDR, 0 );

		}

		void UpdateHourAlarm()
		{
			if( Alarm().Hour().Enabled() )
				writeRegister( RTCC_ALRM_HOUR_ADDR, 0x80 | Func::FromDecToBcd( Alarm().Hour().Value() ) );

			else
				writeRegister( RTCC_ALRM_HOUR_ADDR, 0 );

		}

		void UpdateDayAlarm()
		{
			if( Alarm().Day().Enabled() )
				writeRegister( RTCC_ALRM_DAY_ADDR, 0x80 | Func::FromDecToBcd( Alarm().Day().Value() ) );

			else
				writeRegister( RTCC_ALRM_DAY_ADDR, 0 );

		}

		void UpdateWeekDayAlarm()
		{
			if( Alarm().WeekDay().Enabled() )
				writeRegister( RTCC_ALRM_WEEKDAY_ADDR, 0x80 | Func::FromDecToBcd( Alarm().WeekDay().Value() ) );

			else
				writeRegister( RTCC_ALRM_WEEKDAY_ADDR, 0 );

		}

		void UpdateClockOut()
		{
			if( ClockOut().Enabled() )
				writeRegister( RTCC_SQW_ADDR, 0x80 | uint8_t( ClockOut().Frequency() ) );

			else
				writeRegister( RTCC_SQW_ADDR, 0 );

		}

		void UpdateTimer()
		{
			uint8_t	AValueControl = 0;
			uint8_t	AValue = 0;
			if( Timer().Enabled() )
			{
				if( Timer().ClockFrequency().GetValue() != pcftAuto )
				{
					AValueControl = 0x80 | uint8_t( Timer().ClockFrequency() );
					switch( Timer().ClockFrequency() )
					{
						case pcft4096Hz:
							if( Timer().Period() < 0.062255859375 )
								AValue = ( Timer().Period() * 255 / 0.062255859375 ) + 0.5;

							else
								AValue = 255;

							break;

						case pcft64Hz:
							if( Timer().Period() < 3.984375 )
								AValue = ( Timer().Period() * 255 / 3.9843755 ) + 0.5;

							else
								AValue = 255;

							break;

						case pcft1Hz:
							if( Timer().Period() < 255 )
								AValue = Timer().Period() + 0.5;

							else
								AValue = 255;

							break;

						default:
							if( Timer().Period() < 15300 )
								AValue = ( Timer().Period() * 255 / 15300 ) + 0.5;

							else
								AValue = 255;

							break;
					}
				}

				else
				{
					if( Timer().Period() < 0.062255859375 )
					{
						AValueControl = 0x80;
						AValue = ( Timer().Period() * 255 / 0.062255859375 ) + 0.5;
					}
					else if( Timer().Period() < 3.984375 )
					{
						AValueControl = 0x81;
						AValue = ( Timer().Period() * 255 / 3.984375 ) + 0.5;
					}
					else if( Timer().Period() < 255 )
					{
						AValueControl = 0x82;
						AValue = Timer().Period() + 0.5;
					}
					else
					{
						AValueControl = 0x83;
						if( Timer().Period() <= 15300 )
							AValue = ( Timer().Period() * 255 / 15300 ) + 0.5;

						else
							AValue = 255;
					}
				}
			}

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( RTCC_TIMER1_ADDR );
			C_I2C.write( AValueControl );
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

		void UpdateControl2()
		{
			uint8_t	AValue = ( Timer().InterruptEnabled() ) ? 1 : 0;
			AValue |= ( Alarm().InterruptEnabled() ) ? 0b10 : 0;
			AValue |= ( Timer().PulseInterruptEnabled() ) ? 0b10000 : 0;

			writeRegister( RTCC_STAT2_ADDR, AValue );
		}

	public:
		void SetMilliSecond( uint8_t AValue )
		{
		}

		void SetSecond( uint8_t AValue )
		{
			writeRegister( RTCC_SEC_ADDR, AValue );
		}

		void SetMinute( uint8_t AValue )
		{
			writeRegister( RTCC_MIN_ADDR, Func::FromDecToBcd( AValue ) );
		}

		void SetHour( uint8_t AValue )
		{
			writeRegister( RTCC_HOUR_ADDR, Func::FromDecToBcd( AValue ) );
		}

		void SetWeekDay( uint8_t AValue )
		{
			writeRegister( RTCC_WEEKDAY_ADDR, Func::FromDecToBcd( AValue ) );
		}

		void SetYear( uint16_t AValue )
		{
			writeRegister( RTCC_YEAR_ADDR, Func::FromDecToBcd( AValue % 100 ) );
		}

		Mitov::TDateTime GetDateTime()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( RTCC_SEC_ADDR );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue()), (uint8_t)7 );

			uint8_t  ASecondLowBattery = C_I2C.read();
			uint16_t AMinute = C_I2C.read();
			uint16_t AHour = C_I2C.read();
			uint16_t ADate = C_I2C.read();
			uint16_t AWeekDay = C_I2C.read();
			uint8_t  ACenturyMonth = C_I2C.read();
			uint16_t AYear = C_I2C.read(); // + 2000;

			uint16_t ASecond = Func::FromBcdToDec( ASecondLowBattery & 0x7F );
			AMinute = Func::FromBcdToDec( AMinute );
			AHour = Func::FromBcdToDec( AHour );
			ADate = Func::FromBcdToDec( ADate );
			uint16_t AMonth = Func::FromBcdToDec( ACenturyMonth & 0b11111 );
			AYear = Func::FromBcdToDec( AYear ); // + 2000;

			if( ACenturyMonth & 0x80 )
				AYear += 2100;

			else
				AYear += 2000;

			Mitov::TDateTime ADateTime;
			ADateTime.TryEncodeDateTime( AYear, AMonth, ADate, AHour, AMinute, ASecond, 0 );
			return ADateTime;
		}

		void SetDateTime( const Mitov::TDateTime &ADateTime )
		{
			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( RTCC_SEC_ADDR );

//			C_I2C.write( ( ( Halt & 1 ) << 7 ) | ( Func::FromDecToBcd( ASecond ) & 0x7F ));
			C_I2C.write( Func::FromDecToBcd( ASecond ) & 0x7F );
			C_I2C.write(Func::FromDecToBcd( AMinute ));
			C_I2C.write(Func::FromDecToBcd( AHour ));
			C_I2C.write(Func::FromDecToBcd( ADay ));
			C_I2C.write(Func::FromDecToBcd( AWeekDay ));
			if( AYear >= 2100 )
				C_I2C.write( 0x80 | Func::FromDecToBcd( AMonth ));

			else
				C_I2C.write(Func::FromDecToBcd( AMonth ));

			C_I2C.write(Func::FromDecToBcd( AYear % 100 ));

			C_I2C.endTransmission();
		}

	protected:
		uint8_t readRegister(const uint8_t reg)
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( reg );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue()), (uint8_t)1 );

			return C_I2C.read();
		}

		void writeRegister( const uint8_t reg, const uint8_t value )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( reg );
			C_I2C.write( value );
			C_I2C.endTransmission();
		}

		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

			if( ! T_OutputPin::GetPinIsConnected() )
				return;

//			Serial.println( "ReadTime" );

			// Start at beginning, read entire memory in one go
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( RTCC_SEC_ADDR );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue()), (uint8_t)7 );

			uint8_t  ASecondLowBattery = C_I2C.read();
			uint16_t AMinute = C_I2C.read();
			uint16_t AHour = C_I2C.read();
			uint16_t ADate = C_I2C.read();
			uint16_t AWeekDay = C_I2C.read();
			uint8_t  ACenturyMonth = C_I2C.read();
			uint16_t AYear = C_I2C.read(); // + 2000;

			uint16_t ASecond = Func::FromBcdToDec( ASecondLowBattery & 0x7F );
			AMinute = Func::FromBcdToDec( AMinute );
			AHour = Func::FromBcdToDec( AHour );
			ADate = Func::FromBcdToDec( ADate );
			uint16_t AMonth = Func::FromBcdToDec( ACenturyMonth & 0b11111 );
			AYear = Func::FromBcdToDec( AYear ); // + 2000;

			if( ACenturyMonth & 0x80 )
				AYear += 2100;

			else
				AYear += 2000;

			Mitov::TDateTime ADateTime;

//			Serial.println( AMonth );

			if( ADateTime.TryEncodeDateTime( AYear, AMonth, ADate, AHour, AMinute, ASecond, 0 ))
				T_OutputPin::SetPinValue( ADateTime, ! AFromClock );

//#ifdef _MITOV_PCF8563_RTC_LOW_VOLTAGE_PIN_
			T_LowVoltageOutputPin::SetPinValue( ASecondLowBattery & 0x80 );
//#endif // _MITOV_PCF8563_RTC_LOW_VOLTAGE_PIN_

		}


	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( true );
		}

		inline void SetInputPin_o_Receive( void *_Data )
		{
//			if( WriteProtect )
//				return;

			Mitov::TDateTime &ADateTime = *(Mitov::TDateTime *)_Data;
			SetDateTime( ADateTime );
		}

	public:
		inline void SystemInit()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));    // Issue I2C start signal
			C_I2C.write( uint8_t( 0x0 ));        // start address

			C_I2C.write( uint8_t( 0x0 ));     //control/status1
			C_I2C.write( uint8_t( 0x0 ));     //control/status2
			C_I2C.endTransmission();
/*
			C_I2C.write( uint8_t( 0x81 ));     //set seconds & VL
			C_I2C.write( uint8_t( 0x01 ));    //set minutes
			C_I2C.write( uint8_t( 0x01 ));    //set hour
			C_I2C.write( uint8_t( 0x01 ));    //set day
			C_I2C.write( uint8_t( 0x01 ));    //set weekday
			C_I2C.write( uint8_t( 0x01 ));     //set month, century to 1
			C_I2C.write( uint8_t( 0x01 ));    //set year to 99
			C_I2C.write( uint8_t( 0x80 ));    //minute alarm value reset to 00
			C_I2C.write( uint8_t( 0x80 ));    //hour alarm value reset to 00
			C_I2C.write( uint8_t( 0x80 ));    //day alarm value reset to 00
			C_I2C.write( uint8_t( 0x80 ));    //weekday alarm value reset to 00
*/
//			Serial.println( "TEST" );
			// Update Alarm
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( RTCC_ALRM_MIN_ADDR );
			if( Alarm().Minute().Enabled() )
				C_I2C.write( Func::FromDecToBcd( Alarm().Minute().Value() ) );

			else
				C_I2C.write( 0x80 );

			if( Alarm().Hour().Enabled() )
				C_I2C.write( Func::FromDecToBcd( Alarm().Hour().Value() ) );

			else
				C_I2C.write( 0x80 );

			if( Alarm().Day().Enabled() )
				C_I2C.write( Func::FromDecToBcd( Alarm().Day().Value() ) );

			else
				C_I2C.write( 0x80 );


			if( Alarm().WeekDay().Enabled() )
				C_I2C.write( Func::FromDecToBcd( Alarm().WeekDay().Value() ) );

			else
				C_I2C.write( 0x80 );

			C_I2C.endTransmission();

/*
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));    // Issue I2C start signal
			C_I2C.write( RTCC_SQW_ADDR );
			C_I2C.write( uint8_t( 0x0 ));     //set SQW, see: setSquareWave
			C_I2C.write( uint8_t( 0x0 ));     //timer off
			C_I2C.endTransmission();
*/
			UpdateClockOut();
			UpdateTimer();
			UpdateControl2();

//			Serial.println( "TEST2" );
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() )
			{
//#ifdef _MITOV_PCF8563_RTC_CLOCK_PIN_
				if( ! ClockInputPin_o_IsConnected() )
//#endif // _MITOV_PCF8563_RTC_CLOCK_PIN_
					ReadTime( false );

#if ( defined( _MITOV_PCF8563_RTC_ALARM_PIN_ ) || defined( _MITOV_PCF8563_RTC_TIMER_PIN_ ))
				uint8_t  AStatus = readRegister( RTCC_STAT2_ADDR );

#ifdef _MITOV_PCF8563_RTC_ALARM_PIN_
				if( AStatus & 0b1000 ) // Check Alarm
				{
					// Clear Alarm
					writeRegister( RTCC_STAT2_ADDR, AStatus & ( ~0b1000 ) );
					T_AlarmOutputPin::ClockPin();
				}
#endif // _MITOV_PCF8563_RTC_ALARM_PIN_

#ifdef _MITOV_PCF8563_RTC_TIMER_PIN_
				if( AStatus & 0b100 ) // Check Interrupt
				{
					// Clear Interrupt
					writeRegister( RTCC_STAT2_ADDR, AStatus & ( ~0b100 ) );
					T_TimerOutputPin::ClockPin();
				}

#endif // _MITOV_PCF8563_RTC_TIMER_PIN_

#endif // ( defined( _MITOV_PCF8563_RTC_ALARM_PIN_ ) || defined( _MITOV_PCF8563_RTC_TIMER_PIN_ ))
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif