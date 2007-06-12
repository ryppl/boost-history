/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_CONSUMER_ARGUMENTS_FWD_HPP
#define BOOST_ACT_CONSUMER_POLICY_CONSUMER_ARGUMENTS_FWD_HPP

#include <boost/mpl/pair.hpp>
#include <boost/parameter.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

namespace boost { namespace act {

BOOST_PARAMETER_TEMPLATE_KEYWORD( processor_info )
BOOST_PARAMETER_TEMPLATE_KEYWORD( normalization_info )
BOOST_PARAMETER_TEMPLATE_KEYWORD( constraint )

// Definition here because an argument name is logically more a declaration
// than a definition (also prevents against some possible misuse)
template< typename ProcessorType, typename SharedDataType = void >
struct processor
{
  typedef processor_info< mpl::pair< ProcessorType
                                   , SharedDataType
                                   >
                        >
                        revised_type;
};

#define BOOST_ACT_NUM_CONSUMER_PARAMS 3

#define BOOST_ACT_CONSUMER_PARAMS_DECL()                                       \
  BOOST_PP_ENUM_BINARY_PARAMS( BOOST_ACT_NUM_CONSUMER_PARAMS                   \
                             , typename Arg                                    \
                             , = ::boost::parameter::void_ BOOST_PP_INTERCEPT  \
                             )

#define BOOST_ACT_CONSUMER_PARAMS()                                            \
  BOOST_PP_ENUM_PARAMS( BOOST_ACT_NUM_CONSUMER_PARAMS, typename Arg )

#define BOOST_ACT_CONSUMER_ARGS()                                              \
  BOOST_PP_ENUM_PARAMS( BOOST_ACT_NUM_CONSUMER_PARAMS, Arg )

} }

#endif
