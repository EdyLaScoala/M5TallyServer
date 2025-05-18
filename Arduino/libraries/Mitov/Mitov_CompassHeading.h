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
		typename T_DeclinationAngle,
		typename T_FChangeOnly,
		typename T_FModified,
		typename T_OutputPin,
		typename T_Units
	> class CompassHeading :
		public T_DeclinationAngle,
		public T_FChangeOnly,
		public T_FModified,
		public T_OutputPin,
		public T_Units
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( DeclinationAngle )
		_V_PROP_( Units )

	protected:
		_V_PROP_( FChangeOnly )
		_V_PROP_( FModified )
		float	FValues[ 2 ] = { 0 };

	public:
		inline void SystemLoopEnd() 
		{
			if( ! FModified() )
				return;

			// Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
			// Calculate heading when the magnetometer is level, then correct for signs of axis.
			float AHeading = atan2( FValues[ 1 ], FValues[ 0 ] ); // Y , X

//			Serial.println( AHeading );

			// Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
			// Find yours here: http://www.magnetic-declination.com/
			// If you cannot find your Declination, comment out these two lines, your compass will be slightly off.

			float	ADeclinationAngle;
			switch( Units() )
			{
				case auDegree:
					ADeclinationAngle = DeclinationAngle() * PI / 180;
					break;

				case auRadians:
					ADeclinationAngle = DeclinationAngle();
					break;

				case auNormalized:
					ADeclinationAngle = DeclinationAngle() * 2 * PI;
					break;

			}

			AHeading += ADeclinationAngle;

			// Correct for when signs are reversed.
			if(AHeading < 0)
				AHeading += 2*PI;
    
			// Check for wrap due to addition of declination.
			if(AHeading > 2*PI)
				AHeading -= 2*PI;

			switch( Units() )
			{
				case auDegree:
					AHeading *= 180.0f / PI;
					break;

				case auNormalized:
					AHeading /= 2 * PI;
					break;

			}

			T_OutputPin::SetPinValue( AHeading, FChangeOnly() );
			FChangeOnly() = true;
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FValues[ AIndex ] = *(float *)_Data;
			FModified() = true;
		}

	public:
		CompassHeading()
		{
			FChangeOnly() = false;
			FModified() = true;
		}
	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif