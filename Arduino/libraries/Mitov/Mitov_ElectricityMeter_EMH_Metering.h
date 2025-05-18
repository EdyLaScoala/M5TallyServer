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
	namespace EMHMeteringConsts
	{
		const uint8_t START_SEQUENCE[] = { 0x1B, 0x1B, 0x1B, 0x1B, 0x01, 0x01, 0x01, 0x01 };
		const uint8_t END_SEQUENCE[] = { 0x1B, 0x1B, 0x1B, 0x1B, 0x1A};

//		const uint8_t POWER_IN[] =       { 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x00, 0xFF };
//		const uint8_t POWER_OUT[] =      { 0x77, 0x07, 0x01, 0x00, 0x02, 0x08, 0x00, 0xFF };
//		const uint8_t POWER_CURRTENT[] = { 0x77, 0x07, 0x01, 0x00, 0x10, 0x07, 0x00, 0xFF };

//		const uint8_t CTotal_Consumption[] = { 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x00, 0xFF };

//		const uint8_t CConsumption_Rate1[] = { 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x01, 0xFF };
//		const uint8_t CConsumption_Rate2[] = { 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x02, 0xFF };

//		const uint8_t CTotal_Delivery[] =	 { 0x77, 0x07, 0x01, 0x00, 0x02, 0x08, 0x00, 0xFF };

//		const uint8_t CCurrent_Power[] =	 { 0x77, 0x07, 0x01, 0x00, 0x0F, 0x07, 0x00, 0xFF };

		const uint8_t CTotal_Consumption[] = { 0x01, 0x08, 0x00, 0xFF };

		const uint8_t CConsumption_Rate1[] = { 0x01, 0x08, 0x01, 0xFF };
		const uint8_t CConsumption_Rate2[] = { 0x01, 0x08, 0x02, 0xFF };

		const uint8_t CTotal_Delivery[] =	 { 0x02, 0x08, 0x00, 0xFF };

//		const uint8_t CCurrent_Power[] =	 { 0x0F, 0x07, 0x00, 0xFF };
		const uint8_t CCurrent_Power[] =	 { 0x10, 0x07, 0x00, 0xFF };

		const uint8_t CSerialNumber[] =		 { 0x00, 0x00, 0x09, 0xFF };

//														 01   00   01   08   00   FF
	}
//---------------------------------------------------------------------------
	template <
		typename T_Rate1OutputPin,
		typename T_Rate2OutputPin,
		typename T_TotalOutputPin
	> class TArduinoEMHMeteringElectricityMeterConsumtion :
		public T_Rate1OutputPin,
		public T_Rate2OutputPin,
		public T_TotalOutputPin
	{
	public:
		_V_PIN_( Rate1OutputPin )
		_V_PIN_( Rate2OutputPin )
		_V_PIN_( TotalOutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Consumtion,
		typename T_CurrentPowerOutputPin,
		typename T_ErrorOutputPin,
		typename T_SerialNumberOutputPin,
		typename T_TotalDeliveryOutputPin
	> class ElectricityMeter_EMH_Metering :
		public T_SerialNumberOutputPin,
		public T_TotalDeliveryOutputPin,
		public T_ErrorOutputPin,
		public T_CurrentPowerOutputPin,
		public T_Consumtion
	{
	public:
		_V_PIN_( SerialNumberOutputPin )
		_V_PIN_( TotalDeliveryOutputPin )
		_V_PIN_( CurrentPowerOutputPin )
		_V_PIN_( Consumtion )
		_V_PIN_( ErrorOutputPin )

	protected:
		static const uint16_t	BUFFER_SIZE = 3840; // Max datagram duration 400ms at 9600 Baud
//		static const uint32_t	CRC_FLAG_REFLECT           = (((1<<31) | (0<<30)) | ((1<<29) | (0<<28))); //Reflect in- and outgoing bytes (refin=true refout=true)
//		static const uint32_t	CRC_FLAG_XOR               = (1<<26);                                     //Perform XOR on result

//		static const uint32_t	POLY = 0x1021;

	protected:
//		enum TMeteringState { msStart, msReadMessage, msReadMessageEnd, msReadChecksum, msProcessMessage };
		enum TMeteringState { msStart, msReadMessage };

	protected:
		uint8_t			FBuffer[ BUFFER_SIZE ];
//		uint16_t		FTemplateIndex = 0;
		uint16_t		FPosition = 0;
		TMeteringState	FState = msStart;

	protected:
//		uint16_t x25( const uint8_t *data, const uint16_t datalen )
//		{
		  // poly=0x1021 init=0xffff refin=true refout=true xorout=0xffff check=0x906e
//		  return generic( 0x1021, 0XFFFF, CRC_FLAG_REFLECT | CRC_FLAG_XOR, data, datalen );
//		}

/*
		inline int16_t CRC16processByte( uint8_t data )
		{
			crc = crc ^ ((uint16_t)data << 8);
			for (uint8_t  i = 0; i < 8; i++)
			{
				if (crc & 0x8000)
					crc = (crc << 1) ^ POLY;

				else
					crc <<= 1;

			}

			return crc;
		}
*/
/*
		uint16_t CRC16ProcessBuffer( const char *data_p, uint16_t length )
		{
			uint16_t crc = 0xFFFF;

			while( length-- )
			{
//				processByte(*data_p++);

				crc = crc ^ ( uint16_t( *data_p++ ) << 8 );
				for( uint8_t  i = 0; i < 8; ++i )
				{
					if (crc & 0x8000)
						crc = (crc << 1) ^ POLY;

					else
						crc <<= 1;

				}
			}

			crc ^= 0xFFFF;

			return crc;
		}
*/
//		uint16_t ax25crc16(unsigned char *data_p, uint16_t lenght)
		uint16_t CRC16ProcessBuffer( unsigned char *data_p, uint16_t lenght )
		{
			uint16_t crc = 0xFFFF;
//			uint32_t data;
			uint16_t crc16_table[] =
			{
					0x0000, 0x1081, 0x2102, 0x3183,
					0x4204, 0x5285, 0x6306, 0x7387,
					0x8408, 0x9489, 0xa50a, 0xb58b,
					0xc60c, 0xd68d, 0xe70e, 0xf78f
			};

			while( lenght -- )
			{
				crc = ( crc >> 4 ) ^ crc16_table[(crc & 0xf) ^ (*data_p & 0xf)];
				crc = ( crc >> 4 ) ^ crc16_table[(crc & 0xf) ^ (*data_p++ >> 4)];
			}

			uint32_t data = crc;
			crc = (crc << 8) | (data >> 8 & 0xff); // do byte swap here that is needed by AX25 standard
			return (~crc);
		}

		bool ProcessValue( uint16_t &AIndex, float &AFloatValue )
		{
//			cp = (uint8_t *)(found_at) + pattern_size;
//			AIndex += pattern_size;

			// Ingore status byte
//			cp += (*cp & 0x0f);
			AIndex += ( FBuffer[ AIndex ] & 0x0f );

			// Ignore time byte
//			cp += (*cp & 0x0f);
			AIndex += ( FBuffer[ AIndex ] & 0x0f );

			// Save unit
//			len = *cp & 0x0f;
//			values[i].unit = *(cp + 1);
//			cp += len;

			uint8_t len = ( FBuffer[ AIndex ] & 0x0f );
			uint8_t AUnit = FBuffer[ AIndex + 1 ];
			AIndex += len;

//			Serial.println( "len" );
//			Serial.println( len );

			// Save scaler
//			len = *cp & 0x0f;
//			values[i].scaler = *(cp + 1);
//			cp += len;

			len = ( FBuffer[ AIndex ] & 0x0f );
			int8_t AScaler = int8_t ( FBuffer[ AIndex + 1 ] );
			AIndex += len;

//			Serial.println( "len" );
//			Serial.println( len );

			// Save value
//			type = *cp & 0x70;
//			len = *cp & 0x0f;
//			cp++;

			uint8_t type = ( FBuffer[ AIndex ] & 0x70 );
			len = ( FBuffer[ AIndex ++ ] & 0x0f );

//			Serial.println( "len" );
//			Serial.println( len );

			uint64_t uvalue = 0;
			uint8_t nlen = len;
			while( --nlen )
			{
				uvalue <<= 8;
//				uvalue |= *cp++;
				uvalue |= FBuffer[ AIndex ++ ];
			}

//			values[i].value = (type == 0x50 ? (int64_t)uvalue : uvalue);

//			Serial.println( "VALUES" );
//			Serial.println( type, HEX );
//			Serial.println( (uint32_t)uvalue );
//			Serial.println( AScaler );

			if( type == 0x50 )
				AFloatValue = (( int64_t( int32_t( uvalue )) * ( pow(10, AScaler))) );

			else
				AFloatValue = (( uvalue * ( pow(10, AScaler))) );

//			Serial.println( "RESULT" );
//			Serial.println( AFloatValue );
			return true;
		}

	public:
		inline void _DirectPinReceive( void *_Data )
		{
			Mitov::TDataBlock &AData = *(Mitov::TDataBlock *)_Data;

			for( int j = 0; j < AData.Size; ++j )
			{
				uint8_t AValue = AData.Data[ j ];

//				Serial.println( AValue, HEX );

				switch( FState )
				{
					case msStart:
						if( AValue != EMHMeteringConsts::START_SEQUENCE[ FPosition ] )
						{
							ErrorOutputPin().ClockPin();
//							Serial.println( "ERROR1" );
							FPosition = 0;
							break;
						}

//						Serial.println( FPosition );

						FBuffer[ FPosition ++ ] = AValue;
						if( FPosition == sizeof( EMHMeteringConsts::START_SEQUENCE ))
							FState = msReadMessage;

						break;

					case msReadMessage:
//						Serial.println( FPosition );
//						Serial.println( "msReadMessage" );
	//					if ( FPosition >= BUFFER_SIZE - 3 )
						if ( FPosition >= BUFFER_SIZE - 1 )
						{
							ErrorOutputPin().ClockPin();
							FState = msStart;
//							Serial.println( "ERROR2" );
							FPosition = 0;
							break;
						}

						FBuffer[ FPosition ++ ] = AValue;
						if( FPosition > sizeof( EMHMeteringConsts::END_SEQUENCE ) + sizeof( EMHMeteringConsts::START_SEQUENCE ) + 3 )
						{
							if( memcmp( FBuffer + FPosition - sizeof( EMHMeteringConsts::START_SEQUENCE ) - 1, EMHMeteringConsts::START_SEQUENCE, sizeof( EMHMeteringConsts::START_SEQUENCE ) ) == 0 )
							{
								ErrorOutputPin().ClockPin();
//								Serial.println( "START_SEQUENCE 11111111" );
								FPosition = sizeof( EMHMeteringConsts::START_SEQUENCE ) + 1;
								break;
							}

							if( memcmp( FBuffer + FPosition - sizeof( EMHMeteringConsts::END_SEQUENCE ) - 3, EMHMeteringConsts::END_SEQUENCE, sizeof( EMHMeteringConsts::END_SEQUENCE ) ) == 0 )
							{

/*
								Serial.println( "END_SEQUENCE" );
								for( int32_t ATest = 0; ATest < FPosition; ++ATest )
								{
//									Serial.print( "0x" );
									if( FBuffer[ ATest ] < 0x10 )
										Serial.print( "0" );

									Serial.print( FBuffer[ ATest ], HEX );
									Serial.print( " " );
								}

								Serial.println();
*/
								uint16_t ACheckSum1 = CRC16ProcessBuffer( FBuffer, FPosition - 2 );
//								uint16_t ACheckSum2 = ( uint16_t( FBuffer[ FPosition - 1 ] ) << 8 ) | FBuffer[ FPosition - 2 ];
								uint16_t ACheckSum2 = ( uint16_t( FBuffer[ FPosition - 2 ] ) << 8 ) | FBuffer[ FPosition - 1 ];

//									Serial.println( ACheckSum1, HEX );
//									Serial.println( ACheckSum2, HEX );
//									uint16_t ACheckSum3 = CRC16ProcessBuffer( FBuffer, FPosition - 2 );
//									Serial.println( ACheckSum3, HEX );

								if( ACheckSum1 != ACheckSum2 )
								{
									FState = msStart;
									ErrorOutputPin().ClockPin();
//									Serial.println( "ERROR3" );
									FPosition = 0;
									break;
								}

								for( int16_t i = 0; i < FPosition - sizeof( EMHMeteringConsts::END_SEQUENCE ) - 3 - 4; ++i )
								{
									if( FBuffer[ i ] != 0x77 )
										continue;

									if( FBuffer[ i + 1 ] != 0x07 )
										continue;

									if( FBuffer[ i + 2 ] != 0x01 )
										continue;

									if( FBuffer[ i + 3 ] != 0x00 )
										continue;

									if( memcmp( FBuffer + i + 4, EMHMeteringConsts::CTotal_Consumption, sizeof( EMHMeteringConsts::CTotal_Consumption )) == 0 )
									{
										float AFloatValue;
										uint16_t AIndex = i + 4 + sizeof( EMHMeteringConsts::CTotal_Consumption );
										if( ProcessValue( AIndex, AFloatValue ))
										{
											i = AIndex - 1;
											Consumtion().TotalOutputPin().SetPinValue( AFloatValue );
										}
									}

									if( memcmp( FBuffer + i + 4, EMHMeteringConsts::CConsumption_Rate1, sizeof( EMHMeteringConsts::CConsumption_Rate1 )) == 0 )
									{
										float AFloatValue;
										uint16_t AIndex = i + 4 + sizeof( EMHMeteringConsts::CConsumption_Rate1 );
										if( ProcessValue( AIndex, AFloatValue ))
										{
											i = AIndex - 1;
											Consumtion().Rate1OutputPin().SetPinValue( AFloatValue );
										}
									}

									if( memcmp( FBuffer + i + 4, EMHMeteringConsts::CConsumption_Rate2, sizeof( EMHMeteringConsts::CConsumption_Rate2 )) == 0 )
									{
										float AFloatValue;
										uint16_t AIndex = i + 4 + sizeof( EMHMeteringConsts::CConsumption_Rate2 );
										if( ProcessValue( AIndex, AFloatValue ))
										{
											i = AIndex - 1;
											Consumtion().Rate2OutputPin().SetPinValue( AFloatValue );
										}
									}

									if( memcmp( FBuffer + i + 4, EMHMeteringConsts::CTotal_Delivery, sizeof( EMHMeteringConsts::CTotal_Delivery )) == 0 )
									{
										float AFloatValue;
										uint16_t AIndex = i + 4 + sizeof( EMHMeteringConsts::CTotal_Delivery );
										if( ProcessValue( AIndex, AFloatValue ))
										{
											i = AIndex - 1;
											TotalDeliveryOutputPin().SetPinValue( AFloatValue );
										}
									}

									if( memcmp( FBuffer + i + 4, EMHMeteringConsts::CCurrent_Power, sizeof( EMHMeteringConsts::CCurrent_Power )) == 0 )
									{
//										Serial.println( "TEST1" );
										float AFloatValue;
										uint16_t AIndex = i + 4 + sizeof( EMHMeteringConsts::CCurrent_Power );
										if( ProcessValue( AIndex, AFloatValue ))
										{
											i = AIndex - 1;
											CurrentPowerOutputPin().SetPinValue( AFloatValue );
										}
									}

									if( memcmp( FBuffer + i + 4, EMHMeteringConsts::CSerialNumber, sizeof( EMHMeteringConsts::CSerialNumber )) == 0 )
									{
										TDataBlock AData( 10, FBuffer + i + 5 + sizeof( EMHMeteringConsts::CSerialNumber ) + 4 );
										SerialNumberOutputPin().SetPinValue( AData );
/*
										Serial.println( "SERIAL" );
										for( uint8_t x = 0; x < 10; ++x )
										{
											Serial.print( "-" );

											uint8_t AValue = FBuffer[ i + x + 5 + sizeof( EMHMeteringConsts::CSerialNumber ) + 4 ];

											if( AValue < 0x10 )
												Serial.print( "0" );

											Serial.print( AValue, HEX );
											Serial.print( " " );

										}

										Serial.println();
*/
/*
										float AFloatValue;
										uint16_t AIndex = i + 4 + sizeof( EMHMeteringConsts::CSerialNumber );
										if( ProcessValue( AIndex, AFloatValue ))
										{
											i = AIndex - 1;
											CurrentPowerOutputPin().SetPinValue( AFloatValue );
										}
*/
									}


	//								0x77, 0x07, 0x01, 0x00

								}

	//							FState = msReadChecksum;
								FPosition = 0;
								FState = msStart;
//								Serial.println( "RESTART" );
							}

	/*
							if( FBuffer[ FPosition  == *EMHMeteringConsts::END_SEQUENCE )
							{
								FTemplateIndex = 0;
								FState = msReadMessageEnd;
							}
	*/
						}

						break;

	/*
					case msReadMessageEnd:
						if ( FPosition >= BUFFER_SIZE - 3 )
						{
							FPosition = 0;
							break;
						}

						FBuffer[ FPosition ++ ] = AValue;
						if( AValue != EMHMeteringConsts::END_SEQUENCE[ FTemplateIndex ++ ] )
							FState = msReadMessage;

						break;

					case msReadChecksum:
						break;
	*/

	//				case msProcessMessage:
	//					break;

				}
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
