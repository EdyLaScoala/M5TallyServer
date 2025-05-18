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
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_InitialValue,
		typename T_Inverted,
		typename T_RedOutputPin
	> class RGBLed :
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_InitialValue,
		public T_Inverted,
		public T_RedOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )

	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Inverted )

	protected:
		inline float Convert( float AValue )
		{
			if( Inverted() )
				return 1 - AValue;

			return AValue;
		}

		void UpdateColor()
		{
			T_RedOutputPin::SetPinValue( Convert( InitialValue().GetReference().Red / 255.0 ));
			T_GreenOutputPin::SetPinValue( Convert( InitialValue().GetReference().Green / 255.0 ));
			T_BlueOutputPin::SetPinValue( Convert( InitialValue().GetReference().Blue / 255.0 ));
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_InitialValue,
		typename T_Inverted,
		typename T_RedOutputPin,
		typename T_WhiteOutputPin
	> class RGBWLed :
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_InitialValue,
		public T_Inverted,
		public T_RedOutputPin,
		public T_WhiteOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )

	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Inverted )

	protected:
		inline float Convert( float AValue )
		{
			if( Inverted() )
				return 1 - AValue;

			return AValue;
		}

	protected:
		void UpdateColor()
		{
			T_RedOutputPin::SetPinValue( Convert( InitialValue().GetReference().Red / 255.0 ));
			T_GreenOutputPin::SetPinValue( Convert( InitialValue().GetReference().Green / 255.0 ));
			T_BlueOutputPin::SetPinValue( Convert( InitialValue().GetReference().Blue / 255.0 ));
			T_WhiteOutputPin::SetPinValue( Convert( InitialValue().GetReference().White / 255.0 ));
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TRGBWColor &AColor = *(TRGBWColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}
	};
//---------------------------------------------------------------------------
	template <
		uint8_t	C_BLUE_PIN,
		uint8_t	C_GREEN_PIN,
		uint8_t	C_RED_PIN,
		typename T_InitialValue
	> class Arduino_SingleRGBLedModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
			Analog.Write( C_RED_PIN, InitialValue().GetReference().Red / 255.0 );
			Analog.Write( C_GREEN_PIN, InitialValue().GetReference().Green / 255.0 );
			Analog.Write( C_BLUE_PIN, InitialValue().GetReference().Blue / 255.0 );
//			T_RedOutputPin::SetPinValue( Convert( InitialValue().GetReference().Red / 255.0 ));
//			T_GreenOutputPin::SetPinValue( Convert( InitialValue().GetReference().Green / 255.0 ));
//			T_BlueOutputPin::SetPinValue( Convert( InitialValue().GetReference().Blue / 255.0 ));
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_BLUE_PIN, OUTPUT );
		}

		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}
	};
//---------------------------------------------------------------------------
	template <
		uint8_t	C_BLUE_PIN,
		uint8_t	C_GREEN_PIN,
		uint8_t	C_RED_PIN,
		typename T_InitialValue
	> class Arduino_SingleInvertedRGBLedModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
			Analog.Write( C_RED_PIN, float( 255 - InitialValue().GetReference().Red ) / 255.0 );
			Analog.Write( C_GREEN_PIN, float( 255 - InitialValue().GetReference().Green ) / 255.0 );
			Analog.Write( C_BLUE_PIN, float( 255 - InitialValue().GetReference().Blue ) / 255.0 );
//			T_RedOutputPin::SetPinValue( Convert( InitialValue().GetReference().Red / 255.0 ));
//			T_GreenOutputPin::SetPinValue( Convert( InitialValue().GetReference().Green / 255.0 ));
//			T_BlueOutputPin::SetPinValue( Convert( InitialValue().GetReference().Blue / 255.0 ));
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_BLUE_PIN, OUTPUT );
		}

		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif