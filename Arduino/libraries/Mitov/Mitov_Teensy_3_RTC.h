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
#include <TimeLib.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Compensate,
		typename T_Enabled,
		typename T_OutputPin
	> class Teensy3RTC :
		public T_Compensate,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

		_V_PROP_( Compensate )

	public:
		inline void UpdateCompensate()
		{
			if( Compensate().GetValue() )
			{
				float APPM = Compensate() * 1000000;
				Teensy3Clock.compensate( ( APPM / 8 ) + 0.5 );
			}
		}

	public:
		inline void SetMilliSecond( uint8_t AValue )
		{
		}

		void SetSecond( uint8_t AValue )
		{
			time_t ATime = Teensy3Clock.get();
			tmElements_t AElements;
			breakTime( ATime, AElements );
			AElements.Second = AValue;
			Teensy3Clock.set( makeTime( AElements ) );
		}

		void SetMinute( uint8_t AValue )
		{
			time_t ATime = Teensy3Clock.get();
			tmElements_t AElements;
			breakTime( ATime, AElements );
			AElements.Minute = AValue;
			Teensy3Clock.set( makeTime( AElements ) );
		}

		void SetHour( uint8_t AValue )
		{
			time_t ATime = Teensy3Clock.get();
			tmElements_t AElements;
			breakTime( ATime, AElements );
			AElements.Hour = AValue;
			Teensy3Clock.set( makeTime( AElements ) );
		}

		void SetWeekDay( uint8_t AValue )
		{
			time_t ATime = Teensy3Clock.get();
			tmElements_t AElements;
			breakTime( ATime, AElements );
			AElements.Wday = AValue;
			Teensy3Clock.set( makeTime( AElements ) );			
		}

		void SetYear( uint16_t AValue )
		{
			time_t ATime = Teensy3Clock.get();
			tmElements_t AElements;
			breakTime( ATime, AElements );
			AElements.Year = AValue - 1970;
			Teensy3Clock.set( makeTime( AElements ) );
		}

		Mitov::TDateTime GetDateTime()
		{
			time_t ATime = Teensy3Clock.get();
			uint16_t ASecond = second( ATime );
			uint16_t AMinute = minute( ATime );
			uint16_t AHour = hour( ATime );
			uint16_t ADate = day( ATime );
			uint16_t AMonth = month( ATime );
			uint16_t AYear = year( ATime );
			
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
		}

	protected:
		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

			time_t ATime = Teensy3Clock.get();
			uint16_t ASecond = second( ATime );
			uint16_t AMinute = minute( ATime );
			uint16_t AHour = hour( ATime );
			uint16_t ADate = day( ATime );
			uint16_t AMonth = month( ATime );
//			uint16_t ADay = weekday( ATime );
			uint16_t AYear = year( ATime );
			
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
		}

	public:
		inline void SystemInit()
		{
//			if( Enabled )
			UpdateCompensate();
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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif