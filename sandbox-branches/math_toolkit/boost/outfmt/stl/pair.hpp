// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_Pair_HPP
#define BOOST_IOFM_STL_Pair_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <utility>
#  include <boost/outfmt/detail/naryval.hpp>

   // type deduction:
   BOOST_IO_CLASSIFY_TYPE( 2, std::pair, boost::io::pair_type )

   namespace boost { namespace io
   {
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

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     define BOOST_IOFM_ATD
#  else
#     define BOOST_IOFM_ATD , boost::io::pairfmt()
#  endif

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
         return( os << boost::io::formatob( p BOOST_IOFM_ATD ));
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
         return( is >> boost::io::formatob( p BOOST_IOFM_ATD ));
      }
#  else
      template< typename T1, typename T2 >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::pair< T1, T2 > & p
      )
      {
         return( os << boost::io::formatob( p BOOST_IOFM_ATD ));
      }

      template< typename T1, typename T2 >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::pair< T1, T2 > & p
      )
      {
         return( is >> boost::io::formatob( p BOOST_IOFM_ATD ));
      }
#  endif

#  undef BOOST_IOFM_ATD
#endif
