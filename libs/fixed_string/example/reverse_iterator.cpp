// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// fixed_string implements the basic_string interface so it is easy to interoperate with
// existing STL/string code. This means that it has a lower learning curve.

#include <boost/fixed_string/fixed_string.hpp>

int main()
{
   boost::fixed_string< 15 >    cstr;

   cstr = "Halo Welt!";
   std::cout << cstr << '\n';

   for( boost::fixed_string< 15 >::reverse_iterator i = cstr.rbegin(); i != cstr.rend(); ++i )
      std::cout << *i;
   std::cout << '\n';

   return( 0 );
}
