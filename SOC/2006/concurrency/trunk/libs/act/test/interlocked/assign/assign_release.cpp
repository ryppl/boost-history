/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "../assign_retrieve_helper.hpp"
#include <boost/thread/barrier.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include "assign_helper.hpp"

#include <boost/act/interlocked/assign/assign_acquire.hpp>

struct interlocked_assign_acquire_set
{
  template< typename VariableType, typename SourceType >
  typename boost::remove_cv< VariableType >::type
  operator ()( VariableType& var, SourceType new_val ) const
  {
    return boost::act::interlocked::assign_acquire( var, new_val );
  }
};

int test_main( int, char *[] )
{

  test_assign_retrieve_no_threads( single_thread_basic_get()
                                 , interlocked_assign_acquire_set()
                                 );

  test_assign_retrieve_no_threads( interlocked_retrieve_get()
                                 , interlocked_assign_acquire_set()
                                 );

  {
    boost::barrier barrier( 2 );

    test_assign_retrieve_with_threads( barrier
                                     , interlocked_retrieve_get()
                                     , interlocked_assign_acquire_set()
                                     );
  }

  brute_assign_test( interlocked_assign_acquire_set() );

  return 0;
}