// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_BOOST_MathOctonion_HPP
#define BOOST_IOFM_BOOST_MathOctonion_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/math/octonion.hpp>

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 1, boost::math::octonion, boost::io::pair_type )

   namespace boost { namespace io
   {
      // naryval:

      BOOST_IO_NARY_PARAM1( boost::math::octonion, nary8_type< T > )

      template< int n, typename T >
      T getval( const boost::math::octonion< T > & o )
      {
         return( detail::selector< n, T >::value
         (
            o.R_component_1(), o.R_component_2(), o.R_component_3(), o.R_component_4(),
            o.R_component_5(), o.R_component_6(), o.R_component_7(), o.R_component_8()
         ));
      }

      template< typename T >
      void assignval( boost::math::octonion< T > & o, const T & a, const T & b, const T & c, const T & d, const T & e, const T & f, const T & g, const T & h )
      {
         o = boost::math::octonion< T >( a, b, c, d, e, f, g, h );
      }
   }}
#endif
