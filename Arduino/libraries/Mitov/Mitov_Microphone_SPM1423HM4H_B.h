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
#include <driver/i2s.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_BufferSize,
		typename T_CountBuffers,
		typename T_OutputPin,
		typename T_SampleRate
	> class MicrophoneSPM1423HM4H_B :
		public T_BufferSize,
		public T_CountBuffers,
		public T_OutputPin,
		public T_SampleRate
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( BufferSize )
		_V_PROP_( CountBuffers )
		_V_PROP_( SampleRate )

	public:
		inline void SystemInit()
		{
			i2s_config_t i2s_config = 
			{
				.mode					= (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
				.sample_rate			= int( SampleRate().GetValue() ),
				.bits_per_sample		= I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
				.channel_format			= I2S_CHANNEL_FMT_ALL_RIGHT,
				.communication_format	= I2S_COMM_FORMAT_I2S,
				.intr_alloc_flags		= ESP_INTR_FLAG_LEVEL1,
				.dma_buf_count			= int( CountBuffers().GetValue() ),
				.dma_buf_len			= int( BufferSize().GetValue() ) // 128
			};

			i2s_pin_config_t pin_config =
			{
				.bck_io_num   = I2S_PIN_NO_CHANGE,
				.ws_io_num    = 0, // PIN_CLK;
				.data_out_num = I2S_PIN_NO_CHANGE,
				.data_in_num  = 34 // PIN_DATA;
			};
	   
			i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
			i2s_set_pin(I2S_NUM_0, &pin_config);
			i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
		}

		inline void SystemLoopBegin()
		{
			uint16_t *ABuffer = new uint16_t[ BufferSize().GetValue() * CountBuffers().GetValue() ];
			size_t ABytesRead;
			i2s_read( I2S_NUM_0, (char*) ABuffer, BufferSize().GetValue() * CountBuffers().GetValue() * 2, &ABytesRead, (100 / portTICK_RATE_MS));
			if( ABytesRead )
			{
//				Serial.println( ABytesRead );
				uint32_t ASamplesCount = ABytesRead / 2;
				float *AOutBuffer = new float[ ASamplesCount ];

				for( int i = 0; i < ASamplesCount; ++i )
					AOutBuffer[ i ] = float( ABuffer[ i ] * 3 ) / 0x7FFF; 

//			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
				delete [] ABuffer;

				TArray<float> APinBuffer( ASamplesCount, AOutBuffer );
				T_OutputPin::SetPinValue( APinBuffer );
				delete [] AOutBuffer;
			}

			else
				delete [] ABuffer;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


