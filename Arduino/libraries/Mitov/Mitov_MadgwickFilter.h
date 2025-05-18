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
		typename T_FChangeOnly,
		typename T_FModified,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_ProportionalGain,
		typename T_Units
	> class MadgwickFilter :
		public T_FChangeOnly,
		public T_FModified,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_ProportionalGain,
		public T_Units
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	public:
		_V_PROP_( ProportionalGain )
		_V_PROP_( Units )

	protected:
		_V_PROP_( FChangeOnly )
		_V_PROP_( FModified )

		unsigned long FLastTime = 0;

	protected:
		float FAccelerometerValues[ 3 ];
		float FGyroscopeValues[ 3 ];
		float FCompassValues[ 3 ];

	protected:
//		float beta;				// algorithm gain
		float q0 = 1.0f;
		float q1 = 0.0f;
		float q2 = 0.0f;
		float q3 = 0.0f;	// quaternion of sensor frame relative to auxiliary frame

	public:
		void AccelerometerInputPins_o_Receive( int AIndex, void *_Data )
		{
			FAccelerometerValues[ AIndex ] = *(float *)_Data;
			FModified() = true;
		}

		void GyroscopeInputPins_o_Receive( int AIndex, void *_Data )
		{
			FGyroscopeValues[ AIndex ] = *(float *)_Data;
			FModified() = true;
		}

		void CompassInputPins_o_Receive( int AIndex, void *_Data )
		{
			FCompassValues[ AIndex ] = *(float *)_Data;
			FModified() = true;
		}

	protected:
		float invSqrt(float x) 
		{
			float halfx = 0.5f * x;
			float y = x;
			long i = *(long*)&y;
			i = 0x5f3759df - (i>>1);
			y = *(float*)&i;
			y = y * (1.5f - (halfx * y * y));
			y = y * (1.5f - (halfx * y * y));
			return y;
		}

		void computeAngles()
		{
			float roll = atan2f(q0*q1 + q2*q3, 0.5f - q1*q1 - q2*q2);
			float pitch = asinf(-2.0f * (q1*q3 - q0*q2));
			float yaw = atan2f(q1*q2 + q0*q3, 0.5f - q2*q2 - q3*q3);

			T_OutputPins_X::SetPinValue( roll, FChangeOnly() );
			T_OutputPins_Y::SetPinValue( pitch, FChangeOnly() );
			T_OutputPins_Z::SetPinValue( yaw, FChangeOnly() );
		}

		void updateIMU( float ADeltaTime, float gx, float gy, float gz )
		{
			// Rate of change of quaternion from gyroscope
			float qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
			float qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
			float qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
			float qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

			float ax = FAccelerometerValues[ 0 ];
			float ay = FAccelerometerValues[ 1 ];
			float az = FAccelerometerValues[ 2 ];

			// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
			if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) 
			{
				// Normalise accelerometer measurement
				float recipNorm = invSqrt(ax * ax + ay * ay + az * az);
				ax *= recipNorm;
				ay *= recipNorm;
				az *= recipNorm;

				// Auxiliary variables to avoid repeated arithmetic
				float _2q0 = 2.0f * q0;
				float _2q1 = 2.0f * q1;
				float _2q2 = 2.0f * q2;
				float _2q3 = 2.0f * q3;
				float _4q0 = 4.0f * q0;
				float _4q1 = 4.0f * q1;
				float _4q2 = 4.0f * q2;
				float _8q1 = 8.0f * q1;
				float _8q2 = 8.0f * q2;
				float q0q0 = q0 * q0;
				float q1q1 = q1 * q1;
				float q2q2 = q2 * q2;
				float q3q3 = q3 * q3;

				// Gradient decent algorithm corrective step
				float s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
				float s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
				float s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
				float s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
				recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
				s0 *= recipNorm;
				s1 *= recipNorm;
				s2 *= recipNorm;
				s3 *= recipNorm;

				float beta = ProportionalGain() * 2;
				// Apply feedback step
				qDot1 -= beta * s0;
				qDot2 -= beta * s1;
				qDot3 -= beta * s2;
				qDot4 -= beta * s3;
			}

			// Integrate rate of change of quaternion to yield quaternion
			q0 += qDot1 * ADeltaTime;
			q1 += qDot2 * ADeltaTime;
			q2 += qDot3 * ADeltaTime;
			q3 += qDot4 * ADeltaTime;

			// Normalise quaternion
			float recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
			q0 *= recipNorm;
			q1 *= recipNorm;
			q2 *= recipNorm;
			q3 *= recipNorm;

			computeAngles();
		}

	public:
		inline void SystemLoopEnd()
		{
			if( ! FModified() )
				return;

			unsigned long ANewTime = micros();

			float ADeltaTime = (ANewTime - FLastTime) / 1000000.0;

			FLastTime = ANewTime;

			float gx, gy, gz;

			switch( Units() )
			{
				case auRadians:
					gx = FGyroscopeValues[ 0 ];
					gy = FGyroscopeValues[ 1 ];
					gz = FGyroscopeValues[ 2 ];
					break;

				case auDegree:
					gx = FGyroscopeValues[ 0 ] * 0.0174533f;
					gy = FGyroscopeValues[ 1 ] * 0.0174533f;
					gz = FGyroscopeValues[ 2 ] * 0.0174533f;
					break;

				case auNormalized:
					gx = FGyroscopeValues[ 0 ] * 2 * PI;
					gy = FGyroscopeValues[ 1 ] * 2 * PI;
					gz = FGyroscopeValues[ 2 ] * 2 * PI;
					break;
			}

			// Convert gyroscope degrees/sec to radians/sec
//			gx *= 0.0174533f;
//			gy *= 0.0174533f;
//			gz *= 0.0174533f;

			// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
			if((FCompassValues[ 0 ] == 0.0f) && (FCompassValues[ 1 ] == 0.0f) && ( FCompassValues[ 2 ] == 0.0f)) 
			{
				updateIMU( ADeltaTime, gx, gy, gz );
				return;
			}

			float ax = FAccelerometerValues[ 0 ];
			float ay = FAccelerometerValues[ 1 ];
			float az = FAccelerometerValues[ 2 ];

			// Rate of change of quaternion from gyroscope
			float qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
			float qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
			float qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
			float qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

			// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
			if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) 
			{
				// Normalise accelerometer measurement
				float recipNorm = invSqrt(ax * ax + ay * ay + az * az);
				ax *= recipNorm;
				ay *= recipNorm;
				az *= recipNorm;

				// Normalise magnetometer measurement
				float mx = FCompassValues[ 0 ];
				float my = FCompassValues[ 1 ];
				float mz = FCompassValues[ 2 ];

				recipNorm = invSqrt(mx * mx + my * my + mz * mz);
				mx *= recipNorm;
				my *= recipNorm;
				mz *= recipNorm;

				// Auxiliary variables to avoid repeated arithmetic
				float _2q0mx = 2.0f * q0 * mx;
				float _2q0my = 2.0f * q0 * my;
				float _2q0mz = 2.0f * q0 * mz;
				float _2q1mx = 2.0f * q1 * mx;
				float _2q0 = 2.0f * q0;
				float _2q1 = 2.0f * q1;
				float _2q2 = 2.0f * q2;
				float _2q3 = 2.0f * q3;
				float _2q0q2 = 2.0f * q0 * q2;
				float _2q2q3 = 2.0f * q2 * q3;
				float q0q0 = q0 * q0;
				float q0q1 = q0 * q1;
				float q0q2 = q0 * q2;
				float q0q3 = q0 * q3;
				float q1q1 = q1 * q1;
				float q1q2 = q1 * q2;
				float q1q3 = q1 * q3;
				float q2q2 = q2 * q2;
				float q2q3 = q2 * q3;
				float q3q3 = q3 * q3;

				// Reference direction of Earth's magnetic field
				float hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
				float hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
				float _2bx = sqrtf(hx * hx + hy * hy);
				float _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
				float _4bx = 2.0f * _2bx;
				float _4bz = 2.0f * _2bz;

				// Gradient decent algorithm corrective step
				float s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
				float s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
				float s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
				float s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

				recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
				s0 *= recipNorm;
				s1 *= recipNorm;
				s2 *= recipNorm;
				s3 *= recipNorm;

				float beta = ProportionalGain() * 2;

				// Apply feedback step
				qDot1 -= beta * s0;
				qDot2 -= beta * s1;
				qDot3 -= beta * s2;
				qDot4 -= beta * s3;
			}

			// Integrate rate of change of quaternion to yield quaternion
			q0 += qDot1 * ADeltaTime;
			q1 += qDot2 * ADeltaTime;
			q2 += qDot3 * ADeltaTime;
			q3 += qDot4 * ADeltaTime;

			// Normalise quaternion
			float recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
			q0 *= recipNorm;
			q1 *= recipNorm;
			q2 *= recipNorm;
			q3 *= recipNorm;
			computeAngles();
/*
			float AHeading;

			AHeading = ComputeAngle( InputPins[ 2 ], InputPins[ 1 ] );
			OutputPins[ 0 ].SetValue( AHeading, FChangeOnly );

			AHeading = ComputeAngle( InputPins[ 2 ], InputPins[ 0 ] );
			OutputPins[ 1 ].SetValue( AHeading, FChangeOnly );

			AHeading = ComputeAngle( InputPins[ 1 ], InputPins[ 0 ] );
			OutputPins[ 2 ].SetValue( AHeading, FChangeOnly );
*/
			FChangeOnly() = true;
		}

		inline void SystemStart()
		{
			FLastTime = micros();
		}

	public:
		inline MadgwickFilter()
		{
			FChangeOnly() = false;
			FModified() = true;
		}
	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif