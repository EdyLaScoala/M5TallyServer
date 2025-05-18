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
		typename T_IsPoweredOutputPin,
		typename T_OverCurrentOutputPin
	> class ArduinoMacchinaPowerSupplyModule :
		public T_Enabled,
		public T_IsPoweredOutputPin,
		public T_OverCurrentOutputPin
	{
	public:
		_V_PIN_( IsPoweredOutputPin )
		_V_PIN_( OverCurrentOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void UpdateConfig()
		{
			digitalWrite( I_SENSE_EN, ( Enabled() ) ? HIGH : LOW );
		}

	protected:
		void UpdateOutputs( bool AChangeOnly )
		{
			if( T_IsPoweredOutputPin::GetPinIsConnected() )
			{
				bool AValue = ( digitalRead( I_SENSE ) == HIGH );
				T_IsPoweredOutputPin::SetPinValue( AValue, AChangeOnly );
			}

			if( T_OverCurrentOutputPin::GetPinIsConnected() )
			{
				bool AValue = ( digitalRead( I_SENSE_INT ) == HIGH );
				T_OverCurrentOutputPin::SetPinValue( AValue, AChangeOnly );
			}
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs( true );
		}

		inline void SystemLoopBegin()
		{
			UpdateOutputs( false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif