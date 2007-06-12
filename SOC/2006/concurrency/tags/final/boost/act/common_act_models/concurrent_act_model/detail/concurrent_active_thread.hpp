//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

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
