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
#define Console UNGUARDED DEBUG PRINT!!!
#endif

// #define __TCP_SERVER__DEBUG__
// #define __TCP_CLIENT__DEBUG__

namespace Mitov
{
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT, typename T> class EthernetSocketInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.print( "DoReceive" );
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();

//				Serial.print( "PRINT: " );
//				Serial.println( *(T*)_Data );

				C_ROOT.GetPrint().println( *(T*)_Data );
				C_ROOT.EndPacket();
			}
		}

	};
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT> class EthernetSocketInput<T_ROOT, C_ROOT, uint64_t>
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.print( "DoReceive" );
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();

//				Serial.print( "PRINT: " );
//				Serial.println( *(T*)_Data );

				C_ROOT.GetPrint().println( Func::UInt64ToString( *(uint64_t*)_Data ));
				C_ROOT.EndPacket();
			}
		}

	};
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT> class EthernetSocketInput<T_ROOT, C_ROOT, int64_t>
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.print( "DoReceive" );
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();

//				Serial.print( "PRINT: " );
//				Serial.println( *(T*)_Data );

				C_ROOT.GetPrint().println( Func::Int64ToString( *(int64_t*)_Data ));
				C_ROOT.EndPacket();
			}
		}

	};
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT, typename T> class EthernetSocketInput_Binary
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.println( "InputPin_o_Receive" );
			if( C_ROOT.CanSend() )
			{
//				Serial.println( "TEST11" );
				C_ROOT.BeginPacket();
				Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;

//				Serial.println( ADataBlock.Size );
//				Serial.print( ADataBlock.ToString() );

				C_ROOT.GetPrint().write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT, typename T> class EthernetSocketInput_Buffer
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				Mitov::TArray<float> ADataBlock = *(Mitov::TArray<float>*)_Data;

//				Serial.println( ADataBlock.Size );
				C_ROOT.GetPrint().write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
//				C_ROOT.GetPrint().write( *(unsigned char*)_Data );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT, typename T> class EthernetSocketInput_String
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				C_ROOT.GetPrint().println( (char*)_Data );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT> class EthernetSocketInput_Clock
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				C_ROOT.GetPrint().println( "(Clock)" );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template <typename T_ROOT, T_ROOT &C_ROOT, typename T_OBJECT> class EthernetSocketInput_Object
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				C_ROOT.GetPrint().println( ((T_OBJECT *)_Data)->ToString().c_str() );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
}	

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#undef Console
#endif

