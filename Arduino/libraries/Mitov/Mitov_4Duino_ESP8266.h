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
namespace PinCalls
{
	class T4DDuinoESP8266SerialPinCallerReceive
	{
	public:
		void Notify( void *_Data );

	};
} // PinCalls
//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	typedef Mitov::VisuinoSoftwareSerial<
		9, // 0
		8, // 1
		Mitov::ConstantProperty<9, uint32_t, 0 >, // AfterSendingDelay
		Mitov::ConstantProperty<7, bool, true >, // Enabled
		Mitov::ConstantProperty<51, uint32_t, 0>, // FEndTime
		Mitov::ConstantProperty<50, bool, false>, // FSending
		Mitov::GenericPin_EmbeddedPinImplementation<5, ::PinCalls::T4DDuinoESP8266SerialPinCallerReceive>, // OutputPin
		Mitov::DigitalPin_NoImplementation<4 >, // SendingOutputPin
		Mitov::ConstantProperty<8, uint32_t, 19200 > // Speed
	> T4DDuinoESP8266Serial;
//---------------------------------------------------------------------------
	T4DDuinoESP8266Serial GESP8266SoftwareSerial;
//---------------------------------------------------------------------------
	class T4DDuinoESP8266Reset
	{
	public:
		template <typename T> static inline void Reset( T *AInstance )
		{
			digitalWrite( 17, HIGH );
			delay(100);
			digitalWrite( 17, LOW );
			delay(3000);
			
			GESP8266SoftwareSerial.SetSpeed( 115200 );
			delay(100);
			GESP8266SoftwareSerial.GetStream().println( "AT+UART_CUR=19200,8,1,0,0" );
			delay(10);
			GESP8266SoftwareSerial.SetSpeed( 19200 );
			delay(10);
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

