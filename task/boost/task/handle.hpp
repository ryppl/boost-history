
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_HANDLE_H
#define BOOST_TASK_HANDLE_H

#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/task/detail/interrupter.hpp>
#include <boost/task/future.hpp>
#include <boost/task/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{

template< typename Channel >
class static_pool;

struct own_thread;
struct new_thread;
struct as_sub_task;

template< typename R >
class handle
{
private:
	template< typename Channel >
	friend class static_pool;
	friend struct own_thread;
	friend struct new_thread;
	friend struct as_sub_task;
	template< typename Iterator >
	friend void waitfor_all( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend void waitfor_all( T1 & t1, T2 & t2);
	template< typename T1, typename T2, typename T3 >
	friend void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5);
	template< typename Iterator >
	friend Iterator waitfor_any( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2);
	template< typename T1, typename T2, typename T3 >
	friend unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5);

	shared_future< R >		fut_;
	detail::interrupter		intr_;

	handle( shared_future< R > fut)
	:
	fut_( fut),
	intr_()
	{}

	handle(
		shared_future< R > fut,
		detail::interrupter const& intr)
	:
	fut_( fut),
	intr_( intr)
	{}

public:
	handle()
	: fut_(), intr_()
	{}

	void interrupt()
	{ intr_.interrupt(); }

	void interrupt_and_wait()
	{
		interrupt();
		wait();
	}

	bool interrupt_and_wait_until( system_time const& abs_time)
	{
		interrupt();
		return wait_until( abs_time);
	}

	template< typename Duration >
	bool interrupt_and_wait_for( Duration const& rel_time)
	{
		interrupt();
		return wait_for( rel_time);
	}

	bool interruption_requested()
	{ return intr_.interruption_requested(); }

	R get()
	{
		try
		{ return fut_.get(); }
		catch ( future_uninitialized const&)
		{ throw task_uninitialized(); }
		catch ( broken_promise const&)
		{ throw broken_task(); }
		catch ( promise_already_satisfied const&)
		{ throw task_already_executed(); }
	}

	bool is_ready() const
	{ return fut_.is_ready(); }

	bool has_value() const
	{ return fut_.has_value(); }

	bool has_exception() const
	{ return fut_.has_exception(); }

	void wait() const
	{
		try
		{ fut_.wait(); }
		catch ( future_uninitialized const&)
		{ throw task_uninitialized(); }
		catch ( broken_promise const&)
		{ throw broken_task(); }
		catch ( thread_interrupted const&)
		{ throw task_interrupted(); }
	}

	template< typename Duration >
	bool wait_for( Duration const& rel_time) const
	{
		try
		{ return fut_.timed_wait( rel_time); }
		catch ( future_uninitialized const&)
		{ throw task_uninitialized(); }
		catch ( broken_promise const&)
		{ throw broken_task(); }
		catch ( thread_interrupted const&)
		{ throw task_interrupted(); }
	}

	bool wait_until( system_time const& abs_time) const
	{
		try
		{ return fut_.timed_wait_until( abs_time); }
		catch ( future_uninitialized const&)
		{ throw task_uninitialized(); }
		catch ( broken_promise const&)
		{ throw broken_task(); }
		catch ( thread_interrupted const&)
		{ throw task_interrupted(); }
	}

	shared_future< R > & get_future()
	{ return fut_; }

	void swap( handle< R > & other)
	{
		fut_.swap( other.fut_);
		intr_.swap( other.intr_);
	}
};

template< typename Iterator >
void waitfor_all( Iterator begin, Iterator end)
{
	try
	{
		for ( Iterator i = begin; i != end; ++i)
			i->wait();
	}
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2 >
void waitfor_all( T1 & t1, T2 & t2)
{
	try
	{ wait_for_all( t1.fut_, t2.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3)
{
	try
	{ wait_for_all( t1.fut_, t2.fut_, t3.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4)
{
	try
	{ wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5)
{
	try
	{ wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename Iterator >
Iterator waitfor_any( Iterator begin, Iterator end)
{
	try
	{
		boost::detail::future_waiter waiter;
		for ( Iterator i = begin; i != end; ++i)
			waiter.add( i->fut_);
		return next( begin, waiter.wait() );
	}
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2)
{
	try
	{ return wait_for_any( t1.fut_, t2.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3)
{
	try
	{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4)
{
	try
	{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5)
{
	try
	{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_HANDLE_H
