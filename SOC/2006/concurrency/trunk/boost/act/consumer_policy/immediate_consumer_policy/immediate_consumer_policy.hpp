/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_IMMEDIATE_CONSUMER_ALLOCATOR_ALLOC_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_IMMEDIATE_CONSUMER_ALLOCATOR_ALLOC_HPP

namespace boost
{
namespace act
{

// ToDo: Template for thread allocator
// ToDo: Add appropriate default for ResultType (result of Process or void)
class immediate_consumer_allocator
{
public:
  template< typename ValueType, typename Process, typename ResultType >
  class consumer;

  template< typename ValueType, typename Process, typename ResultType >
  class scoped_consumer;

  template< typename ValueType, typename Process, typename ResultType >
  class safe_consumer;

  template< typename ValueType, typename Process, typename ResultType >
  class safe_scoped_consumer;

/*
  // ToDo: Make copyable
  template< typename BufferData, typename ConsumerOperation >
  class safe_consumer
  {
  private:
    class consumer_main
    {
    public:
      explicit consumer_main( safe_consumer_allocator& consumer_body_init )
        : consumer_body_m( consumer_body_init ) {}
    public:
      void operator ()() const
      {
        BufferData curr_data;

        mutex::scoped_lock lock( mutex_m );
        while( !(    consumer_body_m.closing_m
                  && ( consumer_body_m.num_fun_waiting == 0 )
                )
             )
        {
          if( consumer_body_m.num_fun_waiting == 0 )
            consumer_body_m.consumer_condition_m.wait( lock );

          // If the consumer is triggered to close, exit the loop
          // Note: The only time the buffer can be empty here is
          //       if the consumer was triggered to close
          if( consumer_body_m.num_fun_waiting == 0 ) break;

          curr_data = consumer_body_m.function_m;

          --consumer_body_m.num_fun_waiting;

          consumer_body_m.producer_condition_m.notify_one();

          lock.unlock();

          consumer_body_m.consume_m( curr_data );

          consumer_body_m.function_completion_condition_m.notify_one();

          lock.lock();
        }
      }
    private:
      safe_consumer& consumer_body_m;
    };
  public:
    // ToDo: Construct the object from within the consumer thread
    safe_consumer()
    {
      mutex::scoped_lock lock( mutex_m );
      num_fun_waiting = 0;
      closing_m = false;
      new( &thread_m ) boost::thread( consumer_main( *this ) );
      // ToDo: Only construct if non-trivial constructor
      no_lock_call( lock, construct() );
    }

    ~safe_consumer()
    {
      {
        mutex::scoped_lock lock( mutex_m );
        closing_m = true;
        // ToDo: Only destruct if non-trivial destructor
        no_lock_call( lock, destruct() );
        consumer_condition_m.notify_one(); // Notify the consumer of the close
      }

      thread_object().join();
      thread_object().~thread();
    }
  private:
    boost::thread& thread_object()
    {
      return reinterpret_cast< boost::thread& >( thread_m );
    }
  public:
    void consume( BufferData data )
    {
      // ToDo: Wait for fun to finish
      mutex::scoped_lock lock( mutex_m );
      no_lock_consume( lock, data );
    }

    void consume( BufferData data ) const
    {
      mutex::scoped_lock lock( mutex_m );
      no_lock_consume( lock, data );
    }
  private:
    // Only called by implementation when already under a lock
    void no_lock_consume( mutex::scoped_lock& lock, BufferData& data )
    {
      function_m = ref( function );

      ++num_fun_waiting;

      consumer_condition_m.notify_one();

      function_completion_condition_m.wait( lock );
    }

    // Only called by implementation when already under a lock
    void no_lock_consume( mutex::scoped_lock& lock, BufferData& data ) const
    {
      const_call_fun< FunctionType > const const_fun = function;

      function_m = ref( const_fun );

      ++num_fun_waiting;

      consumer_condition_m.notify_one();

      function_completion_condition_m.wait( lock );
    }
  private:
    typedef typename mpl::if_< is_void< ConsumerData >
                             , int
                             , ConsumerData
                             >
                             ::type consumer_data_type;
  private:
    ConsumerOperation consume_m;
    mutable mutex mutex_m;
    mutable condition producer_condition_m,
                      consumer_condition_m,
                      function_completion_condition_m;
    typename detail::raw_storage< boost::thread >::type thread_m;
    typename detail::raw_storage< consumer_data_type >::type consumer_data_m; // ToDo: Make special case for reference
    mutable typename detail::raw_storage< function_type >::type function_m; // ToDo: Get proper function type
    mutable unsigned int num_fun_waiting;
    mutable bool closing_m;
  };

  template< typename BufferData, typename ConsumerOperation >
  class safe_scoped_consumer
    : safe_consumer< BufferData, ConsumerOperation >
  {
  public:
    using safe_consumer< BufferData, ConsumerOperation >::consume;
  };*/
};

}
}

#endif
