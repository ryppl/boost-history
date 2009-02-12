//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __BOOST_DEFAULTED_FUNCTIONS__HPP
#define __BOOST_DEFAULTED_FUNCTIONS__HPP

#include <memory>
#include <boost/config.hpp>

#if defined(BOOST_HAS_DEFAULTED_FUNCTIONS)

    #define BOOST_DEFAULT_CONSTRUCTOR_DEFAULT(T)        \
      T()=default;                                      \

    #define BOOST_COPY_ASSIGNEMENT_DELETE(T)            \
      T& operator=(const T& rhs)=default;               \

#else // !defined(BOOST_HAS_DEFAULTED_FUNCTIONS)

    #define BOOST_DEFAULT_CONSTRUCTOR_DEFAULT(T)        \
      inline T(){};                                     \

    #define BOOST_COPY_ASSIGNEMENT_DELETE(T)            \
      inline T& operator=(const T& rhs) {               \
        T tmp(rhs);                                     \
        swap(tmp);\
        return this;\
      }

#endif // !defined(BOOST_HAS_DEFAULTED_FUNCTIONS)