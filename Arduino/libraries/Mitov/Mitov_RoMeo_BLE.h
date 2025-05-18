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
		typename T_OutputPins_S1,
		typename T_OutputPins_S2,
		typename T_OutputPins_S3,
		typename T_OutputPins_S4,
		typename T_OutputPins_S5
	> class ArduinoRoMeoBLEButtons :
		public T_OutputPins_S1,
		public T_OutputPins_S2,
		public T_OutputPins_S3,
		public T_OutputPins_S4,
		public T_OutputPins_S5
	{
	public:
		_V_PIN_( OutputPins_S1 )
		_V_PIN_( OutputPins_S2 )
		_V_PIN_( OutputPins_S3 )
		_V_PIN_( OutputPins_S4 )
		_V_PIN_( OutputPins_S5 )

	protected:
		void ReadButtons( bool AOnlyChanged )
		{
			const int  adc_key_val[ 5 ] = { 30, 150, 360, 535, 760 };

			int input = analogRead( 7 );
			int AButtonIndex = -1;
			for( int i = 0; i < 5; ++ i )
				if (input < adc_key_val[i])
				{
					AButtonIndex = i;
					break;
				}
		
			T_OutputPins_S1::SetPinValue( ( 0 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S2::SetPinValue( ( 1 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S3::SetPinValue( ( 2 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S4::SetPinValue( ( 3 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S5::SetPinValue( ( 4 == AButtonIndex ), AOnlyChanged );
		}

	public:
		inline void SystemStart()
		{
			ReadButtons( false );
		}

		inline void SystemLoopBegin()
		{
			ReadButtons( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif