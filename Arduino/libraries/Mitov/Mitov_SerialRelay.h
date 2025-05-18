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

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		uint8_t C_INDEX,
		typename T_InitialValue
	> class SerialRelay :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void UpdateOutput()
		{
//			const uint8_t COn[] = { 0xA0, 0x01, 0x01, 0xA2 };
//			const uint8_t COff[] = { 0xA0, 0x01, 0x00, 0xA1 };

			const uint8_t COn[] = { 0xA0, 0x01 + C_INDEX, 0x01, 0xA2 + C_INDEX };
			const uint8_t COff[] = { 0xA0, 0x01 + C_INDEX, 0x00, 0xA1 + C_INDEX };

			if( Enabled().GetValue() && InitialValue().GetValue() )
				Serial.write( COn, 4 );

			else
				Serial.write( COff, 4 );

			
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( InitialValue() == *(bool *)_Data )
				return;

			InitialValue() = *(bool *)_Data;
			UpdateOutput();
		}

	public:
		inline void SystemInit()
		{
			UpdateOutput();
		}
	};
//---------------------------------------------------------------------------
}

