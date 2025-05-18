////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_PWM_PCA9685.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	const	uint8_t YahboomMicrobitRobotNeoPixelPin = Mitov::MicroBitPin_16;
	const	uint8_t YahboomMicrobitRobotIRReceiverPin = Mitov::MicroBitPin_8;
	const	uint8_t YahboomMicrobitRobotIRObstacleDetectPin = Mitov::MicroBitPin_3;
	const	uint8_t YahboomMicrobitRobotIRObstacleLightPin = Mitov::MicroBitPin_9;
	const	uint8_t YahboomMicrobitRobotBuzzerPin = Mitov::MicroBitPin_0;
	const	uint8_t YahboomMicrobitRobotLeftLineFollowingSensorPin = Mitov::MicroBitPin_2;
	const	uint8_t YahboomMicrobitRobotRightLineFollowingSensorPin = Mitov::MicroBitPin_1;
	const	uint8_t YahboomMicrobitRobotSonicPingPin = Mitov::MicroBitPin_14;
	const	uint8_t YahboomMicrobitRobotSonicEchoPin = Mitov::MicroBitPin_15;

//---------------------------------------------------------------------------
namespace YahBoomVisuinoConstants
{
class FloatValue_Frequency
{
public:
    inline static constexpr float GetValue() { return 60; }
};

class FloatValue_OscilatorFrequency
{
public:
    inline static constexpr float GetValue() { return 25000000; }
};

} // VisuinoConstants
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Channels_UpdateValues
//		typename T_ClockInputPin_o_IsConnected,
//		typename T_Enabled,
//		typename T_ExternalClock,
//		typename T_Frequency,
//		typename T_OpenDrain,
//		typename T_OscilatorFrequency,
//		typename T_OutputLogic,
//		typename T_Sleep,
//		typename T_UpdateOnAck
	> class YahboomMicrobitPWM : 
		public PCA9685PWM <
				T_I2C, C_I2C,
//				65,
				Mitov::ConstantProperty<5, uint8_t, 65 >, // Address
				T_Channels_UpdateValues,
//				Mitov::EmbeddedCallChain<CallChains::UpdateValues1>, // Channels_UpdateValues
				Mitov::ConstantProperty<4, bool, false>, // ClockInputPin_o_IsConnected
				Mitov::ConstantProperty<15, bool, true>, // Enabled
				Mitov::ConstantProperty<10, bool, false>, // ExternalClock
				Mitov::ConstantPropertyFloat<2, float, YahBoomVisuinoConstants::FloatValue_Frequency>, // Frequency
				Mitov::ConstantProperty<7, bool, false>, // OpenDrain
				Mitov::ConstantPropertyFloat<6, float, YahBoomVisuinoConstants::FloatValue_OscilatorFrequency>, // OscilatorFrequency
				Mitov::NestedProperty<13, Mitov::TArduinoAnalogPCA9685OutputLogic<
					Mitov::ConstantProperty<12, bool, false>, // Inverted
					Mitov::ConstantProperty<13, Mitov::TArduinoAnalogPCA9685OutputLogicMode, Mitov::olmZero> // Mode
					> >, // OutputLogic
				Mitov::ConstantProperty<8, bool, false>, // Sleep
				Mitov::ConstantProperty<9, bool, false> // UpdateOnAck
			>
	{
/*
		typedef PCA9685PWM <
				T_I2C, C_I2C,
				65,
				T_Channels_UpdateValues,
				Mitov::ConstantProperty<4, bool, false>, // ClockInputPin_o_IsConnected
				Mitov::ConstantProperty<15, bool, true>, // Enabled
				Mitov::ConstantProperty<10, bool, false>, // ExternalClock
				Mitov::ConstantPropertyFloat<2, float, YahBoomVisuinoConstants::FloatValue_Frequency>, // Frequency
				Mitov::ConstantProperty<7, bool, false>, // OpenDrain
				Mitov::ConstantPropertyFloat<6, float, YahBoomVisuinoConstants::FloatValue_OscilatorFrequency>, // OscilatorFrequency
				Mitov::NestedProperty<13, Mitov::TArduinoAnalogPCA9685OutputLogic<
					Mitov::ConstantProperty<12, bool, false>, // Inverted
					Mitov::ConstantProperty<13, Mitov::TArduinoAnalogPCA9685OutputLogicMode, Mitov::olmZero> // Mode
					> >, // OutputLogic
				Mitov::ConstantProperty<8, bool, false>, // Sleep
				Mitov::ConstantProperty<9, bool, false> // UpdateOnAck
			> inherited;

	public:
		YahboomMicrobitPWM()
		{
			inherited::Frequency = 60; // For Servo compatibility
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_PWM, T_PWM &C_PWM,
		typename T_InitialValue
	> class YahboomMicrobitRobotLEDLights :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	public:
		inline void SystemStart()
		{
			C_PWM.WriteChannelData( 0, 0, ( float( InitialValue().GetReference().Red ) / 255 ) * 4095 + 0.5 );
			C_PWM.WriteChannelData( 1, 0, ( float( InitialValue().GetReference().Green ) / 255 ) * 4095 + 0.5 );
			C_PWM.WriteChannelData( 2, 0, ( float( InitialValue().GetReference().Blue ) / 255 ) * 4095 + 0.5 );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetReference() )
				return;

			InitialValue() = AColor;
			SystemStart();
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_Enabled,
		typename T_InitialValue
	> class YahboomMicrobitRobotLED :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( FModified )

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				SetChannelValue( C_INDEX, 1 - InitialValue );

		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AOnValue = *(float *)_Data;
			SetChannelValue( AOnValue );
		}

	};
//---------------------------------------------------------------------------
	template <uint8_t C_INDEX, typename T_OWNER, T_OWNER &C_OWNER> class YahboomMicrobitRobotLED_Enable
	{
	public:
		bool	Enabled = false;
		float	InitialValue = 0;

	public:
		void UpdateOutputs()
		{
			if( Enabled )
				C_OWNER.SetChannelValue( C_INDEX, 1 - InitialValue );

			else
				C_OWNER.SetChannelValue( C_INDEX, 1 );

		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue == InitialValue )
				return;

			InitialValue = AValue;
			UpdateOutputs();
		}
	};
*/
//---------------------------------------------------------------------------
	class TAnalogTransform_YahboomMicrobitRobotServo
	{
	public:
		static inline float Transform( float AValue ) { return Func::MapRange( AValue, 0.0f, 1.0f, 0.036630037f, 0.146520147f ); }

	};
//---------------------------------------------------------------------------
/*
	template <uint8_t C_INDEX, typename T_OWNER, T_OWNER &C_OWNER> class YahboomMicrobitRobotServo
	{
	public:
		float	InitialValue = 0;

	public:
		inline void SystemStart()
		{
			float	AValue = MitovConstrain( InitialValue, 0.0f, 1.0f );
			AValue = Func::MapRange( AValue, 0.0f, 1.0f, 0.036630037f, 0.146520147f );
			C_OWNER.SetChannelValue( C_INDEX + 3, AValue );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue == InitialValue )
				return;

			InitialValue = AValue;
			SystemStart();
		}
	};
//---------------------------------------------------------------------------
	template <uint8_t C_INDEX, typename T_OWNER, T_OWNER &C_OWNER> class YahboomMicrobitRobotServo_Enable
	{
	public:
		bool	Enabled = false;
		float	InitialValue = 0;

	public:
		void UpdateOutputs()
		{
			if( Enabled )
			{
				float	AValue = MitovConstrain( InitialValue, 0.0f, 1.0f );
				AValue = Func::MapRange( AValue, 0.0f, 1.0f, 0.036630037f, 0.146520147f );
				C_OWNER.SetChannelValue( C_INDEX + 3, AValue );
			}

		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( AValue == InitialValue )
				return;

			InitialValue = AValue;
			UpdateOutputs();
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class YahboomMicrobitRobotObstacleAvoidanceSensor :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
			digitalWrite( YahboomMicrobitRobotIRObstacleLightPin, ( Enabled() ) ? LOW : HIGH );
		}

	public:
		inline void SystemInit()
		{
			pinMode( YahboomMicrobitRobotIRObstacleLightPin, OUTPUT );
			pinMode( YahboomMicrobitRobotIRObstacleDetectPin, INPUT );
		}

		inline void SystemStart()
		{
			UpdateEnabled();
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() )
			{
				bool AValue = ! digitalRead( YahboomMicrobitRobotIRObstacleDetectPin );
				T_OutputPin::SetPinValue( AValue );
			}

			else
				T_OutputPin::SetPinValueLow();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
