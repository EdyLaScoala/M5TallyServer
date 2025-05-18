////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// http://www.legolab.daimi.au.dk/CSaEA/RCX/Manual.dir/Sensors.dir/Sensors.html
// https://seriousrobotics.wordpress.com/2011/11/20/using-rcx-lego-sensors-with-arduino/

#pragma once

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		int C_ChargeOutputPin, 
		typename T_ChargePeriod,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FHasValue,
		typename T_MaxValue,
		typename T_MinValue,
		typename T_Normalize,
		typename T_OutputPin,
		int C_SensorInputPin
	> class LEGO_RCX_LightSensor :
		public T_ChargePeriod,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FHasValue,
		public T_MaxValue,
		public T_MinValue,
		public T_Normalize,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ChargePeriod )
		_V_PROP_( MinValue )
		_V_PROP_( MaxValue )

		_V_PROP_( Normalize )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FHasValue )

		float			FValue;
		unsigned long	FStartTime;

	public:
		inline void SystemStart()
		{
			FStartTime = micros();
			pinMode( C_ChargeOutputPin, OUTPUT );
			digitalWrite( C_ChargeOutputPin, HIGH );
		}

		inline void SystemLoopBegin()
		{
			if( ( micros() - FStartTime ) >= ChargePeriod() * 1000 )
			{
				pinMode( C_ChargeOutputPin, INPUT );

				analogRead( C_SensorInputPin ); // Give some settle time
				FValue = analogRead( C_SensorInputPin );
				if( Normalize() )
					FValue = ( FValue - MinValue() ) / ( MaxValue() - MinValue() );

				pinMode( C_ChargeOutputPin, OUTPUT );
				digitalWrite( C_ChargeOutputPin, HIGH );
				FHasValue() = true;

				FStartTime = micros();
			}

			if( ! ClockInputPin_o_IsConnected() )
				ReadSensor();

		}

	protected:
		void ReadSensor()
		{
			if( FHasValue() )
				T_OutputPin::SetPinValue( FValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline LEGO_RCX_LightSensor()
		{
			FHasValue() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif