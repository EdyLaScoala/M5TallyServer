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
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_InitialValue
	> class PCF8591_DAC :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	};
//---------------------------------------------------------------------------
	enum TArduinoPCF8591ADCMode { adcSingleEnded, adc3Differential, adcMixed, adc2Differential };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_DAC,
//		uint8_t C_END_CHANNEL,
		typename T_Enabled,
		typename T_Mode,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3,
		int8_t C_OutputPins_MAX_CONNECTED_INDEX,
		int8_t C_OutputPins_MIN_CONNECTED_INDEX
//		uint8_t C_START_CHANNEL
	> class PCF8591 :
		public T_Address,
		public T_DAC,
		public T_Enabled,
		public T_Mode,
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )

	public:
		_V_PROP_( DAC )

	public:
//		bool	ClockInputPin_o_IsConnected : 1;
//		_V_PROP_( DAC_ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Mode )

	protected:
//		uint8_t FStartChannel : 2;
//		uint8_t FEndChannel : 2;

//		uint8_t	FConfig = 0xFF;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ProcessData( false, false );
		}

//#ifdef _MITOV_PCF8591_DAC_CLOCK_PIN_
		inline void DAC_ClockInputPin_o_Receive( void *_Data )
		{
			ProcessData( true, false );
		}
//#endif

//#ifdef _MITOV_PCF8591_DAC_INPUT_PIN_
		inline void DAC_InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue == DAC().InitialValue() )
				return;

			DAC().InitialValue() = AValue;
//#ifdef _MITOV_PCF8591_DAC_CLOCK_PIN_
			if( ! DAC().ClockInputPin_o_IsConnected() )
//#endif
				ProcessData( true, false );

		}
//#endif

	protected:
		void ProcessData( bool AFromDAC, bool AFirstTime )
		{
			if( ! Enabled() )
				return;

			uint8_t	AStartChannel;
			uint8_t AMaxChannel;
			switch( Mode() )
			{
				case adcSingleEnded:	AMaxChannel = 4; break;
				case adc2Differential:	AMaxChannel = 2; break;
				default: AMaxChannel = 3;
			}

			if( C_OutputPins_MIN_CONNECTED_INDEX > AMaxChannel || C_OutputPins_MIN_CONNECTED_INDEX < 0 )
				AStartChannel = AMaxChannel;

			else
				AStartChannel = C_OutputPins_MIN_CONNECTED_INDEX;


//			for( AStartChannel = 0; AStartChannel < AMaxChannel; ++AStartChannel )
//				if( OutputPins[ i ].IsConnected() )
//					break;

/*
			while( AMaxChannel > AStartChannel )
			{
				if( OutputPins[ AMaxChannel ].IsConnected() )
					break;

				--AMaxChannel;
			}
*/

			uint8_t	AConfig =	AStartChannel | 
								0b100 | 
								uint8_t( Mode() ) << 4;

			if( DAC().Enabled() )
				AConfig |= 0b01000000;

			uint8_t	AOutValue = ( DAC().InitialValue() * 255 ) + 0.5;

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( AConfig );
//			Serial.println( AConfig, BIN );
			if( AFromDAC )
				C_I2C.write( AOutValue );

			C_I2C.endTransmission();

			if( AFromDAC )
				if( ! AFirstTime )
					return;

			if( C_OutputPins_MAX_CONNECTED_INDEX < AStartChannel )
				return;

			int8_t ACount = C_OutputPins_MAX_CONNECTED_INDEX - AStartChannel + 1;

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( ACount + 1 ));

			C_I2C.read(); // Skip old value
			for( int8_t i = 0; i < ACount; ++i )
			{
				bool AIsDiferential = false;
				if((( AStartChannel + i ) < 2 ) && ( Mode() == adc2Differential ) )
					AIsDiferential = true;

				else if((( AStartChannel + i ) == 2 ) && ( Mode() == adcMixed ) )
					AIsDiferential = true;

				float AValue;
				if( AIsDiferential )
					AValue = ( float( int8_t( C_I2C.read() )) / 255.0 ) + 0.5;

				else
  					AValue = float( C_I2C.read() ) / 255.0;

//				OutputPins[ AStartChannel + i ].SetValue( AValue, ! AFirstTime );

				switch( AStartChannel + i )
				{
					case 0: T_OutputPins_0::SetPinValue( AValue, ! AFirstTime ); break;
					case 1: T_OutputPins_1::SetPinValue( AValue, ! AFirstTime ); break;
					case 2: T_OutputPins_2::SetPinValue( AValue, ! AFirstTime ); break;
					case 3: T_OutputPins_3::SetPinValue( AValue, ! AFirstTime ); break;
				}

			}
		}

	public:
		inline void SystemStart()
		{
			ProcessData( true, true );
		}

		inline void SystemLoopBegin()
		{
			ProcessData( false, false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif