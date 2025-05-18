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
		typename T_InitialValue,
		typename T_InvertedOutputPin,
		typename T_OutputPin
	> class SRFlipFlop : 
		public T_InitialValue,
		public T_InvertedOutputPin,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( InvertedOutputPin )

	public:
		_V_PROP_( InitialValue )

	protected:
		inline void SetValue( bool AValue )
		{
			InitialValue() = AValue;
			SendOutput();
		}

		inline void SendOutput()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue() );
			T_InvertedOutputPin::SetPinValue( ! InitialValue().GetValue() );
		}

	public:
		inline void SetInputPin_o_Receive( void *_Data )
		{
			SetValue( true );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			SetValue( false );
		}

	public:
		inline void SystemStart()
		{
			SendOutput();
		}

/*
	public:
		SRFlipFlop() :
			InitialValue( false ),
			FToggleValue( true ),
			FData( false )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_FToggleValue,
		typename T_InitialValue,
		typename T_InvertedOutputPin,
		typename T_OutputPin
	> class TFlipFlop : 
		public Mitov::SRFlipFlop<
			T_InitialValue,
			T_InvertedOutputPin,
			T_OutputPin
		>,
		public T_FToggleValue
	{
		typedef Mitov::SRFlipFlop<
				T_InitialValue,
				T_InvertedOutputPin,
				T_OutputPin
			> inherited;

	protected:
		_V_PROP_( FToggleValue )

	public:
		inline void ToggleInputPin_o_Receive( void *_Data )
		{
			FToggleValue() = *(bool *)_Data;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( FToggleValue() )
				inherited::SetValue( ! inherited::InitialValue().GetValue() );
		}

	public:
		TFlipFlop()
		{
			FToggleValue() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FData,
		typename T_InitialValue,
		typename T_InvertedOutputPin,
		typename T_OutputPin
	> class DFlipFlop :
		public Mitov::SRFlipFlop<
			T_InitialValue,
			T_InvertedOutputPin,
			T_OutputPin
		>,
		public T_FData
	{
		typedef Mitov::SRFlipFlop<
				T_InitialValue,
				T_InvertedOutputPin,
				T_OutputPin
			> inherited;

	protected:
		_V_PROP_( FData )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			inherited::SetValue( FData() );
		}

		inline void DataInputPin_o_Receive( void *_Data )
		{
			FData() = *(bool *)_Data;
		}

	public:
		DFlipFlop()
		{
			FData() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FJ,
		typename T_FK,
		typename T_InitialValue,
		typename T_InvertedOutputPin,
		typename T_OutputPin
	> class JKFlipFlop :
		public T_FJ,
		public T_FK,
		public T_InitialValue,
		public T_InvertedOutputPin,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( InvertedOutputPin )

	public:
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( FJ )
		_V_PROP_( FK )

	public:
		inline void JInputPin_o_Receive( void *_Data )
		{
			FJ() = *(bool *)_Data;
		}

		inline void KInputPin_o_Receive( void *_Data )
		{
			FK() = *(bool *)_Data;
		}

		void ClockInputPin_o_Receive( void *_Data )
		{
			if( FJ() && FK() )
				SetValue( ! InitialValue().GetValue() );

			else if( FJ() )
				SetValue( true );

			else if( FK() )
				SetValue( false );

		}

	protected:
		inline void SetValue( bool AValue )
		{
			InitialValue() = AValue;
			SendOutput();
		}

		inline void SendOutput()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue() );
			T_InvertedOutputPin::SetPinValue( ! InitialValue().GetValue() );
		}

	public:
		inline void SystemStart()
		{
			SendOutput();
		}

	public:
		JKFlipFlop()
		{
			FJ() = false;
			FK() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif