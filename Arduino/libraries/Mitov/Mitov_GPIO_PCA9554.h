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
namespace PCF8575
{
	const uint8_t REG_INPUT		= 0x00;
	const uint8_t REG_OUTPUT	= 0x01;
	const uint8_t REG_POLINV	= 0x02;
	const uint8_t REG_CONFIG	= 0x03;
}
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_GetMode
	> class GPIO_PCA9554 :
		public T_Address
	{
	public:
		_V_PROP_( Address )

	protected:
//		uint8_t	FDirectionBits = 0xFF;
//		uint8_t	FPolarityBits = 0x00;
		uint8_t	FReadBits	= 0x00;
		uint8_t	FWriteBits	= 0x00;

	public:
		void UpdateAll()
		{
			uint8_t ADirection = 0xFF;
			uint8_t AInverted = 0;
			T_Channels_GetMode::Call( ADirection, AInverted );

//			Serial.println( ADirection, BIN );
//			Serial.println( AInverted, BIN );

			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), PCF8575::REG_OUTPUT, ADirection );
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), PCF8575::REG_POLINV, AInverted );
		}

	public:
		inline void PerformRead()
		{
			FReadBits = I2C::ReadByte( C_I2C, uint8_t( Address().GetValue()), PCF8575::REG_INPUT );
		}

	public:
		inline bool GetBitValue( uint8_t AIndex )
		{
			return( ( FReadBits & ( ((uint8_t)1 ) << AIndex )) != 0 );
		}

		void SetChannelValue( int AIndex, bool AValue )
		{
			if( AValue )
				FWriteBits |= ( (uint8_t)1 ) << AIndex;

			else
				FWriteBits &= ~( ((uint8_t)1 ) << AIndex );
		}

	public:
/*
		void SetChannelDirection( int AIndex, bool AIsOutput )
		{
			if( AValue )
				FDirectionBits &= ~( ((uint8_t)1 ) << AIndex );

			else
				FDirectionBits |= ( (uint8_t)1 ) << AIndex;
		}
*/
	public:
		inline void SystemStart()
		{
			UpdateAll();
		}

		inline void SystemLoopBegin()
		{
			PerformRead();
		}

		inline void SystemLoopUpdateHardware()
		{
//			Serial.println( FWriteBits, BIN );
//			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), PCF8575::REG_OUTPUT, FWriteBits );
			I2C::WriteByte( C_I2C, uint8_t( Address().GetValue()), PCF8575::REG_OUTPUT, 0xFF );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_IsCombinedInOut,
		typename T_IsInverted,
		typename T_IsOutput,
		typename T_OutputPin
	> class GPIO_PCA9554_Channel :
		public T_InitialValue,
		public T_IsCombinedInOut,
		public T_IsInverted,
		public T_IsOutput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialValue )
		_V_PROP_( IsInverted )
        _V_PROP_( IsOutput )

	public:
        inline void PinDirectionsInit()
        {
			C_OWNER.UpdateAll();
        }

	public:
		inline void GetMode( uint8_t &ADirection, uint8_t &AInverted )
		{
			if( IsOutput().GetValue() )
				ADirection &= ~( uint8_t( 1 ) << C_INDEX );

			if( IsInverted().GetValue() )
				AInverted |= uint8_t( 1 ) << C_INDEX;
		}

	public:
		inline void SetMode( int AMode ) // Placeholder for direct pin control!
		{
//			pinMode( FPinNumber, AMode );
		}

		inline void SetInitialMode( int AMode ) // Placeholder for direct pin control!
		{
//			SetMode( AMode );
		}

		bool DigitalRead()
		{
//			Serial.println( "TEST1" );
			C_OWNER.PerformRead();
//			Serial.println( FInValue );
			return C_OWNER.GetBitValue( C_INDEX );
		}

		inline void DigitalWrite( bool AValue )
		{
			InitialValue() = AValue;
			C_OWNER.SetChannelValue( C_INDEX, AValue );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			DigitalWrite( *(bool *)_Data );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue() );
		}

		inline void SystemLoopBegin()
		{
			bool AValue = C_OWNER.GetBitValue( C_INDEX );
			T_OutputPin::SetPinValue( AValue );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

