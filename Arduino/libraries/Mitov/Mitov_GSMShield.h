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
/*
	class GSMModule
	{
	public:
		virtual void StartModule()
		{
		}

		virtual void StopModule()
		{
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IsStarted,
		typename T_PIN,
		typename T_PacketService_StartModule,
		typename T_PacketService_StopModule,
//		typename T_PacketService,
		typename T_Restart
	> class GSMShield :
		public T_Enabled,
		public T_IsStarted,
		public T_PIN,
//		public T_PacketService,
		public T_Restart
	{
	public:
		_V_PROP_( PIN )
//		_V_PROP_( PacketService )
		_V_PROP_( Enabled )
		_V_PROP_( Restart )

	public:
		_V_PROP_( IsStarted )

	protected:
		GSM	FGsm;

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				StartShield();

			else
				StopShield();

		}

	protected:
		void StopShield()
		{
			T_PacketService_StopModule::Call();
//			T_PacketService::StopModule();
//			for( int i = 0; i < Modules.size(); ++i )
//				Modules[ i ]->StopModule();

			FGsm.shutdown();
			IsStarted() = false;
		}

/*
        void RestartShield()
		{
			if( ! Enabled )
				return;

			StartShield();
		}
*/
		void StartShield()
		{
			if( PIN() == "" )
			{
//				if( ! Restart() )
					IsStarted() = ( FGsm.begin( 0, Restart() ) == GSM_READY );

//				else
//					IsStarted() = ( FGsm.begin() == GSM_READY );
			}

			else
				IsStarted() = ( FGsm.begin( (char *)PIN().c_str(), Restart() ) == GSM_READY );

			if( IsStarted() )
				T_PacketService_StartModule::Call();
//				T_PacketService::StartModule();

//			for( int i = 0; i < Modules.size(); ++i )
//				Modules[ i ]->StartModule();
		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartShield();

		}

/*
	public:
		inline void SystemLoopBegin()
		{
			if( IsStarted() )
				T_PacketService::ModuleSystemLoopBegin();
		}
*/
	public:
		inline GSMShield()
		{
			IsStarted() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif