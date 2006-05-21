// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/property/rank1_property.hpp>
#include <boost/property/rank1_aliased_property.hpp>
#include <boost/test/minimal.hpp>

/** Implement a property value type that defines an [] operator. */

class array_type: public boost::property_type< int >
{
   int val;
   public:
      array_type & operator=( int i ){ val = i; return *this; }
      operator int() const{ return val; }
      int & operator[]( int ){ return val; }
      const int & operator[]( int ) const{ return val; }
   public:
      array_type( int i = 0 ): val( i ){}
};

bool operator==( const array_type & a1, const array_type & a2 )
{
   return int( a1 ) == int( a2 );
}

bool operator==( const array_type & a1, int a2 )
{
   return int( a1 ) == a2;
}

// --end array_type

typedef boost::rank1_property< boost::rank1_aliased_property< int > > int1D;
typedef boost::rank1_property
        <
           boost::rank1_aliased_property< array_type, int & >
        > prop1D;

int test_main( int, char *[] )
{
   array_type v[ 10 ];
   prop1D     a( v );

   int        w[ 10 ];
   int1D      b( w );

   // get/set variants

   a.set( 1, 5 );
   b.set( 1, 5 );
   BOOST_TEST( a.get( 1 ) == 5 );
   BOOST_TEST( b.get( 1 ) == 5 );

   // operator() variants

   a( 2, 6 );
   b( 2, 6 );
   BOOST_TEST( a( 2 ) == 6 );
   BOOST_TEST( b( 2 ) == 6 );

   // C++ native interface

   a[ 3 ] = 7;
   b[ 3 ] = 7;
   BOOST_TEST( static_cast< array_type >( a[ 3 ]) == 7 );
   BOOST_TEST( static_cast< int >( b[ 3 ]) == 7 );
   BOOST_TEST( a[ 3 ] == 7 );
   BOOST_TEST( b[ 3 ] == 7 );

   ++a[ 2 ];
   ++b[ 2 ];
   BOOST_TEST( a[ 2 ] == 7 );
   BOOST_TEST( a[ 2 ] == a[ 3 ]);
   BOOST_TEST( b[ 2 ] == 7 );
   BOOST_TEST( b[ 2 ] == b[ 3 ]);
   BOOST_TEST( a[ 2 ] == b[ 2 ]);

   ++a[ 3 ][ 1 ]; // note: increments temporary!
   BOOST_TEST( a[ 3 ][ 1 ] == 7 );

   return 0;
}
