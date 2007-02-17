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

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/act/detail/circular_buffer.hpp>

#include <boost/act/detail/raw_storage.hpp>

namespace boost
{
namespace act
{

// ToDo: Template for thread allocator

// ToDo: Make default BufferSize toggleable
template< ::std::size_t BufferSize = 6 >
class concurrent_consumer_allocator
{
public:
  template< typename BufferData, typename Process >
  class scoped_consumer
    : concurrent_consumer_allocator
  {
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
        // ToDo: Catch exceptions and report to scoped_consumer in debug mode
        mutex::scoped_lock lock( consumer_body_m.mutex_m );
        while( !(    consumer_body_m.closing_m
                  && consumer_body_m.buffer_m.empty()
                )
             )
        {
          if( consumer_body_m.buffer_m.empty() )
            consumer_body_m.condition_m.wait( lock );

          // Note: The only time the buffer can be empty here is
          //       if the consumer was triggered to close
          if( consumer_body_m.buffer_m.empty() ) break;

          {
            consumer_body_m.process_m( consumer_body_m.buffer_m.front() );

            bool const was_full = consumer_body_m.buffer_m.full();
            consumer_body_m.buffer_m.pop_front();

            // Notify the producer that the buffer isn't full when necessary
            if( was_full )
              consumer_body_m.condition_m.notify_one();

            lock.unlock();
            lock.lock();
          }
        }
      }
    private:
      scoped_consumer& consumer_body_m;
    };
  public:
    explicit scoped_consumer(concurrent_consumer_allocator const& allocator_init)
      { intialize(); }

    explicit scoped_consumer( Process const& consume_init )
      : process_m( consumer_init ) { intialize(); }

    scoped_consumer() { intialize(); }
  private:
    void intialize()
    {
      // ToDo: Possibly initialize all data under this lock
      mutex::scoped_lock lock( mutex_m );
      closing_m = false;
      new( &thread_m ) thread( consumer_main( *this ) );
    }
  private:
    thread& thread_object()
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
    // ToDo: Allow in-place construction
    void consume( BufferData const& data )
    {
      mutex::scoped_lock lock( mutex_m );
      if( buffer_m.full() )
        condition_m.wait( lock );

      bool const was_empty = buffer_m.empty();
      buffer_m.push_back( data );

      // Notify the consumer of the function when necessary
      if( was_empty )
        condition_m.notify_one();
    }
  private:
    // ToDo: Change to initialize under a lock
    detail::circular_buffer< BufferData, BufferSize > buffer_m;
    Process process_m;
    typename detail::raw_storage< thread >::type thread_m;
    mutex mutex_m;
    condition condition_m;
    bool closing_m;
  };
};

}
}

#endif
