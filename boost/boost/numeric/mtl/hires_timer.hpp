//  mtl hires_timer.hpp header file  ----------------------------------------//

//  (C) Copyright Jeremy Siek 2001. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  (C) Copyright Beman Dawes 1994-99. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  Revision History
//  01 Apr 01  Branched from boost timer to provide version using gettimeofday
//  12 Jan 01  Change to inline implementation to allow use without library
//             builds. See docs for more rationale. (Beman Dawes) 
//  25 Sep 99  elapsed_max() and elapsed_min() added (John Maddock)
//  16 Jul 99  Second beta
//   6 Jul 99  Initial boost version

#ifndef MTL_HIRES_TIMER_HPP
#define MTL_HIRES_TIMER_HPP

#include <boost/config.hpp>
#include <unistd.h>
#include <sys/time.h>

#ifndef BOOST_NO_LIMITS
#include <limits>
#else
#include <limits.h>
#endif

namespace mtl {

//  hires_timer  ------------------------------------------------------------//

//  A hires_timer object measures elapsed time.

//  It is recommended that implementations measure wall clock rather than CPU
//  time since the intended use is performance measurement on systems where
//  total elapsed time is more important than just process or CPU time.

//  Warnings: The maximum measurable elapsed time may well be only 596.5+ hours
//  due to implementation limitations.  The accuracy of timings depends on the
//  accuracy of timing information provided by the underlying platform, and
//  this varies a great deal from platform to platform.

class hires_timer
{
public:
  hires_timer()
    { gettimeofday(&m_start_time, 0); }	    // post: elapsed()==0
  //hires_timer( const hires_timer& src );  // post: elapsed()==src.elapsed()
  //~hires_timer(){}
  //hires_timer& operator=(const hires_timer& src);
  //   post: elapsed()==src.elapsed()
  void   restart() { gettimeofday(&m_start_time, 0); } // post: elapsed()==0
  double elapsed() const                    // return elapsed time in seconds
  {
    struct timeval cur_time;
    gettimeofday(&cur_time, 0);
    long elap_sec = cur_time.tv_sec - m_start_time.tv_sec;
    long elap_usec = cur_time.tv_usec - m_start_time.tv_usec;
    return  double(elap_sec * 1e6 + elap_usec) * 1e-6;
  }

  double elapsed_max() const   // return estimated maximum value for elapsed()
  // Portability warning: elapsed_max() may return too high a value on systems
  // where std::clock_t overflows or resets at surprising values.
  {
#ifndef BOOST_NO_LIMITS
    return double(std::numeric_limits<long>::max());
#else
    return DBL_MAX;
#endif
  }

  double elapsed_min() const            // return minimum value for elapsed()
  {
    hires_timer t;
    while (t.elapsed() == 0) ;
    return t.elapsed();
  }

 private:
  struct timeval m_start_time;
}; // hires_timer


} // namespace mtl

#endif  // MTL_HIRES_TIMER_HPP
