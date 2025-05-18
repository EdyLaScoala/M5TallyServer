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
	enum TArduinoMaximMotorCurrentRegulationMode { crmRipple25, crmFastDecay, crmSlowDecay };
//---------------------------------------------------------------------------
	template <
		typename T_MAX7300,
		typename T_MAX5387_1,
		typename T_MAX5387_2
	> class TArduinoMaximMotorDriverAddresses :
		public T_MAX7300,
		public T_MAX5387_1,
		public T_MAX5387_2
	{
	public:
		_V_PROP_( MAX7300 )
		_V_PROP_( MAX5387_1 )
		_V_PROP_( MAX5387_2 )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CurrentRegulationMode,
		typename T_Enabled,
		typename T_FaultOutputPin,
		uint8_t C_INDEX,
		typename T_InitialSpeed,
		typename T_MaxCurrent,
		typename T_MotorControlOutputPin,
		typename T_SenseResistor
	> class Maxim_MotorDriverChannel :
		public T_CurrentRegulationMode,
		public T_Enabled,
		public T_FaultOutputPin,
		public T_InitialSpeed,
		public T_MaxCurrent,
		public T_MotorControlOutputPin,
		public T_SenseResistor
	{
	public:
		_V_PIN_( FaultOutputPin )
		_V_PIN_( MotorControlOutputPin )


	public:
		_V_PROP_( Enabled )
		_V_PROP_( CurrentRegulationMode )
		_V_PROP_( InitialSpeed )
		_V_PROP_( MaxCurrent )
		_V_PROP_( SenseResistor )

	public:
		void UpdateEnabled()
		{
			C_OWNER.FChanged |= 0b00010000 << C_INDEX;
			C_OWNER.FModified() = true;
		}

        void UpdateMaxCurrent()
        {
			C_OWNER.FChanged |= 0b1 << C_INDEX;
			C_OWNER.FModified() = true;
        }

        void UpdateCurrentRegulationMode()
        {
			C_OWNER.FChanged |= 0b00010000 << C_INDEX;
			C_OWNER.FModified() = true;
        }

	public:
		inline void SpeedInputPin_o_Receive( void * _Data )
		{
			float AValue = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
//			Serial.println( AValue );
			if( InitialSpeed() == AValue )
				return;

			if(( AValue > 0.5 && InitialSpeed() <= 0.5 ) || ( AValue <= 0.5 && InitialSpeed() > 0.5 ))
				C_OWNER.FChanged |= 0b00010000 << C_INDEX;

			InitialSpeed() = AValue;

			C_OWNER.FModified() = true;

//			FOwner.SetChannelValue( FIndex, AValue );
		}

	public:
		uint8_t GetRegisterPins()
		{
			uint8_t	Result;
			if( Enabled() )
				Result = 0;

			else
				Result = 1;

			if( InitialSpeed() > 0.5 )
				Result |= 0b10;

			switch( CurrentRegulationMode() )
			{
				case crmRipple25:
					Result |= 0b1000;

				case crmFastDecay:
					Result |= 0b1100;

				case crmSlowDecay:
					Result |= 0b0100;

			}

			return Result;
		}

		uint8_t GetPotentiometer()
		{
//			uint8_t ATest1 = Reference * 255 + 0.5;
//			Serial.println( ATest1 );
			return ( MaxCurrent() * ( SenseResistor() * 10 ) / 2.6 ) * 255 + 0.5;
//			return Reference * 255 + 0.5;
//			return abs( FSpeed - 0.5 ) * 2 * 255 + 0.5;
		}

		void UpdateSpeed()
		{
			float AValue = fabs( InitialSpeed().GetValue() - 0.5 ) * 2;
//			Serial.println( inherited::FSpeed );
			T_MotorControlOutputPin::SetPinValue( AValue );
		}

	public:
		void UpdateFailure( uint8_t AFailureFlags )
		{
			T_FaultOutputPin::SetPinValue( ( AFailureFlags & ( 1 << C_INDEX )) == 0 );
		}

		void GetPotentiometers( uint8_t & ARegisterValue0, uint8_t & APotentiometer0, uint8_t & ARegisterValue1, uint8_t & APotentiometer1, uint8_t & ARegisterValue2, uint8_t & APotentiometer2, uint8_t & ARegisterValue3, uint8_t & APotentiometer3 )
		{
			switch( C_INDEX )
			{
				case 0:
					ARegisterValue0 = GetRegisterPins();
					APotentiometer0 = GetPotentiometer();
					break;

				case 1:
					ARegisterValue1 = GetRegisterPins();
					APotentiometer1 = GetPotentiometer();
					break;

				case 2:
					ARegisterValue2 = GetRegisterPins();
					APotentiometer2 = GetPotentiometer();
					break;

				case 3:
					ARegisterValue3 = GetRegisterPins();
					APotentiometer3 = GetPotentiometer();
					break;

			}
		}

	public:
		inline void SystemStart()
		{
			T_FaultOutputPin::SetPinValue( false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Addresses,
		typename T_Channels_GetPotentiometerRegisters,
		typename T_Channels_UpdateFailure,
		typename T_Channels_UpdateSpeed,
		typename T_FModified
	> class Maxim_MotorDriverShield :
		public T_Addresses,
		public T_FModified
	{
	public:
		_V_PROP_( Addresses )

	public:
//		Mitov::SimpleList<Maxim_MotorDriverBasicChannel<T_INSTANCE> *>	FChannels;
		_V_PROP_( FModified )

		uint8_t FChanged = 0xFF;

	public:
		inline void FaultInputPin_o_Receive( void * _Data )
		{
			if( *(bool *)_Data )
				UpdateFailures();
		}

	protected:
		void DoPotentiometerTransmission( uint8_t Address, uint8_t ARegister, uint8_t AData )
		{
			uint8_t AAddress = 0b0101000 | (( Address & 0b111 ) << 1 );

//			Serial.print( AAddress );
//			Serial.print( " " );
//			Serial.print( ARegister );
//			Serial.print( " " );
//			Serial.println( AData );

			C_I2C.beginTransmission( AAddress );
			C_I2C.write( ARegister );
			C_I2C.write( AData );
			C_I2C.endTransmission();
		}

		void DoGPIOTransmissionAt( uint8_t ARegister, uint8_t AData )
		{
			uint8_t AAddress = 0b01000000 | ( Addresses().MAX7300() & 0b1111 );

			C_I2C.beginTransmission( AAddress );
			C_I2C.write( ARegister );
			C_I2C.write( AData );
			C_I2C.endTransmission();
		}

		uint8_t DoGPIOReadAt( uint8_t ARegister )
		{
			uint8_t AAddress = 0b01000000 | ( Addresses().MAX7300() & 0b1111 );

			C_I2C.beginTransmission( AAddress );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( AAddress, uint8_t( 1 ) );
			return C_I2C.read();
		}

	protected:
		void UpdateSettings()
		{
			uint8_t  ARegisterValue0 = 0;
			uint8_t  ARegisterValue1 = 0;
			uint8_t  ARegisterValue2 = 0;
			uint8_t  ARegisterValue3 = 0;

			uint8_t  APotentiometer0 = 0;
			uint8_t  APotentiometer1 = 0;
			uint8_t  APotentiometer2 = 0;
			uint8_t  APotentiometer3 = 0;

			T_Channels_GetPotentiometerRegisters::Call( ARegisterValue0, APotentiometer0, ARegisterValue1, APotentiometer1, ARegisterValue2, APotentiometer2, ARegisterValue3, APotentiometer3 );

			if( FChanged & 0b00110000 )
			{
				uint8_t	MAX7300Reg = ARegisterValue0 | ( ARegisterValue1 << 4 );
//					Serial.println( MAX7300Reg );
				DoGPIOTransmissionAt( 0x44, MAX7300Reg );
			}

			if( FChanged & 0b11000000 )
			{
				uint8_t	MAX7300Reg = ARegisterValue2 | ( ARegisterValue3 << 4 );
				DoGPIOTransmissionAt( 0x4C, MAX7300Reg );
			}

			if( FChanged & 0b00000011 )
			{
				if( FChanged & 0b00000001 )
					DoPotentiometerTransmission( Addresses().MAX5387_1(), 0b00010001, APotentiometer0 );

				if( FChanged & 0b00000010 )
					DoPotentiometerTransmission( Addresses().MAX5387_1(), 0b00010010, APotentiometer1 );

			}

			if( FChanged & 0b00001100 )
			{
				if( FChanged & 0b00000100 )
					DoPotentiometerTransmission( Addresses().MAX5387_2(), 0b00010001, APotentiometer2 );

				if( FChanged & 0b00001000 )
					DoPotentiometerTransmission( Addresses().MAX5387_2(), 0b00010010, APotentiometer3 );
			}

			T_Channels_UpdateSpeed::Call();
//			for( int i = 0; i < FChannels.size(); ++i )
//				FChannels[ i ]->UpdateSpeed();

			FChanged = 0;
			FModified() = false;
		}

		void UpdateFailures()
		{
			DoGPIOReadAt( 6 ); // Clear the interrupt bit

			uint8_t AFailureFlags = DoGPIOReadAt( 0x5B ); // Read pins 27 and up

			T_Channels_UpdateFailure::Call( AFailureFlags );

//			for( int i = 0; i < FChannels.size(); ++ i )
//				FChannels[ i ]->UpdateFailure( AFailureFlags & ( 1 << i ) == 0 );

		}

	public:
		inline void SystemStart()
		{
			DoGPIOTransmissionAt( 0x4, 0b10000001 ); // Enable and enable the interrupts
			for( int i = 0x9; i < 0x9 + 4; ++i )
				DoGPIOTransmissionAt( i, 0b01010101 ); // All Output

			DoGPIOTransmissionAt( 0x0E, 0xFF ); // All inputs
			DoGPIOTransmissionAt( 0x0E, 0b01111111 ); // P31 Poutput, the rest inputs

			UpdateSettings();

//			for( int i = 0; i < FChannels.size(); ++i )
//				FChannels[ i ]->InitFailure();

		}

		inline void SystemLoopUpdateHardware()
		{
			if( FModified() )
				UpdateSettings();

		}

	public:
		inline void SystemLoopBegin() // Used only for the shield
		{
			if( digitalRead( 2 ) == HIGH )
				UpdateFailures();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif