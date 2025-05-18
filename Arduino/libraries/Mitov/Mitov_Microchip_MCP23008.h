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
	namespace Microchip_MCP23008_Const
	{
		const uint8_t	REG_IODIR	= 0x00;
		const uint8_t	REG_IPOL	= 0x01;
		const uint8_t	REG_GPPU	= 0x06;
		const uint8_t	REG_GPIO	= 0x09;
		const uint8_t	REG_OLAT	= 0x0A;
	}
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_Channels_GetDirection,
		typename T_Channels_GetInvert,
		typename T_Channels_GetPullUp,
		typename T_Channels_GetValue,
		typename T_Channels_HasInputs,
		typename T_Channels_SendOutput,
		typename T_FModifiedDirection,
        typename T_FModifiedInvert,
		typename T_FModifiedPullUp,
		typename T_FModifiedValue
	> class Microchip_MCP23008 :
		public T_IMPLEMENTATION,
		public T_FModifiedDirection,
        public T_FModifiedInvert,
		public T_FModifiedPullUp,
		public T_FModifiedValue
	{
	public:
		_V_PROP_( FModifiedDirection )
        _V_PROP_( FModifiedInvert )
		_V_PROP_( FModifiedPullUp )
		_V_PROP_( FModifiedValue )

		void UpdateAll( bool AForce )
		{
            if( AForce || FModifiedDirection().GetValue() )
            {
                uint8_t    AValue = 0xFF;

                T_Channels_GetDirection::Call( AValue );

                T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23008_Const::REG_IODIR, AValue );

                FModifiedDirection() = false;
            }

            if( AForce || FModifiedInvert().GetValue() )
            {
                uint8_t    AValue = 0;

                T_Channels_GetInvert::Call( AValue );

                T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23008_Const::REG_IPOL, AValue );

                FModifiedInvert() = false;
            }

            if( AForce || FModifiedPullUp().GetValue() )
            {
                uint8_t    AValue = 0;

                T_Channels_GetPullUp::Call( AValue );

                T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23008_Const::REG_GPPU, AValue );

                FModifiedPullUp() = false;
            }

//            Serial.println( FModifiedValueA().GetValue() );
            if( AForce || FModifiedValue().GetValue() )
            {
                uint8_t    AValue = 0;

                T_Channels_GetValue::Call( AValue );

                T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23008_Const::REG_OLAT, AValue );

                FModifiedValue() = false;
            }
		}

	public:
		uint8_t PerformDirectRead()
		{
		    bool AValue = false;
		    T_Channels_HasInputs::Call( AValue );
		    if( ! AValue )
		        return 0;

			return T_IMPLEMENTATION::ReadRegister8( Microchip_MCP23008_Const::REG_GPIO );
		}

		void PerformRead()
		{
			uint8_t AReadBits = PerformDirectRead();
			T_Channels_SendOutput::Call( AReadBits );
		}

	public:
		inline void SystemLoopBegin()
		{
			PerformRead();
		}

		inline void SystemStart()
		{
			UpdateAll( true );
		}

		inline void SystemLoopUpdateHardware()
		{
//		    Serial.println( "SystemLoopUpdateHardware()" );
			UpdateAll( false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_IsCombinedInOut,
		typename T_IsInverted,
		typename T_IsOutput,
		typename T_IsPullUp,
		typename T_OutputPin
	> class Microchip_MCP23008_Channel :
		public T_InitialValue,
		public T_IsCombinedInOut,
		public T_IsInverted,
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
        _V_PROP_( IsInverted )

	public:
		inline void SetMode( int AMode ) // Placeholder for direct pin control!
		{
//			pinMode( FPinNumber, AMode );
		}

		inline void SetInitialMode( int AMode ) // Placeholder for direct pin control!
		{
		}

		inline bool DigitalRead()
		{
//			Serial.println( "TEST1" );
			return (( C_OWNER.PerformDirectRead() & ( 1 << C_INDEX )) != 0 );
//			Serial.println( FInValue );
//			return C_OWNER.GetBitValue( C_INDEX );
		}

		inline void DigitalWrite( bool AValue )
		{
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;

//            Serial.println( "DigitalWrite" );

            C_OWNER.FModifiedValue() = true;
		}

		inline void SendOutput( uint8_t AValue )
		{
			T_OutputPin::SetPinValue( ( AValue & ( 1 << C_INDEX )) != 0 );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			DigitalWrite( *(bool *)_Data );
		}

	public:
        inline void PinDirectionsInit()
        {
        }

        inline void Update_IsInverted()
        {
            C_OWNER.FModifiedInvert() = true;
        }

        inline void Update_IsOutput()
        {
            C_OWNER.FModifiedDirection() = true;
        }

        inline void Update_IsPullUp()
        {
            C_OWNER.FModifiedPullUp() = true;
        }

	public:
		inline void SystemStart()
		{
		}

		inline void SystemLoopBegin()
		{
		}

	public:
		inline void GetDirection( uint8_t & AValue )
		{
		    if( IsOutput() )
		        AValue &= ~( 1 << C_INDEX );

		}

		inline void GetPullUp( uint8_t & AValue )
		{
		    if( IsPullUp() )
		        AValue |= ( 1 << C_INDEX );

		}

		inline void GetInvert( uint8_t & AValue )
		{
		    if( IsInverted() )
		        AValue |= ( 1 << C_INDEX );

		}

		inline void GetValue( uint8_t & AValue )
		{
		    if( InitialValue() )
		        AValue |= ( 1 << C_INDEX );

		}

		inline void HasInputs( bool & AValue )
		{
		    if( AValue )
		        return;

	        if( T_OutputPin::GetPinIsConnected() )
                AValue = true;

		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif