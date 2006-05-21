// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_Map_HPP
#define BOOST_IOFM_STL_Map_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <map>

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 4, std::map,      boost::io::assoc_container_type )
   BOOST_IO_CLASSIFY_TYPE( 4, std::multimap, boost::io::assoc_container_type )

   // I/O support:

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     define BOOST_IOFM_ATD
#  else
#     define BOOST_IOFM_ATD , boost::io::containerfmt( boost::io::pairfmt())
#  endif

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      // std::map

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, typename T, class Cmp, class Allocator
      >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >      & os,
         const std::map< KeyT, T, Cmp, Allocator > & m
      )
      {
         return( os << boost::io::formatob( m BOOST_IOFM_ATD ));
      }

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, typename T, class Cmp, class Allocator
      >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::map< KeyT, T, Cmp, Allocator >  & m
      )
      {
         return( is >> boost::io::formatob( m BOOST_IOFM_ATD ));
      }

      // std::multimap

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, typename T, class Cmp, class Allocator
      >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >           & os,
         const std::multimap< KeyT, T, Cmp, Allocator > & mm
      )
      {
         return( os << boost::io::formatob( mm BOOST_IOFM_ATD ));
      }

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, typename T, class Cmp, class Allocator
      >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT >     & is,
         std::multimap< KeyT, T, Cmp, Allocator > & mm
      )
      {
         return( is >> boost::io::formatob( mm BOOST_IOFM_ATD ));
      }
#  else
      // std::map

      template< typename KeyT, typename T, class Cmp, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::map< KeyT, T, Cmp, Allocator > & m
      )
      {
         return( os << boost::io::formatob( m BOOST_IOFM_ATD ));
      }

      template< typename KeyT, typename T, class Cmp, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::map< KeyT, T, Cmp, Allocator > & m
      )
      {
         return( is >> boost::io::formatob( m BOOST_IOFM_ATD ));
      }

      // std::multimap

      template< typename KeyT, typename T, class Cmp, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::multimap< KeyT, T, Cmp, Allocator > & mm
      )
      {
         return( os << boost::io::formatob( mm BOOST_IOFM_ATD ));
      }

      template< typename KeyT, typename T, class Cmp, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::multimap< KeyT, T, Cmp, Allocator > & mm
      )
      {
         return( is >> boost::io::formatob( mm BOOST_IOFM_ATD ));
      }
#  endif

#  undef BOOST_IOFM_ATD
#endif
