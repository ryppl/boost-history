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
#ifndef BOOST_SOCKET_IMPL_INITIALISER_HPP
#define BOOST_SOCKET_IMPL_INITIALISER_HPP 1

namespace boost
{
  namespace socket
  {
    namespace impl
    {
      class initialiser
      {
      public:
        static const initialiser& uses_platform();
        ~initialiser();
      private:
        static void initialise();
        static void finalise();
        static bool m_initialised;
      };

    }// namespace
  }// namespace
}// namespace

#endif
