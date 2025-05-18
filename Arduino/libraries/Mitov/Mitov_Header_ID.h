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
	template <
		typename T_Enabled,
        typename T_ID,
        typename T_ID_ApplyValues,
        typename T_ID_GetValue,
		typename T_OutputPin
	> class AddPacketHeaderID :
		public T_Enabled,
        public T_ID,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( ID )

	protected:
		void send( const uint8_t* data, uint8_t len )
		{
			if( ! Enabled() )
				T_OutputPin::SetPinValue( Mitov::TDataBlock( len, data ));

			else
			{
				int ASize = len + T_ID::GetCount();
				uint8_t *AData = new uint8_t[ ASize ];

                T_ID::CopyData( AData );
//				memcpy( AData, ID._Bytes, ID._BytesSize );
				memcpy( AData + T_ID::GetCount(), data, len );

				T_OutputPin::SetPinValue( Mitov::TDataBlock( ASize, AData ));

				delete [] AData;
			}
		}

	public:
		void Print( Mitov::String AValue )
		{
			AValue += "\r\n";
			send( (uint8_t *)AValue.c_str(), AValue.length() );
		}

		void Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		inline void PrintChar( char AValue )
		{
			send( (uint8_t*)&AValue, 1 );
		}

		inline void PrintChar( uint8_t AValue )
		{
			send( &AValue, 1 );
		}

		inline void Write( uint8_t *AData, uint32_t ASize )
		{
			send( AData, ASize );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_ID,
        typename T_ID_ApplyValues,
        typename T_ID_GetValue,
		typename T_OutputPin
	> class DetectPacketHeaderID :
		public T_Enabled,
        public T_ID,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( ID )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			Mitov::TDataBlock AInDataBlock = *(Mitov::TDataBlock*)_Data;

			if( AInDataBlock.Size < T_ID::GetCount() )
				return;

//			if( memcmp( AInDataBlock.Data, ID._Bytes, ID._BytesSize ) != 0 )
            if( T_ID::Compare( AInDataBlock.Data ) != 0 )
				return;

			T_OutputPin::SetPinValue( Mitov::TDataBlock( AInDataBlock.Size - T_ID::GetCount(), AInDataBlock.Data + T_ID::GetCount() ));
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif