/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "assign_retrieve_helper.hpp"
#include <boost/thread/barrier.hpp>

int test_main( int, char *[] )
{
  test_assign_retrieve_no_threads( interlocked_retrieve_get()
                                 , single_thread_basic_set()
                                 );

  test_assign_retrieve_no_threads( interlocked_retrieve_get()
                                 , interlocked_assign_set()
                                 );

  {
    boost::barrier barrier( 2 );

    test_assign_retrieve_with_threads( barrier
                                     , interlocked_retrieve_get()
                                     , interlocked_assign_set()
                                     );
  }

  return 0;
}
