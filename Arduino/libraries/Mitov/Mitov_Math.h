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
#define Min Min
#define Max Max
#define Abs Abs
//---------------------------------------------------------------------------
	#define MITOV_PI 3.14159265359
//---------------------------------------------------------------------------
    template <typename T> class AddValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return AInValue + AParamValue;
        }

	};
//---------------------------------------------------------------------------
	template <typename T> class SubtractValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return AInValue - AParamValue;
        }

	};
//---------------------------------------------------------------------------
	template <typename T> class SubtractFromValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return AParamValue - AInValue;
        }

	};
//---------------------------------------------------------------------------
    template <typename T> class MultiplyByValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return AInValue * AParamValue;
        }

	};
//---------------------------------------------------------------------------
    template <
		typename T_Enabled,
		typename T_OutputPin,
		typename T_TYPE,
		typename T_Value
	> class DivideByValue : 
		public T_Enabled,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
        _V_PROP_( Value )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            T_OutputPin::SetPinValue( ( *(T_TYPE *)_Data ) / Value() );
        }

	};
//---------------------------------------------------------------------------
	template <typename T> class DivideValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			if( AInValue == 0 )
				AInValue = 0.0000000001;

            return AParamValue / AInValue;
        }

	};
//---------------------------------------------------------------------------
	template <typename T> class Inverse
	{
	public:
		inline static T CalculateOutput( T AInValue )
        {
			return -AInValue;
        }

	};
//---------------------------------------------------------------------------
	template <typename T> class Abs
	{
	public:
		inline static T CalculateOutput( T AInValue )
        {
			return abs( AInValue );
        }

	};
//---------------------------------------------------------------------------
	class Log
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return log( AInValue );
        }

	};
//---------------------------------------------------------------------------
	class Square
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return AInValue * AInValue;
        }

	};
//---------------------------------------------------------------------------
	class SquareRoot
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return sqrt( AInValue );
        }

	};
//---------------------------------------------------------------------------
	class Log10
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return log10( AInValue );
        }

	};
//---------------------------------------------------------------------------
	class Log2
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return log( AInValue ) / 0.693147180559945309417;
        }

	};
//---------------------------------------------------------------------------
	template <typename T> class MinLimit
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return ( AInValue < AParamValue ) ? AParamValue : AInValue;
        }

	};
//---------------------------------------------------------------------------
	template <typename T> class MaxLimit
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return ( AInValue > AParamValue ) ? AParamValue : AInValue;
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin,
		typename T_TYPE
	> class Limit : 
		public T_Enabled,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			T_TYPE AInValue = *(T_TYPE *)_Data;

			T_TYPE AOutValue = ( AInValue > Value_Max() ) ? 
					Value_Max() : 
					( AInValue < Value_Min() ) ? Value_Min() : AInValue;


			T_OutputPin::SetPinValue( AOutValue );
        }
	};
//---------------------------------------------------------------------------
	enum CompareValueType { ctEqual, ctNotEqual, ctBigger, ctSmaller, ctBiggerOrEqual, ctSmallerOrEqual };
//---------------------------------------------------------------------------
	class CompareTypeString_Implementation_ctEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue.toUpperCase();
				AValue.toUpperCase();
			}

//			Serial.println( ACurrentValue );
//			Serial.println( AValue );
			return ( ACurrentValue == AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeString_Implementation_ctNotEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue.toUpperCase();
				AValue.toUpperCase();
			}

			return ( ACurrentValue != AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeString_Implementation_ctBigger
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue.toUpperCase();
				AValue.toUpperCase();
			}

			return ( ACurrentValue > AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeString_Implementation_ctSmaller
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue.toUpperCase();
				AValue.toUpperCase();
			}

			return ( ACurrentValue < AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeString_Implementation_ctBiggerOrEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> inline static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue.toUpperCase();
				AValue.toUpperCase();
			}

			return ( ACurrentValue >= AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeString_Implementation_ctSmallerOrEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue.toUpperCase();
				AValue.toUpperCase();
			}

			return ( ACurrentValue <= AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeChar_Implementation_ctEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AValue = toupper( AValue );
			}

			return ( ACurrentValue == AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeChar_Implementation_ctNotEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AValue = toupper( AValue );
			}

			return ( ACurrentValue != AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeChar_Implementation_ctBigger
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AValue = toupper( AValue );
			}

			return ( ACurrentValue > AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeChar_Implementation_ctSmaller
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AValue = toupper( AValue );
			}

			return ( ACurrentValue < AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeChar_Implementation_ctBiggerOrEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AValue = toupper( AValue );
			}

			return ( ACurrentValue >= AValue );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeChar_Implementation_ctSmallerOrEqual
	{
	public:
		template <typename T_INSTANCE, typename T_VALUE> static bool PerformCompare( T_INSTANCE *AInstance, T_VALUE ACurrentValue, T_VALUE AValue )
		{
			if( AInstance->IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AValue = toupper( AValue );
			}

			return ( ACurrentValue <= AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_CompareType,
		typename T_Enabled,
		typename T_FStarted,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class CompareValue :
		public T_Enabled,
		public T_FStarted,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )
        _V_PROP_( Enabled )
		_V_PROP_( OnlyChanged )
//		_V_PROP_( IgnoreCase )

	protected:
		_V_PROP_( FStarted )
//		_V_PROP_( FProcessedOnce )

	protected:
		inline bool PerformCompare( T ACurrentValue, T AValue ) // Needed for specialization
		{
			return T_CompareType::PerformCompare( this, ACurrentValue, AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//			FProcessedOnce() = true;
			if( ! Enabled() )
				return;

			T AValue = *(T*)_Data;

			bool ABoolValue = PerformCompare( AValue, Value().GetValue() );

//			Serial.println( ABoolValue );

			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		CompareValue( T AInitialValue )
		{
//			FOutputValue() = false;
			FStarted() = false;
//			FProcessedOnce() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_CompareType,
		typename T_Enabled,
		typename T_FStarted,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class CompareValue_Binding :
		public T_Enabled,
		public T_FStarted,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )
        _V_PROP_( Enabled )
		_V_PROP_( OnlyChanged )
//		_V_PROP_( IgnoreCase )

	protected:
		_V_PROP_( FStarted )
//		_V_PROP_( FProcessedOnce )

	protected:
		T	FCurrentValue;

    public:
		void SetValue( T AOldValue, T AValue )
		{
			if( OnlyChanged() )
				if( AOldValue == AValue )
					return;

			Value() = AValue;
			ProcessOutput();
		}

	protected:
		inline bool PerformCompare( T ACurrentValue, T AValue ) // Needed for specialization
		{
			return T_CompareType::PerformCompare( this, ACurrentValue, AValue );
		}

	protected:
		void ProcessOutput()
		{
			if( ! Enabled() )
				return;

			bool ABoolValue = PerformCompare( FCurrentValue, Value() );

//			Serial.println( ABoolValue );

			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FCurrentValue = *(T*)_Data;

			//			FProcessedOnce() = true;
			ProcessOutput();
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		CompareValue_Binding( T AInitialValue ) : 
			FCurrentValue( AInitialValue )
		{
//			FOutputValue() = false;
			FStarted() = false;
//			FProcessedOnce() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_CompareType,
		typename T_Enabled,
		typename T_FStarted,
		typename T_OnlyChanged,
		typename T_OutputPin,
        typename T_Value,
        typename T_Value_ApplyValues,
        typename T_Value_GetValue
	> class CompareBinaryValue :
		public T_CompareType,
		public T_Enabled,
		public T_FStarted,
		public T_OnlyChanged,
		public T_OutputPin,
        public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( OnlyChanged )
        _V_PROP_( Value )

	protected:
		_V_PROP_( FStarted )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			Mitov::TDataBlock AValue = *(Mitov::TDataBlock*)_Data;
            bool ABoolValue;
            if( T_Value::GetIsConstant() )
            {
                Mitov::TDataBlock ABuffer( T_Value::GetCount(), new uint8_t[ T_Value::GetCount() ] );
//                TValueArray<uint8_t> ABuffer( true, T_Value::GetCount() );
				T_Value::CopyData( ABuffer.Data );

                ABoolValue = T_CompareType::PerformCompare( this, AValue, ABuffer );
                delete [] ABuffer.Data;
            }

            else
                ABoolValue = T_CompareType::PerformCompare( this, AValue, Mitov::TDataBlock( T_Value::GetCount(), T_Value::GetArray() ));

//			bool ABoolValue = T_CompareType::PerformCompare( this, AValue, Mitov::TDataBlock( Value._BytesSize, Value._Bytes ));

			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
        }

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		CompareBinaryValue()
		{
			FStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_CompareType,
		typename T_Enabled,
		typename T_FStarted,
		typename T_IgnoreCase,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class CompareTextValue :
		public T_Enabled,
		public T_FStarted,
		public T_IgnoreCase,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )
        _V_PROP_( Enabled )
		_V_PROP_( OnlyChanged )
		_V_PROP_( IgnoreCase )

	protected:
		_V_PROP_( FStarted )

	protected:
		inline bool PerformCompare( T ACurrentValue, T AValue ) // Needed for specialization
		{
			return T_CompareType::PerformCompare( this, ACurrentValue, AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			T AValue = ::Mitov::template ExtractValue<T>( _Data );

			bool ABoolValue = PerformCompare( AValue, Value() );

			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		CompareTextValue( T AInitialValue )
		{
			FStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_CompareType,
		typename T_Enabled,
		typename T_FStarted,
		typename T_IgnoreCase,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class CompareTextValue_Binding :
		public T_Enabled,
		public T_FStarted,
		public T_IgnoreCase,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )
        _V_PROP_( Enabled )
		_V_PROP_( OnlyChanged )
		_V_PROP_( IgnoreCase )

	protected:
		_V_PROP_( FStarted )
//		_V_PROP_( FProcessedOnce )

	protected:
		T	FCurrentValue;

    public:
		void SetValue( T AOldValue, T AValue )
		{
			if( OnlyChanged() )
				if( AOldValue == AValue )
					return;

			Value() = AValue;
			ProcessOutput();
		}

	protected:
		inline bool PerformCompare( T ACurrentValue, T AValue ) // Needed for specialization
		{
			return T_CompareType::PerformCompare( this, ACurrentValue, AValue );
		}

	protected:
		void ProcessOutput()
		{
			if( ! Enabled() )
				return;

			bool ABoolValue = PerformCompare( FCurrentValue, Value() );

//			Serial.println( ABoolValue );

			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FCurrentValue = ::Mitov::template ExtractValue<T>( _Data );

			//			FProcessedOnce() = true;
			ProcessOutput();
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		inline CompareTextValue_Binding( T AInitialValue ) : 
			FCurrentValue( AInitialValue )
		{
//			FOutputValue() = false;
			FStarted() = false;
//			FProcessedOnce() = false;
		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T_OUTPUT_IMPLEMENTATION, typename T_IMPLEMENTATION> bool CompareValue<char, T_OUTPUT_IMPLEMENTATION, T_IMPLEMENTATION>::PerformCompare( char ACurrentValue, char AValue )
	{
		if( IgnoreCase )
		{
			ACurrentValue = toupper( ACurrentValue );
			AValue = toupper( AValue );
		}

		switch( CompareType )
		{
			default:
			case ctEqual:
				return ( ACurrentValue == AValue );

			case ctNotEqual:
				return ( ACurrentValue != AValue );

			case ctBigger:
				return ( ACurrentValue > AValue );

			case ctSmaller:
				return ( ACurrentValue < AValue );

			case ctBiggerOrEqual:
				return ( ACurrentValue >= AValue );

			case ctSmallerOrEqual:
				return ( ACurrentValue <= AValue );

		}
	}
//---------------------------------------------------------------------------
	template<> bool CompareValue<Mitov::String, typename T_OUTPUT_IMPLEMENTATION, typename T_IMPLEMENTATION>::PerformCompare( Mitov::String ACurrentValue, Mitov::String AValue )
	{
		if( IgnoreCase )
		{
			ACurrentValue.toUpperCase();
			AValue.toUpperCase();
		}

		switch( CompareType )
		{
			default:
			case ctEqual:
				return ( ACurrentValue == AValue );

			case ctNotEqual:
				return ( ACurrentValue != AValue );

			case ctBigger:
				return ( ACurrentValue > AValue );

			case ctSmaller:
				return ( ACurrentValue < AValue );

			case ctBiggerOrEqual:
				return ( ACurrentValue >= AValue );

			case ctSmallerOrEqual:
				return ( ACurrentValue <= AValue );

		}
	}
//---------------------------------------------------------------------------
	template<> void CompareValue<Mitov::String>::InputPin_o_Receive( void *_Data )
	{
		Mitov::String AValue = Mitov::String( (char*)_Data );

		if( OnlyChanged )
			if( FProcessedOnce )
				if( AValue == FCurrentValue )
					return;

		FCurrentValue = AValue;
		FProcessedOnce = true;

		ProcessOutput();
	}
*/
//---------------------------------------------------------------------------
	class CompareTypeDateTime_Implementation_ctEqual
	{
	public:
		template <typename T_INSTANCE> inline static bool PerformCompare( T_INSTANCE *AInstance, Mitov::TDateTime ACurrentValue, Mitov::TDateTime AValue )
		{
			return ( ACurrentValue.IsEqual( AValue, AInstance->Items() ) );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeDateTime_Implementation_ctNotEqual
	{
	public:
		template <typename T_INSTANCE> inline static bool PerformCompare( T_INSTANCE *AInstance, Mitov::TDateTime ACurrentValue, Mitov::TDateTime AValue )
		{
			return ( ! ACurrentValue.IsEqual( AValue, AInstance->Items() ) );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeDateTime_Implementation_ctBigger
	{
	public:
		template <typename T_INSTANCE> inline static bool PerformCompare( T_INSTANCE *AInstance, Mitov::TDateTime ACurrentValue, Mitov::TDateTime AValue )
		{
			return ( ACurrentValue.IsBigger( AValue, AInstance->Items() ) );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeDateTime_Implementation_ctSmaller
	{
	public:
		template <typename T_INSTANCE> inline static bool PerformCompare( T_INSTANCE *AInstance, Mitov::TDateTime ACurrentValue, Mitov::TDateTime AValue )
		{
			return ( AValue.IsBigger( ACurrentValue, AInstance->Items ) );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeDateTime_Implementation_ctBiggerOrEqual
	{
	public:
		template <typename T_INSTANCE> inline static bool PerformCompare( T_INSTANCE *AInstance, Mitov::TDateTime ACurrentValue, Mitov::TDateTime AValue )
		{
			return ( ACurrentValue.IsBiggerOrEqual( AValue, AInstance->Items() ) );
		}
	};
//---------------------------------------------------------------------------
	class CompareTypeDateTime_Implementation_ctSmallerOrEqual
	{
	public:
		template <typename T_INSTANCE> inline static bool PerformCompare( T_INSTANCE *AInstance, Mitov::TDateTime ACurrentValue, Mitov::TDateTime AValue )
		{
			return ( AValue.IsBiggerOrEqual( ACurrentValue, AInstance->Items() ) );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_CompareType,
		typename T_Enabled,
		typename T_FStarted,
		typename T_Items,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T_Value
	> class CompareDateTimeValue :
		public T_Enabled,
		public T_FStarted,
		public T_Items,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )
        _V_PROP_( Enabled )
		_V_PROP_( OnlyChanged )
		_V_PROP_( Items )

	protected:
		_V_PROP_( FStarted )

//	protected:
//		Mitov::TDateTime	FCurrentValue;

	protected:
//		bool	FProcessedOnce : 1;

    public:
		void SetValue( Mitov::TDateTime AOldValue, Mitov::TDateTime AValue )
		{
			if( OnlyChanged() )
				if( AOldValue == AValue )
					return;

			Value() = AValue;
			ProcessOutput();
		}

	protected:
		bool PerformCompare( Mitov::TDateTime ACurrentValue, Mitov::TDateTime AValue )
		{
			return T_CompareType::PerformCompare( this, ACurrentValue, AValue );
/*
			switch( CompareType )
			{
			default:
			case ctEqual:
				return ( ACurrentValue.IsEqual( AValue, Items ) );

			case ctNotEqual:
				return ( ! ACurrentValue.IsEqual( AValue, Items ) );

			case ctBigger:
				return ( ACurrentValue.IsBigger( AValue, Items ) );

			case ctSmaller:
				return ( AValue.IsBigger( ACurrentValue, Items ) );

			case ctBiggerOrEqual:
				return ( ACurrentValue.IsBiggerOrEqual( AValue, Items ) );

			case ctSmallerOrEqual:
				return ( AValue.IsBiggerOrEqual( ACurrentValue, Items ) );

			}
*/
		}

	protected:
		void ProcessOutput( Mitov::TDateTime &AValue )
		{
			if( ! Enabled() )
				return;

//			Serial.print( FCurrentValue ); Serial.print( " ? " ); Serial.println( Value );

			bool ABoolValue = PerformCompare( AValue, Value() );

/*
			if( ! OnlyChanged )
				if( FOutputValue )
				{
					FOutputValue = false;
					T_OUTPUT_IMPLEMENTATION::SetPinValueLow();
				}
*/
			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			ProcessOutput( *(Mitov::TDateTime*)_Data );
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		inline CompareDateTimeValue()
		{
			FStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FStarted,
		typename T_IncludeLimits,
		typename T_IsOutsideRange,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T_Range,
		typename T_TYPE
	> class CompareRange : 
		public T_Enabled,
		public T_FStarted,
		public T_IncludeLimits,
		public T_IsOutsideRange,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Range
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Range )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IsOutsideRange )
		_V_PROP_( IncludeLimits )
		_V_PROP_( OnlyChanged )

	protected:
		_V_PROP_( FStarted )

    public:
		inline void ProcessOutput( T_TYPE AValue )
		{
			bool ABoolValue = PerformCompare( AValue );

			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	protected:
		inline bool PerformCompare( T_TYPE ACurrentValue )
		{
			if( IsOutsideRange() )
			{
				if( IncludeLimits() )
				{
					if( ACurrentValue >= Range().Value_Max() )
						return true;

					return ( ACurrentValue <= Range().Value_Min() );
				}

				if( ACurrentValue > Range().Value_Max() )
					return true;

				return ( ACurrentValue < Range().Value_Min() );
			}

			if( IncludeLimits() )
			{
				if( ACurrentValue > Range().Value_Max() )
					return false;

				return ! ( ACurrentValue < Range().Value_Min() );
			}

			if( ACurrentValue >= Range().Value_Max() )
				return false;

			return ! ( ACurrentValue <= Range().Value_Min() );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			ProcessOutput( *(T_TYPE *)_Data );
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		inline CompareRange()
		{
			FStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FStarted,
		typename T_IgnoreCase,
		typename T_IncludeLimits,
		typename T_IsOutsideRange,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T_Range
	> class CompareRangeChar : 
		public T_Enabled,
		public T_FStarted,
		public T_IgnoreCase,
		public T_IncludeLimits,
		public T_IsOutsideRange,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Range
	{
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Range )

	protected:
//		char	FCurrentValue;

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IsOutsideRange )
		_V_PROP_( IncludeLimits )
		_V_PROP_( OnlyChanged )
		_V_PROP_( IgnoreCase )

	protected:
		_V_PROP_( FStarted )

    public:
		void ProcessOutput( char AValue )
		{
//			if( ! Enabled() )
//				return;

			bool ABoolValue = PerformCompare( AValue );

/*
			if( ! OnlyChanged )
				if( FOutputValue )
				{
					FOutputValue = false;
					OutputPin.SendValue( false );
				}
*/
			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	protected:
		bool PerformCompare( char ACurrentValue )
		{
			char AMinValue = Range().Value_Min();
			char AMaxValue = Range().Value_Max();
			if( IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AMinValue = toupper( AMinValue );
				AMaxValue = toupper( AMaxValue );
			}

			if( IsOutsideRange() )
			{
				if( IncludeLimits() )
				{
					if( ACurrentValue >= AMaxValue )
						return true;

					return ( ACurrentValue <= AMinValue );
				}

				if( ACurrentValue > AMaxValue )
					return true;

				return ( ACurrentValue < AMinValue );
			}

			if( IncludeLimits() )
			{
				if( ACurrentValue > AMaxValue )
					return false;

				return ! ( ACurrentValue < AMinValue );
			}

			if( ACurrentValue >= AMaxValue )
				return false;

			return ! ( ACurrentValue <= AMinValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			ProcessOutput( *(char*)_Data );
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		inline CompareRangeChar()
		{
			FStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FStarted,
		typename T_IncludeLimits,
		typename T_IsOutsideRange,
		typename T_Items,
		typename T_OnlyChanged,
		typename T_OutputPin,
		typename T_Range
	> class CompareDateTimeRange :
		public T_Enabled,
		public T_FStarted,
		public T_IncludeLimits,
		public T_IsOutsideRange,
		public T_Items,
		public T_OnlyChanged,
		public T_OutputPin,
		public T_Range
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Range )
		_V_PROP_( Items )

//	protected:
//		Mitov::TDateTime	FCurrentValue;

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IsOutsideRange )
		_V_PROP_( IncludeLimits )
		_V_PROP_( OnlyChanged )

	protected:
		_V_PROP_( FStarted )
//		bool	FOutputValue : 1;
//		bool	FProcessedOnce : 1;

    public:
		inline void ProcessOutput( Mitov::TDateTime &AValue )
		{
//			if( ! Enabled() )
//				return;

			bool ABoolValue = PerformCompare( AValue );
/*
			if( ! OnlyChanged )
				if( FOutputValue )
				{
					FOutputValue = false;
					OutputPin.SendValue( false );
				}
*/
			T_OutputPin::SetPinValue( ABoolValue, FStarted().GetValue() && OnlyChanged().GetValue() );
			FStarted() = true;
		}

	protected:
		inline bool PerformCompare( Mitov::TDateTime &ACurrentValue )
		{
			if( IsOutsideRange() )
			{
				if( IncludeLimits() )
				{
					if( ACurrentValue.IsBiggerOrEqual( Range().Value_Max().GetValue(), Items() ) )
						return true;

					return (  Range().Value_Min().GetValue().IsBigger( ACurrentValue, Items() ) );
				}

				if( ACurrentValue.IsBigger( Range().Value_Max().GetValue(), Items() ) )
					return true;

				return ( Range().Value_Min().GetValue().IsBiggerOrEqual( ACurrentValue, Items() )  );
			}

			if( IncludeLimits() )
			{
				if( ACurrentValue.IsBigger( Range().Value_Max().GetValue(), Items() ) )
					return false;

				return ! ( Range().Value_Min().GetValue().IsBiggerOrEqual( ACurrentValue, Items() ) );
			}

			if( ACurrentValue.IsBiggerOrEqual( Range().Value_Max().GetValue(), Items() ) )
				return false;

			return ! ( Range().Value_Min().GetValue().IsBigger( ACurrentValue, Items() ) );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			ProcessOutput( *(Mitov::TDateTime *)_Data );
		}

	public:
		inline void SystemStart()
		{
			if( ! FStarted() )
				T_OutputPin::SetPinValueLow();

		}

	public:
		inline CompareDateTimeRange()
		{
			FStarted() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_BASE,
		typename T_Enabled,
		typename T_FPopulated,
		typename T_OutputPin,
		typename T_TYPE
	> class MinMaxValue :
		public T_Enabled,
		public T_FPopulated,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FPopulated )

	protected:
		T_TYPE	FMinValue;

	protected:
        inline void NewValue( T_TYPE &AValue )
        {
            // Send only modified values to allow clocking!
            FMinValue = AValue;
            T_OutputPin::SetPinValue( FMinValue );
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
			}

			T_TYPE AValue = *(T_TYPE *)_Data;
			if( FPopulated().GetValue() )
			{
				if( T_BASE::PerformCompare( this, AValue, FMinValue ))
					NewValue( AValue );

			}
			else
			{
				FPopulated() = true;
				NewValue( AValue );
			}

		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			FPopulated() = false;
		}

	public:
		inline MinMaxValue()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Period
	> class AveragePeriod :
		public T_Enabled,
		public T_OutputPin,
		public T_Period
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	protected:
		float FSum = 0.0f;
		unsigned long	FCount = 0;
		unsigned long	FLastTime = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			FSum += *(float *)_Data;
			++ FCount;
		}

	public:
		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ACurrentMicros - FLastTime < Period() )
				return;

			if( ! FCount )
				return;

			float AValue = FSum / FCount;

			FSum = 0.0f;
			FCount = 0;
			FLastTime = ACurrentMicros;

			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION_CLASS, 
		int C_NUM_INPUTS, 
		typename T_FModified,
		typename T_OutputPin,
		typename T
	> class MultiInputMathOperation : 
		public T_FModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		_V_PROP_( FModified )

		T		FValues[ C_NUM_INPUTS ] = { 0 };

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FValues[ AIndex ] = *(T *)_Data;
			FModified() = true;
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( false );
		}

		inline void SystemLoopEnd()
		{
			CalculateSendOutput( true );
		}

	protected:
		void CalculateSendOutput( bool AChangeOnly )
		{
//			Serial.println( "CalculateSendOutput+++" );
//			Serial.println( AValue );
			if( FModified() || ( ! AChangeOnly ) )
			{
				T AValue = T_IMPLEMENTATION_CLASS::CalculateOutput( FValues, C_NUM_INPUTS );
				T_OutputPin::SetPinValue( AValue, AChangeOnly ); // Use Send Value for String Compatibility!
			}

//			FPopulated = true;
			FModified() = false;


/*
			T_OUT AValue = CalculateOutput();
//			Serial.println( AValue );
			if( ! AFromStart )
				if( FLastOutput == AValue )
					return;

			OutputPin.SendValue( AValue ); // Use Send Value for String Compatibility!
			FLastOutput = AValue;

			FModified = false;
*/
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T AValue = T_IMPLEMENTATION_CLASS::CalculateOutput( FValues, C_NUM_INPUTS );
//			if( FLastOutput == AValue )
//				return;

			T_OutputPin::SetPinValue( AValue ); // Use Send Value for String Compatibility!
			FModified() = false;
		}

	public:
		inline MultiInputMathOperation()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T> class Add
	{
	public:
		static T CalculateOutput( T *AInputValues, uint16_t ANumInputs )
		{
			T AValue = 0;
			for( int i = 0; i < ANumInputs; ++i )
				AValue += AInputValues[ i ];

			return AValue;
		}

	};
//---------------------------------------------------------------------------
	template <typename T> class Multiply
	{
	public:
		static T CalculateOutput( T *AInputValues, uint16_t ANumInputs )
		{
			T AValue = 1;
			for( int i = 0; i < ANumInputs; ++i )
				AValue *= AInputValues[ i ];

			return AValue;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Value_Max,
		typename T_Value_Min
	> class ValueRange :
		public T_Value_Max,
		public T_Value_Min
	{
	public:
		_V_PROP_( Value_Max )
		_V_PROP_( Value_Min )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InputRange,
		typename T_OutputPin,
		typename T_OutputRange
	> class MapRange : 
		public T_Enabled,
		public T_InputRange,
		public T_OutputPin,
		public T_OutputRange
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PIN_( Enabled )
		_V_PIN_( InputRange )
		_V_PIN_( OutputRange )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AValue = *(float*)_Data;

			float AOutValue = Func::MapRange( AValue, InputRange().Value_Min().GetValue(), InputRange().Value_Max().GetValue(), OutputRange().Value_Min().GetValue(), OutputRange().Value_Max().GetValue() );

			T_OutputPin::SetPinValue( AOutValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Span,
		typename T_Value
	> class AnalogDeadZone : 
		public T_Enabled,
		public T_OutputPin,
		public T_Span,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )
		_V_PROP_( Span )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AValue = *(float*)_Data;

			float AFalfSpan = Span() / 2;

			if( AValue > Value() + AFalfSpan )
				AValue -= AFalfSpan;

			else if( AValue < Value - AFalfSpan )
				AValue += AFalfSpan;

			else
				AValue = Value();

			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InputRange,
		typename T_OutputPin,
		typename T_OutputRange,
		typename T_Span,
		typename T_Value
	> class AnalogDeadZoneScaled :
		public T_Enabled,
		public T_InputRange,
		public T_OutputPin,
		public T_OutputRange,
		public T_Span,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PIN_( Enabled )
		_V_PIN_( InputRange )
		_V_PIN_( OutputRange )

		_V_PIN_( Value )
		_V_PIN_( Span )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AValue = *(float*)_Data;

			float AFalfSpan = Span() / 2;

			if( AValue > Value() + AFalfSpan )
				AValue -= AFalfSpan;

			else if( AValue < Value() - AFalfSpan )
				AValue += AFalfSpan;

			else
				AValue = Value();

			float AOutValue = Func::MapRange( AValue, InputRange().Value_Min().GetValue() + AFalfSpan, InputRange().Value_Max().GetValue() - AFalfSpan, OutputRange().Value_Min().GetValue(), OutputRange().Value_Max().GetValue() );
			T_OutputPin::SetPinValue( AOutValue );
		}

	public:
		inline void SystemStart()
		{
			float AFalfSpan = Span() / 2;

			float AOutValue = Func::MapRange( Value().GetValue(), InputRange().Value_Min().GetValue() + AFalfSpan, InputRange().Value_Max().GetValue() - AFalfSpan, OutputRange().Value_Min().GetValue(), OutputRange().Value_Max().GetValue() );
			T_OutputPin::SetPinValue( AOutValue );
		}

	};
//---------------------------------------------------------------------------
    template <typename T> class RaiseToPower
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return pow( AInValue, AParamValue );
        }

	};
//---------------------------------------------------------------------------
	class Sine
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return sin( AInValue );
        }

	};
//---------------------------------------------------------------------------
	class Cosine
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return cos( AInValue );
        }

	};
//---------------------------------------------------------------------------
	class RadToDegrees
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return AInValue * 180 / MITOV_PI;
        }

	};
//---------------------------------------------------------------------------
	class DegreesToRad
	{
	public:
		inline static float CalculateOutput( float AInValue )
        {
			return AInValue * MITOV_PI / 180;
        }

	};
//---------------------------------------------------------------------------
    template <typename T> class AndValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return AInValue & AParamValue;
        }

	};
//---------------------------------------------------------------------------
    template <typename T> class OrValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return AInValue | AParamValue;
        }

	};
//---------------------------------------------------------------------------
    template <typename T> class XorValue
	{
	public:
		inline static T CalculateOutput( T AInValue, T AParamValue )
        {
			return AInValue ^ AParamValue;
        }

	};
//---------------------------------------------------------------------------
    template <typename T> class NotValue
	{
	public:
		inline static T CalculateOutput( T AInValue )
        {
			return ~ AInValue;
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FHasTime,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_TYPE
	> class Accumulate : 
		public T_Enabled,
		public T_FHasTime,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( FHasTime )
		unsigned long	FLastTime = 0;

	public:
		inline void UpdateInitialValue()
		{
			T_OutputPin::SetPinValue( InitialValue() );
		}

	protected:
        T_TYPE FilterValue( T_TYPE AValue )
        {
			unsigned long ATime = micros();
			if( ! FHasTime() )			
			{
				FHasTime() = true;
				return InitialValue();
			}

			float dT = ( ATime - FLastTime ) / 1000000.0f;
			InitialValue().GetReference() += AValue * dT;
            return InitialValue();
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            T_TYPE AOutValue = FilterValue( *(T_TYPE *)_Data );

			T_OutputPin::SetPinValue( AOutValue );
        }

	public:
		inline Accumulate()
		{
			FHasTime() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FHasTime,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Units
	> class AccumulateAngle : 
		public T_Enabled,
		public T_FHasTime,
		public T_InitialValue,
		public T_OutputPin,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )

        _V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Units )

	protected:
		_V_PROP_( FHasTime )

	protected:
		unsigned long	FLastTime = 0;

	public:
		void SetInitialValue( float AValue )
		{
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			T_OutputPin::SetPinValue( InitialValue() );
		}

	protected:
        float FilterValue( float AValue )
        {
			if( ! FHasTime() )			
			{
				FHasTime() = true;
				return InitialValue();
			}

			unsigned long ATime = micros();
			float dT = ( ATime - FLastTime ) / 1000000.0f;
			InitialValue().GetReference() += AValue * dT;
			switch( Units )
			{
				case auDegree:
					InitialValue() = posmod( InitialValue(), 360.0f );
					break;

				case auRadians:
					InitialValue() = posmod( InitialValue(), 2 * PI );
					break;

				case auNormalized:
					InitialValue() = posmod( InitialValue(), 1.0f );
					break;

			}

            return InitialValue();
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            float AOutValue = FilterValue( *(float*)_Data );

			T_OutputPin::SetPinValue( AOutValue );
        }

	public:
		inline AccumulateAngle()
		{
			FHasTime() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FModified,
		typename T_FPopulated,
		typename T_OutputPin
	> class Analog2DChangeDistance :
		public T_FModified,
		public T_FPopulated,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		_V_PROP_( FPopulated )
		_V_PROP_( FModified )

	protected:
		float FOldValues[ 2 ];
		float FValues[ 2 ];

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FPopulated() = FPopulated().GetValue() | ( 1 << AIndex );
			FValues[ AIndex ] = *(float *)_Data;
//			Serial.println( FPopulated().GetValue() );
//			Serial.println( AIndex );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( 0, false );
		}

		inline void SystemLoopBegin()
		{
			if( FPopulated() != 0b11 )
				return;


			if( ! FModified().GetValue() )
			{
				FModified() = true;
				return;
			}

			float X = FValues[ 0 ] - FOldValues[ 0 ];
			float Y = FValues[ 1 ] - FOldValues[ 1 ];
			float AResult = sqrtf( X * X + Y * Y );
			T_OutputPin::SetPinValue( AResult, false );

			FOldValues[ 0 ] = FValues[ 0 ];
			FOldValues[ 1 ] = FValues[ 1 ];
			FPopulated() = 0;

		}

	public:
		inline Analog2DChangeDistance()
		{
			FPopulated() = 0;
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
#undef Abs
#undef Min
#undef Max
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif