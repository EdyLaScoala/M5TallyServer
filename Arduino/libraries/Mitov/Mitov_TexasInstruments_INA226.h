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
#include <Mitov_TexasInstruments_INA260.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//#define __INA226_DEBUG__

namespace Mitov
{
    namespace INA226Config
    {
		const uint8_t REG_CONFIG	    = 0x00;
        const uint8_t REG_SHUNTVOLTAGE  = 0x01;
		const uint8_t REG_BUSVOLTAGE	= 0x02;
		const uint8_t REG_POWER	        = 0x03;
		const uint8_t REG_CURRENT		= 0x04;
		const uint8_t REG_MASK_ENABLE   = 0x06;
		const uint8_t REG_ALERT_LIMIT   = 0x07;
		const uint8_t REG_MANUF_ID      = 0xFE;
		const uint8_t REG_DIE_ID        = 0xFF;
    };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
        typename T_Alerts,
        typename T_Averaging,
        typename T_ClockInputPin_o_IsConnected,
        typename T_ConversionTime,
		typename T_Enabled,
		typename T_OutputPins_Power,
		typename T_OutputPins_BusVoltage,
		typename T_OutputPins_Current,
		typename T_OutputPins_ShuntVoltage,
        typename T_PowerDown,
        typename T_ShuntResistor,
        typename T_SingleShot
	> class TexasInstrumentsINA226 :
		public T_Address,
        public T_Alerts,
        public T_Averaging,
        public T_ClockInputPin_o_IsConnected,
        public T_ConversionTime,
		public T_Enabled,
		public T_OutputPins_Power,
		public T_OutputPins_BusVoltage,
		public T_OutputPins_Current,
		public T_OutputPins_ShuntVoltage,
        public T_PowerDown,
        public T_ShuntResistor,
        public T_SingleShot
	{
	public:
		_V_PIN_( OutputPins_Power )
		_V_PIN_( OutputPins_BusVoltage )
		_V_PIN_( OutputPins_Current )
        _V_PIN_( OutputPins_ShuntVoltage )

	public:
		_V_PROP_( Address )
        _V_PROP_( Alerts )
        _V_PROP_( Averaging )
		_V_PROP_( Enabled )
        _V_PROP_( PowerDown )
        _V_PROP_( ShuntResistor )
        _V_PROP_( SingleShot )
        _V_PROP_( ConversionTime )

	protected:
        _V_PROP_( ClockInputPin_o_IsConnected )

//	public:
//		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void UpdateConfig() // Used by setters!
		{
			IntUpdateConfig( false );
		}

		inline void UpdateMaskRegister() // Used by setters!
        {
            uint16_t AValue = ( Alerts().Limit().Current().Above().GetValue() ) ? 0b1000000000000000 : 0;

            if( Alerts().Limit().Current().Bellow().GetValue() )
                AValue |= 0b0100000000000000;

            if( Alerts().Limit().BusVoltage().Above().GetValue() )
                AValue |= 0b0010000000000000;

            if( Alerts().Limit().BusVoltage().Bellow().GetValue() )
                AValue |= 0b0001000000000000;

            if( Alerts().Limit().PowerAbove().GetValue() )
                AValue |= 0b0000100000000000;

            if( Alerts().DataReady().GetValue() )
                AValue |= 0b0000010000000000;

            if( Alerts().Inverted().GetValue() )
                AValue |= 0b0000000000000010;

            if( Alerts().Latch().GetValue() )
                AValue |= 0b0000000000000001;

			WriteRegister( INA260Config::REG_MASK_ENABLE, AValue );
        }

		inline void UpdateLimitRegister() // Used by setters!
        {
            bool AIsVoltsAmpers =   ( Alerts().Limit().Current().Above().GetValue() ) ||
                                    ( Alerts().Limit().Current().Bellow().GetValue() ) ||
                                    ( Alerts().Limit().BusVoltage().Above().GetValue() ) ||
                                    ( Alerts().Limit().BusVoltage().Bellow().GetValue() );

            float AValue = Alerts().Value().GetValue();
            if( AIsVoltsAmpers )
                AValue /= 1.25;

            else
                AValue /= 10;

            AValue *= 1000;

            WriteRegister( INA260Config::REG_ALERT_LIMIT, AValue );
        }

    public:
		uint16_t ReadRegister( uint8_t ARegister )
		{
#ifdef __INA226_DEBUG__
            Serial.print( "R " );
            Serial.println( ARegister, HEX );
#endif

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(ARegister);                       // Register
			C_I2C.endTransmission();

//			delay(1); // Max 12-bit conversion time is 586us per sample

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) );
			// Shift values to create properly formed integer
			uint16_t AValue = C_I2C.read();
			return (( AValue << 8 ) | C_I2C.read());
		}

		void WriteRegister( uint8_t ARegister, uint16_t AValue )
		{
#ifdef __INA226_DEBUG__
            Serial.print( "W " );
            Serial.print( ARegister, HEX );
            Serial.print( " : " );
            Serial.println( AValue, BIN );
#endif

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(ARegister);                       // Register
			C_I2C.write((AValue >> 8) & 0xFF);       // Upper 8-bits
			C_I2C.write(AValue & 0xFF);              // Lower 8-bits
			C_I2C.endTransmission();
		}

	protected:
        void UpdateRegisters()
        {
            UpdateConfig();
            UpdateLimitRegister();
            UpdateMaskRegister();
        }

	protected:
		void IntUpdateConfig( bool AReset )
		{
			uint16_t config;

            if( PowerDown() )
                config = 0b000;

            else
            {
    			bool ANeedsShunt = false;
	    		bool ANeedsBus = false;
//                T_Channels_NeedsShuntBus::Call( ANeedsShunt, ANeedsBus );
                if( SingleShot() && ClockInputPin_o_IsConnected() )
                {
                    if( ANeedsShunt && ANeedsBus )
                        config = 0b011;

                    else if( ANeedsShunt )
                        config = 0b001;

                    else
                        config = 0b010;

                }

                else
                {
                    if( ANeedsShunt && ANeedsBus )
                        config = 0b111;

                    else if( ANeedsShunt )
                        config = 0b101;

                    else
                        config = 0b110;

                }
            }

			config |= 0b0100000000000000;

            if( AReset )
                config |= 0b1000000000000000;

            if( Averaging() > 512 )
                config |= 0b0000111000000000;

            else if( Averaging() > 256 )
                config |= 0b0000110000000000;

            else if( Averaging() > 128 )
                config |= 0b0000101000000000;

            else if( Averaging() > 64 )
                config |= 0b0000100000000000;

            else if( Averaging() > 16 )
                config |= 0b0000011000000000;

            else if( Averaging() > 4 )
                config |= 0b0000010000000000;

            else if( Averaging() > 1 )
                config |= 0b0000001000000000;

            if( ConversionTime().Bus() > 4156 )
                config |= 0b0000000111000000;

            else if( ConversionTime().Bus() > 2116 )
                config |= 0b0000000110000000;

            else if( ConversionTime().Bus() > 1100 )
                config |= 0b0000000101000000;

            else if( ConversionTime().Bus() > 588 )
                config |= 0b0000000100000000;

            else if( ConversionTime().Bus() > 332 )
                config |= 0b0000000011000000;

            else if( ConversionTime().Bus() > 204 )
                config |= 0b0000000010000000;

            else if( ConversionTime().Bus() > 140 )
                config |= 0b0000000001000000;

            if( ConversionTime().Shunt() > 4156 )
                config |= 0b0000000000111000;

            else if( ConversionTime().Shunt() > 2116 )
                config |= 0b0000000000110000;

            else if( ConversionTime().Shunt() > 1100 )
                config |= 0b0000000000101000;

            else if( ConversionTime().Shunt() > 588 )
                config |= 0b0000000000100000;

            else if( ConversionTime().Shunt() > 332 )
                config |= 0b0000000000011000;

            else if( ConversionTime().Shunt() > 204 )
                config |= 0b0000000000010000;

            else if( ConversionTime().Shunt() > 140 )
                config |= 0b0000000000001000;

			WriteRegister( INA260Config::REG_CONFIG, config );
        }

    protected:
		void ReadData( bool ASingleShot )
		{
			if( ! Enabled() )
				return;

			if( ASingleShot && SingleShot().GetValue() )
				IntUpdateConfig( false ); // Force reading!

            while( true )
            {
                uint16_t AStatusValue = ReadRegister( INA260Config::REG_MASK_ENABLE );

                Alerts().OverflowOutputPin().SetPinValue( ( AStatusValue & 0b0000000000000100 ) != 0 );
                Alerts().Limit().OutputPin().SetPinValue( ( AStatusValue & 0b0000000000010000 ) != 0 );

//                T_Channels_ProcessStatusData::Call( AStatusValue );

//                Alerts().BadTimingOutputPin().SetPinValue( ( AStatusValue & 0b0000000000000010 ) != 0 );
//                Alerts().PowerValid().OutputPin().SetPinValue( ( AStatusValue & 0b0000000000000100 ) != 0 );
//                if( AStatusValue

                if( AStatusValue & 0b0000000000001000 )
                    break;

                if( ! ( ASingleShot && SingleShot().GetValue() ))
                    break;
            }

			bool APowerConnected = T_OutputPins_Power::GetPinIsConnected();
			bool ACurrentConnected = T_OutputPins_Current::GetPinIsConnected();
			bool AShuntVoltageConnected = T_OutputPins_ShuntVoltage::GetPinIsConnected();

			bool ABusVoltageConnected = T_OutputPins_BusVoltage::GetPinIsConnected();
			bool ANeedsShuntVoltage = AShuntVoltageConnected || ACurrentConnected || APowerConnected;
			bool ANeedsCurrent = ACurrentConnected || APowerConnected;
			bool ANeedsBusVoltage = APowerConnected || ABusVoltageConnected || ASingleShot;

			float ABusVoltage = 0;
			if( ANeedsBusVoltage )
			{
				int16_t ARawBusValue = int16_t( ReadRegister( INA226Config::REG_BUSVOLTAGE ));
				ABusVoltage = ARawBusValue;
				ABusVoltage *= 0.00125;
			}

			float AShuntVoltage;
			float ACurrent;
			if( ANeedsShuntVoltage )
			{
				AShuntVoltage = int16_t( ReadRegister( INA226Config::REG_SHUNTVOLTAGE ));
				AShuntVoltage *= ( 0.0000025 * 1000 ); // In mV

				if( ANeedsCurrent )
					ACurrent = ( AShuntVoltage / ShuntResistor() );
			}

			if( APowerConnected ) // Power
			{
//				float AValue = ReadRegister( INA219_REG_POWER );
				float AValue = ACurrent * ABusVoltage;
				T_OutputPins_Power::SetPinValue( AValue );
			}

    		T_OutputPins_BusVoltage::SetPinValue( ABusVoltage );
			T_OutputPins_Current::SetPinValue( ACurrent );
			T_OutputPins_ShuntVoltage::SetPinValue( AShuntVoltage );
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( true );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			IntUpdateConfig( true );
            UpdateRegisters();
		}

	public:
		inline void SystemStart()
		{
			IntUpdateConfig( true );
//            Serial.println( ReadRegister( REG_CRIT_ALERT_LIM_1 ), HEX );
			UpdateRegisters();
		}

        inline void SystemLoopBegin()
        {
			ReadData( false );
        }

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

