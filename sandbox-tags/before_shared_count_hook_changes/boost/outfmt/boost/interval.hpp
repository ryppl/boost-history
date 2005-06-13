// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_BOOST_Interval_HPP
#define BOOST_IOFM_BOOST_Interval_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/numeric/interval.hpp>


   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 2, boost::numeric::interval, boost::io::nary2base_type )

   namespace boost { namespace io
   {
      // naryval:

      BOOST_IO_NARY_PARAM2( boost::numeric::interval, inseperable_pair< T > )

      template< int n, typename T, class Traits >
      T getval( const boost::numeric::interval< T, Traits > & i )
      {
         return( detail::selector< n, T >::value( i.lower(), i.upper()));
      }

      template< typename T, class Traits >
      void assignval( boost::numeric::interval< T, Traits > & i, const T & a, const T & b )
      {
         i = boost::numeric::interval< T, Traits >( a, b );
      }
   }}
#endif
