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
	template <
		typename T_OutputPin
	> class RaspberryPiConsole :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		template <typename T> void Print( T AValue )
		{
			Console.println( AValue );
		}

		inline void PrintChar( char AValue )
		{
			Console.write( AValue );
//			send( (uint8_t*)&AValue, 1 );
		}

		inline void PrintChar( uint8_t AValue )
		{
			Console.write( AValue );
//			send( &AValue, 1 );
		}

		inline size_t Write( uint8_t *AData, uint32_t ASize )
		{
			return Console.write( AData, ASize );
		}

	    inline size_t Write(uint8_t AByte)
		{
			return Console.write( AByte );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

