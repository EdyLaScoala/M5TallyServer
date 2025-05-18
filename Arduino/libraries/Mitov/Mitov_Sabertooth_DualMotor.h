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
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Enabled,
		typename T_SlaveSelectOutputPin
	> class SabertoothDualMotorSimple :
		public T_Enabled,
		public T_SlaveSelectOutputPin
	{
	public:
		_V_PIN_( SlaveSelectOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		int8_t FSpeeds[2] = { 255, 255 };

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				SendStart();

			else
				SendStop();
		}

	protected:
		void SendStart()
		{
			float AValue = FSpeeds[ 0 ];
			FSpeeds[ 0 ] = -1;
			SetSpeed( 0, AValue );

			AValue = FSpeeds[ 1 ];
			FSpeeds[ 1 ] = -1;
			SetSpeed( 1, AValue );
		}

		inline void SendStop()
		{
			SendValue( 0 );
		}

		void SetSpeed( int AIndex, float ASpeed )
		{
			ASpeed = MitovConstrain( ASpeed, 0.0f, 1.0f );
			uint8_t AValue = ASpeed * 126;

			if( FSpeeds[ AIndex ] == AValue )
				return;

			FSpeeds[ AIndex ] = AValue;
			if( ! Enabled() )
				return;


			if( AIndex )
				AValue += 128;

			else
				++ AValue;

			SendValue( AValue );
		}

		void SendValue( uint8_t AValue )
		{
			T_SlaveSelectOutputPin::SetPinValueHigh();

			C_SERIAL.GetStream().write( AValue );

			if( T_SlaveSelectOutputPin::GetPinIsConnected() )
			{
				delayMicroseconds( 50 );
				T_SlaveSelectOutputPin::SetPinValueLow();
			}
		}

	public:
		inline void SystemStart()
		{
			T_SlaveSelectOutputPin::SetPinValueLow();
		}

	public:
		inline void MotorsInputPins_o_Receive( int AIndex, void *_Data )
		{
			SetSpeed( AIndex, *(float *)_Data );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Address,
		typename T_Deadband,
		typename T_DirectionalMode,
		typename T_Enabled,
		typename T_MaxVoltage,
		typename T_MinVoltage,
		typename T_Ramping,
		typename T_SerialTimeout
	> class SabertoothDualMotorPacket :
		public T_Address,
		public T_Deadband,
		public T_DirectionalMode,
		public T_Enabled,
		public T_MaxVoltage,
		public T_MinVoltage,
		public T_Ramping,
		public T_SerialTimeout
	{
	public:
		_V_PROP_( Address )

		_V_PROP_( MinVoltage )
		_V_PROP_( MaxVoltage )

		_V_PROP_( Ramping )

		_V_PROP_( SerialTimeout )
		_V_PROP_( Deadband )

		_V_PROP_( Enabled )
		_V_PROP_( DirectionalMode )

	protected:
		float FSpeeds[2] = { -1.0, -1.0 };

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				SendStart();

			else
				SendStop();
		}

	public:
		inline void UpdateMinVoltage()
		{
			SendCommand( 2, ( MinVoltage().GetValue() - 6 ) * 5 );
		}

		inline void UpdateMaxVoltage()
		{
			SendCommand( 3, MinVoltage() * 5.12 );
		}

		inline void UpdateRamping()
		{
			SendCommand( 16, Ramping() );
		}

		inline void SendSerialTimeout()
		{
			SendCommand( 14, ( SerialTimeout() + 99 ) / 100 );
		}

		inline void UpdateDeadband()
		{
			SendCommand( 17, Deadband() );
		}

	protected:
		void SendStart()
		{
			SendStartOne( 0 );
			SendStartOne( 1 );
		}

		void SendStartOne( int AIndex )
		{
			int  AValue = ( FSpeeds[ AIndex ] * 255 + 0.5 ) - 128;
			uint8_t  ACommand;
			if( DirectionalMode() )
			{
				if( AIndex == 0 )
					ACommand = 10;

				else
					ACommand = 8;
			}

			else
				ACommand = AIndex << 2; // 0 or 4

			if( AValue < 0 )
			{
				AValue = -AValue;
				ACommand |= 1;
			}

			SendCommand( ACommand, AValue & 0b01111111 );
		}

		void SendStop()
		{
			// Shut down both motors
			SendCommand( 0, 0 );
			SendCommand( 4, 0 );
		}

		void SetSpeed( int AIndex, float ASpeed )
		{
			ASpeed = MitovConstrain( ASpeed, 0.0f, 1.0f );
			if( ASpeed == FSpeeds[ AIndex ] )
				return;

			FSpeeds[ AIndex ] = ASpeed;
			if( Enabled() )
				SendStart();
		}

	protected:
		void SendCommand( uint8_t ACommand, uint8_t AValue )
		{
//			typename T_SERIAL::T_STREAM AStream = C_SERIAL.GetStream();

			uint8_t AAddress = Address() | 0b10000000;
			C_SERIAL.Write( AAddress );
			C_SERIAL.Write( ACommand );
			C_SERIAL.Write( AValue );
			C_SERIAL.Write(( AAddress + ACommand + AValue ) & 0b1111111);
		}

	public:
		inline void SystemStart()
		{
			if( MinVoltage().GetValue() != 6.0 )
				SendCommand( 2, ( MinVoltage().GetValue() - 6 ) * 5 );

			if( MaxVoltage().GetValue() != 25.0 )
				SendCommand( 3, MinVoltage().GetValue() * 5.12 );

			if( Ramping() )
				SendCommand( 16, Ramping() );

			if( SerialTimeout() )
				SendSerialTimeout();

			if( Deadband() )
				SendCommand( 17, Deadband() );
		}

	public:
		inline void MotorsInputPins_o_Receive( int AIndex, void *_Data )
		{
			SetSpeed( AIndex, *(float *)_Data );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif