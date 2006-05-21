// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/property/scalar_property.hpp>
#include <boost/property/aliased_property.hpp>
#include <boost/property/value_property.hpp>
#include <boost/test/minimal.hpp>

struct mytype
{
   inline long calc() const{ return 314159; }
};

typedef boost::scalar_property< boost::value_property<   int    > > valueprop;
typedef boost::scalar_property< boost::value_property<   mytype > > objectprop;
typedef boost::scalar_property< boost::aliased_property< char   > > refprop;

int test_main( int, char *[] )
{
   valueprop i( 0 ), j( 0 );

   // get/set variants

   i.set( 7 );
   BOOST_TEST( i.get() == 7 );

   // operator() variants

   i( 6 );
   BOOST_TEST( i() == 6 );

   // C++ native interface

   i = 6;
   BOOST_TEST( static_cast< int >( i ) == 6 );
   BOOST_TEST( i == 6 );

   // C++ native interface: binary operations

   BOOST_TEST( i + 1 ==  7 );
   BOOST_TEST( i - 1 ==  5 );
   BOOST_TEST( i * 2 == 12 );
   BOOST_TEST( i / 2 ==  3 );
   BOOST_TEST( i % 2 ==  0 );

   // C++ native interface: bitwise operations

   BOOST_TEST( i << 2 == ( 6 << 2 ));
   BOOST_TEST( i >> 2 == ( 6 >> 2 ));

   BOOST_TEST(( i & 2 ) == ( int( 6 ) & 2 ));
   BOOST_TEST(( i | 2 ) == ( int( 6 ) | 2 ));
   BOOST_TEST(( i ^ 2 ) == ( int( 6 ) ^ 2 ));

   // C++ native interface: logical and relational operators

   BOOST_TEST(( i && 2 ) == ( 6 && 2 ));
   BOOST_TEST(( i || 2 ) == ( 6 || 2 ));
   BOOST_TEST(( i != 6 ) == false );
   BOOST_TEST(( i == 6 ) == true  );
   BOOST_TEST(( i <  6 ) == false );
   BOOST_TEST(( i <= 6 ) == true  );
   BOOST_TEST(( i >  6 ) == false );
   BOOST_TEST(( i >= 6 ) == true  );

   // C++ native interface: assignment operators

   i  += 1; BOOST_TEST( i ==  7 );
   i  -= 1; BOOST_TEST( i ==  6 );
   i  *= 2; BOOST_TEST( i == 12 );
   i  /= 2; BOOST_TEST( i ==  6 );
   i  %= 2; BOOST_TEST( i ==  0 );

   i = 5;
   i <<= 2; BOOST_TEST( i == ( 5 << 2 ));
   i >>= 2; BOOST_TEST( i == 5 );
   i  &= 3; BOOST_TEST( i == 1 );
   i  |= 2; BOOST_TEST( i == 3 );
   i  ^= 2; BOOST_TEST( i == ( 3 ^ 2 ));

   i = j = 5;
   BOOST_TEST( i == 5 );
   BOOST_TEST( j == 5 );
   BOOST_TEST( i == j );

   // C++ native interface: unary operations

   BOOST_TEST( ~i == ~5 );
   BOOST_TEST( !i == !5 );
   BOOST_TEST( +i == +5 );
   BOOST_TEST( -i == -5 );

   i = -j; BOOST_TEST( i == -5 );
   j = +i; BOOST_TEST( j == -5 );
   j = -j; BOOST_TEST( j ==  5 );

   ++j; BOOST_TEST( j == 6 );
   --j; BOOST_TEST( j == 5 );

   BOOST_TEST( j++ == 5 && j == 6 );
   BOOST_TEST( j-- == 6 && j == 5 );

   // reference properties

   char    ch;
   refprop k( ch );

   k = 'a';
   BOOST_TEST( k == ch );
   BOOST_TEST( k == 'a' );

   // accessing members of the property value

   mytype mt; // so gcc doesn't think ob( mytype()) is a function decleration.
   objectprop ob( mt );

   BOOST_TEST( ob().calc() == 314159 );
   BOOST_TEST( ob -> calc() == 314159 );
   BOOST_TEST( ob.get().calc() == 314159 );

   return 0;
}
