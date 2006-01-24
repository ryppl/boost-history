// ----------------------------------------------------------------------------
// Copyright (C) 2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROFILER_DETAIL_DUMMY_SEMAPHORE_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_DUMMY_SEMAPHORE_HPP_INCLUDED

namespace boost { namespace profiler { namespace detail 
{

    typedef int semaphore_t;
    
    inline void acquire_semaphore(semaphore_t &semaphore)
    {
    }

    inline void release_semaphore(semaphore_t &semaphore)
    {
    }

} } }

#endif
