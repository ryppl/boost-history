//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_BASIC_FOR_HPP
#define BOOST_ACT_ALGORITHM_BASIC_FOR_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>

#include "../act_fwd.hpp"

#include "../type_traits/is_algo_model.hpp"
#include "../type_traits/is_parallel_safe.hpp"

#include "basic_for/detail/expressions.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename AlgoModel
        , typename AssignType
        , typename ComparisonType
        , typename OperationType
        >
class basic_for_impl
{
public:
  basic_for_impl( AssignType assign_init
                , ComparisonType comparison_init
                , OperationType operation_init
                )
    : assign_m( assign_init )
    , comparison_m( comparison_init )
    , operation_m( operation_init )
  {
  }
public:
  // ToDo: Make compatible with Boost.Lambda
  template< typename FunctionType >
  FunctionType
  operator []( FunctionType function ) const
  {
    return AlgoModel::basic_for_impl( assign_m
                                    , comparison_m
                                    , operation_m
                                    , function
                                    );
  }
private:
  AssignType assign_m;
  ComparisonType comparison_m;
  OperationType operation_m;
};


template< typename AlgoModel
        , typename AssignType
        , typename ComparisonType
        , typename OperationType
        >
basic_for_impl< AlgoModel
              , AssignType
              , ComparisonType
              , OperationType
              >
make_basic_for_impl( AssignType init
                   , ComparisonType compare
                   , OperationType operation
                   )
{
  return basic_for_impl< AlgoModel
                       , AssignType
                       , ComparisonType
                       , OperationType
                       >( init, compare, operation );
}

template< typename AlgoModel >
struct modeled_basic_for_type
{
  template< typename AssignType
          , typename ComparisonType
          , typename OperationType
          >
  typename ::boost::enable_if
  <
    for_params_are_valid< AlgoModel
                        , AssignType
                        , ComparisonType
                        , OperationType
                        >
  , basic_for_impl< AlgoModel
                  , AssignType
                  , ComparisonType
                  , OperationType
                  >
  >
  ::type
  operator ()( AssignType init
             , ComparisonType compare
             , OperationType operation
             ) const
  {
    return make_basic_for_impl< AlgoModel >( init, compare, operation );
  }

  template< typename AssignType
          , typename ComparisonType
          , typename OperationType
          , typename FunctionType
          >
  typename ::boost::enable_if
  <
    for_params_are_valid< AlgoModel
                        , AssignType
                        , ComparisonType
                        , OperationType
                        >
  , FunctionType
  >
  ::type
  operator ()( AssignType init
             , ComparisonType compare
             , OperationType operation
             , FunctionType const& function
             ) const
  {
    return make_basic_for_impl< AlgoModel >( init
                                           , compare
                                           , operation
                                           )
                                           [ function ];
  }
};

struct basic_for_type
{
  basic_for_type()
  {
  }

  template< typename AssignType
          , typename ComparisonType
          , typename OperationType
          >
  typename ::boost::enable_if
  <
    for_params_are_valid< default_algo_model
                        , AssignType
                        , ComparisonType
                        , OperationType
                        >
  , basic_for_impl< default_algo_model
                  , AssignType
                  , ComparisonType
                  , OperationType
                  >
  >
  ::type
  operator ()( AssignType init
             , ComparisonType compare
             , OperationType operation
             ) const
  {
    return make_basic_for_impl< default_algo_model >( init
                                                    , compare
                                                    , operation
                                                    );
  }

  template< typename AssignType
          , typename ComparisonType
          , typename OperationType
          , typename FunctionType
          >
  typename ::boost::enable_if
  <
    for_params_are_valid< default_algo_model
                        , AssignType
                        , ComparisonType
                        , OperationType
                        >
  , FunctionType
  >
  ::type
  operator ()( AssignType init
             , ComparisonType compare
             , OperationType operation
             , FunctionType const& function
             ) const
  {
    return make_basic_for_impl< default_algo_model >( init
                                                    , compare
                                                    , operation
                                                    )
                                                    [ function ];
  }

  template< typename AlgoModel >
  typename ::boost::enable_if
  <
    is_algo_model< AlgoModel >
  , modeled_basic_for_type< AlgoModel >
  >
  ::type
  operator []( AlgoModel model ) const
  {
    return modeled_basic_for_type< AlgoModel >();
  }
};

}

detail::basic_for_type const basic_for;

}
}

#include "../config/default_algo_model.hpp"

#endif
