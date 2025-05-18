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
#include <Mitov_PS2_Controller_Basic.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Enabled,
		typename T_FSmallMotor,
		typename T_Sensors_Process
	> class PS2ControllerSerial :
		public T_Enabled,
		public T_FSmallMotor
	{
	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FSmallMotor )
		int8_t	FLargeMotor = 0;

	public:
		inline void SmallVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FSmallMotor() = *(bool *)_Data;
		}

		inline void LargeVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FLargeMotor = int8_t( MitovConstrain( *(float *)_Data, 0.0f, 1.0f ) * 255 + 0.5 );
		}

	protected:
		inline void ReadController()
		{
			WriteByte( 29 ); // Small Motor
			if( FSmallMotor() )
				WriteByte( 1 );

			else
				WriteByte( 0 );

			WriteByte( 30 ); // Large Motor
			WriteByte( FLargeMotor );
		}

	protected:
		void WriteByte( uint8_t AValue )
		{
			while( C_SERIAL.GetStream().available() > 0 )
				C_SERIAL.GetStream().read();

			C_SERIAL.GetStream().write( AValue );
			C_SERIAL.GetStream().flush(); 	//wait for all data transmitted
		}

		uint8_t ReadByte()
		{
			long waitcount=0;

			while(true)
			{
				if( C_SERIAL.GetStream().available() > 0 )
				{
					uint8_t rec_data = C_SERIAL.GetStream().read();
//					SERIAL_ERR=false;
					return( rec_data );
				}

				waitcount++;
				if( waitcount > 50000 )
				{
//					SERIAL_ERR=true;
					return (0xFF);
				}

			}
		}

	public:
		bool ReadDigital( unsigned int AIndex )
		{
			WriteByte( AIndex );
			return ( ReadByte() == 0 );
		}

		float ReadAnalog( unsigned int AIndex )
		{
			WriteByte( AIndex );
			return float(ReadByte()) / 255;
//			return ((float)Fps2x.Analog( AIndex )) / 255;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( false );
		}

	protected:
		void ReadData( bool AChangeOnly )
		{
			if( ! Enabled() )
				return;

			ReadController();
			T_Sensors_Process::Call( AChangeOnly );
		}

	public:
		inline void SystemStart()
		{
			ReadData( false );
		}

		inline void SystemLoopBegin()
		{
			ReadData( true );
		}

	public:
		inline PS2ControllerSerial()
		{
			FSmallMotor() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif