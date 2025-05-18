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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class ControllinoRTCModule :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				StartModule();

//			else
//				StopModule();

		}

		void SetMilliSecond( uint8_t AValue )
		{
		}

		void SetSecond( uint8_t AValue )
		{
			TDateTime ADateTime = GetDateTime();
			uint16_t AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond );
			Controllino_SetTimeDate( ADay, aWeekDay, AMonth, AYear, AHour, AMinute, AValue );
		}

		void SetMinute( uint8_t AValue )
		{
			TDateTime ADateTime = GetDateTime();
			uint16_t AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond );
			Controllino_SetTimeDate( ADay, aWeekDay, AMonth, AYear, AHour, AValue, ASecond );
		}

		void SetHour( uint8_t AValue )
		{
			TDateTime ADateTime = GetDateTime();
			uint16_t AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond );
			Controllino_SetTimeDate( ADay, aWeekDay, AMonth, AYear, AValue, AMinute, ASecond );
		}

		void SetWeekDay( uint8_t AValue )
		{
			TDateTime ADateTime = GetDateTime();
			uint16_t AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond );
			Controllino_SetTimeDate( ADay, AValue, AMonth, AYear, AHour, AMinute, ASecond );
		}

		void SetYear( uint16_t AValue )
		{
			TDateTime ADateTime = GetDateTime();
			uint16_t AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond );
			Controllino_SetTimeDate( ADay, aWeekDay, AMonth, AValue, AHour, AMinute, ASecond );
		}

		Mitov::TDateTime GetDateTime()
		{
			unsigned char aDay, aWeekDay, aMonth, aYear, aHour, aMinute, aSecond;
//			Controllino_PrintTimeAndDate();

			Controllino_ReadTimeDate( &aDay, &aWeekDay, &aMonth, &aYear, &aHour, &aMinute, &aSecond );
//			Serial.println( "Test2" );
			uint16_t ALongYear = 2000 + aYear;
//			Serial.println( ALongYear );
//			Serial.println( aMonth );
//			Serial.println( aDay );
			TDateTime ADateTime;
//			ADateTime.TryEncodeDate( 2000 + aYear, aMonth, aDay );
//			ADateTime.TryEncodeTime( aHour, aMinute, aSecond, 0 );
			ADateTime.TryEncodeDateTime( ALongYear, aMonth, aDay, aHour, aMinute, aSecond, 0 );

			return ADateTime;
		}

		void SetDateTime( const Mitov::TDateTime &ADateTime )
		{
			uint16_t AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond );
			Controllino_SetTimeDate( ADay, aWeekDay, AMonth, AYear, AHour, AMinute, ASecond );
		}


	protected:
/*
		void StopModule()
		{
		}
*/
		void StartModule()
		{
//			Serial.println( "START" );
//			SPI.begin();
//			Controllino_RTC_init( 65 );
			Controllino_RTC_init( 0 );
//			Controllino_RTCSSInit();
//			Controllino_SetRTCSS( true );
		}

		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

			if( ! T_OutputPin::GetPinIsConnected() )
				return;

//			Serial.println( "Test1" );
			unsigned char aDay, aWeekDay, aMonth, aYear, aHour, aMinute, aSecond;
//			Controllino_PrintTimeAndDate();

			Controllino_ReadTimeDate( &aDay, &aWeekDay, &aMonth, &aYear, &aHour, &aMinute, &aSecond );
//			Serial.println( "Test2" );
			uint16_t ALongYear = 2000 + aYear;
//			Serial.println( ALongYear );
//			Serial.println( aMonth );
//			Serial.println( aDay );
			TDateTime ADateTime;
//			ADateTime.TryEncodeDate( 2000 + aYear, aMonth, aDay );
//			ADateTime.TryEncodeTime( aHour, aMinute, aSecond, 0 );
			if( ADateTime.TryEncodeDateTime( ALongYear, aMonth, aDay, aHour, aMinute, aSecond, 0 ))
				T_OutputPin::SetPinValue( ADateTime );

		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartModule();

		}

		inline void SystemLoopBegin()
		{
			ReadTime( false );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( true );
		}

		void SetInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			uint16_t AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond;

//			Serial.println( "Test2" );
			((TDateTime *)_Data)->DecodeDateTime( AYear, AMonth, ADay, aWeekDay, AHour, AMinute, ASecond, AMilliSecond );
			AYear %= 100;
			Controllino_SetTimeDate( ADay, aWeekDay, AMonth, AYear, AHour, AMinute, ASecond );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif