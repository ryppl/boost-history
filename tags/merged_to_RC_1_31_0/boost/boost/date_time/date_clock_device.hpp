#ifndef DATE_CLOCK_DEVICE_HPP___
#define DATE_CLOCK_DEVICE_HPP___

/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland 
 * $Date$
 */

#include "boost/date_time/c_time.hpp"


namespace boost {
namespace date_time {

  //! A clock providing day level services based on C time_t capabilities
  /*! This clock uses Posix interfaces as its implemenation and hence
   *  uses the timezone settings of the operating system.  Incorrect
   *  user settings will result in incorrect results for the calls
   *  to local_day.
   */
  template<class date_type> 
  class day_clock
  {
  public:
    typedef typename date_type::ymd_type ymd_type;
    //! Get the local day as a date type
    static date_type local_day() 
    {
      return date_type(local_day_ymd());
    }
    //! Get the local day as a ymd_type
    static typename date_type::ymd_type local_day_ymd() 
    {
      ::std::tm* curr = get_local_time();
      return ymd_type(curr->tm_year + 1900, 
                      curr->tm_mon + 1, 
                      curr->tm_mday);
    }
    //! Get the current day in universal date as a ymd_type
    static typename date_type::ymd_type universal_day_ymd() 
    {
      ::std::tm* curr = get_universal_time();
      return ymd_type(curr->tm_year + 1900, 
                      curr->tm_mon + 1, 
                      curr->tm_mday);
    }
    //! Get the UTC day as a date type
    static date_type universal_day() 
    {
      return date_type(universal_day_ymd());
    }

  private:
    static ::std::tm* get_local_time() 
    {
      ::std::time_t t;
      ::std::time(&t);
      return ::std::localtime(&t);
    }
    static ::std::tm* get_universal_time() 
    {
      ::std::time_t t;
      ::std::time(&t);
      return ::std::gmtime(&t);
    }

  };

} } //namespace date_time


#endif
