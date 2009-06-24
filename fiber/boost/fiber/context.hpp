//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_CONTEXT_H
#define BOOST_FIBERS_CONTEXT_H

#include <boost/fibers/detail/context.hpp>
#include <boost/fibers/detail/fiber_base.hpp>

namespace boost {
namespace fibers
{
typedef detail::context< detail::fiber_base >	context;
} }

#endif // BOOST_FIBERS_CONTEXT_H

