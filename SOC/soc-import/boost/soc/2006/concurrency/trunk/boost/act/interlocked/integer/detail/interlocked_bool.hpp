/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INTEGER_DETAIL_INTERLOCKED_BOOL_HPP
#define BOOST_ACT_INTERLOCKED_INTEGER_DETAIL_INTERLOCKED_BOOL_HPP

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename InternalType >
class interlocked_bool
{
  template< typename >
  friend class interlocked_bool;
public:
  typedef InternalType internal_type;
public:
  interlocked_bool() {}
  interlocked_bool( bool value_init ) : value_m( value_init ) {}
public:
// Note: The following function produces a warning in msvc++
/*
  interlocked_bool( interlocked_bool const& source )
    : value_m( source.value_m ) {}
*/

  interlocked_bool( interlocked_bool const volatile& source )
    : value_m( source.value_m ) {}
public:
  template< typename OtherInternalType >
  interlocked_bool( interlocked_bool< OtherInternalType > const& source )
    : value_m( source.value_m ) {}

  template< typename OtherInternalType >
  interlocked_bool
  ( interlocked_bool< OtherInternalType > const volatile& source )
    : value_m( source.value_m ) {}
public:
  interlocked_bool& operator =( bool value_new )
  {
    value_m = value_new;
    return *this;
  }

  interlocked_bool volatile& operator =( bool value_new ) volatile
  {
    value_m = value_new;
    return *this;
  }
public:

// Note: The following function produces a warning in msvc++
/*
  interlocked_bool& operator =( interlocked_bool const& value_new )
  {
    value_m = value_new;
    return *this;
  }
*/

// Note: The following function produces a warning in msvc++
/*
  interlocked_bool& operator =( interlocked_bool volatile const& value_new )
  {
    value_m = value_new;
    return *this;
  }
*/
public:
  interlocked_bool volatile&
  operator =( interlocked_bool const& value_new ) volatile
  {
    value_m = value_new;
    return *this;
  }

// Note: The following function produces a warning in msvc++
/*
  interlocked_bool volatile&
  operator =( interlocked_bool volatile const& value_new ) volatile
  {
    value_m = value_new;
    return *this;
  }
*/
public:
  template< typename OtherInternalType >
  interlocked_bool& operator =
  ( interlocked_bool< OtherInternalType > const& value_new )
  {
    value_m = value_new.value_m;
    return *this;
  }

  template< typename OtherInternalType >
  interlocked_bool& operator =
  ( interlocked_bool< OtherInternalType > volatile const& value_new )
  {
    value_m = value_new.value_m;
    return *this;
  }
public:
  template< typename OtherInternalType >
  interlocked_bool volatile&
  operator =( interlocked_bool< OtherInternalType > const& value_new ) volatile
  {
    value_m = value_new.value_m;
    return *this;
  }

  template< typename OtherInternalType >
  interlocked_bool volatile&
  operator =
  ( interlocked_bool< OtherInternalType > volatile const& value_new ) volatile
  {
    value_m = value_new.value_m;
    return *this;
  }
public:
  operator bool() const
  {
    return value_m != 0;
  }

  operator bool() volatile const
  {
    return value_m != 0;
  }
private:
  friend bool operator ==( interlocked_bool const& left
                         , interlocked_bool const& right
                         )
  {
    return left.value_m == right.value_m;
  }

  friend bool operator ==( interlocked_bool const volatile& left
                         , interlocked_bool const& right
                         )
  {
    return left.value_m == right.value_m;
  }

  friend bool operator ==( interlocked_bool const& left
                         , interlocked_bool const volatile& right
                         )
  {
    return left.value_m == right.value_m;
  }

  friend bool operator ==( interlocked_bool const volatile& left
                         , interlocked_bool const volatile& right
                         )
  {
    return left.value_m == right.value_m;
  }
private:
  friend bool operator !=( interlocked_bool const& left
                         , interlocked_bool const& right
                         )
  {
    return left != right;
  }

  friend bool operator !=( interlocked_bool const volatile& left
                         , interlocked_bool const& right
                         )
  {
    return left != right;
  }

  friend bool operator !=( interlocked_bool const& left
                         , interlocked_bool const volatile& right
                         )
  {
    return left != right;
  }

  friend bool operator !=( interlocked_bool const volatile& left
                         , interlocked_bool const volatile& right
                         )
  {
    return left != right;
  }
private:
  friend
  InternalType& interlocked_bool_internal_address( interlocked_bool& target )
  {
    return target.value_m;
  }

  friend
  InternalType const&
  interlocked_bool_internal_address( interlocked_bool const& target )
  {
    return target.value_m;
  }
private:
  friend
  InternalType volatile&
  interlocked_bool_internal_value( interlocked_bool volatile& target )
  {
    return target.value_m;
  }

  friend
  InternalType const volatile&
  interlocked_bool_internal_value( interlocked_bool const volatile& target )
  {
    return target.value_m;
  }
private:
  InternalType value_m;
};

template< typename Type >
struct is_interlocked_bool : mpl::false_ {};

template< typename InternalType >
struct is_interlocked_bool< interlocked_bool< InternalType > >
  : mpl::true_ {};

template< typename InternalType >
struct is_interlocked_bool< interlocked_bool< InternalType > const >
  : mpl::true_ {};

template< typename InternalType >
struct is_interlocked_bool< interlocked_bool< InternalType > volatile >
  : mpl::true_ {};

template< typename InternalType >
struct is_interlocked_bool< interlocked_bool< InternalType > const volatile >
  : mpl::true_ {};

} } } } // namespace boost

#endif
