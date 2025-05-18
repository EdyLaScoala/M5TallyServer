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
#include "Mitov_BasicGenerator.h"

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FReference,
		typename T_OutputPin
	> class UVLightML8511 :
		public T_Enabled,
		public T_FReference,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FReference ) // = 0.66;

	protected:
		float	FValue = 0;

	protected:
		inline void Compute()
		{
			if( ! Enabled().GetValue() )
				return;

			//Use the 3.3V power pin as a reference to get a very accurate output value from sensor
			if( FReference().GetValue() == 0 )
			{
				T_OutputPin::SetPinValue( 15.0 );
				return;
			}

			float outputVoltage = 3.3 / FReference().GetValue() * FValue;
			float uvIntensity = Func::MapRange<float>( outputVoltage, 0.99, 2.9, 0.0, 15.0 );

			T_OutputPin::SetPinValue( uvIntensity );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Compute();
		}

		inline void ReferenceInputPin_o_Receive( void *_Data )
		{
			FReference() = *((float *)_Data );
		}

		inline void SensorInputPin_o_Receive( void *_Data )
		{
			FValue = *((float *)_Data );
		}

	public:
		inline void SystemStart()
		{
			Compute();
		}

		inline void SystemLoopBegin()
		{
			Compute();
		}

	};
//---------------------------------------------------------------------------
}

