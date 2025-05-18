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
/*
	class KeypadBasicKeyElement
	{
	public:
		virtual void SetButtonValue( bool AValue )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	class BasicPinRead
	{
	public:
		virtual void SetMode( int AMode ) = 0;
		virtual bool DigitalRead() = 0;
	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class TypedPinRead : public BasicPinRead
	{
	public:
		virtual void SetMode( int AMode ) override
		{
			C_OWNER.SetMode( AMode );
		}

		virtual bool DigitalRead() override
		{
			return C_OWNER.DigitalRead();
		}
	};
//---------------------------------------------------------------------------
	template <
		int C_X, int C_Y,
		typename T_ColumnsOutputPins_SetPinValue,
		typename T_DebounceInterval,
		typename T_FInScanning,
		typename T_Keys_SetButtonValue
	> class Keypad :
		public T_DebounceInterval,
		public T_FInScanning,
		public T_Keys_SetButtonValue
	{
		typedef Keypad<
				C_X, C_Y,
				T_ColumnsOutputPins_SetPinValue,
				T_DebounceInterval,
				T_FInScanning,
				T_Keys_SetButtonValue
			> SELF;

	public:
		Mitov::BasicPinRead	**_Pins;

	public:
		_V_PROP_( DebounceInterval )

	protected:
		_V_PROP_( FInScanning )

//		KeypadBasicKeyElement	*FKeyMap[ C_X ][ C_Y ];

	public:
		inline SELF *GetRootOwner()
		{
			return this;
		}

/*
		inline void RegisterKey( int ARow, int ACol, KeypadBasicKeyElement *AKey )
		{
			FKeyMap[ ACol ][ ARow ] = AKey;
		}
*/
	public:
		inline void SystemInit()
		{
			for( int i = 0; i < C_Y ; ++i )
				_Pins[ i ]->SetMode( INPUT_PULLUP );
//				pinMode( _Pins[ i ], INPUT_PULLUP );

		}

		inline void SystemLoopBegin()
		{
			if( ! FInScanning() )
			{
#ifdef __KEYPAD_DEBUG__
				Serial.println( "Start Scanning!" );
#endif
				FInScanning() = true;
				for( int i = 0; i < C_X; ++i )
					T_ColumnsOutputPins_SetPinValue::Call( i, false );
			}

			for( int i = 0; i < C_Y; ++i )
  				if( ! _Pins[ i ]->DigitalRead() )
					FInScanning() = false;

#ifdef __KEYPAD_DEBUG__
			if( ! FInScanning() )
				Serial.println( "----------------------------" );
#endif

			for( int i = 0; i < C_X; ++i )
			{
				if( ! FInScanning() )
				{
					for( int j = 0; j < C_X; ++j )
					{
#ifdef __KEYPAD_DEBUG__
						Serial.print( i != j ); Serial.print( " " );
#endif
						T_ColumnsOutputPins_SetPinValue::Call( j, i != j );
					}

#ifdef __KEYPAD_DEBUG__
					Serial.println( "" );
#endif
				}

	//			delay( 1000 );

#ifdef __KEYPAD_DEBUG__
				if( ! FInScanning() )
					Serial.println( "+++++++++++++++++++++++++" );
#endif

				ComponentsHardware::SystemUpdateHardware();
				for( int j = 0; j < C_Y; ++j )
					T_Keys_SetButtonValue::Call( i, j, _Pins[ j ]->DigitalRead() );
/*
				for( int j = 0; j < C_Y; ++j )
					if( FKeyMap[ i ][ j ] )
					{
#ifdef __KEYPAD_DEBUG__
						if( ! FInScanning )
						{
							if( ! _Pins[ j ]->DigitalRead() )
							{
								Serial.print( i ); Serial.print( "," ); Serial.println( j );
							}
	//						Serial.print( i ); Serial.print( "," ); Serial.print( j ); Serial.print( " = " ); Serial.print( _Pins[ j ]->DigitalRead() ); Serial.print( "  " ); 
						}
#endif
						FKeyMap[ i ][ j ]->SetButtonValue( _Pins[ j ]->DigitalRead() );
					}
*/
	//			if( ! FInScanning )
	//				Serial.println( "" );
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_COLUMN,
		uint32_t C_ROW,
		typename T_FLastValue,
		typename T_FValue,
		typename T_OutputPin
	> class DigitalKeypadKey :
		public T_FLastValue,
		public T_FValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		unsigned long	FLastTime = 0;
		_V_PROP_( FValue )
		_V_PROP_( FLastValue )

	public:
		inline void SetButtonValue( uint32_t AColumn, uint32_t ARow, bool AValue )
		{
			if( ( C_COLUMN != AColumn ) || ( C_ROW != ARow ) )
				return;

			if( AValue != FLastValue() )
				FLastTime = millis();

			FLastValue() = AValue;
			if( FValue().GetValue() != FLastValue().GetValue() )
				if( millis() - FLastTime > C_OWNER.DebounceInterval() )
				{
					FValue() = FLastValue();
					T_OutputPin::SetPinValue( ! FValue() );
				}

		}

	public:
		inline DigitalKeypadKey()
		{
			FLastValue() = false;
			FValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Keys_SetButtonValue,
		typename T_OutputPin
	> class CharacterKeyGroup :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline T_OWNER *GetRootOwner()
		{
			return C_OWNER.GetRootOwner();
		}

		inline void SetButtonValue( uint32_t AColumn, uint32_t ARow, bool AValue )
		{
			T_Keys_SetButtonValue::Call( AColumn, ARow, AValue );
		}
/*
		inline void RegisterKey( int ARow, int ACol, KeypadBasicKeyElement *AKey )
		{
			C_OWNER.RegisterKey( ARow, ACol, AKey );
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_COLUMN,
		uint32_t C_ROW,
		typename T_Character,
		typename T_FLastValue,
		typename T_FValue
	> class CharKeypadKey :
		public T_Character,
		public T_FLastValue,
		public T_FValue
	{
	public:
		_V_PROP_( Character )

	protected:
		unsigned long	FLastTime = 0;
		_V_PROP_( FValue )
		_V_PROP_( FLastValue )

	public:
		inline void SetButtonValue( uint32_t AColumn, uint32_t ARow, bool AValue )
		{
			if( ( C_COLUMN != AColumn ) || ( C_ROW != ARow ) )
				return;

			if( AValue != FLastValue() )
				FLastTime = millis();

			FLastValue() = AValue;
			if( FValue().GetValue() != FLastValue().GetValue() )
				if( millis() - FLastTime > C_OWNER.GetRootOwner()->DebounceInterval() )
				{
					FValue() = FLastValue();
					if( ! FValue() )
						C_OWNER.OutputPin().SetPinValue( Character() );

//					OutputPin.Notify( &FValue );
				}

		}

	public:
		inline CharKeypadKey()
		{
			FValue() = false;
			FLastValue() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif