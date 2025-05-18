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

// #define __LIS3MDL__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace LIS3MDLConst
	{
		const uint8_t CTRL_REG1_M        = 0x20;
		const uint8_t CTRL_REG2_M        = 0x21;
		const uint8_t CTRL_REG3_M        = 0x22;
		const uint8_t CTRL_REG4_M        = 0x23;
		const uint8_t STATUS_REG_M       = 0x27;
		const uint8_t OUT_X_L_M          = 0x28;
	}
//---------------------------------------------------------------------------
	namespace TArduinoLIS3MDLCompassPerformanceMode
	{
		enum TArduinoLIS3MDLCompassPerformanceMode { Low, Medium, High, UltraHigh };
	}
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_Enabled,
		typename T_FastRate,
		typename T_FullScaleRange,
		typename T_LowPowerMode,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_PowerDown,
		typename T_SampleRate,
		typename T_SelfTest,
//		typename T_SingleShot,
		typename T_TemperatureCompensation,
		typename T_XYPerformanceMode,
		typename T_ZPerformanceMode
	> class LIS3MDL : 
		public T_IMPLEMENTATION,
		public T_Enabled,
		public T_FastRate,
		public T_FullScaleRange,
		public T_LowPowerMode,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_PowerDown,
		public T_SampleRate,
		public T_SelfTest,
//		public T_SingleShot,
		public T_TemperatureCompensation,
		public T_XYPerformanceMode,
		public T_ZPerformanceMode		
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( FastRate )
		_V_PROP_( FullScaleRange )
		_V_PROP_( LowPowerMode )
		_V_PROP_( PowerDown )
		_V_PROP_( SampleRate )
		_V_PROP_( SelfTest )
//		_V_PROP_( SingleShot )
		_V_PROP_( TemperatureCompensation )
		_V_PROP_( XYPerformanceMode )
		_V_PROP_( ZPerformanceMode )

	public:
		void Update_CTRL_REG1_M()
		{
			uint8_t AValue = ( TemperatureCompensation() ) ? 0b10000000 : 0;

			if( SelfTest() )
				AValue |= 0b00000001;

			if( FastRate() )
				AValue |= 0b00000010;

			switch( XYPerformanceMode() )
			{
//				case TArduinoLIS3MDLCompassPerformanceMode::Low	:
//					AValue |= 0b00000000;
//					break;

				case TArduinoLIS3MDLCompassPerformanceMode::Medium	:
					AValue |= 0b00100000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::High	:
					AValue |= 0b01000000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::UltraHigh	:
					AValue |= 0b01100000;
			}

			if( SampleRate().GetValue() > 40.0 )
				AValue |= 0b01110000;

			else if( SampleRate().GetValue() > 20.0 )
				AValue |= 0b01100000;

			else if( SampleRate().GetValue() > 10.0 )
				AValue |= 0b01010000;

			else if( SampleRate().GetValue() > 5.5 )
				AValue |= 0b01000000;

			else if( SampleRate().GetValue() > 2.5 )
				AValue |= 0b00110000;

			else if( SampleRate().GetValue() > 1.25 )
				AValue |= 0b00100000;

			else if( SampleRate().GetValue() > 0.625 )
				AValue |= 0b00010000;

			T_IMPLEMENTATION::WriteRegister8( LIS3MDLConst::CTRL_REG1_M, AValue );
		}

		void Update_CTRL_REG2_M()
		{
			uint8_t AValue;

			if( FullScaleRange().GetValue() > 12 )
				AValue |= 0b01100000;

			else if( FullScaleRange().GetValue() > 8 )
				AValue |= 0b01000000;

			else if( FullScaleRange().GetValue() > 8 )
				AValue |= 0b00100000;

			else
				AValue |= 0b00000000;

			T_IMPLEMENTATION::WriteRegister8( LIS3MDLConst::CTRL_REG2_M, AValue );
		}

		void Update_CTRL_REG3_M()
		{
			uint8_t AValue = ( LowPowerMode() ) ? 00100000 : 0;

			if( PowerDown() )
				AValue |= 0b00000010;

//			if( SingleShot() )
//				AValue |= 0b00000001;

			T_IMPLEMENTATION::WriteRegister8( LIS3MDLConst::CTRL_REG3_M, AValue );
		}

		void Update_CTRL_REG4_M()
		{
			uint8_t AValue;

			switch( ZPerformanceMode() )
			{
				case TArduinoLIS3MDLCompassPerformanceMode::Low	:
					AValue = 0b00000000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::Medium	:
					AValue = 0b00000100;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::High	:
					AValue = 0b00001000;
					break;

				case TArduinoLIS3MDLCompassPerformanceMode::UltraHigh	:
					AValue = 0b00001100;
			}

			T_IMPLEMENTATION::WriteRegister8( LIS3MDLConst::CTRL_REG4_M, AValue );
		}

	protected:
		void ResetCompass()
		{
			T_IMPLEMENTATION::WriteRegister8( LIS3MDLConst::CTRL_REG2_M, 0x0C );
		}

		void ReadSensorsCompass( bool AClocked )
		{
			if( Enabled() )
			{
				uint8_t ACompassStaus = T_IMPLEMENTATION::ReadRegister8( LIS3MDLConst::STATUS_REG_M );
//				Serial.println( ACompassStaus, BIN );
				if( ( ACompassStaus & 0b00001000 ) || AClocked )
				{
					T_IMPLEMENTATION::StartReadRegisters( LIS3MDLConst::OUT_X_L_M, 6 );
					int16_t AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					float AFloatValue = ( float( AValue ) / 0xFFFF ) * FullScaleRange().GetValue() * 2 * 100.0;

					OutputPinsX().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * FullScaleRange().GetValue() * 2 * 100.0;

					OutputPinsY().SetPinValue( AFloatValue );

					AValue = T_IMPLEMENTATION::ReadUInt8();
					AValue |= int16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8;

					AFloatValue = ( float( AValue ) / 0xFFFF ) * FullScaleRange().GetValue() * 2 * 100.0;

					OutputPinsZ().SetPinValue( AFloatValue );

					T_IMPLEMENTATION::EndReadRegisters();
				}
			}
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			ResetCompass();
			SystemStartCompassClock();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensorsCompass( true );
		}

	public:
		inline void SystemInit()
		{
			T_IMPLEMENTATION::Init();

			ResetCompass();

			delay( 10 );
		}

		inline void SystemStartCompassClock()
		{
			Update_CTRL_REG1_M();
			Update_CTRL_REG2_M();
			Update_CTRL_REG3_M();
			Update_CTRL_REG4_M();
		}

		inline void SystemStartCompass()
		{
			SystemStartCompassClock();
			ReadSensorsCompass( false );
		}

		inline void SystemLoopBeginCompass()
		{
			ReadSensorsCompass( false );

//			Serial.println( T_IMPLEMENTATION::ReadRegister16( 0x2E ) );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

