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
#include <Mitov_StandardSerial.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//---------------------------------------------------------------------------
namespace Declarations
{
	void G_Arduino_Canaduino_PLC_500_24_WiFiModuleSerial_SystemLoopBegin( void *_Data );
}
//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	class TArduino_Canaduino_PLC_500_24_WiFiModule
	{
/*
public:
		inline void SystemInit()
		{
			Serial3.begin( 115200 );
		}

		inline SERIAL_TYPE &GetStream()
		{
			return Serial3;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_DataBits,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_Parity,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits
	> class TArduino_Canaduino_PLC_500_24_WiFiModuleSerial : 
		public SerialPort <
				T_SERIAL_TYPE, 
				C_SERIAL, 
				T_AfterSendingDelay,
				T_DataBits,
				T_Enabled,
				T_FEndTime,
				T_FSending,
				T_OutputPin,
				T_Parity,
				T_SendingOutputPin,
				T_Speed,
				T_StopBits
			>
	{
		typedef SerialPort <
				T_SERIAL_TYPE, 
				C_SERIAL, 
				T_AfterSendingDelay,
				T_DataBits,
				T_Enabled,
				T_FEndTime,
				T_FSending,
				T_OutputPin,
				T_Parity,
				T_SendingOutputPin,
				T_Speed,
				T_StopBits
			> inherited;

	public:
		inline void SystemLoopBeginOutput()
		{
#ifdef MITOV_PLC_500_24_WIFI
			if( ! Enabled() )
				return;

			uint8_t ABuffer[ 20 ];

			int i = 0;
			for( ; i < 20; ++i )
			{
				int AData = C_SERIAL.read();
//				Serial.println( AData );
				if( AData < 0 )
					break;

				ABuffer[ i ] = AData;
			}

			if( i > 0 )
			{
				Mitov::TDataBlock ADataBlock( i, ABuffer );
				::Declarations::G_Arduino_Canaduino_PLC_500_24_WiFiModuleSerial_SystemLoopBegin( &ADataBlock );
				T_OutputPin::SetPinValue( ADataBlock );
			}

#else // MITOV_PLC_500_24_WIFI
			inherited::SystemLoopBeginOutput();
#endif // MITOV_PLC_500_24_WIFI
		}

	};
//---------------------------------------------------------------------------
	class TArduino_Canaduino_PLC_500_24_WiFiModule_Reset
	{
	public:
		template <typename T> static inline void Reset( T *AInstance )
		{
			digitalWrite( 17, HIGH );
			delay(100);
			digitalWrite( 17, LOW );
			delay(3000);
			
//			Serial3.SetSpeed( 115200 );
//			delay(100);
//			Serial3.GetStream().println( "AT+UART_CUR=19200,8,1,0,0" );
//			delay(10);
//			Serial3.SetSpeed( 19200 );
//			delay(10);
		}
	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_RESET,
		typename T_AccessPoint,
		typename T_AccessPointAddressOutputPin,
		typename T_AccessPointMACOutputPin,
		uint32_t C_ACESS_POINTS_COUNT,
		typename T_FEmptyLineDetected,
		typename T_FInSocketProcessing,
		typename T_FInSocketReceiving,
		typename T_FIsStarted,
		typename T_FReadConnectionId,
		typename T_RemoteAccessPoints_TryConnect,
		typename T_SendReset,
		typename T_StationAddressOutputPin,
		typename T_StationMACOutputPin
	> class TArduino_Canaduino_PLC_500_24_WiFiModule : 
		public ESP8266 <
				TArduino_Canaduino_PLC_500_24_WiFiModuleSerial,
				G_Arduino_Canaduino_PLC_500_24_WiFiModuleSerial,
				TArduino_Canaduino_PLC_500_24_WiFiModule_Reset,
				T_AccessPoint,
				T_AccessPointAddressOutputPin,
				T_AccessPointMACOutputPin,
				C_ACESS_POINTS_COUNT,
				T_FEmptyLineDetected,
				T_FInSocketProcessing,
				T_FInSocketReceiving,
				T_FIsStarted,
				T_FReadConnectionId,
				T_RemoteAccessPoints_TryConnect,
				T_SendReset,
				T_StationAddressOutputPin,
				T_StationMACOutputPin
			>

	{
	};
*/
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

