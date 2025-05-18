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

#include <SPI.h>

//#if defined( VISUINO_ANY_ESP32 )
    #define private public
    #include <SD.h>
    #undef private
//#else
//    #include <SdFat.h>
//#endif

#ifdef VISUINO_ANY_ESP32
	#include "vfs_api.h"
	#include "sd_diskio.h"
	#include "ff.h"
	#include "FS.h"
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

#ifdef VISUINO_ANY_ESP32
	typedef fs::SDFS SDClass;
#endif
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class MicroSDCardElementOperation :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CheckExists,
		typename T_Create,
		typename T_PathName,
		typename T_Remove
	> class MicroSDCardElementDirectory :
		public T_CheckExists,
		public T_Create,
		public T_Remove,
		public T_PathName
	{
	public:
		_V_PROP_( PathName )

	public:
		_V_PROP_( CheckExists )
		_V_PROP_( Create )
		_V_PROP_( Remove )

	public:
		inline bool DoExists( Mitov::String APathName )
		{
			return C_OWNER.DoExists( JoinPath( APathName ));
		}

		inline bool DoCreate( Mitov::String APathName )
		{
			return C_OWNER.DoCreate( JoinPath( APathName ));
		}

		inline bool DoRemoveDir( Mitov::String APathName )
		{
			return C_OWNER.DoRemoveDir( JoinPath( APathName ));
		}

		inline bool DoRemoveFile( Mitov::String APathName )
		{
			return C_OWNER.DoRemoveFile( JoinPath( APathName ));
		}

		inline bool DoOpen( Mitov::String APathName, MITOV_FILE_MODE mode, File &AFile )
		{
			return C_OWNER.DoOpen( JoinPath( APathName ), mode, AFile );
		}

	protected:
		Mitov::String JoinPath( Mitov::String AValue )
		{
			Mitov::String APath = PathName();
			if( ! APath.endsWith( "/" ) )
				APath = APath + "/";

			if( AValue.startsWith( "/" ) )
				AValue.remove( 0, 1 );

			return APath + AValue;
		}

	public:
		void CheckExists_ClockInputPin_o_Receive( void *_Data )
		{
			CheckExists().SetPinValue( C_OWNER.DoExists( PathName() ));
		}

		void Create_ClockInputPin_o_Receive( void *_Data )
		{
			Create().SetPinValue( C_OWNER.DoCreate( PathName() ));
		}

		void Remove_ClockInputPin_o_Receive( void *_Data )
		{
			Remove().SetPinValue( C_OWNER.DoRemoveDir( PathName() ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FailedOutputPin,
		typename T_OutputPin,
		typename T_PathName,
		typename T_Recursive
	> class MicroSDCardElementList :
		public T_OutputPin,
		public T_FailedOutputPin,
		public T_Recursive,
		public T_PathName
	{
	public:
		_V_PIN_( OutputPin )
//#ifdef MICROSD_LIST_USE_FAILED_OUTPUT_PIN
		_V_PIN_( FailedOutputPin )
//#endif

	public:
		_V_PROP_( PathName )
		_V_PROP_( Recursive )

	protected:
		void ProcessLevel( Mitov::String APath, Mitov::String APrefix )
		{
			File AFile;
			if( ! C_OWNER.DoOpen( APath, FILE_READ, AFile ))
			{
//#ifdef MICROSD_LIST_USE_FAILED_OUTPUT_PIN
				T_FailedOutputPin::ClockPin();
//#endif
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
				C_OWNER.SendErrorInfo( "Missing Path" );
#endif
				return;
			}

			if( APath.endsWith( "/" ))
				APath = APath.substring( 0, APath.length() - 1 );

			while (true)
			{
				File AEntry =  AFile.openNextFile();
				if (! AEntry)
				  // no more files
				  break;

				Mitov::String AItemName = APrefix + AEntry.name();
				T_OutputPin::SetPinValue( AItemName.c_str() );

				if( Recursive() )
					if( AEntry.isDirectory())
						ProcessLevel( APath + '/' + AEntry.name(), AItemName + '/' );

				AEntry.close();
			  }

			AFile.close();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "" );
//			Serial.println( PathName );
//			Serial.println( "" );
			if( PathName() == "" )
				ProcessLevel( "/", "" );

			else
				ProcessLevel( PathName(), "" );
/*
			File AFile;
			if( ! C_OWNER.DoOpen( PathName, FILE_READ, AFile ))
			{
				FailedOutputPin.Notify( nullptr );
				return;
			}

			while (true)
			{
				File AEntry =  AFile.openNextFile();
				if (! AEntry)
				  // no more files
				  break;

				OutputPin.Notify( AEntry.name() );
				AEntry.close();
			  }

			AFile.close();
*/
		}

	public:
		inline void SystemStart()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Append,
		typename T_CacheSize,
		typename T_Enabled,
		typename T_FCurrentPosition,
		typename T_FHasWrite,
		typename T_FInWriteMode,
		typename T_FPositionStored,
		typename T_FReadWritePosition,
		typename T_FWriteCount,
//		typename T_FReadMoved,
//		typename T_FWriteMoved,
		typename T_KeepClosed,
		typename T_KeepFlushed,
		typename T_NewLine,
		typename T_PathName,
		typename T_ReadPositionOutputPin,
		typename T_Remove,
		typename T_Size,
		typename T_WritePositionOutputPin
	> class MicroSDCardElementFile :
		public T_ReadPositionOutputPin,
		public T_WritePositionOutputPin,
		public T_Append,
		public T_CacheSize,
		public T_Enabled,
		public T_Remove,
		public T_Size,
		public T_NewLine,
		public T_KeepFlushed,
		public T_KeepClosed,
		public T_PathName,
		public T_FInWriteMode,
		public T_FPositionStored,
		public T_FReadWritePosition,
		public T_FWriteCount,
//		public T_FReadMoved,
//		public T_FWriteMoved,
		public T_FCurrentPosition,
		public T_FHasWrite
	{
	public:
		_V_PIN_( ReadPositionOutputPin )
		_V_PIN_( WritePositionOutputPin )

	public:
		_V_PROP_( Append )
		_V_PROP_( PathName )
		_V_PROP_( CacheSize )
		_V_PROP_( Enabled )
		_V_PROP_( NewLine )
		_V_PROP_( KeepFlushed )
		_V_PROP_( KeepClosed )
		_V_PROP_( Size )
		_V_PROP_( Remove )

	public:
		_V_PROP_( FHasWrite )

	protected:
//		_V_PROP_( FReadMoved )
//		_V_PROP_( FWriteMoved )

		_V_PROP_( FWriteCount )
		_V_PROP_( FInWriteMode )
		_V_PROP_( FPositionStored )
		_V_PROP_( FCurrentPosition )
		_V_PROP_( FReadWritePosition )

//		uint32_t	FWritePosition = 0;
//		uint32_t	FReadPosition = 0;

	public:
		File FFile;

	public:
/*
		inline void SetHasWrite()
		{
			FHasWrite() = true;
		}
*/
		inline int Read()
		{
			if( ! TryOpen() )
				return -1;

			SwitchToReadWriteMode( false );
			return FFile.read();
		}

		inline int Peek()
		{
			if( ! TryOpen() )
				return -1;

			SwitchToReadWriteMode( false );
			return FFile.peek();
		}

		inline size_t ReadBytes( uint8_t *AData, size_t ASize )
		{
			if( ! TryOpen() )
				return 0;

//			Serial.println( "ReadBytes" );
			SwitchToReadWriteMode( false );

//			Serial.print( "POS: " );
//			Serial.println( FFile.position() );

#if( defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_ESP8266 ) || defined( VISUINO_TEENSY ))
			int AResult = FFile.readBytes( (char *)AData, ASize );
#else
			int AResult = FFile.readBytes( (uint8_t *)AData, ASize );
#endif
//			Serial.print( "POS1: " );
//			Serial.println( FFile.position() );
			return AResult;
		}

		inline bool DataAvailable()
		{
			if( ! TryOpen() )
				return false;

			SwitchToReadWriteMode( false );
			return FFile.available();
		}

	public:
		void PrintChar( char AValue )
		{
			if( ! TryOpen() )
				return;

			SwitchToReadWriteMode( true );
			FFile.print( AValue );
			if( AValue == 10 )
				UpdateWrite();

		}

		template <typename T> void Print( T AValue )
		{
			if( ! TryOpen() )
				return;

			SwitchToReadWriteMode( true );

//			Serial.println( "TEST2" );
//			Serial.println( AValue );

//			Serial.println( FFile.position() );

			if( NewLine() )
				FFile.println( AValue );

			else
				FFile.print( AValue );

//			Serial.println( FFile.position() );

			UpdateWrite();
		}

		void Write( void *_Data, size_t ASize )
		{
			if( ! TryOpen() )
				return;

//			Serial.println( "TEST1" );

			SwitchToReadWriteMode( true );
			FFile.write( (uint8_t *)_Data, ASize );
			FWriteCount() = FWriteCount().GetValue() + ASize;
			if( FWriteCount().GetValue() > CacheSize().GetValue() )
			{
				FWriteCount() = 0;
				UpdateWrite();
			}

			else
				ReportWrite();

		}

		void BeginRead()
		{
//			Serial.println( "BeginRead()" );
			FWriteCount() = 0;
			UpdateWrite();

			SwitchToReadWriteMode( false );
		}

//		inline void ResetReadPosition()
//		{
//			FFile.seek( FReadPosition().GetValue() );
//		}

		inline void SeekRead( uint32_t AValue )
		{
			if( FInWriteMode() )
				FReadWritePosition() = AValue;

			else
				FFile.seek( AValue );

			T_ReadPositionOutputPin::SetPinValue( AValue );
		}

		inline void SeekWrite( uint32_t AValue )
		{
			if( FInWriteMode() )
				FFile.seek( AValue );

			else
				FReadWritePosition() = AValue;

			T_WritePositionOutputPin::SetPinValue( AValue );
		}

	protected:
		void SwitchToReadWriteMode( bool AInWriteMode )
		{
			if( ! FHasWrite().GetValue() )
				return;

//			Serial.print( "SwitchToReadWriteMode ---- : " );
//			Serial.println( AInWriteMode );
			if( FInWriteMode().GetValue() == AInWriteMode )
				return;

//			Serial.print( "SwitchToReadWriteMode : " );
//			Serial.println( AInWriteMode );

			uint32_t APosition;
			if( FFile )
 				APosition = FFile.position();

			else
				APosition = FCurrentPosition();

			FFile.seek( FReadWritePosition().GetValue() );
			FReadWritePosition() = APosition;
			FInWriteMode() = AInWriteMode;
		}

		void ReportWrite()
		{
			if( T_WritePositionOutputPin::GetPinIsConnected() )
				T_WritePositionOutputPin::SetPinValue( FFile.position() ); // Read BEFORE Close!!!

		}

		void UpdateWrite()
		{
			ReportWrite();

//			Serial.println( "UpdateWrite()" );
//			Serial.println( FFile.position() );

			if( KeepClosed() )
			{
				if( ! FCurrentPosition().GetIsConstant() )
					if( FFile )
						FCurrentPosition() = FFile.position();

				FFile.close();
			}

			else if( KeepFlushed() )
				FFile.flush();


		}

	public:
		bool TryOpen()
		{
			if( FFile )
				return true;

//			Serial.println( "OPEN1" );
			if( ! Enabled() )
				return false;

			if( PathName().GetValue() == "" )
				return false;

//			Serial.println( PathName );
//			Serial.println( FHasWrite().GetValue() );
			bool AResult;
			if( FHasWrite() )
				AResult = C_OWNER.DoOpen( PathName(), FILE_WRITE, FFile );

			else
				AResult = C_OWNER.DoOpen( PathName(), FILE_READ, FFile );

			if( ! FPositionStored().GetValue() )
			{
				FCurrentPosition() = FFile.size();
				FPositionStored() = true;
			}

//			Serial.println( "GET:" );
//			Serial.println( FCurrentPosition() );

//			Serial.println( FFile.position() );

			FFile.seek( FCurrentPosition());

//			Serial.println( FFile.position() );

			return AResult;
		}

	public:
		inline void CloseInputPin_o_Receive( void *_Data )
		{
			if( ! FCurrentPosition().GetIsConstant() )
				if( FFile )
					FCurrentPosition() = FFile.position();

			FFile.close();
		}

		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FFile.flush();
		}

		inline void Size_ClockInputPin_o_Receive( void *_Data )
		{
//#ifdef MicroSDCardElementFile_Size
			Size().OutputPin().SetPinValue( FFile.size() );
//#endif
		}

		inline void Remove_ClockInputPin_o_Receive( void *_Data )
		{
			Remove().SetPinValue( C_OWNER.DoRemoveFile( PathName() ));
		}

	public:
		inline MicroSDCardElementFile() // bool AHasWrite )
		{
//			FHasWrite() = AHasWrite;
			FInWriteMode() = false;
			FWriteCount() = 0;
			FReadWritePosition() = 0;
			FCurrentPosition() = 0;
			FPositionStored() = false;
//			FReadMoved() = false;
//			FWriteMoved() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin,
		typename T_PathName
	> class MicroSDCardElementExists :
		public T_OutputPin,
		public T_PathName
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( PathName )

	public:
		inline void SystemStart()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( C_OWNER.DoExists( PathName() ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_ErrorInfoOutputPin,
		typename T_FRunning,
		typename T_FailedOutputPin,
		typename T_SuccessOutputPin,
		typename T_TYPE
	> class MicroSDCardBasic :
		public T_Enabled,
		public T_FailedOutputPin,
		public T_FRunning,
		public T_SuccessOutputPin,
		public T_ErrorInfoOutputPin
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SuccessOutputPin )
		_V_PIN_( ErrorInfoOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FRunning )

	public:
		bool DoExists( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

//			Serial.println( "COOL1" );
//			Serial.println( APathName );
			return FCard.exists( APathName );
		}

		bool DoCreate( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			return FCard.mkdir( APathName );
		}

		bool DoRemoveDir( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			return FCard.rmdir( APathName );
		}

		bool DoRemoveFile( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			return FCard.remove( APathName );
		}

		bool DoOpen( Mitov::String APathName, MITOV_FILE_MODE mode, File &AFile )
		{
//			Serial.print( "O1 :" );
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			if( ! APathName.startsWith( "/" ))
				APathName = Mitov::String( "/" ) + APathName;

//			char *ATmp = new char[ APathName.length() + 1 ];
//			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

//			Serial.print( "T1 :" );
//			Serial.println( ATmp );
			AFile = FCard.open( APathName.c_str(), mode );
//			AFile = FCard->open( "/LOG.TXT", mode );
//			delete [] ATmp;

//			Serial.println( AFile );

			return AFile;
		}

	public:
#if defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_TEENSY )
		T_TYPE	&FCard;
#else
		T_TYPE	FCard;
#endif

	public:
		inline void SendErrorInfo( const char *AMessage )
		{
			T_ErrorInfoOutputPin::SetPinValue( AMessage );
		}


#if defined( VISUINO_ANY_ESP32 ) || defined( VISUINO_TEENSY )
	public:
		inline MicroSDCardBasic() :
			FCard( SD )
		{
		}
#endif

	};
//---------------------------------------------------------------------------
	template <
		int PIN_NUMBER,
		typename T_Enabled,
		typename T_ErrorInfoOutputPin,
		typename T_FRunning,
		typename T_FailedOutputPin,
		typename T_SuccessOutputPin,
		typename T_TYPE
	> class MicroSDCard :
	    public MicroSDCardBasic<
			T_Enabled,
			T_ErrorInfoOutputPin,
			T_FRunning,
			T_FailedOutputPin,
			T_SuccessOutputPin,
			T_TYPE
		>
	{
		typedef MicroSDCardBasic<
			T_Enabled,
			T_ErrorInfoOutputPin,
			T_FRunning,
			T_FailedOutputPin,
			T_SuccessOutputPin,
			T_TYPE
		>	inherited;

	public:
		void UpdateEnabled()
		{
			if( inherited::Enabled() )
			{
//				Serial.println( "TEST1" );
				inherited::FRunning() = inherited::FCard.begin( PIN_NUMBER );
				if( inherited::FRunning() )
					T_SuccessOutputPin::ClockPin();

				else
				{
//					Serial.println( "FAIL" );
					T_FailedOutputPin::ClockPin();
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
					T_ErrorInfoOutputPin::SetPinValue( "Fail to open microSD" );
#endif
					inherited::Enabled() = false;
					UpdateEnabled();
				}
//				Serial.println( "COOL" );
			}

			else
			{
#ifndef VISUINO_TEENSY
				inherited::FCard.end();
#endif
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

	};
//---------------------------------------------------------------------------
#if defined( VISUINO_ANY_ESP32 )
	template <
		typename T_SPI, T_SPI &C_SPI,
		int PIN_NUMBER,
		typename T_Enabled,
		typename T_ErrorInfoOutputPin,
		typename T_FRunning,
		typename T_FailedOutputPin,
		typename T_SuccessOutputPin,
		typename T_TYPE
	> class MicroSDCardCustom :
		public T_FailedOutputPin,
		public T_SuccessOutputPin,
		public T_ErrorInfoOutputPin,
		public T_FRunning,
		public T_Enabled
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SuccessOutputPin )
		_V_PIN_( ErrorInfoOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FRunning )

	public:
		bool DoExists( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

//			Serial.println( "COOL1" );
//			Serial.println( APathName );
			return FCard.exists( APathName );
		}

		bool DoCreate( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			return FCard.mkdir( APathName );
		}

		bool DoRemoveDir( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			return FCard.rmdir( APathName );
		}

		bool DoRemoveFile( Mitov::String APathName )
		{
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			return FCard.remove( APathName );
		}

		bool DoOpen( Mitov::String APathName, MITOV_FILE_MODE mode, File &AFile )
		{
//			Serial.print( "O1 :" );
			if( ! ( FRunning().GetValue() && Enabled().GetValue() ) )
				return false;

			if( ! APathName.startsWith( "/" ))
				APathName = Mitov::String( "/" ) + APathName;

//			char *ATmp = new char[ APathName.length() + 1 ];
//			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

//			Serial.print( "T1 :" );
//			Serial.println( ATmp );
			AFile = FCard.open( APathName, mode );
//			AFile = FCard->open( "/LOG.TXT", mode );
//			delete [] ATmp;

//			Serial.println( AFile );

			return AFile;
		}

	public:
		T_TYPE	FCard;

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
//				Serial.println( "TEST1" );
#if defined( VISUINO_ANY_ESP32 )
				FRunning() = FCard.begin( PIN_NUMBER, C_SPI );
#else
				FRunning() = FCard.begin( PIN_NUMBER );
#endif
				if( FRunning() )
					T_SuccessOutputPin::ClockPin();

				else
				{
//					Serial.println( "FAIL" );
					T_FailedOutputPin::ClockPin();
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
					T_ErrorInfoOutputPin::SetPinValue( "Fail to open microSD" );
#endif
					Enabled() = false;
					UpdateEnabled();
				}
//				Serial.println( "COOL" );
			}
			else
			{
				FCard.end();
//				delete FCard;
//				FCard = nullptr;
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

	public:
		MicroSDCardCustom() :
			FCard( FSImplPtr( new VFSImpl()) )
		{
		}
/*
	public:
		virtual ~MicroSDCard()
		{
			if( FCard )
				delete FCard;
		}
*/
	};
#endif // defined( VISUINO_ANY_ESP32 )
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
