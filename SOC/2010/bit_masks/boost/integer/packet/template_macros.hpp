//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTEGER_PACKET_TEMPLATE_EXPANTION_MACROS_HPP
#define BOOST_INTEGER_PACKET_TEMPLATE_EXPANTION_MACROS_HPP

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#define BOOST_PACKET_SIZE_MAX 10

/** Helps create the T1, ... TN.
 *  DATA is T,
 *  COUNT is N,
 */
#define BOOST_PACKET_TEMPLATE_PARAMETER(z,COUNT,DATA) \
    DATA ## COUNT

/**
 *
 */
#define BOOST_PACKET_TEMPLATE_PARAMETER_SEQUENCE() \
    BOOST_PP_ENUM(  BOOST_PACKET_SIZE_MAX,\
                    BOOST_PACKET_TEMPLATE_PARAMETER, \
                    typename T )


#endif
