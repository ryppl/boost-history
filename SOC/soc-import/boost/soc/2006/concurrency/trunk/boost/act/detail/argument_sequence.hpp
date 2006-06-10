//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ARGUMENT_SEQUENCE_HPP
#define BOOST_ACT_DETAIL_ARGUMENT_SEQUENCE_HPP

#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/utility/enable_if.hpp>

#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType >
struct argument_sequence;

#define BOOST_ACT_DETAIL_ARGUMENT_SPEC( z, curr_num_params, dummy )            \
template< typename ReturnType                                                  \
          BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, curr_num_params, typename Param )\
        >                                                                      \
struct argument_sequence                                                       \
       < ReturnType ( BOOST_PP_ENUM_PARAMS_Z( z, curr_num_params, Param ) ) >  \
{                                                                              \
  typedef ::boost::mpl::deque                                                  \
          < BOOST_PP_ENUM_PARAMS_Z( z, curr_num_params, Param ) > type;        \
}; 

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_ARGUMENT_SPEC
               , BOOST_PP_NIL
               )

}
}
}

#endif
