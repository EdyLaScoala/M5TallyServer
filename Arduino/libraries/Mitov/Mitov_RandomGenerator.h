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
#define Min Min
#define Max Max
	template <
		typename T, 
		typename T_BASE,
		typename T_Enabled,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin,
		typename T_Seed
	> class CommonRandomGenerator : 
		public T_BASE,
		public T_Enabled,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin,
		public T_Seed
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public: // Needs to be public due to compiler bug :-(
		T	FValue = T( 0 );

	public:
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )
		_V_PROP_( Seed )

	public:
		inline void SystemStartInit()
		{
			randomSeed( Seed() );
		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
			Generate( false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
            if( Enabled() )
            {
				if( Value_Min().GetValue() == Value_Max().GetValue() )
					FValue = Value_Min();

				else
					T_BASE::GenerateValue( this );
			}

			T_OutputPin::SetPinValue( FValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	};
//---------------------------------------------------------------------------
	class RandomAnalogGenerator
	{
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			float AMin = MitovMin( AInherited->Value_Min().GetValue(), AInherited->Value_Max().GetValue() );
			float AMax = MitovMax( AInherited->Value_Min().GetValue(), AInherited->Value_Max().GetValue() );
//			double ARandom = random( -2147483648, 2147483647 );
//			double ARandom = random( -1147483648, 1147483647 );
#ifdef VISUINO_ESP32
			double ARandom = esp_random();
			AInherited->FValue = AMin + ( ARandom / double( 0xFFFFFFFF ) ) * (AMax - AMin);
#else
			double ARandom = random( -1147483648, 1147483647 );
//			FValue = ARandom;
			ARandom += 1147483648;
			AInherited->FValue = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
#endif
		}

	};
//---------------------------------------------------------------------------
	template <typename T_DATA> class RandomIntegerGenerator
	{
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			T_DATA AMin = MitovMin( AInherited->Value_Min().GetValue(), AInherited->Value_Max().GetValue() );
			T_DATA AMax = MitovMax( AInherited->Value_Min().GetValue(), AInherited->Value_Max().GetValue() );
			AInherited->FValue = random( AMin, AMax + 1 );
		}

	};
//---------------------------------------------------------------------------
	class RandomComplexImplementation
	{
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			float AMin = MitovMin( AInherited->Value_Min().GetValue().Real, AInherited->Value_Max().GetValue().Real );
			float AMax = MitovMax( AInherited->Value_Min().GetValue().Real, AInherited->Value_Max().GetValue().Real );

			double ARandom = random( -1147483648, 1147483647 );
			ARandom += 1147483648;
			AInherited->FValue.Real = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);

			AMin = MitovMin( AInherited->Value_Min().GetValue().Imaginary, AInherited->Value_Max().GetValue().Imaginary );
			AMax = MitovMax( AInherited->Value_Min().GetValue().Imaginary, AInherited->Value_Max().GetValue().Imaginary );

			ARandom = random( -1147483648, 1147483647 );
			ARandom += 1147483648;
			AInherited->FValue.Imaginary = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FValue,
		typename T_OutputPin,
		typename T_Seed
	>class RandomDigitalGenerator :
		public T_Enabled,
		public T_FValue,
		public T_OutputPin,
		public T_Seed
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FValue )

	public:
		_V_PROP_( Seed )

	public:
		inline void SystemStartInit()
		{
			randomSeed( Seed() );
		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
			Generate( false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	protected:
		inline void GenerateValue()
		{
			FValue() = random( 0, 2 );
		}

		inline void Generate( bool AChangeOnly )
		{
            if( Enabled() )
				GenerateValue();

			T_OutputPin::SetPinValue( FValue(), AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	public:
		RandomDigitalGenerator()
		{
			FValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_MaxFrequency,
		typename T_MinFrequency,
		typename T_OutputPin,
		typename T_Seed
	> class RandomClockGenerator :
		public T_Enabled,
		public T_MaxFrequency,
		public T_MinFrequency,
		public T_OutputPin,
		public T_Seed
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		unsigned long FLastTime = 0;
		unsigned long FPeriod = 0;

	public:
		_V_PROP_( Seed )

		_V_PROP_( Enabled )
		_V_PROP_( MinFrequency )
		_V_PROP_( MaxFrequency )

	protected:
		void GeneratePeriod()
		{
			float AMin = MitovMin( MinFrequency().GetValue(), MaxFrequency().GetValue() );
			float AMax = MitovMax( MinFrequency().GetValue(), MaxFrequency().GetValue() );
			double ARandom = random( -1147483648, 1147483647 );
			ARandom += 1147483648;
			ARandom = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
			ARandom = MitovMax( (double)AMin, MitovMin( (double)AMax, (double)ARandom ));

			if( ARandom == 0 )
				FPeriod = 1000000;

			else
				FPeriod = ( (( 1.0 / ARandom ) * 1000000 ) + 0.5 );

		}

	public:
		inline void SystemLoopBegin()
		{
			while( micros() - FLastTime >= FPeriod )
			{
				if( Enabled() )
					T_OutputPin::ClockPin();

				FLastTime += FPeriod;
				GeneratePeriod();
			}
		}

		inline void SystemStart()
		{
			GeneratePeriod();
			FLastTime = micros();
		}

	};
//---------------------------------------------------------------------------
	class RandomDateTimeGenerator
	{
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			int32_t ARandomDate = random( AInherited->Value_Min().GetValue().Date, AInherited->Value_Max().GetValue().Date );
			int32_t ARandomTime;
			if( ARandomDate == AInherited->Value_Min().GetValue().Date )
				ARandomTime = random( AInherited->Value_Min().GetValue().Time, MSecsPerDay );

			else if( ARandomDate == AInherited->Value_Max().GetValue().Date )
				ARandomTime = random( 0, AInherited->Value_Max().GetValue().Time );

			else
				ARandomTime = random( 0, MSecsPerDay );

			AInherited->FValue.Date = ARandomDate;
			AInherited->FValue.Time = ARandomTime;
/*
			float AMin = MitovMin( Min.Value, Max.Value );
			float AMax = MitovMax( Min.Value, Max.Value );
//			double ARandom = random( -2147483648, 2147483647 );
			double ARandom = random( -1147483648, 1147483647 );
//			FValue = ARandom;
			ARandom += 1147483648;
			FValue.Value = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
*/
		}

	};
//---------------------------------------------------------------------------
	class RandomQuaternionImplementation
	{
	protected:
		template <typename T> void GenerateValue( T *AInherited )
		{
			for( int i =0; i < 4; ++i )
			{
				float AMin = MitovMin( AInherited->Value_Min().GetValue().Values[ i ], AInherited->Value_Max().GetValue().Values[ i ] );
				float AMax = MitovMax( AInherited->Value_Min().GetValue().Values[ i ], AInherited->Value_Max().GetValue().Values[ i ] );

				double ARandom = random( -1147483648, 1147483647 );
				ARandom += 1147483648;
				AInherited->FValue.Values[ i ] = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
			}

		}
	};
//---------------------------------------------------------------------------
#undef Min
#undef Max
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif