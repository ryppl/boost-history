//
//  boost/memory/winapi/winbase.h (*)
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef _BOOST_MEMORY_WINAPI_WINBASE_H_
#define _BOOST_MEMORY_WINAPI_WINBASE_H_

#if defined(_WIN32)

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef _WINBASE_
#include <winbase.h>
#endif

#else

#define BOOST_NO_WINSDK

#ifndef _BOOST_MEMORY_WINAPI_WTYPES_H_
#include "wtypes.h"
#endif

#ifndef _BOOST_MEMORY_WINAPI_POSIX_PTHREAD_HPP_
#include "posix/pthread.hpp"
#endif

#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* _BOOST_MEMORY_WINAPI_WINBASE_H_ */
