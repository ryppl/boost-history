//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_GUARD_H
#define BOOST_TP_DETAIL_GUARD_H

#include <boost/assert.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/utility.hpp>

namespace boost { namespace tp {
namespace detail
{
class guard
: private noncopyable
{
private:
	volatile uint32_t	&	active_worker_;

public:
	guard( volatile uint32_t & active_worker)
	: active_worker_( active_worker)
	{
		BOOST_ASSERT( active_worker_ >= 0);
		interprocess::detail::atomic_inc32( & active_worker_);
	}

	~guard()
	{
		interprocess::detail::atomic_dec32( & active_worker_);
		BOOST_ASSERT( active_worker_ >= 0);
	}
};
} } }

#endif // BOOST_TP_DETAIL_GUARD_H

