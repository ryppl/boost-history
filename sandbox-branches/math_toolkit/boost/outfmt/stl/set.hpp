// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_Set_HPP
#define BOOST_IOFM_STL_Set_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <set>

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 3, std::set,      boost::io::set_container_type )
   BOOST_IO_CLASSIFY_TYPE( 3, std::multiset, boost::io::set_container_type )

   // I/O support:

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     define BOOST_IOFM_ATD
#  else
#     define BOOST_IOFM_ATD , boost::io::containerfmt()
#  endif

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      // std::set

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, class Cmp, class Allocator
      >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >   & os,
         const std::set< KeyT, Cmp, Allocator > & s
      )
      {
         return( os << boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, class Cmp, class Allocator
      >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::set< KeyT, Cmp, Allocator >     & s
      )
      {
         return( is >> boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      // std::multiset

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, class Cmp, class Allocator
      >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT >        & os,
         const std::multiset< KeyT, Cmp, Allocator > & ms
      )
      {
         return( os << boost::io::formatob( ms BOOST_IOFM_ATD ));
      }

      template
      <
         typename CharT, class TraitsT,
         typename KeyT, class Cmp, class Allocator
      >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT >  & is,
         std::multiset< KeyT, Cmp, Allocator > & ms
      )
      {
         return( is >> boost::io::formatob( ms BOOST_IOFM_ATD ));
      }
#  else
      // std::set

      template< typename KeyT, class Cmp, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::set< KeyT, Cmp, Allocator > & s
      )
      {
         return( os << boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      template< typename KeyT, class Cmp, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::set< KeyT, Cmp, Allocator > & s
      )
      {
         return( is >> boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      // std::multiset

      template< typename KeyT, class Cmp, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::multiset< KeyT, Cmp, Allocator > & ms
      )
      {
         return( os << boost::io::formatob( ms BOOST_IOFM_ATD ));
      }

      template< typename KeyT, class Cmp, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::multiset< KeyT, Cmp, Allocator > & ms
      )
      {
         return( is >> boost::io::formatob( ms BOOST_IOFM_ATD ));
      }
#  endif

#  undef BOOST_IOFM_ATD
#endif
