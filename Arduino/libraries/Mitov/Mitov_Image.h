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
        typename T_DataOutputPin,
		typename T_Enabled,
		typename T_FormatOutputPin,
		typename T_HeightOutputPin,
        typename T_WidthOutputPin
	> class ExtractImageInfo :
        public T_DataOutputPin,
		public T_Enabled,
		public T_FormatOutputPin,
		public T_HeightOutputPin,
        public T_WidthOutputPin
	{
	public:
		_V_PIN_( DataOutputPin )
		_V_PIN_( FormatOutputPin )
		_V_PIN_( HeightOutputPin )
		_V_PIN_( WidthOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			TImageBuffer &AValue = *(TImageBuffer*)_Data;

//            Serial.println( AValue.GetSize() );

            T_HeightOutputPin::SetPinValue( AValue.Height );
            T_WidthOutputPin::SetPinValue( AValue.Width );
            T_FormatOutputPin::SetPinValue( AValue.PixelFormat );

            if( T_DataOutputPin::GetPinIsConnected() )
                T_DataOutputPin::SetPinValue( Mitov::TDataBlock( AValue.GetSize(), AValue.Data ) );

        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_IncludeFormat,
        typename T_OutputPin
    > class ImageToBinary :
		public T_Enabled,
        public T_IncludeFormat,
        public T_OutputPin
    {
	public:
        _V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
        _V_PROP_( IncludeFormat )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			TImageBuffer &ABuffer = *(TImageBuffer*)_Data;

            uint8_t *ABufferData = nullptr;
            uint8_t *AData;
            uint32_t ASize = TImageFormat::GetSize( ABuffer.PixelFormat, ABuffer.Width, ABuffer.Height );
            if( ! IncludeFormat().GetValue() )
                AData = ABuffer.Data;

            else
            {
                uint8_t AOffset = 1;

                AOffset += Func::GetVariableSize( ABuffer.Width );
                AOffset += Func::GetVariableSize( ABuffer.Height );

                ABufferData = new uint8_t[ ASize + AOffset ];

                ABufferData[ 0 ] = ABuffer.PixelFormat;
                uint32_t ADataIndex = 1;

        		Func::GetAddVariableSizeValue( ABufferData, ADataIndex, ABuffer.Width );
        		Func::GetAddVariableSizeValue( ABufferData, ADataIndex, ABuffer.Height );

                memcpy( ABufferData + AOffset, ABuffer.Data, ASize );
                AData = ABufferData;
                ASize += AOffset;
            }

            T_OutputPin::SetPinValue( Mitov::TDataBlock( ASize, AData ) );

            if( ABufferData )
                delete [] ABufferData;

        }

    };
//---------------------------------------------------------------------------
	template <
        typename T_ExtractFromData,
        typename T_Height,
        typename T_PixelFormat,
        typename T_Width
    > class TArduinoBinaryToImageFormat :
        public T_ExtractFromData,
        public T_Height,
        public T_PixelFormat,
        public T_Width
    {
	public:
		_V_PROP_( ExtractFromData )
		_V_PROP_( Height )
		_V_PROP_( PixelFormat )
		_V_PROP_( Width )

    };
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_Format,
        typename T_OutputPin
    > class BinaryToImage :
		public T_Enabled,
        public T_Format,
        public T_OutputPin
    {
	public:
        _V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
        _V_PROP_( Format )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			TArray<uint8_t> &ABuffer = *(TArray<uint8_t>*)_Data;
/*
            for( int i = 0; i < ABuffer.Size; ++ i )
            {
                Serial.print( ABuffer.Data[ i ] );
                Serial.print( "," );
            }
*/
            TImageFormat::TImageFormat AFormat;
            uint32_t AWidth;
            uint32_t AHeight;
            uint8_t *ABufferData = nullptr;
            uint8_t *AData;

            if( Format().ExtractFromData() )
            {
//                AData = ABuffer.Data;

                if( ABuffer.Size < 3 )
                    return; // Not enogh data

                AFormat = TImageFormat::TImageFormat( ABuffer.Data[ 0 ] );
                if( AFormat > TImageFormat::MaxFormat )
                    return; // Incorrect image format

                uint32_t ADataIndex = 1;
                AWidth = Func::GetVariableSizeValue( ABuffer.Data, ABuffer.Size, ADataIndex );
                AHeight = Func::GetVariableSizeValue( ABuffer.Data, ABuffer.Size, ADataIndex );
                if( AWidth == 0 || AHeight == 0 )
                    return; // Incorrect image size

                uint32_t ASize = TImageFormat::GetSize( AFormat, AWidth, AHeight );
                if( ASize + ADataIndex <= ABuffer.Size )
                    AData = ABuffer.Data + ADataIndex;

                else
                {
                    ABufferData = new uint8_t[ ASize ];
                    memcpy( ABufferData, ABuffer.Data + ADataIndex, ABuffer.Size - ADataIndex );
                    memset( ABufferData + ABuffer.Size - ADataIndex, 0, ASize - ( ABuffer.Size - ADataIndex ));
                    AData = ABufferData;
                }

            }

            else
            {
//                Serial.println( "STEP1" );
                AFormat = Format().PixelFormat();
                AWidth = Format().Width();
                AHeight = Format().Height();
                uint32_t ASize = TImageFormat::GetSize( AFormat, AWidth, AHeight );
//                Serial.println( ASize );
//                Serial.println( ABuffer.Size );
                if( ASize <= ABuffer.Size )
                    AData = ABuffer.Data;

                else
                {
                    ABufferData = new uint8_t[ ASize ];
                    AData = ABufferData;
                    memcpy( ABufferData, ABuffer.Data, ABuffer.Size );
                    memset( ABufferData + ABuffer.Size, 0, ASize - ABuffer.Size );
                }
            }

            T_OutputPin::SetPinValue( TImageBuffer( AFormat, AWidth, AHeight, AData ) );

            if( ABufferData )
                delete [] ABufferData;

        }

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

