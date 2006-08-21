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
