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
	class BooleanOr
	{
	public:
		inline static bool CalculateOutput( uint8_t *AInputValues, uint16_t ANumInputs )
		{
			bool AValue = false;
			for ( int i = 0; i < ANumInputs; ++i )
				AValue |= (( AInputValues[ i / 8 ] & ( 1 << ( i & 0b111 ) )) != 0 );

			return AValue;
		}

	};
//---------------------------------------------------------------------------
	class BooleanAnd
	{
	public:
		inline static bool CalculateOutput( uint8_t *AInputValues, uint16_t ANumInputs )
		{
			bool AValue = (ANumInputs > 0);
			for ( int i = 0; i < ANumInputs; ++i )
				AValue &= (( AInputValues[ i / 8 ] & ( 1 << ( i & 0b111 ) )) != 0 );

			return AValue;
		}

	};
//---------------------------------------------------------------------------
	class BooleanXor
	{
	public:
		inline static bool CalculateOutput( uint8_t *AInputValues, uint16_t ANumInputs )
		{
			bool AValue = false;
			for ( int i = 0; i < ANumInputs; ++i )
				AValue ^= (( AInputValues[ i / 8 ] & ( 1 << ( i & 0b111 ) )) != 0 );

			return AValue;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION_CLASS, 
		int C_NUM_INPUTS,
		typename T_FModified,
		typename T_OutputPin
	> class BooleanOperation :
		public T_FModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		_V_PROP_( FModified ) // For speed optimization!

		uint8_t	FValues[ ( C_NUM_INPUTS + 7 ) / 8 ] = { 0 };

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			if( Func::SetBitField( FValues, AIndex, *(bool *)_Data ))
				FModified() = true;

		}

	protected:
		inline void CallCalculateSendOutput( bool AFromStart ) // Work around compiler bug
		{
			CalculateSendOutput( AFromStart );
		}

		void CalculateSendOutput( bool AChangeOnly )
		{
//			Serial.println( "CalculateSendOutput+++" );
			bool AValue = T_IMPLEMENTATION_CLASS::CalculateOutput( FValues, C_NUM_INPUTS );
//			Serial.println( AValue );
			T_OutputPin::SetPinValue( AValue, AChangeOnly );
			FModified() = false;
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( false );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( true );

		}

	public:
		BooleanOperation()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
/*
	class BooleanInverter
	{
	public:
		inline static bool CalculateOutput( bool AInValue )
        {
			return ! AInValue;
        }

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class BooleanInverter :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

    public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            T_OutputPin::SetPinValue( ! *(bool*)_Data );
        }

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif