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
