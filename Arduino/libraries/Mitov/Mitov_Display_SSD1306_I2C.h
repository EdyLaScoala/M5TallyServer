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
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address
	> class Display_odtSSD_I2C :
		public T_Address
	{
	public:
        _V_PROP_( Address )

	public:
		void SendCommand( uint8_t ACommand )
		{
//			Serial.println( ACommand, HEX );
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( uint8_t( 0x00 )); // Co = 0, D/C = 0
			C_I2C.write( ACommand );
			C_I2C.endTransmission();
		}

		template <typename T> void SendBuffer( T *AInstance, uint8_t *ABuffer, uint16_t ASize )
		{
			if( AInstance->Type().GetValue() == odtSH1106 )
			{
				SendCommand(SSD1306Const::SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
				SendCommand(SSD1306Const::SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
				SendCommand(SSD1306Const::SSD1306_SETSTARTLINE | 0x0); // line #0

				// save I2C bitrate
			#ifdef TWBR
				uint8_t twbrbackup = TWBR;
				TWBR = 12; // upgrade to 400KHz!
			#endif

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
					SendCommand(0xB0 + i + m_row);//set page address
					SendCommand(m_col & 0xf);//set lower column address
					SendCommand(0x10 | (m_col >> 4));//set higher column address

					for( uint8_t j = 0; j < 8; j++)
					{
						C_I2C.beginTransmission( uint8_t( Address().GetValue()) );
						C_I2C.write( SSD1306Const::SSD1306_SETSTARTLINE );
						C_I2C.write( ABuffer + p, width );
						p += width;
						C_I2C.endTransmission();
					}
				}

			#ifdef TWBR
				TWBR = twbrbackup;
			#endif
			}

			else
			{
				// save I2C bitrate
			#ifdef TWBR
				uint8_t twbrbackup = TWBR;
				TWBR = 12; // upgrade to 400KHz!
			#endif

				// I2C
	//			for( uint16_t i=0; i < ( WIDTH * HEIGHT /8); i += 16 )
				for( uint16_t i=0; i < ASize; i += 16 )
				{
					// send a bunch of data in one xmission
					C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
					C_I2C.write( SSD1306Const::SSD1306_SETSTARTLINE );
					C_I2C.write( ABuffer + i, 16 );
					C_I2C.endTransmission();
				}

			#ifdef TWBR
				TWBR = twbrbackup;
			#endif
			}
		}

		inline void IntSystemInitStart()
		{
#ifdef __SAM3X8E__
			// Force 400 KHz I2C, rawr! (Uses pins 20, 21 for SDA, SCL)
			TWI1->TWI_CWGR = 0;
			TWI1->TWI_CWGR = ((VARIANT_MCK / (2 * 400000)) - 4) * 0x101;
#endif
		}

		inline void IntSystemInitEnd() {} // Placeholder
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_BackgroundColor,
		typename T_Contrast,
		typename T_Elements_Render,
		uint8_t HEIGHT,
		typename T_PowerOn,
		typename T_PreChargePeriod,
		typename T_ResetOutputPin,
		typename T_Text,
		typename T_UseChargePump,
		uint8_t WIDTH
	> class Display_odtSSD1306_I2C :
		public T_Address,
		public T_PreChargePeriod,
		public T_Contrast,
		public T_PowerOn,
		public T_ResetOutputPin,
		public T_Text,
		public T_UseChargePump
	{
	public:
		typedef T_BackgroundColor TYPE_T_BACKGROUND_COLOR;
		typedef T_Text TYPE_T_TEXT;

	public:
		_V_PROP_( Address )
		_V_PROP_( UseChargePump )
		_V_PROP_( Contrast )
		_V_PROP_( PowerOn )
		_V_PROP_( Text )
		_V_PROP_( PreChargePeriod )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::Call();
		}

	public:
/*
		inline static void SendPinsConfigCommand()
		{
			SendCommand( C_COMPinsConfig );
		}
*/
		void SendCommand( uint8_t ACommand )
		{
//			Serial.println( ACommand, HEX );
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( uint8_t( 0x00 )); // Co = 0, D/C = 0
			C_I2C.write( ACommand );
			C_I2C.endTransmission();
		}

/*
		static void SendStartAddress( uint16_t AAddress )
		{
//			return;

//			SendCommand(SSD1306Const::SSD1306_COLUMNADDR);
//			SendCommand( AAddress );   // Column start address (0 = reset)
//			SendCommand( WIDTH - 1 ); // Column end address (127 = reset)

			SendCommand(SSD1306Const::SSD1306_PAGEADDR);
			SendCommand( AAddress / 128 ); // Page start address (0 = reset)

			if( HEIGHT == 64 )
				SendCommand(7); // Page end address

			else if( HEIGHT == 32 )
				SendCommand(3); // Page end address

			else if( HEIGHT == 16 )
				SendCommand(1); // Page end address

			SendCommand( SSD1306Const::SSD1306_COLUMNADDR );
			SendCommand( AAddress % 128 );
			SendCommand( WIDTH - 1 ); // Column end address (127 = reset)
		}

		static void ReadBuffer( uint8_t *ABuffer, uint16_t ALength )
		{
		#ifdef TWBR
			uint8_t twbrbackup = TWBR;
			TWBR = 12; // upgrade to 400KHz!
		#endif

			// I2C
			// send a bunch of data in one xmission
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( SSD1306Const::SSD1306_SETSTARTLINE );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue()), ALength + 1 );

			uint8_t AValue = C_I2C.read(); // Dummy

            for ( int16_t i = 0; C_I2C.available() && ( i < ALength ); i++ )
			{
				*ABuffer ++ = C_I2C.read();
//				uint8_t ANewValue = C_I2C.read();
//                *ABuffer ++ = ( AValue >> 1 ) | ( ANewValue << 7 );
//				AValue = ANewValue;
			}

		#ifdef TWBR
			TWBR = twbrbackup;
		#endif
		}

		static void SendBuffer( uint8_t *ABuffer, int16_t ALength )
		{
//			Serial.println( "SendBuffer" );

		#ifdef TWBR
			uint8_t twbrbackup = TWBR;
			TWBR = 12; // upgrade to 400KHz!
		#endif

			// I2C
			int16_t i = 0;
			for( ; i < ALength - 16; i += 16 )
			{
				Serial.println( "16" );

				// send a bunch of data in one xmission
				C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
				C_I2C.write( SSD1306Const::SSD1306_SETSTARTLINE );
				C_I2C.write( ABuffer + i, 16 );
				C_I2C.endTransmission();
			}

			if( i < ALength )
			{
				Serial.println( ALength - i );

				C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
				C_I2C.write( SSD1306Const::SSD1306_SETSTARTLINE );
				C_I2C.write( ABuffer + i, ALength - i );
				C_I2C.endTransmission();
			}

		#ifdef TWBR
			TWBR = twbrbackup;
		#endif
		}
*/
		void SendBuffer( uint8_t *ABuffer )
		{
/*
			SendCommand(SSD1306Const::SSD1306_COLUMNADDR);
			SendCommand(0);   // Column start address (0 = reset)
			SendCommand( WIDTH - 1 ); // Column end address (127 = reset)

			SendCommand(SSD1306Const::SSD1306_PAGEADDR);
			SendCommand(0); // Page start address (0 = reset)

			if( HEIGHT == 64 )
				SendCommand(7); // Page end address

			if( HEIGHT == 32 )
				SendCommand(3); // Page end address

			if( HEIGHT == 16 )
				SendCommand(1); // Page end address
*/
			// save I2C bitrate
		#ifdef TWBR
			uint8_t twbrbackup = TWBR;
			TWBR = 12; // upgrade to 400KHz!
		#endif

			// I2C
			for( uint16_t i=0; i < ( WIDTH * HEIGHT /8); i += 16 )
			{
				// send a bunch of data in one xmission
				C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
				C_I2C.write( SSD1306Const::SSD1306_SETSTARTLINE );
				C_I2C.write( ABuffer + i, 16 );
				C_I2C.endTransmission();
			}

		#ifdef TWBR
			TWBR = twbrbackup;
		#endif
		}

		inline void SetResetHigh()
		{
			T_ResetOutputPin::SetPinValueHigh();
		}

		inline void SetResetLow()
		{
			T_ResetOutputPin::SetPinValueLow();
		}

		inline void IntSystemInitStart()
		{
#ifdef __SAM3X8E__
			// Force 400 KHz I2C, rawr! (Uses pins 20, 21 for SDA, SCL)
			TWI1->TWI_CWGR = 0;
			TWI1->TWI_CWGR = ((VARIANT_MCK / (2 * 400000)) - 4) * 0x101;
#endif
		}

		inline void IntSystemInitEnd() {} // Placeholder
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_BackgroundColor,
		typename T_Contrast,
		typename T_Elements_Render,
		uint8_t HEIGHT,
		typename T_PowerOn,
		typename T_PreChargePeriod,
		typename T_ResetOutputPin,
		typename T_Text,
		typename T_UseChargePump,
		uint8_t WIDTH
	> class Display_odtSH1106_I2C :
		public Display_odtSSD1306_I2C<
				T_I2C, C_I2C,
				T_Address,
				T_BackgroundColor,
				T_Contrast,
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
		typedef Display_odtSSD1306_I2C<
				T_I2C, C_I2C,
				T_Address,
				T_BackgroundColor,
				T_Contrast,
				T_Elements_Render,
				HEIGHT,
				T_PowerOn,
				T_PreChargePeriod,
				T_ResetOutputPin,
				T_Text,
				T_UseChargePump,
				WIDTH
			> inherited;

	public:
		void SendBuffer( uint8_t *ABuffer )
		{
			inherited::SendCommand(SSD1306Const::SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
			inherited::SendCommand(SSD1306Const::SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
			inherited::SendCommand(SSD1306Const::SSD1306_SETSTARTLINE | 0x0); // line #0

			// save I2C bitrate
		#ifdef TWBR
			uint8_t twbrbackup = TWBR;
			TWBR = 12; // upgrade to 400KHz!
		#endif

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
					C_I2C.beginTransmission( uint8_t( inherited::Address().GetValue()) );
					C_I2C.write( SSD1306Const::SSD1306_SETSTARTLINE );
					C_I2C.write( ABuffer + p, width );
					p += width;
					C_I2C.endTransmission();
				}
			}

		#ifdef TWBR
			TWBR = twbrbackup;
		#endif

		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif