//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BFT_INTERFACE_MACROS_HPP
#define BOOST_BFT_INTERFACE_MACROS_HPP


/** Helps produce the sequence T0, ... TN, both with and with out typename 
 * before it.
 */
#define BOOST_BFT_INTERFACE_PARAM_LIST(Z,N,DATA) \
    DATA ## N

/** Used for creating template parameters for specilization.
 *  Creates the following pattern
 *      typename T0, ... typename TN
 */
#define BOOST_BFT_TEMPLATE_ARGS_NO_DEFAULTS() \
    BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                    BOOST_BFT_INTERFACE_PARAM_LIST, \
                    typename T )


/** Used for specilizing over the bitfield_tuple sequence.
 *  Creates the following pattern
 *      T0, ... TN
 */
#define BOOST_BFT_SPECILIZATION_PARAM_LIST() \
    BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                    BOOST_BFT_INTERFACE_PARAM_LIST, \
                    T )

#endif
