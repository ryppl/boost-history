//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_ATOMIC_DETAIL_NATIVE_ATOMIC_TYPES_HPP
#define BOOST_ACT_ATOMIC_DETAIL_NATIVE_ATOMIC_TYPES_HPP

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <boost/operators.hpp>

#include "atomic_operators.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename UnqualifiedType >
class atomic_type< UnqualifiedType
                 , typename ::boost::enable_if
                   <
                     mpl::and_
                     <
                       is_scalar< UnqualifiedType >
                     , mpl::not_< is_pointer< UnqualifiedType > >
                     >
                   >
                   ::type
                 >
{
public:
  typedef UnqualifiedType value_type;
public:
  atomic_type() {}
  atomic_type( UnqualifiedType const& source )
    : value_m( source )
  {
  }
public:
  atomic_type& operator =( UnqualifiedType const& source )
  {
    value_m = source;
    return *this;
  }
public:
  friend UnqualifiedType value_of( atomic_type const& target )
  {
    return target.value_m;
  }/*
public:
  operator UnqualifiedType() const
  {
    return value_m;
  }*/
private:
  friend UnqualifiedType volatile& detail_raw_value( atomic_type& target )
  {
    return target.value_m;
  }
private:
  UnqualifiedType volatile value_m;
};

// ToDo: Possibly allow indexing with atomic objects
template< typename UnqualifiedType >
class atomic_type< UnqualifiedType
                 , typename ::boost::enable_if
                   <
                     is_pointer< UnqualifiedType >
                   >
                   ::type
                 >
  : dereferenceable< atomic_type< UnqualifiedType >, UnqualifiedType* >
  , indexable< atomic_type< UnqualifiedType >
             , typename iterator_difference< UnqualifiedType >
             , typename remove_pointer< UnqualifiedType >::type&
             >
{
  typedef dereferenceable< atomic_type< UnqualifiedType >, UnqualifiedType* >
            dereferenceable_base;

  typedef indexable< atomic_type< UnqualifiedType >
                   , typename iterator_difference< UnqualifiedType >
                   , typename remove_pointer< UnqualifiedType >::type&
                   >
            indexable_base;
public:
  typedef UnqualifiedType value_type;
public:
  atomic_type() {}
  atomic_type( UnqualifiedType const& source )
    : value_m( source )
  {
  }
public:
  atomic_type& operator =( UnqualifiedType const& source )
  {
    value_m = source;
    return *this;
  }
public:
  using dereferenceable_base::operator ->;
  using indexable_base::operator [];
private:
  friend UnqualifiedType volatile& detail_raw_value( atomic_type& target )
  {
    return target.value_m;
  }
  friend UnqualifiedType value_of( atomic_type const& target )
  {
    return target.value_m;
  }/*
public:
  operator UnqualifiedType() const
  {
    return value_m;
  }*/
private:
  UnqualifiedType volatile value_m;
};

// ToDo: Make atomic array types and atomic member pointer types

}
}
}

#endif
