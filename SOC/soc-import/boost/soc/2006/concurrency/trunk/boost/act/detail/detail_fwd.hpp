//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_DETAIL_FWD_HPP
#define BOOST_ACT_DETAIL_DETAIL_FWD_HPP

namespace boost
{
namespace act
{
namespace detail
{

template< typename ActModel
        , typename ActableFunctionType
        , typename PassedParamSequence
        >
struct active_call_enabler;

template< typename ActModel
        , typename ActableFunctionType
        , typename PassedParamSequence
        >
struct active_call_disabler;

template< typename ActiveObject
        , typename ActableFunctionType
        , typename PassedParamSequence
        >
struct queue_function_enabler;

template< typename ActiveObject
        , typename ActableFunctionType
        , typename PassedParamSequence
        >
struct queue_function_disabler;

}
}
}

#endif
