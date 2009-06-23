// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_MONOTONIC_HPP
#define BOOST_MONOTONIC_MONOTONIC_HPP

#include <boost/monotonic/config.hpp>
#include <boost/monotonic/forward_declarations.hpp>
#include <boost/monotonic/allocator.hpp>

#ifdef BOOST_MONOTONIC_THREADS
#	include <boost/monotonic/shared_allocator.hpp>
//TODO #	include <boost/monotonic/thread_local_allocator.hpp>
#endif

#include <boost/monotonic/container/string.hpp>
#include <boost/monotonic/container/vector.hpp>
#include <boost/monotonic/container/list.hpp>
#include <boost/monotonic/container/set.hpp>
#include <boost/monotonic/container/map.hpp>

namespace boost
{
	namespace monotonic
	{

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_MONOTONIC_HPP

//EOF
