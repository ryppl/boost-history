// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.5] -- modifiers

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   boost::fixed_string< 15 > s1;
   boost::fixed_string< 20 > s2;
   boost::fixed_string< 15 > s3;

   const char * str = "1234567890";

   // assign

   s1.assign( "abc" );
   s2.assign( "123456", 4 );
   s3.assign( 6, '*' );

   BOOST_CHECK( s1 == "abc" );
   BOOST_CHECK( s2 == "1234" );
   BOOST_CHECK( s3 == "******" );

   s1.assign( s2 );
   s3.assign( s2, 1, 2 );

   BOOST_CHECK( s1 == s2 );
   BOOST_CHECK( s1 == "1234" );
   BOOST_CHECK( s3 == "23" );

   s1.assign( str, str + 5 );
   s3.assign( s2.begin(), s2.end());

   BOOST_CHECK( s1 == "12345" );
   BOOST_CHECK( s3 == "1234" );

   // append

   boost::fixed_string< 15 > s4( "Hello " );
   boost::fixed_string< 20 > s5( "abc" );
   boost::fixed_string< 15 > s6( "1234" );

   s4.append( "World" );
   s5.append( "defghijk", 5 );
   s2.append( 3, '5' );

   BOOST_CHECK( s4 == "Hello World" );
   BOOST_CHECK( s5 == "abcdefgh" );
   BOOST_CHECK( s2 == "1234555" );

   s6.append( s5 );
   s5.append( s6, 2, 4 );
   s1.append( s3.rbegin(), s3.rend());

   BOOST_CHECK( s6 == "1234abcdefgh" );
   BOOST_CHECK( s5 == "abcdefgh34ab" );
   BOOST_CHECK( s1 == "123454321" );

   // push_back

   s1.clear();
   s1.push_back( 'A' );
   s1.push_back( 'B' );
   s1.push_back( 'C' );

   BOOST_CHECK( s1 == "ABC" );

   // operator+=

   s1 += s2;
   s2 += "4321";
   s5 += 'c';

   BOOST_CHECK( s1 == "ABC1234555" );
   BOOST_CHECK( s2 == "12345554321" );
   BOOST_CHECK( s5 == "abcdefgh34abc" );

   // erase

   s2.erase();
   s1.erase( 8 );
   s5.erase( 6, 4 );

   BOOST_CHECK( s2.empty());
   BOOST_CHECK( s2 == "" );
   BOOST_CHECK( s1 == "ABC12345" );
   BOOST_CHECK( s5 == "abcdefabc" );

   s1.erase( s1.begin() + 3 );
   s5.erase( s5.begin() + 3, s5.end() - 3 );

   BOOST_CHECK( s1 == "ABC2345" );
   BOOST_CHECK( s5 == "abcabc" );

   // copy

   char s[ 20 ];

   s1.copy( s, 5 );
   BOOST_CHECK( strcmp( s, "ABC23" ) == 0 );

   s1.copy( s, 5, 2 );
   BOOST_CHECK( strcmp( s, "C2345" ) == 0 );

   // insert
   boost::fixed_string< 10 > fs( "abcdef" );

   s1 = s2 = s3 = s4 = s5 = s6 = "1234567890";
   s1.insert( 5, boost::fixed_string< 10 >( "abc" ));
   s2.insert( 5, fs, 4 );
   s3.insert( 5, fs, 1, 4 );
   s4.insert( 5, "ABC" );
   s5.insert( 5, "abcdef", 4 );
   s6.insert( 5, 4, '+' );

   BOOST_CHECK( s1 == "12345abc67890" );
   BOOST_CHECK( s2 == "12345abcd67890" );
   BOOST_CHECK( s3 == "12345bcde67890" );
   BOOST_CHECK( s4 == "12345ABC67890" );
   BOOST_CHECK( s5 == "12345abcd67890" );
   BOOST_CHECK( s6 == "12345++++67890" );

   s1 = s2 = s3 = s4 = "1234567890";
   s6 = "abcd";
   s1.insert( s1.begin() + 5, '-' );
   s2.insert( s2.begin() + 5, 3, 'a' );
   s3.insert( s3.begin() + 5, s6.begin(),  s6.end());
   s4.insert( s4.begin() + 5, s6.rbegin(), s6.rend());

   BOOST_CHECK( s1 == "12345-67890" );
   BOOST_CHECK( s2 == "12345aaa67890" );
   BOOST_CHECK( s3 == "12345abcd67890" );
   BOOST_CHECK( s4 == "12345dcba67890" );

   // replace

   s1 = s2 = s3 = s4 = s5 = "1234567890";
   s1.replace( 3, 4, fs );
   s2.replace( 3, 4, fs, 1, 3 );
   s3.replace( 3, 4, "hello" );
   s4.replace( 3, 4, "abcdef", 3 );
   s5.replace( 3, 4, 6, '*' );

   BOOST_CHECK( s1 == "123abcdef890" );
   BOOST_CHECK( s2 == "123bcd890" );
   BOOST_CHECK( s3 == "123hello890" );
   BOOST_CHECK( s4 == "123abc890" );
   BOOST_CHECK( s5 == "123******890" );

   s1 = s2 = s3 = s4 = s5 = s6 = "1234567890";
   s1.replace( s1.begin() + 2, s1.begin() + 4, fs );
   s2.replace( s2.begin() + 2, s2.begin() + 4, "abcd" );
   s3.replace( s3.begin() + 2, s3.begin() + 4, "abcd", 2 );
   s4.replace( s4.begin() + 2, s4.begin() + 4, 6, '-' );
   s5.replace( s5.begin() + 2, s5.begin() + 4, fs.begin(),  fs.end());
   s6.replace( s6.begin() + 2, s6.begin() + 4, fs.rbegin(), fs.rend());

   BOOST_CHECK( s1 == "12abcdef567890" );
   BOOST_CHECK( s2 == "12abcd567890" );
   BOOST_CHECK( s3 == "12ab567890" );
   BOOST_CHECK( s4 == "12------567890" );
   BOOST_CHECK( s5 == "12abcdef567890" );
   BOOST_CHECK( s6 == "12fedcba567890" );

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
