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
	template<
		typename T1,
		typename T2
	>
	friend void wait_for_any( T1 &, T2 &);
	template<
		typename T1,
		typename T2,
		typename T3
	>
	friend void wait_for_any( T1 &, T2 &, T3 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4
	>
	friend void wait_for_any( T1 &, T2 &, T3 &, T4 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4,
		typename T5
	>
	friend void wait_for_any( T1 &, T2 &, T3 &, T4 &, T5 &);
	template<
		typename T1,
		typename T2
	>
	friend void wait_for_all( T1 &, T2 &);
	template<
		typename T1,
		typename T2,
		typename T3
	>
	friend void wait_for_all( T1 &, T2 &, T3 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4
	>
	friend void wait_for_all( T1 &, T2 &, T3 &, T4 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4,
		typename T5
	>
	friend void wait_for_all( T1 &, T2 &, T3 &, T4 &, T5 &);
	
	jss::shared_future< R >	fut_;
	detail::interrupter		intr_;

public:
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

	bool interrupt_requested()
	{ return intr_.interrupt_requested(); }

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
	template<
		typename T1,
		typename T2
	>
	friend void wait_for_any( T1 &, T2 &);
	template<
		typename T1,
		typename T2,
		typename T3
	>
	friend void wait_for_any( T1 &, T2 &, T3 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4
	>
	friend void wait_for_any( T1 &, T2 &, T3 &, T4 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4,
		typename T5
	>
	friend void wait_for_any( T1 &, T2 &, T3 &, T4 &, T5 &);
	template<
		typename T1,
		typename T2
	>
	friend void wait_for_all( T1 &, T2 &);
	template<
		typename T1,
		typename T2,
		typename T3
	>
	friend void wait_for_all( T1 &, T2 &, T3 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4
	>
	friend void wait_for_all( T1 &, T2 &, T3 &, T4 &);
	template<
		typename T1,
		typename T2,
		typename T3,
		typename T4,
		typename T5
	>
	friend void wait_for_all( T1 &, T2 &, T3 &, T4 &, T5 &);
	
	jss::shared_future< void >	fut_;
	detail::interrupter			intr_;

public:
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

	bool interrupt_requested()
	{ return intr_.interrupt_requested(); }

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

template<
	typename T1,
	typename T2
>
void wait_for_all( T1 & t1, T2 & t2)
{ jss::wait_for_all( t1.fut_, t2.fut_); };

template<
	typename T1,
	typename T2,
	typename T3
>
void wait_for_all( T1 & t1, T2 & t2, T3 & t3)
{ jss::wait_for_all( t1.fut_, t2.fut_, t3.fut_); };

template<
	typename T1,
	typename T2,
	typename T3,
	typename T4
>
void wait_for_all( T1 & t1, T2 & t2, T3 & t3, T4 & t4)
{ jss::wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_); };

template<
	typename T1,
	typename T2,
	typename T3,
	typename T4,
	typename T5
>
void wait_for_all( T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5)
{ jss::wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); };

// template<
// 	typename T1,
// 	typename T2
// >
// void wait_for_any( T1 & t1, T2 & t2)
// { jss::wait_for_any( t1.fut_, t2.fut_); };
// 
// template<
// 	typename T1,
// 	typename T2,
// 	typename T3
// >
// void wait_for_any( T1 & t1, T2 & t2, T3 & t3)
// { jss::wait_for_any( t1.fut_, t2.fut_, t3.fut_); };
// 
// template<
// 	typename T1,
// 	typename T2,
// 	typename T3,
// 	typename T4
// >
// void wait_for_any( T1 & t1, T2 & t2, T3 & t3, T4 & t4)
// { jss::wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_); };
// 
// template<
// 	typename T1,
// 	typename T2,
// 	typename T3,
// 	typename T4,
// 	typename T5
// >
// void wait_for_any( T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5)
// { jss::wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); };
} }

#endif // BOOST_TP_TASK_H
