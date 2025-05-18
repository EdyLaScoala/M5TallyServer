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

#ifdef VISUINO_ANY_ESP32
#else
#include <Servo.h>
#endif

namespace Mitov
{
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER,
		typename T_Reversed
	> class MitovServo :
		public T_Enabled,
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse,
		public T_Reversed
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )
		_V_PROP_( Reversed )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
#ifdef VISUINO_ANY_ESP32
				Mitov::ServoAttach( PIN_NUMBER );
#else
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
#endif
				UpdatePosition( InitialValue().GetValue() );
			}
			else
#ifdef VISUINO_ANY_ESP32
				Mitov::ServoDetach( PIN_NUMBER );
#else
				FServo.detach();
#endif
		}

	protected:
#ifndef VISUINO_ANY_ESP32
		Servo	FServo;
#endif

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( Enabled().GetIsConstant() )
			{
				if( Enabled() )
					UpdatePosition( *((float *)_Data) );
			}

			else
			{
				InitialValue() = *((float *)_Data);
				if( Enabled() )
					UpdatePosition( *((float *)_Data) );
			}
		}

	protected:
		void UpdatePosition( float AValue )
		{
			AValue = MitovConstrain( AValue, 0.0f, 1.0f );
			if( Reversed() )
				AValue = 1.0 - AValue;

#ifdef VISUINO_ANY_ESP32
			AValue *= 180;
			Mitov::ServoWrite( PIN_NUMBER, AValue, MinPulse().GetValue(), MaxPulse().GetValue() );
#else
//			FServo.write( AValue );
			AValue = ( AValue * (MaxPulse().GetValue() - MinPulse().GetValue())) + MinPulse().GetValue(); // Scale input to microseconds
			FServo.writeMicroseconds( AValue );
#endif
		}

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				UpdateEnabled();

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER,
		typename T_Reversed
	> class ServoShieldElement :
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse,
		public T_Reversed
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )
		_V_PROP_( Reversed )

	protected:
#ifndef VISUINO_ANY_ESP32
		Servo	FServo;
#endif

	public:
		inline void SystemInit( float AInitialValue )
		{
#ifdef VISUINO_ANY_ESP32
			Mitov::ServoAttach( PIN_NUMBER );
#else
			FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
#endif
			InputPin_o_Receive( &AInitialValue );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			float AValue = MitovConstrain( *((float *)_Data), 0.0f, 1.0f );
			if( Reversed() )
				AValue = 1.0 - AValue;

#ifdef VISUINO_ANY_ESP32
			AValue = MitovConstrain( AValue, 0.0f, 1.0f ) * 180;
			Mitov::ServoWrite( PIN_NUMBER, AValue, MinPulse().GetValue(), MaxPulse().GetValue() );
#else
			AValue = (MitovConstrain( AValue, 0.0f, 1.0f ) * (MaxPulse().GetValue() - MinPulse().GetValue())) + MinPulse().GetValue(); // Scale input to microseconds
			FServo.writeMicroseconds( AValue );
//			FServo.write( AValue );
#endif
		}

	};
//---------------------------------------------------------------------------
}

