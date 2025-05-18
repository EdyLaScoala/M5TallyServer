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
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Offset
	> class TArduinoTimeDaylightOffset :
		public T_Enabled,
		public T_Offset
	{
    public:
        _V_PROP_( Enabled )
		_V_PROP_( Offset )

	};
//---------------------------------------------------------------------------
	template <
		typename T_SOCKET, T_SOCKET &C_SOCKET,
		typename T_BaseDate,
		typename T_DaylightOffset,
		typename T_Enabled,
		typename T_FIndex,
		typename T_OutputPin,
		typename T_SetPort,
		typename T_TimeZoneOffset
	> class InternetTime :
		public T_BaseDate,
		public T_DaylightOffset,
		public T_Enabled,
		public T_FIndex,
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
		_V_PROP_( FIndex )
		uint8_t	FTimeValue[ 4 ];
		unsigned long	FStartTime;

	public:
		void _DirectPinReceive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
//			Serial.println( "Receive" );
//			Serial.println( ADataBlock.Size );
			uint8_t *ASrc = ADataBlock.Data;
			for( int i = 0; i < ADataBlock.Size; ++i )
			{
				FTimeValue[ 3 - FIndex() ] = *ASrc++;
				FIndex() = FIndex() + 1;
				if( FIndex().GetValue() == 4 )
					break;

//				Serial.println( FIndex );
			}

			if( FIndex().GetValue() == 4 )
			{
				FIndex() = 0;
				unsigned long	ADelay = ( millis() - FStartTime ) / 2;
				C_SOCKET.StopSocket();

//    Result := ( ((BufCard + (FRoundTripDelay div 1000))/ (24 * 60 * 60) ) + Int(fBaseDate))

				uint32_t	ATimeValue;
				memcpy( &ATimeValue, FTimeValue, sizeof( ATimeValue ));
//				Serial.println( ATimeValue );
				TDateTime ADateTime = BaseDate();
//				ADateTime.AddYears( 100 );
//				ATimeValue -= 3155673600;
//				Serial.println( ATimeValue );
				uint32_t	ATimeValue1 = ATimeValue / ( 60 * 60 );
				ADateTime.AddHours( ATimeValue1 );
				ADateTime.AddSeconds( ATimeValue % ( 60 * 60 ));
//				ADateTime.AddSeconds( ATimeValue );
				ADateTime.AddMilliSeconds( ADelay );

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
			}
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			FIndex() = 0;
			C_SOCKET.StopSocket();
//			Serial.println( "Try Start" );
			if( SetPort() )
			{
				C_SOCKET.Port() = 37;
				C_SOCKET.SetRemotePort( 37 );
			}

			C_SOCKET.ForceStart();
//			C_SOCKET.Enabled() = true;
//			C_SOCKET.TryStartSocket();
			C_SOCKET.BeginPacket();
			C_SOCKET.EndPacket();
			FStartTime = millis();
		}

	public:
		inline InternetTime()
		{
			FIndex() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif