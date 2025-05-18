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
		typename T_DATA,
		typename T_Enabled,
		typename T_FLastData,
//		typename T_FForceRemember,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_STORE
	> class Remember :
		public T_Enabled,
		public T_FLastData,
//		public T_FForceRemember,
		public T_InitialValue,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( InitialValue )

    public:
		_V_PROP_( RecallInputPin_o_IsConnected )

	protected:
		_V_PROP_( FLastData )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_STORE AValue = ExtractValue<T_STORE>( _Data );
			FLastData() = AValue;
		}

		void RememberInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
			{
				InitialValue() = FLastData().GetValue();
				if( ! RecallInputPin_o_IsConnected().GetValue() )
					T_OutputPin::SetPinValue( InitialValue().GetValue() );
			}
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue().GetValue() );
		}

	public:
		inline Remember()
		{
			FLastData() = InitialValue().GetValue();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_DATA,
		typename T_Enabled,
//		typename T_FForceRemember,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_STORE
	> class Remember_Simple :
		public T_Enabled,
//		public T_FForceRemember,
		public T_InitialValue,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( InitialValue )

    public:
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_STORE AValue = ExtractValue<T_STORE>( _Data );
    		InitialValue() = AValue;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_DATA,
		typename T_Enabled,
		typename T_InitialValueElements,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected
    > class RememberArray :
		public T_Enabled,
		public T_InitialValueElements,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected
    {
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

    public:
		_V_PROP_( RecallInputPin_o_IsConnected )

    protected:
        TValueArray<T_DATA> FLastData;
        TValueArray<T_DATA> FInitialValue;

	public:
        inline void SetIndexedValue( uint32_t AIndex )
        {
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FLastData = *((TArray<T_DATA> *) _Data );
		}

		void RememberInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
			{
				FInitialValue = FLastData;
				if( ! RecallInputPin_o_IsConnected().GetValue() )
					T_OutputPin::SetPinValue( FInitialValue );
			}
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( FInitialValue );

		}

	public:
		inline RememberArray() :
            FLastData( T_InitialValueElements::GetCount(), T_InitialValueElements::GetValue() ),
            FInitialValue( T_InitialValueElements::GetCount(), T_InitialValueElements::GetValue() )
		{
		}
    };
//---------------------------------------------------------------------------
	template <
		typename T_DATA,
		typename T_Enabled,
		typename T_InitialValueElements,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected
    > class RememberArray_Simple :
		public T_Enabled,
		public T_InitialValueElements,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected
    {
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

    public:
		_V_PROP_( RecallInputPin_o_IsConnected )

    protected:
        TValueArray<T_DATA> FInitialValue;

	public:
        inline void SetIndexedValue( uint32_t AIndex )
        {
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//            Serial.println( "InputPin_o_Receive" );
    		FInitialValue = *((TArray<T_DATA> *) _Data );
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
            {
/*
                for( int i = 0; i < FInitialValue.Size; ++ i )
                {
                    Serial.print( FInitialValue.Data[ i ] );
                    Serial.print( "," );
                }
*/
				T_OutputPin::SetPinValue( FInitialValue );
            }

		}

	public:
		inline RememberArray_Simple() :
            FInitialValue( T_InitialValueElements::GetCount(), T_InitialValueElements::GetValue() )
		{
		}
    };
//---------------------------------------------------------------------------
/*
	template<> void Remember<Mitov::String, char>::InputPin_o_Receive( void *_Data )
	{
		Mitov::String AValue = (char *)_Data;
		FLastData = AValue;
		FPopulated = true;

		if( FForceRemember )
		{
			FSavedData = FLastData;
#ifdef _MITOV_REMEMBER_RECALL_PIN_
			if( ! RecallInputPin_o_IsConnected )
#endif // _MITOV_REMEMBER_RECALL_PIN_
				OutputPin.SendValue( FSavedData );
		}

		FForceRemember = false;
	}
*/
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif