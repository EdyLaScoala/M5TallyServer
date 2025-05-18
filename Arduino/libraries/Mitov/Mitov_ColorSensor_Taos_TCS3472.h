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
#include <Mitov_BasicMPU.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __TCS3472__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
namespace TCS3472
{
	const uint8_t ENABLE	= 0x00; //< Interrupt Enable register
	const uint8_t ATIME		= 0x01; //< Integration time
	const uint8_t WTIME		= 0x03; //< Wait time (if TCS34725_ENABLE_WEN is asserted)
	const uint8_t AILTL		= 0x04; //< Clear channel lower interrupt threshold (lower byte)
	const uint8_t AILTH		= 0x05; //< Clear channel lower interrupt threshold (higher byte)
	const uint8_t AIHTL		= 0x06; //< Clear channel upper interrupt threshold (lower byte)
	const uint8_t AIHTH		= 0x07; //< Clear channel upper interrupt threshold (higher byte)
	const uint8_t PERS		= 0x0C; //< Persistence register - basic SW filtering mechanism for interrupts
	const uint8_t CONFIG	= 0x0D; //< Configuration
	const uint8_t CONTROL	= 0x0F; //< Set the gain level for the sensor
	const uint8_t DEVICE_ID	= 0x12; //< 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727
	const uint8_t STATUS	= 0x13; //< Device status
	const uint8_t CDATAL	= 0x14; //< Clear channel data low byte
	const uint8_t CDATAH	= 0x15; //< Clear channel data high byte
	const uint8_t RDATAL	= 0x16; //< Red channel data low byte
	const uint8_t RDATAH	= 0x17; //< Red channel data high byte
	const uint8_t GDATAL	= 0x18; //< Green channel data low byte
	const uint8_t GDATAH	= 0x19; //< Green channel data high byte
	const uint8_t BDATAL	= 0x1A; //< Blue channel data low byte
	const uint8_t BDATAH	= 0x1B; //< Blue channel data high byte
}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Period
	> class TArduinoColorSensorThaosTCS3472Wait :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_High,
		typename T_Low,
		typename T_OutputPin,
		typename T_Persistence
	> class TArduinoColorSensorThaosTCS3472Threshold :
		public T_Enabled,
		public T_High,
		public T_Low,
		public T_OutputPin,
		public T_Persistence
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( High )
		_V_PROP_( Low )
		_V_PROP_( Persistence )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ColorOutputPin,
		typename T_Enabled,
//		typename T_FHasData,
		typename T_Gain,
		typename T_IntegrationTime,
		typename T_OutputPins_Red,
		typename T_OutputPins_Green,
		typename T_OutputPins_Blue,
		typename T_OutputPins_Clear,
		typename T_PoweredUp,
		typename T_ThresholdCleat,
		typename T_Wait
	> class ColorSensorTaosTCS3472 :
		public T_Address,
		public T_ColorOutputPin,
		public T_Enabled,
//		public T_FHasData,
		public T_Gain,
		public T_IntegrationTime,
		public T_OutputPins_Red,
		public T_OutputPins_Green,
		public T_OutputPins_Blue,
		public T_OutputPins_Clear,
		public T_PoweredUp,
		public T_ThresholdCleat,
		public T_Wait
	{
	public:
		_V_PIN_( OutputPins_Red )
		_V_PIN_( OutputPins_Green )
		_V_PIN_( OutputPins_Blue )
		_V_PIN_( OutputPins_Clear )

		_V_PIN_( ColorOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Gain )
		_V_PROP_( IntegrationTime )
		_V_PROP_( PoweredUp )
		_V_PROP_( ThresholdCleat )
		_V_PROP_( Wait )

//	protected:
//		_V_PROP_( FHasData )

	protected:
		void WriteRegister8( uint8_t reg, uint8_t value )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( 0b10100000 | reg );
			C_I2C.write( value );
			C_I2C.endTransmission();
		}

		void WriteRegister16( uint8_t reg, uint16_t value )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( 0b10100000 | reg );
			C_I2C.write( uint8_t( value ) );
			C_I2C.write( uint8_t( value << 8 ) );
			C_I2C.endTransmission();
		}

		uint8_t ReadRegister8( uint8_t reg )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( 0b10100000 | reg );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ) );
			return C_I2C.read();
		}

		uint16_t ReadRegister16( uint8_t reg )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( 0b10100000 | reg );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 2 ) );
			uint16_t AValue = C_I2C.read(); // Low bits
			AValue |= uint16_t( C_I2C.read() ) << 8; // High bits
			return AValue;
		}

		void StartReadRegisters( uint8_t reg, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( 0b10100000 | reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), ACount );
		}

	public:
		void UpdateGain()
		{
			uint8_t AValue = 0;

			if( Gain().GetValue() > 16 )
				AValue = 0b11;

			else if( Gain().GetValue() > 4 )
				AValue = 0b10;

			else if( Gain().GetValue() > 1 )
				AValue = 0b01;

			WriteRegister8( TCS3472::CONTROL, AValue );
		}

		void UpdateIntegrationTime()
		{
			uint8_t AValue = Func::MapRange<float>( IntegrationTime().GetValue(), 2.4, 700, 255, 0 ) + 0.5;
			WriteRegister8( TCS3472::ATIME, AValue );
		}

		void UpdateWaitTime()
		{
			uint8_t AValue;
			if( Wait().Period() > 614 )
				AValue = Func::MapRange<float>( IntegrationTime().GetValue(), 28.8, 7368, 255, 0 ) + 0.5;

			else
				AValue = Func::MapRange<float>( IntegrationTime().GetValue(), 2.4, 614, 255, 0 ) + 0.5;

			WriteRegister8( TCS3472::WTIME, AValue );
		}

		void UpdatePowerUp()
		{
			if( PoweredUp() )
			{
				WriteRegister8( TCS3472::ENABLE, 0b00000001 ); // PowerUp First!

				if( Enabled() )
					delay( 3 );

			}

			UpdateEnable();
		}

		void UpdateEnable()
		{
			uint8_t AValue = 0;
			if( Wait().Enabled() )
				AValue |= 0b00001000;

			if( Enabled() )
				AValue |= 0b00000010;

			if( PoweredUp() )
				AValue |= 0b00000001;

			if( ThresholdCleat().Enabled() && ThresholdCleat().OutputPin().GetPinIsConnected() )
				AValue |= 0b00010000;

			WriteRegister8( TCS3472::ENABLE, AValue );
		}

		void UpdateThresholdLow()
		{
			WriteRegister16( TCS3472::AILTL, ThresholdCleat().Low() );
		}

		void UpdateThresholdHigh()
		{
			WriteRegister16( TCS3472::AIHTL, ThresholdCleat().High() );
		}

		void UpdateThresholdPersistence()
		{
			uint8_t AValue = 0;
			if( ThresholdCleat().Persistence() < 2 )
				AValue = 0b0001;

			else if( ThresholdCleat().Persistence() < 3 )
				AValue = 0b0010;

			else if( ThresholdCleat().Persistence() < 5 )
				AValue = 0b0011;

			else if( ThresholdCleat().Persistence() < 10 )
				AValue = 0b0100;

			else if( ThresholdCleat().Persistence() < 15 )
				AValue = 0b0101;

			else if( ThresholdCleat().Persistence() < 20 )
				AValue = 0b0110;

			else if( ThresholdCleat().Persistence() < 25 )
				AValue = 0b0111;

			else if( ThresholdCleat().Persistence() < 30 )
				AValue = 0b1000;

			else if( ThresholdCleat().Persistence() < 35 )
				AValue = 0b1001;

			else if( ThresholdCleat().Persistence() < 40 )
				AValue = 0b1010;

			else if( ThresholdCleat().Persistence() < 45 )
				AValue = 0b1011;

			else if( ThresholdCleat().Persistence() < 50 )
				AValue = 0b1100;

			else if( ThresholdCleat().Persistence() < 55 )
				AValue = 0b1101;

			else if( ThresholdCleat().Persistence() < 60 )
				AValue = 0b1110;

			else
				AValue = 0b1111;
			
			WriteRegister8( TCS3472::PERS, AValue );
		}

		void ReadThreshold( bool AChangeOnly )
		{
			uint8_t AValue = ReadRegister8( TCS3472::STATUS );
			ThresholdCleat().OutputPin().SetPinValue(( AValue & 0b00010000 ), AChangeOnly );
		}

		void ReadSensors( bool AChangeOnly, bool AForceRead )
		{
			uint8_t AValue = ReadRegister8( TCS3472::STATUS );

			ThresholdCleat().OutputPin().SetPinValue(( AValue & 0b00010000 ), AChangeOnly );

//			if( ( AValue & 0b00000001 ) || AForceRead || FHasData().GetValue() )
			if( ( AValue & 0b00000001 ) || AForceRead )
			{
				if( ColorOutputPin().GetPinIsConnected() || ( OutputPins_Clear().GetPinIsConnected() && OutputPins_Blue().GetPinIsConnected() ))
				{

					StartReadRegisters( TCS3472::CDATAL, 8 );

					uint16_t AClear = C_I2C.read(); // Low bits
					AClear |= uint16_t( C_I2C.read() ) << 8; // High bits

					uint16_t ARed = C_I2C.read(); // Low bits
					ARed |= uint16_t( C_I2C.read() ) << 8; // High bits

//					Serial.println( ARed );

					uint16_t AGreen = C_I2C.read(); // Low bits
					AGreen |= uint16_t( C_I2C.read() ) << 8; // High bits

					uint16_t ABlue = C_I2C.read(); // Low bits
					ABlue |= uint16_t( C_I2C.read() ) << 8; // High bits
/*
					uint16_t AClear = ReadRegister16( TCS3472::CDATAL );
					uint16_t ARed = ReadRegister16( TCS3472::RDATAL );
					uint16_t AGreen = ReadRegister16( TCS3472::GDATAL );
					uint16_t ABlue = ReadRegister16( TCS3472::BDATAL );

					Serial.println( AClear );
					Serial.println( ARed );
					Serial.println( AGreen );
					Serial.println( ABlue );
*/

					OutputPins_Clear().SetPinValue( AClear, AChangeOnly );
					OutputPins_Red().SetPinValue( ARed, AChangeOnly );
					OutputPins_Green().SetPinValue( AGreen, AChangeOnly );
					OutputPins_Blue().SetPinValue( ABlue, AChangeOnly );

					if( ColorOutputPin().GetPinIsConnected() )
					{
						if( ! AClear )
							AClear = 1;

						TColor AColor( 
								( float( ARed ) / AClear ) * 255.0 + 0.5,
								( float( AGreen ) / AClear ) * 255.0 + 0.5,
								( float( ABlue ) / AClear ) * 255.0 + 0.5
							);

						ColorOutputPin().SetPinValue( AColor, AChangeOnly );
					}
				}

				else if( OutputPins_Red().GetPinIsConnected() && OutputPins_Blue().GetPinIsConnected() )
				{
					StartReadRegisters( TCS3472::RDATAL, 6 );

					uint16_t AValue2 = C_I2C.read(); // Low bits
					AValue2 |= uint16_t( C_I2C.read() ) << 8; // High bits

					uint16_t AValue3 = C_I2C.read(); // Low bits
					AValue3 |= uint16_t( C_I2C.read() ) << 8; // High bits

					uint16_t AValue4 = C_I2C.read(); // Low bits
					AValue4 |= uint16_t( C_I2C.read() ) << 8; // High bits

					OutputPins_Red().SetPinValue( AValue2, AChangeOnly );
					OutputPins_Green().SetPinValue( AValue3, AChangeOnly );
					OutputPins_Blue().SetPinValue( AValue4, AChangeOnly );
				}

				else if( OutputPins_Red().GetPinIsConnected() && OutputPins_Green().GetPinIsConnected() )
				{
					StartReadRegisters( TCS3472::RDATAL, 4 );

					uint16_t AValue2 = C_I2C.read(); // Low bits
					AValue2 |= uint16_t( C_I2C.read() ) << 8; // High bits

					uint16_t AValue3 = C_I2C.read(); // Low bits
					AValue3 |= uint16_t( C_I2C.read() ) << 8; // High bits

					OutputPins_Red().SetPinValue( AValue2, AChangeOnly );
					OutputPins_Green().SetPinValue( AValue3, AChangeOnly );
				}

				else if( OutputPins_Green().GetPinIsConnected() && OutputPins_Blue().GetPinIsConnected() )
				{
					StartReadRegisters( TCS3472::GDATAL, 4 );

					uint16_t AValue2 = C_I2C.read(); // Low bits
					AValue2 |= uint16_t( C_I2C.read() ) << 8; // High bits

					uint16_t AValue3 = C_I2C.read(); // Low bits
					AValue3 |= uint16_t( C_I2C.read() ) << 8; // High bits

					OutputPins_Green().SetPinValue( AValue2, AChangeOnly );
					OutputPins_Blue().SetPinValue( AValue3, AChangeOnly );
				}

				else if( OutputPins_Clear().GetPinIsConnected() )
				{
					StartReadRegisters( TCS3472::CDATAL, 2 );

					uint16_t AValue1 = C_I2C.read(); // Low bits
					AValue1 |= uint16_t( C_I2C.read() ) << 8; // High bits
					OutputPins_Clear().SetPinValue( AValue1, AChangeOnly );
				}

				else if( OutputPins_Red().GetPinIsConnected() )
				{
					StartReadRegisters( TCS3472::RDATAL, 2 );

					uint16_t AValue1 = C_I2C.read(); // Low bits
					AValue1 |= uint16_t( C_I2C.read() ) << 8; // High bits

					OutputPins_Red().SetPinValue( AValue1, AChangeOnly );
				}

				else if( OutputPins_Green().GetPinIsConnected() )
				{
					StartReadRegisters( TCS3472::GDATAL, 2 );

					uint16_t AValue1 = C_I2C.read(); // Low bits
					AValue1 |= uint16_t( C_I2C.read() ) << 8; // High bits

					OutputPins_Green().SetPinValue( AValue1, AChangeOnly );
				}

				else if( OutputPins_Blue().GetPinIsConnected() )
				{
					StartReadRegisters( TCS3472::BDATAL, 2 );

					uint16_t AValue1 = C_I2C.read(); // Low bits
					AValue1 |= uint16_t( C_I2C.read() ) << 8; // High bits

					OutputPins_Blue().SetPinValue( AValue1, AChangeOnly );
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false, true );
		}

	public:
		inline void SystemInit()
		{
			UpdateGain();
			UpdateIntegrationTime();
			UpdateWaitTime();

			UpdateThresholdLow();
			UpdateThresholdHigh();
			UpdateThresholdPersistence();

			UpdatePowerUp();
		}

		inline void SystemStart()
		{
			ReadSensors( false, false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true, false );
		}

		inline void SystemStartThreshold()
		{
			ReadThreshold( false );
		}

		inline void SystemLoopBeginThreshold()
		{
			ReadThreshold( true );
		}

/*
	public:
		inline ColorSensorTaosTCS3472()
		{
			FHasData() = false;
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

