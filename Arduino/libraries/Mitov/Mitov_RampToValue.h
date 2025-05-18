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
	template <
		typename T_Enabled,
		typename T_FModified,
		typename T_InitialValue,
		typename T_MaxStep,
		typename T_OutputPin,
		typename T_Slope
	> class RampToValue :
		public T_Enabled,
		public T_FModified,
		public T_InitialValue,
		public T_MaxStep,
		public T_OutputPin,
		public T_Slope
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MaxStep )
		_V_PROP_( Slope )

	protected:
		_V_PROP_( FModified )

	protected:
		float	FCurrentValue = 0.0f;

	public:
		inline void UpdateInitialValue()
		{
			FCurrentValue = InitialValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float *)_Data;
//			Serial.println( InitialValue() );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue, false );
			FModified() = false;
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();
			T_OutputPin::SetPinValue( FCurrentValue, false );
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros )
		{
			float ATargetValue = InitialValue().GetValue();
			if( FCurrentValue != ATargetValue )
			{
//				Serial.println( __FUNCTION__ );
//				Serial.print( FCurrentValue ); Serial.print( " " ); Serial.println( ATargetValue ); 
				FModified() = true;
				if( ! Enabled() )
					FCurrentValue = ATargetValue;

				else
				{
					float ACoef =  Slope() / 1000000.0;
//					float ASlope = fabs( ATargetValue - FCurrentValue );
					float ARamp = AElapsedMicros * ACoef;

					if( MaxStep().GetValue() != 0 )
					{
						if( ARamp > 0 )
						{
							if( ARamp > MaxStep().GetValue() )
								ARamp = MaxStep().GetValue();
						}

						else
						{
							if( ARamp < - MaxStep().GetValue() )
								ARamp = - MaxStep().GetValue();
						}
					}
//					Serial.println( ASlope );
					if( FCurrentValue < ATargetValue )
					{
						FCurrentValue += ARamp;
						if( FCurrentValue > ATargetValue )
							FCurrentValue = ATargetValue;

					}

					else
					{
						FCurrentValue -= ARamp;
						if( FCurrentValue < ATargetValue )
							FCurrentValue = ATargetValue;

					}

//					Serial.println( FCurrentValue );
				}
			}
		}

		inline void SystemLoopBegin()
		{
			if( FModified() )
			{
				T_OutputPin::SetPinValue( FCurrentValue, true );
				FModified() = false;
			}
		}

	public:
		inline RampToValue()
		{
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
	namespace RampToColorTypes
	{
		void ProcessColor( uint8_t &ACurrentColor, uint8_t &ATargetColor, int ARamp )
		{
			int ANewColor = ACurrentColor;
			if( ACurrentColor < ATargetColor )
			{
				ANewColor += ARamp;
				if( ANewColor > ATargetColor )
					ANewColor = ATargetColor;

			}
			else
			{
				ANewColor -= ARamp;
				if( ANewColor < ATargetColor )
					ANewColor = ATargetColor;

			}

			ACurrentColor = ANewColor;
		}

	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Slope
	> class RampToColorValue :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin,
		public T_Slope
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Slope )

	public:
		inline void UpdateInitialValue()
		{
			FCurrentValue = InitialValue();
		}

	protected:
		unsigned long	FLastTimeRed = 0;
		unsigned long	FLastTimeGreen = 0;
		unsigned long	FLastTimeBlue = 0;
		float			FSlopeRed;
		float			FSlopeGreen;
		float			FSlopeBlue;
//		TColor			FSlopeValues;
		TColor			FCurrentValue;
//		TColor			FTargetValue;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor AValue = *(TColor *)_Data;
			InitialValue() = AValue;

			unsigned long ACurrentMicros = micros();
			FLastTimeRed = ACurrentMicros;
			FLastTimeGreen = ACurrentMicros;
			FLastTimeBlue = ACurrentMicros;

//			Serial.println( fabs( float( AValue.Red ) - float( FCurrentValue.Red ) ));

			FSlopeRed =  ( fabs( float( AValue.Red ) - float( FCurrentValue.Red )) * Slope() / 1000000.0 );
			FSlopeGreen =  ( fabs( float( AValue.Green ) - int( FCurrentValue.Green )) * Slope() / 1000000.0 );
			FSlopeBlue =  ( fabs( float( AValue.Blue ) - float( FCurrentValue.Blue )) * Slope() / 1000000.0 );

//			Serial.println( FSlopeRed );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
			unsigned long ACurrentMicros = micros();

			TColor ATargetValue = InitialValue().GetValue();
			if( FCurrentValue != ATargetValue )
			{
//				Serial.println( FCurrentValue.ToString() );
				if( ! Enabled() )
					FCurrentValue = InitialValue();

				else
				{
//					float ACoef =  Slope() / 1000000;
//					int ASlope = abs( int( ATargetValue.Red ) - int( FCurrentValue.Red ) );
//					int ARamp = abs( ACoef * float( ACurrentMicros - FLastTimeRed ) * ASlope ) + 0.5;
					int ARamp = FSlopeRed * ( ACurrentMicros - FLastTimeRed ) + 0.5;
//					Serial.println( ARamp );
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Red, ATargetValue.Red, ARamp );
						FLastTimeRed = ACurrentMicros;
					}

//					ASlope = abs( int( ATargetValue.Green ) - int( FCurrentValue.Green ) );
					ARamp = FSlopeGreen * ( ACurrentMicros - FLastTimeGreen ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Green, ATargetValue.Green, ARamp );
						FLastTimeGreen = ACurrentMicros;
					}

//					ASlope = abs( int( ATargetValue.Blue ) - int( FCurrentValue.Blue ) );
					ARamp = FSlopeBlue * ( ACurrentMicros - FLastTimeBlue ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Blue, ATargetValue.Blue, ARamp );
						FLastTimeBlue = ACurrentMicros;
					}
				}

//				Serial.println( FCurrentValue.ToString());
				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
			}

			else if( ! AChangeOnly )
			{
				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
				FLastTimeRed = ACurrentMicros;
				FLastTimeGreen = ACurrentMicros;
				FLastTimeBlue = ACurrentMicros;
			}
//			inherited::SendOutput();
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();

			T_OutputPin::SetPinValue( FCurrentValue, false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Slope
	> class RampToRGBWColorValue :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin,
		public T_Slope
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Slope )

	public:
		inline void UpdateInitialValue()
		{
			FCurrentValue = InitialValue();
		}

	protected:
		unsigned long	FLastTimeRed = 0;
		unsigned long	FLastTimeGreen = 0;
		unsigned long	FLastTimeBlue = 0;
		unsigned long	FLastTimeWhite = 0;
		float			FSlopeRed;
		float			FSlopeGreen;
		float			FSlopeBlue;
		float			FSlopeWhite;
//		TRGBWColor		FSlopeValues;
		TRGBWColor		FCurrentValue;
//		TRGBWColor		FTargetValue;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TRGBWColor AValue = *(TRGBWColor *)_Data;
			InitialValue() = AValue;

			unsigned long ACurrentMicros = micros();
			FLastTimeRed = ACurrentMicros;
			FLastTimeGreen = ACurrentMicros;
			FLastTimeBlue = ACurrentMicros;
			FLastTimeWhite = ACurrentMicros;

			FSlopeRed =  ( fabs( float( AValue.Red ) - float( FCurrentValue.Red )) * Slope() / 1000000.0 );
			FSlopeGreen =  ( fabs( float( AValue.Green ) - int( FCurrentValue.Green )) * Slope() / 1000000.0 );
			FSlopeBlue =  ( fabs( float( AValue.Blue ) - float( FCurrentValue.Blue )) * Slope() / 1000000.0 );
			FSlopeWhite =  ( fabs( float( AValue.White ) - float( FCurrentValue.White )) * Slope() / 1000000.0 );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
			unsigned long ACurrentMicros = micros();

			TRGBWColor	ATargetValue = InitialValue().GetValue();
			if( FCurrentValue != ATargetValue )
			{
				if( ! Enabled() )
					FCurrentValue = ATargetValue;

				else
				{
					int ARamp = FSlopeRed * ( ACurrentMicros - FLastTimeRed ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Red, ATargetValue.Red, ARamp );
						FLastTimeRed = ACurrentMicros;
					}

					ARamp = FSlopeGreen * ( ACurrentMicros - FLastTimeGreen ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Green, ATargetValue.Green, ARamp );
						FLastTimeGreen = ACurrentMicros;
					}

					ARamp = FSlopeBlue * ( ACurrentMicros - FLastTimeBlue ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Blue, ATargetValue.Blue, ARamp );
						FLastTimeBlue = ACurrentMicros;
					}

					ARamp = FSlopeWhite * ( ACurrentMicros - FLastTimeWhite ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.White, ATargetValue.White, ARamp );
						FLastTimeWhite = ACurrentMicros;
					}
				}

				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
			}

			else if( ! AChangeOnly )
			{
				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
				FLastTimeRed = ACurrentMicros;
				FLastTimeGreen = ACurrentMicros;
				FLastTimeBlue = ACurrentMicros;
				FLastTimeWhite = ACurrentMicros;
			}
//			inherited::SendOutput();
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();
			T_OutputPin::SetPinValue( FCurrentValue, false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Slope
	> class RampToAlphaColorValue :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin,
		public T_Slope
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Slope )

	public:
		inline void UpdateInitialValue()
		{
			FCurrentValue = InitialValue();
		}

	protected:
		unsigned long	FLastTimeRed = 0;
		unsigned long	FLastTimeGreen = 0;
		unsigned long	FLastTimeBlue = 0;
		unsigned long	FLastTimeAlpha = 0;
		float			FSlopeRed;
		float			FSlopeGreen;
		float			FSlopeBlue;
		float			FSlopeAlpha;
//		TAlphaColor		FSlopeValues;
		TAlphaColor		FCurrentValue;
//		TAlphaColor		FTargetValue;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TAlphaColor AValue = *(TAlphaColor *)_Data;
			InitialValue() = AValue;

			unsigned long ACurrentMicros = micros();
			FLastTimeRed = ACurrentMicros;
			FLastTimeGreen = ACurrentMicros;
			FLastTimeBlue = ACurrentMicros;
			FLastTimeAlpha = ACurrentMicros;

			FSlopeRed =  ( fabs( float( AValue.Red ) - float( FCurrentValue.Red )) * Slope() / 1000000.0 );
			FSlopeGreen =  ( fabs( float( AValue.Green ) - int( FCurrentValue.Green )) * Slope() / 1000000.0 );
			FSlopeBlue =  ( fabs( float( AValue.Blue ) - float( FCurrentValue.Blue )) * Slope() / 1000000.0 );
			FSlopeAlpha =  ( fabs( float( AValue.Alpha ) - float( FCurrentValue.Alpha )) * Slope() / 1000000.0 );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
			unsigned long ACurrentMicros = micros();

			TAlphaColor	ATargetValue = InitialValue().GetValue();
			if( FCurrentValue != ATargetValue )
			{
				if( ! Enabled() )
					FCurrentValue = ATargetValue;

				else
				{
					int ARamp = FSlopeRed * ( ACurrentMicros - FLastTimeRed ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Red, ATargetValue.Red, ARamp );
						FLastTimeRed = ACurrentMicros;
					}

					ARamp = FSlopeGreen * ( ACurrentMicros - FLastTimeGreen ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Green, ATargetValue.Green, ARamp );
						FLastTimeGreen = ACurrentMicros;
					}

					ARamp = FSlopeBlue * ( ACurrentMicros - FLastTimeBlue ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Blue, ATargetValue.Blue, ARamp );
						FLastTimeBlue = ACurrentMicros;
					}

					ARamp = FSlopeAlpha * ( ACurrentMicros - FLastTimeAlpha ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Alpha, ATargetValue.Alpha, ARamp );
						FLastTimeAlpha = ACurrentMicros;
					}
				}

				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
			}

			else if( ! AChangeOnly )
			{
				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
				FLastTimeRed = ACurrentMicros;
				FLastTimeGreen = ACurrentMicros;
				FLastTimeBlue = ACurrentMicros;
				FLastTimeAlpha = ACurrentMicros;
			}
//			inherited::SendOutput();
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();
			T_OutputPin::SetPinValue( FCurrentValue, false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Slope
	> class RampToRGBWAlphaColorValue :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin,
		public T_Slope
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Slope )

	public:
		inline void UpdateInitialValue()
		{
			FCurrentValue = InitialValue();
		}

	protected:
		unsigned long	FLastTimeRed = 0;
		unsigned long	FLastTimeGreen = 0;
		unsigned long	FLastTimeBlue = 0;
		unsigned long	FLastTimeWhite = 0;
		unsigned long	FLastTimeAlpha = 0;
		float			FSlopeRed;
		float			FSlopeGreen;
		float			FSlopeBlue;
		float			FSlopeWhite;
		float			FSlopeAlpha;
//		TAlphaColor		FSlopeValues;
		TRGBWAlphaColor	FCurrentValue;
//		TAlphaColor		FTargetValue;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TRGBWAlphaColor AValue = *(TRGBWAlphaColor *)_Data;
			InitialValue() = AValue;

//            TRGBWAlphaColor ATest1 = InitialValue().GetValue();

			unsigned long ACurrentMicros = micros();
			FLastTimeRed = ACurrentMicros;
			FLastTimeGreen = ACurrentMicros;
			FLastTimeBlue = ACurrentMicros;
			FLastTimeWhite = ACurrentMicros;
			FLastTimeAlpha = ACurrentMicros;

			FSlopeRed =  ( fabs( float( AValue.Red ) - float( FCurrentValue.Red )) * Slope() / 1000000.0 );
			FSlopeGreen =  ( fabs( float( AValue.Green ) - int( FCurrentValue.Green )) * Slope() / 1000000.0 );
			FSlopeBlue =  ( fabs( float( AValue.Blue ) - float( FCurrentValue.Blue )) * Slope() / 1000000.0 );
			FSlopeWhite =  ( fabs( float( AValue.White ) - float( FCurrentValue.White )) * Slope() / 1000000.0 );
			FSlopeAlpha =  ( fabs( float( AValue.Alpha ) - float( FCurrentValue.Alpha )) * Slope() / 1000000.0 );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
			unsigned long ACurrentMicros = micros();

			TRGBWAlphaColor	ATargetValue = InitialValue().GetValue();
			if( FCurrentValue != ATargetValue )
			{
				if( ! Enabled() )
					FCurrentValue = ATargetValue;

				else
				{
					int ARamp = FSlopeRed * ( ACurrentMicros - FLastTimeRed ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Red, ATargetValue.Red, ARamp );
						FLastTimeRed = ACurrentMicros;
					}

					ARamp = FSlopeGreen * ( ACurrentMicros - FLastTimeGreen ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Green, ATargetValue.Green, ARamp );
						FLastTimeGreen = ACurrentMicros;
					}

					ARamp = FSlopeBlue * ( ACurrentMicros - FLastTimeBlue ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Blue, ATargetValue.Blue, ARamp );
						FLastTimeBlue = ACurrentMicros;
					}

					ARamp = FSlopeWhite * ( ACurrentMicros - FLastTimeWhite ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.White, ATargetValue.White, ARamp );
						FLastTimeWhite = ACurrentMicros;
					}

					ARamp = FSlopeAlpha * ( ACurrentMicros - FLastTimeAlpha ) + 0.5;
					if( ARamp )
					{
						RampToColorTypes::ProcessColor( FCurrentValue.Alpha, ATargetValue.Alpha, ARamp );
						FLastTimeAlpha = ACurrentMicros;
					}
				}

				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
			}

			else if( ! AChangeOnly )
			{
				T_OutputPin::SetPinValue( FCurrentValue, AChangeOnly );
				FLastTimeRed = ACurrentMicros;
				FLastTimeGreen = ACurrentMicros;
				FLastTimeBlue = ACurrentMicros;
				FLastTimeWhite = ACurrentMicros;
				FLastTimeAlpha = ACurrentMicros;
			}
//			inherited::SendOutput();
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();
			T_OutputPin::SetPinValue( FCurrentValue, false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif