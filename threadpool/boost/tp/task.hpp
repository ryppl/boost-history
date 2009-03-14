//  Copright (c) 2008 Oliver Kowalke. Distributed under the Boost
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
	jss::shared_future< R >	fut_;
	detail::interrupter		intr_;

public:
	task()
	: fut_(), intr_()
	{}

	task(
		jss::shared_future< R > const& fut,
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

	R get()
	{ return fut_.get(); }

	bool is_ready() const
	{ return fut_.is_ready(); }

	bool has_value() const
	{ return fut_.has_value(); }

	bool has_exception() const
	{ return fut_.has_exception(); }

	void wait() const
	{ fut_.wait(); }

    template< typename Duration >
    bool timed_wait( Duration const& rel_time) const
	{ return fut_.timed_wait( rel_time); }

    bool timed_wait_until( system_time const& abs_time) const
	{ return fut_.timed_wait_until( abs_time); }
};

template<>
class task< void >
{
private:
	jss::shared_future< void >	fut_;
	detail::interrupter			intr_;

public:
	task()
	: fut_(), intr_()
	{}

	task(
		jss::shared_future< void > const& fut,
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

	void get()
	{ fut_.get(); }

	bool is_ready() const
	{ return fut_.is_ready(); }

	bool has_value() const
	{ return fut_.has_value(); }

	bool has_exception() const
	{ return fut_.has_exception(); }

	void wait() const
	{ fut_.wait(); }

    template< typename Duration >
    bool timed_wait( Duration const& rel_time) const
	{ return fut_.timed_wait( rel_time); }

    bool timed_wait_until( system_time const& abs_time) const
	{ return fut_.timed_wait_until( abs_time); }
};
} }

#endif // BOOST_TP_TASK_H
