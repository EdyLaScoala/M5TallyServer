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
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPins_Primary,
		typename T_OutputPins_Secondary
	> class GroveDigitalInput :
		public T_Enabled,
		public T_OutputPins_Primary,
		public T_OutputPins_Secondary
	{
	public:
		_V_PIN_( OutputPins_Primary )
		_V_PIN_( OutputPins_Secondary )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			T_OutputPins_Primary::SetPinValue( digitalRead( C_1_PIN_NUMBER ) != LOW, AChangeOnly );
			T_OutputPins_Secondary::SetPinValue( digitalRead( C_0_PIN_NUMBER ) != LOW, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_0_PIN_NUMBER, INPUT );
			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPins_Primary,
		typename T_OutputPins_Secondary
	> class GroveDigitalInputInverted :
		public T_Enabled,
		public T_OutputPins_Primary,
		public T_OutputPins_Secondary
	{
	public:
		_V_PIN_( OutputPins_Primary )
		_V_PIN_( OutputPins_Secondary )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			T_OutputPins_Primary::SetPinValue( digitalRead( C_1_PIN_NUMBER ) != HIGH, AChangeOnly );
			T_OutputPins_Secondary::SetPinValue( digitalRead( C_0_PIN_NUMBER ) != HIGH, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_0_PIN_NUMBER, INPUT );
			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPins_Primary,
		typename T_OutputPins_Secondary
	> class GroveAnalogInput :
		public T_Enabled,
		public T_OutputPins_Primary,
		public T_OutputPins_Secondary
	{
	public:
		_V_PIN_( OutputPins_Primary )
		_V_PIN_( OutputPins_Secondary )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

#ifdef VISUINO_TEENSY
//			analogReadRes( FResolution );
			T_OutputPins_Primary::SetPinValue( analogRead( C_1_PIN_NUMBER ) / float( Mitov::GAnalogInDivider ), AChangeOnly );
			T_OutputPins_Secondary::SetPinValue( analogRead( C_0_PIN_NUMBER ) / float( Mitov::GAnalogInDivider ), AChangeOnly );
#else // VISUINO_TEENSY
			T_OutputPins_Primary::SetPinValue( analogRead( C_1_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE ), AChangeOnly );
			T_OutputPins_Secondary::SetPinValue( analogRead( C_0_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE ), AChangeOnly );
#endif // VISUINO_TEENSY
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
//			pinMode( C_0_PIN_NUMBER, INPUT );
//			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveAnalogSingleInput :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

#ifdef VISUINO_TEENSY
//			analogReadRes( FResolution );
			T_OutputPin::SetPinValue( analogRead( C_0_PIN_NUMBER ) / float( Mitov::GAnalogInDivider ), AChangeOnly );
#else // VISUINO_TEENSY
			T_OutputPin::SetPinValue( analogRead( C_0_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE ), AChangeOnly );
#endif // VISUINO_TEENSY
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
//			pinMode( C_0_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveAnalogSingleInputInverted :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

#ifdef VISUINO_TEENSY
//			analogReadRes( FResolution );
			T_OutputPin::SetPinValue( 1.0f - ( analogRead( C_0_PIN_NUMBER ) / float( Mitov::GAnalogInDivider )), AChangeOnly );
#else // VISUINO_TEENSY
			T_OutputPin::SetPinValue( 1.0f - ( analogRead( C_0_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE )), AChangeOnly );
#endif // VISUINO_TEENSY
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
//			pinMode( C_0_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveAnalogSingleSecondInput :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

#ifdef VISUINO_TEENSY
//			analogReadRes( FResolution );
			T_OutputPin::SetPinValue( analogRead( C_1_PIN_NUMBER ) / float( Mitov::GAnalogInDivider ), AChangeOnly );
#else // VISUINO_TEENSY
			T_OutputPin::SetPinValue( analogRead( C_1_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE ), AChangeOnly );
#endif // VISUINO_TEENSY
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
//			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveAnalogSingleSecondInputInverted :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

#ifdef VISUINO_TEENSY
//			analogReadRes( FResolution );
			T_OutputPin::SetPinValue( 1.0f - ( analogRead( C_1_PIN_NUMBER ) / float( Mitov::GAnalogInDivider )), AChangeOnly );
#else // VISUINO_TEENSY
			T_OutputPin::SetPinValue( 1.0f - ( analogRead( C_1_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE )), AChangeOnly );
#endif // VISUINO_TEENSY
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
//			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveDigitalSingleInput :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			T_OutputPin::SetPinValue( digitalRead( C_0_PIN_NUMBER ) != LOW, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_0_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveDigitalSingleInputInverted :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			T_OutputPin::SetPinValue( digitalRead( C_0_PIN_NUMBER ) == LOW, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_0_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveDigitalSingleSecondInput :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			T_OutputPin::SetPinValue( digitalRead( C_1_PIN_NUMBER ) != LOW, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_OutputPin
	> class GroveDigitalSingleSecondInputInverted :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			T_OutputPin::SetPinValue( digitalRead( C_1_PIN_NUMBER ) == LOW, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_AnalogOutputPin,
		typename T_DigitalOutputPin,
		typename T_Enabled
	> class GroveDigitalAndAnalogInputInverted :
		public T_AnalogOutputPin,
		public T_DigitalOutputPin,
		public T_Enabled
	{
	public:
		_V_PIN_( AnalogOutputPin )
		_V_PIN_( DigitalOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			T_DigitalOutputPin::SetPinValue( digitalRead( C_0_PIN_NUMBER ) == LOW, AChangeOnly );
#ifdef VISUINO_TEENSY
//			analogReadRes( FResolution );
			T_AnalogOutputPin::SetPinValue( 1.0f - ( analogRead( C_1_PIN_NUMBER ) / float( Mitov::GAnalogInDivider )), AChangeOnly );
#else // VISUINO_TEENSY
			T_AnalogOutputPin::SetPinValue( 1.0f - ( analogRead( C_1_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE )), AChangeOnly );
#endif // VISUINO_TEENSY
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_1_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue
	> class GroveDigitalOutputChannel :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			digitalWrite( T_OWNER::GetPin( C_INDEX ), *(bool *)_Data );
		}

	public:
		inline void SystemStart()
		{
			pinMode( T_OWNER::GetPin( C_INDEX ), OUTPUT );
			digitalWrite( T_OWNER::GetPin( C_INDEX ), InitialValue().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER
//		typename T_Enabled
	> class GroveDigitalOutput
//		public T_Enabled
	{
//	public:
//		_V_PROP_( Enabled )

	public:
		inline static uint8_t GetPin( const uint8_t AIndex )
		{
			if( AIndex )
				return C_0_PIN_NUMBER;

			return C_1_PIN_NUMBER;
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_InitialValue
	> class GroveDigitalSingleOutput :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			digitalWrite( C_1_PIN_NUMBER, *(bool *)_Data );
		}

	public:
		inline void SystemStart()
		{
			pinMode( C_1_PIN_NUMBER, OUTPUT );
			digitalWrite( C_1_PIN_NUMBER, InitialValue().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_InitialValue
	> class GroveSecondaryDigitalSingleOutput :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			digitalWrite( C_0_PIN_NUMBER, *(bool *)_Data );
		}

	public:
		inline void SystemStart()
		{
			pinMode( C_0_PIN_NUMBER, OUTPUT );
			digitalWrite( C_0_PIN_NUMBER, InitialValue().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_0_PIN_NUMBER,
		uint8_t C_1_PIN_NUMBER,
		typename T_Enabled,
		typename T_InitialValue
	> class GroveAnalogSingleOutput :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		void UpdateValue()
		{
			if( Enabled() )
				Analog.Write( C_0_PIN_NUMBER, InitialValue().GetValue() );

			else
				Analog.WriteRaw( C_0_PIN_NUMBER, 0 );

		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = MitovConstrain<float>( *(float *)_Data, 0.0f, 1.0f );
			if( AValue == InitialValue().GetValue() )
				return;

			InitialValue() = AValue;
			UpdateValue();
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_0_PIN_NUMBER, OUTPUT );
		}

		inline void SystemStart()
		{
			UpdateValue();
//			digitalWrite( C_0_PIN_NUMBER, true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

