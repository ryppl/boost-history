#ifndef BOOST_FIBERS_DETAIL_HAS_SYNC_H
#define BOOST_FIBERS_DETAIL_HAS_SYNC_H

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
// based on boost/smart_ptr/detail/sp_has_sync.hpp
//
//  Copyright (c) 2008, 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  Defines the BOOST_SP_HAS_SYNC macro if the __sync_* intrinsics
//  are available.
//

#if defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )

#define BOOST_FIBER_HAS_SYNC

#if defined( __arm__ )  || defined( __armel__ )
#undef BOOST_FIBER_HAS_SYNC
#endif

#if defined( __hppa ) || defined( __hppa__ )
#undef BOOST_FIBER_HAS_SYNC
#endif

#if defined( __m68k__ )
#undef BOOST_FIBER_HAS_SYNC
#endif

#if defined( __sparc__ )
#undef BOOST_FIBER_HAS_SYNC
#endif

#if defined( __INTEL_COMPILER ) && !defined( __ia64__ )
#undef BOOST_FIBER_HAS_SYNC
#endif

#endif // __GNUC__ * 100 + __GNUC_MINOR__ >= 401

#endif // BOOST_FIBERS_DETAIL_HAS_SYNC_H
