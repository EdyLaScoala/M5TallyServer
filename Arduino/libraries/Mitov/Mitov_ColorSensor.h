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

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoColorSensorThaosFrequencyScaling { cfsScaling2, cfsScaling20, cfsScaling100 };
//---------------------------------------------------------------------------
	template <
		typename T_Blue,
		typename T_Clear,
		typename T_Green,
		typename T_Red
	> class TArduinoColorSensorThaosScaleDividers :
		public T_Blue,
		public T_Clear,
		public T_Green,
		public T_Red
	{
	public:
		_V_PROP_( Red )
		_V_PROP_( Green )
		_V_PROP_( Blue )
		_V_PROP_( Clear )

	};
//---------------------------------------------------------------------------
	template <
//		typename T_ColorOutputPin,
		typename T_Enabled,
		typename T_FCurrentState,
		typename T_FrequencyScaling,
		typename T_OutputPinsRed,
		typename T_OutputPinsGreen,
		typename T_OutputPinsBlue,
		typename T_OutputPinsClear,
		typename T_PoweredUp,
		typename T_ScaleDividers,
		typename T_SensorSelectOutputPins_S0,
		typename T_SensorSelectOutputPins_S1,
		typename T_SensorSelectOutputPins_S2,
		typename T_SensorSelectOutputPins_S3
	> class ColorSensor :
//		public T_ColorOutputPin,
		public T_Enabled,
		public T_FCurrentState,
		public T_FrequencyScaling,
		public T_OutputPinsRed,
		public T_OutputPinsGreen,
		public T_OutputPinsBlue,
		public T_OutputPinsClear,
		public T_PoweredUp,
		public T_ScaleDividers,
		public T_SensorSelectOutputPins_S0,
		public T_SensorSelectOutputPins_S1,
		public T_SensorSelectOutputPins_S2,
		public T_SensorSelectOutputPins_S3
	{
	public:
//		_V_PIN_( ColorOutputPin )

		_V_PIN_( OutputPinsRed )
		_V_PIN_( OutputPinsGreen )
		_V_PIN_( OutputPinsBlue )
		_V_PIN_( OutputPinsClear )

		_V_PIN_( SensorSelectOutputPins_S0 )
		_V_PIN_( SensorSelectOutputPins_S1 )
		_V_PIN_( SensorSelectOutputPins_S2 )
		_V_PIN_( SensorSelectOutputPins_S3 )

	public:
		_V_PROP_( PoweredUp )
		_V_PROP_( FrequencyScaling )
		_V_PROP_( ScaleDividers )

	protected:
		_V_PROP_( FCurrentState )

		uint32_t FCounter = 0;
		unsigned long FLastTime = 0;

	public:
		void UpdateFrequencyScaling()
		{
			if( PoweredUp() )
			{
				switch( FrequencyScaling() )
				{
					case cfsScaling2:
						T_SensorSelectOutputPins_S0::SetPinValueLow();
						T_SensorSelectOutputPins_S1::SetPinValueHigh();
						break;

					case cfsScaling20:
						T_SensorSelectOutputPins_S0::SetPinValueHigh();
						T_SensorSelectOutputPins_S1::SetPinValueLow();
						break;

					case cfsScaling100:
						T_SensorSelectOutputPins_S0::SetPinValueHigh();
						T_SensorSelectOutputPins_S1::SetPinValueHigh();
						break;
				}
			}

			else
			{
				T_SensorSelectOutputPins_S0::SetPinValueLow();
				T_SensorSelectOutputPins_S1::SetPinValueLow();
			}
		}

	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler( bool )
		{
			++ FCounter;
		}

	public:
		inline void SystemInit()
		{
			UpdateFrequencyScaling();
		}

		inline void SystemStart()
		{
			FLastTime = micros();

			T_SensorSelectOutputPins_S2::SetPinValueLow();
			T_SensorSelectOutputPins_S3::SetPinValueLow();
		}

		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			unsigned long ATimeDiff = ( ACurrentMicros - FLastTime );

			noInterrupts(); // Protect the FReceivedPackets from being corrupted
			uint32_t ACount = FCounter; 
			if( ( ACount < 10 ) && ( ATimeDiff < 1000 ) )
			{
				interrupts();
				return;
			}

//			FCounter = 0;

			interrupts();

//			Serial.println( ACount );

			float AFrequency = ( float( ACount ) / ATimeDiff ) * 1000000;
//			Serial.println( AFrequency );

			FLastTime = ACurrentMicros;

			const uint8_t PinMap[] = { 0, 2, 3, 1 };

			switch( FCurrentState() )
			{
				case 0: // Red
					AFrequency /= ScaleDividers().Red();
					break;

				case 1: // Blue
					AFrequency /= ScaleDividers().Blue();
					break;

				case 2: // Clear
					AFrequency /= ScaleDividers().Clear();
					break;

				case 3: // Green
					AFrequency /= ScaleDividers().Green();
					break;
			};

//			OutputPins[ PinMap[ FCurrentState() ] ].Notify( &AFrequency );
			switch( PinMap[ FCurrentState() ] )
			{
				case 0: T_OutputPinsRed::SetPinValue( AFrequency ); break;
				case 1: T_OutputPinsGreen::SetPinValue( AFrequency ); break;
				case 2: T_OutputPinsBlue::SetPinValue( AFrequency ); break;
				case 3: T_OutputPinsClear::SetPinValue( AFrequency ); break;
			}

/*
			switch( FCurrentState )
			{
				case 0:
					break;

				case 1:
					break;

				case 2:
					break;

				case 3:
					break;
			}
*/
			for( int i = 0; i < 4; ++i )
			{
				FCurrentState() = FCurrentState() + 1;
				if( FCurrentState() > 3 )
					FCurrentState() = 0;

//				if( OutputPins[ PinMap[ FCurrentState() ] ].IsConnected() )
//					break;
				bool AIsConnected = false;
				switch( PinMap[ FCurrentState() ] )
				{
					case 0: AIsConnected = T_OutputPinsRed::GetPinIsConnected(); break;
					case 1: AIsConnected = T_OutputPinsGreen::GetPinIsConnected(); break;
					case 2: AIsConnected = T_OutputPinsBlue::GetPinIsConnected(); break;
					case 3: AIsConnected = T_OutputPinsClear::GetPinIsConnected(); break;
				}

				if( AIsConnected )
					break;
			}

			T_SensorSelectOutputPins_S2::SetPinValue( ( FCurrentState() & 1 ) != 0 );
			T_SensorSelectOutputPins_S3::SetPinValue( ( FCurrentState() & 2 ) != 0 );

//			delay( 10 );

			// Restart the count with the new pin settings
			FLastTime = micros();
			noInterrupts(); // Protect the FReceivedPackets from being corrupted
			FCounter = 0;
			interrupts();
		}

	public:
		inline ColorSensor()
		{
			FCurrentState() = 0;
		}
/*
		ColorSensor( int AInterruptPin, void (*AInterruptRoutine)() )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			// On all other platforms, its innocuous, belt and braces
			pinMode( AInterruptPin, INPUT ); 

			int AInterruptNumber = digitalPinToInterrupt( AInterruptPin );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, RISING );
		}
*/
	};
}