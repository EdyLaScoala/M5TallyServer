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
#include <CurieIMU.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
//		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_Range,
		typename T_Rate
	> class Arduino101Gyroscope :
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_Range,
		public T_Rate
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	public:
		_V_PROP_( Rate )
		_V_PROP_( Range )

	public:
		void SetRate( uint16_t AOldValue, uint16_t AValue )
		{
			static const uint16_t CList[] = { 25, 50, 100, 200, 400, 800, 1600, 3200 };
			AValue = ConstrainToList<uint16_t>( AValue, CList, 8 );
			if( AOldValue == AValue )
				return;

			Rate() = AValue;
			CurieIMU.setGyroRate( Rate() );
		}

		void SetRange( uint16_t AOldValue, uint16_t AValue )
		{
			static const uint16_t CList[] = { 125, 250, 500, 1000, 2000 };
			AValue = ConstrainToList<uint16_t>( AValue, CList, 5 );
			if( AOldValue == AValue )
				return;

			Range() = AValue;
			CurieIMU.setGyroRange( Range() );
		}

	protected:
		inline float convertRawGyro(int gRaw) 
		{
			// since we are using 250 degrees/seconds range
			// -250 maps to a raw value of -32768
			// +250 maps to a raw value of 32767
  
			float g = (gRaw * 250.0) / 32768.0;

			return g;
		}

	protected:
		void ReadSensors()
		{
			int gxRaw, gyRaw, gzRaw;         // raw gyro values
			CurieIMU.readGyro(gxRaw, gyRaw, gzRaw);
			T_OutputPins_X::SendValue( convertRawGyro( gxRaw ));
			T_OutputPins_Y::SendValue( convertRawGyro( gyRaw ));
			T_OutputPins_Z::SendValue( convertRawGyro( gzRaw ));
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadSensors();
		}

		inline void SystemStart()
		{
			CurieIMU.setGyroRate( Rate() );
			CurieIMU.setGyroRange( Range() );
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_Range,
		typename T_Rate
	> class Arduino101Accelerometer :
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_Range,
		public T_Rate
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	public:
		_V_PROP_( Rate )
		_V_PROP_( Range )

/*
	public:
		static Arduino101SensorInterrupt *FTapInterrupt;
		static Arduino101SensorInterrupt *FShockInterrupt;
		static Arduino101SensorInterrupt *FMotionInterrupt;
		static Arduino101SensorInterrupt *FZeroMotionInterrupt;
*/
/*
	public:
		static void eventCallback()
		{
			C_OWNER.HandleInterrupt();
			if( FTapInterrupt )
				FTapInterrupt->HandleInterrupt();

			if( FShockInterrupt )
				FShockInterrupt->HandleInterrupt();

			if( FMotionInterrupt )
				FMotionInterrupt->HandleInterrupt();

			if( FZeroMotionInterrupt )
				FZeroMotionInterrupt->HandleInterrupt();
		}

*/
	public:
		void SetRate( float AOldValue, float AValue )
		{
			static const float CList[] = { 12.5, 25, 50, 100, 200, 400, 800, 1600 };
			AValue = ConstrainToList<float>( AValue, CList, 8 );
			if( AOldValue == AValue )
				return;

			Rate() = AValue;
			CurieIMU.setAccelerometerRate( Rate );
		}

		void SetRange( uint8_t AOldValue, uint8_t AValue )
		{
			static const uint8_t CList[] = { 2, 4, 8, 16 };
			AValue = ConstrainToList<uint8_t>( AValue, CList, 4 );
			if( AOldValue == AValue )
				return;

			Range() = AValue;
			CurieIMU.setAccelerometerRange( Range() );
		}

	protected:
		inline float convertRawAcceleration(int aRaw) 
		{
			// since we are using 2G range
			// -2g maps to a raw value of -32768
			// +2g maps to a raw value of 32767
  
			float a = (aRaw * 2.0) / 32768.0;

			return a;
		}

	protected:
		void ReadSensors()
		{
			int axRaw, ayRaw, azRaw;         // raw gyro values
			CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);
			T_OutputPins_X::SetPinValue( convertRawAcceleration( axRaw ));
			T_OutputPins_Y::SetPinValue( convertRawAcceleration( ayRaw ));
			T_OutputPins_Z::SetPinValue( convertRawAcceleration( azRaw ));
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors();
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadSensors();
		}

		inline void SystemStart()
		{
			CurieIMU.setAccelerometerRate( Rate() );
			CurieIMU.setAccelerometerRange( Range() );
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_Range,
		typename T_Rate
	> Arduino101SensorInterrupt *Arduino101Accelerometer<
		T_OutputPins_X,
		T_OutputPins_Y,
		T_OutputPins_Z,
		T_Range,
		T_Rate
	>::FTapInterrupt = nullptr;

	template <
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_Range,
		typename T_Rate
	> Arduino101SensorInterrupt *Arduino101Accelerometer<
		T_OutputPins_X,
		T_OutputPins_Y,
		T_OutputPins_Z,
		T_Range,
		T_Rate
	>::FShockInterrupt = nullptr;

	template <
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_Range,
		typename T_Rate
	> Arduino101SensorInterrupt *Arduino101Accelerometer<
		T_OutputPins_X,
		T_OutputPins_Y,
		T_OutputPins_Z,
		T_Range,
		T_Rate
	>::FMotionInterrupt = nullptr;

	template <
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_Range,
		typename T_Rate
	> Arduino101SensorInterrupt *Arduino101Accelerometer<
		T_OutputPins_X,
		T_OutputPins_Y,
		T_OutputPins_Z,
		T_Range,
		T_Rate
	>::FZeroMotionInterrupt = nullptr;
*/
//---------------------------------------------------------------------------
	template <
		typename T_InFahrenheit,
		typename T_OutputPin
	> class Arduino101Thermometer :
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InFahrenheit )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			float AValue = CurieIMU.readTemperature();
			AValue = ( AValue / 32767.0) + 23;
			if( InFahrenheit() )
				AValue = Func::ConvertCtoF( AValue );

			T_OutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( true );
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

	};
//---------------------------------------------------------------------------	
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_DoubleTapOutputPin,
		typename T_DurationDoubleTap,
		typename T_DurationQuiet,
		typename T_DurationShock,
		typename T_FDetectedDoubleTap,
		typename T_OutputPins_X_Up,
		typename T_OutputPins_X_Down,
		typename T_OutputPins_Y_Up,
		typename T_OutputPins_Y_Down,
		typename T_OutputPins_Z_Up,
		typename T_OutputPins_Z_Down,
		typename T_Threshold
	> class Arduino101CurieDetectTap :
		public T_DoubleTapOutputPin,
		public T_DurationDoubleTap,
		public T_DurationQuiet,
		public T_DurationShock,
		public T_FDetectedDoubleTap,
		public T_OutputPins_X_Up,
		public T_OutputPins_X_Down,
		public T_OutputPins_Y_Up,
		public T_OutputPins_Y_Down,
		public T_OutputPins_Z_Up,
		public T_OutputPins_Z_Down,
		public T_Threshold
	{
	public:
		_V_PIN_( OutputPins_X_Up )
		_V_PIN_( OutputPins_X_Down )
		_V_PIN_( OutputPins_Y_Up )
		_V_PIN_( OutputPins_Y_Down )
		_V_PIN_( OutputPins_Z_Up )
		_V_PIN_( OutputPins_Z_Down )

		_V_PIN_( DoubleTapOutputPin )

	public:
		_V_PROP_( Threshold )
		_V_PROP_( DurationQuiet )
		_V_PROP_( DurationShock )
		_V_PROP_( DurationDoubleTap )

	protected:
		_V_PROP_( FDetectedDoubleTap )

		uint8_t	FDetectedBits = 0;

	public:
		inline void UpdateThreshold()
		{
			CurieIMU.setDetectionThreshold( CURIE_IMU_SHOCK, Threshold() );
		}

		inline void UpdateDurationQuiet()
		{
			CurieIMU.setDetectionDuration( CURIE_IMU_TAP_QUIET, DurationQuiet() );
		}

		inline void UpdateDurationShock()
		{
			CurieIMU.setDetectionDuration( CURIE_IMU_TAP_SHOCK, DurationShock() );
		}

		inline void UpdateDurationDoubleTap()
		{
			CurieIMU.setDetectionDuration( CURIE_IMU_DOUBLE_TAP, DurationDoubleTap() );
		}

	public:
		inline void HandleInterrupt()
		{
//			Serial.println( "TEST!!!!!!!!" );
/*
			if( CurieIMU.getInterruptStatus(CURIE_IMU_TAP_SHOCK))
				Serial.println( "TEST-11111111----!!!!!!!!" );

			if( CurieIMU.getInterruptStatus(CURIE_IMU_TAP_QUIET))
				Serial.println( "TEST-22222222----!!!!!!!!" );
*/
//return;
			if( CurieIMU.getInterruptStatus(CURIE_IMU_TAP))
			{
//				Serial.println( "TEST-11111111----!!!!!!!!" );
				FDetectedBits = 0;
				for( int axis = 0; axis < 3; ++axis )
				{
					if( CurieIMU.tapDetected( axis, POSITIVE ) )
						FDetectedBits |= 1 << ( axis * 2 );

					if( CurieIMU.tapDetected( axis, NEGATIVE ) )
						FDetectedBits |= 1 << ( axis * 2 + 1 );

//					OutputPins[ axis * 2 ].SendValue( CurieIMU.shockDetected( axis, POSITIVE ));
//					OutputPins[ axis * 2 + 1 ].SendValue( CurieIMU.shockDetected( axis, NEGATIVE ));
				}
			}

			if( CurieIMU.getInterruptStatus( CURIE_IMU_DOUBLE_TAP ))
				FDetectedDoubleTap() = true;

		}

	public:
		inline void SystemLoopBegin()
		{

//			CurieIMU.noInterrupts(CURIE_IMU_TAP);
			uint8_t ADetectedBits = FDetectedBits;
			FDetectedBits = 0;
//			CurieIMU.interrupts(CURIE_IMU_TAP);
			if( ADetectedBits )
			{
//				Serial.println( ADetectedBits );
				for( int axis = 0; axis < 3; ++axis )
				{
					if( ADetectedBits & ( 1 << ( axis * 2 )) )
					{
//						OutputPins[ axis * 2 ].Notify( nullptr );
						switch( axis )
						{
							case 0: T_OutputPins_X_Up::ClockPin(); break;
							case 1: T_OutputPins_Y_Up::ClockPin(); break;
							case 2: T_OutputPins_Z_Up::ClockPin(); break;
						}
					}

					if( ADetectedBits & ( 1 << ( axis * 2 + 1 )) )
					{
//						OutputPins[ axis * 2 + 1 ].Notify( nullptr );
						switch( axis )
						{
							case 0: T_OutputPins_X_Down::ClockPin(); break;
							case 1: T_OutputPins_Y_Down::ClockPin(); break;
							case 2: T_OutputPins_Z_Down::ClockPin(); break;
						}
					}
				}
			}

			if( FDetectedDoubleTap() )
			{
				FDetectedDoubleTap() = false;
				T_DoubleTapOutputPin::ClockPin();
			}
		}

		inline void SystemStart()
		{
//			Arduino101Accelerometer::FTapInterrupt = this;
			C_OWNER.AttachInterrupt();
//			CurieIMU.attachInterrupt(Arduino101Accelerometer::eventCallback);
			CurieIMU.setDetectionThreshold( CURIE_IMU_TAP, Threshold() );
			CurieIMU.setDetectionDuration( CURIE_IMU_TAP_QUIET, DurationQuiet() );
			CurieIMU.setDetectionDuration( CURIE_IMU_TAP_SHOCK, DurationShock() );
			CurieIMU.setDetectionDuration( CURIE_IMU_DOUBLE_TAP, DurationDoubleTap() );
			CurieIMU.interrupts( CURIE_IMU_TAP );
			CurieIMU.interrupts( CURIE_IMU_DOUBLE_TAP );
//			CurieIMU.interrupts( CURIE_IMU_TAP_SHOCK );
//			CurieIMU.interrupts( CURIE_IMU_TAP_QUIET );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Duration,
		typename T_OutputPins_X_Up,
		typename T_OutputPins_X_Down,
		typename T_OutputPins_Y_Up,
		typename T_OutputPins_Y_Down,
		typename T_OutputPins_Z_Up,
		typename T_OutputPins_Z_Down,
		typename T_Threshold
	> class Arduino101CurieDetectShock :
		public T_Duration,
		public T_OutputPins_X_Up,
		public T_OutputPins_X_Down,
		public T_OutputPins_Y_Up,
		public T_OutputPins_Y_Down,
		public T_OutputPins_Z_Up,
		public T_OutputPins_Z_Down,
		public T_Threshold
	{
	public:
		_V_PIN_( OutputPins_X_Up )
		_V_PIN_( OutputPins_X_Down )
		_V_PIN_( OutputPins_Y_Up )
		_V_PIN_( OutputPins_Y_Down )
		_V_PIN_( OutputPins_Z_Up )
		_V_PIN_( OutputPins_Z_Down )

	public:
		_V_PROP_( Threshold )
		_V_PROP_( Duration )

	protected:
		uint8_t	FDetectedBits = 0;

	public:
		inline void UpdateThreshold()
		{
			CurieIMU.setDetectionThreshold( CURIE_IMU_SHOCK, Threshold() );
		}

		inline void UpdateDuration()
		{
			CurieIMU.setDetectionDuration( CURIE_IMU_SHOCK, Duration() );
		}

	public:
		inline void HandleInterrupt()
		{
//			Serial.println( "TEST!!!!!!!!" );
			if( CurieIMU.getInterruptStatus( CURIE_IMU_SHOCK ))
			{
//				Serial.println( "TEST-11111111----!!!!!!!!" );
				FDetectedBits = 0;
				for( int axis = 0; axis < 3; ++axis )
				{
					if( CurieIMU.shockDetected( axis, POSITIVE ) )
						FDetectedBits |= 1 << ( axis * 2 );

					if( CurieIMU.shockDetected( axis, NEGATIVE ) )
						FDetectedBits |= 1 << ( axis * 2 + 1 );

//					OutputPins[ axis * 2 ].SendValue( CurieIMU.shockDetected( axis, POSITIVE ));
//					OutputPins[ axis * 2 + 1 ].SendValue( CurieIMU.shockDetected( axis, NEGATIVE ));
				}
			}
		}

	public:
		inline void SystemLoopBegin()
		{
//			CurieIMU.noInterrupts(CURIE_IMU_SHOCK);
			uint8_t ADetectedBits = FDetectedBits;
			FDetectedBits = 0;
//			CurieIMU.interrupts(CURIE_IMU_SHOCK);
			if( ADetectedBits )
			{
//				Serial.println( ADetectedBits );
				for( int axis = 0; axis < 3; ++axis )
				{
					if( ADetectedBits & ( 1 << ( axis * 2 )) )
					{
//						OutputPins[ axis * 2 ].Notify( nullptr );
						switch( axis )
						{
							case 0: T_OutputPins_X_Up::ClockPin(); break;
							case 1: T_OutputPins_Y_Up::ClockPin(); break;
							case 2: T_OutputPins_Z_Up::ClockPin(); break;
						}
					}

					if( ADetectedBits & ( 1 << ( axis * 2 + 1 )) )
					{
//						OutputPins[ axis * 2 + 1 ].Notify( nullptr );
						switch( axis )
						{
							case 0: T_OutputPins_X_Down::ClockPin(); break;
							case 1: T_OutputPins_Y_Down::ClockPin(); break;
							case 2: T_OutputPins_Z_Down::ClockPin(); break;
						}
					}
				}
			}
		}

		inline void SystemStart()
		{
//			Arduino101Accelerometer::FShockInterrupt = this;
			C_OWNER.AttachInterrupt();
//			CurieIMU.attachInterrupt(Arduino101Accelerometer::eventCallback);
			CurieIMU.setDetectionThreshold( CURIE_IMU_SHOCK, Threshold() );
			CurieIMU.setDetectionDuration( CURIE_IMU_SHOCK, Duration() );
			CurieIMU.interrupts( CURIE_IMU_SHOCK );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Duration,
		typename T_FDetected,
		typename T_OutputPin,
		typename T_Threshold
	> class Arduino101CurieDetectZeroMotion :
		public T_Duration,
		public T_FDetected,
		public T_OutputPin,
		public T_Threshold
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Threshold )
		_V_PROP_( Duration )

	protected:
		_V_PROP_( FDetected )

	public:
		inline void UpdateThreshold()
		{
			CurieIMU.setDetectionThreshold( CURIE_IMU_ZERO_MOTION, Threshold() );
		}

		inline void UpdateDuration()
		{
			CurieIMU.setDetectionDuration( CURIE_IMU_ZERO_MOTION, Duration() );
		}

	public:
		inline void HandleInterrupt()
		{
			if( CurieIMU.getInterruptStatus( CURIE_IMU_ZERO_MOTION ))
				FDetected() = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FDetected() )
			{
				FDetected() = false;
				T_OutputPin::ClockPin();
			}
		}

		inline void SystemStart()
		{
//			Arduino101Accelerometer::FZeroMotionInterrupt = this;
			C_OWNER.AttachInterrupt();
//			CurieIMU.attachInterrupt(Arduino101Accelerometer::eventCallback);
			CurieIMU.setDetectionThreshold( CURIE_IMU_ZERO_MOTION, Threshold() );
			CurieIMU.setDetectionDuration( CURIE_IMU_ZERO_MOTION, Duration() );
			CurieIMU.interrupts( CURIE_IMU_ZERO_MOTION );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPins_X_Up,
		typename T_OutputPins_X_Down,
		typename T_OutputPins_Y_Up,
		typename T_OutputPins_Y_Down,
		typename T_OutputPins_Z_Up,
		typename T_OutputPins_Z_Down,
		typename T_SampleCount,
		typename T_Threshold
	> class Arduino101CurieDetectMotion :
		public T_OutputPins_X_Up,
		public T_OutputPins_X_Down,
		public T_OutputPins_Y_Up,
		public T_OutputPins_Y_Down,
		public T_OutputPins_Z_Up,
		public T_OutputPins_Z_Down,
		public T_SampleCount,
		public T_Threshold
	{
	public:
		_V_PIN_( OutputPins_X_Up )
		_V_PIN_( OutputPins_X_Down )
		_V_PIN_( OutputPins_Y_Up )
		_V_PIN_( OutputPins_Y_Down )
		_V_PIN_( OutputPins_Z_Up )
		_V_PIN_( OutputPins_Z_Down )

	public:
		_V_PROP_( Threshold )
		_V_PROP_( SampleCount )

	protected:
		uint8_t	FDetectedBits = 0;

	public:
		inline void UpdateThreshold()
		{
			CurieIMU.setDetectionThreshold( CURIE_IMU_MOTION, Threshold() );
		}

		inline void UpdateSampleCount()
		{
			CurieIMU.setDetectionDuration( CURIE_IMU_MOTION, SampleCount() );
		}

	public:
		inline void HandleInterrupt()
		{
//			Serial.println( "TEST!!!!!!!!" );
			if( CurieIMU.getInterruptStatus( CURIE_IMU_SHOCK ))
			{
//				Serial.println( "TEST-11111111----!!!!!!!!" );
				FDetectedBits = 0;
				for( int axis = 0; axis < 3; ++axis )
				{
					if( CurieIMU.shockDetected( axis, POSITIVE ) )
						FDetectedBits |= 1 << ( axis * 2 );

					if( CurieIMU.shockDetected( axis, NEGATIVE ) )
						FDetectedBits |= 1 << ( axis * 2 + 1 );

//					OutputPins[ axis * 2 ].SendValue( CurieIMU.shockDetected( axis, POSITIVE ));
//					OutputPins[ axis * 2 + 1 ].SendValue( CurieIMU.shockDetected( axis, NEGATIVE ));
				}
			}
		}

	public:
		inline void SystemLoopBegin()
		{
//			CurieIMU.noInterrupts(CURIE_IMU_SHOCK);
			uint8_t ADetectedBits = FDetectedBits;
			FDetectedBits = 0;
//			CurieIMU.interrupts(CURIE_IMU_SHOCK);
			if( ADetectedBits )
			{
//				Serial.println( ADetectedBits );
				for( int axis = 0; axis < 3; ++axis )
				{
					if( ADetectedBits & ( 1 << ( axis * 2 )) )
					{
//						OutputPins[ axis * 2 ].Notify( nullptr );
						switch( axis )
						{
							case 0: T_OutputPins_X_Up::ClockPin(); break;
							case 1: T_OutputPins_Y_Up::ClockPin(); break;
							case 2: T_OutputPins_Z_Up::ClockPin(); break;
						}
					}

					if( ADetectedBits & ( 1 << ( axis * 2 + 1 )) )
					{
//						OutputPins[ axis * 2 + 1 ].Notify( nullptr );
						switch( axis )
						{
							case 0: T_OutputPins_X_Down::ClockPin(); break;
							case 1: T_OutputPins_Y_Down::ClockPin(); break;
							case 2: T_OutputPins_Z_Down::ClockPin(); break;
						}
					}
				}
			}
		}

		inline void SystemStart()
		{
//			Arduino101Accelerometer::FMotionInterrupt = this;
			C_OWNER.AttachInterrupt();
//			CurieIMU.attachInterrupt(Arduino101Accelerometer::eventCallback);
			CurieIMU.setDetectionThreshold( CURIE_IMU_MOTION, Threshold() );
			CurieIMU.setDetectionDuration( CURIE_IMU_MOTION, SampleCount() );
			CurieIMU.interrupts( CURIE_IMU_MOTION );
		}
	};
//---------------------------------------------------------------------------
	enum TArduino101StepMode { smNormal = CURIE_IMU_STEP_MODE_NORMAL, smSensitive = CURIE_IMU_STEP_MODE_SENSITIVE, smRobust = CURIE_IMU_STEP_MODE_ROBUST };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_Mode,
		typename T_OutputPin
	> class Arduino101CurieDetectSteps :
		public T_Enabled,
		public T_Mode,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Mode )

	public:
		inline void UpdateEnabled()
		{
			CurieIMU.setStepCountEnabled( Enabled() );
		}

		inline void UpdateMode()
		{
			CurieIMU.setStepDetectionMode( Mode() );
		}

	public:
		inline void SystemLoopBegin()
		{
			T_OutputPin::SetPinValue( CurieIMU.getStepCount(), true );
		}

		inline void SystemStart()
		{
//			Arduino101Accelerometer::FStepInterrupt = this;
			C_OWNER.AttachInterrupt();
//			CurieIMU.attachInterrupt( Arduino101Accelerometer::eventCallback );
			CurieIMU.setStepDetectionMode( Mode() );
			CurieIMU.setStepCountEnabled( Enabled() );
			CurieIMU.resetStepCount();
			T_OutputPin::SetPinValue( 0, false );
//			CurieIMU.interrupts( CURIE_IMU_STEP );
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			CurieIMU.resetStepCount();
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( CurieIMU.getStepCount(), true );
		}

	};
//---------------------------------------------------------------------------
	enum TArduino101AccelerometerOrientation : uint8_t { aoXUp, aoXDown, aoYUp, aoYDown, aoZUp, aoZDown };
//---------------------------------------------------------------------------
	template <
		typename T_CalibratingOutputPin,
		typename T_Orientation
	> class Arduino101CalibrateAccelerometer :
		public T_CalibratingOutputPin,
		public T_Orientation
	{
	public:
		_V_PIN_( CalibratingOutputPin )

	public:
		_V_PROP_( Orientation )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			int ADirections[] = {0, 0, 0};
			switch( Orientation() )
			{
				case aoXUp:
					ADirections[ 0 ] = 1;
					break;

				case aoXDown:
					ADirections[ 0 ] = -1;
					break;

				case aoYUp:
					ADirections[ 1 ] = 1;
					break;

				case aoYDown:
					ADirections[ 1 ] = -1;
					break;

				case aoZUp:
					ADirections[ 2 ] = 1;
					break;

				case aoZDown:
					ADirections[ 2 ] = -1;
					break;

			}

			T_CalibratingOutputPin::SetPinValueHigh();
			CurieIMU.autoCalibrateXAccelOffset( ADirections[ 0 ] );
			CurieIMU.autoCalibrateYAccelOffset( ADirections[ 1 ] );
			CurieIMU.autoCalibrateZAccelOffset( ADirections[ 2 ] );
			T_CalibratingOutputPin::SetPinValueLow();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_CalibratingOutputPin,
		typename T_Orientation
	> class Arduino101CalibrateGyroscope :
		public T_CalibratingOutputPin,
		public T_Orientation
	{
	public:
		_V_PIN_( CalibratingOutputPin )

	public:
		_V_PROP_( Orientation )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			T_CalibratingOutputPin::SetPinValueHigh();
			CurieIMU.autoCalibrateGyroOffset();
			T_CalibratingOutputPin::SetPinValueLow();
		}

	};

//---------------------------------------------------------------------------
	template <
		int C_AXIS,
		typename T_Offset
	> class Arduino101AccelerometerOffset :
		public T_Offset
	{
	public:
		_V_PROP_( Offset )

	public:
		inline void UpdateOffset()
		{
			CurieIMU.setAccelerometerOffset( C_AXIS, Offset() );
		}

	public:
		inline void SystemStart()
		{
			CurieIMU.setAccelerometerOffset( C_AXIS, Offset() );
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_AXIS,
		typename T_Offset
	> class Arduino101GyroscopeOffset :
		public T_Offset
	{
	public:
		_V_PROP_( Offset )

	public:
		inline void UpdateOffset()
		{
			CurieIMU.setGyroOffset( C_AXIS, Offset() );
		}

	public:
		inline void SystemStart()
		{
			CurieIMU.setGyroOffset( C_AXIS, Offset() );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Functions_HandleInterrupt
	> class Arduino101CurieIMU
	{
	public:
		static __ICACHE_RAM_ATTR__ void eventCallback()
		{
			T_Functions_HandleInterrupt::Call();
		}

	public:
		inline void AttachInterrupt()
		{
			CurieIMU.attachInterrupt( eventCallback );
		}

/*
	public:
		inline void HandleInterrupt()
		{
			T_Functions_HandleInterrupt::Call();
		}
*/
	public:
		inline static void SystemStart()
		{
			CurieIMU.begin();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif