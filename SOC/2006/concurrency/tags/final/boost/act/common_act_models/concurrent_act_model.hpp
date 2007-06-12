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

#ifndef BOOST_ACT_COMMON_ACT_MODELS_CONCURRENT_ACT_MODEL_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_CONCURRENT_ACT_MODEL_HPP

#include <exception>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>

#include "../act_model.hpp"

#include "../detail/thread_manager.hpp"
#include "../detail/result_type.hpp"

#include "concurrent_act_model/detail/concurrent_function_queue.hpp"
#include "concurrent_act_model/detail/concurrent_action_internals.hpp"
#include "concurrent_act_model/detail/concurrent_construction_tags.hpp"

#include <boost/mpl/bool.hpp>

#include <boost/thread/condition.hpp>

#include "../active/active_no_default.hpp"

#include "../detail/execute_if.hpp"

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
    // ToDo: Appropriately handle arrays
    typedef LogicalType type;
  };
public:
  // ToDo: Use empty base optimization where applicable
  //       Allow construction
  //       Make cv-qualified queue_function_impl overloads
  template< typename RepresentedType >
  class active_impl
  {
  private:
    typedef detail::concurrent_function_queue< RepresentedType >
              function_queue_type;
  private:
    struct construction_queuer
    {
      template< typename FunctionPackageType >
      static void execute( function_queue_type& function_queue
                         , FunctionPackageType const& function_package
                         )
      {
        function_queue.queue_construction( function_package );
      }
    };
  public:
    template< typename FunctionPackageType >
    active_impl( FunctionPackageType const& function_package )
      : function_queue_m( new function_queue_type )
    {
      detail::execute_if
      <
        mpl::not_
        <
          mpl::and_< has_trivial_constructor< RepresentedType >
                   , typename FunctionPackageType::param_sequence_is_empty
                   > // ToDo: Change to use lazy metafunction
        >
      , construction_queuer
      >
      ::execute( *function_queue_m, function_package );
    }

    active_impl( active_impl_constructor_tag
               , detail::concurrent_no_active_construction_tag
               )
      : function_queue_m( new function_queue_type )
    {
    }

    template< typename FunctionPackageType >
    active_impl( active_impl_constructor_tag
               , detail::concurrent_local_active_construction_tag
               , FunctionPackageType const& function_package
               )
      : function_queue_m( new function_queue_type( function_package ) )
    {
    }
  public:
    ~active_impl()
    {
      function_queue_m->queue_destruction();
      // Note: Active thread deletes
    }

    template< typename FunctionPackageType >
    void queue_function
    (
      shared_ptr
      <
        detail::concurrent_action_internals
          < typename FunctionPackageType::result_type >
      >
      const& internals
    , FunctionPackageType const& function_package
    ) const
    {
      function_queue_m->queue_function( internals, function_package );
    }
  private:
    struct action_construction_queuer
    {
      template< typename FunctionPackageType >
      static void execute( function_queue_type& function_queue
                         , shared_ptr
                           <
                             detail::concurrent_action_internals
                               < RepresentedType >
                           >
                           internals
                         , FunctionPackageType const& function_package
                         )
      {
        function_queue
          .queue_action_construction( internals, function_package );
      }
    };
  public:
    template< typename FunctionPackageType >
    void queue_action_construction
    (
      shared_ptr
      <
        detail::concurrent_action_internals< RepresentedType >
      >
      const& internals
    , FunctionPackageType const& function_package
    ) const
    {
      detail::execute_if
      <
        mpl::not_
        <
          mpl::and_< has_trivial_constructor< RepresentedType >
                   , typename FunctionPackageType::param_sequence_is_empty
                   > // ToDo: Change to use lazy metafunction
        >
      , action_construction_queuer
      >
      ::execute( *this, internals, function_package );
    }
  public:
    void* raw_object_storage() const
    {
      return function_queue_m->raw_object_storage();
    }
  public:/*
    // ToDo: Change return type for void
    typename base_type::inactive_value_type inactive_value() const
    {
      return base_type::inactive_value();
    }*/
  private:
    function_queue_type* function_queue_m;
  };
public:
  // ToDo: Move implementation to another file
  //       Account for void type
  template< typename ResultType >
  class action_impl
  {
    template< typename FunctionPackageType >
    friend class action_function;
  private:
    typedef detail::concurrent_action_internals< ResultType > internals_type;
    typedef shared_ptr< internals_type > shared_internals_type;
  private:
    typedef typename add_active< ResultType, concurrent_act_model >::type
              active_object_type;
  public:
    // ToDo: Possibly rewrite to eliminate extra copy of internal function
    //       ( encapsulate reference counted function )
    template< typename FunctionPackageType >
    action_impl( FunctionPackageType const& function )
      : internals_m( new internals_type )
    {
      internals_m->queue_function( internals_m, function );
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
      : internals_m( new internals_type )
    {
      target.queue_function( internals_m, function_package );
    }
  protected:
    template< typename FunctionPackageType >
    void queue_function
    (
      shared_ptr
      <
        detail::concurrent_action_internals
          < typename FunctionPackageType::result_type >
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
  public:
    bool is_complete() const
    {
      return internals_m->is_complete();
    }
    void wait() const
    {
      internals_m->wait();
    }
  public:
    active_object_type& active_object() const
    {
      return internals_m->active_object();
    }
  private:
    shared_internals_type internals_m;
  };
};

}
}

#endif
