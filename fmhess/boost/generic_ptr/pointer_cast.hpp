//
//  generic_ptr/pointer_cast.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_POINTER_CAST_HPP_INCLUDED
#define BOOST_GENERIC_PTR_POINTER_CAST_HPP_INCLUDED

#include <boost/generic_ptr/pointer_traits.hpp>

namespace boost
{
  namespace generic_ptr
  {
    // two-argument cast overloads for raw pointers
    template<typename T, typename U>
    T* static_pointer_cast(U *r, boost::mpl::identity<T>)
    {
      return static_cast<T*>(r);
    }
    template<typename T, typename U>
    T* const_pointer_cast(U *r, boost::mpl::identity<T>)
    {
      return const_cast<T*>(r);
    }
    template<typename T, typename U>
    T* dynamic_pointer_cast(U *r, boost::mpl::identity<T>)
    {
      return dynamic_cast<T*>(r);
    }

    template<typename ToValueType, typename U>
    typename rebind<U, ToValueType>::other static_pointer_cast(const U &r)
    {
      return static_pointer_cast(r, mpl::identity<ToValueType>());
    }
    template<typename ToValueType, typename U>
    typename rebind<U, ToValueType>::other const_pointer_cast(const U &r)
    {
      return const_pointer_cast(r, mpl::identity<ToValueType>());
    }
    template<typename ToValueType, typename U>
    typename rebind<U, ToValueType>::other dynamic_pointer_cast(const U &r)
    {
      return dynamic_pointer_cast(r, mpl::identity<ToValueType>());
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_POINTER_CAST_HPP_INCLUDED
