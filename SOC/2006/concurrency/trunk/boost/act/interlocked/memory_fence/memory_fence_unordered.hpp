/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_MEMORY_FENCE_MEMORY_FENCE_UNORDERED_HPP
#define BOOST_ACT_INTERLOCKED_MEMORY_FENCE_MEMORY_FENCE_UNORDERED_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/act/interlocked/semantics/unordered.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Semantics >
typename enable_if< is_same< Semantics, unordered > >::type memory_fence()
{
}

} } }

#endif
