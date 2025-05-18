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
#include "Mitov_BasicGenerator.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	class TriangleAnalogGenerator
	{
	public:
		// -1 - 1
//		float	Asymmetry = 0.0f;

	protected:
		template <typename T> typename T::T_DATA CalculateValue( T *AInherited )
		{
			float AAssymetryPount = 0.5 + AInherited->Asymmetry() / 2;
			float APhase = V_FMOD( ( AInherited->FPhase + AInherited->Phase() ), 1 );
			if( APhase < AAssymetryPount )
				return AInherited->Offset() - AInherited->Amplitude() + AInherited->Amplitude() * 2 * ( APhase / AAssymetryPount );

			return AInherited->Offset() - AInherited->Amplitude() + AInherited->Amplitude() * 2 * ( 1 - (( APhase - AAssymetryPount ) / ( 1 - AAssymetryPount )));
		}

	};
//---------------------------------------------------------------------------
	class TriangleIntegerGenerator
	{
	public:
		// -1 - 1
//		float	Asymmetry = 0.0f;

	protected:
		template <typename T> typename T::T_DATA CalculateValue( T *AInherited )
		{
			float AAssymetryPount = 0.5 + AInherited->Asymmetry() / 2;
			float APhase = V_FMOD( ( AInherited->FPhase + AInherited->Phase() ), 1 );
			if( APhase < AAssymetryPount )
				return ( AInherited->Offset() - AInherited->Amplitude() + float( AInherited->Amplitude() ) * 2 * ( APhase / AAssymetryPount ) ) + 0.5;

			return ( AInherited->Offset() - AInherited->Amplitude() + float( AInherited->Amplitude() ) * 2 * ( 1 - (( APhase - AAssymetryPount ) / ( 1 - AAssymetryPount ))) ) + 0.5;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif