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
	enum TArduinoAD7606Oversampling { aosNone, aos2, aos4, aos8, aos16, aos32, aos64 };
//---------------------------------------------------------------------------
	template <
		uint16_t C_BusyInputPin,
		typename T_ChipSelectOutputPin,
		uint16_t C_DataInputPins0,
		uint16_t C_DataInputPins1,
		uint16_t C_DataInputPins10,
		uint16_t C_DataInputPins11,
		uint16_t C_DataInputPins12,
		uint16_t C_DataInputPins13,
		uint16_t C_DataInputPins14,
		uint16_t C_DataInputPins15,
		uint16_t C_DataInputPins2,
		uint16_t C_DataInputPins3,
		uint16_t C_DataInputPins4,
		uint16_t C_DataInputPins5,
		uint16_t C_DataInputPins6,
		uint16_t C_DataInputPins7,
		uint16_t C_DataInputPins8,
		uint16_t C_DataInputPins9,
		typename T_Enabled,
		typename T_MostSignificantFirst,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3,
		typename T_OutputPins_4,
		typename T_OutputPins_5,
		typename T_OutputPins_6,
		typename T_OutputPins_7,
		typename T_Oversampling,
		typename T_OversamplingOutputPins_0,
		typename T_OversamplingOutputPins_1,
		typename T_OversamplingOutputPins_2,
		typename T_ReadOutputPin,
		typename T_ResetOutputPin,
		typename T_StartOutputPin
	> class AD7606Parallel16bit :
		public T_ChipSelectOutputPin,
		public T_Enabled,
		public T_MostSignificantFirst,
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3,
		public T_OutputPins_4,
		public T_OutputPins_5,
		public T_OutputPins_6,
		public T_OutputPins_7,
		public T_Oversampling,
		public T_OversamplingOutputPins_0,
		public T_OversamplingOutputPins_1,
		public T_OversamplingOutputPins_2,
		public T_ReadOutputPin,
		public T_ResetOutputPin,
		public T_StartOutputPin
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

		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( OversamplingOutputPins_0 )
		_V_PIN_( OversamplingOutputPins_1 )
		_V_PIN_( OversamplingOutputPins_2 )
		_V_PIN_( ReadOutputPin )
		_V_PIN_( ResetOutputPin )
		_V_PIN_( StartOutputPin )

	public:
		_V_PROP_( Enabled )

		_V_PROP_( Oversampling )

	protected:
//		uint16_t FPins[ 16 + 5 ];

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

//			Serial.println( "READ" );
//			T_ChipSelectOutputPin::SetPinValueLow();
			T_StartOutputPin::SetPinValueLow();
//			digitalWrite( C_START_PIN, LOW );

			unsigned long ACurrentTime = micros();

			T_StartOutputPin::SetPinValueHigh();
//			digitalWrite( C_START_PIN, HIGH );

//			Serial.println( "WAIT" );
			while( micros() - ACurrentTime < 4 ); // Wait 4 uS
//			Serial.println( "WAIT1" );
			while( digitalRead( C_BusyInputPin ) != LOW ); // Wait to finish reading
//			Serial.println( "WAIT2" );

			T_ChipSelectOutputPin::SetPinValueLow();
//			digitalWrite( C_CHIP_SELECT_PIN, LOW );

			for( int AChannel = 0; AChannel < 8; ++AChannel )
			{
				T_ReadOutputPin::SetPinValueLow();
//				digitalWrite( C_READ_PIN, LOW );

				uint16_t AResult = 0;
//				for( int i = 0; i < 16; ++i )
//					if( digitalRead( FPins[ i ] ) == HIGH )
//						AResult |= ( 1 << i );

				if( digitalRead( C_DataInputPins0 ) == HIGH )
					AResult |= ( 1 << 0 );
					
				if( digitalRead( C_DataInputPins1 ) == HIGH )
					AResult |= ( 1 << 1 );
					
				if( digitalRead( C_DataInputPins2 ) == HIGH )
					AResult |= ( 1 << 2 );
					
				if( digitalRead( C_DataInputPins3 ) == HIGH )
					AResult |= ( 1 << 3 );
					
				if( digitalRead( C_DataInputPins4 ) == HIGH )
					AResult |= ( 1 << 4 );
					
				if( digitalRead( C_DataInputPins5 ) == HIGH )
					AResult |= ( 1 << 5 );
					
				if( digitalRead( C_DataInputPins6 ) == HIGH )
					AResult |= ( 1 << 6 );
					
				if( digitalRead( C_DataInputPins7 ) == HIGH )
					AResult |= ( 1 << 7 );

				if( digitalRead( C_DataInputPins8 ) == HIGH )
					AResult |= ( 1 << 8 );

				if( digitalRead( C_DataInputPins9 ) == HIGH )
					AResult |= ( 1 << 9 );

				if( digitalRead( C_DataInputPins10 ) == HIGH )
					AResult |= ( 1 << 10 );

				if( digitalRead( C_DataInputPins11 ) == HIGH )
					AResult |= ( 1 << 11 );

				if( digitalRead( C_DataInputPins12 ) == HIGH )
					AResult |= ( 1 << 12 );

				if( digitalRead( C_DataInputPins13 ) == HIGH )
					AResult |= ( 1 << 13 );

				if( digitalRead( C_DataInputPins14 ) == HIGH )
					AResult |= ( 1 << 14 );

				if( digitalRead( C_DataInputPins15 ) == HIGH )
					AResult |= ( 1 << 15 );

				T_ReadOutputPin::SetPinValueHigh();
//				digitalWrite( C_READ_PIN, HIGH );

//				if( ! AChannel )
//					Serial.println( AResult, HEX );

				float AValue = float( int16_t( AResult )) / 0x7FFF;
//				OutputPins[ AChannel ].Notify( &AValue );
				switch( AChannel )
				{
					case 0: T_OutputPins_0::SetPinValue( AValue ); break;
					case 1: T_OutputPins_1::SetPinValue( AValue ); break;
					case 2: T_OutputPins_2::SetPinValue( AValue ); break;
					case 3: T_OutputPins_3::SetPinValue( AValue ); break;
					case 4: T_OutputPins_4::SetPinValue( AValue ); break;
					case 5: T_OutputPins_5::SetPinValue( AValue ); break;
					case 6: T_OutputPins_6::SetPinValue( AValue ); break;
					case 7: T_OutputPins_7::SetPinValue( AValue ); break;
				}
			}

			T_ChipSelectOutputPin::SetPinValueHigh();
//			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
		}

		void ResetInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "RESET" );
			T_ResetOutputPin::SetPinValueHigh();
//			digitalWrite( C_RESET_PIN, HIGH );
			delay( 1 );
			T_ResetOutputPin::SetPinValueLow();
//			digitalWrite( C_RESET_PIN, LOW );
		}

	public:
		inline void SystemStart()
		{
//			for( int i = 0; i < 16; ++ i )
//				pinMode( FPins[ i ], INPUT );

			pinMode( C_DataInputPins0,  INPUT );
			pinMode( C_DataInputPins1,  INPUT );
			pinMode( C_DataInputPins2,  INPUT );
			pinMode( C_DataInputPins3,  INPUT );
			pinMode( C_DataInputPins4,  INPUT );
			pinMode( C_DataInputPins5,  INPUT );
			pinMode( C_DataInputPins6,  INPUT );
			pinMode( C_DataInputPins7,  INPUT );
			pinMode( C_DataInputPins8,  INPUT );
			pinMode( C_DataInputPins9,  INPUT );
			pinMode( C_DataInputPins10, INPUT );
			pinMode( C_DataInputPins11, INPUT );
			pinMode( C_DataInputPins12, INPUT );
			pinMode( C_DataInputPins13, INPUT );
			pinMode( C_DataInputPins14, INPUT );
			pinMode( C_DataInputPins15, INPUT );

//			for( int i = 16; i < 16 + 4; ++ i )
//				pinMode( FPins[ i ], OUTPUT );

//			pinMode( C_CHIP_SELECT_PIN, OUTPUT );
//			pinMode( C_READ_PIN, OUTPUT );
//			pinMode( C_RESET_PIN, OUTPUT );
//			pinMode( C_START_PIN, OUTPUT );

			T_ChipSelectOutputPin::SetPinValueHigh();
//			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
  			T_ReadOutputPin::SetPinValueHigh();
//			digitalWrite( C_READ_PIN, HIGH );
			T_StartOutputPin::SetPinValueHigh();
//			digitalWrite( C_START_PIN, HIGH );
			T_ResetOutputPin::SetPinValueLow();
//			digitalWrite( C_RESET_PIN, LOW );

			pinMode( C_BusyInputPin, INPUT );
			
//			for( int i = 0; i < 3; ++i )
//				OversamplingOutputPins[ i ].SendValue<bool>( Oversampling & ( 1 << i ) != 0 );
			
			T_OversamplingOutputPins_0::SetPinValue( ( Oversampling().GetValue() & ( 1 )) != 0 );
			T_OversamplingOutputPins_1::SetPinValue( ( Oversampling().GetValue() & ( 1 << 1 )) != 0 );
			T_OversamplingOutputPins_2::SetPinValue( ( Oversampling().GetValue() & ( 1 << 2 )) != 0 );

			ResetInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint16_t C_BusyInputPin,
		typename T_ChipSelectOutputPin,
		uint16_t C_DataInputPins0,
		uint16_t C_DataInputPins1,
		uint16_t C_DataInputPins10,
		uint16_t C_DataInputPins11,
		uint16_t C_DataInputPins12,
		uint16_t C_DataInputPins13,
		uint16_t C_DataInputPins14,
		uint16_t C_DataInputPins15,
		uint16_t C_DataInputPins2,
		uint16_t C_DataInputPins3,
		uint16_t C_DataInputPins4,
		uint16_t C_DataInputPins5,
		uint16_t C_DataInputPins6,
		uint16_t C_DataInputPins7,
		uint16_t C_DataInputPins8,
		uint16_t C_DataInputPins9,
		typename T_Enabled,
		typename T_MostSignificantFirst,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3,
		typename T_OutputPins_4,
		typename T_OutputPins_5,
		typename T_OutputPins_6,
		typename T_OutputPins_7,
		typename T_Oversampling,
		typename T_OversamplingOutputPins_0,
		typename T_OversamplingOutputPins_1,
		typename T_OversamplingOutputPins_2,
		typename T_ReadOutputPin,
		typename T_ResetOutputPin,
		typename T_StartOutputPin
	> class AD7606Parallel8bit :
		public T_ChipSelectOutputPin,
		public T_Enabled,
		public T_MostSignificantFirst,
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3,
		public T_OutputPins_4,
		public T_OutputPins_5,
		public T_OutputPins_6,
		public T_OutputPins_7,
		public T_Oversampling,
		public T_OversamplingOutputPins_0,
		public T_OversamplingOutputPins_1,
		public T_OversamplingOutputPins_2,
		public T_ReadOutputPin,
		public T_ResetOutputPin,
		public T_StartOutputPin
	{
//	protected:
//		static const uint8_t RESET_PIN = 8;
//		static const uint8_t CHIP_SELECT_PIN = 9;
//		static const uint8_t READ_PIN = 10;
//		static const uint8_t START_PIN = 11;
//		static const uint8_t BUSY_PIN = 12;

	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )
		_V_PIN_( OutputPins_4 )
		_V_PIN_( OutputPins_5 )
		_V_PIN_( OutputPins_6 )
		_V_PIN_( OutputPins_7 )

		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( OversamplingOutputPins_0 )
		_V_PIN_( OversamplingOutputPins_1 )
		_V_PIN_( OversamplingOutputPins_2 )
		_V_PIN_( ReadOutputPin )
		_V_PIN_( ResetOutputPin )
		_V_PIN_( StartOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( MostSignificantFirst )

		_V_PROP_( Oversampling )

//	protected:
//		uint16_t FPins[ 8 + 5 ];

	protected:
		uint16_t WriteMostSignificant()
		{
			uint16_t AResult = 0;

			if( digitalRead( C_DataInputPins0 ) == HIGH )
				AResult |= ( 1 << ( 8 + 0 ));
					
			if( digitalRead( C_DataInputPins1 ) == HIGH )
				AResult |= ( 1 << ( 8 + 1 ));
					
			if( digitalRead( C_DataInputPins2 ) == HIGH )
				AResult |= ( 1 << ( 8 + 2 ));
					
			if( digitalRead( C_DataInputPins3 ) == HIGH )
				AResult |= ( 1 << ( 8 + 3 ));
					
			if( digitalRead( C_DataInputPins4 ) == HIGH )
				AResult |= ( 1 << ( 8 + 4 ));
					
			if( digitalRead( C_DataInputPins5 ) == HIGH )
				AResult |= ( 1 << ( 8 + 5 ));
					
			if( digitalRead( C_DataInputPins6 ) == HIGH )
				AResult |= ( 1 << ( 8 + 6 ));
					
			if( digitalRead( C_DataInputPins7 ) == HIGH )
				AResult |= ( 1 << ( 8 + 7 ));
					
			return AResult;
		}

		uint16_t WriteLeastSignificant()
		{
			uint16_t AResult = 0;

			if( digitalRead( C_DataInputPins0 ) == HIGH )
				AResult |= ( 1 << 0 );
					
			if( digitalRead( C_DataInputPins1 ) == HIGH )
				AResult |= ( 1 << 1 );
					
			if( digitalRead( C_DataInputPins2 ) == HIGH )
				AResult |= ( 1 << 2 );
					
			if( digitalRead( C_DataInputPins3 ) == HIGH )
				AResult |= ( 1 << 3 );
					
			if( digitalRead( C_DataInputPins4 ) == HIGH )
				AResult |= ( 1 << 4 );
					
			if( digitalRead( C_DataInputPins5 ) == HIGH )
				AResult |= ( 1 << 5 );
					
			if( digitalRead( C_DataInputPins6 ) == HIGH )
				AResult |= ( 1 << 6 );
					
			if( digitalRead( C_DataInputPins7 ) == HIGH )
				AResult |= ( 1 << 7 );

			return AResult;
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

//			Serial.println( "READ" );
			T_StartOutputPin::SetPinValueLow();
//			digitalWrite( C_START_PIN, LOW );

			unsigned long ACurrentTime = micros();

			T_StartOutputPin::SetPinValueHigh();
//			digitalWrite( C_START_PIN, HIGH );

//			Serial.println( "WAIT" );
			while( micros() - ACurrentTime < 4 ); // Wait 4 uS
//			Serial.println( "WAIT1" );
			while( digitalRead( C_BusyInputPin ) != LOW ); // Wait to finish reading
//			Serial.println( "WAIT2" );

			T_ChipSelectOutputPin::SetPinValueLow();
//			digitalWrite( C_CHIP_SELECT_PIN, LOW );

			for( int AChannel = 0; AChannel < 8; ++AChannel )
			{
				uint16_t AResult = 0;

				T_ReadOutputPin::SetPinValueLow();
//				digitalWrite( C_READ_PIN, LOW );

				if( MostSignificantFirst() )
				{
					AResult |= WriteMostSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << ( 8 + i ));
				}

				else
				{
					AResult |= WriteLeastSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << i );
				}

				T_ReadOutputPin::SetPinValueHigh();
//				digitalWrite( C_READ_PIN, HIGH );

				delay( 1 );

				T_ReadOutputPin::SetPinValueLow();
//				digitalWrite( C_READ_PIN, LOW );

				if( MostSignificantFirst() )
				{
					AResult |= WriteLeastSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << i );
				}

				else
				{
					AResult |= WriteMostSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << ( 8 + i ));
				}

				T_ReadOutputPin::SetPinValueHigh();
//				digitalWrite( C_READ_PIN, HIGH );

//				if( ! AChannel )
//					Serial.println( AResult, HEX );

				float AValue = float( int16_t( AResult )) / 0x7FFF;
//				OutputPins[ AChannel ].Notify( &AValue );
				switch( AChannel )
				{
					case 0: T_OutputPins_0::SetPinValue( AValue ); break;
					case 1: T_OutputPins_1::SetPinValue( AValue ); break;
					case 2: T_OutputPins_2::SetPinValue( AValue ); break;
					case 3: T_OutputPins_3::SetPinValue( AValue ); break;
					case 4: T_OutputPins_4::SetPinValue( AValue ); break;
					case 5: T_OutputPins_5::SetPinValue( AValue ); break;
					case 6: T_OutputPins_6::SetPinValue( AValue ); break;
					case 7: T_OutputPins_7::SetPinValue( AValue ); break;
				}
			}

            T_ChipSelectOutputPin::SetPinValueHigh();
//			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
		}

		void ResetInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "RESET" );
            T_ResetOutputPin::SetPinValueHigh();
//			digitalWrite( C_RESET_PIN, HIGH );
			delay( 1 );
            T_ResetOutputPin::SetPinValueLow();
//			digitalWrite( C_RESET_PIN, LOW );
		}

	public:
		inline void SystemStart()
		{
//			for( int i = 0; i < 8; ++ i )
//				pinMode( FPins[ i ], INPUT );

			pinMode( C_DataInputPins0, INPUT );
			pinMode( C_DataInputPins1, INPUT );
			pinMode( C_DataInputPins2, INPUT );
			pinMode( C_DataInputPins3, INPUT );
			pinMode( C_DataInputPins4, INPUT );
			pinMode( C_DataInputPins5, INPUT );
			pinMode( C_DataInputPins6, INPUT );
			pinMode( C_DataInputPins7, INPUT );

//			for( int i = 8; i < 8 + 4; ++ i )
//				pinMode( FPins[ i ], OUTPUT );

//			pinMode( C_CHIP_SELECT_PIN, OUTPUT );
//			pinMode( C_READ_PIN, OUTPUT );
//			pinMode( C_RESET_PIN, OUTPUT );
//			pinMode( C_START_PIN, OUTPUT );

            T_ChipSelectOutputPin::SetPinValueHigh();
//			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
            T_ReadOutputPin::SetPinValueHigh();
//			digitalWrite( C_READ_PIN, HIGH );
			T_StartOutputPin::SetPinValueHigh();
//			digitalWrite( C_START_PIN, HIGH );
			T_ResetOutputPin::SetPinValueLow();
//			digitalWrite( C_RESET_PIN, LOW );

			pinMode( C_BusyInputPin, INPUT );
			
//			for( int i = 0; i < 3; ++i )
//				OversamplingOutputPins[ i ].SendValue<bool>( Oversampling & ( 1 << i ) != 0 );
			
			T_OversamplingOutputPins_0::SetPinValue( ( Oversampling().GetValue() & ( 1 )) != 0 );
			T_OversamplingOutputPins_1::SetPinValue( ( Oversampling().GetValue() & ( 1 << 1 )) != 0 );
			T_OversamplingOutputPins_2::SetPinValue( ( Oversampling().GetValue() & ( 1 << 2 )) != 0 );

			ResetInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif