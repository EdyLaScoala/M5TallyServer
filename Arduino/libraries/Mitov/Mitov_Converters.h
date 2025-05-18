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
		typename T_OutputPin,
		typename T_Scale,
		typename T_TYPE
	> class TypedToAnalog : 
		public T_OutputPin,
		public T_Scale
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Scale )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = ( *(T_TYPE*)_Data ) * Scale();
			T_OutputPin::SetPinValue( AFloatValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Base,
		typename T_BeginText,
		typename T_DirectCast,
		typename T_ElementPrefix,
		typename T_EndText,
		typename T_MaxSize,
		typename T_OutputPin,
		typename T_SeparatorText
	> class BinaryToText :
		public T_Base,
		public T_BeginText,
		public T_DirectCast,
		public T_ElementPrefix,
		public T_EndText,
		public T_MaxSize,
		public T_OutputPin,
		public T_SeparatorText
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Base )
		_V_PROP_( BeginText )
		_V_PROP_( DirectCast )
		_V_PROP_( ElementPrefix )
		_V_PROP_( EndText )
		_V_PROP_( SeparatorText )
		_V_PROP_( MaxSize )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;

			if( DirectCast() )
			{
				char *AText = new char[ ADataBlock.Size + 1 ];

				memcpy( AText, ADataBlock.Data, ADataBlock.Size );

				AText[ ADataBlock.Size ] = '\0';

				T_OutputPin::SetPinValue( AText, false );

				delete [] AText;
			}

			else
				T_OutputPin::SetPinValue( ADataBlock.ToString( MaxSize(), Base(), BeginText(), EndText(), SeparatorText(), ElementPrefix() ).c_str(), false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_Round,
		typename T_Scale
	> class AnalogToInteger :
		public T_Round,
		public T_Scale,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Round )
		_V_PROP_( Scale )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = *(float*)_Data * Scale().GetValue();
			if( Round() )
			{
				if( AFloatValue < 0 )
					AFloatValue -= 0.5f;

				else
					AFloatValue += 0.5f;
			}

			int32_t AIntValue = AFloatValue;
			T_OutputPin::SetPinValue( AIntValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Constrain,
		typename T_OutputPin,
		typename T_Round,
		typename T_Scale
	> class AnalogToUnsigned :
		public T_Constrain,
		public T_Round,
		public T_Scale,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Round )
		_V_PROP_( Constrain )
		_V_PROP_( Scale )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = *(float*)_Data * Scale();
			if( Round() )
				AFloatValue += 0.5f;

			long AIntValue = AFloatValue;
			if( Constrain() )
				AIntValue &= 0x7FFFFFFF;

			T_OutputPin::SetPinValue( AIntValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_MinWidth,
		typename T_OutputPin,
		typename T_Precision	
	> class AnalogToText :
		public T_MinWidth,
		public T_OutputPin,
		public T_Precision	
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( MinWidth )
		_V_PROP_( Precision )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = *(float*)_Data;
			char AText[ 16 ];
			dtostrf( AFloatValue, MinWidth(), Precision(), AText );
			T_OutputPin::SetPinValue( AText );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Base,
		typename T_OutputPin,
		typename T_TYPE
	> class TypedToText :
		public T_Base,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Base )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_TYPE AValue = *(T_TYPE *)_Data;
			char AText[ 16 ];
			ltoa( AValue, AText, Base() );
			T_OutputPin::SetPinValue( AText );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FalseValue,
		typename T_OutputPin,
//		typename T_TYPE,
		typename T_TrueValue
	> class DigitalToType :
		public T_FalseValue,
		public T_OutputPin,
//		public T_TYPE,
		public T_TrueValue
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( TrueValue )
		_V_PROP_( FalseValue )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( *(bool *)_Data )
				T_OutputPin::SetPinValue( TrueValue().GetValue() );

			else
				T_OutputPin::SetPinValue( FalseValue().GetValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_FalseValue,
		typename T_OutputPin,
//		typename T_TYPE,
		typename T_TrueValue
	> class DigitalToBinary :
		public T_FalseValue,
		public T_OutputPin,
//		public T_TYPE,
		public T_TrueValue
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( TrueValue )
		_V_PROP_( FalseValue )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( *(bool *)_Data )
				T_OutputPin::SetPinValue( TDataBlock( TrueValue().GetCount(), TrueValue().GetValue() ));

			else
				T_OutputPin::SetPinValue( TDataBlock( FalseValue().GetCount(), FalseValue().GetValue() ));

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class TextToAnalog : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char*)_Data;
			float AValue = strtod( AText, nullptr );
			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Base,
		typename T_OutputPin
	> class TextToInteger :
		public T_Base,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Base )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char*)_Data;
//			long AValue = atoi( AText );
			char *AEnd;
			long AValue = strtol( AText, &AEnd, Base() );
			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Base,
		typename T_OutputPin
	> class TextToUnsigned :
		public T_Base,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Base )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char*)_Data;
//			unsigned long AValue = atoi( AText );
			char *AEnd;
			unsigned long AValue = strtol( AText, &AEnd, Base() );
			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Constrain,
		typename T_OutputPin
	> class UnsignedToInteger :
		public T_Constrain,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Constrain )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			unsigned long AValue = *(unsigned long*)_Data;
			if( Constrain() )
				AValue &= 0x7FFFFFFF;

			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Constrain,
		typename T_OutputPin
	> class IntegerToUnsigned :
		public T_Constrain,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Constrain )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			long AValue = *(long*)_Data;
			if( Constrain() )
				AValue &= 0x7FFFFFFF;

			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_InitialValue,
		typename T_OutputPins_SetPinValue
	> class UnsignedToDigital :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void SetValue( uint32_t AValue, bool AUpdate )
		{
			for( long i = 0; i < C_NUM_INPUTS; ++i )
			{
				uint32_t ABit = uint32_t(1) << i;
				bool AOldBitValue = ( InitialValue() & ABit );
				bool ANewBitValue = ( AValue & ABit );
				if( AUpdate || AOldBitValue != ANewBitValue )
					T_OutputPins_SetPinValue::Call( i, ANewBitValue );

			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			unsigned long AValue = *(unsigned long *)_Data;
			if( InitialValue() == AValue )
				return;

			SetValue( AValue, false );
			InitialValue() = AValue;
		}

	public:
		inline void SystemInit()
		{
			SetValue( InitialValue(), true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Reverse
	> class CelsiusToFahrenheit :
		public T_Enabled,
		public T_OutputPin,
		public T_Reverse
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reverse )

	protected:
        inline float FilterValue( float AValue )
        {
			if( Reverse() )
				return ( AValue - 32.0 ) / ( 9.0/5.0 );

			else
				return AValue * ( 9.0/5.0 ) + 32.0;
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
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Reverse
	> class CelsiusToKelvin :
		public T_Enabled,
		public T_OutputPin,
		public T_Reverse
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reverse )

	protected:
        inline float FilterValue( float AValue )
        {
			if( Reverse() )
				return AValue - 273.15;

			else
				return AValue + 273.15;
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Reverse
	> class MetresToFeet :
		public T_Enabled,
		public T_OutputPin,
		public T_Reverse
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reverse )

	protected:
        inline float FilterValue( float AValue )
        {
			if( Reverse() )
				return AValue * 0.3048;

			else
				return AValue / 0.3048;
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_IsEnding,
		typename T_Enabled,
		typename T_EndOnNewLine,
		typename T_FIndex,
		int T_SIZE,
		typename T_OutputPin,
		typename T_Truncate,
		typename T_UpdateOnEachChar
	> class CharToText :
		public T_Enabled,
		public T_EndOnNewLine,
		public T_FIndex,
		public T_OutputPin,
		public T_Truncate,
		public T_UpdateOnEachChar
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( EndOnNewLine )
		_V_PROP_( Truncate )
		_V_PROP_( UpdateOnEachChar )

	public:
		char	FBuffer[ T_SIZE + 1 ];
		_V_PROP_( FIndex )

	protected:
		void SendBufferNoReset()
		{
			FBuffer[ FIndex() ] = '\0';
			T_OutputPin::SetPinValue( FBuffer, false );
		}

		void SendBuffer()
		{
			SendBufferNoReset();
			FIndex() = 0;
		}

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			char AValue = *(char *)_Data;

			if( EndOnNewLine() )
			{
				if( AValue == '\n' )
					return;

				if( AValue == '\r' )
				{
					SendBuffer();
					return;
				}
			}
			
			if( FIndex() >= T_SIZE )
			{
				if( Truncate() )
					return;

				SendBuffer();
			}

			FBuffer[ FIndex() ] = AValue;
			FIndex() = FIndex() + 1; // Bitfields reference is not possible

			bool AIsEnding = false;
			T_Elements_IsEnding::Call( AIsEnding );
			if( AIsEnding )
			{
			    SendBuffer();
                return;
			}

			if( UpdateOnEachChar() )
				SendBufferNoReset();

        }

		inline void ClearInputPin_o_Receive( void *_Data )
		{
			FIndex() = 0;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendBuffer();
		}
		
		void DeleteOneInputPin_o_Receive( void *_Data )
		{
		    if( ! FIndex().GetValue() )
		        return;
		        
		    FIndex() = FIndex() - 1; // Bitfields reference is not possible
		    
			if( UpdateOnEachChar() )
				SendBufferNoReset();
				
		}

	public:
		CharToText()
		{
			FIndex() = 0;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Include,
		typename T_Value
	> class TArduinoCharToTextEndingTextElement :
		public T_Enabled,
		public T_Include,
		public T_Value
	{
	public:
        _V_PROP_( Enabled )
		_V_PROP_( Include )
        _V_PROP_( Value )

	public:
		inline void IsEnding( bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled().GetValue() )
				return;

			uint8_t ALingth = Value().GetValue().length();
			uint8_t ABufferLength = C_OWNER.FIndex().GetValue();

			if( ABufferLength < ALingth )
				return;

			char *ABuffer = C_OWNER.FBuffer + ABufferLength - ALingth;
//			char *ATemplate = Value().GetValue().c_str();

			for( int i = 0; i < ALingth; ++i )
				if( Value().GetValue()[ i ] != ABuffer[ i ] )
					return;

			if( ! Include().GetValue() )
				C_OWNER.FIndex() = ABufferLength - ALingth;

//			Serial.println( "Test1" );
			AResult = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AddNewLine,
		typename T_AddReturn,
		typename T_Enabled,
		typename T_OutputPin
	> class TextToChar :
		public T_AddNewLine,
		public T_AddReturn,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( Enabled )
		_V_PROP_( AddReturn )
		_V_PROP_( AddNewLine )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			char *AValue = (char *)_Data;
			while( *AValue )
				T_OutputPin::SetPinValue( *AValue ++, false );

			if( AddReturn() )
				T_OutputPin::SetPinValue( '\r', false );

			if( AddNewLine() )
				T_OutputPin::SetPinValue( '\n', false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_BaseLinePressure,
		typename T_Enabled,
		typename T_OutputPin
	> class PressureToAltitude :
		public T_BaseLinePressure,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( BaseLinePressure )

	protected:
        inline float FilterValue( float AValue )
        {
			return ( 44330.0 * ( 1 - pow( AValue / BaseLinePressure(), 1 / 5.255 )));
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Altitude,
		typename T_Enabled,
		typename T_OutputPin
	> class AltitudePressureToSeaLevelPressure :
		public T_Altitude,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Altitude )

	protected:
        float FilterValue( float AValue )
        {
			return ( AValue / pow( 1 - ( Altitude() / 44330.0 ), 5.255 ));
        }

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }
	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS, 
		typename T_FModified,
		typename T_OutputPin
	> class AnalogToAnalogArray : 
		public T_FModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin ) 

	protected:
		_V_PROP_( FModified )
		float	FBufferData[ C_NUM_INPUTS ];

	protected:
		void CalculateSendOutput( bool AFromStart )
		{
			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
			T_OutputPin::SetPinValue( ABuffer );
		}

	public:
		void ClockInputPin_o_Receive( void * )
		{
			if( FModified() )
				CalculateSendOutput( false );
		}

		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FBufferData[ AIndex ] = *(float *)_Data;
			FModified() = true;
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( false );

		}

	public:
		AnalogToAnalogArray()
		{
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS, 
		typename T_FModified,
		typename T_OutputPin
	> class AnalogArrayToAnalogArray : // TODO!!!!
		public T_FModified,
		public T_OutputPin
	{
	};
//---------------------------------------------------------------------------
	class ReceivePin
	{
	public:
		virtual void Receive( void *_Data ) = 0;
	};
//---------------------------------------------------------------------------
	template <typename T> class ValueSinkPin : public ReceivePin
	{
	public:
		T Value = T( 0 );

	public:
		virtual void Receive( void *_Data ) override
		{
			Value = *(T*)_Data;
		}
	};
//---------------------------------------------------------------------------
	template <typename T> class SingleElementArrayPin : public ValueSinkPin<Mitov::TValueArray<T> >
	{
		typedef ValueSinkPin<Mitov::TValueArray<T> > inherited;

	public:
		virtual void Receive( void *_Data ) override
		{
/*
			Serial.print( "Pin: " ); Serial.println( (int)this );
			Serial.println( inherited::Value.Size );
			Serial.println( (int)inherited::Value.Data );
			delay( 200 );
*/
			T AValue = *(T*)_Data;
			*inherited::Value.Data = AValue;

//			OpenWire::SinkPin::Receive( _Data );
		}

	};
//---------------------------------------------------------------------------
	class FloatArraySinkPinsArray
	{
	protected:
		ValueSinkPin<Mitov::TValueArray<float> >	**FInputPins;

	public:
		ValueSinkPin<Mitov::TValueArray<float> >& operator[] ( int AIndex ) 
		{
          return *( FInputPins[ AIndex ] );
		}

	public:
		inline void SetPin( int AIndex, ValueSinkPin<Mitov::TValueArray<float> > *APin )
		{
			FInputPins[ AIndex ] = APin;
		}

	public:
		FloatArraySinkPinsArray( int ASize )
		{
			FInputPins = new ValueSinkPin<Mitov::TValueArray<float> >*[ ASize ];
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS, 
		typename T_FModified,
		typename T_OutputPin
	> class MixedAnalogToAnalogArray : 
		public T_FModified,
		public T_OutputPin
	{
	public:
		FloatArraySinkPinsArray		InputPins = FloatArraySinkPinsArray( C_NUM_INPUTS );

	public:
		_V_PIN_( OutputPin ) 

	protected:
		_V_PROP_( FModified )

	protected:
		void CalculateSendOutput( bool AFromStart )
		{
//			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
//			float *AData = 
//			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
			int ASize = 0;

			for( int i = 0; i < C_NUM_INPUTS; ++i )
				ASize += InputPins[ i ].Value.Size;

//			Serial.println( "Size" );
//			Serial.println( ASize );
//			delay( 1000 );

			float *AData = new float[ ASize ];
			float *APtr = AData;
			for( int i = 0; i < C_NUM_INPUTS; ++i )
			{
				int ABlockSize = InputPins[ i ].Value.Size;
				memcpy( APtr, InputPins[ i ].Value.Data, ABlockSize * sizeof( APtr[ 0 ] ));
				APtr += ABlockSize;
			}

			TArray<float> ABuffer( ASize, AData );
			T_OutputPin::SetPinValue( ABuffer );

			delete [] AData;
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			InputPins[ AIndex ].Receive( _Data );
			FModified() = true;
		}

		void ClockInputPin_o_Receive( void * )
		{
			if( FModified() )
				CalculateSendOutput( false );
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( false );

		}

	public:
		MixedAnalogToAnalogArray()
		{
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_INPUTS,
		typename T_OutputPins_SetPinValue
	> class AnalogArrayToAnalog
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int ASize = (( TArray<float> *)_Data )->Size;
			float *AData = (( TArray<float> *)_Data )->Data;

			for( int i = 0; i < C_NUM_INPUTS; ++i )
				T_OutputPins_SetPinValue::Call( i, *AData++ );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_Real,
		typename T_OutputPins_Imaginary
	> class ComplexToAnalog :
		public T_OutputPins_Real,
		public T_OutputPins_Imaginary
	{
	public:
		_V_PIN_( OutputPins_Real )
		_V_PIN_( OutputPins_Imaginary )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TComplex *AComplex = (Mitov::TComplex *)_Data;
			T_OutputPins_Real::SetPinValue( AComplex->Real );
			T_OutputPins_Imaginary::SetPinValue( AComplex->Imaginary );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FPopulated,
		typename T_OutputPin
	> class AnalogToComplex :
		public T_FPopulated,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		_V_PROP_( FPopulated )

	protected:
		Mitov::TComplex FValue;

	public:
		void InputPins_o_Receive( uint8_t AIndex, void *_Data )
		{
			if( AIndex )
				FValue.Imaginary = *(float *)_Data;

			else
				FValue.Real = *(float *)_Data;

			if( FPopulated().GetValue() == 0b11 )
				OutputPin().SetPinValue( FValue );

			else
				FPopulated() = FPopulated().GetValue() | ( 1 << AIndex );

		}

	public:
		inline AnalogToComplex()
		{
			FPopulated() = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_Imaginary_X,
		typename T_OutputPins_Imaginary_Y,
		typename T_OutputPins_Imaginary_Z,
		typename T_RealOutputPin
	> class QuaternionToAnalog :
		public T_OutputPins_Imaginary_X,
		public T_OutputPins_Imaginary_Y,
		public T_OutputPins_Imaginary_Z,
		public T_RealOutputPin
	{
	public:
		_V_PIN_( OutputPins_Imaginary_X )
		_V_PIN_( OutputPins_Imaginary_Y )
		_V_PIN_( OutputPins_Imaginary_Z )
		_V_PIN_( RealOutputPin )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TQuaternion *AQuaternion = (Mitov::TQuaternion *)_Data;
			T_OutputPins_Imaginary_X::SetPinValue( AQuaternion->Imaginary_X() );
			T_OutputPins_Imaginary_Y::SetPinValue( AQuaternion->Imaginary_Y() );
			T_OutputPins_Imaginary_Z::SetPinValue( AQuaternion->Imaginary_Z() );
			T_RealOutputPin::SetPinValue( AQuaternion->Real() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FPopulated,
		typename T_OutputPin
	> class AnalogToQuaternion :
		public T_FPopulated,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		_V_PROP_( FPopulated )

	protected:
		Mitov::TQuaternion FValue;

	public:
		void ImaginaryInputPins_o_Receive( uint8_t AIndex, void *_Data )
		{
			FValue.Values[ AIndex ] = *(float *)_Data;

			if( FPopulated().GetValue() == 0b1111 )
				OutputPin().SetPinValue( FValue );

			else
				FPopulated() = FPopulated().GetValue() | ( 1 << AIndex );

		}

		inline void RealInputPin_o_Receive( void *_Data )
		{
			ImaginaryInputPins_o_Receive( 3, _Data );
		}

	public:
		inline AnalogToQuaternion()
		{
			FPopulated() = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
        typename T_InitialValue,
        typename T_OutputPin
    > class Orientation3DTo2D :
        public T_InitialValue,
        public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialValue )

	public:
        void InputPin_o_Receive( void *_Data )
        {
            uint8_t AValue = *((uint8_t *) _Data );
            if( AValue >= TArduino3DOrientation::Front )
                return;

            T_OutputPin::SetPinValue( TArduinoGraphicsOrientation( AValue ));
        }

	public:
        inline void SystemStart()
        {
            T_OutputPin::SetPinValue( InitialValue().GetValue() );
        }

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif