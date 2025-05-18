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
#include <Mitov_GPIO_PCF857X.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_OutputPin
	> class BedacDIO_I2C_InputChannel :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		
	public:
	    inline void SendOutput( uint8_t AValue )
	    {
	        T_OutputPin::SetPinValue( ( C_INDEX & ( 1 << C_INDEX )) != 0 );	        
	    }

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue
	> class BedacDIO_I2C_OutputChannel :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			uint8_t AValue = *(bool *)_Data;
			if( InitialValue() == AValue )
			    return;
			    
			InitialValue() = AValue;
			
			C_OWNER.SetModified();
		}

	public:
	    inline void GetInput( uint8_t & AValue )
	    {
	        if( ! InitialValue().GetValue() )
	            AValue &= ~( 1 << C_INDEX );

	    }

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		uint8_t C_COUNT_Inputs,
		typename T_Channels_GetInput,
		typename T_Channels_SendOutput,
		typename T_FModified
	> class BedacDIO_I2C :
		public T_Address,
		public T_FModified
	{
	public:
		_V_PROP_( Address )

    protected:
		_V_PROP_( FModified )

    protected:
		void PerformRead()
		{
		    if( ! C_COUNT_Inputs )
		        return;
		        
			C_I2C.requestFrom( uint8_t( Address().GetValue()), uint8_t( 1 ) );
			uint8_t AReadBits = C_I2C.read();
			T_Channels_SendOutput::Call( AReadBits );
		}
			
		void UpdateAll()
		{
		    if( ! FModified().GetValue() )
		        return;
		        
		    uint8_t ABits = 0xFF;
		    T_Channels_GetInput::Call( ABits );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( ABits );
			C_I2C.endTransmission();		    
			
			FModified() = false;
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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
