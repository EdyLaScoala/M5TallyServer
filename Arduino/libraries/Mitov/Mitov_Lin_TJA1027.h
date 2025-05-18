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
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Delay,
		typename T_Elements_HandleReceive,
		typename T_Elements_HandleResponse,
		typename T_Elements_SendPacket,
		typename T_Elements_SendResponse,
		typename T_Enabled,
//		typename T_FCountCheck,
//		typename T_FInProcessByte,
		typename T_FReadIndex,
		typename T_FSize,
		typename T_OutputPin,
//		uint8_t C_RESPONSE_COUNT,
//		uint8_t C_RECEIVE_COUNT,
		typename T_SleepOutputPin
	> class MitovLinTJA1027 :
		public T_Delay,
		public T_Enabled,
//		public T_FCountCheck,
//		public T_FInProcessByte,
		public T_FReadIndex,
		public T_FSize,
		public T_OutputPin,
		public T_SleepOutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( SleepOutputPin )

//	protected:
//		static const uint32_t bound_rate = 10417; // 10417 is best for LIN Interface, most device should work
//		static const uint8_t period = 96; // in microseconds, 1s/10417
//		static const uint8_t no_bits = 13;
//		static const uint32_t del = period * no_bits; // delay for number of bits (no-bits) in microseconds, depends on period

	public:
		_V_PROP_( Delay )
		_V_PROP_( Enabled )

	protected:
//		Mitov::TValueArray FResponseArray;
		unsigned long	FLastTime;
//		_V_PROP_( FInProcessByte )
//		_V_PROP_( FCountCheck )
		_V_PROP_( FSize )
		uint16_t FProcessingRequestIndex = 0;
		Mitov::SimpleList<uint16_t>	FRequests;

	public:
		_V_PROP_( FReadIndex )
		uint8_t FResponseArray[ 8 + 1 + 2 ];

	public:
		template <typename T_ELEMENT> inline void HandleResponse( T_ELEMENT *AElement )
		{
			SendOutput();
			FReadIndex() = 0;
			SetSend( true );
			delayMicroseconds( 5 );

			uint8_t ASum = 0;

//			uint8_t *AData = AElement->InitialValue.Data;
			int ASize = AElement->InitialValue().GetCount();
			for( int i = 0; i < ASize; ++i )
			{

//									Serial.print( AData[ i ], HEX ); Serial.print( " " );
                uint8_t AValue = AElement->InitialValue().GetByteAt( i );
				ASum += AValue;
				C_SERIAL.Write( AValue );
			}

//			ASum = 255 - ( ASum + 1 );
			ASum = 254 - ASum;

			C_SERIAL.Write( ASum );

//								Serial.println( "REPLY" );

			FLastTime = micros();
			SetSend( false );
//			return;
		}

	protected:
		void SendOutput()
		{
			Mitov::TDataBlock ADataBlock( FReadIndex(), FResponseArray );
			T_OutputPin::SetPinValue( ADataBlock );
		}

		void ProcessByte( uint8_t AValue )
		{
//			Serial.println( AValue, HEX );
//			Serial.println( FReadIndex() );
//			Serial.println( FSize() );

//			unsigned long	ACurrentTime = micros();
//			if( ( ! FReadIndex().GetValue() ) || ( ACurrentTime - FLastTime ) < Delay().GetValue() )
//			{
				FLastTime = micros();
				if( FReadIndex() < FSize() )
				{
					FResponseArray[ FReadIndex() ] = AValue;
					FReadIndex() = FReadIndex() + 1;
//					return;
				}

/*
			if( FReadIndex() == 2 )
			{ // Check if it is request!
//						Serial.println( "++++" );
				if( FResponseArray[ 0 ] == 0x55 )
				{
					T_Elements_HandleResponse::Call( FResponseArray[ 1 ] );
					return;
				}
			}
*/
//			if( FReadIndex() >= FSize())
//				ProcessBuffer();

		}

		void ProcessBuffer()
		{
			SendOutput();

			if( FReadIndex() < 2 )
			{
				FReadIndex() = 0;
				return;
			}

			if( FResponseArray[ 0 ] != 0x55 )
				return; // Error!

			uint8_t AChecksum = FResponseArray[ FReadIndex() - 1 ];
			uint8_t ASum = 0;
			for( int i = 2; i < FReadIndex() - 1; ++i )
			{
//					Serial.print( FResponseArray[i], HEX ); Serial.print( " " );
				ASum += FResponseArray[i];
			}

//				Serial.println( "" );

			ASum = 254 - ASum;
//				Serial.print( ASum, HEX ); Serial.print( " - " ); Serial.println( AChecksum, HEX );
			if( AChecksum != ASum )
				return; // Error!

			T_Elements_HandleReceive::Call( FResponseArray[ 1 ] );
/*
			for( int i = 0; i < C_RECEIVE_COUNT; ++i )
				if( FReceives[ i ]->ID == FResponseArray[ 1 ] )
				{
					FReceives[ i ]->SendData( FResponseArray + 2, FReadIndex() - 3 );
					FReadIndex() = 0;
					return;
				}
*/
			FReadIndex() = 0;
		}

		inline void SetSend( bool AValue )
		{
			T_SleepOutputPin::SetPinValue( AValue );
		}

	public:
		template <typename T_ELEMENT> void SendOneRequest( T_ELEMENT *ASender, uint16_t ASenderIndex, uint8_t AID, uint8_t ASize )
		{
			if( ASize > 8 )
				ASize = 8;

//			Serial.println( "REQUEST" );

//			FResponseArray.Resize( ASize );
//			if( ASize )
			{
				FSize() = ASize + 1;
				FReadIndex() = 0;
				FProcessingRequestIndex = ASenderIndex + 1;
			}

			SetSend( true );
//			uint32_t del = period * no_bits; // delay for number of bits (no-bits) in microseconds, depends on period

			unsigned long	ACurrentTime = micros();
			unsigned long	ALastDelay = ACurrentTime - FLastTime;
			if( ALastDelay < Delay().GetValue() )
				delayMicroseconds( Delay().GetValue() - ALastDelay ); // delay

			delayMicroseconds( 10 );

			C_SERIAL.GetStream().write( 0x55 );
			C_SERIAL.GetStream().write( AID );

//			return;
//			Serial.println( "TEST1" );

			ACurrentTime = micros();
			FLastTime = ACurrentTime;
/*
			if( ASize )
			{
				while( FReadIndex() < FSize() )
				{
					C_SERIAL_OUTPUT.SystemLoopBegin( ACurrentTime );
					ACurrentTime = micros();
					if( ( ACurrentTime - FLastTime ) > del )
						break;
				}

				uint8_t AChecksum = FResponseArray[ FReadIndex() - 1 ];
				uint8_t ASum = 0;
				for( int i = 0; i < FReadIndex() - 1; ++i )
					ASum += FResponseArray[i];

				ASum = 254 - ASum;
				FProcessingRequest->SendResponse( FResponseArray, FReadIndex(), ( AChecksum != ASum ) );
				FProcessingRequest = nullptr;
				FSize() = 8 + 1 + 2;
			}
*/
			SetSend( false );
		}

		template <typename T_ELEMENT> void SendOneWrite( T_ELEMENT *AElement, uint8_t AID )
		{
			SetSend( true );
//			uint32_t del = period * no_bits; // delay for number of bits (no-bits) in microseconds, depends on period

//			uint8_t ASum = 0;
//			uint8_t *AData = AElement->InitialValue.Data;
//			for( int i = 0; i < AElement->InitialValue.Size; ++i )
//				ASum += AElement->InitialValue().Data().GetByteAt( i );
//				ASum += AData[ i ];

			unsigned long	ACurrentTime = micros();
			unsigned long	ALastDelay = ACurrentTime - FLastTime;
			if( ALastDelay < Delay().GetValue() )
				delayMicroseconds( Delay().GetValue() - ALastDelay ); // delay

			delayMicroseconds( 10 );

//			typename T_SERIAL::T_STREAM AStream = C_SERIAL.GetStream();
			C_SERIAL.Write( 0x55 );
			C_SERIAL.Write( AID );

			uint8_t ASum = 0;
			for( int i = 0; i < AElement->InitialValue().GetCount(); ++i )
            {
                uint8_t AValue = AElement->InitialValue().GetByteAt( i );
				ASum += AValue;
//				C_SERIAL.Write( AData[ i ] );
				C_SERIAL.Write( AValue );
            }

//			ASum = 255 - ( ASum + 1 );
			ASum = 254 - ASum;

			C_SERIAL.Write( ASum );

			FLastTime = micros();

			SetSend( false );
		}

	public:
		void _DirectPinReceive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
//			Serial.println( ADataBlock.ToString() );
//			delay( 200 );
			for( int i = 0; i < ADataBlock.Size; ++i )
				ProcessByte( ADataBlock.Data[ i ] );
		}

	public:
		template <typename T_ELEMENT> void SendRequest( T_ELEMENT *ASender, uint16_t ASenderIndex, uint8_t AID )
		{
			if( ! Enabled() )
				return;

			if( ! FProcessingRequestIndex )
				SendOneRequest( ASender, ASenderIndex, AID, 8 );

			else
			{
//				TRequestElement AElement( ASender, AID, 8 ); // Requires temporary variable due to compiler bug! :-(
				FRequests.push_back( ASenderIndex );
			}

		}

		template <typename T_ELEMENT> void SendWrite( T_ELEMENT *ASender, uint16_t ASenderIndex, uint8_t AID )
		{
			if( ! Enabled() )
				return;

			if( ! FProcessingRequestIndex )
				SendOneWrite<T_ELEMENT>( ASender, AID );

			else
			{
//				TRequestElement AElement( ASender, AID, 9 ); // Requires temporary variable due to compiler bug! :-(
				FRequests.push_back( ASenderIndex );
			}

		}

	public:
		inline void SystemInit()
		{
			FLastTime = micros();
		}

/*
		inline void SystemStart()
		{
//			C_SERIAL.begin( bound_rate );
		}
*/
		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

			if( ( micros() - FLastTime ) > Delay().GetValue() )
			{
				if( ! FProcessingRequestIndex )
				{
					if( FReadIndex() > 0 )
					{
						if( FReadIndex() == 2 )
						{
							if( FResponseArray[ 0 ] == 0x55 )
							{
								T_Elements_HandleResponse::Call( FResponseArray[ 1 ] );
								FReadIndex() = 0;
							}
						}

						else
							ProcessBuffer();

					}

				}

				else
				{
					if( FReadIndex() > 0 )
					{
						uint8_t AChecksum = FResponseArray[ FReadIndex() - 1 ];
						uint8_t ASum = 0;
						for( int i = 0; i < FReadIndex() - 1; ++i )
							ASum += FResponseArray[i];

						ASum = 254 - ASum;
						T_Elements_SendResponse::Call( FProcessingRequestIndex - 1, FResponseArray, FReadIndex() - 1, ( AChecksum != ASum ) );
//						FProcessingRequest = nullptr;
						FSize() = 8 + 1 + 2;
					 }

					 else
						T_Elements_SendResponse::Call( FProcessingRequestIndex - 1, nullptr, 0, true );

					FProcessingRequestIndex = 0;
					FReadIndex() = 0;
				}

			}

			if( ! FProcessingRequestIndex )
				if( FRequests.size() )
				{
					uint16_t AElementIndex = FRequests[ 0 ];
					FRequests.pop_front();

					T_Elements_SendPacket::Call( FProcessingRequestIndex );
				}

		}

	public:
		inline MitovLinTJA1027()
		{
//			FInProcessByte() = false;
//			FCountCheck() = 0;
			FReadIndex() = 0;
			FSize() = 8 + 1 + 2;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ErrorOutputPin,
		typename T_ID,
		uint16_t C_INDEX,
		typename T_OutputPin
	> class TArduinoLinBusReadRawElement :
//		public T_ClockInputPin_o_IsConnected,
//		public TArduinoLinBusBasicReadRawElement
		public T_ErrorOutputPin,
		public T_ID,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ErrorOutputPin )

	public:
		_V_PROP_( ID )

	public:
		void SendResponse( uint8_t *AResponseArray, uint8_t ASize, bool AError )
		{
			if( AError )
				T_ErrorOutputPin::ClockPin();

			else
			{
				Mitov::TDataBlock ADataBlock( ASize, AResponseArray );
				T_OutputPin::SetPinValue( ADataBlock );
			}
		}

		void SendPacket()
		{
			C_OWNER.SendOneRequest( this, C_INDEX, ID().GetValue(), 8 );
        }

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SendRequest( this, C_INDEX, ID().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ID,
		uint8_t C_INDEX,
        typename T_InitialValue
	> class TArduinoLinBusWriteRawElement :
		public T_ClockInputPin_o_IsConnected,
		public T_ID,
        public T_InitialValue
	{
	public:
		_V_PROP_( ID )
        _V_PROP_( InitialValue )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void SendPacket()
		{
			C_OWNER.SendOneWrite( this, ID().GetValue() );
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//			InitialValue = *(Mitov::TDataBlock*)_Data;
			Mitov::TDataBlock &AValue = *(Mitov::TDataBlock*)_Data;

            T_InitialValue::SetValue( AValue.Data, AValue.Size );

			if( ! ClockInputPin_o_IsConnected().GetValue() )
				C_OWNER.SendWrite( this, C_INDEX, ID().GetValue() );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SendWrite( this, C_INDEX, ID().GetValue() );
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ID,
		uint8_t C_INDEX,
        typename T_InitialValue
	> class TArduinoLinBusWriteRawElement_Variable
		public T_ClockInputPin_o_IsConnected,
		public T_ID,
        public T_InitialValue
	{
	public:
		_V_PROP_( ID )
        _V_PROP_( InitialValue )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void SendPacket()
		{
			C_OWNER.SendOneWrite( this, ID().GetValue() );
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue = *(Mitov::TDataBlock*)_Data;

			if( ! ClockInputPin_o_IsConnected().GetValue() )
				C_OWNER.SendWrite( this, C_INDEX, ID().GetValue() );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SendWrite( this, C_INDEX, ID().GetValue() );
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ID,
		uint8_t C_INDEX,
		typename T_InitialValue
	> class TArduinoLinBusRawDataResponseElement : // public TArduinoLinBusBasicResponseElement
		public T_ID,
		public T_InitialValue
	{
	public:
		_V_PROP_( ID )
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//			InitialValue = *(Mitov::TDataBlock*)_Data;
			Mitov::TDataBlock &AValue = *(Mitov::TDataBlock*)_Data;

            T_InitialValue::SetValue( AValue.Data, AValue.Size );
//			Serial.println( InitialValue.ToString() );
		}

	public:
		inline void HandleResponse( uint8_t AID )
		{
			if( ID().GetValue() == AID )
				C_OWNER.HandleResponse( this );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ID,
		uint8_t C_INDEX,
		typename T_OutputPin
	> class TArduinoLinBusRawDataReceiveElement :
		public T_ID,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( ID )

	public:
		inline void SendData( uint8_t *AData, uint8_t ASize )
		{
			Mitov::TDataBlock ADataBlock( ASize, AData );
			T_OutputPin::SetPinValue( ADataBlock );
		}

//	public:
//		inline void HandleResponse( uint8_t AID )
//		{
//			if( ID() == AID )
//				C_OWNER.HandleResponse( this );

//		}

		inline void HandleReceive( uint8_t AID )
		{
			if( ID().GetValue() == AID )
				SendData( C_OWNER.FResponseArray + 2, C_OWNER.FReadIndex() - 3 );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

