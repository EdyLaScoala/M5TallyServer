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

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
        typename T_Elements,
        typename T_Elements_ApplyValues,
        typename T_Elements_GetValue,
		typename T_InitialIndex,
		typename T_OutputPin,
		typename T
	> class ValueArray :
		public T_ClockInputPin_o_IsConnected,
        public T_Elements,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )
		_V_PROP_( Elements )

//	public:
//		T *_Values;
//		uint32_t _MaxIndex;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

//	public:
//		inline operator T *() { return _Values; }

    public:
        T *GetValues() { return T_Elements::GetArray(); }

    protected:
        void SendOutput()
        {
            T AValue = T_Elements::GetValue( InitialIndex() );

            T_Elements_GetValue::Call( InitialIndex(), AValue );

            T_OutputPin::SetPinValue( AValue );
        }

	public:
		void SetIndexedValue( uint32_t AIndex )
        {
//            T_Elements::SetValue( AIndex, AValue );
			if( ! ClockInputPin_o_IsConnected() )
                if( AIndex == InitialIndex() )
                    SendOutput();
        }

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > T_Elements::GetCount() - 1 )
				InitialIndex() = T_Elements::GetCount() - 1;

			UpdateArrayValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            SendOutput();
		}

		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > T_Elements::GetCount() - 1 )
				AIndex = T_Elements::GetCount() - 1;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;

			UpdateArrayValue();
		}

	public:
//        void UpdateArrayValueIndex( uint32_t AIndex )
//        {
//			if( ! ClockInputPin_o_IsConnected() )
//                if( AIndex == InitialIndex() )
//    				T_OutputPin::SetPinValue( T_Elements::GetValue( AIndex ));
//        }

		void UpdateArrayValue()
		{
			if( ! ClockInputPin_o_IsConnected() )
                SendOutput();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
        typename T_Elements,
        typename T_Elements_GetValue,
		typename T_InitialIndex,
		typename T_OutputPin
	> class ValueArrayDigital :
		public T_ClockInputPin_o_IsConnected,
        public T_Elements,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )
        _V_PROP_( Elements )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

    protected:
        void SendOutput()
        {
            bool AValue = T_Elements::GetValue( InitialIndex() / 8 ) & ( 1 << ( InitialIndex() & 0b111 ));

            T_Elements_GetValue::Call( InitialIndex(), AValue );

            T_OutputPin::SetPinValue( AValue );
        }

	public:
		void SetIndexedValue( uint32_t AIndex )
        {
			if( ! ClockInputPin_o_IsConnected() )
                if( AIndex == InitialIndex() )
                    SendOutput();
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            SendOutput();
		}

		void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > T_Elements::GetCount() - 1 )
				AIndex = T_Elements::GetCount() - 1;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;
			UpdateArrayValue();
		}

	public:
		void UpdateArrayValue()
		{
			if( ! ClockInputPin_o_IsConnected() )
                SendOutput();

		}

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > T_Elements::GetCount() - 1 )
				InitialIndex() = T_Elements::GetCount() - 1;

			UpdateArrayValue();
		}

	};
//---------------------------------------------------------------------------
	template <
        typename T_Value
    > class TArduinoTextValueElement :
        public T_Value
    {
	public:
		_V_PROP_( Value )

	public:
        inline void GetValue( const char * &AValue )
        {
            AValue = Value().c_str();
        }

    };
//---------------------------------------------------------------------------
	template <
        typename T_Value
    > class TArduinoBinaryValueElement :
        public T_Value
    {
	public:
		_V_PROP_( Value )

	public:
        inline void GetValue( const uint8_t * &AValue, uint32_t & ASize )
        {
            AValue = Value().Allocate();
            ASize = Value().GetCount();
        }

        inline void ReleaseValue( const uint8_t * AValue )
        {
            Value().Release( AValue );
        }

    };
//---------------------------------------------------------------------------
	template <
        uint32_t C_COUNT,
		typename T_ClockInputPin_o_IsConnected,
        typename T_Elements_GetValue,
		typename T_InitialIndex,
		typename T_OutputPin
	> class TextValueArray :
		public T_ClockInputPin_o_IsConnected,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

    protected:
        inline void SendOutput()
        {
            const char * AValue = "";
            T_Elements_GetValue::Call( InitialIndex(), AValue );
			T_OutputPin::SetPinValue( AValue );
        }

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > C_COUNT - 1 )
				InitialIndex() = C_COUNT - 1;

			UpdateArrayValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            SendOutput();
		}

		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > C_COUNT - 1 )
				AIndex = C_COUNT - 1;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;

			UpdateArrayValue();
		}

	public:
		void UpdateArrayValue()
		{
			if( ! ClockInputPin_o_IsConnected() )
                SendOutput();
		}

	};
//---------------------------------------------------------------------------
	template <
        uint32_t C_COUNT,
		typename T_ClockInputPin_o_IsConnected,
        typename T_Elements_GetValue,
        typename T_Elements_ReleaseValue,
		typename T_InitialIndex,
		typename T_OutputPin
	> class BinaryValueArray :
		public T_ClockInputPin_o_IsConnected,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

    protected:
        inline void SendOutput()
        {
            const uint8_t * AValue = nullptr;
            uint32_t ASize = 0;
            uint32_t AIndex = InitialIndex();
            T_Elements_GetValue::Call( AIndex, AValue, ASize );
			T_OutputPin::SetPinValue( TDataBlock( ASize, AValue ));
            T_Elements_ReleaseValue::Call( AIndex, AValue );
        }

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > C_COUNT - 1 )
				InitialIndex() = C_COUNT - 1;

			UpdateArrayValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            SendOutput();
		}

		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > C_COUNT - 1 )
				AIndex = C_COUNT - 1;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;

			UpdateArrayValue();
		}

	public:
		void UpdateArrayValue()
		{
			if( ! ClockInputPin_o_IsConnected() )
                SendOutput();
		}

	};
//---------------------------------------------------------------------------
}

