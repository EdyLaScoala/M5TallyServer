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
#include <Mitov_SoftwareSerial.h>
#include <Mitov_Maiyout_Serial_MP3.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//---------------------------------------------------------------------------
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
	> class ElegooPenguinBotMP3Player :
		public MitovMaiyoutSerialMP3<
			T_SERIAL, C_SERIAL,
			T_Element_Response,
			T_Elements_UpdateOutput,
			T_Enabled,
			T_EqualizerMode,
			T_ErrorOutputPin,
			T_FExpectFormatResult,
			T_FExpectReset,
			T_FFirstTry,
			T_FIsSending,
			T_Playback,
			T_SDCard,
			T_TimeOutPeriod,
			T_TimeoutErrorOutputPin,
			T_Track,
			T_Volume
		>
	{
	public:
		typedef MitovMaiyoutSerialMP3<
				T_SERIAL, C_SERIAL,
				T_Element_Response,
				T_Elements_UpdateOutput,
				T_Enabled,
				T_EqualizerMode,
				T_ErrorOutputPin,
				T_FExpectFormatResult,
				T_FExpectReset,
				T_FFirstTry,
				T_FIsSending,
				T_Playback,
				T_SDCard,
				T_TimeOutPeriod,
				T_TimeoutErrorOutputPin,
				T_Track,
				T_Volume
			> inherited;

	public:
		inline void UpdateEnabled()
		{
			digitalWrite( 7, ( Enabled().GetValue() ) ? HIGH : LOW );
		}

//	protected:
//		static inherited	*FInstance;

	protected:
//		static void PinCallerReceive( void *_Data )
//		{
//			FInstance->_DirectPinReceive( _Data );
//		}

	public:
		inline void SystemInit( bool AEnabled )
		{
//			FInstance = this;
//			GSoftwareSerial.OutputPin().SetCallback( PinCallerReceive );

			pinMode( 7, OUTPUT );
			digitalWrite( 7, (AEnabled) ? HIGH : LOW );

			C_SERIAL.SystemInit();
			inherited::SystemInit();
		}

		inline void SystemLoopBegin()
		{
			C_SERIAL.SystemLoopBegin();
			inherited::SystemLoopBegin();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif