// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.4] -- element access

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>
#include <sstream>

int test_main( int, char *[] )
{
   boost::fixed_string< 15 >           str( "1234567890" );
   const boost::char_string &          cstr = str;

   // operator[]

   {
      std::ostringstream               ss;
      for( size_t i = 0; i < str.length(); ++i )
      {
         ss << str[ i ] << cstr[ i ];
      }
      BOOST_TEST( ss.str() == "11223344556677889900" );
   }

   // at

   {
      std::ostringstream               ss;
      for( size_t i = 0; i < str.length(); ++i )
      {
         ss << str.at( i ) << cstr.at( i );
      }
      BOOST_TEST( ss.str() == "11223344556677889900" );
   }

   return( 0 );
}
