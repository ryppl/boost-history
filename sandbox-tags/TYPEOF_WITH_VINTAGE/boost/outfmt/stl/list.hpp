// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_List_HPP
#define BOOST_IOFM_STL_List_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <list>
#  if defined(__MWERKS__)
#     include <boost/mpl/int.hpp>
#  endif

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 2, std::list, boost::io::seq_container_type )

   // I/O support:

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     define BOOST_IOFM_ATD
#  else
#     define BOOST_IOFM_ATD , boost::io::containerfmt()
#  endif

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const std::list< T, Allocator >      & l
      )
      {
         return( os << boost::io::formatob( l BOOST_IOFM_ATD ));
      }

      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::list< T, Allocator >            & l
      )
      {
         return( is >> boost::io::formatob( l BOOST_IOFM_ATD ));
      }
#  else
      template< typename T, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::list< T, Allocator > & l
      )
      {
         return( os << boost::io::formatob( l BOOST_IOFM_ATD ));
      }

      template< typename T, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::list< T, Allocator > & l
      )
      {
         return( is >> boost::io::formatob( l BOOST_IOFM_ATD ));
      }
#  endif

#  undef BOOST_IOFM_ATD
#endif
