#ifndef POSIX_TIME_CONFIG_HPP___
#define POSIX_TIME_CONFIG_HPP___
/* Copyright (c) 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland
 */

#include "boost/gdtl/time_duration.hpp"
#include "boost/gdtl/time_resolution_traits.hpp"
#include "boost/gdtl/gregorian/gregorian_types.hpp"
#include "boost/gdtl/wrapping_int.hpp"
#include "boost/limits.hpp"
//force the definition of INT64_C macro used in posix_time_system
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#   if __GNUC__ >=  2
#   undef BOOST_HAS_STDINT_H
#   endif
#endif
#include "boost/cstdint.hpp"
#include <cmath>
#include <cstdlib> //for MCW 7.2 std::abs(long long)

namespace boost {
namespace posix_time {
  
//Remove the following line if you want 64 bit millisecond resolution time
//#define BOOST_GDTL_POSIX_TIME_STD_CONFIG

#ifdef BOOST_GDTL_POSIX_TIME_STD_CONFIG
  // set up conditional test compilations
#define BOOST_GDTL_HAS_MILLISECONDS
#define BOOST_GDTL_HAS_MICROSECONDS
#define BOOST_GDTL_HAS_NANOSECONDS
  typedef gdtl::time_resolution_traits<boost::int64_t, boost::gdtl::nano, 
				       1000000000, 9 > time_res_traits;
#else
  // set up conditional test compilations
#define BOOST_GDTL_HAS_MILLISECONDS
#define BOOST_GDTL_HAS_MICROSECONDS
#undef  BOOST_GDTL_HAS_NANOSECONDS
  typedef gdtl::time_resolution_traits<boost::int64_t, boost::gdtl::micro, 
				       1000000, 6 > time_res_traits;


// #undef BOOST_GDTL_HAS_MILLISECONDS
// #undef BOOST_GDTL_HAS_MICROSECONDS
// #undef BOOST_GDTL_HAS_NANOSECONDS
//   typedef gdtl::time_resolution_traits<boost::int64_t, boost::gdtl::tenth, 
// 				       10, 0 > time_res_traits;

#endif


  //! Base time duration type
  /*! \ingroup time_basics
   */
  class time_duration :
    public gdtl::time_duration<time_duration, time_res_traits>
  {
  public:
    typedef time_res_traits rep_type;
    typedef time_res_traits::day_type day_type;
    typedef time_res_traits::hour_type hour_type;
    typedef time_res_traits::min_type min_type;
    typedef time_res_traits::sec_type sec_type;
    typedef time_res_traits::fractional_seconds_type fractional_seconds_type;
    typedef time_res_traits::tick_type tick_type;
    time_duration(hour_type hour,
		  min_type min,
		  sec_type sec,
		  fractional_seconds_type fs=0) :
      gdtl::time_duration<time_duration, time_res_traits>(hour,min,sec,fs)
    {}
    time_duration() :
      gdtl::time_duration<time_duration, time_res_traits>(0,0,0)
    {}
    //Give duration access to ticks constructor -- hide from users
    friend class gdtl::time_duration<time_duration, time_res_traits>;
  private:
    explicit time_duration(tick_type ticks) :
      gdtl::time_duration<time_duration, time_res_traits>(ticks)
    {}
  };

#ifdef BOOST_GDTL_POSIX_TIME_STD_CONFIG

  //! Simple implementation for the time rep
  struct simple_time_rep
  {
    typedef gregorian::date      date_type;
    typedef time_duration        time_duration_type;
    simple_time_rep(date_type d, time_duration_type tod) :
      day(d),
      time_of_day(tod)
    {}
    date_type day;
    time_duration_type time_of_day;
  };

  class posix_time_system_config 
  {
   public:
    typedef simple_time_rep time_rep_type;
    typedef gregorian::date date_type;
    typedef gregorian::date_duration date_duration_type;
    typedef time_duration time_duration_type;
    typedef time_res_traits::tick_type int_type;
    typedef time_res_traits resolution_traits;
  };

#else

  class millisec_posix_time_system_config 
  {
   public:
    typedef boost::int64_t time_rep_type;
    typedef gregorian::date date_type;
    typedef gregorian::date_duration date_duration_type;
    typedef time_duration time_duration_type;
    typedef time_res_traits::tick_type int_type;
    typedef time_res_traits resolution_traits;
  };

#endif

} }//namespace posix_time

/* Copyright (c) 2002
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#endif


