// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This example demonstrates how fixed_string can integrate seamlessly with C-based code
// providing guards against buffer overflow.

#include <boost/fixed_string/fsstring.hpp> // for strcpy; strcat
#include <boost/fixed_string/fsstdio.hpp>  // for sprintf

int main()
{
   boost::fixed_string< 15 >    cstr;

   strcpy( cstr, "Meine Grosse Welt!" );
   printf( "%s\n", cstr.c_str()); // c_str() call needed here because of variable args

   sprintf( cstr, "Hello %d Worlds!", 10 );
   printf( "%s\n", cstr.c_str());
   
   strcpy( cstr, "Hello" );
   printf( "%s\n", cstr.c_str());

   strcat( cstr, " 25 Worlds!" );
   printf( "%s\n", cstr.c_str());

   return( 0 );
}
