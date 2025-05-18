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
	template <
		typename T,
		typename T_Enabled,
		typename T_FIndex,
		typename T_Mean,
		typename T_OutputPin,
		typename T_Seed,
		typename T_StandardDeviation
	> class CommonGaussGenerator :
		public T_Enabled,
		public T_FIndex,
		public T_Mean,
		public T_OutputPin,
		public T_Seed,
		public T_StandardDeviation
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public: // Needs to be public due to compiler bug :-(
		T	FValue = T( 0 );

	public:
		_V_PIN_( Mean )
		_V_PIN_( StandardDeviation )
		_V_PIN_( Seed )

	protected:
		_V_PIN_( FIndex )
		T FRandoms[ 2 ];

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
				if( ! FIndex() )
				{
					float W;
					float X1;
					float X2;
					do
					{
						X1 = random( -1147483648, 1147483647 ) / 1147483647.0;
						X2 = random( -1147483648, 1147483647 ) / 1147483647.0;
						W = X1 * X1 + X2 * X2;
					}
					while(  W >= 1.0 );

					W = StandardDeviation() * sqrt( (-2.0 * log( W ) ) / W );
/*
					Serial.println( W );
					Serial.println( Mean );
					Serial.println( X1 );
					Serial.print( "RESULT1: " ); Serial.println( Mean + X1 * W );
					Serial.println( X2 );
					Serial.print( "RESULT2: " ); Serial.println( Mean + X2 * W );
*/
					FRandoms[ 0 ] = Mean() + X1 * W;
					FRandoms[ 1 ] = Mean() + X2 * W;
				}

				FValue = FRandoms[ FIndex() ];
				FIndex() = FIndex() ^ 1;
			}

			T_OutputPin::SetPinValue( FValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	public:
		inline CommonGaussGenerator()
		{
			FIndex() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif