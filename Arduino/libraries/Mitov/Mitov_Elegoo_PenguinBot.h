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
		typename T_OutputPins_Left,
		typename T_OutputPins_Right
	> class ElegooPenguinBotInfraredDistanceSensors :
		public T_OutputPins_Left,
		public T_OutputPins_Right
	{
	public:
		void ReadValues()
		{
			float AValue = analogRead( A0 );
			AValue /= VISUINO_ANALOG_IN_RANGE;
			T_OutputPins_Left::SetPinValue( AValue );

			AValue = analogRead( A1 );
			AValue /= VISUINO_ANALOG_IN_RANGE;
			T_OutputPins_Right::SetPinValue( AValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValues();
		}

	public:
		inline void SystemStart()
		{
			ReadValues();
		}

		inline void SystemLoopBegin()
		{
			ReadValues();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class ElegooPenguinBotBatterySensor :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void ReadValue()
		{
			if( ! Enabled() )
				return;

			const float RES1 = 10000;
			const float RES2 = 2000;
			const float VREF = 1.1;

			float AValue = analogRead( A4 );
			float volMeasure = AValue * VREF / 1024;
			float VCC = volMeasure * (RES1 + RES2) / RES2;
			T_OutputPin::SetPinValue( VCC );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue();
		}

	public:
		inline void SystemStart()
		{
			pinMode( A4, INPUT );
			ReadValue();
		}

		inline void SystemLoopBegin()
		{
			ReadValue();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
