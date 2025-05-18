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
#include <Mitov_BasicEthernet.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	namespace ArduinoInternetTime
	{
		const uint8_t NTP_PACKET_SIZE = 48;
		const uint32_t SEVENZYYEARS = 2208988800UL;

		const uint8_t RequestPacket[ NTP_PACKET_SIZE ] =
		{
			0b11100011, // LI, Version, Mode
			0, // Stratum, or type of clock
			6, // Polling Interval
			0xEC, // Peer Clock Precision
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			49,
			0x4E,
			49,
			52
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_SOCKET, T_SOCKET &C_SOCKET,
		typename T_BaseDate,
		typename T_DaylightOffset,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_SetPort,
		typename T_TimeZoneOffset
	> class NetworkTime :
		public T_BaseDate,
		public T_DaylightOffset,
		public T_Enabled,
		public T_OutputPin,
		public T_SetPort,
		public T_TimeZoneOffset
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( TimeZoneOffset )
		_V_PROP_( BaseDate )
		_V_PROP_( DaylightOffset )
        _V_PROP_( Enabled )
		_V_PROP_( SetPort )

	protected:
		unsigned long	FStartTime;

	public:
		void _DirectPinReceive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
//			Serial.println( "Receive" );
//			Serial.println( ADataBlock.Size );

			if( ADataBlock.Size != ArduinoInternetTime::NTP_PACKET_SIZE )
				return;

			unsigned long	ADelay = ( millis() - FStartTime ) / 2;
			uint32_t highWord = word(ADataBlock.Data[40], ADataBlock.Data[41]);
			uint32_t lowWord = word(ADataBlock.Data[42], ADataBlock.Data[43]);

			// combine the four bytes (two words) into a long integer
			// this is NTP time (seconds since Jan 1 1900):
			uint32_t secsSince1900 = highWord << 16 | lowWord;

			highWord = word(ADataBlock.Data[44], ADataBlock.Data[45]);
			lowWord = word(ADataBlock.Data[46], ADataBlock.Data[47]);

			uint64_t AIntFraction = highWord << 16 | lowWord;

//			Serial.println( AIntFraction );
			uint16_t AMssec = ( AIntFraction * 1000) >> 32;
//			Serial.println( AMssec );

			TDateTime ADateTime = BaseDate(); // ( 693596, 0 );

			uint32_t	ATimeValue1 = secsSince1900 / ( 60 * 60 );
			ADateTime.AddHours( ATimeValue1 );
			ADateTime.AddSeconds( secsSince1900 % ( 60 * 60 ));
			ADateTime.AddMilliSeconds( AMssec + ADelay );
			
			if( TimeZoneOffset().GetValue() != 0 ) // For compiler optimization
			{
				int32_t	AIntOffset = TimeZoneOffset();

//					Serial.println( AIntOffset );

				ADateTime.AddHours( AIntOffset );
				float AFraction = V_FMOD( TimeZoneOffset(), 1 );
//					Serial.println( AFraction );

				AIntOffset = AFraction * 60;

//					Serial.println( AIntOffset );
				ADateTime.AddMinutes( AIntOffset );
			}

			if( DaylightOffset().Enabled() && ( DaylightOffset().Offset().GetValue() != 0 )) // For compiler optimization
			{
				int32_t	AIntOffset = DaylightOffset().Offset();
				ADateTime.AddHours( AIntOffset );

				float AFraction = V_FMOD( DaylightOffset().Offset(), 1 );
				AIntOffset = AFraction * 60;

//					Serial.println( AIntOffset );
				ADateTime.AddMinutes( AIntOffset );
			}

			T_OutputPin::SetPinValue( ADateTime );

//			Serial.println( ADateTime.ToString() );

/*
			uint32_t AurrentEpoc = secsSince1900 - ArduinoInternetTime::SEVENZYYEARS;

			uint8_t ASecond = AurrentEpoc % 60;
			Serial.println( ASecond );

			uint8_t AMinute = ( AurrentEpoc % 3600) / 60;
			Serial.println( AMinute );

			uint8_t AHour = ( AurrentEpoc % 86400L) / 3600;
			Serial.println( AHour );

			uint8_t ADay = (( AurrentEpoc  / 86400L) + 4 ) % 7;
			Serial.println( ADay );
*/
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			FIndex = 0;
//			C_SOCKET.StopSocket();
//			Serial.println( "Try Start" );
//			if( SetPort() )
//				C_SOCKET.Port = 37;

//			C_SOCKET.ForceStart();
//			C_SOCKET.Enabled = true;
//			C_SOCKET.TryStartSocket();

			if( SetPort() )
				C_SOCKET.RemotePort() = 123;

			if( ! C_SOCKET.CanSend() )
				return;

			C_SOCKET.BeginPacket();
			C_SOCKET.GetPrint().write( ArduinoInternetTime::RequestPacket, ArduinoInternetTime::NTP_PACKET_SIZE );
			C_SOCKET.EndPacket();

//			Serial.println( "Test1" );
			FStartTime = millis();
		}

	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
