//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_TASK_H
#define BOOST_TP_TASK_H

#include <boost/future.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/tp/detail/interrupter.hpp>

namespace boost { namespace tp
{
template< typename R >
class task
{
private:
	shared_future< R >	fut_;
	detail::interrupter			intr_;

public:
	task()
	: fut_(), intr_()
	{}

	task(
		shared_future< R > const& fut,
		detail::interrupter const& intr)
	:
	fut_( fut),
	intr_( intr)
	{}

	void interrupt()
	{ intr_.interrupt(); }

	void interrupt_and_wait()
	{ intr_.interrupt_and_wait(); }

	void interrupt_and_wait( system_time const& abs_time)
	{ intr_.interrupt_and_wait( abs_time); }

	template< typename Duration >
	void interrupt_and_wait( Duration const& rel_time)
	{ intr_.interrupt_and_wait( rel_time); }

	bool interruption_requested()
	{ return intr_.interruption_requested(); }

	shared_future< R > result()
	{ return fut_; }
};

template<>
class task< void >
{
private:
	shared_future< void >	fut_;
	detail::interrupter				intr_;

public:
	task()
	: fut_(), intr_()
	{}
	task(
		shared_future< void > const& fut,
		detail::interrupter const& intr)
	:
	fut_( fut),
	intr_( intr)
	{}

	void interrupt()
	{ intr_.interrupt(); }

	void interrupt_and_wait()
	{ intr_.interrupt_and_wait(); }

	void interrupt_and_wait( system_time const& abs_time)
	{ intr_.interrupt_and_wait( abs_time); }

	template< typename Duration >
	void interrupt_and_wait( Duration const& rel_time)
	{ intr_.interrupt_and_wait( rel_time); }

	bool interruption_requested()
	{ return intr_.interruption_requested(); }

	shared_future< void > result()
	{ return fut_; }
};

} }

#endif // BOOST_TP_TASK_H
