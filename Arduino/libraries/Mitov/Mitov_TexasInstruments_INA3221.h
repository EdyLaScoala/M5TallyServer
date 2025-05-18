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

//#define __INA3221_DEBUG__

namespace Mitov
{
    namespace INA3221Config
    {
		const uint8_t REG_CONFIG			    = 0x00;
		const uint8_t REG_SHUNTVOLTAGE_1	    = 0x01;
		const uint8_t REG_BUSVOLTAGE_1	        = 0x02;
		const uint8_t REG_SHUNTVOLTAGE_2	    = 0x03;
		const uint8_t REG_BUSVOLTAGE_2	        = 0x04;
		const uint8_t REG_SHUNTVOLTAGE_3	    = 0x05;
		const uint8_t REG_BUSVOLTAGE_3	        = 0x06;

		const uint8_t REG_CRIT_ALERT_LIM_1      = 0x07;
		const uint8_t REG_WARNING_ALERT_LIM_1   = 0x08;
		const uint8_t REG_CRIT_ALERT_LIM_2      = 0x09;
		const uint8_t REG_WARNING_ALERT_LIM_2   = 0x0A;
		const uint8_t REG_CRIT_ALERT_LIM_3      = 0x0B;
		const uint8_t REG_WARNING_ALERT_LIM_3   = 0x0C;
		const uint8_t REG_SHUNTV_SUM            = 0x0D;
		const uint8_t REG_SHUNTV_SUM_LIM        = 0x0E;
		const uint8_t REG_MASK_ENABLE           = 0x0F;
		const uint8_t REG_PWR_VALID_HI_LIM      = 0x10;
		const uint8_t REG_PWR_VALID_LO_LIM      = 0x11;
		const uint8_t REG_MANUF_ID              = 0xFE;
		const uint8_t REG_DIE_ID                = 0xFF;
    };
//---------------------------------------------------------------------------
	template <
        typename T_Limits,
        typename T_OutputPin
    > class TArduinoTexasInstrumentsINA3221PowerValid :
        public T_Limits,
        public T_OutputPin
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Limits )

    };
//---------------------------------------------------------------------------
	template <
        typename T_Value_Max,
        typename T_Value_Min
    > class TArduinoTexasInstrumentsINA3221PowerValidLimits :
        public T_Value_Max,
        public T_Value_Min
    {
	public:
		_V_PROP_( Value_Max )
		_V_PROP_( Value_Min )

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
//        typename T_Enabled,
        typename T_OutputPin,
        typename T_Voltage
    > class TArduinoTexasInstrumentsINA3221CriticalAlert :
//        public T_Enabled,
        public T_OutputPin,
        public T_Voltage
    {
	public:
		_V_PIN_( OutputPin )

	public:
//		_V_PROP_( Enabled )
		_V_PROP_( Voltage )

	public:
        inline void UpdateMaskRegister()
        {
            C_OWNER.UpdateMaskRegister();
        }

        inline void UpdateVoltage()
        {
            C_OWNER.UpdateCriticalAlertVoltage( Voltage() );
        }

	public:
        inline void UpdateRegisters()
        {
            UpdateVoltage();
        }

        inline void ProcessStatusData( uint8_t AChannel, uint16_t AValue )
        {
            T_OutputPin::SetPinValue( ( AValue | ( 0b0000001000000000 >> AChannel ) ) != 0 );
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
//        typename T_Enabled,
        typename T_OutputPin,
        typename T_Voltage
    > class TArduinoTexasInstrumentsINA3221WarningAlert :
//        public T_Enabled,
        public T_OutputPin,
        public T_Voltage
    {
	public:
		_V_PIN_( OutputPin )

	public:
//		_V_PROP_( Enabled )
		_V_PROP_( Voltage )

	public:
        inline void UpdateMaskRegister()
        {
            C_OWNER.UpdateMaskRegister();
        }

        inline void UpdateVoltage()
        {
            C_OWNER.UpdateWarningAlertVoltage( Voltage() );
        }

	public:
        inline void UpdateRegisters()
        {
            UpdateVoltage();
        }

        inline void ProcessStatusData( uint8_t AChannel, uint16_t AValue )
        {
            T_OutputPin::SetPinValue( ( AValue | ( 0b0000000000100000 >> AChannel ) ) != 0 );
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_OutputPin,
        typename T_Voltage
    > class TArduinoTexasInstrumentsINA3221SumAlert :
        public T_OutputPin,
        public T_Voltage
    {
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Voltage )

	public:
        inline void UpdateRegisters() {}

        inline void ProcessStatusData( uint16_t AValue )
        {
            T_OutputPin::SetPinValue( ( AValue | 0b0000000001000000 ) != 0 );
        }

        inline void UpdateVoltage()
        {
            C_OWNER.UpdateWarningAlertVoltage( Voltage() );
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Critical_ProcessStatusData,
        typename T_Critical_UpdateRegisters,
        typename T_Warning_ProcessStatusData,
        typename T_Warning_UpdateRegisters
    > class TArduinoTexasInstrumentsINA3221ChannelAlerts
    {
    public:
        inline void UpdateMaskRegister()
        {
            C_OWNER.UpdateMaskRegister();
        }

        inline void UpdateCriticalAlertVoltage( float AValue )
        {
            C_OWNER.UpdateCriticalAlertVoltage( AValue );
        }

        inline void UpdateWarningAlertVoltage( float AValue )
        {
            C_OWNER.UpdateWarningAlertVoltage( AValue );
        }

    public:
        inline void UpdateRegisters()
        {
            T_Critical_UpdateRegisters::Call();
            T_Warning_UpdateRegisters::Call();
        }

        inline void ProcessStatusData( uint8_t AChannel, uint16_t AValue )
        {
            T_Critical_ProcessStatusData::Call( AChannel, AValue );
            T_Warning_ProcessStatusData::Call( AChannel, AValue );
        }

    };
//---------------------------------------------------------------------------
	template <
        typename T_Critical,
        typename T_Warning
    > class TArduinoTexasInstrumentsINA3221AlertsLatching :
        public T_Critical,
        public T_Warning
    {
	public:
		_V_PROP_( Critical )
		_V_PROP_( Warning )

    };
//---------------------------------------------------------------------------
	template <
        typename T_BadTimingOutputPin,
        typename T_Latching,
        typename T_PowerValid
    > class TArduinoTexasInstrumentsINA3221Alerts :
        public T_BadTimingOutputPin,
        public T_Latching,
        public T_PowerValid
    {
	public:
        _V_PIN_( BadTimingOutputPin )

	public:
		_V_PROP_( Latching )
		_V_PROP_( PowerValid )

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
//        typename T_Alert,
        typename T_Alert_ProcessStatusData,
        typename T_Alert_UpdateRegisters,
		typename T_Enabled,
        typename T_ShuntVoltageOutputPin
    > class TArduinoTexasInstrumentsINA3221SumChannel :
//        public T_Alert,
		public T_Enabled,
        public T_ShuntVoltageOutputPin
    {
	public:
		_V_PIN_( ShuntVoltageOutputPin )

	public:
//        _V_PROP_( Alert )
		_V_PROP_( Enabled )

	public:
        inline void ProcessStatusData( uint16_t AValue )
        {
            T_Alert_ProcessStatusData::Call( AValue );
        }

        inline void ReadData()
        {
            if( T_ShuntVoltageOutputPin::GetPinIsConnected() )
            {
                float AValue = C_OWNER.ReadRegister( INA3221Config::REG_SHUNTV_SUM );
                T_ShuntVoltageOutputPin::SetPinValue( AValue );
            }
        }

        inline void UpdateWarningAlertVoltage( float AValue )
        {
            C_OWNER.UpdateWarningAlertVoltage( 3, AValue );
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
        typename T_AddToSum,
//        typename T_Alerts,
        typename T_Alerts_ProcessStatusData,
        typename T_Alerts_UpdateRegisters,
		typename T_Enabled,
        uint8_t C_INDEX,
		typename T_OutputPins_Power,
		typename T_OutputPins_BusVoltage,
		typename T_OutputPins_Current,
		typename T_OutputPins_ShuntVoltage,
        typename T_ShuntResistor
	> class TArduinoTexasInstrumentsINA3221Channel :
        public T_AddToSum,
//        public T_Alerts,
		public T_Enabled,
		public T_OutputPins_Power,
		public T_OutputPins_BusVoltage,
		public T_OutputPins_Current,
		public T_OutputPins_ShuntVoltage,
        public T_ShuntResistor
	{
	public:
		_V_PIN_( OutputPins_Power )
		_V_PIN_( OutputPins_BusVoltage )
		_V_PIN_( OutputPins_Current )
		_V_PIN_( OutputPins_ShuntVoltage )

	public:
        _V_PROP_( AddToSum )
//        _V_PROP_( Alerts )
		_V_PROP_( Enabled )
        _V_PROP_( ShuntResistor )

	public:
        inline void ChannelConfig( uint16_t &AConfig )
        {
            if( ! Enabled().GetValue() )
                return;

            if( T_OutputPins_Power::GetPinIsConnected() || T_OutputPins_BusVoltage::GetPinIsConnected() || T_OutputPins_Current::GetPinIsConnected() || T_OutputPins_ShuntVoltage::GetPinIsConnected() )
                AConfig |= 0b0100000000000000 >> C_INDEX;

        }

        inline void ApplyChannelSumMask( uint16_t &AMask )
        {
            if( AddToSum().GetValue() )
                AMask |= 0b0100000000000000 >> C_INDEX;

        }

        inline void NeedsShuntBus( bool &ANeedsShunt, bool &ANeedsBus )
        {
			ANeedsShunt |= T_OutputPins_Power::GetPinIsConnected() || T_OutputPins_ShuntVoltage::GetPinIsConnected() || T_OutputPins_Current::GetPinIsConnected();
			ANeedsBus |= T_OutputPins_Power::GetPinIsConnected() || T_OutputPins_BusVoltage::GetPinIsConnected();
        }

        inline void UpdateRegisters()
        {
            T_Alerts_UpdateRegisters::Call();
        }

        inline void ProcessStatusData( uint16_t AValue )
        {
            T_Alerts_ProcessStatusData::Call( C_INDEX, AValue );
        }

        inline void ReadData()
        {
			bool APowerConnected = T_OutputPins_Power::GetPinIsConnected();
			bool ACurrentConnected = T_OutputPins_Current::GetPinIsConnected();
			bool AShuntVoltageConnected = T_OutputPins_ShuntVoltage::GetPinIsConnected();

			bool ABusVoltageConnected = T_OutputPins_BusVoltage::GetPinIsConnected();

			bool ANeedsShuntVoltage = AShuntVoltageConnected || ACurrentConnected || APowerConnected;
			bool ANeedsCurrent = ACurrentConnected || APowerConnected;
			bool ANeedsBusVoltage = APowerConnected || ABusVoltageConnected;

			float ABusVoltage = 0;
			if( ANeedsBusVoltage )
			{
				ABusVoltage = C_OWNER.ReadRegister( INA3221Config::REG_BUSVOLTAGE_1 + C_INDEX * 2 );
				ABusVoltage *= 0.001;
			}

			float AShuntVoltage;
			float ACurrent;
			if( ANeedsShuntVoltage )
			{
				AShuntVoltage = (int16_t)C_OWNER.ReadRegister( INA3221Config::REG_SHUNTVOLTAGE_1 + C_INDEX * 2 );
				AShuntVoltage *= 0.01;

				if( ANeedsCurrent )
					ACurrent = ( AShuntVoltage / ShuntResistor().GetValue() );
			}

			if( APowerConnected ) // Power
			{
				float AValue = ACurrent * ABusVoltage;
				T_OutputPins_Power::SetPinValue( AValue );
			}

    		T_OutputPins_BusVoltage::SetPinValue( ABusVoltage );
			T_OutputPins_Current::SetPinValue( ACurrent );
			T_OutputPins_ShuntVoltage::SetPinValue( AShuntVoltage );
        }

    public:
        inline void UpdateCriticalAlertVoltage( float AValue )
        {
            C_OWNER.UpdateCriticalAlertVoltage( C_INDEX, AValue );
        }

        inline void UpdateWarningAlertVoltage( float AValue )
        {
            C_OWNER.UpdateWarningAlertVoltage( C_INDEX, AValue );
        }

	};
//---------------------------------------------------------------------------
	template <
        typename T_Bus,
        typename T_Shunt
    > class TArduinoTexasInstrumentsINAConversionTime :
        public T_Bus,
        public T_Shunt
    {
	public:
		_V_PROP_( Bus )
		_V_PROP_( Shunt )

    };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
        typename T_Alerts,
        typename T_Averaging,
        typename T_Channels_ApplyChannelSumMask,
        typename T_Channels_ChannelConfig,
        typename T_Channels_NeedsShuntBus,
        typename T_Channels_ProcessStatusData,
        typename T_Channels_ReadData,
        typename T_Channels_UpdateRegisters,
        typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
        typename T_PowerDown,
        typename T_SingleShot,
        typename T_VoltageConversionTime
	> class TexasInstrumentsINA3221 :
		public T_Address,
        public T_Alerts,
        public T_Averaging,
        public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
        public T_PowerDown,
        public T_SingleShot,
        public T_VoltageConversionTime
	{
	public:
		_V_PROP_( Address )
        _V_PROP_( Alerts )
        _V_PROP_( Averaging )
		_V_PROP_( Enabled )
        _V_PROP_( PowerDown )
        _V_PROP_( SingleShot )
        _V_PROP_( VoltageConversionTime )

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
            uint16_t AValue = 0;

            T_Channels_ApplyChannelSumMask::Call( AValue );

            if( Alerts().Latching().Warning() )
                AValue |= 0b0000100000000000;

            if( Alerts().Latching().Critical() )
                AValue |= 0b0000010000000000;

			WriteRegister( INA3221Config::REG_MASK_ENABLE, AValue );
        }

        void UpdateAlertsMinLimitRegister()
        {
            uint16_t AIntValue = Alerts().PowerValid().Limits().Value_Min().GetValue() * 1000 + 0.5;
            WriteRegister( INA3221Config::REG_PWR_VALID_LO_LIM, AIntValue );
        }

        void UpdateAlertsMaxLimitRegister()
        {
            uint16_t AIntValue = Alerts().PowerValid().Limits().Value_Max().GetValue() * 1000 + 0.5;
            WriteRegister( INA3221Config::REG_PWR_VALID_HI_LIM, AIntValue );
        }

    public:
        inline void UpdateCriticalAlertVoltage( uint8_t AChannelIndex, float AValue )
        {
            uint16_t AIntValue = AValue * 1000 + 0.5;
            WriteRegister( INA3221Config::REG_CRIT_ALERT_LIM_1 + AChannelIndex * 2, AIntValue );
        }

        inline void UpdateWarningAlertVoltage( uint8_t AChannelIndex, float AValue )
        {
            uint16_t AIntValue = AValue * 1000 + 0.5;
            WriteRegister( INA3221Config::REG_WARNING_ALERT_LIM_1 + AChannelIndex * 2, AIntValue );
        }

    public:
		uint16_t ReadRegister( uint8_t ARegister )
		{
#ifdef __INA3221_DEBUG__
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
#ifdef __INA3221_DEBUG__
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
            UpdateAlertsMinLimitRegister();
            UpdateAlertsMaxLimitRegister();
            UpdateMaskRegister();
            T_Channels_UpdateRegisters::Call();

//            UpdateCriticalAlertVoltage( 0, float AValue );
//            UpdateWarningAlertVoltage( 0, float AValue )

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
                T_Channels_NeedsShuntBus::Call( ANeedsShunt, ANeedsBus );
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
/*
			else if( SingleShot() && ClockInputPin_o_IsConnected() )
			{
				if( ANeedsShunt && ANeedsBus )
					mode = 0b011;

				else if( ANeedsShunt )
					mode = 0b001;

				else
					mode = 0b010;
			}

			else
			{
				if( ANeedsShunt && ANeedsBus )
					mode = 0b111;

				else if( ANeedsShunt )
					mode = 0b101;

				else
					mode = 0b110;
			}
*/
            if( AReset )
                config |= 0b1000000000000000;

            T_Channels_ChannelConfig::Call( config );

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

            if( VoltageConversionTime().Bus() > 4156 )
                config |= 0b0000000111000000;

            else if( VoltageConversionTime().Bus() > 2116 )
                config |= 0b0000000110000000;

            else if( VoltageConversionTime().Bus() > 1100 )
                config |= 0b0000000101000000;

            else if( VoltageConversionTime().Bus() > 588 )
                config |= 0b0000000100000000;

            else if( VoltageConversionTime().Bus() > 332 )
                config |= 0b0000000011000000;

            else if( VoltageConversionTime().Bus() > 204 )
                config |= 0b0000000010000000;

            else if( VoltageConversionTime().Bus() > 140 )
                config |= 0b0000000001000000;

            if( VoltageConversionTime().Shunt() > 4156 )
                config |= 0b0000000000111000;

            else if( VoltageConversionTime().Shunt() > 2116 )
                config |= 0b0000000000110000;

            else if( VoltageConversionTime().Shunt() > 1100 )
                config |= 0b0000000000101000;

            else if( VoltageConversionTime().Shunt() > 588 )
                config |= 0b0000000000100000;

            else if( VoltageConversionTime().Shunt() > 332 )
                config |= 0b0000000000011000;

            else if( VoltageConversionTime().Shunt() > 204 )
                config |= 0b0000000000010000;

            else if( VoltageConversionTime().Shunt() > 140 )
                config |= 0b0000000000001000;

			WriteRegister( INA3221Config::REG_CONFIG, config );
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
                uint16_t AStatusValue = ReadRegister( INA3221Config::REG_MASK_ENABLE );
                T_Channels_ProcessStatusData::Call( AStatusValue );

                Alerts().BadTimingOutputPin().SetPinValue( ( AStatusValue & 0b0000000000000010 ) != 0 );
                Alerts().PowerValid().OutputPin().SetPinValue( ( AStatusValue & 0b0000000000000100 ) != 0 );
//                if( AStatusValue

                if( ! ( ASingleShot && SingleShot().GetValue() ))
                    break;
            }

            T_Channels_ReadData::Call();
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
//        return;
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

