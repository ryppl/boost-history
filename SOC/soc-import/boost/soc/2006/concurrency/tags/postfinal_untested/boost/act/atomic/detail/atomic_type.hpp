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

#ifndef BOOST_ACT_ATOMIC_DETAIL_ATOMIC_TYPE_HPP
#define BOOST_ACT_ATOMIC_DETAIL_ATOMIC_TYPE_HPP

#include <boost/tuple/tuple.hpp> // ToDo: Remove

#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/mpl/deque.hpp>

#include "../../active/active_interface.hpp"

#include "../../detail/active_helper.hpp"

#include "../../act_model.hpp"

#include <boost/bind.hpp>

#include "../../config/max_active_call_params.hpp"

#include "../../detail/function_package.hpp"
#include "../../detail/tie.hpp"
#include "../../active/detail/constructor_caller.hpp"

#include "../../common_act_models/concurrent_act_model.hpp"

#include "../../active/active_no_default.hpp"

namespace boost
{
namespace act
{
namespace detail
{

struct direct_return_meta
{
  template< typename Type, typename ActModel >
  struct apply
  {
    typedef Type type;
  };
};

template< typename UnqualifiedType, typename Enabler = void >
class atomic_type;

// Note: This will not work until a thread-safe concurrent active type
//       implementation is completed.
#if 0

// ToDo: Error if Type is qualified
template< typename UnqualifiedType, typename Enabler = void >
class atomic_type
  : public active_interface< UnqualifiedType
                           , active_details
                             <
                               atomic_type< UnqualifiedType
                                          , concurrent_act_model
                                          >
                             , concurrent_act_model
                             , direct_return_meta
                             >
                           >
  , BOOST_ACTIVE_M_T_BASE((UnqualifiedType),(concurrent_act_model))
{
public:
  typedef UnqualifiedType represented_type;
  typedef UnqualifiedType value_type;
private:
  typedef concurrent_act_model::active_impl< UnqualifiedType >
            active_impl_base_type;

  typedef BOOST_ACTIVE_M_T((UnqualifiedType),(concurrent_act_model))
            active_type_base;
public: // ToDo: Make private
  active_impl_base_type& impl()
  {
    return static_cast< active_impl_base_type& >( *this );
  }

  active_impl_base_type const& impl() const
  {
    return static_cast< active_impl_base_type const& >( *this );
  }
public:
  atomic_type()
  {
  }

// ToDo: Unwrap references
// ToDo: Make implicit constructor versions if is non-explicitly convertible
#define BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_MACRO( z, num_params, dummy )      \
  template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Param ) >          \
  explicit atomic_type( BOOST_PP_ENUM_BINARY_PARAMS_Z( z, num_params           \
                                                     , Param                   \
                                                     , & arg                   \
                                                     )                         \
                      )                                                        \
    : active_type_base                                                         \
      (                                                                        \
        BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg )                           \
      )                                                                        \
  {                                                                            \
  }

  BOOST_PP_REPEAT_FROM_TO( 1, BOOST_PP_INC( BOOST_ACT_MAX_PARAMS )
                         , BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_MACRO
                         , BOOST_PP_NIL
                         )

#undef BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_MACRO
public:
  UnqualifiedType inactive_value() const
  {
    return active_impl_base_type::inactive_value();
  }

  friend UnqualifiedType value_of( atomic_type const& target )
  {
    return target.inactive_value();
  }
};

#endif

}
}
}

#endif
