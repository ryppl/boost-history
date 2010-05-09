/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_BOUNDS_HPP_JOFA_100330
#define BOOST_ITL_INTERVAL_BOUNDS_HPP_JOFA_100330

#include <boost/utility/enable_if.hpp>
#include <boost/itl/type_traits/has_dynamic_bounds.hpp>

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
    BOOST_STATIC_CONSTANT(bound_type, _open      = 0);
    BOOST_STATIC_CONSTANT(bound_type, _leftopen  = 1);
    BOOST_STATIC_CONSTANT(bound_type, _rightopen = 2);
    BOOST_STATIC_CONSTANT(bound_type, _closed    = 3);

    BOOST_STATIC_CONSTANT(bound_type, _right     = 1);
    BOOST_STATIC_CONSTANT(bound_type, _left      = 2);
    BOOST_STATIC_CONSTANT(bound_type, _all       = 3);

public:
    interval_bounds():_bits(){}
    explicit interval_bounds(bound_type bounds): _bits(bounds){}
    interval_bounds all  ()const { return interval_bounds(_bits & _all  ); }
    interval_bounds left ()const { return interval_bounds(_bits & _left ); }
    interval_bounds right()const { return interval_bounds(_bits & _right); }
    interval_bounds reverse_left ()const { return interval_bounds((~_bits>>1) & _right); }
    interval_bounds reverse_right()const { return interval_bounds((~_bits<<1) & _left ); }

    bound_type bits()const{ return _bits; }

    static interval_bounds open()      { return interval_bounds(_open);     }
    static interval_bounds left_open() { return interval_bounds(_leftopen); }
    static interval_bounds right_open(){ return interval_bounds(_rightopen);}
    static interval_bounds closed()    { return interval_bounds(_closed);   }

public:
    bound_type _bits;
};


inline interval_bounds left(interval_bounds x1)
{ return interval_bounds(x1._bits & interval_bounds::_left); }

inline interval_bounds right(interval_bounds x1)
{ return interval_bounds(x1._bits & interval_bounds::_right); }

inline interval_bounds all(interval_bounds x1)
{ return interval_bounds(x1._bits & interval_bounds::_all); }

inline bool operator == (const interval_bounds x1, const interval_bounds x2)
{ return x1._bits == x2._bits; }

inline bool operator != (const interval_bounds x1, const interval_bounds x2)
{ return x1._bits != x2._bits; }

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
{ return all(interval_bounds(~(x1._bits))); }

inline interval_bounds outer_bounds(interval_bounds x1, interval_bounds x2)
{ return left(x1) | right(x2); }

inline interval_bounds inner_bounds(interval_bounds x1, interval_bounds x2)
{ return interval_bounds(x1.reverse_right() | x2.reverse_left()); }

inline interval_bounds left_bounds(interval_bounds x1, interval_bounds x2)
{ return left(x1) | (left(x2) >> 1); }

inline interval_bounds right_bounds(interval_bounds x1, interval_bounds x2)
{ return (right(x1) <<1 ) | right(x2); }

inline interval_bounds left_subtract_bounds(interval_bounds x1, interval_bounds x2)
{ return right(x1) | ~(right(x2) << 1); }

inline interval_bounds right_subtract_bounds(interval_bounds x1, interval_bounds x2)
{ return left(x1) | ~(left(x2) >> 1); }

inline bool is_complementary(interval_bounds x1)
{ return x1 == interval_bounds::right_open() || x1 == interval_bounds::left_open(); }

inline std::string left_bracket(interval_bounds bounds)
{ return bounds.left().bits()==2 ? "[" : "("; }

inline std::string right_bracket(interval_bounds bounds)
{ return bounds.right().bits()==1 ? "]" : ")"; }

template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits> &stream, 
   interval_bounds const& object)
{
    return stream << "'" << left_bracket(object) << right_bracket(object) << "'";
}



template<class IntervalT>
inline typename 
boost::enable_if<has_dynamic_bounds<IntervalT>, interval_bounds>::type
outer_bounds(const IntervalT& x1, const IntervalT& x2)
{ return outer_bounds(x1.bounds(), x2.bounds()); }

template<class IntervalT>
inline typename 
boost::enable_if<has_dynamic_bounds<IntervalT>, interval_bounds>::type
inner_bounds(const IntervalT& x1, const IntervalT& x2)
{ return inner_bounds(x1.bounds(), x2.bounds()); }

template<class IntervalT>
inline typename 
boost::enable_if<has_dynamic_bounds<IntervalT>, interval_bounds>::type
left_bounds(const IntervalT& x1, const IntervalT& x2)
{ return left_bounds(x1.bounds(), x2.bounds()); }

template<class IntervalT>
inline typename 
boost::enable_if<has_dynamic_bounds<IntervalT>, interval_bounds>::type
right_bounds(const IntervalT& x1, const IntervalT& x2)
{ return right_bounds(x1.bounds(), x2.bounds()); }

template<class IntervalT>
inline typename 
boost::enable_if<has_dynamic_bounds<IntervalT>, interval_bounds>::type
left_subtract_bounds(const IntervalT& x1, const IntervalT& x2)
{ return left_subtract_bounds(x1.bounds(), x2.bounds()); }

template<class IntervalT>
inline typename 
boost::enable_if<has_dynamic_bounds<IntervalT>, interval_bounds>::type
right_subtract_bounds(const IntervalT& x1, const IntervalT& x2)
{ return right_subtract_bounds(x1.bounds(), x2.bounds()); }


template<class DomainT, ITL_COMPARE Compare>
class bounded_value
{
public:
    typedef DomainT domain_type;
    typedef bounded_value<DomainT,Compare> type;
public:
    bounded_value(const domain_type& value, interval_bounds bound)
        : _value(value), _bound(bound) {}

    domain_type     value()const { return _value; }
    interval_bounds bound()const { return _bound; }

private:
    domain_type     _value;
    interval_bounds _bound;
};

}} // namespace itl boost

#endif

