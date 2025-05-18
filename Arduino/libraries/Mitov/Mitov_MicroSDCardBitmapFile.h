////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_MicroSDCard.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_PathName
	> class MicroSDCardElementBitmapFile : 
		public T_PathName
	{
	public:
		_V_PROP_( PathName )

	protected:
		bool OpenFile( File &AFile, uint32_t &bmpWidth, uint32_t &bmpHeight, uint32_t &bmpImageoffset, bool &flip ) // Share to save template memory!
		{
			if( PathName() == "" )
				return false;

			AFile = C_OWNER.FCard.open( PathName().c_str() );
			if( ! AFile )
			{
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
				C_OWNER.ErrorInfoOutputPin().SendValue( "Missing file" );
#endif
				return false;
			}

			// Parse BMP header
			if(read16(AFile) != 0x4D42) 
			{ // BMP signature
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
				C_OWNER.ErrorInfoOutputPin().SendValue( "Invalid Bitmap Signature" );
#endif
				return false;
			}

			Skip( AFile, 8 ); // Read & ignore Size and creator bytes

			bmpImageoffset = read32( AFile ); // Start of image data

			read32( AFile ); // Skip header size

			bmpWidth  = read32( AFile );
			bmpHeight = read32( AFile );

			if(read16( AFile ) != 1) 
			{ // # planes -- must be '1'
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
				C_OWNER.ErrorInfoOutputPin().SendValue( "More than 1 Bitmap planes" );
#endif
				return false;
			}

			if( read16( AFile ) != 24 )
			{ // Depth -- must be '24'
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
				C_OWNER.ErrorInfoOutputPin().SendValue( "Bitmap depth is not 24 bits" );
#endif
				return false;
			}

			if( read32( AFile ) )
			{
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
				C_OWNER.ErrorInfoOutputPin().SendValue( "Compressed Bitmap not supported" );
#endif
				return false;
			}

			// If bmpHeight is negative, image is in top-down order.
			// This is not canon but has been observed in the wild.
			if(bmpHeight < 0) 
			{
				bmpHeight = -bmpHeight;
				flip = false;
			}
			else
				flip = true;

			return true;
		}

	public:
		template <typename T_ELEMENT> void RenderGraphics( T_ELEMENT *AElement, uint32_t X, uint32_t Y )
		{
			uint32_t bmpWidth;
			uint32_t bmpHeight;
			uint32_t bmpImageoffset;
			File AFile;
			bool flip;

//			Serial.println( "RenderGraphics" );

			if( ! OpenFile( AFile, bmpWidth, bmpHeight, bmpImageoffset, flip ))
				return;

//			Serial.println( "RenderGraphics 1" );

			// BMP rows are padded (if needed) to 4-byte boundary
			uint32_t rowSize = (bmpWidth * 3 + 3) & ~3;

			uint32_t w = bmpWidth;
			uint32_t h = bmpHeight;

			uint32_t AGraphicsWidth = AElement->width();
			uint32_t AGraphicsHeight = AElement->height();

			if(( X + w-1) >= AGraphicsWidth )
				w = AGraphicsWidth  - X;

			if((Y + h-1) >= AGraphicsHeight ) 
				h = AGraphicsHeight - Y;

			uint8_t  sdbuffer[3*20]; // pixel buffer (R+G+B per pixel)
			uint32_t pos = 0;
			uint8_t  buffidx = sizeof(sdbuffer);

			for ( int32_t row=0; row<h; row++) 
			{ // For each scanline...
				// Seek to start of scan line.  It might seem labor-
				// intensive to be doing this on every line, but this
				// method covers a lot of gritty details like cropping
				// and scanline padding.  Also, the seek only takes
				// place if the file position actually needs to change
				// (avoids a lot of cluster math in SD library).
				if( flip ) // Bitmap is stored bottom-to-top order (normal BMP)
					pos = bmpImageoffset + ( bmpHeight - 1 - row) * rowSize;

				else     // Bitmap is stored top-to-bottom
					pos = bmpImageoffset + row * rowSize;

				if( AFile.position() != pos) 
				{ // Need seek?
					AFile.seek(pos);
					buffidx = sizeof(sdbuffer); // Force buffer reload
				}

				for (int32_t col=0; col<w; col++) 
				{ // For each pixel...
					// Time to read more pixel data?
					if (buffidx >= sizeof(sdbuffer))
					{ // Indeed
						AFile.read(sdbuffer, sizeof(sdbuffer));
						buffidx = 0; // Set index to beginning
					}

					// Convert pixel from BMP to TFT format, push to display
					uint8_t b = sdbuffer[buffidx++];
					uint8_t g = sdbuffer[buffidx++];
					uint8_t r = sdbuffer[buffidx++];
//					Serial.println( r );
					AElement->drawPixel( X + col, Y + row, AElement->MakeColorColor( r, g, b ));
					//pushColor(tft.Color565(r,g,b));
//					AElement->drawPixel( X + col, Y + row, 0xFF000000 | ( uint32_t( r ) << 16 ) | ( uint32_t( g ) << 8 ) | b );
				} // end pixel
			} // end scanline

		}

	protected:
		void	Skip( File &AFile, uint32_t ASize )
		{
			while( ASize-- )
				AFile.read();
		}

		uint16_t read16(File &AFile ) 
		{
			uint16_t result;
			((uint8_t *)&result)[0] = AFile.read(); // LSB
			((uint8_t *)&result)[1] = AFile.read(); // MSB
//			Serial.println( result, HEX );
			return result;
		}

		uint32_t read32(File &AFile ) 
		{
			uint32_t result;
			((uint8_t *)&result)[0] = AFile.read(); // LSB
			((uint8_t *)&result)[1] = AFile.read();
			((uint8_t *)&result)[2] = AFile.read();
			((uint8_t *)&result)[3] = AFile.read(); // MSB
			return result;
		}

/*
	public:
		inline void SystemStart()
		{
			ReadInputPin_o_Receive( nullptr );
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif