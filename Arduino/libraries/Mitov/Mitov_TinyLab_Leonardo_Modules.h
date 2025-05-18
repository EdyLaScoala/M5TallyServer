////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_RotaryEncoderSensor.h>

//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InLux,
		typename T_OutputPin
	> class TArduino_TinyLab_Leonardo_LightSensorModule :
		public T_Enabled,
		public T_InLux,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InLux )

    protected:
		void ReadSensor()
		{
		    if( ! Enabled() )
		        return;

            float AValue;

            if( InLux() )
                AValue = ((2500.0 / (analogRead( 2 ) * ( 5.0 / 1024.0 ))) - 500) / 10.0;

            else
                AValue = float( analogRead( 2 )) / float( VISUINO_ANALOG_IN_RANGE );

            T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void SystemStart()
		{
		    ReadSensor();
		}

		inline void SystemLoopBegin()
		{
		    ReadSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockMode,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin,
		typename T_PressOutputPin,
		typename T_UpOutputPin
	> class TinyLab_Leonardo_RotaryEncoderSensor_Module :
        public RotaryEncoderSensor_Interrupt <
                T_B_PIN, C_B_PIN,
                T_ClockMode,
                T_DebounceInterval,
                T_DownOutputPin,
                T_Enabled,
                T_FCount,
                T_InitialValue,
                T_Value_Max,
                T_Value_Min,
                T_OutputPin,
                T_UpOutputPin
	        >,
		public T_PressOutputPin
	{
	public:
		_V_PIN_( PressOutputPin )

	public:
        inline void SetValue( uint8_t AValue )
        {
             T_PressOutputPin::SetPinValue( ( AValue & 0x001 ) != 0 );
        }

	};

//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_PIN_1,
		typename T_PIN_2,
	    typename T_OutputPins_0,
	    typename T_OutputPins_1,
	    typename T_OutputPins_2,
	    typename T_OutputPins_3
	> class TArduino_TinyLab_Leonardo_ButtonsModule :
		public T_PIN_1,
		public T_PIN_2,
	    public T_OutputPins_0,
	    public T_OutputPins_1,
	    public T_OutputPins_2,
	    public T_OutputPins_3
	{
	public:
		typedef typename T_PIN_1::T_PIN_TYPE T_PIN_TYPE_0;
		typedef typename T_PIN_2::T_PIN_TYPE T_PIN_TYPE_1;

	public:
		static T_PIN_TYPE_0 PinObject0;
		static T_PIN_TYPE_1 PinObject1;

	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )

	protected:
		inline void ReadValues( bool AChangeOnly )
		{
			T_OutputPins_0::SetPinValue( T_PIN_1::ReadValue(), AChangeOnly );
			T_OutputPins_1::SetPinValue( T_PIN_2::ReadValue(), AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValues( false );
		}


	public:
        inline void SetValue( uint8_t AValue )
        {
//             T_OutputPins_2::SetPinValue( ( AValue & 0x100 ) != 0 );
//             T_OutputPins_3::SetPinValue( ( AValue & 0x010 ) != 0 );

             T_OutputPins_2::SetPinValue( ( AValue & 0x010 ) != 0 );
             T_OutputPins_3::SetPinValue( ( AValue & 0x100 ) != 0 );
        }

	public:
		inline void SystemInit()
		{
			T_PIN_1::Init();
			T_PIN_2::Init();
		}

		inline void SystemStart()
		{
			ReadValues( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValues( true );
		}
	};
//---------------------------------------------------------------------------
	template <
        typename T_Modules_SetValue
    > class TArduino_TinyLab_Leonardo_Board
    {
    private:
//			0.0
//	-		0.045455
//		X	0.09091
//	-		0.147505
//	X		0.2041
//	-		0.23345
//	X	X	0.2628
//	-		0.3814
//X			0.5
//	-		0.51
//X		X	0.5238
//	-		0.5403
//X	X		0.5568
//	-		0.5662
//X	X	X	0.5756
		void ReadSensor()
		{
		    float AAnalogValue = float( analogRead( 5 )) / float( VISUINO_ANALOG_IN_RANGE );

            uint8_t AValue;
		    if( AAnalogValue > 0.5662 )
		        AValue = 0b111;

		    else if( AAnalogValue > 0.5403 )
		        AValue = 0b110;

		    else if( AAnalogValue > 0.51 )
		        AValue = 0b101;

		    else if( AAnalogValue > 0.3814 )
		        AValue = 0b100;

		    else if( AAnalogValue > 0.23345 )
		        AValue = 0b011;

		    else if( AAnalogValue > 0.147505 )
		        AValue = 0b010;

		    else if( AAnalogValue > 0.045455 )
		        AValue = 0b001;

		    else
		        AValue = 0b000;

		    T_Modules_SetValue::Call( AValue );

//			0.0
//	-		0.045455 *
//		X	0.09091
//	-		0.147505 *
//	X		0.2041
//	-		0.23345 *
//	X	X	0.2628
//	-		0.3814 *
//X			0.5
//	-		0.51   *
//X		X	0.5238
//	-		0.5403 *
//X	X		0.5568
//	-		0.5662 *
//X	X	X	0.5756
		}

	public:
		inline void SystemStart()
		{
		    if( T_Modules_SetValue::Count() )
		        ReadSensor();
		}

		inline void SystemLoopBegin()
		{
		    if( T_Modules_SetValue::Count() )
    		    ReadSensor();

		}

    };
//---------------------------------------------------------------------------
} // Mitov
