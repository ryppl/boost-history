/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPLEMENTATION_INFO_FENCE_SUPPORT_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_IMPLEMENTATION_INFO_FENCE_SUPPORT_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

  #if WINVER >= 0x0600 // Windows Vista

    // Note: Same name as vista windows on purpose
    #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                         \
            ( vista, ( full_fence )( load_fence )( store_fence ) )

  #else

    // ToDo: Add support (SSE sfence, SSE2 lfence, mfence)

  #endif // End WINVER checks

#elif defined( __GNUC__ ) // if gcc

  // ToDo: Add support (SSE sfence, SSE2 lfence, mfence)

#elif defined( _OPENMP ) // Else, resort to OpenMP (full fence only, via flush)

  #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                           \
          ( openmp, ( full_fence ) )

#endif

#endif
