//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_THREAD_MANAGER_HPP
#define BOOST_ACT_DETAIL_THREAD_MANAGER_HPP

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>

#include <boost/ptr_container/ptr_deque.hpp>

#include <vector>
#include <algorithm>

namespace boost
{
namespace act
{
namespace detail
{

class thread_manager_impl
{
private:
  typedef ::boost::mutex mutex_type;
  typedef mutex_type::scoped_lock scoped_lock_type;
  typedef ::boost::thread thread_type;
private:
  typedef ::boost::ptr_deque< ::boost::thread* > thread_handle_container_type;
  typedef thread_handle_container_type::iterator iterator_type;
public:
  thread_manager_impl()
    : curr_thread_container_m( thread_containers_m )
    , is_waiting_m( false )
  {
  }
public:
  class handle
  {
    friend class thread_manager_impl;
  public:
    handle()
    {
    }
  private:
    explicit handle( iterator_type iterator_init )
      : iterator_m( iterator_init )
    {
    }
  private:
    iterator_type iterator_m;
  };
private:
  struct thread_joiner
  {
    void operator ()( thread_type& target_thread ) const
    {
      target_thread.join();
    }
  };
public:
  ~thread_manager_impl()
  {
    for(;;)
    {
      thread_handle_container_type* closing_thread_container;

      {
        scoped_lock_type const lock( mutex_m );

        is_waiting_m = true;

        if( curr_thread_container_m->empty() )
          return;

        closing_thread_container = swap_thread_containers();
      }

      ::std::for_each( closing_thread_container->begin()
                     , closing_thread_container->end()
                     , thread_joiner()
                     );

      closing_thread_container->clear();

      // Note: Block here for portability
      is_waiting_m = false;
    }
  }
public:
  typedef ::boost::function< void() > function_type;
public:
  handle add_thread( function_type const& function )
  {
    function_type* new_function = new function_type( function );

    // ToDo: Change
    thread_type* const new_thread = new thread_type( function );

    scoped_lock_type const lock( mutex_m );

    curr_thread_container_m->push_back( new_thread );

    iterator_type handle_iterator = curr_thread_container_m->end();
    --handle_iterator;

    return handle( handle_iterator );
  }

  void remove_thread( handle const& thread_to_remove )
  {
    void* thread_to_close;

    {
      scoped_lock_type const lock( mutex_m );

      if( is_waiting_m )
        return;

      ::std::swap( *thread_to_remove.iterator_m
                 , curr_thread_container_m->back()
                 );

      thread_to_close = curr_thread_container_m->back();
      curr_thread_container_m->pop_back();
    }

    CloseHandle( thread_to_close );
  }
private:
  // Returns a pointer to the closing thread containers
  thread_handle_container_type* swap_thread_containers()
  {
    if( curr_thread_container_m == thread_containers_m )
    {
      curr_thread_container_m = thread_containers_m + 1;
      return thread_containers_m;
    }
    else
    {
      curr_thread_container_m = thread_containers_m;
      return thread_containers_m + 1;
    }
  }
private:
  // ToDo: Use fast allocator
  thread_handle_container_type thread_containers_m[2];
  thread_handle_container_type* curr_thread_container_m;
  mutex_type mutex_m;
  bool volatile is_waiting_m; // NOTE: THIS IS NOT PORTABLE, CHANGE
};

class thread_manager_type
{
public:
  typedef thread_manager_impl::handle handle;
public:
  thread_manager_type()
  {
    if( reference_count_m == 0 )
      new( &storage_m ) thread_manager_impl;

    ++reference_count_m;
  }

  ~thread_manager_type()
  {
    if( reference_count_m == 1 )
      thread_manager_instance().~thread_manager_impl();

    --reference_count_m;
  }
public:
  typedef ::boost::function< void() > function_type;
public:
  handle add_thread( function_type const& function )
  {
    return thread_manager_instance().add_thread( function );
  }

  void remove_thread( handle const& thread_to_remove )
  {
    return thread_manager_instance().remove_thread( thread_to_remove );
  }
private:
  static thread_manager_impl& thread_manager_instance()
  {
    return *reinterpret_cast< thread_manager_impl* >( &storage_m );
  }
private:
  static unsigned int reference_count_m;
  // ToDo: Use aligned storage
  static char storage_m[ sizeof( thread_manager_impl ) ];
};

unsigned int thread_manager_type::reference_count_m = 0;

char thread_manager_type::storage_m[ sizeof( thread_manager_impl ) ];

namespace
{
  thread_manager_type thread_manager;
}

}
}
}

#endif
