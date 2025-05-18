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
#include <genieArduino.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Index,
		typename T_OutputPin,
		int V_OBJECT
	> class ViSiGenieTypedCharOut :
		public T_Index,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Index )


//	public:
//		bool	EnterNewLine;

	protected:
		inline void PrcessInValue( uint16_t AValue )
		{
			char ATypedValue = *(char *)AValue;
			T_OutputPin::SetPinValue( ATypedValue );
//			if( EnterNewLine )
//				if( *ATypedValue == '\r' )
//					OutputPin.SendValue( '\n' );
		}

	public:
		inline void Start() {}
		inline void Process() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed )
		{
			if( AProcessed )
				return;

//			Serial.println( "ProcessOut" );
			if( Event.reportObject.cmd == GENIE_REPORT_EVENT )
			{
/*
				Serial.println( "GENIE_REPORT_EVENT" );
				Serial.print( Event.reportObject.object );
				Serial.print( " - " );
				Serial.println( Event.reportObject.index );
*/
				if( Event.reportObject.object == V_OBJECT )
					if( Event.reportObject.index == Index().GetValue() )
					{
						uint16_t AValue = AGenie.GetEventData(&Event);
//						Serial.println( AValue );
						PrcessInValue( AValue );

						AProcessed = true;
					}

			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_InitialValue,
		typename T_OnlyChanged,
		int V_OBJECT
	> class ViSiGenieTypedUnsignedIn :
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_InitialValue,
		public T_OnlyChanged
	{
	public:
		_V_PROP_( Index )
		_V_PROP_( InitialValue )

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		uint32_t	FOldValue;
		
	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue == InitialValue().GetValue() )
					return;

			if( ! FReceived().GetValue() )
				return;

			FStarted() = true;
			FOldValue = InitialValue();

			C_OWNER.FGenie.WriteObject(V_OBJECT, Index().GetValue(), InitialValue() );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(uint32_t *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue() == FOldValue )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	public:
		ViSiGenieTypedUnsignedIn()
		{
			FStarted() = false;
			FReceived() = false;
			FOldValue = InitialValue();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_InitialValue,
		typename T_OnlyChanged,
		int C_TYPE_H,
		int C_TYPE_L
	> class ViSiGenieTypedUnsignedIn32bit :
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_InitialValue,
		public T_OnlyChanged
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Index )

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		uint32_t	FOldValue;
		
	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue == InitialValue().GetValue() )
					return;

			if( ! FReceived().GetValue() )
				return;

			FStarted() = true;
			FOldValue = InitialValue();

			C_OWNER.FGenie.WriteObject( C_TYPE_L, Index().GetValue(), FOldValue & 0xFFFF );
			C_OWNER.FGenie.WriteObject( C_TYPE_H, Index().GetValue(), FOldValue >> 16 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(uint32_t *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue() == FOldValue )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	public:
		ViSiGenieTypedUnsignedIn32bit()
		{
			FStarted() = false;
			FReceived() = false;
			FOldValue = InitialValue();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_InitialValue,
		typename T_OnlyChanged,
		int C_TYPE_H,
		int C_TYPE_L
	> class ViSiGenieTypedAnalogIn32bit :
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_InitialValue,
		public T_OnlyChanged
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Index )

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		uint32_t	FOldValue;
		
	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue == InitialValue().GetValue() )
					return;

			if( ! FReceived().GetValue() )
				return;

			FStarted() = true;
			FOldValue = InitialValue();

			C_OWNER.FGenie.WriteObject( C_TYPE_L, Index().GetValue(), uint32_t( FOldValue ) & 0xFFFF );
			C_OWNER.FGenie.WriteObject( C_TYPE_H, Index().GetValue(), uint32_t( FOldValue ) >> 16 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue() == FOldValue )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	public:
		ViSiGenieTypedAnalogIn32bit()
		{
			FStarted() = false;
			FReceived() = false;
			FOldValue = InitialValue();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_InitialValue,
		typename T_OnlyChanged,
		typename T_OutputPin,
		int V_OBJECT
	> class ViSiGenieTypedUnsignedInOut :
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_InitialValue,
		public T_OnlyChanged,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Index )
		_V_PROP_( InitialValue )

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		uint32_t		FOldValue;

		
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(uint32_t *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue().GetValue() == FOldValue )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	protected:
		inline uint16_t	GetValue()	{ return InitialValue(); };

	public:
		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue == InitialValue() )
					return;

			if( ! FReceived() )
				return;

			FStarted() = true;
			FOldValue = InitialValue();

			C_OWNER.FGenie.WriteObject(V_OBJECT, Index().GetValue(), GetValue() );
		}

	protected:
		void PrcessInValue( uint16_t AValue )
		{
			uint32_t ATypedValue = (uint32_t)AValue;
			T_OutputPin::SetPinValue( ATypedValue, true );
		}

	public:
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed )
		{
			if( AProcessed )
				return;

//			Serial.println( "ProcessOut" );
			if( Event.reportObject.cmd == GENIE_REPORT_EVENT )
			{
/*
				Serial.println( "GENIE_REPORT_EVENT" );
				Serial.print( Event.reportObject.object );
				Serial.print( " - " );
				Serial.println( Event.reportObject.index );
*/
				if( Event.reportObject.object == V_OBJECT )
					if( Event.reportObject.index == Index().GetValue() )
					{
						uint16_t AValue = AGenie.GetEventData(&Event);
//						Serial.println( AValue );
						PrcessInValue( AValue );

						AProcessed = true;
					}

			}
		}

	public:
		inline void Start()
		{
			T_OutputPin::SetPinValue( InitialValue(), false );
		}

	public:
		ViSiGenieTypedUnsignedInOut()
		{
			FStarted() = false;
			FReceived() = false;
			FOldValue = InitialValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FOldValue,
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_InitialValue,
		typename T_OnlyChanged,
		int V_OBJECT
	> class ViSiGenieTypedDigitalIn :
		public T_FOldValue,
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_InitialValue,
		public T_OnlyChanged
	{
	public:
		_V_PROP_( Index )
		_V_PROP_( InitialValue )

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		_V_PROP_( FOldValue )
		
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(bool *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue().GetValue() == FOldValue().GetValue() )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	protected:
		inline uint16_t	GetValue() { return InitialValue() ? 1 : 0; }

	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}
		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue().GetValue() == InitialValue().GetValue() )
					return;

			if( ! FReceived() )
				return;

			FStarted() = true;
			FOldValue() = InitialValue();

			C_OWNER.FGenie.WriteObject(V_OBJECT, Index().GetValue(), GetValue() );
		}

	public:
		ViSiGenieTypedDigitalIn()
		{
			FStarted() = false;
			FReceived() = false;
			FOldValue() = InitialValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FOldReceivedValue,
		typename T_FOldValue,
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_InitialValue,
		typename T_OnlyChanged,
		typename T_OutputPin,
		int V_OBJECT
	> class ViSiGenieTypedDigitalInOut :
		public T_FOldReceivedValue,
		public T_FOldValue,
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_InitialValue,
		public T_OnlyChanged,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Index )
		_V_PROP_( InitialValue )

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		_V_PROP_( FOldValue )

	protected:
		_V_PROP_( FOldReceivedValue )

		
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(bool *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue().GetValue() == FOldValue().GetValue() )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	public:
		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue().GetValue() == InitialValue().GetValue() )
					return;

			if( ! FReceived() )
				return;

			FStarted() = true;
			FOldValue() = InitialValue();

			C_OWNER.FGenie.WriteObject(V_OBJECT, Index().GetValue(), GetValue() );
		}

	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed )
		{
			if( AProcessed )
				return;

//			Serial.println( "ProcessOut" );
			if( Event.reportObject.cmd == GENIE_REPORT_EVENT )
			{
/*
				Serial.println( "GENIE_REPORT_EVENT" );
				Serial.print( Event.reportObject.object );
				Serial.print( " - " );
				Serial.println( Event.reportObject.index );
*/
				if( Event.reportObject.object == V_OBJECT )
					if( Event.reportObject.index == Index().GetValue() )
					{
						uint16_t AValue = AGenie.GetEventData(&Event);
//						Serial.println( AValue );
						PrcessInValue( AValue );

						AProcessed = true;
					}

			}
		}

	protected:
		inline uint16_t	GetValue() { return InitialValue().GetValue() ? 1 : 0; } 

	protected:
		inline void PrcessInValue( uint16_t AValue )
		{
			bool ATypedValue = (bool)AValue;
			if( ! FOldReceivedValue() )
				if( ! ATypedValue )
					T_OutputPin::SetPinValueHigh();

			FOldReceivedValue() = ATypedValue;
			T_OutputPin::SetPinValue( ATypedValue );
		}

	public:
		inline ViSiGenieTypedDigitalInOut()
		{
			FStarted() = false;
			FReceived() = false;
			FOldReceivedValue() = false;
			FOldValue() = InitialValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
//		typename T_InitialValue,
		typename T_OnlyChanged,
		typename T_OutputPin,
		int V_OBJECT
	> class ViSiGenieTypedColorInOut :
		public T_FReceived,
		public T_FStarted,
		public T_Index,
//		public T_InitialValue,
		public T_OnlyChanged,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Index )

	public:
		Mitov::TColor	InitialValue;

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		Mitov::TColor	FOldValue;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue = *(Mitov::TColor *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue == FOldValue )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	protected:
		inline uint16_t GetValue()
		{ 
			return ( ( InitialValue.Red >> 3 ) << ( 6 + 5 )) | ( ( InitialValue.Green >> 2 ) & 0b111111 ) << 5 | ( ( InitialValue.Blue >> 3 ) & 0b11111 );
		}

	public:
		inline void Start() {}
		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue == InitialValue )
					return;

			if( ! FReceived() )
				return;

			FStarted() = true;
			FOldValue = InitialValue;

			C_OWNER.FGenie.WriteObject(V_OBJECT, Index().GetValue(), GetValue() );
		}

	protected:
		inline void PrcessInValue( uint16_t AValue )
		{
			Mitov::TColor ATypedValue; // = AValue;
			ATypedValue.Red = ( AValue & 0b1111100000000000 ) >> ( 6 + 5 - 3 ); //0b1111100000000000 
			ATypedValue.Green = ( AValue & 0b11111100000 ) >> ( 5 - 2 ); 
			ATypedValue.Blue = ( AValue & 0b11111 ) << 3; 
			T_OutputPin::SetPinValue( ATypedValue );
		}

	public:
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed )
		{
			if( AProcessed )
				return;

//			Serial.println( "ProcessOut" );
			if( Event.reportObject.cmd == GENIE_REPORT_EVENT )
			{
/*
				Serial.println( "GENIE_REPORT_EVENT" );
				Serial.print( Event.reportObject.object );
				Serial.print( " - " );
				Serial.println( Event.reportObject.index );
*/
				if( Event.reportObject.object == V_OBJECT )
					if( Event.reportObject.index == Index().GetValue() )
					{
						uint16_t AValue = AGenie.GetEventData(&Event);
//						Serial.println( AValue );
						PrcessInValue( AValue );

						AProcessed = true;
					}

			}
		}

	public:
		inline ViSiGenieTypedColorInOut()
		{
			FStarted() = false;
			FReceived() = false;
//			FOldValue = InitialValue;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_OnlyChanged,
		int V_OBJECT
	> class ViSiGenieTypedStringIn :
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_OnlyChanged
	{
	public:
		_V_PROP_( Index )

	public:
		Mitov::String	InitialValue;

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		Mitov::String	FOldValue;
		
	public:
		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue = (char *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue == FOldValue )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue == InitialValue )
					return;

			if( ! FReceived() )
				return;

			FStarted() = true;
			FOldValue = InitialValue;

			C_OWNER.FGenie.WriteStr(Index().GetValue(), InitialValue.c_str() );
		}

	public:
		ViSiGenieTypedStringIn()
		{
			FStarted() = false;
			FReceived() = false;
			FOldValue = InitialValue;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FReceived,
		typename T_FStarted,
		typename T_Index,
		typename T_OnlyChanged,
		int V_OBJECT
	> class ViSiGenieTypedInherentStringIn :
		public T_FReceived,
		public T_FStarted,
		public T_Index,
		public T_OnlyChanged
	{
	public:
		_V_PROP_( Index )

	public:
		Mitov::String	InitialValue;

	public:
		_V_PROP_( OnlyChanged )

	public:
		_V_PROP_( FStarted )
		_V_PROP_( FReceived )

	public:
		Mitov::String	FOldValue;
		
	public:
		inline void RefreshInputPin_o_Receive( void *_Data )
		{
			FStarted() = false;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue = (char *)_Data;
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( InitialValue == FOldValue )
					return;

			FReceived() = true;
			C_OWNER.FModified() = true;
		}

		inline void DefaultInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FGenie.WriteInhLabel( Index().GetValue() );
		}

	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

		inline void Process()
		{
			if( OnlyChanged().GetValue() && FStarted().GetValue() )
				if( FOldValue == InitialValue )
					return;

			if( ! FReceived() )
				return;

			FStarted() = true;
			FOldValue = InitialValue;

			C_OWNER.FGenie.WriteInhLabel( Index().GetValue(), InitialValue.c_str() );
		}

	public:
		ViSiGenieTypedInherentStringIn()
		{
			FStarted() = false;
			FReceived() = false;
			FOldValue = InitialValue;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Index,
		typename T_OutputPin,
		int V_OBJECT
	> class ViSiGenieTypedClockInOut :
		public T_Index,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Index )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.FGenie.WriteObject( V_OBJECT, Index().GetValue(), 0 );
		}


	public:
		inline void Start() {}
		inline void Process() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed )
		{
			if( AProcessed )
				return;

//			Serial.println( "ProcessOut" );
			if( Event.reportObject.cmd == GENIE_REPORT_EVENT )
			{
/*
				Serial.println( "GENIE_REPORT_EVENT" );
				Serial.print( Event.reportObject.object );
				Serial.print( " - " );
				Serial.println( Event.reportObject.index );
*/
				if( Event.reportObject.object == V_OBJECT )
					if( Event.reportObject.index == Index().GetValue() )
					{
						T_OutputPin::ClockPin();
//						uint16_t AValue = AGenie.GetEventData(&Event);
//						Serial.println( AValue );
//						PrcessInValue( AValue );

						AProcessed = true;
					}

			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_NUM_CHANNELS,
		typename T_Index
	> class ViSiGenieSpectrum :
		public T_Index
	{
	public:
		_V_PROP_( Index )

	protected:
		float	FValues[ C_NUM_CHANNELS ];
		uint8_t	FModified[ ( C_NUM_CHANNELS + 7 ) / 8 ];

	public:
		void ColumnsInputPins_o_Receive( int AIndex, void *_Data )
		{
			float AValue = MitovConstrain( *(float *)_Data, 0.0f, 1.0f );
			if( FValues[ AIndex ] == AValue )
				return;

			Func::SetBitField( FModified, AIndex, true );
			C_OWNER.FModified() = true;
			FValues[ AIndex ] = AValue;
		}
		
	public:
		inline void Start() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

		inline void Process()
		{
			for( int i = 0; i < C_NUM_CHANNELS; i ++ )
				if(( FModified[ i / 8 ] & ( 1 << ( i & 0b111 ))) != 0 )
				{
					uint16_t AValue = ( i << 8 ) | ( uint8_t( FValues[ i ] * 255 + 0.5 ));
					C_OWNER.FGenie.WriteObject( GENIE_OBJ_SPECTRUM, Index().GetValue(), AValue );
				}

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_Index,
		typename T_OutputPin
	> class ViSiGenieMagicObject :
		public T_Index,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Index )

	public:
		inline void Start() {}
		inline void Process() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) 
		{
			if( Index().GetValue() != AIndex )
				return;

//			Serial.println( "ProcessMagicOut" );
//			Serial.println( ALength );

			uint8_t *ABuffer = new uint8_t[ ALength ];

			for( uint32_t i = 0; i < ALength; ++ i )
				ABuffer[ i ] = AGenie.GetNextByte();

			Mitov::TDataBlock ADataBlock( ALength, ABuffer ); 

			T_OutputPin::SetPinValue( ADataBlock, false );

			delete [] ABuffer;

		}

		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			if( ADataBlock.Size < 256 )
			{
//				Serial.println( "InputPin_o_Receive" );
//				Serial.println( ADataBlock.Size );
//				Serial.println( Index().GetValue() );
				C_OWNER.FGenie.WriteMagicBytes( Index().GetValue(), ADataBlock.Data, ADataBlock.Size );
			}

			else
			{
				if( ADataBlock.Size & 1 )
				{
					uint8_t *ABuffer = new uint8_t[ ADataBlock.Size + 1 ];

					memcpy( ABuffer, ADataBlock.Data, ADataBlock.Size );
					ABuffer[ ADataBlock.Size ] = 0;

					C_OWNER.FGenie.WriteMagicDBytes( Index().GetValue(), (uint16_t *)ABuffer, ( ADataBlock.Size + 1 ) / 2 );

					delete [] ABuffer;
				}

				else
					C_OWNER.FGenie.WriteMagicDBytes( Index().GetValue(), (uint16_t *)ADataBlock.Data, ADataBlock.Size / 2 );

			}
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Contrast,
		typename T_FModified,
		typename T_Objects_Process,
		typename T_Objects_ProcessMagicOut,
		typename T_Objects_ProcessOut,
		typename T_Objects_Start,
		typename T_ResetOutputPin
	> class Display4DSystems :
		public T_Contrast,
		public T_FModified,
		public T_ResetOutputPin
	{
	public:
		_V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( Contrast )

	public:
		_V_PROP_( FModified )

	public:
		static Genie FGenie;

	protected:
		static void MagicByteHandler( uint8_t index, uint8_t length )
		{
//			Serial.println( "MagicByteHandler" );
//			Serial.println( index );

			T_Objects_ProcessMagicOut::Call( FGenie, index, length );
		}

		static void MagicDoubleByteHandler( uint8_t index, uint8_t length )
		{
			T_Objects_ProcessMagicOut::Call( FGenie, index, length * 2 );
		}

	public:
		inline void UpdateContrast()
		{
			FGenie.WriteContrast( Contrast() * 15 + 0.5 );
		}

	public:
		inline void SystemStart() 
		{
			FGenie.Begin( C_SERIAL.GetStream() );
			FGenie.AttachMagicByteReader( MagicByteHandler );
			FGenie.AttachMagicDoubleByteReader( MagicDoubleByteHandler );
			if( T_ResetOutputPin::GetPinIsConnected() )
			{
//FGenie.AttachEventHandler(myGenieEventHandler); // Attach the user function Event Handler for processing events
//			T_ResetOutputPin::SetPinValueHigh();
				T_ResetOutputPin::SetPinValueLow();

				delay( 100 );

//			T_ResetOutputPin::SetPinValueLow();
				T_ResetOutputPin::SetPinValueHigh();

				delay( 3500 );
			}

			FGenie.WriteContrast( Contrast() * 15 + 0.5 );

			T_Objects_Start::Call();
		}

		inline void SystemLoopEnd() 
		{
			FGenie.DoEvents(false); // This calls the library each loop to process the queued responses from the display

			if( FModified() )
				T_Objects_Process::Call();
//				for( int i = 0; i < FElements.size(); ++ i )
//					FElements[ i ]->Process();

			genieFrame AEvent;
			if( FGenie.DequeueEvent(&AEvent))
			{
//				Serial.println( "DequeueEvent" );
				bool AProcessed = false;
				T_Objects_ProcessOut::Call( FGenie, AEvent, AProcessed );
//				for( int i = 0; i < FOutElements.size(); ++ i )
//					if( FOutElements[ i ]->ProcessOut( FGenie, AEvent ))
//						break;
			}
		}
	};
//---------------------------------------------------------------------------
template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Contrast,
		typename T_FModified,
		typename T_Objects_Process,
		typename T_Objects_ProcessMagicOut,
		typename T_Objects_ProcessOut,
		typename T_Objects_Start,
		typename T_ResetOutputPin
	> Genie Display4DSystems<T_SERIAL, C_SERIAL,
		T_Contrast,
		T_FModified,
		T_Objects_Process,
		T_Objects_ProcessMagicOut,
		T_Objects_ProcessOut,
		T_Objects_Start,
		T_ResetOutputPin>::FGenie;
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_CHANNELS,
		typename T_Volume
	> class ViSiGenieSounds :
		public T_Volume
	{
	public:
		_V_PROP_( Volume )

	public:
		inline void UpdateVolume()
		{
			C_OWNER.FGenie.WriteObject( GENIE_OBJ_SOUND, 1, Volume().GetValue() * 100 + 0.5 );
		}

	public:
		inline void Start()
		{
			C_OWNER.FGenie.WriteObject( GENIE_OBJ_SOUND, 1, Volume().GetValue() * 100 + 0.5 );
		}

		inline void Process() {}
		inline void ProcessMagicOut( Genie & AGenie, uint8_t AIndex, uint8_t ALength ) {}
		inline void ProcessOut( Genie &AGenie, genieFrame &Event, bool &AProcessed ) {}

	public:
		inline void TracksStartInputPins_o_Receive( int AIndex, void *_Data )
		{
			C_OWNER.FGenie.WriteObject( GENIE_OBJ_SOUND, 0, AIndex );
		}

		inline void StopInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FGenie.WriteObject( GENIE_OBJ_SOUND, 4, 0 );
		}

		inline void PauseInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FGenie.WriteObject( GENIE_OBJ_SOUND, 2, 0 );
		}

		inline void ResumeInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FGenie.WriteObject( GENIE_OBJ_SOUND, 3, 0 );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif