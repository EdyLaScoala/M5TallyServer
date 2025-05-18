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
		int C_PIN_NUMBER,
		bool T_INITIAL_VALUE,
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_IsRawInput,
		typename T_OutputPin
	> class ESP32_DAC_DigitalChannel :
		public T_IsAnalog,
		public T_IsCombinedInOut,
		public T_IsOutput,
		public T_IsPullDown,
		public T_IsPullUp,
		public T_IsRawInput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsAnalog )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOutput )
		_V_PROP_( IsPullDown )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsRawInput )

	protected:
//        bool FIsOutput : 1;
//        bool FIsPullUp : 1;
//        bool FIsPullDown : 1;
//        bool FIsOpenDrain : 1;
//        bool FIsAnalog : 1;
//		bool FLastOutput : 1;
//		bool FRawInput : 1;
//		bool FCombinedInOut : 1;

//	public:
//		OpenWire::SinkPin	AnalogInputPin;

	protected:
        void PinDirectionsInit()
        {
			if( IsRawInput() )
				return;

            if( IsOutput() )
			{
				if( IsAnalog() )
					pinMode( C_PIN_NUMBER, ANALOG );

				else
					pinMode( C_PIN_NUMBER, OUTPUT );
			}

            else
				pinMode( C_PIN_NUMBER, IsPullUp().GetValue() ? INPUT_PULLUP : ( IsPullDown().GetValue() ? INPUT_PULLDOWN : INPUT ) );
        }

	public:
		inline void SystemInit()
		{
            PinDirectionsInit();
		}

		inline void SystemStart()
		{
			if( IsRawInput() )
				return;

			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    	T_OutputPin::SetPinValue( AValue, false );
		}

		inline void SystemLoopBegin()
        {
            if( ! IsOutput().GetValue() )
            {
    			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//				if( AValue == FLastOutput )
//					return;

//				FLastOutput = AValue;
//	Serial.println( AData.Value );
	    		T_OutputPin::SetPinValue( AValue, true );
            }
        }

	public:
		void AnalogInputPin_o_Receive( void *_Data )
        {
            if( IsOutput().GetValue() && IsAnalog().GetValue() )
            {
                float AValue = *(float*)_Data;
                dacWrite( C_PIN_NUMBER, ( AValue * 255 ) + 0.5 );
            }
        }

		void DigitalInputPin_o_Receive( void *_Data )
		{
			if( IsRawInput() )
			{
				*((int*)_Data) = C_PIN_NUMBER;
				return;
			}

            if( ( IsCombinedInOut().GetValue() || IsOutput().GetValue() ) && ( ! IsAnalog().GetValue() ))
            {
				bool AValue = *(bool *)_Data;

				if( AValue )
				  if( IsCombinedInOut() )
					  pinMode( C_PIN_NUMBER, OUTPUT );

                digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );

				if( ! AValue )
					if( IsCombinedInOut() )
						pinMode( C_PIN_NUMBER, IsPullUp().GetValue() ? INPUT_PULLUP : ( IsPullDown().GetValue() ? INPUT_PULLDOWN : INPUT ) );

            }
		}

	public:
        void SetIsOutput( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsOutput() = AValue;
            PinDirectionsInit();
        }

        void SetIsPullUp( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullUp() = AValue;
            PinDirectionsInit();
        }

        void SetIsAnalog( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsAnalog() = AValue;
            PinDirectionsInit();
        }

/*
        void SetIsOpenDrain( bool AValue )
        {
            if( FIsOpenDrain == AValue )
                return;

            FIsOpenDrain = AValue;
            PinDirectionsInit();
        }
*/
/*
	public:
		ESP32_DAC_DigitalChannel( bool AIsOutput, bool AIsPullUp, bool AIsPullDown, bool AIsOpenDrain, bool AIsAnalog, bool ACombinedInOut, bool ARawInput ) :
            FIsOutput( AIsOutput ),
            FIsPullUp( AIsPullUp ),
			FIsPullDown( AIsPullDown ),
//			FIsOpenDrain( AIsOpenDrain ),
            FIsAnalog( AIsAnalog ),
			FRawInput( ARawInput ),
			FCombinedInOut( ACombinedInOut )
		{
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif