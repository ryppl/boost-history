
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  parts are based on boost.future by Anthony Williams

#ifndef BOOST_TASKS_HANDLE_H
#define BOOST_TASKS_HANDLE_H

#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/task/context.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/spin/future.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {

template< typename T >
struct is_handle_type;

template< typename R >
class handle
{
private:
	template< typename T1, typename T2 >
	friend void waitfor_all( handle< T1 > &, handle< T2 > &);
	template< typename T1, typename T2, typename T3 >
	friend void waitfor_all( handle< T1 > &, handle< T2 > &, handle< T3 > &);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend void waitfor_all( handle< T1 > &, handle< T2 > &, handle< T3 > &, handle< T4 > &);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend void waitfor_all( handle< T1 > &, handle< T2 > &, handle< T3 > &, handle< T4 > &, handle< T5 > &);
	template< typename Iterator >
	friend typename disable_if< is_handle_type< Iterator >, Iterator >::type waitfor_any( Iterator begin, Iterator end);
	template< typename H1, typename H2 >
	friend typename enable_if< is_handle_type< H1 >, unsigned >::type waitfor_any( H1 &, H2 &);
	template< typename T1, typename T2, typename T3 >
	friend unsigned int waitfor_any( handle< T1 > &, handle< T2 > &, handle< T3 > &);
	template< typename T1, typename T2, typename T3, typename T4 >
	friend unsigned int waitfor_any( handle< T1 > &, handle< T2 > &, handle< T3 > &, handle< T4 > &);
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	friend unsigned int waitfor_any( handle< T1 > &, handle< T2 > &, handle< T3 > &, handle< T4 > &, handle< T5 > &);

	spin::shared_future< R >	fut_;
	context						ctx_;

public:
	handle() :
		fut_(), ctx_()
	{}

	handle(
			spin::shared_future< R > const& fut,
			context const& ctx) :
		fut_( fut),
		ctx_( ctx)
	{}

	void interrupt()
	{ ctx_.interrupt(); }

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

	template< typename TimeDuration >
	bool interrupt_and_wait_for( TimeDuration const& rel_time)
	{
		interrupt();
		return wait_for( rel_time);
	}

	bool interruption_requested()
	{ return ctx_.interruption_requested(); }

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

	template< typename TimeDuration >
	bool wait_for( TimeDuration const& rel_time) const
	{ return wait_until( get_system_time() + rel_time); }

	void swap( handle< R > & other)
	{
		fut_.swap( other.fut_);
 		ctx_.swap( other.ctx_);
	}
};

template< typename T >
struct is_handle_type
{ BOOST_STATIC_CONSTANT( bool, value = false); };

template< typename T >
struct is_handle_type< handle< T > >
{
    BOOST_STATIC_CONSTANT( bool, value = true);
};

template< typename Iterator >
typename disable_if< is_handle_type< Iterator >, void >::type waitfor_all( Iterator begin, Iterator end)
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
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2)
{
	try
	{ spin::wait_for_all( t1.fut_, t2.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3)
{
	try
	{ spin::wait_for_all( t1.fut_, t2.fut_, t3.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4)
{
	try
	{ spin::wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void waitfor_all( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5)
{
	try
	{ spin::wait_for_all( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename Iterator >
typename disable_if< is_handle_type< Iterator >, Iterator >::type waitfor_any( Iterator begin, Iterator end)
{
	try
	{
		boost::detail::future_waiter waiter;
		for ( Iterator i = begin; i != end; ++i)
			waiter.add( * ( i->fut_) );
		return next( begin, waiter.wait() );
	}
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename H1, typename H2 >
typename enable_if< is_handle_type< H1 >, unsigned >::type waitfor_any( H1 & h1, H2 & h2)
{
	try
	{ return spin::wait_for_any( h1.fut_, h2.fut_ ); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3)
{
	try
	{ return spin::wait_for_any( t1.fut_, t2.fut_, t3.fut_ ); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4)
{
	try
	{ return spin::wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_ ); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
unsigned int waitfor_any( handle< T1 > & t1, handle< T2 > & t2, handle< T3 > & t3, handle< T4 > & t4, handle< T5 > & t5)
{
	try
	{ return spin::wait_for_any( t1.fut_, t2.fut_, t3.fut_, t4.fut_, t5.fut_ ); }
	catch ( thread_interrupted const&)
	{ throw task_interrupted(); }
}

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_HANDLE_H
