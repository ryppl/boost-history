/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_BOUNDED_QUEUE_ARGS_FWD_HPP
#define BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_BOUNDED_QUEUE_ARGS_FWD_HPP

#include <cstddef>


// ToDo: Use more fine-grained headers
#include <boost/parameter.hpp>


#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#define BOOST_ACT_NUM_BOUNDED_QUEUE_POLICY_PARAMS() 2

#define BOOST_ACT_BOUNDED_QUEUE_POLICY_PARAMS_DECL()                           \
  BOOST_PP_ENUM_BINARY_PARAMS( BOOST_ACT_NUM_BOUNDED_QUEUE_POLICY_PARAMS()     \
                             , typename Arg                                    \
                             , = ::boost::parameter::void_ BOOST_PP_INTERCEPT  \
                             )

#define BOOST_ACT_BOUNDED_QUEUE_POLICY_PARAMS()                                \
  BOOST_PP_ENUM_PARAMS( BOOST_ACT_NUM_BOUNDED_QUEUE_POLICY_PARAMS()            \
                      , typename Arg                                           \
                      )

#define BOOST_ACT_BOUNDED_QUEUE_POLICY_ARGS()                                  \
  BOOST_PP_ENUM_PARAMS( BOOST_ACT_NUM_BOUNDED_QUEUE_POLICY_PARAMS(), Arg )

namespace boost { namespace act {

BOOST_PARAMETER_TEMPLATE_KEYWORD( max_size_t )
BOOST_PARAMETER_TEMPLATE_KEYWORD( allocator )

template< ::std::size_t MaxSize >
struct max_size;

} }

#endif
