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

// #define __LPS22HB__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace LPS22HBConst
	{
		const uint8_t INTERRUPT_CFG	= 0x0B;
		const uint8_t THS_P_L       = 0x0C;
		const uint8_t THS_P_H       = 0x0D;
		const uint8_t WHO_AM_I      = 0x0F;
		const uint8_t CTRL_REG1     = 0x10;
		const uint8_t CTRL_REG2     = 0x11;
		const uint8_t CTRL_REG3     = 0x12;
		const uint8_t FIFO_CTRL     = 0x14;
		const uint8_t REF_P_XL		= 0x15;
		const uint8_t REF_P_L		= 0x16;
		const uint8_t REF_P_H		= 0x17;
		const uint8_t RPDS_L		= 0x18;
		const uint8_t RPDS_H		= 0x19;
		const uint8_t RES_CONF		= 0x1A;
		const uint8_t INT_SOURCE	= 0x25;
		const uint8_t FIFO_STATUS	= 0x26;
		const uint8_t STATUS		= 0x27;
		const uint8_t PRESS_OUT_XL	= 0x28;
		const uint8_t PRESS_OUT_L	= 0x29;
		const uint8_t PRESS_OUT_H	= 0x2A;
		const uint8_t TEMP_OUT_L	= 0x2B;
		const uint8_t TEMP_OUT_H	= 0x2C;
		const uint8_t LPFP_RES		= 0x33;
	}
//---------------------------------------------------------------------------
	template <
		typename T_CutoffFrequencyDivider,
		typename T_Enabled
	> class TArduinoLSM9DS1AccelerometerFilter :
		public T_CutoffFrequencyDivider,
		public T_Enabled
	{
	public:
		_V_PROP_( CutoffFrequencyDivider )
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_LowPassFilter,
		typename T_PressureOutputPin,
		typename T_SampleRate,
		typename T_SingleShot,
		typename T_TemperatureOutputPin
	> class PressureLPS22HB : 
		public T_IMPLEMENTATION,
		public T_Enabled,
		public T_LowPassFilter,
		public T_InFahrenheit,
		public T_PressureOutputPin,
		public T_SampleRate,
		public T_SingleShot,
		public T_TemperatureOutputPin
	{
	public:
		_V_PIN_( PressureOutputPin )
		_V_PIN_( TemperatureOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( LowPassFilter )
		_V_PROP_( InFahrenheit )
		_V_PROP_( SampleRate )
		_V_PROP_( SingleShot )

	public:
		void Update_CTRL_REG1()
		{
			uint8_t AValue = ( LowPassFilter().Enabled() ) ? 0b00001000 : 0;

			if( ! SingleShot().GetValue() )
			{
				if( SampleRate().GetValue() > 50.0 )
					AValue |= 0b01010000;

				else if( SampleRate().GetValue() > 25.0 )
					AValue |= 0b01000000;

				else if( SampleRate().GetValue() > 10.0 )
					AValue |= 0b00110000;

				else if( SampleRate().GetValue() > 1.0 )
					AValue |= 0b00100000;

				else
					AValue |= 0b00010000;
			}

			if( LowPassFilter().CutoffFrequencyDivider().GetValue() > 9 )
				AValue |= 0b00001100;

			else if( LowPassFilter().CutoffFrequencyDivider().GetValue() > 2 )
				AValue |= 0b00001000;

//			Serial.println( AValue, BIN );

			T_IMPLEMENTATION::WriteRegister8( LPS22HBConst::CTRL_REG1, AValue );
		}

	protected:
		void ReadSensors( bool AClocked, bool AStartNextReading )
		{
			if( ! Enabled().GetValue() )
				return;

			uint8_t AStaus = T_IMPLEMENTATION::ReadRegister8( LPS22HBConst::STATUS );

//			Serial.println( AStaus, BIN );

			bool AStartNext = false;

			if( T_TemperatureOutputPin::GetPinIsConnected() )
				if( ( AStaus & 0b00000010 ) || AClocked )
				{
					float AValue = float( uint32_t( T_IMPLEMENTATION::ReadRegister8( LPS22HBConst::TEMP_OUT_L )) |
							( uint32_t( T_IMPLEMENTATION::ReadRegister8( LPS22HBConst::TEMP_OUT_H )) << 8 )) / 100.0;

					T_TemperatureOutputPin::SetPinValue( AValue );
					AStartNext = true;
				}

			if( T_PressureOutputPin::GetPinIsConnected() )
				if( ( AStaus & 0b00000001 ) || AClocked )
				{
					float AValue = float(T_IMPLEMENTATION::ReadRegister8( LPS22HBConst::PRESS_OUT_XL ) |
							( uint32_t( T_IMPLEMENTATION::ReadRegister8( LPS22HBConst::PRESS_OUT_L )) << 8) | 
							( uint32_t( T_IMPLEMENTATION::ReadRegister8( LPS22HBConst::PRESS_OUT_H )) << 16)) * 1000.0 / 40960.0;

					T_PressureOutputPin::SetPinValue( AValue );
					AStartNext = true;
				}

			if( AStartNextReading )
				if( AStartNext )
					if( SingleShot() )
						T_IMPLEMENTATION::WriteRegister8( LPS22HBConst::CTRL_REG2, 0b00000001 );
	
		}

		void Reset()
		{
			T_IMPLEMENTATION::WriteRegister8( LPS22HBConst::CTRL_REG2, 0b00000100 );
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			SystemStartClock();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( SingleShot() )
				T_IMPLEMENTATION::WriteRegister8( LPS22HBConst::CTRL_REG2, 0b00000001 );

			else
				ReadSensors( true, false );

		}

	public:
		inline void SystemStartClock()
		{
			Reset();
			Update_CTRL_REG1();
			T_IMPLEMENTATION::WriteRegister8( LPS22HBConst::CTRL_REG2, 0 );
		}

		inline void SystemStart()
		{
			SystemStartClock();
			ReadSensors( false, true );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( false, true );
		}

		inline void SystemLoopBeginClock()
		{
			if( SingleShot() )
				ReadSensors( false, false );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

