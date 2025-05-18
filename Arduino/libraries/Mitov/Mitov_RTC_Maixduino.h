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
#include <rtc.h>
#include <time.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class RTC_Maixduino :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void SetMilliSecond( uint8_t AValue )
		{
		}

		void SetSecond( uint8_t AValue )
		{
			int ASecond;
			int AMinute;
			int AHour;
			int ADate;
			int AMonth;
			int AYear;

			rtc_timer_get( &AYear, &AMonth, &ADate, &AHour, &AMinute, &ASecond );
			rtc_timer_set( AYear, AMonth, ADate, AHour, AMinute, int( AValue ));
		}

		void SetMinute( uint8_t AValue )
		{
			int ASecond;
			int AMinute;
			int AHour;
			int ADate;
			int AMonth;
			int AYear;

			rtc_timer_get( &AYear, &AMonth, &ADate, &AHour, &AMinute, &ASecond );
			rtc_timer_set( AYear, AMonth, ADate, AHour, int( AValue ), ASecond );
		}

		void SetHour( uint8_t AValue )
		{
			int ASecond;
			int AMinute;
			int AHour;
			int ADate;
			int AMonth;
			int AYear;

			rtc_timer_get( &AYear, &AMonth, &ADate, &AHour, &AMinute, &ASecond );
			rtc_timer_set( AYear, AMonth, ADate, int( AValue ), AMinute, ASecond );
		}

		void SetWeekDay( uint8_t AValue )
		{
		}

		void SetYear( uint16_t AValue )
		{
			int ASecond;
			int AMinute;
			int AHour;
			int ADate;
			int AMonth;
			int AYear;

			rtc_timer_get( &AYear, &AMonth, &ADate, &AHour, &AMinute, &ASecond );
			rtc_timer_set( int( AValue ), AMonth, ADate, AHour, AMinute, ASecond );
		}

		Mitov::TDateTime GetDateTime()
		{
			int ASecond;
			int AMinute;
			int AHour;
			int ADate;
			int AMonth;
			int AYear;

			rtc_timer_get( &AYear, &AMonth, &ADate, &AHour, &AMinute, &ASecond );

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

			rtc_timer_set( int( AYear ), int( AMonth ), int( ADay ), int( AHour ), int( AMinute ), int( ASecond ));
		}

	protected:
		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

			int ASecond;
			int AMinute;
			int AHour;
			int ADate;
			int AMonth;
//			int ADay = weekday( ATime );
			int AYear;

			rtc_timer_get( &AYear, &AMonth, &ADate, &AHour, &AMinute, &ASecond );

			delay( 1 );
//			Serial.println( ASecond );

			Mitov::TDateTime ADateTime;

			if( ADateTime.TryEncodeDateTime( AYear, AMonth, ADate, AHour, AMinute, ASecond, 0 ))
				T_OutputPin::SetPinValue( ADateTime, ! AFromClock );


		}


	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( true );
		}

		inline void SetInputPin_o_Receive( void *_Data )
		{
			Mitov::TDateTime &ADateTime = *(Mitov::TDateTime *)_Data;

			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

			rtc_timer_set( AYear, AMonth, ADay, AHour, AMinute, ASecond );
/*
			Mitov::TDateTime &ADateTime = *(Mitov::TDateTime *)_Data;

			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

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
		inline void SystemLoopBegin()
		{
			ReadTime( false );
		}

		inline void SystemInit()
		{
			rtc_init();

			rtc_timer_set( 2020, 1, 1, 0, 0, 0 );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

