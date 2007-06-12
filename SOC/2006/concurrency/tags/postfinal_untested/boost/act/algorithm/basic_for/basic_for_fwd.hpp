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

#ifndef BOOST_ACT_ALGORITHM_BASIC_FOR_BASIC_FOR_FWD_HPP
#define BOOST_ACT_ALGORITHM_BASIC_FOR_BASIC_FOR_FWD_HPP

#include "../detail/make_algo.hpp"

#include "detail/expressions.hpp"

namespace boost
{
namespace act
{
namespace detail
{
template< typename ExtendedParams
        , typename AssignType
        , typename ComparisonType
        , typename OperationType
        >
class bound_basic_for
{
public:
  bound_basic_for( ExtendedParams const& extended_params_init
                 , AssignType assign_init
                 , ComparisonType comparison_init
                 , OperationType operation_init
                 )
    : extended_params_m( extended_params_init )
    , assign_m( assign_init )
    , comparison_m( comparison_init )
    , operation_m( operation_init )
  {
  }
private:
  typedef typename associated_algo_model< ExtendedParams >::type algo_model;
public:
  // ToDo: Possibly change to take function not by const ref
  template< typename FunctionType >
  FunctionType
  operator []( FunctionType const& function ) const
  {
    return ::boost::act::basic_for_impl< algo_model >
             ::execute( extended_params_m
                      , assign_m
                      , comparison_m
                      , operation_m
                      , function
                      );
  }

  template< typename FunctionType >
  FunctionType
  operator ()( FunctionType const& function ) const
  {
    return ::boost::act::basic_for_impl< algo_model >
             ::execute( extended_params_m
                      , assign_m
                      , comparison_m
                      , operation_m
                      , function
                      );
  }
private:
  ExtendedParams const extended_params_m;
  AssignType assign_m;
  ComparisonType comparison_m;
  OperationType operation_m;
};

template< typename ExtendedParams
        , typename AssignType
        , typename ComparisonType
        , typename OperationType
        >
bound_basic_for< ExtendedParams
               , AssignType
               , ComparisonType
               , OperationType
               >
make_bound_basic_for( ExtendedParams const& extended_params_init
                    , AssignType assign_init
                    , ComparisonType comparison_init
                    , OperationType operation_init
                    )
{
  return bound_basic_for< ExtendedParams
                        , AssignType
                        , ComparisonType
                        , OperationType
                        >
                        ( extended_params_init
                        , assign_init
                        , comparison_init
                        , operation_init
                        );
}

}

BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS( basic_for
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
}
}

#endif

/*

type (a,b,c)

a

#define HELPER_type(dummy) 1

#define TYPE( thing )
HELPER_ ## thing

*/
