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
#include <SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Enabled,
		typename T_FModifiedMap,
		typename T_FModifiedPorts
	> class Maxim_MAX7300 :
		public T_Address,
		public T_Enabled,
		public T_FModifiedMap,
		public T_FModifiedPorts
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FModifiedMap )
		_V_PROP_( FModifiedPorts )

		uint8_t		FConfigMap[ 7 ][ 2 ];
		uint32_t    FPorts[ 2 ];
		uint32_t    FReadMap = 0;
		uint32_t    FReadRegistersMap = 0;
		uint32_t    FReadBits;

//	public:
//		BasicMicrochip_MAX7300_Channel *FChannels[ C_CHANNELS_COUNT ];

	protected:
		void BeginTransmissionAt( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));

			C_I2C.write( ARegister );
		}

		void WriteRegister( uint8_t ARegister, uint8_t AValue )
		{
//			Serial.print( "WriteRegister: " ); Serial.print( ARegister ); Serial.print( " " ); Serial.println( AValue );

			BeginTransmissionAt( ARegister );
			C_I2C.write( AValue );
			C_I2C.endTransmission();
		}

		void UpdateEnable()
		{
			if( Enabled() )
				WriteRegister( 4, 1 );

			else
				WriteRegister( 4, 0 );

		}

		void UpdateConfigMap()
		{
//			Serial.println( "UpdateConfigMap" );
			bool AInUpdate = false;
			for( int i = 0; i < 7; ++i )
			{
				if( FConfigMap[ i ][ 0 ] != FConfigMap[ i ][ 1 ] )
				{
					if( ! AInUpdate )
					{
//						Serial.print( "BeginTransmissionAt: " ); Serial.println( 9 + i );
						BeginTransmissionAt( 9 + i );
						AInUpdate = true;
					}

//					Serial.println( i );
//					Serial.println( FConfigMap[ i ][ 0 ] );

					C_I2C.write( FConfigMap[ i ][ 0 ] );
					FConfigMap[ i ][ 1 ] = FConfigMap[ i ][ 0 ];
				}

				else if( AInUpdate )
				{
					C_I2C.endTransmission();
					AInUpdate = false;
				}
			}

			if( AInUpdate )
				C_I2C.endTransmission();

			FReadRegistersMap = 0;
			for( int i = 0; i <= 0x58 - 0x44; ++i )
				if( FReadMap & ((uint32_t)0xFF) << i )
				{
					FReadRegistersMap |= ((uint32_t)1 ) << i;
					i += 7;
				}

//			Serial.print( "FReadRegistersMap: " ); Serial.println( FReadRegistersMap );

			FModifiedMap() = false;
		}

		void UpdatePorts()
		{
//			Serial.println( "UpdatePorts" );
//			Serial.println( FPorts[ 0 ] );
//			Serial.println( FPorts[ 1 ] );
			for( int i = 0; i < 28; ++i )
			{
				uint32_t AMask = ((uint32_t)1) << i;
//				Serial.println( AMask );
				if( ( FPorts[ 0 ] & AMask ) != ( FPorts[ 1 ] & AMask ) )
				{
//   					Serial.println( i );
//					Serial.println( AMask );
					WriteRegister( 0x44 + i, FPorts[ 0 ] >> i );
					i += 8;
//					for( int j = i / 8; j < 4; j ++ )
//						WriteRegister( 0x44 + j * 8, FPorts[ 0 ] >> j * 8 );

//					break;
				}
			}
			
			FPorts[ 1 ] = FPorts[ 0 ];
			FModifiedPorts() = false;
		}

	public:
		inline void SystemStart()
		{
			UpdateEnable();

			for( int i = 0; i < 7; ++i )
				FConfigMap[ i ][ 1 ] = ~FConfigMap[ i ][ 0 ];

			FPorts[ 1 ] = ~FPorts[ 0 ];

			UpdateConfigMap();
			UpdatePorts();

//			UpdateOutput();
		}

		inline void SystemLoopBegin()
		{
//			if( ! ReadInputPin.IsConnected() )
				PerformRead();
		}

		inline void SystemLoopUpdateHardware()
		{
			if( FModifiedMap() )
//				if( ! ClockInputPin.IsConnected() )
				UpdateConfigMap();

			if( FModifiedPorts() )
				UpdatePorts();

		}

	public:
		inline bool GetBitValue( uint32_t AIndex )
		{
			return( ( FReadBits & ( ((uint32_t)1 ) << AIndex )) != 0 );
		}

		void SetChannelValue( int AIndex, bool AValue )
		{
//			AIndex += 4;

//			Serial.println( "SetChannelValue" );
//			Serial.println( AIndex );
//			Serial.println( AValue );

			FPorts[ 0 ] &= ~( ((uint32_t)0b1 ) << AIndex );
			FPorts[ 0 ] |= ( ((uint32_t)AValue ) & 1 ) << AIndex;
			FModifiedPorts() = true;
		}

		void SetChannelMode( int AIndex, int AMode )
		{
//			AIndex += 4;

//			Serial.println( "SetChannelMode" );
//			Serial.println( AIndex );
//			Serial.println( AMode );

			int AConfigIndex = AIndex / 4;
			int AOffset = ( AIndex % 4 ) * 2;

//			Serial.println( AConfigIndex );
//			Serial.println( AOffset );

			FConfigMap[ AConfigIndex ][ 0 ] &= ~( ((uint32_t)0b11) << AOffset );
			FConfigMap[ AConfigIndex ][ 0 ] |= ((uint32_t)AMode) << AOffset;

			if( AMode & 0b10 ) // Check if it is input
				FReadMap |= ((uint32_t)1) << AIndex;

			else
				FReadMap &= ~( ((uint32_t)1) << AIndex );

			FModifiedMap() = true;
		}

	public:
		void PerformRead()
		{
			if( ! FReadRegistersMap )
				return;

			FReadBits = 0;
			for( int i = 0; i <= 0x58 - 0x44; ++i )
				if( (((uint32_t)1 ) << i ) & FReadRegistersMap )
				{
					BeginTransmissionAt( 0x44 + i );
					C_I2C.endTransmission();
					C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ) );
					while (C_I2C.available() < 1 );

					uint32_t ARegister = C_I2C.read();

					FReadBits |= ARegister << i;

//					Serial.print( i ); Serial.print( ": " ); Serial.println( ARegister );

					i += 7;
				}

//			for( int i = 0; i < C_CHANNELS_COUNT; ++i )
//				FChannels[ i ]->UpdateOutput( GetBitValue( i ));

		}

	public:
		inline Maxim_MAX7300()
		{
			FModifiedPorts() = false;
			FModifiedMap() = false;

			for( int i = 0; i < 7; ++i )
				FConfigMap[ i ][ 0 ] = 0b10101010;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_IsCombinedInOut,
		typename T_IsOutput,
		typename T_IsPullUp,
		typename T_OutputPin
	> class Maxim_MAX7300_Channel :
		public T_InitialValue,
		public T_IsCombinedInOut,
		public T_IsOutput,
		public T_IsPullUp,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialValue )
        _V_PROP_( IsOutput )
        _V_PROP_( IsPullUp )

	public:
        void PinDirectionsInit()
        {
            if( IsOutput() )
			{
                C_OWNER.SetChannelMode( C_INDEX, 1 ); //pinMode( FPinNumber, OUTPUT );
				C_OWNER.SetChannelValue( C_INDEX, InitialValue() );
			}

            else
            {
                if( IsPullUp() )
                    C_OWNER.SetChannelMode( C_INDEX, 3 ); //pinMode( FPinNumber, INPUT_PULLUP );

                else
                    C_OWNER.SetChannelMode( C_INDEX, 2 ); //pinMode( FPinNumber, INPUT );
            }
        }

	public:
		inline void SetMode( int AMode ) // Placeholder for direct pin control!
		{
//			pinMode( FPinNumber, AMode );
		}

		inline void SetInitialMode( int AMode ) // Placeholder for direct pin control!
		{
//			SetMode( AMode );
		}

		bool DigitalRead()
		{
//			Serial.println( "TEST1" );
			C_OWNER.PerformRead();
//			Serial.println( FInValue );
			return C_OWNER.GetBitValue( C_INDEX );
		}

		inline void DigitalWrite( bool AValue )
		{
			InitialValue() = AValue;
			if( IsOutput() )
				C_OWNER.SetChannelValue( C_INDEX, AValue );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			DigitalWrite( *(bool *)_Data );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue() );
		}

		inline void SystemLoopBegin()
		{
			bool AValue = C_OWNER.GetBitValue( C_INDEX );
			T_OutputPin::SetPinValue( AValue );
		}

	public:
//		Maxim_MAX7300_Channel( bool AInitialValue, bool AIsOutput, bool AIsCombinedInOut, bool AIsPullUp ) :
//			IsOutput( AIsOutput ),
//			IsPullUp( AIsPullUp ),
//			FValue( AInitialValue )
		inline Maxim_MAX7300_Channel()
		{
			PinDirectionsInit();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif