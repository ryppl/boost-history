//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_POOL_H
#define BOOST_TP_POOL_H

#include <cstddef>
#include <list>
#include <utility>
#include <vector>

#include <boost/future.hpp>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/tp/detail/atomic.hpp>
#include <boost/tp/detail/callable.hpp>
#include <boost/tp/detail/interrupter.hpp>
#ifdef BOOST_TP_BIND_WORKER_TO_PROCESSOR
#include <boost/tp/detail/bind_processor.hpp>
#endif
#include <boost/tp/detail/worker.hpp>
#include <boost/tp/detail/worker_group.hpp>
#include <boost/tp/exceptions.hpp>
#include <boost/tp/poolsize.hpp>
#include <boost/tp/scanns.hpp>
#include <boost/tp/task.hpp>
#include <boost/tp/watermark.hpp>

namespace boost { namespace tp
{
template< typename Channel >
class pool : private noncopyable
{
private:
	template< typename Pred >
	friend void this_task::reschedule_until( Pred const&);

	template< typename Pool >
	friend Pool & this_task::get_thread_pool();

	friend class detail::worker;

	typedef Channel						channel;
	typedef typename channel::item		channel_item;


	detail::worker_group						wg_;
	shared_mutex								mtx_wg_;
	unsigned int								state_;
	channel		 								channel_;
	unsigned int								active_worker_;
	unsigned int								idle_worker_;

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

#ifdef BOOST_TP_BIND_WORKER_TO_PROCESSOR
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
	{ return detail::atomic_inc_32( & state_); }

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
			throw invalid_timeduration("argument asleep is not valid");
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
			throw invalid_timeduration("argument asleep is not valid");
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns);
		lk.unlock();
	}

#ifdef BOOST_TP_BIND_WORKER_TO_PROCESSOR
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
			throw invalid_timeduration("argument asleep is not valid");
		std::size_t psize( thread::hardware_concurrency() );
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
			throw invalid_timeduration("argument asleep is not valid");
		std::size_t psize( thread::hardware_concurrency() );
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

	const std::vector< detail::callable > shutdown_now()
	{
		if ( closed_() || close_() > 1)
			return std::vector< detail::callable >();

		channel_.deactivate_now();
		shared_lock< shared_mutex > lk( mtx_wg_);
		wg_.signal_shutdown_now_all();
		wg_.interrupt_all();
		wg_.join_all();
		lk.unlock();
		std::vector< detail::callable > drain( channel_.drain() );

		return drain;
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

	template< typename Act >
	task< typename result_of< Act() >::type > submit( Act const& act)
	{
		typedef typename result_of< Act() >::type R;
		detail::interrupter intr;
		packaged_task< R > tsk( act);
		shared_future< R > f( tsk.get_future() );
		detail::worker * w( detail::worker::tss_get() );
		if ( w)
		{
			function< bool() > wcb(
				bind(
					& shared_future< R >::is_ready,
					f) );
			tsk.set_wait_callback(
				bind(
					( void ( detail::worker::*)( function< bool() > const&) ) & detail::worker::reschedule_until,
					w,
					wcb) );
			w->put( detail::callable( move( tsk) ), intr);
			return task< R >( f, intr);
		}
		else
		{
			if ( closed_() )
				throw task_rejected("pool is closed");

			channel_item itm( detail::callable( move( tsk) ), intr);
			channel_.put( itm);
			return task< R >( f, intr);
		}
	}

	template<
		typename Act,
		typename Attr
	>
	task< typename result_of< Act() >::type > submit(
		Act const& act,
		Attr const& attr)
	{
		typedef typename result_of< Act() >::type R;
		detail::interrupter intr;
		packaged_task< R > tsk( act);
		shared_future< R > f( tsk.get_future() );
		detail::worker * w( detail::worker::tss_get() );
		if ( w)
		{
			function< bool() > wcb(
				bind(
					& shared_future< R >::is_ready,
					f) );
			tsk.set_wait_callback(
				bind(
					( void ( detail::worker::*)( function< bool() > const&) ) & detail::worker::reschedule_until,
					w,
					wcb) );
			w->put( detail::callable( move( tsk) ), intr);
			return task< R >( f, intr);
		}
		else
		{
			if ( closed_() )
				throw task_rejected("pool is closed");

			channel_item itm( detail::callable( move( tsk) ), attr, intr);
			channel_.put( itm);
			return task< R >( f, intr);
		}
	}
};
}}

#endif // BOOST_TP_POOL_H

