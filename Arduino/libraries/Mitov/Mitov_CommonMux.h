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
		typename T_InitialSelectValue,
		typename T_OutputPin,
		typename T
	> class ToggleSwitch :
		public T_InitialSelectValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialSelectValue )

	protected:
		T	FTrueValue;
		T	FFalseValue;

	public:
		inline void TrueInputPin_o_Receive( void *_Data )
		{
//			FTrueValue = *(T *)_Data;
			FTrueValue = ::Mitov::template ExtractValue<T>( _Data );
		}

		inline void FalseInputPin_o_Receive( void *_Data )
		{
//			FFalseValue = *(T *)_Data;
			FFalseValue = ::Mitov::template ExtractValue<T>( _Data );
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			InitialSelectValue() = *(bool *)_Data;
			CalculateSendOutput( false );
		}

	protected:
		inline void CalculateSendOutput( bool AFromStart )
		{
			T AValue = ( InitialSelectValue() ) ? FTrueValue : FFalseValue;
			T_OutputPin::SetPinValue( AValue, ! AFromStart );
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			CalculateSendOutput( false );
		}

	public:
		inline ToggleSwitch( T AInitValue ) :
			FTrueValue( AInitValue ),
			FFalseValue( AInitValue )
		{
		}

		inline ToggleSwitch() {}

	};
//---------------------------------------------------------------------------
/*
	template<> void ToggleSwitch<Mitov::String>::TrueInputPin_o_Receive( void *_Data )
	{
		FTrueValue = (char *)_Data;
	}
//---------------------------------------------------------------------------
	template<> void ToggleSwitch<Mitov::String>::FalseInputPin_o_Receive( void *_Data )
	{
		FFalseValue = (char *)_Data;
	}
*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
	template <
		typename T_FFalseValue,
		typename T_FTrueValue,
		typename T_InitialSelectValue,
		typename T_OutputPin
	> class DigitalToggleSwitch :
		public T_FFalseValue,
		public T_FTrueValue,
		public T_InitialSelectValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialSelectValue )

	protected:
		_V_PROP_( FTrueValue )
		_V_PROP_( FFalseValue )

	public:
		inline void TrueInputPin_o_Receive( void *_Data )
		{
			FTrueValue() = *(bool *)_Data;
		}

		inline void FalseInputPin_o_Receive( void *_Data )
		{
			FFalseValue() = *(bool *)_Data;
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			InitialSelectValue() = *(bool *)_Data;
			CalculateSendOutput( false );
		}

	protected:
		inline void CalculateSendOutput( bool AFromStart )
		{
			bool AValue = ( InitialSelectValue() ) ? FTrueValue() : FFalseValue();
			T_OutputPin::SetPinValue( AValue, ! AFromStart );
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			CalculateSendOutput( false );
		}

	public:
		inline DigitalToggleSwitch()
		{
			FTrueValue() = false;
			FFalseValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialSelectValue,
		typename T_OutputPin
	> class ClockToggleSwitch :
		public T_InitialSelectValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialSelectValue )

	public:
		inline void SelectInputPin_o_Receive( void *_Data )
		{
			InitialSelectValue() = *(bool *)_Data;
		}

		void TrueInputPin_o_Receive( void *_Data )
		{
			if( InitialSelectValue() )
				T_OutputPin::ClockPin();
		}

		void FalseInputPin_o_Receive( void *_Data )
		{
			if( ! InitialSelectValue() )
				T_OutputPin::ClockPin();
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_InitialChannel,
		typename T_OutputPin,
		typename T
	> class CommonMux :
		public T_InitialChannel,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PIN_( InitialChannel )

	protected:
		T		FValues[ C_NUM_INPUTS ];

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FValues[ AIndex ] = *(T *)_Data;
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			uint32_t AChannel = *(uint32_t *)_Data;
			if( AChannel >= C_NUM_INPUTS )
				AChannel = C_NUM_INPUTS - 1;

			if( InitialChannel().GetValue() == AChannel )
				return;

//			Serial.println( AChannel );

			InitialChannel() = AChannel;
			CalculateSendOutput( false );
		}

	protected:
		void CalculateSendOutput( bool AFromStart )
		{
//			Serial.println( "CalculateSendOutput+++" );
			T AValue = FValues[ InitialChannel().GetValue() ];
//			Serial.println( AValue );
			T_OutputPin::SetPinValue( AValue, ! AFromStart );
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			CalculateSendOutput( false );
		}

	public:
		inline CommonMux()
		{
			 // Use memset for TDateTime compatibility!
			memset( FValues, 0, sizeof( FValues ));
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_InitialChannel,
		typename T_OutputPin
	> class TextMux :
		public T_InitialChannel,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialChannel )

	protected:
		Mitov::String	FValues[ C_NUM_INPUTS ];

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FValues[ AIndex ] = (char *)_Data;
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			uint32_t AChannel = *(uint32_t *)_Data;
			if( AChannel >= C_NUM_INPUTS )
				AChannel = C_NUM_INPUTS - 1;

			if( InitialChannel().GetValue() == AChannel )
				return;

//			Serial.println( AChannel );

			InitialChannel() = AChannel;
			CalculateSendOutput( false );
		}

	protected:
		inline void CalculateSendOutput( bool AFromStart )
		{
//			Serial.println( "CalculateSendOutput+++" );
			Mitov::String AValue = FValues[ InitialChannel().GetValue() ];
//			Serial.println( AValue );
			T_OutputPin::SetPinValue( AValue, ! AFromStart ); // Use Send Value for String Compatibility!
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			CalculateSendOutput( false );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_InitialChannel,
		typename T_OutputPin
	> class DigitalMux :
		public T_InitialChannel,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialChannel )

	protected:
		uint8_t		FValues[ ( C_NUM_INPUTS + 7 ) / 8 ] = { 0 };

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( AValue )
				FValues[ AIndex / 8 ] |= 1 << ( AIndex & 0b111 );  // same as % 8

			else
				FValues[ AIndex / 8 ] &= ~( 1 << ( AIndex & 0b111 ));  // same as % 8

		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			uint32_t AChannel = *(uint32_t *)_Data;
			if( AChannel >= C_NUM_INPUTS )
				AChannel = C_NUM_INPUTS - 1;

			if( InitialChannel().GetValue() == AChannel )
				return;

//			Serial.println( AChannel );

			InitialChannel() = AChannel;
			CalculateSendOutput( false );
		}

	protected:
		inline void CalculateSendOutput( bool AFromStart )
		{
//			Serial.println( "CalculateSendOutput+++" );
			bool AValue = (( FValues[ InitialChannel().GetValue() / 8 ] & ( 1 << ( InitialChannel().GetValue() & 0b111 ))) != 0 );
//			Serial.println( AValue );

			T_OutputPin::SetPinValue( AValue, ! AFromStart );
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			CalculateSendOutput( false );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_InitialChannel,
		typename T_OutputPin
	> class ClockMux :
		public T_InitialChannel,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialChannel )

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			if( AIndex == InitialChannel().GetValue() )
				T_OutputPin::ClockPin();
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			uint32_t AChannel = *(uint32_t *)_Data;
			if( AChannel >= C_NUM_INPUTS )
				AChannel = C_NUM_INPUTS - 1;

			InitialChannel() = AChannel;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif