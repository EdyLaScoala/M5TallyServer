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
#include <PDM.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_BufferSize,
		typename T_Gain,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_SampleRate
	> class MicrophoneNano33Sense :
		public T_BufferSize,
		public T_Gain,
		public T_Enabled,
		public T_OutputPin,
		public T_SampleRate
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( BufferSize )
		_V_PROP_( Enabled )
		_V_PROP_( Gain )
		_V_PROP_( SampleRate )

	public:
		static int16_t	*FBuffer;
		static uint32_t	 FSize;

	protected:
		static void DoPDMdata() 
		{
//			Serial.println( "DoPDMdata" );

			if( ! FBuffer )
				return;

			int32_t ABytesRead = PDM.available();
			if( ABytesRead <= 0 )
				return;

			PDM.read( FBuffer, ABytesRead );
			FSize = ABytesRead;

//  Serial.println( "onPDMdata" );
  // query the number of bytes available
//  int bytesAvailable = PDM.available();

  // read into the sample buffer
//  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
//  samplesRead = bytesAvailable / 2;
		}

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				PDM.begin(1, SampleRate());

			else
				PDM.end();

		}

		void UpdateSampleRate()
		{
			if( Enabled() )
			{
				PDM.end();
				PDM.begin(1, SampleRate());
			}
		}

		void UpdateBufferSize()
		{
			if( Enabled() )
				PDM.end();

			delete [] FBuffer;
			FBuffer = new int16_t[ BufferSize() ];
			PDM.setBufferSize( BufferSize() );

			if( Enabled() )
				PDM.begin(1, SampleRate());
		}

		void UpdateGain()
		{
			if( Enabled() )
				PDM.end();

			PDM.setGain( Gain() * 255 + 0.5 );

			if( Enabled() )
				PDM.begin(1, SampleRate());
		}

	public:
		inline void SystemStart()
		{
			PDM.onReceive( DoPDMdata );
			FBuffer = new int16_t[ BufferSize() ];
//			Serial.print( "PDM.setBufferSize: " );
//			Serial.println( BufferSize() * 2 );
			PDM.setBufferSize( BufferSize() * 2 );

//			Serial.print( "PDM.setGain: " );
//			Serial.println( Gain() );
			PDM.setGain( Gain() * 255 + 0.5 );
			UpdateEnabled();
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() )
				return;

//			int32_t ABytesRead = PDM.available();
			if( FSize <= 0 )
				return;

//			Serial.println( "+++" );
//			return;

			FSize &= ~uint32_t( 1 );

//			uint8_t *ABuffer = new uint8_t[ ABytesRead ];

//			PDM.read( ABuffer, ABytesRead );

//			uint32_t ASamplesCount = ABytesRead / 2;
			uint32_t ASamplesCount = FSize / 2;
			FSize = 0;
//			Serial.println( ASamplesCount );
//			return;

			float *AOutBuffer = new float[ ASamplesCount ];

			for( int i = 0; i < ASamplesCount; ++i )
				AOutBuffer[ i ] = float( FBuffer[ i ] * 3 ) / 0x7FFF; 

//			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
//			delete [] ABuffer;

			TArray<float> APinBuffer( ASamplesCount, AOutBuffer );
			T_OutputPin::SetPinValue( APinBuffer );
			delete [] AOutBuffer;
		}

	};
//---------------------------------------------------------------------------
template <
		typename T_BufferSize,
		typename T_Gain,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_SampleRate
	> int16_t * MicrophoneNano33Sense<
		T_BufferSize,
		T_Gain,
		T_Enabled,
		T_OutputPin,
		T_SampleRate>::FBuffer = nullptr;
//---------------------------------------------------------------------------
template <
		typename T_BufferSize,
		typename T_Gain,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_SampleRate
	> uint32_t MicrophoneNano33Sense<
		T_BufferSize,
		T_Gain,
		T_Enabled,
		T_OutputPin,
		T_SampleRate>::FSize = 0;
//---------------------------------------------------------------------------
}
