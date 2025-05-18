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
#include <Mitov_Graphics.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __NEXTION__DEBUG__

namespace Mitov
{
#define Min Min
#define Max Max

	class ArduinoDisplayNextionElementBasicElement;
//---------------------------------------------------------------------------
	struct TDisplayNextionResponseElementBasic
	{
	public:
		virtual bool ProcessResponse( uint8_t *ABuffer ) { return false; }
		virtual bool ProcessTextResponse( Mitov::String AText ) { return false; }

	public:
		virtual ~TDisplayNextionResponseElementBasic() {}
	};
//---------------------------------------------------------------------------
	class ArduinoDisplayNextionElementBasicElement
	{
	public:
		virtual void Render() {}
		virtual void SendRequest() {}
		virtual uint8_t GetID() { return -1; }
		virtual uint8_t GetPageNumber() { return -1; }

	public:
		void ActivatePageNo( uint8_t APageNo ) 
		{
			if( GetPageNumber() == APageNo )
				Render();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_Identified,
		typename T_PageIdentified,
		typename T_PageIndex
	> class ArduinoDisplayNextionElementBasic : public ArduinoDisplayNextionElementBasicElement,
		public T_ElementID,
		public T_ElementName,
		public T_Identified,
		public T_PageIdentified,
		public T_PageIndex
	{
		typedef ArduinoDisplayNextionElementBasicElement inherited;

	protected:
		struct TResponseSendMe : public TDisplayNextionResponseElementBasic
		{
		public:
			ArduinoDisplayNextionElementBasic *FOwner;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
#ifdef __NEXTION__DEBUG__
				Serial.println( "Page Number TEST" );
#endif //__NEXTION__DEBUG__

				if( ABuffer[ 0 ] != 0x66 )
					return false;

#ifdef __NEXTION__DEBUG__
				Serial.print( "Page Number: " ); Serial.println( ABuffer[ 1 ] );
				Serial.println( FOwner->ElementName() );
#endif //__NEXTION__DEBUG__
				FOwner->PageIndex() = ABuffer[ 1 ];
				FOwner->PageIdentified() = true;
//				if( FOwner->FHasData )
					FOwner->Render();

				return true;
			}

		public:
			TResponseSendMe( ArduinoDisplayNextionElementBasic *AOwner ) :
				FOwner( AOwner )
			{
			}
		};

		struct TResponseElementGetID : public TDisplayNextionResponseElementBasic
		{
		public:
			ArduinoDisplayNextionElementBasic *FOwner;
			uint8_t	FExpectedCode;
			bool	FIsPageTest;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
#ifdef __NEXTION__DEBUG__
				Serial.println( "ID TEST" );
#endif //__NEXTION__DEBUG__
				if( ABuffer[ 0 ] != FExpectedCode )
					return false;

#ifdef __NEXTION__DEBUG__
				Serial.print( "PageIdentified: " );
				Serial.println( FOwner->PageIdentified() );
#endif //__NEXTION__DEBUG__

				if( ! FIsPageTest )
					if( FOwner->PageIdentified() )
						if( FOwner->PageIndex().GetValue() != C_OWNER.GetActivePageNumber() )
							return false;

#ifdef __NEXTION__DEBUG__
				Serial.print( "ID: " ); Serial.println( ABuffer[ 1 ] );
				Serial.println( FOwner->ElementName().GetValue() );
#endif //__NEXTION__DEBUG__
				FOwner->ElementID() = ABuffer[ 1 ];
				FOwner->Identified() = true;
				if( ! FOwner->PageIdentified() )
					C_OWNER.AddRequestFront( "sendme", FOwner->PageIndex().GetValue(), new TResponseSendMe( FOwner ));

				return true;
			}

		public:
			TResponseElementGetID( ArduinoDisplayNextionElementBasic *AOwner, uint8_t AExpectedCode, bool AIsPageTest ) :
				FOwner( AOwner ),
				FExpectedCode( AExpectedCode ),
				FIsPageTest( AIsPageTest )
			{
			}
		};

	public:
		_V_PROP_( PageIndex )

		_V_PROP_( ElementID )
		_V_PROP_( ElementName )
		_V_PROP_( Identified )
		_V_PROP_( PageIdentified )

//	public:
//		virtual bool RequestInfo() { return false; }

	public:
		virtual uint8_t GetID() override
		{
			return ElementID().GetValue();
		}

		virtual uint8_t GetPageNumber() override
		{
			return PageIndex().GetValue();
		}

		inline T_OWNER &GetOwner()
		{
			return C_OWNER;
		}

	public:
		Mitov::String GetElementName() 
		{ 
			return C_OWNER.GetPageNamePath() + ElementName().GetValue();
		}

	protected:
		void RequestElementID( Mitov::String AName )
		{
#ifdef __NEXTION__DEBUG__
			Serial.print( "RequestElementID: " ); Serial.println( AName );
#endif //__NEXTION__DEBUG__

			C_OWNER.AddRequest( Mitov::String( "get " ) + AName + ".id", PageIndex(), new TResponseElementGetID( this, 0x71, false ));
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	public:
		ArduinoDisplayNextionElementBasic()
//			inherited( APageID ),
//			ElementID( AElementID ),
		{
			Identified() = ( ElementID().GetValue() >= 0 );
			PageIdentified() = ( PageIndex().GetValue() >= 0 );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color
	> class DisplayNextionElementFillScreen : public ArduinoDisplayNextionElementBasicElement,
		public T_Color
	{
	public:
		_V_PROP_( Color )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
///			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			C_OWNER.SendCommand( ( Mitov::String( "cls " ) + Color().GetValue().BGRTo565Color() ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Height,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementFillRectangle : public ArduinoDisplayNextionElementBasicElement,
		public T_Color,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			C_OWNER.SendCommand( ( Mitov::String( "fill " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + Width() + "," + Height() + "," + Color().GetValue().BGRTo565Color() ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Height,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementDrawRectangle : public ArduinoDisplayNextionElementBasicElement,
		public T_Color,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			C_OWNER.SendCommand( ( Mitov::String( "draw " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + ( AParentX + X() + Width() ) + "," + ( AParentY + Y() + Height() ) + "," + Color().GetValue().BGRTo565Color() ).c_str() );
		}

	};
//---------------------------------------------------------------------------	
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Crop,
		typename T_Height,
		typename T_PictureIndex,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementDrawPicture : public ArduinoDisplayNextionElementBasicElement,
		public T_Crop,
		public T_Height,
		public T_PictureIndex,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( Crop )
		_V_PROP_( PictureIndex )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			if( Crop() )
				C_OWNER.SendCommand( ( Mitov::String( "picq " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + Width().GetValue() + "," + Height().GetValue() + "," + PictureIndex().GetValue() ).c_str() );

			else
				C_OWNER.SendCommand( ( Mitov::String( "pic " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + PictureIndex().GetValue() ).c_str() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Controls_ActivatePageNo,
		typename T_Controls_Render,
		typename T_Controls_ResetEvent,
		typename T_Controls_SendRequest,
		typename T_Controls_TryProcessEvent,
		typename T_ElementID,
		typename T_ElementName,
		typename T_Identified,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementActivatePage :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> //, public DisplayNextionIntf
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		inline void ResetEvent() 
		{
			T_Controls_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Controls_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		void ActivatePageNo( uint8_t APageNo )
		{
			T_Controls_ActivatePageNo::Call( APageNo );
		}

	public:
		virtual void SendRequest() override
		{
			T_Controls_SendRequest::Call();
		}

		void ActiveatePage( ArduinoDisplayNextionElementBasicElement *APage )
		{
			C_OWNER.ActiveatePage( APage );
		}

		bool IsPageActive( ArduinoDisplayNextionElementBasicElement *APage )
		{
			return C_OWNER.IsPageActive( APage );
		}

		void AddRequest( Mitov::String ARequest, int8_t APage, TDisplayNextionResponseElementBasic *AResponse )
		{
			C_OWNER.AddRequest( ARequest, APage, AResponse );
		}

		void AddRequestFront( Mitov::String ARequest, int8_t APage, TDisplayNextionResponseElementBasic *AResponse )
		{
			C_OWNER.AddRequestFront( ARequest, APage, AResponse );
		}

/*
		void RegisterEvents( DisplayNextionEventsIntf *AEvents )
		{
			C_OWNER.RegisterEvents( AEvents );
		}
*/
		void SendCommand( const char *ACommand )
		{
			if( C_OWNER.IsPageActive( this ))
				C_OWNER.SendCommand( ACommand );

		}

		inline uint8_t GetActivePageNumber()
		{
			return C_OWNER.GetActivePageNumber();
		}

		inline bool GetIsStarted() { return C_OWNER.GetIsStarted(); }



//		virtual	uint8_t GetPageNumber() override
//		{
//			return ID;
//		}

		inline void RetryRequests()
		{
			C_OWNER.RetryRequests();
		}

		inline Mitov::String GetPageNamePath()
		{
			return inherited::ElementName().GetValue() + ".";
		}

/*
		virtual void SendRequest() override
		{
			if( ! Identified )
				FOwner.AddRequest( "sendme", new TResponseElementGetID( this ));

			inherited::SendRequest();
		}
*/
	public:
		virtual void Render() override
		{
#ifdef __NEXTION__DEBUG__
			Serial.print( "ACTIVATE: " );
			Serial.println( ElementName().GetValue() );
#endif // __NEXTION__DEBUG__
			C_OWNER.ActiveatePage( this );
//			TGraphicsPos AParentX, AParentY;
//			C_OWNER.GetPosition( AParentX, AParentY );
			C_OWNER.SendCommand( ( Mitov::String( "page " ) + inherited::ElementName().GetValue() ).c_str() );

			///////////////// CUSES MEMORY PROBLEMS!!!!!!!!
			if( ! inherited::Identified() )
			{
				inherited::PageIdentified() = true;
				C_OWNER.AddRequest( "sendme", -1, new struct inherited::TResponseElementGetID( this, 0x66, true ));
			}

//			C_OWNER.SendCommand( "sendme"
			T_Controls_Render::Call();
			C_OWNER.RetryRequests();
		}

/*
		virtual void SendRequest() override
		{
			if( ! Identified )
				sendme
//				RequestElementID( ElementName );

			inherited::SendRequest();
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_Angle,
		typename T_Value
	> class TArduinoDisplayNextionAnalogGaugeMinValue :
		public T_Angle,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( Angle )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_FHasData,
		typename T_FUpdatedOnce,
		typename T_Identified,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementAnalogGauge : 
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>,
		public T_FHasData,
		public T_FUpdatedOnce,
		public T_Value_Max,
		public T_Value_Min
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

	public:
		_V_PROP_( FHasData )

	protected:
		_V_PROP_( FUpdatedOnce )

	protected:
		float	FValue	= 0.0f;

//	public:
//		virtual bool RequestInfo() override;

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			if( ! FHasData() )
				return;

//			Serial.println( "STEP1" );
			if( ! inherited::Identified() )
				return;

//			Serial.println( "STEP2" );
			if( ! inherited::PageIdentified() )
				return;

//			Serial.println( "STEP3" );
			if( inherited::PageIndex() != C_OWNER.GetActivePageNumber() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			int16_t AAngle = Func::MapRange( MitovConstrain( FValue, Value_Min().Value().GetValue(), Value_Max().Value().GetValue() ), Value_Min().Value().GetValue(), Value_Max().Value().GetValue(), float( Value_Min().Angle().GetValue() ), float( Value_Max().Angle().GetValue() ) ) + 0.5;
			if( AAngle < 0 )
				AAngle += 360;

//			Serial.println( AAngle );
			C_OWNER.SendCommand( ( inherited::GetElementName() + ".val=" + AAngle ).c_str() );
			FUpdatedOnce() = true;
		}

	public:
		virtual void SendRequest() override
		{
//			Serial.println( "TEST5" );
//			Serial.println( ElementName );
//			Serial.println( GetElementName() );
			if( ! inherited::Identified() )
				inherited::RequestElementID( inherited::GetElementName() );

			inherited::SendRequest();
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			if( FUpdatedOnce() )
				if( FValue == *(float*)_Data )
					return;

			FValue = *(float*)_Data;
			FHasData() = true;
			Render();
		}

	public:
		inline DisplayNextionElementAnalogGauge()
		{
			FHasData() = false;
			FUpdatedOnce() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_FHasData,
		typename T_FUpdatedOnce,
		typename T_Identified,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementAnalogProgressBar :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>,
		public T_FHasData,
		public T_FUpdatedOnce,
		public T_Value_Max,
		public T_Value_Min
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

	public:
		_V_PROP_( FHasData )

	protected:
		_V_PROP_( FUpdatedOnce )

	protected:
		float	FValue = 0.0f;

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			if( ! FHasData() )
				return;

			if( ! inherited::Identified() )
				return;

			if( ! inherited::PageIdentified() )
				return;

			if( inherited::PageIndex() != C_OWNER.GetActivePageNumber() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			int16_t AAngle = Func::MapRange( MitovConstrain( FValue, Value_Min().GetValue(), Value_Max().GetValue() ), Value_Min().GetValue(), Value_Max().GetValue(), 0.0f, 100.0f ) + 0.5;
			C_OWNER.SendCommand( ( inherited::GetElementName() + ".val=" + AAngle ).c_str() );
			FUpdatedOnce() = true;
		}

	public:
		virtual void SendRequest() override
		{
			if( ! inherited::Identified() )
				inherited::RequestElementID( inherited::GetElementName() );

			inherited::SendRequest();
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			if( FUpdatedOnce() )
				if( FValue == *(float*)_Data )
					return;

			FValue = *(float*)_Data;
			FHasData() = true;
			Render();
		}

	public:
		inline DisplayNextionElementAnalogProgressBar()
		{
			FHasData() = false;
			FUpdatedOnce() = false;
		}


	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_FHasData,
		typename T_FUpdatedOnce,
		typename T_Identified,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementText :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>,
		public T_FHasData,
		public T_FUpdatedOnce
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PROP_( FHasData )

	protected:
		_V_PROP_( FUpdatedOnce )

	protected:
		Mitov::String	FValue;

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			if( ! FHasData() )
				return;

//			Serial.println( "STEP1" );
			if( ! inherited::Identified() )
				return;

//			Serial.println( "STEP2" );
			if( ! inherited::PageIdentified() )
				return;

//			Serial.println( "STEP3" );
			if( inherited::PageIndex() != C_OWNER.GetActivePageNumber() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "RENDER"  );
			C_OWNER.SendCommand( ( inherited::GetElementName() + ".txt=\"" + FValue + "\"" ).c_str() );
			FUpdatedOnce() = true;
		}

	public:
		virtual void SendRequest() override
		{
			if( ! inherited::Identified() )
				inherited::RequestElementID( inherited::GetElementName() );

			inherited::SendRequest();
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
//			Serial.println( "T1" );
			Mitov::String AValue = (char *)_Data;
			if( FUpdatedOnce() )
				if( FValue == AValue )
					return;

#ifdef __NEXTION__DEBUG__
			Serial.println( __FUNCTION__ );
#endif //__NEXTION__DEBUG__

			FValue = AValue;
			FHasData() = true;
			Render();
		}

	public:
		inline DisplayNextionElementText()
		{
			FHasData() = false;
			FUpdatedOnce() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_FHasData,
		typename T_FUpdatedOnce,
		typename T_Identified,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementNumber :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>,
		public T_FHasData,
		public T_FUpdatedOnce
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PROP_( FHasData )

	protected:
		_V_PROP_( FUpdatedOnce )

	protected:
		uint32_t	FValue = 0;

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			if( ! FHasData() )
				return;

			if( ! inherited::Identified() )
				return;

			if( ! inherited::PageIdentified() )
				return;

			if( inherited::PageIndex() != C_OWNER.GetActivePageNumber() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			C_OWNER.SendCommand( ( inherited::GetElementName() + ".val=" + FValue ).c_str() );
			FUpdatedOnce() = true;
		}

	public:
		virtual void SendRequest() override
		{
			if( ! inherited::Identified() )
				inherited::RequestElementID( inherited::GetElementName() );

			inherited::SendRequest();
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			if( FUpdatedOnce() )
				if( FValue == *(uint32_t*)_Data )
					return;

			FValue = *(uint32_t*)_Data;
			FHasData() = true;
			Render();
		}

	public:
		inline DisplayNextionElementNumber()
		{
			FHasData() = false;
			FUpdatedOnce() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_FHasData,
		typename T_FUpdatedOnce,
		typename T_Identified,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementIntegerVariable :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>,
		public T_FHasData,
		public T_FUpdatedOnce
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PROP_( FHasData )

	protected:
		_V_PROP_( FUpdatedOnce )

	protected:
		int32_t	FValue = 0;

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			if( ! FHasData() )
				return;

			if( ! inherited::Identified() )
				return;

			if( ! inherited::PageIdentified() )
				return;

			if( inherited::PageIndex() != C_OWNER.GetActivePageNumber() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			C_OWNER.SendCommand( ( inherited::GetElementName() + ".val=" + FValue ).c_str() );
			FUpdatedOnce() = true;
		}

	public:
		virtual void SendRequest() override
		{
			if( ! inherited::Identified() )
				inherited::RequestElementID( inherited::GetElementName() );

			inherited::SendRequest();
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			if( FUpdatedOnce() )
				if( FValue == *(int32_t*)_Data )
					return;

			FValue = *(int32_t*)_Data;
			FHasData() = true;
			Render();
		}

	public:
		inline DisplayNextionElementIntegerVariable()
		{
			FHasData() = false;
			FUpdatedOnce() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_FValue,
		typename T_Identified,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementTimer :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>,
		public T_FValue
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	protected:
		_V_PROP_( FValue )

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			if( ! inherited::Identified() )
				return;

			if( ! inherited::PageIdentified() )
				return;

			if( inherited::PageIndex() != C_OWNER.GetActivePageNumber() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

			C_OWNER.SendCommand( ( inherited::GetElementName() + ".en=" + (( FValue().GetValue() ) ? "1" : "0" ) ).c_str() );
		}

	public:
		virtual void SendRequest() override
		{
			if( ! inherited::Identified() )
				inherited::RequestElementID( inherited::GetElementName() );

			inherited::SendRequest();
		}

	public:
		inline void EnabledInputPin_o_Receive( void * _Data )
		{
//			if( FValue == *(bool*)_Data )
//				return;

			FValue() = *(bool*)_Data;
			Render();
		}

	public:
		inline DisplayNextionElementTimer()
		{
			FValue() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Radius,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementFillCircle : public ArduinoDisplayNextionElementBasicElement,
		public T_Color,
		public T_Radius,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Radius )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			C_OWNER.SendCommand( ( Mitov::String( "cirs " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + Radius() + "," + Color().GetValue().BGRTo565Color() ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Radius,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementDrawCircle : public ArduinoDisplayNextionElementBasicElement,
		public T_Color,
		public T_Radius,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Radius )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			C_OWNER.SendCommand( ( Mitov::String( "cir " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + Radius() + "," + Color().GetValue().BGRTo565Color() ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Color,
		typename T_Height,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementDrawLine : public ArduinoDisplayNextionElementBasicElement,
		public T_Color,
		public T_Height,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			C_OWNER.SendCommand( ( Mitov::String( "line " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + ( AParentX + X() + Width() ) + "," + ( AParentY + Y() + Height() ) + "," + Color().GetValue().BGRTo565Color() ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Angle,
		typename T_Begin,
		typename T_Color,
		typename T_End,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementDrawAngledLine : public ArduinoDisplayNextionElementBasicElement,
		public T_Angle,
		public T_Begin,
		public T_Color,
		public T_End,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Angle )
		_V_PROP_( Begin )
		_V_PROP_( End )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			float ARadians = Angle() * PI / 180.0f;
//			T_OWNER::drawLine( C_OWNER, AParentX + X() + Begin() * cos( ARadians ), AParentY + Y() + Begin() * sin( ARadians ), AParentX + X() + End() * cos( ARadians ), AParentY + Y() + End() * sin( ARadians ), Color().GetValue() );

			C_OWNER.SendCommand( ( Mitov::String( "line " ) + ( AParentX + X() + Begin() * cos( ARadians ) ) + "," + ( AParentY + Y() + Begin() * sin( ARadians ) ) + "," + ( AParentX + X() + End() * cos( ARadians ) ) + "," + ( AParentY + Y() + End() * sin( ARadians ) ) + "," + Color().GetValue().BGRTo565Color() ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	enum TArduinoDisplayNextionTextBackgroundMode { bmClear, bmColor, bmPictureCrop, bmPicture };
//---------------------------------------------------------------------------
	template <
		typename T_Color,
		typename T_Mode,
		typename T_PictureIndex
	> class TArduinoDisplayNextionTextBackground :
		public T_Color,
		public T_Mode,
		public T_PictureIndex
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( PictureIndex )
		_V_PROP_( Mode )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Background,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Color,
		typename T_FontIndex,
		typename T_Height,
		typename T_HorizontalAlign,
		typename T_InitialValue,
		typename T_VerticalAlign,
		typename T_Width,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementDrawText : public ArduinoDisplayNextionElementBasicElement,
		public T_Background,
		public T_ClockInputPin_o_IsConnected,
		public T_Color,
		public T_FontIndex,
		public T_Height,
		public T_HorizontalAlign,
		public T_InitialValue,
		public T_VerticalAlign,
		public T_Width,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( Color )
		_V_PROP_( Background )
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Width )
		_V_PROP_( Height )
		_V_PROP_( FontIndex )
		_V_PROP_( HorizontalAlign )
		_V_PROP_( VerticalAlign )
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		template <typename T> void Print( T AValue )
		{
			 InitialValue() = Mitov::String( AValue );
			 if( ! ClockInputPin_o_IsConnected() )
				 Render();
		}

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		virtual void Render() override
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );
			Mitov::String ABackgroundColorText;
			Mitov::String ABackgroundTypeText;
			switch( Background().Mode().GetValue() )
			{
				case bmClear:
					ABackgroundColorText = "NULL";
					ABackgroundTypeText = "1";
					break;

				case bmColor:
					ABackgroundColorText = Mitov::String( Background().Color().GetValue().BGRTo565Color());
					ABackgroundTypeText = "1";
					break;

				case bmPictureCrop:
					ABackgroundColorText = Mitov::String( Background().PictureIndex() );
					ABackgroundTypeText = "0";
					break;

				case bmPicture:
					ABackgroundColorText = Mitov::String( Background().PictureIndex() );
					ABackgroundTypeText = "2";
					break;

			}

			C_OWNER.SendCommand( ( Mitov::String( "xstr " ) + ( AParentX + X() ) + "," + ( AParentY + Y() ) + "," + Width() + "," + Height() + "," + FontIndex() + "," + Color().GetValue().BGRTo565Color() + "," + ABackgroundColorText + "," + uint16_t( HorizontalAlign().GetValue() ) + "," + uint16_t( VerticalAlign().GetValue() ) + "," + ABackgroundTypeText + ",\"" + InitialValue().GetValue() + "\"" ).c_str() );
//			C_OWNER.SendCommand( ( Mitov::String( "xstr " ) + ( AParentX + X ) + "," + ( AParentY + Y ) + "," + Width + "," + Height + "," + FontIndex + "," + Color.BGRTo565Color() + ",NULL," + uint16_t( HorizontalAlign ) + "," + uint16_t( VerticalAlign ) + ",1,\"" + InitialValue + "\"" ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_ActivatePageNo,
		typename T_Elements_Render,
		typename T_Elements_ResetEvent,
		typename T_Elements_SendRequest,
		typename T_Elements_TryProcessEvent,
		typename T_X,
		typename T_Y
	> class DisplayNextionElementDrawScene :
		public ArduinoDisplayNextionElementBasicElement, // public DisplayNextionIntf,
		public T_X,
		public T_Y
	{
		typedef ArduinoDisplayNextionElementBasicElement inherited;

	public:
		_V_PROP_( X )
		_V_PROP_( Y )

//	protected:
//		Mitov::SimpleList<ArduinoDisplayNextionElementBasicElement *>	FElements;

/*
	public:
		virtual uint8_t GetID() override
		{
			return -1;
		}

		virtual uint8_t GetPageNumber() override
		{
			return -1;
		}
*/
	public:
		inline void ResetEvent() 
		{
			T_Elements_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Elements_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		inline void ActiveatePage( ArduinoDisplayNextionElementBasicElement *APage )
		{
			C_OWNER.ActiveatePage( APage );
		}

		inline bool IsPageActive( ArduinoDisplayNextionElementBasicElement *APage )
		{
			return C_OWNER.IsPageActive( APage );
		}

		inline void SendCommand( const char *ACommand )
		{
			C_OWNER.SendCommand( ACommand );
		}

		inline uint8_t GetActivePageNumber()
		{
			return C_OWNER.GetActivePageNumber();
		}

		inline bool GetIsStarted() { return C_OWNER.GetIsStarted(); }

		inline void RetryRequests()
		{
			C_OWNER.RetryRequests();
		}

		inline void AddRequest( Mitov::String ARequest, int8_t APage, TDisplayNextionResponseElementBasic *AResponse )
		{
			C_OWNER.AddRequest( ARequest, APage, AResponse );
		}

		inline void AddRequestFront( Mitov::String ARequest, int8_t APage, TDisplayNextionResponseElementBasic *AResponse )
		{
			C_OWNER.AddRequestFront( ARequest, APage, AResponse );
		}

	public:
		inline constexpr const char * GetPageNamePath() { return ""; }

		void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{ 
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			AX = AParentX + X();
			AY = AParentY + Y();
		}

	public:
		virtual void Render() override
		{
			T_Elements_Render::Call();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Elements_ActivatePageNo,
		typename T_Elements_Render,
		typename T_Elements_ResetEvent,
		typename T_Elements_SendRequest,
		typename T_Elements_TryProcessEvent,
		typename T_FStarted,
		typename T_ResetDisplay
	> class DisplayNextion : // public DisplayNextionIntf,
		public T_FStarted,
		public T_ResetDisplay
	{
	protected:
		ArduinoDisplayNextionElementBasicElement *FActivePage = nullptr;

	protected:
		struct TRequestElement
		{
		public:
			Mitov::String						Request;
			TDisplayNextionResponseElementBasic *Response;
			int8_t								Page;

		public:
			TRequestElement( Mitov::String ARequest, int8_t APage, TDisplayNextionResponseElementBasic *AResponse ) :
				Request( ARequest ),
				Response( AResponse ),
				Page( APage )
			{
			}
		};

		struct TResponseReset : public TDisplayNextionResponseElementBasic
		{
//		public:
//			DisplayNextion *FOwner;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
				if( ABuffer[ 0 ] != 0x88 )
					return false;

#ifdef __NEXTION__DEBUG__
				Serial.println( "RESET Confirmed" );
#endif // __NEXTION__DEBUG__

				return true;
			}

/*
		public:
			TResponseReset( DisplayNextion *AOwner ) :
				FOwner( AOwner )
			{
			}
*/
		};

	public:
//		Mitov::SimpleObjectList<DisplayNextionEventsIntf *>	FEventElements;
		Mitov::SimpleObjectDeleteList<TRequestElement *>	FRequestElements;
		TDisplayNextionResponseElementBasic *FLastResponse = nullptr;
//		Mitov::SimpleObjectDeleteList<TDisplayNextionResponseElementBasic *>	FResponseHandlersQueue;

//	protected:
//		Mitov::SimpleList<ArduinoDisplayNextionElementBasicElement *>	FElements;

	public:
		_V_PROP_( ResetDisplay )

	protected:
		_V_PROP_( FStarted )

		uint8_t			FBuffer[ 10 ];
		uint8_t			FIndex = 0;
		uint8_t			FLength = 0;
		unsigned long	FLastTime = 0;
		Mitov::String	FCurrentRequest;
		Mitov::String	FResultText;
		uint8_t			FActivePageNumber = 0;

	public:
		inline bool GetIsStarted() { return FStarted(); }

	public:
		inline constexpr const char * GetPageNamePath() { return ""; }

		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{
			AX = 0;
			AY = 0; 
		}

		void ActiveatePage( ArduinoDisplayNextionElementBasicElement *APage )
		{
			FActivePage = APage;
			if( FActivePage )
				FActivePageNumber = FActivePage->GetID();
		}

		bool IsPageActive( ArduinoDisplayNextionElementBasicElement *APage )
		{
			if( ! FActivePage )
//				if( APage )
//					if( APage->GetElementName() == "page0" )
						return true;

			return ( APage == FActivePage );
		}

		void SendCommand( const char *ACommand )
		{
#ifdef __NEXTION__DEBUG__
			Serial.print( "SendCommand: " ); Serial.println( ACommand );
#endif // __NEXTION__DEBUG__

			C_SERIAL.GetStream().print( ACommand );
			C_SERIAL.GetStream().print( "\xFF\xFF\xFF" );
			FLastTime = millis();
		}

		uint8_t GetActivePageNumber()
		{
			return FActivePageNumber;
		}

		void RetryRequests()
		{
//#ifdef __NEXTION__DEBUG__
//			Serial.print( "FElements: " ); Serial.println( FElements.size() );
//#endif // __NEXTION__DEBUG__

			T_Elements_SendRequest::Call();
//			for( int i = 0; i < FElements.size(); ++i )
//				FElements[ i ]->SendRequest();

//			C_OWNER.RetryRequests();
		}

	protected:
		void ProcessRequestQueue()
		{
#ifdef __NEXTION__DEBUG__
			Serial.println( "ProcessRequestQueue" );
#endif // __NEXTION__DEBUG__

//			if( ! FStarted )
//				return;

//			Serial.println( FRequestElements.size() );
			if( FLastResponse )
			{
#ifdef __NEXTION__DEBUG__
				Serial.println( "Has FLastResponse" );
#endif // __NEXTION__DEBUG__
				return;
			}

#ifdef __NEXTION__DEBUG__
			Serial.println( FRequestElements.size() );
#endif // __NEXTION__DEBUG__
			uint32_t ASkipped = 0;
			while( FRequestElements.size() > ASkipped )
			{
				TDisplayNextionResponseElementBasic *AElement = FRequestElements[ 0 ]->Response;
				Mitov::String ARequest = FRequestElements[ 0 ]->Request;
				int8_t APage = FRequestElements[ 0 ]->Page;
				FRequestElements.pop_front();
				if( APage >= 0 )
					if( APage != FActivePageNumber )
					{
						++ ASkipped;
						FRequestElements.push_back( new TRequestElement( ARequest, APage, AElement ));
						continue;
					}

				FCurrentRequest = ARequest;
				SendCommand( FCurrentRequest.c_str() );

#ifdef __NEXTION__DEBUG__
				Serial.print( "REQUEST: " );
				Serial.print( FCurrentRequest );
				Serial.print( " PAGE: " );
				Serial.println( APage );
#endif // __NEXTION__DEBUG__

				FLastResponse = AElement;
#ifdef __NEXTION__DEBUG__
				Serial.print( "FLastResponse = " );
				Serial.println( uint32_t( FLastResponse ));
#endif // __NEXTION__DEBUG__
				break;
//				FResponseHandlersQueue.push_back( AElement );
//				if( AElement->RequestInfo())
//					break;

//				Mitov::String AText = FRequestElements[ 0 ].ID;
//				FResponseHandlersQueue.push_back( new TResponseElementGetID( FRequestElements[ 0 ] ));
//				FRequestElements.pop_front();
//				SendCommand( AText );
			}
		}

		void ProcessChar( uint8_t AChar )
		{
			if( ResetDisplay() )
				if( ! FStarted() )
				{
					if( AChar != 0x88 )
						return;

					FStarted() = true;
				}

#ifdef __NEXTION__DEBUG__
			Serial.print( "FIndex: " ); Serial.print( FIndex ); Serial.print( " - "); Serial.println( AChar, HEX );
#endif // __NEXTION__DEBUG__

			if( FIndex == 0 )
			{
#ifdef __NEXTION__DEBUG__
				Serial.print( "RESULT CODE: " ); Serial.println( AChar, HEX );
//				Serial.print( "FIndex: " ); Serial.println( FIndex );

				Serial.print( "FLastResponse = " );
				Serial.println( uint32_t( FLastResponse ));

				Serial.println( FCurrentRequest );
#endif // __NEXTION__DEBUG__
				switch( AChar )
				{
					case 0x00: // Invalid instruction
					case 0x03: // Page ID invalid
					case 0x04: // Picture ID invalid
					case 0x05: // Font ID invalid
					case 0x1A: // Variable name invalid
					case 0x1B: // 	Variable operation invalid
						FLength = 4;
						break;

					case 0x65:
						FLength = 7;
						break;

					case 0x66:
						FLength = 5;
						break;

					case 0x67:
						FLength = 9;
						break;

					case 0x68:
						FLength = 9;
						break;

					case 0x70: // String variable data returns
						FLength = 4; // Skip the sring content
						break;

					case 0x71:
						FLength = 8;
//						Serial.println( "FLength = 8" );
						break;

					case 0x86:
						FLength = 4;
						break;

					case 0x87:
						FLength = 4;
						break;

					case 0x88:
						FLength = 4; // ?
						break;

					case 0x89:
						FLength = 4; // ?
						break;

					case 0xFE:
						FLength = 4; // ?
						break;

					default:
						return; // Unknown Command

				}
			}

			if( FIndex > 0 )
				if( FBuffer[ 0 ] == 0x70 ) // String variable data returns
					if( AChar != 0xFF )
					{
						FResultText += char( AChar );
						return;
					}

			FBuffer[ FIndex ++ ] = AChar;
			if( FIndex < FLength )
				return;

#ifdef __NEXTION__DEBUG__
			Serial.println( "BUFFER READY" );
#endif //__NEXTION__DEBUG__

			FIndex = 0;
			if( ( FBuffer[ FLength - 1 ] != 0xFF ) && ( FBuffer[ FLength - 2 ] != 0xFF ) && ( FBuffer[ FLength - 3 ] != 0xFF ) )
			{
#ifdef __NEXTION__DEBUG__
				Serial.println( "Bad command ending" );
#endif //__NEXTION__DEBUG__
				// Bad command ending
				return;
			}

			switch( FBuffer[ 0 ] )
			{
					case 0x65: // Touch event return data
					{
#ifdef __NEXTION__DEBUG__
						Serial.println( "Touch event" );
//						Serial.println( FEventElements.size() );
#endif // __NEXTION__DEBUG__
						bool AProcessed = false;
						T_Elements_TryProcessEvent::Call( FBuffer, AProcessed );
//						for( int i = 0; i < FEventElements.size(); ++i )
//							if( FEventElements[ i ]->TryProcessEvent( FBuffer ))
//								break;

						break;
					}

					case 0x1A:
#ifdef __NEXTION__DEBUG__
						Serial.println( "INVALID" );
#endif // __NEXTION__DEBUG__
//						FLastResponse->ProcessResponse( nullptr );
						delete FLastResponse;
						FLastResponse = nullptr;
						FCurrentRequest = "";
						ProcessRequestQueue();
						SendCommand( "sendme" );
						break;

					case 0x66:
						FActivePageNumber = FBuffer[ 1 ];
#ifdef __NEXTION__DEBUG__
						Serial.print( "ACTIVATE PAGE: " );
						Serial.println( FActivePageNumber );
#endif // __NEXTION__DEBUG__

						if( FLastResponse )
						{
#ifdef __NEXTION__DEBUG__
							Serial.println( "TEST11" );
#endif // __NEXTION__DEBUG__
							if( FLastResponse->ProcessResponse( FBuffer ))
							{
#ifdef __NEXTION__DEBUG__
								Serial.println( "CLEAR RESPONSE" );
#endif // __NEXTION__DEBUG__
								delete FLastResponse;
								FLastResponse = nullptr;
								FCurrentRequest = "";
							}
							else
							{
								RetryRequests();
								break;
							}

						}
						T_Elements_ActivatePageNo::Call( FActivePageNumber );

//						FCurrentRequest = "";
						ProcessRequestQueue();
						break;

					case 0x88: // System Startup

					case 0x71: // Numeric variable data returns
#ifdef __NEXTION__DEBUG__
						Serial.println( "TEST" );
#endif // __NEXTION__DEBUG__
						if( FLastResponse )
						{
#ifdef __NEXTION__DEBUG__
							Serial.println( "TEST11" );
#endif // __NEXTION__DEBUG__
							if( FLastResponse->ProcessResponse( FBuffer ))
							{
#ifdef __NEXTION__DEBUG__
								Serial.println( "CLEAR RESPONSE" );
#endif // __NEXTION__DEBUG__
								delete FLastResponse;
								FLastResponse = nullptr;
								FCurrentRequest = "";
								ProcessRequestQueue();
							}
							else if( FBuffer[ 0 ] == 0x66 )
								RetryRequests();

						}
						else if( FBuffer[ 0 ] == 0x66 )
							RetryRequests();

						break;

					case 0x70: // String variable data returns
//						Serial.println( "TEST1" );
#ifdef __NEXTION__DEBUG__
						Serial.println( "TEST1" );
#endif // __NEXTION__DEBUG__
						if( FLastResponse )
						{
#ifdef __NEXTION__DEBUG__
							Serial.println( "TEST2" );
							Serial.println( FResultText );
#endif // __NEXTION__DEBUG__
							if( FLastResponse->ProcessTextResponse( FResultText ))
							{
								delete FLastResponse;
								FLastResponse = nullptr;
								FCurrentRequest = "";
								ProcessRequestQueue();
							}
						}
						FResultText = "";
						break;
			}

/*
			if( AChar != 10 )
			{
				FBuffer[ FIndex ++ ] = AChar;
				if( FIndex < 255 )
					return;
			}
*/
//			FBuffer[ FIndex ] = '\0';
//			FIndex = 0;

//			Mitov::String AString = FBuffer;

		}

	public:
		void AddRequest( Mitov::String ARequest, int8_t APage, TDisplayNextionResponseElementBasic *AResponse )
		{
//			Serial.print( "REQUEST: " ); Serial.println( ARequest );

			FRequestElements.push_back( new TRequestElement( ARequest, APage, AResponse ));
			ProcessRequestQueue();
//			FResponseHandlersQueue.push_back( AResponse );
		}

		void AddRequestFront( Mitov::String ARequest, int8_t APage, TDisplayNextionResponseElementBasic *AResponse )
		{
			FRequestElements.push_front( new TRequestElement( ARequest, APage, AResponse ));
			ProcessRequestQueue();
		}

//		virtual	void RegisterEvents( DisplayNextionEventsIntf *AEvents ) override
//		{
//			FEventElements.push_back( AEvents );
//		}

	public:
		void _DirectPinReceive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			for( uint32_t i = 0; i < ADataBlock.Size; ++i )
				ProcessChar( ADataBlock.Data[ i ] );
		}

	public:
		inline void SystemInit()
		{
//			FStarted() = true;
			if( ResetDisplay() )
				AddRequest( "rest", -1, new TResponseReset );

//			delay( 1000 );
			RetryRequests();
			ProcessRequestQueue();
			T_Elements_ResetEvent::Call();
//			for( int i = 0; i < FEventElements.size(); ++i )
//				FEventElements[ i ]->ResetEvent();

			FLastTime = millis();
		}

		inline void SystemLoopBegin()
		{
//			ReadSerial();
			if( FCurrentRequest != "" )
				if( millis() - FLastTime > 1000 )
				{
					FLastTime = millis();
#ifdef __NEXTION__DEBUG__
					Serial.println( "TIMEOUT REPEAT" );
#endif // __NEXTION__DEBUG__
					SendCommand( FCurrentRequest.c_str() );
//					ProcessRequestQueue();
				}
		}
/*
		virtual void SystemLoopBegin() override
		{
//			Serial.println( "." );
//			if( FNeedsRead || ( ! ClockInputPin_IsConnected ))
				ReadSensor();

			inherited::SystemLoopBegin();
		}
*/
	public:
		inline DisplayNextion()
		{
			FStarted() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_Identified,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_PageIdentified,
		typename T_PageIndex,
		typename T_Ready
	> class DisplayNextionElementScope :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>,
		public T_Ready
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		uint8_t	ID = 1;
		uint8_t	Height = 255;

	public:
		_V_PROP_( Ready )

	protected:
		struct TResponseElementGetValue : public TDisplayNextionResponseElementBasic
		{
		public:
			DisplayNextionElementScope *FOwner;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
//				Serial.print( "Height: " ); Serial.println( ABuffer[ 1 ] );
				FOwner->Height = ABuffer[ 1 ];
				FOwner->Ready() = true;
				return true;
			}

		public:
			TResponseElementGetValue( DisplayNextionElementScope *AOwner ) :
				FOwner( AOwner )
			{
			}
		};

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

/*
	public:
		virtual bool RequestInfo() override
		{
			Serial.println( ElementName );
			RequestElementID( ElementName );

			C_OWNER.AddRequest( Mitov::String( "get " ) + ElementName + ".h", new TResponseElementGetValue( this )); // Get the Height

//			C_OWNER.FResponseHandlersQueue.push_back( new TResponseElementGetValue( this ));
//			C_OWNER.SendCommand( ( Mitov::String( "get " ) + ElementName + ".h" ).c_str() ); // Get the Height
			return true;
		}
*/
	public:
		inline void ClearInputPin_o_Receive( void * )
		{
			C_OWNER.SendCommand( ( Mitov::String( "cle " ) + ID + ",255" ).c_str() );
		}

	public:
		virtual void SendRequest() override
		{
//			Serial.println( "TEST___" );
			if( inherited::Identified() )
				Ready() = true;

			else
			{
//				Serial.println( "TEST___1" );
				inherited::RequestElementID( inherited::GetElementName() );
				C_OWNER.AddRequest( Mitov::String( "get " ) + inherited::GetElementName() + ".h", inherited::PageIndex(), new TResponseElementGetValue( this ) );
			}

			inherited::SendRequest();
		}

	public:
		inline DisplayNextionElementScope()
		{
			Ready() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX
	> class DisplayNextionScopeChannel
	{
	public:
		inline void InputPin_o_Receive( void * _Data )
		{
//			Serial.println( "Test1" );
			if( ! C_OWNER.Ready() )
				return;

			float AFloatValue = MitovMax( 0.0f, MitovMin( 1.0f, *(float *)_Data ));
#ifdef __NEXTION__DEBUG__
			Serial.print( "Channel: " );
			Serial.println( AFloatValue );
#endif //__NEXTION__DEBUG__
			int AIntValue = int( AFloatValue * C_OWNER.Height + 0.5 );
			C_OWNER.GetOwner().SendCommand( ( Mitov::String( "add " ) + C_OWNER.ElementID() + "," + C_INDEX + "," + AIntValue ).c_str() );
		}

		inline void ClearInputPin_o_Receive( void * )
		{
			C_OWNER.GetOwner().SendCommand( ( Mitov::String( "cle " ) + C_OWNER.ElementID() + "," + C_INDEX ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	enum TArduinoDisplayNextionElementButtonEvents { nbeBoth, nbeDown, nbeUp };
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_Events,
		typename T_Identified,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_OutputPin,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementButton :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>, // public DisplayNextionEventsIntf, 
			public T_Events,
			public T_OutputPin

	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PIN_( OutputPin ) 

	public:
		_V_PROP_( Events )

	public:
		inline void ResetEvent()
		{
			T_OutputPin::SetPinValueLow();
			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );

			if( AProcessed )
				return;

			if( ! inherited::Identified() )
				return;

			if( ABuffer[ 2 ] != inherited::ElementID() )
				return;

			if( inherited::PageIndex() != 255 )
				if( ABuffer[ 1 ] != inherited::PageIndex() ) //FOwner.GetPageNumber() )
					return;

			if( ABuffer[ 3 ] )
			{
				if( Events().GetValue() == nbeBoth )
					T_OutputPin::SetPinValueHigh();

				else if( Events().GetValue() == nbeDown )
				{
					T_OutputPin::SetPinValueHigh();
					T_OutputPin::SetPinValueLow();
				}
			}

			else
			{
				if( Events().GetValue() == nbeBoth )
					T_OutputPin::SetPinValueLow();
					
				else if( Events().GetValue() == nbeUp )
				{
					T_OutputPin::SetPinValueHigh();
					T_OutputPin::SetPinValueLow();
				}
			}

			AProcessed = true;
		}

	public:
		virtual void SendRequest() override
		{
#ifdef __NEXTION__DEBUG__
			Serial.print( "BUTTON Identified: " ); Serial.println( inherited::Identified() );
#endif //__NEXTION__DEBUG__
			if( ! inherited::Identified() )
				inherited::RequestElementID( inherited::GetElementName() );

			inherited::SendRequest();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_Identified,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_OutputPin,
		typename T_PageIdentified,
		typename T_PageIndex
	> class DisplayNextionElementCheckBox : 
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>, // public DisplayNextionEventsIntf, 
		public T_OutputPin
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PIN_( OutputPin ) 

	protected:
		struct TResponseElementGetValue : public TDisplayNextionResponseElementBasic
		{
		public:
			DisplayNextionElementCheckBox *FOwner;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
//				Serial.print( "Height: " ); Serial.println( ABuffer[ 1 ] );
				FOwner->SetPinValue( ABuffer[ 1 ] != 0 );
				return true;
			}

		public:
			TResponseElementGetValue( DisplayNextionElementCheckBox *AOwner ) :
				FOwner( AOwner )
			{
			}
		};

	public:
		inline void ResetEvent()
		{
			T_Modifiers_ResetEvent::Call();
		}

	public:
		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
			if( AProcessed )
				return;

			if( ! inherited::Identified() )
				return;

			if( ABuffer[ 2 ] != inherited::ElementID() )
				return;

			if( inherited::PageIndex() != 255 )
				if( ABuffer[ 1 ] != inherited::PageIndex() ) // FOwner.GetPageNumber() )
					return;

			RequestState();
			AProcessed = true;
		}

	protected:
		void RequestState()
		{
			C_OWNER.AddRequest( Mitov::String( "get " ) + inherited::GetElementName() + ".val", inherited::PageIndex(), new TResponseElementGetValue( this ) );
		}

	public:
		virtual void SendRequest() override
		{
			if( ! inherited::Identified() )
			{
				inherited::RequestElementID( inherited::GetElementName() );
				RequestState();
			}

			inherited::SendRequest();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ElementID,
		typename T_ElementName,
		typename T_FPressed,
		typename T_Identified,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_Modifiers_ResetEvent,
		typename T_Modifiers_TryProcessEvent,
		typename T_OutputPin,
		typename T_PageIdentified,
		typename T_PageIndex,
		typename T_PressedScanPeriod
	> class DisplayNextionElementSlider :
		public ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			>, // public DisplayNextionEventsIntf,
		public T_FPressed,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin,
		public T_PressedScanPeriod
	{
		typedef ArduinoDisplayNextionElementBasic<
				T_OWNER, C_OWNER,
				T_ElementID,
				T_ElementName,
				T_Identified,
				T_PageIdentified,
				T_PageIndex
			> inherited;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

		_V_PROP_( PressedScanPeriod )

	protected:
#ifdef __NEXTION_SCAN_SLIDER__
		unsigned long	FLastTime = 0;
#endif
		_V_PROP_( FPressed )

	protected:
		struct TResponseElementGetValue : public TDisplayNextionResponseElementBasic
		{
		public:
			DisplayNextionElementSlider *FOwner;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
				uint32_t AValue = ABuffer[ 1 ] | ( int32_t( ABuffer[ 2 ] ) << 8 ) | ( int32_t( ABuffer[ 3 ] ) << 16 ) | ( int32_t( ABuffer[ 4 ] ) << 24 );
				float AFloatValue = float( AValue - FOwner->Value_Min().GetValue() ) / ( FOwner->Value_Max().GetValue() - FOwner->Value_Min().GetValue() );
				FOwner->SetPinValue( AFloatValue );
//				FOwner->OutputPin.SendValue<int32_t>( ABuffer[ 1 ] | ( int32_t( ABuffer[ 2 ] ) << 8 ) | ( int32_t( ABuffer[ 3 ] ) << 16 ) | ( int32_t( ABuffer[ 4 ] ) << 24 ) );
//				Serial.print( "Height: " ); Serial.println( ABuffer[ 1 ] );
//				FOwner->OutputPin.SetValue( ABuffer[ 1 ] != 0 );
				return true;
			}

		public:
			TResponseElementGetValue( DisplayNextionElementSlider *AOwner ) :
				FOwner( AOwner )
			{
			}
		};

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			RequestState();
		}

	public:
		inline void ResetEvent() 
		{
			T_Modifiers_ResetEvent::Call();
		}

	public:
		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
			if( AProcessed )
				return;

#ifdef __NEXTION__DEBUG__
			Serial.println( "Slider.TryProcessEvent" );
			Serial.println( inherited::Identified() );
			Serial.println( inherited::ElementID() );
			Serial.println( inherited::PageIndex() );
#endif
			if( ! inherited::Identified() )
				return;

			if( ABuffer[ 2 ] != inherited::ElementID() )
				return;

			if( inherited::PageIndex() != 255 )
				if( ABuffer[ 1 ] != inherited::PageIndex() ) //FOwner.GetPageNumber() )
					return;

			RequestState();
#ifdef __NEXTION_SCAN_SLIDER__
			FPressed() = ( ABuffer[ 3 ] != 0 );
			if ( FPressed() )
				if( PressedScanPeriod() )
					FLastTime = millis();
#endif
			AProcessed = true;
		}

	protected:
		void RequestState()
		{
			C_OWNER.AddRequest( Mitov::String( "get " ) + inherited::GetElementName() + ".val", inherited::PageIndex(), new TResponseElementGetValue( this ) );
		}

#ifdef __NEXTION_SCAN_SLIDER__
	public:
		inline void SystemLoopBegin()
		{
			if( FPressed() )
				if( PressedScanPeriod() )
					if( millis() - FLastTime > PressedScanPeriod() )
					{
						FLastTime = millis();
//						Serial.println( "SCAN" );
						RequestState();
					}

		}
#endif

	public:
		virtual void SendRequest() override
		{
			if( ! inherited::Identified() )
			{
				inherited::RequestElementID( inherited::GetElementName() );
				RequestState();
			}

			inherited::SendRequest();
		}

	public:
		inline DisplayNextionElementSlider()
		{
			FPressed() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_PropertyName,
		typename T_Value
	> class DisplayNextionElementModifierPropertySetterString :
		public T_PropertyName,
		public T_Value
	{
	public:
		_V_PROP_( PropertyName )
		_V_PROP_( Value )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.GetOwner().SendCommand(( C_OWNER.GetElementName() + "." + PropertyName().GetValue() + "=\"" + Value().GetValue() + "\"" ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_PropertyName,
		typename T_Value
	> class DisplayNextionElementModifierPropertySetterInteger :
		public T_PropertyName,
		public T_Value
	{
	public:
		_V_PROP_( PropertyName )
		_V_PROP_( Value )

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.GetOwner().SendCommand(( C_OWNER.GetElementName() + "." + PropertyName().GetValue() + "=" + Value().GetValue() ).c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FRequested,
		typename T_OutputPin,
		typename T_PropertyName
	> class DisplayNextionElementModifierPropertyGetterInteger :
		public T_FRequested,
		public T_OutputPin,
		public T_PropertyName
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( PropertyName )

	public:
		_V_PROP_( FRequested )

	protected:
		struct TResponseElementGetValue : public TDisplayNextionResponseElementBasic
		{
		public:
			DisplayNextionElementModifierPropertyGetterInteger *FOwner;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
//				Serial.println( "RESPONSE" );
				FOwner->SetPinValue( ABuffer[ 1 ] | ( int32_t( ABuffer[ 2 ] ) << 8 ) | ( int32_t( ABuffer[ 3 ] ) << 16 ) | ( int32_t( ABuffer[ 4 ] ) << 24 ) );
				return true;
			}

		public:
			TResponseElementGetValue( DisplayNextionElementModifierPropertyGetterInteger *AOwner ) :
				FOwner( AOwner )
			{
				FOwner->FRequested() = true;
			}

			virtual ~TResponseElementGetValue()
			{
				FOwner->FRequested() = false;
			}
		};

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "GET" );
			if( ! FRequested() )
				C_OWNER.GetOwner().AddRequest( Mitov::String( "get " ) + C_OWNER.GetElementName() + "." + PropertyName(), -1, new TResponseElementGetValue( this ) );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FRequested,
		typename T_OutputPin,
		typename T_PropertyName
	> class DisplayNextionElementModifierPropertyGetterString :
		public T_FRequested,
		public T_OutputPin,
		public T_PropertyName
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( PropertyName )

	public:
		_V_PROP_( FRequested )

	protected:
		struct TResponseElementGetValue : public TDisplayNextionResponseElementBasic
		{
		public:
			DisplayNextionElementModifierPropertyGetterString *FOwner;

		public:
			virtual bool ProcessTextResponse( Mitov::String AText ) override
			{
//				Serial.println( "RESPONSE" );
//				FOwner->OutputPin.SendValue<int32_t>( ABuffer[ 1 ] | ( int32_t( ABuffer[ 2 ] ) << 8 ) | ( int32_t( ABuffer[ 3 ] ) << 16 ) | ( int32_t( ABuffer[ 4 ] ) << 24 ) );
				FOwner->SetPinValue( AText );
				return true;
			}

		public:
			TResponseElementGetValue( DisplayNextionElementModifierPropertyGetterString *AOwner ) :
				FOwner( AOwner )
			{
				FOwner->FRequested() = true;
			}

			virtual ~TResponseElementGetValue()
			{
				FOwner->FRequested() = false;
			}
		};

	public:
		inline void ResetEvent() 
		{
//			T_Modifiers_ResetEvent::Call();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
//			T_Modifiers_TryProcessEvent::Call( ABuffer, AProcessed );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "GET" );
			if( ! FRequested() )
				C_OWNER.GetOwner().AddRequest( Mitov::String( "get " ) + C_OWNER.GetElementName() + "." + PropertyName().GetValue(), -1, new TResponseElementGetValue( this ) );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class DisplayNextionElementModifierPressedState : //	public DisplayNextionEventsIntf,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void ResetEvent()
		{
			T_OutputPin::SetPinValueLow();
		}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			if( AProcessed )
				return;

			if( ! C_OWNER.Identified() )
				return;

			if( ABuffer[ 2 ] != C_OWNER.ElementID() )
				return;

			if( ABuffer[ 1 ] != C_OWNER.PageIndex() ) // FOwner.FOwner.GetPageNumber() )
				return;

			T_OutputPin::SetPinValue( ABuffer[ 3 ] != 0 );
			AProcessed = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class DisplayNextionElementModifierPressEvent : // public DisplayNextionEventsIntf,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void ResetEvent() {}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			if( AProcessed )
				return;

			if( ! C_OWNER.Identified() )
				return;

			if( ABuffer[ 2 ] != C_OWNER.ElementID() )
				return;

			if( ABuffer[ 1 ] != C_OWNER.PageIndex() ) // FOwner.FOwner.GetPageNumber() )
				return;

			if( ABuffer[ 3 ] )
				T_OutputPin::ClockPin();

			AProcessed = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class DisplayNextionElementModifierReleaseEvent : // public DisplayNextionEventsIntf,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void ResetEvent() {}

		inline void TryProcessEvent( uint8_t *ABuffer, bool &AProcessed )
		{
			if( AProcessed )
				return;

			if( ! C_OWNER.Identified() )
				return;

			if( ABuffer[ 2 ] != C_OWNER.ElementID() )
				return;

			if( ABuffer[ 1 ] != C_OWNER.PageIndex() ) // FOwner.FOwner.GetPageNumber() )
				return;

			if( ! ABuffer[ 3 ] )
				T_OutputPin::ClockPin();

			AProcessed = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Command
	> class DisplayNextionElementCommand :
		public T_Command
	{
	public:
		_V_PROP_( Command )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SendCommand( Command().c_str() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Command,
		typename T_FRequested,
		typename T_OutputPin
	> class DisplayNextionElementGetterCommandString :
		public T_Command,
		public T_FRequested,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Command )

	public:
		_V_PROP_( FRequested )

	protected:
		struct TResponseElementGetValue : public TDisplayNextionResponseElementBasic
		{
		public:
			DisplayNextionElementGetterCommandString *FOwner;

		public:
			virtual bool ProcessTextResponse( Mitov::String AText ) override
			{
//				Serial.println( "RESPONSE" );
				FOwner->SetPinValue( AText );
				return true;
			}

		public:
			TResponseElementGetValue( DisplayNextionElementGetterCommandString *AOwner ) :
				FOwner( AOwner )
			{
				FOwner->FRequested() = true;
			}

			virtual ~TResponseElementGetValue()
			{
				FOwner->FRequested() = false;
			}
		};

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! FRequested() )
				C_OWNER.AddRequest( Command(), -1, new TResponseElementGetValue( this ) );

		}

	public:
		inline DisplayNextionElementGetterCommandString()
		{
			FRequested() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Command,
		typename T_FRequested,
		typename T_OutputPin
	> class DisplayNextionElementGetterCommandInteger :
		public T_Command,
		public T_FRequested,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Command )

	public:
		_V_PROP_( FRequested )

	protected:
		struct TResponseElementGetValue : public TDisplayNextionResponseElementBasic
		{
		public:
			DisplayNextionElementGetterCommandInteger *FOwner;

		public:
			virtual bool ProcessResponse( uint8_t *ABuffer ) override
			{
//				Serial.println( "RESPONSE" );
				FOwner->SetPinValue( ABuffer[ 1 ] | ( int32_t( ABuffer[ 2 ] ) << 8 ) | ( int32_t( ABuffer[ 3 ] ) << 16 ) | ( int32_t( ABuffer[ 4 ] ) << 24 ) );
				return true;
			}

		public:
			TResponseElementGetValue( DisplayNextionElementGetterCommandInteger *AOwner ) :
				FOwner( AOwner )
			{
				FOwner->FRequested() = true;
//				Serial.println( "CREATE" );
			}

			virtual ~TResponseElementGetValue()
			{
				FOwner->FRequested() = false;
//				Serial.println( "DESTROY" );
			}
		};

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! FRequested() )
				C_OWNER.AddRequest( Command(), -1, new TResponseElementGetValue( this ) );
		}

	public:
		inline DisplayNextionElementGetterCommandInteger()
		{
			FRequested() = false;
		}

	};
//---------------------------------------------------------------------------
#undef Min
#undef Max
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif