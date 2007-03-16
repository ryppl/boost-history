// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_HashSet_HPP
#define BOOST_IOFM_STL_HashSet_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  if !defined(BOOST_IOFM_HASH_CONTAINERS)
#     error STL implementation does not support the <hash_set> extension.
#  endif

#  if defined(BOOST_IO_NO_DEPRECATED_MODIFIER) && defined(BOOST_DINKUMWARE_STDLIB)
#     define _DEPRECATED_TEMP _DEPRECATED
#     define _DEPRECATED
#  endif
#     include <hash_set>
#  if defined(BOOST_IO_NO_DEPRECATED_MODIFIER) && defined(BOOST_DINKUMWARE_STDLIB)
#     define _DEPRECATED _DEPRECATED_TEMP
#  endif

   // type deduction:

#  if defined(BOOST_DINKUMWARE_STDLIB)
      BOOST_IO_CLASSIFY_TYPE( 3, BOOST_IOFM_STDEXT::hash_set,      boost::io::set_container_type )
      BOOST_IO_CLASSIFY_TYPE( 3,BOOST_IOFM_STDEXT::hash_multiset, boost::io::set_container_type )
#  else // SGI containers
      BOOST_IO_CLASSIFY_TYPE( 4, BOOST_IOFM_STDEXT::hash_set,   boost::io::set_container_type )
      BOOST_IO_CLASSIFY_TYPE( 4, BOOST_IOFM_STDEXT::hash_multiset, boost::io::set_container_type )
#     endif

   // I/O support:

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     define BOOST_IOFM_ATD
#  else
#     define BOOST_IOFM_ATD , boost::io::containerfmt()
#  endif

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      // stdext::hash_set

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const BOOST_IOFM_STDEXT::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( os << boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         BOOST_IOFM_STDEXT::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( is >> boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      // stdext::hash_multiset

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const BOOST_IOFM_STDEXT::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( os << boost::io::formatob( ms BOOST_IOFM_ATD ));
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_SET_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         BOOST_IOFM_STDEXT::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( is >> boost::io::formatob( ms BOOST_IOFM_ATD ));
      }
#  else
      // stdext::hash_set

      template< BOOST_IOFM_HASH_SET_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const BOOST_IOFM_STDEXT::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( os << boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      template< BOOST_IOFM_HASH_SET_T >
      inline std::istream & operator>>
      (
         std::istream & is,
         BOOST_IOFM_STDEXT::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( is >> boost::io::formatob( s BOOST_IOFM_ATD ));
      }

      // stdext::hash_multiset

      template< BOOST_IOFM_HASH_SET_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const BOOST_IOFM_STDEXT::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( os << boost::io::formatob( ms BOOST_IOFM_ATD ));
      }

      template< BOOST_IOFM_HASH_SET_T >
      inline std::istream & operator>>
      (
         std::istream & is,
         BOOST_IOFM_STDEXT::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( is >> boost::io::formatob( ms BOOST_IOFM_ATD ));
      }
#  endif

#  undef BOOST_IOFM_ATD
#endif
