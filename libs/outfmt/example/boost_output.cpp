// (C) Copyright 2003: Reece H. Dunn

#include <iostream>  // std::cout

#include <boost/outfmt/formatob.hpp>
#if !defined(BOOST_IOFM__NO_LIB_INTERVAL)
#  include <boost/numeric/interval.hpp>
#endif

int main()
{
   // boost examples

#  if !defined(BOOST_IOFM__NO_LIB_INTERVAL)
      boost::numeric::interval< int >  i(    10, 20 );
#  endif
#  if !defined(BOOST_IOFM__NO_LIB_RATIONAL)
      boost::rational< int >           rat(   2,  3 );
#  endif
   boost::compressed_pair< char, int > cp(  'A', 10 );

   // formatter configurations

   boost::io::pair_output< char >   pairfmt1; // [format]: (a,b)

   // output boost types

#  if !defined(BOOST_IOFM__NO_LIB_INTERVAL)
      std::cout << "interval< int > = "
                << boost::io::formatobout( i, boost::io::pairfmt())
                << '\n';

      // [results]: interval< int > = ( 10, 20 )
#  endif

#  if !defined(BOOST_IOFM__NO_LIB_RATIONAL)
      std::cout << "rational< int > = "
                << boost::io::formatobout( rat, pairfmt1 )
                << '\n';

      // [results]: rational< int > = (2,3)
#  endif

   std::cout << "compressed_pair< char, int > = "
             << boost::io::formatobout( cp, pairfmt1 )
             << '\n' << '\n';

   // [results]: compressed_pair< char, int > = (A,10)

#  if !defined(BOOST_IOFM__NO_LIB_QUATERNION)
      boost::math::quaternion< float >    h( 0.1f, 0.2f, 0.3f, 0.4f );

      std::cout << "quaternion< float > = "
                << boost::io::formatobout( h, boost::io::naryfmt())
                << '\n';

      // [results]: quaternion< float > = ( 0.1, 0.2, 0.3, 0.4 )
#  endif

#  if !defined(BOOST_IOFM__NO_LIB_OCTONION)
      boost::math::octonion<   float >    o( 0.1f, 0.2f, 0.3f, 0.4f, 1.1f, 1.2f, 1.3f, 1.4f );

      std::cout << "octonion< float > = "
                << boost::io::formatobout( o, boost::io::naryfmt())
                << '\n';

      // [results]: octonion< float > = ( 0.1, 0.2, 0.3, 0.4, 1.1, 1.2, 1.3, 1.4 )
#  endif

   return( 0 );
}
