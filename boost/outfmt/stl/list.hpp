// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_STL_List_HPP
#define BOOST_IOFM_STL_List_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <list>

   // type deduction:

   namespace boost { namespace io { namespace detail
   {
      BOOST_IO_CLASSIFY_TYPE_2( std::list, boost::io::seq_container_type );
   }}}

   // I/O support:

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const std::list< T, Allocator >      & l
      )
      {
         return( os << boost::io::formatob( l, boost::io::containerfmt()));
      }

      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::list< T, Allocator >            & l
      )
      {
         return( is >> boost::io::formatob( l, boost::io::containerfmt()));
      }
#  else
      template< typename T, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::list< T, Allocator > & l
      )
      {
         return( os << boost::io::formatob( l, boost::io::containerfmt()));
      }

      template< typename T, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::list< T, Allocator > & l
      )
      {
         return( is >> boost::io::formatob( l, boost::io::containerfmt()));
      }
#  endif
#endif
