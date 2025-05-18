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
#include <Wire.h> //I2C Arduino Library
#include <Mitov_Basic_I2C.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoMPUClockSource { mcsAutoSelect = 6, mcsInternal = 0, mcsGyroX = 1, mcsGyroY = 2, mcsGyroZ = 3, mcsExt32K = 4, mcsExt19M = 5, mcsReset = 7 };
//---------------------------------------------------------------------------
/*
	class MPUOptionalSensor
	{
	public:
		bool Enabled : 1;
//		bool Queue : 1;
		bool SelfTest : 1; // Added to save space as bitfield

	public:
		MPUOptionalSensor() :
			Enabled( true ),
//			Queue( false ),
			SelfTest( false )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_SelfTest
	> class MPUOptionalSelfTestSensor :
		public T_Enabled,
		public T_SelfTest
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( SelfTest )
	};
//---------------------------------------------------------------------------
	enum TArduinoMPU9250AccelerometerRange { ar2g, ar4g, ar8g, ar16g };
//---------------------------------------------------------------------------
	typedef enum
	{
      af_AB_1130Hz_AF_4KHz,
      af_AB_460Hz_AF_1KHz,
      af_AB_184Hz_AF_1KHz,
      af_AB_92Hz_AF_1KHz,
      af_AB_41Hz_AF_1KHz,
      af_AB_20Hz_AF_1KHz,
      af_AB_10Hz_AF_1KHz,
      af_AB_5Hz_AF_1KHz
	} TArduinoMPU9250AccelerometerFilter;
//---------------------------------------------------------------------------
	template <
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class MPUAccelerometer : // : public MPUOptionalAxesSensor
		public T_FullScaleRange,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	public:
		_V_PROP_( FullScaleRange )

	public:
		inline bool GetIsEnabledX() { return ( X().Enabled() && OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( Y().Enabled() && OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( Z().Enabled() && OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }

//		bool	Queue : 1;
//		ArduinoMPU9250AccelerometerFilter	Filter : 3; // Added in the base class to save spece with bitfields

/*
	public:
		MPUAccelerometer() :
			FullScaleRange( ar2g ),
//			Queue( false ),
			Filter( af_AB_1130Hz_AF_4KHz )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	enum TArduinoMPU9250GyroscopeRange { gr250dps, gr500dps, gr1000dps, gr2000dps };
//---------------------------------------------------------------------------
	template <
		typename T_FullScaleRange,
		typename T_OutputPinsX,
		typename T_OutputPinsY,
		typename T_OutputPinsZ,
		typename T_Units,
		typename T_X,
		typename T_Y,
		typename T_Z
	> class MPUGyroscope : // public MPUOptionalAxesSensor
		public T_FullScaleRange,
		public T_OutputPinsX,
		public T_OutputPinsY,
		public T_OutputPinsZ,
		public T_Units,
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PIN_( OutputPinsX )
		_V_PIN_( OutputPinsY )
		_V_PIN_( OutputPinsZ )

	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	public:
		_V_PROP_( FullScaleRange )
		_V_PROP_( Units )

	public:
		inline bool GetIsEnabledX() { return ( X().Enabled() && OutputPinsX().GetPinIsConnected() ); }
		inline bool GetIsEnabledY() { return ( Y().Enabled() && OutputPinsY().GetPinIsConnected() ); }
		inline bool GetIsEnabledZ() { return ( Z().Enabled() && OutputPinsZ().GetPinIsConnected() ); }

		inline bool GetIsEnabled() { return GetIsEnabledX() || GetIsEnabledY() || GetIsEnabledZ(); }

	};
//---------------------------------------------------------------------------
	enum TArduinoMPU9250FrameSynchronizationLocation { fslDisabled, fslThermometer, fslGyroscopeX, fslGyroscopeY, fslGyroscopeZ, fslAccelerometerX, fslAccelerometerY, fslAccelerometerZ };
//---------------------------------------------------------------------------
	template <
		typename T_EnableInterrupt,
		typename T_InterruptOnLowLevel,
		typename T_Location,
		typename T_OutputPin
	> class MPUFrameSynchronization :
		public T_EnableInterrupt,
		public T_InterruptOnLowLevel,
		public T_Location,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Location )
		_V_PROP_( EnableInterrupt )
		_V_PROP_( InterruptOnLowLevel )

/*
	public:
		MPUFrameSynchronization() :
			Location( fslDisabled ),
			EnableInterrupt( false ),
			InterruptOnLowLevel( false )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_Inverted,
		typename T_Latch,
		typename T_OnFrameSync,
		typename T_OnRawReady,
		typename T_OpenDrain
	> class MPUInterrupt :
		public T_Inverted,
		public T_Latch,
		public T_OnFrameSync,
		public T_OnRawReady,
		public T_OpenDrain
	{
	public:
		_V_PROP_( Inverted )
		_V_PROP_( OpenDrain )
		_V_PROP_( Latch )
//		_V_PROP_( OnMotion )
//		_V_PROP_( OnQueueOverflow )
		_V_PROP_( OnFrameSync )
		_V_PROP_( OnRawReady )
/*
		bool	Inverted : 1; // Added in the base class to save memory trough bitfields!
		bool	OpenDrain : 1; // Added in the base class to save memory trough bitfields!
		bool	Latch : 1; // Added in the base class to save memory trough bitfields!
		bool	OnMotion : 1; // Added in the base class to save memory trough bitfields!
//		bool	OnQueueOverflow : 1;
		bool	OnFrameSync : 1;
		bool	OnRawReady : 1;

		public:
		MPUInterrupt() :
			Inverted( false ),
			OpenDrain( false ),
			Latch( true ),
			OnMotion( false ),
//			OnQueueOverflow( false ),
			OnFrameSync( false ),
			OnRawReady( false )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_SampleFrequency
	> class MPU6050LowPowerMode :
		public T_Enabled,
		public T_SampleFrequency
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( SampleFrequency )

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif