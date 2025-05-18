////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <sys/socket.h>

namespace Mitov
{
	int MitovConnect (int __fd, sockaddr* __addr, socklen_t __len)
	{
//		extern int connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
		return connect( __fd, (__CONST_SOCKADDR_ARG)__addr, (socklen_t)__len );
	}
}
