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
		typename T_MotorsOutputPins_Left,
		typename T_MotorsOutputPins_Right
	> class SteeringDifferential :
		public T_MotorsOutputPins_Left,
		public T_MotorsOutputPins_Right
	{
	public:
		_V_PIN_( MotorsOutputPins_Left )
		_V_PIN_( MotorsOutputPins_Right )

	protected:
//		float FDirection = 0.5f;
//		float FSpeed = 0.5f;

		float FSteeringValues[ 2 ] = { 0.5f, 0.5f };

	protected:
		void UpdateMotors()
		{
			float ADirOffset = FSteeringValues[ 0 ] - 0.5f;

			float ASpeed = FSteeringValues[ 1 ];

			float ALeftMotor = ASpeed + ADirOffset;
			float ARightMotor = ASpeed - ADirOffset;

			if( ALeftMotor > 1.0f )
			{
				ARightMotor -= ( ALeftMotor - 1 );
				ALeftMotor = 1.0f;
			}

			else if( ALeftMotor < 0.0f )
			{
				ARightMotor -= ALeftMotor;
				ALeftMotor = 0.0f;
			}

			else if( ARightMotor > 1.0 )
			{
				ALeftMotor -= ( ARightMotor - 1 );
				ARightMotor = 1.0f;
			}

			else if( ARightMotor < 0.0f )
			{
				ALeftMotor -= ARightMotor;
				ARightMotor = 0.0f;
			}

			T_MotorsOutputPins_Left::SetPinValue( ALeftMotor );
			T_MotorsOutputPins_Right::SetPinValue( ARightMotor );
		}

	public:
		void SteeringInputPins_o_Receive( int AIndex, void *_Data )
		{
			float AValue = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( FSteeringValues[ AIndex ] == AValue )
				return;

			FSteeringValues[ AIndex ] = AValue;
			UpdateMotors();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif