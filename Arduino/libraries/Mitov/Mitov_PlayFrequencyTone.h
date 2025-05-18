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
		typename T_Elements_TryActive,
		typename T_Enabled,
		typename T_FPlaying,
		typename T_InitialFrequency,
		uint32_t C_OutputPin
	> class PlayFrequencyTone :
		public T_Enabled,
		public T_FPlaying,
		public T_InitialFrequency
	{
	public:
		_V_PROP_( InitialFrequency )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FPlaying )

	protected:
#ifdef VISUINO_ANY_ESP32
		bool		FAttached = false;
#endif

#ifdef VISUINO_ANY_ESP32
	protected:
		void noTone( uint8_t APin )
		{
			if( ! FAttached )
				return;

			::Mitov::ToneDetach( APin );
			FAttached = false;
		}

		void tone( uint8_t APin, float AFrequency )
		{
			if( ! AFrequency )
				noTone( APin );

			else
			{
				if( ! FAttached )
				{
					::Mitov::ToneAttach( APin );
					FAttached = true;
				}

				::Mitov::PlayTone( APin, AFrequency );
			}
		}

#endif
	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				UpdateFrequency();

			else if( FPlaying() )
			{
				noTone( C_OutputPin );
				FPlaying() = false;
			}
		}

	protected:
		void UpdateFrequency()
		{
			if( InitialFrequency().GetValue() == 0.0f )
			{
				if( FPlaying() )
				{
					noTone( C_OutputPin );
					FPlaying() = false;
				}
			}

			else
			{
				tone( C_OutputPin, InitialFrequency() );
				FPlaying() = true;
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			float AValue = *((float *)_Data);
			if( AValue < 0.0f )
				AValue = 0.0f;

			if( InitialFrequency().GetValue() == AValue )
				return;

			InitialFrequency() = AValue;
			UpdateFrequency();
//			FServo.write( AValue );
		}

	public:
		inline void SystemStart()
		{
			UpdateEnabled();
		}

		inline void SystemLoopBegin() {}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_FPlaying,
		typename T_Enabled,
		typename T_InitialFrequency,
		uint32_t C_OutputPin
	> class PlayFrequencyToneDuration :
		public T_Enabled,
		public T_FPlaying,
		public T_InitialFrequency
	{
	public:
		_V_PROP_( InitialFrequency )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FPlaying )

	protected:
#ifdef VISUINO_ANY_ESP32
		bool		FAttached : 1;
		bool		FStarted : 1;

		unsigned long	FLastTime = 0;
#endif
		uint32_t	FCurentDuration = 0;

#ifdef VISUINO_ANY_ESP32
	protected:
		void noTone( uint8_t APin )
		{
			if( ! FAttached )
				return;

			::Mitov::ToneDetach( APin );
			FAttached = false;
			FStarted = false;
		}

		void tone( uint8_t APin, float AFrequency )
		{
			FStarted = false;
			if( ! AFrequency )
				noTone( APin );

			else
			{
				if( ! FAttached )
				{
					::Mitov::ToneAttach( APin );
					FAttached = true;
				}

				::Mitov::PlayTone( APin, AFrequency );
			}
		}

		void tone( uint8_t APin, float AFrequency, uint32_t ADuration )
		{
			if( ! AFrequency )
			{
				if( FPlaying() )
				{
					noTone( APin );
					FPlaying() = false;
				}

				return;
			}

			FLastTime = millis();
			tone( APin, AFrequency );
			FPlaying() = true;
			FStarted = true;
		}

#endif
	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				UpdateFrequency();

			else if( FPlaying() )
			{
				noTone( C_OutputPin );
				FPlaying() = false;
			}
		}

	protected:
		void UpdateFrequency()
		{
			if( InitialFrequency().GetValue() == 0.0f )
			{
				if( FPlaying() )
				{
					noTone( C_OutputPin );
					FPlaying() = false;
				}
			}

			else
			{
				if( FCurentDuration )
					tone( C_OutputPin, InitialFrequency(), FCurentDuration );

				else
					tone( C_OutputPin, InitialFrequency() );

				FPlaying() = true;
			}
		}

	public:
		void ExternalSetValue( float AFrequency, uint32_t ADuration )
		{
			if( ! ADuration )
				if( ( InitialFrequency().GetValue() == AFrequency ) && ( ! FCurentDuration ))
					return;

			InitialFrequency() = AFrequency;
			FCurentDuration = ADuration;
			UpdateEnabled();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			float AValue = *((float *)_Data);
			if( AValue < 0.0f )
				AValue = 0.0f;

			if( InitialFrequency().GetValue() == AValue )
				return;

			InitialFrequency() = AValue;
			FCurentDuration = 0;
			UpdateFrequency();
//			FServo.write( AValue );
		}

	public:
		inline void SystemStart()
		{
			UpdateFrequency();
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ANY_ESP32
			if( FStarted )
				if( millis() - FLastTime > FCurentDuration )
					if( FPlaying() )
					{
						noTone( C_OutputPin );
						FPlaying() = false;
					}

#endif
		}

	public:
		inline PlayFrequencyToneDuration()
#ifdef VISUINO_ANY_ESP32
			: FStarted( false ),
			FAttached( false )
#endif
		{
			FPlaying() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Duration,
		typename T_Frequency
	> class PlayFrequencyToneSetValueElement :
		public T_Duration,
		public T_Frequency
	{
	public:
		_V_PROP_( Frequency )
		_V_PROP_( Duration )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.ExternalSetValue( Frequency(), Duration() );
		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER> class PlayFrequencyToneStateSetValueElementBasic
	{
	public:
		float		Frequency = 0.0f;
		uint32_t	Duration = 0;

	protected:
		bool	FInputValue = false;

	public:
		bool TryActive( T_OWNER *AOwner )
		{
			if( FInputValue )
			{
				AOwner->ExternalSetValue( Frequency, Duration );
				return true;
			}

			return false;
		}

	};
//---------------------------------------------------------------------------
*/
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Duration,
		typename T_FInputValue,
		typename T_Frequency
	> class PlayFrequencyToneStateSetValueElement : // public PlayFrequencyToneStateSetValueElementBasic<T_OWNER>
		public T_Duration,
		public T_FInputValue,
		public T_Frequency
	{
//		typedef PlayFrequencyToneStateSetValueElementBasic<T_OWNER> inherited;

	public:
		_V_PROP_( Frequency )
		_V_PROP_( Duration )

	protected:
		_V_PROP_( FInputValue )

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

			if( FInputValue() )
			{
				C_OWNER.ExternalSetValue( Frequency(), Duration() );
				AResult = true;
			}

			else
				AResult = false;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue().GetValue() == *(bool *)_Data )
				return;

			FInputValue() = *(bool *)_Data;
			C_OWNER.ResetValue();
		}

	public:
		inline PlayFrequencyToneStateSetValueElement()
		{
			FInputValue() = false;
//			C_OWNER.RegisterElement( this );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_Enabled,
		typename T_FPlaying,
		typename T_InitialFrequency,
		uint32_t C_OutputPin
	> class PlayFrequencyToneDurationState :
		public T_Enabled,
		public T_FPlaying,
		public T_InitialFrequency
	{
	public:
		_V_PROP_( InitialFrequency )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FPlaying )

	protected:
#ifdef VISUINO_ANY_ESP32
		bool		FAttached : 1;
		bool		FStarted : 1;

		unsigned long	FLastTime = 0;
#endif // VISUINO_ANY_ESP32

	protected:
		float		FCurrentFrequency = 0.0;
		uint32_t	FCurentDuration = 0;

//	protected:
//		Mitov::SimpleList<PlayFrequencyToneStateSetValueElementBasic<PlayFrequencyToneStates> *>	FElements;

	public:
		void ResetValue()
		{
//			for( int i = FElements.size(); i -- ; )
//				if( FElements[ i ]->TryActive( this ) )
//					return;

			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

			FCurrentFrequency = InitialFrequency();
			FCurentDuration = 0;
			UpdateEnabled();
		}

		void ExternalSetValue( float AFrequency, uint32_t ADuration )
		{
			if( ! ADuration )
				if( ( InitialFrequency() == AFrequency ) && ( ! FCurentDuration ))
					return;

			FCurrentFrequency = AFrequency;
			FCurentDuration = ADuration;
//			Serial.println( "ExternalSetValue" );
			UpdateEnabled();
		}

#ifdef VISUINO_ANY_ESP32
	protected:
		void noTone( uint8_t APin )
		{
			if( ! FAttached )
				return;

			::Mitov::ToneDetach( APin );
			FAttached = false;
			FStarted = false;
		}

		void tone( uint8_t APin, float AFrequency )
		{
			FStarted = false;
			if( ! AFrequency )
				noTone( APin );

			else
			{
				if( ! FAttached )
				{
					::Mitov::ToneAttach( APin );
					FAttached = true;
				}

				::Mitov::PlayTone( APin, AFrequency );
			}
		}

		void tone( uint8_t APin, float AFrequency, uint32_t ADuration )
		{
			if( ! AFrequency )
			{
				noTone( APin );
				return;
			}

			FLastTime = millis();
			tone( APin, AFrequency );
			FStarted = true;
		}

#endif // VISUINO_ANY_ESP32
/*
	public:
		inline void RegisterElement( PlayFrequencyToneStateSetValueElementBasic<PlayFrequencyToneStates> *AValue )
		{
			FElements.push_back( AValue );
		}
*/
/*
#ifdef SEEEDUINO_WIO_TERMINAL
		inline void noTone( uint8_t APin )
		{
			tone( APin, 0, 0 ); // Workaround Wio library bug!
		}
#endif // SEEEDUINO_WIO_TERMINAL
*/
	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				UpdateFrequency();

			else
			{
				if( FPlaying() )
				{
					noTone( C_OutputPin );
					FPlaying() = false;
				}
			}
		}

	protected:
		void UpdateFrequency()
		{
			if( FCurrentFrequency == 0.0f )
			{
				if( FPlaying() )
				{
					noTone( C_OutputPin );
					FPlaying() = false;
				}
			}

			else
			{
				if( FCurentDuration )
					tone( C_OutputPin, FCurrentFrequency, FCurentDuration );

				else
					tone( C_OutputPin, FCurrentFrequency );

				FPlaying() = true;
			}

		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			float AValue = *((float *)_Data);
			if( AValue < 0.0f )
				AValue = 0.0f;

			if( InitialFrequency().GetValue() == AValue )
				return;

			InitialFrequency() = AValue;
			ResetValue();
//			FServo.write( AValue );
		}

	public:
		inline void SystemStart()
		{
			UpdateFrequency();
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ANY_ESP32
			if( FStarted )
				if( millis() - FLastTime > FCurentDuration )
					if( FPlaying() )
					{
						noTone( C_OutputPin );
						FPlaying() = false;
					}

#endif
		}

	public:
		inline PlayFrequencyToneDurationState()
#ifdef VISUINO_ANY_ESP32
			: FStarted( false ),
			FAttached( false )
#endif
		{
			FPlaying() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif