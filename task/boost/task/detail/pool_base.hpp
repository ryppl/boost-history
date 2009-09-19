
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_POOL_BASE_H
#define BOOST_TASK_DETAIL_POOL_BASE_H

#include <cstddef>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/detail/bind_processor.hpp>
#include <boost/task/detail/worker.hpp>
#include <boost/task/detail/worker_group.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/context.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/poolsize.hpp>
#include <boost/task/scanns.hpp>
#include <boost/task/stacksize.hpp>
#include <boost/task/task.hpp>
#include <boost/task/watermark.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task {
namespace detail
{

template< typename Queue, typename UMS >
class pool_base
{
private:
	friend class worker;

	template< typename T, typename X, typename Z >
	friend class worker_object;

	typedef Queue							queue_type;
	typedef typename queue_type::value_type	value_type;

	UMS						ums_;	
	worker_group			wg_;
	shared_mutex			mtx_wg_;
	volatile uint32_t		state_;
	queue_type			 	queue_;
	volatile uint32_t		idle_worker_;

	void worker_entry_()
	{
		shared_lock< shared_mutex > lk( mtx_wg_);
		typename detail::worker_group::iterator i( wg_.find( this_thread::get_id() ) );
		lk.unlock();
		BOOST_ASSERT( i != wg_.end() );

		worker w( * i);
		w.run();
	}

	void create_worker_(
		poolsize const& psize,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size)
	{
		wg_.insert(
			worker(
				* this,
				ums_,
				psize,
				asleep,
				max_scns,
				stack_size,
				boost::bind(
					& pool_base::worker_entry_,
					this) ) );
	}

# if defined(BOOST_HAS_PROCESSOR_BINDINGS)
	void worker_entry_( std::size_t n)
	{
		this_thread::bind_to_processor( n);
		worker_entry_();
	}

	void create_worker_(
		poolsize const& psize,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size,
		std::size_t n)
	{
		wg_.insert(
			worker(
				* this,
				ums_,
				psize,
				asleep,
				max_scns,
				stack_size,
				boost::bind(
					& pool_base::worker_entry_,
					this,
					n) ) );
	}
# endif

	std::size_t size_() const
	{ return wg_.size(); }

	bool closed_() const
	{ return state_ > 0; }

	bool close_()
	{ return atomic_fetch_add( & state_, 1) > 0; }

public:
	explicit pool_base(
		poolsize const& psize,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size)
	:
	ums_(),
	wg_(),
	mtx_wg_(),
	state_( 0),
	queue_(),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		lock_guard< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns, stack_size);
	}

	explicit pool_base(
		poolsize const& psize,
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size)
	:
	wg_(),
	mtx_wg_(),
	state_( 0),
	queue_(
		hwm,
		lwm),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		lock_guard< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns, stack_size);
	}

# if defined(BOOST_HAS_PROCESSOR_BINDINGS)
	explicit pool_base(
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size)
	:
	wg_(),
	mtx_wg_(),
	state_( 0),
	queue_(),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		poolsize psize( thread::hardware_concurrency() );
		BOOST_ASSERT( psize > 0);
		lock_guard< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns, stack_size, i);
	}

	explicit pool_base(
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size)
	:
	wg_(),
	mtx_wg_(),
	state_( 0),
	queue_(
		hwm,
		lwm),
	idle_worker_( 0)
	{
		if ( asleep.is_special() || asleep.is_negative() )
			throw invalid_timeduration();
		poolsize psize( thread::hardware_concurrency() );
		BOOST_ASSERT( psize > 0);
		lock_guard< shared_mutex > lk( mtx_wg_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( psize, asleep, max_scns, stack_size, i);
	}
# endif

	~pool_base()
	{ shutdown(); }

	void interrupt_all_worker()
	{
		if ( closed_() ) return;

		shared_lock< shared_mutex > lk( mtx_wg_);
		wg_.interrupt_all();
	}

	void shutdown()
	{
		if ( closed_() || close_() ) return;

		queue_.deactivate();
		shared_lock< shared_mutex > lk( mtx_wg_);
		wg_.signal_shutdown_all();
		wg_.join_all();
	}

	const void shutdown_now()
	{
		if ( closed_() || close_() ) return;

		queue_.deactivate();
		shared_lock< shared_mutex > lk( mtx_wg_);
		wg_.signal_shutdown_now_all();
		wg_.interrupt_all();
		wg_.join_all();
	}

	std::size_t size()
	{
		shared_lock< shared_mutex > lk( mtx_wg_);
		return size_();
	}

	bool closed()
	{ return closed_(); }

	std::size_t upper_bound()
	{ return queue_.upper_bound(); }

	void upper_bound( high_watermark const& hwm)
	{ queue_.upper_bound( hwm); }

	std::size_t lower_bound()
	{ return queue_.lower_bound(); }

	void lower_bound( low_watermark const lwm)
	{ queue_.lower_bound( lwm); }

	template< typename R >
	handle< R > submit( task< R > t)
	{
		if ( closed_() )
			throw task_rejected("pool is closed");

		shared_future< R > f( t.get_future() );
		context ctx;
		handle< R > h( f, ctx);
		queue_.put( callable( boost::move( t), ctx) );
		return h;
	}

	template< typename R, typename Attr >
	handle< R > submit( task< R > t, Attr const& attr)
	{
		if ( closed_() )
			throw task_rejected("pool is closed");

		shared_future< R > f( t.get_future() );
		context ctx;
		handle< R > h( f, ctx);
		queue_.put(
				value_type(
					callable( boost::move( t), ctx),
					attr) );
		return h;
	}
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_POOL_BASE_H

