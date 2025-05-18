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

// #define _SERIAL_GPS_DEBUG_INPUT_
// #define _SERIAL_GPS_DEBUG

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace GPSNumaTypes
	{
		enum TCommandState { csStart, /*csGPRMC, csGPGGA, csGPGSV,*/ csEndOfLine };
		enum TFields { fTime, fValidity, fLatitude, fNorthSouth, fLongitude, fEastWest, fSpeed, fCourse, fDate, fVariation, 
//				fLocalTime,
				fMode, // Optional
				fNavigationStatus,  // Optional
				fNumSatellites, fPrecision, fAltitude, fWGS84,
				fMessageNumber, fPRN, fElevation, fAzimuth, fSNR, fSkip, fChecksum };

		const TFields CCheckSum[] = { fChecksum };
		const TFields GPRMC[] = { fTime, fValidity, fLatitude, fLongitude, fSpeed, fCourse, fDate, fVariation, fMode, fNavigationStatus, fChecksum };
		const TFields GPGGA[] = 
		{
				fSkip, // Time
				fSkip, // Latitude
				fSkip, // Latitude N/S
				fSkip, // Longitude
				fSkip, // Longitude E/W
				fSkip, // GPS quality indicator
				fNumSatellites, 
				fPrecision, 
				fAltitude, 
				fSkip, // Meters  (Antenna height unit)
				fWGS84,
//				fSkip, // Meters
				fChecksum
		};

		const TFields GPGSV[] = 
		{
				fSkip, // Total number of messages of this type in this cycle
				fMessageNumber,
				fSkip, // Total number of SVs in view
				fPRN, // SV PRN number (Satellite number)
				fElevation, // Elevation in degrees, 90 maximum
				fAzimuth, // Azimuth, degrees from true north, 000 to 359
				fSNR, // SNR, 00-99 dB (null when not tracking)
				// Optional Signal ID
				fChecksum
		};

//		const TFields GPZDA[] =
//		{
//			fLocalTime,
//			fChecksum
//		};



	};
//---------------------------------------------------------------------------
	template <
		typename T_BaseYear,
		typename T_CourseOutputPins_Speed,
		typename T_CourseOutputPins_Course,
		typename T_DateTimeOutputPin,
		typename T_Enabled,
		typename T_FHour,
		typename T_FMilliSeconds,
		typename T_FMinutes,
		typename T_FSeconds,
		typename T_FState,
		typename T_FValuePopulated,
		typename T_HorizontalPrecisionOutputPin,
		typename T_InvalidOutputPin,
		typename T_LocationOutputPins_Latitude,
		typename T_LocationOutputPins_Longitude,
		typename T_LocationOutputPins_Altitude,
		typename T_LocationOutputPins_Height_Above_WGS84_Ellipsoid,
		typename T_LocationOutputPins_Magnetic_Variation,
		typename T_MessageErrorOutputPin,
		typename T_ModeOutputPin,
		typename T_NavigationStatusOutputPin,
		typename T_SatellitesOutputPins_Count,
		typename T_SatellitesOutputPins_Start_Scan,
		typename T_SatellitesOutputPins_Satellite_Number,
		typename T_SatellitesOutputPins_Elevation,
		typename T_SatellitesOutputPins_Azimuth,
		typename T_SatellitesOutputPins_Signal_Strength,
		typename T_SatellitesOutputPins_Satelite_Found
	> class MitovSerialGPS : 
		public T_BaseYear,
		public T_CourseOutputPins_Speed,
		public T_CourseOutputPins_Course,
		public T_DateTimeOutputPin,
		public T_Enabled,
		public T_FHour,
		public T_FMilliSeconds,
		public T_FMinutes,
		public T_FSeconds,
		public T_FState,
		public T_FValuePopulated,
		public T_HorizontalPrecisionOutputPin,
		public T_InvalidOutputPin,
		public T_LocationOutputPins_Latitude,
		public T_LocationOutputPins_Longitude,
		public T_LocationOutputPins_Altitude,
		public T_LocationOutputPins_Height_Above_WGS84_Ellipsoid,
		public T_LocationOutputPins_Magnetic_Variation,
		public T_MessageErrorOutputPin,
		public T_ModeOutputPin,
		public T_NavigationStatusOutputPin,
		public T_SatellitesOutputPins_Count,
		public T_SatellitesOutputPins_Start_Scan,
		public T_SatellitesOutputPins_Satellite_Number,
		public T_SatellitesOutputPins_Elevation,
		public T_SatellitesOutputPins_Azimuth,
		public T_SatellitesOutputPins_Signal_Strength,
		public T_SatellitesOutputPins_Satelite_Found
	{
	public:
		_V_PIN_( CourseOutputPins_Speed )
		_V_PIN_( CourseOutputPins_Course )
		_V_PIN_( DateTimeOutputPin )
		_V_PIN_( HorizontalPrecisionOutputPin )
		_V_PIN_( LocationOutputPins_Latitude )
		_V_PIN_( LocationOutputPins_Longitude )
		_V_PIN_( LocationOutputPins_Altitude )
		_V_PIN_( LocationOutputPins_Height_Above_WGS84_Ellipsoid )
		_V_PIN_( LocationOutputPins_Magnetic_Variation )
		_V_PIN_( MessageErrorOutputPin )
		_V_PIN_( ModeOutputPin )
		_V_PIN_( NavigationStatusOutputPin )
		_V_PIN_( SatellitesOutputPins_Count )
		_V_PIN_( SatellitesOutputPins_Start_Scan )
		_V_PIN_( SatellitesOutputPins_Satellite_Number )
		_V_PIN_( SatellitesOutputPins_Elevation )
		_V_PIN_( SatellitesOutputPins_Azimuth )
		_V_PIN_( SatellitesOutputPins_Signal_Strength )
		_V_PIN_( SatellitesOutputPins_Satelite_Found )

	public:
		_V_PROP_( BaseYear )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FValuePopulated )
		_V_PROP_( FState )

	protected:
		_V_PROP_( FHour )
		_V_PROP_( FMinutes )
		_V_PROP_( FSeconds )
		_V_PROP_( FMilliSeconds )

	protected:
		uint8_t	FIndex = 0;
//		char	*FBuffer = nullptr; //[110]; // Do not make less than 100!
		char	FDataBuffer[ 15 ];
		uint8_t FByteCheckSum = '$';
		const GPSNumaTypes::TFields	*FCurrentField = nullptr;

	public:
		void _DirectPinReceive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			for( int i = 0; i < ADataBlock.Size; ++i )
				ProcessChar( ADataBlock.Data[ i ] );
		}

		void InputPin_o_Receive( void *_Data )
		{	
			ProcessChar( *(char *)_Data );
//			for( char *AData = (char *)_Data; *AData; ++ AData )
//				ProcessChar( *AData );

//			ProcessChar( 13 );
		}

	protected:
		template <typename T> static inline void DebugLn( T AValue )
		{
#ifdef _SERIAL_GPS_DEBUG
			Serial.println( AValue );
#endif
		}

		template <typename T> static inline void Debug( T AValue )
		{
#ifdef _SERIAL_GPS_DEBUG
			Serial.print( AValue );
#endif
		}

	protected:
		void ProcessChar( char AChar )
		{
#ifdef _SERIAL_GPS_DEBUG_INPUT_
			Serial.print( AChar );
#endif
//			return;
//			Debug( AChar );
			bool AInChecksum = false;
			if( FCurrentField )
				if( *FCurrentField == GPSNumaTypes::fChecksum )
					if( FValuePopulated() )
						AInChecksum = true;

			if( ! AInChecksum )
			{
				if( ( AChar == 13 ) || ( AChar == 10 ) )
				{
					DebugLn( "<RESET>" );
					FState() = GPSNumaTypes::csStart;
					FCurrentField = nullptr;
					FIndex = 0;
					return;
				}

				if( AChar != '*' )
					FByteCheckSum ^= AChar;

			}

			if( AChar == '$' )
				FByteCheckSum = 0;

			if( FIndex >= sizeof( FDataBuffer ) )
			{
				Restart();
				return;
			}

			if( FCurrentField )
			{
				bool ATryNextField;
				do
				{
					ATryNextField = false;
					switch( *FCurrentField )
					{
						case GPSNumaTypes::fTime:
							if( ProcessTime( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

//						case GPSNumaTypes::fLocalTime:
//							if( ProcessLocalTime( AChar ) )
//								ATryNextField = NextFieldCheck( AChar );

//							break;

						case GPSNumaTypes::fValidity:
							if( ProcessWarning( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fLatitude:
							if( ProcessLatitude( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;
						
						case GPSNumaTypes::fLongitude:
							if( ProcessLongitude( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;
						
						case GPSNumaTypes::fSpeed:
							if( ProcessSpeed( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fCourse:
							if( ProcessCourse( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fDate:
							if( ProcessDate( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;				

						case GPSNumaTypes::fVariation:
							if( ProcessVariation( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;				

						case GPSNumaTypes::fMode:
							if( ProcessMode( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;				
												
						case GPSNumaTypes::fNavigationStatus:
							if( ProcessNavigationStatus( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;				

						case GPSNumaTypes::fChecksum:
							if( ProcessChecksum( AChar ) )
								Restart();

							break;			

						case GPSNumaTypes::fSkip:
							if( ProcessSkip( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fNumSatellites:
							if( ProcessNumSatellites( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fPrecision:
							if( ProcessPrecision( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fAltitude:
							if( ProcessAltitude( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fWGS84:
							if( ProcessWGS84( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fMessageNumber:
							if( ProcessMessageNumber( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fPRN:
							if( ProcessPRN_SatelliteNumber( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fElevation:
							if( ProcessElevation( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fAzimuth:
							if( ProcessAzimuth( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

						case GPSNumaTypes::fSNR:
							if( ProcessSNR_SignalStrength( AChar ) )
								ATryNextField = NextFieldCheck( AChar );

							break;

					}
				}
				while( ATryNextField );
				return;
			}

			switch( FState() )
			{
/*
				case GPSNumaTypes::csEndOfLine:
					if( ( AChar == 13 ) || ( AChar == 10 ) )
					{
						FState = GPSNumaTypes::csStart;
					}

					break;
*/
				case GPSNumaTypes::csStart:
					if( ( AChar == 13 ) || ( AChar == 10 ) )
						break;

					Debug( AChar );
					FDataBuffer[ FIndex ++ ] = AChar;
					if( FIndex == 7 )
					{
						DebugLn(" + " );
						FDataBuffer[ FIndex ] = 0;
						DebugLn( FDataBuffer );
						FIndex = 0;
						if( ( memcmp( FDataBuffer, "$GPRMC,", 7 ) == 0 ) || ( memcmp( FDataBuffer, "$GNRMC,", 7 ) == 0 ) )
						{
							DebugLn( " GPRMC " );
							FCurrentField = GPSNumaTypes::GPRMC;
						}

						else if( ( memcmp( FDataBuffer, "$GPGGA,", 7 ) == 0 ) || ( memcmp( FDataBuffer, "$GNGGA,", 7 ) == 0 ) )
						{
							DebugLn( " GPGGA " );
							FCurrentField = GPSNumaTypes::GPGGA;
						}
							
						else if( memcmp( FDataBuffer, "$GPGSV,", 7 ) == 0 )
						{
							DebugLn( " GPGSV " );
							FCurrentField = GPSNumaTypes::GPGSV;
						}

						else
							FState() = GPSNumaTypes::csEndOfLine;
					}

					break;
			}

/*
			if( AChar == 13 )
				return;

			if( AChar != 10 )
			{
				if( ! FBuffer )
					FBuffer = new char[ 110 ];

				FBuffer[ FIndex ++ ] = AChar;
//				Debug( AChar );
				if( FIndex < 109 )
					return;
			}

			FBuffer[ FIndex ] = '\0';
			DebugLn( FIndex );

			for( int i = 0; i < FIndex; ++i )
				Debug( FBuffer[ i ] );

			Debug( "" );
			Mitov::String AString = FBuffer;

			DebugLn( AString );
			delete [] FBuffer;
			FBuffer = nullptr;

			DebugLn( "---" );
//			delay( 100 );


			if( AString.startsWith( "$GPRMC" ))
			{
				if( ! ProcessGPRMC( AString ))
					InvalidOutputPin.SendValue( true );
			}
			else if( AString.startsWith( "$GPGGA" ))
				ProcessGPGGA( AString );

			else if( AString.startsWith( "$GPGSV" ))
				ProcessGPGSV( AString );

			FIndex = 0;
*/
		}

		void NextField()
		{
			DebugLn( "NextField" );

			FValuePopulated() = false;
			++FCurrentField;
			FIndex = 0;
//			FByteCheckSum = 0;
		}

		bool NextFieldCheck( char AChar )
		{
			NextField();
			// Check if we have switched to Checksum
			if( *FCurrentField == GPSNumaTypes::fChecksum )
				if( ProcessChecksum( AChar ) )
				{
					Restart();
					return false;
				}

			return ( AChar == '*' );
		}

/*
		void SetToChecksum( char AChar )
		{
			FCurrentField = GPSNumaTypes::CCheckSum;
			FValuePopulated() = false;

			FIndex = 0;
			if( ProcessChecksum( AChar ) )
				Restart();

//			FByteCheckSum = 0;
		}
*/
		void Restart()
		{
			FState() = GPSNumaTypes::csEndOfLine;
			FCurrentField = nullptr;
			FValuePopulated() = false;
			FIndex = 0;
			FByteCheckSum = '$';
		}

		bool ProcessSpeed( char AChar )
		{
			DebugLn( "ProcessSpeed" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				float ASpeedFloat = atof( FDataBuffer );
				DebugLn( ASpeedFloat );
				T_CourseOutputPins_Speed::SetPinValue( ASpeedFloat );
//				T_COURSE::SetSpeed( ASpeedFloat );
				return true;
			}

			if( ! ( isdigit( AChar ) || ( AChar == '.' ) || ( AChar == '-' )))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessCourse( char AChar )
		{
			DebugLn( "ProcessCourse" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				float ACourseFloat = atof( FDataBuffer );
				DebugLn( ACourseFloat );
				T_CourseOutputPins_Course::SetPinValue( ACourseFloat );
//				T_COURSE::SetCourse( ACourseFloat );
				return true;
			}

			if( ! ( isdigit( AChar ) || ( AChar == '.' ) || ( AChar == '-' )))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessDate( char AChar )
		{
			DebugLn( "ProcessDate" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
				for( ; FIndex < 6; ++ FIndex )
					FDataBuffer[ FIndex ] = '0';

				FDataBuffer[ FIndex ] = 0;
				DebugLn( FDataBuffer );

				int8_t ADay = Func::FromCharsToInt8( FDataBuffer, 2 );
				int8_t AMonth = Func::FromCharsToInt8( FDataBuffer + 2, 2 );
				int8_t AYear = Func::FromCharsToInt8( FDataBuffer + 4, 2 );

				TDateTime ADateTime;
				if( ADateTime.TryEncodeDateTime( BaseYear() + AYear, AMonth, ADay, FHour(), FMinutes(), FSeconds(), FMilliSeconds() ))
					T_DateTimeOutputPin::SetPinValue( ADateTime );
				
//				T_DATE_TIME::SetDate( ADay, AMonth, AYear );
				return true;
			}

			if( ! isdigit( AChar ))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ConvertCoordinateToFloat( float &AResult )
		{
//			Serial.println( "" );
//			Serial.println( "ConvertCoordinateToFloat" );
//			Serial.print( "FIndex = " ); Serial.println( FIndex );
			int APointPosition = 0;
			for( int i = 0; i < FIndex; ++i )
			{
//				Serial.println( FDataBuffer[ i ] );
				if( FDataBuffer[ i ] == '.' )
				{
					APointPosition = i;
					break;
				}

				if( ! isDigit( FDataBuffer[ i ] ) )
					return false;
			}

			if( APointPosition < 2 )
			{
				if( FDataBuffer[ 0 ] == '\0' )
					return false;

				AResult = atof( FDataBuffer ) / 60;
            }

			else
			{
//				Serial.println( FDataBuffer );
//				Serial.println( FDataBuffer + APointPosition );
				AResult = atof( FDataBuffer + APointPosition - 2 ) / 60;
//				Serial.println( AResult );
				FDataBuffer[ APointPosition - 2 ] = '\0';
				AResult += atoi( FDataBuffer );
			}


			return true;
		}

		bool ProcessLatitude( char AChar )
		{
			DebugLn( "ProcessLatitude" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
				if( FValuePopulated() )
				{
					DebugLn( "TEST DIRECTION" );

					float ALatitudeFloat;
					if( ConvertCoordinateToFloat( ALatitudeFloat ))
					{
						DebugLn( FDataBuffer[ FIndex - 1 ] );

						if( FDataBuffer[ FIndex - 1 ] == 'S' )
							ALatitudeFloat = -ALatitudeFloat;
						
						DebugLn( ALatitudeFloat );
						T_LocationOutputPins_Latitude::SetPinValue( ALatitudeFloat );
					}

					return true;
				}

				FValuePopulated() = true;

				FDataBuffer[ FIndex ] = '\0';
				return false;
			}

			if( ! FValuePopulated() )
				if( ! ( isdigit( AChar ) || ( AChar == '.' ) || ( AChar == '-' )))
				{
					Restart();
					return false;
				}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessLongitude( char AChar )
		{
			DebugLn( "ProcessLongitude" );
			DebugLn( AChar );
//			DebugLn( FValuePopulated() );

			if( AChar == ',' )
			{
				if( FValuePopulated() )
				{
					DebugLn( "TEST DIRECTION" );

					float ALongitudeFloat;
					if( ConvertCoordinateToFloat( ALongitudeFloat ))
					{
						DebugLn( FDataBuffer[ FIndex - 1 ] );

						if( FDataBuffer[ FIndex - 1 ] == 'W' )
							ALongitudeFloat = -ALongitudeFloat;
						
						DebugLn( ALongitudeFloat );
						T_LocationOutputPins_Longitude::SetPinValue( ALongitudeFloat );
					}

					return true;
				}

				FValuePopulated() = true;

				FDataBuffer[ FIndex ] = '\0';
				return false;
			}

			if( ! FValuePopulated() )
				if( ! ( isdigit( AChar ) || ( AChar == '.' )))
				{
					Restart();
					return false;
				}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

/*
		void SendOutputs()
		{
			DebugLn( "SEND OUTPUTS" );
		}
*/
		bool ProcessSkip( char AChar )
		{
			DebugLn( "ProcessSkip" );
			DebugLn( AChar );

			return  (( AChar == ',' ) || ( AChar == '*' ));
		}

		bool ProcessNumSatellites( char AChar )
		{
			DebugLn( "ProcessNumSatellites" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				uint8_t ANumSatellites = atoi( FDataBuffer );
				DebugLn( ANumSatellites );
				T_SatellitesOutputPins_Count::SetPinValue( ANumSatellites );
//				T_SATELITES::SetNumSatellites( ANumSatellites );
				return true;
			}

			if( ! isdigit( AChar ))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessPrecision( char AChar )
		{
			DebugLn( "ProcessPrecision" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				float ACourseFloat = atof( FDataBuffer );
				DebugLn( ACourseFloat );
				T_HorizontalPrecisionOutputPin::SetPinValue( ACourseFloat );
//				T_PRECISSION::SetHorizontalPrecision( ACourseFloat );
				return true;
			}

			if( ! ( isdigit( AChar ) || ( AChar == '.' ) || ( AChar == '-' )))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessAltitude( char AChar )
		{
			DebugLn( "ProcessPrecision" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				float ACourseFloat = atof( FDataBuffer );
				DebugLn( ACourseFloat );
				T_LocationOutputPins_Altitude::SetPinValue( ACourseFloat );
//				T_LOCATION::SetAltitude( ACourseFloat );
				return true;
			}

			if( ! ( isdigit( AChar ) || ( AChar == '.' ) || ( AChar == '-' )))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessWGS84( char AChar )
		{
			DebugLn( "ProcessWGS84" );
			DebugLn( AChar );

//			Serial.println( "ProcessWGS84" );
//			Serial.println( AChar );

			if( AChar == ',' )
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				float ACourseFloat = atof( FDataBuffer );
				DebugLn( ACourseFloat );
				T_LocationOutputPins_Height_Above_WGS84_Ellipsoid::SetPinValue( ACourseFloat );
//				T_LOCATION::SetWGS84( ACourseFloat );
				return true;
			}

			if( ! ( isdigit( AChar ) || ( AChar == '.' ) || ( AChar == '-' )))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessMessageNumber( char AChar )
		{
			DebugLn( "ProcessMessageNumber" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				DebugLn( FIndex );
//				FDataBuffer[ FIndex ] = '\0';
				if( FIndex == 1 )
					if( FDataBuffer[ 0 ] == '1' )
						T_SatellitesOutputPins_Start_Scan::ClockPin();
//						T_SATELITES::StartScan();


//				uint8_t ANumSatellites = atoi( FDataBuffer );
//				AMesageNo == "1"
//				DebugLn( ANumSatellites );
//				T_SATELITES::SetNumSatellites( ANumSatellites );
				return true;
			}

			if( ! isdigit( AChar ))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessPRN_SatelliteNumber( char AChar )
		{
			DebugLn( "ProcessPRN" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				uint8_t ASatelliteNumber = atoi( FDataBuffer );
				DebugLn( ASatelliteNumber );
				T_SatellitesOutputPins_Satellite_Number::SetPinValue( ASatelliteNumber );
//				T_SATELITES::SetSatelliteNumber( ASatelliteNumber );
				return true;
			}

			if( ! isdigit( AChar ))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessElevation( char AChar )
		{
			DebugLn( "ProcessElevation" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				uint8_t AElevation = atoi( FDataBuffer );
				DebugLn( AElevation );
				T_SatellitesOutputPins_Elevation::SetPinValue( AElevation );
//				T_SATELITES::SetElevation( AElevation );
				return true;
			}

			if( ! ( isdigit( AChar ) || ( AChar == '-' )))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessAzimuth( char AChar )
		{
			DebugLn( "ProcessAzimuth" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				uint8_t AAzimuth = atoi( FDataBuffer );
				DebugLn( AAzimuth );
				T_SatellitesOutputPins_Azimuth::SetPinValue( AAzimuth );
//				T_SATELITES::SetAzimuth( AAzimuth );
				return true;
			}

			if( ! isdigit( AChar ))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessSNR_SignalStrength( char AChar )
		{
			DebugLn( "ProcessSNR" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				uint8_t ASignalStrength = atoi( FDataBuffer );
				DebugLn( ASignalStrength );
				T_SatellitesOutputPins_Signal_Strength::SetPinValue( ASignalStrength );
				T_SatellitesOutputPins_Satelite_Found::ClockPin(); // Clock on last Item
//				T_SATELITES::SetSignalStrength( ASignalStrength );
				return true;
			}

			if( ! isdigit( AChar ))
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessVariation( char AChar )
		{
			DebugLn( "ProcessVariation" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				if( FValuePopulated() )
				{
					DebugLn( "TEST DIRECTION" );

					float AVariationFloat = atof( FDataBuffer );
					if( FDataBuffer[ FIndex - 1 ] == 'W' )
						AVariationFloat = -AVariationFloat;
						
					DebugLn( AVariationFloat );
					T_LocationOutputPins_Magnetic_Variation::SetPinValue( AVariationFloat );
//					T_LOCATION::SetVariation( AVariationFloat );

/*
					if( AChar == '*' )
					{
						SetToChecksum( AChar );
						return false;
					}
*/
					return true;
				}

				FValuePopulated() = true;

//				DebugLn( FIndex );
				FDataBuffer[ FIndex ] = '\0';

				return false;
			}

			if( ! FValuePopulated() )
				if( ! ( isdigit( AChar ) || ( AChar == '.' )))
				{
					Restart();
					return false;
				}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessChecksum( char AChar )
		{
			DebugLn( "ProcessChecksum" );
			DebugLn( AChar );

			if( ( AChar == 13 ) || ( AChar == 10 ) )
			{
				if( ! FValuePopulated() ) 
					return true;

				Debug( "FIndex: " );	DebugLn( FIndex );

				if( FIndex != 2 )
					return true;

//				FByteCheckSum ^= '*';
				Mitov::String AChecksumText( FByteCheckSum, HEX );
				AChecksumText.toUpperCase();
				while( AChecksumText.length() < 2 )
					AChecksumText = "0" + AChecksumText;

				DebugLn( AChecksumText );
				FDataBuffer[ FIndex ] = 0;
				DebugLn( FDataBuffer );

				T_MessageErrorOutputPin::SetPinValue( memcmp( AChecksumText.c_str(), FDataBuffer, 2 ) != 0 );
//				T_CRC_ERROR::SetCRCError( memcmp( AChecksumText.c_str(), FDataBuffer, 2 ) != 0 );
//					SendOutputs();

				return true;
			}

			if( ! FValuePopulated() ) 
			{
				if( AChar == '*' )
					FValuePopulated() = true;

				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

		bool ProcessNavigationStatus( char AChar )
		{
//			Serial.println( "ProcessNavigationStatus" );
			DebugLn( "ProcessNavigationStatus" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				if( ! FIndex )
					T_NavigationStatusOutputPin::SetPinValue( 0 );

				else
					switch( AChar )
					{
						case 'S':
							T_NavigationStatusOutputPin::SetPinValue( 1 );
							break;

						case 'C':
							T_NavigationStatusOutputPin::SetPinValue( 2 );
							break;

						case 'U':
							T_NavigationStatusOutputPin::SetPinValue( 3 );
							break;

						case 'V':
							T_NavigationStatusOutputPin::SetPinValue( 4 );
							break;

						default:
							T_NavigationStatusOutputPin::SetPinValue( 0 );

					}

/*
				if( AChar == '*' )
				{
					SetToChecksum( AChar );
					return false;
				}
*/
				return true;
			}

			FDataBuffer[ 0 ] = AChar;
			FIndex = 1;
			return false;
		}

		bool ProcessMode( char AChar )
		{
			DebugLn( "ProcessMode" );
			DebugLn( AChar );

			if(( AChar == ',' ) || ( AChar == '*' ))
			{
				if( ! FIndex )
					T_ModeOutputPin::SetPinValue( 0 );

				else
					switch( AChar )
					{
						case 'N':
							T_ModeOutputPin::SetPinValue( 1 );
							break;

						case 'A':
							T_ModeOutputPin::SetPinValue( 2 );
							break;

						case 'D':
							T_ModeOutputPin::SetPinValue( 3 );
							break;

						case 'E':
							T_ModeOutputPin::SetPinValue( 4 );
							break;

						case 'R':
							T_ModeOutputPin::SetPinValue( 5 );
							break;

						case 'F':
							T_ModeOutputPin::SetPinValue( 6 );
							break;

						case 'M':
							T_ModeOutputPin::SetPinValue( 7 );
							break;

						case 'S':
							T_ModeOutputPin::SetPinValue( 8 );
							break;

						default:
							T_ModeOutputPin::SetPinValue( 0 );
							break;

					}
						
/*
				if( AChar == '*' )
				{
					SetToChecksum( AChar );
					return false;
				}
*/
				return true;
			}

			FDataBuffer[ 0 ] = AChar;
			FIndex = 1;
			return false;
		}

		bool ProcessWarning( char AChar )
		{
			DebugLn( "ProcessWarning" );
			DebugLn( AChar );

			if( AChar == ',' )
				return true;

			T_InvalidOutputPin::SetPinValue( AChar != 'A' );

			return false;
		}

/*
		bool ProcessLocalTime( char AChar )
		{
			DebugLn( "ProcessLocalTime" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
			}

		}
*/
		bool ProcessTime( char AChar )
		{
			DebugLn( "ProcessTime" );
			DebugLn( AChar );

			if( AChar == ',' )
			{
				DebugLn( FIndex );
				if( FIndex < 6 )
				{
					Restart();
					return false;
				}

				for( ; FIndex < 9; ++ FIndex )
					FDataBuffer[ FIndex ] = '0';

				FDataBuffer[ FIndex ] = 0;
				DebugLn( FDataBuffer );

//				Serial.println( "" );
//				Serial.println( FDataBuffer );

//				if( FIndex == 6 )
//				if( FIndex != 6 )
//				{
//					Restart();
//					return false;
//				}

				FHour() = Func::FromCharsToInt8( FDataBuffer, 2 );
				FMinutes() = Func::FromCharsToInt8( FDataBuffer + 2, 2 );

//				Serial.println( Func::FromCharsToInt8( FDataBuffer + 4, 2 ));
				FSeconds() = Func::FromCharsToInt8( FDataBuffer + 4, 2 );
				FMilliSeconds() = Func::FromCharsToInt16( FDataBuffer + 6, 3 );
//				Serial.println( FSeconds().GetValue() );
//				T_DATE_TIME::SetTime( AHour, AMinute, ASecond, AMilli );
				return true;
			}

			if( FIndex == 6 )
				if( AChar == '.' )
					return false;
	
			if( ! isdigit( AChar ) )
			{
				Restart();
				return false;
			}

			FDataBuffer[ FIndex ++ ] = AChar; 
			return false;
		}

	public:
		inline MitovSerialGPS()
		{
			FValuePopulated() = false;
			FState() = GPSNumaTypes::csStart;
		}

	};
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif