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
//#include <SPI.h>
#include <Mitov_BasicMPU.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
namespace LIS3DH
{
/*!
 *  STATUS_REG_AUX register
 *   321OR  1, 2 and 3 axis data overrun. Default value: 0
 *          (0: no overrun has occurred; 1: a new set of data has overwritten
 * the previous ones) 3OR    3 axis data overrun. Default value: 0 (0: no
 * overrun has occurred; 1: a new data for the 3-axis has overwritten the
 * previous one) 2OR    2 axis data overrun. Default value: 0 (0: no overrun has
 * occurred; 1: a new data for the 4-axis has overwritten the previous one) 1OR
 * 1 axis data overrun. Default value: 0 (0: no overrun has occurred; 1: a new
 * data for the 1-axis has overwritten the previous one) 321DA  1, 2 and 3 axis
 * new data available. Default value: 0 (0: a new set of data is not yet
 * available; 1: a new set of data is available) 3DA:   3 axis new data
 * available. Default value: 0 (0: a new data for the 3-axis is not yet
 * available; 1: a new data for the 3-axis is available) 2DA:   2 axis new data
 * available. Default value: 0 (0: a new data for the 2-axis is not yet
 * available; 1: a new data for the 2-axis is available) 1DA    1 axis new data
 * available. Default value: 0 (0: a new data for the 1-axis is not yet
 * available; 1: a new data for the 1-axis is available)
 */
	const uint8_t REG_STATUS1		= 0x07;
	const uint8_t REG_OUTADC1_L		= 0x08; // **< ADC1. Low value
	const uint8_t REG_OUTADC1_H		= 0x09; // **< ADC1. High value
	const uint8_t REG_OUTADC2_L		= 0x0A; // **< ADC2. Low value
	const uint8_t REG_OUTADC2_H		= 0x0B; // **< ADC2. High value
	const uint8_t REG_OUTADC3_L		= 0x0C; // **< ADC3. Low value
	const uint8_t REG_OUTADC3_H		= 0x0D; // **< ADC3. High value/
	const uint8_t REG_INTCOUNT		= 0x0E; // **< INT_COUNTER register [IC7, IC6, IC5, IC4, IC3, IC2, IC1, IC0]
	const uint8_t REG_WHOAMI		= 0x0F; // **< Device identification register. [0, 0, 1, 1, 0, 0, 1, 1]

	const uint8_t REG_CTRL0			= 0x1E;

/*!
 *  TEMP_CFG_REG
 *  Temperature configuration register.
 *   ADC_PD   ADC enable. Default value: 0
 *            (0: ADC disabled; 1: ADC enabled)
 *   TEMP_EN  Temperature sensor (T) enable. Default value: 0
 *            (0: T disabled; 1: T enabled)
 */
	const uint8_t REG_TEMPCFG		= 0x1F;
/*!
 *  CTRL_REG1
 *  [ODR3, ODR2, ODR1, ODR0, LPen, Zen, Yen, Xen]
 *   ODR3-0  Data rate selection. Default value: 00
 *           (0000:50 Hz; Others: Refer to Datasheet Table 26, “Data rate
 * configuration”) LPen    Low power mode enable. Default value: 0 (0: normal
 * mode, 1: low power mode) Zen     Z axis enable. Default value: 1 (0: Z axis
 * disabled; 1: Z axis enabled) Yen     Y axis enable. Default value: 1 (0: Y
 * axis disabled; 1: Y axis enabled) Xen     X axis enable. Default value: 1 (0:
 * X axis disabled; 1: X axis enabled)
 */
	const uint8_t REG_CTRL1			= 0x20;
/*!
 *  CTRL_REG2
 *  [HPM1, HPM0, HPCF2, HPCF1, FDS, HPCLICK, HPIS2, HPIS1]
 *   HPM1-0  High pass filter mode selection. Default value: 00
 *           Refer to Table 29, "High pass filter mode configuration"
 *   HPCF2-1 High pass filter cut off frequency selection
 *   FDS     Filtered data selection. Default value: 0
 *					 (0: internal filter bypassed; 1: data from
 *internal filter sent to output register and FIFO) HPCLICK High pass filter
 *enabled for CLICK function. (0: filter bypassed; 1: filter enabled) HPIS2   X
 *axis enable. Default value: 1 (0: X axis disabled; 1: X axis enabled) HPIS1
 *High pass filter enabled for AOI function on interrupt 1, (0: filter bypassed;
 *1: filter enabled)
 */
	const uint8_t REG_CTRL2			= 0x21;
/*!
 *  CTRL_REG3
 *  [I1_CLICK, I1_AOI1, I1_AOI2, I1_DRDY1, I1_DRDY2, I1_WTM, I1_OVERRUN, --]
 *   I1_CLICK    CLICK interrupt on INT1. Default value 0.
 *						   (0: Disable; 1: Enable)
 *   I1_AOI1     AOI1 interrupt on INT1. Default value 0.
 *						   (0: Disable; 1: Enable)
 *   I1_AOI2     AOI2 interrupt on INT1. Default value 0.
 *               (0: Disable; 1: Enable)
 *   I1_DRDY1    DRDY1 interrupt on INT1. Default value 0.
 *               (0: Disable; 1: Enable)
 *   I1_DRDY2    DRDY2 interrupt on INT1. Default value 0.
 *               (0: Disable; 1: Enable)
 *   I1_WTM      FIFO Watermark interrupt on INT1. Default value 0.
 *               (0: Disable; 1: Enable)
 *   I1_OVERRUN  FIFO Overrun interrupt on INT1. Default value 0.
 * 							 (0: Disable; 1: Enable)
 */
	const uint8_t REG_CTRL3			= 0x22;
/*!
 *  CTRL_REG4
 *  [BDU, BLE, FS1, FS0, HR, ST1, ST0, SIM]
 *   BDU      Block data update. Default value: 0
 *            (0: continuos update; 1: output registers not updated until MSB
 * and LSB reading) BLE      Big/little endian data selection. Default value 0.
 *            (0: Data LSB @ lower address; 1: Data MSB @ lower address)
 *   FS1-FS0  Full scale selection. default value: 00
 *            (00: +/- 2G; 01: +/- 4G; 10: +/- 8G; 11: +/- 16G)
 *   HR       High resolution output mode: Default value: 0
 *            (0: High resolution disable; 1: High resolution Enable)
 *   ST1-ST0  Self test enable. Default value: 00
 *            (00: Self test disabled; Other: See Table 34)
 *   SIM      SPI serial interface mode selection. Default value: 0
 *            (0: 4-wire interface; 1: 3-wire interface).
 */
	const uint8_t REG_CTRL4			= 0x23;
/*!
 *  CTRL_REG5
 *  [BOOT, FIFO_EN, --, --, LIR_INT1, D4D_INT1, 0, 0]
 *   BOOT     Reboot memory content. Default value: 0
 *            (0: normal mode; 1: reboot memory content)
 *   FIFO_EN  FIFO enable. Default value: 0
 *            (0: FIFO disable; 1: FIFO Enable)
 *   LIR_INT1 Latch interrupt request on INT1_SRC register, with INT1_SRC
 * register cleared by reading INT1_SRC itself. Default value: 0. (0: interrupt
 * request not latched; 1: interrupt request latched) D4D_INT1 4D enable: 4D
 * detection is enabled on INT1 when 6D bit on INT1_CFG is set to 1.
 */
	const uint8_t REG_CTRL5			= 0x24;

/*!
 *  CTRL_REG6
 *  [I2_CLICKen, I2_INT1, 0, BOOT_I1, 0, --, H_L, -]
 */
	const uint8_t REG_CTRL6			= 0x25;
	const uint8_t REG_REFERENCE		= 0x26; // **< REFERENCE/DATACAPTURE
/*!
 *  STATUS_REG
 *  [ZYXOR, ZOR, YOR, XOR, ZYXDA, ZDA, YDA, XDA]
 *   ZYXOR    X, Y and Z axis data overrun. Default value: 0
 *            (0: no overrun has occurred; 1: a new set of data has overwritten
 * the previous ones) ZOR      Z axis data overrun. Default value: 0 (0: no
 * overrun has occurred; 1: a new data for the Z-axis has overwritten the
 * previous one) YOR      Y axis data overrun. Default value: 0 (0: no overrun
 * has occurred;  1: a new data for the Y-axis has overwritten the previous one)
 *   XOR      X axis data overrun. Default value: 0
 *            (0: no overrun has occurred; 1: a new data for the X-axis has
 * overwritten the previous one) ZYXDA    X, Y and Z axis new data available.
 * Default value: 0 (0: a new set of data is not yet available; 1: a new set of
 * data is available) ZDA      Z axis new data available. Default value: 0 (0: a
 * new data for the Z-axis is not yet available; 1: a new data for the Z-axis is
 * available) YDA      Y axis new data available. Default value: 0 (0: a new
 * data for the Y-axis is not yet available; 1: a new data for the Y-axis is
 * available)
 */
	const uint8_t REG_STATUS2		= 0x27;
	const uint8_t REG_OUT_X_L		= 0x28; // **< X-axis acceleration data. Low value
	const uint8_t REG_OUT_X_H		= 0x29; // **< X-axis acceleration data. High value
	const uint8_t REG_OUT_Y_L		= 0x2A; // **< Y-axis acceleration data. Low value
	const uint8_t REG_OUT_Y_H		= 0x2B; // **< Y-axis acceleration data. High value
	const uint8_t REG_OUT_Z_L		= 0x2C; // **< Z-axis acceleration data. Low value
	const uint8_t REG_OUT_Z_H		= 0x2D; // **< Z-axis acceleration data. High value
/*!
 *  FIFO_CTRL_REG
 *  [FM1, FM0, TR, FTH4, FTH3, FTH2, FTH1, FTH0]
 *   FM1-FM0  FIFO mode selection. Default value: 00 (see Table 44)
 *   TR       Trigger selection. Default value: 0
 *            0: Trigger event liked to trigger signal on INT1
 *            1: Trigger event liked to trigger signal on INT2
 *   FTH4:0   Default value: 0
 */
	const uint8_t REG_FIFOCTRL		= 0x2E;
	const uint8_t REG_FIFOSRC		= 0x2F; // **< FIFO_SRC_REG [WTM, OVRN_FIFO, EMPTY, FSS4, FSS3, FSS2, FSS1, FSS0]
/*!
 *  INT1_CFG
 *  [AOI, 6D, ZHIE/ZUPE, ZLIE/ZDOWNE, YHIE/YUPE, XHIE/XUPE, XLIE/XDOWNE]
 *   AOI         And/Or combination of Interrupt events. Default value: 0. Refer
 * to Datasheet Table 48, "Interrupt mode" 6D          6 direction detection
 * function enabled. Default value: 0. Refer to Datasheet Table 48, "Interrupt
 * mode" ZHIE/ZUPE   Enable interrupt generation on Z high event or on Direction
 * recognition. Default value: 0. (0: disable interrupt request; 1: enable
 * interrupt request) ZLIE/ZDOWNE Enable interrupt generation on Z low event or
 * on Direction recognition. Default value: 0. YHIE/YUPE   Enable interrupt
 * generation on Y high event or on Direction recognition. Default value: 0. (0:
 * disable interrupt request; 1: enable interrupt request.) YLIE/YDOWNE Enable
 * interrupt generation on Y low event or on Direction recognition. Default
 * value: 0. (0: disable interrupt request; 1: enable interrupt request.)
 *   XHIE/XUPE   Enable interrupt generation on X high event or on Direction
 * recognition. Default value: 0. (0: disable interrupt request; 1: enable
 * interrupt request.) XLIE/XDOWNE Enable interrupt generation on X low event or
 * on Direction recognition. Default value: 0. (0: disable interrupt request; 1:
 * enable interrupt request.)
 */
	const uint8_t REG_INT1CFG		= 0x30;
/*!
 *  INT1_SRC
 *   [0, IA, ZH, ZL, YH, YL, XH, XL]
 *    IA  Interrupt active. Default value: 0
 *        (0: no interrupt has been generated; 1: one or more interrupts have
 * been generated) ZH  Z high. Default value: 0 (0: no interrupt, 1: Z High
 * event has occurred) ZL  Z low. Default value: 0 (0: no interrupt; 1: Z Low
 * event has occurred) YH  Y high. Default value: 0 (0: no interrupt, 1: Y High
 * event has occurred) YL  Y low. Default value: 0 (0: no interrupt, 1: Y Low
 * event has occurred) XH  X high. Default value: 0 (0: no interrupt, 1: X High
 * event has occurred) XL  X low. Default value: 0 (0: no interrupt, 1: X Low
 * event has occurred)
 *
 *    Interrupt 1 source register. Read only register.
 *    Reading at this address clears INT1_SRC IA bit (and the interrupt signal
 * on INT 1 pin) and allows the refreshment of data in the INT1_SRC register if
 * the latched option  was chosen.
 */
	const uint8_t REG_INT1SRC		= 0x31;
	const uint8_t REG_INT1THS		= 0x32; // **< INT1_THS register [0, THS6, THS5, THS4, THS3, THS1, THS0]
	const uint8_t REG_INT1DUR		= 0x33; // **< INT1_DURATION [0, D6, D5, D4, D3, D2, D1, D0]
/*!
 *  CLICK_CFG
 *   [--, --, ZD, ZS, YD, YS, XD, XS]
 *   ZD  Enable interrupt double CLICK-CLICK on Z axis. Default value: 0
 *       (0: disable interrupt request;
 *        1: enable interrupt request on measured accel. value higher than
 * preset threshold) ZS  Enable interrupt single CLICK-CLICK on Z axis. Default
 * value: 0 (0: disable interrupt request; 1: enable interrupt request on
 * measured accel. value higher than preset threshold) YD  Enable interrupt
 * double CLICK-CLICK on Y axis. Default value: 0 (0: disable interrupt request;
 *        1: enable interrupt request on measured accel. value higher than
 * preset threshold) YS  Enable interrupt single CLICK-CLICK on Y axis. Default
 * value: 0 (0: disable interrupt request; 1: enable interrupt request on
 * measured accel. value higher than preset threshold) XD  Enable interrupt
 * double CLICK-CLICK on X axis. Default value: 0 (0: disable interrupt request;
 * 1: enable interrupt request on measured accel. value higher than preset
 * threshold) XS  Enable interrupt single CLICK-CLICK on X axis. Default value:
 * 0 (0: disable interrupt request; 1: enable interrupt request on measured
 * accel. value higher than preset threshold)
 */
	const uint8_t REG_CLICKCFG		= 0x38;
/*!
 *  CLICK_SRC
 *   [-, IA, DCLICK, SCLICK, Sign, Z, Y, X]
 *   IA  Interrupt active. Default value: 0
 *       (0: no interrupt has been generated; 1: one or more interrupts have
 * been generated) DCLICK  Double CLICK-CLICK enable. Default value: 0 (0:double
 * CLICK-CLICK detection disable, 1: double CLICK-CLICK detection enable) SCLICK
 * Single CLICK-CLICK enable. Default value: 0 (0:Single CLICK-CLICK detection
 * disable, 1: single CLICK-CLICK detection enable) Sign    CLICK-CLICK Sign.
 *           (0: positive detection, 1: negative detection)
 *   Z       Z CLICK-CLICK detection. Default value: 0
 *           (0: no interrupt, 1: Z High event has occurred)
 *   Y       Y CLICK-CLICK detection. Default value: 0
 *           (0: no interrupt, 1: Y High event has occurred)
 *   X       X CLICK-CLICK detection. Default value: 0
 *           (0: no interrupt, 1: X High event has occurred)
 */
	const uint8_t REG_CLICKSRC		= 0x39;
/*!
 *  CLICK_THS
 *   [-, Ths6, Ths5, Ths4, Ths3, Ths2, Ths1, Ths0]
 *   Ths6-Ths0  CLICK-CLICK threshold. Default value: 000 0000
 */
	const uint8_t REG_CLICKTHS		= 0x3A;
/*!
 *  TIME_LIMIT
 *   [-, TLI6, TLI5, TLI4, TLI3, TLI2, TLI1, TLI0]
 *   TLI7-TLI0  CLICK-CLICK Time Limit. Default value: 000 0000
 */
	const uint8_t REG_TIMELIMIT		= 0x3B;
/*!
 *  TIME_LATANCY
 *   [-, TLA6, TLIA5, TLA4, TLA3, TLA2, TLA1, TLA0]
 *   TLA7-TLA0  CLICK-CLICK Time Latency. Default value: 000 0000
 */
	const uint8_t REG_TIMELATENCY	= 0x3C;
/*!
 *  TIME_WINDOW
 *   [TW7, TW6, TW5, TW4, TW3, TW2, TW1, TW0]
 *   TW7-TW0  CLICK-CLICK Time window
 */
	const uint8_t REG_TIMEWINDOW	= 0x3D;
}
//---------------------------------------------------------------------------
namespace TArduinoAccelerometerLIS3DHDataRate
{
	enum TArduinoAccelerometerLIS3DHDataRate
	{
		Normal_1_Hz,
		Normal_10_Hz,
		Normal_25_Hz,
		Normal_50_Hz,
		Normal_100_Hz,
		Normal_200_Hz,
		Normal_400_Hz_LowPower_1_60_kHz,
		Normal_1_344_kHz_LowPower_5_376_kHz
	};
}
//---------------------------------------------------------------------------
	template <
		typename T_FullScaleRange,
		typename T_HighResolution,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z
	> class STMicroelectronicsLIS3DHAccelerometer :
		public T_FullScaleRange,
		public T_HighResolution,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z
	{
	public:
		_V_PIN_( FullScaleRange )
		_V_PIN_( HighResolution )
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InFahrenheit
	> class STMicroelectronicsLIS3DHThermometer :
		public T_Enabled,
		public T_InFahrenheit
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Latency,
		typename T_Limit,
		typename T_Window
	> class TArduinoAccelerometerLIS3DHTapTiming :
		public T_Latency,
		public T_Limit,
		public T_Window
	{
	public:
		_V_PROP_( Latency )
		_V_PROP_( Limit )
		_V_PROP_( Window )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DoubleTap,
		typename T_SingleTap,
		typename T_Threshold,
		typename T_Timing,
		typename T_OutputPins_X_Positive,
		typename T_OutputPins_X_Negative,
		typename T_OutputPins_Y_Positive,
		typename T_OutputPins_Y_Negative,
		typename T_OutputPins_Z_Positive,
		typename T_OutputPins_Z_Negative
	> class TArduinoAccelerometerLIS3DHTap :
		public T_DoubleTap,
		public T_SingleTap,
		public T_Threshold,
		public T_Timing,
		public T_OutputPins_X_Positive,
		public T_OutputPins_X_Negative,
		public T_OutputPins_Y_Positive,
		public T_OutputPins_Y_Negative,
		public T_OutputPins_Z_Positive,
		public T_OutputPins_Z_Negative
	{
	public:
		_V_PIN_( OutputPins_X_Positive )
		_V_PIN_( OutputPins_X_Negative )
		_V_PIN_( OutputPins_Y_Positive )
		_V_PIN_( OutputPins_Y_Negative )
		_V_PIN_( OutputPins_Z_Positive )
		_V_PIN_( OutputPins_Z_Negative )

	public:
		_V_PROP_( DoubleTap )
		_V_PROP_( SingleTap )
		_V_PROP_( Threshold )
		_V_PROP_( Timing )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2
//		typename T_Thermometer
	> class STMicroelectronicsLIS3DHADC :
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2
//		public T_Thermometer
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )

//	public:
//		_V_PROP_( Thermometer )

	public:
		volatile bool GetIsEnabled() { return T_OutputPins_0::GetPinIsConnected() || T_OutputPins_1::GetPinIsConnected() || T_OutputPins_2::GetPinIsConnected(); }

	};
//---------------------------------------------------------------------------
	template <
//		typename T_I2C, T_I2C &C_I2C,
		typename T_INTERFACE,
		typename T_Accelerometer,
//		typename T_Address,
		typename T_AnalogInputs,
		typename T_DataRate,
		typename T_EnablePullUp,
		typename T_Enabled,
		typename T_LowPower,
		typename T_PowerDown,
		typename T_Tap
	> class STMicroelectronicsLIS3DH :
		public T_INTERFACE,
		public T_Accelerometer,
//		public T_Address,
		public T_AnalogInputs,
		public T_DataRate,
		public T_EnablePullUp,
		public T_Enabled,
		public T_LowPower,
		public T_PowerDown,
		public T_Tap
	{
	public:
		_V_PROP_( Accelerometer )
//		_V_PROP_( Address )
		_V_PROP_( AnalogInputs )
		_V_PROP_( DataRate )
		_V_PROP_( EnablePullUp )
		_V_PROP_( Enabled )
		_V_PROP_( LowPower )
		_V_PROP_( PowerDown )
		_V_PROP_( Tap )

	protected:
		float ConvertADC( uint16_t AValue )
		{
			if( LowPower().GetValue() )
				return float( AValue ) / 0xFF;

			return float( AValue ) / float( 0b0000001111111111 );
		}

		float ConvertAcceleration( uint16_t AIntValue )
		{
//			Serial.println( AIntValue );

			if( LowPower().GetValue() )
				AIntValue <<= 8;  // 8 bit

			else if( Accelerometer().HighResolution() )
				AIntValue <<= 4;  // 12 bit

			else
				AIntValue <<= 6;  // 10 bit

			float AValue = float( int16_t( AIntValue ) ) / 0x7FFF;

			switch( Accelerometer().FullScaleRange().GetValue() )
			{
				case ar2g : AValue *= 2.0f; break;
				case ar4g : AValue *= 4.0f; break;
				case ar8g : AValue *= 8.0f; break;
				default : AValue *= 16.0f; break;
			}

			return AValue;
		}

		void ReadTapSensor( bool AChangeOnly )
		{
			if( ! ( Tap().SingleTap().GetValue() || Tap().DoubleTap().GetValue() ) )
				return;

			uint8_t AValue = T_INTERFACE::ReadRegister8( LIS3DH::REG_CLICKSRC );
//			if( AValue > 2 )
//				Serial.println( AValue, BIN );

			if( AValue & 0b1000 )
			{
				if( AValue & 0b001 )
					Tap().OutputPins_X_Positive().ClockPin();

				if( AValue & 0b010 )
					Tap().OutputPins_Y_Positive().ClockPin();

				if( AValue & 0b100 )
					Tap().OutputPins_Z_Positive().ClockPin();

			}

			else
			{
				if( AValue & 0b001 )
					Tap().OutputPins_X_Negative().ClockPin();

				if( AValue & 0b010 )
					Tap().OutputPins_Y_Negative().ClockPin();

				if( AValue & 0b100 )
					Tap().OutputPins_Z_Negative().ClockPin();

			}

		}

		void ReadSensors( bool AChangeOnly, bool AForceRead )
		{
			if( ! Enabled().GetValue() )
				return;

			if( AnalogInputs().OutputPins_0().GetPinIsConnected() && AnalogInputs().OutputPins_1().GetPinIsConnected() && AnalogInputs().OutputPins_2().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUTADC1_L, 6 );

				uint16_t AValue1 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue1 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue3 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue3 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				AnalogInputs().OutputPins_0().SetPinValue( ConvertADC( AValue1 ), AChangeOnly );
				AnalogInputs().OutputPins_1().SetPinValue( ConvertADC( AValue2 ), AChangeOnly );
				AnalogInputs().OutputPins_2().SetPinValue( ConvertADC( AValue3 ), AChangeOnly );
			}

			else if( AnalogInputs().OutputPins_0().GetPinIsConnected() && AnalogInputs().OutputPins_1().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUTADC1_L, 4 );

				uint16_t AValue1 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue1 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				AnalogInputs().OutputPins_0().SetPinValue( ConvertADC( AValue1 ), AChangeOnly );
				AnalogInputs().OutputPins_1().SetPinValue( ConvertADC( AValue2 ), AChangeOnly );
			}

			else if( AnalogInputs().OutputPins_1().GetPinIsConnected() && AnalogInputs().OutputPins_2().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUTADC2_L, 4 );

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue3 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue3 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				AnalogInputs().OutputPins_1().SetPinValue( ConvertADC( AValue2 ), AChangeOnly );
				AnalogInputs().OutputPins_2().SetPinValue( ConvertADC( AValue3 ), AChangeOnly );
			}

			else
			{
				if( AnalogInputs().OutputPins_0().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUTADC1_L, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

					T_INTERFACE::EndReadRegisters();

					AnalogInputs().OutputPins_0().SetPinValue( ConvertADC( AValue ), AChangeOnly );
				}

				if( AnalogInputs().OutputPins_1().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUTADC2_L, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

					T_INTERFACE::EndReadRegisters();

					AnalogInputs().OutputPins_1().SetPinValue( ConvertADC( AValue ), AChangeOnly );
				}

				if( AnalogInputs().OutputPins_2().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUTADC3_L, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

					T_INTERFACE::EndReadRegisters();

//					Serial.println( AValue );

					AnalogInputs().OutputPins_2().SetPinValue( ConvertADC( AValue ), AChangeOnly );
				}
			}

			if( ! AForceRead )
			{
				uint8_t AValue = T_INTERFACE::ReadRegister8( LIS3DH::REG_STATUS1 );
//				Serial.println( AValue, BIN );
				if( !( AValue & 0b00001000 ))
					return;
			}

			if( Accelerometer().OutputPins_X().GetPinIsConnected() && Accelerometer().OutputPins_Y().GetPinIsConnected() && Accelerometer().OutputPins_Z().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUT_X_L, 6 );

				uint16_t AValue1 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue1 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue3 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue3 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				Accelerometer().OutputPins_X().SetPinValue( ConvertAcceleration( AValue1 ), AChangeOnly );
				Accelerometer().OutputPins_Y().SetPinValue( ConvertAcceleration( AValue2 ), AChangeOnly );
				Accelerometer().OutputPins_Z().SetPinValue( ConvertAcceleration( AValue3 ), AChangeOnly );
			}

			else if( Accelerometer().OutputPins_X().GetPinIsConnected() && Accelerometer().OutputPins_Y().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUT_X_L, 4 );

				uint16_t AValue1 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue1 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				Accelerometer().OutputPins_X().SetPinValue( ConvertAcceleration( AValue1 ), AChangeOnly );
				Accelerometer().OutputPins_Y().SetPinValue( ConvertAcceleration( AValue2 ), AChangeOnly );
			}

			else if( Accelerometer().OutputPins_Y().GetPinIsConnected() && Accelerometer().OutputPins_Z().GetPinIsConnected() )
			{
				T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUT_Y_L, 4 );

				uint16_t AValue2 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue2 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				uint16_t AValue3 = T_INTERFACE::ReadUInt8(); // Low bits
				AValue3 |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

				T_INTERFACE::EndReadRegisters();

				Accelerometer().OutputPins_Y().SetPinValue( ConvertAcceleration( AValue2 ), AChangeOnly );
				Accelerometer().OutputPins_Z().SetPinValue( ConvertAcceleration( AValue3 ), AChangeOnly );
			}

			else
			{
				if( Accelerometer().OutputPins_X().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUT_X_L, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits
//					Serial.println( AValue );
//					return;

					T_INTERFACE::EndReadRegisters();

					Accelerometer().OutputPins_X().SetPinValue( ConvertAcceleration( AValue ), AChangeOnly );
				}

				if( Accelerometer().OutputPins_Y().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUT_Y_L, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

					T_INTERFACE::EndReadRegisters();

					Accelerometer().OutputPins_Y().SetPinValue( ConvertAcceleration( AValue ), AChangeOnly );
				}

				if( Accelerometer().OutputPins_Z().GetPinIsConnected() )
				{
					T_INTERFACE::StartReadRegisters( LIS3DH::REG_OUT_Z_L, 2 );
					uint16_t AValue = T_INTERFACE::ReadUInt8(); // Low bits
					AValue |= uint16_t( T_INTERFACE::ReadUInt8() ) << 8; // High bits

					T_INTERFACE::EndReadRegisters();

					Accelerometer().OutputPins_Z().SetPinValue( ConvertAcceleration( AValue ), AChangeOnly );
				}
			}

//			T_INTERFACE::ReadRegister8( LIS3DH::REG_CTRL4 );
		}

	public:
		inline void UpdateConfig0()
		{
			if( EnablePullUp().GetValue() )
				T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL0, 0 );

			else
				T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL0, 0b00010000 );
			
		}

		inline void UpdateTempConfig()
		{
			uint8_t AValue = 0;
			if( AnalogInputs().GetIsEnabled() )
			{
				AValue |= 0b10000000;
//				if( AnalogInputs().Thermometer().Enabled())
//					AValue |= 0b01000000;
			}

//			Serial.println( AValue, BIN );
//			delay( 10000 );
//			Serial.println( AValue, BIN );
//			delay( 10000 );
			T_INTERFACE::WriteRegister8( LIS3DH::REG_TEMPCFG, AValue );
		}

		inline void UpdateConfig1()
		{
			if( PowerDown() )
			{
				T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL1, 0 );
				return;
			}

			uint8_t AValue = 0;
			if( LowPower() )
				AValue = 0b00001000;

			if( Accelerometer().OutputPins_X().GetPinIsConnected() || Tap().OutputPins_X_Positive().GetPinIsConnected() || Tap().OutputPins_X_Negative().GetPinIsConnected() )
				AValue |= 0b00000001; 

			if( Accelerometer().OutputPins_Y().GetPinIsConnected() || Tap().OutputPins_Y_Positive().GetPinIsConnected() || Tap().OutputPins_Y_Negative().GetPinIsConnected() )
				AValue |= 0b00000010; 

			if( Accelerometer().OutputPins_Z().GetPinIsConnected() || Tap().OutputPins_Z_Positive().GetPinIsConnected() || Tap().OutputPins_Z_Negative().GetPinIsConnected() )
				AValue |= 0b00000100; 

//				AValue |= 0b00000111; 
			switch( DataRate().GetValue() )
			{
				case TArduinoAccelerometerLIS3DHDataRate::Normal_1_Hz							: AValue |= 0b00010000; break;
				case TArduinoAccelerometerLIS3DHDataRate::Normal_10_Hz							: AValue |= 0b00100000; break;
				case TArduinoAccelerometerLIS3DHDataRate::Normal_25_Hz							: AValue |= 0b00110000; break;
				case TArduinoAccelerometerLIS3DHDataRate::Normal_50_Hz							: AValue |= 0b01000000; break;
				case TArduinoAccelerometerLIS3DHDataRate::Normal_100_Hz							: AValue |= 0b01010000; break;
				case TArduinoAccelerometerLIS3DHDataRate::Normal_200_Hz							: AValue |= 0b01100000; break;
				case TArduinoAccelerometerLIS3DHDataRate::Normal_400_Hz_LowPower_1_60_kHz		:
					if( LowPower().GetValue() )
						AValue |= 0b10000000;

					else
						AValue |= 0b01110000;

					break;

				case TArduinoAccelerometerLIS3DHDataRate::Normal_1_344_kHz_LowPower_5_376_kHz	: AValue |= 0b10010000; break;
			}

			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL1, AValue );
		}

		inline void UpdateConfig4()
		{
			uint8_t AValue = 0;
			if( Accelerometer().HighResolution() )
				AValue |= 0b00001000;

			switch( Accelerometer().FullScaleRange().GetValue() )
			{
//				case ar2g	:
				case ar4g	: AValue |= 0b00010000; break;
				case ar8g	: AValue |= 0b00100000; break;
				case ar16g	: AValue |= 0b00110000; break;
			}

			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL4, AValue );
		}

		inline void UpdateThresholdConfig()
		{
			uint8_t AValue;
			float AFloatValue = Tap().Threshold().GetValue();
/*
			switch( Accelerometer().FullScaleRange().GetValue() )
			{
				case ar4g	: AFloatValue /= 4.0f; break;
				case ar8g	: AFloatValue /= 8.0f; break;
				case ar16g	: AFloatValue /= 16.0f; break;
				default     : AFloatValue /= 2.0f; break;
			}
*/
//			if( LowPower().GetValue() )
				AValue = ( AFloatValue * 0x3F ) + 0.5f;  // 8 bit

//			else if( Accelerometer().HighResolution() )
//				AValue = ( AFloatValue * 0x0000011111111111 ) + 0.5f;  // 12 bit

//			else
//				AValue = ( AFloatValue * 0x0000000111111111 ) + 0.5f;  // 10 bit

//			delay( 5000 );
//			Serial.println( AFloatValue );
//			Serial.println( AValue );
//			delay( 10000 );

			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKTHS, AValue | 0x80 );
		}

		inline void UpdateTimeLimit()
		{
			T_INTERFACE::WriteRegister8( LIS3DH::REG_TIMELIMIT, Tap().Timing().Limit().GetValue() );
		}

		inline void UpdateTimeLatency()
		{
			T_INTERFACE::WriteRegister8( LIS3DH::REG_TIMELATENCY, Tap().Timing().Latency().GetValue() );
		}

		inline void UpdateTimeWindow()
		{
			T_INTERFACE::WriteRegister8( LIS3DH::REG_TIMEWINDOW, Tap().Timing().Window().GetValue() );
		}

		inline void UpdateClickConfig()
		{

//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKCFG, 0x2A );
//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKCFG, 0b00111111 );
//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKCFG, 0x15 );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false, true );
		}

	public:
		inline void SystemInit()
		{
///*
			UpdateConfig0();
			UpdateConfig1();
			UpdateConfig4();

			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL2, 0 );
			T_INTERFACE::WriteRegister8( LIS3DH::REG_FIFOCTRL, 0 );
			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL3, 0x10 );

			UpdateTempConfig();
//*/


//  // enable all axes, normal mode
//  T_INTERFACE::WriteRegister8(LIS3DH::REG_CTRL1, 0x07);
  // 400Hz rate
//  setDataRate(LIS3DH_DATARATE_400_HZ);
//  T_INTERFACE::WriteRegister8(LIS3DH::REG_CTRL1, 0x37);

//  // High res & BDU enabled
//  T_INTERFACE::WriteRegister8(LIS3DH::REG_CTRL4, 0x88);

//  // DRDY on INT1
//  T_INTERFACE::WriteRegister8(LIS3DH::REG_CTRL3, 0x10);

  // Turn on orientation config
  // T_INTERFACE::WriteRegister8(LIS3DH::REG_PL_CFG, 0x40);

  // enable adcs
//  T_INTERFACE::WriteRegister8(LIS3DH::REG_TEMPCFG, 0x80);

/*
			// enable all axes, normal mode
			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL1, 0x07 );
			// 400Hz rate
//			setDataRate(LIS3DH_DATARATE_400_HZ);

			// High res & BDU enabled
			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL4, 0x88 );

			// DRDY on INT1
			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL3, 0x10 );

			// Turn on orientation config
			// writeRegister8(LIS3DH_REG_PL_CFG, 0x40);

			// enable adcs
			T_INTERFACE::WriteRegister8( LIS3DH::REG_TEMPCFG, 0x80 );
*/
		}

		inline void SystemLoopStart()
		{
		}

		inline void SystemStart()
		{
			ReadSensors( false, false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true, false );
		}

		inline void SystemStartTap()
		{
			UpdateThresholdConfig();
			UpdateTimeLimit();
			UpdateTimeLatency();
			UpdateTimeWindow();
			UpdateClickConfig();

//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL3, 0x90 );
  //writeRegister8(LIS3DH_REG_CTRL3, 0x80); // turn on int1 click
//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CTRL5, 0x08 ); // latch interrupt on int1

//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKCFG, 0x2A );
//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKCFG, 0b00111111 );
////			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKCFG, 0x15 );
//			T_INTERFACE::WriteRegister8( LIS3DH::REG_CLICKSRC, 0b00110000 );

			ReadTapSensor( false );
		}

		inline void SystemLoopBeginTap()
		{
			ReadTapSensor( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

