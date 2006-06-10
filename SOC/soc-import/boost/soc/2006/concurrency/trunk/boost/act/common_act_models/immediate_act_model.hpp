//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ACT_MODELS_IMMEDIATE_ACT_MODEL_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_IMMEDIATE_ACT_MODEL_HPP

#include <boost/type_traits/add_reference.hpp>

#include "../act_model.hpp"

#include "../detail/object_encapsulator.hpp"

namespace boost
{
namespace act
{

struct immediate_act_model
  : act_model
{
  // ToDo: Possibly change this to pass simple types by value
  //       (use call_traits for non-array types)
  template< typename SourceType >
  struct param
    : ::boost::add_reference< SourceType >
  {
  };

  // ToDo: Make stored object mutable
  // ToDo: Possibly move implementation to another file
  template< typename ResultType >
  class action_impl
    : public detail::object_encapsulator< ResultType > // ToDo: Make private
  {
  public:
    typedef detail::object_encapsulator< ResultType > object_base_type;
  protected:
    action_impl()
    {
    }
  protected:
    template< typename FunctionType >
    action_impl( FunctionType const& function )
      : object_base_type( function )
    {
    }

    template< typename ActiveImplType
            , typename FunctionType
            >
    action_impl( ActiveImplType& target, FunctionType const& function_package )
      : object_base_type( target, function_package )
    {
    }

    template< typename ActiveImplType
            , typename FunctionType
            >
    action_impl( ActiveImplType const& target, FunctionType const& function_package )
      : object_base_type( target, function_package )
    {
    }
  protected:
    bool is_complete() const
    {
      return true;
    }

    void wait() const
    {
      return;
    }
  };

  // ToDo: Allow construction
  //       Make cv-qualified queue_function_impl overloads
  template< typename RepresentedType >
  class active_impl
    : public detail::object_encapsulator< RepresentedType >
  {
    typedef detail::object_encapsulator< RepresentedType > base_type;
  public:
    typename base_type::inactive_value_type inactive_value() const
    {
      return base_type::inactive_value();
    }
  };
};

}
}

#endif
