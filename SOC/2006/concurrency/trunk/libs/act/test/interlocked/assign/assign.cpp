/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_CONFIG_INTERLOCKED_HAS( assign, _ )

#include "../store_load_helper.hpp"
#include <boost/thread/barrier.hpp>
#include <boost/act/interlocked/assign/assign.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include "assign_helper.hpp"

struct interlocked_assign_set
{
  template< typename VariableType, typename SourceType >
  typename boost::remove_cv< VariableType >::type
  operator ()( VariableType& var, SourceType new_val ) const
  {
    return boost::act::interlocked::assign( var, new_val );
  }
};

#endif

int test_main( int, char *[] )
{
#if BOOST_ACT_CONFIG_INTERLOCKED_HAS( assign, _ )
  test_store_load_no_threads( single_thread_basic_get()
                            , interlocked_assign_set()
                            );

  test_store_load_no_threads( interlocked_load_get()
                            , interlocked_assign_set()
                            );

  {
    boost::barrier barrier( 2 );

    test_store_load_with_threads( barrier
                                , interlocked_load_get()
                                , interlocked_assign_set()
                                );
  }

  brute_assign_test( interlocked_assign_set() );
#else
  BOOST_FAIL( "assign< default_ > not implemented on this system." );
#endif
  return 0;
}
