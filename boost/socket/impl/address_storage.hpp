// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma once
#endif

/// include guard
#ifndef BOOST_SOCKET_IMPL_ADDRESS_STORAGE_HPP
#define BOOST_SOCKET_IMPL_ADDRESS_STORAGE_HPP 1

#include "boost/config.hpp"
#include <cstddef>

namespace boost
{
  namespace socket
  {
    namespace impl
    {

      //! reserve storage for socket addresses
      struct address_storage
      {
        BOOST_STATIC_CONSTANT(std::size_t, len=128);
        unsigned char storage[len];
      };

    }// namespace
  }// namespace
}// namespace

#endif
