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
	template <
		typename T_Enabled,
		typename T_FModified,
		typename T_InitialValue,
		typename T_OutputPin
    > class EncodeDateTime :
		public T_Enabled,
		public T_FModified,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( InitialValue )

	protected:
		_V_PROP_( FModified )

		int16_t	FValues[ 7 ] = { 0 };

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			int32_t	AValue = *(int32_t *)_Data;
			if( FValues[ AIndex ] == AValue )
				return;

			FValues[ AIndex ] = AValue;
			FModified() = true;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			TDateTime AValue = CalculateOutput();
			T_OutputPin::SetPinValue( AValue, false );
		}

	protected:
		inline void CalculateSendOutput( bool AFromStart )
		{
			TDateTime AValue = CalculateOutput();
			T_OutputPin::SetPinValue( AValue, ! AFromStart );
			FModified() = false;
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( false );

		}

	protected:
		inline TDateTime CalculateOutput()
		{
			TDateTime ADate;
			ADate.TryEncodeDateTime( FValues[ 0 ], FValues[ 1 ], FValues[ 2 ], FValues[ 3 ], FValues[ 4 ], FValues[ 5 ], FValues[ 6 ] );
			return ADate;
		}

	public:
		EncodeDateTime()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_Year,
		typename T_OutputPins_Month,
		typename T_OutputPins_Day,
		typename T_OutputPins_Hour,
		typename T_OutputPins_Minute,
		typename T_OutputPins_Second,
		typename T_OutputPins_MilliSecond,
		typename T_OutputPins_WeekDay
	> class DecodeDateTime :
		public T_OutputPins_Year,
		public T_OutputPins_Month,
		public T_OutputPins_Day,
		public T_OutputPins_Hour,
		public T_OutputPins_Minute,
		public T_OutputPins_Second,
		public T_OutputPins_MilliSecond,
		public T_OutputPins_WeekDay
	{
	public:
		_V_PIN_( OutputPins_Year )
		_V_PIN_( OutputPins_Month )
		_V_PIN_( OutputPins_Day )
		_V_PIN_( OutputPins_Hour )
		_V_PIN_( OutputPins_Minute )
		_V_PIN_( OutputPins_Second )
		_V_PIN_( OutputPins_MilliSecond )
		_V_PIN_( OutputPins_WeekDay )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TDateTime &ADate = *(TDateTime *)_Data;
			uint16_t AItem16[ 8 ];
			ADate.DecodeDateTime( AItem16[ 0 ], AItem16[ 1 ], AItem16[ 2 ], AItem16[ 7 ], AItem16[ 3 ], AItem16[ 4 ], AItem16[ 5 ], AItem16[ 6 ] );
			T_OutputPins_Year::SetPinValue( AItem16[ 0 ] );
			T_OutputPins_Month::SetPinValue( AItem16[ 1 ] );
			T_OutputPins_Day::SetPinValue( AItem16[ 2 ] );
			T_OutputPins_Hour::SetPinValue( AItem16[ 3 ] );
			T_OutputPins_Minute::SetPinValue( AItem16[ 4 ] );
			T_OutputPins_Second::SetPinValue( AItem16[ 5 ] );
			T_OutputPins_MilliSecond::SetPinValue( AItem16[ 6 ] );
			T_OutputPins_WeekDay::SetPinValue( AItem16[ 7 ] );
		}
	};	
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class CompileDateTime :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		TDateTime	FValue;

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( FValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( FValue );

		}

	public:
		inline CompileDateTime()
		{
			const char *monthName[12] = 
			{
			  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
			  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
			};

			char AMonth[12];
			int ADay, AYear;
			uint8_t AMonthIndex;
			sscanf( __DATE__, "%s %d %d", AMonth, &ADay, &AYear);

			int AHour, AMin, ASec;
			sscanf( __TIME__, "%d:%d:%d", &AHour, &AMin, &ASec);

			for (AMonthIndex = 0; AMonthIndex < 12; AMonthIndex++) 
				if (strcmp(AMonth, monthName[AMonthIndex]) == 0)
					break;

			FValue.TryEncodeDateTime( AYear, AMonthIndex + 1, ADay, AHour, AMin, ASec, 0 );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Days,
		typename T_Enabled,
		typename T_Hours,
		typename T_MilliSeconds,
		typename T_Minutes,
		typename T_Months,
		typename T_OutputPin,
		typename T_Seconds,
		typename T_Years
	> class AddDateTime :
		public T_Days,
		public T_Enabled,
		public T_Hours,
		public T_MilliSeconds,
		public T_Minutes,
		public T_Months,
		public T_OutputPin,
		public T_Seconds,
		public T_Years
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

		_V_PROP_( Years )
		_V_PROP_( Months )
		_V_PROP_( Days )
		_V_PROP_( Hours )
		_V_PROP_( Minutes )
		_V_PROP_( Seconds )
		_V_PROP_( MilliSeconds )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			TDateTime ADate = *(TDateTime *)_Data;
			if( Years() )
				ADate.AddYears( Years() );

			if( Months() )
				ADate.AddMonths( Months() );

			if( Days() )
				ADate.AddDays( Days() );

			if( Hours() )
				ADate.AddHours( Hours() );

			if( Minutes() )
				ADate.AddMinutes( Minutes() );

			if( Seconds() )
				ADate.AddSeconds( Seconds() );

			if( MilliSeconds() )
				ADate.AddMilliSeconds( MilliSeconds() );

			T_OutputPin::SetPinValue( ADate );
		}

	};
//---------------------------------------------------------------------------
	template <
        typename T_MilliSeconds,
		typename T_OutputPin
    > class DateTimeToUnixEpoch :
        public T_MilliSeconds,
		public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( MilliSeconds )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TDateTime ADate = *(TDateTime *)_Data;

            uint16_t AYear;
            uint16_t AMonth;
            uint16_t ADay;
            uint16_t AHour;
            uint16_t AMinute;
            uint16_t ASecond;
            uint16_t AMilliSecond;
            ADate.DecodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond );

            //January and February are counted as months 13 and 14 of the previous year
            if( AMonth <= 2)
            {
               AMonth += 12;
               AYear -= 1;
            }

            //Convert years to days
            uint64_t ATime = (365 * uint32_t( AYear )) + (AYear / 4) - (AYear / 100) + (AYear / 400);
//            Serial.println( ATime );
            //Convert months to days
            ATime += (30 * AMonth ) + (3 * (AMonth + 1) / 5) + ADay;
            //Unix time starts on January 1sATime, 1970
            ATime -= 719561;
            //Convert days to seconds
            ATime *= 86400;
            //Add hours, minutes and seconds
            ATime += (3600 * uint32_t( AHour ) ) + (60 * AMinute ) + ASecond;

            if( MilliSeconds() )
            {
                ATime *= 1000;
                ATime += AMilliSecond;
            }

            T_OutputPin::SetPinValue( ATime );
        }

    };
//---------------------------------------------------------------------------
	template <
        typename T_MilliSeconds,
		typename T_OutputPin
    > class UnixEpochToDateTime :
        public T_MilliSeconds,
		public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( MilliSeconds )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            uint64_t t = *(uint64_t *)_Data;

            uint32_t a;
            uint32_t b;
            uint32_t c;
            uint32_t d;
            uint32_t e;
            uint32_t f;

            //Negative Unix time values are not supported
            if(t < 1)
               t = 0;

            //Clear milliseconds
            uint16_t AMilliSeconds;

            if( MilliSeconds() )
            {
                AMilliSeconds = t % 1000;
                t /= 1000;
            }

            else
                AMilliSeconds = 0;

            //Retrieve hours, minutes and seconds
            uint16_t ASecond = t % 60;
            t /= 60;
            uint16_t AMinute = t % 60;
            t /= 60;
            uint16_t AHour = t % 24;
            t /= 24;

            //Convert Unix time to date
            a = (uint32_t) ((4 * t + 102032) / 146097 + 15);
            b = (uint32_t) (t + 2442113 + a - (a / 4));
            c = (20 * b - 2442) / 7305;
            d = b - 365 * c - (c / 4);
            e = d * 1000 / 30601;
            f = d - e * 30 - e * 601 / 1000;

            //January and February are counted as months 13 and 14 of the previous year
            if(e <= 13)
            {
               c -= 4716;
               e -= 1;
            }

            else
            {
               c -= 4715;
               e -= 13;
            }

            //Retrieve year, month and day
            uint16_t AYear = c;
            uint16_t AMonth = e;
            uint16_t ADay = f;

            TDateTime ADate;
            if( ADate.TryEncodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, 0 ))
                T_OutputPin::SetPinValue( ADate );

        }
    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif