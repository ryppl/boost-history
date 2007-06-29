/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_INTERLOCKED_HAS( load, _ )

#include "../store_load_helper.hpp"
#include <boost/thread/barrier.hpp>

#endif

int test_main( int, char *[] )
{
#if BOOST_ACT_INTERLOCKED_HAS( load, _ )

  test_store_load_no_threads( interlocked_load_get()
                                 , single_thread_basic_set()
                                 );

  test_store_load_no_threads( interlocked_load_get()
                            , interlocked_store_set()
                            );

  {
    boost::barrier barrier( 2 );

    test_store_load_with_threads( barrier
                                , interlocked_load_get()
                                , interlocked_store_set()
                                );
  }
#else
  BOOST_FAIL( "load< default_ > not implemented on this system." );
#endif
  return 0;
}
