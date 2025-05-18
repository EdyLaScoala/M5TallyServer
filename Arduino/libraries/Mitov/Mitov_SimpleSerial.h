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
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_Enabled,
		typename T_OutputPin
	> class BasicSerialWrapper :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )

	public:
		typedef Stream & T_STREAM;

	public:
		inline Stream &GetStream()
		{
			return C_SERIAL;
		}

	public:
		inline void SystemLoopBeginOutput()
		{
			if( ! Enabled() )
				return;

			uint8_t ABuffer[ 20 ];

			int i = 0;
			for( ; i < 20; ++i )
			{
				int AData = C_SERIAL.read();
				if( AData < 0 )
					break;

				ABuffer[ i ] = AData;
			}

			if( i > 0 )
				T_OutputPin::SetPinValue( Mitov::TDataBlock( i, ABuffer ));

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_Enabled,
		typename T_OutputPin
	> class SimpleSerial : public BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin>
	{
		typedef BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin> inherited;

	public:
		void ResetSerial()
		{
			C_SERIAL.end();
			StartPort();
		}

	protected:
		inline void SerialEnd() {} // Placeholder

		inline void StartPort()
		{
			C_SERIAL.begin();
		}

	public:
		template <typename T> void Print( T AValue )
		{
			if( inherited::Enabled() )
				C_SERIAL.println( AValue );

		}

		void PrintChar( char AValue )
		{
			if( inherited::Enabled() )
				C_SERIAL.print( AValue );

		}

		size_t Write( uint8_t *AData, uint32_t ASize )
		{
			if( inherited::Enabled() )
				return C_SERIAL.write( AData, ASize );

			return 0;
		}

	    size_t Write(uint8_t byte)
		{
			if( inherited::Enabled() )
				return C_SERIAL.write( byte );

			return 0;
		}

		inline int AvailableForWrite()
		{
			return C_SERIAL.availableForWrite();
		}

	public:
		inline void SystemLoopBegin()
		{
		}

		inline void SystemInit()
		{
			if( inherited::Enabled() )
				StartPort();

		}

	public:
        void RestartPort() // Used by setters
		{
			if( ! inherited::Enabled() )
				return;

			C_SERIAL.end();
			StartPort();
		}

	public:
		void UpdateEnabled()
		{
			if( inherited::Enabled() )
				StartPort();

			else
			{
				C_SERIAL.end();
				SerialEnd();
			}
		}

	};
//---------------------------------------------------------------------------
} // Mitov
