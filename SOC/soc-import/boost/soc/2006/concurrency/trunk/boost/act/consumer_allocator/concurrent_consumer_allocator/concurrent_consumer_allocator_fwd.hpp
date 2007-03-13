/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_FWD_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_FWD_HPP

#include <cstddef>

#include <boost/act/worker_allocator/thread_allocator/basic_thread_allocator/basic_thread_allocator_fwd.hpp>

namespace boost
{
namespace act
{

// ToDo: Template for thread allocator

// ToDo: Make defaults toggleable
template< typename ThreadAllocator = basic_thread_allocator
        , ::std::size_t BufferSize = 6
        >
class concurrent_consumer_allocator;

}
}

#include <boost/act/worker_allocator/thread_allocator/basic_thread_allocator.hpp>

#endif
