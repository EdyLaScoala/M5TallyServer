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
		typename T_DirectionOutputPin,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop,
		typename T_SpeedOutputPin
	> class L9110SDualMotorDriverChannel :
		public T_DirectionOutputPin,
		public T_Enabled,
		public T_InitialValue,
//		public T_SlopeRun,
//		public T_SlopeStop,
		public T_SpeedOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPin )
		_V_PIN_( SpeedOutputPin )

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
			if( Enabled() )
			{
				float AOutSpeed = fabs( InitialValue() - 0.5 ) * 2;
				bool ADirection = InitialValue() > 0.5;

				if( ADirection )
					AOutSpeed = 1 - AOutSpeed;

	//			SpeedOutputPin.Notify( &AOutSpeed );
	//			DirectionOutputPin.Notify( &ADirection );

				T_SpeedOutputPin::SetPinValue( AOutSpeed );
				T_DirectionOutputPin::SetPinValue( ADirection );
			}
			else
			{
//				SpeedOutputPin.SendValue( 0.5 );
//				DirectionOutputPin.SendValue( false );

				T_SpeedOutputPin::SetPinValue( 0.5 );
				T_DirectionOutputPin::SetPinValue( false );
			}
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for L9110SDualMotorDriverChannel_Slopped compatibility
	};
//---------------------------------------------------------------------------
	template <
		typename T_DirectionOutputPin,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop,
		typename T_SpeedOutputPin
	> class L9110SDualMotorDriverChannel_Slopped :
		public T_DirectionOutputPin,
		public T_Enabled,
		public T_InitialValue,
		public T_SlopeRun,
		public T_SlopeStop,
		public T_SpeedOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPin )
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

				if( ADirection )
					AOutSpeed = 1 - AOutSpeed;

//				SpeedOutputPin.Notify( &AOutSpeed );
//				DirectionOutputPin.Notify( &ADirection );

				T_SpeedOutputPin::SetPinValue( AOutSpeed );
				T_DirectionOutputPin::SetPinValue( ADirection );
			}

			else
			{
//				SpeedOutputPin.SendValue( 0.5 );
//				DirectionOutputPin.SendValue( false );

				T_SpeedOutputPin::SetPinValue( 0.5 );
				T_DirectionOutputPin::SetPinValue( false );
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

			unsigned long ACurrentMicros = micros();

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
					float ARamp = abs( AElapsedMicros * ASlope / 1000000 );
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
		typename T_ForwardOutputPin,
		typename T_InitialValue,
		typename T_ReverseOutputPin,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class L9110SDualMotorDriverPWMChannel : 
		public T_Enabled,
		public T_ForwardOutputPin,
		public T_InitialValue,
		public T_ReverseOutputPin
//		public T_SlopeRun,
//		public T_SlopeStop
	{
	public:
		_V_PIN_( ForwardOutputPin )
		_V_PIN_( ReverseOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
//		_V_PROP_( SlopeRun )
//		_V_PROP_( SlopeStop )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialValue() == ASpeed )
				return;


			InitialValue() = ASpeed;
//			Serial.println( InitialValue() );
			UpdateOutputs();
		}

	public:
		void UpdateOutputs()
		{
			if( Enabled() )
			{
				float AOutSpeed = fabs( InitialValue().GetValue() - 0.5 ) * 2;
//				Serial.println( AOutSpeed );
				if( InitialValue() > 0.5 )
				{
					T_ReverseOutputPin::SetPinValue( 0 );
					T_ForwardOutputPin::SetPinValue( AOutSpeed );
				}

				else
				{
					T_ForwardOutputPin::SetPinValue( 0 );
					T_ReverseOutputPin::SetPinValue( AOutSpeed );
				}
			}

			else
			{
				T_ForwardOutputPin::SetPinValue( 0 );
				T_ReverseOutputPin::SetPinValue( 0 );
			}
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

//		inline void SystemLoopBegin() {} // Placeholder for L298NDualMotorDriverPWMChannel_Slopped compatibility
		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for L298NDualMotorDriverPWMChannel_Slopped compatibility

	};
//---------------------------------------------------------------------------	
	template <
		typename T_Enabled,
		typename T_ForwardOutputPin,
		typename T_InitialValue,
		typename T_ReverseOutputPin,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class L9110SDualMotorDriverPWMChannel_Slopped : 
		public T_Enabled,
		public T_ForwardOutputPin,
		public T_InitialValue,
		public T_ReverseOutputPin,
		public T_SlopeRun,
		public T_SlopeStop
	{
	public:
		_V_PIN_( ForwardOutputPin )
		_V_PIN_( ReverseOutputPin )

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
				if( FCurrentValue > 0.5 )
				{
					T_ReverseOutputPin::SetPinValue( 0 );
					T_ForwardOutputPin::SetPinValue( AOutSpeed );
				}
				else
				{
					T_ForwardOutputPin::SetPinValue( 0 );
					T_ReverseOutputPin::SetPinValue( AOutSpeed );
				}
			}

			else
			{
				T_ForwardOutputPin::SetPinValue( 0 );
				T_ReverseOutputPin::SetPinValue( 0 );
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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif