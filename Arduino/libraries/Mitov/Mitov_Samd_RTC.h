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
#ifdef VISUINO_SAMD51
	#include <Mitov_RTCZero.h>
#else
	#include <RTCZero.h>
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
  namespace TArduino_Samd_RTC_AlarmMode 
  {
	  enum TArduino_Samd_RTC_AlarmMode 
	  {
		   OncePerMinute,
		   OncePerHour,
		   OncePerDay,
		   OncePerMonth,
		   OncePerYear,
		   ExactTime
	  };
  }
//---------------------------------------------------------------------------
	template <
		typename T_AlarmOutputPin,
		typename T_Date,
		typename T_Enabled,
		typename T_Hour,
		typename T_Minute,
		typename T_Mode,
		typename T_Second
	> class TArduino_Samd_RTC_Alarm :
		public T_AlarmOutputPin,
		public T_Date,
		public T_Enabled,
		public T_Hour,
		public T_Minute,
		public T_Mode,
		public T_Second
	{
	public:
		_V_PIN_( AlarmOutputPin )

	public:
		_V_PROP_( Date )
		_V_PROP_( Enabled )
		_V_PROP_( Hour )
		_V_PROP_( Minute )
		_V_PROP_( Mode )
		_V_PROP_( Second )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Alarm,
		typename T_Enabled,
		typename T_OutputPin
	> class Samd_RTC :
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
		RTCZero FRtc;

	protected:
		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

#ifdef SEEEDUINO_WIO_TERMINAL
			FRtc.RTCreadRequest();
#endif
			uint8_t AYear = FRtc.getYear();
			uint8_t AMonth = FRtc.getMonth();
			uint8_t ADay = FRtc.getDay();

			uint8_t AHours = FRtc.getHours();
			uint8_t AMinutes = FRtc.getMinutes();
			uint8_t ASeconds = FRtc.getSeconds();

			Mitov::TDateTime ADateTime;

			if( ADateTime.TryEncodeDateTime( VISUINO_BASE_YEAR + AYear, AMonth, ADay, AHours, AMinutes, ASeconds, 0 ))
				T_OutputPin::SetPinValue( ADateTime, ! AFromClock );

		}

	public:
		Mitov::TDateTime GetDateTime()
		{
			Mitov::TDateTime ADateTime;

			uint8_t AYear = FRtc.getYear();
			uint8_t AMonth = FRtc.getMonth();
			uint8_t ADay = FRtc.getDay();

			uint8_t AHours = FRtc.getHours();
			uint8_t AMinutes = FRtc.getMinutes();
			uint8_t ASeconds = FRtc.getSeconds();

			ADateTime.TryEncodeDateTime( VISUINO_BASE_YEAR + AYear, AMonth, ADay, AHours, AMinutes, ASeconds, 0 );

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

			FRtc.setYear( AYear );
			FRtc.setMonth( AMonth );
			FRtc.setDay( ADay );

			FRtc.setHours( AHour );
			FRtc.setMinutes( AMinute );
			FRtc.setSeconds( ASecond );
#ifdef SEEEDUINO_WIO_TERMINAL
			FRtc.UpdateTime();
#endif
		}

		inline void SetMilliSecond( uint8_t AValue )
		{
		}

		inline void SetSecond( uint8_t AValue )
		{
			FRtc.setSeconds( AValue );
#ifdef SEEEDUINO_WIO_TERMINAL
			FRtc.UpdateTime();
#endif
		}

		inline void SetMinute( uint8_t AValue )
		{
			FRtc.setMinutes( AValue );
#ifdef SEEEDUINO_WIO_TERMINAL
			FRtc.UpdateTime();
#endif
		}

		inline void SetHour( uint8_t AValue )
		{
			FRtc.setHours( AValue );
#ifdef SEEEDUINO_WIO_TERMINAL
			FRtc.UpdateTime();
#endif
		}

		inline void SetWeekDay( uint8_t AValue )
		{
		}

		inline void SetYear( uint16_t AValue )
		{
			FRtc.setYear( AValue );
#ifdef SEEEDUINO_WIO_TERMINAL
			FRtc.UpdateTime();
#endif
		}

	public:
		inline void Alarm_SleepInputPin_o_Receive( void *_Data )
		{
			FRtc.standbyMode();
		}

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

	protected:
		static void *FInstance;

	protected:
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

	public:
		inline void SystemInit()
		{
			FInstance = this;
			FRtc.begin();

			if( Alarm().AlarmOutputPin().GetPinIsConnected() )
				FRtc.attachInterrupt( OnInternalAlarmStatic );

		}

		inline void SystemStart()
		{
            UpdateAlarmSecond();
		    UpdateAlarmMinute();
            UpdateAlarmHour();
            UpdateAlarmDate();
			UpdateAlarmMode();
			ReadTime( true );
		}

		inline void SystemLoopBegin()
		{
			ReadTime( false );
		}

	};
//---------------------------------------------------------------------------
template <
		typename T_Alarm,
		typename T_Enabled,
		typename T_OutputPin
	> void * Samd_RTC<
		T_Alarm,
		T_Enabled,
		T_OutputPin
		>::FInstance = nullptr;
//---------------------------------------------------------------------------
}

