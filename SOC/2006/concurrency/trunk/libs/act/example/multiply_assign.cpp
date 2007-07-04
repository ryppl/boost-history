/*==============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//[ multiply_assign
//Bring in the multiply_assign operation (a valid Interlocked Operation).
#include <boost/act/interlocked/multiply_assign.hpp>

// Bring in interlocked integer types.
#include <boost/act/interlocked/integer.hpp>

/* ... */

int main()
{
  using namespace ::boost::act::interlocked;

  // Create an interlocked integer.
  int_fast32_t shared_integer = 2;

  /* ... (code for thread creation not shown for simplicity) ... */

  // Atomically multiply shared_integer by 4 and store the result back to
  // shared_integer. Default semantics are used (in this case, the default is
  // acq_rel, as noted in the documentation for multiply_assign).
  multiply_assign( shared_integer, 4 );

  // Perform a multiply_assign operation with unordered semantics.
  multiply_assign< unordered >( shared_integer, 2 );

  // Perform a multiply_assign operation with acquire semantics.
  multiply_assign< acquire >( shared_integer, 3 );

  /* ... */

  // Perform a multiply_assign operation explicitly with default semantics.
  multiply_assign< default_ >( shared_integer, 3 );
}
//]
