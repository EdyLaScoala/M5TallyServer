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
	class StringPrint : public Print
	{
		typedef Print inherited;

	public:
		Mitov::String Value;

	public:
	    virtual size_t write( uint8_t AChar )
		{
			Value += (char)AChar;
			return 1;
		}
	};
//---------------------------------------------------------------------------
	typedef Mitov::SimpleList<uint8_t, uint16_t> TBufferPrintDataList;
//---------------------------------------------------------------------------
	class BufferPrint : public Print
	{
		typedef Print inherited;

	public:
		TBufferPrintDataList Value;

	public:
	    virtual size_t write( uint8_t AChar )
		{
			Value.push_back( AChar );
			return 1;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif