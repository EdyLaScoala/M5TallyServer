////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Display_SSD1306.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_DataCommandOutputPin
	> class Display_SPI :
		public T_ChipSelectOutputPin,
		public T_DataCommandOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( DataCommandOutputPin )

	public:
		void SendCommand6( uint8_t ACommand1, uint8_t ACommand2, uint8_t ACommand3, uint8_t ACommand4, uint8_t ACommand5, uint8_t ACommand6 )
		{
			T_DataCommandOutputPin::SetPinValueLow();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction(SPISettings( 8000000, MSBFIRST, SPI_MODE0));

			C_SPI.transfer( ACommand1 );
			C_SPI.transfer( ACommand2 );
			C_SPI.transfer( ACommand3 );
			C_SPI.transfer( ACommand4 );
			C_SPI.transfer( ACommand5 );
			C_SPI.transfer( ACommand6 );

			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void SendCommand( uint8_t ACommand )
		{
//			T_ChipSelectOutputPin::SetPinValue( true );
			T_DataCommandOutputPin::SetPinValueLow();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction(SPISettings( 8000000, MSBFIRST, SPI_MODE0));
			C_SPI.transfer( ACommand );
			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void SendData16( uint16_t AData )
		{
			StartSendData();

//			C_SPI.transfer( AData >> 8 );
			C_SPI.transfer16( AData );

			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		static inline void w_data_end()
		{
			C_SPI.endTransaction();
		}

		static inline void spiwrite( uint8_t AHi, uint8_t ALo)
		{
			C_SPI.transfer( AHi );
			C_SPI.transfer( ALo );
		}

		static inline void transfer( uint8_t ACommand )
		{
			C_SPI.transfer( ACommand );
		}

		inline void StartSendData()
		{
			T_DataCommandOutputPin::SetPinValueHigh();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction(SPISettings( 8000000, MSBFIRST, SPI_MODE0));
		}

		inline void SetChipSelectHigh()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void SetChipSelectLow()
		{
			T_ChipSelectOutputPin::SetPinValueLow();
		}

		inline void IntSystemInitStart()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

/*
		inline void SetResetHigh()
		{
			T_ResetOutputPin::SetPinValueHigh();
		}

		inline void SetResetLow()
		{
			T_ResetOutputPin::SetPinValueLow();
		}
*/
		inline void SetRegisterSelectHigh()
		{
			T_DataCommandOutputPin::SetPinValueHigh();
		}

		inline void SetRegisterSelectLow()
		{
			T_DataCommandOutputPin::SetPinValueLow();
		}

		inline void IntSystemInitEnd() {} // Placeholder
	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_DataCommandOutputPin
	> class Display_odtSSD_SPI :
		public Display_SPI<
				T_SPI, C_SPI,
				T_ChipSelectOutputPin,
				T_DataCommandOutputPin
			>
	{
		typedef Display_SPI<
				T_SPI, C_SPI,
				T_ChipSelectOutputPin,
				T_DataCommandOutputPin
			> inherited;

	public:
/*
		inline void SendPinsConfigCommand()
		{
			inherited::SendCommand( C_COMPinsConfig );
		}
*/
		template <typename T> void SendBuffer( T *AInstance, uint8_t *ABuffer, uint16_t ASize )
		{
			if( AInstance->Type().GetValue() == odtSH1106 )
			{
				inherited::SendCommand(SSD1306Const::SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
				inherited::SendCommand(SSD1306Const::SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
				inherited::SendCommand(SSD1306Const::SSD1306_SETSTARTLINE | 0x0); // line #0

				uint8_t height=64;
				uint8_t width=132;
				height >>= 3;
				width >>= 3;
				// I2C
				uint8_t m_row = 0;
				uint8_t m_col = 2;
				int p = 0;
				for ( uint8_t i = 0; i < height; i++)
				{
					// send a bunch of data in one xmission
					inherited::SendCommand(0xB0 + i + m_row);//set page address
					inherited::SendCommand(m_col & 0xf);//set lower column address
					inherited::SendCommand(0x10 | (m_col >> 4));//set higher column address

					for( uint8_t j = 0; j < 8; j++)
					{
//						T_ChipSelectOutputPin::SetPinValue( true );
//						T_DataCommandOutputPin::SetPinValue( true );
//						T_ChipSelectOutputPin::SetPinValue( false );

						inherited::DataCommandOutputPin().SetPinValueHigh();
						inherited::ChipSelectOutputPin().SetPinValueLow();

						C_SPI.beginTransaction(SPISettings( 8000000, MSBFIRST, SPI_MODE0));

						for ( uint8_t k = 0; k < width; k++, p++)
							C_SPI.transfer( ABuffer[ p ] );

						C_SPI.endTransaction();

						inherited::ChipSelectOutputPin().SetPinValueHigh();
					}
				}
			}

			else
			{
/*
				inherited::SendCommand(SSD1306Const::SSD1306_COLUMNADDR);
				inherited::SendCommand(0);   // Column start address (0 = reset)
				inherited::SendCommand( WIDTH - 1 ); // Column end address (127 = reset)

				inherited::SendCommand(SSD1306Const::SSD1306_PAGEADDR);
				inherited::SendCommand(0); // Page start address (0 = reset)
*/
				inherited::DataCommandOutputPin().SetPinValueHigh();
				inherited::ChipSelectOutputPin().SetPinValueLow();
//				T_ChipSelectOutputPin::StartSendData();
//				T_ChipSelectOutputPin::SetPinValue( true );

				C_SPI.beginTransaction(SPISettings( 8000000, MSBFIRST, SPI_MODE0));

//				for (uint16_t i = 0; i < ( WIDTH * HEIGHT / 8 ); ++i )
				for (uint16_t i = 0; i < ASize; ++i )
					C_SPI.transfer( ABuffer[ i ] );

				C_SPI.endTransaction();

				inherited::ChipSelectOutputPin().SetPinValueHigh();
			}
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_BackgroundColor,
		typename T_ChipSelectOutputPin,
		typename T_Contrast,
		typename T_DataCommandOutputPin,
		typename T_Elements_Render,
		uint8_t HEIGHT,
		typename T_PowerOn,
		typename T_PreChargePeriod,
		typename T_ResetOutputPin,
		typename T_Text,
		typename T_UseChargePump,
		uint8_t WIDTH
	> class Display_odtSH1106_SPI :
		public Display_odtSSD1306_SPI<
				T_SPI, C_SPI,
				T_BackgroundColor,
				T_ChipSelectOutputPin,
				T_Contrast,
				T_DataCommandOutputPin,
				T_Elements_Render,
				HEIGHT,
				T_PowerOn,
				T_PreChargePeriod,
				T_ResetOutputPin,
				T_Text,
				T_UseChargePump,
				WIDTH
			>
	{
		typedef Display_odtSSD1306_SPI<
				T_SPI, C_SPI,
				T_BackgroundColor,
				T_ChipSelectOutputPin,
				T_Contrast,
				T_DataCommandOutputPin,
				T_Elements_Render,
				HEIGHT,
				T_PowerOn,
				T_PreChargePeriod,
				T_ResetOutputPin,
				T_Text,
				T_UseChargePump,
				WIDTH
			>  inherited;

	public:
		void SendBuffer( uint8_t *ABuffer )
		{
			inherited::SendCommand(SSD1306Const::SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
			inherited::SendCommand(SSD1306Const::SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
			inherited::SendCommand(SSD1306Const::SSD1306_SETSTARTLINE | 0x0); // line #0

			uint8_t height=64;
			uint8_t width=132;
			height >>= 3;
			width >>= 3;
			// I2C
			uint8_t m_row = 0;
			uint8_t m_col = 2;
			int p = 0;
			for ( uint8_t i = 0; i < height; i++)
			{
				// send a bunch of data in one xmission
				inherited::SendCommand(0xB0 + i + m_row);//set page address
				inherited::SendCommand(m_col & 0xf);//set lower column address
				inherited::SendCommand(0x10 | (m_col >> 4));//set higher column address

				for( uint8_t j = 0; j < 8; j++)
				{
//					T_ChipSelectOutputPin::SetPinValue( true );
//					T_DataCommandOutputPin::SetPinValue( true );
//					T_ChipSelectOutputPin::SetPinValue( false );

					inherited::DataCommandOutputPin().SetPinValueHigh();
					inherited::ChipSelectOutputPin().SetPinValueLow();

					C_SPI.beginTransaction(SPISettings( 8000000, MSBFIRST, SPI_MODE0));

					for ( uint8_t k = 0; k < width; k++, p++)
						C_SPI.transfer( ABuffer[ p ] );

					C_SPI.endTransaction();

					inherited::ChipSelectOutputPin().SetPinValueHigh();
				}
			}
		}
	};
 */
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif