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
#include "Mitov_BasicGenerator.h"

// https://wiki.bitcraze.io/_media/projects:crazyflie2:expansionboards:pot0189-pmw3901mb-txqt-ds-r1.00-200317_20170331160807_public.pdf

namespace Mitov
{
//---------------------------------------------------------------------------
namespace PMW3901
{
	static const uint8_t PROGMEM
	  Init1[] =
		{                 
			0x7F, 0x00,
			0x61, 0xAD,
			0x7F, 0x03,
			0x40, 0x00,
			0x7F, 0x05,
			0x41, 0xB3,
			0x43, 0xF1,
			0x45, 0x14,
			0x5B, 0x32,
			0x5F, 0x34,
			0x7B, 0x08,
			0x7F, 0x06,
			0x44, 0x1B,
			0x40, 0xBF,
			0x4E, 0x3F,
			0x7F, 0x08,
			0x65, 0x20,
			0x6A, 0x18,
			0x7F, 0x09,
			0x4F, 0xAF,
			0x5F, 0x40,
			0x48, 0x80,
			0x49, 0x80,
			0x57, 0x77,
			0x60, 0x78,
			0x61, 0x78,
			0x62, 0x08,
			0x63, 0x50,
			0x7F, 0x0A,
			0x45, 0x60,
			0x7F, 0x00,
			0x4D, 0x11,
			0x55, 0x80,
			0x74, 0x1F,
			0x75, 0x1F,
			0x4A, 0x78,
			0x4B, 0x78,
			0x44, 0x08,
			0x45, 0x50,
			0x64, 0xFF,
			0x65, 0x1F,
			0x7F, 0x14,
			0x65, 0x60,
			0x66, 0x08,
			0x63, 0x78,
			0x7F, 0x15,
			0x48, 0x58,
			0x7F, 0x07,
			0x41, 0x0D,
			0x43, 0x14,
			0x4B, 0x0E,
			0x45, 0x0F,
			0x44, 0x42,
			0x4C, 0x80,
			0x7F, 0x10,
			0x5B, 0x02,
			0x7F, 0x07,
			0x40, 0x41,
			0x70, 0x00
		};

	static const uint8_t PROGMEM
	  Init2[] =
		{
			0x32, 0x44,
			0x7F, 0x07,
			0x40, 0x40,
			0x7F, 0x06,
			0x62, 0xf0,
			0x63, 0x00,
			0x7F, 0x0D,
			0x48, 0xC0,
			0x6F, 0xd5,
			0x7F, 0x00,
			0x5B, 0xa0,
			0x4E, 0xA8,
			0x5A, 0x50,
			0x40, 0x80
		};

	static const uint8_t PROGMEM
	  InitImage[] =
		{
			0x7F, 0x07,  //Magic frame readout registers
			0x41, 0x1D,
			0x4C, 0x00,
			0x7F, 0x08,
			0x6A, 0x38,
			0x7F, 0x00,
			0x55, 0x04,
			0x40, 0x80,
			0x4D, 0x11,

			0x70, 0x00,   //More magic? 
			0x58, 0xFF
		};

}
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI, 
		typename T_ChipSelectOutputPin,
		typename T_SPISpeed
	> class OpticFlowPMW3901 :
		public T_ChipSelectOutputPin,
		public T_SPISpeed
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( SPISpeed )

	public:
		uint8_t read8(uint8_t reg)
		{
			C_SPI.beginTransaction(SPISettings( SPISpeed(), MSBFIRST, SPI_MODE3 ));
			T_ChipSelectOutputPin::SetPinValueLow();

//			delayMicroseconds(50);

			C_SPI.transfer( reg & 0x7F );

//			delayMicroseconds(50);

			uint8_t AValue = C_SPI.transfer( 0 );

//			delayMicroseconds(100);

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 

			return AValue;
		}

		void write8(uint8_t reg, uint8_t value)
		{
//			Serial.print( "W " );
//			Serial.print( reg, HEX );
//			Serial.print( ": " );
//			Serial.println( value, HEX );

			C_SPI.beginTransaction(SPISettings( SPISpeed(), MSBFIRST, SPI_MODE3 ));
			T_ChipSelectOutputPin::SetPinValueLow();

//			delayMicroseconds(50);

			C_SPI.transfer( reg | 0x80 );
			C_SPI.transfer(value);

//			delayMicroseconds(50);

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 

//			delayMicroseconds(200);
		}

	public:
		inline void SystemInit()
		{
//			delay ( 2000 );
			C_SPI.beginTransaction(SPISettings( SPISpeed(), MSBFIRST, SPI_MODE3 ));
			// Make sure the SPI bus is reset
			T_ChipSelectOutputPin::SetPinValueHigh();
			delay( 1 );
			T_ChipSelectOutputPin::SetPinValueLow();
			delay( 1 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			delay( 1 );
			C_SPI.endTransaction(); 

			// Power on reset
			write8( 0x3A, 0x5A );
			delay(5);
			// Test the SPI communication, checking chipId and inverse chipId
//			uint8_t chipId = read8( 0x00 );
//			uint8_t dIpihc = read8( 0x5F );

//			delay( 1000 );
//			Serial.println( chipId, HEX );
//			Serial.println( dIpihc, HEX );
//			delay( 1000 );

//			return;

//			if (chipId != 0x49 && dIpihc != 0xB8) return false;

			// Reading the motion registers one time
			read8( 0x02 );
			read8( 0x03 );
			read8( 0x04 );
			read8( 0x05 );
			read8( 0x06 );
			delay(1);

			// initRegisters
			for( int i = 0; i < sizeof( PMW3901::Init1 ); i += 2 )
				write8( pgm_read_byte( PMW3901::Init1 + i ), pgm_read_byte( PMW3901::Init1 + i + 1 ) );

			delay( 100 );

			for( int i = 0; i < sizeof( PMW3901::Init2 ); i += 2 )
				write8( pgm_read_byte( PMW3901::Init2 + i ), pgm_read_byte( PMW3901::Init2 + i + 1 ) );

//			delay ( 1000 );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_OutputPins_X,
		typename T_OutputPins_Y
	> class TArduinoOpticFlowPMW3901_Movement :
		public T_Enabled,
		public T_OutputPins_X,
		public T_OutputPins_Y
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )

	public:
		_V_PROP_( Enabled )

	protected:
		inline void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

//			Serial.println( C_OWNER.read8(0x16));

			C_OWNER.read8( 0x02 );
			int16_t deltaX = ((int16_t)C_OWNER.read8( 0x04 ) << 8) | C_OWNER.read8( 0x03 );
			int16_t deltaY = ((int16_t)C_OWNER.read8( 0x06 ) << 8) | C_OWNER.read8( 0x05 );

//			return;

			T_OutputPins_X::SetPinValue( deltaX );
			T_OutputPins_Y::SetPinValue( deltaY );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_OutputPin
	> class TArduinoOpticFlowPMW3901_Image :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

		uint8_t		FBuffer[ 1225 ];
		uint16_t	FIndex;
//		bool		FNeedsStart

	protected:
		inline void ReadSensor( bool ARestart )
		{
//			Serial.println( FIndex );

			if( FIndex >= 1225 )
				return;

			if( ! Enabled().GetValue() )
				return;

//			int count = 0;
			uint8_t a; //temp value for reading register
			uint8_t b; //temp value for second register
			uint8_t hold; //holding value for checking bits
			uint8_t mask = 0x0c; //mask to take bits 2 and 3 from b
			uint8_t pixel = 0; //temp holding value for pixel

//			uint8_t ABuffer[ 1225 ];

//			Serial.println( "START1" );

			a = C_OWNER.read8(0x58); //read register
			hold = a >> 6; //right shift to leave top two bits for ease of check.
			if(( hold == 0x03 ) || ( hold == 0x00 ))
				return;

//			Serial.println( "START2" );
			b = C_OWNER.read8(0x58); //read next set to get lower 2 bits
			pixel = a; //set pixel to a
			pixel = pixel << 2; //push left to 7:2
			pixel += (b & mask); //set lower 2 from b to 1:0
			FBuffer[ FIndex++ ] = pixel; //put temp value in fbuffer array

			if( FIndex >= 1225 )
			{
				C_OWNER.write8( 0x70, 0x00 );   //More magic? 
				C_OWNER.write8( 0x58, 0xFF );

				TImageBuffer AOutBuffer( TImageFormat::GrayScale, 35, 35, FBuffer );
//				TImageBuffer AOutBuffer( TImageFormat::GrayScale, 8, 8, FBuffer );
				T_OutputPin::SetPinValue( AOutBuffer );

				if( ARestart )
					FIndex = 0;
			}

/*
			for( int ii = 0; ii < 1225; ++ii )
			{ //for 1 frame of 1225 pixels (35*35)
				//check status bits 6 and 7
				//if 01 move upper 6 bits into temp value
				//if 00 or 11, reread
				//else lower 2 bits into temp value
//				Serial.println( ii );
				a = C_OWNER.read8(0x58); //read register
				hold = a >> 6; //right shift to leave top two bits for ease of check.

				while(( hold == 0x03 ) || ( hold == 0x00 )) 
				{ //if data is either invalid status
					a = C_OWNER.read8(0x58); //reread loop
					hold = a >> 6;
//					Serial.print( "." );
				}

				if( hold == 0x01 )
				{ //if data is upper 6 bits
					b = C_OWNER.read8(0x58); //read next set to get lower 2 bits
					pixel = a; //set pixel to a
					pixel = pixel << 2; //push left to 7:2
					pixel += (b & mask); //set lower 2 from b to 1:0
					FBuffer[ count++ ] = pixel; //put temp value in fbuffer array
					//delayMicroseconds(100);
				}
			}

			C_OWNER.write8( 0x70, 0x00 );   //More magic? 
			C_OWNER.write8( 0x58, 0xFF );

			TImageBuffer AOutBuffer( TImageFormat::Grayscale8, 35, 35, ABuffer );

			T_OutputPin::SetPinValue( AOutBuffer );

//			Mitov::String AText = AOutBuffer.ToString();
//			Serial.println( AText );
*/
/*
			int temp = registerRead(0x58); //Read status register 
			int check = temp>>6; //rightshift 6 bits so only top two stay

			while(check == 0x03)
			{ //while bits aren't set denoting ready state
				temp = registerRead(0x58); //keep reading and testing 
				check = temp>>6; //shift again 
			}  
*/
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			FIndex = 0;
			ReadSensor( false );
		}

	public:
		inline void SystemInit()
		{
			for( int i = 0; i < sizeof( PMW3901::InitImage ); i += 2 )
				C_OWNER.write8( pgm_read_byte( PMW3901::InitImage + i ), pgm_read_byte( PMW3901::InitImage + i + 1 ) );

			int temp = C_OWNER.read8( 0x58 ); //Read status register 
			int check = temp >> 6; //rightshift 6 bits so only top two stay

			while( check == 0x03 )
			{ //while bits aren't set denoting ready state
				temp = C_OWNER.read8( 0x58 ); //keep reading and testing 
				check = temp >> 6; //shift again 
			} 

			delayMicroseconds(50);
		}

		inline void SystemStartNoClock()
		{
			FIndex = 0;
			ReadSensor( true );
		}

		inline void SystemStartClock()
		{
			FIndex = 1225;
		}

		inline void SystemLoopBeginNoClock()
		{
			ReadSensor( true );
		}

		inline void SystemLoopBeginClock()
		{
			ReadSensor( false );
		}

	};
//---------------------------------------------------------------------------
}

