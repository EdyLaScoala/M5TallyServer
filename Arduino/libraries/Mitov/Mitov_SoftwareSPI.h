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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <int MOSI_PIN_NUMBER, int CLOCK_PIN_NUMBER> class VisuinoSoftwareSPI
	{
	public:
		uint16_t transfer16(uint16_t data)
		{
			shiftOut( MOSI_PIN_NUMBER, CLOCK_PIN_NUMBER, MSBFIRST, data >> 8 );
			shiftOut( MOSI_PIN_NUMBER, CLOCK_PIN_NUMBER, MSBFIRST, data );

			return 0;
		}

		uint8_t transfer(uint8_t data)
		{
			shiftOut( MOSI_PIN_NUMBER, CLOCK_PIN_NUMBER, MSBFIRST, data );

			return 0;
		}

		void transfer(void *buf, size_t count)
		{
			for( int i = 0; i < count; ++i )
				shiftOut( MOSI_PIN_NUMBER, CLOCK_PIN_NUMBER, MSBFIRST, ((uint8_t *)buf)[ i ] );
		}

		void beginTransaction(SPISettings settings)
		{
		}

		void endTransaction()
		{
		}

		void setDataMode( uint8_t dataMode )
		{
		}

/*
		void setBitOrder( uint8_t bitOrder )
		{
		}
*/
		void setClockDivider( uint8_t clockDiv )
		{
		}

		void usingInterrupt( uint8_t interruptNumber )
		{
		}

	public:
		inline void SystemInit()
		{
			pinMode( MOSI_PIN_NUMBER, OUTPUT );
			pinMode( CLOCK_PIN_NUMBER, OUTPUT );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif