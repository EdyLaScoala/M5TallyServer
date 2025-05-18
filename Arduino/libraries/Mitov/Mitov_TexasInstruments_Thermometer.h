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
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class TexasInstruments_Thermometer :
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
	    _V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = (*(float *)_Data);
			if( Enabled() )
			{
    			AValue *= 500;

    			if( InFahrenheit() )
    				AValue = AValue * ( 9.0/5.0 ) + 32.0;

    	    }

			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template <
	    uint8_t C_PIN,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class TexasInstruments_Thermometer_Module :
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
	    _V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		void ReadSensor()
		{
			if( ! Enabled() )
			    return;

		    float AValue = float( analogRead( C_PIN )) / float( VISUINO_ANALOG_IN_RANGE );

   			AValue *= 500;

   			if( InFahrenheit() )
   				AValue = AValue * ( 9.0/5.0 ) + 32.0;

			T_OutputPin::SetPinValue( AValue );
		}

	public:
        inline void SystemStart()
        {
            ReadSensor();
        }

        inline void SystemLoopBegin()
        {
            ReadSensor();
        }
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif