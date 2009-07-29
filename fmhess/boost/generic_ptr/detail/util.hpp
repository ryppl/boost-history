//
//  generic_ptr/detail/util.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GENERIC_PTR_DETAIL_UTIL_HPP_INCLUDED
#define BOOST_GENERIC_PTR_DETAIL_UTIL_HPP_INCLUDED

namespace boost
{
  namespace generic_ptr
  {
    namespace detail
    {
      struct static_cast_tag {};
      struct const_cast_tag {};
      struct dynamic_cast_tag {};

      template<typename T>
      void set_plain_old_pointer_to_null(T * &p)
      {
        p = 0;
      }
      template<typename T>
      void set_plain_old_pointer_to_null(const T&)
      {}
    } // namespace detail
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_DETAIL_UTIL_HPP_INCLUDED
