/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "../assign_load_helper.hpp"
#include <boost/thread/barrier.hpp>
#include "assign_helper.hpp"

int test_main( int, char *[] )
{
  test_assign_load_no_threads( single_thread_basic_get()
                                 , interlocked_assign_set()
                                 );

  test_assign_load_no_threads( interlocked_load_get()
                                 , interlocked_assign_set()
                                 );

  {
    boost::barrier barrier( 2 );

    test_assign_load_with_threads( barrier
                                     , interlocked_load_get()
                                     , interlocked_assign_set()
                                     );
  }

  brute_assign_test( interlocked_assign_set() );

  return 0;
}
