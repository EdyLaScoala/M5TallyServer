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
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddValue,
		typename T_Value
	> class RTCSetMilliSecond :
		public T_AddValue,
		public T_Value
	{
	public:
		_V_PROP_( AddValue )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( AddValue() )
			{
				TDateTime ADateTime = C_OWNER.GetDateTime();
				ADateTime.AddMilliSeconds( Value() );
				C_OWNER.SetDateTime( ADateTime );
				return;
			}

			int32_t AValue = Value() % 1000;
			if( AValue < 0 )
				AValue += 1000;

			C_OWNER.SetMilliSecond( AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddValue,
		typename T_Value
	> class RTCSetSecond :
		public T_AddValue,
		public T_Value
	{
	public:
		_V_PROP_( AddValue )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( AddValue() )
			{
				TDateTime ADateTime = C_OWNER.GetDateTime();
				ADateTime.AddSeconds( Value() );
				C_OWNER.SetDateTime( ADateTime );
				return;
			}

			int32_t AValue = Value() % 60;
			if( AValue < 0 )
				AValue += 60;

			C_OWNER.SetSecond( AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddValue,
		typename T_Value
	> class RTCSetMinute :
		public T_AddValue,
		public T_Value
	{
	public:
		_V_PROP_( AddValue )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( AddValue() )
			{
				TDateTime ADateTime = C_OWNER.GetDateTime();
				ADateTime.AddMinutes( Value() );
				C_OWNER.SetDateTime( ADateTime );
				return;
			}

			int32_t AValue = Value() % 60;
			if( AValue < 0 )
				AValue += 60;

			C_OWNER.SetMinute( AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddValue,
		typename T_Value
	> class RTCSetHour :
		public T_AddValue,
		public T_Value
	{
	public:
		_V_PROP_( AddValue )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( AddValue() )
			{
				TDateTime ADateTime = C_OWNER.GetDateTime();
				ADateTime.AddHours( Value() );
				C_OWNER.SetDateTime( ADateTime );
				return;
			}

			int32_t AValue = Value() % 24;
			if( AValue < 0 )
				AValue += 24;

			C_OWNER.SetHour( AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddValue,
		typename T_Value
	> class RTCSetDay :
		public T_AddValue,
		public T_Value
	{
	public:
		_V_PROP_( AddValue )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			TDateTime ADateTime = C_OWNER.GetDateTime();
			if( AddValue() )
				ADateTime.AddDays( Value() );

			else
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
				ADay = Value();
				if( ! ADateTime.TryEncodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond ))
					return;

			}

			C_OWNER.SetDateTime( ADateTime );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddValue,
		typename T_Value
	> class RTCSetMonth :
		public T_AddValue,
		public T_Value
	{
	public:
		_V_PROP_( AddValue )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			TDateTime ADateTime = C_OWNER.GetDateTime();
			if( AddValue() )
				ADateTime.AddMonths( Value() );

			else
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
				AMonth = Value();
				if( ! ADateTime.TryEncodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond ))
					return;

			}

			C_OWNER.SetDateTime( ADateTime );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AddValue,
		typename T_Value
	> class RTCSetYear :
		public T_AddValue,
		public T_Value
	{
	public:
		_V_PROP_( AddValue )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( AddValue() )
			{
				TDateTime ADateTime = C_OWNER.GetDateTime();
				ADateTime.AddYears( Value() );
				C_OWNER.SetDateTime( ADateTime );
				return;
			}

			C_OWNER.SetYear( Value() );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Value
	> class RTCSetWeekDay :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SetWeekDay( Value() );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif