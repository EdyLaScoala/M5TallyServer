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
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FClocked,
		uint8_t C_INDEX,
		typename T_OutputPin
    > class TArduinoMicrochip_ADC_MCP342XChannel :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FClocked,
		public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

    protected:
		_V_PROP_( FClocked )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
		    if( C_OWNER.CanStartChannel( C_INDEX ))
		        FClocked() = true;

		}

	public:
		inline void GetActiveChannels( uint8_t & AValue )
		{
		    if( ! Enabled().GetValue() )
		        return;

	        AValue |= ( 0b1 << C_INDEX );

	        if( T_ClockInputPin_o_IsConnected() && ( ! FClocked() ))
		        AValue |= ( 0b10000 << C_INDEX );

//		    if( C_INDEX == 0 )
//		        Serial.println( AValue, BIN );

		}

        inline void SendOutput( float AValue )
        {
            if( FClocked() )
            {
                FClocked() = false;
                T_OutputPin::SetPinValue( AValue );
            }
        }

    public:
        inline TArduinoMicrochip_ADC_MCP342XChannel()
        {
            FClocked() = false;
        }

    };
//---------------------------------------------------------------------------
	template <
	    typename T_AllChannels,
	    typename T_Channel,
	    typename T_ContinuosSingleChannel
    > class TArduinoMicrochip_ADC_MCP342XMode :
	    public T_AllChannels,
	    public T_Channel,
	    public T_ContinuosSingleChannel
    {
	public:
		_V_PROP_( AllChannels )
		_V_PROP_( Channel )
		_V_PROP_( ContinuosSingleChannel )

    public:
        inline bool IsSingleChannel()
        {
            return( ContinuosSingleChannel().GetValue() || ( ! AllChannels().GetValue() ));
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_GetActiveChannels,
        typename T_Channels_SendOutput,
//        typename T_FClockReading,
//        typename T_FClockedIndex,
        typename T_FIndex,
		typename T_Gain,
		typename T_Mode,
		typename T_Resolution
	> class Microchip_ADC_MCP342X :
		public T_Address,
//        public T_FClockReading,
//        public T_FClockedIndex,
        public T_FIndex,
		public T_Gain,
		public T_Mode,
		public T_Resolution
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Gain )
		_V_PROP_( Mode )
		_V_PROP_( Resolution )

    protected:
//		_V_PROP_( FClockReading )
//		_V_PROP_( FClockedIndex )
		_V_PROP_( FIndex )

    protected:
	    void Update_CONFIG_Start( bool AStartReading )
	    {
	        uint8_t AValue = ( Mode().ContinuosSingleChannel().GetValue() ) ? 0b00010000 : 0;

	        if( Resolution() > 16 )
	            AValue |= 0b00001100;

	        else if( Resolution() > 14 )
	            AValue |= 0b00001000;

	        else if( Resolution() > 12 )
	            AValue |= 0b00000100;

	        if( Gain() > 4 )
	            AValue |= 0b00000011;

	        else if( Gain() > 2 )
	            AValue |= 0b00000010;

	        else if( Gain() > 1 )
	            AValue |= 0b00000001;

//            uint8_t AActiveChannels = 0;
//            T_Channels_GetActiveChannels::Call( AActiveChannels );

	        if( AStartReading )
	            AValue |= 0b10000000;

	        uint8_t AChannel;
	        if( Mode().IsSingleChannel())
	            AChannel = Mode().Channel().GetValue();

	        else
	            AChannel = FIndex();

	        AValue |= ( AChannel << 5 );

//	        Serial.println( AValue, BIN );
//	        while( true );

            C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
            C_I2C.write( AValue );
            C_I2C.endTransmission();
	    }

	public:
	    inline void Update_CONFIG()
	    {
	        Update_CONFIG_Start( false );
	    }

	public:
        bool CanStartChannel( uint8_t AValue )
        {
            if( ! IsSingleChannel() )
                return true;

            if( Mode().Channel() == AValue )
                return true;

            return false;
/*
            if( IsSingleChannel() )
                if( )
            uint8_t AActiveChannels = 0;
            T_Channels_GetActiveChannels::Call( AActiveChannels );

            if( ( AActiveChannels & 0b00001111 ) & ( ~ ( AActiveChannels >> 4 ) ) )
                return false; // There are other ccan pins

            FIndex
//            FClockReading() = true;
*/
        }

	public:
	    inline void SystemInit()
	    {
            uint8_t AActiveChannels = 0;
            T_Channels_GetActiveChannels::Call( AActiveChannels );
            FIndex() = 0;
            for( int i = 0; i < 4; ++ i )
                if( ( AActiveChannels & ( ~ ( AActiveChannels >> 4 ) ) ) & ( 1 << i ) )
                {
                    FIndex() = i;
                    break;
                }

	    }

	    inline void SystemStart()
	    {
	        Update_CONFIG_Start( true );
	    }

	    inline void SystemLoopBegin()
	    {
            uint8_t AActiveChannels = 0;
            T_Channels_GetActiveChannels::Call( AActiveChannels );
	        if( ( AActiveChannels & 0b1111 ) == 0 )
	            return;

	        uint8_t AReadLength = ( Resolution() > 16 ) ? 4 : 3;

	        C_I2C.requestFrom( uint8_t( Address().GetValue() ), AReadLength );
	        int32_t AIntValue = C_I2C.read();
//            Serial.println( AIntValue );
	        for( int i = 0; i < AReadLength - 2; ++i )
	        {
	            AIntValue <<= 8;
	            AIntValue |= C_I2C.read();
//	            Serial.println( AIntValue );
	        }

//	        if( AReadLength == 2 )
//	            AIntValue <<= 16;

//	        else
//	            AIntValue <<= 8;

	        if( Resolution() > 16 )
	            AIntValue <<= ( 16 - 2 ); // 18

	        else if( Resolution() > 14 )
	            AIntValue <<= ( 16 ); // 16

	        else if( Resolution() > 12 )
	            AIntValue <<= ( 16 + 2 ); // 14

	        else
	            AIntValue <<= ( 16 + 4 ); // 12

	        uint8_t AStatus = C_I2C.read();

	        if( ( AStatus & 0b10000000 ) == 0 ) // 0 = Complete!
            {
//                Serial.println( AIntValue );

//                while( true );

                float AValue = float( AIntValue ) / float( 0x7FFFFFFF );

                if( Mode().IsSingleChannel() )
    	            T_Channels_SendOutput::Call( Mode().Channel(), AValue );

    	        else
    	        {
	                T_Channels_SendOutput::Call( FIndex(), AValue );

	                if( ( AActiveChannels & 0b00001111 ) & ( ~ ( AActiveChannels >> 4 ) ) )
                    {
                        int AIndex = FIndex() + 1;
                        bool AFound = false;
                        while( AIndex < 4 )
                        {
                            if( ( AActiveChannels & ( ~ ( AActiveChannels >> 4 ) ) ) & ( 1 << AIndex ) )
                            {
                                FIndex() = AIndex;
                                AFound = true;
                                break;
                            }

                            ++ AIndex;
                        }

                        if( ! AFound )
                        {
                            AIndex = 0;
                            while( AIndex < FIndex() )
                            {
                                if( ( AActiveChannels & ( ~ ( AActiveChannels >> 4 ) ) ) & ( 1 << AIndex ) )
                                {
                                    FIndex() = AIndex;
                                    break;
                                }

                                ++ AIndex;
                            }
                        }
                    }

//                    Serial.println( FIndex().GetValue()  )  ;

	                Update_CONFIG_Start( true );
	            }

            }

	    }

/*
	public:
        inline Microchip_ADC_MCP342X()
        {
            FIndex() = 0;
        }
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif