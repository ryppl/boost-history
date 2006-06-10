//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_CALL_PARAMS_ARE_VALID_HPP
#define BOOST_ACT_DETAIL_ACTIVE_CALL_PARAMS_ARE_VALID_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits/function_traits.hpp>

#include "argument_sequence.hpp"
#include "meta_for.hpp"

// ToDo: Unwrap reference wrappers

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Take into account reference types and futures
template< typename SourceType
        , typename FunctionType
        , ::std::size_t Index
        >
struct is_valid_n_param
  : ::boost::is_same
    <
      typename ::boost::remove_cv< typename ::boost::remove_reference< SourceType >::type >::type
    , typename ::boost::remove_cv
      <
        typename ::boost::mpl::at_c
        <
          typename argument_sequence< FunctionType >::type
        , Index
        >
        ::type
      >
      ::type
    >
{
};

template< typename FunctionType
        , typename ParamSequence
        , typename CurrIndex
        , typename IsValid
        >
struct param_checker_state
{
  typedef FunctionType function_type;
  typedef ParamSequence param_sequence;
  typedef CurrIndex curr_index;
  typedef IsValid is_valid;
};

template< typename ParamCheckerState >
struct param_checker_predicate
  : ::boost::mpl::and_
    <
      typename ParamCheckerState::is_valid
    , ::boost::mpl::less
      <
        typename ParamCheckerState::curr_index
      , typename ::boost::mpl::size
          < typename ParamCheckerState::param_sequence >::type
      >
    >
{
};

template< typename ParamCheckerState >
struct param_checker_op
  : ParamCheckerState
{
  typedef typename ::boost::mpl::next
          <
            typename ParamCheckerState::curr_index
          >
          ::type curr_index;

  typedef param_checker_op type;
};

template< typename ParamCheckerState >
struct param_checker_body
  : ParamCheckerState
{
  typedef typename is_valid_n_param
          <
            typename ::boost::mpl::at
            <
              typename ParamCheckerState::param_sequence
            , typename ParamCheckerState::curr_index
            >
            ::type
          , typename ParamCheckerState::function_type
          , ParamCheckerState::curr_index::value
          >
          ::type is_valid;

  typedef param_checker_body type;
};

template< typename FunctionType
        , typename ParamSequence
        >
struct active_call_params_are_valid_impl
  : meta_for< param_checker_state
              <
                FunctionType
              , ParamSequence
              , ::boost::mpl::size_t< 0 >
              , ::boost::mpl::true_
              >
            , param_checker_predicate< ::boost::mpl::_1 >
            , param_checker_op< ::boost::mpl::_1 >
            , param_checker_body< ::boost::mpl::_1 >
            >
            ::type::is_valid
{
};

template< typename FunctionType
        , typename ParamSequence
        >
struct active_call_params_are_valid
  : ::boost::mpl::and_
    <
      ::boost::mpl::equal_to
      <
        typename ::boost::mpl::size< ParamSequence >::type
      , ::boost::mpl::size_t< ::boost::function_traits< FunctionType >::arity >
      >
    , active_call_params_are_valid_impl< FunctionType, ParamSequence >
    >
{
};

}
}
}

#endif
