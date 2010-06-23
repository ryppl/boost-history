//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>

#ifndef BOOST_BITFIELD_TUPLE_TEMPLATE_EXPANSION_MACROS_HPP
#define BOOST_BITFIELD_TUPLE_TEMPLATE_EXPANSION_MACROS_HPP

#ifndef BOOST_BFT_PARAM_COUNT
#define BOOST_BFT_PARAM_COUNT 10
#endif

#define BOOST_MAKE_BFT_TEMPLATE_PARAMS() \
            BOOST_PP_ENUM(BOOST_BFT_PARAM_COUNT, BOOST_BFT_MAKE_PARAMS, NOTHING)

/** Macro for generating template parameters
 *  for a bitfield_tuple.
 */
#define BOOST_BFT_MAKE_PARAMS(Z, N, DATA) \
            typename T ## N = mpl::void_


#endif
