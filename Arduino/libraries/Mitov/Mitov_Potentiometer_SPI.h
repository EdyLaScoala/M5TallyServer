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
		uint8_t C_RESOLUTION,
		typename T_ConnectTerminalA,
		typename T_ConnectTerminalB,
		typename T_ConnectWiper,
		typename T_Enabled,
		typename T_FConfigModified,
		typename T_FValueModified,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_NonVolatile
	> class PotentiometerSPIChannel :
		public T_ConnectTerminalA,
		public T_ConnectTerminalB,
		public T_ConnectWiper,
		public T_Enabled,
		public T_FConfigModified,
		public T_FValueModified,
		public T_InitialValue,
		public T_NonVolatile
	{
	public:
		_V_PROP_( InitialValue )

	public:
		_V_PROP_( Enabled )

		_V_PROP_( ConnectTerminalA )
		_V_PROP_( ConnectWiper )
		_V_PROP_( ConnectTerminalB )
		_V_PROP_( NonVolatile )

	protected:
		_V_PROP_( FConfigModified )
		_V_PROP_( FValueModified )

	public:
		inline void ConfigModified()
		{
			FValueModified() = true;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			FValueModified() = true;
		}

	public:
		void InitChannel() // T_OWNER *AOwner, uint8_t AIndex )
		{
			if( ! FConfigModified() )
				return;

			FConfigModified() = false;

			uint16_t ATerminalControlValue = C_OWNER.SPI_read( 4 );
			uint16_t ANewValue;
			if( ConnectTerminalB() )
				ANewValue = 1;

			else
				ANewValue = 0;

			if( ConnectWiper() )
				ANewValue |= B10;

			if( ConnectTerminalA() )
				ANewValue |= B100;

			if( ! Enabled() )
				ANewValue |= B1000;

			if( C_INDEX == 1 )
			{
				ATerminalControlValue &= 0xFF0F;
				ATerminalControlValue |= ( ANewValue << 4 );
			}

			else
			{
				ATerminalControlValue &= 0xFFF0;
				ATerminalControlValue |= ANewValue;
			}

			C_OWNER.SPI_write( 4, ATerminalControlValue );
		}

		void SendOutput() // ( T_OWNER *AOwner, uint8_t AIndex )
		{
			if( ! FValueModified() )
				return;

			FValueModified() = false;

			int wiper_pos = ( InitialValue() * C_RESOLUTION );

			wiper_pos = constrain( wiper_pos, 0, C_RESOLUTION );

			uint8_t data_byte;
			uint8_t cmd_byte = C_INDEX;

			// Calculate the 9-bit data value to send
			data_byte = uint8_t(wiper_pos & 0x00FF);
			if( wiper_pos > C_RESOLUTION - 1 )
				if( C_RESOLUTION == 255 )
					cmd_byte |= B00000001; // Table 5-1 (page 36)

			C_OWNER.SPI_write( cmd_byte, data_byte );

			if( NonVolatile() )
			{
				// EEPROM write cycles take 4ms each. So we block with delay(5); after any NV Writes
				C_OWNER.SPI_write( cmd_byte | B10, data_byte );
				delay(5);
			}
		}

	public:
		inline PotentiometerSPIChannel()
		{
			FConfigModified() = false;
			FValueModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		uint8_t C_COUNT_CHANNELS,
		typename T_Channels_InitChannel,
		typename T_Channels_SendOutput,
		typename T_ChipSelectOutputPin,
		typename T_Enabled
	> class Potentiometer_SPI :
		public T_ChipSelectOutputPin,
		public T_Enabled
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( Enabled )

//	public:
//		PotentiometerSPIChannel<Potentiometer_SPI<T_SPI, C_SPI, C_COUNT_CHANNELS, C_RESOLUTION>, C_RESOLUTION>	Channels[ C_COUNT_CHANNELS ];

	protected:
		static const uint8_t kADR_WIPER0       = B00000000;
		static const uint8_t kADR_WIPER1       = B00010000;

		static const uint8_t kCMD_READ         = B00001100;
	    static const uint8_t kCMD_WRITE        = B00000000;

	    static const uint8_t kADR_VOLATILE     = B00000000;
		static const uint8_t kADR_NON_VOLATILE = B00100000;

		static const uint8_t kTCON_REGISTER    = B01000000;
		static const uint8_t kSTATUS_REGISTER  = B01010000;

	protected:
		inline uint16_t byte2uint16( uint8_t high_byte, uint8_t low_byte )
		{
			return (uint16_t)high_byte<<8 | (uint16_t)low_byte;
		}

	public:
		inline void UpdateEnabled()
		{
			T_Channels_InitChannel::Call();
		}

	public:
		inline void ClockInputPin_o_Receive( void * )
		{
			T_Channels_SendOutput::Call();
		}

	public:
		void SPI_write( uint8_t cmd_byte, uint8_t data_byte )
		{
			cmd_byte |= kCMD_WRITE;
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer(cmd_byte);
			C_SPI.transfer(data_byte);
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		uint16_t SPI_read( uint8_t cmd_byte )
		{
			cmd_byte |= kCMD_READ;
			T_ChipSelectOutputPin::SetPinValueLow();
			uint8_t high_byte = C_SPI.transfer(cmd_byte);
			uint8_t low_byte  = C_SPI.transfer(0xFF);
			T_ChipSelectOutputPin::SetPinValueHigh();
			return byte2uint16(high_byte, low_byte);
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif