//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_ADJUSTMENT_H
#define BOOST_TP_ADJUSTMENT_H

#include <cstddef>

#include <boost/tp/poolsize.hpp>

namespace boost { namespace tp
{
struct depend_on_core
{
	bool operator ()(
		poolsize size,
		core_poolsize const& core_size,
		max_poolsize const& max_size,
		std::size_t,
		bool full) const
	{ return size < core_size || full; }
};

struct depend_on_nothing
{
	bool operator ()(
		poolsize,
		core_poolsize const&,
		max_poolsize const&,
		std::size_t,
		bool) const
	{ return true; }
};

struct depend_on_pending
{
	bool operator ()(
		poolsize size,
		core_poolsize const& core_size,
		max_poolsize const& max_size,
		std::size_t pending,
		bool full) const
	{ return size < core_size || pending > 0; }
};
} }

#endif // BOOST_TP_ADJUSTMENT_H
