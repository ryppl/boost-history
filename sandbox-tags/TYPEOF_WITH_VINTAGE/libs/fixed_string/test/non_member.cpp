// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.7] -- non-member functions

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>
#include <sstream>

int test_main( int, char *[] )
{
   typedef boost::fixed_string< 15 >   string15;

   // lhs + rhs

#  if defined(BOOST_MSVC) && ( BOOST_MSVC <= 1200 )
      string15                         s1; // VC6: error C2666: '+' : 2 overloads have similar conversions
#  else
      string15                         s1 = string15( "Hello " ) + string15( "World" );
#  endif
   string15                            s2 = string15( "Hello " ) + "World";
   string15                            s3 = "Hello " + string15( "World" );

   BOOST_TEST( s1 == "Hello World" );
   BOOST_TEST( s2 == "Hello World" );
   BOOST_TEST( s3 == "Hello World" );

   BOOST_TEST( string15( "ABC" ) + 'D' == "ABCD" );
   BOOST_TEST( 'A' + string15( "BC" ) == "ABC" );

   // ==; !=

   s1 = "Welt";
   s2 = "Welt";

   BOOST_TEST( s1 == s2 );
   BOOST_TEST( s1 == "Welt" );
   BOOST_TEST( "Hello World" == s3 );

   BOOST_TEST( s1 != s3 );
   BOOST_TEST( s1 != "World" );
   BOOST_TEST( "abc" != s2 );

   // <; <=; >; >=

   s3 = "World";

   BOOST_TEST( s1 < s3 );
   BOOST_TEST( "Abc" < s3 );
   BOOST_TEST( s1 < "Xyz" );

   BOOST_TEST( s1 <= s3 );
   BOOST_TEST( "General" <= s3 );
   BOOST_TEST( s1 <= "Zebra" );

   BOOST_TEST( s1 <= s2 );     // test equality
   BOOST_TEST( "Welt" <= s1 ); // test equality
   BOOST_TEST( s1 <= "Welt" ); // test equality

   BOOST_TEST( s3 > s1 );
   BOOST_TEST( "Xyz" > s1 );
   BOOST_TEST( s3 > "Abc" );

   BOOST_TEST( s3 >= s1 );
   BOOST_TEST( s3 >= "General" );
   BOOST_TEST( "Zebra" >= s1 );

   BOOST_TEST( s2 >= s1 );     // test equality
   BOOST_TEST( s1 >= "Welt" ); // test equality
   BOOST_TEST( "Welt" >= s1 ); // test equality

   // inserters; extractors

   {
      std::stringstream                ss;
      boost::fixed_string< 25 >        s1( "This is  \n\t a string!" );
      ss << s1;

      BOOST_TEST( ss.str() == "This is  \n\t a string!" );

      ss >> s1 >> s2 >> s3;

      BOOST_TEST( s1 == "This" );
      BOOST_TEST( s2 == "is" );
      BOOST_TEST( s3 == "a" );
   }

   // getline

   {
      std::stringstream                ss( "Meine Welt|My World\nSchoen|Beautiful" );
      std::getline( ss, s1, '|' );
      std::getline( ss, s2 );

      BOOST_TEST( s1 == "Meine Welt" );
      BOOST_TEST( s2 == "My World" );

      std::getline( ss, s1, '|' );
      std::getline( ss, s2 );

      BOOST_TEST( s1 == "Schoen" );
      BOOST_TEST( s2 == "Beautiful" );
   }

   // swap

   {
      boost::fixed_string< 15 >        s1( "abcdef" );
      boost::fixed_string< 30 >        s2( "12345" );
      s1.swap( s2 );

      BOOST_TEST( s1 == "12345" );
      BOOST_TEST( s2 == "abcdef" );
   }

   return( 0 );
}
