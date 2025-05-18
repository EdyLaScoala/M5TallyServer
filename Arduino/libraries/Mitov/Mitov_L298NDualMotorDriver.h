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
	template <
		typename T_Enabled,
		typename T_ForwardOutputPin,
		typename T_InitialValue,
		typename T_ReverseOutputPin,
		typename T_SlopeRun,
		typename T_SlopeStop,
		typename T_SpeedOutputPin
	> class L298NDualMotorDriverChannel :
		public T_Enabled,
		public T_ForwardOutputPin,
		public T_InitialValue,
		public T_ReverseOutputPin,
//		public T_SlopeRun,
//		public T_SlopeStop,
		public T_SpeedOutputPin
	{
	public:
		_V_PIN_( ForwardOutputPin )
		_V_PIN_( ReverseOutputPin )
		_V_PIN_( SpeedOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue().GetValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
			UpdateOutputs();
		}

	public:
		void UpdateOutputs()
		{
			if( Enabled() )
			{
				float AInitialValue = InitialValue().GetValue();
				float AOutSpeed = fabs( AInitialValue - 0.5 ) * 2;
				bool ADirection = AInitialValue > 0.5;

				T_ReverseOutputPin::SetPinValue( ADirection );
				ADirection = ! ADirection;

				T_ForwardOutputPin::SetPinValue( ADirection );
				T_SpeedOutputPin::SetPinValue( AOutSpeed );
			}

			else
			{
				T_SpeedOutputPin::SetPinValue( 0.5 );
				T_ForwardOutputPin::SetPinValue( false );
				T_ReverseOutputPin::SetPinValue( false );
			}
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for *_Slopped compatibility

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_ForwardOutputPin,
		typename T_InitialValue,
		typename T_ReverseOutputPin,
		typename T_SlopeRun,
		typename T_SlopeStop,
		typename T_SpeedOutputPin
	> class L298NDualMotorDriverChannel_Slopped :
		public T_Enabled,
		public T_ForwardOutputPin,
		public T_InitialValue,
		public T_ReverseOutputPin,
		public T_SlopeRun,
		public T_SlopeStop,
		public T_SpeedOutputPin
	{
	public:
		_V_PIN_( ForwardOutputPin )
		_V_PIN_( ReverseOutputPin )
		_V_PIN_( SpeedOutputPin )

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
		void	UpdateOutputs()
		{
			if( FCurrentValue != 0.5 )
			{
				float AOutSpeed = fabs( FCurrentValue - 0.5 ) * 2;
				bool ADirection = FCurrentValue > 0.5;

//				ReverseOutputPin.Notify( &ADirection );
				T_ReverseOutputPin::SetPinValue( ADirection );
				ADirection = ! ADirection;
//				ForwardOutputPin.Notify( &ADirection );
				T_ForwardOutputPin::SetPinValue( ADirection );

//				SpeedOutputPin.Notify( &AOutSpeed );
				T_SpeedOutputPin::SetPinValue( AOutSpeed );
			}

			else
			{
//				SpeedOutputPin.SendValue( 0.5 );
				T_SpeedOutputPin::SetPinValue( 0.5 );
//				ForwardOutputPin.SendValue( false );
				T_ForwardOutputPin::SetPinValue( false );
//				ReverseOutputPin.SendValue( false );
				T_ReverseOutputPin::SetPinValue( false );
			}
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
	template <
		typename T_Enabled,
		typename T_InitialValue,
		int PIN_ENABLE,
		int PIN_FORWARD,
		int PIN_REVERSE,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class MotorShieldChannel :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )

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
		void	UpdateOutputs()
		{
			bool AForward;
			bool AReverse;
			float AOutSpeed;
			if( Enabled() && ( InitialValue() != 0.5 ))
			{
				AOutSpeed = fabs( InitialValue() - 0.5 ) * 2;
				bool ADirection = InitialValue() > 0.5;

				AReverse = ADirection;
				AForward = ! ADirection;
			}

			else
			{
				AOutSpeed = 0;
				AReverse = false;
				AForward = false;
			}

			digitalWrite( PIN_FORWARD, AForward ? HIGH : LOW );
			digitalWrite( PIN_REVERSE, AReverse ? HIGH : LOW );
			Analog.Write( PIN_ENABLE, AOutSpeed );
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for *_Slopped compatibility

	public:
		MotorShieldChannel()
		{
			pinMode( PIN_ENABLE, OUTPUT );
			pinMode( PIN_FORWARD, OUTPUT );
			pinMode( PIN_REVERSE, OUTPUT );
		}
	};

//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		int PIN_ENABLE,
		int PIN_FORWARD,
		int PIN_REVERSE,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class MotorShieldChannel_Slopped :
		public T_Enabled,
		public T_InitialValue,
		public T_SlopeRun,
		public T_SlopeStop
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )
		_V_PROP_( SlopeRun )
		_V_PROP_( SlopeStop )

	protected:
		float	FCurrentValue = 0.0f;
		unsigned long	FLastTime = 0;

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
		void	UpdateOutputs()
		{
			bool AForward;
			bool AReverse;
			float AOutSpeed;
			if( FCurrentValue != 0.5 )
			{
				AOutSpeed = fabs( FCurrentValue - 0.5 ) * 2;
				bool ADirection = FCurrentValue > 0.5;

				AReverse = ADirection;
				AForward = ! ADirection;
			}

			else
			{
				AOutSpeed = 0;
				AReverse = false;
				AForward = false;
			}

			digitalWrite( PIN_FORWARD, AForward ? HIGH : LOW );
			digitalWrite( PIN_REVERSE, AReverse ? HIGH : LOW );
			Analog.Write( PIN_ENABLE, AOutSpeed );
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

	public:
		MotorShieldChannel_Slopped()
		{
			pinMode( PIN_FORWARD, OUTPUT );
			pinMode( PIN_REVERSE, OUTPUT );
			pinMode( PIN_ENABLE, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif