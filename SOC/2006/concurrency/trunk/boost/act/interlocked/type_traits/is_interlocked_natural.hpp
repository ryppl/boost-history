/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_INTERLOCKED_NATURAL_HPP
#define BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_INTERLOCKED_NATURAL_HPP

#include <boost/act/interlocked/type_traits/is_interlocked_arithmetic.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type >
struct is_interlocked_natural : is_interlocked_arithmetic< Type > {};

} } }

#endif
