/*
  RTC library for Arduino Zero.
  Copyright (c) 2015 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "Arduino.h"

typedef void(*voidFuncPtr)(void);

namespace RTCZeroConst
{
// Default date & time after reset
	const uint16_t DEFAULT_YEAR = 2000; // 2000..2063
	const uint8_t DEFAULT_MONTH = 1;   // 1..12
	const uint8_t DEFAULT_DAY = 1;     // 1..31
	const uint8_t DEFAULT_HOUR = 0;    // 1..23
	const uint8_t DEFAULT_MINUTE = 0;  // 0..59
	const uint8_t DEFAULT_SECOND = 0;  // 0..59
}

class RTCZero 
{
protected:
    voidFuncPtr RTC_callBack = nullptr;
    
public:

  enum Alarm_Match: uint8_t // Should we have this enum or just use the identifiers from /component/rtc.h ?
  {
        MATCH_OFF = RTC_MODE2_MASK_SEL_OFF_Val,                  // Never
        MATCH_SS = RTC_MODE2_MASK_SEL_SS_Val,                    // Every Minute
        MATCH_MMSS = RTC_MODE2_MASK_SEL_MMSS_Val,                // Every Hour
        MATCH_HHMMSS = RTC_MODE2_MASK_SEL_HHMMSS_Val,            // Every Day
        MATCH_DHHMMSS = RTC_MODE2_MASK_SEL_DDHHMMSS_Val,         // Every Month
        MATCH_MMDDHHMMSS = RTC_MODE2_MASK_SEL_MMDDHHMMSS_Val,    // Every Year
        MATCH_YYMMDDHHMMSS = RTC_MODE2_MASK_SEL_YYMMDDHHMMSS_Val // Once, on a specific date and a specific time
  };

//  RTCZero();
  void begin()
  {
    config32kOSC();

    uint16_t tmp_reg_a = 0;

    // If the RTC is in clock mode and the reset was
    // not due to POR or BOD, preserve the clock time
    // POR causes a reset anyway, BOD behaviour is?
    bool validTime = false;
    RTC_MODE2_CLOCK_Type oldTime;

    if (RTC->MODE2.CTRLA.reg & RTC_MODE2_CTRLA_MODE_CLOCK)
    {
        validTime = true;
        oldTime.reg = RTC->MODE2.CLOCK.reg;
    }

    configureClock();

    RTCdisable();

    RTCreset();

    tmp_reg_a |= RTC_MODE2_CTRLA_MODE_CLOCK;        // set clock operating mode
    tmp_reg_a |= RTC_MODE2_CTRLA_PRESCALER_DIV1024; // set prescaler to 1024 for MODE2 so that f = GCLK_RTC / 1024 = 1.024K / 1024 = 1Hz
    tmp_reg_a &= ~RTC_MODE2_CTRLA_MATCHCLR;         // disable clear on match

    //According to the datasheet RTC_MODE2_CTRL_CLKREP = 0 for 24h
    tmp_reg_a &= ~RTC_MODE2_CTRLA_CLKREP; // 24h time representation

    RTC->MODE2.CTRLA.reg = tmp_reg_a;
    while (RTCisSyncing())
        ;

    NVIC_EnableIRQ(RTC_IRQn); // enable RTC interrupt
    NVIC_SetPriority(RTC_IRQn, 0x00);

    RTC->MODE2.INTENSET.reg |= RTC_MODE2_INTENSET_ALARM0; // enable alarm0 interrupt
    RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = MATCH_OFF;    // default alarm0 match is off (disabled)

    RTC->MODE2.INTENSET.reg |= RTC_MODE2_INTENSET_ALARM1; // enable alarm1 interrupt
    RTC->MODE2.Mode2Alarm[1].MASK.bit.SEL = MATCH_OFF;    // default alarm1 match is off (disabled)

    while (RTCisSyncing())
        ;

    RTCenable();
    RTCresetRemove();

    // If desired and valid, restore the time value, else use first valid time value
    if ((validTime) && (oldTime.reg != 0L))
    {
        RTC->MODE2.CLOCK.reg = oldTime.reg;
    }
    else
    {
        RTC->MODE2.CLOCK.reg = RTC_MODE2_CLOCK_YEAR( RTCZeroConst::DEFAULT_YEAR - 2000 ) | RTC_MODE2_CLOCK_MONTH( RTCZeroConst::DEFAULT_MONTH ) | RTC_MODE2_CLOCK_DAY( RTCZeroConst::DEFAULT_DAY ) | RTC_MODE2_CLOCK_HOUR( RTCZeroConst::DEFAULT_HOUR ) | RTC_MODE2_CLOCK_MINUTE( RTCZeroConst::DEFAULT_MINUTE ) | RTC_MODE2_CLOCK_SECOND( RTCZeroConst::DEFAULT_SECOND );
    }

    while (RTCisSyncing())
        ;

//    return true;
  }

  void enableAlarm(Alarm_Match match)
  {
    uint8_t id = 0; // For now only one alarm!

    if (id >= 2)
        return;
    RTC->MODE2.Mode2Alarm[id].MASK.bit.SEL = match;
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM_Msk))
        ;

  }
  
  void disableAlarm()
  {
    uint8_t id = 0; // For now only one alarm!

    if (id >= 2)
        return;
    RTC->MODE2.Mode2Alarm[id].MASK.bit.SEL = 0x00;
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM_Msk))
        ;

  }

  void attachInterrupt(voidFuncPtr callback)
  {
    RTC_callBack = callback;
  }
  
  void detachInterrupt()
  {
    RTC_callBack = NULL;
  }
  
  void standbyMode()
  {
    // Entering standby mode when connected
    // via the native USB port causes issues.
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __DSB();
    __WFI();
  }
  
  /* Get Functions */

  inline uint8_t getSeconds() { return RTC->MODE2.CLOCK.bit.SECOND; }
  inline uint8_t getMinutes() { return RTC->MODE2.CLOCK.bit.MINUTE; }
  inline uint8_t getHours() { return RTC->MODE2.CLOCK.bit.HOUR; }
  
  inline uint8_t getDay() { return RTC->MODE2.CLOCK.bit.DAY; }
  inline uint8_t getMonth() { return RTC->MODE2.CLOCK.bit.MONTH; }
  inline uint8_t getYear() { return RTC->MODE2.CLOCK.bit.YEAR; }
  
//  uint8_t getAlarmSeconds();
//  uint8_t getAlarmMinutes();
//  uint8_t getAlarmHours();

//  uint8_t getAlarmDay();
//  uint8_t getAlarmMonth();
//  uint8_t getAlarmYear();

  /* Set Functions */

  inline void setSeconds(uint8_t seconds)
  {
    RTC->MODE2.CLOCK.bit.SECOND = seconds;
  }
  
  inline void setMinutes(uint8_t minutes)
  {
    RTC->MODE2.CLOCK.bit.MINUTE = minutes;
  }  
  
  inline void setHours(uint8_t hours)
  {
    RTC->MODE2.CLOCK.bit.HOUR = hours;
  }  
  
//  void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

  inline void setDay(uint8_t day)
  {
    RTC->MODE2.CLOCK.bit.DAY = day;
  }
  
  inline void setMonth(uint8_t month)
  {
    RTC->MODE2.CLOCK.bit.MONTH = month;
  }
  
  inline void setYear(uint8_t year)
  {
    RTC->MODE2.CLOCK.bit.YEAR = year;
  }

  void UpdateTime()
  {
    while (RTCisSyncing())
        ;
  }
  
//  void setDate(uint8_t day, uint8_t month, uint8_t year);

  void setAlarmSeconds( uint8_t seconds ) 
  { 
    RTC->MODE2.Mode2Alarm[ 0 ].ALARM.bit.SECOND = seconds; 
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM0))
        ;
    
  }
  
  void setAlarmMinutes( uint8_t minutes ) 
  { 
    RTC->MODE2.Mode2Alarm[ 0 ].ALARM.bit.MINUTE = minutes; 
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM0))
        ;
    
  }
  
  void setAlarmHours( uint8_t hours ) 
  { 
    RTC->MODE2.Mode2Alarm[ 0 ].ALARM.bit.HOUR = hours; 
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM0))
        ;
    
  }
//  void setAlarmTime( uint8_t hours, uint8_t minutes, uint8_t seconds );

  void setAlarmDay( uint8_t day ) 
  { 
    RTC->MODE2.Mode2Alarm[ 0 ].ALARM.bit.DAY = day; 
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM0))
        ;
    
  }
  
  void setAlarmMonth( uint8_t month ) 
  { 
    RTC->MODE2.Mode2Alarm[ 0 ].ALARM.bit.MONTH = month; 
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM0))
        ;
    
  }
  
  void setAlarmYear( uint16_t year ) 
  { 
    RTC->MODE2.Mode2Alarm[ 0 ].ALARM.bit.YEAR = year - 2000; 
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_ALARM0))
        ;
    
  }
//  void setAlarmDate(uint8_t day, uint8_t month, uint8_t year);

  /* Epoch Functions */

  uint32_t getEpoch();
  uint32_t getY2kEpoch();
  void setEpoch(uint32_t ts);
  void setY2kEpoch(uint32_t ts);
  void setAlarmEpoch(uint32_t ts);

//  bool isConfigured() {
//    return _configured;
//  }

public:
  void RTCreadRequest()
  {
    RTC->MODE2.CTRLA.reg |= RTC_MODE2_CTRLA_CLOCKSYNC;
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_SWRST | RTC_MODE2_SYNCBUSY_ENABLE | RTC_MODE2_SYNCBUSY_CLOCKSYNC))
        ;

  }

private:
//  bool _configured;

  void config32kOSC(void)
  {
#ifdef CRYSTALLESS
    // This platform does not have external crystal.
    // Thus, we have to enable OSCULP32K to generate RTC clock.
    void *hw = (void *)OSC32KCTRL;
    uint16_t calib = 0;

    calib = ((Osc32kctrl *)hw)->OSCULP32K.reg;
    calib = (calib & OSC32KCTRL_OSCULP32K_CALIB_Msk) >> OSC32KCTRL_OSCULP32K_CALIB_Pos;
    //re-write calibrate
    ((Osc32kctrl *)hw)->OSCULP32K.reg &= ~OSC32KCTRL_OSCULP32K_WRTLOCK;

    ((Osc32kctrl *)hw)->OSCULP32K.reg |= calib;
    ((Osc32kctrl *)hw)->OSCULP32K.reg &= ~(OSC32KCTRL_OSCULP32K_EN32K);
    ((Osc32kctrl *)hw)->OSCULP32K.reg |= OSC32KCTRL_OSCULP32K_EN1K; //enable 1.024KHz OUPUT

    ((Osc32kctrl *)hw)->OSCULP32K.reg |= OSC32KCTRL_OSCULP32K_WRTLOCK;

    //Use 1.024KHz for RTC
    ((Osc32kctrl *)hw)->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL(OSC32KCTRL_RTCCTRL_RTCSEL_ULP1K_Val);
#else
    // This platform has external crystal and Arduino core has already enabled the XOSC32K oscillator.
    // But the Arduino core does not enable 1.024[kHz] output.
    // Thus all we have to do is just enable 1.024[kHz] output by setting XOSC32K.EN1k to 1.
    OSC32KCTRL->XOSC32K.bit.EN1K = 1;
    // Use 1.024[kHz] output for RTC.
    OSC32KCTRL->RTCCTRL.bit.RTCSEL = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC1K_Val;
#endif
  }

  void configureClock(void)
  {
    MCLK->APBAMASK.reg |= MCLK_APBAMASK_RTC | MCLK_APBAMASK_OSC32KCTRL;
  }

  bool RTCisSyncing(uint32_t flag = RTC_MODE2_SYNCBUSY_MASK_)
  {
    return (RTC->MODE2.SYNCBUSY.reg & flag);
  }
  
  void RTCdisable()
  {
    RTC->MODE2.CTRLA.reg &= ~RTC_MODE2_CTRLA_ENABLE; // disable RTC
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_SWRST | RTC_MODE2_SYNCBUSY_ENABLE | RTC_MODE2_SYNCBUSY_CLOCKSYNC))
        ;
    
  }
  
  void RTCenable()
  {
    RTC->MODE2.CTRLA.reg |= RTC_MODE2_CTRLA_ENABLE; // enable RTC
    while (RTCisSyncing(RTC_MODE2_SYNCBUSY_SWRST | RTC_MODE2_SYNCBUSY_ENABLE | RTC_MODE2_SYNCBUSY_CLOCKSYNC))
        ;
    
  }
  
  void RTCreset()
  {
    RTC->MODE2.CTRLA.reg |= RTC_MODE2_CTRLA_SWRST; // software reset
    while (RTCisSyncing(RTC_MODE2_CTRLA_SWRST))
        ;
    
  }
  
  void RTCresetRemove()
  {
    RTC->MODE2.CTRLA.reg &= ~RTC_MODE2_CTRLA_SWRST; // software reset remove
    while (RTCisSyncing(RTC_MODE2_CTRLA_SWRST))
        ;
    
  }
  
};

