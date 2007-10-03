// Copyright (C) 2007 Steven Watanabe, Joseph Gauterin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org


#ifndef BOOST_UTILITY_SWAP_HPP
#define BOOST_UTILITY_SWAP_HPP

#include <algorithm> //for std::swap

namespace boost_swap_impl
{
  template<class T>
  void swap_impl(T& left, T& right)
  {
    using std::swap;//use std::swap if argument dependent lookup fails
    swap(left,right);
  }
}

namespace boost
{
  namespace swap_adl_barrier
  {
    template<class T>
    void swap(T& left, T& right)
    {
      ::boost_swap_impl::swap_impl(left, right);
    }
  }

  using swap_adl_barrier::swap;
}

#endif
