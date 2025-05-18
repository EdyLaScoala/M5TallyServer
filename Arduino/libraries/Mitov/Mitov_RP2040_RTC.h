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
//#include "rtc_api.h"

extern "C" 
{
  typedef void(* rtc_callback_t )(void);
  
  void rtc_init           (void);
  bool rtc_set_datetime   (datetime_t *t);
  bool rtc_get_datetime   (datetime_t *t);
  void rtc_set_alarm      (datetime_t *t, rtc_callback_t user_callback);
  void rtc_disable_alarm  (void);
//  void rtc_enable_alarm   (void);

  bool rtc_alarm_repeats(datetime_t *t);

  void datetime_to_str(char *buf, uint buf_size, const datetime_t *t);
}

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_AlarmOutputPin,
		typename T_Day,
		typename T_Enabled,
		typename T_FAlarm,
		typename T_Hour,
		typename T_Minute,
		typename T_Month,
		typename T_Second,
		typename T_Year
	> class TArduino_RP2040_RTC_Alarm :
		public T_AlarmOutputPin,
		public T_Day,
		public T_Enabled,
		public T_FAlarm,
		public T_Hour,
		public T_Minute,
		public T_Month,
		public T_Second,
		public T_Year
	{
	public:
		_V_PIN_( AlarmOutputPin )

	protected:
		static void *FInstance;

	public:
		_V_PROP_( Day )
		_V_PROP_( Enabled )
		_V_PROP_( Hour )
		_V_PROP_( Minute )
		_V_PROP_( Month )
		_V_PROP_( Second )
		_V_PROP_( Year )

	protected:
		_V_PROP_( FAlarm )

	public:
		void UpdateEnabled()
		{
			// RTC is also used to wakeup from sleep, so enable even if theer are no pins connected
//			if( ! Alarm().AlarmOutputPin().GetPinIsConnected() )
//				return;

			if( Enabled() )
				UpdateAlarmTime();
//				rtc_enable_alarm();

			else
				rtc_disable_alarm();

		}

		void UpdateAlarmTime()
		{
			datetime_t ADate;

			ADate.day = ( Day().Enabled() ) ? Day().Value() : -1;
			ADate.hour = ( Hour().Enabled() ) ? Hour().Value() : -1;
			ADate.min = ( Minute().Enabled() ) ? Minute().Value() : -1;
			ADate.month = ( Month().Enabled() ) ? Month().Value() : -1;
			ADate.sec = ( Second().Enabled() ) ? Second().Value() : -1;
			ADate.year = ( Year().Enabled() ) ? Year().Value() : -1;

//			Serial.println( "UpdateAlarmTime" );
			rtc_set_alarm( &ADate, OnInternalAlarmStatic );
		}

		inline void UpdateMinuteAlarm()
		{
			UpdateAlarmTime();
		}

		inline void UpdateYearAlarm()
		{
			UpdateAlarmTime();
		}

		inline void UpdateHourAlarm()
		{
			UpdateHourAlarm();
		}

		inline void UpdateMonthAlarm()
		{
			UpdateHourAlarm();
		}

		inline void UpdateDayAlarm()
		{
			UpdateHourAlarm();
		}

	protected:
		static void OnInternalAlarmStatic()
		{
			((TArduino_RP2040_RTC_Alarm *)FInstance )->FAlarm() = true;
//			Serial.println( "ALARM" );
		}

	public:
		inline void Init()
		{
//			UpdateAlarmTime();
			UpdateEnabled();
		}

		inline void LoopBegin()
		{
			if( FAlarm() )
			{
				FAlarm() = false;
				AlarmOutputPin().ClockPin();
			}
		}

	public:
        TArduino_RP2040_RTC_Alarm()
        {
			FAlarm() = false;
			FInstance = this;
        }
	};
//---------------------------------------------------------------------------
template <
		typename T_AlarmOutputPin,
		typename T_Day,
		typename T_Enabled,
		typename T_FAlarm,
		typename T_Hour,
		typename T_Minute,
		typename T_Month,
		typename T_Second,
		typename T_Year
	> void * TArduino_RP2040_RTC_Alarm<
		T_AlarmOutputPin,
		T_Day,
		T_Enabled,
		T_FAlarm,
		T_Hour,
		T_Minute,
		T_Month,
		T_Second,
		T_Year
		>::FInstance = nullptr;
//---------------------------------------------------------------------------
	template <
		typename T_Alarm,
		typename T_Enabled,
		typename T_OutputPin
	> class RP2040_RTC :
		public T_Alarm,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Alarm )
		_V_PROP_( Enabled )

	protected:
		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

			datetime_t ATime;
			rtc_get_datetime( &ATime );

			uint8_t AYear = ATime.year;
			uint8_t AMonth = ATime.month;
			uint8_t ADay = ATime.day;

			uint8_t AHours = ATime.hour;
			uint8_t AMinutes = ATime.min;
			uint8_t ASeconds = ATime.sec;

//			Serial.println( AYear );

			Mitov::TDateTime ADateTime;

			if( ADateTime.TryEncodeDateTime( AYear, AMonth, ADay, AHours, AMinutes, ASeconds, 0 ))
				T_OutputPin::SetPinValue( ADateTime, ! AFromClock );

		}

	public:
		Mitov::TDateTime GetDateTime()
		{
			datetime_t ATime;
			rtc_get_datetime( &ATime );

			uint8_t AYear = ATime.year;
			uint8_t AMonth = ATime.month;
			uint8_t ADay = ATime.day;

			uint8_t AHours = ATime.hour;
			uint8_t AMinutes = ATime.min;
			uint8_t ASeconds = ATime.sec;

			Mitov::TDateTime ADateTime;
			ADateTime.TryEncodeDateTime( AYear, AMonth, ADay, AHours, AMinutes, ASeconds, 0 );

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

//			AYear %= 100;

			datetime_t ATime;
			ATime.year = AYear;
			ATime.month = AMonth;
			ATime.day = ADay;

			ATime.hour = AHour;
			ATime.min = AMinute;
			ATime.sec = ASecond;

			rtc_set_datetime( &ATime );
		}

		inline void SetMilliSecond( uint8_t AValue )
		{
		}

		inline void SetSecond( uint8_t AValue )
		{
			datetime_t ATime;
			rtc_get_datetime( &ATime );
			ATime.sec = AValue;
			rtc_set_datetime( &ATime );
		}

		inline void SetMinute( uint8_t AValue )
		{
			datetime_t ATime;
			rtc_get_datetime( &ATime );
			ATime.min = AValue;
			rtc_set_datetime( &ATime );
		}

		inline void SetHour( uint8_t AValue )
		{
			datetime_t ATime;
			rtc_get_datetime( &ATime );
			ATime.hour = AValue;
			rtc_set_datetime( &ATime );
		}

		inline void SetWeekDay( uint8_t AValue )
		{
		}

		inline void SetYear( uint16_t AValue )
		{
			datetime_t ATime;
			rtc_get_datetime( &ATime );
			ATime.year = AValue;
			rtc_set_datetime( &ATime );
		}

	public:
/*
		inline void Alarm_SleepInputPin_o_Receive( void *_Data )
		{
			FRtc.standbyMode();
		}
*/
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( true );
		}

		inline void SetInputPin_o_Receive( void *_Data )
		{
			Mitov::TDateTime &ADateTime = *(Mitov::TDateTime *)_Data;
			SetDateTime( ADateTime );
		}

	public:
		inline void UpdateEnabled() {} // Placeholder

/*
		inline void UpdateAlarmSecond()
		{
			FRtc.setAlarmSeconds( Alarm().Second() );
		}

		inline void UpdateAlarmMinute()
		{
			FRtc.setAlarmMinutes( Alarm().Minute() );
		}

		inline void UpdateAlarmHour()
		{
			FRtc.setAlarmHours( Alarm().Hour() );
		}

		inline void UpdateAlarmDate()
		{
			FRtc.setAlarmDay( Alarm().Date() );
		}

		void UpdateAlarmMode()
		{
			RTCZero::Alarm_Match AMode;
			if( ! Alarm().Enabled().GetValue() )
				AMode = RTCZero::MATCH_OFF;

			else
			{
				switch( Alarm().Mode().GetValue() )
				{
					case TArduino_Samd_RTC_AlarmMode::OncePerMinute : AMode = RTCZero::MATCH_SS;
					case TArduino_Samd_RTC_AlarmMode::OncePerHour : AMode = RTCZero::MATCH_MMSS;
					case TArduino_Samd_RTC_AlarmMode::OncePerDay : AMode = RTCZero::MATCH_HHMMSS;
					case TArduino_Samd_RTC_AlarmMode::OncePerMonth : AMode = RTCZero::MATCH_DHHMMSS;
					case TArduino_Samd_RTC_AlarmMode::OncePerYear : AMode = RTCZero::MATCH_MMDDHHMMSS;
					default : AMode = RTCZero::MATCH_YYMMDDHHMMSS;
				}
			}

			FRtc.enableAlarm( AMode );
		}
*/
	protected:
/*
		void OnInternalAlarm()
		{
			Alarm().AlarmOutputPin().ClockPin();
		}

		static void OnInternalAlarmStatic()
		{
			((Samd_RTC *)FInstance )->OnInternalAlarm();
//			T_Alarm::T_DEF_AlarmOutputPin::ClockPin();
//			Serial.println( "ALARM" );
//			((Samd_RTC *)AInstance )->OnInternalAlarm();
		}
*/
	public:
		inline void SystemInit()
		{
  			rtc_init();
//			FRtc.begin();

			Alarm().Init();
//			if( Alarm().AlarmOutputPin().GetPinIsConnected() )
//				FRtc.attachInterrupt( OnInternalAlarmStatic );

		}

		inline void SystemStart()
		{
/*
            UpdateAlarmSecond();
		    UpdateAlarmMinute();
            UpdateAlarmHour();
            UpdateAlarmDate();
			UpdateAlarmMode();
*/
			ReadTime( true );
		}

		inline void SystemLoopBegin()
		{
			Alarm().LoopBegin();
			ReadTime( false );
		}

	};
//---------------------------------------------------------------------------
}

