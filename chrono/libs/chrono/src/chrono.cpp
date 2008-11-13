//  chrono.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// define BOOST_CHRONO_SOURCE so that <boost/filesystem/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE 

#include <boost/chrono.hpp>
#include <stdexcept>

//----------------------------------------------------------------------------//
//                                                                            //
//                     Platform-specific Implementations                      //
//                                                                            //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(BOOST_CHRONO_WINDOWS_API)

#include <windows.h>
#undef min
#undef max

namespace
{
  double get_nanosecs_per_tic()
  {
    LARGE_INTEGER freq;
    if ( !QueryPerformanceFrequency( &freq ) )
    {
      throw
        std::runtime_error( "monotonic_clock: QueryPerformanceFrequency failed" );
    }
    return 1000000000.0L / freq.QuadPart;
  }

  double nanosecs_per_tic = get_nanosecs_per_tic();
}

namespace boost
{
namespace chrono
{

  monotonic_clock::time_point monotonic_clock::now()
  {
    LARGE_INTEGER pcount;
    if ( !QueryPerformanceCounter( &pcount ) )
    {
        throw
          std::runtime_error( "monotonic_clock: QueryPerformanceCounter failed" );
    }

    return time_point(duration(
      static_cast<monotonic_clock::rep>(nanosecs_per_tic * pcount.QuadPart) ));
  }


  system_clock::time_point system_clock::now()
  {
    FILETIME ft;
    ::GetSystemTimeAsFileTime( &ft );  // never fails
    return time_point(duration(
      (static_cast<__int64>( ft.dwHighDateTime ) << 32) | ft.dwLowDateTime));
  }

  std::time_t system_clock::to_time_t(const system_clock::time_point& t)
  {
      __int64 temp = t.time_since_epoch().count();

  #   if !defined( BOOST_MSVC ) || BOOST_MSVC > 1300 // > VC++ 7.0
      temp -= 116444736000000000LL;  // delta from epoch in microseconds
  #   else
      temp -= 116444736000000000;
  #   endif

      temp /= 10000000;
      return static_cast<std::time_t>( temp );
  }

  system_clock::time_point system_clock::from_time_t(std::time_t t)
  {
      __int64 temp = t;
      temp *= 10000000;

  #   if !defined( BOOST_MSVC ) || BOOST_MSVC > 1300 // > VC++ 7.0
      temp += 116444736000000000LL;
  #   else
      temp += 116444736000000000;
  #   endif

      return time_point(duration(temp));
  }

}  // namespace chrono
}  // namespace boost

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_MAC_API)

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_POSIX_API)

//#include <sys/time.h>
#include <time.h>

namespace boost
{
namespace chrono
{

  //system_clock::time_point system_clock::now()
  //{
  //  timeval tod;
  //  ::gettimeofday( &tod, 0 );
  //
  //  return time_point(duration(
  //    (static_cast<system_clock::rep>( tod.tv_sec ) * 1000000) + tod.tv_usec));
  //}

  #ifndef CLOCK_REALTIME
  # error <time.h> does not supply CLOCK_REALTIME
  #endif

  system_clock::time_point system_clock::now()
  {
    timespec ts;
    if ( ::clock_gettime( CLOCK_REALTIME, &ts ) )
    {
        boost::throw_exception(
          std::runtime_error( "system_clock: clock_gettime failed" ));
    }

    return time_point(duration(
      static_cast<system_clock::rep>( ts.tv_sec ) * 1000000000 + ts.tv_nsec));
  }

  std::time_t system_clock::to_time_t(const system_clock::time_point& t)
  {
      return static_cast<std::time_t>( t.time_since_epoch().count() / 1000000000 );
  }

  system_clock::time_point system_clock::from_time_t(std::time_t t)
  {
      return time_point(duration(static_cast<system_clock::rep>(t) * 1000000000));
  }

  #ifndef CLOCK_MONOTONIC
  # error <time.h> does not supply CLOCK_MONOTONIC
  #endif

  monotonic_clock::time_point monotonic_clock::now()
  {
    timespec ts;
    if ( ::clock_gettime( CLOCK_MONOTONIC, &ts ) )
    {
        boost::throw_exception(
          std::runtime_error( "monotonic_clock: clock_gettime failed" ));
    }

    return time_point(duration(
      static_cast<monotonic_clock::rep>( ts.tv_sec ) * 1000000000 + ts.tv_nsec));
  }

}  // namespace chrono
}  // namespace boost

#endif  // POSIX
