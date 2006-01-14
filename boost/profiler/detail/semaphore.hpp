#ifndef BOOST_PROFILER_DETAIL_SEMAPHORE_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_SEMAPHORE_HPP_INCLUDED

#include <boost/config.hpp>

#if !defined(BOOST_HAS_THREADS)
    #include <boost/profiler/detail/portable/dummy_semaphore.hpp>
#elif defined(BOOST_WINDOWS) && defined(BOOST_MSVC)
    #include <boost/profiler/detail/msvc/semaphore.hpp>
#else
    #error Your platform is not currently supported
#endif

#endif

