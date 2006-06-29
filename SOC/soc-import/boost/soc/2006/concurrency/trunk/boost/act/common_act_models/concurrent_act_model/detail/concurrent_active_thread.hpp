//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_DETAIL_CONCURRENT_ACTIVE_THREAD_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_DETAIL_CONCURRENT_ACTIVE_THREAD_HPP

#include <boost/type_traits/aligned_storage.hpp>

#include <iostream>

namespace boost
{
namespace act
{

class default_concurrent_active_function
{
public:
  template< typename FunctionType >
  void operator ()( FunctionType& function ) const
  {
    function();
  }
};

namespace detail
{

template< typename RepresentedType >
class concurrent_function_queue;

// ToDo: Fix add thread call (get thread handle via thread queue)
template< typename RepresentedType
        , typename MainFunctionType = default_concurrent_active_function
        >
class concurrent_active_thread
  : MainFunctionType
{
public:
  // ToDo: Make constructor which accepts MainFunctionType constructor args
  concurrent_active_thread
    ( concurrent_function_queue< RepresentedType > const*
        concurrent_function_queue_init
    )
    : function_queue_m( concurrent_function_queue_init )
  {
  }
public:
  void operator ()()
  {
    // ToDo: Move to main function
    do
      function_queue_m->process_queue();
    while( !function_queue_m->is_complete() );

    detail::thread_manager_type::handle const thread_handle
      = function_queue_m->thread_handle();

    delete function_queue_m;

    detail::thread_manager.remove_thread( thread_handle );
  }
private:
  concurrent_function_queue< RepresentedType > const* const function_queue_m;
};

}
}
}

#include "concurrent_function_queue.hpp"

#endif
