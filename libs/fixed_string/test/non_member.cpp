// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.7] -- non-member functions

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>
#include <sstream>

int test_main( int, char *[] )
{
   typedef boost::fixed_string< 15 > string15;

   // lhs + rhs

#  if defined(BOOST_MSVC) && ( BOOST_MSVC <= 1200 )
      string15                         s1; // VC6: error C2666: '+' : 2 overloads have similar conversions
#  else
      string15                         s1 = string15( "Hello " ) + string15( "World" );
#  endif
   string15                            s2 = string15( "Hello " ) + "World";
   string15                            s3 = "Hello " + string15( "World" );

   BOOST_CHECK( s1 == "Hello World" );
   BOOST_CHECK( s2 == "Hello World" );
   BOOST_CHECK( s3 == "Hello World" );

   BOOST_CHECK( string15( "ABC" ) + 'D' == "ABCD" );
   BOOST_CHECK( 'A' + string15( "BC" ) == "ABC" );

   // ==; !=

   s1 = "Welt";
   s2 = "Welt";

   BOOST_CHECK( s1 == s2 );
   BOOST_CHECK( s1 == "Welt" );
   BOOST_CHECK( "Hello World" == s3 );

   BOOST_CHECK( s1 != s3 );
   BOOST_CHECK( s1 != "World" );
   BOOST_CHECK( "abc" != s2 );

   // <; <=; >; >=

   s3 = "World";

   BOOST_CHECK( s1 < s3 );
   BOOST_CHECK( "Abc" < s3 );
   BOOST_CHECK( s1 < "Xyz" );

   BOOST_CHECK( s1 <= s3 );
   BOOST_CHECK( "General" <= s3 );
   BOOST_CHECK( s1 <= "Zebra" );

   BOOST_CHECK( s1 <= s2 );     // test equality
   BOOST_CHECK( "Welt" <= s1 ); // test equality
   BOOST_CHECK( s1 <= "Welt" ); // test equality

   BOOST_CHECK( s3 > s1 );
   BOOST_CHECK( "Xyz" > s1 );
   BOOST_CHECK( s3 > "Abc" );

   BOOST_CHECK( s3 >= s1 );
   BOOST_CHECK( s3 >= "General" );
   BOOST_CHECK( "Zebra" >= s1 );

   BOOST_CHECK( s2 >= s1 );     // test equality
   BOOST_CHECK( s1 >= "Welt" ); // test equality
   BOOST_CHECK( "Welt" >= s1 ); // test equality

   // inserters; extractors

   {
      std::stringstream         ss;
      boost::fixed_string< 25 > s1( "This is  \n\t a string!" );
      ss << s1;

      BOOST_CHECK( ss.str() == "This is  \n\t a string!" );

      ss >> s1 >> s2 >> s3;

      BOOST_CHECK( s1 == "This" );
      BOOST_CHECK( s2 == "is" );
      BOOST_CHECK( s3 == "a" );
   }

   // getline

   {
      std::stringstream ss( "Meine Welt|My World\nSchoen|Beautiful" );
      std::getline( ss, s1, '|' );
      std::getline( ss, s2 );

      BOOST_CHECK( s1 == "Meine Welt" );
      BOOST_CHECK( s2 == "My World" );

      std::getline( ss, s1, '|' );
      std::getline( ss, s2 );

      BOOST_CHECK( s1 == "Schoen" );
      BOOST_CHECK( s2 == "Beautiful" );
   }

   // swap

   {
      boost::fixed_string< 15 > s1( "abcdef" );
      boost::fixed_string< 30 > s2( "12345" );
      s1.swap( s2 );

      BOOST_CHECK( s1 == "12345" );
      BOOST_CHECK( s2 == "abcdef" );
   }

   return 0;
}
