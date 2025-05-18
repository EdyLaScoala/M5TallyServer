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
	namespace TArduinoClockMode
	{
		enum TArduinoClockMode { Falling, Rising, Change };
	}
//---------------------------------------------------------------------------
	template <
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_ClockMode,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
		typename T_FAValue,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin,
		typename T_UpOutputPin
	> class RotaryEncoderSensor :
		public T_ClockMode,
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
		public T_FAValue,
		public T_FCount,
		public T_InitialValue,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )

	public:
		_V_PROP_( ClockMode )
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

	protected:
		_V_PROP_( FCount )
		_V_PROP_( FAValue )

	public:
		void ExternalSetValue( int32_t AIndex, int32_t AValue )
		{
            if( AValue > Value_Max().Value().GetValue() )
                AValue = Value_Max().Value().GetValue();

            if( AValue < Value_Min().Value().GetValue() )
                AValue = Value_Min().Value().GetValue();

			FCount() = AValue;
			T_OutputPin::SetPinValue( FCount() );
		}

        void ExternalAddValue( int32_t AValue )
        {
			if( AValue > 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				if( FCount() < AMax - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Max().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMax;

				}
			}

			else if( AValue < 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );

				if( FCount() > AMin - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Min().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMin;

				}
			}

			T_OutputPin::SetPinValue( FCount() );
        }

	protected:
		inline void	CountUp()
		{
			if( T_Value_Max::RollOver() || ( FCount() < T_Value_Max::Value() ))
			{
				FCount() = FCount() + 1;  // Bitfields can't use reference for ++
				if( FCount() > T_Value_Max::Value() )
					FCount() = T_Value_Min::Value();

			}
		}

		inline void	CountDown()
		{
			if( T_Value_Min::RollOver() || ( FCount() > T_Value_Min::Value() ))
			{
				FCount() = FCount() - 1;  // Bitfields can't use reference for --
				if( FCount() < T_Value_Min::Value() )
					FCount() = T_Value_Max::Value();

			}
		}

	public:
		void ResetInputPin_o_Receive( void *_Data )
		{
			FCount() = InitialValue();
			T_OutputPin::SetPinValue( FCount() );
		}

		inline void AInputPin_o_Receive( void *_Data )
		{
			bool AValue = *( bool *)_Data;

			if( FAValue() == AValue )
				return;

			FAValue() = AValue;
			if( Enabled() )
			{
				switch( ClockMode() )
				{
					case TArduinoClockMode::Falling:
						if( AValue )
							return;

						break;

					case TArduinoClockMode::Rising:
						if( ! AValue )
							return;

						break;

				}

				if( AValue == C_B_PIN.DigitalRead() )
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountDown();

					T_DownOutputPin::ClockPin();
				}

				else
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountUp();

					T_UpOutputPin::ClockPin();
				}

				T_OutputPin::SetPinValue( FCount() );
/*
				if( AValue )
				{
					if( C_B_PIN.DigitalRead() )
						T_DownOutputPin::ClockPin();

					else
						T_UpOutputPin::ClockPin();
				}
				else
				{
					if( C_B_PIN.DigitalRead() )
						T_UpOutputPin::ClockPin();

					else
						T_DownOutputPin::ClockPin();
				}
*/
			}
		}

/*
		void DoReceiveB( bool AValue )
		{
			FBPopulated = true;
			if( FBValue == AValue )
				return;

			FBValue = AValue;
		}
*/
	public:
		inline void SystemInit()
		{
//			Serial.println( "Counter::SystemInit" );
			FCount() = InitialValue();

			T_OutputPin::SetPinValue( FCount() );
		}

		inline void SystemStart()
		{
			if( FCount() != InitialValue() )
				T_OutputPin::SetPinValue( FCount() );

		}

		inline void SystemLoopBegin() {} // Placeholder

	public:
		inline RotaryEncoderSensor()
//			FAPopulated( false ),
//			FBPopulated( false ),
//			FBValue( false )
		{
			FAValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_ClockMode,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin,
		typename T_UpOutputPin
	> class RotaryEncoderSensor_Interrupt : // public OpenWire::InterruptObject,
		public T_ClockMode,
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
		public T_FCount,
		public T_InitialValue,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )

	public:
		_V_PROP_( ClockMode )
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

//	protected:
//		_V_PROP_( FAValue )

	protected:
		_V_PROP_( FCount )

	public:
		void ExternalSetValue( int32_t AIndex, int32_t AValue )
		{
            if( AValue > Value_Max().Value().GetValue() )
                AValue = Value_Max().Value().GetValue();

            if( AValue < Value_Min().Value().GetValue() )
                AValue = Value_Min().Value().GetValue();

			FCount() = AValue;
			T_OutputPin::SetPinValue( FCount() );
		}

	protected:
		int8_t FUpCount = 0;

	public:
		void __ICACHE_RAM_ATTR__ InterruptHandlerA( bool AValue )
		{
			if( Enabled() )
			{
				switch( ClockMode() )
				{
					case TArduinoClockMode::Falling:
						if( AValue )
							return;

						break;

					case TArduinoClockMode::Rising:
						if( ! AValue )
							return;

						break;

				}

				if( AValue == C_B_PIN.DigitalRead() )
					--FUpCount;

				else
					++FUpCount;

			}
		}

/*
	protected:
		void DoReceiveB( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			FBPopulated = true;
			if( FBValue == AValue )
				return;

			FBValue = AValue;
		}
*/
	protected:
		inline void	CountUp()
		{
			if( T_Value_Max::RollOver() || ( FCount() < T_Value_Max::Value() ))
			{
				FCount() = FCount() + 1;  // Bitfields can't use reference for ++
				if( FCount() > T_Value_Max::Value() )
					FCount() = T_Value_Min::Value();

			}
		}

		inline void	CountDown()
		{
			if( T_Value_Min::RollOver() || ( FCount() > T_Value_Min::Value() ))
			{
				FCount() = FCount() - 1;  // Bitfields can't use reference for --
				if( FCount() < T_Value_Min::Value() )
					FCount() = T_Value_Max::Value();

			}
		}

	public:
		void ResetInputPin_o_Receive( void *_Data )
		{
			FCount() = InitialValue();
			T_OutputPin::SetPinValue( FCount() );
		}

	public:
		inline void SystemInit()
		{
//			Serial.println( "Counter::SystemInit" );
			FCount() = InitialValue();

			T_OutputPin::SetPinValue( FCount() );
		}

		inline void SystemStart()
		{
			if( FCount() != InitialValue() )
				T_OutputPin::SetPinValue( FCount() );

		}

		inline void SystemLoopBegin()
		{
			noInterrupts();

			int8_t AUpCount = FUpCount;
			FUpCount = 0;

			interrupts();

			if( ! AUpCount )
				return;

			if( AUpCount < 0 )
			{
				while( AUpCount ++ )
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountDown();

					T_DownOutputPin::ClockPin();
				}
			}

			else
			{
				while( AUpCount -- )
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountUp();

					T_UpOutputPin::ClockPin();
				}
			}

			T_OutputPin::SetPinValue( FCount() );
		}

/*
	public:
		inline RotaryEncoderSensor_Interrupt()
//			FAPopulated( false ),
//			FBPopulated( false ),
//			FBValue( false )
		{
			FAValue() = false;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_ClockMode,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
		typename T_FALastValue,
		typename T_FAValue,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin,
		typename T_UpOutputPin
	> class RotaryEncoderSensor_Debounce :
		public T_ClockMode,
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
		public T_FALastValue,
		public T_FAValue,
		public T_FCount,
		public T_InitialValue,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )

	public:
		_V_PROP_( ClockMode )
		_V_PROP_( InitialValue )
		_V_PROP_( DebounceInterval )
		_V_PROP_( Enabled )
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

	protected:
		_V_PROP_( FCount )
		_V_PROP_( FAValue )

		_V_PROP_( FALastValue )

	public:
		void ExternalSetValue( int32_t AIndex, int32_t AValue )
		{
            if( AValue > Value_Max().Value().GetValue() )
                AValue = Value_Max().Value().GetValue();

            if( AValue < Value_Min().Value().GetValue() )
                AValue = Value_Min().Value().GetValue();

			FCount() = AValue;
			T_OutputPin::SetPinValue( FCount() );
		}

        void ExternalAddValue( int32_t AValue )
        {
			if( AValue > 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				if( FCount() < AMax - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Max().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMax;

				}
			}

			else if( AValue < 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );

				if( FCount() > AMin - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Min().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMin;

				}
			}

			T_OutputPin::SetPinValue( FCount() );
        }

	protected:
		unsigned long	FALastTime = 0;

	protected:
		inline void	CountUp()
		{
			if( T_Value_Max::RollOver() || ( FCount() < T_Value_Max::Value() ))
			{
				FCount() = FCount() + 1;  // Bitfields can't use reference for ++
				if( FCount() > T_Value_Max::Value() )
					FCount() = T_Value_Min::Value();

			}
		}

		inline void	CountDown()
		{
			if( T_Value_Min::RollOver() || ( FCount() > T_Value_Min::Value() ))
			{
				FCount() = FCount() - 1;  // Bitfields can't use reference for --
				if( FCount() < T_Value_Min::Value() )
					FCount() = T_Value_Max::Value();

			}
		}

	protected:
		void DoValueReceiveA( bool AValue )
		{
			if( FAValue() == AValue )
				return;

			FAValue() = AValue;
			if( Enabled() )
			{
				switch( ClockMode() )
				{
					case TArduinoClockMode::Falling:
						if( AValue )
							return;

						break;

					case TArduinoClockMode::Rising:
						if( ! AValue )
							return;

						break;

				}

				if( AValue == C_B_PIN.DigitalRead() )
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountDown();

					T_DownOutputPin::ClockPin();
				}

				else
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountUp();

					T_UpOutputPin::ClockPin();
				}

				T_OutputPin::SetPinValue( FCount() );
			}
		}

/*
		void DoValueReceiveB( bool AValue )
		{
			FBPopulated = true;
			if( FBValue == AValue )
				return;

			FBValue = AValue;
		}
*/
	public:
		void ResetInputPin_o_Receive( void *_Data )
		{
			FCount() = InitialValue();
			T_OutputPin::SetPinValue( FCount() );
		}

		inline void AInputPin_o_Receive( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			if( AValue != FALastValue() )
			{
				FALastTime = millis();
//				FInTimer = true;
				FALastValue() = AValue;
			}

		}

/*
		void DoReceiveB( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			if( AValue != FBLastValue )
				FBLastTime = millis();

			FBLastValue = AValue;
		}
*/

	public:
		inline void SystemInit()
		{
//			Serial.println( "Counter::SystemInit" );
			FCount() = InitialValue();

			T_OutputPin::SetPinValue( FCount() );
		}

		inline void SystemStart()
		{
			if( FCount() != InitialValue() )
				T_OutputPin::SetPinValue( FCount() );

		}

		inline void SystemLoopBegin()
		{
			if( FAValue().GetValue() != FALastValue().GetValue() )
				if( millis() - FALastTime > DebounceInterval() )
					DoValueReceiveA( FALastValue() );

/*
			if( FBValue != FBLastValue )
				if( millis() - FBLastTime > DebounceInterval )
					DoValueReceiveB( FBLastValue );
*/
		}

	public:
		inline RotaryEncoderSensor_Debounce()
//			FInTimer( false )
		{
			FAValue() = false;
			FALastValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_ClockMode,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
//		typename T_FInTimer,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin,
//		typename T_FAValue,
		typename T_UpOutputPin
	> class RotaryEncoderSensor_Interrupt_Debounce : // public OpenWire::InterruptObject,
		public T_ClockMode,
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
//		public T_FInTimer,
		public T_FCount,
		public T_InitialValue,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin,
//		public T_FAValue,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ClockMode )
		_V_PROP_( InitialValue )
		_V_PROP_( DebounceInterval )
		_V_PROP_( Enabled )
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

	protected:
		_V_PROP_( FCount )

	public:
		void ExternalSetValue( int32_t AIndex, int32_t AValue )
		{
            if( AValue > Value_Max().Value().GetValue() )
                AValue = Value_Max().Value().GetValue();

            if( AValue < Value_Min().Value().GetValue() )
                AValue = Value_Min().Value().GetValue();

			FCount() = AValue;
			T_OutputPin::SetPinValue( FCount() );
		}

        void ExternalAddValue( int32_t AValue )
        {
			if( AValue > 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				if( FCount() < AMax - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Max().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMax;

				}
			}

			else if( AValue < 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );

				if( FCount() > AMin - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Min().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMin;

				}
			}

			T_OutputPin::SetPinValue( FCount() );
        }

	protected:
//		_V_PROP_( FAValue )

//		_V_PROP_( FInTimer )
		int8_t FUpCount = 0;

		unsigned long	FALastTime = 0;

//		bool FLastB = false;

	public:
		void __ICACHE_RAM_ATTR__ InterruptHandlerA( bool AValue )
		{
			unsigned long	ATime = millis();
			unsigned long	AInterval = ( ATime - FALastTime );
			FALastTime = ATime;

			switch( ClockMode() )
			{
				case TArduinoClockMode::Falling:
					if( AValue )
						return;

					break;

				case TArduinoClockMode::Rising:
					if( ! AValue )
						return;

					break;

			}

//			if( AValue )
//				return;

//			if( FAValue == AValue )
//				return;

//			FAValue = AValue;
			if( Enabled() )
			{
				bool ABValue = C_B_PIN.DigitalRead();
//				Serial.println( ABValue );
//				if( FLastB == ABValue )
//					return;

//				if( FInTimer() )
					if( AInterval <= DebounceInterval().GetValue() )
						return;

//				FLastB = ABValue;
//				Serial.println( AInterval );

//				FInTimer() = true;
//				FALastTime = ATime;
//				if( AValue )
				if( AValue == ABValue )
					--FUpCount;

				else
					++FUpCount;

			}
		}

	protected:
		inline void	CountUp()
		{
			if( T_Value_Max::RollOver() || ( FCount() < T_Value_Max::Value() ))
			{
				FCount() = FCount() + 1;  // Bitfields can't use reference for ++
				if( FCount() > T_Value_Max::Value() )
					FCount() = T_Value_Min::Value();

			}
		}

		inline void	CountDown()
		{
			if( T_Value_Min::RollOver() || ( FCount() > T_Value_Min::Value() ))
			{
				FCount() = FCount() - 1;  // Bitfields can't use reference for --
				if( FCount() < T_Value_Min::Value() )
					FCount() = T_Value_Max::Value();

			}
		}

	public:
		void ResetInputPin_o_Receive( void *_Data )
		{
			FCount() = InitialValue();
			T_OutputPin::SetPinValue( FCount() );
		}

	public:
		inline void SystemInit()
		{
//			Serial.println( "Counter::SystemInit" );
			FCount() = InitialValue();

			T_OutputPin::SetPinValue( FCount() );
		}

		inline void SystemStart()
		{
			if( FCount() != InitialValue() )
				T_OutputPin::SetPinValue( FCount() );

		}

		inline void SystemLoopBegin()
		{
			unsigned long ATime = millis();

			noInterrupts();

			int8_t AUpCount = FUpCount;
			FUpCount = 0;

			interrupts();

			if( ! AUpCount )
				return;

			if( AUpCount < 0 )
			{
				while( AUpCount ++ )
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountDown();

					T_DownOutputPin::ClockPin();
				}
			}

			else
			{
				while( AUpCount -- )
				{
					if( T_OutputPin::GetPinIsConnected() )
						CountUp();

					T_UpOutputPin::ClockPin();
				}
			}

			T_OutputPin::SetPinValue( FCount() );
		}

	public:
		inline RotaryEncoderSensor_Interrupt_Debounce()
		{
			FALastTime = millis();
//			FInTimer() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif