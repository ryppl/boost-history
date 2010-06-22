//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>

#ifndef BOOST_BITFIELD_TUPLE_TEMPLATE_EXPANSION_MACROS_HPP
#define BOOST_BITFIELD_TUPLE_TEMPLATE_EXPANSION_MACROS_HPP

/** Macro for generating template parameters
 *  for a bitfield_tuple.
 */
#define BOOST_BFT_MAKE_PARAMS(Z, N, DATA) \
            typename T##( BOOST_PP_ADD(N,1) ) = mpl::void_


#endif
