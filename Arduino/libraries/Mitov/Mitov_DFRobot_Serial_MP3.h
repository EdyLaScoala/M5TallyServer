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
//#include <DFRobotDFPlayerMini.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	enum TArduinoDFRobotMP3PlayerEqualizerMode { emNormal, emPop, emRock, emJazz, emClassic, emBass };
//---------------------------------------------------------------------------
	enum TArduinoDFRobotMP3PlayerOutputDevice { odUDisk, odSD, odAux, odSleep, odFlash };
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Value
	> class TArduinoDFRobotMP3PlayerGain :
		public T_Enabled,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_CardInsertedOutputPin,
		typename T_CardOnlineOutputPin,
		typename T_CardRemovedOutputPin,
		typename T_EnableDAC,
		typename T_Enabled,
		typename T_EqualizerMode,
		typename T_ErrorOutputPin,
//		typename T_FIsAvailable,
		typename T_FIsSending,
		typename T_FProcessed,
		typename T_Gain,
		typename T_Loop,
		typename T_OutputDevice,
		typename T_TimeOutPeriod,
		typename T_TimeoutErrorOutputPin,
		typename T_Volume
	> class MitovDFRobotSerialMP3 :
		public T_CardInsertedOutputPin,
		public T_CardOnlineOutputPin,
		public T_CardRemovedOutputPin,
		public T_EnableDAC,
		public T_Enabled,
		public T_EqualizerMode,
		public T_ErrorOutputPin,
//		public T_FIsAvailable,
		public T_FIsSending,
		public T_FProcessed,
		public T_Gain,
		public T_Loop,
		public T_OutputDevice,
		public T_TimeOutPeriod,
		public T_TimeoutErrorOutputPin,
		public T_Volume
	{
	public:
		_V_PIN_( CardInsertedOutputPin )
		_V_PIN_( CardRemovedOutputPin )
		_V_PIN_( CardOnlineOutputPin )
		_V_PIN_( TimeoutErrorOutputPin )
		_V_PIN_( ErrorOutputPin )

	protected:
		static const uint8_t MP3Busy = 1;
		static const uint8_t MP3Sleeping = 2;
		static const uint8_t MP3SerialWrongStack = 3;
		static const uint8_t MP3CheckSumNotMatch = 4;
		static const uint8_t MP3FileIndexOut = 5;
		static const uint8_t MP3FileMismatch = 6;
		static const uint8_t MP3Advertise = 7;

		static const uint8_t Stack_Header = 0;
		static const uint8_t Stack_Version = 1;
		static const uint8_t Stack_Length = 2;
		static const uint8_t Stack_Command = 3;
		static const uint8_t Stack_ACK = 4;
		static const uint8_t Stack_Parameter = 5;
		static const uint8_t Stack_CheckSum = 7;
		static const uint8_t Stack_End = 9;

	public:
		_V_PROP_( Volume )
		_V_PROP_( TimeOutPeriod )
		_V_PROP_( Loop )
		_V_PROP_( EnableDAC )
		_V_PROP_( EqualizerMode )
		_V_PROP_( OutputDevice )
		_V_PROP_( Gain )

	public:
		inline void UpdateVolume()
		{
			SetIntVolume( Volume() * 30 + 0.5 );
		}

		void UpdateLoop()
		{
			EnableLoop( ( Loop().GetValue() ) ? 0x01 : 0x00 );
		}

		void UpdateEnableDAC()
		{
			SendCommand( 0x1A, ( EnableDAC().GetValue() ) ? 0x00 : 0x01 );
		}

		inline void UpdateEqualizerMode()
		{
			SendCommand( 0x07, EqualizerMode().GetValue() );
		}

		void UpdateOutputDevice()
		{
			SendCommand( 0x09, OutputDevice().GetValue() );
			delay(200);
		}

		void UpdateGain()
		{
			SendCommand( 0x10, ( Gain().Enabled() ) ? 1 : 0, Gain().Value() );
		}

	protected:
		uint8_t FReceiveBuffer[ 10 ];
		uint8_t FSendBuffer[ 10 ] = {0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF};
		unsigned long FTimeOutTimer;
		_V_PROP_( FIsSending )
//		_V_PROP_( FIsAvailable )
		_V_PROP_( FProcessed )
		uint8_t FReceivedIndex = 0;
		uint16_t FHandleParameter = 0;

	protected:
		inline uint8_t	ReadByte()
		{
			uint8_t	AResult = C_SERIAL.GetStream().read();
//			Serial.println( AResult, HEX );
			return AResult;
		}

		void uint16ToArray(uint16_t value, uint8_t *array)
		{
			*array = (uint8_t)(value>>8);
			*(array+1) = (uint8_t)(value);
		}

		void SendBuffer()
		{
			while ( FIsSending() ) 
				ProcessInput();

#ifdef _DFROBOT_SERIAL_MP3_DEBUG
			Serial.println();
			Serial.print(F("sending:"));
			for (int i=0; i<sizeof( FSendBuffer ); i++) 
			{
				Serial.print( FSendBuffer[i],HEX);
				Serial.print(F(" "));
			}
			Serial.println();
#endif
			C_SERIAL.GetStream().write( FSendBuffer, sizeof( FSendBuffer ));
			FTimeOutTimer = millis();
			FIsSending() = true; //_sending[Stack_ACK];
		}

		uint16_t CalculateCheckSum(uint8_t *buffer)
		{
			uint16_t sum = 0;
			for (int i=Stack_Version; i<Stack_CheckSum; i++)
				sum += buffer[i];
  
			return -sum;
		}

		void ProcessInput()
		{
			while( C_SERIAL.GetStream().available()) 
			{
				if ( FReceivedIndex == 0) 
				{
					FReceiveBuffer[Stack_Header] = ReadByte();
#ifdef _DFROBOT_SERIAL_MP3_DEBUG
					Serial.print(F("received:"));
					Serial.print(FReceiveBuffer[FReceivedIndex],HEX);
					Serial.print(F(" "));
#endif
					if (FReceiveBuffer[Stack_Header] == 0x7E) 
					{
//						FIsAvailable() = false;
						FProcessed() = false;
						FReceivedIndex ++;
					}
				}
				else
				{
					FReceiveBuffer[FReceivedIndex] = ReadByte();
#ifdef _DFROBOT_SERIAL_MP3_DEBUG
					Serial.print(FReceiveBuffer[FReceivedIndex],HEX);
					Serial.print(F(" "));
#endif
					switch( FReceivedIndex )
					{
						case Stack_Version:
							if (FReceiveBuffer[FReceivedIndex] != 0xFF) 
								ErrorWrongBuffer();

							break;

						case Stack_Length:
							if (FReceiveBuffer[FReceivedIndex] != 0x06) 
								ErrorWrongBuffer();

							break;

						case Stack_End:
#ifdef _DFROBOT_SERIAL_MP3_DEBUG
							Serial.println();
#endif
							if (FReceiveBuffer[FReceivedIndex] != 0xEF)
							{
								ErrorWrongBuffer();
								return;
							}
							else
							{
								if ( ValidateStack()) 
								{
									FReceivedIndex = 0;
									ParseStack();
									FProcessed() = true;
//									if ( FIsAvailable() && !FSendBuffer[Stack_ACK]) 
//										FIsSending() = false;

									return; // _isAvailable;
								}
								else
								{
									ErrorWrongBuffer();
									return;
								}
							}
							break;

					}
					FReceivedIndex++;
				}
			}
  
			if ( FIsSending() && (millis() - FTimeOutTimer >= TimeOutPeriod() )) 
			{
				ErrorTimeout();
				FProcessed() = true;
			}
  
//			return _isAvailable;
		}


		void ParseStack()
		{
			uint8_t		AHandleCommand = *(FReceiveBuffer + Stack_Command);
			FHandleParameter =  ArrayToUint16(FReceiveBuffer + Stack_Parameter);

//			FIsAvailable() = true;
			switch (AHandleCommand) 
			{
				case 0x3D:
					PlayFinished( FHandleParameter );
//					handleMessage(DFPlayerPlayFinished, FHandleParameter);
					break;

				case 0x3F:
					if ( FHandleParameter & 0x02) 
						PlayerCardOnline();
//						handleMessage(DFPlayerCardOnline, FHandleParameter);

					break;

				case 0x3A:
					if ( FHandleParameter & 0x02)
						PlayerCardInserted();
//						handleMessage(DFPlayerCardInserted, FHandleParameter);

					break;

				case 0x3B:
					if ( FHandleParameter & 0x02)
						PlayerCardRemoved();
//						handleMessage(DFPlayerCardRemoved, FHandleParameter);

					break;

				case 0x40:
					HandleError( FHandleParameter );
//					handleMessage(DFPlayerError, FHandleParameter);
					break;

				case 0x41:
					FIsSending() = false;
					break;

				case 0x3C:
				case 0x3E:
				case 0x42:
				case 0x43:
				case 0x44:
				case 0x45:
				case 0x46:
				case 0x47:
				case 0x48:
				case 0x49:
				case 0x4B:
				case 0x4C:
				case 0x4D:
				case 0x4E:
				case 0x4F:
//					FIsAvailable() = true;
					break;

				default:
					ErrorWrongBuffer();
			}
		}

		void HandleError( uint16_t AError )
		{
			FIsSending() = false;

			T_ErrorOutputPin::SetPinValue( AError );
		}

		void PlayFinished( uint16_t ANumber )
		{
		}

		void ErrorWrongBuffer()
		{
			T_ErrorOutputPin::SetPinValue( MP3SerialWrongStack );
		}

		void ErrorTimeout()
		{
			FIsSending() = false;
			T_TimeoutErrorOutputPin::ClockPin();
		}

		inline void PlayerCardOnline()
		{
			T_CardOnlineOutputPin::ClockPin();
		}

		inline void PlayerCardInserted()
		{
			T_CardInsertedOutputPin::ClockPin();
		}

		inline void PlayerCardRemoved()
		{
			T_CardRemovedOutputPin::ClockPin();
		}

		bool ValidateStack()
		{
			return CalculateCheckSum( FReceiveBuffer ) == ArrayToUint16( FReceiveBuffer + Stack_CheckSum );
		}

		uint16_t ArrayToUint16(uint8_t *array)
		{
			uint16_t value = *array;
			value <<=8;
			value += *(array+1);
			return value;
		}

	public:
		void SendCommand(uint8_t command, uint16_t argument)
		{
			FSendBuffer[Stack_Command] = command;
			uint16ToArray(argument, FSendBuffer + Stack_Parameter);
			uint16ToArray( CalculateCheckSum( FSendBuffer ), FSendBuffer + Stack_CheckSum );
			SendBuffer();
		}

		void SendCommand(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow)
		{
			uint16_t buffer = argumentHigh;
			buffer <<= 8;
			SendCommand(command, buffer | argumentLow);
		}

		void Play( int fileNumber )
		{
			SendCommand( 0x03, fileNumber );
//			if( Loop )
//				EnableLoop( 0x01 );

//			else
//				EnableLoop( 0x00 );

		}

		inline void LoopFile( int fileNumber )
		{
			SendCommand( 0x08, fileNumber );
		}

		inline void Next()
		{
			SendCommand( 0x01, 0 );
		}

		inline void Previous()
		{
			SendCommand( 0x02, 0 );
		}

		inline void PlayFolder(uint8_t folderNumber, uint8_t fileNumber)
		{
			SendCommand(0x0F, folderNumber, fileNumber);
		}

		inline void PlayLargeFolder(uint8_t folderNumber, uint16_t fileNumber)
		{
			SendCommand(0x14, (((uint16_t)folderNumber) << 12) | fileNumber);
		}

/*
		inline void Sleep()
		{
			SendCommand( 0x0A, 0 );
		}
*/
		inline void Start()
		{
			SendCommand( 0x0A, 0 );
		}

		uint32_t	GetFilesCount()
		{
			switch( OutputDevice() ) 
			{
				case odUDisk:
					SendCommand( 0x47, 0 );
					break;

				case odSD:
					SendCommand( 0x48, 0 );
					break;

				case odFlash:
					SendCommand( 0x49, 0 );
					break;

				default:
					return 0;
			}

			return GetResult();
		}

		uint32_t	GetFilesCountInFolder( uint8_t folderNumber )
		{
			SendCommand( 0x4E, folderNumber );
			return GetResult();
		}

		uint32_t	GetResult()
		{
			FProcessed() = false;
			while( ! FProcessed() )
				ProcessInput();

			return FHandleParameter;
		}

/*
		inline void PlayMP3FolderFile( uint16_t fileNumber )
		{
			SendCommand( 0x12, fileNumber );
		}

		inline void PlayAdvertisementFolderFile( uint16_t fileNumber )
		{
			SendCommand( 0x13, fileNumber );
		}
*/
		inline void SetIntVolume(uint8_t volume)
		{
			SendCommand( 0x06, volume );
		}

		inline void EnableLoop( uint8_t AValue )
		{
			SendCommand( 0x11, AValue );
		}

		inline void Reset()
		{
			TimeOutPeriod() = TimeOutPeriod() + 3000;
			SendCommand( 0x0C, 0 );
			TimeOutPeriod() = TimeOutPeriod() - 3000;
			delay( 200 );
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			Reset();
		}

		inline void NextInputPin_o_Receive( void *_Data )
		{
			Next();
		}

		inline void PreviousInputPin_o_Receive( void *_Data )
		{
			Previous();
		}

	public:
		inline void SystemInit()
		{
//			FPlayer.begin( C_SERIAL.GetStream() );

//			C_SERIAL.GetStream().begin( 9600 );

//			Serial.println( "START" );
			Reset();

//			Serial.println( "DEVICE" );
			SendCommand( 0x09, OutputDevice() );
			delay(200);

//			Serial.println( "VOLUME" );
			SetIntVolume( Volume() * 30 + 0.5 );

//			Serial.println( "EqualizerMode" );
			SendCommand( 0x07, EqualizerMode() );
			UpdateGain();

			if( EnableDAC() )
				SendCommand( 0x1A, 0x00 );

			else
				SendCommand( 0x1A, 0x01 );

			if( Loop() )
				EnableLoop( 0x01 );

			else
				EnableLoop( 0x00 );

			delay(600);
		}

		inline void SystemLoopBegin()
		{
			ProcessInput();
		}

	public:
		inline MitovDFRobotSerialMP3()
		{
			FIsSending() = false;
//			FIsAvailable() = false;
			FProcessed() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_File,
		typename T_Folder
	> class DFRobotMP3PlayerElementPlayFolderFile :
		public T_File,
		public T_Folder
	{
	public:
		_V_PROP_( Folder )
		_V_PROP_( File )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.PlayFolder( Folder(), File() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_File,
		typename T_Folder
	> class DFRobotMP3PlayerElementPlayLargeFolderFile :
		public T_File,
		public T_Folder
	{
	public:
		_V_PROP_( Folder )
		_V_PROP_( File )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.PlayLargeFolder( Folder(), File() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_File,
		uint8_t T_COMMAND
	> class DFRobotMP3PlayerElementPlayFile :
		public T_File
	{
	public:
		_V_PROP_( File )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( T_COMMAND, HEX );
//			C_OWNER.Play( File );
			C_OWNER.SendCommand( T_COMMAND, File() );
//			if( T_COMMAND == 3 )
//				C_OWNER.EnableLoop( 0x01 );

//			C_OWNER.EnableLoop( 0x00 );
//			Serial.println( "Playing" );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_File,
		typename T_Loop
	> class DFRobotMP3PlayerElementPlayFileLoop :
		public T_File,
		public T_Loop
	{
	public:
		_V_PROP_( File )
		_V_PROP_( Loop )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( T_COMMAND, HEX );
			if( Loop() )
				C_OWNER.LoopFile( File() );

			else
				C_OWNER.Play( File() );
//			FOwner.SendCommand( T_COMMAND, File );
//			if( T_COMMAND == 3 )
//				FOwner.EnableLoop( 0x01 );

//			FOwner.EnableLoop( 0x00 );
//			Serial.println( "Playing" );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class DFRobotMP3PlayerElementGetFilesCount :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			uint32_t AResult = C_OWNER.GetFilesCount();
			T_OutputPin::SetPinValue( AResult );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Folder,
		typename T_OutputPin
	> class DFRobotMP3PlayerElementGetFilesCountInFolder :
		public T_Folder,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Folder )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			uint32_t	AResult = C_OWNER.GetFilesCountInFolder( Folder() );
			T_OutputPin::SetPinValue( AResult );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER, uint8_t T_COMMAND> class DFRobotMP3PlayerElementTypedCommand
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SendCommand( T_COMMAND, 0 );
//			C_OWNER.Sleep();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Folder
	> class DFRobotMP3PlayerElementLoopFolder :
		public T_Folder
	{
	public:
		_V_PROP_( Folder )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SendCommand( 0x17, Folder() );
		}

	};
//---------------------------------------------------------------------------
/*
	template <typename T_OWNER, T_OWNER &C_OWNER> class DFRobotMP3PlayerElementPlayMP3FolderFile
	{
	public:
		uint16_t	File = 1;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.PlayMP3FolderFile( File );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class DFRobotMP3PlayerElementPlayAdvertisementFolderFile
	{
	public:
		uint16_t	File = 1;

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.PlayAdvertisementFolderFile( File );
		}

	};
*/
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif