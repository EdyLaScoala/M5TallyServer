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
		int C_NUM_OUTPUTS,
		typename T_InitialChannel,
		typename T_OutputPins_SetPinValue
	> class ClockDemux :
		public T_InitialChannel
	{
	public:
		_V_PROP_( InitialChannel )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T_OutputPins_SetPinValue::Call( InitialChannel().GetValue() );
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			uint32_t AChannel = *(uint32_t *)_Data;
			if( AChannel >= C_NUM_OUTPUTS )
				AChannel = C_NUM_OUTPUTS - 1;

			InitialChannel() = AChannel;
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_OUTPUTS,
		typename T_InitialChannel,
		typename T_OutputPins_SetPinValue,
		typename T
	> class CommonDemux :
		public T_InitialChannel
	{
	public:
		_V_PROP_( InitialChannel )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_OutputPins_SetPinValue::Call( InitialChannel().GetValue(), *(T *)_Data );
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			uint32_t AChannel = *(uint32_t *)_Data;
			if( AChannel >= C_NUM_OUTPUTS )
				AChannel = C_NUM_OUTPUTS - 1;

			InitialChannel() = AChannel;
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_NUM_OUTPUTS,
		typename T_InitialChannel,
		typename T_OutputPins_SetPinValue
	> class TextDemux :
		public T_InitialChannel
	{
	public:
		_V_PROP_( InitialChannel )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_OutputPins_SetPinValue::Call( InitialChannel().GetValue(), (char *)_Data );
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			uint32_t AChannel = *(uint32_t *)_Data;
			if( AChannel >= C_NUM_OUTPUTS )
				AChannel = C_NUM_OUTPUTS - 1;

			InitialChannel() = AChannel;
		}
	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_FalseOutputPin,
		typename T_InitialSelectValue,
		typename T_TrueOutputPin
	> class ClockToggleDemux :
		public T_FalseOutputPin,
		public T_InitialSelectValue,
		public T_TrueOutputPin
	{
	public:
		_V_PIN_( FalseOutputPin )
		_V_PIN_( TrueOutputPin )

	public:
		_V_PROP_( InitialSelectValue )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( InitialSelectValue() )
				TrueOutputPin().ClockPin();

			else
				FalseOutputPin().ClockPin();

		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			InitialSelectValue() = *(bool *)_Data;
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_FalseOutputPin,
		typename T_InitialSelectValue,
		typename T_TrueOutputPin
	> class ToggleDemux :
		public T_FalseOutputPin,
		public T_InitialSelectValue,
		public T_TrueOutputPin
	{
	public:
		_V_PIN_( FalseOutputPin )
		_V_PIN_( TrueOutputPin )

	public:
		_V_PROP_( InitialSelectValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( InitialSelectValue() )
				TrueOutputPin().PinNotify( _Data );

			else
				FalseOutputPin().PinNotify( _Data );
		}

		inline void SelectInputPin_o_Receive( void *_Data )
		{
			InitialSelectValue() = *(bool *)_Data;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif