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
#include <DAC.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled
	> class GoldilocksDAC :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		float	FValues[ 2 ];

	public:
		inline void SystemStart()
		{
			DAC_init(TRUE);
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FValues[ AIndex ] = *(float *)_Data;
			if( ! ClockInputPin_o_IsConnected() )
				ClockInputPin_o_Receive( _Data );

		}

		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			uint16_t aValue = FValues[ 0 ] * 1023 + 0.5;
			uint16_t bValue = FValues[ 1 ] * 1023 + 0.5;
			DAC_out( &aValue, &bValue );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif