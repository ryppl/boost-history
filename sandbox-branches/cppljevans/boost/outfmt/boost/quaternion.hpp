// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_BOOST_MathQuaternion_HPP
#define BOOST_IOFM_BOOST_MathQuaternion_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/math/quaternion.hpp>

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 1, boost::math::quaternion, boost::io::pair_type )

   namespace boost { namespace io
   {
      // naryval:

      BOOST_IO_NARY_PARAM1( boost::math::quaternion, nary4_type< T > )

      template< int n, typename T >
      T getval( const boost::math::quaternion< T > & h )
      {
         return( detail::selector< n, T >::value
         (
            h.R_component_1(), h.R_component_2(),
            h.R_component_3(), h.R_component_4()
         ));
      }

      template< typename T >
      void assignval( boost::math::quaternion< T > & q, const T & a, const T & b, const T & c, const T & d )
      {
         q = boost::math::quaternion< T >( a, b, c, d );
      }
   }}
#endif
