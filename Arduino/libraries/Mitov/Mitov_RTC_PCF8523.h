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
#include <Mitov_RTC_PCF8563.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoPCF8523TimerClockFrequency { pcf23t4096Hz, pcf23t64Hz, pcf23t1Hz, pcf23t1_60th_Hz, pcf23t1_3600th_Hz, pcf23tAuto };
//---------------------------------------------------------------------------
	template <
		typename T_ClockFrequency,
		typename T_Enabled,
		typename T_InterruptEnabled,
		typename T_LowPulseWidth,
		typename T_Period,
		typename T_PulseInterruptEnabled,
		typename T_TimerOutputPin
	> class TArduinoPCF8523TimerBElement :
		public T_ClockFrequency,
		public T_Enabled,
		public T_InterruptEnabled,
		public T_LowPulseWidth,
		public T_Period,
		public T_PulseInterruptEnabled,
		public T_TimerOutputPin
	{
	public:
		_V_PIN_( TimerOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InterruptEnabled )
		_V_PROP_( PulseInterruptEnabled )
		_V_PROP_( ClockFrequency )
		_V_PROP_( Period )
		_V_PROP_( LowPulseWidth )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockFrequency,
		typename T_Enabled,
		typename T_InterruptEnabled,
		typename T_IsWatchdog,
		typename T_Period,
		typename T_PulseInterruptEnabled,
		typename T_SecondClockOutputPin,
		typename T_SecondInterruptEnabled,
		typename T_TimerOutputPin
	> class TArduinoPCF8523SecondAndTimerAElement :
		public T_ClockFrequency,
		public T_Enabled,
		public T_InterruptEnabled,
		public T_IsWatchdog,
		public T_Period,
		public T_PulseInterruptEnabled,
		public T_SecondClockOutputPin,
		public T_SecondInterruptEnabled,
		public T_TimerOutputPin
	{
	public:
		_V_PIN_( TimerOutputPin )
		_V_PIN_( SecondClockOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InterruptEnabled )
		_V_PROP_( PulseInterruptEnabled )
		_V_PROP_( IsWatchdog )
		_V_PROP_( SecondInterruptEnabled )
		_V_PROP_( ClockFrequency )
		_V_PROP_( Period )

	};
//---------------------------------------------------------------------------
	enum TArduinoPCF8523ClockOutFrequency { pcf23co32768Hz, pcf23co16384Hz, pcf23co8192Hz, pcf23co4096Hz, pcf23co1024Hz, pcf23co32Hz, pcf23co1Hz };
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Frequency
	> class TArduinoPCF8523ClockOutElement :
		public T_Enabled,
		public T_Frequency
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Frequency )

	};
//---------------------------------------------------------------------------
	template <
		typename T_CorrectionInterruptEnabled,
		typename T_PerMinute,
		typename T_Value
	> class TArduinoRTCPCF8523Offset :
		public T_CorrectionInterruptEnabled,
		public T_PerMinute,
		public T_Value
	{
	public:
		_V_PROP_( PerMinute	)
		_V_PROP_( Value )
		_V_PROP_( CorrectionInterruptEnabled )

	};
//---------------------------------------------------------------------------
	enum TArduinoRTCPCF8523PowerSwitchMode { rtc23psmStandard, rtc23psmDirect, rtc23psmSinglePower };
//---------------------------------------------------------------------------
	template <
		typename T_BatteryLowInterruptEnabled,
		typename T_BatteryLowOutputPin,
		typename T_PowerSwitchInterruptEnabled,
		typename T_PowerSwitchedOutputPin,
		typename T_SwitchMode
	> class TArduinoRTCPCF8523PowerManagement :
		public T_BatteryLowInterruptEnabled,
		public T_BatteryLowOutputPin,
		public T_PowerSwitchedOutputPin,
		public T_PowerSwitchInterruptEnabled,
		public T_SwitchMode
	{
	public:
		_V_PIN_( BatteryLowOutputPin )
		_V_PIN_( PowerSwitchedOutputPin )

	public:
		_V_PROP_( SwitchMode )
		_V_PROP_( BatteryLowInterruptEnabled )
		_V_PROP_( PowerSwitchInterruptEnabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Alarm,
		typename T_AlarmOutputPin,
		typename T_Capacitor12_6,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ClockOut,
		typename T_Enabled,
		typename T_IntegrityCompromisedOutputPin,
		typename T_Offset,
		typename T_OutputPin,
		typename T_PowerManagement,
		typename T_SecondAndTimerA,
		typename T_TimerB
	> class RTC_PCF8523 :
		public T_Address,
		public T_Alarm,
		public T_AlarmOutputPin,
		public T_Capacitor12_6,
		public T_ClockInputPin_o_IsConnected,
		public T_ClockOut,
		public T_Enabled,
		public T_IntegrityCompromisedOutputPin,
		public T_Offset,
		public T_OutputPin,
		public T_PowerManagement,
		public T_SecondAndTimerA,
		public T_TimerB
	{
	protected:
//		const uint8_t	PCF_ADDRESS = ( uint8_t( 0xD0 ) >> 1 );

		// register addresses in the rtc
		const uint8_t	RTCC_STAT1_ADDR				= 0x00;
		const uint8_t	RTCC_STAT2_ADDR				= 0x01;
		const uint8_t	RTCC_STAT3_ADDR				= 0x02;
		const uint8_t	RTCC_SEC_ADDR				= 0x03;
		const uint8_t	RTCC_MIN_ADDR				= 0x04;
		const uint8_t	RTCC_HOUR_ADDR				= 0x05;
		const uint8_t	RTCC_DAY_ADDR				= 0x06;
		const uint8_t	RTCC_WEEKDAY_ADDR			= 0x07;
		const uint8_t	RTCC_MONTH_ADDR				= 0x08;
		const uint8_t	RTCC_YEAR_ADDR				= 0x09;
		const uint8_t	RTCC_ALRM_MIN_ADDR			= 0x0A;
		const uint8_t	RTCC_ALRM_HOUR_ADDR			= 0x0B;
		const uint8_t	RTCC_ALRM_DAY_ADDR			= 0x0C;
		const uint8_t	RTCC_ALRM_WEEKDAY_ADDR		= 0x0D;
		const uint8_t	RTCC_OFFSET_ADDR			= 0x0E;
		const uint8_t	RTCC_TIMER_CLOCKOUT_ADDR	= 0x0F;
		const uint8_t	RTCC_TIMER1_ADDR_1			= 0x10;
		const uint8_t	RTCC_TIMER1_ADDR_2			= 0x11;
		const uint8_t	RTCC_TIMER2_ADDR_1			= 0x12;
		const uint8_t	RTCC_TIMER2_ADDR_2			= 0x13;

	public:
		_V_PIN_( OutputPin )

//#ifdef _MITOV_PCF8523_RTC_INTEGRITY_PIN_
		_V_PIN_( IntegrityCompromisedOutputPin )
//#endif

//#ifdef _MITOV_PCF8523_RTC_ALARM_PIN_
		_V_PIN_( AlarmOutputPin )
//#endif

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Capacitor12_6 )

	public:
		_V_PROP_( Offset )
		_V_PROP_( Alarm )
		_V_PROP_( SecondAndTimerA )
		_V_PROP_( TimerB )
		_V_PROP_( ClockOut )
		_V_PROP_( PowerManagement )

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
			uint8_t	AValue =	(( SecondAndTimerA().PulseInterruptEnabled() ) ? 0x80 : 0 ) |
								(( TimerB().PulseInterruptEnabled() ) ? 0b01000000 : 0 );

			if( ClockOut().Enabled() )
				AValue |= ( uint8_t( ClockOut().Frequency() ) << 3 );

			else
				AValue |= 0b11000;

			if( SecondAndTimerA().Enabled() )
				AValue |= ( SecondAndTimerA().IsWatchdog() ) ? 0b100 : 0b010;

			if( TimerB().Enabled() )
				AValue |= 1;

			writeRegister( RTCC_TIMER_CLOCKOUT_ADDR, AValue );
		}

		void UpdateTimerA()
		{
			uint8_t	AValueControl = 0;
			uint8_t	AValue = 0;
			if( SecondAndTimerA().Enabled() )
			{
				if( SecondAndTimerA().ClockFrequency() != pcf23tAuto )
				{
					AValueControl = uint8_t( SecondAndTimerA().ClockFrequency() );
					switch( SecondAndTimerA().ClockFrequency() )
					{
						case pcf23t4096Hz:
							if( SecondAndTimerA().Period() < 0.062255859375 )
								AValue = ( SecondAndTimerA().Period() * 255 / 0.062255859375 ) + 0.5;

							else
								AValue = 255;

							break;

						case pcf23t64Hz:
							if( SecondAndTimerA().Period() < 3.984375 )
								AValue = ( SecondAndTimerA().Period() * 255 / 3.9843755 ) + 0.5;

							else
								AValue = 255;

							break;

						case pcf23t1Hz:
							if( SecondAndTimerA().Period() < 255 )
								AValue = SecondAndTimerA().Period() + 0.5;

							else
								AValue = 255;

							break;

						case pcf23t1_60th_Hz:
							if( SecondAndTimerA().Period() < 15300 )
								AValue = ( SecondAndTimerA().Period() * 255 / 15300 ) + 0.5;

							else
								AValue = 255;

							break;

						default:
							if( SecondAndTimerA().Period() < 918000 )
								AValue = ( SecondAndTimerA().Period() * 255 / 918000 ) + 0.5;

							else
								AValue = 255;

							break;
					}
				}

				else
				{
					if( SecondAndTimerA().Period() < 0.062255859375 )
					{
						AValueControl = 0x00;
						AValue = ( SecondAndTimerA().Period() * 255 / 0.062255859375 ) + 0.5;
					}
					else if( SecondAndTimerA().Period() < 3.984375 )
					{
						AValueControl = 0x01;
						AValue = ( SecondAndTimerA().Period() * 255 / 3.984375 ) + 0.5;
					}
					else if( SecondAndTimerA().Period() < 255 )
					{
						AValueControl = 0x02;
						AValue = SecondAndTimerA().Period() + 0.5;
					}
					else if( SecondAndTimerA().Period() <= 15300 )
					{
						AValueControl = 0x03;
						AValue = ( SecondAndTimerA().Period() * 255 / 15300 ) + 0.5;
					}
					else
					{
						AValueControl = 0x04;
						if( SecondAndTimerA().Period() <= 918000 )
							AValue = ( SecondAndTimerA().Period() * 255 / 918000 ) + 0.5;

						else
							AValue = 255;
					}
				}
			}

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( RTCC_TIMER1_ADDR_1 );
			C_I2C.write( AValueControl );
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

		void UpdateTimerB()
		{
			uint8_t	AValueControl = 0;
			uint8_t	AValue = 0;
			if( TimerB().Enabled() )
			{
				if( TimerB().ClockFrequency().GetValue() != pcf23tAuto )
				{
					AValueControl = uint8_t( TimerB().ClockFrequency() );
					switch( TimerB().ClockFrequency() )
					{
						case pcf23t4096Hz:
							if( TimerB().Period() < 0.062255859375 )
								AValue = ( TimerB().Period() * 255 / 0.062255859375 ) + 0.5;

							else
								AValue = 255;

							break;

						case pcf23t64Hz:
							if( TimerB().Period() < 3.984375 )
								AValue = ( TimerB().Period() * 255 / 3.9843755 ) + 0.5;

							else
								AValue = 255;

							break;

						case pcf23t1Hz:
							if( TimerB().Period() < 255 )
								AValue = TimerB().Period() + 0.5;

							else
								AValue = 255;

							break;

						case pcf23t1_60th_Hz:
							if( TimerB().Period() < 15300 )
								AValue = ( TimerB().Period() * 255 / 15300 ) + 0.5;

							else
								AValue = 255;

							break;

						default:
							if( TimerB().Period() < 918000 )
								AValue = ( TimerB().Period() * 255 / 918000 ) + 0.5;

							else
								AValue = 255;

							break;
					}
				}

				else
				{
					if( TimerB().Period() < 0.062255859375 )
					{
						AValueControl = 0x00;
						AValue = ( TimerB().Period() * 255 / 0.062255859375 ) + 0.5;
					}
					else if( TimerB().Period() < 3.984375 )
					{
						AValueControl = 0x01;
						AValue = ( TimerB().Period() * 255 / 3.984375 ) + 0.5;
					}
					else if( TimerB().Period() < 255 )
					{
						AValueControl = 0x02;
						AValue = TimerB().Period() + 0.5;
					}
					else if( TimerB().Period() <= 15300 )
					{
						AValueControl = 0x03;
						AValue = ( TimerB().Period() * 255 / 15300 ) + 0.5;
					}
					else
					{
						AValueControl = 0x04;
						if( TimerB().Period() <= 918000 )
							AValue = ( TimerB().Period() * 255 / 918000 ) + 0.5;

						else
							AValue = 255;
					}
				}
			}

			const float CPulseWidth[] = { 46.875, 62.500, 78.125, 93.750,  125.000, 156.250, 187.500 };

			if( TimerB().LowPulseWidth() >= 218.750 )
				AValueControl |= 0b1110000;

			else
			{
				for( int i = 0; i < MITOV_ARRAY_SIZE( CPulseWidth ); ++i )
					if( TimerB().LowPulseWidth() <= CPulseWidth[ i ] )
					{
						AValueControl |= i << 4;
						break;
					}
			}

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( RTCC_TIMER2_ADDR_1 );
			C_I2C.write( AValueControl );
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

		void UpdateControl1()
		{
			uint8_t AValue =	(( Capacitor12_6() ) ? 0x80 : 0 ) |
								(( SecondAndTimerA().SecondInterruptEnabled() ) ? 0b100 : 0 ) |
								(( Alarm().InterruptEnabled() ) ? 0b10 : 0 ) |
								(( Offset().CorrectionInterruptEnabled() ) ? 0b1 : 0 );

			writeRegister( RTCC_STAT1_ADDR, AValue );
		}

		void UpdateControl2()
		{
			uint8_t AValue =	(( TimerB().InterruptEnabled() ) ? 1 : 0 ) |
								(( SecondAndTimerA().InterruptEnabled() ) ? 0b110 : 0 );

			writeRegister( RTCC_STAT2_ADDR, AValue );
		}

		void UpdateControl3()
		{
//			rtc23psmStandard = 0, rtc23psmDirect = 0b00100000, rtc23psmSinglePower = 0b01100000
			const uint8_t CModes[] = { 0, 0b00100000, 0b01100000 };

			uint8_t AValue =	0x80 |
								(( PowerManagement().BatteryLowInterruptEnabled() ) ? 1 : 0 ) |
								(( PowerManagement().PowerSwitchInterruptEnabled() ) ? 0b10 : 0 ) |
								CModes[ PowerManagement().SwitchMode() ];

#ifdef _MITOV_PCF8523_RTC_LOW_VOLTAGE_PIN_
			AValue &= 0x7F;
#else // _MITOV_PCF8523_RTC_LOW_VOLTAGE_PIN_
			if( PowerManagement().BatteryLowInterruptEnabled() )
				AValue &= 0x7F;
#endif // _MITOV_PCF8523_RTC_LOW_VOLTAGE_PIN_

			writeRegister( RTCC_STAT3_ADDR, AValue );
		}

		void UpdateOffset()
		{
			int8_t	AIntValue = Offset().Value();
			uint8_t	AValue = *((uint8_t *)&AIntValue );
			if( Offset().PerMinute() )
				AValue |= 0x80;

			writeRegister( RTCC_OFFSET_ADDR, AValue );
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

#ifdef _MITOV_PCF8523_RTC_INTEGRITY_PIN_
			T_IntegrityCompromisedOutputPin::SetPinValue( ASecondLowBattery & 0x80 );
#endif // _MITOV_PCF8523_RTC_INTEGRITY_PIN_

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
			C_I2C.write( uint8_t( 0x0 ));     //control/status3
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

			UpdateOffset();

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
			UpdateTimerA();
			UpdateTimerB();
			UpdateControl1();
			UpdateControl2();
			UpdateControl3();

//			Serial.println( "TEST2" );
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() )
			{
//#ifdef _MITOV_PCF8523_RTC_CLOCK_PIN_
				if( ! ClockInputPin_o_IsConnected() )
//#endif // _MITOV_PCF8523_RTC_CLOCK_PIN_
					ReadTime( false );

#if ( defined( _MITOV_PCF8523_RTC_ALARM_PIN_ ) || defined( _MITOV_PCF8523_RTC_SECOND_CLOCK_PIN_ ) || defined( _MITOV_PCF8523_RTC_TIMER_CLOCK_PIN_ ))
				uint8_t  AStatus = readRegister( RTCC_STAT2_ADDR );

#if defined( _MITOV_PCF8523_RTC_TIMER_CLOCK_PIN_ )
				if( SecondAndTimerA().IsWatchdog() )
				{
					if( AStatus & 0b10000000 ) // Check Interrupt
					{
						// Clear Interrupt
						writeRegister( RTCC_STAT2_ADDR, AStatus & ( ~0b10000000 ) );
						SecondAndTimerA().TimerOutputPin().ClockPin();
					}
				}
				else
				{
					if( AStatus & 0b01000000 ) // Check Interrupt
					{
						// Clear Interrupt
						writeRegister( RTCC_STAT2_ADDR, AStatus & ( ~0b01000000 ) );
						SecondAndTimerA().TimerOutputPin().ClockPin();
					}
				}

				if( AStatus & 0b00100000 ) // Check Interrupt
				{
					// Clear Interrupt
					writeRegister( RTCC_STAT2_ADDR, AStatus & ( ~0b00100000 ) );
					TimerB().TimerOutputPin().ClockPin();
				}

#endif // _MITOV_PCF8523_RTC_TIMER_CLOCK_PIN_

//				Serial.println( "TEST" );
#if defined( _MITOV_PCF8523_RTC_ALARM_PIN_ )
				if( AStatus & 0b1000 ) // Check Alarm
				{
					// Clear Alarm
					writeRegister( RTCC_STAT2_ADDR, AStatus & ( ~0b1000 ) );
					T_AlarmOutputPin::ClockPin();
				}
#endif // _MITOV_PCF8523_RTC_ALARM_PIN_

#if defined( _MITOV_PCF8523_RTC_SECOND_CLOCK_PIN_ )
				if( AStatus & 0b10000 ) // Seconds Interrupt
				{
					// Clear Interrupt
					writeRegister( RTCC_STAT2_ADDR, AStatus & ( ~0b10000 ) );
					SecondAndTimerA().SecondClockOutputPin().ClockPin();
				}
#endif // _MITOV_PCF8523_RTC_ALARM_PIN_

#endif // ( defined( _MITOV_PCF8523_RTC_ALARM_PIN_ ) || defined( _MITOV_PCF8523_RTC_SECOND_CLOCK_PIN_ ))

#if ( defined( _MITOV_PCF8523_RTC_LOW_VOLTAGE_PIN_ ) || defined( _MITOV_PCF8523_RTC_POWER_SWITCHED_PIN_ ))
				uint8_t  AStatus3 = readRegister( RTCC_STAT3_ADDR );
				if( AStatus3 & 0b1000 ) // Switch Interrupt
				{
					writeRegister( RTCC_STAT3_ADDR, AStatus3 & ( ~0b1000 ) );
					PowerManagement().PowerSwitchedOutputPin().ClockPin();
				}

				PowerManagement().BatteryLowOutputPin().SetPinValue( ( AStatus3 & 0b1000 ) != 0 ); // Switch Interrupt
#endif // ( defined( _MITOV_PCF8523_RTC_LOW_VOLTAGE_PIN_ ) || defined( _MITOV_PCF8523_RTC_POWER_SWITCHED_PIN_ ))

			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif