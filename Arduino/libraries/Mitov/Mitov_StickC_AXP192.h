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
#include <Wire.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ChargingOutputPin,
//		typename T_ClockInputPin_o_IsConnected,
		typename T_DischargingOutputPin,
		typename T_Enabled,
		typename T_OutputPin
	> class AXP192_Charge :
		public T_ChargingOutputPin,
//		public T_ClockInputPin_o_IsConnected,
		public T_DischargingOutputPin,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( ChargingOutputPin )
		_V_PIN_( DischargingOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

//	public:
//		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		uint32_t GetCoulombchargeData(void)
		{
			Wire1.beginTransmission( 0x34 );
			Wire1.write(0xB0);
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf = Wire1.read();

			Wire1.beginTransmission( 0x34 );
			Wire1.write(0xB1);
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf1 = Wire1.read();

			Wire1.beginTransmission( 0x34 );
			Wire1.write(0xB2);
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf2 = Wire1.read();

			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0xB3 );
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf3 = Wire1.read();

			uint32_t coin = ((buf << 24) + (buf1 << 16) + (buf2 << 8) + buf3);

			return coin;
		}

		uint32_t GetCoulombDischargeData(void)
		{
			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0xB4 );
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf = Wire1.read();

			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0xB5 );
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf1 = Wire1.read();

			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0xB6 );
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf2 = Wire1.read();

			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0xB7 );
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf3 = Wire1.read();

			uint32_t coout = ((buf << 24) + (buf1 << 16) + (buf2 << 8) + buf3);

			return coout;
		}

	public:
		void ReadData( bool AChangeOnly )
		{
			float ACoin = 0;
			float ACoout = 0;

			if( T_ChargingOutputPin::GetPinIsConnected() || T_OutputPin::GetPinIsConnected() )
				ACoin = GetCoulombchargeData();

			if( T_DischargingOutputPin::GetPinIsConnected() || T_OutputPin::GetPinIsConnected() )
				ACoout = GetCoulombDischargeData();

			if( T_OutputPin::GetPinIsConnected() )
			{
				//c = 65536 * current_LSB * (coin - coout) / 3600 / ADC rate
				//Adc rate can be read from 84H ,change this variable if you change the ADC reate
				float AValue = ( 65536 * 0.5f * ( ACoin - ACoout ) / 3600.0f / 25.0f ) / 1000.0f;
				T_OutputPin::SetPinValue( AValue, AChangeOnly );
			}

			if( T_ChargingOutputPin::GetPinIsConnected() )
			{
				float AValue = ( 65536 * 0.5f * ACoin / 3600.0f / 25.0f ) / 1000.0f;
				T_ChargingOutputPin::SetPinValue( AValue, AChangeOnly );
			}

			if( T_DischargingOutputPin::GetPinIsConnected() )
			{
				float AValue = ( 65536 * 0.5f * ACoout / 3600.0f / 25.0f ) / 1000.0f;
				T_DischargingOutputPin::SetPinValue( AValue, AChangeOnly );
			}
		}

	public:
		inline void SystemInit()
		{
			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0xB8 );
			Wire1.write( 0x80 );
			Wire1.endTransmission();
		}

		inline void SystemStart()
		{
			ReadData( false );
		}

		inline void SystemLoopBegin()
		{
			ReadData( true );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( false );
		}

		inline void ClearInputPin_o_Receive( void *_Data )
		{
			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0xB8 );
			Wire1.write( 0xA0 );
			Wire1.endTransmission();
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_ChargeOutputPin,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_PowerOutputPin,
		typename T_VoltageOutputPin
	> class AXP192_Battery :
//		public T_ChargeOutputPin,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_PowerOutputPin,
		public T_VoltageOutputPin
	{
	public:
//		_V_PIN_( ChargeOutputPin )
		_V_PIN_( PowerOutputPin )
		_V_PIN_( VoltageOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		void ReadBatteryVoltage( bool AChangeOnly )
		{
			uint16_t vbat = 0;

			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0x78 ); // battery voltage LSB buff
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint16_t buf = Wire1.read();

			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0x79 ); // battery voltage MSB buff
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf2 = Wire1.read();

			vbat = ((buf << 4) + buf2); // V

			float AValue = vbat * 1.1f / 1000.0f;
			T_VoltageOutputPin::SetPinValue( AValue, AChangeOnly );
		}

		void ReadBatteryPower( bool AChangeOnly )
		{
			uint32_t power = 0;

			Wire1.beginTransmission(0x34);
			Wire1.write(0x70);
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint32_t buf0 = Wire1.read();

			Wire1.beginTransmission(0x34);
			Wire1.write(0x71);
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint32_t buf1 = Wire1.read();

			Wire1.beginTransmission(0x34);
			Wire1.write(0x72);
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf2 = Wire1.read();

			power = (buf0 << 16) + (buf1 << 8) + buf2;
			float AValue = power * 1.1f * 0.5f / 1000.0f;
			T_PowerOutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( T_VoltageOutputPin::GetPinIsConnected() )
				ReadBatteryVoltage( false );

			if( T_PowerOutputPin::GetPinIsConnected() )
				ReadBatteryPower( false );

//			if( T_ChargeOutputPin::GetPinIsConnected() )
//				ReadBatteryCharge( false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class AXP192_Thermometer :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline bool GetIsEnabled() { return OutputPin().GetPinIsConnected() && Enabled().GetValue(); }

	public:
		void ReadThermometer( bool AChangeOnly )
		{
			uint16_t temp = 0;

			// get temp of AXP192
			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0x5e );
			Wire1.endTransmission();
			Wire1.requestFrom( 0x34, 1 );
			uint8_t buf = Wire1.read();

			Wire1.beginTransmission(0x34);
			Wire1.write(0x5f);
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf2 = Wire1.read();

			temp = (( uint16_t(buf ) << 4) + buf2);

			float AValue = -144.7f + temp * 0.1f;

			if( InFahrenheit() )
				AValue = Func::ConvertCtoF( AValue );

			T_OutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadThermometer( false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_CurrentOutputPin,
		typename T_Enabled,
		typename T_VoltageOutputPin
	> class AXP192_ACPower :
		public T_ClockInputPin_o_IsConnected,
		public T_CurrentOutputPin,
		public T_Enabled,
		public T_VoltageOutputPin
	{
	public:
		_V_PIN_( CurrentOutputPin )
		_V_PIN_( VoltageOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		void ReadVoltage( bool AChangeOnly )
		{
			// ACIN connect to the M5stickC 5Vin port
			Wire1.beginTransmission(0x34);
			Wire1.write(0x56); // ACIN voltage LSB buff
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf = Wire1.read();

			Wire1.beginTransmission(0x34);
			Wire1.write(0x57); // ACIN voltage MSB buff
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf2 = Wire1.read();

			uint16_t vin = ((buf << 4) + buf2); // V

			float AValue = ( vin * 1.7f ) / 1000;
			T_VoltageOutputPin::SetPinValue( AValue, AChangeOnly );
		}

		void ReadCurrent( bool AChangeOnly )
		{
			// ACIN current
			Wire1.beginTransmission(0x34);
			Wire1.write(0x58); // ACIN current LSB buff
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf = Wire1.read();

			Wire1.beginTransmission(0x34);
			Wire1.write(0x59); // ACIN current MSB buff
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf2 = Wire1.read();

			uint16_t iin = ((buf << 4) + buf2);

			float AValue = ( iin * 0.625 ) / 1000;
			T_CurrentOutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( T_VoltageOutputPin::GetPinIsConnected() )
				ReadVoltage( false );

			if( T_CurrentOutputPin::GetPinIsConnected() )
				ReadCurrent( false );

		}

	public:
		inline void SystemStartVoltage()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadVoltage( false );
		}

		inline void SystemLoopBeginVoltage()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadVoltage( true );
		}

		inline void SystemStartCurrent()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadCurrent( false );
		}

		inline void SystemLoopBeginCurrent()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadCurrent( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_CurrentOutputPin,
		typename T_Enabled,
		typename T_VoltageOutputPin
	> class AXP192_USBPower :
		public T_ClockInputPin_o_IsConnected,
		public T_CurrentOutputPin,
		public T_Enabled,
		public T_VoltageOutputPin
	{
	public:
		_V_PIN_( CurrentOutputPin )
		_V_PIN_( VoltageOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		void ReadVoltage( bool AChangeOnly )
		{
			uint16_t vin = 0;

			//vbus voltage
			Wire1.beginTransmission(0x34);
			Wire1.write(0x5a); // vbus voltage LSB
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf = Wire1.read();

			Wire1.beginTransmission(0x34);
			Wire1.write(0x5b); // vbus voltage MSB
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf2 = Wire1.read();

			vin = ((buf << 4) + buf2); // V

			float AValue = ( vin * 1.7f ) / 1000;
			T_VoltageOutputPin::SetPinValue( AValue, AChangeOnly );
		}

		void ReadCurrent( bool AChangeOnly )
		{
			// Vbus current
			Wire1.beginTransmission(0x34);
			Wire1.write(0x5c); // vbus  current LSB
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf = Wire1.read();

			Wire1.beginTransmission(0x34);
			Wire1.write(0x5d); // vbus  current MSB
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf2 = Wire1.read();

			uint16_t iin = ((buf << 4) + buf2);

			 float AValue = ( iin * 0.375 ) / 1000;
			T_CurrentOutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( T_VoltageOutputPin::GetPinIsConnected() )
				ReadVoltage( false );

			if( T_CurrentOutputPin::GetPinIsConnected() )
				ReadCurrent( false );

		}

	public:
		inline void SystemStartVoltage()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadVoltage( false );
		}

		inline void SystemLoopBeginVoltage()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadVoltage( true );
		}

		inline void SystemStartCurrent()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadCurrent( false );
		}

		inline void SystemLoopBeginCurrent()
		{
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				ReadCurrent( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Battery,
		typename T_Thermometer
	> class Arduino_StickC_AXP192 :
		public T_Battery,
		public T_Thermometer
	{
	public:
		_V_PROP_( Battery )
		_V_PROP_( Thermometer )

	public:
		inline void SystemInit()
		{
/*
			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x10 );
			C_I2C.write( 0xff );  //OLED_VPP Enable
			C_I2C.endTransmission();

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x28 );
			C_I2C.write( 0xcc ); //Enable LDO2&LDO3, LED&TFT 3.0V
			C_I2C.endTransmission();
*/

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x82 );  //Enable all the ADCs
			C_I2C.write( 0xff );
			C_I2C.endTransmission();

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x33 );  //Enable Charging, 100mA, 4.2V, End at current less than 10%
			C_I2C.write( 0xC0 );
			C_I2C.endTransmission();

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0xB8 );  //Enable Colume Counter
			C_I2C.write( 0x80 );
			C_I2C.endTransmission();

/*
			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x12 );
			C_I2C.write( 0x4d ); //Enable DC-DC1, OLED_VDD, 5B V_EXT
			C_I2C.endTransmission();
*/
			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x36 );  //pek setting
			C_I2C.write( 0b00001100 ); //shut down setting with 4s,auto shut donw while  btn press longer then shut donw setting
			C_I2C.endTransmission();

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x90 );
			C_I2C.write( 0x02 ); //gpio0 setting
			C_I2C.endTransmission();

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x30 ); //VBUS limit
			C_I2C.write( 0xe0 );
			C_I2C.endTransmission();

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x39 );
			C_I2C.write( 0xFC );
			C_I2C.endTransmission();

			C_I2C.beginTransmission( 0x34 );
			C_I2C.write( 0x35 );
			C_I2C.write( 0xA2 );
			C_I2C.endTransmission();
		}

		inline void SystemStartThermometer()
		{
			if( ! Thermometer().ClockInputPin_o_IsConnected().GetValue() )
				Thermometer().ReadThermometer( false );

		}

		inline void SystemLoopBeginThermometer()
		{
			if( ! Thermometer().ClockInputPin_o_IsConnected().GetValue() )
				Thermometer().ReadThermometer( true );

		}

		inline void SystemStartBatteryVoltage()
		{
			if( ! Battery().ClockInputPin_o_IsConnected().GetValue() )
				Battery().ReadBatteryVoltage( false );

		}

		inline void SystemLoopBeginBatteryVoltage()
		{
			if( ! Battery().ClockInputPin_o_IsConnected().GetValue() )
				Battery().ReadBatteryVoltage( true );

		}

		inline void SystemStartBatteryPower()
		{
			if( ! Battery().ClockInputPin_o_IsConnected().GetValue() )
				Battery().ReadBatteryPower( false );

		}

		inline void SystemLoopBeginBatteryPower()
		{
			if( ! Battery().ClockInputPin_o_IsConnected().GetValue() )
				Battery().ReadBatteryPower( true );

		}

/*
		inline void SystemStartCoulombsStored()
		{
			if( ! Coulombs().ClockInputPin_o_IsConnected().GetValue() )
				Coulombs().ReadCoulombsStored( false );

		}

		inline void SystemLoopBeginCoulombsStored()
		{
			if( ! Coulombs().ClockInputPin_o_IsConnected().GetValue() )
				Coulombs().ReadCoulombsStored( true );

		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_LongPressOutputPin,
		typename T_ShortPressOutputPin
	> class ArduinoAXP192_Button :
		public T_LongPressOutputPin,
		public T_ShortPressOutputPin
	{
	public:
		_V_PIN_( LongPressOutputPin )
		_V_PIN_( ShortPressOutputPin )

	public:
		inline void SystemLoopBegin()
		{
			C_I2C.beginTransmission(0x34);
			C_I2C.write(0x46);
			C_I2C.endTransmission();
			C_I2C.requestFrom(0x34, 1);
			uint8_t AState = C_I2C.read();
			// 0 not press, 0x01 long press, 0x02 press
			if( AState )
			{
				C_I2C.beginTransmission(0x34);
				C_I2C.write(0x46);
				C_I2C.write(0x03);
				C_I2C.endTransmission();

//				Serial.println( AState );

				switch( AState )
				{
				case 1 :
					T_LongPressOutputPin::ClockPin();
					break;

				case 2 :
					T_ShortPressOutputPin::ClockPin();
					break;
				}

			}

		}
	};
//---------------------------------------------------------------------------
}

