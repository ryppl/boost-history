// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_STL_Pair_HPP
#define BOOST_IOFM_STL_Pair_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <utility>
#  include <boost/outfmt/detail/naryval.hpp>

   namespace boost { namespace io
   {
      // type deduction:

      namespace detail
      {
         BOOST_IO_CLASSIFY_TYPE_2( std::pair, boost::io::pair_type );
      }

      // naryval:

      /*
      BOOST_IO_NARY_PARAM2( std::pair, seperable_pair )

      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::type  getval( const std::pair< T1, T2 > & p )
      {
         return( detail::selector< n, T1, T2 >::value( p.first, p.second ));
      }

      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::ref_type
                                                 refval( std::pair< T1, T2 > & p )
      {
         return( detail::selector< n, T1, T2 >::ref( p.first, p.second ));
      }
      */
   }}

   // I/O support:

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      template
      <
         typename CharT, class TraitsT,
         typename T1, typename T2
      >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const std::pair< T1, T2 >            & p
      )
      {
         return( os << boost::io::formatob( p, boost::io::pairfmt()));
      }

      template
      <
         typename CharT, class TraitsT,
         typename T1, typename T2
      >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::pair< T1, T2 >                  & p
      )
      {
         return( is >> boost::io::formatob( p, boost::io::pairfmt()));
      }
#  else
      template< typename T1, typename T2 >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::pair< T1, T2 > & p
      )
      {
         return( os << boost::io::formatob( p, boost::io::pairfmt()));
      }

      template< typename T1, typename T2 >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::pair< T1, T2 > & p
      )
      {
         return( is >> boost::io::formatob( p, boost::io::pairfmt()));
      }
#  endif
#endif
