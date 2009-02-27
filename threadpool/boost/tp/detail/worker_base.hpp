//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_WORKER_BASE_H
#define BOOST_TP_DETAIL_WORKER_BASE_H

#include <cstddef>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/tp/detail/callable.hpp>
#include <boost/tp/detail/interrupter.hpp>

namespace boost { namespace tp {
namespace detail
{

struct worker_base 
{
	virtual ~worker_base() {}
		
	virtual const shared_ptr< thread > thrd() const = 0;

	virtual const thread::id get_id() const = 0;

	virtual void join() const = 0;

	virtual void interrupt() const = 0;

	virtual void put(
		callable const& ca,
		interrupter const& intr) = 0;

	virtual bool try_take(
		callable & ca,
		interrupter & intr) = 0;

	virtual bool try_steal(
		callable & ca,
		interrupter & intr) = 0;

	virtual bool empty() const = 0;

	virtual void signal_shutdown() = 0;

	virtual void signal_shutdown_now() = 0;

	virtual bool shutdown() = 0;

	virtual bool shutdown_now() = 0;

	virtual std::size_t scanns() const = 0;

	virtual void increment_scanns() = 0;

	virtual void reset_scanns() = 0;
	
	virtual void reschedule_until( function< bool() > const&) = 0;
};

}}}

#endif // BOOST_TP_DETAIL_WORKER_BASE_H

