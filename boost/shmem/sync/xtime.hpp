//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_XTIME_HPP
#define BOOST_SHMEM_XTIME_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/cstdint.hpp>
#include <assert.h>

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#else
#include <time.h>
#endif

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#include <boost/shmem/sync/win32/win32_sync_primitives.hpp>
#endif

namespace boost {

namespace shmem {

enum xtime_clock_types
{
    TIME_UTC=1
//    TIME_TAI,
//    TIME_MONOTONIC,
//    TIME_PROCESS,
//    TIME_THREAD,
//    TIME_LOCAL,
//    TIME_SYNC,
//    TIME_RESOLUTION
};

struct xtime
{
 #if defined(BOOST_NO_INT64_T)
    typedef int_fast32_t xtime_sec_t; //INT_FAST32_MIN <= sec <= INT_FAST32_MAX
 #else
    typedef int_fast64_t xtime_sec_t; //INT_FAST64_MIN <= sec <= INT_FAST64_MAX
 #endif
    typedef int_fast32_t xtime_nsec_t; //0 <= xtime.nsec < NANOSECONDS_PER_SECOND

    xtime_sec_t sec;
    xtime_nsec_t nsec;

   bool operator < (const xtime &other) const
      {  return sec < other.sec || (sec == other.sec && nsec < other.nsec) ;  }

   bool operator == (const xtime &other) const
      {  return sec == other.sec && nsec == other.nsec;  }

   bool operator != (const xtime &other) const
      {  return !(*this == other);  }

   bool operator > (const xtime &other) const
      {  return other < *this;  }

   bool operator <= (const xtime &other) const
      {  return !(other < *this);  }

   bool operator >= (const xtime &other) const
      {  return !(*this < other);  }

};



inline int xtime_get(struct xtime* xtp, int clock_type)
{
    if (clock_type == TIME_UTC){
      #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
        filetime_t ft;
        get_system_time_as_file_time(&ft);
        static const boost::uint64_t timespec_to_filetime_offset =
            (boost::uint64_t)(116444736000000000);
        
        const boost::uint64_t ft64 =
            (static_cast<boost::uint64_t>(ft.dwHighDateTime) << 32)
            + ft.dwLowDateTime;

        xtp->sec = static_cast<xtime::xtime_sec_t>(
            (ft64 - timespec_to_filetime_offset) / 10000000
        );

        xtp->nsec = static_cast<xtime::xtime_nsec_t>(
            ((ft64 - timespec_to_filetime_offset) % 10000000) * 100
        );


      #else

        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        xtp->sec = ts.tv_sec;
        xtp->nsec = ts.tv_nsec;
        return clock_type;
      #endif

        return clock_type;
    }
    return 0;
}

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

inline void to_duration(xtime xt, int& milliseconds)
{
   const int NanosecondsPerSecond = 1000*1000*1000;
   const int NanosecondsPerMSecond = 1000*1000;
   const int MSecondsPerSecond = 1000;

   xtime cur;
   int res = 0;
   res = xtime_get(&cur, TIME_UTC);
   assert(res == TIME_UTC);

   if (xt <= cur){
      milliseconds = 0;
   }
   else{
      if (cur.nsec > xt.nsec){
         xt.nsec += NanosecondsPerSecond;
         --xt.sec;
      }
      milliseconds = (int)((xt.sec - cur.sec) * MSecondsPerSecond) +
         (((xt.nsec - cur.nsec) + (NanosecondsPerMSecond/2)) /
               NanosecondsPerMSecond);
   }
}

#else
/*!pthreads timed functions need timespec structures*/
inline static void xtime_to_timespec(const xtime& xt, timespec& ts)
{
   const int NanosecondsPerSecond = 1000*1000*1000;

   ts.tv_sec  = static_cast<int>(xt.sec);
   ts.tv_nsec = static_cast<int>(xt.nsec);
   if(ts.tv_nsec > NanosecondsPerSecond){
      ts.tv_sec += ts.tv_nsec / NanosecondsPerSecond;
      ts.tv_nsec %= NanosecondsPerSecond;
   }
}
#endif


}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //#ifndef BOOST_SHMEM_XTIME_HPP

