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

#if ! ( defined( VISUINO_TEENSY ) || defined( VISUINO_RPI_PICO ))
  #include <Keyboard.h>
#endif

namespace Mitov
{
	template <
		typename T_Enabled
	> class MitovHIDKeyboard :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				Keyboard.begin();

			else
				Keyboard.end();

		}

	public:
		template <typename T> void Print( T AValue )
		{
			if( Enabled() )
			{
				Keyboard.println( AValue );
/*
				FSending = true;
				SendingOutputPin.SendValue( FSending );
				FStartTime = micros();
				GetStream().println( AValue );
*/
			}
		}

		inline void PrintChar( char AValue )
		{
			if( Enabled() )
			{
				Keyboard.write( AValue );
/*
				FSending = true;
				SendingOutputPin.SendValue( FSending );
				FStartTime = micros();
				GetStream().print( AValue );
*/
			}
		}

		void Write( uint8_t *AData, uint32_t ASize )
		{
			for( uint32_t i = 0; i < ASize; ++ i )
				Keyboard.write( AData[ i ] );

//			GetStream().write( AData, ASize );
		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				Keyboard.begin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Character,
		typename T_FPressed
	> class MitovHIDKeyboardElementKey :
		public T_Character,
		public T_FPressed
	{
	public:
		_V_PROP_( Character )

	protected:
		_V_PROP_( FPressed )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( C_OWNER.Enabled() )
			{
				bool AValue = *(bool *)_Data;
				if( AValue != FPressed().GetValue() )
				{
					FPressed() = AValue;
					if( AValue )
						Keyboard.press( Character() );

					else
						Keyboard.release( Character() );

				}
			}
		}
	};
//---------------------------------------------------------------------------
}

