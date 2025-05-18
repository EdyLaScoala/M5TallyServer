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

// Based on ideas from https://github.com/empierre/arduino/blob/master/AirQuality-Multiple_Gas_Sensor1_4.ino

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
/*
	class GasSensorMQ7 : public Mitov::CommonFilter
	{
		typedef Mitov::CommonFilter inherited;

    public:
        bool	Enabled = true;
		float	LoadResistor = 10000.0;
		float	CleanAirValue = 0.10;

	protected:
		virtual void DoReceive( void *_Data ) override
		{
//			float Ro = (((1.0 - CleanAirValue) * LoadResistor / CleanAirValue ) / LoadResistor); // Ro =[(Vc - Vo) * RL ] / (Vo * Ro_clean_air_factor)
			float Ro = (((1.0 - CleanAirValue) * LoadResistor ) / ( CleanAirValue * LoadResistor )); // Ro =[(Vc - Vo) * RL ] / (Vo * Ro_clean_air_factor)

			float Vo = *((float *)_Data);
			float Rs = (( 1.0 - Vo) * LoadResistor) / Vo;
			float RsRo_ratio = Rs / Ro;
			float ppm = pow((RsRo_ratio/22.073), (1/-0.66659));

			Serial.print( Ro ); Serial.print( " / " ); Serial.println( Rs );

			inherited::OutputPin.Notify( &ppm );
		}

	};
*/
	namespace GasSensorMQ135Const
	{
		const float	CCurves[ 6 ][ 2 ] =
		{
			{113.7105289, -3.019713765},  //MQ135 CO2
			{726.7809737, -4.040111669},  //MQ135 CO
			{47.01770503, -3.281901967},  //MQ135 CH3 - Methyl
			{84.07117895, -4.41107687},   //MQ135 NH4 - Ammonium
			{7.010800878, -2.122018939},  //MQ135 (CH3)2CO - Acetone
			{74.77989144, 3.010328075}    //MQ135 C2H50H - Alcohol, Ethanol
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_CleanAirValue,
		typename T_Enabled,
		typename T_FCalibrating,
		typename T_FChangeOnly,
		typename T_FFirstSample,
		typename T_LoadResistor,
		typename T_OutputPins_CO2,
		typename T_OutputPins_CO,
		typename T_OutputPins_Methyl,
		typename T_OutputPins_Ammonium,
		typename T_OutputPins_Acetone,
		typename T_OutputPins_Ethanol
	> class GasSensorMQ135 :
		public T_CleanAirValue,
		public T_Enabled,
		public T_FCalibrating,
		public T_FChangeOnly,
		public T_FFirstSample,
		public T_LoadResistor,
		public T_OutputPins_CO2,
		public T_OutputPins_CO,
		public T_OutputPins_Methyl,
		public T_OutputPins_Ammonium,
		public T_OutputPins_Acetone,
		public T_OutputPins_Ethanol
	{
//		static const float           CO2Curve[2]     =  {113.7105289, -3.019713765};  //MQ135
//		static const float           CO_secCurve[2]  =  {726.7809737, -4.040111669};  //MQ135
//		static const float           NH4Curve[2]     =  {84.07117895, -4.41107687};   //MQ135 Ammonium
//		static const float           C2H50H_Curve[2] =  {74.77989144, 3.010328075};   //MQ135 Ethanol
//		static const float           CH3Curve[2]     =  {47.01770503, -3.281901967};  //MQ135 Methyl
//		static const float           CH3_2COCurve[2] =  {7.010800878, -2.122018939};  //MQ135 Acetone

    public:
		_V_PIN_( OutputPins_CO2 )
		_V_PIN_( OutputPins_CO )
		_V_PIN_( OutputPins_Methyl )
		_V_PIN_( OutputPins_Ammonium )
		_V_PIN_( OutputPins_Acetone )
		_V_PIN_( OutputPins_Ethanol )

    public:
		_V_PROP_( LoadResistor )
		_V_PROP_( CleanAirValue )

        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FChangeOnly )

	public:
		_V_PROP_( FCalibrating )
		_V_PROP_( FFirstSample )

    public:
		float FTemperature = 20;
		float FHumidity	= 33;

	protected:
		float Convert( uint8_t AIndex, float Ro, float Rs )
		{
			const float	CORA = 0.00035;
			const float	CORB = 0.02718;
			const float	CORC = 1.39538;
			const float	CORD = 0.0018;

			float PPM = (GasSensorMQ135Const::CCurves[ AIndex ][ 0 ] * pow(( Rs / Ro ), GasSensorMQ135Const::CCurves[ AIndex ][ 1 ]));

//			Serial.println( FTemperature );
//			Serial.println( FHumidity );

			float ACorrectionFactor = CORA * FTemperature * FTemperature - CORB * FTemperature + CORC - ( FHumidity - 33.0 ) * CORD;

			PPM *= ACorrectionFactor;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *((float *)_Data );

			if( FCalibrating() )
			{
				if( FFirstSample() )
				{
					FFirstSample() = false;
					CleanAirValue() = AValue;
				}

				else
					CleanAirValue() = ( CleanAirValue() + AValue ) / 2;

				return;
			}

//			Serial.println( CleanAirValue );

			float Ro = ((1000 * LoadResistor() ) / CleanAirValue() - LoadResistor() );
			float Rs = ((1000 * LoadResistor() ) / AValue - LoadResistor() );
			if( T_OutputPins_CO2::GetPinIsConnected() )
				T_OutputPins_CO2::SetPinValue( Convert( 0, Ro, Rs ), FChangeOnly() );
/*
		_V_PIN_( OutputPins_CO2 )
		_V_PIN_( OutputPins_CO )
		_V_PIN_( OutputPins_Methyl )
		_V_PIN_( OutputPins_Ammonium )
		_V_PIN_( OutputPins_Acetone )
		_V_PIN_( OutputPins_Ethanol )

			for( int i = 0; i < 6; ++i )
				if( OutputPins[ i ].IsConnected() )
				{					
					const float	CORA = 0.00035;
					const float	CORB = 0.02718;
					const float	CORC = 1.39538;
					const float	CORD = 0.0018;

					float PPM = (CCurves[ i ][ 0 ] * pow(( Rs / Ro ), CCurves[ i ][ 1 ]));

//					Serial.println( FTemperature );
//					Serial.println( FHumidity );

					float ACorrectionFactor = CORA * FTemperature * FTemperature - CORB * FTemperature + CORC - ( FHumidity - 33.0 ) * CORD;

					PPM *= ACorrectionFactor;

					OutputPins[ i ].SetValue( PPM, FChangeOnly );
				}
*/			
			FChangeOnly() = true;
		}

	public:
		inline GasSensorMQ135()
		{
			FChangeOnly() = false;
			FCalibrating() = false;
			FFirstSample() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class TArduinoGasSensorMQCorrection
	{
	public:
		inline void TemperatureInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FTemperature = *(float *)_Data;
		}

		inline void HumidityInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FHumidity = *(float *)_Data;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CallibratingOutputPin,
		typename T_Period
	> class TArduinoGasSensorMQCallibration :
		public T_CallibratingOutputPin,
		public T_Period
	{
	public:
		_V_PIN_( CallibratingOutputPin )

	public:
		_V_PROP_( Period )

	protected:
		unsigned long	FLastTime = 0;

	public:
		void CallibrateInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FCalibrating() = true;
			C_OWNER.FFirstSample() = true;
			FLastTime = millis();
			T_CallibratingOutputPin::SetPinValue( true, true );
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! C_OWNER.FCalibrating() )
				return;

			if( millis() - FLastTime > Period() )
			{
				C_OWNER.FCalibrating() = false;
				T_CallibratingOutputPin::SetPinValue( false, true );
			}
		}

		inline void SystemStart()
		{
			T_CallibratingOutputPin::SetPinValue( false, false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif