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
namespace BMM150
{
	const uint8_t REG_CHIP_ID			= 0x40;
	const uint8_t REG_DATA_X_LSB		= 0x42;
	const uint8_t REG_DATA_X_MSB		= 0x43;
	const uint8_t REG_DATA_Y_LSB		= 0x44;
	const uint8_t REG_DATA_Y_MSB		= 0x45;
	const uint8_t REG_DATA_Z_LSB		= 0x46;
	const uint8_t REG_DATA_Z_MSB		= 0x47;
	const uint8_t REG_DATA_READY_STATUS	= 0x48;
	const uint8_t REG_INTERRUPT_STATUS	= 0x4A;
	const uint8_t REG_POWER_CONTROL		= 0x4B;
	const uint8_t REG_OP_MODE			= 0x4C;
	const uint8_t REG_INT_CONFIG		= 0x4D;
	const uint8_t REG_AXES_ENABLE		= 0x4E;
	const uint8_t REG_LOW_THRESHOLD		= 0x4F;
	const uint8_t REG_HIGH_THRESHOLD	= 0x50;
	const uint8_t REG_REP_XY		    = 0x51;
	const uint8_t REG_REP_Z				= 0x52;

	// Trim Extended Registers
	const uint8_t REG_DIG_X1            = 0x5D;
	const uint8_t REG_DIG_Y1            = 0x5E;
	const uint8_t REG_DIG_Z4_LSB        = 0x62;
	const uint8_t REG_DIG_Z4_MSB        = 0x63;
	const uint8_t REG_DIG_X2            = 0x64;
	const uint8_t REG_DIG_Y2            = 0x65;
	const uint8_t REG_DIG_Z2_LSB        = 0x68;
	const uint8_t REG_DIG_Z2_MSB        = 0x69;
	const uint8_t REG_DIG_Z1_LSB        = 0x6A;
	const uint8_t REG_DIG_Z1_MSB        = 0x6B;
	const uint8_t REG_DIG_XYZ1_LSB      = 0x6C;
	const uint8_t REG_DIG_XYZ1_MSB      = 0x6D;
	const uint8_t REG_DIG_Z3_LSB        = 0x6E;
	const uint8_t REG_DIG_Z3_MSB        = 0x6F;
	const uint8_t REG_DIG_XY2           = 0x70;
	const uint8_t REG_DIG_XY1           = 0x71;


	// OVERFLOW DEFINITIONS
	const int32_t XYAXES_FLIP_OVERFLOW_ADCVAL	= -4096;
	const int32_t ZAXIS_HALL_OVERFLOW_ADCVAL	= -16384;
	const int32_t OVERFLOW_OUTPUT			    = -32768;
	const int32_t NEGATIVE_SATURATION_Z         = -32767;
	const int32_t POSITIVE_SATURATION_Z			= 32767;
	const float OVERFLOW_OUTPUT_FLOAT			= 0.0f;

}
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_Enabled,
		typename T_OutputPins_X,
		typename T_OutputPins_Y,
		typename T_OutputPins_Z,
		typename T_RepeatXY,
		typename T_RepeatZ,
		typename T_SamplingRate
	> class BoshBMM150 : 
		public T_IMPLEMENTATION,
		public T_Enabled,
		public T_OutputPins_X,
		public T_OutputPins_Y,
		public T_OutputPins_Z,
		public T_RepeatXY,
		public T_RepeatZ,
		public T_SamplingRate
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( OutputPins_Z )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( SamplingRate )
		_V_PROP_( RepeatXY )
		_V_PROP_( RepeatZ )

	protected:		
		int8_t		FDig_x1; //! trim x1 data		
		int8_t		FDig_y1; //! trim y1 data		
		int8_t		FDig_x2; //! trim x2 data		
		int8_t		FDig_y2; //! trim y2 data
		uint16_t	FDig_z1; //! trim z1 data
		int16_t		FDig_z2; //! trim z2 data
		int16_t		FDig_z3; //! trim z3 data
		int16_t		FDig_z4; //! trim z4 data
		uint8_t		FDig_xy1; //! trim xy1 data
		int8_t		FDig_xy2; //! trim xy2 data
		uint16_t	FDig_xyz1; //! trim xyz1 data

//		int16_t		FOffsetX;
//		int16_t		FOffsetY;
//		int16_t		FOffsetZ;

	protected:
		void ReadTrimRegisters()
		{
//			uint8_t trim_x1y1[2] = {0};
			uint8_t trim_xyz_data[4] = {0};
			uint8_t trim_xy1xy2[10] = {0};
			uint16_t temp_msb = 0;

//				dev->trim_data.dig_x1 = (int8_t)trim_x1y1[0];
//				dev->trim_data.dig_y1 = (int8_t)trim_x1y1[1];
//				dev->trim_data.dig_x2 = (int8_t)trim_xyz_data[2];
//				dev->trim_data.dig_y2 = (int8_t)trim_xyz_data[3];
//				temp_msb = ((uint16_t)trim_xy1xy2[3]) << 8;
//				dev->trim_data.dig_z1 = (uint16_t)(temp_msb | trim_xy1xy2[2]);
//				temp_msb = ((uint16_t)trim_xy1xy2[1]) << 8;
//				dev->trim_data.dig_z2 = (int16_t)(temp_msb | trim_xy1xy2[0]);
//				temp_msb = ((uint16_t)trim_xy1xy2[7]) << 8;
//				dev->trim_data.dig_z3 = (int16_t)(temp_msb | trim_xy1xy2[6]);
//				temp_msb = ((uint16_t)trim_xyz_data[1]) << 8;
//				dev->trim_data.dig_z4 = (int16_t)(temp_msb | trim_xyz_data[0]);
//				dev->trim_data.dig_xy1 = trim_xy1xy2[9];
//				dev->trim_data.dig_xy2 = (int8_t)trim_xy1xy2[8];
//				temp_msb = ((uint16_t)(trim_xy1xy2[5] & 0x7F)) << 8;
//				dev->trim_data.dig_xyz1 = (uint16_t)(temp_msb | trim_xy1xy2[4]);

			// Trim register value is read
			T_IMPLEMENTATION::StartReadRegisters( BMM150::REG_DIG_X1, 16 );

			FDig_x1 = int8_t( T_IMPLEMENTATION::ReadUInt8() );
			FDig_y1 = int8_t( T_IMPLEMENTATION::ReadUInt8() );

			FDig_z4 = T_IMPLEMENTATION::ReadUInt8();
			FDig_z4 |= int16_t( int8_t( T_IMPLEMENTATION::ReadUInt8() )) << 8;

			FDig_x2 = int8_t( T_IMPLEMENTATION::ReadUInt8() );
			FDig_y2 = int8_t( T_IMPLEMENTATION::ReadUInt8() );

			FDig_z2 = T_IMPLEMENTATION::ReadUInt8(); // 0
			FDig_z2 |= int16_t( int8_t( T_IMPLEMENTATION::ReadUInt8() ) ) << 8; // 1

			FDig_z1 = T_IMPLEMENTATION::ReadUInt8(); // 2
			FDig_z1 |= uint16_t( T_IMPLEMENTATION::ReadUInt8() ) << 8; // 3

			FDig_xyz1 = T_IMPLEMENTATION::ReadUInt8(); // 4
			FDig_xyz1 |= int16_t( int8_t( T_IMPLEMENTATION::ReadUInt8() )) << 8; // 5

			FDig_z3 = T_IMPLEMENTATION::ReadUInt8(); // 6
			FDig_z3 |= int16_t( int8_t( T_IMPLEMENTATION::ReadUInt8() ) ) << 8; // 7

			FDig_xy2 = int8_t( T_IMPLEMENTATION::ReadUInt8() ); // 8
			FDig_xy1 = T_IMPLEMENTATION::ReadUInt8(); // 9

			T_IMPLEMENTATION::EndReadRegisters();
		}

		float CompensateX( int16_t mag_data_x, uint16_t data_rhall )
		{
			float retval = 0;
			float process_comp_x0;
			float process_comp_x1;
			float process_comp_x2;
			float process_comp_x3;
			float process_comp_x4;

			// Overflow condition check
			if ((mag_data_x != BMM150::XYAXES_FLIP_OVERFLOW_ADCVAL) &&
				(data_rhall != 0) && (FDig_xyz1 != 0)) 
			{
					// Processing compensation equations
					process_comp_x0 = (((float)FDig_xyz1) * 16384.0f / data_rhall);
					retval = (process_comp_x0 - 16384.0f);
					process_comp_x1 = ((float)FDig_xy2) * (retval * retval / 268435456.0f);
					process_comp_x2 = process_comp_x1 + retval * ((float)FDig_xy1) / 16384.0f;
					process_comp_x3 = ((float)FDig_x2) + 160.0f;
					process_comp_x4 = mag_data_x * ((process_comp_x2 + 256.0f) * process_comp_x3);
					retval = ((process_comp_x4 / 8192.0f) + (((float)FDig_x1) * 8.0f)) / 16.0f;
			}

			else
				// overflow, set output to 0.0f
				retval = BMM150::OVERFLOW_OUTPUT_FLOAT;

			return retval;
		}

		float CompensateY( int16_t mag_data_y, uint16_t data_rhall )
		{
			float retval = 0;
			float process_comp_y0;
			float process_comp_y1;
			float process_comp_y2;
			float process_comp_y3;
			float process_comp_y4;

			// Overflow condition check
			if ((mag_data_y != BMM150::XYAXES_FLIP_OVERFLOW_ADCVAL)
				&& (data_rhall != 0) && (FDig_xyz1 != 0)) 
			{
					// Processing compensation equations
					process_comp_y0 = ((float)FDig_xyz1) * 16384.0f / data_rhall;
					retval = process_comp_y0 - 16384.0f;
					process_comp_y1 = ((float)FDig_xy2) * (retval * retval / 268435456.0f);
					process_comp_y2 = process_comp_y1 + retval * ((float)FDig_xy1) / 16384.0f;
					process_comp_y3 = ((float)FDig_y2) + 160.0f;
					process_comp_y4 = mag_data_y * (((process_comp_y2) + 256.0f) * process_comp_y3);
					retval = ((process_comp_y4 / 8192.0f) + (((float)FDig_y1) * 8.0f)) / 16.0f;
			}

			else
				// overflow, set output to 0.0f
				retval = BMM150::OVERFLOW_OUTPUT_FLOAT;

			return retval;
		}

		float CompensateZ( int16_t mag_data_z, uint16_t data_rhall )
		{
			float retval = 0;
			float process_comp_z0;
			float process_comp_z1;
			float process_comp_z2;
			float process_comp_z3;
			float process_comp_z4;
			float process_comp_z5;

			 // Overflow condition check
			if ((mag_data_z != BMM150::ZAXIS_HALL_OVERFLOW_ADCVAL) &&
				(FDig_z2 != 0) && (FDig_z1 != 0)
				&& (FDig_xyz1 != 0) && (data_rhall != 0)) 
			{
					// Processing compensation equations
					process_comp_z0 = ((float)mag_data_z) - ((float)FDig_z4);
					process_comp_z1 = ((float)data_rhall) - ((float)FDig_xyz1);
					process_comp_z2 = (((float)FDig_z3) * process_comp_z1);
					process_comp_z3 = ((float)FDig_z1) * ((float)data_rhall) / 32768.0f;
					process_comp_z4 = ((float)FDig_z2) + process_comp_z3;
					process_comp_z5 = (process_comp_z0 * 131072.0f) - process_comp_z2;
					retval = (process_comp_z5 / ((process_comp_z4) * 4.0f)) / 16.0f;
			}

			else
				// overflow, set output to 0.0f
				retval = BMM150::OVERFLOW_OUTPUT_FLOAT;

			return retval;
		}

/*
		int16_t CompensateX( int16_t mag_data_x, uint16_t data_rhall )
		{
			int16_t retval;
			uint16_t process_comp_x0 = 0;
			int32_t process_comp_x1;
			uint16_t process_comp_x2;
			int32_t process_comp_x3;
			int32_t process_comp_x4;
			int32_t process_comp_x5;
			int32_t process_comp_x6;
			int32_t process_comp_x7;
			int32_t process_comp_x8;
			int32_t process_comp_x9;
			int32_t process_comp_x10;

			// Overflow condition check
			if (mag_data_x != BMM150::XYAXES_FLIP_OVERFLOW_ADCVAL ) 
			{
				if (data_rhall != 0) 
					// Availability of valid data
					process_comp_x0 = data_rhall;

				else if ( FDig_xyz1 != 0) 
					process_comp_x0 = FDig_xyz1;

				else
					process_comp_x0 = 0;

				if (process_comp_x0 != 0) 
				{
					// Processing compensation equations
					process_comp_x1 = ((int32_t) FDig_xyz1) * 16384;
					process_comp_x2 = ((uint16_t)(process_comp_x1 / process_comp_x0)) - ((uint16_t)0x4000);
					retval = ((int16_t)process_comp_x2);
					process_comp_x3 = (((int32_t)retval) * ((int32_t)retval));
					process_comp_x4 = (((int32_t)FDig_xy2) * (process_comp_x3 / 128));
					process_comp_x5 = (int32_t)(((int16_t)FDig_xy1) * 128);
					process_comp_x6 = ((int32_t)retval) * process_comp_x5;
					process_comp_x7 = (((process_comp_x4 + process_comp_x6) / 512) + ((int32_t)0x100000));
					process_comp_x8 = ((int32_t)(((int16_t)FDig_x2) + ((int16_t)0xA0)));
					process_comp_x9 = ((process_comp_x7 * process_comp_x8) / 4096);
					process_comp_x10 = ((int32_t)mag_data_x) * process_comp_x9;
					retval = ((int16_t)(process_comp_x10 / 8192));
					retval = (retval + (((int16_t)FDig_x1) * 8)) / 16;
				}
				else
					retval = BMM150::OVERFLOW_OUTPUT;
			}
			else
				// Overflow condition
				retval = BMM150::OVERFLOW_OUTPUT;

			return retval;
		}

		int16_t CompensateY( int16_t mag_data_y, uint16_t data_rhall )
		{
			int16_t retval;
			uint16_t process_comp_y0 = 0;
			int32_t process_comp_y1;
			uint16_t process_comp_y2;
			int32_t process_comp_y3;
			int32_t process_comp_y4;
			int32_t process_comp_y5;
			int32_t process_comp_y6;
			int32_t process_comp_y7;
			int32_t process_comp_y8;
			int32_t process_comp_y9;

			// Overflow condition check
			if( mag_data_y != BMM150::XYAXES_FLIP_OVERFLOW_ADCVAL )
			{
				if (data_rhall != 0)
					// Availability of valid data
					process_comp_y0 = data_rhall;

				else if ( FDig_xyz1 != 0 )
					process_comp_y0 = FDig_xyz1;

				else
					process_comp_y0 = 0;

				if (process_comp_y0 != 0) 
				{
					// Processing compensation equations
					process_comp_y1 = (((int32_t) FDig_xyz1) * 16384) / process_comp_y0;
					process_comp_y2 = ((uint16_t)process_comp_y1) - ((uint16_t)0x4000);
					retval = ((int16_t)process_comp_y2);
					process_comp_y3 = ((int32_t) retval) * ((int32_t)retval);
					process_comp_y4 = ((int32_t) FDig_xy2) * (process_comp_y3 / 128);
					process_comp_y5 = ((int32_t)(((int16_t) FDig_xy1) * 128));
					process_comp_y6 = ((process_comp_y4 + (((int32_t)retval) * process_comp_y5)) / 512);
					process_comp_y7 = ((int32_t)(((int16_t) FDig_y2) + ((int16_t)0xA0)));
					process_comp_y8 = (((process_comp_y6 + ((int32_t)0x100000)) * process_comp_y7) / 4096);
					process_comp_y9 = (((int32_t)mag_data_y) * process_comp_y8);
					retval = (int16_t)(process_comp_y9 / 8192);
					retval = (retval + (((int16_t) FDig_y1) * 8)) / 16;
				}
				else
					retval = BMM150::OVERFLOW_OUTPUT;

			}
			else
				// Overflow condition
				retval = BMM150::OVERFLOW_OUTPUT;

			return retval;
		}

		int16_t CompensateZ(int16_t mag_data_z, uint16_t data_rhall)
		{
			int32_t retval;
			int16_t process_comp_z0;
			int32_t process_comp_z1;
			int32_t process_comp_z2;
			int32_t process_comp_z3;
			int16_t process_comp_z4;

			if (mag_data_z != BMM150::ZAXIS_HALL_OVERFLOW_ADCVAL)
			{
				if ((FDig_z2 != 0) && (FDig_z1 != 0)
				&& (data_rhall != 0) && (FDig_xyz1 != 0))
				{
					// Processing compensation equations
					process_comp_z0 = ((int16_t)data_rhall) - ((int16_t) FDig_xyz1);
					process_comp_z1 = (((int32_t)FDig_z3) * ((int32_t)(process_comp_z0))) / 4;
					process_comp_z2 = (((int32_t)(mag_data_z - FDig_z4)) * 32768);
					process_comp_z3 = ((int32_t)FDig_z1) * (((int16_t)data_rhall) * 2);
					process_comp_z4 = (int16_t)((process_comp_z3 + (32768)) / 65536);
					retval = ((process_comp_z2 - process_comp_z1) / (FDig_z2 + process_comp_z4));

					// saturate result to +/- 2 micro-tesla
					if (retval > BMM150::POSITIVE_SATURATION_Z)
						retval =  BMM150::POSITIVE_SATURATION_Z;

					else
					{
						if (retval < BMM150::NEGATIVE_SATURATION_Z)
							retval = BMM150::NEGATIVE_SATURATION_Z;
					}

					// Conversion of LSB to micro-tesla
					retval = retval / 16;
				} 

				else
					retval = BMM150::OVERFLOW_OUTPUT;

			}
			else
				// Overflow condition
				retval = BMM150::OVERFLOW_OUTPUT;

			return (int16_t)retval;
		}
*/
		void SuspendToSleepMode(void)
		{
//			set_power_control_bit(BMM150_POWER_CNTRL_ENABLE);

			T_IMPLEMENTATION::WriteRegister8( BMM150::REG_POWER_CONTROL, 0x01 );
			// Start-up time delay of 3ms
			delay(3);
		}

		bool ReadMagData( float &x, float &y, float &z )
		{
//			BMM150::mag_data AResult;
			T_IMPLEMENTATION::StartReadRegisters( BMM150::REG_DATA_X_LSB, 8 );

			// Mag X axis data
			int16_t AResultX = T_IMPLEMENTATION::ReadUInt8() >> 3;
			AResultX |= int16_t( int8_t( T_IMPLEMENTATION::ReadUInt8() )) << 5;

			int16_t AResultY = T_IMPLEMENTATION::ReadUInt8() >> 3;
			AResultY |= int16_t( int8_t( T_IMPLEMENTATION::ReadUInt8() )) << 5;

			int16_t AResultZ = T_IMPLEMENTATION::ReadUInt8() >> 1;
			AResultZ |= int16_t( int8_t( T_IMPLEMENTATION::ReadUInt8() )) << 7;

			// Mag R-HALL data
			uint8_t AValue = T_IMPLEMENTATION::ReadUInt8();
			uint16_t ARawDataR = AValue >> 2;
			ARawDataR |= uint16_t( T_IMPLEMENTATION::ReadUInt8() ) << 6;

			x = CompensateX( AResultX, ARawDataR );
			y = CompensateY( AResultY, ARawDataR );
			z = CompensateZ( AResultZ, ARawDataR );

			T_IMPLEMENTATION::EndReadRegisters();

			return (( AValue & 0b00000001 ) != 0 );
		}
/*
		void Callibrate()
		{
			int16_t value_x_min = 0;
			int16_t value_x_max = 0;
			int16_t value_y_min = 0;
			int16_t value_y_max = 0;
			int16_t value_z_min = 0;
			int16_t value_z_max = 0;
			uint32_t timeStart = 0;

			BMM150::mag_data AData = ReadMagData();

			value_x_min = AData.x;
			value_x_max = AData.x;
			value_y_min = AData.y;
			value_y_max = AData.y;
			value_z_min = AData.z;
			value_z_max = AData.z;
			delay(100);

			timeStart = millis();
  
			while((millis() - timeStart) < 10 )
			{
				AData = ReadMagData();
    
				// Update x-Axis max/min value
				if( value_x_min > AData.x )
				{
					value_x_min = AData.x;
					// Serial.print("Update value_x_min: ");
					// Serial.println(value_x_min);

				} 

				else if( value_x_max < AData.x )
				{
					value_x_max = AData.x;
					// Serial.print("update value_x_max: ");
					// Serial.println(value_x_max);
				}

				// Update y-Axis max/min value
				if( value_y_min > AData.y )
				{
					value_y_min = AData.y;
					// Serial.print("Update value_y_min: ");
					// Serial.println(value_y_min);

				} 

				else if( value_y_max < AData.y )
				{
					value_y_max = AData.y;
					// Serial.print("update value_y_max: ");
					// Serial.println(value_y_max);
				}

				// Update z-Axis max/min value
				if( value_z_min > AData.z )
				{
					value_z_min = AData.z;
					// Serial.print("Update value_z_min: ");
					// Serial.println(value_z_min);

				} 

				else if( value_z_max < AData.z )
				{
					value_z_max = AData.z;
					// Serial.print("update value_z_max: ");
					// Serial.println(value_z_max);
				}
    
	//			Serial.print(".");
				delay(1);
			}

			FOffsetX = value_x_min + (value_x_max - value_x_min)/2;
			FOffsetY = value_y_min + (value_y_max - value_y_min)/2;
			FOffsetZ = value_z_min + (value_z_max - value_z_min)/2;
		}
*/
		void ReadSensors( bool AChangeOnly, bool AForceRead )
		{
			if( ! Enabled().GetValue() )
				return;

			float x;
			float y;
			float z;

			bool AHasData = ReadMagData( x, y, z );
			if( ( ! AHasData ) && ( ! AForceRead ) )
				return;

			T_OutputPins_X::SetPinValue( x, AChangeOnly );
			T_OutputPins_Y::SetPinValue( y, AChangeOnly );
			T_OutputPins_Z::SetPinValue( z, AChangeOnly );
		}

	public:
		inline void UpdateOperationMode()
		{
			uint8_t AValue;

			if( SamplingRate().GetValue() < 6 )
				AValue = 0b001000;

			else if( SamplingRate().GetValue() < 8 )
				AValue = 0b010000;

			else if( SamplingRate().GetValue() < 10 )
				AValue = 0b011000;

			else if( SamplingRate().GetValue() < 15 )
				AValue = 0b000000;

			else if( SamplingRate().GetValue() < 20 )
				AValue = 0b100000;

			else if( SamplingRate().GetValue() < 25 )
				AValue = 0b101000;

			else if( SamplingRate().GetValue() < 30 )
				AValue = 0b110000;

			else
				AValue = 0b111000;

			T_IMPLEMENTATION::WriteRegister8( BMM150::REG_OP_MODE, AValue );
		}

		inline void UpdateRepetitionsXY()
		{
			T_IMPLEMENTATION::WriteRegister8( BMM150::REG_REP_XY, RepeatXY().GetValue() >> 1 );
		}

		inline void UpdateRepetitionsZ()
		{
			T_IMPLEMENTATION::WriteRegister8( BMM150::REG_REP_Z, RepeatZ().GetValue() - 1 );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false, true );
		}

		inline void ResetInputPin_o_Receive( void *_Data )
		{
			T_IMPLEMENTATION::WriteRegister8( BMM150::REG_POWER_CONTROL, 0x01 | 0x82 );
		}

	public:
		inline void SystemStartCommon()
		{
			SuspendToSleepMode();

			// Check chip ID
//			uint8_t id = i2c_read(BMM150_CHIP_ID_ADDR);	
//			if(id != BMM150_CHIP_ID)
//				return BMM150_E_ID_NOT_CONFORM;

			ReadTrimRegisters();

			UpdateOperationMode();
			UpdateRepetitionsXY();
			UpdateRepetitionsZ();
		}

		inline void SystemStart()
		{
			ReadSensors( false, false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true, false );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


