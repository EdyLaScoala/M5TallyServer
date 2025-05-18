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
#include <Mitov_Microchip_MCP4725.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_ShiftDown,
		typename T_ShiftDownMode,
		typename T_WriteToEEPROM
	> class ArduinoEVIVEAnalogOutput :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_InitialValue,
		public T_ShiftDown,
		public T_ShiftDownMode,
		public T_WriteToEEPROM
	{
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( ShiftDown )
		_V_PROP_( WriteToEEPROM )
		_V_PROP_( ShiftDownMode )

		_V_PROP_( InitialValue )

		float	CurrentValue = 0;

	protected:
		static const uint8_t	MCP4726_CMD_WRITEDAC		= 0x40;  // Writes data to the DAC
		static const uint8_t	MCP4726_CMD_WRITEDACEEPROM  = 0x60;  // Writes data to the DAC and the EEPROM (persisting the assigned value after reset)

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float * )_Data;
			if( Enabled() )
				if( ! ClockInputPin_o_IsConnected() )
					ClockInputPin_o_Receive( nullptr );

		}

		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

//			Serial.println( "DoClockReceive" );
			CurrentValue = InitialValue();
			UpdateValue();
		}

	public:
		void UpdateValue()
		{
			uint16_t AValue = MitovConstrain( CurrentValue, 0.0f, 1.0f ) * 0xFFF + 0.5;
#ifdef TWBR
			uint8_t twbrback = TWBR;
			TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz
#endif
			Wire.beginTransmission( 0x62 );

			uint8_t	ACommand;
  			if( WriteToEEPROM() )
				ACommand = MCP4726_CMD_WRITEDACEEPROM;

			else
				ACommand = MCP4726_CMD_WRITEDAC;

			if( ShiftDown() )
				ACommand |= uint8_t( ShiftDownMode() ) + 1;

			Wire.write( ACommand );

//			Serial.println( ACommand, HEX );
//			Serial.println( CurrentValue );
//			Serial.println( AValue );

			Wire.write( AValue / 16);                   // Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
			Wire.write(( AValue % 16) << 4);            // Lower data bits          (D3.D2.D1.D0.x.x.x.x)
			Wire.endTransmission();

#ifdef TWBR
			TWBR = twbrback;
#endif		
		}

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				ClockInputPin_o_Receive( nullptr );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif