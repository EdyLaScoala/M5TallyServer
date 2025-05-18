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
#include <Mitov_Display_ILI9341.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
//	TArduinoTFTLCDShieldTouchCoordinateMode = ( cmScreen, cmNormalized, cmRaw );
//---------------------------------------------------------------------------
	template <
		typename T_MaxRaw,
		typename T_MinRaw,
		typename T_Raw
	> class TArduinoTFTLCDShieldTouchCoordinate :
		public T_MaxRaw,
		public T_MinRaw,
		public T_Raw
	{
	public:
		_V_PROP_( Raw )
//		_V_PROP_( Mode )
		_V_PROP_( MinRaw )
		_V_PROP_( MaxRaw )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_MinPressure,
		typename T_PressureOutputPin,
		typename T_XCoordinate,
		typename T_XOutputPin,
		typename T_YCoordinate,
		typename T_YOutputPin
	> class TFTShieldTouchScreen :
		public T_XOutputPin,
		public T_YOutputPin,
		public T_PressureOutputPin,
		public T_MinPressure,
		public T_XCoordinate,
		public T_YCoordinate
	{
	protected:
		static const int NUMSAMPLES = 5;

	public:
		_V_PIN_( XOutputPin )
		_V_PIN_( YOutputPin )
		_V_PIN_( PressureOutputPin )

	public:
		_V_PROP_( MinPressure )
		_V_PROP_( XCoordinate )
		_V_PROP_( YCoordinate )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			const int _yp = A1;  // must be an analog pin, use "An" notation!
			const int _xm = A2;  // must be an analog pin, use "An" notation!
			const int _ym = 7;   // can be a digital pin
			const int _xp = 6;   // can be a digital pin

			int32_t x, y;
			int samples[NUMSAMPLES];
			uint8_t i;

			bool	valid = true;

//			delay( 10 );

			pinMode(_yp, INPUT);
			pinMode(_ym, INPUT);

//			delay( 1 );

			digitalWrite(_yp, LOW);
			digitalWrite(_ym, LOW);

			pinMode(_xp, OUTPUT);
			pinMode(_xm, OUTPUT);

			digitalWrite(_xp, HIGH);
			digitalWrite(_xm, LOW);

			for (i=0; i<NUMSAMPLES; i++)
				samples[i] = analogRead(_yp);
  
			Func::insert_sort(samples, NUMSAMPLES);
			if( abs( samples[ 1 ] - samples[ 3 ] ) > 10 )
				valid = false;

			x = (1023-samples[NUMSAMPLES/2]);

			pinMode(_xp, INPUT);
			pinMode(_xm, INPUT);
//		   *portOutputRegister(xp_port) &= ~xp_pin;
			digitalWrite(_xp, LOW);
   
			pinMode(_yp, OUTPUT);
//		   *portOutputRegister(yp_port) |= yp_pin;
			digitalWrite(_yp, HIGH);
			pinMode(_ym, OUTPUT);
  
			for (i=0; i<NUMSAMPLES; i++)
				samples[i] = analogRead(_xm);
		   
			Func::insert_sort(samples, NUMSAMPLES);
			if( abs( samples[ 1 ] - samples[ 3 ] ) > 10 )
				valid = false;

			y = (1023-samples[NUMSAMPLES/2]);

			// Set X+ to ground
			pinMode(_xp, OUTPUT);
//			*portOutputRegister(xp_port) &= ~xp_pin;
			digitalWrite(_xp, LOW);
  
			// Set Y- to VCC
//			*portOutputRegister(ym_port) |= ym_pin;
			digitalWrite(_ym, HIGH); 
  
			// Hi-Z X- and Y+
//			*portOutputRegister(yp_port) &= ~yp_pin;
			digitalWrite(_yp, LOW);
			pinMode(_yp, INPUT);
  
/*
			for (i=0; i<NUMSAMPLES; i++)
				samples[i] = analogRead(_xm);

			int z1 = samples[NUMSAMPLES/2]; //analogRead(_xm); 

			for (i=0; i<NUMSAMPLES; i++)
				samples[i] = analogRead(_yp);

			int z2 = samples[NUMSAMPLES/2];
*/
			int z1 = analogRead(_xm); 
			int z2 = analogRead(_yp);

/*
			float _rxplate = 300;
			if (_rxplate != 0) 
			{
				// now read the x 
				float rtouch;
				rtouch = z2;
				rtouch /= z1;
				rtouch -= 1;
				rtouch *= x;
				rtouch *= _rxplate;
				rtouch /= 1024;
     
				z = rtouch;
			} 
			else
*/
//			z = (1023-(z2-z1));


//			Serial.println( z );
			float z;
			if( ! valid )
				z = 0;

			else
			{
//				float ARange = 1023 - z2;
				z = (1023.0-(z2-z1)) / 1023.0;
			}

			if( z > MinPressure() )
			{
//				Serial.println( y );
				if( ! XCoordinate().Raw() )
				{
//					case cmNormalized:
//						x = MapRange( x, XCoordinate.MinRaw, XCoordinate.MaxRaw, 0.0f, 1.0f ); break;

//					case cmScreen:
					{
						x = Func::MapRange<float>( MitovMin<float>( XCoordinate().MaxRaw(), MitovMax<float>( XCoordinate().MinRaw(), x )), YCoordinate().MinRaw(), YCoordinate().MaxRaw(), 0, C_OWNER.height() );
//						y = Func::MapRange<float>( MitovMin( YCoordinate.MaxRaw, MitovMax( YCoordinate.MinRaw, y )), YCoordinate.MinRaw, YCoordinate.MaxRaw, 0, FOwner.height() );
						switch( C_OWNER.Orientation() )
						{
							case goDown :
							case goLeft :
								x = C_OWNER.height() - x;
						}
					}
				}

				if( ! YCoordinate().Raw() )
				{
//					case cmNormalized:
//						y = MapRange( y, YCoordinate.MinRaw, YCoordinate.MaxRaw, 0.0f, 1.0f ); break;

//					case cmScreen:
					{
						y = Func::MapRange<float>( MitovMin<float>( YCoordinate().MaxRaw(), MitovMax<float>( YCoordinate().MinRaw(), y )), XCoordinate().MinRaw(), XCoordinate().MaxRaw(), 0, C_OWNER.width() );
						switch( C_OWNER.Orientation() )
						{
							case goDown :
							case goLeft :
								y = C_OWNER.width() - y;
						}
					}
				}

				switch( C_OWNER.Orientation() )
				{
					case goLeft :
					case goRight :
						T_XOutputPin::SetPinValue( x );
						T_YOutputPin::SetPinValue( y );
						break;

					case goUp :
					case goDown :
						T_XOutputPin::SetPinValue( y );
						T_YOutputPin::SetPinValue( x );
						break;
				}
			}

			T_PressureOutputPin::SetPinValue( z );
			pinMode(_xp, OUTPUT);
			pinMode(_xm, OUTPUT);

			pinMode(_yp, OUTPUT);
			pinMode(_xm, OUTPUT);

//			delay( 10 );

//			Serial.println( z );
		}

	public:
		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif