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

#ifndef BOOST_ACT_FUNCTIONAL_ALWAYS_HPP
#define BOOST_ACT_FUNCTIONAL_ALWAYS_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/act/config/max_params.hpp>

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Define nested result_of

template< typename ChildType, typename ResultType >
struct always_base
{
#define BOOST_ACT_DETAIL_ALWAYS_OP( z, curr_num_params, dummy )                \
template< BOOST_ENUM_PARAMS_Z( z, curr_num_params, typename Type ) >           \
ResultType operator ()( BOOST_ENUM_BINARY_PARAMS_Z( z, curr_num_params         \
                                                  , Type const&, arg           \
                                                  )                            \
{                                                                              \
  return static_cast< ChildType const& >( *this )();                           \
}

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_PP_INC( BOOST_ACT_MAX_PARAMS )
                       , BOOST_ACT_DETAIL_ALWAYS_OP
                       , BOOST_PP_NIL
                       )
};

}

template< typename Type >
class always
  : private detail::always_base< always< Type >, Type >
{
public:
  always() {}

  // ToDo: Support in_place_factory
  

  always( Type const& value_init )
    : value_m( value_init )
  {
  }
public:
  Type operator ()() const
  {
    return value_m;
  }

  using detail::always_base< always< Type >, Type >::operator ();
private:
  // ToDo: Use raw storage (so in_place_factory can be used)
  Type value_m;
};

template< typename FunctionEncapsulator >
class static_always
  : private detail::always_base< static_always< FunctionEncapsulator >
                               , typename FunctionEncapsulator::result_type
                               >
{
public:
  typename FunctionEncapsulator::result_type operator ()() const
  {
    return FunctionEncapsulator::execute;
  }

  using detail::always_base< static_always< FunctionEncapsulator >
                           , typename FunctionEncapsulator::result_type
                           >
          ::operator ();
};

template< typename Type, Type Value >
class simple_always
  : private detail::always_base< simple_always< Type, Value >, Type >
{
public:
  Type operator ()() const
  {
    return Value;
  }

  using detail::always_base< simple_always< Type, Value >, Type >::operator ();
};

typedef simple_always< bool, true > always_true_type;
typedef simple_always< bool, false > always_false_type;

always_true_type const always_true;
always_false_type const always_false;

}
}

#endif
