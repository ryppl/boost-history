#ifndef _DATE_TIME_INT_ADAPTER_HPP__
#define _DATE_TIME_INT_ADAPTER_HPP__
/* Copyright (c) 2001, 2003 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland, Bart Garst
 */


#include "boost/limits.hpp" //work around compilers without limits
#include "boost/date_time/special_defs.hpp"
#include "boost/date_time/locale_config.hpp"
#include <iostream>

namespace boost {
namespace date_time {


//! Adapter to create integer types with +-infinity, and not a value
/*! This class is used internally in counted date/time representations.
 *  It adds the floating point like features of infinities and
 *  not a number. It also provides mathmatical operations with
 *  consideration to special values following these rules:
 *@code
 *  +infinity  -  infinity  == Not A Number (NAN)
 *   infinity  *  non-zero  == infinity
 *   infinity  *  zero      == NAN
 *  +infinity  * -integer   == -infinity
 *   infinity  /  infinity  == NAN
 *   infinity  *  infinity  == infinity 
 *@endcode 
 */
template<typename int_type_>
class int_adapter {
public:
  typedef int_type_ int_type;
  int_adapter(int_type v) :
    value_(v)
  {}
  static bool has_infinity()
  {
    return  true;
  }
  static const int_adapter  pos_infinity()
  {
    return ::std::numeric_limits<int_type>::max();
  }
  static const int_adapter  neg_infinity()
  {
    return ::std::numeric_limits<int_type>::min();
  }
  static const int_adapter  not_a_number()
  {
    return ::std::numeric_limits<int_type>::max()-1;
  }
  static  int_adapter max()
  {
    return ::std::numeric_limits<int_type>::max()-2;
  }
  static  int_adapter min()
  {
    return ::std::numeric_limits<int_type>::min()+1;
  }
  static int_adapter from_special(special_values sv)
  {
    switch (sv) {
    case not_a_date_time: return not_a_number();
    case neg_infin:       return neg_infinity();
    case pos_infin:       return pos_infinity();
    case max_date_time:   return max();
    case min_date_time:   return min();
    default:              return not_a_number();
    }
  }
  static bool is_inf(int_type v)
  {
    return (v == neg_infinity().as_number() ||
            v == pos_infinity().as_number());
  }
  static bool is_neg_infinity(int_type v)
  {
    return (v == neg_infinity().as_number());
  }
  static bool is_pos_infinity(int_type v)
  {
    return (v == pos_infinity().as_number());
  }
  static bool is_not_a_number(int_type v)
  {
    return (v == not_a_number().as_number());
  }
  //! Returns either special value type or is_not_special
  static special_values to_special(int_type v)
  {
    if (is_not_a_number(v)) return not_a_date_time;
    if (is_neg_infinity(v)) return neg_infin;
    if (is_pos_infinity(v)) return pos_infin;
    return not_special;
  }

  //-3 leaves room for representations of infinity and not a date
  static  int_type maxcount()
  {
    return ::std::numeric_limits<int_type>::max()-3;
  }
  bool is_infinity() const
  {
    return (value_ == neg_infinity().as_number() ||
            value_ == pos_infinity().as_number());
  }
  bool is_nan() const
  {
    return (value_ == not_a_number().as_number());
  }
  bool is_special() const
  {
    return(is_infinity() || is_nan()); 
  }
  bool operator==(const int_adapter& rhs) const
  {
    return (compare(rhs) == 0);
  }
  bool operator==(const int& rhs) const
  {
    if(!std::numeric_limits<int_type>::is_signed)
    {
      if(is_neg_infinity(value_) && rhs == 0)
      {
        return false;
      }
    }
    return (compare(rhs) == 0);
  }
  bool operator!=(const int_adapter& rhs) const
  {
    return (compare(rhs) != 0);
  }
  bool operator!=(const int& rhs) const
  {
    if(!std::numeric_limits<int_type>::is_signed)
    {
      if(is_neg_infinity(value_) && rhs == 0)
      {
        return true;
      }
    }
    return (compare(rhs) != 0);
  }
  bool operator<(const int_adapter& rhs) const
  {
    return (compare(rhs) == -1);
  }
  bool operator<(const int& rhs) const
  {
    if(!std::numeric_limits<int_type>::is_signed)
    {
      if(is_neg_infinity(value_) && rhs == 0)
      {
        return true;
      }
    }
    return (compare(rhs) == -1);
  }
  bool operator>(const int_adapter& rhs) const
  {
    return (compare(rhs) == 1);
  }
  int_type as_number() const
  {
    return value_;
  }
  //! Returns either special value type or is_not_special
  special_values as_special() const
  {
    return int_adapter::to_special(value_);
  }
  //creates nasty ambiguities
//   operator int_type() const
//   {
//     return value_;
//   }

  /*! Operator allows for adding dissimilar int_adapter types.
   * The return type will match that of the the calling object's type */
  template<class rhs_type>
  inline
  int_adapter operator+(const int_adapter<rhs_type>& rhs) const
  {
    if(is_special() || rhs.is_special())
    {
      if (is_nan() || rhs.is_nan()) 
      {
        return int_adapter::not_a_number();
      }
      if((is_pos_infinity(value_) && rhs.is_neg_infinity(rhs.as_number())) ||
	  (is_neg_infinity(value_) && rhs.is_pos_infinity(rhs.as_number())) )
      {
        return int_adapter::not_a_number();
      }
      if (is_infinity()) 
      {
        return *this;
      }
      if (rhs.is_pos_infinity(rhs.as_number())) 
      {
        return int_adapter::pos_infinity();
      }
      if (rhs.is_neg_infinity(rhs.as_number())) 
      {
        return int_adapter::neg_infinity();
      }
    }
    return int_adapter<int_type>(value_ + rhs.as_number());
  }

  int_adapter operator+(const int_type rhs) const
  {
    if(is_special())
    {
      if (is_nan()) 
      {
        return int_adapter<int_type>(not_a_number());
      }
      if (is_infinity()) 
      {
        return *this;
      }
    }
    return int_adapter<int_type>(value_ + rhs);
  }
  
  /*! Operator allows for subtracting dissimilar int_adapter types.
   * The return type will match that of the the calling object's type */
  template<class rhs_type>
  inline
  int_adapter operator-(const int_adapter<rhs_type>& rhs)const
  {
    if(is_special() || rhs.is_special())
    {
      if (is_nan() || rhs.is_nan()) 
      {
        return int_adapter::not_a_number();
      }
      if((is_pos_infinity(value_) && rhs.is_pos_infinity(rhs.as_number())) ||
         (is_neg_infinity(value_) && rhs.is_neg_infinity(rhs.as_number())) )
      {
        return int_adapter::not_a_number();
      }
      if (is_infinity()) 
      {
        return *this;
      }
      if (rhs.is_pos_infinity(rhs.as_number())) 
      {
        return int_adapter::pos_infinity();
      }
      if (rhs.is_neg_infinity(rhs.as_number())) 
      {
        return int_adapter::neg_infinity();
      }
    }
    return int_adapter<int_type>(value_ - rhs.as_number());
  }
  int_adapter operator-(const int_type rhs) const
  {
    if(is_special())
    {
      if (is_nan()) 
      {
        return int_adapter<int_type>(not_a_number());
      }
      if (is_infinity()) 
      {
        return *this;
      }
    }
    return int_adapter<int_type>(value_ - rhs);
  }

  // should templatize this to be consistant with op +-
  int_adapter operator*(const int_adapter& rhs)const
  {
    if(this->is_special() || rhs.is_special())
    {
      return mult_div_specials(rhs);
    }
    return int_adapter<int_type>(value_ * rhs.value_);
  }
  /*! Provided for cases when automatic conversion from 
   * 'int' to 'int_adapter' causes incorrect results. */
  int_adapter operator*(const int rhs) const
  {
    if(is_special())
    {
      return mult_div_specials(rhs);
    }
    return int_adapter<int_type>(value_ * rhs);
  }

  // should templatize this to be consistant with op +-
  int_adapter operator/(const int_adapter& rhs)const
  {
    if(this->is_special() || rhs.is_special())
    {
      if(is_infinity() && rhs.is_infinity())
      {
	return int_adapter<int_type>(not_a_number());
      }
      if(rhs != 0)
      {
        return mult_div_specials(rhs);
      }
      else { // let divide by zero blow itself up
        return int_adapter<int_type>(value_ / rhs.value_);
      }
    }
    return int_adapter<int_type>(value_ / rhs.value_);
  }
  /*! Provided for cases when automatic conversion from 
   * 'int' to 'int_adapter' causes incorrect results. */
  int_adapter operator/(const int rhs) const
  {
    if(is_special() && rhs != 0)
    {
      return mult_div_specials(rhs);
    }
    return int_adapter<int_type>(value_ / rhs);
  }
#ifndef BOOST_DATE_TIME_NO_LOCALE
  /*! Expected output is either a numeric representation 
   * or a special values representation.<BR> 
   * Ex. "12", "+infinity", "not-a-number", etc. */
  friend std::ostream& operator<<(std::ostream& os, 
      const int_adapter<int_type> ia)
  {
    if(ia.is_special())
    {
      // switch copied from date_names_put.hpp
      switch(ia.as_special())
      {
	case not_a_date_time:
	  os << "not-a-number";
	  break;
	case pos_infin:
	  os << "+infinity";
	  break;
	case neg_infin:
	  os << "-infinity";
	  break;
	default:
	  os << "";
      }
    }
    else {
      os << ia.as_number(); 
    }
    return os;
  }
#endif
private:
  int_type value_;
  
  //! returns -1, 0, 1, or 2 if 'this' is <, ==, >, or 'nan comparison' rhs
  int compare(const int_adapter& rhs)const
  {
    if(this->is_special() || rhs.is_special())
    {
      if(this->is_nan() || rhs.is_nan())
      {
	if(this->is_nan() && rhs.is_nan()){
	  return 0; // equal
	}
	else {
	  return 2; // nan
	}
      }
      if((is_neg_infinity(value_) && !is_neg_infinity(rhs.value_)) ||
	  (is_pos_infinity(rhs.value_) && !is_pos_infinity(value_)) )
      {
	return -1; // less than
      }
      if((is_pos_infinity(value_) && !is_pos_infinity(rhs.value_)) ||
	  (is_neg_infinity(rhs.value_) && !is_neg_infinity(value_)) )
      {
	return 1; // greater than
      }
    }
    if(value_ < rhs.value_) return -1;
    if(value_ > rhs.value_) return 1;
    // implied-> if(value_ == rhs.value_) 
    return 0;
  }
  /* When multiplying and dividing with at least 1 special value
   * very simmilar rules apply. In those cases where the rules
   * are different, they are handled in the respective operator 
   * function. */
  //! Assumes at least 'this' or 'rhs' is a special value
  int_adapter mult_div_specials(const int_adapter& rhs)const
  {
    int min; 
    if(std::numeric_limits<int_type>::is_signed)
    {
      min = 0;
    }
    else {
      min = 1;// there is no zero with unsigned
    }
    if(this->is_nan() || rhs.is_nan())
    {
      return int_adapter<int_type>(not_a_number());
    }
    if((*this > 0 && rhs > 0) || (*this < min && rhs < min))
    {
        return int_adapter<int_type>(pos_infinity());
    }
    if((*this > 0 && rhs < min) || (*this < min && rhs > 0))
    {
        return int_adapter<int_type>(neg_infinity());
    }
    //implied -> if(this->value_ == 0 || rhs.value_ == 0)
    return int_adapter<int_type>(not_a_number());
  }
  /* Overloaded function necessary because of special
   * situation where int_adapter is instantiated with 
   * 'unsigned' and func is called with negative int.
   * It would produce incorrect results since 'unsigned'
   * wraps around when initialized with a negative value */
  //! Assumes 'this' is a special value
  int_adapter mult_div_specials(const int& rhs)const
  {
    int min; 
    if(std::numeric_limits<int_type>::is_signed)
    {
      min = 0;
    }
    else {
      min = 1;// there is no zero with unsigned
    }
    if(this->is_nan())
    {
      return int_adapter<int_type>(not_a_number());
    }
    if((*this > 0 && rhs > 0) || (*this < min && rhs < 0))
    {
        return int_adapter<int_type>(pos_infinity());
    }
    if((*this > 0 && rhs < 0) || (*this < min && rhs > 0))
    {
        return int_adapter<int_type>(neg_infinity());
    }
    //implied -> if(this->value_ == 0 || rhs.value_ == 0)
    return int_adapter<int_type>(not_a_number());
  }
  
};

} } //namespace date_time

/*
 * Copyright (c) 2001, 2003
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided as is without express or implied warranty.
 *
 *
 * Author:  Jeff Garland (jeff@CrystalClearSoftware.com)
 * Created: Sat Sep  8 19:37:11 2001
 *
 */


#endif
