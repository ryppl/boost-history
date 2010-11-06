/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT_HPP

// Forces rescan
#define BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT( left_, ... )                  \
BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT_IMPL( left_, __VA_ARGS__ )

#define BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT_IMPL( left_, ... )             \
left_ ## __VA_ARGS__

#endif // BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT_HPP
