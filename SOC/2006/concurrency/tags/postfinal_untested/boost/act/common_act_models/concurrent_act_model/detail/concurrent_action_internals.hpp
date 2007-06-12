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
    typedef typename add_active< ResultType, concurrent_act_model >
              ::type active_object_type;
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

  void set_trigger()
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

  void* raw_object_storage() const
  {
    return active_object_m.impl().raw_object_storage();
  }
private:
  trigger completion_trigger_m;
  active_object_type active_object_m;
};

}
}
}

#endif
