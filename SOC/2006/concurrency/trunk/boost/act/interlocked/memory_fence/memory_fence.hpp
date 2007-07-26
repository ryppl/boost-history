/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_MEMORY_FENCE_MEMORY_FENCE_HPP
#define BOOST_ACT_INTERLOCKED_MEMORY_FENCE_MEMORY_FENCE_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_INTERLOCKED_HAS( memory_fence, _ )

#include <boost/act/interlocked/semantics/default.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/act/interlocked/memory_fence/memory_fence_sequential.hpp>

namespace boost { namespace act { namespace interlocked {

void memory_fence()
{
  return interlocked::memory_fence< sequential >();
}

template< typename Semantics >
typename enable_if< is_same< Semantics, default_ > >::type memory_fence()
{
  return interlocked::memory_fence< sequential >();
}

} } }

#endif

#endif
