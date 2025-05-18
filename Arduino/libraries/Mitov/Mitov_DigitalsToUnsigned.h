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
		int C_NUM_INPUTS,
		typename T_FModified,
		typename T_OutputPin
	> class DigitalsToUnsigned :
		public T_FModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		_V_PIN_( FModified )

		uint8_t	FValues[ ( C_NUM_INPUTS + 7 ) / 8 ] = { 0 };

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			if( Func::SetBitField( FValues, AIndex, *(bool *)_Data ))
				FModified() = true;

		}

	protected:
		void CalculateSendOutput( bool AFromStart )
		{
			uint32_t AValue = 0;
			for( uint32_t i = C_NUM_INPUTS; i--; )
				if(( FValues[ i / 8 ] & ( 1 << ( i & 0b111 ))) != 0 )
					AValue |= ((uint32_t)1) << i;

			T_OutputPin::SetPinValue( AValue, ! AFromStart );
			FModified() = false;
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( false );

		}

	public:
		inline DigitalsToUnsigned()
		{
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif