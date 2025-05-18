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
		typename T_DataOutputPin,
		typename T_Enabled,
		typename T_Frequency,
		typename T_FrequencyUpdateOutputPin,
		typename T_Phase,
		typename T_ReferenceClockFrequency,
		typename T_ResetOutputPin,
		typename T_WordLoadClockOutputPin
	> class AnalogDevices_AD9850_Serial :
		public T_DataOutputPin,
		public T_Enabled,
		public T_Frequency,
		public T_FrequencyUpdateOutputPin,
		public T_Phase,
		public T_ReferenceClockFrequency,
		public T_ResetOutputPin,
		public T_WordLoadClockOutputPin
	{
	public:
		_V_PIN_( WordLoadClockOutputPin )
		_V_PIN_( FrequencyUpdateOutputPin )
		_V_PIN_( DataOutputPin )
		_V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( ReferenceClockFrequency )
		_V_PROP_( Frequency )
		_V_PROP_( Phase )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				UpdateFrequencyAndPhase( true );

			else
				PowerDown();
		}

		void UpdateFrequency()
		{
			if( Enabled() )
				UpdateFrequencyAndPhase( false );

		}

		void UpdatePhase()
		{
			if( Enabled() )
				UpdateFrequencyAndPhase( true );
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	protected:
		void UpdateFrequencyAndPhase( bool AUpdatePhase )
		{
//			unsigned char i,w;
//			long int y;
//			double x;
	  
			//Calculate the frequency of the HEX value
			float x = 4294967295 / ( ReferenceClockFrequency() / 1000000 ) ;//Suitable for 125M Crystal 
			float AFrequency = Frequency() / 1000000;
			AFrequency *= x;
			uint32_t	y = AFrequency;

//			Serial.println( y );

			for( int i = 0; i < 32; ++i )
			{
				T_DataOutputPin::SetPinValue( ( y >> i ) & 0x01, false );
				T_WordLoadClockOutputPin::SetPinValue( true, false );
				T_WordLoadClockOutputPin::SetPinValue( false, false );
			}

			if( AUpdatePhase )
			{
//				uint8_t w = ( Enabled )
				uint8_t w = ( 0b00011111 * Phase() ) + 0.5;
				w <<= 3;
//				uint8_t w = 0;
				for( int i = 0; i < 8; ++i )
				{
					T_DataOutputPin::SetPinValue( ( w >> i ) & 0x01, false );
					T_WordLoadClockOutputPin::SetPinValue( true, false );
					T_WordLoadClockOutputPin::SetPinValue( false, false );
				}
			}

			T_FrequencyUpdateOutputPin::SetPinValue( true, false );
			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
		}

		void PowerDown()
		{
			T_DataOutputPin::SetPinValue( false, false );

			T_FrequencyUpdateOutputPin::SetPinValue( true, false );

			T_WordLoadClockOutputPin::SetPinValue( true, false );

			T_FrequencyUpdateOutputPin::SetPinValue( false, false );

			T_WordLoadClockOutputPin::SetPinValue( false, false );

			T_WordLoadClockOutputPin::SetPinValue( true, false );
			T_WordLoadClockOutputPin::SetPinValue( false, false );

			T_DataOutputPin::SetPinValue( true, false );

			for( int i = 0; i < 6; ++ i )
			{
				T_WordLoadClockOutputPin::SetPinValue( true, false );
				T_WordLoadClockOutputPin::SetPinValue( false, false );
			}

			T_DataOutputPin::SetPinValue( false, false );

			T_FrequencyUpdateOutputPin::SetPinValue( true, false );
			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
//			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
		}

		void Reset()
		{
			T_ResetOutputPin::SetPinValue( true, false );
			T_ResetOutputPin::SetPinValue( false, false );

			T_WordLoadClockOutputPin::SetPinValue( true, false );
			T_WordLoadClockOutputPin::SetPinValue( false, false );

			T_FrequencyUpdateOutputPin::SetPinValue( true, false );
			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
			if( Enabled() )
				UpdateFrequencyAndPhase( true );

			else
				PowerDown();

		}

	public:
		inline void SystemStart()
		{
			T_ResetOutputPin::SetPinValue( false, false );

			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
			T_WordLoadClockOutputPin::SetPinValue( false, false );
			T_DataOutputPin::SetPinValue( false, false );

			Reset();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Frequency,
		typename T_FrequencyUpdateOutputPin,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3,
		typename T_OutputPins_4,
		typename T_OutputPins_5,
		typename T_OutputPins_6,
		typename T_OutputPins_7,
		typename T_Phase,
		typename T_ReferenceClockFrequency,
		typename T_ResetOutputPin,
		typename T_WordLoadClockOutputPin
	> class AnalogDevices_AD9850_Parallel :
		public T_Enabled,
		public T_Frequency,
		public T_FrequencyUpdateOutputPin,
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3,
		public T_OutputPins_4,
		public T_OutputPins_5,
		public T_OutputPins_6,
		public T_OutputPins_7,
		public T_Phase,
		public T_ReferenceClockFrequency,
		public T_ResetOutputPin,
		public T_WordLoadClockOutputPin
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )
		_V_PIN_( OutputPins_4 )
		_V_PIN_( OutputPins_5 )
		_V_PIN_( OutputPins_6 )
		_V_PIN_( OutputPins_7 )

		_V_PIN_( WordLoadClockOutputPin )
		_V_PIN_( FrequencyUpdateOutputPin )
		_V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( ReferenceClockFrequency )
		_V_PROP_( Frequency )
		_V_PROP_( Phase )

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
				PowerUp();
				UpdateFrequencyAndPhase( true );
			}

			else
				PowerDown();
		}

		void UpdateFrequency()
		{
			if( Enabled() )
				UpdateFrequencyAndPhase( true );

		}

		void UpdatePhase()
		{
			if( Enabled() )
				UpdateFrequencyAndPhase( false );

		}

	public:
		inline void SystemStart()
		{
			T_ResetOutputPin::SetPinValue( false, false );

			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
			T_WordLoadClockOutputPin::SetPinValue( false, false );
			SendByte( 0 );
//			DataOutputPin::SetPinValue( false, false );

			Reset();
		}

	protected:
		void SendByte( uint8_t AValue )
		{
//			for( int i = 0; i < 8; ++i )
//				OutputPins[ i ].SendValue<bool>( (( AValue >> i ) & 1 ) != 0 );

			T_OutputPins_0::SetPinValue( (( AValue >> 0 ) & 1 ) != 0 );
			T_OutputPins_1::SetPinValue( (( AValue >> 1 ) & 1 ) != 0 );
			T_OutputPins_2::SetPinValue( (( AValue >> 2 ) & 1 ) != 0 );
			T_OutputPins_3::SetPinValue( (( AValue >> 3 ) & 1 ) != 0 );
			T_OutputPins_4::SetPinValue( (( AValue >> 4 ) & 1 ) != 0 );
			T_OutputPins_5::SetPinValue( (( AValue >> 5 ) & 1 ) != 0 );
			T_OutputPins_6::SetPinValue( (( AValue >> 6 ) & 1 ) != 0 );
			T_OutputPins_7::SetPinValue( (( AValue >> 7 ) & 1 ) != 0 );

			T_WordLoadClockOutputPin::SetPinValue( true, false );
			T_WordLoadClockOutputPin::SetPinValue( false, false );
		}

		void Reset()
		{
			T_ResetOutputPin::SetPinValue( true, false );
			T_ResetOutputPin::SetPinValue( false, false );

//			WordLoadClockOutputPin::SetPinValue( true, false );
//			WordLoadClockOutputPin::SetPinValue( false, false );

//			FrequencyUpdateOutputPin::SetPinValue( true, false );
//			FrequencyUpdateOutputPin::SetPinValue( false, false );
			if( Enabled() )
				UpdateFrequencyAndPhase( true );

			else
				PowerDown();

		}

		void UpdateFrequencyAndPhase( bool AUpdateFrequency )
		{
//			unsigned char i,w;
//			long int y;
//			double x;
			uint8_t w = ( 0b00011111 * Phase() ) + 0.5;
			w <<= 3;

			SendByte( w );
	  
			if( AUpdateFrequency )
			{
				//Calculate the frequency of the HEX value
				float x = 4294967295 / ( ReferenceClockFrequency() / 1000000 ) ;//Suitable for 125M Crystal 
				float AFrequency = Frequency() / 1000000;
				AFrequency *= x;
				uint32_t	y = AFrequency;

	//			Serial.println( y );

				for( int i = 0; i < 4; ++i )
					SendByte( y >> 8 * ( 3 - i ));

			}

/*
			for( int i = 0; i < 32; ++i )
			{
				DataOutputPin::SetPinValue( ( y >> i ) & 0x01, false );
				WordLoadClockOutputPin::SetPinValue( true, false );
				WordLoadClockOutputPin::SetPinValue( false, false );
			}
*/
			T_FrequencyUpdateOutputPin::SetPinValue( true, false );
			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
		}

		void PowerDown()
		{
			SendByte( 0b11111100 );

			T_FrequencyUpdateOutputPin::SetPinValue( true, false );
			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
		}

		void PowerUp()
		{
			SendByte( 0 );

			T_FrequencyUpdateOutputPin::SetPinValue( true, false );
			T_FrequencyUpdateOutputPin::SetPinValue( false, false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

