/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_ARGS_TO_SEQUENCE_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_ARGS_TO_SEQUENCE_HPP

#include <boost/auto_function/detail/num_args.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>

#define BOOST_AUTO_FUNCTION_DETAIL_ARGS_TO_SEQUENCE( ... )                     \
BOOST_PP_TUPLE_TO_SEQ( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( __VA_ARGS__ )      \
                     , ( __VA_ARGS__ )                                         \
                     )

#endif // BOOST_AUTO_FUNCTION_DETAIL_ARGS_TO_SEQUENCE_HPP
