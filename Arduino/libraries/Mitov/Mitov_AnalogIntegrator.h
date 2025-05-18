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
//		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FCurrentValue,
		typename T_FLastInput,
		typename T_InitialValie,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin
	> class AnalogIntegrator :
//		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FCurrentValue,
		public T_FLastInput,
		public T_InitialValie,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValie )
		_V_PROP_( Value_Max )
		_V_PROP_( Value_Min )

	protected:
//		_V_PROP_( ClockInputPin_o_IsConnected )
		_V_PROP_( FCurrentValue )
		_V_PROP_( FLastInput )

	protected:
		void Calculate( float AValue )
		{
			if( AValue > 0 )
			{
				float AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				float AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				if( FCurrentValue() < AMax - AValue )
					FCurrentValue() = FCurrentValue().GetValue() + AValue;

                else
				{
					if( Value_Max().RollOver() )
						FCurrentValue() = V_FMOD( FCurrentValue() + AValue, AMax - AMin ) + AMin;

					else
						FCurrentValue() = AMax;

				}
			}

			else if( AValue < 0 )
			{
				float AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				float AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );

				if( FCurrentValue() > AMin - AValue )
					FCurrentValue() = FCurrentValue().GetValue() + AValue;

                else
				{
					if( Value_Min().RollOver() )
						FCurrentValue() = V_FMOD( FCurrentValue() + AValue, AMax - AMin ) + AMin;

					else
						FCurrentValue() = AMin;

				}
			}

			T_OutputPin::SetPinValue( FCurrentValue() );
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
//			if( ClockInputPin_o_IsConnected() )
				FLastInput() = AValue;

//			else
//				Calculate( AValue );

		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Calculate( FLastInput() );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			FCurrentValue() = InitialValie().GetValue();
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue() = InitialValie().GetValue();
            T_OutputPin::SetPinValue( FCurrentValue() );
		}

		inline void SystemLoopBegin()
		{
			Calculate( FLastInput() );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif