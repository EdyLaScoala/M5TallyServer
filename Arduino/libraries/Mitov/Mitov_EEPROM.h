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
#include <Mitov_Memory.h>

#ifdef VISUINO_LINKIT_ONE
	#include <LEEPROM.h>
#else
	#include <EEPROM.h>
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		uint32_t C_SIZE,
		typename T_Elements_ProcessWrite,
		typename T_Enabled
	> class ArduinoEEPROM : // : public Mitov::MemoryIntf
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		bool ReadData( uint32_t AAddress, uint32_t ASize, uint8_t *AData )
		{
			if( ! Enabled() )
				return false;

			if( ASize == 0 )
				return	true;

	        for( int i = 0; i < ASize; ++i )
				*AData++ = EEPROM.read( AAddress++ );

			return true;
		}

		bool WriteData( uint32_t AAddress, uint32_t ASize, uint8_t *AData )
		{
			if( ! Enabled() )
				return false;

#if defined( VISUINO_ESP8266 ) || defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_LINKIT_ONE )
//			EEPROM.begin( ASize );

//			Serial.print( "ASize: " ); Serial.println( ASize );
//			Serial.print( "AAddress: " ); Serial.println( AAddress );

	        for( int i = 0; i < ASize; ++i )
				EEPROM.write( AAddress++, *AData++ );

	#ifndef VISUINO_LINKIT_ONE
			EEPROM.commit();
	#endif // VISUINO_LINKIT_ONE

#else
	        for( int i = 0; i < ASize; ++i )
				EEPROM.update( AAddress++, *AData++ );
#endif

			return true;
		}

	public:
		inline void SystemInit()
		{
/*
			uint32_t AAddress = 0;
			uint8_t AOffset = 0;
			for( int i = 0; i < FElements.size(); ++ i )
				FElements[ i ]->PopulateAddress( AAddress, AOffset );
*/
#if defined( VISUINO_ESP8266 ) || defined( VISUINO_ANY_ESP32 )
//			Serial.print( "AOffset: " ); Serial.println( AAddress );
			EEPROM.begin( C_SIZE );
#endif
		}

		inline void SystemLoopUpdateHardware()
		{
			T_Elements_ProcessWrite::Call();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->ProcessWrite( this );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif