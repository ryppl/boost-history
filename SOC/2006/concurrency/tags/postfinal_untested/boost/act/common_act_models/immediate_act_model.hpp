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

#ifndef BOOST_ACT_COMMON_ACT_MODELS_IMMEDIATE_ACT_MODEL_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_IMMEDIATE_ACT_MODEL_HPP

#include <boost/type_traits/add_reference.hpp>

#include "../act_model.hpp"

#include "../active/active_no_default.hpp"

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
    : add_reference< SourceType >
  {
  };

  // ToDo: Allow construction
  //       Make cv-qualified queue_function_impl overloads
  template< typename RepresentedType >
  class active_impl
    : public detail::object_encapsulator< RepresentedType >
  {
    typedef detail::object_encapsulator< RepresentedType > base_type;
  public:
    template< typename FunctionPackageType >
    active_impl( FunctionPackageType function_package )
      : base_type( function_package )
    {
    }

    template< typename FunctionType >
    active_impl( active_impl_constructor_tag, FunctionType const& function )
      : base_type( function )
    {
    }

    template< typename ActiveImplType
            , typename FunctionType
            >
    active_impl( active_impl_constructor_tag
               , ActiveImplType& target
               , FunctionType const& function_package
               )
      : base_type( target, function_package )
    {
    }

    template< typename ActiveImplType
            , typename FunctionType
            >
    active_impl( active_impl_constructor_tag
               , ActiveImplType const& target
               , FunctionType const& function_package
               )
      : base_type( target, function_package )
    {
    }

    ~active_impl()
    {
      this->destroy_object();
    }
  public:
    typename base_type::inactive_value_type inactive_value() const
    {
      return base_type::inactive_value();
    }
  };

  // ToDo: Make stored object mutable
  // ToDo: Possibly move implementation to another file
  template< typename ResultType >
  class action_impl
  {
  private:
    typedef typename add_active< ResultType, immediate_act_model >
              ::type active_object_type;
  protected:
    action_impl()
    {
    }
  protected:
    template< typename FunctionType >
    action_impl( FunctionType const& function )
      : active_object_m( active_impl_constructor_tag(), function )
    {
    }

    template< typename ActiveImplType
            , typename FunctionType
            >
    action_impl( ActiveImplType& target
               , FunctionType const& function_package
               )
      : active_object_m( active_impl_constructor_tag()
                       , target, function_package
                       )
    {
    }

    template< typename ActiveImplType
            , typename FunctionType
            >
    action_impl( ActiveImplType const& target
               , FunctionType const& function_package
               )
      : active_object_m( active_impl_constructor_tag()
                       , target, function_package
                       )
    {
    }
  public:
    active_object_type& active_object() const
    {
      return active_object_m;
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
  public:
    typedef ResultType inactive_value_type;

    typename active_impl< ResultType >::stored_object_return_type
    stored_object() const
    {
      return active_object().impl().stored_object();
    }
  private:
    // ToDo: Rewrite to use empty base optimization
    mutable active_object_type active_object_m;
  };
};

}
}

#endif
