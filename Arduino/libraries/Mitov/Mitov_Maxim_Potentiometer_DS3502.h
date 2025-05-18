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
		typename T_InitialValue,
		typename T_NonVolatile
	> class TArduinoMaximPotentiometerDS3502_I2C :
		public T_Address,
		public T_Enabled,
		public T_InitialValue,
		public T_NonVolatile
	{
	protected:
		static const uint8_t WIPER	= 0x00; ///< Wiper value register
		static const uint8_t MODE	= 0x02;  ///< Mode selection register

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( NonVolatile )


	public:
		inline void UpdateNonVolatile()
		{
			WriteRegister( MODE, ( NonVolatile().GetValue() ) ? 0x00 : 0x80 );
		}

	protected:
		void WriteRegister( uint8_t ARegister, uint8_t Adata )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));

			C_I2C.write( ARegister );
			C_I2C.write( Adata );

			C_I2C.endTransmission();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *( float * )_Data;
			WriteRegister( WIPER, ( 127 * AValue ) + 0.5 );
		}

	public:
		inline void SystemStart()
		{
			UpdateNonVolatile();
			WriteRegister( WIPER, ( 127 * InitialValue().GetValue() ) + 0.5 );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

