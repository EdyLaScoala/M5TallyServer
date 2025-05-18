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
            typename T_IMPLEMENTATION,
            typename T_IMPLEMENTATION_ORIENTATION,
            typename T_IMPLEMENTATION_TYPE,
            typename T_BackgroundColor,
            typename T_Brightness,
            typename T_Elements_Render,
            typename T_Enabled,
            typename T_FCursorX,
            typename T_FCursorY,
            typename T_Inverted,
            typename T_Orientation,
            typename T_OverridePixelOffset,
            typename T_ResetOutputPin,
            typename T_Text
    	> class DisplayST7735Basic_M5Stack_StickC : 
		public DisplayST7735Basic <
            T_IMPLEMENTATION,
            T_IMPLEMENTATION_ORIENTATION,
            T_IMPLEMENTATION_TYPE,
            T_BackgroundColor,
            T_Elements_Render,
            T_FCursorX,
            T_FCursorY,
            T_Inverted,
            T_Orientation,
            T_OverridePixelOffset,
            T_ResetOutputPin,
            T_Text
		>,
		public T_Brightness,
		public T_Enabled
	{
		typedef DisplayST7735Basic <
            T_IMPLEMENTATION,
            T_IMPLEMENTATION_ORIENTATION,
            T_IMPLEMENTATION_TYPE,
            T_BackgroundColor,
            T_Elements_Render,
            T_FCursorX,
            T_FCursorY,
            T_Inverted,
            T_Orientation,
            T_OverridePixelOffset,
            T_ResetOutputPin,
            T_Text
		> inherited;

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Brightness )

	public:
		void UpdateEnabled()
		{
			Wire1.beginTransmission(0x34);
			Wire1.write(0x12);  
			if( Enabled() )
				Wire1.write(0x4d); //Enable DC-DC1, OLED_VDD, 5B V_EXT

			else
				Wire1.write( 0x01 );

			Wire1.endTransmission();
		}

		void UpdateBrightness()
		{
			uint8_t AValue = Brightness().GetValue() * 12 + 0.5;
			if (AValue > 12)
				AValue = 12;
    
			Wire1.beginTransmission(0x34);
			Wire1.write(0x28);  //LDO2andLDO3 setting
			Wire1.endTransmission();
			Wire1.requestFrom(0x34, 1);
			uint8_t buf = Wire1.read(); //read previous setting 

			Wire1.beginTransmission(0x34);
			Wire1.write(0x28);  
			Wire1.write(((buf & 0x0f) | (AValue << 4))); //just change the LDO2 setting
			Wire1.endTransmission();
		}


	public:
		inline void SystemStart()
		{
			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0x10 );
			Wire1.write( 0xff );  //OLED_VPP Enable
			Wire1.endTransmission();

			Wire1.beginTransmission( 0x34 );
			Wire1.write( 0x28 );
			Wire1.write( 0xCC ); //Enable LDO2&LDO3, LED&TFT 3.0V
			Wire1.endTransmission();

			UpdateBrightness();

			UpdateEnabled();

			inherited::SystemStart();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

