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
#include <Mitov_ComplementaryFilter.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin,
		typename T_RateProcessNoise,
		typename T_ValueObservedNoise,
		typename T_ValueProcessNoise
	> class KalmanAnalog :
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin,
		public T_RateProcessNoise,
		public T_ValueObservedNoise,
		public T_ValueProcessNoise
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ValueProcessNoise )
		_V_PROP_( RateProcessNoise )
		_V_PROP_( ValueObservedNoise )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FHasTime )
		unsigned long	FLastTime = 0;
		float x_bias = 0;
		float P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;	
		float FRateValue = 0;
		float FValue = 0;
		float FOldValue = 0;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			FValue = *(float *)_Data;
			if( ! Enabled() )
			{
				FOldValue = FValue;
				T_OutputPin::SetPinValue( FValue );
			}

		}

		inline void RateInputPin_o_Receive( void *_Data )
		{
			FRateValue = *(float *)_Data;
		}

	public:
		inline void SystemLoopEnd()
		{
			unsigned long ATime = micros();
			if( ! FHasTime() )			
			{
				FHasTime() = true;
				FLastTime = ATime;
				FOldValue = FValue;
				T_OutputPin::SetPinValue( FValue );
				return;
			}

			if( ! Enabled() )
				return;

			float dt = ( ATime - FLastTime ) / 1000000.0f;
			FLastTime = ATime;

			float x_angle = FOldValue;

			x_angle += dt * ( FRateValue - x_bias );

			P_00 +=  - dt * (P_10 + P_01) + ValueProcessNoise() * dt;
			P_01 +=  - dt * P_11;
			P_10 +=  - dt * P_11;
			P_11 +=  + RateProcessNoise() * dt;
    
			float y = FValue - x_angle;
			float S = P_00 + ValueObservedNoise();
			float K_0 = P_00 / S;
			float K_1 = P_10 / S;
    
			x_angle +=  K_0 * y;
			x_bias  +=  K_1 * y;
			P_00 -= K_0 * P_00;
			P_01 -= K_0 * P_01;
			P_10 -= K_1 * P_00;
			P_11 -= K_1 * P_01;

			FOldValue = x_angle;
			T_OutputPin::SetPinValue( x_angle );
		}

	public:
		inline KalmanAnalog()
		{
			FHasTime() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin,
		typename T_RateProcessNoise,
		typename T_Units,
		typename T_ValueObservedNoise,
		typename T_ValueProcessNoise
	> class KalmanAngle :
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin,
		public T_RateProcessNoise,
		public T_Units,
		public T_ValueObservedNoise,
		public T_ValueProcessNoise
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ValueProcessNoise )
		_V_PROP_( RateProcessNoise )
		_V_PROP_( ValueObservedNoise )

		_V_PROP_( Units )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FHasTime )
		unsigned long	FLastTime = 0;
		float x_bias = 0;
		float P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;	
		float FRateValue = 0;
		float FValue = 0;
		float FOldValue = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FValue = *(float *)_Data;
			if( ! Enabled() )
			{
				FOldValue = FValue;
				T_OutputPin::SetPinValue( FValue );
			}

		}

		inline void RateInputPin_o_Receive( void *_Data )
		{
			FRateValue = *(float *)_Data;
		}

	public:
		inline void SystemLoopEnd()
		{
			unsigned long ATime = micros();
			if( ! FHasTime() )
			{
				FHasTime() = true;
				FLastTime = ATime;
				FOldValue = FValue;
				T_OutputPin::SetPinValue( FValue );
				return;
			}

			if( ! Enabled() )
				return;

			float dt = ( ATime - FLastTime ) / 1000000.0f;
			FLastTime = ATime;

			float x_angle = FOldValue;

			x_angle += dt * ( FRateValue - x_bias );

			P_00 +=  - dt * (P_10 + P_01) + ValueProcessNoise() * dt;
			P_01 +=  - dt * P_11;
			P_10 +=  - dt * P_11;
			P_11 +=  + RateProcessNoise() * dt;
    
			float y = FValue - x_angle;
			float S = P_00 + ValueObservedNoise();
			float K_0 = P_00 / S;
			float K_1 = P_10 / S;
    
			x_angle +=  K_0 * y;
			x_bias  +=  K_1 * y;
			P_00 -= K_0 * P_00;
			P_01 -= K_0 * P_01;
			P_10 -= K_1 * P_00;
			P_11 -= K_1 * P_01;

			switch( Units() )
			{
				case auDegree:
					x_angle = posmod( x_angle, 360.0f );
					break;

				case auRadians:
					x_angle = posmod( x_angle, 2 * PI );
					break;

				case auNormalized:
					x_angle = posmod( x_angle, 1.0f );
					break;

			}

			FOldValue = x_angle;
			T_OutputPin::SetPinValue( x_angle );
		}

	public:
		inline KalmanAngle()
		{
			FHasTime() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif