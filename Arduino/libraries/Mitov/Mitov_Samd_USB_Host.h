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

#ifndef Notify
  #define Notify(...) ((void)0)
#endif
#include <Usb.h>

#ifdef Notify
  #undef Notify
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	class TArduinoSamdUSBHostModule
	{
	protected:
		USBHost FUSBHost;

	public:
		inline USBHost *GetUSBHost() { return &FUSBHost; }

	public:
		inline void SystemInit()
		{
#ifdef SEEEDUINO_WIO_TERMINAL
			FUSBHost.Init();
			digitalWrite(PIN_USB_HOST_ENABLE, LOW);
			digitalWrite(OUTPUT_CTR_5V, HIGH);
#endif // SEEEDUINO_WIO_TERMINAL
		}

		inline void SystemLoopBegin()
		{
			FUSBHost.Task();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif