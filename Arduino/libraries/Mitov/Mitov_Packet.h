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
#include <Mitov_Structure.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	class BasicPacket
	{
	protected:
		uint32_t	FBufferSize;
		uint32_t	FCurrentSize;

		uint8_t *FBuffers[ 2 ];

	public:
		void CheckSize(	uint32_t ASize )
		{
			if( ASize <= FCurrentSize )
				return;

			uint8_t *AOldBuffer = FBuffers[ 0 ];
			FBuffers[ 0 ] = new uint8_t[ ASize ];
			memcpy( FBuffers[ 0 ], AOldBuffer, FCurrentSize );
			delete [] AOldBuffer;

			AOldBuffer = FBuffers[ 1 ];
			FBuffers[ 1 ] = new uint8_t[ ASize ];
			memcpy( FBuffers[ 1 ], AOldBuffer, FCurrentSize );
			delete [] AOldBuffer;

			FCurrentSize = ASize;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T_PIN,
		typename T
	> class BasicTypedPacketSinkElement :
		public T_FPopulated
	{
		_V_PROP_( FPopulated )

	protected:
		T	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T AValue = *(T_PIN*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;
//			Serial.println( sizeof( double ) );
			AResult = sizeof( T ) * 8; 
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{ 
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			C_OWNER.CheckSize( ADataIndex + sizeof( T ) );

#ifdef VISUINO_MEMORY_ALLIGN
			memcpy( ADataStart + ADataIndex, &FValue, sizeof( T ));
#else
			*(T*)( ADataStart + ADataIndex ) = FValue;
#endif
			ADataIndex += sizeof( T );
		}

		inline void Expand( uint8_t * const & AInBuffer, uint8_t * const & AOutBuffer, int & ASize, bool & AResult ) 
		{ 
			AResult = false;
		}

	public:
		BasicTypedPacketSinkElement( bool APopulated )
		{
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_PIN,
		typename T
	> class BasicTypedUnpacketSourceElement : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = true; 
			AResult = sizeof( T ) * 8; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			if( AOffset )
			{
				if( ! AAccumulator->SkipBytes( 1 ))
				{
					AResult = false;
					return;
				}

				AOffset = 0;
			}

			T_PIN AValue;
			if( ! AAccumulator->ExtractBytes( sizeof( T ), &AValue ))
			{
				AResult = false;
				return;
			}

			T_OutputPin::SetPinValue( AValue );
//			OutputPin.Notify( &AValue );
			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;
			APosition += sizeof( T ) * 8;
			AResult = false;
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OutputPin> class UnpacketDigitalBinaryElement : public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = false;
			AResult = 1; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			uint8_t AValue;
			if( ! AAccumulator->PeekBytes( 1, &AValue ))
			{
				AResult = false;
				return;
			}

			bool ABoolValue = (( AValue & ( 1 << AOffset )) != 0 );
			++AOffset;

			if( AOffset >= 8 )
			{
				AOffset = 0;
				if( ! AAccumulator->SkipBytes( 1 ))
				{
					AResult = false;
					return;
				}
			}

			T_OutputPin::SetPinValue( ABoolValue );

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{
			++ APosition;
			AResult = false;
		}
	};
//---------------------------------------------------------------------------
	template <
        typename T_IdleOutputPin,
        typename T_OutputPin
    > class UnpacketClockBinaryElement :
        public T_IdleOutputPin,
        public T_OutputPin
	{
	public:
        _V_PIN_( IdleOutputPin )
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = false;
			AResult = 1; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			uint8_t AValue;
			if( ! AAccumulator->PeekBytes( 1, &AValue ))
			{
				AResult = false;
				return;
			}

			bool ABoolValue = (( AValue & ( 1 << AOffset )) != 0 );
			++AOffset;

			if( AOffset >= 8 )
			{
				AOffset = 0;
				if( ! AAccumulator->SkipBytes( 1 ))
				{
					AResult = false;
					return;
				}
			}

			if( ABoolValue )
				T_OutputPin::ClockPin();

			else
				T_IdleOutputPin::ClockPin();

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{
			++ APosition;
			AResult = false;
		}
	};
//---------------------------------------------------------------------------
	template <typename T_OutputPin> class UnpacketTextBinaryElement : public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = false;
			AResult = 1; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			uint32_t ASize = 0;
			uint8_t AShift = 0;
			uint8_t AByteSize;
			do
			{
				if( ! AAccumulator->PeekBytes( 1, &AByteSize ))
				{
					AResult = false;
					return;
				}

				AAccumulator->SkipBytes( 1 );

				ASize += ( AByteSize & 0x7F ) << ( AShift * 7 );

				++ AShift;
			}
			while( AByteSize & 0x80 );

			uint8_t *AData = new uint8_t[ ASize + 1 ];

			if( ! AAccumulator->PeekBytes( ASize, AData ))
			{
				delete [] AData;
				AResult = false;
				return;
			}

			AAccumulator->SkipBytes( ASize );

			AData[ ASize ] = '\0';

			T_OutputPin::SetPinValue( (char *)AData );

			delete [] AData;

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			AResult = false;

			uint32_t	ASize1 = 0;
			uint8_t		AShift = 0;
			uint8_t		AByteSize;
			do
			{
				if( ! AInAccumulator->PeekBytesFrom( ( APosition / 8 ) + AShift, 1, &AByteSize ))
				{
					AIsValid = false;
					return;
				}

				ASize1 += ( AByteSize & 0x7F ) << ( AShift * 7 );
				++ AShift;
			}
			while( AByteSize & 0x80 );

            if( AShift > 1 )
                ANewBitsSize += ( AShift - 1 ) * 8;

//			uint8_t AValue;
//			if( ! AOperation.InAccumulator->PeekBytesFrom( AOperation.Position / 8, 1, &AValue ))
//				return;

            ANewBitsSize += ASize1 * 8;
			if( AInAccumulator->size() < ASize1 + AShift )
			{
				AIsValid = false;
				return;
			}

			APosition += ( ASize1 + AShift ) * 8;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin, 
		typename T
	> class UnpacketArrayBinaryElement : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = false;
			AResult = 1; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			uint32_t ASize = 0;
			uint8_t AShift = 0;
			uint8_t AByteSize;
			do
			{
				if( ! AAccumulator->PeekBytes( 1, &AByteSize ))
				{
					AResult = false;
					return;
				}

				AAccumulator->SkipBytes( 1 );

				ASize += ( AByteSize & 0x7F ) << ( AShift * 7 );

				++ AShift;
			}
			while( AByteSize & 0x80 );

			T *AData = new T[ ASize ];

			if( ! AAccumulator->PeekBytes( ASize * sizeof( T ), AData ))
			{
				delete [] AData;
				AResult = false;
				return;
			}

			AAccumulator->SkipBytes( ASize * sizeof( T ) );

			Mitov::TArray<T> ADataBlock( ASize, AData );
			T_OutputPin::SetPinValue( ADataBlock );

			delete [] AData;

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			AResult = false;

			uint32_t	ASize1 = 0;
			uint8_t		AShift = 0;
			uint8_t		AByteSize;
			do
			{
				if( ! AInAccumulator->PeekBytesFrom( ( APosition / 8 ) + AShift, 1, &AByteSize ))
					return;

				ASize1 += ( AByteSize & 0x7F ) << ( AShift * 7 );
				++ AShift;
			}
			while( AByteSize & 0x80 );

            if( AShift > 1 )
                ANewBitsSize += ( AShift - 1 ) * 8;

//			uint8_t AValue;
//			if( ! AOperation.InAccumulator->PeekBytesFrom( AOperation.Position / 8, 1, &AValue ))
//				return;

			ASize1 *= sizeof( T );
            ANewBitsSize += ASize1 * 8;

			if( AInAccumulator->size() < ASize1 + AShift )
			{
				AIsValid = false;
				return;
			}

			APosition += ( ASize1 + AShift ) * 8;
		}

	};
//---------------------------------------------------------------------------
    template <
        typename T_Bytes
	> class UnpacketMarkerBinaryElement :
        public T_Bytes
	{
	public:
		_V_PROP_( Bytes )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = true; 
			AResult = T_Bytes::GetCount() * 2 * 8;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			int ABytesSize = T_Bytes::GetCount();
			int ASearchPos;
/*
            if( T_Bytes::GetIsConstant() )
            {
                TValueArray<uint8_t> ABuffer( true, T_Bytes::GetCount() );
				T_Bytes::CopyData( ABuffer.Data );

    			if( ! AInAccumulator->FindBytes( ABytesSize, ABuffer.Data, APosition / 8, T_Bytes::GetCount(), ASearchPos ))
	    			AIsCorruptedPacket = true;

                else if( ASearchPos != APosition / 8 + ABytesSize )
                    AIsCorruptedPacket = true;
            }

            else
            {
    			if( ! AInAccumulator->FindBytes( ABytesSize, T_Bytes::GetArray(), APosition / 8, T_Bytes::GetCount(), ASearchPos ))
	    			AIsCorruptedPacket = true;

                else if( ASearchPos != APosition / 8 + ABytesSize )
                    AIsCorruptedPacket = true;
            }
*/
            if( ! AInAccumulator->FindBytes<T_Bytes>( APosition / 8, T_Bytes::GetCount(), ASearchPos ))
                AIsCorruptedPacket = true;

            else if( ASearchPos != APosition / 8 + ABytesSize )
                AIsCorruptedPacket = true;

			APosition += ABytesSize * 8;
			AResult = false; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{ 
			if( AOffset != 0 )
			{
				if( ! AAccumulator->SkipBytes( 1 ))
				{
					AResult = false;
					return;
				}

				AOffset = 0;
			}

			AResult = AAccumulator->SkipBytes( T_Bytes::GetCount() );
//			if( ! AAccumulator->SkipBytes( T_Bytes::GetCount() ))
//				return false;

//			return true; 
		}
	};
//---------------------------------------------------------------------------
	template <
        typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Bytes,
        typename T_Bytes_ApplyValues,
        typename T_Bytes_GetValue
    > class PacketMarkerBinaryElement :
        public T_Bytes
	{
	public:
		_V_PROP_( Bytes )

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= true;
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;
			AResult = T_Bytes::GetCount() * 8;
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( T_Bytes::GetCount() == 0 )
				return;

			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );
			C_OWNER.CheckSize( ADataIndex + T_Bytes::GetCount() );

            T_Bytes::CopyData( ADataStart + ADataIndex );
//			memcpy( ADataStart + ADataIndex, Bytes._Bytes, T_Bytes::GetCount() );
			ADataIndex += T_Bytes::GetCount();
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled
	> class PacketChecksumElement :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= true;
		}

		inline int GetSize( bool &AAlligned )
		{ 
			if( Enabled() )
			{
				AAlligned = true;
				return 8;
			}

			return 0;
		}

		inline void GetData( BasicPacket *AOwner, uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( ! Enabled() )
				return;

			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

//			BasicPacket &AInstance = *(( BasicPacket *) AOperation.Instance );
			AOwner->CheckSize( ADataIndex + 1 );

			uint8_t	AChecksum = 0;
			uint8_t *AData = ADataStart;
			for( int i = 0; i < ADataIndex; ++ i )
				AChecksum ^= *AData ++;

//			for( ; AOperation.DataStart < AOperation.Data; ++ AOperation.DataStart )
//				AChecksum ^= *AOperation.DataStart;

			*AData = AChecksum;
//			AOperation.DataStart[ AOperation.DataIndex ] = AChecksum;
			++ ADataIndex;
		}

		inline bool Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize ) 
		{ 
			return false;
		}

	public: // Unpack
		int Start( bool &AAllign )
        {
			if( ! Enabled() )
				return 0;

            AAllign = true;
            return 8;
        }

		bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket )
        {
			if( ! Enabled() )
				return false;

			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

            uint8_t *ABytes = new uint8_t[ APosition / 8 + 1 ];
//            SetLength( ABytes, AOperation.Position + 1 );

            if( ! AInAccumulator->PeekBytes( APosition / 8 + 1, ABytes ))
            {
                AIsValid = false;
                delete [] ABytes;
				return false;
            }

            uint8_t AChecksum = 0;

//            for AByte in ABytes do
//                AChecksum ^= AByte;
            for( int i = 0; i < APosition / 8 + 1; ++i )
                AChecksum ^= ABytes[ i ];

            delete [] ABytes;

            if( AChecksum )
            {
                AIsCorruptedPacket = true;
				return false;
            }

            APosition += 8;
			return false;
        }

		bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset )
        {
			if( ! Enabled() )
				return true;

            if( AOffset )
            {
                if( ! AAccumulator->SkipBytes( 1 ))
					return false;

                AOffset = 0;
            }

            AAccumulator->SkipBytes( 1 );
			return true;
        }
	};
//---------------------------------------------------------------------------
    template <
        typename T_Bytes,
        typename T_Bytes_ApplyValues,
        typename T_Bytes_GetValue
	> class TArduinoHeadMarkerBinaryUnpacketElement :
        public T_Bytes
	{
	public:
		_V_PROP_( Bytes )

//	protected:
//		Mitov::SimpleList<uint8_t>	FDoubledBytes;

	public:
		int Start( bool &AAllign )
		{ 
//			for( unsigned int i = 0; i < T_Bytes::GetCount(); ++i )
//				FDoubledBytes.push_back( Bytes._Bytes[ i ] );

//			for( unsigned int i = 0; i < T_Bytes::GetCount(); ++i )
//				FDoubledBytes.push_back( Bytes._Bytes[ i ] );

			AAllign = true; 
			return T_Bytes::GetCount() * 2 * 8;
		}

		bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset )
		{
			if( AOffset )
			{
				if( ! AAccumulator->SkipBytes( 1 ))
					return false;

				AOffset = 0;
			}

			//  AOldDebugBuf := AAccumulator.GetAsBuffer();
			AAccumulator->SkipBytes( T_Bytes::GetCount() * 2 );
			return true;
		}

		bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket )
		{
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			int ABytesSize = T_Bytes::GetCount();
			if( ! ABytesSize )
				return false;

//			uint8_t *ABytes = Bytes._Bytes;
			//  if( not AOperation.InAccumulator.FindSkipBytes( ABytesSize * 2, @FDoubledBytes[ 0 ], ASkipped )) then
			//    Exit( False );

			int AFindPosition;

			uint8_t *ADoubledBytes = new uint8_t[ ABytesSize * 2 ];
//			memcpy( ADoubledBytes, ABytes, ABytesSize );
//			memcpy( ADoubledBytes + ABytesSize, ABytes, ABytesSize );
            T_Bytes::CopyData( ADoubledBytes );
            T_Bytes::CopyData( ADoubledBytes + ABytesSize );
//			for( unsigned int i = 0; i < T_Bytes::GetCount(); ++i )
//				ADoubledBytes[ i ] Bytes._Bytes[ i ] );

			bool AFindResult = AInAccumulator->FindBytes( ABytesSize * 2, ADoubledBytes, AFindPosition );
			delete [] ADoubledBytes;

			if( ! AFindResult )
			{
				AIsValid = false;
				return false;
			}

			AFindPosition -= ABytesSize * 2;
			AInAccumulator->SkipBytes( AFindPosition );

            APosition = ( APosition + 7 ) / 8;
            APosition *= 8;

			APosition += ABytesSize * 2 * 8;

			//  Dec( AOperation.Size, AFindPosition );

			//  if( AOperation.Size <= ABytesSize * 2 ) then
			//    Exit( False );

			//  ANewDebugBuf := AOperation.InAccumulator.GetAsBuffer();

			if( ! AInAccumulator->FindBytes<T_Bytes>( ABytesSize * 2, AStartSize - ABytesSize * 2, AFindPosition ))
				return false;

			//  ANewDebugBuf := AOperation.InAccumulator.GetAsBuffer();
//			AMarkerBuffer := TSLBlockBuffer.CreateData( ABytes, ABytesSize );

			//  if( not AOperation.InAccumulator.FindBytes( ABytesSize, ABytes, AOperation.Size - ABytesSize * 2, AFindPosition )) then
			//    Exit( False );

//			AOutAccumulator->append( ABytes, ABytesSize );
//			AOutAccumulator->append( ABytes, ABytesSize );

			for(;;)
			{
				Mitov::SimpleList<uint8_t, uint16_t>	ABuffer;
				ABuffer.AddCount( AFindPosition );
//				ABuffer := TSLBlockBuffer.CreateSize( AFindPosition );
				AInAccumulator->ExtractBytes( AFindPosition, ABuffer );
				AOutAccumulator->append( ABuffer, AFindPosition );

                AStartSize -= AFindPosition;

				uint8_t ACount;
				AInAccumulator->ExtractBytes( 1, &ACount );

				for( int i = 1; i <= ACount; ++i )
					AOutAccumulator->Append<T_Bytes>();
//					AOutAccumulator->append( ABytes, ABytesSize );

                AStartSize -= ABytesSize * ACount;

//				ASize = ABytesSize * ( ACount + 1 );

				if( ! AInAccumulator->FindBytes<T_Bytes>( 0, AStartSize, AFindPosition ))
					break;

			}

            if( AInAccumulator->size() < AStartSize )
			{
				AIsValid = false;
				return false;
			}

            Mitov::SimpleList<uint8_t, uint16_t>	ABuffer;
            ABuffer.AddCount( AStartSize );

    		AInAccumulator->ExtractBytes( AStartSize, ABuffer );
			AOutAccumulator->append( ABuffer, AStartSize );

			return true;
		}

	public:
		inline void SkipHeader( TPacketAccumulator *AAccumulator ) 
		{
			AAccumulator->SkipBytes( T_Bytes::GetCount() * 2 );
		}

	};
//---------------------------------------------------------------------------
	template <
        typename T_Bytes,
        typename T_Bytes_ApplyValues,
        typename T_Bytes_GetValue
    > class TArduinoHeadMarkerBinaryPacketElement :
        public T_Bytes
	{
	public:
		_V_PROP_( Bytes )

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= true;
		}

		inline int GetSize( bool &AAlligned )
		{ 
			AAlligned = true;
			return T_Bytes::GetCount() * 2 * 8;
		}

		template <typename T_OWNER> inline void GetData( T_OWNER *AOwner, uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( T_Bytes::GetCount() == 0 )
				return;

			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );

			AOwner->CheckSize( ADataIndex + T_Bytes::GetCount() * 2 );

            T_Bytes::CopyData( ADataStart + ADataIndex );
//			memcpy( ADataStart + ADataIndex, Bytes._Bytes, T_Bytes::GetCount() );
			ADataIndex += T_Bytes::GetCount();

            T_Bytes::CopyData( ADataStart + ADataIndex );
//			memcpy( ADataStart + ADataIndex, Bytes._Bytes, T_Bytes::GetCount() );
			ADataIndex += T_Bytes::GetCount();
		}

		template <typename T_OWNER> inline bool Expand( T_OWNER *AOwner, uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize )
		{
			if( T_Bytes::GetCount() == 0 )
				return false;

//			memcpy( AOperation.OutBuffer, AOperation.InBuffer, AOperation.Size );
//			return true;

//			uint8_t *ABytes = Bytes._Bytes;
			int ABytesSize = T_Bytes::GetCount();

			int AOutSize = ABytesSize * 2;

//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );
//			Serial.println( "Expand 1" );
//			Serial.println( AOperation.Size );
			AOwner->CheckSize( AOutSize );
			memcpy( AOutBuffer, AInBuffer, AOutSize );

			uint32_t AInIndex = AOutSize;
			uint32_t AOutIndex = AOutSize;

//			AOperation.OutBuffer += AOutSize;
//			AOperation.InBuffer += AOutSize;

			int i = AOutSize;
			while( i < ASize - ABytesSize * 2 + 1 )
			{
//				Serial.println( "Step 1" );
//				Serial.println( AOperation.Size );
//				if( memcmp( AInBuffer + AInIndex, ABytes, ABytesSize ) == 0 )
				if( T_Bytes::Compare( AInBuffer + AInIndex ) == 0 )
//					if( memcmp( AInBuffer + AInIndex + ABytesSize, ABytes, ABytesSize ) == 0 )
					{
//						Serial.println( "Step 2" );
//						Serial.println( AOperation.Size );
						AOwner->CheckSize( ABytesSize + MitovMax( AInIndex, AOutIndex ) );
						memcpy( AOutBuffer + AOutIndex, AInBuffer + AInIndex, ABytesSize );
						AInIndex += ABytesSize; // * 2;
//						AOperation.InBuffer += ABytesSize * 2;

//						Serial.println( "START" );
						AOutIndex += ABytesSize;
//						AOperation.OutBuffer += ABytesSize;
						uint8_t ACount = 0;
						i += ABytesSize;
						for( ; i < ASize - ABytesSize + 1; i += ABytesSize )
						{
							if( T_Bytes::Compare( AInBuffer + AInIndex ) != 0 )
//							if( memcmp( AInBuffer + AInIndex, ABytes, ABytesSize ) != 0 )
								break;

//							Serial.println( "INC" );
							++ ACount;
							if( ACount == 255 )
								break;

//							AOperation.InBuffer += ABytesSize;
							AInIndex += ABytesSize;
//                            i += ABytesSize;
						}

                        if( ACount == T_Bytes::GetByteAt( 0 ) )
//						if( ACount == *ABytes )
							if( ACount > 0 )
							{
//								Serial.println( "DEC" );
								-- ACount;
								AInIndex -= ABytesSize;
//								AOperation.InBuffer -= ABytesSize;
								i -= ABytesSize;
							}

						
//						Serial.println( "Step 3" );
//						Serial.println( AOperation.Size );
						AOwner->CheckSize( AOutIndex + 1 );
						AOutBuffer[ AOutIndex ++ ] = ACount;
						AOutSize += ABytesSize + 1;
						continue;
					}

//				Serial.println( "Step 4" );
//				Serial.println( AOperation.Size );
//				Serial.println( AInIndex );
//				Serial.println( AOutIndex );
//				delay( 1000 );
//				Serial.println( uint32_t( AOperation.OutBuffer ));
				AOwner->CheckSize( MitovMax( AInIndex, AOutIndex ) + 1 );
//				Serial.println( uint32_t( AOperation.OutBuffer ));
//				Serial.println( "Step 4 +" );
//				Serial.println( AOperation.Size );
//				delay( 1000 );
				AOutBuffer[ AOutIndex ++ ] = AInBuffer[ AInIndex ++ ];
//Serial.print( "++AOutSize" );
				++ AOutSize;
                ++ i;
			}

			int ACorrection = ASize - i;
//			Serial.println( "Step 5" );
//			Serial.println( AOperation.Size );
//			Serial.println( AInIndex );
//			Serial.println( AOutIndex );
//			Serial.println( ACorrection );
//			delay( 1000 );
			AOwner->CheckSize( ACorrection + MitovMax( AInIndex, AOutIndex ) );
			memcpy( AOutBuffer + AOutIndex, AInBuffer + AInIndex, ACorrection );

//			Serial.println( "Step 6" );
//			Serial.println( AOperation.Size );
//			Serial.println( AOutSize );
//			Serial.println( ACorrection );
//			delay( 1000 );
//			*AOperation.OutBuffer = 5;

//			Serial.print( "ADebugCount : " );
//			Serial.println( ADebugCount );

//			Serial.print( "AOutSize : " );
//			Serial.println( AOutSize );

//			Serial.print( "TEST : " );
//			Serial.println( ACorrection );
//			Serial.println( i );

			ASize = AOutSize + ACorrection;
			return true;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FBoolValue,
		typename T_FPopulated
	> class PacketDigitalBinaryElement :
		public T_FBoolValue,
		public T_FPopulated
	{
	public:
		_V_PROP_( FBoolValue )

	protected:
		_V_PROP_( FPopulated )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FBoolValue() )
					return;

			FPopulated() = true;
			FBoolValue() = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = false;
			AResult = 1; 
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );
			C_OWNER.CheckSize( ADataIndex );

			uint8_t *AData = ADataStart + ADataIndex;
			*AData &= 0xFF >> ( 8 - AOffset ); // Zero the upper bits
			if( FBoolValue() )
				*AData |= 1 << AOffset;	// Set the bit

			++AOffset;
			if( AOffset >= 8 )
			{
				AOffset = 0;
				++ ADataIndex;
			}
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	public:
		PacketDigitalBinaryElement( bool APopulated )
		{
			FBoolValue() = false;
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_BufferSize,
		typename T_FCount
	> class PacketClockBinaryElement :
		public T_BufferSize,
		protected T_FCount
	{
	public:
		_V_PROP_( BufferSize )

	public:
		_V_PROP_( FCount )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FCount().GetValue() >= BufferSize().GetValue() )
				return;

			FCount() = FCount() + 1;

			C_OWNER.FRefreshed() = true;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
//			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = false;
			AResult = 1; 
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );
			C_OWNER.CheckSize( ADataIndex );

			uint8_t *AData = ADataStart + ADataIndex;
			*AData &= 0xFF >> ( 8 - AOffset ); // Zero the upper bits
			if( FCount() )
			{
				FCount() = FCount() - 1;
				*AData |= 1 << AOffset;	// Set the bit

				if( FCount() )
				{
					C_OWNER.FRefreshed() = true;
					C_OWNER.FModified() = true;
				}
            }

			++AOffset;
			if( AOffset >= 8 )
			{
				AOffset = 0;
				++ ADataIndex;
			}
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated
	> class PacketTextBinaryElement :
		public T_FPopulated
	{
	protected:
		_V_PROP_( FPopulated )

	protected:
		Mitov::String	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AValue = (char*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			if( FValue.length() != AValue.length() )
				C_OWNER.FNeedsNewSize() = true;

			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;

			uint32_t ALength = FValue.length();
			uint32_t ALength1 = ALength;

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				ALength1 >>= 7;
			}
			while( ALength1 );

			AResult = ( ALength + ACount ) * 8;
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{ 
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			uint32_t ALength = FValue.length();
			uint32_t ALength1 = ALength;
//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				C_OWNER.CheckSize( ADataIndex + ALength + ACount );
				ADataStart[ ADataIndex ] = ALength1 & 0x7F;

				ALength1 >>= 7;
				if( ALength1 )
					ADataStart[ ADataIndex ] |= 0x80;

				ADataIndex ++;
			}
			while( ALength1 );

//			AOperation.DataStart[ AOperation.DataIndex ] = ALength;
			memcpy( ADataStart + ADataIndex, FValue.c_str(), ALength );
			ADataIndex += ALength;
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	public:
		PacketTextBinaryElement( bool APopulated )
		{
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T
	> class PacketArrayBinaryElement :
		public T_FPopulated
	{
		_V_PROP_( FPopulated )

	protected:
		TValueArray<T>	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TArray<T> &AValue = *(TArray<T>*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			if( FValue.Size != AValue.Size )
				C_OWNER.FNeedsNewSize() = true;

			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;

			uint32_t ALength = FValue.Size;
			uint32_t ALength1 = ALength;

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				ALength1 >>= 7;
			}
			while( ALength1 );

			AResult = ( ALength * sizeof( T ) + ACount ) * 8;
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{ 
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			uint32_t ALength = FValue.Size;
			uint32_t ALength1 = ALength;
//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				C_OWNER.CheckSize( ADataIndex + ALength + ACount );
				ADataStart[ ADataIndex ] = ALength1 & 0x7F;

				ALength1 >>= 7;
				if( ALength1 )
					ADataStart[ ADataIndex ] |= 0x80;

				ADataIndex ++;
			}
			while( ALength1 );

			ALength *= sizeof( T );
			memcpy( ADataStart + ADataIndex, FValue.Data, ALength );
			ADataIndex += ALength;
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	public:
		PacketArrayBinaryElement( bool APopulated )
		{
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated
	> class PacketImageBinaryElement :
		public T_FPopulated
	{
		_V_PROP_( FPopulated )

	protected:
		TStoredImageBuffer	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TImageBuffer &AValue = *(TImageBuffer*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			if( FValue.GetSize() != AValue.GetSize() )
				C_OWNER.FNeedsNewSize() = true;

			FValue = AValue;
/*
            for( int i = 0; i < AValue.Width * AValue.Height / 2; ++i )
            {
                Serial.println( AValue.Data[ i ], HEX );
            }
*/
			C_OWNER.FModified() = true;
		}

	protected:
//		inline void GetAddSize( int &AResult, uint32_t ALength )
//		{
//			do
//			{
//				++ AResult;
//				ALength >>= 7;
//			}
//			while( ALength );
//		}

		inline void GetAddSizeValue( uint8_t *ADataStart, uint32_t &ADataIndex, uint32_t AValue )
		{ 
			uint8_t ACount = 0;
			do
			{
				++ ACount;
				C_OWNER.CheckSize( ADataIndex + AValue + ACount );
				ADataStart[ ADataIndex ] = AValue & 0x7F;

				AValue >>= 7;
				if( AValue )
					ADataStart[ ADataIndex ] |= 0x80;

				ADataIndex ++;
			}
			while( AValue );
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;

			AResult = 0;
			AResult += Func::GetVariableSize( FValue.Width );
			AResult += Func::GetVariableSize( FValue.Height );

//			Serial.println( AResult );

			AResult = ( AResult + ( FValue.GetSize() + 1 )) * 8;
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{ 
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			GetAddSizeValue( ADataStart, ADataIndex, FValue.Width );
			GetAddSizeValue( ADataStart, ADataIndex, FValue.Height );

			ADataStart[ ADataIndex ++ ] = FValue.PixelFormat;

			uint32_t ALength = FValue.GetSize();
			memcpy( ADataStart + ADataIndex, FValue.Data, ALength );

			ADataIndex += ALength;
/*
            FILE* pFile = fopen("D:\\PackBuffer1_3_C_1.bin", "wb");

            fwrite( ADataStart, 1, ADataIndex, pFile );

            fclose(pFile);
*/
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	public:
		PacketImageBinaryElement( bool APopulated )
		{
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class UnpacketImageBinaryElement :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		bool GetSkipSize( TPacketAccumulator *AAccumulator, bool &AResult, uint32_t &ASize )
		{
			ASize = 0;

			uint8_t AShift = 0;
			uint8_t AByteSize;
			do
			{
				if( ! AAccumulator->PeekBytes( 1, &AByteSize ))
				{
					AResult = false;
					return false;
				}

				AAccumulator->SkipBytes( 1 );

				ASize += ( AByteSize & 0x7F ) << ( AShift * 7 );

				++ AShift;
			}
			while( AByteSize & 0x80 );

			return true;
		}


		bool GetPeekSize( TPacketAccumulator *AAccumulator, int APosition, uint8_t &AShift, uint32_t &AResult )
		{
			AResult = 0;

			uint8_t	AByteSize;
			do
			{
				if( ! AAccumulator->PeekBytesFrom( APosition + AShift, 1, &AByteSize ))
					return false;

				AResult += ( AByteSize & 0x7F ) << ( AShift * 7 );
				++ AShift;
			}
			while( AByteSize & 0x80 );

			return true;
		}

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
//			Serial.println( "Start" );

			AAllign = false;
			AResult = 3; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
//			Serial.println( "Process" );

			uint32_t AWidth;
			if( ! GetSkipSize( AAccumulator, AResult, AWidth ))
				return;

			uint32_t AHeight;
			if( ! GetSkipSize( AAccumulator, AResult, AHeight ))
				return;

			uint8_t APixelFormatByte;
			if( ! AAccumulator->PeekBytes( 1, &APixelFormatByte ))
			{
				AResult = false;
				return;
			}

			AAccumulator->SkipBytes( 1 );

//			Serial.println( AWidth );
//			Serial.println( AHeight );
//			Serial.println( APixelFormatByte );

			TImageFormat::TImageFormat APixelFormat = TImageFormat::TImageFormat( APixelFormatByte );

			uint32_t ASize = TImageFormat::GetSize( APixelFormat, AWidth, AHeight );

			uint8_t *AData = new uint8_t[ ASize ];

			if( ! AAccumulator->PeekBytes( ASize, AData ))
			{
				delete [] AData;
				AResult = false;
				return;
			}

			AAccumulator->SkipBytes( ASize );

			TImageBuffer AOutBuffer( APixelFormat, AWidth, AHeight, AData );
			T_OutputPin::SetPinValue( AOutBuffer );

			delete [] AData;

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AStartSize, int &ANewBitsSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{
//			Serial.println( "ExpandBuffers" );

			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			AResult = false;

			uint8_t AShift1 = 0;
			uint8_t AShift2 = 0;

//			Serial.println( APosition );

			uint32_t AWidth;
			if( ! GetPeekSize( AInAccumulator, ( APosition / 8 ), AShift1, AWidth ))
			{
				AIsValid = false;
				return;
			}

            if( AShift1 > 1 )
                ANewBitsSize += ( AShift1 - 1 ) * 8;

//			Serial.println( AWidth );
//			Serial.println( AShift1 );

			uint32_t AHeight;
			if( ! GetPeekSize( AInAccumulator, ( APosition / 8 ) + AShift1, AShift2, AHeight ))
			{
				AIsValid = false;
				return;
			}

            if( AShift2 > 1 )
                ANewBitsSize += ( AShift2 - 1 ) * 8;
//			Serial.println( AHeight );


//			Serial.println( AShift2 );

			uint8_t APixelFormatByte;
			if( ! AInAccumulator->PeekBytesFrom( ( APosition / 8 ) + AShift1 + AShift2, 1, &APixelFormatByte ))
			{
				AIsValid = false;
				return;
			}

//			Serial.println( APixelFormatByte );

			TImageFormat::TImageFormat APixelFormat = TImageFormat::TImageFormat( APixelFormatByte );

			uint32_t ASize1 = TImageFormat::GetSize( APixelFormat, AWidth, AHeight );

//			Serial.print( "ASize1: " );
//			Serial.println( ASize1 );

//			Serial.println( AInAccumulator->size() );

//			uint8_t AValue;
//			if( ! AOperation.InAccumulator->PeekBytesFrom( AOperation.Position / 8, 1, &AValue ))
//				return;

            ANewBitsSize += ASize1 * 8;

			if( AInAccumulator->size() < ASize1 + AShift1 + AShift2 + 1 )
			{
				AIsValid = false;
				return;
			}

			APosition += ( ASize1 + AShift1 + AShift2 + 1 ) * 8;
//			Serial.println( "PASS" );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint32_t C_COUNT, 
		typename T_Checksum,
		typename T_Elements_CheckPopulated,
		typename T_Elements_Expand,
		typename T_Elements_GetData,
		typename T_Elements_Elements_GetSize,
		typename T_FModified,
		typename T_FNeedsNewSize,
		typename T_FRefreshed,
        typename T_HeadMarker,
		typename T_OnlyModified,
		typename T_OutputPin
	> class Packet : public BasicPacket,
		public T_Checksum,
		public T_FModified,
		public T_FNeedsNewSize,
		public T_FRefreshed,
        public T_HeadMarker,
		public T_OnlyModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( HeadMarker )
		_V_PROP_( OnlyModified )

	public:
		_V_PROP_( FNeedsNewSize )
		_V_PROP_( FModified )
		_V_PROP_( FRefreshed )
//		float	SizeReserve	= 2.0;

//		PacketHeadMarkerBinaryElement<SELF>	HeadMarker;
		_V_PROP_( Checksum )

	protected:
		inline bool IsPopulated()
		{
			bool AResult = true;
			T_Elements_CheckPopulated::Call( AResult );
			return AResult;
		}

		inline void GetData( uint8_t *&ADataStart, uint32_t &ADataIndex, uint8_t &AOffset ) 
		{
			HeadMarker().GetData( this, ADataStart, ADataIndex, AOffset );
			T_Elements_GetData::Call( ADataStart, ADataIndex, AOffset );
			Checksum().GetData( this, ADataStart, ADataIndex, AOffset );

//			PacketTypes::TGetDataOperation AOperation( this, ADataStart, ADataIndex, AOffset );
//			ProcessCallChain( AIndex, &AOperation );
		}

		inline bool Expand( uint32_t AIndex, uint8_t *&AInBuffer, uint8_t *&AOutBuffer, int &ASize ) 
		{  
			if( ! AIndex )
				return HeadMarker().Expand( this, AInBuffer, AOutBuffer, ASize );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().Expand( AInBuffer, AOutBuffer, ASize );

			bool AResult = false;
			T_Elements_Expand::Call( AIndex - 1, AInBuffer, AOutBuffer, ASize, AResult );
			return AResult;
		}

		inline int GetSize( uint32_t AIndex, bool &AAlligned )
		{
			if( ! AIndex )
				return HeadMarker().GetSize( AAlligned );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().GetSize( AAlligned );

			int AResult;
			T_Elements_Elements_GetSize::Call( AIndex - 1, AAlligned, AResult );
			return AResult;
		}

	protected:
		void RecalculateSize()
		{
			FBufferSize = 0;
//			FAllElements.push_back( &HeadMarker );
//			for( int i = 0; i < Elements.size(); ++i )
//				FAllElements.push_back( Elements[ i ] );

//			FAllElements.push_back( &Checksum );

			for( int i = 0; i < C_COUNT + 2; ++i )
			{
				bool AAlligned = false;
				FBufferSize += GetSize( i, AAlligned );
//				Serial.println( FBufferSize );
				if( AAlligned )
				{
					FBufferSize = ( FBufferSize + 7 ) / 8;
					FBufferSize *= 8;
				}
			}

//			Serial.println( FBufferSize );

			FBufferSize = ( FBufferSize + 7 ) / 8;
			FCurrentSize = uint32_t( FBufferSize * 1.3 );
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "++++++" );
//			int AElementCount = FAllElements.size();
			if( OnlyModified() )
			{
				if( ! FModified() )
					return;
			}

			else if( FRefreshed() )
			{
				if( ! IsPopulated())
					return;

			}

			if( FNeedsNewSize() )
				RecalculateSize();

			FBuffers[ 0 ] = new uint8_t[ FCurrentSize ];
			FBuffers[ 1 ] = new uint8_t[ FCurrentSize ];

            memset( FBuffers[ 0 ], 1, FCurrentSize );

//			uint8_t *ADataPtr = FBuffers[ 0 ];
			uint32_t	ADataIndex = 0;
			uint8_t AOffset = 0;

			FModified() = false;
			FRefreshed() = false;

			GetData( FBuffers[ 0 ], ADataIndex, AOffset );

//            std::ofstream wf("D:\\PackBuffer1_3_C", ios::out | ios::binary);
//            wf.write( FBuffers[ 0 ], FBufferSize );

/*
            FILE* pFile = fopen("D:\\PackBuffer1_3_C.bin", "wb");

            fwrite( FBuffers[ 0 ], 1, FBufferSize, pFile );

            fclose(pFile);
*/
			int ASize = FBufferSize;
//			Serial.println( ASize );
			uint8_t ABufferIndex = 0;
//			Serial.println( "--------" );
			for( int i = 0; i < C_COUNT + 2; ++i )
			{
//				Serial.println( ABufferIndex );
//				Serial.println( 1 ^ ( ABufferIndex & 1 ) );
				uint8_t *&AInBuffer = FBuffers[ ABufferIndex ];
				uint8_t *&AOutBuffer = FBuffers[ 1 ^ ABufferIndex ];
				if( Expand( i, AInBuffer, AOutBuffer, ASize ))
					ABufferIndex ^= 1;

			}

//			Serial.println( "SendPinValue" );
//			Serial.println( ASize );
//			delay( 1000 );

			delete [] FBuffers[ 1 ^ ABufferIndex ];

/*
            FILE* pFile = fopen("D:\\PackBuffer2_3_C.bin", "wb");

            fwrite( FBuffers[ 1 ], 1, ASize, pFile );

            fclose(pFile);
*/
			uint8_t *ADataPtr = FBuffers[ ABufferIndex ];
			T_OutputPin::SendPinValue( Mitov::TDataBlock( ASize, ADataPtr ));

			delete [] FBuffers[ ABufferIndex ];

//			Serial.println( ASize );
//			while( ASize-- )
//				OutputPin.Notify( ADataPtr++ );

		}

	public:
		inline void SystemLoopBegin()
		{
			if( FRefreshed() )
				ClockInputPin_o_Receive( nullptr );

		}

		inline void SystemStart()
		{
			RecalculateSize();
		}

		Packet()
		{
			FNeedsNewSize() = false;
			FModified() = false;
			FRefreshed() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		uint32_t C_COUNT,
		typename T_Checksum,
		typename T_Elements_ExpandBuffers,
		typename T_Elements_Process,
		typename T_Elements_Start,
        typename T_HeadMarker,
		typename T_UnpackedOutputPin
	> class Unpacket :
		public T_Checksum,
        public T_HeadMarker,
		public T_UnpackedOutputPin
	{
	public:
		_V_PIN_( UnpackedOutputPin )

	public:
		_V_PROP_( HeadMarker )
		_V_PROP_( Checksum )

	protected:
		TPacketAccumulator	FAccumulator;
		int					FMinBitsSize = 0;
//		Mitov::SimpleList<BasicUnpacketElement*>	FAllElements;

//	public:
//		void ( *Elements_CallChain )( uint32_t AIndex, PacketTypes::TBasicOperation *AOperation ) = nullptr;

	protected:
        inline int GetMinByteSize() { return ( FMinBitsSize + 7 ) / 8; }

		inline bool ExpandBuffers( uint32_t AIndex, TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int AInitialSize, int &ACurrentSize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket )
		{
			if( ! AIndex )
				return HeadMarker().ExpandBuffers( AInAccumulator, AOutAccumulator, AInitialSize, ACurrentSize, APosition, AIsValid, AIsCorruptedPacket );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().ExpandBuffers( AInAccumulator, AOutAccumulator, AInitialSize, ACurrentSize, APosition, AIsValid, AIsCorruptedPacket );

			bool AResult = false;

			T_Elements_ExpandBuffers::Call( AIndex - 1, AInAccumulator, AOutAccumulator, AInitialSize, ACurrentSize, APosition, AIsValid, AIsCorruptedPacket, AResult );

			return AResult;
		}

		inline int Start( uint32_t AIndex, bool &AAllign )
		{
			if( ! AIndex )
				return HeadMarker().Start( AAllign );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().Start( AAllign );

			int AResult;
			T_Elements_Start::Call( AIndex - 1, AAllign, AResult );

//			Serial.println( AOperation.Result );

			return AResult;
		}

		bool Process( uint32_t AIndex, TPacketAccumulator *AAccumulator, uint8_t &AOffset )
		{ 
			if( ! AIndex )
				return HeadMarker().Process( AAccumulator, AOffset );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().Process( AAccumulator, AOffset );

			bool AResult= false;
			T_Elements_Process::Call( AIndex - 1, AAccumulator, AOffset, AResult );
			return AResult;
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FMinBitsSize == 0 )
				return;

//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );
//			Serial.println( "T0:" );
//			Serial.println( FAccumulator.size() );

			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;

//			Serial.println( ABlock.Size );

			while( ABlock.Size -- )
			{
//				Serial.print( "+" );
				FAccumulator.push_back( *ABlock.Data++ );
//				Serial.print( "-" );
			}

//			Serial.println( "" );
//			Serial.println( FAccumulator.size() );

//			Serial.println( "T1" );

			TPacketAccumulator	AOldAccumulator;
			TPacketAccumulator	*AInAccumulator;
			TPacketAccumulator	*AAccumulator;

            uint32_t AMinSize = GetMinByteSize();
			while( FAccumulator.size() >= AMinSize )
			{
//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );
//				Serial.println( "T2" );
//AOldAccumulator.clear();
//AOldAccumulator.append( FAccumulator, FAccumulator.size() );
//return;
				AOldAccumulator.assign( FAccumulator );
//return;
//				AInAccumulator = &FAccumulator;

				int AInitialSize = AMinSize;
                bool ANeedsExpansion = true;
				bool AIsCorruptedPacket = false;
				AAccumulator = &AOldAccumulator;
//                bool AFirstTime = true;
                while( ANeedsExpansion )
                {
                    if( FAccumulator.size() < AInitialSize )
                        return;

                    AOldAccumulator.assign( FAccumulator );
                    ANeedsExpansion = false;

  //                  if( AFirstTime )
//                        AFirstTime = false;

//                    else
//                    {
//                        FAccumulator.assign( AOldAccumulator );
//        				AAccumulator = &FAccumulator;
//                    }

                    TPacketAccumulator	ARealOutAccumulator;
                    TPacketAccumulator	*AOutAccumulator = &ARealOutAccumulator;
    				AAccumulator = &AOldAccumulator;
    				bool AIsValid = true;
                    AIsCorruptedPacket = false;

                    int APosition = 0;
                    int ACurrentSize = FMinBitsSize;
//    				AOutAccumulator->clear();
    				AInAccumulator = &AOldAccumulator;
    //return;
                    for( int i = 0; i < C_COUNT + 2; ++i )
                    {
    //					Serial.println( "enter" );
    //					Serial.println( FAccumulator.size() );

                        bool AExpanded = ExpandBuffers( i, AInAccumulator, AOutAccumulator, AInitialSize, ACurrentSize, APosition, AIsValid, AIsCorruptedPacket );
                        if( ACurrentSize > AInitialSize * 8 )
                        {
                            AInitialSize = ( ACurrentSize + 7 ) / 8;
                            ANeedsExpansion = true;
                            break;
                        }

                        if( AExpanded )
                        {
    //						Serial.println( "ExpandBuffers" );
    //return;
                            if( AIsCorruptedPacket )
                            {
                                AOldAccumulator.assign( FAccumulator );
                                HeadMarker().SkipHeader( &AOldAccumulator );
                                break;
                            }

                            AAccumulator = AOutAccumulator;
                            if( AAccumulator == &AOldAccumulator )
                            {
    //							Serial.println( "AAccumulator == &FAccumulator" );
                                AInAccumulator = AOutAccumulator;
                                AOutAccumulator = &AOldAccumulator; // Check if it is correct!
                                AOutAccumulator->clear();
                            }

                            else
                            {
                                swap( AInAccumulator, AOutAccumulator );
                                AIsValid = true;
                            }

                        }

    //					Serial.println( "Step1" );
                        if( AIsCorruptedPacket )
                        {
    //						Serial.println( "AIsCorruptedPacket" );
                            AOldAccumulator.assign( FAccumulator );
                            HeadMarker().SkipHeader( &AOldAccumulator );
                            break;
                        }

                        if( ! AIsValid )
                        {
                            return;
//                            FAccumulator.assign( AOldAccumulator );
    //						Serial.println( "Non Valid" );
//                            return;
                        }

                    }
                }

//				Serial.println( "ExpandBuffers OUT" );
				if( AIsCorruptedPacket )
				{
//					Serial.println( "AIsCorruptedPacket SKIPPING" );
//return;
//					if( ! HeadMarker().T_Bytes::GetCount() )
						FAccumulator.SkipBytes( 1 );

					continue;
				}

				if( AAccumulator->size() < GetMinByteSize() )
				{
					FAccumulator.assign( AOldAccumulator );
					return;
				}

/* TEMPORARY REMOVED!!!
				if( ASize < FMinSize )
		//      for AItem in FAllElements do
		//        if( not AItem.CanProcess( AAccumulator )) then
				{
					FAccumulator.assign( AOldAccumulator );
					return;
				}
*/
                FAccumulator.assign( AOldAccumulator );

                AAccumulator = &FAccumulator;
//				Serial.println( "PROCESS" );
				uint8_t AOffset = 0;
				for( int i = 0; i < C_COUNT + 2; ++i )
					if( ! Process( i, AAccumulator, AOffset ))
						return;


                if( AOffset )
                  FAccumulator.SkipBytes( 1 );

				T_UnpackedOutputPin::ClockPin();
//				Serial.println( "exit" );
//				Serial.println( FAccumulator.size() );
//				return;

			}

//			Serial.println( "DoReceive exit" );
		}

	public:
		inline void SystemStart()
		{
//			Serial.println( "FMinSize" );
//			FAllElements.push_back( &HeadMarker );

//			for( int i = 0; i < Elements.size(); ++i )
//				FAllElements.push_back( Elements[ i ] );

//			FAllElements.push_back( &Checksum );

			FMinBitsSize = 0;
			for( int i = 0; i < C_COUNT + 2; ++i )
			{
				  bool AAlligned = false;
				  FMinBitsSize += Start( i, AAlligned );
				  if( AAlligned )
				  {
					  FMinBitsSize = ( FMinBitsSize + 7 ) / 8;
//					  FMinSize = FMinSize * 8;
					  FMinBitsSize *= 8;
				  }
			}

//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );

//			FMinSize = ( FMinSize + 7 ) / 8;

//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif