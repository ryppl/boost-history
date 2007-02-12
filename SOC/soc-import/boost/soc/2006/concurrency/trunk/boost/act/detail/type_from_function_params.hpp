/*=============================================================================
    Copyright (c) 2006-2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

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
