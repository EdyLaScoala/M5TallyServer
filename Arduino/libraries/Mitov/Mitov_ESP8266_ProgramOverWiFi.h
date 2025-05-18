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
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ErrorOutputPin,
		typename T_Hostname,
		typename T_Password,
		typename T_Port,
		typename T_ProgressOutputPin,
		typename T_UploadingOutputPin
	> class ProgramESP8266OverWiFi :
		public T_ErrorOutputPin,
		public T_Hostname,
		public T_Password,
		public T_Port,
		public T_ProgressOutputPin,
		public T_UploadingOutputPin
	{
	public:
		_V_PIN_( UploadingOutputPin )
		_V_PIN_( ProgressOutputPin )
		_V_PIN_( ErrorOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Hostname )
		_V_PROP_( Password )
		
	public:
		inline void TryStart()
		{
			ArduinoOTA.onStart([ this ]() 
					{
						T_UploadingOutputPin::SetPinValueHigh();
//						StartOutputPin.Notify( nullptr );
					}
				);

			ArduinoOTA.onEnd([ this ]() 
					{
						T_UploadingOutputPin::SetPinValueLow();
//						EndOutputPin.Notify( nullptr );
					}
				);

			ArduinoOTA.onProgress([ this ](unsigned int progress, unsigned int total) 
					{
						float AValue = float( progress ) / total;
						T_ProgressOutputPin::SetPinValue( AValue, true );
//						Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
					}
				);

			ArduinoOTA.onError([ this ](ota_error_t error) 
					{
						T_ErrorOutputPin::SetPinValue( error );
					}
				);

			ArduinoOTA.setPort( Port() );
//			ArduinoOTA.setRebootOnSuccess( RebootOnSuccess );
//			ArduinoOTA.setMdnsEnabled( MdnsEnabled );

//			ArduinoOTA.setPassword( "admin" );
//			ArduinoOTA.setPassword( nullptr );
//			Serial.println( "TEST1" );
//			delay( 1000 );

			if( Hostname().GetValue() != "" )
				ArduinoOTA.setHostname( (char *)Hostname().c_str() );

			if( Password().GetValue() != "" )
				ArduinoOTA.setPassword( (char *)Password().c_str() );

			else
				ArduinoOTA.setPassword( nullptr );

//			ArduinoOTA.setPassword( "" );
			ArduinoOTA.begin();


//			Serial.println( ArduinoOTA.getHostname());
		}

	public:
		inline void SystemStart()
		{
			T_ProgressOutputPin::SetPinValue( 0, false );
		}

		inline void SystemLoopBegin()
		{
			ArduinoOTA.handle();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif