// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fixed_string/formatter.hpp>

inline void print( const char * msg )
{
   std::cout << "Message: " << msg << '\n';
}

int main()
{
   std::cout << boost::formatter( "Hello %s Welt!", "Meine" ) << '\n';

   print( boost::formatterex< 100 >( "This is test number %d", 95 ));

   return( 0 );
}
