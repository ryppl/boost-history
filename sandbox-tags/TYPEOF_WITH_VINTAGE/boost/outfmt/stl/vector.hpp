// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_Vector_HPP
#define BOOST_IOFM_STL_Vector_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <vector>

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 2, std::vector, boost::io::seq_container_type )

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
         const std::vector< T, Allocator >    & v
      )
      {
         return( os << boost::io::formatob( v BOOST_IOFM_ATD ));
      }

      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::vector< T, Allocator >          & v
      )
      {
         return( is >> boost::io::formatob( v BOOST_IOFM_ATD ));
      }
#  else
      template< typename T, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::vector< T, Allocator > & v
      )
      {
         return( os << boost::io::formatob( v BOOST_IOFM_ATD ));
      }

      template< typename T, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::vector< T, Allocator > & v
      )
      {
         return( is >> boost::io::formatob( v BOOST_IOFM_ATD ));
      }
#  endif

#  undef BOOST_IOFM_ATD
#endif
