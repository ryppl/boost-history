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

#ifndef BOOST_ACT_DETAIL_TYPE_FROM_FUNCTION_PARAMS_HPP
#define BOOST_ACT_DETAIL_TYPE_FROM_FUNCTION_PARAMS_HPP

// ToDo: Just include declaration
#include <boost/utility/enable_if.hpp>

namespace boost
{
namespace act
{

template< typename Type >
struct lazy_type
  : Type
{
};

template< typename Type >
struct possible_void_type
{
  typedef Type type;
};

namespace detail
{

template< typename FunctionType, typename Dummy = void >
struct type_from_function_params;

template< typename ReturnType, typename Dummy >
struct type_from_function_params< ReturnType(), Dummy >
{
  typedef void type;
};

template< typename ReturnType, typename ParamType, typename Dummy >
struct type_from_function_params< ReturnType( ParamType ), Dummy >
{
  typedef ParamType type;
};

template< typename ReturnType, typename Type, typename Dummy >
struct type_from_function_params< ReturnType( lazy_type< Type > ), Dummy >
  : lazy_type< Type >
{
};

template< typename ReturnType, bool Condition, typename TargetType
        , typename Dummy
        >
struct type_from_function_params
         < ReturnType( enable_if_c< Condition, TargetType > ), Dummy >
  : enable_if_c< Condition, TargetType >
{
};

template< typename ReturnType, bool Condition, typename TargetType
        , typename Dummy
        >
struct type_from_function_params
         < ReturnType( lazy_enable_if_c< Condition, TargetType > ), Dummy >
  : lazy_enable_if_c< Condition, TargetType >
{
};

template< typename ReturnType, typename Condition, typename TargetType
        , typename Dummy
        >
struct type_from_function_params
         < ReturnType( enable_if< Condition, TargetType > ), Dummy >
  : enable_if< Condition, TargetType >
{
};

template< typename ReturnType, typename Condition, typename TargetType
        , typename Dummy
        >
struct type_from_function_params
         < ReturnType( lazy_enable_if< Condition, TargetType > ), Dummy >
  : lazy_enable_if< Condition, TargetType >
{
};

}
}
}

#endif
