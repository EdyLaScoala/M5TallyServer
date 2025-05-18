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
/*
	class PCA9685PWMChannel
	{
	public:
		float	InitialValue = 0.0f;
		float	OnValue = 0.0f;

	};
*/
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class PCA9685PWMChannelBasic
	{
	public:
		void UpdateChannelValue( int AIndex, float AValue )
		{
			uint16_t	on = Channels[ AIndex ].OnValue * 4095 + 0.5;
			uint16_t	off = AValue * 4095 + 0.5;

//			Serial.print( "AIndex: " );
//			Serial.println( AIndex );
//			Serial.println( on );
//			Serial.println( off );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(PCA9685Const::LED0_ON_L + 4 * AIndex );
			C_I2C.write(on);
			C_I2C.write(on>>8);
			C_I2C.write(off);
			C_I2C.write(off>>8);
			C_I2C.endTransmission();

//			FChangedChannels &= ~( 1 << AIndex );
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FModified,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_OnValue,
		typename T_TRANSFORM
	> class PCA9685PWMChannel :
		public T_Enabled,
		public T_FModified,
		public T_InitialValue,
		public T_OnValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( OnValue )

	protected:
		_V_PROP_( FModified )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AOnValue = *(float *)_Data;
			SetChannelValue( AOnValue );
		}

	public:
		void UpdateValues()
		{
			if( FModified() )
				UpdateChannelValue();

		}

		void UpdateChannelValue()
		{
			uint16_t AOn = OnValue() * 4095 + 0.5;
			uint16_t AOff = T_TRANSFORM::Transform( InitialValue() ) * 4095 + 0.5;

//			Serial.print( "AIndex: " );
//			Serial.println( AIndex );
//			Serial.println( on );
//			Serial.println( off );

			C_OWNER.WriteChannelData( C_INDEX, AOn, AOff );

//			FChangedChannels &= ~( 1 << AIndex );
			FModified() = false;
		}

		inline void SetChannelValue( float AOnValue )
		{
			AOnValue = MitovConstrain( AOnValue, 0.0f, 1.0f );
			if( InitialValue() == AOnValue )
				return;

			InitialValue() = AOnValue;
			if( C_OWNER.Sleep() || ( ! C_OWNER.Enabled() ))
			{
				FModified() = true; //|= ( 1 << AIndex );
				return;
			}

//#ifdef _MITOV_PWM_PCA9685_CLOCK_PIN_
			if( C_OWNER.ClockInputPin_o_IsConnected() )
			{
				FModified() = true;
//				FChangedChannels |= ( 1 << AIndex );
				return;
			}
//#endif

			UpdateChannelValue();
		}

/*
		inline void AnalogWrite( float AValue )
		{
			SetChannelValue( AValue );
		}
*/
		inline void SetPinValue( float AValue ) // Used for direct pin access
		{
			SetChannelValue( AValue );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_FModified,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_Reversed,
		typename T_TRANSFORM
	> class PCA9685ServoChannel :
		public T_Enabled,
		public T_FModified,
		public T_InitialValue,
		public T_Reversed
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Reversed )

	protected:
		_V_PROP_( FModified )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AOnValue = *(float *)_Data;
			SetChannelValue( AOnValue );
		}

	public:
		void UpdateValues()
		{
			if( FModified() )
				UpdateChannelValue();

		}

		inline void UpdateChannelValue()
		{
//			uint16_t	off = AValue * 4095 + 0.5;
			float AValue = MitovConstrain( InitialValue().GetValue(), 0.0f, 1.0f );
			if( Reversed() )
				AValue = 1.0 - AValue;

			uint16_t	AOff = AValue * ( 600 - 150 ) + 150.5;

//			Serial.print( "AIndex: " );
//			Serial.println( AIndex );
//			Serial.println( on );
//			Serial.println( off );

			C_OWNER.WriteChannelData( C_INDEX, 0, AOff );

//			FChangedChannels &= ~( 1 << AIndex );
			FModified() = false;
		}

		inline void SetChannelValue( float AOnValue )
		{
			AOnValue = MitovConstrain( AOnValue, 0.0f, 1.0f );
			if( InitialValue() == AOnValue )
				return;

			InitialValue() = AOnValue;
			if( C_OWNER.Sleep() || ( ! C_OWNER.Enabled() ))
			{
				FModified() = true; //|= ( 1 << AIndex );
				return;
			}

//#ifdef _MITOV_PWM_PCA9685_CLOCK_PIN_
			if( C_OWNER.ClockInputPin_o_IsConnected() )
			{
				FModified() = true;
//				FChangedChannels |= ( 1 << AIndex );
				return;
			}
//#endif

			UpdateChannelValue();
		}
/*
		inline void AnalogWrite( float AValue )
		{
			SetChannelValue( AValue );
		}

		inline void SetPinValue( float AValue )
		{
			SetChannelValue( AValue );
		}
*/
	};
//---------------------------------------------------------------------------
	enum TArduinoAnalogPCA9685OutputLogicMode { olmZero, olmOne, olmHighImpedance };
//---------------------------------------------------------------------------
	template <
		typename T_Inverted,
		typename T_Mode
	> class TArduinoAnalogPCA9685OutputLogic :
		public T_Inverted,
		public T_Mode
	{
	public:
		_V_PROP_( Inverted )
		_V_PROP_( Mode )

	};
//---------------------------------------------------------------------------
	namespace PCA9685Const
	{
		const uint8_t	PCA9685_MODE1 = 0x0;
		const uint8_t	PCA9685_MODE2 = 0x1;

		const uint8_t	PCA9685_SUBADR1 = 0x2;
		const uint8_t	PCA9685_SUBADR2 = 0x3;
		const uint8_t	PCA9685_SUBADR3 = 0x4;

		const uint8_t	PCA9685_PRESCALE = 0xFE;

		const uint8_t	LED0_ON_L = 0x6;
		const uint8_t	LED0_ON_H = 0x7;
		const uint8_t	LED0_OFF_L = 0x8;
		const uint8_t	LED0_OFF_H = 0x9;

		const uint8_t	ALLLED_ON_L = 0xFA;
		const uint8_t	ALLLED_ON_H = 0xFB;
		const uint8_t	ALLLED_OFF_L = 0xFC;
		const uint8_t	ALLLED_OFF_H = 0xFD;
	}
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C
	> class PCA9685PWM_Basic
	{
	protected:
		void WriteChannelDataShared( uint8_t AAddress, uint8_t AIndex, uint16_t AOn, uint16_t AOff )
		{
			C_I2C.beginTransmission( AAddress );
			C_I2C.write(PCA9685Const::LED0_ON_L + 4 * AIndex );
			C_I2C.write( AOn );
			C_I2C.write( AOn >> 8 );
			C_I2C.write( AOff );
			C_I2C.write( AOff >> 8 );
			C_I2C.endTransmission();
		}

		void WriteChannelDataSharedOff( uint8_t AAddress, uint8_t AIndex, uint16_t AOff )
		{
			C_I2C.beginTransmission( AAddress );
			C_I2C.write( PCA9685Const::LED0_OFF_L + 4 * AIndex );
			C_I2C.write( AOff );
			C_I2C.write( AOff >> 8 );
			C_I2C.endTransmission();
		}

	protected:
		uint8_t Read8( uint8_t AAddress, uint8_t ARegister ) 
		{
			C_I2C.beginTransmission( AAddress );
			C_I2C.write( ARegister );
			C_I2C.endTransmission();

			C_I2C.requestFrom( AAddress, (uint8_t)1);
			return C_I2C.read();
		}

		void Write8( uint8_t AAddress, uint8_t ARegister, uint8_t AData ) 
		{
//  Serial.print( "addr: " ); Serial.print( addr, HEX );
//  Serial.print( " = " ); Serial.println( d, HEX );
			C_I2C.beginTransmission( AAddress );
			C_I2C.write( ARegister );
			C_I2C.write( AData );
			C_I2C.endTransmission();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_UpdateValues,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_ExternalClock,
		typename T_Frequency,
		typename T_OpenDrain,
		typename T_OscilatorFrequency,
		typename T_OutputLogic,
		typename T_Sleep,
		typename T_UpdateOnAck
	> class PCA9685PWM :
		public PCA9685PWM_Basic<T_I2C, C_I2C>,
		public T_Address,
		public T_Channels_UpdateValues,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_ExternalClock,
		public T_Frequency,
		public T_OpenDrain,
		public T_OscilatorFrequency,
		public T_OutputLogic,
		public T_Sleep,
		public T_UpdateOnAck
	{
		typedef PCA9685PWM_Basic<T_I2C, C_I2C> inherited;
//	public:
//		PCA9685PWMChannel	Channels[ 16 ];

	public:
		_V_PROP_( Address )
		_V_PROP_( OutputLogic )
		_V_PROP_( OscilatorFrequency )
		_V_PROP_( Frequency )
		_V_PROP_( Enabled )
		_V_PROP_( OpenDrain )
		_V_PROP_( Sleep )
		_V_PROP_( UpdateOnAck )
		_V_PROP_( ExternalClock )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

//	protected:
//		uint16_t	FChangedChannels = 0xFFFF;

	public:
		inline void WriteChannelData( uint8_t AIndex, uint16_t AOn, uint16_t AOff )
		{
			inherited::WriteChannelDataShared( uint8_t( Address().GetValue() ), AIndex, AOn, AOff );
		}

		void UpdateFrequency()
		{
			float Afreq = Frequency() * 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
			float prescaleval = OscilatorFrequency();
			prescaleval /= 4096;
			prescaleval /= Afreq;
			prescaleval -= 1;
			uint8_t prescale = floor(prescaleval + 0.5);

			uint8_t oldmode = Read8(PCA9685Const::PCA9685_MODE1);
			uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
			Write8(PCA9685Const::PCA9685_MODE1, newmode); // go to sleep
			Write8(PCA9685Const::PCA9685_PRESCALE, prescale); // set the prescaler
//			Serial.println( prescale );
			Write8(PCA9685Const::PCA9685_MODE1, oldmode);
			delay(1);
			Write8(PCA9685Const::PCA9685_MODE1, oldmode | 0xA0);  //  This sets the MODE1 register to turn on auto increment.
													// This is why the beginTransmission below was not working.
		}

		void UpdateConfig()
		{
			UpdateConfigOnly();
			UpdateAll();
		}

/*
	public:
		void SetChannelValue( int AIndex, float AOnValue ) // Used for direct access
		{
			AOnValue = MitovConstrain( AOnValue, 0.0f, 1.0f );
			if( Channels[ AIndex ].InitialValue == AOnValue )
				return;

			Channels[ AIndex ].InitialValue = AOnValue;
			if( Sleep || ( ! Enabled ))
			{
				FChangedChannels |= ( 1 << AIndex );
				return;
			}

//#ifdef _MITOV_PWM_PCA9685_CLOCK_PIN_
			if( ClockInputPin_o_IsConnected )
			{
				FChangedChannels |= ( 1 << AIndex );
				return;
			}
//#endif

			UpdateChannelValue( AIndex, AOnValue );
		}
*/
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			UpdateAll();
		}

/*
		inline void Channel_InputPin_o_Receive( int AIndex, void *_Data )
		{
			float AOnValue = *(float *)_Data;
			SetChannelValue( AIndex, AOnValue );
		}
*/

	protected:
		void UpdateConfigOnly()
		{
			uint8_t	AValue = 0b100000;

			if( ExternalClock() )
				AValue |= 0b1000000;

			if( Sleep() )
				AValue |= 0b10000;

//			Serial.print( "PCA9685_MODE1: " );
//			Serial.println( AValue, BIN );

			Write8( PCA9685Const::PCA9685_MODE1, AValue );

			AValue = 0;

			if( OutputLogic().Inverted() )
				AValue |= 0b10000;

			if( UpdateOnAck() )
				AValue |= 0b1000;

			if( ! OpenDrain() )
				AValue |= 0b100;

			AValue |= OutputLogic().Mode();

//			Serial.print( "PCA9685_MODE2: " );
//			Serial.println( AValue, BIN );

			Write8( PCA9685Const::PCA9685_MODE2, AValue );
		}

		void UpdateAll()
		{
			if( ! Enabled() )
				return;

			if( Sleep() )
				return;

			T_Channels_UpdateValues::Call();
//			for( int i = 0; i < 16; ++i )
//				if( FChangedChannels & ( 1 << i ) )
//					UpdateChannelValue( i, Channels[ i ].InitialValue );

		}

/*
	void UpdateChannelValue( int AIndex, float AValue )
		{
			uint16_t	on = Channels[ AIndex ].OnValue * 4095 + 0.5;
			uint16_t	off = AValue * 4095 + 0.5;

//			Serial.print( "AIndex: " );
//			Serial.println( AIndex );
//			Serial.println( on );
//			Serial.println( off );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write(PCA9685Const::LED0_ON_L + 4 * AIndex );
			C_I2C.write(on);
			C_I2C.write(on>>8);
			C_I2C.write(off);
			C_I2C.write(off>>8);
			C_I2C.endTransmission();

			FChangedChannels &= ~( 1 << AIndex );
		}
*/
	protected:
		inline uint8_t Read8( uint8_t ARegister )
		{
			return inherited::Read8( uint8_t( Address().GetValue() ), ARegister );
		}

		inline void Write8(uint8_t ARegister, uint8_t AData ) 
		{
			inherited::Write8( uint8_t( Address().GetValue() ), ARegister, AData );
		}

	public:
		inline void SystemStart()
		{
			Write8( PCA9685Const::PCA9685_MODE1, 0x0 );
			UpdateConfigOnly();
			UpdateFrequency();
			UpdateAll();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_UpdateValues,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_ExternalClock,
		typename T_OpenDrain,
		typename T_OscilatorFrequency,
		typename T_OutputLogic,
		typename T_Sleep,
		typename T_UpdateOnAck
	> class PCA9685Servo :
		public PCA9685PWM_Basic<T_I2C, C_I2C>,
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_ExternalClock,
		public T_OpenDrain,
		public T_OscilatorFrequency,
		public T_OutputLogic,
		public T_Sleep,
		public T_UpdateOnAck
	{
//	public:
//		PCA9685ServoChannel	Channels[ 16 ];

		typedef PCA9685PWM_Basic<T_I2C, C_I2C> inherited;

	public:
		_V_PROP_( Address )
		_V_PROP_( OutputLogic )
		_V_PROP_( OscilatorFrequency )
		_V_PROP_( Enabled )
		_V_PROP_( OpenDrain )
		_V_PROP_( Sleep )
		_V_PROP_( UpdateOnAck )
		_V_PROP_( ExternalClock )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

//	protected:
//		uint16_t	FChangedChannels = 0xFFFF;

	public:
		void UpdateFrequency()
		{
			float Afreq = 60; // * 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
			float prescaleval = OscilatorFrequency();
			prescaleval /= 4096;
			prescaleval /= Afreq;
			prescaleval -= 1;
			uint8_t prescale = floor(prescaleval + 0.5);

			uint8_t oldmode = Read8( PCA9685Const::PCA9685_MODE1);
			uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
			Write8( PCA9685Const::PCA9685_MODE1, newmode); // go to sleep
			Write8( PCA9685Const::PCA9685_PRESCALE, prescale); // set the prescaler
//			Serial.println( prescale );
			Write8( PCA9685Const::PCA9685_MODE1, oldmode);
			delay(1);
			Write8( PCA9685Const::PCA9685_MODE1, oldmode | 0xA0);  //  This sets the MODE1 register to turn on auto increment.
													// This is why the beginTransmission below was not working.
		}

		void UpdateConfig()
		{
			UpdateConfigOnly();
			UpdateAll();
		}

	protected:
		inline uint8_t Read8( uint8_t ARegister )
		{
			return inherited::Read8( uint8_t( Address().GetValue() ), ARegister );
		}

		inline void Write8(uint8_t ARegister, uint8_t AData ) 
		{
			inherited::Write8( uint8_t( Address().GetValue() ), ARegister, AData );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			UpdateAll();
//			ProcessData( false, false );
		}

/*
		void Channel_InputPin_o_Receive( int AIndex, void *_Data )
		{
			float AOnValue = *(float *)_Data;
			AOnValue = MitovConstrain( AOnValue, 0.0f, 1.0f );
			if( Channels[ AIndex ].InitialValue == AOnValue )
				return;

			Channels[ AIndex ].InitialValue = AOnValue;
			if( Sleep || ( ! Enabled ))
			{
				FChangedChannels |= ( 1 << AIndex );
				return;
			}

//#ifdef _MITOV_SERVO_PCA9685_CLOCK_PIN_
			if( ClockInputPin_o_IsConnected )
			{
				FChangedChannels |= ( 1 << AIndex );
				return;
			}
//#endif

			UpdateChannelValue( AIndex, AOnValue );
		}
*/
	public:
		inline void WriteChannelData( uint8_t AIndex, uint16_t AOn, uint16_t AOff )
		{
			inherited::WriteChannelDataSharedOff( uint8_t( Address().GetValue() ), AIndex, AOff );
		}

	protected:
		void UpdateConfigOnly()
		{
			uint8_t	AValue = 0b100000;

			if( ExternalClock() )
				AValue |= 0b1000000;

			if( Sleep() )
				AValue |= 0b10000;

//			Serial.print( "PCA9685_MODE1: " );
//			Serial.println( AValue, BIN );

			Write8( PCA9685Const::PCA9685_MODE1, AValue );

			AValue = 0;

			if( OutputLogic().Inverted() )
				AValue |= 0b10000;

			if( UpdateOnAck() )
				AValue |= 0b1000;

			if( ! OpenDrain() )
				AValue |= 0b100;

			AValue |= OutputLogic().Mode();

//			Serial.print( "PCA9685_MODE2: " );
//			Serial.println( AValue, BIN );

			Write8( PCA9685Const::PCA9685_MODE2, AValue );
		}

		void UpdateAll()
		{
			if( ! Enabled() )
				return;

			if( Sleep() )
				return;

			T_Channels_UpdateValues::Call();

//			for( int i = 0; i < 16; ++i )
//				if( FChangedChannels & ( 1 << i ) )
//					UpdateChannelValue( i, Channels[ i ].InitialValue );

		}

/*
		void UpdateChannelValue( int AIndex, float AValue )
		{
//			uint16_t	off = AValue * 4095 + 0.5;
			uint16_t	off = AValue * ( 600 - 150 ) + 150.5;

//			Serial.print( "AIndex: " );
//			Serial.println( AIndex );
//			Serial.println( on );
//			Serial.println( off );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( PCA9685Const::LED0_OFF_L + 4 * AIndex );
			C_I2C.write(off);
			C_I2C.write(off>>8);
			C_I2C.endTransmission();

			FChangedChannels &= ~( 1 << AIndex );
		}
*/
	public:
		inline void SystemStart()
		{
			Write8( PCA9685Const::PCA9685_MODE1, 0x0);
			UpdateConfigOnly();
			UpdateFrequency();
			UpdateAll();
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_INDEX,
		typename T_OWNER, T_OWNER &C_OWNER
	> class PCA9685PWMChannelAccess
	{
	public:
		inline void SetPinValue( float AValue ) // Used for direct pin access
		{
//			Serial.println( AValue );
			C_OWNER.WriteChannelData( C_INDEX, 0, AValue * 4095 + 0.5 );
		}
	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif