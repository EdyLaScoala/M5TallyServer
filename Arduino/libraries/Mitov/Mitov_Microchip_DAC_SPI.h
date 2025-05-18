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
#include <SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FModified,
		typename T_Gain,
		uint8_t C_INDEX,
		typename T_InitialValue,
		uint8_t C_MULTIPLIER
	> class Microchip_DACSPIChannel :
		public T_Enabled,
		public T_FModified,
		public T_Gain,
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

		_V_PROP_( Enabled )
		_V_PROP_( Gain )

	protected:
		_V_PROP_( FModified )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			FModified() = true;
		}

	public:
		inline void SendOutput()
		{
			if( ! FModified() )
				return;

			FModified() = true;

			word volt_digits = InitialValue() * C_MULTIPLIER + 0.5;

			word packet = volt_digits & 0b111111111111;  //shift voltage setting digits
			if( Enabled() )
				if( C_OWNER.Enabled() )
					packet |= 1 << 12;

			if( Gain() )
				packet |= 1 << 13;   //add gain setting

			if( C_INDEX )
				packet |= 1 << 15;   //add gain setting

			C_OWNER.SPI_write( packet );
		}

	public:
		inline void ValueModified()
		{
			FModified() = true;
		}

	public:
		inline Microchip_DACSPIChannel()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		uint8_t C_COUNT_CHANNELS,
		typename T_Channels_SendOutput,
		typename T_Channels_ValueModified,
		typename T_ChipSelectOutputPin,
		typename T_Enabled,
		typename T_LatchOutputPin
	> class Microchip_DAC_SPI :
		public T_Channels_SendOutput,
		public T_Channels_ValueModified,
		public T_ChipSelectOutputPin,
		public T_Enabled,
		public T_LatchOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( LatchOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void UpdateEnabled()
		{
			T_Channels_ValueModified::Call();
		}

	protected:
		void UpdateLatch()
		{
			T_LatchOutputPin::SetPinValueLow();
			T_LatchOutputPin::SetPinValueHigh();
		}

	public:
		inline void SystemLoopEnd()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			UpdateLatch();
			T_Channels_SendOutput::Call();
		}

	public:
		void SPI_write( word APacket )
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( APacket >> 8 );
			C_SPI.transfer( APacket & 0xFF );
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif