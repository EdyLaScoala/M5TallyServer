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

namespace Mitov
{
//---------------------------------------------------------------------------
namespace TArduinoBluetoothUartType
{
	enum TArduinoBluetoothUartType { Nordic, HC08 };
}
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class VirtualSerialData :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.DataReceived( _Data );
		}

	public:
		inline void SendData( uint32_t ALength, uint8_t *AData )
		{
			if( T_OutputPin::GetPinIsConnected() )
				T_OutputPin::SetPinValue( Mitov::TDataBlock( ALength, AData ) );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_BufferSize,
		typename T_Data_SendData,
		typename T_Enabled,
		typename T_OutputPin
	> class VirtualSerial :
		public T_BufferSize,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( BufferSize )

	protected:
	    static size_t write_Impl( VirtualSerial * AInstance, uint8_t AByte )
		{
			return AInstance->Write( AByte );
		}

	public:
		inline void DataReceived( void * _Data )
		{
			if( T_OutputPin::GetPinIsConnected() )
				if( Enabled() )
				{
					Mitov::TDataBlock *ADataBlock = ( Mitov::TDataBlock * )_Data;
					T_OutputPin::SetPinValue( *ADataBlock );
				}

		}

	public:
		typedef VirtualStreamOut<VirtualSerial> T_STREAM;


	public:
		inline VirtualStreamOut<VirtualSerial> GetStream()
		{
			return VirtualStreamOut<VirtualSerial>( this, (typename VirtualStreamOut<VirtualSerial>::TCallback) &VirtualSerial::write_Impl );
		}

	public:
		inline template <typename T> void Print( T AValue )
		{
			if( Enabled() )
				GetStream().println( AValue );

		}

		inline void Print( Mitov::String AValue )
		{
			if( Enabled() )
				GetStream().println( AValue );

		}

		inline void Print( char *AValue )
		{
			if( Enabled() )
				GetStream().println( AValue );

		}

		inline void PrintChar( char AValue )
		{
			if( Enabled() )
				GetStream().print( AValue );

		}

		size_t Write( uint8_t *AData, uint32_t ASize )
		{
			if( Enabled() )
				for( uint32_t i = 0; i < ASize; ++i )
					Write( *AData ++ );

            return ASize;
		}

	    inline size_t Write(uint8_t byte)
		{
			if( Enabled() )
			{
				T_Data_SendData::Call( 1, &byte );
				return 1;
			}

			return 0;
		}

	public:
		inline void SystemLoopBeginOutput()
		{
			if( ! Enabled() )
				return;

			uint8_t ABuffer[ 20 ];

			int i = 0;
			for( ; i < 20; ++i )
			{
				int AData = GetStream().read();
				if( AData < 0 )
					break;

				ABuffer[ i ] = AData;
			}

			if( i > 0 )
				T_OutputPin::SetPinValue( Mitov::TDataBlock( i, ABuffer ));

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_BufferSize,
		typename T_Data_SendData,
		typename T_Enabled,
		typename T_OutputPin
	> class VirtualSerial_Stream :
		public Buffered_Stream,
		public T_BufferSize,
		public T_Enabled,
		public T_OutputPin
	{
		typedef Buffered_Stream inherited;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( BufferSize )

	public:
	    virtual size_t write( uint8_t AByte ) override
		{
			if( ! Enabled().GetValue() )
				return 0;

			T_Data_SendData::Call( 1, &AByte );
			return 1;
		}

	public:
		inline void DataReceived( void * _Data )
		{
			if( ! Enabled().GetValue() )
				return;

			Mitov::TDataBlock *ADataBlock = ( Mitov::TDataBlock * )_Data;
			SendToBuffer( ADataBlock->Data, ADataBlock->Size, BufferSize().GetValue() );
			T_OutputPin::SetPinValue( *ADataBlock );
		}

	public:
		template <typename T> void Print( T AValue )
		{
			if( Enabled() )
				println( AValue );

		}

		inline void Print( Mitov::String AValue )
		{
			if( Enabled() )
				println( AValue );

		}

		inline void Print( char *AValue )
		{
			if( Enabled() )
				println( AValue );

		}

		inline void PrintChar( char AValue )
		{
			if( Enabled() )
				print( AValue );

		}

		inline size_t Write( uint8_t *AData, uint32_t ASize )
		{
			if( Enabled() )
				return write( AData, ASize );

            return 0;
		}

		inline size_t Write( uint8_t AValue )
		{
			if( Enabled() )
				return write( AValue );

            return 0;
		}

	};
//---------------------------------------------------------------------------
}

