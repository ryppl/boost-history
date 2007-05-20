/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_WORKER_POLICY_THREAD_POLICY_BASIC_THREAD_POLICY_HPP
#define BOOST_ACT_WORKER_POLICY_THREAD_POLICY_BASIC_THREAD_POLICY_HPP

#include <boost/act/worker_policy/thread_policy/detail/thread_manager.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

namespace boost
{
namespace act
{

class basic_thread_policy
{
public:
  template< typename ResultType = void >
  class tracked_operation
  {
  public:
    
  private:
    // ToDo: Use empty base optimization
    typename mpl::if_< is_void< ResultType >
                     , int
                     , ResultType
                     >
                     ::type value_m;
  };
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
    unmanaged_worker( FunctionType function )
      : thread_m( function ), is_joined_m( false )
    {
    }

    template< typename FunctionType >
    unmanaged_worker( basic_worker_policy const&, FunctionType function )
      : thread_m( function ), is_joined_m( false )
    {
    }
  public:
    void join()
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
    safe_unmanaged_worker( FunctionType function )
      : thread_m( function ), is_joined_m( false )
    {
    }

    template< typename FunctionType >
    safe_unmanaged_worker( basic_worker_policy const&, FunctionType function )
      : thread_m( function ), is_joined_m( false )
    {
    }
  public:
    void join()
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
};

}
}

#endif
