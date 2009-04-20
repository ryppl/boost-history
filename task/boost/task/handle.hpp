
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

namespace boost { namespace task
{
template< typename T >
class task;

template< typename R >
class handle
{
private:
	template< typename T >
	friend class task;
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

	handle( shared_future< R > const& fut, detail::interrupter const& intr)
	: fut_( fut), intr_( intr)
	{}

public:
	handle()
	: fut_(), intr_()
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
	{
		try
		{ return fut_.get(); }
		catch ( broken_promise const&)
		{ throw broken_task(); }
	}

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

	void swap( handle< R > & other)
	{
		fut_.swap( other.fut_);
		intr_.swap( other.intr_);
	}
};

template<>
class handle< void >
{
private:
	template< typename Channel >
	friend class task;
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

	shared_future< void >	fut_;
	detail::interrupter		intr_;

	handle( shared_future< void > const& fut, detail::interrupter const& intr)
	: fut_( fut), intr_( intr)
	{}

public:
	handle()
	: fut_(), intr_()
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
	{
		try
		{ fut_.get(); }
		catch ( broken_promise const&)
		{ throw broken_task(); }
	}

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

	void swap( handle< void > & other)
	{
		fut_.swap( other.fut_);
		intr_.swap( other.intr_);
	}
};

template< typename Iterator >
void waitfor_all( Iterator begin, Iterator end)
{
	for ( Iterator i = begin; i != end; ++i)
		i->wait();
}

template< typename T1, typename T2 >
void waitfor_all( T1 & t1, T2 & t2)
{ wait_for_all( t1.fut_, t2.fut_); }

template< typename T1, typename T2, typename T3 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_); }

template< typename T1, typename T2, typename T3, typename T4 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }

template< typename Iterator >
Iterator waitfor_any( Iterator begin, Iterator end)
{
	boost::detail::future_waiter waiter;
	for ( Iterator i = begin; i != end; ++i)
		waiter.add( i->fut_);
	return next( begin, waiter.wait() );
}

template< typename T1, typename T2 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2)
{ return wait_for_any( t1.fut_, t2.fut_); }

template< typename T1, typename T2, typename T3 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_); }

template< typename T1, typename T2, typename T3, typename T4 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }

}}

#endif // BOOST_TASK_HANDLE_H
