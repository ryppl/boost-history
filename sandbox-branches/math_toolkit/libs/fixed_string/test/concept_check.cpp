// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fixed_string/fixed_string.hpp>

int main()
{
   boost::fixed_string< 1, int > oops; // 'value' not a member of 'char_type_error'

   return 0;
}
