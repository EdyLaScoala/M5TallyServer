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
#include "Mitov_RandomGenerator.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
#define Min Min
#define Max Max
//---------------------------------------------------------------------------
    class RandomColorImplementation
    {
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			int AMin = MitovMin( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );
			int AMax = MitovMax( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );

			AInherited->FValue.Red = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );

			AInherited->FValue.Green = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );

			AInherited->FValue.Blue = random( AMin, AMax + 1 );
		}
    };
//---------------------------------------------------------------------------
    class RandomAlphaColorImplementation
    {
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			int AMin = MitovMin( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );
			int AMax = MitovMax( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );

			AInherited->FValue.Red = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );

			AInherited->FValue.Green = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );

			AInherited->FValue.Blue = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Alpha, AInherited->Value_Max().GetReference().Alpha );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Alpha, AInherited->Value_Max().GetReference().Alpha );

			AInherited->FValue.Alpha = random( AMin, AMax + 1 );
		}
    };
//---------------------------------------------------------------------------
	class RandomRGBWColorImplementation
    {
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			int AMin = MitovMin( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );
			int AMax = MitovMax( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );

			AInherited->FValue.Red = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );

			AInherited->FValue.Green = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );

			AInherited->FValue.Blue = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().White, AInherited->Value_Max().GetReference().White );
			AMax = MitovMax( AInherited->Value_Min().GetReference().White, AInherited->Value_Max().GetReference().White );

			AInherited->FValue.White = random( AMin, AMax + 1 );
		}
	};
//---------------------------------------------------------------------------
	class RandomRGBWAlphaColorImplementation
    {
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			int AMin = MitovMin( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );
			int AMax = MitovMax( AInherited->Value_Min().GetReference().Red, AInherited->Value_Max().GetReference().Red );

			AInherited->FValue.Red = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Green, AInherited->Value_Max().GetReference().Green );

			AInherited->FValue.Green = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Blue, AInherited->Value_Max().GetReference().Blue );

			AInherited->FValue.Blue = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().White, AInherited->Value_Max().GetReference().White );
			AMax = MitovMax( AInherited->Value_Min().GetReference().White, AInherited->Value_Max().GetReference().White );

			AInherited->FValue.White = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Value_Min().GetReference().Alpha, AInherited->Value_Max().GetReference().Alpha );
			AMax = MitovMax( AInherited->Value_Min().GetReference().Alpha, AInherited->Value_Max().GetReference().Alpha );

			AInherited->FValue.Alpha = random( AMin, AMax + 1 );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialValue,
		typename T_OutputPin
	> class AnalogToColor :
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PIN_( InitialValue )

	public:
		inline void RedInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Red == AValue )
				return;

			InitialValue().GetReference().Red = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		inline void GreenInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Green == AValue )
				return;

			InitialValue().GetReference().Green = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		inline void BlueInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Blue == AValue )
				return;

			InitialValue().GetReference().Blue = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue(), false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialValue,
		typename T_OutputPin
	> class AnalogToAlphaColor :
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PIN_( InitialValue )

	public:
		inline void RedInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Red == AValue )
				return;

			InitialValue().GetReference().Red = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		inline void GreenInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Green == AValue )
				return;

			InitialValue().GetReference().Green = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		inline void BlueInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Blue == AValue )
				return;

			InitialValue().GetReference().Blue = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		inline void AlphaInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Alpha == AValue )
				return;

			InitialValue().GetReference().Alpha = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue(), false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialValue,
		typename T_OutputPin
	> class AnalogToRGBWColor :
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PIN_( InitialValue )

	public:
		void RedInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Red == AValue )
				return;

			InitialValue().GetReference().Red = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void GreenInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Green == AValue )
				return;

			InitialValue().GetReference().Green = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void BlueInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Blue == AValue )
				return;

			InitialValue().GetReference().Blue = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void WhiteInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().White == AValue )
				return;

			InitialValue().GetReference().White = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue(), false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialValue,
		typename T_OutputPin
	> class AnalogToRGBWAlphaColor :
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PIN_( InitialValue )

	public:
		void RedInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Red == AValue )
				return;

			InitialValue().GetReference().Red = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void GreenInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Green == AValue )
				return;

			InitialValue().GetReference().Green = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void BlueInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Blue == AValue )
				return;

			InitialValue().GetReference().Blue = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void WhiteInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().White == AValue )
				return;

			InitialValue().GetReference().White = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void AlphaInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetReference().Alpha == AValue )
				return;

			InitialValue().GetReference().Alpha = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue(), false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_RedOutputPin
	> class ColorToAnalog : 
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_RedOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;

			T_RedOutputPin::SetPinValue( AColor.Red / 255.0 );
			T_GreenOutputPin::SetPinValue( AColor.Green / 255.0 );
			T_BlueOutputPin::SetPinValue( AColor.Blue / 255.0 );
		}

	};	
//---------------------------------------------------------------------------
	template <
		typename T_AlphaOutputPin,
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_RedOutputPin
	> class AlphaColorToAnalog : 
		public T_AlphaOutputPin,
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_RedOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )
		_V_PIN_( AlphaOutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;

			T_RedOutputPin::SetPinValue( AColor.Red / 255.0 );
			T_GreenOutputPin::SetPinValue( AColor.Green / 255.0 );
			T_BlueOutputPin::SetPinValue( AColor.Blue / 255.0 );
			T_AlphaOutputPin::SetPinValue( AColor.Alpha / 255.0 );
		}

	};	
//---------------------------------------------------------------------------
	template <
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_RedOutputPin,
		typename T_WhiteOutputPin
	> class RGBWColorToAnalog : 
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_RedOutputPin,
		public T_WhiteOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )
		_V_PIN_( WhiteOutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TRGBWColor &AColor = *(TRGBWColor *)_Data;

			T_RedOutputPin::SetPinValue( AColor.Red / 255.0 );
			T_GreenOutputPin::SetPinValue( AColor.Green / 255.0 );
			T_BlueOutputPin::SetPinValue( AColor.Blue / 255.0 );
			T_WhiteOutputPin::SetPinValue( AColor.White / 255.0 );
		}

	};	
//---------------------------------------------------------------------------
	template <
		typename T_AlphaOutputPin,
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_RedOutputPin,
		typename T_WhiteOutputPin
	> class RGBWAlphaColorToAnalog : 
		public T_AlphaOutputPin,
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_RedOutputPin,
		public T_WhiteOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )
		_V_PIN_( WhiteOutputPin )
		_V_PIN_( AlphaOutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TRGBWAlphaColor &AColor = *(TRGBWAlphaColor *)_Data;

			T_RedOutputPin::SetPinValue( AColor.Red / 255.0 );
			T_GreenOutputPin::SetPinValue( AColor.Green / 255.0 );
			T_BlueOutputPin::SetPinValue( AColor.Blue / 255.0 );
			T_WhiteOutputPin::SetPinValue( AColor.White / 255.0 );
			T_AlphaOutputPin::SetPinValue( AColor.Alpha / 255.0 );
		}

	};	
//---------------------------------------------------------------------------
	template <
		typename T_Alpha,
		typename T_InitialValue,
		typename T_OutputPin
	> class ColorToAlphaColor :
		public T_Alpha,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Alpha )
		_V_PROP_( InitialValue )

	public:
		void UpdateAlpha()
		{
			InitialValue().GetReference().Alpha = Alpha().GetValue() * 255 + 0.5;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			InitialValue().GetReference().Red = AColor.Red;
			InitialValue().GetReference().Green = AColor.Green;
			InitialValue().GetReference().Blue = AColor.Blue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
            UpdateAlpha();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_White
	> class ColorToRGBWColor :
		public T_InitialValue,
		public T_OutputPin,
		public T_White
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialValue )
		_V_PROP_( White )

	public:
		void UpdateWhite()
		{
			InitialValue().GetReference().White = White().GetValue() * 255 + 0.5;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			InitialValue().GetReference().Red = AColor.Red;
			InitialValue().GetReference().Green = AColor.Green;
			InitialValue().GetReference().Blue = AColor.Blue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
            UpdateWhite();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Alpha,
		typename T_InitialValue,
		typename T_OutputPin
	> class RGBWColorToRGBWAlphaColor :
		public T_Alpha,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Alpha )
		_V_PROP_( InitialValue )

	public:
		void UpdateWhite()
		{
			InitialValue().GetReference().Alpha = Alpha().GetValue() * 255 + 0.5;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TRGBWColor &AColor = *(TRGBWColor *)_Data;
			InitialValue().GetReference().Red = AColor.Red;
			InitialValue().GetReference().Green = AColor.Green;
			InitialValue().GetReference().Blue = AColor.Blue;
			InitialValue().GetReference().White = AColor.White;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
            UpdateWhite();
		}

	};
//---------------------------------------------------------------------------
#undef Min
#undef Max
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif