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

#ifndef BOOST_ACT_COMMON_ACT_MODELS_DETAIL_CONCURRENT_FUNCTION_QUEUE_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_DETAIL_CONCURRENT_FUNCTION_QUEUE_HPP

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/function.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/identity.hpp>

#include <deque>
#include <algorithm>

#include "../../../detail/thread_manager.hpp"
#include "../../../detail/execute_if.hpp"
#include "../../../detail/object_encapsulator.hpp"

#include "concurrent_active_thread.hpp"
#include "concurrent_action_function.hpp"
#include "concurrent_construction_function.hpp"
#include "concurrent_action_internals.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type, typename Enabler = void >
class concurrent_execute_function
{
public:
  explicit concurrent_execute_function( void* target_init )
    : target_m( static_cast< Type* >( target_init ) )
  {
  }
public:
  template< typename FunctionType >
  void operator ()( FunctionType& function ) const
  {
    function( target_m );
  }
private:
  Type* const target_m;
};

template< typename Type >
class concurrent_execute_function< Type
                                 , typename enable_if< is_void< Type > >::type
                                 >
{
public:
  explicit concurrent_execute_function( void* dummy_target_init )
  {
  }
public:
  template< typename FunctionType >
  void operator ()( FunctionType& function ) const
  {
    function();
  }
};

template< typename Type >
struct concurrent_function_type
  : mpl::eval_if< is_void< Type >
                , mpl::identity< function< void () > >
                , mpl::identity< function< void ( Type* const ) > >
                >
{
};

// Note: Should only be dynamically allocated with new
// ToDo: Template for queue policy and thread manager
template< typename RepresentedType >
class concurrent_function_queue
  : object_encapsulator< RepresentedType >
{
private:
  typedef mutex mutex_type;
  typedef mutex_type::scoped_lock scoped_lock_type;
public:
  typedef typename concurrent_function_type< RepresentedType >
            ::type function_type;
  // ToDo: Use fast allocator
  typedef ::std::deque< function_type > container_type;
public:
  concurrent_function_queue()
    : object_encapsulator< RepresentedType >
        ( no_object_encapsulator_construction() )
    , queueable_queue_m( &function_queues_m[0] )
    , dequeueable_queue_m( &function_queues_m[1] )
    , functions_queued_m( false )
    , thread_created_m( false )
    , object_destroyed_m( false )
  {
  }
public:
  template< typename FunctionPackage >
  explicit concurrent_function_queue( FunctionPackage const& function_package )
    : object_encapsulator< RepresentedType >
        ( no_object_encapsulator_construction() )
    , queueable_queue_m( &function_queues_m[0] )
    , dequeueable_queue_m( &function_queues_m[1] )
    , functions_queued_m( false )
    , thread_created_m( false )
    , object_destroyed_m( false )
  {
    typedef typename FunctionPackage::stored_function_type stored_function_type;

    stored_function_type stored_function = function_package.store_function();

    queued_function_caller( this->raw_object_pointer(), stored_function );
  }
public:
  // Note: Only called from the active object's internal thread
  bool is_complete() const
  {
    return object_destroyed_m;
  }

  bool thread_is_created() const
  {
    return thread_created_m;
  }

  detail::thread_manager_type::handle thread_handle() const
  {
    return thread_handle_m;
  }
private:
public:
  // Note: Only called from the active object's internal thread
  void process_queue() const
  {
    swap_function_queues();

    ::std::for_each( dequeueable_queue_m->begin(), dequeueable_queue_m->end()
                   , concurrent_execute_function< RepresentedType >
                       ( this->raw_object_pointer() )
                   );

    dequeueable_queue_m->clear();
  }

  void process_function() const; // ToDo: Define

public: // ToDo: Change to private when friend is fixed
  // Note: Only used by the active object's internal thread
  class destroy
  {
  public:
    destroy( bool& is_complete_init )
      : is_complete_m( is_complete_init )
    {
    }
  public:
    void operator ()() const
    {
      is_complete_m = true;
    }

    void operator ()( RepresentedType const* target ) const
    {
      target->~RepresentedType();
      is_complete_m = true;
    }
  private:
    bool& is_complete_m;
  };
public:
  template< typename FunctionPackageType >
  void queue_function
  (
    shared_ptr
    <
      concurrent_action_internals< typename FunctionPackageType::result_type >
    >
    const& internals
  , FunctionPackageType const& function_package
  ) const
  {
    raw_queue_function
      ( make_concurrent_action_function( internals, function_package ) );
  }

  template< typename FunctionPackageType >
  void queue_construction( FunctionPackageType const& function_package ) const
  {
    raw_queue_function
    ( make_concurrent_construction_function( function_package )
    );
  }

  template< typename FunctionPackageType >
  void queue_action_construction
  (
    shared_ptr
    <
      concurrent_action_internals< RepresentedType >
    >
    const& internals
  , FunctionPackageType const& function_package
  ) const
  {
    raw_queue_function
    ( make_concurrent_action_construction_function< RepresentedType >
        ( internals, function_package )
    );
  }

  // ToDo: Possibly move outside of template
  struct queue_destruction_impl
  {
    static void execute( concurrent_function_queue const& target_queue
                       , bool& object_destroyed_trigger
                       )
    {
      target_queue.raw_queue_function
        ( destroy( object_destroyed_trigger ) );
    }
  };

  struct delete_self
  {
    static void execute( concurrent_function_queue const& target_queue
                       , bool& object_destroyed_trigger
                       )
    {
      delete &target_queue;
    }
  };

  void queue_destruction() const
  {
    if( thread_is_created() )
      queue_destruction_impl::execute( *this, object_destroyed_m );
    else
      execute_if< mpl::or_< is_void< RepresentedType >
                          , has_trivial_destructor< RepresentedType >
                          >
                , delete_self
                , queue_destruction_impl
                >
                ::execute( *this, object_destroyed_m );
  }
public: // ToDo: Change to private when friend is fixed

  // Note: Takes a dynamically allocated function (and takes ownership)
  template< typename FunctionType >
  void raw_queue_function( FunctionType function ) const
  {
    scoped_lock_type const scoped_lock( mutex_m );

    try_create_thread();

    queueable_queue_m->push_back( function );

    functions_queued_m = true;

    queue_condition_m.notify_one();
  }
public:
  // ToDo: Change to placement construct (use in_place_factory)
  void* raw_object_storage() const
  {
    return this->raw_object_pointer();
  }
private:
  // Note: Only called by process_queue
  //       Swaps once functions are queued, waits if necessary
  void swap_function_queues() const
  {
    scoped_lock_type scoped_lock( mutex_m );

    if( !functions_queued_m )
      queue_condition_m.wait( scoped_lock );

    ::std::swap( queueable_queue_m, dequeueable_queue_m );

    functions_queued_m = false;
  }
private:
  // Note: Only call when locked
  void try_create_thread() const
  {
    if( !thread_created_m )
    {
      thread_handle_m
        = thread_manager.add_thread
            ( concurrent_active_thread< RepresentedType >( this ) );

      thread_created_m = true;
    }
  }
private:
  mutable mutex_type mutex_m;
  mutable container_type function_queues_m[2];
  mutable container_type* queueable_queue_m,
                        * dequeueable_queue_m;
  mutable condition queue_condition_m;
  mutable bool functions_queued_m;
  mutable detail::thread_manager_type::handle thread_handle_m;
  mutable bool thread_created_m,
               object_destroyed_m;
};

}
}
}

#endif
