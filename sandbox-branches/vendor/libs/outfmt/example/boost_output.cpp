// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>  // std::cout

#include <boost/outfmt/formatob.hpp>
#include <boost/outfmt/boost/compressed_pair.hpp>
#if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
#  include <boost/outfmt/boost/interval.hpp>
#endif
#if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
#  include <boost/outfmt/boost/quaternion.hpp>
#endif
#if !defined(BOOST_IOFM_NO_LIB_OCTONIAN)
#  include <boost/outfmt/boost/octonion.hpp>
#endif
#if !defined(BOOST_IOFM_NO_LIB_RATIONAL)
#  include <boost/outfmt/boost/rational.hpp>
#endif

int main()
{
   // boost examples

#  if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
      boost::numeric::interval< int >  i(    10, 20 );
#  endif
#  if !defined(BOOST_IOFM_NO_LIB_RATIONAL)
      boost::rational< int >           rat(   2,  3 );
#  endif
   boost::compressed_pair< char, int > cp(  'A', 10 );

   // formatter configurations

   boost::io::pairfmt_t< char >        pairfmt1; // [format]: (a,b)

   // output boost types

#  if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
      std::cout << "interval< int > = "
                << boost::io::formatob( i, boost::io::pairfmt())
                << '\n';

      // [results]: interval< int > = ( 10, 20 )
#  endif

#  if !defined(BOOST_IOFM_NO_LIB_RATIONAL)
      std::cout << "rational< int > = "
                << boost::io::formatob( rat, pairfmt1 )
                << '\n';

      // [results]: rational< int > = (2,3)
#  endif

   std::cout << "compressed_pair< char, int > = "
             << boost::io::formatob( cp, pairfmt1 )
             << '\n' << '\n';

   // [results]: compressed_pair< char, int > = (A,10)

#  if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
      boost::math::quaternion< float > h( 0.1f, 0.2f, 0.3f, 0.4f );

      std::cout << "quaternion< float > = "
                << boost::io::formatob( h, boost::io::naryfmt())
                << '\n';

      // [results]: quaternion< float > = ( 0.1, 0.2, 0.3, 0.4 )
#  endif

#  if !defined(BOOST_IOFM_NO_LIB_OCTONION)
      boost::math::octonion< float >   o( 0.1f, 0.2f, 0.3f, 0.4f, 1.1f, 1.2f, 1.3f, 1.4f );

      std::cout << "octonion< float > = "
                << boost::io::formatob( o, boost::io::naryfmt())
                << '\n';

      // [results]: octonion< float > = ( 0.1, 0.2, 0.3, 0.4, 1.1, 1.2, 1.3, 1.4 )
#  endif

   return( 0 );
}
