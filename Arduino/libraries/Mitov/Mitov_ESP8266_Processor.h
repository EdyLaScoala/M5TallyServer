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
#include <Esp.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	class ESP8266ProcessorReset
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ESP.restart();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class ESP8266GetProcessorResetReason :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( ESP.getResetReason());
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( ESP.getResetReason());
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class ESP8266GetProcessorResetInfo :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( ESP.getResetInfo());
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( ESP.getResetInfo());
		}

	};
//---------------------------------------------------------------------------
	enum TArduinoESP8266ProcessorModuleDeepSleepMode { dsmDefault = int( RF_DEFAULT ), dsmCalibration = int( RF_CAL ), dsmNoCalibration = int( RF_NO_CAL ), dsmDisableRF = int( RF_DISABLED ) };
//---------------------------------------------------------------------------
	template <
		typename T_Mode,
		typename T_Period
	> class ESP8266ProcessorDeepSleep :
		public T_Mode,
		public T_Period
	{
	public:
		_V_PROP_( Mode )
		_V_PROP_( Period )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ESP.deepSleep( Period(), RFMode( Mode().GetValue() ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class ESP8266GetArduinoAvailableRAM :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue( false );
		}

	protected:
		inline void ReadValue( bool AChangeOnly )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( ESP.getFreeHeap(), AChangeOnly );

		}

	public:
		inline void SystemStart()
		{
			ReadValue( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValue( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif