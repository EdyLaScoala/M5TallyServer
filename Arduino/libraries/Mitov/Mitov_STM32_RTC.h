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
//---------------------------------------------------------------------------
	template <
		typename T_External,
		typename T_HighSpeed
	> class TArduinoSTM32_RTC_ClockSource :
		public T_External,
		public T_HighSpeed
	{
	public:
		_V_PROP_( External )
		_V_PROP_( HighSpeed )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Hour,
		typename T_MilliSecond,
		typename T_Minute,
		typename T_OutputPin,
		typename T_Second,
		typename T_WeekDay
	> class TArduinoSTM32_RTCAlarm :
		public T_Enabled,
		public T_Hour,
		public T_MilliSecond,
		public T_Minute,
		public T_OutputPin,
		public T_Second,
		public T_WeekDay
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Hour )
		_V_PROP_( MilliSecond )
		_V_PROP_( Minute )
		_V_PROP_( Second )
		_V_PROP_( WeekDay )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Alarm,
		typename T_ClockSource,
		typename T_Enabled,
		typename T_OutputPin
	> class STM32_RTC :
		public T_Alarm,
		public T_ClockSource,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Alarm )
		_V_PROP_( ClockSource )
		_V_PROP_( Enabled )

	protected:
		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

			uint8_t AHours;
			uint8_t AMinutes;
			uint8_t ASeconds;
			uint32_t ASubSeconds;
			hourAM_PM_t APeriod;

			RTC_GetTime( &AHours, &AMinutes, &ASeconds, &ASubSeconds, &APeriod );


			uint8_t AYear;
			uint8_t AMonth;
			uint8_t ADay;
			uint8_t AWeekDay;

			RTC_GetDate( &AYear, &AMonth, &ADay, &AWeekDay );

//			Serial.println( ASubSeconds );

			Mitov::TDateTime ADateTime;

			uint16_t AMilliSeconds = uint32_t( 999 ) * ( ASubSeconds >> 16 ) / 0xFFFF;
//			uint16_t AMilliSeconds = uint32_t( 999 ) * ( 0xFFFF ) / 0xFFFF;
//			Serial.println( AMilliSeconds );

			if( ADateTime.TryEncodeDateTime( VISUINO_BASE_YEAR + AYear, AMonth, ADay, AHours, AMinutes, ASeconds, AMilliSeconds ))
				T_OutputPin::SetPinValue( ADateTime, ! AFromClock );

		}

	public:
		Mitov::TDateTime GetDateTime()
		{
			uint8_t AHours;
			uint8_t AMinutes;
			uint8_t ASeconds;
			uint32_t ASubSeconds;
			hourAM_PM_t APeriod;

			RTC_GetTime( &AHours, &AMinutes, &ASeconds, &ASubSeconds, &APeriod );


			uint8_t AYear;
			uint8_t AMonth;
			uint8_t ADay;
			uint8_t AWeekDay;

			RTC_GetDate( &AYear, &AMonth, &ADay, &AWeekDay );
			
			uint16_t AMilliSeconds = uint32_t( 999 ) * ( ASubSeconds >> 16 ) / 0xFFFF;

			Mitov::TDateTime ADateTime;

			ADateTime.TryEncodeDateTime( VISUINO_BASE_YEAR + AYear, AMonth, ADay, AHours, AMinutes, ASeconds, AMilliSeconds );

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

			AYear %= 100;

//			delay( 1000 );
//			Serial.println( AYear );

//			uint16_t AMilliSeconds = uint32_t( 999 ) * ( ASubSeconds >> 16 ) / 0xFFFF;

			uint32_t ASubSeconds =  ( AMilliSecond * uint32_t( 0xFFFF ) ) / 999;
			ASubSeconds <<= 16;

			RTC_SetTime( AHour, AMinute, ASecond, ASubSeconds, ( AHour > 11 ) ? HOUR_PM : HOUR_AM );
			RTC_SetDate( AYear, AMonth, ADay, AWeekDay );

		}

		void SetMilliSecond( uint8_t AValue )
		{
			uint8_t AHours;
			uint8_t AMinutes;
			uint8_t ASeconds;
			uint32_t ASubSeconds;
			hourAM_PM_t APeriod;

			RTC_GetTime( &AHours, &AMinutes, &ASeconds, &ASubSeconds, &APeriod );


			ASubSeconds =  ( AValue * uint32_t( 0xFFFF ) ) / 999;
			ASubSeconds <<= 16;

			RTC_SetTime( AHours, AMinutes, ASeconds, ASubSeconds, ( AHours > 11 ) ? HOUR_PM : HOUR_AM );
		}

		void SetSecond( uint8_t AValue )
		{
			uint8_t AHours;
			uint8_t AMinutes;
			uint8_t ASeconds;
			uint32_t ASubSeconds;
			hourAM_PM_t APeriod;

			RTC_GetTime( &AHours, &AMinutes, &ASeconds, &ASubSeconds, &APeriod );

			RTC_SetTime( AHours, AMinutes, AValue, ASubSeconds, ( AHours > 11 ) ? HOUR_PM : HOUR_AM );
		}

		void SetMinute( uint8_t AValue )
		{
			uint8_t AHours;
			uint8_t AMinutes;
			uint8_t ASeconds;
			uint32_t ASubSeconds;
			hourAM_PM_t APeriod;

			RTC_GetTime( &AHours, &AMinutes, &ASeconds, &ASubSeconds, &APeriod );

			RTC_SetTime( AHours, AValue, ASeconds, ASubSeconds, ( AHours > 11 ) ? HOUR_PM : HOUR_AM );
		}

		void SetHour( uint8_t AValue )
		{
			uint8_t AHours;
			uint8_t AMinutes;
			uint8_t ASeconds;
			uint32_t ASubSeconds;
			hourAM_PM_t APeriod;

			RTC_GetTime( &AHours, &AMinutes, &ASeconds, &ASubSeconds, &APeriod );

			RTC_SetTime( AValue, AMinutes, ASeconds, ASubSeconds, ( AHours > 11 ) ? HOUR_PM : HOUR_AM );
		}

		void SetWeekDay( uint8_t AValue )
		{
			uint8_t AYear;
			uint8_t AMonth;
			uint8_t ADay;
			uint8_t AWeekDay;

			RTC_GetDate( &AYear, &AMonth, &ADay, &AWeekDay );

			RTC_SetDate( AYear, AMonth, ADay, AValue );
		}

		void SetYear( uint16_t AValue )
		{
			uint8_t AYear;
			uint8_t AMonth;
			uint8_t ADay;
			uint8_t AWeekDay;

			RTC_GetDate( &AYear, &AMonth, &ADay, &AWeekDay );

			RTC_SetDate( AValue, AMonth, ADay, AWeekDay );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( true );
		}

		inline void SetInputPin_o_Receive( void *_Data )
		{
			Mitov::TDateTime &ADateTime = *(Mitov::TDateTime *)_Data;
			SetDateTime( ADateTime );

/*
			tmElements_t tm;
			tm.Second = ASecond; 
			tm.Minute = AMinute; 
			tm.Hour = AHour; 
			tm.Wday = AWeekDay + 1;   // day of week, sunday is day 1
			tm.Day = ADay;
			tm.Month = AMonth; 
			tm.Year = AYear - 1970;   // offset from 1970;
			
			time_t ATime = makeTime( tm );
			Teensy3Clock.set( ATime );
*/
		}

	public:
//		inline void UpdateEnableAlarm()
		void UpdateEnableAlarm()
		{
//			Serial.println( "TEST2" );
			if( Alarm().Enabled() )
			{
				uint8_t mask = ( Alarm().WeekDay().Enabled().GetValue() ) ? 0 : 0b1000;
				if( ! Alarm().Hour().Enabled().GetValue() )
					mask |= 0b0100;

				if( ! Alarm().Minute().Enabled().GetValue() )
					mask |= 0b0010;

				if( ! Alarm().Second().Enabled().GetValue() )
					mask |= 0b0001;

				uint32_t ASubSeconds =  ( Alarm().MilliSecond().GetValue() * uint32_t( 0xFFFF ) ) / 999;
				ASubSeconds <<= 16;

				uint8_t AWeekDay = Alarm().WeekDay().Value().GetValue();
				uint8_t AHour = Alarm().Hour().Value().GetValue();
				uint8_t AMinute = Alarm().Minute().Value().GetValue();
				uint8_t ASecond = Alarm().Second().Value().GetValue();

//				Serial.println( "UpdateEnableAlarm" );
//				Serial.println( mask, BIN );

//				Serial.println( "WeekDay" );
//				Serial.println( int( Alarm().WeekDay().Value().GetValue() ));

//				return;

//				Serial.println( "WeekDay" );
//				Serial.println( "Hour" );
//				Serial.println( int( Alarm().Hour().Value().GetValue() ));

//				Serial.println( "Minute" );
//				Serial.println( AMinute );

//				Serial.println( "Second" );
//				Serial.println( ASecond );

//				Serial.println( "ASubSeconds" );
//				Serial.println( ASubSeconds );

//				return;

				RTC_StartAlarm( AWeekDay, AHour, AMinute, ASecond, ASubSeconds, ( AHour > 11 ) ? HOUR_PM : HOUR_AM, mask );
			}

			else
				RTC_StopAlarm();

		}

		inline void UpdateMinuteAlarm() { UpdateEnableAlarm(); }
		inline void UpdateHourAlarm() { UpdateEnableAlarm(); }
		inline void UpdateWeekDayAlarm() { UpdateEnableAlarm(); }

	protected:
		void OnInternalAlarm()
		{
			Alarm().OutputPin().ClockPin();
		}

		static void OnInternalAlarmStatic( void *AInstance )
		{
//			Serial.println( "ALARM" );
			((STM32_RTC *)AInstance )->OnInternalAlarm();
		}

	public:
//		inline void SystemInit()
		void SystemInit()
		{
			sourceClock_t ASource;
			if( ClockSource().External() )
				ASource = ( ClockSource().HighSpeed() ) ? HSE_CLOCK : LSE_CLOCK;

			else
				ASource = ( ClockSource().HighSpeed() ) ? HSI_CLOCK : LSI_CLOCK;

			RTC_init( HOUR_FORMAT_24, ASource, false );

			delay( 1000 );

			if( Alarm().OutputPin().GetPinIsConnected() )
			{
//				Serial.println( "SystemInit 1" );
				attachAlarmCallback( OnInternalAlarmStatic, this );
				UpdateEnableAlarm();
//				attachAlarmCallback( OnInternalAlarmStatic, this );
			}

//			Serial.println( "SystemInit" );
		}

		inline void SystemStart()
		{
			ReadTime( true );
		}

		inline void SystemLoopBegin()
		{
			ReadTime( false );
		}

	};
//---------------------------------------------------------------------------
}

