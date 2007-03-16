// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_HashMap_HPP
#define BOOST_IOFM_STL_HashMap_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  if !defined(BOOST_IOFM_HASH_CONTAINERS)
#     error STL implementation does not support the <hash_map> extension.
#  endif

#  if defined(BOOST_IO_NO_DEPRECATED_MODIFIER) && defined(BOOST_DINKUMWARE_STDLIB)
#     define _DEPRECATED_TEMP _DEPRECATED
#     define _DEPRECATED
#  endif
#     include <hash_map>
#  if defined(BOOST_IO_NO_DEPRECATED_MODIFIER) && defined(BOOST_DINKUMWARE_STDLIB)
#     define _DEPRECATED _DEPRECATED_TEMP
#  endif

   // type deduction:

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
#     if defined(BOOST_DINKUMWARE_STDLIB)
         BOOST_IO_CLASSIFY_TYPE( 4, BOOST_IOFM_STDEXT::hash_map,      boost::io::assoc_container_type )
         BOOST_IO_CLASSIFY_TYPE( 4, BOOST_IOFM_STDEXT::hash_multimap, boost::io::assoc_container_type )
#     else // SGI containers
         BOOST_IO_CLASSIFY_TYPE( 5, BOOST_IOFM_STDEXT::hash_map,      boost::io::assoc_container_type )
         BOOST_IO_CLASSIFY_TYPE( 5, BOOST_IOFM_STDEXT::hash_multimap, boost::io::assoc_container_type )
#     endif
#  endif

   // I/O support:

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     define BOOST_IOFM_ATD
#  else
#     define BOOST_IOFM_ATD , boost::io::containerfmt( boost::io::pairfmt())
#  endif

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      // stdext::hash_map

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const BOOST_IOFM_STDEXT::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return( os << boost::io::formatob( m BOOST_IOFM_ATD  ));
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         BOOST_IOFM_STDEXT::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return( is >> boost::io::formatob( m BOOST_IOFM_ATD ));
      }

      // stdext::hash_multimap

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const BOOST_IOFM_STDEXT::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return( os << boost::io::formatob( mm BOOST_IOFM_ATD ));
      }

      template< typename CharT, class TraitsT, BOOST_IOFM_HASH_MAP_T >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         BOOST_IOFM_STDEXT::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return( is >> boost::io::formatob( mm BOOST_IOFM_ATD ));
      }
#  else
      // stdext::hash_map

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const BOOST_IOFM_STDEXT::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return( os << boost::io::formatob( m BOOST_IOFM_ATD ));
      }

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::istream & operator>>
      (
         std::istream & is,
         BOOST_IOFM_STDEXT::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return( is >> boost::io::formatob( m BOOST_IOFM_ATD ));
      }

      // stdext::hash_multimap 

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const BOOST_IOFM_STDEXT::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return( os << boost::io::formatob( mm BOOST_IOFM_ATD ));
      }

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::istream & operator>>
      (
         std::istream & is,
         BOOST_IOFM_STDEXT::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return( is >> boost::io::formatob( mm BOOST_IOFM_ATD ));
      }
#  endif

#  undef BOOST_IOFM_ATD
#endif
