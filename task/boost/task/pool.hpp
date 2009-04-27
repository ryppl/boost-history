
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_POOL_H
#define BOOST_TASK_POOL_H

#include <cstddef>
#include <utility>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/detail/bind_processor.hpp>
#include <boost/task/detail/interrupter.hpp>
#include <boost/task/detail/pool_callable.hpp>
#include <boost/task/detail/worker.hpp>
#include <boost/task/detail/worker_group.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/future.hpp>
#include <boost/task/async_handle.hpp>
#include <boost/task/poolsize.hpp>
#include <boost/task/scanns.hpp>
#include <boost/task/task.hpp>
#include <boost/task/watermark.hpp>

namespace boost { namespace task
{
template< typename Channel >
class pool : private noncopyable
{
private:
	friend class detail::worker;

	typedef Channel					channel;
	typedef typename channel::item	channel_item;

	detail::worker_group	wg_;
	shared_mutex			mtx_wg_;
	volatile uint32_t		state_;
	channel		 			channel_;
	volatile uint32_t		active_worker_;
	volatile uint32_t		idle_worker_;

	void worker_entry_()
	{
		shared_lock< shared_mutex > lk( mtx_wg_);
		typename detail::worker_group::iterator i( wg_.find( this_thread::get_id() ) );
		lk.unlock();
		BOOST_ASSERT( i != wg_.end() );

		detail::worker w( * i);
		w.run();
	}
	
	void create_worker_(
		poolsize const& psize,
		posix_time::time_duration const& asleep,
		scanns const& max_scns)
	{
		wg_.insert(
			detail::worker(
				* this,
				psize,
				asleep,
				max_scns,
				boost::bind(
					& pool::worker_entry_,
					this) ) );
	}

#ifdef BOOST_HAS_PROCESSOR_BINDINGS
	void worker_entry_( std::size_t n)
	{
		this_thread::bind_to_processor( n);
		worker_entry_();
	}

	void create_worker_(
		poolsize const& psize,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		std::size_t n)
	{
		wg_.insert(
			detail::worker(
				* this,
				psize,
				asleep,
				max_scns,
				boost::bind(
					& pool::worker_entry_,
					this,
					n) ) );
	}
#endif

	std::size_t active_() const
	{ return active_worker_; }

	std::size_t idle_() const
	{ return size_() - active_(); }

	std::size_t size_() const
	{ return wg_.size(); }

	bool closed_() const
	{ return state_ > 0; }

	unsigned int close_()
	{ return detail::atomic_fetch_add( & state_, 1); }

public:
	explicit pool(
		poolsize const& psize,
		posix_time::time_duration const& asleep = posix_time::microseconds( 10),
		scanns const& max_scns = scanns( 20) )
	:
	wg_(),
	mtx_wg_(),
	state_( 0),
	channel_(),
	active_worker_( 0),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns);
		lk.unlock();
	}

	explicit pool(
		poolsize const& psize,
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep = posix_time::microseconds( 100),
		scanns const& max_scns = scanns( 20) )
	:
	wg_(),
	mtx_wg_(),
	state_( 0),
	channel_(
		hwm,
		lwm),
	active_worker_( 0),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns);
		lk.unlock();
	}

#ifdef BOOST_HAS_PROCESSOR_BINDINGS
	explicit pool(
		posix_time::time_duration const& asleep = posix_time::microseconds( 10),
		scanns const& max_scns = scanns( 20) )
	:
	wg_(),
	mtx_wg_(),
	state_( 0),
	channel_(),
	active_worker_( 0),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		poolsize psize( thread::hardware_concurrency() );
		BOOST_ASSERT( psize > 0);
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns, i);
		lk.unlock();
	}

	explicit pool(
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep = posix_time::microseconds( 100),
		scanns const& max_scns = scanns( 20) )
	:
	wg_(),
	mtx_wg_(),
	state_( 0),
	channel_(
		hwm,
		lwm),
	active_worker_( 0),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		poolsize psize( thread::hardware_concurrency() );
		BOOST_ASSERT( psize > 0);
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns, i);
		lk.unlock();
	}
#endif

	~pool()
	{ shutdown(); }

	std::size_t active()
	{
		shared_lock< shared_mutex > lk( mtx_wg_);
		return active_();
	}

	std::size_t idle()
	{
		shared_lock< shared_mutex > lk( mtx_wg_);
		return idle_();
	}
	
	void shutdown()
	{
		if ( closed_() || close_() > 1) return;

		channel_.deactivate();
		shared_lock< shared_mutex > lk( mtx_wg_);
		wg_.signal_shutdown_all();
		wg_.join_all();
		lk.unlock();
	}

	const void shutdown_now()
	{
		if ( closed_() || close_() > 1) return;

		channel_.deactivate_now();
		shared_lock< shared_mutex > lk( mtx_wg_);
		wg_.signal_shutdown_now_all();
		wg_.interrupt_all();
		wg_.join_all();
		lk.unlock();
	}

	std::size_t size()
	{
		shared_lock< shared_mutex > lk( mtx_wg_);
		return size_();
	}

	bool closed()
	{ return closed_(); }

	void clear()
	{ channel_.clear(); }

	bool empty()
	{ return channel_.empty(); }

	std::size_t pending()
	{ return channel_.size(); }

	std::size_t upper_bound()
	{ return channel_.upper_bound(); }

	void upper_bound( high_watermark const& hwm)
	{ return channel_.upper_bound( hwm); }

	std::size_t lower_bound()
	{ return channel_.lower_bound(); }

	void lower_bound( low_watermark const lwm)
	{ return channel_.lower_bound( lwm); }

	template< typename R >
	async_handle< R > submit( task< R > t)
	{
		detail::interrupter intr;
		shared_future< R > fut( t.get_future() );
		detail::worker * w( detail::worker::tss_get() );
		if ( w)
		{
			function< bool() > wcb(
				bind(
					& shared_future< R >::is_ready,
					fut) );
			t.set_wait_callback(
				bind(
					( void ( detail::worker::*)( function< bool() > const&) ) & detail::worker::reschedule_until,
					w,
					wcb) );
			w->put( detail::pool_callable( t, intr) );
			return async_handle< R >( t.get_id(), fut, intr);
		}
		else
		{
			if ( closed_() )
				throw task_rejected("pool is closed");

			channel_.put( detail::pool_callable( t, intr) );
			return async_handle< R >( t.get_id(), fut, intr);
		}
	}

	template<
		typename R,
		typename Attr
	>
	async_handle< R > submit( task< R > t, Attr const& attr)
	{
		detail::interrupter intr;
		shared_future< R > fut( t.get_future() );
		detail::worker * w( detail::worker::tss_get() );
		if ( w)
		{
			function< bool() > wcb(
				bind(
					& shared_future< R >::is_ready,
					fut) );
			t.set_wait_callback(
				bind(
					( void ( detail::worker::*)( function< bool() > const&) ) & detail::worker::reschedule_until,
					w,
					wcb) );
			w->put( detail::pool_callable( t, intr) );
			return async_handle< R >( t.get_id(), fut, intr);
		}
		else
		{
			if ( closed_() )
				throw task_rejected("pool is closed");

			channel_.put( channel_item( detail::pool_callable( t, intr), attr) );
			return async_handle< R >( t.get_id(), fut, intr);
		}
	}
};
}}

#endif // BOOST_TASK_POOL_H

