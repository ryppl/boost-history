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

#ifndef BOOST_ACT_COMMON_ACT_MODELS_DETAIL_CONCURRENT_FUNCTION_QUEUE_BASE_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_DETAIL_CONCURRENT_FUNCTION_QUEUE_BASE_HPP

#include <boost/function.hpp>

#include <cstddef>

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Possibly make thread-safe
template< ::std::size_t MaxQueueSize >
class simple_concurrent_function_deque_impl
{
public:
  typedef function< void() >* value_type;
public:
  simple_concurrent_function_queue_impl()
    : begin_address_m( queue_m )
    , end_address_m( begin_address_m )
  {
  }

  ~simple_concurrent_function_queue_impl()
  {
    clear();
  }
private:
  struct delete_element
  {
    void operator ()( value_type element ) const
    {
      delete element;
    }
  };
public:
  // Note: Takes ownership of passed dynamically allocated function
  void push_back( function_type* function_to_add )
  {
    try
    {
      mutex::scoped_lock lock( mutex_m );

      if( full() )
        dequeue_not_full_m.wait( lock );

      *end_address_m = function_to_add;

      if( end_address_m == queue_m + ( QueueSize - 1 ) )
        end_address_m = queue_m;
    }
    catch( ... )
    {
      if( end_address_m >= begin_address_m )
        for_each( begin_address_m, end_address_m, delete_element() );
      else
      {
        for_each( begin_address_m, queue_m + QueueSize, delete_element() );
        for_each( queue_m, end_address_m, delete_element() );
      }

      throw;
    }
  }

  void process_queue()
  {
    
  }
public:
  bool full() const
  {
    return front_address_m == end_address_m;
  }
private:
  value_type* queue_m[QueueSize];
  value_type* begin_address_m,
            * volatile end_address_m;
  mutex mutex_m;
  condition dequeue_not_full_m;
};

template< ::std::size_t QueueSize >
class concurrent_function_queue_base
{
public:
  
private:
};

// ToDo: Use ptr_deque implementation if above certain size
template<>
class concurrent_function_queue_base< 0 >
{
};

}
}
}

#endif
