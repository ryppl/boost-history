// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_BOOST_Rational_HPP
#define BOOST_IOFM_BOOST_Rational_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/rational.hpp>

   namespace boost { namespace io
   {
      // type deduction:

      namespace detail
      {
         BOOST_IO_CLASSIFY_TYPE_1( boost::rational, boost::io::nary2int_type );
      }

      // naryval:

      BOOST_IO_NARY_PARAM1( boost::rational, inseperable_pair< T > )

      template< int n, typename T >
      T getval( const boost::rational< T > & r )
      {
         return( detail::selector< n, T >::value( r.numerator(), r.denominator()));
      }

      template< typename T >
      void assignval( boost::rational< T > & r, const T & a, const T & b )
      {
         r = boost::rational< T >( a, b );
      }
   }}
#endif
