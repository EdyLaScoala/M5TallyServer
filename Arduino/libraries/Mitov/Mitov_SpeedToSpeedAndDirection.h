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
		typename T_DirectionOutputPin,
		typename T_Enabled,
		typename T_InverseForward,
		typename T_InverseReverse,
		typename T_SpeedOutputPin
	> class SpeedToSpeedAndDirection :
		public T_DirectionOutputPin,
		public T_Enabled,
		public T_InverseForward,
		public T_InverseReverse,
		public T_SpeedOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPin )
		_V_PIN_( SpeedOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InverseForward )
		_V_PROP_( InverseReverse )

	public:
		void UpdateOutputs()
		{
			if( Enabled() )
			{
				float AOutSpeed = fabs( FCurrentSpeed - 0.5 ) * 2;
				bool ADirection = FCurrentSpeed > 0.5;

				if( ADirection )
				{
					if( InverseForward() )
						AOutSpeed = 1 - AOutSpeed;
				}

				else
				{
					if( InverseReverse() )
						AOutSpeed = 1 - AOutSpeed;
				}

				T_SpeedOutputPin::SetPinValue( AOutSpeed );
				T_DirectionOutputPin::SetPinValue( ADirection );
			}

			else
			{
				T_SpeedOutputPin::SetPinValue( 0 );
				T_DirectionOutputPin::SetPinValue( false );
			}
		}

	protected:
		float	FCurrentSpeed = 0.5f;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( FCurrentSpeed == ASpeed )
				return;

			FCurrentSpeed = ASpeed;
			UpdateOutputs();
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialReverse,
		typename T_InitialSpeed,
		typename T_OutputPin
	> class SpeedAndDirectionToSpeed :
		public T_Enabled,
		public T_InitialReverse,
		public T_InitialSpeed,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialReverse )
		_V_PROP_( InitialSpeed )

	protected:
		void SendOutput( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			float ASpeed = InitialSpeed().GetValue() / 2;

			if( InitialReverse() )
				ASpeed = 0.5 - ASpeed;

			else
				ASpeed += 0.5;

			OutputPin().SetPinValue( ASpeed, AChangeOnly );
		}

	public:
		inline void SpeedInputPin_o_Receive( void *_Data )
		{
			float AValue = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( InitialSpeed().GetValue() == AValue )
				return;

			InitialSpeed() = AValue;
			SendOutput( true );
		}

		inline void ReverseInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( InitialReverse().GetValue() == AValue )
				return;

			InitialReverse() = AValue;
			SendOutput( true );
		}

	public:
		inline void SystemStart()
		{
			SendOutput( false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif