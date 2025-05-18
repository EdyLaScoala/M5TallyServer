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
	namespace Microchip_MCP23017_Const
	{
		const uint8_t	REG_IODIRA	= 0x00;
		const uint8_t	REG_IODIRB	= 0x01;
		const uint8_t	REG_IPOLA	= 0x02;
		const uint8_t	REG_IPOLB	= 0x03;
		const uint8_t	REG_GPPUA	= 0x0C;
		const uint8_t	REG_GPPUB	= 0x0D;
		const uint8_t	REG_GPIOA	= 0x12;
		const uint8_t	REG_GPIOB	= 0x13;
		const uint8_t	REG_OLATA	= 0x14;
		const uint8_t	REG_OLATB	= 0x15;
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
		typename T_FModifiedDirectionA,
		typename T_FModifiedDirectionB,
        typename T_FModifiedInvertA,
		typename T_FModifiedInvertB,
		typename T_FModifiedPullUpA,
		typename T_FModifiedPullUpB,
		typename T_FModifiedValueA,
		typename T_FModifiedValueB
	> class Microchip_MCP23017 :
		public T_IMPLEMENTATION,
		public T_FModifiedDirectionA,
		public T_FModifiedDirectionB,
        public T_FModifiedInvertA,
		public T_FModifiedInvertB,
		public T_FModifiedPullUpA,
		public T_FModifiedPullUpB,
		public T_FModifiedValueA,
		public T_FModifiedValueB
	{
	public:
		_V_PROP_( FModifiedDirectionA )
		_V_PROP_( FModifiedDirectionB )
        _V_PROP_( FModifiedInvertA )
		_V_PROP_( FModifiedInvertB )
		_V_PROP_( FModifiedPullUpA )
		_V_PROP_( FModifiedPullUpB )
		_V_PROP_( FModifiedValueA )
		_V_PROP_( FModifiedValueB )

		void UpdateAll( bool AForce )
		{
            if( AForce || FModifiedDirectionA().GetValue() || FModifiedDirectionB().GetValue() )
            {
                uint16_t    AValue = 0xFFFF;

                T_Channels_GetDirection::Call( AValue );

    		    if( AForce || FModifiedDirectionA().GetValue() && FModifiedDirectionB().GetValue() )
                    T_IMPLEMENTATION::WriteRegister16( Microchip_MCP23017_Const::REG_IODIRA, AValue );

    	        else if( FModifiedDirectionA().GetValue() )
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_IODIRA, AValue );

    	        else
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_IODIRB, AValue >> 8 );

                FModifiedDirectionA() = false;
                FModifiedDirectionB() = false;
            }

            if( AForce || FModifiedInvertA().GetValue() || FModifiedInvertB().GetValue() )
            {
                uint16_t    AValue = 0;

                T_Channels_GetInvert::Call( AValue );

    		    if( AForce || FModifiedInvertA().GetValue() && FModifiedInvertB().GetValue() )
                    T_IMPLEMENTATION::WriteRegister16( Microchip_MCP23017_Const::REG_IPOLA, AValue );

    	        else if( FModifiedInvertA().GetValue() )
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_IPOLA, AValue );

    	        else
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_IPOLB, AValue >> 8 );

                FModifiedInvertA() = false;
                FModifiedInvertB() = false;
            }

            if( AForce || FModifiedPullUpA().GetValue() || FModifiedPullUpB().GetValue() )
            {
                uint16_t    AValue = 0;

                T_Channels_GetPullUp::Call( AValue );

    		    if( AForce || FModifiedPullUpA().GetValue() && FModifiedPullUpB().GetValue() )
                    T_IMPLEMENTATION::WriteRegister16( Microchip_MCP23017_Const::REG_GPPUA, AValue );

    	        else if( FModifiedPullUpA().GetValue() )
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_GPPUA, AValue );

    	        else
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_GPPUB, AValue >> 8 );

                FModifiedPullUpA() = false;
                FModifiedPullUpB() = false;
            }

//            Serial.println( FModifiedValueA().GetValue() );
            if( AForce || FModifiedValueA().GetValue() || FModifiedValueB().GetValue() )
            {
                uint16_t    AValue = 0;

                T_Channels_GetValue::Call( AValue );

    		    if( AForce || FModifiedValueA().GetValue() && FModifiedValueB().GetValue() )
                    T_IMPLEMENTATION::WriteRegister16( Microchip_MCP23017_Const::REG_OLATA, AValue );

    	        else if( FModifiedValueA().GetValue() )
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_OLATA, AValue );

    	        else
                    T_IMPLEMENTATION::WriteRegister8( Microchip_MCP23017_Const::REG_OLATB, AValue >> 8 );

                FModifiedValueA() = false;
                FModifiedValueB() = false;
            }
		}

	public:
		uint16_t PerformDirectRead()
		{
		    bool AValueA = false;
		    bool AValueB = false;
		    T_Channels_HasInputs::Call( AValueA, AValueB );
		    if( ! ( AValueA || AValueB ))
		        return 0;

		    if( AValueA && AValueB )
				return T_IMPLEMENTATION::ReadRegister16( Microchip_MCP23017_Const::REG_GPIOA );

		    else if( AValueA )
				return T_IMPLEMENTATION::ReadRegister8( Microchip_MCP23017_Const::REG_GPIOA );

		    else
				return T_IMPLEMENTATION::ReadRegister8( Microchip_MCP23017_Const::REG_GPIOB );


		}

		void PerformRead()
		{
			uint16_t AReadBits = PerformDirectRead();
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
	> class Microchip_MCP23017_Channel :
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

            if( C_INDEX < 8 )
                C_OWNER.FModifiedValueA() = true;

            else
                C_OWNER.FModifiedValueB() = true;

//			C_OWNER.SetChannelValue( C_INDEX, AValue );
		}

		inline void SendOutput( uint16_t AValue )
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
            if( C_INDEX < 8 )
                C_OWNER.FModifiedInvertA() = true;

            else
                C_OWNER.FModifiedInvertB() = true;

        }

        inline void Update_IsOutput()
        {
            if( C_INDEX < 8 )
                C_OWNER.FModifiedDirectionA() = true;

            else
                C_OWNER.FModifiedDirectionB() = true;

        }

        inline void Update_IsPullUp()
        {
            if( C_INDEX < 8 )
                C_OWNER.FModifiedPullUpA() = true;

            else
                C_OWNER.FModifiedPullUpB() = true;

        }

	public:
		inline void SystemStart()
		{
		}

		inline void SystemLoopBegin()
		{
		}

	public:
		inline void GetDirection( uint16_t & AValue )
		{
		    if( IsOutput() )
		        AValue &= ~( 1 << C_INDEX );

		}

		inline void GetPullUp( uint16_t & AValue )
		{
		    if( IsPullUp() )
		        AValue |= ( 1 << C_INDEX );

		}

		inline void GetInvert( uint16_t & AValue )
		{
		    if( IsInverted() )
		        AValue |= ( 1 << C_INDEX );

		}

		inline void GetValue( uint16_t & AValue )
		{
		    if( InitialValue() )
		        AValue |= ( 1 << C_INDEX );

		}

		inline void HasInputs( bool & AValueA, bool & AValueB )
		{
	        if( T_OutputPin::GetPinIsConnected() )
	        {
	            if( C_INDEX < 8 )
	                AValueA = true;

	            else
	                AValueB = true;

	        }
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif