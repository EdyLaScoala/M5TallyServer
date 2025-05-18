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
		typename T_FOldValue,
		typename T_OutputPin
	> class RCServoMeter :
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
			if( FOldValue() == AValue )
				return;

			unsigned long ANow = micros();
			FOldValue() = AValue;
			if( AValue )
			{
				FStartTime = ANow;
				return;
			}

			float APeriod = ANow - FStartTime;
//			float AFloatValue = MitovMin<float>( ( APeriod - 1000 ) / 1000, 1 );
			float AFloatValue = MitovMax<float>( MitovMin<float>( ( float( APeriod ) - 500 ) / 1900, 1.0f ), 0.0f );

			T_OutputPin::SetPinValue( AFloatValue );
		}

	public:
		inline void SystemLoopBegin() {} // Placeholder

	public:
		inline RCServoMeter()
		{
			FOldValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FSignaled,
		typename T_OutputPin
	> class RCServoMeter_Interrupt :
//		public OpenWire::InterruptObject,
		public T_Enabled,
		public T_FSignaled,
		public T_OutputPin
	{
    public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FSignaled )

		unsigned long	FStartTime = 0;
		unsigned long	FPeriod = 0;

	public:
		inline void __ICACHE_RAM_ATTR__ InterruptHandler( bool AValue )
		{
			unsigned long ANow = micros();
			if( AValue )
			{
				FStartTime = ANow;
				return;
			}

			FPeriod = ANow - FStartTime;
			FSignaled() = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

//			Serial.println( "TEST" );
			if( ! FSignaled() )
				return;

			noInterrupts(); // Protect the FReceivedPackets from being corrupted
			float APeriod = FPeriod;
			FSignaled() = false;
			interrupts();

//			float AFloatValue = MitovMin( ( float( APeriod ) - 2000 ) / 2000, 1 );
			float AFloatValue = MitovMax<float>( MitovMin<float>( ( float( APeriod ) - 500 ) / 1900, 1.0f ), 0.0f );
			T_OutputPin::SetPinValue( AFloatValue );
		}

	public:
		inline RCServoMeter_Interrupt()
		{
			FSignaled() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif