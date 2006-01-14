#ifndef BOOST_PROFILER_DETAIL_MSVC_DUMMY_SEMAPHORE_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_MSVC_DUMMY_SEMAPHORE_HPP_INCLUDED

#include <windows.h>

namespace boost { namespace profiler { namespace detail 
{

    typedef int semaphore_t;
    
    inline void acquire_semaphore(semaphore_t *semaphore)
    {
    }

    inline void release_semaphore(semaphore_t *semaphore)
    {
    }

} } }

#endif
