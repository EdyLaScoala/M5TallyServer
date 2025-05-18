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
		typename T_Enabled,
		typename T_OutputPins_SetPinValue
	> class M5StackMakeyCapacitiveTouchUnitI2C : 
		public T_Address,
		public T_Enabled
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 2 ));
			if( C_I2C.available() )
			{
				uint16_t AKeys = C_I2C.read();
				AKeys |= uint16_t( C_I2C.read() ) << 8;

				for( int i = 0; i < 16; ++i )
					T_OutputPins_SetPinValue::Call( i, ( AKeys & ( 1 << i )) != 0, AChangeOnly );

			}

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

