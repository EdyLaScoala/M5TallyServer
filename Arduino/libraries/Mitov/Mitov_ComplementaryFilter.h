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
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin,
		typename T_TimeConstant
	> class ComplementaryAnalog :
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin,
		public T_TimeConstant
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( TimeConstant )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FHasTime )

		unsigned long	FLastTime = 0;
		float			FValue = 0;
		float			FOutValue = 0;
		float			FRateValue = 0;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			FValue = *(float *)_Data;
			if( ! Enabled() )
			{
				FOutValue = *(float *)_Data;
				T_OutputPin::SetPinValue( *(float *)_Data );
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
				FOutValue = FValue;
				T_OutputPin::SetPinValue( FValue );
				return;
			}

			if( ! Enabled() )
				return;

			float dT = ( ATime - FLastTime ) / 1000000.0f;
			FLastTime = ATime;

			float a = TimeConstant() / ( TimeConstant() + dT );

			float AAngle = a * ( FOutValue + FRateValue * dT) + (1-a) * FValue;

			FOutValue = AAngle;
			T_OutputPin::SetPinValue( AAngle );
		}

	public:
		ComplementaryAnalog()
		{
			FHasTime() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin,
		typename T_TimeConstant,
		typename T_Units
	> class ComplementaryAngle :
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin,
		public T_TimeConstant,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( TimeConstant )
		_V_PROP_( Enabled )
		_V_PROP_( Units )

	protected:
		_V_PROP_( FHasTime )

		unsigned long	FLastTime = 0;
		float			FValue = 0;
		float			FOutValue = 0;
		float			FRateValue = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FValue = *(float *)_Data;
			if( ! Enabled() )
			{
				FOutValue = *(float *)_Data;
				T_OutputPin::SetPinValue( *(float *)_Data );
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
				FOutValue = FValue;
				T_OutputPin::SetPinValue( FValue );
				return;
			}

			if( ! Enabled() )
				return;

			float dT = ( ATime - FLastTime ) / 1000000.0f;
			FLastTime = ATime;

			float a = TimeConstant() / ( TimeConstant() + dT );

			float AAngle = a * ( FOutValue + FRateValue * dT ) + (1-a) * FValue;

			switch( Units() )
			{
				case auDegree:
					AAngle = posmod( AAngle, 360.0f );
					break;

				case auRadians:
					AAngle = posmod( AAngle, 2 * PI );
					break;

				case auNormalized:
					AAngle = posmod( AAngle, 1.0f );
					break;

			}

			FOutValue = AAngle;
			T_OutputPin::SetPinValue( AAngle );
		}

	public:
		ComplementaryAngle()
		{
			FHasTime() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Coefficient,
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin
	> class Complementary2 :
		public T_Coefficient,
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Coefficient )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FHasTime )

		unsigned long	FLastTime = 0;
		float			FValue = 0;
		float			FOutValue = 0;
		float			FRateValue = 0;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			FValue = *(float *)_Data;
			if( ! Enabled() )
			{
				FOutValue = *(float *)_Data;
				T_OutputPin::SetPinValue( *(float *)_Data );
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
				FOutValue = FValue;
				T_OutputPin::SetPinValue( FValue );
				return;
			}

			if( ! Enabled() )
				return;

			float dT = ( ATime - FLastTime ) / 1000000.0f;
			FLastTime = ATime;

			float x_angle2C = FOutValue;
			float newAngle = FValue;

			float x1 = (newAngle -   x_angle2C) * Coefficient() * Coefficient();
			float y1 = dT * x1 + y1;
			float x2 = y1 + (newAngle -   x_angle2C) * 2 * Coefficient() + FRateValue;
			x_angle2C = dT * x2 + x_angle2C;

			FOutValue = x_angle2C;
			T_OutputPin::SetPinValue( x_angle2C );
		}

	public:
		Complementary2()
		{
			FHasTime() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Coefficient,
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin,
		typename T_Units
	> class ComplementaryAngle2 :
		public T_Coefficient,
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Coefficient )
		_V_PROP_( Enabled )
		_V_PROP_( Units )

	protected:
		_V_PROP_( FHasTime )
		unsigned long	FLastTime = 0;
		float			FValue = 0;
		float			FOutValue = 0;
		float			FRateValue = 0;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			FValue = *(float *)_Data;
			if( ! Enabled() )
			{
				FOutValue = *(float *)_Data;
				T_OutputPin::SetPinValue( *(float *)_Data );
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
				FOutValue = FValue;
				T_OutputPin::SetPinValue( FValue );
				return;
			}

			if( ! Enabled() )
				return;

			float dT = ( ATime - FLastTime ) / 1000000.0f;
			FLastTime = ATime;

			float x_angle2C = FOutValue;
			float newAngle = FValue;

			float x1 = (newAngle -   x_angle2C) * Coefficient() * Coefficient();
			float y1 = dT * x1 + y1;
			float x2 = y1 + (newAngle -   x_angle2C) * 2 * Coefficient() + FRateValue;
			x_angle2C = dT * x2 + x_angle2C;

			switch( Units() )
			{
				case auDegree:
					x_angle2C = posmod( x_angle2C, 360.0f );
					break;

				case auRadians:
					x_angle2C = posmod( x_angle2C, 2 * PI );
					break;

				case auNormalized:
					x_angle2C = posmod( x_angle2C, 1.0f );
					break;

			}

			FOutValue = x_angle2C;
			T_OutputPin::SetPinValue( x_angle2C );
		}

	public:
		ComplementaryAngle2()
		{
			FHasTime() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif