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
#include <Wire.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
//	enum DS3231ClockFrequency { rtc3231fDisabled, rtc3231f1Hz, rtc3231f1024Hz, rtc3231f4096Hz, rtc3231f8192Hz };
//---------------------------------------------------------------------------
	enum TArduinoRTCDS3231Alarm1Mode { rtc3231a1mOncePerSecond, rtc3231a1mOnSeconds, rtc3231a1mOnMinutesSeconds, rtc3231a1mOnHoursMinutesSeconds, rtc3231a1mOnDateHoursMinutesSeconds, rtc3231a1mOnWeekDayHoursMinutesSeconds };
//---------------------------------------------------------------------------
	enum TArduinoRTCDS3231Alarm2Mode { rtc3231a2mOncePerMinute, rtc3231a2mOnMinutes, rtc3231a2mOnHoursMinutes, rtc3231a2mOnDateHoursMinutes, rtc3231a2mOnWeekDayHoursMinutes };
//---------------------------------------------------------------------------
	template <
		typename T_AlarmOutputPin,
		typename T_Date,
		typename T_Enabled,
		typename T_Hour,
		typename T_InterruptEnabled,
		typename T_Minute,
		typename T_Mode,
		typename T_Second,
		typename T_WeekDay
	> class TArduinoRTCDS3231AlarmOne :
		public T_AlarmOutputPin,
		public T_Date,
		public T_Enabled,
		public T_Hour,
		public T_InterruptEnabled,
		public T_Minute,
		public T_Mode,
		public T_Second,
		public T_WeekDay
	{
	public:
		_V_PIN_( AlarmOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InterruptEnabled )
		_V_PROP_( Mode )
		_V_PROP_( Second )
		_V_PROP_( Minute )
		_V_PROP_( Hour )
		_V_PROP_( Date )
		_V_PROP_( WeekDay )

	};
//---------------------------------------------------------------------------
	template <
		typename T_AlarmOutputPin,
		typename T_Date,
		typename T_Enabled,
		typename T_Hour,
		typename T_InterruptEnabled,
		typename T_Minute,
		typename T_Mode,
		typename T_WeekDay
	> class TArduinoRTCDS3231AlarmTwo :
		public T_AlarmOutputPin,
		public T_Date,
		public T_Enabled,
		public T_Hour,
		public T_InterruptEnabled,
		public T_Minute,
		public T_Mode,
		public T_WeekDay
	{
	public:
		_V_PIN_( AlarmOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InterruptEnabled )
		_V_PROP_( Mode )
		_V_PROP_( Minute )
		_V_PROP_( Hour )
		_V_PROP_( Date )
		_V_PROP_( WeekDay )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_AlarmOne,
		typename T_AlarmTwo,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_Halt,
		typename T_OutputPin,
		typename T_TemperatureOutputPin
	> class RTC_DS3231 :
		public T_Address,
		public T_AlarmOne,
		public T_AlarmTwo,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_Halt,
		public T_OutputPin,
		public T_TemperatureOutputPin
	{
		enum Register
		{
			kSecondReg			= 0x00,
			kMinuteReg			= 0x01,
			kHourReg			= 0x02,
			kWeekDayReg			= 0x03,
			kDateReg			= 0x04,
			kMonthReg			= 0x05,
			kYearReg			= 0x06,
			kAlarm1SecondReg	= 0x07,
			kAlarm1MinuteReg	= 0x08,
			kAlarm1HourReg		= 0x09,
			kAlarm1DayReg		= 0x0A,
			kAlarm2MinuteReg	= 0x0B,
			kAlarm2HourReg		= 0x0C,
			kAlarm2DayReg		= 0x0D,
			kControlReg			= 0x0E
//			kClockReg	= 7,

			// The RAM register space follows the clock register space.
//			kRamAddress0     = 8
		};

//		static const uint8_t	DS3231_ADDRESS  = 0x68;

		static const uint8_t	RTC_STATUS = 0x0F;

//Status register bits
		static const uint8_t	OSF		= 7;
		static const uint8_t	BB32KHZ = 6;
		static const uint8_t	CRATE1	= 5;
		static const uint8_t	CRATE0	= 4;
		static const uint8_t	EN32KHZ = 3;
		static const uint8_t	BSY		= 2;
		static const uint8_t	A2F		= 1;
		static const uint8_t	A1F		= 0;

	public:
		_V_PIN_( OutputPin )

		_V_PIN_( TemperatureOutputPin )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address );
		_V_PROP_( Enabled )
		_V_PROP_( Halt )

		_V_PROP_( AlarmOne )
		_V_PROP_( AlarmTwo )

	public:
		void UpdateControl()
		{
			uint8_t	AValue = ( AlarmOne().InterruptEnabled().GetValue() ) ? 1 : 0;
			if( AlarmTwo().InterruptEnabled().GetValue() )
				AValue |= 0b10;

			writeRegister( kControlReg, AValue );
		}

		void UpdateHalt()
		{
			uint8_t sec = readRegister( kSecondReg );

			sec &= ~(1 << 7);
			sec |= ( ( Halt() & 1 ) << 7 );
			writeRegister( kSecondReg, sec );
		}

		void UpdateAlarmOneSecond()
		{
			uint8_t	AValue = Func::FromDecToBcd( AlarmOne().Second() );
			if( AlarmOne().Mode() == rtc3231a1mOncePerSecond )
				AValue |= 0x80;

			writeRegister( kAlarm1SecondReg, AValue );
		}

		void UpdateAlarmOneMinute()
		{
			uint8_t	AValue = Func::FromDecToBcd( AlarmOne().Minute() );
			if( AlarmOne().Mode() <= rtc3231a1mOnSeconds )
				AValue |= 0x80;

			writeRegister( kAlarm1MinuteReg, AValue );
		}

		void UpdateAlarmOneHour()
		{
			uint8_t	AValue = Func::FromDecToBcd( AlarmOne().Hour() );
			if( AlarmOne().Mode() <= rtc3231a1mOnMinutesSeconds )
				AValue |= 0x80;

			writeRegister( kAlarm1HourReg, AValue );
		}

		void UpdateAlarmOneDate()
		{
			if( AlarmOne().Mode() == rtc3231a1mOnWeekDayHoursMinutesSeconds )
				return;

			uint8_t	AValue = Func::FromDecToBcd( AlarmOne().Date() );
			if( AlarmOne().Mode() <= rtc3231a1mOnHoursMinutesSeconds )
				AValue |= 0x80;

			writeRegister( kAlarm1DayReg, AValue );
		}

		void UpdateAlarmOneWeekDay()
		{
			if( AlarmOne().Mode() != rtc3231a1mOnWeekDayHoursMinutesSeconds )
				return;

			uint8_t	AValue = AlarmOne().WeekDay() + 0x41;
			writeRegister( kAlarm1DayReg, AValue );
		}

		void UpdateAlarmOne()
		{
			uint8_t	ASecond = Func::FromDecToBcd( AlarmOne().Second() );
			if( AlarmOne().Mode() == rtc3231a1mOncePerSecond )
				ASecond |= 0x80;

			uint8_t	AMinute = Func::FromDecToBcd( AlarmOne().Minute() );
			if( AlarmOne().Mode() <= rtc3231a1mOnSeconds )
				AMinute |= 0x80;

			uint8_t	AHour = Func::FromDecToBcd( AlarmOne().Hour() );
			if( AlarmOne().Mode() <= rtc3231a1mOnMinutesSeconds )
				AHour |= 0x80;

			uint8_t	ADay;
			if( AlarmOne().Mode() == rtc3231a1mOnWeekDayHoursMinutesSeconds )
				ADay = AlarmOne().WeekDay() + 0x41;

			else
			{
				ADay = Func::FromDecToBcd( AlarmOne().Date() );
				if( AlarmOne().Mode() <= rtc3231a1mOnHoursMinutesSeconds )
					ADay |= 0x80;
			}

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( kAlarm1SecondReg );

			C_I2C.write( ASecond );
			C_I2C.write( AMinute );
			C_I2C.write( AHour );
			C_I2C.write( ADay );

			C_I2C.endTransmission();
		}

		void UpdateAlarmTwoMinute()
		{
			uint8_t	AValue = Func::FromDecToBcd( AlarmTwo().Minute() );
			if( AlarmTwo().Mode() == rtc3231a2mOncePerMinute )
				AValue |= 0x80;

			writeRegister( kAlarm2MinuteReg, AValue );
		}

		void UpdateAlarmTwoHour()
		{
			uint8_t	AValue = Func::FromDecToBcd( AlarmTwo().Hour() );
			if( AlarmTwo().Mode() <= rtc3231a2mOnMinutes )
				AValue |= 0x80;

			writeRegister( kAlarm2HourReg, AValue );
		}

		void UpdateAlarmTwoDate()
		{
			if( AlarmTwo().Mode() == rtc3231a2mOnWeekDayHoursMinutes )
				return;

			uint8_t	AValue = Func::FromDecToBcd( AlarmTwo().Date() );
			if( AlarmTwo().Mode() <= rtc3231a2mOnHoursMinutes )
				AValue |= 0x80;

			writeRegister( kAlarm2DayReg, AValue );
		}

		void UpdateAlarmTwoWeekDay()
		{
			if( AlarmTwo().Mode() != rtc3231a2mOnWeekDayHoursMinutes )
				return;

			uint8_t	AValue = AlarmTwo().WeekDay() + 0x41;
			writeRegister( kAlarm2DayReg, AValue );
		}

		void UpdateAlarmTwo()
		{
			uint8_t	AMinute = Func::FromDecToBcd( AlarmTwo().Minute() );
			if( AlarmTwo().Mode() == rtc3231a2mOncePerMinute )
				AMinute |= 0x80;

			uint8_t	AHour = Func::FromDecToBcd( AlarmTwo().Hour() );
			if( AlarmTwo().Mode() <= rtc3231a2mOnMinutes )
				AHour |= 0x80;

			uint8_t	ADay;
			if( AlarmTwo().Mode() == rtc3231a2mOnWeekDayHoursMinutes )
				ADay = AlarmTwo().WeekDay() + 0x41;

			else
			{
				ADay = Func::FromDecToBcd( AlarmTwo().Date() );
				if( AlarmTwo().Mode() <= rtc3231a2mOnHoursMinutes )
					ADay |= 0x80;
			}

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( kAlarm2MinuteReg );

			C_I2C.write( AMinute );
			C_I2C.write( AHour );
			C_I2C.write( ADay );

			C_I2C.endTransmission();
		}

	public:
		void SetMilliSecond( uint8_t AValue )
		{
		}

		void SetSecond( uint8_t AValue )
		{
			writeRegister( kSecondReg, ( ( Halt() & 1 ) << 7 ) | Func::FromDecToBcd( AValue ) );
		}

		void SetMinute( uint8_t AValue )
		{
			writeRegister( kMinuteReg, Func::FromDecToBcd( AValue ) );
		}

		void SetHour( uint8_t AValue )
		{
			writeRegister( kHourReg, Func::FromDecToBcd( AValue ) );
		}

		void SetWeekDay( uint8_t AValue )
		{
			writeRegister( kWeekDayReg, Func::FromDecToBcd( AValue ) );
		}

		void SetYear( uint16_t AValue )
		{
			writeRegister( kYearReg, Func::FromDecToBcd( AValue % 100 ) );
		}

		Mitov::TDateTime GetDateTime()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( uint8_t( 0 ));
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 7 ));
			uint16_t ASecond = Func::FromBcdToDec(C_I2C.read() & 0x7F);
			uint16_t AMinute = Func::FromBcdToDec(C_I2C.read());
			uint16_t AHour = hourFromRegisterValue( C_I2C.read() );
			uint16_t ADay = C_I2C.read();
			uint16_t ADate = Func::FromBcdToDec(C_I2C.read());
			uint16_t AMonth = Func::FromBcdToDec(C_I2C.read());
			uint16_t AYear = Func::FromBcdToDec(C_I2C.read()) + 2000;

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

			C_I2C.write( uint8_t( 0 )); // start at location 0

			C_I2C.write( ( ( Halt() & 1 ) << 7 ) | Func::FromDecToBcd( ASecond ));
			C_I2C.write(Func::FromDecToBcd( AMinute ));
			C_I2C.write(Func::FromDecToBcd( AHour ));
			C_I2C.write(Func::FromDecToBcd( AWeekDay ));
			C_I2C.write(Func::FromDecToBcd( ADay ));
			C_I2C.write(Func::FromDecToBcd( AMonth ));
			C_I2C.write(Func::FromDecToBcd( AYear % 100 ));

			C_I2C.endTransmission();

			uint8_t s = readRegister(RTC_STATUS);        //read the status register
			writeRegister( RTC_STATUS, s & ~( 1 << OSF) );  //clear the Oscillator Stop Flag
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

		uint8_t hourFromRegisterValue(const uint8_t value)
		{
			uint8_t adj;
			if (value & 128)  // 12-hour mode
				adj = 12 * ((value & 32) >> 5);

			else           // 24-hour mode
				adj = 10 * ((value & (32 + 16)) >> 4);

			return (value & 15) + adj;
		}

		void ReadTime( bool AFromClock )
		{
			if( ! Enabled() )
				return;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
			{
				C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
				C_I2C.write( uint8_t( 0x11 ));
				C_I2C.endTransmission();
				C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 2 ));

				int16_t ATemp = int16_t( C_I2C.read() ) << 8;
				ATemp |= C_I2C.read();

				float ATemperature = float( ATemp ) / 256;
				T_TemperatureOutputPin::SetPinValue( ATemperature );
//				Serial.println( ATemperature );
			}

//			Serial.println( "ReadTime" );
			if( T_OutputPin::GetPinIsConnected() )
			{
				C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
				C_I2C.write( uint8_t( 0 ));
				C_I2C.endTransmission();

				C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 7 ));
				uint16_t ASecond = Func::FromBcdToDec(C_I2C.read() & 0x7F);
				uint16_t AMinute = Func::FromBcdToDec(C_I2C.read());
				uint16_t AHour = hourFromRegisterValue( C_I2C.read() );
				uint16_t ADay = C_I2C.read();
				uint16_t ADate = Func::FromBcdToDec(C_I2C.read());
				uint16_t AMonth = Func::FromBcdToDec(C_I2C.read());
				uint16_t AYear = Func::FromBcdToDec(C_I2C.read()) + 2000;

				Mitov::TDateTime ADateTime;

				if( ADateTime.TryEncodeDateTime( AYear, AMonth, ADate, AHour, AMinute, ASecond, 0 ))
					T_OutputPin::SetPinValue( ADateTime, ! AFromClock );
			}


		}

/*
		void UpdateClockFrequency()
		{
			const uint8_t CValues [] = { 0x00, 0x80, 0x40, 0x41, 0x42, 0x43 };

			uint8_t AValue = readRegister( kControlReg );

			sec &= ~(1 << 7);
			sec |= ( ( Halt & 1 ) << 7 );
			writeRegister( kSecondReg, sec );

			writeRegister( kControlReg, CValues[ ClockFrequency ] );
		}
*/
	public:
		inline void SetInputPin_o_Receive( void *_Data )
		{
			Mitov::TDateTime &ADateTime = *(Mitov::TDateTime *)_Data;
			SetDateTime( ADateTime );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( true );
		}

	public:
		inline void SystemInit()
		{
			UpdateHalt();
#ifdef _MITOV_DS3231_RTC_ALARM_ONE_
			UpdateAlarmOne();
#endif	// _MITOV_DS3231_RTC_ALARM_ONE_

#ifdef _MITOV_DS3231_RTC_ALARM_TWO_
			UpdateAlarmTwo();
#endif	// _MITOV_DS3231_RTC_ALARM_TWO_
//			UpdateClockFrequency();
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			if( ! ClockInputPin_o_IsConnected() )
				ReadTime( false );

			if( ( AlarmOne().Enabled() && AlarmOne().AlarmOutputPin().GetPinIsConnected() ) || ( AlarmTwo().Enabled() && AlarmTwo().AlarmOutputPin().GetPinIsConnected() ))
			{
				uint8_t AValue = readRegister( RTC_STATUS );        //read the status register
				if( AValue & 0b11 )
				{
					// Clear the alarm bits
					writeRegister( RTC_STATUS, AValue & ( ~ 0b11 ) );

					if( AValue & 0b01 )
						AlarmOne().AlarmOutputPin().ClockPin();

					if( AValue & 0b10 )
						AlarmTwo().AlarmOutputPin().ClockPin();
				}
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif