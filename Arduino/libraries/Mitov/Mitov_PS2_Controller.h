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
#include <PS2X_lib.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FSmallMotor,
		typename T_Sensors_Process
	> class PS2Controller :
		public T_Enabled,
		public T_FSmallMotor
	{
	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FSmallMotor )
		int8_t	FLargeMotor = 0;

	protected:
		PS2X Fps2x;

	public:
		inline void SmallVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FSmallMotor() = *(bool *)_Data;
		}

		inline void LargeVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FLargeMotor = (int8_t)( MitovConstrain( *(float *)_Data, 0.0f, 1.0f ) * 255 + 0.5 );
		}

	protected:
		inline void ReadController()
		{
			Fps2x.read_gamepad( FSmallMotor(), FLargeMotor );
		}

	public:
		inline bool	ReadDigital( unsigned int AIndex )
		{
			return Fps2x.Button( AIndex );
		}

		inline float	ReadAnalog( unsigned int AIndex )
		{
			return ((float)Fps2x.Analog( AIndex )) / 255;
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
		inline PS2Controller( int ADataPinNumber, int ACommandPinNumber, int AAttentionPinNumber, int AClockPinNumber )
		{
			Fps2x.config_gamepad( AClockPinNumber, ACommandPinNumber, AAttentionPinNumber, ADataPinNumber, true, true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Sensors_Process
	> class PS2Guitar :
		public T_Enabled	
	{
	public:
		_V_PROP_( Enabled )

	protected:
		PS2X Fps2x;

	protected:
		inline void ReadController()
		{
			Fps2x.read_gamepad();
		}

	public:
		inline bool	ReadDigital( unsigned int AIndex )
		{
			return Fps2x.Button( AIndex );
		}

		inline float ReadAnalog( unsigned int AIndex )
		{
			return ((float)Fps2x.Analog( AIndex )) / 255;
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
		inline PS2Guitar( int ADataPinNumber, int ACommandPinNumber, int AAttentionPinNumber, int AClockPinNumber )
		{
			Fps2x.config_gamepad( AClockPinNumber, ACommandPinNumber, AAttentionPinNumber, ADataPinNumber, true, true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif