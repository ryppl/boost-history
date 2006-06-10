//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ACT_MODELS_LAZY_ACT_MODEL_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_LAZY_ACT_MODEL_HPP

#include <exception>

#include "../act_model.hpp"
#include "../type_traits/add_action.hpp"

#include "../detail/object_encapsulator.hpp"

namespace boost
{
namespace act
{

struct lazy_act_model
  : act_model< lazy_act_model
             , is_synchronous
             >
{
  typedef ::boost::mpl::true_ uses_parallel_for;

  typedef ::boost::mpl::false_ uses_serial_for;

  // ToDo: Possibly change this to pass simple types by value
  //       Account for array types
  template< typename SourceType >
  struct param
  {
    typedef SourceType type;
  };
public:

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
    action_impl( ActiveImplType& target, FunctionType const& function )
      : object_base_type( target.stored_object, function )
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

  // ToDo: Use empty base optimization where applicable
  //       Allow construction
  //       Make cv-qualified queue_function_impl overloads
  template< typename RepresentedType >
  class active_impl
    : public detail::object_encapsulator< RepresentedType >
  {
    typedef detail::object_encapsulator< RepresentedType > base_type;
  public:
    // ToDo: Change return type for void
    typename base_type::inactive_value_type inactive_value() const
    {
      return base_type::inactive_value();
    }
  };
};
};

}
}

#endif
