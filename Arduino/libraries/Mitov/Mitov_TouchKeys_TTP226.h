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
		typename T_ClockOutputPin,
		uint8_t C_DataInputPin,
		typename T_Enabled,
		typename T_OutputPins_SetPinValue,
        typename T_ResetOutputPin
	> class TArduinoTouchKeysTTP226_Serial :
		public T_ClockOutputPin,
		public T_Enabled,
        public T_ResetOutputPin
	{
	public:
        _V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void UpdateEnabled()
		{
			if( Enabled().GetValue() )
				return;

			for( int i = 0; i < 8; ++ i )
				T_OutputPins_SetPinValue::Call( i, false );

		}

	protected:
		void ReadButtons()
		{
			if( ! Enabled().GetValue() )
				return;

			delayMicroseconds( 62 );
			for( int i = 0; i < 8; ++ i )
			{
				T_ClockOutputPin::SetPinValueLow();
				delayMicroseconds( 62 );
				T_OutputPins_SetPinValue::Call( i, digitalRead( C_DataInputPin ) == HIGH );
				T_ClockOutputPin::SetPinValueHigh();
				delayMicroseconds( 62 );
			}
		}


	public:
		inline void SystemStart()
		{
    		T_ClockOutputPin::SetPinValueLow();
            T_ResetOutputPin::SetPinValueLow();
            delayMicroseconds( 100 );
            T_ResetOutputPin::SetPinValueHigh();
            delayMicroseconds( 100 );
            T_ResetOutputPin::SetPinValueLow();
			ReadButtons();
		}

		inline void SystemLoopBegin()
		{
			ReadButtons();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

