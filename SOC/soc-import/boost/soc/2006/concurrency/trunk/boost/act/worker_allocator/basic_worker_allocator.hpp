/*=============================================================================
    Copyright (c) 2006-2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_WORKER_ALLOCATOR_BASIC_WORKER_ALLOCATOR_HPP
#define BOOST_ACT_WORKER_ALLOCATOR_BASIC_WORKER_ALLOCATOR_HPP

#include <boost/act/worker_allocator/basic_worker_allocator/detail/thread_manager.hpp>
#include <boost/thread/thread.hpp>
#include <boost/noncopyable.hpp>

namespace boost
{
namespace act
{

class basic_worker_policy
{
private:
  //typedef detail::thread_manager_type::handle thread_handle;
public:
  template< typename FunctionType >
  void spawn_wild_worker( FunctionType function )
  {
    detail::thread_manager.add_wild_thread( function );
  }
public:
  // ToDo: Revise implementation to use internals of thread (avoid extra mutex)
  class unmanaged_worker
    : noncopyable
  {
    friend class basic_worker_policy;
  public:
    template< typename FunctionType >
    unmanaged_worker( basic_worker_policy&, FunctionType function )
      : thread_m( function ), is_joined_m( false )
    {
    }
  private:
    void join_impl()
    {
      if( !is_joined_m )
      {
        thread_m.join();
        is_joined_m = true;
      }
    }
  private:
    thread thread_m;
    bool is_joined_m;
  };

  class safe_unmanaged_worker
    : noncopyable
  {
    friend class basic_worker_policy;
  public:
    template< typename FunctionType >
    safe_unmanaged_worker( basic_worker_policy&, FunctionType function )
      : thread_m( function ), is_joined_m( false )
    {
    }
  private:
    void join_impl()
    {
      mutex::scoped_lock const lock( mutex_m );
      if( !is_joined_m )
      {
        thread_m.join();
        is_joined_m = true;
      }
    }
  private:
    thread thread_m;
    mutex mutex_m;
    bool is_joined_m;
  };
public:
  void join( unmanaged_worker& target_worker )
  {
    target_worker.join_impl();
  }

  void join( safe_unmanaged_worker& target_worker )
  {
    target_worker.join_impl();
  }
};

}
}

#endif
