// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.2] -- iterators

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>
#include <sstream>

int test_main( int, char *[] )
{
   boost::fixed_string< 15 >           cstr( "1234567890" );

   // iterators

   {
      std::ostringstream               str;
      for( boost::fixed_string< 15 >::iterator i = cstr.begin(); i != cstr.end(); ++i )
      {
         str << *i;
         *i = 'a';
      }
      BOOST_TEST( str.str() == "1234567890" );
      BOOST_TEST( cstr == "aaaaaaaaaa" );
   }

   cstr = "1234567890";

   {
      std::ostringstream               str;
      for( boost::fixed_string< 15 >::const_iterator i = cstr.begin(); i != cstr.end(); ++i )
         str << *i;
      BOOST_TEST( str.str() == "1234567890" );
   }

   // reverse iterators

   {
      std::ostringstream               str;
      for( boost::fixed_string< 15 >::reverse_iterator i = cstr.rbegin(); i != cstr.rend(); ++i )
      {
         str << *i;
         *i = 'a';
      }
      BOOST_TEST( str.str() == "0987654321" );
      BOOST_TEST( cstr == "aaaaaaaaaa" );
   }

   cstr = "1234567890";

   {
      std::ostringstream               str;
      for( boost::fixed_string< 15 >::const_reverse_iterator i = cstr.rbegin(); i != cstr.rend(); ++i )
         str << *i;
      BOOST_TEST( str.str() == "0987654321" );
   }

   return( 0 );
}
