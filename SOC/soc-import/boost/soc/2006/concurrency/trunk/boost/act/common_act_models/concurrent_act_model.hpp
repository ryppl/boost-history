//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ACT_MODELS_CONCURRENT_ACT_MODEL_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_CONCURRENT_ACT_MODEL_HPP

#include <exception>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_deque.hpp>

#include "../act_model.hpp"

#include "../detail/thread_manager.hpp"
#include "../detail/result_type.hpp"

#include <windows.h>

#include <deque>

#include <boost/mpl/bool.hpp>

#include <boost/thread/condition.hpp>

/*

action:

contains shared_ptr to internals (event handle, raw storage)

lazily creates thread and function queue when operations upon it are queued




active

immediately creates thread and function queue



ToDo: Connect function queue with thread (queue encapsulates thread)

*/

namespace boost
{
namespace act
{

struct concurrent_act_model
  : act_model
{
  template< typename LogicalType >
  struct param
  {
    // ToDo: Make pass by copy (and appropriately handle arrays)
    typedef LogicalType type;
  };

private:
  template< typename RepresentedType >
  class function_queue;

  // ToDo: Fix add thread call (get thread handle via thread queue)
  template< typename RepresentedType >
  class active_thread
  {
  public:
    active_thread( function_queue< RepresentedType >& function_queue_init )
      : function_queue_m( function_queue_init )
    {
    }
  private:
    class execute_function
    {
    public:
      explicit execute_function( RepresentedType& target_init )
        : target_m( target_init )
      {
      }
    public:
      void operator ()( function_type& func ) const
      {
        func( target_m );
      }
    private:
      RepresentedType& target_m;
    };
  public:
    void operator ()() const
    {
      {
        // ToDo: Make aligned storage for object
        char object_storage[ sizeof( RepresentedType ) ];

        container_type current_functions;

        // ToDo: Remove later (construct via queue)
        RepresentedType& object = *new( &object_storage ) RepresentedType;

        do
        {
          function_queue_m.swap_function_queues( current_functions );

          // ToDo: Pass address of represented type to execute function
          ::std::for_each( current_functions.begin()
                         , current_functions.end()
                         , execute_function( object )
                         );

          // ToDo: Make sure current_functions is cleared prior to thread removal
          current_functions.clear();
        }
        while( !function_queue_m.is_complete() );
      }

      function_queue_m.release_thread();
    }
  private:
    function_queue< RepresentedType >& function_queue_m;
  };

private:
  template< typename ResultType >
  struct action_internals;

  template< typename FunctionPackageType >
  class action_function;

  template< typename FunctionPackageType >
  static
  action_function< FunctionPackageType >
  make_action_function
  (
    shared_ptr
    <
      action_internals< typename FunctionPackageType::result_type >
    >
    const& internals
  , FunctionPackageType const& function
  );

  template< typename RepresentedType >
  class function_queue
  {
  private:
    typedef mutex mutex_type;
    typedef mutex_type::scoped_lock scoped_lock_type;
    typedef function< void( RepresentedType& ) > function_type;
    // ToDo: Use fast allocator
    typedef ptr_deque< function_type > container_type;
  public:
    function_queue()
      : thread_impl_m( 0 )
      , is_complete_m( false )
    {
      // ToDo: Create thread
    }

    ~function_queue()
    {
      // ToDo: Remove -- perform in thread. detail::thread_manager_type::remove_thread( thread_handle_m );
    }
  public:
    detail::thread_manager_type::handle thread_handle() const
    {
      return thread_handle_m;
    }
  protected:
    template< typename FunctionPackageType >
    void queue_function
    (
      shared_ptr
      <
        action_internals< typename FunctionPackageType::result_type >
      >
      const& internals
    , FunctionPackageType const& function_package
    ) const
    {
      raw_queue_function
        ( new function_type
            ( make_action_function( internals, function_package ) )
        );
    }
  private:
    class destroy
    {
    public:
      destroy( unsigned int volatile& target_trigger_init )
        : target_signal_m( target_signal_init )
      {
      }
    public:
      // ToDo: Possibly remove thread and close event handle
      void operator ()( RepresentedType const& target ) const
      {
        target.~RepresentedType();

        target_signal_m = true; // PORTABILITY WARNING: MUST BE ATOMIC
      }
    private:
      unsigned int volatile& target_signal_m;
    };
  public:
    void queue_destruction() const
    {
      raw_queue_function
        ( new function_type( destroy( is_complete_m ) ) );
    }
  private:
    bool is_complete() const
    {
      return is_complete_m != 0; // PORTABILITY WARNING: MUST BE ATOMIC
    }
  private:
    void raw_queue_function( function_type* function ) const
    {
      try
      {
        scoped_lock_type const scoped_lock( mutex_m );

        function_queue_m.push_back( function );

        queue_condition_m.notify_one();
      }
      catch( ... )
      {
        delete function;
        throw;
      }
    }
  private:
    void swap_function_queues( container_type& target_function_queue ) const
    {
      scoped_lock_type scoped_lock( mutex_m );

      queue_condition_m.wait( scoped_lock );

      function_queue_m.swap( target_function_queue );
    }
  private:
    mutex_type mutex_m;
    shared_ptr< RepresentedType > value_m;
    container_type function_queue_m;
    unsigned int volatile is_complete_m;
    condition queue_condition_m;
    detail::thread_manager_type::handle const thread_handle_m;
  };

  template< typename ResultType >
  struct action_internals_base
    : noncopyable
  {
    trigger completion_trigger;
  public:
    action_internals_base()
      : function_queue_m( 0 )
    {
    }

    ~action_internals_base()
    {
      if( function_queue_m )
        queue_destruction();
    }
  private:
    typedef action_internals< ResultType > internals_type;
    typedef shared_ptr< internals_type > shared_internals_type;
  protected:
    template< typename FunctionPackageType >
    void queue_function
    (
      shared_ptr
      <
        action_internals< typename FunctionPackageType::result_type >
      >
      const& internals
    , FunctionPackageType const& function_package
    ) const
    {
      get_function_queue().queue_function
        ( static_cast< internals_type >( *this ), function_package );
    }

    void queue_destruction() const
    {
      get_function_queue().queue_destruction();
    }

    bool is_complete() const
    {
      return get_function_queue().is_complete();
    }

    void swap_function_queues( container_type& target_function_queue ) const
    {
      get_function_queue().swap_function_queues( target_function_queue );
    }
  private:
    function_queue_data& get_function_queue() const
    {
      if( !function_queue_m )
        function_queue_m = new function_queue< ResultType >;

      return *function_queue_data_m;
    }
  private:
    function_queue< ResultType >* function_queue_m;
  };
private:
  template< typename ResultType >
  class nonvoid_action_internals
    : public action_internals_base< ResultType >
  {
    using nonvoid_action_internals::is_constructed;
    using nonvoid_action_internals::mutex;
  public:
    optional< ResultType > value;
  private:
    typedef mutex mutex_type;
    typedef mutex_type::scoped_lock scoped_lock_type;
    // ToDo: Account for const member functions
    typedef function< void( RepresentedType& ) > function_type;
    // ToDo: Use fast allocator
    typedef ptr_deque< function_type > container_type;
  public:
    using function_queue_base::queue_function;
  public:
    void queue_destructor() const
    {
      if( !is_constructed_m )
      {
        {
          scoped_lock_type const lock( mutex_m );

          if( !is_constructed_m )
            return;
        }

        action_internals_base::queue_destructor();

        // ToDo: Create thread

        return;
      }

      action_internals_base::queue_destructor();

      // ToDo: Create thread if necessary (rethink this function)
    }
  private:
    container_type function_queue_m;
  };

  template< typename VoidType >
  struct void_action_internals
    : action_internals_base< VoidType >
  {
  };

  template< typename ResultType >
  struct action_internals
    : mpl::if_< is_void< ResultType >
              , void_action_internals< ResultType >
              , nonvoid_action_internals< ResultType >
              >
              ::type
  {
  };

  template< typename FunctionPackageType >
  class action_function_type_base_base
  {
  private:
    typedef typename FunctionPackageType::result_type result_type;

    typedef action_internals< result_type > internals_type;

    typedef typename FunctionPackageType::stored_function_type stored_function_type;
  public:
    action_function_type_base_base
    (
      shared_ptr< internals_type > const& action_internals_init
    , FunctionPackageType const& function_init
    )
      : action_internals_m( action_internals_init )
      , function_m( function_init.store_function() )
    {
    }
  protected:
    weak_ptr< internals_type > const action_internals_m;
    mutable stored_function_type function_m; // ToDo: Possibly remove mutable
  };

  template< typename FunctionPackageType >
  class action_function_type_nonvoid_base
    : action_function_type_base_base< FunctionPackageType >
  {
  private:
    typedef action_function_type_base_base< FunctionPackageType >
              base;

    typedef typename FunctionPackageType::result_type result_type;

    typedef action_internals< result_type > internals_type;

    typedef shared_ptr< internals_type > shared_internals_type;
  public:
    action_function_type_nonvoid_base
    (
      shared_internals_type const& action_internals_init
    , FunctionPackageType const& function_init
    )
      : base( action_internals_init, function_init )
    {
    }
  public:
    void operator ()() const
    {
      // ToDo: Use in-place construction to eliminate copy
      result_type const result = base::function_m();

      // ToDo: Move this to the base
      shared_internals_type const internals = base::action_internals_m.lock();

      if( internals )
      {
        internals->value = result;

        internals->completion_trigger = true;
      }
    }

    template< typename TargetType >
    void operator ()( TargetType& target ) const
    {
      result_type const result = base::function_m( target );

      // ToDo: Move this to the base
      shared_internals_type const internals = base::action_internals_m.lock();

      if( internals )
      {
        internals->value = result;

        internals->completion_trigger = true;
      }
    }
  };

  template< typename FunctionPackageType >
  class action_function_type_void_base
    : action_function_type_base_base< FunctionPackageType >
  {
  private:
    typedef action_function_type_base_base< FunctionPackageType > base;

    typedef typename FunctionPackageType::result_type result_type;

    typedef action_internals< result_type > internals_type;

    typedef shared_ptr< internals_type > shared_internals_type;
  public:
    action_function_type_void_base
    (
      shared_internals_type const& action_internals_init
    , FunctionPackageType const& function_init
    )
      : base( action_internals_init, function_init )
    {
    }
  public:
    void operator ()() const
    {
      base::function_m();

      // ToDo: Move this to the base
      shared_internals_type const internals = base::action_internals_m.lock();

      if( internals )
        internals->completion_trigger = true;
    }

    template< typename TargetType >
    void operator ()( TargetType& target ) const
    {
      base::function_m( target );

      // ToDo: Move this to the base
      shared_internals_type const internals = base::action_internals_m.lock();

      if( internals )
        internals->completion_trigger = true;
    }
  };

  template< typename FunctionPackageType >
  class action_function
  {
  public:
    typedef typename FunctionPackageType::result_type result_type;

    typedef action_internals< result_type > internals_type;

    typedef shared_ptr< internals_type > shared_internals_type;
  private:
    typedef typename mpl::if_
            <
              is_void< result_type >
            , action_function_type_void_base< FunctionPackageType >
            , action_function_type_nonvoid_base< FunctionPackageType >
            >
            ::type function_type_base;
  private:
    class function_type
      : public function_type_base // ToDo: Change to private
    {
    public:
      function_type( typename action_function::shared_internals_type const&
                       action_internals_init
                   , FunctionPackageType const& function_init
                   )
        : function_type_base( action_internals_init
                            , function_init
                            )
      {
      }
    public:
      using function_type_base::operator ();
    };
  public:
    action_function( shared_internals_type const& action_internals_init
                   , FunctionPackageType const& function_init
                   )
      : function_m( new function_type( action_internals_init
                                     , function_init
                                     )
                  )
    {
    }
  public:
    void operator ()() const
    {
      (*function_m)();
    }

    template< typename TargetType >
    void operator ()( TargetType& target ) const
    {
      (*function_m)( target );
    }
  private:
    shared_ptr< function_type > function_m;
  };

  template< typename FunctionPackageType >
  static
  action_function< FunctionPackageType >
  make_action_function
  (
    shared_ptr
    <
      action_internals< typename FunctionPackageType::result_type >
    >
    const& internals
  , FunctionPackageType const& function
  )
  {
    return action_function< FunctionPackageType >( internals, function );
  }
private:
  struct no_construct_tag {};
public:
  // ToDo: Move implementation to another file
  //       Account for void type
  template< typename ResultType >
  class action_impl
  {
    template< typename FunctionPackageType >
    friend class action_function;

    friend class active_thread< ResultType >;
  public:
    // ToDo: Possibly rewrite to eliminate extra copy of internal function
    //       ( encapsulate reference counted function )
    template< typename FunctionPackageType >
    action_impl( FunctionPackageType const& function )
      : internals_m( new action_internals< ResultType > )
    {
    }
  public:
    template< typename ActiveImplType
            , typename FunctionPackageType
            >
    action_impl
    (
      ActiveImplType& target
    , FunctionPackageType const& function_package
    )
      : internals_m( new action_internals< ResultType > )
    {
      // ToDo: Make queue_function work differently when const
      target.queue_function( internals_m, function_package );
    }
  private:
    typedef action_internals< ResultType > internals_type;
    typedef shared_ptr< internals_type > shared_internals_type;
  protected:
    template< typename FunctionPackageType >
    void queue_function
    (
      shared_ptr
      <
        action_internals< typename FunctionPackageType::result_type >
      >
      const& internals
    , FunctionPackageType const& function_package
    ) const
    {
      internals_m->queue_function( internals, function_package );
    }
  public:
    void queue_destruction() const
    {
      internals_m->queue_destruction();
    }
  private:
    bool is_complete() const
    {
      return internals_m->is_complete();
    }
    void wait() const
    {
      internals_m->wait();
    }
  private:
    typedef BOOST_ACTIVE_M_T((ResultType),(concurrent_act_model)) active_type;
  private:
    shared_ptr< action_internals< ResultType > > internals_m;
  };

  // ToDo: Use empty base optimization where applicable
  //       Allow construction
  //       Make cv-qualified queue_function_impl overloads
  template< typename RepresentedType >
  class active_impl
    : function_queue< RepresentedType >
  {
  public:
    active_impl()
    {
      thread_func const thread_func_instance;

      thread_impl_m = thread_func_instance.impl.get();

      thread_handle_m
        = detail::thread_manager.add_thread( thread_func_instance );
    }
  public:
    ~active_impl()
    {
      thread_impl_m->add_destruction_to_queue( thread_handle_m );
    }

    template< typename FunctionPackageType >
    void queue_function
    (
      shared_ptr
      <
        action_internals< typename FunctionPackageType::result_type >
      >
      const& internals
    , FunctionPackageType const& function_package
    ) const
    {
      thread_impl_m->add_function_to_queue( internals, function_package );
    }
  public:/*
    // ToDo: Change return type for void
    typename base_type::inactive_value_type inactive_value() const
    {
      return base_type::inactive_value();
    }*/
  private:
    thread_impl* thread_impl_m;
    detail::thread_manager_type::handle thread_handle_m;
  };
};

}
}

#endif
