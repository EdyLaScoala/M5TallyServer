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
	namespace APDS9960Const
	{
		const uint8_t ENABLE      = 0x80;
		const uint8_t ATIME       = 0x81;
		const uint8_t WTIME       = 0x83;
		const uint8_t AILTL       = 0x84;
		const uint8_t AILTH       = 0x85;
		const uint8_t AIHTL       = 0x86;
		const uint8_t AIHTH       = 0x87;
		const uint8_t PILT        = 0x89;
		const uint8_t PIHT        = 0x8B;
		const uint8_t PERS        = 0x8C;
		const uint8_t CONFIG1     = 0x8D;
		const uint8_t PPULSE      = 0x8E;
		const uint8_t CONTROL     = 0x8F;
		const uint8_t CONFIG2     = 0x90;
		const uint8_t ID          = 0x92;
		const uint8_t STATUS      = 0x93;
		const uint8_t CDATAL      = 0x94;
		const uint8_t CDATAH      = 0x95;
		const uint8_t RDATAL      = 0x96;
		const uint8_t RDATAH      = 0x97;
		const uint8_t GDATAL      = 0x98;
		const uint8_t GDATAH      = 0x99;
		const uint8_t BDATAL      = 0x9A;
		const uint8_t BDATAH      = 0x9B;
		const uint8_t PDATA       = 0x9C;
		const uint8_t POFFSET_UR  = 0x9D;
		const uint8_t POFFSET_DL  = 0x9E;
		const uint8_t CONFIG3     = 0x9F;
		const uint8_t GPENTH      = 0xA0;
		const uint8_t GEXTH       = 0xA1;
		const uint8_t GCONF1      = 0xA2;
		const uint8_t GCONF2      = 0xA3;
		const uint8_t GOFFSET_U   = 0xA4;
		const uint8_t GOFFSET_D   = 0xA5;
		const uint8_t GPULSE      = 0xA6;
		const uint8_t GOFFSET_L   = 0xA7;
		const uint8_t GOFFSET_R   = 0xA9;
		const uint8_t GCONF3      = 0xAA;
		const uint8_t GCONF4      = 0xAB;
		const uint8_t GFLVL       = 0xAE;
		const uint8_t GSTATUS     = 0xAF;
		const uint8_t IFORCE      = 0xE4;
		const uint8_t PICLEAR     = 0xE5;
		const uint8_t CICLEAR     = 0xE6;
		const uint8_t AICLEAR     = 0xE7;
		const uint8_t GFIFO_U     = 0xFC;
		const uint8_t GFIFO_D     = 0xFD;
		const uint8_t GFIFO_L     = 0xFE;
		const uint8_t GFIFO_R     = 0xFF;
	}
//---------------------------------------------------------------------------
	template <
		typename T_FClocked,
		typename T_Gain,
		typename T_LEDPulses,
		typename T_OutputPin
	> class TArduinoAPDS9960Proximity :
		public T_FClocked,
		public T_Gain,
		public T_LEDPulses,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Gain )
		_V_PROP_( LEDPulses )

	public:
		_V_PROP_( FClocked )

	public:
		bool GetIsEnabled() { return T_OutputPin::GetPinIsConnected(); }

	public:
		inline TArduinoAPDS9960Proximity()
		{
			FClocked() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ColorOutputPin,
		typename T_FClocked,
		typename T_Gain,
		typename T_OutputPins_Red,
		typename T_OutputPins_Green,
		typename T_OutputPins_Blue,
		typename T_OutputPins_Clear
	> class TArduinoAPDS9960Color :
		public T_ColorOutputPin,
		public T_FClocked,
		public T_Gain,
		public T_OutputPins_Red,
		public T_OutputPins_Green,
		public T_OutputPins_Blue,
		public T_OutputPins_Clear
	{
	public:
		_V_PIN_( ColorOutputPin )
		_V_PIN_( OutputPins_Red )
		_V_PIN_( OutputPins_Green )
		_V_PIN_( OutputPins_Blue )
		_V_PIN_( OutputPins_Clear )

	public:
		_V_PROP_( Gain )

	public:
		_V_PROP_( FClocked )

	public:
		bool GetIsEnabled() { return T_OutputPins_Red::GetPinIsConnected() || T_OutputPins_Green::GetPinIsConnected() || T_OutputPins_Blue::GetPinIsConnected() || T_OutputPins_Clear::GetPinIsConnected() || T_ColorOutputPin::GetPinIsConnected(); }

	public:
		inline TArduinoAPDS9960Color()
		{
			FClocked() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Count,
		typename T_Length
	> class TArduinoAPDS9960LEDPulses :
		public T_Count,
		public T_Length
	{
	public:
		_V_PROP_( Count )
		_V_PROP_( Length )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DirectionOutputPins_X,
		typename T_DirectionOutputPins_Y,
		typename T_FClocked,
		typename T_LEDPulses,
		typename T_OutputPins_Up,
		typename T_OutputPins_Down,
		typename T_OutputPins_Left,
		typename T_OutputPins_Right,
		typename T_Threshold,
		typename T_Timeout
	> class TArduinoAPDS9960Gesture :
		public T_DirectionOutputPins_X,
		public T_DirectionOutputPins_Y,
		public T_FClocked,
		public T_LEDPulses,
		public T_OutputPins_Up,
		public T_OutputPins_Down,
		public T_OutputPins_Left,
		public T_OutputPins_Right,
		public T_Threshold,
		public T_Timeout
	{
	public:
		_V_PIN_( DirectionOutputPins_X )
		_V_PIN_( DirectionOutputPins_Y )

		_V_PIN_( OutputPins_Up )
		_V_PIN_( OutputPins_Down )
		_V_PIN_( OutputPins_Left )
		_V_PIN_( OutputPins_Right )

	public:
		_V_PROP_( LEDPulses )
		_V_PROP_( Threshold )
		_V_PROP_( Timeout )

	public:
		_V_PROP_( FClocked )

	public:
		bool GetIsEnabled() { return T_DirectionOutputPins_X::GetPinIsConnected() || T_DirectionOutputPins_Y::GetPinIsConnected() || T_OutputPins_Up::GetPinIsConnected() || T_OutputPins_Down::GetPinIsConnected() || T_OutputPins_Left::GetPinIsConnected() || T_OutputPins_Right::GetPinIsConnected(); }

	public:
		inline TArduinoAPDS9960Gesture()
		{
			FClocked() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Color,
		typename T_FLastTime,
		typename T_FX0,
		typename T_FX1,
		typename T_FY0,
		typename T_FY1,
		typename T_Gesture,
		typename T_IntegrationTime,
		typename T_LEDPower,
		typename T_PowerDown,
		typename T_Proximity,
		typename T_WaitTime
	> class APDS9960I2C : 
		public T_Address,
		public T_Color,
		public T_FLastTime,
		public T_FX0,
		public T_FX1,
		public T_FY0,
		public T_FY1,
		public T_Gesture,
		public T_IntegrationTime,
		public T_LEDPower,
		public T_PowerDown,
		public T_Proximity,
		public T_WaitTime
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Color )
		_V_PROP_( Gesture )
		_V_PROP_( IntegrationTime )
		_V_PROP_( LEDPower )
		_V_PROP_( PowerDown )
		_V_PROP_( Proximity )
		_V_PROP_( WaitTime )

	protected:
		enum TPolarity { pNone, pPositive, pNegative };

	protected:
		_V_PROP_( FLastTime )

		_V_PROP_( FX0 )
		_V_PROP_( FX1 )

		_V_PROP_( FY0 )
		_V_PROP_( FY1 )

	protected:
		void WriteRegister8( uint8_t ARegister, uint8_t value )
		{
//			Serial.print( ARegister, HEX ); Serial.print( " : " ); Serial.println( value, HEX );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ARegister );
			C_I2C.write( value );
			C_I2C.endTransmission();
		}

		uint8_t ReadRegister8(uint8_t ARegister)
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

			C_I2C.requestFrom( uint8_t( Address().GetValue() ), uint8_t( 1 ) );
			return C_I2C.read();
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), ACount );
		}

		uint16_t Read16()
		{
			uint16_t AResult = C_I2C.read();
			return AResult | uint16_t( C_I2C.read()) << 8;
		}


	public:
		void Update_WTIME_CONFIG1()
		{
			uint8_t AValue;
			if( WaitTime() > 0.712 )
			{
				WriteRegister8( APDS9960Const::CONFIG1, 0b00000010 );

				AValue = 255 - MitovMin<uint8_t>( 255, ( ( float( WaitTime()) - 0.03 ) / ( 0.00278 * 12 )));
			}

			else
			{
				WriteRegister8( APDS9960Const::CONFIG1, 0b00000000 );

				AValue = 255 - MitovMin<uint8_t>( 255, ( ( float( WaitTime()) - 0.00278 ) / 0.00278 ));
			}

//			AValue = 0xFF;
			WriteRegister8( APDS9960Const::WTIME, AValue );
//			Update_WTIME();
		}

		void Update_WTIME_CONFIG1_ENABLE()
		{
			Update_WTIME_CONFIG1();
			Update_ENABLE();
		}

		void Update_GPULSE()
		{
			uint8_t AValue = MitovMin<uint8_t>( 63, Gesture().Count().GetValue() );

			if( Gesture().Length().GetValue() > 16 )
				AValue |= 0b11000000;

			else if( Gesture().Length().GetValue() > 8 )
				AValue |= 0b10000000;

			else if( Gesture().Length().GetValue() > 4 )
				AValue |= 0b01000000;

			WriteRegister8( APDS9960Const::GPULSE, AValue );
		}

		void Update_PPULSE()
		{
			uint8_t AValue = MitovMin<uint8_t>( 63, Proximity().Count().GetValue() );

			if( Proximity().Length().GetValue() > 16 )
				AValue |= 0b11000000;

			else if( Proximity().Length().GetValue() > 8 )
				AValue |= 0b10000000;

			else if( Proximity().Length().GetValue() > 4 )
				AValue |= 0b01000000;

			WriteRegister8( APDS9960Const::PPULSE, AValue );
		}

		void Update_ATIME()
		{
			uint8_t AValue = 255 - MitovMin<uint8_t>( 255, ( ( IntegrationTime().GetValue() - 2.78 ) / 2.78 ));
//AValue = 0xFC;
			WriteRegister8( APDS9960Const::ATIME, AValue );
		}

		void Update_GCONF4()
		{
			uint8_t AValue = ( Gesture().GetIsEnabled() ) ? 0b00000011 : 0;
//			AValue = 0b00000011;
			WriteRegister8( APDS9960Const::GCONF4, AValue );
		}

		void Update_ENABLE()
		{
			uint8_t AValue = ( PowerDown() ) ? 0 : 0b00000001 ;


			if( WaitTime().GetValue() > 0 )
				AValue |= 0b00001000; // Wait

			if( Gesture().GetIsEnabled() )
				AValue |= 0b01000000;

			if( Proximity().GetIsEnabled() )
				AValue |= 0b00000100;

			if( Color().GetIsEnabled() )
				AValue |= 0b00000010;

			WriteRegister8( APDS9960Const::ENABLE, AValue );
		}

		void Update_CONTROL()
		{
			uint8_t AValue;

			if( LEDPower().GetValue() > 50.0 )
				AValue = 0b00000000;

			else if( LEDPower().GetValue() > 25.0 )
				AValue = 0b01000000;

			else if( LEDPower().GetValue() > 12.5 )
				AValue = 0b10000000;

			else
				AValue = 0b11000000;


			if( Proximity().Gain().GetValue() > 4 )
				AValue |= 0b00001100;

			else if( Proximity().Gain().GetValue() > 2 )
				AValue |= 0b00001000;

			else if( Proximity().Gain().GetValue() > 1 )
				AValue |= 0b00000100;


			if( Color().Gain().GetValue() > 16 )
				AValue |= 0b00000011;

			else if( Color().Gain().GetValue() > 4 )
				AValue |= 0b00000010;

			else if( Color().Gain().GetValue() > 1 )
				AValue |= 0b00000001;


			WriteRegister8( APDS9960Const::CONTROL, AValue );
		}

	public:
		inline void Proximity_ClockInputPin_o_Receive( void *_Data )
		{
			Proximity().FClocked() = true;
		}

		inline void Color_ClockInputPin_o_Receive( void *_Data )
		{
			Color().FClocked() = true;
		}

		inline void Gesture_ClockInputPin_o_Receive( void *_Data )
		{
			Gesture().FClocked() = true;
		}

	public:
		inline void SystemStart()
		{
///*
			WriteRegister8( APDS9960Const::ENABLE, 0x00 );
			Update_WTIME_CONFIG1();
			Update_GPULSE();
			Update_PPULSE();

////			WriteRegister8( 0xAB, 2 );

			Update_GCONF4();

////			WriteRegister8( 0x80, 1 );

			Update_ENABLE();

			Update_ATIME();

			Update_CONTROL();

//			WriteRegister8( 0x80, 9 );

//			WriteRegister8( 0x80, 0xD );


			delay(10);

			FLastTime() = millis();
//*/
/*
			WriteRegister8( APDS9960Const::ENABLE, 0x00 );
			WriteRegister8( APDS9960Const::WTIME, 0xFF );
			WriteRegister8( APDS9960Const::GPULSE, 0x8F ); // 16us, 16 pulses // default is: 0x40 = 8us, 1 pulse
			WriteRegister8( APDS9960Const::PPULSE, 0x8F ); // 16us, 16 pulses // default is: 0x40 = 8us, 1 pulse

//			WriteRegister8( APDS9960Const::GPULSE, 0xBF ); // 16us, 16 pulses // default is: 0x40 = 8us, 1 pulse
//			WriteRegister8( APDS9960Const::PPULSE, 0xBF ); // 16us, 16 pulses // default is: 0x40 = 8us, 1 pulse

			WriteRegister8( APDS9960Const::GCONF4, 0b00000010 );
			WriteRegister8( APDS9960Const::GCONF4, 0b00000011 );

			WriteRegister8( APDS9960Const::ENABLE, 0b00000001 );

			WriteRegister8( APDS9960Const::ENABLE, 0b00001001 );

//			WriteRegister8( APDS9960Const::GestureIntEnable(true)) return false;
//			WriteRegister8( APDS9960Const::GestureMode(true)) return false;
//			if (!enablePower()) return false;
//			if (!enableWait()) return false;
			// set ADC integration time to 10 ms
			WriteRegister8( APDS9960Const::ATIME, 256 - (10 / 2.78));
			// set ADC gain 4x (0x00 => 1x, 0x01 => 4x, 0x02 => 16x, 0x03 => 64x)
			WriteRegister8( APDS9960Const::CONTROL, 0x02);
			delay(10);
			// enable power
//			if (!enablePower()) return false;

			WriteRegister8( APDS9960Const::ENABLE, 0b00001001 );

			WriteRegister8( APDS9960Const::ENABLE, 0b00001101 );
*/
		}

		inline void SystemLoopBegin()
		{
			uint8_t AStatus = ReadRegister8( APDS9960Const::STATUS );
//			Serial.println( AStatus, BIN );
			if( Proximity().GetIsEnabled() )
				if( AStatus & 0b00000010 )
				{
					uint8_t AValue = ReadRegister8( APDS9960Const::PDATA );

					if( Proximity().FClocked() )
					{
						Proximity().OutputPin().SetPinValue( float( AValue ) / 255.0 );					
						Proximity().FClocked() = false;
					}
//						Proximity().OutputPin().SetPinValue( float( 255 - AValue ) / 255.0 );

				}

			if( Color().GetIsEnabled() )
				if( AStatus & 0b00000001 )
				{
					StartReadRegisters( APDS9960Const::CDATAL, 8 );
					float AClear = Read16();
					float ARed = Read16();
					float AGreen = Read16();
					float ABlue = Read16();

					if( Color().FClocked() )
					{
						Color().FClocked() = false;

						Color().OutputPins_Clear().SetPinValue( AClear );
						Color().OutputPins_Red().SetPinValue( ARed );
						Color().OutputPins_Green().SetPinValue( AGreen );
						Color().OutputPins_Blue().SetPinValue( ABlue );

						if( Color().ColorOutputPin().GetPinIsConnected() )
						{
							if( ! AClear )
								AClear = 1;

							TColor AColor( 
									( float( ARed ) / AClear ) * 255.0 + 0.5,
									( float( AGreen ) / AClear ) * 255.0 + 0.5,
									( float( ABlue ) / AClear ) * 255.0 + 0.5
								);

							Color().ColorOutputPin().SetPinValue( AColor );
						}
					}

				}

			if( Gesture().GetIsEnabled() )
			{
				if( ( FX0().GetValue() != pNone ) || ( FY0().GetValue() != pNone ) )
					if( millis() - FLastTime().GetValue() > Gesture().Timeout().GetValue() )
					{
//						Serial.println( '0' );
						FX0() = pNone;
						FX1() = pNone;

						FY0() = pNone;
						FY1() = pNone;
					}

//				uint8_t AGStatus = ReadRegister8( APDS9960Const::GSTATUS );

//				if( AGStatus & 0b00000001 )
				uint8_t AFIFICount = ReadRegister8( APDS9960Const::GFLVL );
				if( AFIFICount > 0 )
				{
					uint8_t ACount = AFIFICount * 4; 

					StartReadRegisters( APDS9960Const::GFIFO_U, ACount );
					uint8_t fifo_data[128];

					for( uint8_t i = 0; i < ACount; ++i )
						fifo_data[ i ] = C_I2C.read();

//					uint8_t ADirection = 0;

					for (int i = 0; i+3 < ACount; i += 4 ) 
					{
						uint8_t AUp = fifo_data[i];
						uint8_t ADown = fifo_data[i+1];
						uint8_t ALeft = fifo_data[i+2];
						uint8_t ARight = fifo_data[i+3];

						int32_t AX = int32_t( ARight ) - int32_t( ALeft );
						int32_t AY = int32_t( AUp ) - int32_t( ADown );
	
						if( Gesture().FClocked() )
						{
							Gesture().FClocked() = false;
							Gesture().DirectionOutputPins_X().SetPinValue( float( AX ) / 511 + 0.5 );
							Gesture().DirectionOutputPins_Y().SetPinValue( float( AY ) / 511 + 0.5 );
						}

						int8_t AThreshold = Gesture().Threshold().GetValue() * 255 + 0.5; //10; //Gesture().Threshold().GetValue();

						if( AX > AThreshold )
						{
							if( FX0().GetValue() == pNone )
								FLastTime() = millis();

//							Serial.print( "+" );
							if( FX1().GetValue() == pNegative )
								FX1() = pNone;

							if( FX0().GetValue() == pNegative )
								FX1() = pPositive;

							else
								FX0() = pPositive;
						}

						if( AX < - AThreshold ) // ( Gesture().Threshold().GetValue() ))
						{
							if( FX0().GetValue() == pNone )
								FLastTime() = millis();

//							Serial.println( AX );
//							Serial.print( "-" );
//							Serial.println( AThreshold );
							if( FX1().GetValue() == pPositive )
								FX1() = pNone;

							if( FX0().GetValue() == pPositive )
								FX1() = pNegative;

							else
								FX0() = pNegative;
						}

						if( AY > AThreshold )
						{
							if( FY0().GetValue() == pNone )
								FLastTime() = millis();

//							Serial.print( "+" );
							if( FY1().GetValue() == pNegative )
								FY1() = pNone;

							if( FY0().GetValue() == pNegative )
								FY1() = pPositive;

							else
								FY0() = pPositive;
						}

						if( AY < - AThreshold ) // ( Gesture().Threshold().GetValue() ))
						{
							if( FY0().GetValue() == pNone )
								FLastTime() = millis();

//							Serial.println( AX );
//							Serial.print( "-" );
//							Serial.println( AThreshold );
							if( FY1().GetValue() == pPositive )
								FY1() = pNone;

							if( FY0().GetValue() == pPositive )
								FY1() = pNegative;

							else
								FY0() = pNegative;
						}

						if( ( FX0().GetValue() != pNone ) && ( FX1().GetValue() != pNone ) )
							if( abs( AX ) < AThreshold )
							{
								if( FX1().GetValue() == pPositive )
									Gesture().OutputPins_Left().ClockPin();

								else
									Gesture().OutputPins_Right().ClockPin();

								FX0() = pNone;
								FX1() = pNone;
							}

						if( ( FY0().GetValue() != pNone ) && ( FY1().GetValue() != pNone ) )
							if( abs( AY ) < AThreshold )
							{
								if( FY1().GetValue() == pPositive )
									Gesture().OutputPins_Down().ClockPin();

								else
									Gesture().OutputPins_Up().ClockPin();

								FY0() = pNone;
								FY1() = pNone;
							}

//						Serial.println( AY );
					}
				}
			}
		}

	public:
		APDS9960I2C()
		{
			FX0() = pNone;
			FX1() = pNone;

			FY0() = pNone;
			FY1() = pNone;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

