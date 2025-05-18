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
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
		typename T_ID,
        typename T_InitialValue,
		typename T_Request
	> class CanBus_SendMessage :
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_ID,
        public T_InitialValue,
		public T_Request
	{
	public:
		_V_PROP_( Extended )
		_V_PROP_( ID )
		_V_PROP_( InitialValue )
		_V_PROP_( Request )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void ProcessMessage( uint8_t  ALength, uint8_t * ABuffer ) {}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
/*
            if( T_InitialValue::GetIsConstant() )
            {
                TValueArray<uint8_t> ABuffer( true, T_InitialValue::GetCount() );
                T_InitialValue::CopyData( ABuffer.Data );
            }

            else
    			C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), Request().GetValue(), T_InitialValue::GetCount(), T_InitialValue::GetArray() );
*/
            uint8_t *ABuffer = T_InitialValue::Allocate();
//            T_InitialValue_ApplyValues::Call( ABuffer );
   			C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), Request().GetValue(), T_InitialValue::GetCount(), ABuffer );
            T_InitialValue::Release( ABuffer );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
			InitialValue = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ExtendedOutputPin,
		typename T_ID,
		typename T_MessageOutputPin,
		typename T_RequestOutputPin
	> class CanBus_ReceiveMessage : 
		public T_ExtendedOutputPin,
		public T_ID,
		public T_MessageOutputPin,
		public T_RequestOutputPin
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( ID )

	public:
		Mitov::TValueArray<uint8_t> InitialValue;

	public:
		inline void ProcessMessage( uint8_t  ALength, uint8_t * ABuffer )
		{
//			T_IDOutputPin::SetPinValue( *(uint32_t *)ABuffer );

			T_ExtendedOutputPin::SetPinValue( ABuffer[ 4 ] & 0b01 );
			T_RequestOutputPin::SetPinValue( ABuffer[ 4 ] & 0b10 );

			if( T_MessageOutputPin::GetPinIsConnected() )
			{
				TDataBlock AData( ALength, ABuffer + 5 );
				T_MessageOutputPin::SetPinValue( AData );
			}

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
		typename T_ExtendedOutputPin,
		typename T_ID,
        typename T_InitialValue,
		typename T_MessageOutputPin,
		typename T_RequestOutputPin
	> class CanBus_RequestMessage : 
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_ExtendedOutputPin,
		public T_ID,
        public T_InitialValue,
		public T_MessageOutputPin,
		public T_RequestOutputPin
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( Extended )
		_V_PROP_( ID )
		_V_PROP_( InitialValue )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void ProcessMessage( uint8_t  ALength, uint8_t * ABuffer ) {}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
            uint8_t *ABuffer = T_InitialValue::Allocate();
//            T_InitialValue_ApplyValues::Call( ABuffer );
   			C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), true, T_InitialValue::GetCount(), ABuffer );
            T_InitialValue::Release( ABuffer );

//			C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), true, InitialValue.Size, InitialValue.Data );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
			InitialValue = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
