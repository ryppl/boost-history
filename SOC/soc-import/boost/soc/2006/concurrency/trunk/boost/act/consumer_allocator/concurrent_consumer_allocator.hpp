/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_HPP

#include <cstddef>
#include <new>

#include <boost/utility/in_place_factory.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/type_traits/is_void.hpp>

#include <boost/function.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/act/detail/execute_if.hpp>
#include <boost/act/detail/circular_buffer.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct make_consumer_fun_type
{
  typedef void type( typename add_reference< Type >::type );
};

}

// ToDo: Make default BufferSize toggleable
template< ::std::size_t BufferSize = 6 >
class concurrent_consumer_allocator
{
public:
  template< typename ConsumerData = void >
  class scoped_consumer
  {
    typedef typename mpl::eval_if
            <
              is_void< ConsumerData >
            , mpl::identity< void () >
            , detail::make_consumer_fun_type< ConsumerData >
            >
            ::type raw_function_type;

    typedef function< raw_function_type > function_type;

    // ToDo: Use empty base optimization
    typedef typename mpl::if_< is_void< ConsumerData >
                             , int
                             , ConsumerData
                             >
                             consumer_data_type;
  public:
    struct basic_call
    {
      template< typename Type >
      static void execute( function_type& function, Type& target )
      {
        function( target );
      }
    };

    struct void_call
    {
      template< typename Type >
      static void execute( function_type& function, Type& target )
      {
        function();
      }
    };
  public:
    // ToDo: Possibly detect exceptions thrown by consumer
    class consumer_main
    {
    public:
      consumer_main( scoped_consumer& consumer_body_init )
        : consumer_body_m( consumer_body_init ) {}
    public:
      void operator ()() const
      {
        // ToDo: Possibly revise lifetime of curr_function

        // ToDo: Catch exceptions and report to scoped_consumer

        // ToDo: Optimize by removing function copies
        mutex::scoped_lock lock( consumer_body_m.mutex_m );
        while( !(    consumer_body_m.closing_m
                  && consumer_body_m.buffer_m.empty()
                )
             )
        {
          if( consumer_body_m.buffer_m.empty() )
            consumer_body_m.condition_m.wait( lock );

          // If the consumer is triggered to close, exit the loop
          // Note: The only time the buffer can be empty here is
          //       if the consumer was triggered to close
          if( consumer_body_m.buffer_m.empty() ) break;

          {
            ::boost::function< void() > curr_function
              = consumer_body_m.buffer_m.front();

            bool const was_full = consumer_body_m.buffer_m.full();
            consumer_body_m.buffer_m.pop_front();

            // Notify the producer that the buffer isn't full when necessary
            if( was_full )
              consumer_body_m.condition_m.notify_one();

            lock.unlock();
            detail::execute_if< is_void< ConsumerData >
                              , void_call
                              , basic_call
                              >
                              ::execute( curr_function
                                       , consumer_body_m.consumer_data_m
                                       );
          }
          lock.lock();
        }
      }
    private:
      scoped_consumer& consumer_body_m;
    };
  public:
    scoped_consumer()
      : closing_m( false )
    {
      // ToDo: Possibly initialize all data under this lock
      mutex::scoped_lock lock( mutex_m );
      new( &thread_m ) boost::thread( consumer_main( *this ) );
    }
  private:
    boost::thread& thread_object()
    {
      return reinterpret_cast< boost::thread& >( thread_m );
    }
  public:
    ~scoped_consumer()
    {
      {
        mutex::scoped_lock lock( mutex_m );
        closing_m = true;
        condition_m.notify_one(); // Notify the consumer of the close
      }

      thread_object().join();
      thread_object().~thread();
    }
  public:
    template< typename FunctionType >
    void call( FunctionType const& function )
    {
      mutex::scoped_lock lock( mutex_m );
      if( buffer_m.full() )
        condition_m.wait( lock );

      bool const was_empty = buffer_m.empty();
      buffer_m.push_back( in_place( function ) );

      // Notify the consumer of the function when necessary
      if( was_empty )
        condition_m.notify_one();
    }
  private:
    detail::circular_buffer< function_type, BufferSize > buffer_m;
    consumer_data_type consumer_data_m;
    typename aligned_storage< sizeof( boost::thread )
                            , alignment_of< boost::thread >::value
                            >
                            ::type thread_m;
    mutex mutex_m;
    condition condition_m;
    bool closing_m;
  };
};

}
}

#endif
