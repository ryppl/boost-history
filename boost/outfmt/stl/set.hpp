// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_STL_Set_HPP
#define BOOST_IOFM_STL_Set_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <set>

   // type deduction:

   namespace boost { namespace io { namespace detail
   {
      BOOST_IO_CLASSIFY_TYPE_3( std::set,      boost::io::set_container_type );
      BOOST_IO_CLASSIFY_TYPE_3( std::multiset, boost::io::set_container_type );
   }}}

   // I/O support:

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
         return( os << boost::io::formatob( s, boost::io::containerfmt()));
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
         return( is >> boost::io::formatob( s, boost::io::containerfmt()));
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
         return( os << boost::io::formatob( ms, boost::io::containerfmt()));
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
         return( is >> boost::io::formatob( ms, boost::io::containerfmt()));
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
         return( os << boost::io::formatob( s, boost::io::containerfmt()));
      }

      template< typename KeyT, class Cmp, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::set< KeyT, Cmp, Allocator > & s
      )
      {
         return( is >> boost::io::formatob( s, boost::io::containerfmt()));
      }

      // std::multiset

      template< typename KeyT, class Cmp, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::multiset< KeyT, Cmp, Allocator > & ms
      )
      {
         return( os << boost::io::formatob( ms, boost::io::containerfmt()));
      }

      template< typename KeyT, class Cmp, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::multiset< KeyT, Cmp, Allocator > & ms
      )
      {
         return( is >> boost::io::formatob( ms, boost::io::containerfmt()));
      }
#  endif
#endif