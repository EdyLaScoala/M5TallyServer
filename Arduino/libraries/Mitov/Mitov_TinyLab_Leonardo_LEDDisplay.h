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
#include <Mitov_SoftwareSPI.h>

namespace Mitov
{
//---------------------------------------------------------------------------
namespace TinyLab
{
//---------------------------------------------------------------------------
namespace Types
{
typedef Mitov::VisuinoSoftwareSPI<
  12, // ControlOutputPins0
  10 // ControlOutputPins1
   > DisplaySoftwareSPI;
} // Types

namespace Instances
{
::Mitov::TinyLab::Types::DisplaySoftwareSPI DisplaySoftwareSPI;
} // Instances
//---------------------------------------------------------------------------
} // TinyLab
//---------------------------------------------------------------------------
    template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_Enabled,
		typename T_FModified,
		typename T_Intensity,
		int COUNT_CONTROLLERS
    > class TinyLab_MaximLedController :
        public MaximLedController <
        		T_SPI, C_SPI,
        		T_ChipSelectOutputPin,
        		T_Enabled,
        		T_FModified,
        		T_Intensity,
        		COUNT_CONTROLLERS
            >
    {
        typedef MaximLedController <
        		T_SPI, C_SPI,
        		T_ChipSelectOutputPin,
        		T_Enabled,
        		T_FModified,
        		T_Intensity,
        		COUNT_CONTROLLERS
            > inherited;
    public:
        inline void SystemInit()
        {
            ::Mitov::TinyLab::Instances::DisplaySoftwareSPI.SystemInit();
            inherited::SystemInit();
        }

    };
//---------------------------------------------------------------------------
} // Mitov
