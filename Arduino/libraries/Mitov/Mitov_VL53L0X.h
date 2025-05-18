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
#include "Wire.h"
//#include "MitovEmbedded_vl53l0x/Mitov_vl53l0x_api.h"
//#include "MitovEmbedded_vl53l0x/Mitov_vl53l0x_platform.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

/*
  static const uint8_t SETUP_SIGNAL_LIMIT         (0.1*65536)
  static const uint8_t SETUP_SIGMA_LIMIT          (60*65536)
  static const uint8_t SETUP_MAX_TIME_FOR_RANGING     33000
  static const uint8_t SETUP_PRE_RANGE_VCSEL_PERIOD   18
  static const uint8_t SETUP_FINAL_RANGE_VCSEL_PERIOD 14
*/

namespace Mitov
{
namespace CallChains
{
	class T_VL53L0X_Apply_NullChain
	{
	public:
		static void Call() {}

	};
}
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address
	> class LaserRangerSetAddress :
		public T_Address
	{
	public:
		_V_PROP_( Address )

	public:
		inline void Apply()
		{
			C_OWNER.SetDeviceAddress( uint8_t( Address().GetValue() ));
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SetDeviceAddress( uint8_t( Address().GetValue() ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FinalRange,
		typename T_PreRange
	> class TArduinoLaserRangerVL53L0XPulsePeriods :
		public T_FinalRange,
		public T_PreRange
	{
	public:
		_V_PROP_( FinalRange )
		_V_PROP_( PreRange )
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		const uint8_t *C_InterruptSettingBuffer,
		uint32_t C_InterruptSettingBuffer_Size,
		const uint8_t *C_TuningSettingBuffer,
		uint32_t C_TuningSettingBuffer_Size,
		typename T_DitanceOutputPin,
		typename T_Elements_Apply,
//		typename T_HighPrecision,
		typename T_Enabled,
		typename T_PulsePeriods,
		typename T_ReturnSignalRateLimit,
		typename T_SamplingPeriod,
		typename T_Timeout,
//		typename T_SigmaOutputPin,
		typename T_Use2_8V_I2C
//		typename T_RangeIgnoreThreshold,
//		typename T_SigmaLimit
	> class LaserRangerVL53L0X :
		public T_Address,
		public T_DitanceOutputPin,
		public T_Elements_Apply,
//		public T_HighPrecision,
		public T_Enabled,
		public T_PulsePeriods,
		public T_ReturnSignalRateLimit,
		public T_SamplingPeriod,
		public T_Timeout,
//		public T_SigmaOutputPin,
		public T_Use2_8V_I2C
//		public T_RangeIgnoreThreshold,
//		public T_SigmaLimit
	{
	public:
		_V_PIN_( DitanceOutputPin )
//		_V_PIN_( SigmaOutputPin )

	public:
		_V_PROP_( Address )
//		_V_PROP_( HighPrecision )
		_V_PROP_( Use2_8V_I2C )
		_V_PROP_( Enabled )
		_V_PROP_( PulsePeriods )
		_V_PROP_( ReturnSignalRateLimit )
		_V_PROP_( SamplingPeriod )
		_V_PROP_( Timeout )
//		_V_PROP_( RangeIgnoreThreshold )
//		_V_PROP_( SigmaLimit )

	protected:
		typedef uint32_t FixPoint1616_t;

/*
		struct SequenceStepEnables
		{
			bool tcc;
			bool msrc; 
			bool dss;
			bool pre_range;
			bool final_range;
		};
*/
		struct SequenceStepTimeouts
		{
			uint16_t pre_range_vcsel_period_pclks, final_range_vcsel_period_pclks;

			uint16_t msrc_dss_tcc_mclks, pre_range_mclks, final_range_mclks;
			uint32_t msrc_dss_tcc_us,    pre_range_us,    final_range_us;
		};
/*
	protected:
		static const uint8_t REF_SPAD_BUFFER_SIZE = 6;
*/
	protected:
		void Write8( uint8_t AIndex, uint8_t AData )
		{
#ifdef VL53L0X_I2C_DEBUG
			Serial.print( "\tWriting 1 to addr 0x"); Serial.print( AIndex, HEX ); Serial.print(": 0x" ); Serial.println( AData, HEX );
#endif
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
			C_I2C.write( AData );
			C_I2C.endTransmission();
		}

		uint8_t ReadByte( uint8_t AIndex )
		{
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( T_Address::GetValue() ), uint8_t( 1 ));
#ifdef VL53L0X_I2C_DEBUG
			uint8_t tmp = C_I2C.read();
			Serial.print("\tReading 1 from addr 0x"); Serial.print( AIndex, HEX); Serial.print(": 0x"); Serial.println( tmp, HEX );
			return tmp;
#else
			return C_I2C.read();
#endif
		}

		void Update8( uint8_t AIndex, uint8_t AAndData, uint8_t AOrData )
		{
			uint8_t AData = ReadByte( AIndex );
			AData = ( AData & AAndData) | AOrData;
			Write8( AIndex, AData );
		}

		uint16_t Read16( uint8_t AIndex )
		{
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( T_Address::GetValue() ), uint8_t( 2 ));

			uint16_t tmp;
			tmp = C_I2C.read();
			tmp <<= 8;
			tmp |= C_I2C.read();

#ifdef VL53L0X_I2C_DEBUG
			Serial.print("\tReading 2 from addr 0x"); Serial.print( AIndex, HEX); Serial.print(": 0x"); Serial.println( tmp, HEX );
#endif
			return tmp;
		}

		void Write16( uint8_t AIndex, uint16_t AData )
		{
#ifdef VL53L0X_I2C_DEBUG
			Serial.print( "\tWriting 2 to addr 0x"); Serial.print( AIndex, HEX ); Serial.print(": 0x"); Serial.println( AData, HEX );
#endif
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
			C_I2C.write( uint8_t( AData >> 8 ));
			C_I2C.write( uint8_t( AData ));
			C_I2C.endTransmission();
		}

		uint32_t Read32( uint8_t AIndex )
		{
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( T_Address::GetValue() ), uint8_t( 4 ));

			uint32_t tmp = C_I2C.read();
			tmp <<= 8;
			tmp |= C_I2C.read();
			tmp <<= 8;
			tmp |= C_I2C.read();
			tmp <<= 8;
			tmp |= C_I2C.read();

#ifdef VL53L0X_I2C_DEBUG
			Serial.print("\tReading 4 from addr 0x"); Serial.print( AIndex, HEX); Serial.print(": 0x"); Serial.println( tmp, HEX );
#endif
			return tmp;
		}

		void Write32( uint8_t AIndex, uint32_t AData )
		{
#ifdef VL53L0X_I2C_DEBUG
			Serial.print( "\tWriting 4 to addr 0x"); Serial.print( AIndex, HEX ); Serial.print(": 0x"); Serial.println( AData, HEX );
#endif
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
			C_I2C.write( uint8_t( AData >> 24 ));
			C_I2C.write( uint8_t( AData >> 16 ));
			C_I2C.write( uint8_t( AData >> 8 ));
			C_I2C.write( uint8_t( AData ));
			C_I2C.endTransmission();
		}

		void ReadMulti( uint8_t AIndex, uint8_t *pdata, uint32_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( T_Address::GetValue()), uint8_t( ACount ));
#ifdef VL53L0X_I2C_DEBUG
			Serial.print("\tReading "); Serial.print( ACount ); Serial.print(" from addr 0x"); Serial.print( AIndex, HEX); Serial.print(": ");
#endif

			while( ACount -- ) 
			{
				*pdata = C_I2C.read();
#ifdef VL53L0X_I2C_DEBUG
				Serial.print("0x"); Serial.print(pdata[0], HEX); Serial.print(", ");
#endif
				pdata++;
			}

#ifdef VL53L0X_I2C_DEBUG
			Serial.println();
#endif
		}

		void WriteMulti( uint8_t AIndex, uint8_t *pdata, uint32_t ACount )
		{
			C_I2C.beginTransmission( uint8_t( T_Address::GetValue() ));
			C_I2C.write( AIndex );
#ifdef VL53L0X_I2C_DEBUG
			Serial.print("\tWriting "); Serial.print( ACount ); Serial.print(" to addr 0x"); Serial.print( AIndex, HEX ); Serial.print(": ");
#endif
			while( ACount -- ) 
			{
				C_I2C.write((uint8_t)pdata[0]);
#ifdef VL53L0X_I2C_DEBUG
				Serial.print("0x"); Serial.print(pdata[0], HEX); Serial.print(", ");
#endif
				pdata++;
			}
#ifdef VL53L0X_I2C_DEBUG
			Serial.println();
#endif
			C_I2C.endTransmission();
		}

	protected:
//  enables->tcc          = (sequence_config >> 4) & 0x1;
//  enables->dss          = (sequence_config >> 3) & 0x1;
//  enables->msrc         = (sequence_config >> 2) & 0x1;
//  enables->pre_range    = (sequence_config >> 6) & 0x1;
//  enables->final_range  = (sequence_config >> 7) & 0x1;

		static const uint8_t TCC_ENABLED_FLAG			= 1 << 4;
		static const uint8_t DSS_ENABLED_FLAG			= 1 << 3;
		static const uint8_t MSRC_ENABLED_FLAG			= 1 << 2;
		static const uint8_t PRE_RANGE_ENABLED_FLAG		= 1 << 6;
		static const uint8_t FINAL_RANGE_ENABLED_FLAG	= 1 << 7;


		static const uint8_t REG_SYSRANGE_START									= 0x00;

		// ** mask existing bit in #VL53L0X_REG_SYSRANGE_START
		static const uint8_t REG_SYSRANGE_MODE_MASK								= 0x0F;
		// ** bit 0 in #VL53L0X_REG_SYSRANGE_START write 1 toggle state in
		// * continuous mode and arm next shot in single shot mode
		static const uint8_t REG_SYSRANGE_MODE_START_STOP						= 0x01;
		// ** bit 1 write 0 in #VL53L0X_REG_SYSRANGE_START set single shot mode
		static const uint8_t REG_SYSRANGE_MODE_SINGLESHOT						= 0x00;
		// ** bit 1 write 1 in #VL53L0X_REG_SYSRANGE_START set back-to-back
		// *  operation mode */
		static const uint8_t REG_SYSRANGE_MODE_BACKTOBACK						= 0x02;
		// ** bit 2 write 1 in #VL53L0X_REG_SYSRANGE_START set timed operation
		// *  mode */
		static const uint8_t REG_SYSRANGE_MODE_TIMED							= 0x04;
		// ** bit 3 write 1 in #VL53L0X_REG_SYSRANGE_START set histogram operation
		// *  mode
		static const uint8_t REG_SYSRANGE_MODE_HISTOGRAM						= 0x08;

//		static const uint8_t REG_SYSTEM_SEQUENCE_CONFIG							= 0x01;

//		static const uint8_t REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV				= 0x89;

		// Check Limit registers
		static const uint8_t REG_MSRC_CONFIG_CONTROL							= 0x60;

		static const uint8_t REG_PRE_RANGE_CONFIG_MIN_SNR						= 0x27;
		static const uint8_t REG_PRE_RANGE_CONFIG_VALID_PHASE_LOW				= 0x56;
		static const uint8_t REG_PRE_RANGE_CONFIG_VALID_PHASE_HIGH				= 0x57;
		static const uint8_t REG_PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT				= 0x64;

		static const uint8_t REG_FINAL_RANGE_CONFIG_MIN_SNR						= 0x67;
		static const uint8_t REG_FINAL_RANGE_CONFIG_VALID_PHASE_LOW				= 0x47;
		static const uint8_t REG_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH			= 0x48;
		static const uint8_t REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT	= 0x44;

		static const uint8_t REG_GLOBAL_CONFIG_VCSEL_WIDTH						= 0x32;
		static const uint8_t REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0   			= 0xB0;
		static const uint8_t REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_1   			= 0xB1;
		static const uint8_t REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_2   			= 0xB2;
		static const uint8_t REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_3   			= 0xB3;
		static const uint8_t REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_4   			= 0xB4;
		static const uint8_t REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_5   			= 0xB5;

		static const uint8_t REG_GLOBAL_CONFIG_REF_EN_START_SELECT   			= 0xB6;
		static const uint8_t REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD 			= 0x4E; // 0x14E
		static const uint8_t REG_DYNAMIC_SPAD_REF_EN_START_OFFSET    			= 0x4F; // 0x14F
		static const uint8_t REG_POWER_MANAGEMENT_GO1_POWER_FORCE    			= 0x80;


		static const uint8_t REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD				= 0x70;
		static const uint8_t REG_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI			= 0x71;
		static const uint8_t REG_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO			= 0x72;
		static const uint8_t REG_CROSSTALK_COMPENSATION_PEAK_RATE_MCPS			= 0x20;

		static const uint8_t REG_MSRC_CONFIG_TIMEOUT_MACROP						= 0x46;

		static const uint8_t REG_I2C_SLAVE_DEVICE_ADDRESS						= 0x8A;

		/* PRE RANGE registers */
		static const uint8_t REG_PRE_RANGE_CONFIG_VCSEL_PERIOD					= 0x50;
		static const uint8_t REG_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI				= 0x51;
		static const uint8_t REG_PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO				= 0x52;


		static const uint8_t REG_SYSTEM_THRESH_HIGH								= 0x0C;
		static const uint8_t REG_SYSTEM_THRESH_LOW								= 0x0E;


		static const uint8_t REG_SYSTEM_SEQUENCE_CONFIG							= 0x01;
		static const uint8_t REG_SYSTEM_RANGE_CONFIG							= 0x09;
		static const uint8_t REG_SYSTEM_INTERMEASUREMENT_PERIOD					= 0x04;

		static const uint8_t REG_SYSTEM_INTERRUPT_CONFIG_GPIO					= 0x0A;

		static const uint8_t REG_SYSTEM_INTERRUPT_GPIO_DISABLED					= 0x00;
		static const uint8_t REG_SYSTEM_INTERRUPT_GPIO_LEVEL_LOW				= 0x01;
		static const uint8_t REG_SYSTEM_INTERRUPT_GPIO_LEVEL_HIGH				= 0x02;
		static const uint8_t REG_SYSTEM_INTERRUPT_GPIO_OUT_OF_WINDOW			= 0x03;
		static const uint8_t REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY			= 0x04;

		static const uint8_t REG_GPIO_HV_MUX_ACTIVE_HIGH						= 0x84;

		static const uint8_t REG_SYSTEM_INTERRUPT_CLEAR							= 0x0B;


		/* Result registers */
		static const uint8_t REG_RESULT_INTERRUPT_STATUS						= 0x13;
		static const uint8_t REG_RESULT_RANGE_STATUS							= 0x14;

		static const uint8_t REG_RESULT_CORE_PAGE								= 1;
		static const uint8_t REG_RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN  		= 0xBC;
		static const uint8_t REG_RESULT_CORE_RANGING_TOTAL_EVENTS_RTN   		= 0xC0;
		static const uint8_t REG_RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF  		= 0xD0;
		static const uint8_t REG_RESULT_CORE_RANGING_TOTAL_EVENTS_REF   		= 0xD4;
		static const uint8_t REG_RESULT_PEAK_SIGNAL_RATE_REF             		= 0xB6;

		static const uint8_t REG_OSC_CALIBRATE_VAL								= 0xF8;

		// Speed of light in um per 1E-10 Seconds

		static const uint16_t SPEED_OF_LIGHT_IN_AIR								= 2997;

		static const uint8_t REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV				= 0x89;

		static const uint8_t REG_ALGO_PHASECAL_LIM								= 0x30; // 0x130
		static const uint8_t REG_ALGO_PHASECAL_CONFIG_TIMEOUT					= 0x30;

	protected:
		uint8_t FStopVariable; // read by init and used when starting measurement; is StopVariable field of VL53L0X_DevData_t structure in API
		uint32_t measurement_timing_budget_us;

//	protected:
//		VL53L0X_Dev_t<T_I2C, C_I2C, T_Address, T_HighPrecision, C_InterruptSettingBuffer, C_InterruptSettingBuffer_Size, C_TuningSettingBuffer, C_TuningSettingBuffer_Size, T_RangeIgnoreThreshold, T_SigmaLimit>	FDevice;

	protected:
		inline static FixPoint1616_t ConvertTo1616( float AValue )
		{
			return FixPoint1616_t( AValue * 0xFFFF + 0.5 );
		}

		inline static uint16_t ConvertTo97( float AValue )
		{
			return uint16_t( AValue * (1 << 7));
			//return uint16_t( ConvertTo1616( AValue ) >>9 );
//			return FixPoint1616_t( ( AValue * 0xFFFF + 0.5 ) >> );
		}

/*
		static void PollingDelay() // usually best implemented as a real function
		{
//	static const uint8_t VL53L0X_POLLINGDELAY_LOOPNB  = 250;
			for( volatile uint32_t i = 0; i < 250; i++ )
			{
				//Do nothing
				asm("nop");
			}
		}
*/
		void load_registers( const uint8_t *pTuningSettingBuffer, uint32_t ALength )
		{
			for( uint32_t i = 0; i < ALength; ++i )
			{
				uint8_t ARegister = pgm_read_byte( pTuningSettingBuffer ++ );
				uint8_t AData = pgm_read_byte( pTuningSettingBuffer ++ );
				Write8( ARegister, AData );
			}
		}

		// Decode VCSEL (vertical cavity surface emitting laser) pulse period in PCLKs
		// from register value
		// based on VL53L0X_decode_vcsel_period()
		inline uint8_t decodeVcselPeriod( uint8_t reg_val )
		{
			return (((reg_val) + 1) << 1);
		}

		inline bool checkTimeoutExpired( unsigned long timeout_start_ms )
		{
//			Serial.println( Timeout().GetValue() );
			return ( Timeout().GetValue() > 0 && ( millis() - timeout_start_ms ) > Timeout().GetValue() );
		}

		// Decode sequence step timeout in MCLKs from register value
		// based on VL53L0X_decode_timeout()
		// Note: the original function returned a uint32_t, but the return value is
		// always stored in a uint16_t.
		inline uint16_t decodeTimeout(uint16_t reg_val)
		{
			// format: "(LSByte * 2^MSByte) + 1"
			return (uint16_t)((reg_val & 0x00FF) <<
					(uint16_t)((reg_val & 0xFF00) >> 8)) + 1;
		}

		// Calculate macro period in *nanoseconds* from VCSEL period in PCLKs
		// based on VL53L0X_calc_macro_period_ps()
		// PLL_period_ps = 1655; macro_period_vclks = 2304
		inline uint32_t calcMacroPeriod( uint8_t vcsel_period_pclks )
		{
			return ((( uint32_t( 2304 ) * vcsel_period_pclks * 1655) + 500) / 1000);
		}

		// Convert sequence step timeout from MCLKs to microseconds with given VCSEL period in PCLKs
		// based on VL53L0X_calc_timeout_us()
		inline uint32_t timeoutMclksToMicroseconds(uint16_t timeout_period_mclks, uint8_t vcsel_period_pclks)
		{
			uint32_t macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

			return ((timeout_period_mclks * macro_period_ns) + (macro_period_ns / 2)) / 1000;
		}

		// Convert sequence step timeout from microseconds to MCLKs with given VCSEL period in PCLKs
		// based on VL53L0X_calc_timeout_mclks()
		inline uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_period_us, uint8_t vcsel_period_pclks)
		{
			uint32_t macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

			return (((timeout_period_us * 1000) + (macro_period_ns / 2)) / macro_period_ns);
		}

		// based on VL53L0X_perform_single_ref_calibration()
		bool performSingleRefCalibration( uint8_t vhv_init_byte )
		{
			Write8( REG_SYSRANGE_START, 0x01 | vhv_init_byte ); // VL53L0X_REG_SYSRANGE_MODE_START_STOP

			unsigned long timeout_start_ms = millis();

			while(( ReadByte( REG_RESULT_INTERRUPT_STATUS ) & 0x07 ) == 0 )
//			{
//				Serial.println( "TEST" );
				if( checkTimeoutExpired( timeout_start_ms ))
					return false;
//			}
	
			Write8( REG_SYSTEM_INTERRUPT_CLEAR, 0x01 );
			Write8(REG_SYSRANGE_START, 0x00);

			return true;
		}

		// Get reference SPAD (single photon avalanche diode) count and type
		// based on VL53L0X_get_info_from_device(),
		// but only gets reference SPAD count and type
		void getSpadInfo( uint8_t &count, bool &type_is_aperture )
		{
			uint8_t tmp;

			Write8(0x80, 0x01);
			Write8(0xFF, 0x01);
			Write8(0x00, 0x00);

			Write8(0xFF, 0x06);
			Write8(0x83, ReadByte( 0x83 ) | 0x04);
			Write8(0xFF, 0x07);
			Write8(0x81, 0x01);

			Write8(0x80, 0x01);

			Write8(0x94, 0x6b);
			Write8(0x83, 0x00);

//			startTimeout();

			while( ReadByte( 0x83 ) == 0x00 )
				;
//				if (checkTimeoutExpired()) { return false; }

			Write8(0x83, 0x01);
			tmp = ReadByte( 0x92 );

			count = tmp & 0x7f;
			type_is_aperture = ( tmp >> 7 ) & 0x01;

			Write8(0x81, 0x00);
			Write8(0xFF, 0x06);
			Write8(0x83, ReadByte( 0x83 )  & ~0x04);
			Write8(0xFF, 0x01);
			Write8(0x00, 0x01);

			Write8(0xFF, 0x00);
			Write8(0x80, 0x00);

//			return true;
		}

		// Get the measurement timing budget in microseconds
		// based on VL53L0X_get_measurement_timing_budget_micro_seconds()
		// in us
		uint32_t getMeasurementTimingBudget(void)
		{
//			SequenceStepEnables enables;
			SequenceStepTimeouts timeouts;

			uint16_t const StartOverhead     = 1910; // note that this is different than the value in set_
			uint16_t const EndOverhead        = 960;
			uint16_t const MsrcOverhead       = 660;
			uint16_t const TccOverhead        = 590;
			uint16_t const DssOverhead        = 690;
			uint16_t const PreRangeOverhead   = 660;
			uint16_t const FinalRangeOverhead = 550;

			// "Start and end overhead times always present"
			uint32_t budget_us = StartOverhead + EndOverhead;

/*
//			getSequenceStepEnables( &enables );
//  enables->tcc          = (sequence_config >> 4) & 0x1;
//  enables->dss          = (sequence_config >> 3) & 0x1;
//  enables->msrc         = (sequence_config >> 2) & 0x1;
//  enables->pre_range    = (sequence_config >> 6) & 0x1;
//  enables->final_range  = (sequence_config >> 7) & 0x1;
*/
		    uint8_t sequence_config = ReadByte( REG_SYSTEM_SEQUENCE_CONFIG );
			getSequenceStepTimeouts( sequence_config, timeouts );

//			if (enables.tcc)
			if( sequence_config & TCC_ENABLED_FLAG )
				budget_us += (timeouts.msrc_dss_tcc_us + TccOverhead);

//			if (enables.dss)
			if ( sequence_config & DSS_ENABLED_FLAG )
				budget_us += 2 * (timeouts.msrc_dss_tcc_us + DssOverhead);
	
//			else if (enables.msrc)
			else if ( sequence_config & MSRC_ENABLED_FLAG )
				budget_us += (timeouts.msrc_dss_tcc_us + MsrcOverhead);

//			if (enables.pre_range)
			if ( sequence_config & PRE_RANGE_ENABLED_FLAG )
				budget_us += (timeouts.pre_range_us + PreRangeOverhead);

//			if (enables.final_range)
			if ( sequence_config & FINAL_RANGE_ENABLED_FLAG )
				budget_us += (timeouts.final_range_us + FinalRangeOverhead);

			measurement_timing_budget_us = budget_us; // store for internal reuse
			return budget_us;
		}

		// Set the measurement timing budget in microseconds, which is the time allowed
		// for one measurement; the ST API and this library take care of splitting the
		// timing budget among the sub-steps in the ranging sequence. A longer timing
		// budget allows for more accurate measurements. Increasing the budget by a
		// factor of N decreases the range measurement standard deviation by a factor of
		// sqrt(N). Defaults to about 33 milliseconds; the minimum is 20 ms.
		// based on VL53L0X_set_measurement_timing_budget_micro_seconds()
		bool setMeasurementTimingBudget( uint32_t budget_us )
		{
//			SequenceStepEnables enables;
			SequenceStepTimeouts timeouts;

			uint16_t const StartOverhead      = 1320; // note that this is different than the value in get_
			uint16_t const EndOverhead        = 960;
			uint16_t const MsrcOverhead       = 660;
			uint16_t const TccOverhead        = 590;
			uint16_t const DssOverhead        = 690;
			uint16_t const PreRangeOverhead   = 660;
			uint16_t const FinalRangeOverhead = 550;

			uint32_t const MinTimingBudget = 20000;

			if (budget_us < MinTimingBudget)
				return false;

			uint32_t used_budget_us = StartOverhead + EndOverhead;

		    uint8_t sequence_config = ReadByte( REG_SYSTEM_SEQUENCE_CONFIG );
/*
//			getSequenceStepEnables( &enables );
//  enables->tcc          = (sequence_config >> 4) & 0x1;
//  enables->dss          = (sequence_config >> 3) & 0x1;
//  enables->msrc         = (sequence_config >> 2) & 0x1;
//  enables->pre_range    = (sequence_config >> 6) & 0x1;
//  enables->final_range  = (sequence_config >> 7) & 0x1;
*/
			getSequenceStepTimeouts( sequence_config, timeouts );

//			if (enables.tcc)
			if( sequence_config & TCC_ENABLED_FLAG )
				used_budget_us += ( timeouts.msrc_dss_tcc_us + TccOverhead );

//			if (enables.dss)
			if ( sequence_config & DSS_ENABLED_FLAG )
				used_budget_us += 2 * ( timeouts.msrc_dss_tcc_us + DssOverhead );

//			else if (enables.msrc)
			else if ( sequence_config & MSRC_ENABLED_FLAG )
				used_budget_us += ( timeouts.msrc_dss_tcc_us + MsrcOverhead );

//			if (enables.pre_range)
			if ( sequence_config & PRE_RANGE_ENABLED_FLAG )
				used_budget_us += ( timeouts.pre_range_us + PreRangeOverhead );

//			if (enables.final_range)
			if ( sequence_config & FINAL_RANGE_ENABLED_FLAG )
			{
				used_budget_us += FinalRangeOverhead;

				// "Note that the final range timeout is determined by the timing
				// budget and the sum of all other timeouts within the sequence.
				// If there is no room for the final range timeout, then an error
				// will be set. Otherwise the remaining time will be applied to
				// the final range."

				if (used_budget_us > budget_us)
					// "Requested timeout too big."
					return false;

				uint32_t final_range_timeout_us = budget_us - used_budget_us;

				// set_sequence_step_timeout() begin
				// (SequenceStepId == VL53L0X_SEQUENCESTEP_FINAL_RANGE)

				// "For the final range timeout, the pre-range timeout
				//  must be added. To do this both final and pre-range
				//  timeouts must be expressed in macro periods MClks
				//  because they have different vcsel periods."

				uint16_t final_range_timeout_mclks = timeoutMicrosecondsToMclks( final_range_timeout_us, timeouts.final_range_vcsel_period_pclks );

//				if (enables.pre_range)
				if( sequence_config & PRE_RANGE_ENABLED_FLAG )
					final_range_timeout_mclks += timeouts.pre_range_mclks;

				Write16( REG_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI, encodeTimeout( final_range_timeout_mclks ));

				// set_sequence_step_timeout() end

				measurement_timing_budget_us = budget_us; // store for internal reuse
			}

			return true;
		}

		// Get sequence step timeouts
		// based on get_sequence_step_timeout(),
		// but gets all timeouts instead of just the requested one, and also stores
		// intermediate values
//		void getSequenceStepTimeouts(SequenceStepEnables const * enables, SequenceStepTimeouts * timeouts)
		void getSequenceStepTimeouts( uint8_t sequence_config, SequenceStepTimeouts &timeouts )
		{
//			timeouts.pre_range_vcsel_period_pclks = getVcselPulsePeriod(VcselPeriodPreRange);
			timeouts.pre_range_vcsel_period_pclks = getVcselPulsePeriod_PreRange();

			timeouts.msrc_dss_tcc_mclks = ReadByte( REG_MSRC_CONFIG_TIMEOUT_MACROP ) + 1;
			timeouts.msrc_dss_tcc_us = timeoutMclksToMicroseconds(timeouts.msrc_dss_tcc_mclks, timeouts.pre_range_vcsel_period_pclks);

			timeouts.pre_range_mclks = decodeTimeout( Read16( REG_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI) );

			timeouts.pre_range_us = timeoutMclksToMicroseconds( timeouts.pre_range_mclks, timeouts.pre_range_vcsel_period_pclks );

//			timeouts.final_range_vcsel_period_pclks = getVcselPulsePeriod(VcselPeriodFinalRange);
			timeouts.final_range_vcsel_period_pclks = getVcselPulsePeriod_FinalRange();

			timeouts.final_range_mclks = decodeTimeout( Read16( REG_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI ));

			if( sequence_config & PRE_RANGE_ENABLED_FLAG )
				timeouts.final_range_mclks -= timeouts.pre_range_mclks;

			timeouts.final_range_us = timeoutMclksToMicroseconds( timeouts.final_range_mclks, timeouts.final_range_vcsel_period_pclks );
		}

		// Encode sequence step timeout register value from timeout in MCLKs
		// based on VL53L0X_encode_timeout()
		// Note: the original function took a uint16_t, but the argument passed to it
		// is always a uint16_t.
		uint16_t encodeTimeout( uint16_t timeout_mclks )
		{
			// format: "(LSByte * 2^MSByte) + 1"

			uint32_t ls_byte = 0;
			uint16_t ms_byte = 0;

			if( timeout_mclks > 0 )
			{
				ls_byte = timeout_mclks - 1;

				while ((ls_byte & 0xFFFFFF00) > 0)
				{
					ls_byte >>= 1;
					ms_byte++;
				}

				return (ms_byte << 8) | (ls_byte & 0xFF);
			}

			else 
				return 0;

		}

		// Get the VCSEL pulse period in PCLKs for the given period type.
		// based on VL53L0X_get_vcsel_pulse_period()
		inline uint8_t getVcselPulsePeriod_PreRange()
		{
			return decodeVcselPeriod( ReadByte(REG_PRE_RANGE_CONFIG_VCSEL_PERIOD));
		}

		// Get the VCSEL pulse period in PCLKs for the given period type.
		// based on VL53L0X_get_vcsel_pulse_period()
		inline uint8_t getVcselPulsePeriod_FinalRange()
		{
			return decodeVcselPeriod( ReadByte(REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD));
		}

		void StartContinuous(void)
		{
			// Start continuous ranging measurements. If period_ms (optional) is 0 or not
			// given, continuous back-to-back mode is used (the sensor takes measurements as
			// often as possible); otherwise, continuous timed mode is used, with the given
			// inter-measurement period in milliseconds determining how often the sensor
			// takes a measurement.
			// based on VL53L0X_StartMeasurement()

			Write8( 0x80, 0x01 );
			Write8( 0xFF, 0x01 );
			Write8( 0x00, 0x00 );
			Write8( 0x91, FStopVariable );
			Write8( 0x00, 0x01 );
			Write8( 0xFF, 0x00 );
			Write8( 0x80, 0x00 );

			if( SamplingPeriod().GetValue() != 0 )
			{
				uint32_t period_ms = SamplingPeriod().GetValue();
				// continuous timed mode

				// VL53L0X_SetInterMeasurementPeriodMilliSeconds() begin

				uint32_t osc_calibrate_val = Read16( REG_OSC_CALIBRATE_VAL );

				if( osc_calibrate_val != 0 )
					period_ms *= osc_calibrate_val;

				Write32( REG_SYSTEM_INTERMEASUREMENT_PERIOD, period_ms );

				// VL53L0X_SetInterMeasurementPeriodMilliSeconds() end

				Write8( REG_SYSRANGE_START, 0x04 ); // VL53L0X_REG_SYSRANGE_MODE_TIMED
			}

			else
				// continuous back-to-back mode
				Write8( REG_SYSRANGE_START, 0x02 ); // VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK
		
		}

		// Stop continuous measurements
		// based on VL53L0X_StopMeasurement()
		void StopContinuous(void)
		{
			Write8( REG_SYSRANGE_START, 0x01 ); // VL53L0X_REG_SYSRANGE_MODE_SINGLESHOT

			Write8( 0xFF, 0x01 );
			Write8( 0x00, 0x00 );
			Write8( 0x91, 0x00 );
			Write8( 0x00, 0x01 );
			Write8( 0xFF, 0x00 );
		}

		// Encode VCSEL pulse period register value from period in PCLKs
		// based on VL53L0X_encode_vcsel_period()
		inline uint8_t encodeVcselPeriod( uint8_t period_pclks )
		{
			return (( period_pclks >> 1) - 1);
		}

		void UpdateTimimingBudget()
		{
		  // "Finally, the timing budget must be re-applied"

		  setMeasurementTimingBudget(measurement_timing_budget_us);

		  // "Perform the phase calibration. This is needed after changing on vcsel period."
		  // VL53L0X_perform_phase_calibration() begin

		  uint8_t sequence_config = ReadByte( REG_SYSTEM_SEQUENCE_CONFIG );
		  Write8( REG_SYSTEM_SEQUENCE_CONFIG, 0x02 );
		  performSingleRefCalibration( 0x0 );
		  Write8( REG_SYSTEM_SEQUENCE_CONFIG, sequence_config );

		  // VL53L0X_perform_phase_calibration() end
		}

	public:
		inline void SetDeviceAddress( uint8_t ADeviceAddress )
		{
			Write8( REG_I2C_SLAVE_DEVICE_ADDRESS, ADeviceAddress / 2 );
		}

		void UpdatePulsePeriods_PreRange()
		{
			uint8_t AValue = PulsePeriods().PreRange().GetValue();
			uint8_t vcsel_period_reg = encodeVcselPeriod( AValue );

//			SequenceStepEnables enables;
			SequenceStepTimeouts timeouts;

//			getSequenceStepEnables(&enables);
		    uint8_t sequence_config = ReadByte( REG_SYSTEM_SEQUENCE_CONFIG );
			getSequenceStepTimeouts( sequence_config, timeouts );

			// "Apply specific settings for the requested clock period"
			// "Re-calculate and apply timeouts, in macro periods"

			// "When the VCSEL period for the pre or final range is changed,
			// the corresponding timeout must be read from the device using
			// the current VCSEL period, then the new VCSEL period can be
			// applied. The timeout then must be written back to the device
			// using the new VCSEL period.
			//
			// For the MSRC timeout, the same applies - this timeout being
			// dependant on the pre-range vcsel period."

			if ( AValue < 14 )
				Write8( REG_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x18 );

			else if ( AValue < 16 )
				Write8( REG_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x30 );

			else if ( AValue < 18 )
				Write8( REG_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x40 );

			else
				Write8( REG_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x50 );

			Write8( REG_PRE_RANGE_CONFIG_VALID_PHASE_LOW, 0x08 );

			// apply new VCSEL period
			Write8( REG_PRE_RANGE_CONFIG_VCSEL_PERIOD, vcsel_period_reg );

			// update timeouts

			// set_sequence_step_timeout() begin
			// (SequenceStepId == VL53L0X_SEQUENCESTEP_PRE_RANGE)

			uint16_t new_pre_range_timeout_mclks = timeoutMicrosecondsToMclks( timeouts.pre_range_us, AValue );

			Write16( REG_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI, encodeTimeout(new_pre_range_timeout_mclks));

			// set_sequence_step_timeout() end

			// set_sequence_step_timeout() begin
			// (SequenceStepId == VL53L0X_SEQUENCESTEP_MSRC)

			uint16_t new_msrc_timeout_mclks =
			  timeoutMicrosecondsToMclks(timeouts.msrc_dss_tcc_us, AValue );

			Write8( REG_MSRC_CONFIG_TIMEOUT_MACROP, (new_msrc_timeout_mclks > 256) ? 255 : (new_msrc_timeout_mclks - 1));

			// set_sequence_step_timeout() end


			UpdateTimimingBudget();
		}

		void UpdatePulsePeriods_FinalRange()
		{
			uint8_t AValue = PulsePeriods().FinalRange().GetValue();
			uint8_t vcsel_period_reg = encodeVcselPeriod( AValue );

//			SequenceStepEnables enables;
			SequenceStepTimeouts timeouts;

//			getSequenceStepEnables(&enables);
		    uint8_t sequence_config = ReadByte( REG_SYSTEM_SEQUENCE_CONFIG );
			getSequenceStepTimeouts( sequence_config, timeouts );

			// "Apply specific settings for the requested clock period"
			// "Re-calculate and apply timeouts, in macro periods"

			// "When the VCSEL period for the pre or final range is changed,
			// the corresponding timeout must be read from the device using
			// the current VCSEL period, then the new VCSEL period can be
			// applied. The timeout then must be written back to the device
			// using the new VCSEL period.
			//
			// For the MSRC timeout, the same applies - this timeout being
			// dependant on the pre-range vcsel period."


			if( AValue < 10 )
			{
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x10 );
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_LOW,  0x08 );
				Write8( REG_GLOBAL_CONFIG_VCSEL_WIDTH, 0x02 );
				Write8( REG_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x0C );
				Write8(0xFF, 0x01);
				Write8( REG_ALGO_PHASECAL_LIM, 0x30 );
				Write8(0xFF, 0x00);

			}

			else if( AValue < 12 )
			{
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x28 );
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_LOW,  0x08 );
				Write8( REG_GLOBAL_CONFIG_VCSEL_WIDTH, 0x03 );
				Write8( REG_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x09 );
				Write8( 0xFF, 0x01);
				Write8( REG_ALGO_PHASECAL_LIM, 0x20 );
				Write8( 0xFF, 0x00 );
			}

			else if( AValue < 14 )
			{
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x38);
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_LOW,  0x08);
				Write8( REG_GLOBAL_CONFIG_VCSEL_WIDTH, 0x03);
				Write8( REG_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x08);
				Write8( 0xFF, 0x01);
				Write8( REG_ALGO_PHASECAL_LIM, 0x20);
				Write8( 0xFF, 0x00);
			}

			else
			{
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x48 );
				Write8( REG_FINAL_RANGE_CONFIG_VALID_PHASE_LOW,  0x08 );
				Write8( REG_GLOBAL_CONFIG_VCSEL_WIDTH, 0x03 );
				Write8( REG_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x07 );
				Write8( 0xFF, 0x01 );
				Write8( REG_ALGO_PHASECAL_LIM, 0x20 );
				Write8( 0xFF, 0x00 );
			}

			// apply new VCSEL period
			Write8( REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD, vcsel_period_reg );

			// update timeouts

			// set_sequence_step_timeout() begin
			// (SequenceStepId == VL53L0X_SEQUENCESTEP_FINAL_RANGE)

			// "For the final range timeout, the pre-range timeout
			//  must be added. To do this both final and pre-range
			//  timeouts must be expressed in macro periods MClks
			//  because they have different vcsel periods."

			uint16_t new_final_range_timeout_mclks = timeoutMicrosecondsToMclks(timeouts.final_range_us, AValue );

//			if( enables.pre_range )
			if ( sequence_config & PRE_RANGE_ENABLED_FLAG )
				new_final_range_timeout_mclks += timeouts.pre_range_mclks;

			Write16( REG_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI, encodeTimeout( new_final_range_timeout_mclks ));

			// set_sequence_step_timeout end

			UpdateTimimingBudget();
		}

		inline void UpdateEnabled()
		{
			if( Enabled() )
				StartContinuous();

			else
				StopContinuous();

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SystemLoopBegin();
		}

	public:
		inline void SystemStart()
		{
//			FDevice.I2cDevAddr = Address().GetValue();
//			FDevice.comms_type =  1;
//			FDevice.comms_speed_khz = 400;
//			FDevice.i2c	= &C_I2C;

			if( Use2_8V_I2C() )
				Update8( REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, 0xFE, 0x01 );

			// Set I2C standard mode
			Write8( 0x88, 0x00 );

			T_Elements_Apply::Call();

			// read WHO_AM_I
//			uint8_t b = ReadByte( 0xC0 ); // ????

//	#ifdef USE_IQC_STATION
//		apply_offset_adjustment();
//	#endif

			Write8( 0x80, 0x01 );
			Write8( 0xFF, 0x01 );
			Write8( 0x00, 0x00 );
			FStopVariable = ReadByte( 0x91 );
			Write8( 0x00, 0x01 );
			Write8( 0xFF, 0x00 );
			Write8( 0x80, 0x00 );

			// disable SIGNAL_RATE_MSRC (bit 1) and SIGNAL_RATE_PRE_RANGE (bit 4) limit checks
			Write8( REG_MSRC_CONFIG_CONTROL, ReadByte( REG_MSRC_CONFIG_CONTROL ) | 0x12 );

			// set final range signal rate limit to 0.25 MCPS (million counts per second)
//			UpdateSignalRateLimit();

			Write16( REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT, ConvertTo97( ReturnSignalRateLimit().GetValue() ));

			Write8( REG_SYSTEM_SEQUENCE_CONFIG, 0xFF );

			uint8_t spad_count;
			bool spad_type_is_aperture;

			getSpadInfo( spad_count, spad_type_is_aperture );
//			if ( ! getSpadInfo( spad_count, spad_type_is_aperture ))
//				return;

//			Serial.println(  "TEST2" );

			// The SPAD map (RefGoodSpadMap) is read by VL53L0X_get_info_from_device() in
			// the API, but the same data seems to be more easily readable from
			// REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0 through _6, so read it from there
			uint8_t ref_spad_map[ 6 ];
			ReadMulti( REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0, ref_spad_map, 6 );

			// -- VL53L0X_set_reference_spads() begin (assume NVM values are valid)

			Write8( 0xFF, 0x01 );
			Write8( REG_DYNAMIC_SPAD_REF_EN_START_OFFSET, 0x00 );
			Write8( REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD, 0x2C );
			Write8( 0xFF, 0x00 );
			Write8( REG_GLOBAL_CONFIG_REF_EN_START_SELECT, 0xB4 );

			uint8_t first_spad_to_enable = spad_type_is_aperture ? 12 : 0; // 12 is the first aperture spad
			uint8_t spads_enabled = 0;

			for( uint8_t i = 0; i < 48; ++i )
			{
				if (i < first_spad_to_enable || spads_enabled == spad_count)
				{
					// This bit is lower than the first one that should be enabled, or
					// (reference_spad_count) bits have already been enabled, so zero this bit
					ref_spad_map[i / 8] &= ~(1 << (i % 8));
				}

				else if ((ref_spad_map[i / 8] >> (i % 8)) & 0x1)
					++ spads_enabled;
			}

			WriteMulti( REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0, ref_spad_map, 6 );

			load_registers( C_TuningSettingBuffer, C_TuningSettingBuffer_Size );

			// "Set interrupt config to new sample ready"
			// -- VL53L0X_SetGpioConfig() begin

			Write8( REG_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04 );
			Write8( REG_GPIO_HV_MUX_ACTIVE_HIGH, ReadByte( REG_GPIO_HV_MUX_ACTIVE_HIGH) & ~0x10 ); // active low
			Write8( REG_SYSTEM_INTERRUPT_CLEAR, 0x01 );

			// -- VL53L0X_SetGpioConfig() end

			measurement_timing_budget_us = getMeasurementTimingBudget();

			// "Disable MSRC and TCC by default"
			// MSRC = Minimum Signal Rate Check
			// TCC = Target CentreCheck
			// -- VL53L0X_SetSequenceStepEnable() begin

			Write8( REG_SYSTEM_SEQUENCE_CONFIG, 0xE8 );

			// -- VL53L0X_SetSequenceStepEnable() end

			// "Recalculate timing budget"
			setMeasurementTimingBudget( measurement_timing_budget_us );

			// VL53L0X_StaticInit() end

			// VL53L0X_PerformRefCalibration() begin (VL53L0X_perform_ref_calibration())

			// -- VL53L0X_perform_vhv_calibration() begin

//			Serial.println(  "TEST2 - 1" );
//			delay( 10 );

			Write8( REG_SYSTEM_SEQUENCE_CONFIG, 0x01 );
			if ( ! performSingleRefCalibration(0x40) )
				return; // false;

//			Serial.println(  "TEST3" );
			// -- VL53L0X_perform_vhv_calibration() end

			// -- VL53L0X_perform_phase_calibration() begin

			Write8( REG_SYSTEM_SEQUENCE_CONFIG, 0x02 );
			if(!performSingleRefCalibration(0x00))
				return; // false;

			// -- VL53L0X_perform_phase_calibration() end

			// "restore the previous Sequence Config"
			Write8( REG_SYSTEM_SEQUENCE_CONFIG, 0xE8 );

			// VL53L0X_PerformRefCalibration() end

			if( PulsePeriods().PreRange().GetValue() != 14 )
				UpdatePulsePeriods_PreRange();

			if( PulsePeriods().FinalRange().GetValue() != 10 )
				UpdatePulsePeriods_FinalRange();

			if( Enabled() )
				StartContinuous();

		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			if(( ReadByte( REG_RESULT_INTERRUPT_STATUS ) & 0x07 ) == 0 )
				return;

			// assumptions: Linearity Corrective Gain is 1000 (default);
			// fractional ranging is not enabled
			uint16_t range = Read16( REG_RESULT_RANGE_STATUS + 10 );

			Write8( REG_SYSTEM_INTERRUPT_CLEAR, 0x01 );

			T_DitanceOutputPin::SetPinValue( range );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif