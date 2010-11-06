/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_TAIL_ARGUMENTS_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_TAIL_ARGUMENTS_HPP

#define BOOST_AUTO_FUNCTION_DETAIL_TAIL_ARGUMENTS( head, ... ) __VA_ARGS__

#define BOOST_AUTO_FUNCTION_DETAIL_TAIL_ARGUMENTS_AFTER_RESCAN( ... )          \
BOOST_AUTO_FUNCTION_DETAIL_TAIL_ARGUMENTS( __VA_ARGS__ )

#endif // BOOST_AUTO_FUNCTION_DETAIL_TAIL_ARGUMENTS_HPP
