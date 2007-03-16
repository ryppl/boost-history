// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_Complex_HPP
#define BOOST_IOFM_STL_Complex_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <complex>

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 1, std::complex, boost::io::nary2value_type )

   namespace boost { namespace io
   {
      // naryval:

      BOOST_IO_NARY_PARAM1( std::complex, inseperable_pair< T > )

      template< int n, typename T >
      T getval( const std::complex< T > & c )
      {
         return( detail::selector< n, T >::value( c.real(), c.imag()));
      }

      template< typename T >
      void assignval( std::complex< T > & c, const T & a, const T & b )
      {
         c = std::complex< T >( a, b );
      }
   }}
#endif
