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
		uint32_t C_COUNT,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Elements_StartElement,
		typename T_Enabled,
		typename T_FClocked,
		typename T_FCountStarted,
		typename T_FPulsing,
		typename T_PauseTime,
		typename T_PulseTime,
		typename T_StartOutputPin
	> class MultiSensorPulseStart :
		public T_ClockInputPin_o_IsConnected,
		public T_Elements_StartElement,
		public T_Enabled,
		public T_FClocked,
		public T_FPulsing,
		public T_FCountStarted,
		public T_PauseTime,
		public T_PulseTime,
		public T_StartOutputPin
	{
	public:
		_V_PIN_( StartOutputPin )

	public:
		_V_PROP_( PulseTime )
		_V_PROP_( PauseTime )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		_V_PROP_( FClocked )
		_V_PROP_( FPulsing )
		_V_PROP_( FCountStarted )

		unsigned long	FStartTime;

//	protected:
//		SimpleList<MultiSensorStartBasicElement *> FElements;

	public:
/*
		void RegisterElement( MultiSensorStartBasicElement *AElement )
		{
			FElements.push_back( AElement );
		}
*/
		void ElementReady()
		{
//			Serial.print( "READY: " ); Serial.println( FCountStarted );
			FCountStarted() = FCountStarted() - 1;
			if( ! FCountStarted() )
			{
/*
#ifdef _MITOV_MULTI_SENSOR_START_CLOCK_PIN_
				if( ClockInputPin.IsConnected() )
				{
					if( ! FClocked )
						return;
//				Start();
#endif
*/
//				Serial.println( "RESTART" );
				FStartTime = micros();
			}			
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			unsigned long ACurrentMicros = micros();

			if( FPulsing() )
			{
				if( ( ACurrentMicros - FStartTime ) < PulseTime() )
					return;

				T_StartOutputPin::SetPinValueLow();

				uint32_t ACountStarted = 0;
				T_Elements_StartElement::Call( ACountStarted );
//				for( int i = 0; i < FElements.size(); ++i )
//					if( FElements[ i ]->StartElement())
//						++FCountStarted;

				FCountStarted() = ACountStarted;
				FPulsing() = false;
			}

			else
			{
				if( FCountStarted() )
					return;

#ifdef _MITOV_MULTI_SENSOR_START_CLOCK_PIN_
				if( ClockInputPin_o_IsConnected() )
				{
					if( ! FClocked() )
						return;
				}
				else
#endif
					if( ( micros() - FStartTime ) < ((unsigned long)PauseTime() ) * 1000 )
						return;

				T_StartOutputPin::SetPinValueHigh();
				FPulsing() = true;
				FStartTime = micros();
			}
		}

		inline void SystemStart()
		{
			if( Enabled() )
#ifdef _MITOV_MULTI_SENSOR_START_CLOCK_PIN_
				if( ClockInputPin_o_IsConnected() )
					if( ! FClocked() )
						return;
#endif
			T_StartOutputPin::SetPinValueHigh();
			FPulsing() = true;
			FStartTime = micros();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			FClocked() = true;
		}

	public:
		MultiSensorPulseStart()
		{
			ClockInputPin_o_IsConnected() = false;
			FCountStarted() = 0;
			FClocked() = false;
			FPulsing() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif