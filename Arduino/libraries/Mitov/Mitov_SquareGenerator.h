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
	class TypedSquareGenerator
	{
//	public:
		// -1 - 1
//		_V_PROP_( Asymmetry )
//		float	Asymmetry = 0.0f;

	protected:
		template <typename T> typename T::T_DATA CalculateValue( T *AInherited )
		{
			if( V_FMOD( ( AInherited->FPhase + AInherited->Phase() ), 1 ) < 0.5 + AInherited->Asymmetry() / 2 )
				return AInherited->Offset() - AInherited->Amplitude();

			return AInherited->Offset() + AInherited->Amplitude();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif