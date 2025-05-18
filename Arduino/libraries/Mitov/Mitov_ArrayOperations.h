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
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T,
        typename T_ValueElements,
        typename T_ValueElements_ApplyValues,
        typename T_ValueElements_GetValue
	> class ArduinoSetValueArrayElement :
        public T_ValueElements
	{
	public:
        _V_PROP_( ValueElements )

	public:
		inline void SetIndexedValue( uint32_t AIndex )
        {
//            C_OWNER.SetIndexedValue( 0 );
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            if( T_ValueElements_ApplyValues::Count() )
            {
                typename T_ValueElements::T_ARRAY *ABuffer = T_ValueElements::GetMofifyDataArray();
                T_ValueElements_ApplyValues::Call( ABuffer->Data );
                C_OWNER.ExternalSetValue( *ABuffer );
                delete ABuffer;
            }

            else
            {
                typename T_ValueElements::T_ARRAY *ABuffer = T_ValueElements::GetDataArray();
                C_OWNER.ExternalSetValue( *ABuffer );
                delete ABuffer;
            }
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T,
        typename T_ValueElements,
        typename T_ValueElements_ApplyValues,
        typename T_ValueElements_GetValue
	> class ValueArraySource :
		public T_OutputPin,
        public T_ValueElements
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( ValueElements )

	protected:
		void SetValue()
		{
//			if( ! T_ValueElements::GetCount() )
//				return;

            T_ValueElements::template PinSend<T_OutputPin, T_ValueElements_ApplyValues>( OutputPin() );
		}

	public:
		inline void UpdateArrayValue() {}

	public:
		inline void SetIndexedValue( uint32_t AIndex )
        {
            SetValue();
//            T_ValueElements::SetValue( AIndex, AValue );
        }

		void ExternalSetValue( const TArray<T> &ABuffer )
		{
            if( ABuffer.Size == T_ValueElements::GetCount() )
                if( T_ValueElements::Compare( ABuffer.Data ) == 0 )
    				return;

			T_OutputPin::SetPinValue( ABuffer );
		}

	public:
		inline void SystemStart()
		{
			SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SetValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_FInputValue,
		typename T,
        typename T_ValueElements,
        typename T_ValueElements_ApplyValues,
        typename T_ValueElements_GetValue
	> class ArduinoStateSetArrayValueElement :
		public T_FInputValue,
        public T_ValueElements
	{
	protected:
		_V_PROP_( FInputValue )

    public:
        _V_PROP_( ValueElements )

//	public:
//		T *_Values;
//		uint32_t _Count;

		inline void SetIndexedValue( uint32_t AIndex )
        {
//            C_OWNER.SetIndexedValue( 0 );
			if( FInputValue() )
                C_OWNER.ResetValue();

        }

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

			if( FInputValue() )
			{
//					Serial.println( "TryActive" );
//					Serial.println( Value );
                if( T_ValueElements_ApplyValues::Count() )
                {
                    typename T_ValueElements::T_ARRAY *ABuffer = T_ValueElements::GetMofifyDataArray();
                    T_ValueElements_ApplyValues::Call( ABuffer->Data );
                    C_OWNER.ExternalSetValue( *ABuffer );
                    delete ABuffer;
                }

                else
                {
                    typename T_ValueElements::T_ARRAY *ABuffer = T_ValueElements::GetDataArray();
                    C_OWNER.ExternalSetValue( *ABuffer );
                    delete ABuffer;
//		    		C_OWNER.ExternalSetValue( T_ValueElements::GetValue(), T_ValueElements::GetCount() );
                }

    	    	AResult = true;
			}

		}

		inline void UpdateArrayValue() {}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue() == *(bool *)_Data )
				return;

//			Serial.println( FInputValue );

			FInputValue() = *(bool *)_Data;
//			if( FInputValue )
//				FOwner.ExternalSetValue( nullptr, Value );

//			else
			C_OWNER.ResetValue();
		}

	public:
		ArduinoStateSetArrayValueElement()
		{
			FInputValue() = false;
//			C_OWNER.RegisterElement( this );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T,
        typename T_ValueElements,
        typename T_ValueElements_ApplyValues,
        typename T_ValueElements_GetValue
	> class ValueArraySourceState : // public ValueArraySource<T, C_ELEMENTS_COUNT, T_OUTPUT_PIN>
		public T_OutputPin,
        public T_ValueElements
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( ValueElements )

	protected:
		void SetValue()
		{
			if( ! T_ValueElements::GetCount() )
				return;

//			TArray<T> ABuffer( T_ValueElements::GetCount(), T_ValueElements::GetValue() );
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	protected:
		TValueArray<T> FCurrentValue;

	public:
		inline void UpdateArrayValue() {}

	public:
		void ExternalSetValue( const TArray<T> &ABuffer )
		{
			if( FCurrentValue == ABuffer )
				return;

			FCurrentValue = ABuffer;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

            FCurrentValue.Resize( T_ValueElements::GetCount() );
            T_ValueElements::CopyData( FCurrentValue.Data );

//			FCurrentValue.Assign( T_ValueElements::GetValue(), T_ValueElements::GetCount() );
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
            FCurrentValue.Resize( T_ValueElements::GetCount() );
            T_ValueElements::CopyData( FCurrentValue.Data );
//			FCurrentValue.Assign( T_ValueElements::GetValue(), T_ValueElements::GetCount() );
			if( T_ValueElements::GetCount() )
    			T_OutputPin::SetPinValue( FCurrentValue );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}
	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin
	> class ComplexValueArraySource :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		float *_Values;
		uint32_t _Count;

	public:
		void ExternalSetValue( float *AValues, uint32_t ACount )
		{
			TArray<Mitov::TComplex> AValue( ACount, (Mitov::TComplex *)AValues );
			TArray<Mitov::TComplex> Value( _Count, (Mitov::TComplex *)_Values );
			if( Value == AValue )
				return;

			_Values = AValues;
			_Count = ACount;
			T_OutputPin::SetPinValue( AValue );
		}

	protected:
		void SetValue()
		{
			if( ! _Count )
				return;

			TArray<Mitov::TComplex> ABuffer( _Count, (Mitov::TComplex *)_Values );
			T_OutputPin::SetPinValue( ABuffer );
//			OutputPin.Notify( &ABuffer );
		}

	public:
		inline void SystemStart()
		{
			SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SetValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_OutputPin
	> class ValueComplexArrayStateSource : // public ComplexValueArraySource<C_ELEMENTS_COUNT, T_OUTPUT_PIN>
		public T_OutputPin
	{
//		typedef Mitov::ComplexValueArraySource<C_ELEMENTS_COUNT, T_OUTPUT_PIN> inherited;

	public:
		_V_PIN_( OutputPin )

	public:
		float *_Values;
		uint32_t _Count;

	protected:
		TValueArray<Mitov::TComplex> FCurrentValue;

	public:
		void SetValue()
		{
			if( ! _Count )
				return;

			TArray<Mitov::TComplex> ABuffer( _Count, (Mitov::TComplex *)_Values );
			T_OutputPin::SetPinValue( ABuffer );
//			OutputPin.Notify( &ABuffer );
		}

		void ExternalSetValue( float *AValues, uint32_t ACount )
		{
			TArray<Mitov::TComplex> AValue( ACount, (Mitov::TComplex *)AValues );
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

			FCurrentValue.Assign( (Mitov::TComplex *)_Values, _Count );
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue.Assign( (Mitov::TComplex *)_Values, _Count );
			SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Count,
		typename T_Enabled,
		typename T_Index,
		typename T_OutputPin,
		typename T
	> class ArrayDeleteItems : 
		public T_Count,
		public T_Enabled,
		public T_Index,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin ) 

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Index )
		_V_PROP_( Count )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			TArray<T> *AData = (TArray<T>*)_Data;
			if( Count() == 0 || ( Index() >= AData->Size ))
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			uint8_t ACount = MitovMin<int>( Count().GetValue(), int( AData->Size ) - int( Index().GetValue() ) );
			if( ACount == AData->Size )
				return;

			uint8_t AOutSize = AData->Size - ACount;
			TValueArray<T> ABuffer( true, AOutSize );
			for( int i = 0; i < Index().GetValue(); ++i )
				ABuffer.Data[ i ] = AData->Data[ i ];

			for( int i = Index().GetValue(); i < AOutSize; ++i )
				ABuffer.Data[ i ] = AData->Data[ ACount + i ];

			T_OutputPin::SetPinValue( ABuffer );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Index,
		typename T_OutputPin,
		typename T,
        typename T_ValueElements,
        typename T_ValueElements_ApplyValues,
        typename T_ValueElements_GetValue
	> class ArrayInsertItems :
		public T_Enabled,
		public T_Index,
		public T_OutputPin,
        public T_ValueElements
	{
	public:
		_V_PIN_( OutputPin ) 

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Index )
        _V_PROP_( ValueElements )

	public:
		inline void SetIndexedValue( uint32_t AIndex )
        {
        }

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			TArray<T> *AData = (TArray<T>*)_Data;

            uint32_t AIndex = MitovMin<uint32_t>( Index(), AData->Size );

			if( T_ValueElements::GetCount() == 0 )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			uint8_t AOutSize = AData->Size + T_ValueElements::GetCount();
			TValueArray<T> ABuffer( true, AOutSize );

			for( int i = 0; i < AIndex; ++i )
				ABuffer.Data[ i ] = AData->Data[ i ];

            if( T_ValueElements_ApplyValues::Count() )
            {
                typename T_ValueElements::T_ARRAY *ASubBuffer = T_ValueElements::GetMofifyDataArray();
                T_ValueElements_ApplyValues::Call( ASubBuffer->Data );
      			for( int i = 0; i < T_ValueElements::GetCount(); ++ i )
	    			ABuffer.Data[ i + AIndex ] = ASubBuffer->Data[ i ];

                delete ASubBuffer;
            }

            else
            {
                T_ValueElements::CopyData( ABuffer.Data + AIndex );
//    			for( int i = 0; i < T_ValueElements::GetCount(); ++ i )
//	    			ABuffer.Data[ i + AIndex ] = T_ValueElements::GetValue( i );

            }

			for( int i = AIndex; i < AData->Size; ++i )
				ABuffer.Data[ i + T_ValueElements::GetCount() ] = AData->Data[ i ];

			T_OutputPin::SetPinValue( ABuffer );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Index,
		typename T_OutputPin,
		typename T,
        typename T_ValueElements,
        typename T_ValueElements_ApplyValues,
        typename T_ValueElements_GetValue
	> class ArrayReplaceItemsAtIndex :
		public T_Enabled,
		public T_Index,
		public T_OutputPin,
        public T_ValueElements
	{
	public:
		_V_PIN_( OutputPin ) 

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Index )
        _V_PROP_( ValueElements )

	public:
		inline void SetIndexedValue( uint32_t AIndex )
        {
        }

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			TArray<T> *AData = (TArray<T>*)_Data;

			uint8_t AOutSize = AData->Size;
			if( T_ValueElements::GetCount() == 0 || ( Index() > AOutSize ))
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			uint8_t ACount = MitovMin<int>( T_ValueElements::GetCount(), int( AOutSize ) - int( Index() ) );
			if( ACount == AData->Size )
				return;

            TValueArray<T> ABuffer( true, AOutSize );

			for( int i = 0; i < Index(); ++i )
				ABuffer.Data[ i ] = AData->Data[ i ];

            if( T_ValueElements_ApplyValues::Count() )
            {
                typename T_ValueElements::T_ARRAY *ASubBuffer = T_ValueElements::GetMofifyDataArray();
                T_ValueElements_ApplyValues::Call( ASubBuffer->Data );
      			for( int i = 0; i < ACount; ++ i )
	    			ABuffer.Data[ i + Index() ] = ASubBuffer->Data[ i ];

                delete ASubBuffer;
            }

            else
                T_ValueElements::CopyData( ABuffer.Data + Index(), ACount );

//			for( int i = 0; i < ACount; ++i )
//				ABuffer.Data[ i + Index() ] = T_ValueElements::GetValue( i );

			for( int i = Index() + ACount; i < AOutSize; ++i )
				ABuffer.Data[ i ] = AData->Data[ i ];

//			for( int i = 0; i < Index; ++i )
//				AOutData[ i ] = AData->Data[ i ];

			T_OutputPin::SetPinValue( ABuffer );

//			OutputPin.Notify( &ABuffer );
//			uint8_t AOutSize = AData->Size;

/*
			if( _Count == 0 || ( Index > AData->Size ))
			{
				OutputPin.Notify( _Data );
				return;
			}

			uint8_t AOutSize = AData->Size + _Count;
			T *AOutData = new T[ AOutSize ];

			for( int i = 0; i < Index; ++i )
				AOutData[ i ] = AData->Data[ i ];

			for( int i = 0; i < _Count; ++i )
				AOutData[ i + Index ] = _Values[ i ];

			for( int i = Index; i < AData->Size; ++i )
				AOutData[ i + _Count ] = AData->Data[ i ];

			TArray<T> ABuffer( AOutSize, AOutData );
			OutputPin.Notify( &ABuffer );
			delete [] AOutData;
*/
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_FromValueElements,
        typename T_FromValueElements_ApplyValues,
        typename T_FromValueElements_GetValue,
		typename T_OutputPin,
		typename T,
        typename T_ToValueElements,
        typename T_ToValueElements_ApplyValues,
        typename T_ToValueElements_GetValue
	> class ArrayReplace :
		public T_Enabled,
        public T_FromValueElements,
		public T_OutputPin,
        public T_ToValueElements
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( FromValueElements )
		_V_PROP_( ToValueElements )

	public:
        inline void SetIndexedValue( uint32_t AIndex )
        {
        }

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			if( T_FromValueElements::GetCount() == 0 )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			TArray<T> &AData = *(TArray<T>*)_Data;
			TValueArray<T> AOutData;
			if( T_FromValueElements::GetCount() > AData.Size )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			int AEndIndex = AData.Size;

//			Serial.println( AData.ToString() );

			for( int i = AData.Size - T_FromValueElements::GetCount() + 1; i-- > 0; )
			{
//				for( int j = 0; j < _FromCount; ++j )
				bool AFound = true;
                if( T_FromValueElements_ApplyValues::Count() )
                {
                    for( int j = T_FromValueElements::GetCount(); j --; )
                    {
                        T AValue = T_FromValueElements::GetValue( j );
                        T_FromValueElements_GetValue::Call( j, AValue );
                        if( AData.Data[ i + j ] != AValue )
                        {
                            AFound = false;
                            break;
                        }
                    }
/*
                    typename T_FromValueElements::T_ARRAY *ASubBuffer = T_FromValueElements::GetMofifyDataArray();
                    T_FromValueElements_ApplyValues::Call( ASubBuffer->Data );
                    for( int i = 0; i < T_FromValueElements::GetCount(); ++ i )
                        if( AOutData.Data[ i ] != ASubBuffer->Data[ i ]
                        AOutData.Data[ i ] = ASubBuffer->Data[ i ];

                    delete ASubBuffer;
*/
                }

                else
                {
                    for( int j = T_FromValueElements::GetCount(); j --; )
                    {
//					Serial.print( i ); Serial.print( " + " ); Serial.print( j ); Serial.print( ": " ); Serial.print( AData.Data[ i + j ] ); Serial.print( " " ); Serial.println( _FromValues[ j ] );
                        if( AData.Data[ i + j ] != T_FromValueElements::GetValue( j ) )
                        {
                            AFound = false;
                            break;
                        }
                    }
				}

				if( AFound )
				{
//					Serial.println( "Test" );
					uint32_t ASubSize = AEndIndex - i - T_FromValueElements::GetCount();
					AOutData.ResizeFront( AOutData.Size + ASubSize + T_ToValueElements::GetCount() );

//					Serial.println( ASubSize );
//					delay( 1000 );
					AOutData.Copy( AData.Data + i + T_FromValueElements::GetCount(), ASubSize, T_ToValueElements::GetCount() );
//					Serial.println( AOutData.ToString() );
//					AOutData.Copy( T_ToValueElements::GetValue(), T_ToValueElements::GetCount(), 0 );

                    uint32_t ACount = MitovMin( T_ToValueElements::GetCount(), AOutData.Size );
                    if( T_ToValueElements_ApplyValues::Count() )
                    {
                        typename T_ToValueElements::T_ARRAY *ASubBuffer = T_ToValueElements::GetMofifyDataArray();
                        T_ToValueElements_ApplyValues::Call( ASubBuffer->Data );
                        for( int i = 0; i < ACount; ++ i )
                            AOutData.Data[ i ] = ASubBuffer->Data[ i ];

                        delete ASubBuffer;
                    }

                    else
                        T_ToValueElements::CopyData( AOutData.Data, ACount );

//                    for( int i = 0; i < MitovMin( ASize, inherited::Size - AOffset ); ++i )
//                        inherited::Data[ i + AOffset ] = AData[ i ];

//					Serial.println( AOutData.ToString() );
					AEndIndex = i;
					i -= T_FromValueElements::GetCount() - 1;
				}
			}

			if( AEndIndex )
			{
				AOutData.ResizeFront( AOutData.Size + AEndIndex );
				AOutData.Copy( AData.Data, AEndIndex, 0 );
			}

			T_OutputPin::SetPinValue( AOutData );
		}
	};
//---------------------------------------------------------------------------
	template <
        typename T_Enabled,
        typename T_OutputPin,
        typename T_TYPE
    > class TArduinoArrayToBinary :
        public T_Enabled,
        public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
                return;

			TArray<T_TYPE> &AData = *(TArray<T_TYPE>*)_Data;
            T_OutputPin::SetPinValue( TDataBlock( AData.Size * sizeof( T_TYPE ), AData.Data ));
        }
    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif