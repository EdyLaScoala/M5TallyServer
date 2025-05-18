////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Mitov
{
	template <
		typename T_HasVBusOutputPin,
		typename T_PowerSave
	> class TArduinoRP2040_RaspberryPi_Pico_PowerModule :
		public T_HasVBusOutputPin,
		public T_PowerSave
	{
	public:
		_V_PROP_( HasVBusOutputPin )
		_V_PROP_( PowerSave )

	public:
		void Update_PowerSave()
		{
			digitalWrite( 23, PowerSave().GetValue() ? LOW : HIGH );
		}

	public:
		inline void SystemStart()
		{
			Update_PowerSave();
			T_HasVBusOutputPin::SetPinValue( digitalRead( 24 ) ? HIGH : LOW, false );
		}


		inline void SystemLoopBegin()
		{
			T_HasVBusOutputPin::SetPinValue( digitalRead( 24 ) ? HIGH : LOW, true );
		}

	};

} // Mitov