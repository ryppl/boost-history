//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_CALL_ENABLERS_HPP
#define BOOST_ACT_DETAIL_ACTIVE_CALL_ENABLERS_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/empty_base.hpp>

#include "../action.hpp"
#include "../type_traits/is_act_model.hpp"
#include "../type_traits/is_active.hpp"
#include "../type_traits/remove_active.hpp"


#include "active_call_params_are_valid.hpp"


//ToDo: Take into account active refs (from actions and temporary actives)

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Fix this
template< typename ActableFunctionType, typename ActModel >
struct action_from_actable_function
{
  typedef action< typename ::boost::function_traits< ActableFunctionType >
                    ::result_type
                , ActModel
                >
                type;
};

template< typename ActModel
        , typename FunctionType
        , typename PassedParamSequence
        >
struct active_call_enabler_condition
  : ::boost::mpl::and_
    <
      is_act_model< ActModel >
    , active_call_params_are_valid
      <
        FunctionType
      , PassedParamSequence
      >
    >
{
};

template< typename ActModel
        , typename FunctionType
        , typename PassedParamSequence
        >
struct active_call_enabler
  : ::boost::lazy_enable_if
    <
      active_call_enabler_condition< ActModel
                                   , FunctionType
                                   , PassedParamSequence
                                   >
    , action_from_actable_function< FunctionType, ActModel >
    >
{
};

template< typename ActModel
        , typename FunctionType
        , typename PassedParamSequence
        >
struct active_call_disabler
  : ::boost::disable_if
    <
      active_call_enabler_condition< ActModel
                                   , FunctionType
                                   , PassedParamSequence
                                   >
    >
{
};

template< typename ActiveObject
        , typename FunctionType
        , typename PassedParamSequence
        >
struct queue_function_enabler_impl
  : active_call_enabler< typename ActiveObject::act_model
                       , FunctionType
                       , typename ::boost::mpl::push_front
                         <
                           PassedParamSequence
                         , typename remove_active< ActiveObject >::type
                         >
                         ::type
                       >
{
};

template< typename ActiveObject
        , typename FunctionType
        , typename PassedParamSequence
        >
struct queue_function_enabler
  : ::boost::mpl::if_
    <
      is_active< ActiveObject >
    , queue_function_enabler_impl
      <
        ActiveObject
      , FunctionType
      , PassedParamSequence
      >
    , ::boost::mpl::empty_base
    >
    ::type
{
};

}
}
}

#endif
