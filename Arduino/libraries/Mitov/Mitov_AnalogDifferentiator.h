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
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FCurrentValue,
		typename T_FLastInput,
		typename T_FPopulated,
		typename T_OutputPin
	> class AnalogDifferentiator :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FCurrentValue,
		public T_FLastInput,
		public T_FPopulated,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FCurrentValue )
		_V_PROP_( FLastInput )
		_V_PROP_( FPopulated )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		void Calculate( float AValue )
		{
			if( FPopulated())
			{
				float ADifference = AValue - FCurrentValue();
				T_OutputPin::SetPinValue( ADifference );
			}

            else
                FPopulated() = true;

			FCurrentValue() = AValue;
		}

	public:
		inline void ExternalSetValue( int32_t AIndex, float AValue )
		{
			FCurrentValue() = AValue;
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

        inline void ExternalAddValue( float AValue )
        {
			Calculate( AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			float AValue = *(float *)_Data;
			if( ClockInputPin_o_IsConnected() )
				FLastInput() = AValue;

			else
				Calculate( AValue );

		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Calculate( FLastInput() );
		}

	public:
		inline void SystemStart()
		{
            T_OutputPin::SetPinValue( 0 );
		}

	public:
		inline AnalogDifferentiator()
		{
			FPopulated() = false;
		}


	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif