// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.6] -- string operations

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   // c_str; data; get_allocator
   //                             01234567890123456
   boost::fixed_string< 25 > s1( "abcabcdefedcbacba" );

   BOOST_CHECK( strcmp(  s1.c_str(), "abcabcdefedcbacba" ) == 0 );
   BOOST_CHECK( strncmp( s1.data(),  "abcabcdefedcbacba", 17 ) == 0 );
   BOOST_CHECK( s1.get_allocator() == boost::fixed_string< 5 >().get_allocator());

   // substr

   BOOST_CHECK( s1.substr( 3, 4 ) == "abcd" );

   // find

   BOOST_CHECK( s1.find( boost::fixed_string< 5 >( "bcd" )) == 4 );
   BOOST_CHECK( s1.find( boost::fixed_string< 5 >( "ab" ), 1 ) == 3 );
   BOOST_CHECK( s1.find( "bcd" ) == 4 );
   BOOST_CHECK( s1.find( "ab", 1 ) == 3 );
   BOOST_CHECK( s1.find( "abcde", 1, 2 ) == 3 );
   BOOST_CHECK( s1.find( 'c' ) == 2 );
   BOOST_CHECK( s1.find( 'c', 3 ) == 5 );

   // rfind

   BOOST_CHECK( s1.rfind( boost::fixed_string< 5 >( "cb" )) == 14 );
   BOOST_CHECK( s1.rfind( boost::fixed_string< 5 >( "cb" ), 13 ) == 11 );
   BOOST_CHECK( s1.rfind( "cb" ) == 14 );
   BOOST_CHECK( s1.rfind( "cb", 13 ) == 11 );
   BOOST_CHECK( s1.rfind( "cba", 13, 2 ) == 11 );
   BOOST_CHECK( s1.rfind( 'b' ) == 15 );
   BOOST_CHECK( s1.rfind( 'b', 14 ) == 12 );

   // find_first_of

   BOOST_CHECK( s1.find_first_of( boost::fixed_string< 8 >( "dfcghn" )) == 2 );
   BOOST_CHECK( s1.find_first_of( boost::fixed_string< 8 >( "dfcghn" ), 3 ) == 5 );
   BOOST_CHECK( s1.find_first_of( "dfcghn" ) == 2 );
   BOOST_CHECK( s1.find_first_of( "dfcghn", 3 ) == 5 );
   BOOST_CHECK( s1.find_first_of( "dfcghn", 3, 2 ) == 6 );
   BOOST_CHECK( s1.find_first_of( 'c' ) == 2 );
   BOOST_CHECK( s1.find_first_of( 'c', 3 ) == 5 );

   // find_last_of

   BOOST_CHECK( s1.find_last_of( boost::fixed_string< 8 >( "dfcghn" )) == 14 );
   BOOST_CHECK( s1.find_last_of( boost::fixed_string< 8 >( "dfcghn" ), 13 ) == 11 );
   BOOST_CHECK( s1.find_last_of( "dfcghn" ) == 14 );
   BOOST_CHECK( s1.find_last_of( "dfcghn", 13 ) == 11 );
   BOOST_CHECK( s1.find_last_of( "dfcghn", 13, 2 ) == 10 );
   BOOST_CHECK( s1.find_last_of( 'c' ) == 14 );
   BOOST_CHECK( s1.find_last_of( 'c', 13 ) == 11 );

   // find_first_not_of

   BOOST_CHECK( s1.find_first_not_of( boost::fixed_string< 8 >( "agcebf" )) == 6 );
   BOOST_CHECK( s1.find_first_not_of( boost::fixed_string< 8 >( "agcebf" ), 7 ) == 10 );
   BOOST_CHECK( s1.find_first_not_of( "agcebf" ) == 6 );
   BOOST_CHECK( s1.find_first_not_of( "agcebf", 7 ) == 10 );
   BOOST_CHECK( s1.find_first_not_of( "agcebf", 3, 2 ) == 4 );
   BOOST_CHECK( s1.find_first_not_of( 'a' ) == 1 );
   BOOST_CHECK( s1.find_first_not_of( 'a', 3 ) == 4 );

   // find_last_not_of

   BOOST_CHECK( s1.find_last_not_of( boost::fixed_string< 8 >( "agcebf" )) == 10 );
   BOOST_CHECK( s1.find_last_not_of( boost::fixed_string< 8 >( "agcebf" ), 9 ) == 6 );
   BOOST_CHECK( s1.find_last_not_of( "agcebf" ) == 10 );
   BOOST_CHECK( s1.find_last_not_of( "agcebf", 9 ) == 6 );
   BOOST_CHECK( s1.find_last_not_of( "agcebf", 13, 2 ) == 12 );
   BOOST_CHECK( s1.find_last_not_of( 'a' ) == 15 );
   BOOST_CHECK( s1.find_last_not_of( 'a', 13 ) == 12 );

   // compare

   boost::fixed_string< 20 > s2( "abcabcdefedcbacba" );
   boost::fixed_string< 10 > s3( "abc" );

   BOOST_CHECK( s1.compare( s2 ) == 0 );
   BOOST_CHECK( s1.compare( s3 ) != 0 );
   BOOST_CHECK( s1.compare( 3, 3, s3 ) == 0 );
   BOOST_CHECK( s1.compare( 3, 6, s2, 3, 6 ) == 0 );
   BOOST_CHECK( s1.compare( "abcabcdefedcbacba" ) == 0 );
   BOOST_CHECK( s1.compare( "abc" ) != 0 );
   BOOST_CHECK( s1.compare( 3, 6, "abcdef" ) == 0 );
   BOOST_CHECK( s1.compare( 3, 4, "abcdef", 4 ) == 0 );

   return 0;
}
