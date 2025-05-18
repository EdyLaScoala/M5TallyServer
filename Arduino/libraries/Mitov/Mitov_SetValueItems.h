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
		typename T_OWNER, 
		T_OWNER &C_OWNER, 
		typename T_Value
	> class ArduinoSetValueElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.ExternalSetValue( -1, Value() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Value
	> class ArduinoAddValueElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void TryActive( bool &AResult )
		{
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.ExternalAddValue( Value() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, 
		T_OWNER &C_OWNER, 
		typename T_FInputValue,
//		uint32_t C_INDEX,
		typename T_Value
	> class ArduinoStateSetValueElement :
		public T_FInputValue,
		public T_Value
	{
	protected:
		_V_PROP_( FInputValue ) //= false;

	public:
		_V_PROP_( Value )

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

			if( FInputValue() )
			{
//					Serial.println( "TryActive" );
//					Serial.println( Value );
				C_OWNER.ExternalSetValue( -1, Value().GetValue() );
				AResult = true;
			}

		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue() == *(bool *)_Data )
				return;

//			Serial.println( FInputValue );

			FInputValue() = *(bool *)_Data;
//			if( FInputValue )
//				FOwner.ExternalSetValue( -1, Value );

//			else
			C_OWNER.ResetValue();
		}

	public:
		ArduinoStateSetValueElement()
		{
			FInputValue() = false;
		}

	};
//---------------------------------------------------------------------------    
	template <
		typename T_OWNER, 
		T_OWNER &C_OWNER, 
		uint32_t C_INDEX,
		typename T_Slope,
		typename T_Value
	> class ArduinoRampToValueAnalogElement :
		public T_Slope,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( Slope )

//	protected:
//		unsigned long	FLastTime = 0;

	public:
		inline void SystemLoopBegin( unsigned long AElapsedMicros )
		{
			if( C_OWNER.FActiveElement() != C_INDEX )
				return;

//			Serial.println( "UpdateRamp" );
			float ACurrentValue = C_OWNER.GetCurrentValue();

			if( ACurrentValue == Value() )
				C_OWNER.RegisterForTimeUpdate( C_INDEX, false );

			else
			{
//				float ARamp = abs( ( micros() - C_OWNER.FLastTime ) * Slope() / 1000000 );
				float ARamp = abs( ( AElapsedMicros ) * Slope() / 1000000 );
				if( ACurrentValue < Value() )
				{
					ACurrentValue += ARamp;
//					Serial.println( "UpdateRamp 1" );
					if( ACurrentValue > Value() )
						ACurrentValue = Value();

				}
				else
				{
//					Serial.println( "UpdateRamp 2" );
					ACurrentValue -= ARamp;
					if( ACurrentValue < Value() )
						ACurrentValue = Value();

				}
				
				C_OWNER.ExternalSetValue( C_INDEX, ACurrentValue );
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//			Serial.println( "DoReceived" );
			C_OWNER.RegisterForTimeUpdate( C_INDEX, true );
//			C_OWNER.ExternalSetValue( C_INDEX, Value );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FInputValue,
		uint32_t C_INDEX,
		typename T_Slope,
		typename T_Value
	> class ArduinoStateRampToValueAnalogElement :
		public T_FInputValue,
		public T_Slope,
		public T_Value
	{
//		typedef Mitov::BasicValueStateElement inherited;

	protected:
		_V_PROP_( FInputValue )

	public:
		_V_PROP_( Value )
		_V_PROP_( Slope )

	public:
		inline void SystemLoopBegin( unsigned long AElapsedMicros )
		{
			if( C_OWNER.FActiveElement() != C_INDEX )
				return;

			float ACurrentValue = C_OWNER.GetCurrentValue();

//			Serial.println( "UpdateRamp" );
			if( ACurrentValue == Value() )
				C_OWNER.RegisterForTimeUpdate( C_INDEX, false );

			else
			{
//				float ARamp = abs( ( micros() - C_OWNER.FLastTime ) * Slope() / 1000000 );
				float ARamp = abs( ( AElapsedMicros ) * Slope() / 1000000 );
				if( ACurrentValue < Value() )
				{
					ACurrentValue += ARamp;
//					Serial.println( "UpdateRamp 1" );
					if( ACurrentValue > Value() )
						ACurrentValue = Value();

				}
				else
				{
//					Serial.println( "UpdateRamp 2" );
					ACurrentValue -= ARamp;
					if( ACurrentValue < Value() )
						ACurrentValue = Value();

				}
				
				C_OWNER.ExternalSetValue( C_INDEX, ACurrentValue );
			}
		}

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

//			Serial.println( "TryActive" );
			if( FInputValue() )
			{
				C_OWNER.RegisterForTimeUpdate( C_INDEX, true );
				AResult = true;
			}

		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue() == *(bool *)_Data )
				return;

//			Serial.println( FInputValue );

			FInputValue() = *(bool *)_Data;
			if( FInputValue() )
				C_OWNER.RegisterForTimeUpdate( C_INDEX, true );
//				C_OWNER.ExternalSetValue( -1, Value );

			else
			{
				C_OWNER.RegisterForTimeUpdate( C_INDEX, false );
				C_OWNER.ResetValue();
			}
		}

	public:
		ArduinoStateRampToValueAnalogElement()
		{
			FInputValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER, uint32_t C_INDEX> class ArduinoRampToValueComplexElement
	{
	public:
		TComplex	Value;
		TComplex	Slope;

//	protected:
//		unsigned long	FLastTime = 0;

	public:
		inline void SystemLoopBegin( unsigned long AElapsedMicros )
		{
			if( C_OWNER.FActiveElement() != C_INDEX )
				return;

//			Serial.println( "UpdateRamp" );
			TComplex ACurrentValue = C_OWNER.GetCurrentValue();

			if( ACurrentValue == Value )
				C_OWNER.RegisterForTimeUpdate( C_INDEX, false );

			else
			{
//				float ARamp = abs( ( micros() - C_OWNER.FLastTime ) * Slope.Real / 1000000 );
				float ARamp = abs( ( AElapsedMicros ) * Slope.Real / 1000000 );
				if( ACurrentValue.Real < Value.Real )
				{
					ACurrentValue.Real += ARamp;
//					Serial.println( "UpdateRamp 1" );
					if( ACurrentValue.Real > Value.Real )
						ACurrentValue.Real = Value.Real;

				}
				else
				{
//					Serial.println( "UpdateRamp 2" );
					ACurrentValue.Real -= ARamp;
					if( ACurrentValue.Real < Value.Real )
						ACurrentValue.Real = Value.Real;

				}
				
				ARamp = abs( ( AElapsedMicros ) * Slope.Imaginary / 1000000 );

				if( ACurrentValue.Imaginary < Value.Imaginary )
				{
					ACurrentValue.Imaginary += ARamp;
//					Serial.println( "UpdateRamp 1" );
					if( ACurrentValue.Imaginary > Value.Imaginary )
						ACurrentValue.Imaginary = Value.Imaginary;

				}
				else
				{
//					Serial.println( "UpdateRamp 2" );
					ACurrentValue.Imaginary -= ARamp;
					if( ACurrentValue.Imaginary < Value.Imaginary )
						ACurrentValue.Imaginary = Value.Imaginary;

				}

				C_OWNER.ExternalSetValue( C_INDEX, ACurrentValue );
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//			Serial.println( "DoReceived" );
			C_OWNER.RegisterForTimeUpdate( C_INDEX, true );
//			C_OWNER.ExternalSetValue( C_INDEX, Value );
		}

	public:
		ArduinoRampToValueComplexElement() :
			Slope( 1, 1 )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_FInputValue,
		uint32_t C_INDEX,
		typename T_Slope,
		typename T_Value
	> class ArduinoStateRampToValueComplexElement :
		public T_FInputValue,
		public T_Slope,
		public T_Value
	{
//		typedef Mitov::BasicValueStateElement inherited;

	protected:
		_V_PROP_( FInputValue )

	public:
		_V_PROP_( Value )
		_V_PROP_( Slope )

	public:
		inline void SystemLoopBegin( unsigned long AElapsedMicros )
		{
			if( C_OWNER.FActiveElement() != C_INDEX )
				return;

			TComplex ACurrentValue = C_OWNER.GetCurrentValue();

//			Serial.println( "UpdateRamp" );
			if( ACurrentValue == Value() )
				C_OWNER.RegisterForTimeUpdate( C_INDEX, false );

			else
			{
				float ARamp = abs( ( AElapsedMicros ) * Slope().GetValue().Real / 1000000 );
				if( ACurrentValue.Real < Value().GetValue().Real )
				{
					ACurrentValue.Real += ARamp;
//					Serial.println( "UpdateRamp 1" );
					if( ACurrentValue.Real > Value().GetValue().Real )
						ACurrentValue.Real = Value().GetValue().Real;

				}
				else
				{
//					Serial.println( "UpdateRamp 2" );
					ACurrentValue.Real -= ARamp;
					if( ACurrentValue.Real < Value().GetValue().Real )
						ACurrentValue.Real = Value().GetValue().Real;

				}

				ARamp = abs( ( AElapsedMicros ) * Slope().GetValue().Imaginary / 1000000 );
				if( ACurrentValue.Imaginary < Value().GetValue().Imaginary )
				{
					ACurrentValue.Imaginary += ARamp;
//					Serial.println( "UpdateRamp 1" );
					if( ACurrentValue.Imaginary > Value().GetValue().Imaginary )
						ACurrentValue.Imaginary = Value().GetValue().Imaginary;

				}
				else
				{
//					Serial.println( "UpdateRamp 2" );
					ACurrentValue.Imaginary -= ARamp;
					if( ACurrentValue.Imaginary < Value().GetValue().Imaginary )
						ACurrentValue.Imaginary = Value().GetValue().Imaginary;

				}
				
				C_OWNER.ExternalSetValue( C_INDEX, ACurrentValue );
			}
		}

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

//			Serial.println( "TryActive" );
			if( FInputValue() )
			{
				C_OWNER.RegisterForTimeUpdate( C_INDEX, true );
				AResult = true;
			}

		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue == *(bool *)_Data )
				return;

//			Serial.println( FInputValue );

			FInputValue = *(bool *)_Data;
			if( FInputValue )
				C_OWNER.RegisterForTimeUpdate( C_INDEX, true );
//				C_OWNER.ExternalSetValue( -1, Value );

			else
			{
				C_OWNER.RegisterForTimeUpdate( C_INDEX, false );
				C_OWNER.ResetValue();
			}
		}

	public:
		ArduinoStateRampToValueComplexElement()
		{
			FInputValue() = false;
		}

//	public:
//		ArduinoStateRampToValueComplexElement() :
//			Slope( 1, 1 )
//		{
//		}

	};
//---------------------------------------------------------------------------
	template <
        typename T_OWNER, T_OWNER &C_OWNER,
        typename T_Value,
        typename T_Value_ApplyValues,
        typename T_Value_GetValue
    > class ArduinoBinarySetValueElement :
        public T_Value
	{
	public:
        _V_PROP_( Value )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            typename T_Value::T_ARRAY *ABuffer = T_Value::GetDataArray();
			C_OWNER.ExternalSetValue( *ABuffer );
            delete ABuffer;
//			C_OWNER.ExternalSetValue( Value );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif