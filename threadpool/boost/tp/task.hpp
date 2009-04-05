//  Copright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_TASK_H
#define BOOST_TP_TASK_H

#include <boost/assert.hpp>
#include <boost/future.hpp>
#include <boost/next_prior.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/detail/worker.hpp>

namespace boost {
namespace tp
{
template< typename R >
class task
{
private:
	template< typename Iterator >
	friend void waitfor_all( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend void waitfor_all( T1 & t1, T2 & t2);
	template< typename T1, typename T2, typename T3 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);
	template< typename Iterator >
	friend Iterator waitfor_any( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2);
	template< typename T1, typename T2, typename T3 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);

	shared_future< R >	fut_;
	detail::interrupter	intr_;

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

	void swap( task< R > & other)
	{
		fut_.swap( other.fut_);
		intr_.swap( other.intr_);
	}
};

template<>
class task< void >
{
private:
	template< typename Iterator >
	friend void waitfor_all( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend void waitfor_all( T1 & t1, T2 & t2);
	template< typename T1, typename T2, typename T3 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);
	template< typename Iterator >
	friend Iterator waitfor_any( Iterator begin, Iterator end);
	template< typename T1, typename T2 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2);
	template< typename T1, typename T2, typename T3 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5);

	shared_future< void >	fut_;
	detail::interrupter		intr_;

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

	void swap( task< void > & other)
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
void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_); }

template< typename T1, typename T2, typename T3, typename T4 >
void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4)
{ wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void waitfor_all( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5)
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
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2)
{ return wait_for_any( t1.fut_, t2.fut_); }

template< typename T1, typename T2, typename T3 >
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_); }

template< typename T1, typename T2, typename T3, typename T4 >
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
unsigned int waitfor_any( task< T1 > & t1, task< T2 > & t2, task< T3 > & t3, task< T4 > & t4, task< T5 > & t5)
{ return wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }
}

namespace this_task
{
template< typename Pred >
void reschedule_until( Pred const& pred)
{
	tp::detail::worker * w( tp::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	w->reschedule_until( pred);
}

template< typename Pool >
Pool & get_thread_pool()
{
	tp::detail::worker * w( tp::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	return w->get_thread_pool< Pool >();
}

inline
bool is_worker()
{ return tp::detail::worker::tss_get() != 0; }

inline
thread::id worker_id()
{
	tp::detail::worker * w( tp::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	return w->get_id();
}

template< typename Pool >
void sleep_until( system_time & abs_time)
{
	struct time_reached
	{
		system_time	abs_time;
	
		time_reached( system_time & abs_time_)
		: abs_time( abs_time_)
		{}
	
		bool operator()()
		{ return get_system_time() >= abs_time; }
	};
	
	if ( is_worker() )
	{
		time_reached t( abs_time);
		get_thread_pool< Pool >()->reschedule_until( t);
	}
	else
		this_thread::sleep( abs_time);
}

template< typename Pool >
void yield()
{
	struct always_true
	{
		always_true() {}
		
		bool operator()()
		{ return true; }
	};
	
	if ( is_worker() )
	{
		always_true t;
		get_thread_pool< Pool >()->reschedule_until( t);
	}
	else
		this_thread::yield();
}
} }

#endif // BOOST_TP_TASK_H
