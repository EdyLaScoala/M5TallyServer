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
#include <I2S.h>
#include <FreeRTOS.h>
//#include <semphr.h>
#include <queue.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_BufferSize,
		typename T_CountBuffers,
		typename T_Enabled,
		typename T_OutputPin
	> class MicrophoneMaixduino :
		public T_BufferSize,
		public T_CountBuffers,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( BufferSize )
		_V_PROP_( CountBuffers )
		_V_PROP_( Enabled )

	protected:
		uint16_t	FCurrentBufferSize;
//		uint8_t		FCurrentCountBuffers	: 4;
//		uint8_t		FCurrentBufferIndex		: 4;

//		SemaphoreHandle_t FSemaphore = nullptr;

		QueueHandle_t	FQueue = nullptr;

	protected:
		int32_t *FBuffer;

	protected:
		inline void InterruptEvent()
		{
			TValueArray<float> *AArray = new TValueArray<float>( uint32_t( FCurrentBufferSize ));

			for( int i = 0; i < FCurrentBufferSize; ++i )
				AArray->Data[ i ] = float( FBuffer[ i * 2 ] );
				
			QueueHandle_t AQueue = atomic_read( &FQueue );
			if( xQueueSend( AQueue, (void *)&AArray, 0 ) != pdPASS )
				delete AArray;

			if( ! Enabled().GetValue() )
				return;

			FCurrentBufferSize = MitovMin<uint32_t>( 4096, BufferSize().GetValue());

			i2s_data_t data = (i2s_data_t)
			{
				.rx_channel = DMAC_CHANNEL0,
				.rx_buf = (uint32_t *)( FBuffer ),
				.rx_len = size_t( FCurrentBufferSize * 2 ),
				.transfer_mode = I2S_RECEIVE,
			};

			plic_interrupt_t irq = (plic_interrupt_t)
			{
				.callback = Interrupt,
				.ctx = this,
				.priority = 1
			};

			i2s_handle_data_dma( I2S_DEVICE_0, data, &irq );
		}

	protected:
		static int Interrupt(void *ctx)
		{
			((MicrophoneMaixduino *)ctx)->InterruptEvent();

			return 0;
		}

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
			{
				i2s_data_t data = (i2s_data_t)
				{
					.rx_channel = DMAC_CHANNEL0,
					.rx_buf = (uint32_t *)FBuffer,
					.rx_len = size_t( FCurrentBufferSize * 2 ),
					.transfer_mode = I2S_RECEIVE,
				};

				plic_interrupt_t irq = (plic_interrupt_t)
				{
					.callback = Interrupt,
					.ctx = this,
					.priority = 1
				};

				i2s_handle_data_dma( I2S_DEVICE_0, data, &irq );
			}
		}

		inline void Update_CountBuffers()
		{
			QueueHandle_t AQueue = xQueueCreate( CountBuffers(), sizeof( void * ));
			atomic_set( &FQueue, AQueue );
		}

	public:
		inline void SystemInit()
		{
			fpioa_set_function(20, FUNC_I2S0_IN_D0); 
			fpioa_set_function(19, FUNC_I2S0_WS); 
			fpioa_set_function(18, FUNC_I2S0_SCLK);

			delay( 1000 );
			FCurrentBufferSize = BufferSize();

			FQueue = xQueueCreate( CountBuffers(), sizeof( void * ));

			FBuffer = new int32_t[ FCurrentBufferSize * 2 ];

			memset( FBuffer, 0, sizeof( uint16_t ) * FCurrentBufferSize * 2 );

			i2s_init(I2S_DEVICE_0, I2S_RECEIVER, 0x3);
			i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_0, 
				RESOLUTION_16_BIT, SCLK_CYCLES_32, 
				TRIGGER_LEVEL_4, STANDARD_MODE ); //LEFT_JUSTIFYING_MODE );

			const uint32_t SAMPLE_RATE = 8000;
			uint32_t sample_rate_act = i2s_set_sample_rate(I2S_DEVICE_0, SAMPLE_RATE);
		}

		inline void SystemStart()
		{
			UpdateEnabled();
		}

		inline void SystemLoopBegin()
		{
			TValueArray<float> *AArray;
			if( xQueueReceive( FQueue, &AArray, 0 ) == pdPASS )
			{
//				delay( 1000 );
//				Serial.println( "READ" );

//				TArray<float> AArray1( AArray->Size, AArray->Data );
				if( Enabled().GetValue() )
					T_OutputPin::SetPinValue( *AArray );

//				Serial.println( AArray->Size );

				delete AArray;
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

