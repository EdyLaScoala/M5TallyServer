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
		typename T_Elements_TryActive,
//		typename T_FStateActive,
		typename T_OutputPin,
		typename T_Value
	> class TextValue : 
//		public T_FStateActive,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline void UpdateValue()
		{
			T_OutputPin::SetPinValue( Value() );
		}

		void ExternalSetValue( Mitov::String AValue, bool AFromState )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}
/*
		inline void SetValue( Mitov::String AValue )
		{
			ExternalSetValue( AValue );
		}
*/
	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value() );
		}

	};
//---------------------------------------------------------------------------
/*
	class BindableTextValue : public TextValue
	{
		typedef TextValue inherited;

	protected:
		Mitov::String OldValue = inherited::Value;

	public:
		inline void SystemInit()
		{
			OldValue = inherited::Value;
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value == OldValue )
				return;

			OldValue = inherited::Value;
			inherited::OutputPin.Notify( (void *)OldValue.c_str() );
		}

	public:
		BindableTextValue( char *AValue ) :
			inherited( AValue ),
			OldValue( AValue )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Value
	> class ArduinoSetTextValueElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.ExternalSetValue( Value(), false );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FInputValue,
		typename T_Value
	> class ArduinoStateSetTextValueElement :
		public T_FInputValue,
		public T_Value
	{
	public:
		_V_PROP_( Value )

	protected:
		_V_PROP_( FInputValue )

	public:
		void TryActive( bool &AResult )
		{
			if( FInputValue() )
			{
				C_OWNER.ExternalSetValue( Value(), true );
				AResult = true;
			}
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue() == *(bool *)_Data )
				return;

			FInputValue() = *(bool *)_Data;
			if( FInputValue() )
				C_OWNER.ExternalSetValue( Value(), true );

			else
				C_OWNER.ResetValue();
		}

	public:
		ArduinoStateSetTextValueElement()
		{
			FInputValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_TryActive,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T_Value
	> class TextValueState :
		public T_FStateActive,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	protected:
		Mitov::String FCurrentValue;

	public:
		void SetValue( Mitov::String AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FStateActive() )
				return;

			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalSetValue( Mitov::String AValue, bool AFromState )
		{
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			if( AFromState )
				FStateActive() = true;

			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
//			for( int i = FElements.size(); i -- ; )
//				if( FElements[ i ]->TryActive( this ) )
//					return;

			bool AResult = false;
			T_Elements_TryActive::Call( AResult );
			if( AResult )
				return;

			FStateActive() = false;
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( Value() );
//			if( ! ClockInputPin_o_IsConnected )
//				OutputPin.Notify( (void *)FCurrentValue.c_str() );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline TextValueState()
		{
			FStateActive() = false;
		}
	};
//---------------------------------------------------------------------------
/*
	class BindableTextStateValue : public TextStateValue
	{
		typedef TextValue inherited;

	protected:
		Mitov::String OldValue;

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			OldValue = inherited::Value;
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value == OldValue )
				return;

			OldValue = inherited::Value;
			ResetValue();
//			inherited::OutputPin.Notify( (void *)OldValue.c_str() );
		}

	public:
		using inherited::inherited;

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Text,
		typename T_ITEM, T_ITEM &C_ITEM
	> class TFormattedTextStringItemIndexed :
		public T_Text
	{
//	public:
//		_V_PROP_( Text )

	public:
		inline void Process( Mitov::String &AText )
		{
			AText += T_Text::GetValue() + C_ITEM.GetText();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Text
	> class TFormattedTextStringItemText :
		public T_Text
	{
	public:
		inline void Process( Mitov::String &AText )
		{
			AText += T_Text::GetValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		bool C_Clock_IsConnected,
		uint16_t C_COUNT,
		typename T_Elements_CanProcess,
		typename T_Elements_Process,
		typename T_Elements_Processed,
		typename T_FModified,
		typename T_OutputPin
	> class FormattedText_Fixed :
		public T_FModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( FModified )

	protected:
		void ProcessSendOutput( bool ANeedsCanSend )
		{
			bool ACanSend = true;
			T_Elements_CanProcess::Call( ACanSend );
			if( ! ACanSend )
				return;

			Mitov::String AText;
			T_Elements_Process::Call( AText );

//			Serial.println( AText );
			T_OutputPin::SetPinValue( AText.c_str() );
			FModified() = false;
			T_Elements_Processed::Call();
		}

	public:
		void SetModified()
		{
			FModified() = true;
			if( ! C_Clock_IsConnected )
				ProcessSendOutput( true );
		}

	public:
		inline void SystemStart()
		{
		}

		inline void SystemStartSend()
		{
			ProcessSendOutput( false );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				ProcessSendOutput( false );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ProcessSendOutput( false );
		}

	public:
		FormattedText_Fixed()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		bool C_Clock_IsConnected,
		uint16_t C_COUNT,
		typename T_Elements_CanProcess,
		typename T_Elements_Process,
		typename T_Elements_Processed,
		typename T_FModified,
		typename T_OutputPin,
		typename T_Text
	> class FormattedText : 
		public T_FModified,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Text )

	public:
		_V_PROP_( FModified )

	protected:
		struct TStringItem
		{
			Mitov::String	Text;
			int8_t			Index;
		};

	public:
		void SetModified()
		{
			FModified() = true;
			if( ! C_Clock_IsConnected )
				ProcessSendOutput( true );

		}

	protected:
		Mitov::SimpleList<TStringItem>	FReadyElements;

	protected:
		void AddReadyElement( Mitov::String ATextItem, int16_t AIndex )
		{
//			Serial.print( "AddElement: " ); Serial.print( ATextItem ); Serial.println( AIndex );

			TStringItem	AItem;
			AItem.Text = ATextItem;
//			Serial.print( "AddElement: " ); Serial.println( AItem->Text );
			if( AIndex < C_COUNT )
				AItem.Index = AIndex;

			else
				AItem.Index = -1;

			FReadyElements.push_back( AItem );

//			Serial.println( FReadyElements[ FReadyElements.size() - 1 ]->Text );
//			Serial.println( "DEBUG>>" );
//			for( Mitov::SimpleList<TStringItem *>::iterator Iter = FReadyElements.begin(); Iter != FReadyElements.end(); ++Iter )
//				Serial.println( ( *Iter )->Text );

//			Serial.println( "<<DEBUG" );
		}

	public:
		inline void InitElements( const String &AOldValue, const String &Value )
		{
		}

		void InitElements()
		{
			FReadyElements.clear();
			Mitov::String	ATextItem;
			Mitov::String	AIndexText;
			bool	AInEscape = false;

//			Serial.println( "INIT" );
//			Serial.println( Text );
//			delay( 1000 );

			uint32_t AAutoIndex = 0;

			for( unsigned int i = 0; i < Text().GetValue().length(); ++ i )
			{
				char AChar = Text().GetValue()[ i ];
				if( AInEscape )
				{
					if( AChar >= '0' && AChar <= '9' )
						AIndexText += AChar;

					else
					{
						if( AChar == '%' )
						{
							if( AIndexText.length() == 0 )
							{
								ATextItem += '%';
								AInEscape = false;
							}

							else
							{
								AddReadyElement( ATextItem, AIndexText.toInt() );
								ATextItem = "";
								AIndexText = "";
							}

						}

						else
						{
							if( AIndexText.length() == 0 )
								AddReadyElement( ATextItem, AAutoIndex ++ );

							else
								AddReadyElement( ATextItem, AIndexText.toInt() );

							ATextItem = AChar;
						}

						if( AChar != '%' )
							AInEscape = false;
					}
				}

				else
				{
					if( AChar == '%' )
					{
						AInEscape = true;
						AIndexText = "";
					}

					else
						ATextItem += AChar;

				}

			}

			if( AInEscape )
				AddReadyElement( ATextItem, AIndexText.toInt() );

			else if( ATextItem.length() )
			{
				TStringItem	AItem;

				AItem.Text = ATextItem;
				AItem.Index = -1;

				FReadyElements.push_back( AItem );
			}

//			Serial.println( "DEBUG>>" );
//			for( Mitov::SimpleList<TStringItem *>::iterator Iter = FReadyElements.begin(); Iter != FReadyElements.end(); ++Iter )
//				Serial.println( ( *Iter )->Text );

//			Serial.println( "<<DEBUG" );
		}

	protected:
		void ProcessSendOutput( bool ANeedsCanSend )
		{
			bool ACanSend = true;
			T_Elements_CanProcess::Call( ACanSend );
			if( ! ACanSend )
				return;

//			Serial.println( "ProcessSendOutput" );
			Mitov::String AText;
			TStringItem *AElements = FReadyElements.AsPointer();
			for( int i = 0; i < FReadyElements.size(); ++i )
			{
				AText += AElements[ i ].Text;
//				if( T_Elements_Process::ChainHasItems() )
				if( C_COUNT > 0 )
					if( AElements[ i ].Index >= 0 )
						T_Elements_Process::Call( AElements[ i ].Index, AText );

			}

//			Serial.println( AText );
			T_OutputPin::SetPinValue( AText.c_str() );
			FModified() = false;
			T_Elements_Processed::Call();
		}
	public:
		inline void SystemLoopEnd()
		{
			if( FModified() )
				ProcessSendOutput( false );

		}

		inline void SystemStart()
		{
			InitElements();
		}

		inline void SystemStartSend()
		{
			ProcessSendOutput( false );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ProcessSendOutput( false );
		}

	public:
		FormattedText()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	struct TSplitTextStringItem
	{
	public:
		Mitov::String	Text;
		int8_t			Index;
			
		void *FValue;

	public:
		inline bool NotEmptyText() { return Text[ 0 ] != '\0'; }

		inline bool IsSeparator( char *&AText )
		{
			const char * APtr = Text.c_str();
//			Serial.println( "IsSeparator" );
//			Serial.println( APtr );
			while( *APtr )
			{
				if( ! *AText )
					break;

//				Serial.print( *AText );
//				Serial.print( " - " );
//				Serial.println( *APtr );

				if( *APtr ++ != *AText ++ )
					break;
			}

			return( ! *APtr );
		}

		inline void ProcessTextOnly( char *&AText, bool &ACanSend )
		{
			if( ! ACanSend )
				return;

//			Serial.println( "Process1" );

			const char * APtr = Text.c_str();
			while( *APtr )
			{
				if( ! *AText )
					break;

				if( *APtr++ != *AText++ )
					break;
			}

			ACanSend = ! *APtr;
//			Serial.println( ACanSend );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class SplitTextElementText :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		template <typename T_DELEGATOR> inline void Process( T_DELEGATOR *ADelegator, char *&AText, bool &ACanSend )
		{
//			Mitov::String AUnsignedText;
//			const char *ASeparatorPtr = ASeparatorText;

			ADelegator->FValue = "";
			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( ADelegator->NotEmptyText() && ADelegator->IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator2" );
//					Serial.println( ADelegator->FValue );

					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( ADelegator->FValue );

					return; // Exit without double checking separator
				}

				ADelegator->FValue += *AText ++;
			}

//			Serial.println( ADelegator->FValue );
			ACanSend = ADelegator->IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( ADelegator->FValue );

		}

	public:
		inline void CreateStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "CreateStorage1" );
			AItem.FValue = new Mitov::String;
		}

		inline void DestroyStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "DestroyStorage1" );
			delete (Mitov::String *)AItem.FValue;
		}

		void ProcessDynamic( Mitov::TSplitTextStringItem &AItem, char *&AText, bool &ACanSend )
		{
//			Serial.println( "ProcessDynamic1" );
			Mitov::String &AValue = *( Mitov::String *)AItem.FValue;
			AValue = "";
			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( AItem.NotEmptyText() && AItem.IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator1" );
//					Serial.println( AValue );

					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( AValue );

					return; // Exit without double checking separator
				}

				AValue += *AText ++;
			}

			ACanSend = AItem.IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( AValue );

		}

		void SendDataDynamic( Mitov::TSplitTextStringItem &AItem, bool &ACanSend )
		{
//			Serial.println( "SendDataDynamic" );
//			Serial.println( ACanSend );
//			Serial.println( *( uint32_t *)AItem.FValue );
			Mitov::String &AValue = *( Mitov::String *)AItem.FValue;

			if( ACanSend )
				T_OutputPin::SetPinValue( AValue );

			AValue = "";
		}

	public:
		template <typename T_DELEGATOR> inline void SendData( T_DELEGATOR *ADelegator, bool ACanSend )
		{
			if( ACanSend )
				T_OutputPin::SetPinValue( ADelegator->FValue );

			ADelegator->FValue = "";
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class SplitTextElementUnsigned :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		template <typename T_DELEGATOR> inline void Process( T_DELEGATOR *ADelegator, char *&AText, bool &ACanSend )
		{
//			Mitov::String AUnsignedText;
//			const char *ASeparatorPtr = ASeparatorText;

			ADelegator->FValue = 0;
			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( ADelegator->NotEmptyText() && ADelegator->IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator1" );
//					Serial.println( ADelegator->FValue );
					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( ADelegator->FValue );

					return; // Exit without double checking separator
				}

				char AChar = *AText;
				if( ! isdigit( AChar ))
					break;

				ADelegator->FValue *= 10;
				ADelegator->FValue += ( AChar - '0' );

				++ AText; // Do not increment before checking for isdigit!
			}

			ACanSend = ADelegator->IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( ADelegator->FValue );

		}

	public:
		inline void CreateStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "CreateStorage1" );
			AItem.FValue = new uint32_t;
		}

		inline void DestroyStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "DestroyStorage1" );
			delete (uint32_t *)AItem.FValue;
		}

		void ProcessDynamic( Mitov::TSplitTextStringItem &AItem, char *&AText, bool &ACanSend )
		{
//			Serial.println( "ProcessDynamic1" );
			uint32_t &AValue = *(uint32_t *)AItem.FValue;
			AValue = 0;
			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( AItem.NotEmptyText() && AItem.IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator1" );
//					Serial.println( AValue );
					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( AValue );

					return; // Exit without double checking separator
				}

				char AChar = *AText;
				if( ! isdigit( AChar ))
					break;

				AValue *= 10;
				AValue += ( AChar - '0' );

				++ AText; // Do not increment before checking for isdigit!
			}

			ACanSend = AItem.IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( AValue );

		}

		void SendDataDynamic( Mitov::TSplitTextStringItem &AItem, bool &ACanSend )
		{
//			Serial.println( "SendDataDynamic" );
//			Serial.println( ACanSend );
//			Serial.println( *( uint32_t *)AItem.FValue );
			if( ACanSend )
				T_OutputPin::SetPinValue( *( uint32_t *)AItem.FValue );
		}

	public:
		template <typename T_DELEGATOR> inline void SendData( T_DELEGATOR *ADelegator, bool ACanSend )
		{
			if( ACanSend )
				T_OutputPin::SetPinValue( ADelegator->FValue );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class SplitTextElementInteger :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		template <typename T_DELEGATOR> inline void Process( T_DELEGATOR *ADelegator, char *&AText, bool &ACanSend )
		{
//			Mitov::String AUnsignedText;
//			const char *ASeparatorPtr = ASeparatorText;

			ADelegator->FValue = 0;

			bool AFirstCharProcessed = false;
			bool AIsNegative = false;

			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( ADelegator->NotEmptyText() && ADelegator->IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator3" );
//					Serial.println( ADelegator->FValue );
					if( AIsNegative )
						ADelegator->FValue = -ADelegator->FValue;

					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( ADelegator->FValue );

					return; // Exit without double checking separator
				}

//				Serial.println( "TEST2" );
				char AChar = *AText;
//				Serial.println( AChar );
//				Serial.println( "-----" );
				if( AChar == '-' )
				{
//					Serial.println( "is '-'" );
					if( AFirstCharProcessed )
						break;

					AIsNegative = true;
					AFirstCharProcessed = true;
//					Serial.println( "AIsNegative" );
				}

				else if( AChar == '+' )
				{
					if( AFirstCharProcessed )
						break;

					AFirstCharProcessed = true;
				}

				else
				{
					if( ! isdigit( AChar ))
						break;

					ADelegator->FValue *= 10;
					ADelegator->FValue += ( AChar - '0' );
				}

				++ AText; // Do not increment before checking for isdigit!
			}

			if( AIsNegative )
				ADelegator->FValue = -ADelegator->FValue;

			ACanSend = ADelegator->IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( ADelegator->FValue );

		}

	public:
		inline void CreateStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "CreateStorage1" );
			AItem.FValue = new int32_t;
		}

		inline void DestroyStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "DestroyStorage1" );
			delete (int32_t *)AItem.FValue;
		}

		void ProcessDynamic( Mitov::TSplitTextStringItem &AItem, char *&AText, bool &ACanSend )
		{
//			Serial.println( "ProcessDynamic1" );
			int32_t &AValue = *(int32_t *)AItem.FValue;
			AValue = 0;

			bool AFirstCharProcessed = false;
			bool AIsNegative = false;

			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( AItem.NotEmptyText() && AItem.IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator1" );
//					Serial.println( AValue );

					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( AValue );

					return; // Exit without double checking separator
				}

//				Serial.println( "TEST2" );
				char AChar = *AText;
//				Serial.println( AChar );
//				Serial.println( "-----" );
				if( AChar == '-' )
				{
//					Serial.println( "is '-'" );
					if( AFirstCharProcessed )
						break;

					AIsNegative = true;
					AFirstCharProcessed = true;
//					Serial.println( "AIsNegative" );
				}

				else if( AChar == '+' )
				{
					if( AFirstCharProcessed )
						break;

					AFirstCharProcessed = true;
				}

				else
				{
					if( ! isdigit( AChar ))
						break;

					AValue *= 10;
					AValue += ( AChar - '0' );
				}

				++ AText; // Do not increment before checking for isdigit!
			}

			ACanSend = AItem.IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( AValue );

		}

		void SendDataDynamic( Mitov::TSplitTextStringItem &AItem, bool &ACanSend )
		{
//			Serial.println( "SendDataDynamic" );
//			Serial.println( ACanSend );
//			Serial.println( *( uint32_t *)AItem.FValue );
			if( ACanSend )
				T_OutputPin::SetPinValue( *( int32_t *)AItem.FValue );
		}

	public:
		template <typename T_DELEGATOR> inline void SendData( T_DELEGATOR *ADelegator, bool ACanSend )
		{
			if( ACanSend )
				T_OutputPin::SetPinValue( ADelegator->FValue );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class SplitTextElementAnalog :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		template <typename T_DELEGATOR> inline void Process( T_DELEGATOR *ADelegator, char *&AText, bool &ACanSend )
		{
//			const char *ASeparatorPtr = ASeparatorText;

			Mitov::String AFloatText;

			ADelegator->FValue = 0.0f;
			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( ADelegator->NotEmptyText() && ADelegator->IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator4" );
//					Serial.println( ADelegator->FValue );
					char *AEndPos;
					ADelegator->FValue = strtod( AFloatText.c_str(), &AEndPos );
					if( *AEndPos )
						ACanSend = false;

					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( ADelegator->FValue );

					return; // Exit without double checking separator
				}

				AFloatText += *AText ++;
			}

			ADelegator->FValue = strtod( AFloatText.c_str(), nullptr );

			ACanSend = ADelegator->IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( ADelegator->FValue );

		}

		inline void CreateStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "CreateStorage1" );
			AItem.FValue = new float;
		}

		inline void DestroyStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "DestroyStorage1" );
			delete (float *)AItem.FValue;
		}

		void ProcessDynamic( Mitov::TSplitTextStringItem &AItem, char *&AText, bool &ACanSend )
		{
//			Serial.println( "ProcessDynamic1" );
			float &AValue = *(float *)AItem.FValue;
			AValue = 0.0f;

			Mitov::String AFloatText;
			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( AItem.NotEmptyText() && AItem.IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
//					Serial.println( "IsSeparator4" );
//					Serial.println( ADelegator->FValue );
					char *AEndPos;
					AValue = strtod( AFloatText.c_str(), &AEndPos );
					if( *AEndPos )
						ACanSend = false;


					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( AValue );

					return; // Exit without double checking separator
				}

				AFloatText += *AText ++;
			}

			AValue = strtod( AFloatText.c_str(), nullptr );

			ACanSend = AItem.IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( AValue );

		}

		void SendDataDynamic( Mitov::TSplitTextStringItem &AItem, bool &ACanSend )
		{
//			Serial.println( "SendDataDynamic" );
//			Serial.println( ACanSend );
//			Serial.println( *( uint32_t *)AItem.FValue );
			if( ACanSend )
				T_OutputPin::SetPinValue( *( float *)AItem.FValue );
		}

	public:
		template <typename T_DELEGATOR> inline void SendData( T_DELEGATOR *ADelegator, bool ACanSend )
		{
			if( ACanSend )
				T_OutputPin::SetPinValue( ADelegator->FValue );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FalseValue,
		typename T_OutputPin,
		typename T_TrueValue
	> class SplitTextElementDigital :
		public T_FalseValue,
		public T_OutputPin,
		public T_TrueValue
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( FalseValue )
		_V_PROP_( TrueValue )

	public:
		template <typename T_DELEGATOR> inline void Process( T_DELEGATOR *ADelegator, char *&AText, bool &ACanSend )
		{
//			const char *ASeparatorPtr = ASeparatorText;

			ADelegator->FValue() = false;

			Mitov::String ABoolText;

			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( ADelegator->NotEmptyText() && ADelegator->IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
					if( ABoolText == TrueValue() )
						ADelegator->FValue() = true;

					else if( ABoolText != FalseValue() )
						ACanSend = false;

//					Serial.println( "IsSeparator4" );
//					Serial.println( ADelegator->FValue );
//					char *AEndPos;
//					ADelegator->FValue = strtod( AFloatText.c_str(), &AEndPos );
//					if( *AEndPos )
//						ACanSend = false;

					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( ADelegator->FValue() );

					return; // Exit without double checking separator
				}

				ABoolText += *AText ++;
			}

//			ADelegator->FValue = strtod( AFloatText.c_str(), nullptr );

//			Serial.println( "TEST5" );

			if( ABoolText == TrueValue() )
				ADelegator->FValue() = true;

			else if( ABoolText != FalseValue() )
			{
				ACanSend = false;
				return;
			}

			ACanSend = ADelegator->IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( ADelegator->FValue() );

		}

		inline void CreateStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "CreateStorage1" );
			AItem.FValue = new bool;
		}

		inline void DestroyStorage( Mitov::TSplitTextStringItem &AItem )
		{
//			Serial.println( "DestroyStorage1" );
			delete (bool *)AItem.FValue;
		}

		void ProcessDynamic( Mitov::TSplitTextStringItem &AItem, char *&AText, bool &ACanSend )
		{
//			Serial.println( "ProcessDynamic1" );
			bool &AValue = *(bool *)AItem.FValue;
			AValue = false;

			Mitov::String ABoolText;

			while( *AText )
			{
//				Serial.println( "TEST1" );
				char *ATextCheck = AText;
				if( AItem.NotEmptyText() && AItem.IsSeparator( ATextCheck ) )
				{
					AText = ATextCheck;
					if( ABoolText == TrueValue() )
						AValue = true;

					else if( ABoolText != FalseValue() )
						ACanSend = false;

//					Serial.println( "IsSeparator4" );
//					Serial.println( ADelegator->FValue );
//					char *AEndPos;
//					ADelegator->FValue = strtod( AFloatText.c_str(), &AEndPos );
//					if( *AEndPos )
//						ACanSend = false;

					if( C_OWNER.AllowPartial() )
						T_OutputPin::SetPinValue( AValue );

					return; // Exit without double checking separator
				}

				ABoolText += *AText ++;
			}

//			ADelegator->FValue = strtod( AFloatText.c_str(), nullptr );

//			Serial.println( "TEST5" );

			if( ABoolText == TrueValue() )
				AValue = true;

			else if( ABoolText != FalseValue() )
			{
				ACanSend = false;
				return;
			}

			ACanSend = AItem.IsSeparator( AText );

			if( C_OWNER.AllowPartial() )
				T_OutputPin::SetPinValue( AValue );

		}

		void SendDataDynamic( Mitov::TSplitTextStringItem &AItem, bool &ACanSend )
		{
//			Serial.println( "SendDataDynamic" );
//			Serial.println( ACanSend );
//			Serial.println( *( uint32_t *)AItem.FValue );
			if( ACanSend )
				T_OutputPin::SetPinValue( *( bool *)AItem.FValue );
		}

	public:
		template <typename T_DELEGATOR> inline void SendData( T_DELEGATOR *ADelegator, bool ACanSend )
		{
			if( ACanSend )
				T_OutputPin::SetPinValue( ADelegator->FValue().GetValue() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FValue,
		typename T_Text,
		typename T_ITEM, T_ITEM &C_ITEM
	> class TSplitTextStringItemDigitalIndexed :
		public T_FValue,
		public T_Text
	{
	public:
		_V_PROP_( FValue )

	public:
		inline void Process( char *&AText, bool &ACanSend )
		{
			if( ! ACanSend )
				return;

//			Serial.println( "Process2" );

			C_ITEM.Process( this, AText, ACanSend );
		}

	public:
		inline bool NotEmptyText() { return *T_Text::GetValue() != '\0'; }

		inline bool IsSeparator( char *&AText )
		{
			const char * APtr = T_Text::GetValue();
//			Serial.println( "IsSeparator" );
//			Serial.println( APtr );
			while( *APtr )
			{
				if( ! *AText )
					break;

//				Serial.print( *AText );
//				Serial.print( " - " );
//				Serial.println( *APtr );

				if( *APtr ++ != *AText ++ )
					break;
			}

			return( ! *APtr );
		}

	public:
		inline void SendData( bool ACanSend )
		{
			C_ITEM.SendData( this, ACanSend );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_DATA,
		typename T_Text,
		typename T_ITEM, T_ITEM &C_ITEM
	> class TSplitTextStringItemTypedIndexed :
		public T_Text
	{
	public:
		T_DATA	FValue;

	public:
		inline void Process( char *&AText, bool &ACanSend )
		{
			if( ! ACanSend )
				return;

//			Serial.println( "Process2" );

			C_ITEM.Process( this, AText, ACanSend );
		}

	public:
		inline bool NotEmptyText() { return *T_Text::GetValue() != '\0'; }

		inline bool IsSeparator( char *&AText )
		{
			const char * APtr = T_Text::GetValue();
//			Serial.println( "IsSeparator" );
//			Serial.println( APtr );
			while( *APtr )
			{
				if( ! *AText )
					break;

//				Serial.print( *AText );
//				Serial.print( " - " );
//				Serial.println( *APtr );

				if( *APtr ++ != *AText ++ )
					break;
			}

			return( ! *APtr );
		}

	public:
		inline void SendData( bool ACanSend )
		{
			C_ITEM.SendData( this, ACanSend );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Text
	> class TSplitTextStringItemText :
		public T_Text
	{
	public:
		inline void Process( char *&AText, bool &ACanSend )
		{
			if( ! ACanSend )
				return;

//			Serial.println( "Process1" );

			const char * APtr = T_Text::GetValue();
			while( *APtr )
			{
				if( ! *AText )
					break;

				if( *APtr++ != *AText++ )
					break;
			}

			ACanSend = ! *APtr;
//			Serial.println( ACanSend );
		}

	public:
		inline void SendData( bool ACanSend ) {}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AllowPartial,
		uint16_t C_COUNT,
		typename T_Elements_Process,
		typename T_Elements_SendData,
		typename T_IngnoreEnd
	> class SplitText_Fixed :
		public T_AllowPartial,
		public T_IngnoreEnd
	{
	public:
		_V_PROP_( AllowPartial )
		_V_PROP_( IngnoreEnd )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char *)_Data;
			bool ACanSend = true;
			T_Elements_Process::Call( AText, ACanSend );
			if( ! IngnoreEnd().GetValue() )
				if( *AText )
					ACanSend = false; // There is remaining text!

			if( ! AllowPartial().GetValue() )
				T_Elements_SendData::Call( ACanSend );

		}

	public:
		inline void SystemStart() {}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AllowPartial,
		uint16_t C_COUNT,
		typename T_Elements_CreateStorage,
		typename T_Elements_DestroyStorage,
		typename T_Elements_Process,
		typename T_Elements_SendData,
		typename T_IngnoreEnd,
		typename T_Text
	> class SplitText :
		public T_AllowPartial,
		public T_IngnoreEnd,
		public T_Text
	{
	public:
		_V_PROP_( AllowPartial )
		_V_PROP_( IngnoreEnd )
		_V_PROP_( Text )

	protected:
		Mitov::SimpleList<TSplitTextStringItem>	FProcessElements;

	protected:
		void AddReadyElement( Mitov::String ATextItem, int16_t AIndex )
		{
			TSplitTextStringItem AItem;
			AItem.Text = ATextItem;

//			Serial.print( "AddElement: " ); Serial.print( AItem.Text ); Serial.print( " - " ); Serial.println( AIndex );

			if( AIndex < C_COUNT )
				AItem.Index = AIndex;

			else
				AItem.Index = -1;

			T_Elements_CreateStorage::Call( AItem );

			FProcessElements.push_back( AItem );
		}

	public:
		inline void InitElements( const String &AOldValue, const String &Value )
		{
		}

		void InitElements()
		{
			TSplitTextStringItem *AElements = FProcessElements.AsPointer();
			for( int i = 0; i < FProcessElements.size(); ++i )
				T_Elements_DestroyStorage::Call( AElements[ i ] );

			FProcessElements.clear();
			Mitov::String	ATextItem;
			Mitov::String	AIndexText;
			bool	AInEscape = false;

//			Serial.println( "INIT" );
//			Serial.println( Text );
//			delay( 1000 );

			uint32_t AAutoIndex = 0;

			for( unsigned int i = 0; i < Text().GetValue().length(); ++ i )
			{
				char AChar = Text().GetValue()[ i ];
				if( AInEscape )
				{
					if( AChar >= '0' && AChar <= '9' )
						AIndexText += AChar;

					else
					{
						if( AChar == '%' )
						{
							if( AIndexText.length() == 0 )
							{
								ATextItem += '%';
								AInEscape = false;
							}

							else
							{
////								AddReadyElement( ATextItem, AIndexText.toInt() );
								ATextItem = "";
								AIndexText = "";
							}

						}

						else
						{
							if( AIndexText.length() == 0 )
								AIndexText = AAutoIndex ++;

////							if( AIndexText.length() == 0 )
////								AddReadyElement( ATextItem, AAutoIndex ++ );

////							else
////								AddReadyElement( ATextItem, AIndexText.toInt() );

							ATextItem = AChar;
						}

						if( AChar != '%' )
							AInEscape = false;
					}
				}

				else
				{
					if( AChar == '%' )
					{
						if( AIndexText.length() != 0 )
							AddReadyElement( ATextItem, AIndexText.toInt() );

						else
							AddReadyElement( ATextItem, -1 );

						AInEscape = true;
						AIndexText = "";
					}

					else
						ATextItem += AChar;

				}

			}

			if( AInEscape )
			{
				if( AIndexText.length() == 0 )
					AddReadyElement( "", AAutoIndex );

				else
					AddReadyElement( "", AIndexText.toInt() );
			}

			else if( ATextItem.length() )
			{
				if( AIndexText.length() != 0 )
					AddReadyElement( ATextItem, AIndexText.toInt() );

				else
					AddReadyElement( ATextItem, -1 );

////				TStringItem	*AItem = new TStringItem;

////				AItem->Text = ATextItem;
////				AItem->Index = -1;

////				FReadyElements.push_back( AItem );
			}

//			Serial.println( "DEBUG>>" );
//			for( Mitov::SimpleList<TStringItem *>::iterator Iter = FReadyElements.begin(); Iter != FReadyElements.end(); ++Iter )
//				Serial.println( ( *Iter )->Text );

//			Serial.println( "<<DEBUG" );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char *)_Data;

			if( C_COUNT > 0 )
			{
				bool ACanSend = true;

				TSplitTextStringItem *AElements = FProcessElements.AsPointer();
				uint32_t ASize = FProcessElements.size();
				for( int i = 0; i < ASize; ++i )
				{
					if( AElements[ i ].Index < 0 )
						AElements[ i ].ProcessTextOnly( AText, ACanSend );

					else
						T_Elements_Process::Call( AElements[ i ], AText, ACanSend );

				}

				if( ! IngnoreEnd().GetValue() )
					if( *AText )
						ACanSend = false; // There is remaining text!

				if( ! AllowPartial().GetValue() )
					for( int i = 0; i < ASize; ++i )
					{
//						Serial.println( AElements[ i ].Index );
						if( AElements[ i ].Index >= 0 )
							T_Elements_SendData::Call( AElements[ i ], ACanSend );

					}

			}
/*
			bool ACanSend = true;
			T_Elements_Process::Call( AText, ACanSend );
			if( *AText )
				ACanSend = false; // There is remaining text!

			T_Elements_SendData::Call( ACanSend );
*/
		}

	public:
		inline void SystemStart()
		{
			InitElements();
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class TextFormatElementInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( Mitov::String( *(T*)_Data ));
		}
	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class TextFormatElementInput<T_LCD, C_LCD, uint64_t>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( Func::UInt64ToString( *(uint64_t*)_Data ) );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class TextFormatElementInput<T_LCD, C_LCD, int64_t>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( Func::Int64ToString( *(int64_t*)_Data ) );
		}
	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T> class TextFormatElementInput_String
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( (char*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD> class TextFormatElementInput_Clock
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( "(Clock)" );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T_OBJECT> class TextFormatElementInput_Object
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( ((T_OBJECT *)_Data)->ToString() );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_LCD, T_LCD &C_LCD, typename T_OBJECT> class TextFormatElementInput_Binary
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( ((T_OBJECT *)_Data)->ToString() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FModified,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length
	> class FormattedTextElementText :
		public T_FModified,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length
	{
	public:
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )

	protected:
		_V_PROP_( FModified )

	public:
		void SetValue( Mitov::String AValue )
		{
//			Serial.println( AValue );
			FModified() = true;
			C_OWNER.SetModified();
//			FModified = true;
			InitialValue() = AValue;
		}

	public:
		inline Mitov::String GetText()
		{
			return InitialValue();
		}

		inline void AppendText( Mitov::String &AText )
		{
			AText += InitialValue().GetValue();
		}

		inline void Processed()
		{
			FModified() = false;
		}

		inline void CanProcess( bool &AResult )
		{
			AResult &= FModified().GetValue();
		}

	public:
		inline void SystemStart()
		{
		}

	public:
		inline FormattedTextElementText()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T> class FormattedTextLengthElementTyped : public T
	{
		typedef T inherited;

	public:
		inline Mitov::String GetText()
		{
			Mitov::String AResult = inherited::GetText();
			for( int i = AResult.length(); i < inherited::Length(); ++i )
				AResult = inherited::FillCharacter().GetValue() + AResult;

			return AResult;
		}

		inline void AppendText( Mitov::String &AText )
		{
			AText += GetText();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Base,
		typename T_FModified,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length
	> class FormattedTextElementInteger :
		public T_Base,
		public T_FModified,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length
	{
	public:
		_V_PROP_( Base )
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )

	protected:
		_V_PROP_( FModified )

	public:
		inline Mitov::String GetText()
		{
			char AText[ 16 ];
			ltoa( InitialValue(), AText, Base() );
			return AText;
		}

		inline void AppendText( Mitov::String &AText )
		{
			AText += GetText();
		}

		inline void Processed()
		{
			FModified() = false;
		}

		inline void CanProcess( bool &AResult )
		{
			AResult &= FModified().GetValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t*)_Data;
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			FModified() = true;
			C_OWNER.SetModified();
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	public:
		inline FormattedTextElementInteger()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Base,
		typename T_FModified,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length
	> class FormattedTextElementUnsigned :
		public T_Base,
		public T_FModified,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length
	{
	public:
		_V_PROP_( Base )
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )

	protected:
		_V_PROP_( FModified )

	public:
		inline Mitov::String GetText()
		{
			char AText[ 16 ];
			ultoa( InitialValue(), AText, Base().GetValue() );
			return AText;
//			Serial.println( AText );
		}

		inline void AppendText( Mitov::String &AText )
		{
			AText += GetText();
		}

		inline void Processed()
		{
			FModified() = false;
		}

		inline void CanProcess( bool &AResult )
		{
			AResult &= FModified().GetValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			uint32_t AValue = *(uint32_t*)_Data;
			//Serial.println( AValue );
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			FModified() = true;
			//Serial.println( InitialValue().GetValue() );
			C_OWNER.SetModified();
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	public:
		inline FormattedTextElementUnsigned()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FModified,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length,
		typename T_MinWidth,
		typename T_Precision
	> class FormattedTextElementAnalog :
		public T_FModified,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length,
		public T_MinWidth,
		public T_Precision
	{
	public:
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )
		_V_PROP_( MinWidth )
		_V_PROP_( Precision )

	protected:
		_V_PROP_( FModified )

	public:
		inline Mitov::String GetText()
		{
			char AText[ 16 ];
			dtostrf( InitialValue(),  MinWidth(), Precision(), AText );
//			Serial.println( AText );

			return AText;
		}

		inline void AppendText( Mitov::String &AText )
		{
			AText += GetText();
		}

		inline void Processed()
		{
			FModified() = false;
		}

		inline void CanProcess( bool &AResult )
		{
			AResult &= FModified().GetValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float*)_Data;
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			FModified() = true;
			C_OWNER.SetModified();
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}


	public:
		inline FormattedTextElementAnalog()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FModified,
		typename T_FalseValue,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length,
		typename T_TrueValue
	> class FormattedTextElementDigital :
		public T_FModified,
		public T_FalseValue,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length,
		public T_TrueValue
	{
	public:
		_V_PROP_( FillCharacter )
		_V_PROP_( Length )
		_V_PROP_( TrueValue )
		_V_PROP_( FalseValue )
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( FModified )

	public:
		inline Mitov::String GetText()
		{
			if( InitialValue() )
				return TrueValue();

			return FalseValue();
		}

		inline void AppendText( Mitov::String &AText )
		{
			AText += GetText();
		}

		inline void Processed()
		{
			FModified() = false;
		}

		inline void CanProcess( bool &AResult )
		{
			AResult &= FModified().GetValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool*)_Data;
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			FModified() = true;
			C_OWNER.SetModified();
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	public:
		inline FormattedTextElementDigital()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TextLength :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			int32_t AValue = strlen( (char *)_Data );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class FindTextPosition :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			int32_t AValue = ATextValue.indexOf( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class FindLastTextPosition :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			int32_t AValue = ATextValue.lastIndexOf( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class ContainsText :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			bool AValue = ( ATextValue.indexOf( AText ) >= 0 );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class TextStartsWith :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			bool AValue = ATextValue.startsWith( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class TextEndsWith :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			bool AValue = ATextValue.endsWith( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FromValue,
//		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_ToValue
	> class ReplaceText :
		public T_Enabled,
		public T_FromValue,
//		public T_IgnoreCase,
		public T_OutputPin,
		public T_ToValue
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( FromValue )
		_V_PROP_( ToValue )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = (char *)_Data;

			AValue.replace( FromValue(), ToValue() );

			T_OutputPin::SetPinValue( AValue, false );
        }
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin,
		typename T_Position,
		typename T_WrapAround
	> class SubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin,
		public T_Position,
		public T_WrapAround
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )
		_V_PROP_( Position )
		_V_PROP_( WrapAround )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue;
			if( WrapAround() )
			{
				Mitov::String AInValue = Mitov::String( (char *)_Data );
				uint32_t AInLength = AInValue.length();
				uint32_t ALength = Length();
				uint32_t APosition = Position();
				while( ALength )
				{
					Mitov::String ASubValue = AInValue.substring( APosition, APosition + ALength );
					AValue += ASubValue;
					uint32_t ASublenght = ASubValue.length();
					APosition += ASublenght;
					ALength -= ASublenght;
					if( APosition >= AInLength )
						APosition -= AInLength;

				}
			}

			else
				AValue = Mitov::String( (char *)_Data ).substring( Position(), Position() + Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class LeftSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data ).substring( 0, Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class RightSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue = AValue.substring( AValue.length() - Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin,
		typename T_Position
	> class DeleteSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin,
		public T_Position
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )
		_V_PROP_( Position )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.remove( Position(), Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class DeleteLeftSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.remove( 0, Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class DeleteRightSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.remove( AValue.length() - Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class ToUpperCase :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
				T_OutputPin::PinNotify( _Data );
                return;
            }

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.toUpperCase();

			T_OutputPin::SetPinValue( AValue, false );
        }
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class ToLowerCase :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
				T_OutputPin::PinNotify( _Data );
                return;
            }

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.toLowerCase();

			T_OutputPin::SetPinValue( AValue, false );
        }
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TrimText :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
				T_OutputPin::PinNotify( _Data );
                return;
            }

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.trim();

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

