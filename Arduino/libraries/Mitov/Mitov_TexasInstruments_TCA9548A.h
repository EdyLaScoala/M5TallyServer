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
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_FCurrentIndex
	> class TexasInstrumentsTCA9548A : 
		public T_Address,
		public T_FCurrentIndex
	{
	public:
		_V_PROP_( Address )

	protected:
		_V_PROP_( FCurrentIndex )

	public:
		void SetIndex( uint8_t AIndex )
		{
			if( FCurrentIndex().GetValue() == AIndex )
				return;
			
//			Serial.println( AIndex );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( uint8_t( 1 ) << AIndex );
			C_I2C.endTransmission();

			FCurrentIndex() = AIndex;
//			Serial.println( FCurrentIndex().GetValue() );
		}

		inline static T_I2C &GetI2C() { return C_I2C; }

	public:
		inline TexasInstrumentsTCA9548A()
		{
			FCurrentIndex() = 8;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

