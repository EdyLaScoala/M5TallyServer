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
		typename T_Enabled,
		typename T_FOldValueX,
		typename T_FOldValueY,
		typename T_XOutputPin,
		typename T_YOutputPin
	> class Memsic2125Accelerometer :
		public T_Enabled,
		public T_FOldValueX,
		public T_FOldValueY,
		public T_XOutputPin,
		public T_YOutputPin
	{
	public:
		_V_PIN_( XOutputPin )
		_V_PIN_( YOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FOldValueX )
		_V_PROP_( FOldValueY )

	protected:
		class DataSet
		{
		protected:
			unsigned long	FStartTime = 0;

		public:
			bool CalculateAcceleration( bool AValue, float & AAcceleration )
			{
				unsigned long ANow = micros();
				if( AValue )
				{
					FStartTime = ANow;
					return false;
				}

				long APeriod = ANow - FStartTime;

				if( APeriod == 0 )
					APeriod = 1;

				AAcceleration = (( APeriod / 10 ) - 500) * 8;
				return true;

//				AOutputPin->Notify( &AAcceleration );
			}

		};


	protected:
		DataSet	FXDataSet;
		DataSet	FYDataSet;

	public:
		void XInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			bool AValue = *(bool *)_Data;
			if( FOldValueX() == AValue )
				return;

			FOldValueX() = AValue;
			float AAcceleration;
			if( FXDataSet.CalculateAcceleration( AValue, AAcceleration ))
				T_XOutputPin::SetPinValue( AAcceleration );

		}

		void YInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			bool AValue = *(bool *)_Data;
			if( FOldValueY() == AValue )
				return;

			FOldValueY() = AValue;
			float AAcceleration;
			if( FYDataSet.CalculateAcceleration( AValue, AAcceleration ))
				T_YOutputPin::SetPinValue( AAcceleration );

		}

	public:
		Memsic2125Accelerometer()
		{
			FOldValueX() = false;
			FOldValueY() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_AccelerationOutputPinsX,
		typename T_AccelerationOutputPinsY,
		typename T_AccelerationOutputPinsZ,
		typename T_Enabled
	> class AccelerometerADXL335 :
		public T_AccelerationOutputPinsX,
		public T_AccelerationOutputPinsY,
		public T_AccelerationOutputPinsZ,
		public T_Enabled
	{
	public:
		_V_PIN_( AccelerationOutputPinsX )
		_V_PIN_( AccelerationOutputPinsY )
		_V_PIN_( AccelerationOutputPinsZ )

	public:
		_V_PROP_( Enabled )

	public:
		void AnalogInputPins_o_Receive( int AIndex, void *_Data )
		{
			if( ! Enabled() )
				return;

			// min 0.27 and max 0.4
			float AValue = *(float*)_Data;

			float AOutValue = Func::MapRange<float>( AValue, 0.27, 0.4, -1.0, 1.0 );

			switch( AIndex )
			{
				case 0: T_AccelerationOutputPinsX::SetPinValue( AOutValue ); break;
				case 1: T_AccelerationOutputPinsY::SetPinValue( AOutValue ); break;
				case 2: T_AccelerationOutputPinsZ::SetPinValue( AOutValue ); break;
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif