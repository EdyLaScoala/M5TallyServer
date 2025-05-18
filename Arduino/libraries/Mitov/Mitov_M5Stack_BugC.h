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
	namespace M5StackBugC
	{
		const uint8_t	ADDRESS			= 0x38;
		const uint8_t	LED_REG_ADDRESS	= 0x10;
	}
//---------------------------------------------------------------------------
	class M5StackBugCI2C
	{
	public:
		constexpr static inline uint8_t GetLEDsAddress() { return M5StackBugC::ADDRESS; }
		constexpr static inline uint8_t GetLEDsRegister() { return M5StackBugC::LED_REG_ADDRESS; }

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		int C_COEFFICIENT,
		typename T_Enabled,
		uint8_t	C_INDEX,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class M5StackBugCIndexedMotorElement : 
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue() == ASpeed )
				return;


			InitialValue() = ASpeed;
			UpdateOutputs();
		}

	public:
		void UpdateOutputs()
		{
			Wire.beginTransmission( M5StackBugC::ADDRESS );
			Wire.write( C_INDEX );

			int8_t AValue;

			if( Enabled() )
				AValue = ( InitialValue().GetValue() - 0.5 ) * C_COEFFICIENT;

			else
				AValue = 0;

			Wire.write( AValue );
			Wire.endTransmission();

//			Serial.println( C_INDEX );
//			Serial.println( AValue );
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for L298NDualMotorDriverPWMChannel_Slopped compatibility

	};
//---------------------------------------------------------------------------	
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		int C_COEFFICIENT,
		typename T_Enabled,
		uint8_t	C_INDEX,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class M5StackBugCIndexedMotorElement_Slopped : 
		public T_Enabled,
		public T_InitialValue,
		public T_SlopeRun,
		public T_SlopeStop
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( SlopeRun )
		_V_PROP_( SlopeStop )

	protected:
		float	FCurrentValue = 0.0f;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
			UpdateOutputs();
		}

	public:
		void UpdateOutputs()
		{
			Wire.beginTransmission( M5StackBugC::ADDRESS );
			Wire.write( C_INDEX );

			int8_t AValue = ( FCurrentValue - 0.5 ) * C_COEFFICIENT;

			Wire.write( AValue );
			Wire.endTransmission();
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();

			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros )
		{
			float ATargetValue = ( Enabled() ) ? InitialValue() : 0.5;

			if( FCurrentValue != ATargetValue )
			{
				float ASlope = SlopeRun();
				if( FCurrentValue > ATargetValue )
				{
					if( ATargetValue > 0.5 )
						ASlope = SlopeStop();
				}

				else
				{
					if( ATargetValue < 0.5 )
						ASlope = SlopeStop();
				}

				if( ASlope == 0 )
					FCurrentValue = ATargetValue;

				else
				{
					float ARamp = fabs( AElapsedMicros * ASlope / 1000000 );
					if( FCurrentValue < ATargetValue )
					{
						FCurrentValue += ARamp;
						if( FCurrentValue > ATargetValue )
							FCurrentValue = ATargetValue;

					}
					else
					{
						FCurrentValue -= ARamp;
						if( FCurrentValue < ATargetValue )
							FCurrentValue = ATargetValue;

					}
				}

				UpdateOutputs();
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

