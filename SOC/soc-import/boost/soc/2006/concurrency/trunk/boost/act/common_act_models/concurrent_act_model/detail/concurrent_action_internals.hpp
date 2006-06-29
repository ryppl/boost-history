//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_DETAIL_CONCURRENT_ACTION_INTERNALS_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_DETAIL_CONCURRENT_ACTION_INTERNALS_HPP

#include <boost/noncopyable.hpp>

#include "../../../trigger.hpp"

#include "../../../active/active_no_default.hpp"

#include "concurrent_construction_tags.hpp"

namespace boost
{
namespace act
{

namespace detail
{

template< typename ResultType >
class concurrent_action_internals
  : noncopyable
{
private:
    typedef BOOST_ACTIVE_M_T((ResultType),(concurrent_act_model))
              active_object_type;
public:
  concurrent_action_internals()
    : active_object_m( active_impl_constructor_tag()
                     , concurrent_no_active_construction_tag()
                     )
  {
  }
public:
  void wait() const
  {
    completion_trigger_m.wait();
  }

  void set_trigger() const
  {
    completion_trigger_m = true;
  }

  active_object_type& active_object()
  {
    return active_object_m;
  }

  template< typename FunctionPackageType >
  void queue_action_construction
  (
    shared_ptr
    <
      detail::concurrent_action_internals< ResultType >
    >
    const& internals
  , FunctionPackageType const& function_package
  ) const
  {
    active_object_m.impl().queue_action_construction( internals, function_package );
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
    active_object_m.impl().queue_function( internals, function_package );
  }
private:
  trigger completion_trigger_m;
  active_object_type active_object_m;
};

}
}
}

#endif
