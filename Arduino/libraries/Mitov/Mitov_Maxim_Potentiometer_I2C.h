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
#include <Mitov_Basic_I2C.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_FModified,
		typename T_InitialValue
	> class Maxim_Potentiometer_I2C_Channel :
		public T_FModified,
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( FModified )

	public:
		void GetValue( bool & AIsModified, float & AValue )
		{
			AIsModified = FModified().GetValue();
			AValue = InitialValue().GetValue();
			FModified() = false;
		}

		void GetModified( bool & AIsModified )
		{
			AIsModified |= FModified().GetValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *((float *)_Data);
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_Channels_GetModified,
		typename T_Channels_Channels_GetValue,
		typename T_Enabled
	> class Maxim_Potentiometer_I2C :
		public T_Address,
		public T_Enabled
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	protected:
		void TransmitAt( uint8_t ARegister, uint8_t Adata )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));

			C_I2C.write( ARegister );
			C_I2C.write( Adata );

			C_I2C.endTransmission();
		}

	protected:
		void UpdateOutput()
		{
			if( ! Enabled().GetValue() )
				return;

			bool AIsModified_0 = false;
			float ANewValue_0;
			T_Channels_Channels_GetValue::Call( 0, AIsModified_0, ANewValue_0 );

			TransmitAt( 0b00010001, uint8_t( MitovConstrain( ANewValue_0, 0.0f, 1.0f ) * 255 + 0.5 ));
		}

		void CheckUpdateOutput()
		{
			bool AModified = false;
			T_Channels_Channels_GetModified::Call( AModified );

			if( AModified )
				UpdateOutput();
		}

	public:
		inline void SystemStart()
		{
			UpdateOutput();
		}

		inline void SystemLoopUpdateHardware()
		{
			CheckUpdateOutput();
		}

	public:
		inline void ClockInputPin_o_Receive( void * )
		{
			CheckUpdateOutput();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Channels_Channels_GetModified,
		typename T_Channels_Channels_GetValue,
		typename T_Enabled
	> class Maxim_Potentiometer_I2C_2Channel :
		public T_Address,
		public T_Enabled
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

/*
	public:
		void Channel_InputPin_o_Receive( int AIndex, void *_Data )
		{
			float AValue = MitovConstrain( *((float *)_Data), 0.0f, 1.0f ) * 255;
			if( AValue == Channels[ AIndex ].InitialValue )
				return;

			Channels[ AIndex ].InitialValue = AValue;
			FModified |= 1 << AIndex;
		}
*/
	protected:
		void TransmitAt( uint8_t ARegister, uint8_t Adata )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));

			C_I2C.write( ARegister );
			C_I2C.write( Adata );

			C_I2C.endTransmission();
		}

	protected:
		void UpdateOutput()
		{
			bool AIsModified_0 = false;
			float ANewValue_0;
			T_Channels_Channels_GetValue::Call( 0, AIsModified_0, ANewValue_0 );

			bool AIsModified_1 = false;
			float ANewValue_1;
			T_Channels_Channels_GetValue::Call( 1, AIsModified_1, ANewValue_1 );

			if( ANewValue_0 == ANewValue_1 )
				// Update both channels at once
				TransmitAt( 0b00010011, uint8_t( MitovConstrain( ANewValue_0, 0.0f, 1.0f ) * 255 + 0.5 ));

			else
			{
				if( AIsModified_0 )
					TransmitAt( 0b00010001, uint8_t( MitovConstrain( ANewValue_0, 0.0f, 1.0f ) * 255 + 0.5 ) );

				if( AIsModified_1 )
					TransmitAt( 0b00010010, uint8_t( MitovConstrain( ANewValue_1, 0.0f, 1.0f ) * 255 + 0.5 ) );
			}

/*
			float ANewValue = Channels[ 0 ].InitialValue;
			if( ANewValue == Channels[ 1 ].InitialValue )
				// Update both channels at once
				TransmitAt( 0b00010011, uint8_t( ANewValue ));

			else
			{
				if( FModified().GetValue() & 0b01 )
					TransmitAt( 0b00010001, uint8_t( ANewValue ));

				if( FModified().GetValue() & 0b10 )
					TransmitAt( 0b00010010, uint8_t( Channels[ 1 ].InitialValue ));
			}

			FModified() = 0;
*/
		}

		void CheckUpdateOutput()
		{
			bool AModified = false;
			T_Channels_Channels_GetModified::Call( AModified );

			if( AModified )
				UpdateOutput();
		}

	public:
		inline void SystemStart()
		{
			UpdateOutput();
		}

		inline void SystemLoopUpdateHardware()
		{
			CheckUpdateOutput();
		}

	public:
		inline void ClockInputPin_o_Receive( void * )
		{
			CheckUpdateOutput();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
