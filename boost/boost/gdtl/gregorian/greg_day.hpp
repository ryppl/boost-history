#ifndef GREG_DAY_HPP___
#define GREG_DAY_HPP___
/* Copyright (c) 2000 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland 
 */

#include "boost/gdtl/constrained_value.hpp"
#include <stdexcept>

namespace boost {
namespace gregorian {

  //! Exception type for gregorian day of month (1..31)
  struct bad_day_of_month : public std::out_of_range
  {
    bad_day_of_month() : 
      std::out_of_range("Day of month value is out of range 1..31") 
    {}
  };
  //! Policy class that declares error handling and day of month ranges
  typedef CV::simple_exception_policy<unsigned short, 1, 31, bad_day_of_month> greg_day_policies;

  //! Generated represetation for gregorian day of month
  typedef CV::constrained_value<greg_day_policies> greg_day_rep;

  //! Represent a day of the month (range 1 - 31) 
  /*! This small class allows for simple conversion an integer value into
      a day of the month for a standard gregorian calendar.  The type 
      is automatically range checked so values outside of the range 1-31
      will cause a bad_day_of_month exception
  */
  class greg_day : public greg_day_rep {
  public:
    greg_day(unsigned short day_of_month) : greg_day_rep(day_of_month) {}
    unsigned short as_number() const {return value_;}
    operator unsigned short()  const {return value_;}
  private:
    
  };



} } //namespace gregorian

/* Copyright (c) 2000
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */


#endif
