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
#include <Mitov_Basic_I2C.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
//		typename T_Elements_PopulateAddress,
		typename T_Elements_ProcessWrite,
		typename T_Enabled
	> class ArduinoGenericEEPROM_I2C : // public Mitov::MemoryIntf
		public T_Address,
		public T_Enabled
	{
	public:
		_V_PROP_( Address );
		_V_PROP_( Enabled );

	public:
		bool ReadData( uint32_t AAddress, uint32_t ASize, uint8_t *AData )
		{
			if( ! Enabled() )
				return false;

			if( ASize == 0 )
				return true;

			I2C::ReadBytes_16bitAddress( C_I2C, uint8_t( Address().GetValue()), true, AAddress, ASize, AData );
			AData += ASize;
			AAddress += ASize;

			return true;
		}

		bool WriteData( uint32_t AAddress, uint32_t ASize, uint8_t *AData )
		{
			if( ! Enabled() )
				return false;

	        for( int i = 0; i < ASize; ++i )
				EEPROWrite( AAddress++, *AData++ );

			return true;
		}

	protected:
		inline void EEPROWrite( uint32_t eeaddress, uint8_t data )
		{
			if( ! Enabled() )
				return;

//			Serial.println( eeaddress );
//			Serial.println( data );
//			Serial.println( "-----------" );
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write((uint16_t)(eeaddress >> 8));   // MSB
			C_I2C.write((uint16_t)(eeaddress & 0xFF)); // LSB
			C_I2C.write(data);
			C_I2C.endTransmission();

			delay( 5 );
		}

	public:
/*
		inline void SystemInit()
		{
			uint32_t AAddress = 0;
			uint8_t AOffset = 0;
			T_Elements_PopulateAddress::Call( AAddress, AOffset );
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->PopulateAddress( AAddress, AOffset );

		}
*/
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