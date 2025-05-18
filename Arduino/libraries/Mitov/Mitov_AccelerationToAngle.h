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
	class TArduinoAccelerationToAngleOffsets
	{
	public:
		float	X = 0;
		float	Y = 0;
		float	Z = 0;

	};
//---------------------------------------------------------------------------
	template <
		typename T_FChangeOnly,
		typename T_FModified,
		typename T_ForceOutputPin,
		typename T_Offsets,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Units
	> class AccelerationToAngle :
		public T_FChangeOnly,
		public T_FModified,
		public T_ForceOutputPin,
		public T_Offsets,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Units
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

		_V_PIN_( Offsets )

		_V_PIN_( ForceOutputPin )

		_V_PIN_( Units )

	protected:
		_V_PIN_( FChangeOnly )
		_V_PIN_( FModified )

		float	FValues[ 3 ] = { 0 };

	protected:
		float ComputeAngle( float a, float b, float AOffset	)
		{
			// Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
			// Calculate heading when the magnetometer is level, then correct for signs of axis.
			float AHeading = atan2( a, b ); // Y , X

//			Serial.println( AHeading );

			// Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
			// Find yours here: http://www.magnetic-declination.com/
			// If you cannot find your Declination, comment out these two lines, your compass will be slightly off.

			// Correct for when signs are reversed.
			switch( Units() )
			{
				case auDegree:
					AHeading += AOffset * PI / 180.0f;
					break;

				case auNormalized:
					AHeading += AOffset * 2 * PI;
					break;

				default:
					AHeading += AOffset;
			}
//			if( Units == auDegree )
//				AHeading += AOffset * MITOV_PI / 180;

//			else

			AHeading = posmod( AHeading, 2*PI );
/*
			if(AHeading < 0)
				AHeading += 2*PI;
    
			// Check for wrap due to addition of declination.
			if(AHeading > 2*PI)
				AHeading -= 2*PI;
*/
			switch( Units() )
			{
				case auDegree:
					AHeading *= 180.0f / PI;
					break;

				case auNormalized:
					AHeading /= 2 * PI;
					break;

			}

			return AHeading;
		}

	public:
		inline void SystemLoopEnd()
		{
			if( ! FModified() )
				return;

			float AValue;

			AValue = ComputeAngle( FValues[ 2 ], FValues[ 1 ], Offsets().X() );
			T_OutputPinsX::SetPinValue( AValue, FChangeOnly() );

			AValue = ComputeAngle( FValues[ 2 ], FValues[ 0 ], Offsets().Y() );
			T_OutputPinsY::SetPinValue( AValue, FChangeOnly() );

			AValue = ComputeAngle( FValues[ 1 ], FValues[ 0 ], Offsets().Z() );
			T_OutputPinsZ::SetPinValue( AValue, FChangeOnly() );

			AValue = sqrt( FValues[ 0 ] * FValues[ 0 ] + FValues[ 1 ] * FValues[ 1 ] + FValues[ 2 ] * FValues[ 2 ] );
			T_ForceOutputPin::SetPinValue( AValue, FChangeOnly() );

			FChangeOnly() = true;
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FValues[ AIndex ] = *(float *)_Data;
			FModified() = true;
		}

	public:
		AccelerationToAngle()
		{
			FChangeOnly() = false;
			FModified() = true;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif