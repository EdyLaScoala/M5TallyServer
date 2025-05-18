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

#ifndef Notify
  #define Notify(...) ((void)0)
#endif

#include <hidboot.h>

#ifdef Notify
  #undef Notify
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_X,
		typename T_Y
	> class TArduinoSamdUSBHostMousePositionSetElement :
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.SetPosition( X().GetValue(), Y().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_X,
		typename T_Y
	> class TArduinoSamdUSBHostMousePositionAddValueElement :
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.AddPosition( X().GetValue(), Y().GetValue() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_OutputPin
	> class TArduinoSamdUSBHostMousePositionProperty :
		public T_Enabled,
		public T_FCount,
		public T_InitialValue,
		public T_Value_Max,
		public T_Value_Min,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Value_Max )
		_V_PROP_( Value_Min )

	public:
		_V_PROP_( FCount )

	public:
		void SetValue( int32_t AValue )
		{
            if( AValue > Value_Max().Value().GetValue() )
                AValue = Value_Max().Value().GetValue();

            if( AValue < Value_Min().Value().GetValue() )
                AValue = Value_Min().Value().GetValue();

			FCount() = AValue;
			T_OutputPin::SetPinValue( FCount() );
		}

		void AddValue( int32_t AValue )
		{
			if( AValue > 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				if( FCount() < AMax - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Max().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMax;

				}
			}

			else if( AValue < 0 )
			{
				int32_t AMin = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );
				int32_t AMax = MitovMin( Value_Min().Value().GetValue(), Value_Max().Value().GetValue() );

				if( FCount() > AMin - AValue )
					FCount() = FCount().GetValue() + AValue;

                else
				{
					if( Value_Min().RollOver() )
						FCount() = (( FCount() + AValue ) % ( AMax - AMin )) + AMin;

					else
						FCount() = AMin;

				}
			}

			T_OutputPin::SetPinValue( FCount() );
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_Wheel,
		typename T_X,
		typename T_Y
	> class TArduinoSamdUSBHostMousePositionElement :
//		public T_Wheel,
		public T_X,
		public T_Y
	{
	public:
//		_V_PROP_( Wheel )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Move( MOUSEINFO *mi )
		{
			X().AddValue( mi->dX );
			Y().AddValue( mi->dY );
		}

	public:
		void SetPosition( int32_t AX, int32_t AY )
		{
			X().SetValue( AX );
			Y().SetValue( AY );
		}

		void AddPosition( int32_t AX, int32_t AY )
		{
			X().AddValue( AX );
			Y().AddValue( AY );
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			SystemInit();
		}

	public:
		inline void SystemInit()
		{
			X().FCount() = X().InitialValue();
			Y().FCount() = Y().InitialValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ButtonsOutputPins_0,
		typename T_ButtonsOutputPins_1,
		typename T_ButtonsOutputPins_2,
		typename T_CursorPositionOutputPins_0,
		typename T_CursorPositionOutputPins_1,
		typename T_Elements_Move
//		typename T_WheelOutputPin
	> class TArduinoSamdUSBHostMouse :
	    public MouseReportParser,
		public T_ButtonsOutputPins_0,
		public T_ButtonsOutputPins_1,
		public T_ButtonsOutputPins_2,
		public T_CursorPositionOutputPins_0,
		public T_CursorPositionOutputPins_1
//		public T_WheelOutputPin
	{
	public:
		_V_PIN_( ButtonsOutputPins_0 )
		_V_PIN_( ButtonsOutputPins_1 )
		_V_PIN_( ButtonsOutputPins_2 )

		_V_PIN_( CursorPositionOutputPins_0 )
		_V_PIN_( CursorPositionOutputPins_1 )

//		_V_PIN_( WheelOutputPin )

	protected:
	  virtual void OnMouseMove( MOUSEINFO *mi ) override
	  {
		  T_CursorPositionOutputPins_0::SetPinValue( mi->dX );
		  T_CursorPositionOutputPins_1::SetPinValue( mi->dY );

		  T_Elements_Move::Call( mi );
	  }

	  virtual void OnLeftButtonUp( MOUSEINFO * ) override
	  {
		  T_ButtonsOutputPins_0::SetPinValueLow();
	  }

	  virtual void OnLeftButtonDown( MOUSEINFO * ) override
	  {
		  T_ButtonsOutputPins_0::SetPinValueHigh();
	  }

	  virtual void OnMiddleButtonUp( MOUSEINFO * ) override
	  {
		  T_ButtonsOutputPins_1::SetPinValueLow();
	  }

	  virtual void OnMiddleButtonDown( MOUSEINFO * ) override
	  {
		  T_ButtonsOutputPins_1::SetPinValueHigh();
	  }

	  virtual void OnRightButtonUp( MOUSEINFO * ) override
	  {
		  T_ButtonsOutputPins_2::SetPinValueLow();
	  }

	  virtual void OnRightButtonDown( MOUSEINFO *mi ) override
	  {
		  T_ButtonsOutputPins_2::SetPinValueHigh();
	  }

	private:
		HIDBoot<HID_PROTOCOL_MOUSE> hostMouse;

	public:
		inline void SystemStart()
		{
		  T_CursorPositionOutputPins_0::SetPinValue( 0 );
		  T_CursorPositionOutputPins_1::SetPinValue( 0 );

		  T_ButtonsOutputPins_0::SetPinValueLow();
		  T_ButtonsOutputPins_1::SetPinValueLow();
		  T_ButtonsOutputPins_2::SetPinValueLow();
		}

	public:
		inline TArduinoSamdUSBHostMouse() : 
			hostMouse( C_OWNER.GetUSBHost() )
		{
			hostMouse.SetReportParser( 0, this );
		};

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif