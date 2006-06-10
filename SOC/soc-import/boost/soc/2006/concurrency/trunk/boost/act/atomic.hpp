//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ATOMIC_HPP
#define BOOST_ACT_ATOMIC_HPP

#include <boost/operators.hpp>

namespace boost
{
namespace act
{

// ToDo: Encapsulate an active object if no direct atomic support
// ToDo: Make act like a qualifier
template< typename Type

// ToDo: Consider using acquire and release form (figure out wtf they are)
// ToDo: Make version for 64-bit integers
// ToDo: Make swap functionality
template< typename ArithmeticType >
class atomic_type
      <
        IntegralType
      , typename ::boost::enable_if
        <
          ::boost::is_integral< ArithmeticType >
        >
        ::type
      >
  : ::boost::operators< atomic_type< ArithmeticType > >
{
public:
  atomic_type()
  {
  }

  // ToDo: Use call_traits
  atomic_type( ArithmeticType source )
    : value_m( source )
  {
  }
public:
  friend atomic_type& operator ++( atomic_type& target )
  {
    InterlockedIncrement( &target.value_m );
    return *this;
  }

  friend atomic_type& operator --( atomic_type& target )
  {
    InterlockedDecrement( &target.value_m );
    return *this;
  }
public:
  // ToDo: Make forms of the following which work with other atomic types

  // ToDo: Adjust param type
  friend atomic_type& operator +=( atomic_type& target, ArithmeticType operand )
  {
    InterlockedExchangeAdd( &target.value_m, static_cast< long >( operand ) );
    return *this;
  }

  // ToDo: Adjust param type
  friend atomic_type& operator -=( atomic_type& target, ArithmeticType operand )
  {
    return target += -operand;
  }
public:
  friend bool operator ==( atomic_type const& left, atomic_type const& right )
  {
    return value_m == right.value_m;
  }
private:
  volatile long value_m;
};

}
}

#endif
