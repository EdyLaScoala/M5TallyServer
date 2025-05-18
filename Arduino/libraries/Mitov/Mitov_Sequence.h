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

// #define __SEQUENCE__DEBUG__


namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Delay,
		typename T_Enabled,
		uint32_t C_INDEX, 
		typename T_InMicroSeconds,
		typename T_OutputPin
	> class SequenceElement : 
		public T_OutputPin,
		public T_InMicroSeconds,
		public T_Delay,
		public T_Enabled
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Delay )
		_V_PROP_( InMicroSeconds )
		_V_PROP_( Enabled )

	protected:
		inline uint32_t GetTime()
		{
			if( InMicroSeconds() )
				return micros();

			else
				return millis();
		}

	public:
		void MoveToNextIndex( int32_t *& ACurrentIndex )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! ACurrentIndex )
				return;

#ifdef __SEQUENCE__DEBUG__
			Serial.println( "MoveToNextIndex" );
			Serial.println( C_INDEX );
			Serial.println( *ACurrentIndex );
#endif // __SEQUENCE__DEBUG__

			if( int32_t( C_INDEX ) > *ACurrentIndex )
			{
				C_OWNER.StartPeriod( Delay().GetValue() );
	//					Serial.println( "TEST2" );
				*ACurrentIndex = C_INDEX;
				ACurrentIndex = nullptr;
			}

		}

		void SetElementValue( bool AValue )
		{
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "SetElementValue" );
			Serial.print( "Value: " );
			Serial.println( AValue );
#endif // __SEQUENCE__DEBUG__

			if( AValue )
				C_OWNER.FStartTime = GetTime();

			else
				T_OutputPin::ClockPin();

		}

	public:
		inline void StartInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			C_OWNER.StartElement( C_INDEX );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SignalElement( C_INDEX );
		}

	public:
		inline void SystemLoopBegin()
		{
			uint32_t ACurrentIndex = C_OWNER.FCurrentIndex();
			if( C_INDEX != ACurrentIndex )
				return;

			if( C_OWNER.Enabled() )
				if( ACurrentIndex >= 0 )
					if( ( GetTime() - C_OWNER.FStartTime ) >= Delay() )
					{
#ifdef __SEQUENCE__DEBUG__
						Serial.print( "Signal: " );
						Serial.println( ACurrentIndex );
#endif // __SEQUENCE__DEBUG__
						C_OWNER.SignalElement( ACurrentIndex );
					}

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Delay,
		typename T_Enabled,
		uint32_t C_INDEX,
		typename T_InMicroSeconds,
		typename T_OutputPin
	> class SequenceElementDigital : 
		public T_OutputPin,
		public T_Delay,
		public T_InMicroSeconds,
		public T_Enabled
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Delay )
		_V_PROP_( InMicroSeconds )
		_V_PROP_( Enabled )

	protected:
		inline uint32_t GetTime()
		{
			if( InMicroSeconds() )
				return micros();

			else
				return millis();
		}

	public:
		inline void InitElementValue()
		{
			T_OutputPin::SetPinValue( false );
		}

		void MoveToNextIndex( int32_t *& ACurrentIndex )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! ACurrentIndex )
				return;

			if( int32_t( C_INDEX ) > *ACurrentIndex )
			{
				C_OWNER.StartPeriod( Delay().GetValue() );
				*ACurrentIndex = C_INDEX;
				ACurrentIndex = nullptr;
			}
		}

		inline void SetElementValue( bool AValue )
		{
			T_OutputPin::SetPinValue( AValue );

			if( AValue )
				C_OWNER.FStartTime = GetTime();
		}

	public:
		inline void StartInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			C_OWNER.StartElement( C_INDEX );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SignalElement( C_INDEX );
		}

	public:
		inline void SystemLoopBegin()
		{
			uint32_t ACurrentIndex = C_OWNER.FCurrentIndex();
			if( C_INDEX != ACurrentIndex )
				return;

			if( C_OWNER.Enabled() )
				if( ACurrentIndex >= 0 )
					if( ( GetTime() - C_OWNER.FStartTime ) >= Delay() )
					{
#ifdef __SEQUENCE__DEBUG__
						Serial.print( "Signal: " );
						Serial.println( ACurrentIndex );
#endif // __SEQUENCE__DEBUG__
						C_OWNER.SignalElement( ACurrentIndex );
					}


		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_CHANNELS_COUNT,
		typename T_CanRestart,
		typename T_Count,
		typename T_Elements_InitElementValue,
		typename T_Elements_MoveToNextIndex,
		typename T_Elements_SetElementValue,
		typename T_Enabled,
		typename T_FCurrentIndex,
		uint32_t C_INDEX
	> class SequenceElementRepeat :
		public T_CanRestart,
		public T_Count,
		public T_Enabled,
		public T_FCurrentIndex
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( CanRestart )
		_V_PROP_( Count )

	public:
		_V_PROP_( FCurrentIndex )

	public:
		uint32_t	FStartTime;
		uint32_t	FCount;

	public:
		void SignalElement( uint32_t AIndex )
		{
			if( ! Enabled() )
				return;

			if( FCurrentIndex() == C_CHANNELS_COUNT + 1 ) //< 0 )
				return;

			if( AIndex != FCurrentIndex() )
				return;

#ifdef __SEQUENCE__DEBUG__
			Serial.print( "SignalElement: " );
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__

			T_Elements_SetElementValue::Call( FCurrentIndex(), false );
//			SetElementToFalse( FCurrentIndex() );

			MoveToNextIndex();

			if( FCurrentIndex() < C_CHANNELS_COUNT )
				;
//				FStartTime = millis();

			else if( FCount < Count().GetValue() )
				StartSequence( true );
//				StartInputPin_o_Receive( nullptr );

			else
			{
				FCurrentIndex() = C_CHANNELS_COUNT + 1;
//				C_OWNER.MoveToNextIndex();
				if( C_OWNER.Enabled() )
				{
					uint32_t ACurrentIndex = C_OWNER.FCurrentIndex();
					if( ACurrentIndex >= 0 )
					{
#ifdef __SEQUENCE__DEBUG__
						Serial.print( "Signal: " );
						Serial.println( ACurrentIndex );
#endif // __SEQUENCE__DEBUG__
						C_OWNER.SignalElement( ACurrentIndex );
					}
				}

				return;
			}

			T_Elements_SetElementValue::Call( FCurrentIndex(), true );
//			SetElementToTrue( FCurrentIndex() );
		}

	public:
		void StartPeriod( uint32_t AValue )
		{
			C_OWNER.StartPeriod( AValue );
		}

		void MoveToNextIndex()
		{
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "MoveToNextIndex" );
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__
			int32_t ACurrentIndex = ( FCurrentIndex() == C_CHANNELS_COUNT + 1 ) ? -1 : FCurrentIndex();

			int32_t *ACurrentIndexPrt = &ACurrentIndex; // Will be nulled once it moves to next

//			Serial.println( "ACurrentIndexPrt" );
//			Serial.println( *ACurrentIndexPrt );

			T_Elements_MoveToNextIndex::Call( ACurrentIndexPrt );
#ifdef __SEQUENCE__DEBUG__
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__
			if( ACurrentIndexPrt )
				FCurrentIndex() = C_CHANNELS_COUNT;

			else
				FCurrentIndex() = ACurrentIndex;

		}

	public:
		void MoveToNextIndex( int32_t *& ACurrentIndex )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! ACurrentIndex )
				return;

#ifdef __SEQUENCE__DEBUG__
			Serial.println( "MoveToNextIndex 1" );
			Serial.println( C_INDEX );
			Serial.println( *ACurrentIndex );
#endif // __SEQUENCE__DEBUG__

			if( int32_t( C_INDEX ) > *ACurrentIndex )
			{
//				C_OWNER.StartPeriod( Delay().GetValue() );
//					Serial.println( "TEST2" );
				*ACurrentIndex = C_INDEX;
				ACurrentIndex = nullptr;
			}
		}

		void SetElementValue( bool AValue )
		{
			if( AValue )
				StartSequence( false );
		}

	public:
		inline void StartInputPin_o_Receive( void *_Data )
		{
			StartSequence( false );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SignalElement( C_INDEX );
		}

	protected:
		void StartSequence( bool AIncrementCount )
		{
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "START" );
			Serial.println( FCurrentIndex() );
			Serial.println( CanRestart() );
#endif // __SEQUENCE__DEBUG__
			if( Enabled() )
				if( C_CHANNELS_COUNT )
					if( CanRestart() || ( FCurrentIndex() == C_CHANNELS_COUNT + 1 )) //-1 ))
					{
						FCurrentIndex() = C_CHANNELS_COUNT + 1; //-1;
						MoveToNextIndex();

						if( FCurrentIndex() < C_CHANNELS_COUNT )
						{
							T_Elements_SetElementValue::Call( FCurrentIndex(), true );
//							SetElementToTrue( FCurrentIndex() );
//							FStartTime = millis();
						}

					}

		}

	public:
		inline SequenceElementRepeat()
		{
			FCurrentIndex() = C_CHANNELS_COUNT + 1;
		}

	};
//---------------------------------------------------------------------------
	template <
		uint32_t C_CHANNELS_COUNT,
		typename T_CanRestart,
		typename T_Elements_InitElementValue,
		typename T_Elements_MoveToNextIndex,
		typename T_Elements_SetElementValue,
		typename T_Enabled,
		typename T_FCurrentIndex,
		typename T_IntervalOutputPin,
		typename T_Repeat
	> class Sequence : 
		public T_Enabled,
		public T_CanRestart,
		public T_FCurrentIndex,
		public T_IntervalOutputPin,
		public T_Repeat
	{
	public:
		_V_PIN_( IntervalOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Repeat )
		_V_PROP_( CanRestart )

	public:
		_V_PROP_( FCurrentIndex )

	public:
		uint32_t	FStartTime;

	public:
		void UpdateEnabled() // Used for Visual Live Binding
		{
			if( ! Enabled() )
				return;

			StartInputPin_o_Receive( nullptr );
		}

	public:
		void StartPeriod( uint32_t AValue )
		{
			T_IntervalOutputPin::SetPinValue( AValue );
		}

		void MoveToNextIndex()
		{
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "MoveToNextIndex" );
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__
			int32_t ACurrentIndex = ( FCurrentIndex() == C_CHANNELS_COUNT + 1 ) ? -1 : FCurrentIndex();

			int32_t *ACurrentIndexPrt = &ACurrentIndex; // Will be nulled once it moves to next

//			Serial.println( "ACurrentIndexPrt" );
//			Serial.println( *ACurrentIndexPrt );

			T_Elements_MoveToNextIndex::Call( ACurrentIndexPrt );
#ifdef __SEQUENCE__DEBUG__
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__
			if( ACurrentIndexPrt )
				FCurrentIndex() = C_CHANNELS_COUNT;

			else
				FCurrentIndex() = ACurrentIndex;
				
		}

	public:
		void StartElement( uint32_t AIndex )
		{
			if( ! Enabled() )
				return;

			FCurrentIndex() = AIndex;
		}

		void SignalElement( uint32_t AIndex )
		{
			if( ! Enabled() )
				return;

			if( FCurrentIndex() == C_CHANNELS_COUNT + 1 ) //< 0 )
				return;

			if( AIndex != FCurrentIndex() )
				return;

#ifdef __SEQUENCE__DEBUG__
			Serial.print( "SignalElement: " );
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__

//			Serial.print( "SignalElement1: " );
//			Serial.println( FCurrentIndex );

			T_Elements_SetElementValue::Call( FCurrentIndex(), false );
//			SetElementToFalse( FCurrentIndex() );

			MoveToNextIndex();

			if( FCurrentIndex() < C_CHANNELS_COUNT )
				;
//				FStartTime = millis();

			else if( Repeat() )
				StartInputPin_o_Receive( nullptr );

			else
			{
				FCurrentIndex() = C_CHANNELS_COUNT + 1; //-1
				return;
			}

			T_Elements_SetElementValue::Call( FCurrentIndex(), true );
//			SetElementToTrue( FCurrentIndex() );
//			Serial.print( "FCurrentIndex: " ); Serial.println( FCurrentIndex );
		}

	public:
		void StartInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "----" );
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "START" );
			Serial.println( FCurrentIndex() );
			Serial.println( CanRestart() );
#endif // __SEQUENCE__DEBUG__
			if( Enabled() )
				if( C_CHANNELS_COUNT )
					if( CanRestart() || ( FCurrentIndex() == C_CHANNELS_COUNT + 1 )) //-1 ))
					{
						FCurrentIndex() = C_CHANNELS_COUNT + 1; //-1;
						MoveToNextIndex();

						if( FCurrentIndex() < C_CHANNELS_COUNT )
						{
							T_Elements_SetElementValue::Call( FCurrentIndex(), true );
//							SetElementToTrue( FCurrentIndex() );
//							FStartTime = millis();
						}

					}


		}

	public:
		inline void SystemStart()
		{
//			Serial.println( "TEST2" );
			StartInputPin_o_Receive( nullptr );
		}

	public:
		inline Sequence()
		{
			FCurrentIndex() = C_CHANNELS_COUNT + 1; //-1;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif