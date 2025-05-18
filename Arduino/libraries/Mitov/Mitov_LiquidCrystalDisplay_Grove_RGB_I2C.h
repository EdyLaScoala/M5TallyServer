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
#include <Mitov_LiquidCrystalDisplay_JHD1214_I2C.h>
#include <Mitov_RGBLedControl_PCA9633.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address,
		typename T_Blink,
		typename T_Brightness,
		typename T_Color
	> class TArduinoLiquidCrystalDisplay_Grove_RGB_Backlight :
		public T_Address,
		public T_Blink,
		public T_Brightness,
		public T_Color
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Blink )
		_V_PROP_( Brightness )
		_V_PROP_( Color )

    public:
        inline void UpdateMODE1()
        {
//            uint8_t AValue = ( Sleep().GetValue() ) ? 0b10010000 : 0b10000000;
            WriteReg( PCA9633Const::REG_MODE1, 0b10000000 );
        }

        void UpdateMODE2()
        {
            uint8_t AValue = 0b00000101 |
                                (( Blink().Enabled().GetValue() ) ? 0b00100000 : 0 );
//                                (( OpenDrain().GetValue() ) ? 0 : 0b00000100 );

            WriteReg( PCA9633Const::REG_MODE2, AValue );
        }

        void UpdateColor()
        {
            uint32_t AValue = Color().GetValue();

//            Serial.println( AValue, HEX );
//            C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
//            C_I2C.write( PCA9633Const::REG_PWM0 );

            for( int i = 0; i < 4; ++ i )
            {
//                C_I2C.write( AValue & 0xFF );
                WriteReg( PCA9633Const::REG_PWM2 - i, AValue & 0xFF );
                AValue >>= 8;
            }

//            C_I2C.endTransmission();

        }

/*
        inline void Invalidate()
        {
            FModified() = true;
        }
*/
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
            uint8_t AValue = uint8_t((( ASeconds - 0.041 ) / ( 10.73 - 0.041 )) * 255 + 0.5 );
            WriteReg( PCA9633Const::REG_GRPFREQ, AValue );
        }

    protected:
        void WriteReg( uint8_t ARegister, uint8_t AData )
        {
            C_OWNER.GetI2C().beginTransmission( uint8_t( Address().GetValue() ) );
            C_OWNER.GetI2C().write( ARegister );
            C_OWNER.GetI2C().write( AData );
            C_OWNER.GetI2C().endTransmission();
        }

	public:
		inline void SystemStart()
		{
            UpdateMODE1();
            UpdateMODE2();
            UpdateColor();
            UpdateBrightness();
            UpdateFrequency();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address
	> class TArduinoLiquidCrystalDisplay_Grove_RGB_I2C :
		public LiquidCrystalDisplay_JHD1214_I2C <
				T_I2C, C_I2C,
				T_Address
			>
	{
    public:
        inline T_I2C &GetI2C() { return C_I2C; }
        
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif