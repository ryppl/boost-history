// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.3] -- capacity

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   boost::fixed_string< 15 >           s1( "1234567890" );
   boost::fixed_string< 15 >           s2;
   boost::fixed_string< 20 >           s3( s1 );

   // empty

   BOOST_TEST( !s1.empty());
   BOOST_TEST( s2.empty());

   // size and length

   BOOST_TEST( s1.size() == s1.length());
   BOOST_TEST( s1.size() == 10 );
   BOOST_TEST( s1.size() == s3.size());

   // max_size and capacity -- fixed-capacity string checks

   BOOST_TEST( s2.max_size() == s2.capacity());
   BOOST_TEST( s1.capacity() == s2.capacity());
   BOOST_TEST( s1.capacity() == 15 );
   BOOST_TEST( s1.capacity() != s3.capacity());

   // resize

   s1.resize( 8 );
   s2.resize( 4, 'a' );

   BOOST_TEST( s1.size() == 8 );
   BOOST_TEST( s1 == "12345678" );
   BOOST_TEST( s2.size() == 4 );
   BOOST_TEST( s2 == "aaaa" );

   // reserve

   s1.reserve( 5 );
   s2.reserve( 6 );

   BOOST_TEST( s1.size() == 5 );
   BOOST_TEST( s1 == "12345" );
   BOOST_TEST( s2.length() == 4 ); // note: length unaffected

   // clear

   s3.clear();
   BOOST_TEST( s3.empty());

   return( 0 );
}
