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
#define Min Min
#define Max Max
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_Enabled,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Min,
		typename T_Value_Max,
		typename T_NeedsUpdate
	> class BasicCounter :
		public T_OutputPin,
		public T_Enabled,
		public T_FCount,
		public T_InitialValue,
		public T_Value_Min,
		public T_Value_Max,
		protected T_NeedsUpdate
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Value_Min )
		_V_PROP_( Value_Max )

//    public:
//        T_ENABLED Enabled;
//		bool Reversed : 1;		// Added here to save memory

    public:
		_V_PROP_( NeedsUpdate )

	protected:
		_V_PROP_( FCount )

	public:
		void ExternalSetValue( int32_t AIndex, int32_t AValue )
		{
            if( AValue > Value_Max().Value().GetValue() )
                AValue = Value_Max().Value().GetValue();

            if( AValue < Value_Min().Value().GetValue() )
                AValue = Value_Min().Value().GetValue();

			FCount() = AValue;
			T_OutputPin::SetPinValue( FCount() );
		}

        void ExternalAddValue( int32_t AValue )
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

	public:
		inline void SystemInit()
		{
//			Serial.println( "Counter::SystemInit" );
			FCount() = InitialValue();

			T_OutputPin::SetPinValue( FCount() );
		}

		inline void SystemStart()
		{
//			if( FCount() != InitialValue() )
//				T_OutputPin::SetPinValue( FCount() );

		}

		inline void SystemLoopBegin() // For compatibility with Interrupt version
		{
			if( NeedsUpdate() )
			{
				NeedsUpdate() = false;
				T_OutputPin::SetPinValue( FCount() );
			}
		}

	protected:
		inline void	CountUp( bool ASendOutput )
		{
			if( T_Value_Max::RollOver() || ( FCount() < T_Value_Max::Value() ))
			{
				if( FCount() == T_Value_Max::Value() )
					FCount() = T_Value_Min::Value();

                else
    				FCount() = FCount() + 1;  // Bitfields can't use reference for ++

				if( ASendOutput )
					T_OutputPin::SetPinValue( FCount() );

				else
					NeedsUpdate() = true;

			}
		}

		inline void	CountDown( bool ASendOutput )
		{
			if( T_Value_Min::RollOver() || ( FCount() > T_Value_Min::Value() ))
			{
				if( FCount() == T_Value_Min::Value() )
					FCount() = T_Value_Max::Value();

                else
    				FCount() = FCount() - 1;  // Bitfields can't use reference for --

				if( ASendOutput )
					T_OutputPin::SetPinValue( FCount() );

				else
					NeedsUpdate() = true;

			}
		}

	public:
		void ResetInputPin_o_Receive( void *_Data )
		{
			FCount() = InitialValue();
			T_OutputPin::SetPinValue( FCount() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_NeedsUpdate,
		typename T_OutputPin,
		typename T_Reversed
	> class Counter :
		public Mitov::BasicCounter<
				T_OutputPin,
				T_Enabled,
				T_FCount,
				T_InitialValue,
				T_Value_Min,
				T_Value_Max,
				T_NeedsUpdate
			>,
		public T_Reversed
	{
		typedef Mitov::BasicCounter<
				T_OutputPin,
				T_Enabled,
				T_FCount,
				T_InitialValue,
				T_Value_Min,
				T_Value_Max,
				T_NeedsUpdate
			> inherited;

	public:
		_V_PROP_( Reversed )
//		T_REVERSED	Reversed;

	protected:
		void UpdateCount( bool ASendOutput )
		{
            if( ! inherited::Enabled() )
				return;

			if( Reversed() )
				inherited::CountDown( ASendOutput );

			else
				inherited::CountUp( ASendOutput );

		}

	public:
		inline void __ICACHE_RAM_ATTR__ InterruptHandler( bool AValue )
		{
			UpdateCount( false );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			UpdateCount( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Value_Max,
		typename T_Value_Min,
		typename T_NeedsUpdate,
		typename T_OutputPin
		> class UpDownCounter :
			public Mitov::BasicCounter <
					T_OutputPin,
					T_Enabled,
					T_FCount,
					T_InitialValue,
					T_Value_Min,
					T_Value_Max,
					T_NeedsUpdate
				>
	{
		typedef Mitov::BasicCounter <
				T_OutputPin,
				T_Enabled,
				T_FCount,
				T_InitialValue,
				T_Value_Min,
				T_Value_Max,
				T_NeedsUpdate
			> inherited;

	public:
		void InterruptHandler_Up( bool )
		{
            if( inherited::Enabled() )
				inherited::CountUp( false );
		}

		void InterruptHandler_Down( bool )
		{
            if( inherited::Enabled() )
				inherited::CountDown( false );
		}

	public:
		inline void UpInputPin_o_Receive( void *_Data )
		{
            if( inherited::Enabled() )
				inherited::CountUp( true );
		}

		inline void DownInputPin_o_Receive( void *_Data )
		{
            if( inherited::Enabled() )
				inherited::CountDown( true );
		}

	};
//---------------------------------------------------------------------------
#undef Min
#undef Max
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif