//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ACT_FWD_HPP
#define BOOST_ACT_ACT_FWD_HPP

#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include "detail/detail_fwd.hpp"

#include "config/max_active_call_params.hpp"

#include "detail/enabler_dummy.hpp"

#include "algorithm/detail/make_algo.hpp"

namespace boost
{
namespace act
{

struct act_model;

struct immediate_act_model;
struct lazy_act_model;
struct concurrent_act_model;

typedef immediate_act_model default_act_model;

struct parallel_safe;

struct parallel_algo_model;
struct serial_algo_model;

typedef serial_algo_model default_algo_model;

template< typename RepresentedType = void
        , typename ActModel = default_act_model
        >
class action;

template< typename Type, typename ActiveDetails >
class active_interface;

#define BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNCTION_DECL( z, num_params, dummy )    \
template< typename ActiveObjectType                                            \
        , typename ActableFunctionType                                         \
          BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )     \
        >                                                                      \
typename detail::queue_function_enabler                                        \
<                                                                              \
  ActiveObjectType                                                             \
, ActableFunctionType                                                          \
, ::boost::mpl::deque                                                          \
    < BOOST_PP_ENUM_BINARY_PARAMS_Z(z,num_params,Param,&BOOST_PP_INTERCEPT) >  \
>                                                                              \
::type                                                                         \
queue_function                                                                 \
(                                                                              \
  ActiveObjectType& object                                                     \
, ActableFunctionType const& function                                          \
  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, & arg )        \
);                                                                             \
                                                                               \
template< typename ActiveObjectType                                            \
        , typename ActableFunctionType                                         \
          BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )     \
        >                                                                      \
typename detail::queue_function_disabler                                       \
<                                                                              \
  ActiveObjectType                                                             \
, ActableFunctionType                                                          \
, ::boost::mpl::deque                                                          \
    < BOOST_PP_ENUM_BINARY_PARAMS_Z(z,num_params,Param,&BOOST_PP_INTERCEPT) >  \
>                                                                              \
::type                                                                         \
queue_function                                                                 \
(                                                                              \
  ActiveObjectType& object                                                     \
, ActableFunctionType const& function                                          \
  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, & arg )        \
); 

BOOST_PP_REPEAT( BOOST_PP_DEC( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS )
               , BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNCTION_DECL
               , BOOST_PP_NIL
               )

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType)
                                 ((typename),FunctionType)
                               , (FunctionType)
                               , for_each
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((FunctionType),function)
                               )

template< typename AlgoModel >
struct for_each_safe_random_impl;

template< typename AlgoModel >
struct for_each_safe_forward_impl;

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),SourceIteratorType)
                                 ((typename),TargetIteratorType)
                               , (TargetIteratorType)
                               , copy
                               , ((SourceIteratorType),source_begin)
                                 ((SourceIteratorType),source_end)
                                 ((TargetIteratorType),target_begin)
                               )

template< typename AlgoModel >
struct copy_safe_random_impl;

template< typename AlgoModel >
struct copy_safe_forward_impl;

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType)
                                 ((typename),ValueType)
                               , (IteratorType)
                               , find
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((ValueType const&),value)
                               )

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType)
                                 ((typename),PredicateType)
                               , (IteratorType)
                               , find_if
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((PredicateType),predicate)
                               )

template< typename AlgoModel >
struct find_if_safe_random_impl;

template< typename AlgoModel >
struct find_if_safe_forward_impl;

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType)
                                 ((typename),FunctionType)
                               , (void)
                               , generate
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((FunctionType),function)
                               )

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType)
                                 ((typename),ValueType)
                               , (void)
                               , fill
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((ValueType const&),value)
                               )

//ToDo: Come up with a way to handle overloaded algorithms

/*
BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType1)
                                 ((typename),IteratorType2)
                               , (IteratorType1)
                               , find_end
                               , ((IteratorType1),begin1)
                                 ((IteratorType1),end1)
                                 ((IteratorType2),begin2)
                                 ((IteratorType2),end2)
                               )

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType1)
                                 ((typename),IteratorType2)
                                 ((typename),PredicateType)
                               , (IteratorType1)
                               , find_end
                               , ((IteratorType1),begin1)
                                 ((IteratorType1),end1)
                                 ((IteratorType2),begin2)
                                 ((IteratorType2),end2)
                                 ((PredicateType),predicate)
                               )

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType1)
                                 ((typename),IteratorType2)
                               , (IteratorType1)
                               , find_first_of
                               , ((IteratorType1),begin1)
                                 ((IteratorType1),end1)
                                 ((IteratorType2),begin2)
                                 ((IteratorType2),end2)
                               )

BOOST_ACT_DETAIL_MAKE_ALGO_DECL( ((typename),IteratorType1)
                                 ((typename),IteratorType2)
                                 ((typename),PredicateType)
                               , (IteratorType1)
                               , find_first_of
                               , ((IteratorType1),begin1)
                                 ((IteratorType1),end1)
                                 ((IteratorType2),begin2)
                                 ((IteratorType2),end2)
                                 ((PredicateType),predicate)
                               )
*/
}
}

#endif
