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
		typename T_FFirstTime,
		typename T_FZeroed,
		typename T_MinFrequency,
		typename T_OutputPin
	> class FrequencyMeter :
		public T_Enabled,
		public T_FFirstTime,
		public T_FZeroed,
		public T_MinFrequency,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( MinFrequency )
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FFirstTime )
		_V_PROP_( FZeroed )
		unsigned long	FStartTime = 0;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FFirstTime() )
			{
				FFirstTime() = false;
				FStartTime = micros();
				return;
			}

            if( ! Enabled() )
				return;

			unsigned long ANow = micros();
			unsigned long APeriod = ANow - FStartTime;
			FStartTime = ANow;

			if( APeriod == 0 )
				APeriod = 1;

			float AFrequency =  1000000.0f / APeriod;
			T_OutputPin::SetPinValue( AFrequency );
			FZeroed() = false;
		}

	public:
		inline void SystemLoopBegin() // For compatibility with Interrupt version
		{
			if( ! Enabled() )
				return;

			if( FZeroed() )
				return;

			if( FFirstTime() )
			{
				T_OutputPin::SetPinValue( 0 );
				FZeroed() = true;
			}

			unsigned long ACurrentMicros = micros();
			unsigned long ATimeDiff = ( ACurrentMicros - FStartTime );
            unsigned long ASamplePeriod = ( 1000000 / MinFrequency() ) + 0.5;
//			if( ATimeDiff > 1000000 )
			if( ATimeDiff > ASamplePeriod )
			{
				float AFrequency =  1000000.0f / ATimeDiff;
				if( AFrequency < MinFrequency() )
				{
					T_OutputPin::SetPinValue( 0 );
					FZeroed() = true;
				}

				else
					T_OutputPin::SetPinValue( AFrequency );

//				FZeroed() = true;
//				FStartTime = ACurrentMicros;
			}
		}

	public:
		inline FrequencyMeter()
		{
			FFirstTime() = true;
			FZeroed() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FFirstTime,
		typename T_FZeroed,
		typename T_MinFrequency,
		typename T_OutputPin
	> class FrequencyMeter_Interrupt : 
//		public OpenWire::InterruptObject,
		public T_Enabled,
		public T_FFirstTime,
		public T_FZeroed,
		public T_MinFrequency,
		public T_OutputPin
	{
    public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( MinFrequency )
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FFirstTime )
		_V_PROP_( FZeroed )
		unsigned long	FLastTime = 0;
		uint32_t FCounter = 0;

	public:
		inline void __ICACHE_RAM_ATTR__ InterruptHandler( bool )
		{
			++ FCounter;
		}

/*
	protected:
		void InputPin_o_Receive( void *_Data )
		{
			if( FFirstTime )
			{
				FFirstTime = false;
				FStartTime = micros();
				return;
			}

            if( ! Enabled )
				return;

			unsigned long ANow = micros();
			unsigned long APeriod = ANow - FStartTime;
			FStartTime = ANow;

			if( APeriod == 0 )
				APeriod = 1;

			float AFrequency =  1000000.0f / APeriod;
			OutputPin.Notify( &AFrequency );
		}
*/
	public:
		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ! Enabled() )
			{
				noInterrupts(); // Protect the FReceivedPackets from being corrupted
				FCounter = 0;
				interrupts();
				FLastTime = ACurrentMicros;
				return;
			}				

			unsigned long ATimeDiff = ( ACurrentMicros - FLastTime );

			noInterrupts(); // Protect the FReceivedPackets from being corrupted
			uint32_t ACount = FCounter;
			if( ( ! ACount ) && ( ATimeDiff < 100000 ) )
			{
				interrupts();
				return;
			}

			FCounter = 0;

			interrupts();

			if( FFirstTime() )
				if( ! ACount )
				{
					if( ! FZeroed() )
					{
						T_OutputPin::SetPinValue( 0 );
						FZeroed() = true;
					}

					return;
				}

//			Serial.print( "ACount = " );
//			Serial.println( ACount );

			if( ! ACount )
				ACount = 1;

			else
				FLastTime = ACurrentMicros;

			float AFrequency = ( float( ACount ) / ATimeDiff ) * 1000000;
			if( AFrequency < MinFrequency() )
			{
				if( ! FZeroed() )
				{
					T_OutputPin::SetPinValue( 0 );
					FZeroed() = true;
				}
			}

			else
			{
				FFirstTime() = false;
				FZeroed() = false;
				T_OutputPin::SetPinValue( AFrequency );
			}

//			Serial.println( AFrequency );

//			if( ACount )
//				FLastTime = ACurrentMicros;
		}

	public:
/*
		FrequencyMeter_Interrupt( int AInterruptPin, void (*AInterruptRoutine)() )
//			Enabled( true )
//			FFirstTime( true )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			// On all other platforms, its innocuous, belt and braces
			pinMode( AInterruptPin, INPUT ); 

			int AInterruptNumber = digitalPinToInterrupt( AInterruptPin );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, RISING );
		}
*/
		inline FrequencyMeter_Interrupt()
		{
			FFirstTime() = true;
			FZeroed() = false;
		}
	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif