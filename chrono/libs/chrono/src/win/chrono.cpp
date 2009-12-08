//  win/chrono.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//

#include <windows.h>
#undef min
#undef max

namespace
{
  double get_nanosecs_per_tic()
  {
    LARGE_INTEGER freq;
    if ( !QueryPerformanceFrequency( &freq ) )
      return 0.0L;
    return 1000000000.0L / freq.QuadPart;
  }

  const double nanosecs_per_tic = get_nanosecs_per_tic();
}

namespace boost
{
namespace chrono
{

  monotonic_clock::time_point monotonic_clock::now()
  {

    LARGE_INTEGER pcount;
    if ( nanosecs_per_tic <= 0.0L || !QueryPerformanceCounter( &pcount ) )
    {
      DWORD cause = (nanosecs_per_tic <= 0.0L ? ERROR_NOT_SUPPORTED : ::GetLastError());
      boost::throw_exception(
        system::system_error( cause, system::system_category, "chrono::monotonic_clock" ));
    }

    return time_point(duration(
      static_cast<monotonic_clock::rep>(nanosecs_per_tic * pcount.QuadPart) ));
  }

  monotonic_clock::time_point monotonic_clock::now( system::error_code & ec )
  {
    static double nanosecs_per_tic = get_nanosecs_per_tic();

    LARGE_INTEGER pcount;
    if ( nanosecs_per_tic <= 0.0L || !QueryPerformanceCounter( &pcount ) )
    {
      DWORD cause = (nanosecs_per_tic <= 0.0L ? ERROR_NOT_SUPPORTED : ::GetLastError());
      ec.assign( cause, system::system_category );
      return time_point(duration(0));
    }

    ec.clear();
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

  system_clock::time_point system_clock::now( system::error_code & ec )
  {
    FILETIME ft;
    ::GetSystemTimeAsFileTime( &ft );  // never fails
    ec.clear();
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

