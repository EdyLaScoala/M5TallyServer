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
    namespace PCA9633Const
    {
		const uint8_t REG_MODE1         = 0x00;
		const uint8_t REG_MODE2         = 0x01;
		const uint8_t REG_PWM0          = 0x02;
		const uint8_t REG_PWM1          = 0x03;
		const uint8_t REG_PWM2          = 0x04;
		const uint8_t REG_PWM3          = 0x05;
        const uint8_t REG_GRPPWM        = 0x06;
        const uint8_t REG_GRPFREQ       = 0x07;
        const uint8_t REG_LEDOUT        = 0x08;
        const uint8_t REG_SUBADR1       = 0x09;
        const uint8_t REG_SUBADR2       = 0x0A;
        const uint8_t REG_SUBADR3       = 0x0B;
        const uint8_t REG_ALLCALLADR    = 0x0C;
    }
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Frequency
    > class TArduinoRGBLedControl_PCA9633_Blink :
		public T_Enabled,
		public T_Frequency
    {
    public:
        _V_PROP_( Enabled )
        _V_PROP_( Frequency )

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_FValue,
        uint8_t C_INDEX,
        typename T_InitialValue
    > class TArduinoRGBLedControl_PCA9633_Channel :
        public T_InitialValue,
        public T_FValue
    {
    public:
        _V_PROP_( InitialValue )

    public:
        _V_PROP_( FValue )

    public:
		void InputPin_o_Receive( void *_Data )
		{
            float AValue = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );

            FValue() = uint8_t( AValue * 255 + 0.5f );
            C_OWNER.Invalidate();
        }

    public:
        inline void GetValue( uint32_t & AValue )
        {
            if( FValue().GetIsConstant() )
                AValue |= uint32_t( InitialValue() * 255 + 0.5 ) << ( 8 * C_INDEX );

            else
                AValue |= uint32_t( FValue().GetValue() ) << ( 8 * C_INDEX );

        }

    public:
        inline void SystemInit()
        {
            FValue() = uint8_t( InitialValue() * 255 + 0.5 );
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
        typename T_Blink,
        typename T_Brightness,
        typename T_Channels_GetValue,
        typename T_Enabled,
//        typename T_InitialValue,
        typename T_FModified,
        typename T_OpenDrain,
        typename T_Sleep
    > class TArduinoRGBLedControl_PCA9633_I2C :
		public T_Address,
        public T_Blink,
        public T_Brightness,
        public T_Enabled,
//        public T_InitialValue,
        public T_FModified,
        public T_OpenDrain,
        public T_Sleep
    {
    public:
		_V_PROP_( Address )
        _V_PROP_( Blink )
        _V_PROP_( Brightness )
        _V_PROP_( Enabled )
//        _V_PROP_( InitialValue )
        _V_PROP_( OpenDrain )
        _V_PROP_( Sleep )

    protected:
        _V_PROP_( FModified )

    public:
        void UpdateMODE1()
        {
            uint8_t AValue = ( Sleep().GetValue() ) ? 0b10010000 : 0b10000000;
            WriteReg( PCA9633Const::REG_MODE1, AValue );
        }

        void UpdateMODE2()
        {
            uint8_t AValue = 0b00000001 |
                                (( Blink().Enabled().GetValue() ) ? 0b00100000 : 0 ) |
                                (( OpenDrain().GetValue() ) ? 0 : 0b00000100 );

            WriteReg( PCA9633Const::REG_MODE2, AValue );
        }

        void UpdateOutputs()
        {
            uint32_t AValue = 0;
            T_Channels_GetValue::Call( AValue );
//            Serial.println( AValue, HEX );
//            C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
//            C_I2C.write( PCA9633Const::REG_PWM0 );

            for( int i = 0; i < 4; ++ i )
            {
//                C_I2C.write( AValue & 0xFF );
                WriteReg( PCA9633Const::REG_PWM0 + i, AValue & 0xFF );
                AValue >>= 8;
            }

//            C_I2C.endTransmission();

            FModified() = false;
        }

        inline void Invalidate()
        {
            FModified() = true;
        }

        inline void UpdateMODE2_Brightness()
        {
            UpdateMODE2();
            UpdateBrightness();
        }

        void UpdateBrightness()
        {
            if( ( ! Blink().Enabled().GetValue() ) && ( Brightness().GetValue() >= 1.0 ) )
                WriteReg( PCA9633Const::REG_LEDOUT, 0b10101010 );

            else
            {
                WriteReg( PCA9633Const::REG_LEDOUT, 0b11111111 );
                WriteReg( PCA9633Const::REG_GRPPWM, uint8_t( Brightness().GetValue() * 255 + 0.5 ) );
            }
        }

        void UpdateFrequency()
        {
            float ASeconds = 1 / Blink().Frequency().GetValue();
//            Serial.println( ASeconds );
//            10.73 0.041
//            uint8_t AValue = uint8_t(( 1.0f - ( Blink().Frequency().GetValue() - 0.0932 ) / ( 24.0 - 0.0932 ) ) + 0.5 );
            uint8_t AValue = uint8_t((( ASeconds - 0.041 ) / ( 10.73 - 0.041 )) * 255 + 0.5 );
//            Serial.println( AValue );
            WriteReg( PCA9633Const::REG_GRPFREQ, AValue );
        }

    protected:
        void WriteReg( uint8_t ARegister, uint8_t AData )
        {
            C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
            C_I2C.write( ARegister );
            C_I2C.write( AData );
            C_I2C.endTransmission();
        }

/*
    public:
		void InputPin_o_Receive( void *_Data )
		{
        }
*/
    public:
        void inline SystemInit()
        {
            UpdateMODE1();
            UpdateMODE2();
        }

        void inline SystemStart()
        {
            UpdateOutputs();
            UpdateBrightness();
            UpdateFrequency();
        }

        void inline SystemLoopEnd()
        {
            if( FModified() )
                UpdateOutputs();

        }

    public:
        inline TArduinoRGBLedControl_PCA9633_I2C()
        {
            FModified() = false;
        }

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

