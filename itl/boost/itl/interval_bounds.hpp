/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_BOUNDS_HPP_JOFA_100330
#define BOOST_ITL_INTERVAL_BOUNDS_HPP_JOFA_100330

#include <boost/itl/detail/base_interval.hpp>

namespace boost{namespace itl
{

/// Constants for interval bounds
enum BoundTypes {
    /// Both open: <tt>(x,y)</tt>
    open_bounded             = 0,  // '00'
    /// Left open right closed: <tt>(x,y]</tt>
    left_open                = 1,  // '01' 
    /// Left closed right open: <tt>[x,y)</tt>
    right_open               = 2,  // '10' 
    /// Both closed: <tt>[x,y]</tt>
    closed_bounded           = 3   // '11'
} ;

typedef unsigned char bound_type;

class interval_bounds
{
public:
    explicit interval_bounds(bound_type bounds): _bits(bounds){}
    interval_bounds left ()const { return interval_bounds(_bits & 1); }
    interval_bounds right()const { return interval_bounds(_bits & 2); }
    interval_bounds both ()const { return interval_bounds(_bits & 3); }
    
public:
    bound_type _bits;
};

inline interval_bounds open_()      { return interval_bounds(open_bounded);  }
inline interval_bounds left_open_() { return interval_bounds(left_open);     }
inline interval_bounds right_open_(){ return interval_bounds(right_open);    }
inline interval_bounds closed_()    { return interval_bounds(closed_bounded);}

inline interval_bounds left(interval_bounds x1)
{ return interval_bounds(x1._bits & 1); }

inline interval_bounds right(interval_bounds x1)
{ return interval_bounds(x1._bits & 2); }

inline interval_bounds all(interval_bounds x1)
{ return interval_bounds(x1._bits & 2); }

inline bool operator == (const interval_bounds x1, const interval_bounds x2)
{ return x1._bits == x2._bits; }

inline interval_bounds operator & (interval_bounds x1, interval_bounds x2)
{ return interval_bounds(x1._bits & x2._bits); }

inline interval_bounds operator | (interval_bounds x1, interval_bounds x2)
{ return interval_bounds(x1._bits | x2._bits); }

// left shift (multiplies by 2^shift)
inline interval_bounds operator << (interval_bounds bounds, unsigned int shift)
{ return interval_bounds(bounds._bits << shift); }

// right shift (divides by 2^shift)
inline interval_bounds operator >> (interval_bounds bounds, unsigned int shift)
{ return interval_bounds(bounds._bits >> shift); }

inline interval_bounds operator ~ (interval_bounds x1)
{ return interval_bounds(all(~x1)); }

inline interval_bounds outer_bounds(interval_bounds x1, interval_bounds x2)
{ return left(x1) | right(x2); }

inline interval_bounds inner_bounds(interval_bounds x1, interval_bounds x2)
{ return ~(right(x1) | left(x2)); }

inline interval_bounds left_bounds(interval_bounds x1, interval_bounds x2)
{ return left(x1) | (left(x2) >> 1); }

inline interval_bounds right_bounds(interval_bounds x1, interval_bounds x2)
{ return (right(x1) <<1 ) | right(x2); }

inline interval_bounds left_subtract_bounds(interval_bounds x1, interval_bounds x2)
{ return right(x1) | ~(right(x2) << 1); }

inline interval_bounds right_subtract_bounds(interval_bounds x1, interval_bounds x2)
{ return left(x1) | ~(left(x2) >> 1); }




}} // namespace itl boost

#endif

