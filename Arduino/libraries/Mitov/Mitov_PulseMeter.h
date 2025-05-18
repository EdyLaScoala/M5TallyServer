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
	template <
		typename T_Enabled,
		typename T_FOldValue,
		typename T_OutputPin
	> class PulseMeter :
		public T_Enabled,
		public T_FOldValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FOldValue )
		unsigned long	FStartTime = 0;

    public:
		inline void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
				return;

			bool AValue = *(bool *)_Data;
//			Serial.println( AValue );
			if( FOldValue() == AValue )
				return;

			unsigned long ANow = micros();
			FOldValue() = AValue;
			if( AValue )
			{
				FStartTime = ANow;
				return;
			}

			uint32_t APeriod = ANow - FStartTime;

			T_OutputPin::SetPinValue( APeriod );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( 0 );
		}

		inline void SystemLoopBegin() {} // Placeholder

	public:
		inline PulseMeter()
		{
			FOldValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FChanged,
		typename T_FOldValue,
		typename T_OutputPin
	> class PulseMeter_Interrupt :
//		public OpenWire::InterruptObject,
		public T_Enabled,
		public T_FChanged,
		public T_FOldValue,
		public T_OutputPin
	{
    public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FOldValue )
		_V_PROP_( FChanged )

		unsigned long	FStartTime = 0;
		uint32_t		FPeriod	= 0;

	public:
		inline void __ICACHE_RAM_ATTR__ InterruptHandler( bool AValue )
		{
			if( FOldValue() == AValue )
				return;

			unsigned long ANow = micros();
			FOldValue() = AValue;
			if( AValue )
			{
				FStartTime = ANow;
				return;
			}

			FPeriod = ANow - FStartTime;
			FChanged() = true;
//			Serial.println( "Test" );

		}

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			if( ! FChanged() )
				return;

			uint32_t APeriod = FPeriod;
			FChanged() = false;
			T_OutputPin::SetPinValue( APeriod );
		}

	public:
		inline PulseMeter_Interrupt()
		{
			FOldValue() = false;
			FChanged() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif