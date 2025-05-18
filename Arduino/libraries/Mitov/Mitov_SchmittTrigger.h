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
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Inverted,
		typename T_OutputPin,
		typename T, 
		typename T_Threshold,
		typename T_Value
	> class SchmittTrigger : 
		public T_Enabled,
		public T_InitialValue,
		public T_Inverted,
		public T_OutputPin,
		public T_Threshold,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin ) 

    public:
        _V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Inverted )

    public:
        _V_PROP_( Value )
		_V_PROP_( Threshold )

	protected:
		T	FCurrentValue;

    public:
		void ProcessOutput()
		{
			bool AValue;
			if( ! Enabled().GetValue() )
				AValue = false;

			else
			{
				if( Inverted() )
				{
					if( InitialValue() )
						AValue = ( FCurrentValue < Value().GetValue() + Threshold().GetValue() );

					else
						AValue = ( FCurrentValue < Value().GetValue() - Threshold().GetValue() );

				}

				else
				{
					if( InitialValue() )
						AValue = ( FCurrentValue > Value().GetValue() - Threshold().GetValue() );

					else
						AValue = ( FCurrentValue > Value().GetValue() + Threshold().GetValue() );

				}
			}

			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T AValue = *(T*)_Data;
			if( AValue == FCurrentValue )
				return;

			FCurrentValue = AValue;
			ProcessOutput();
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( InitialValue().GetValue() );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif