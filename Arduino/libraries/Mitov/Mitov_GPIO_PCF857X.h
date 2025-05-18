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
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_GetCurrentValue,
		typename T_Channels_HasInputs,
		typename T_Channels_SendOutput,
		typename T_FModified
	> class GPIO_PCF8574 :
		public T_Address,
		public T_FModified
	{
	public:
		_V_PROP_( Address )

	public:
		_V_PROP_( FModified )

	public:
		uint16_t PerformDirectRead()
        {
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ) );
			return C_I2C.read();
        }

	protected:
		void UpdateAll()
		{
		    if( ! FModified().GetValue() )
		        return;

		    uint16_t ABits = 0xFFFF;
		    T_Channels_GetCurrentValue::Call( ABits );

			C_I2C.beginTransmission( uint8_t( Address().GetValue()) );
			C_I2C.write( ABits );
			C_I2C.endTransmission();

			FModified() = false;
		}

		void PerformRead()
		{
		    bool AHasInputs = false;
		    T_Channels_HasInputs::Call( AHasInputs );
			if( ! AHasInputs )
				return;

			uint8_t AReadBits = PerformDirectRead();
			T_Channels_SendOutput::Call( AReadBits );
		}

	public:
		inline void SetModified()
		{
			FModified() = true;
		}

	public:
		inline void SystemStart()
		{
		    FModified() = true;
			UpdateAll();
			PerformRead();
		}

		inline void SystemLoopBegin()
		{
			PerformRead();
		}

		inline void SystemLoopUpdateHardware()
		{
			UpdateAll();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_GetCurrentValue,
		typename T_Channels_HasInputs,
		typename T_Channels_SendOutput,
		typename T_FModified
	> class GPIO_PCF8575 :
		public T_Address,
		public T_FModified
	{
	public:
		_V_PROP_( Address )

	public:
		_V_PROP_( FModified )

	public:
		uint16_t PerformDirectRead()
        {
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 2 ) );
			uint16_t AReadBits = C_I2C.read();
			AReadBits |= uint16_t( C_I2C.read() ) << 8;

            return AReadBits;
        }

	protected:
		void UpdateAll()
		{
		    if( ! FModified().GetValue() )
		        return;

		    uint16_t ABits = 0xFFFF;
		    T_Channels_GetCurrentValue::Call( ABits );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( uint8_t( ABits ));
			C_I2C.write( uint8_t( ABits >> 8 ));
			C_I2C.endTransmission();

			FModified() = false;
		}

		void PerformRead()
		{
		    bool AHasInputs = false;
		    T_Channels_HasInputs::Call( AHasInputs );
			if( ! AHasInputs )
				return;

			uint8_t AReadBits = PerformDirectRead();
			T_Channels_SendOutput::Call( AReadBits );
		}

	public:
		inline void SetModified()
		{
			FModified() = true;
		}

	public:
		inline void SystemStart()
		{
		    FModified() = true;
			UpdateAll();
			PerformRead();
		}

		inline void SystemLoopBegin()
		{
			PerformRead();
		}

		inline void SystemLoopUpdateHardware()
		{
			UpdateAll();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_IsCombinedInOut,
		typename T_IsOutput,
		typename T_OutputPin
	> class GPIO_PCF857X_Channel :
		public T_InitialValue,
		public T_IsCombinedInOut,
		public T_IsOutput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialValue )
        _V_PROP_( IsOutput )

      protected:
        _V_PROP_( IsCombinedInOut )

	public:
		inline void SetMode( int AMode ) // Placeholder for direct pin control!
		{
		}

		inline void SetInitialMode( int AMode ) // Placeholder for direct pin control!
		{
		}

		bool DigitalRead()
		{
			uint16_t AValue = C_OWNER.PerformDirectRead();
			return (( AValue & ( 1 << C_INDEX )) != 0 );
		}

		inline void DigitalWrite( bool AValue )
		{
			if( InitialValue() == AValue )
			    return;

			InitialValue() = AValue;
			C_OWNER.SetModified();
		}

	public:
	    inline void HasInputs( bool &AValue )
	    {
	        if( AValue )
	            return;

	        if( T_OutputPin::GetPinIsConnected() )
	            AValue = true;

	    }

		inline void GetCurrentValue( uint16_t &AValue )
		{
		    if( ( ! T_OutputPin::GetPinIsConnected()) || IsCombinedInOut() || IsOutput() )
		        if( ! InitialValue().GetValue() )
	                AValue &= ~( 1 << C_INDEX );

		}

		inline void SendOutput( uint16_t AValue )
		{
			T_OutputPin::SetPinValue( ( AValue & ( 1 << C_INDEX )) != 0 );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			DigitalWrite( *(bool *)_Data );
		}

	public:
		inline void SystemStart()
		{
		}

		inline void SystemLoopBegin()
		{
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif