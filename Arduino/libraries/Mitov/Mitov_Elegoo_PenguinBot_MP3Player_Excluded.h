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
#include <Mitov_Maiyout_Serial_MP3.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace BoardDeclarations
{
namespace Types
{
	class SerialPort1 {};
}

namespace Instances
{
	BoardDeclarations::Types::SerialPort1 SerialPort1;
}

}

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Element_Response,
		typename T_Elements_UpdateOutput,
		typename T_Enabled,
		typename T_EqualizerMode,
		typename T_ErrorOutputPin,
		typename T_FExpectFormatResult,
		typename T_FExpectReset,
		typename T_FFirstTry,
		typename T_FIsSending,
		typename T_Playback,
		typename T_SDCard,
		typename T_TimeOutPeriod,
		typename T_TimeoutErrorOutputPin,
		typename T_Track,
		typename T_Volume
	> class Mitov_Elegoo_PenguinBot_MP3Player_Excluded
	{
	public:
		inline void _DirectPinReceive( void *_Data )
		{
		}

	public:
		inline Mitov_Elegoo_PenguinBot_MP3Player_Excluded()
		{
			pinMode( 7, OUTPUT );
			digitalWrite( 7, LOW ); // Disable the Audio Amplifier
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

