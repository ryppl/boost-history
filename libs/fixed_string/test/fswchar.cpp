// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// test the wchar.h C-style string functions (fixed_string variants)

#include <boost/fixed_string/fswchar.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   const wchar_t *                     teststr = L"C-style strings!";
   const int                           testlen = wcslen( teststr );

   boost::fixed_string< 25, wchar_t >  str1;
   boost::fixed_string< 20, wchar_t >  str2;
   boost::fixed_string< 30, wchar_t >  str3;

   // test wcscpy and use it to test wcscmp and wcslen

   wcscpy( str1, teststr );
   BOOST_TEST( wcscmp( str1, teststr ) == 0 );
   BOOST_TEST( wcscmp( str1, teststr ) == wcscmp( teststr, str1 ));
   BOOST_TEST( wcslen( str1 ) == testlen );

   // copy a different string and validate

   wcscpy( str2, L"12345" );
   BOOST_TEST( wcscmp( str2, teststr ) != 0 );
   BOOST_TEST( wcscmp( str2, L"12345" ) == 0 );
   BOOST_TEST( wcslen( str2 ) == 5 );

   // use the copied string to test wcscat

   wcscat( str2, L"67890" );
   BOOST_TEST( wcscmp( str2, L"1234567890" ) == 0 );
   BOOST_TEST( wcslen( str2 ) == 10 );

   // test swprintf

   BOOST_TEST( swprintf( str3, L"1234%d7890", 56 ) == 10 );
   BOOST_TEST( wcscmp( str3, L"1234567890" ) == 0 );
   BOOST_TEST( wcscmp( str2, str3 ) == 0 );
   BOOST_TEST( wcslen( str3 ) == 10 );

   // test wcscpy with a fixed_string as the 2nd argument

   wcscpy( str3, str1 );
   BOOST_TEST( wcscmp( str3, teststr ) == 0 );
   BOOST_TEST( wcslen( str3 ) == testlen );

   return( 0 );
}
