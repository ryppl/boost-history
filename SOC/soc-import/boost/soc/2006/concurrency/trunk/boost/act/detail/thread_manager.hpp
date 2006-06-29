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
  typedef mutex mutex_type;
  typedef try_mutex try_mutex_type;
  typedef mutex_type::scoped_lock scoped_lock_type;
  typedef try_mutex_type::scoped_try_lock scoped_try_lock_type;
  typedef thread thread_type;
private:
  typedef ::std::deque< thread_type* > thread_handle_container_type;
  typedef thread_handle_container_type::iterator iterator_type;
public:
  thread_manager_impl()
    : curr_thread_container_m( thread_containers_m )
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
    void operator ()( thread_type* target_thread ) const
    {
      target_thread->join();
      delete target_thread;
      target_thread = 0;
    }
  };
public:
  ~thread_manager_impl()
  {
    scoped_try_lock_type const destroyer_lock( is_being_destroyed_mutex_m
                                             , true
                                             );

    for(;;)
    {
      thread_handle_container_type* closing_thread_container;

      {
        scoped_lock_type const lock( mutex_m );

        if( curr_thread_container_m->empty() )
          return;

        closing_thread_container = swap_thread_containers();

      ::std::for_each( closing_thread_container->begin()
                     , closing_thread_container->end()
                     , thread_joiner()
                     );

        closing_thread_container->clear();
      }
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

    try
    {
      curr_thread_container_m->push_back( new_thread );
    }
    catch( ... )
    {
      delete new_thread;
      throw;
    }

    iterator_type handle_iterator = curr_thread_container_m->end();
    --handle_iterator;

    return handle( handle_iterator );
  }

  void remove_thread( handle const& thread_to_remove )
  {
    scoped_try_lock_type const destroyer_lock( is_being_destroyed_mutex_m );

    if( destroyer_lock )
    {
      thread_type* const removed_thread = *thread_to_remove.iterator_m;

      {
        // ToDo: Somehow safely check before locking
        scoped_lock_type const lock( mutex_m );

        iterator_type last_iterator = curr_thread_container_m->end();

        --last_iterator;

        ::std::iter_swap( thread_to_remove.iterator_m
                        , last_iterator
                        );

        curr_thread_container_m->pop_back();
      }

      delete removed_thread;
    }
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
  thread_handle_container_type thread_containers_m[2];
  thread_handle_container_type* curr_thread_container_m;
  mutex_type mutex_m;
  try_mutex_type is_being_destroyed_mutex_m;
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
