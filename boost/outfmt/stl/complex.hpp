// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_STL_Complex_HPP
#define BOOST_IOFM_STL_Complex_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <complex>

   namespace boost { namespace io
   {
      // type deduction:

      namespace detail
      {
         BOOST_IO_CLASSIFY_TYPE_1( std::complex, boost::io::nary2value_type );
      }

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
