/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_WORKER_POLICY_THREAD_POLICY_DETAIL_THREAD_MANAGER_HPP
#define BOOST_ACT_WORKER_POLICY_THREAD_POLICY_DETAIL_THREAD_MANAGER_HPP

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/aligned_storage.hpp>

#include <list>
#include <algorithm>

namespace boost { namespace act { namespace detail {

template< typename FunctionType >
class self_cleaning_function;

class thread_manager_type;

class thread_manager_impl
{
  friend class thread_manager_type;
private:
  typedef mutex mutex_type;
  typedef try_mutex try_mutex_type;
  typedef mutex_type::scoped_lock scoped_lock_type;
  typedef try_mutex_type::scoped_try_lock scoped_try_lock_type;
  typedef thread thread_type;
private:
  typedef ::std::list< thread_type* > thread_handle_container_type;
  typedef thread_handle_container_type::iterator iterator_type;
public:
  thread_manager_impl()
    : curr_thread_container_m( thread_containers_m )
    , closing_thread_container_m( thread_containers_m + 1 )
  {
  }
public:
  class raw_handle
  {
    friend class thread_manager_impl;
  public:
    raw_handle()
    {
    }
  private:
    explicit raw_handle( iterator_type iterator_init )
      : iterator_m( iterator_init )
    {
    }
  public:
    iterator_type iterator_m;
  };
/*
  class handle
  {
    struct thread_info  // ToDo: Make constructor
    {
      thread_info() : is_complete( false ) {}
      bool is_complete;
      thread_manager_impl::raw_handle raw_handle;
      thread_manager_impl::mutex_type mutex;
    };
  public:
    void join() const
    {
      mutex_type::scoped_lock lock( thread_info_m->mutex );
      if( !thread_info_m->is_complete )
      {
        (*iterator_m)->join();
        thread_info_m->is_complete = true;
      }
    }
  private:
    ::boost::shared_ptr< thread_info > thread_info_m;
  };*/
private:
  struct thread_joiner
  {
    void operator ()( thread_type* target_thread ) const
    {
      target_thread->join();
      delete target_thread;
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
      scoped_lock_type const lock( mutex_m );

      if( curr_thread_container_m->empty() )
        return;

      swap_thread_containers();

      ::std::for_each( closing_thread_container_m->begin()
                     , closing_thread_container_m->end()
                     , thread_joiner()
                     );

      closing_thread_container_m->clear();
    }
  }
public:
  typedef ::boost::function< void() > function_type;
private:/*
  iterator_type obtain_thread_iterator()
  {
    scoped_lock_type const lock( mutex_m );

    curr_thread_container_m->push_back( 0 );

    iterator_type handle_iterator = curr_thread_container_m->end();
    --handle_iterator;

    return handle( handle_iterator );
  }
*/
  raw_handle raw_add_thread( function_type const& function )
  {
    // ToDo: Change
    thread_type* const new_thread = new thread_type( function );

    {
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

      return raw_handle( handle_iterator );
    }
  }
private:
  void remove_thread( raw_handle const& thread_to_remove )
  {
    scoped_try_lock_type const destroyer_lock( is_being_destroyed_mutex_m );

    if( destroyer_lock )
    {
      iterator_type const& target_iterator = thread_to_remove.iterator_m;

      thread_type* const removed_thread = *target_iterator;

      {
        scoped_lock_type const lock( mutex_m );

        curr_thread_container_m->erase( target_iterator );
      }

      delete removed_thread;
    }
  }
public:

  void add_wild_thread( function_type const& function );

  void join_thread( raw_handle const& thread_to_join )
  {
    (*thread_to_join.iterator_m)->join();
  }
private:
  void swap_thread_containers()
  {
    ::std::swap( curr_thread_container_m, closing_thread_container_m );
  }
private:
  thread_handle_container_type thread_containers_m[2];
  thread_handle_container_type* curr_thread_container_m,
                              * closing_thread_container_m;
  mutex_type mutex_m;
  try_mutex_type is_being_destroyed_mutex_m;
};

class thread_manager_type
{
  template< typename FunctionType >
  friend class self_cleaning_function;
public:
  typedef thread_manager_impl::raw_handle raw_handle;
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
private:
  void remove_thread( raw_handle const& thread_to_remove )
  {
    return thread_manager_instance().remove_thread( thread_to_remove );
  }
public:/*
  raw_handle add_thread( function_type const& function )
  {
    return thread_manager_instance().add_thread( function );
  }
*/
  void add_wild_thread( function_type const& function )
  {
    return thread_manager_instance().add_wild_thread( function );
  }

  // Note: Do not call from thread being joined
  void join_thread( raw_handle const& thread_to_join )
  {
    thread_manager_instance().join_thread( thread_to_join );
  }
private:
  static thread_manager_impl& thread_manager_instance()
  {
    return *reinterpret_cast< thread_manager_impl* >( &storage_m );
  }
private:
  typedef aligned_storage< sizeof( thread_manager_impl )
                         , alignment_of< thread_manager_impl >::value
                         >
                         ::type
            storage_type;
private:
  static unsigned int reference_count_m;
  static storage_type storage_m;
};

unsigned int thread_manager_type::reference_count_m = 0;

thread_manager_type::storage_type thread_manager_type::storage_m;

namespace
{
thread_manager_type thread_manager;
}

struct self_cleaning_function_internals
{
  thread_manager_type::raw_handle handle;
  boost::mutex mutex;
};

// Note: This function object should only be called once
// ToDo: In debug mode check above note
// ToDo: Use locking to make this perfectly safe (for updated handle) 
template< typename FunctionType >
class self_cleaning_function
{
public:
  self_cleaning_function( FunctionType const& function_init
                        , self_cleaning_function_internals* internals_init
                        )
    : function_m( function_init )
    , internals_m( internals_init ) {}
public:
  void operator ()() const
  {
    function_m();

    thread_manager_type::raw_handle handle;

    {
      boost::mutex::scoped_lock lock( internals_m->mutex );
      handle = internals_m->handle;
    }

    // Note: If the following statements are unordered or partially ordered
    //       then the application may close prior to some dynamically allocated
    //       memory being explicitly deallocated and certain objects being
    //       destructed, though in practice this should still be acceptable
    //       behavior. If a better solution can be created, this implementation
    //       may be rewritten.
    delete internals_m;
    thread_manager.remove_thread( handle );
  }
private:
  FunctionType function_m;
  self_cleaning_function_internals* const internals_m;
};


/*
inline
void
thread_manager_impl::add_multitrackable_thread( function_type const& function )
{
  self_cleaning_function_internals* const new_internals
    = new self_cleaning_function_internals;

  try
  {
    self_cleaning_function< function_type >
      thread_main( function
                 , new_internals
                 );

    {
      boost::mutex::scoped_lock lock( new_internals->mutex );
      new_internals->handle = raw_add_thread( thread_main );
    }

  }
  catch( ... )
  {
    delete new_internals;
    throw;
  }
}
*/

inline
void thread_manager_impl::add_wild_thread( function_type const& function )
{
  self_cleaning_function_internals* const new_internals
    = new self_cleaning_function_internals;

  try
  {
    self_cleaning_function< function_type >
      thread_main( function
                 , new_internals
                 );

    {
      boost::mutex::scoped_lock lock( new_internals->mutex );
      new_internals->handle = raw_add_thread( thread_main );
    }

  }
  catch( ... )
  {
    delete new_internals;
    throw;
  }
}

} } }

#endif
