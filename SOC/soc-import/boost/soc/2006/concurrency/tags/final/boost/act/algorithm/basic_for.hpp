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
#include "detail/make_algo.hpp"

#include "basic_for/detail/expressions.hpp"

#include "basic_for/basic_for_fwd.hpp"

// ToDo: Account for parallel safety

namespace boost
{
namespace act
{
BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION( basic_for
                                            , 2
                                            , ( ( ((typename),AssignType)
                                                  ((typename),ComparisonType)
                                                  ((typename),OperationType)
                                                  ((typename),FunctionType)
                                                , ( ::boost::enable_if
                                                    <
                                                      ::boost::act::detail
                                                        ::for_params_are_valid
                                                      < AssignType
                                                      , ComparisonType
                                                      , OperationType
                                                      >
                                                    , FunctionType
                                                    >
                                                  )
                                                , ((AssignType),assign)
                                                  ((ComparisonType),condition)
                                                  ((OperationType),step)
                                                  ((FunctionType),body)
                                                )
                                              , ( ((typename),AssignType)
                                                  ((typename),ComparisonType)
                                                  ((typename),OperationType)
                                                , ( ::boost::enable_if
                                                    <
                                                      ::boost::act::detail
                                                        ::for_params_are_valid
                                                      < AssignType
                                                      , ComparisonType
                                                      , OperationType
                                                      >
                                                    , ::boost::act::detail
                                                          ::bound_basic_for
                                                          < ExtendedParamsType
                                                          , AssignType
                                                          , ComparisonType
                                                          , OperationType
                                                          >
                                                    >
                                                  )
                                                , ((AssignType),assign)
                                                  ((ComparisonType),condition)
                                                  ((OperationType),step)
                                                )
                                              )
                                            )

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER( ((typename),AssignType)
                                      ((typename),ComparisonType)
                                      ((typename),OperationType)
                                      ((typename),FunctionType)
                                    , ( ::boost::enable_if
                                        <
                                          ::boost::act::detail::for_params_are_valid
                                          < AssignType
                                          , ComparisonType
                                          , OperationType
                                          >
                                        , FunctionType
                                        >
                                      )
                                    , basic_for
                                    , ((AssignType),assign)
                                      ((ComparisonType),condition)
                                      ((OperationType),step)
                                      ((FunctionType),body)
                                    )
{
  typedef typename AssignType::value_type for_var_type;

  for( for_var_type var = assign()
     ; condition( var )
     ; step( var )
     )
  {
    for_var_type const& const_var = var;

    body( const_var );
  }

  return body;
}


BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER( ((typename),AssignType)
                                      ((typename),ComparisonType)
                                      ((typename),OperationType)
                                    , ( ::boost::enable_if
                                        <
                                          ::boost::act::detail
                                            ::for_params_are_valid
                                          < AssignType
                                          , ComparisonType
                                          , OperationType
                                          >
                                        , ::boost::act::detail
                                              ::bound_basic_for
                                              < ExtendedParamsType
                                              , AssignType
                                              , ComparisonType
                                              , OperationType
                                              >
                                        >
                                      )
                                    , basic_for
                                    , ((AssignType),assign)
                                      ((ComparisonType),condition)
                                      ((OperationType),step)
                                    )
{
  return detail::make_bound_basic_for
           ( extended_params, assign, condition, step );
}

}
}


/*
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

  template< typename FunctionType >
  FunctionType
  operator ()( FunctionType function ) const
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

detail::basic_for_type const basic_for = detail::basic_for_type();

}
}
*/

#include "../config/default_algo_model.hpp"

#endif
