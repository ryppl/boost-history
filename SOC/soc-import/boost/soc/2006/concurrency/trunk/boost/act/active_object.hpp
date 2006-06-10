//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACTIVE_OBJECT_HPP
#define BOOST_ACTIVE_OBJECT_HPP

#include <cstddef>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/if.hpp>

#include <boost/function.hpp>

#include <boost/pool/pool_allocator.hpp>

#include <boost/ptr_container/ptr_list.hpp>

#include <boost/mpl/and.hpp>

#define BOOST_ACTIVE_FUNCTION_NUM_PARAMS 20

#define BOOST_ACTIVE_FUNCTION_DETAIL_FORWARDED_ARG( z, curr_arg, arg_name )     \
  ::boost::wait( BOOST_PP_CAT( arg_name, curr_arg ) ) 

#define BOOST_ACTIVE_FUNCTION_DETAIL_FORWARDED_ARGS( z, num_args, arg_name )    \
  BOOST_PP_CAT( BOOST_PP_ENUM_, z ) )
  (
    num_args
  , BOOST_ACTIVE_FUNCTION_DETAIL_FORWARDED_ARG
  , arg_name
  )

namespace boost
{
  namespace detail
  {
  }
#if 0
  class thread_allocator
  {
  public:
    void acquire_thread( ::boost::function< void () > const& thread_main )
    {
    }
  };

  typedef basic_thread_allocator default_thread_allocator;

  template< typename ThreadAllocator >
  class asynchronous_active_impl_internals
  {
  public:
    typedef ::boost::shared_ptr< detail::action_data< ReturnType > > action_data;
  public:
    typedef ThreadAllocator thread_allocator;
    typedef ::boost::mutex mutex_type;
    typedef mutex_type::scoped_lock scoped_lock;
  private:
    typedef ::boost::function< void () > queued_func_type;
    typedef ::boost::optional< queued_func_type > optional_queued_func_type;

    typedef ::std::deque< optional_queued_func_type
                        , /* ToDo: Use boost fast allocator */
                        >
                        func_container_type;
  public:
    void process_message( mutex_type& mutex )
    {
      {
        scoped_lock( mutex );
        // ToDo: Possibly use counting allocator or ptr container to avoid copy
        queued_func_type func = queue_m.front();
        queue_m.pop();
      }

      func();

      // ToDo: Possibly sleep if empty
    }
  public:
    template< typename ReturnType, typename MessageType >
    class adapted_message
    {
    public:
      adapted_message( MessageType const& message_init
                     , action_data const& action_data_init
                     )
        : message_m( message_init )
        , action_data_m( action_data_init )
      {
      }
    public:
      void operator ()()
      {
        // ToDo: Handle active_object returns (optimize away copy)

        // ToDo: Handle exceptions (what to do with event?)
        result_type const result = message_m();

        action_data const locked_action_data
          = action_data_m.lock();

        if( locked_action_data )
        {
          // ToDo: Handle exceptions (what to do with event?)
          locked_action_data->result = result;

          // ToDo: Handle return value
          SetEvent( locked_action_data->event_handle() );
        }
      }
    private:
      MessageType message_m;
      ::boost::weak_ptr< detail::action_data< ReturnType > > action_data_m;
    };

    template< typename MessageType >
    class adapted_message< void, MessageType >
    {
    public:
      adapted_message( MessageType const& message_init
                     , ::boost::shared_ptr< detail::action_data< void > > const& action_data_init
                     )
        : message_m( message_init )
        , action_data_m( action_data_init )
      {
      }
    public:
      void operator ()()
      {
        // ToDo: Handle exceptions (what to do with event?)
        message_m();

        ::boost::shared_ptr< detail::action_data< void > > const locked_action_data
          = action_data_m.lock();

        if( locked_action_data )
          SetEvent( locked_action_data->event_handle() );
      }
    private:
      MessageType message_m;
      ::boost::weak_ptr< detail::action_data< void > > action_data_m;
    };
  public:
    template< typename ReturnType
            , typename MessageType
            >
    action< ReturnType > // ToDo: Change type
    receive_message( mutex_type& mutex
                   , MessageType& message
                   )
    {
      action< ReturnType > result;

      adapted_message< ReturnType, MessageType > adapted_function( message, result.action_data_m ); // ToDo: Change to dynamic memory allocation

      {
        scoped_lock( mutex_m );

        if( !queue_m )
          func_container_m = ::boost::in_place();

        func_container_m->push_back( optional_queued_func_type() );
      }

      try
      {
        func_container_m->back() = wrapped_function;
      }
      catch( ... )
      {
        func_container_m->pop_back();
        throw;
      }

      return result;
    }
  private:
    ::boost::shared_ptr< basic_message_handler_internals< ThreadAllocator > > self_m;
    ::boost::optional< func_container_type > func_container_m;
    ::boost::optional< MutexType > mutex_m;
    ::boost::optional< ContainedType > object_m;
    ::boost::optional< ::boost::thread > thread_storage_m;
    bool destroy_thread_m,
         delay_destruction_m;
  };
  
  template< typename ThreadAllocator >
  class basic_message_handler
  {
  public:
    ::boost::shared_ptr< basic_message_handler_internals< ThreadAllocator > > internals_m;
  };
#endif

}

#endif
