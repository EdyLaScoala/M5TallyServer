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
		typename T_I2C, T_I2C &C_I2C,
		typename T_Enabled, 
		typename T_OutputPins_X,
		typename T_OutputPins_Y
	> class ESPCopter_OpticFlow : 
		public T_Enabled, 
		public T_OutputPins_X,
		public T_OutputPins_Y
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )

	public:
		_V_PROP_( Enabled )

//	protected:
//		unsigned long FLastTime = 0;

	protected:
		void ReadSensors()
		{
			C_I2C.requestFrom( 8, 6 );    // request 6 bytes from slave device #8

			int16_t AXValue = ( int16_t( C_I2C.read() ) << 8 ) | C_I2C.read();
			int16_t AYValue = ( int16_t( C_I2C.read() ) << 8 ) | C_I2C.read();
			int16_t ADeltaCsr = ( int16_t( C_I2C.read() ) << 8 ) | C_I2C.read();
			if( (AYValue + AXValue) == ADeltaCsr)
			{
				T_OutputPins_X::SetPinValue( AXValue );
				T_OutputPins_Y::SetPinValue( AYValue );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadSensors();
		}

		inline void SystemStart()
		{
			ReadSensors();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

