
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_STATIC_POOL_H
#define BOOST_TASK_STATIC_POOL_H

#include <cstddef>
#include <utility>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/context.hpp>
#include <boost/task/detail/atomic.hpp>
#include <boost/task/detail/bind_processor.hpp>
#include <boost/task/detail/worker.hpp>
#include <boost/task/detail/worker_group.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/poolsize.hpp>
#include <boost/task/scanns.hpp>
#include <boost/task/task.hpp>
#include <boost/task/watermark.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
template< typename Channel >
class static_pool
{
public:
	typedef Channel		channel;

private:
	template< typename Pool >
	friend struct has_attribute;

	template< typename Pool >
	friend struct attribute_type;

	friend class detail::worker;

	template< typename Channel >
	friend class detail::worker::worker_object< static_pool< Channel > >;

	typedef typename channel::item	channel_item;
	
# if defined(BOOST_HAS_PROCESSOR_BINDINGS)
	struct tag_bind_to_processors {};
# endif
	
	class pool_base
	{
	private:
		friend class detail::worker;

		template< typename Channel >
		friend class detail::worker::worker_object< static_pool< Channel > >;

		detail::worker_group	wg_;
		shared_mutex			mtx_wg_;
		volatile uint32_t		state_;
		channel			 		channel_;
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
			std::size_t n)
		{
			wg_.insert(
				detail::worker(
					* this,
					psize,
					asleep,
					max_scns,
					boost::bind(
						& pool_base::worker_entry_,
						this,
						n) ) );
		}
# endif

		std::size_t active_() const
		{ return active_worker_; }

		std::size_t idle_() const
		{ return size_() - active_(); }

		std::size_t size_() const
		{ return wg_.size(); }

		bool closed_() const
		{ return state_ > 0; }

		bool close_()
		{ return detail::atomic_fetch_add( & state_, 1) > 1; }

	public:
		explicit pool_base(
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
			lock_guard< shared_mutex > lk( mtx_wg_);
			for ( std::size_t i( 0); i < psize; ++i)
				create_worker_( psize, asleep, max_scns);
		}

		explicit pool_base(
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
			lock_guard< shared_mutex > lk( mtx_wg_);
			for ( std::size_t i( 0); i < psize; ++i)
				create_worker_( psize, asleep, max_scns);
		}

# if defined(BOOST_HAS_PROCESSOR_BINDINGS)
		explicit pool_base(
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
			lock_guard< shared_mutex > lk( mtx_wg_);
			for ( std::size_t i( 0); i < psize; ++i)
				create_worker_( psize, asleep, max_scns, i);
		}

		explicit pool_base(
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
			lock_guard< shared_mutex > lk( mtx_wg_);
			for ( std::size_t i( 0); i < psize; ++i)
				create_worker_( psize, asleep, max_scns, i);
		}
# endif

		~pool_base()
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

		void interrupt_all_worker()
		{
			if ( closed_() ) return;

			shared_lock< shared_mutex > lk( mtx_wg_);
			wg_.interrupt_all();
		}

		void shutdown()
		{
			if ( closed_() || close_() ) return;

			channel_.deactivate();
			shared_lock< shared_mutex > lk( mtx_wg_);
			wg_.signal_shutdown_all();
			wg_.join_all();
		}

		const void shutdown_now()
		{
			if ( closed_() || close_() ) return;

			channel_.deactivate_now();
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

		void clear()
		{ channel_.clear(); }

		bool empty()
		{ return channel_.empty(); }

		std::size_t pending()
		{ return channel_.size(); }

		std::size_t upper_bound()
		{ return channel_.upper_bound(); }

		void upper_bound( high_watermark const& hwm)
		{ channel_.upper_bound( hwm); }

		std::size_t lower_bound()
		{ return channel_.lower_bound(); }

		void lower_bound( low_watermark const lwm)
		{ channel_.lower_bound( lwm); }

		template< typename R >
		handle< R > submit( task< R > t)
		{
			if ( closed_() )
				throw task_rejected("pool is closed");

			shared_future< R > f( t.get_future() );
			context ctx;
			handle< R > h( ctx.get_handle( f) );
			channel_.put(
					ctx.get_callable( boost::move( t) ) );
			return h;
		}

		template< typename R, typename Attr >
		handle< R > submit( task< R > t, Attr const& attr)
		{
			if ( closed_() )
				throw task_rejected("pool is closed");

			shared_future< R > f( t.get_future() );
			context ctx;
			handle< R > h( ctx.get_handle( f) );
			channel_.put(
					channel_item(
						ctx.get_callable( boost::move( t) ),
						attr) );
			return h;
		}
	};
	
	shared_ptr< pool_base >		pool_;

	static_pool( static_pool &);
	static_pool & operator=( static_pool &);

public:
	static_pool()
	: pool_()
	{}
	
	explicit static_pool(
		poolsize const& psize,
		posix_time::time_duration const& asleep = posix_time::microseconds( 10),
		scanns const& max_scns = scanns( 20) )
	: pool_( new pool_base( psize, asleep, max_scns) )
	{}

	explicit static_pool(
		poolsize const& psize,
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep = posix_time::microseconds( 100),
		scanns const& max_scns = scanns( 20) )
	: pool_( new pool_base( psize, hwm, lwm, asleep, max_scns) )
	{}

# if defined(BOOST_HAS_PROCESSOR_BINDINGS)
	explicit static_pool(
		tag_bind_to_processors,
		posix_time::time_duration const& asleep = posix_time::microseconds( 10),
		scanns const& max_scns = scanns( 20) )
	: pool_( new pool_base( asleep, max_scns) )
	{}

	explicit static_pool(
		tag_bind_to_processors,
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep = posix_time::microseconds( 100),
		scanns const& max_scns = scanns( 20) )
	: pool_( new pool_base( hwm, lwm, asleep, max_scns) )
	{}

	static tag_bind_to_processors bind_to_processors()
	{ return tag_bind_to_processors(); }
# endif

# if defined(BOOST_HAS_RVALUE_REFS)
	static_pool( static_pool && other)
	: pool_()
	{ pool_.swap( other.pool_); }

	static_pool & operator=( static_pool && other)
	{
	    static_pool tmp( static_cast< static_pool && >( other) );
	    swap( tmp);
	    return * this;
	}

	static_pool && move()
	{ return static_cast< static_pool && >( * this); }
# else
	static_pool( boost::detail::thread_move_t< static_pool > other)
	: pool_()
	{ pool_.swap( other->pool_); }

	static_pool & operator=( boost::detail::thread_move_t< static_pool > other)
	{
	    static_pool tmp( other);
	    swap( tmp);
	    return * this;
	}

	operator boost::detail::thread_move_t< static_pool >()
	{ return move(); }

	boost::detail::thread_move_t< static_pool > move()
	{
		boost::detail::thread_move_t< static_pool > t( * this);
		return t;
	}
# endif

	std::size_t active()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->active();
	}

	std::size_t idle()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->idle();
	}

	void interrupt_all_worker()
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->interrupt_all_worker();
	}

	void shutdown()
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->shutdown();
	}

	const void shutdown_now()
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->shutdown_now();
	}

	std::size_t size()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->size();
	}

	bool closed()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->closed();
	}

	void clear()
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->clear();
	}

	bool empty()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->empty();
	}

	std::size_t pending()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->pending();
	}

	std::size_t upper_bound()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->upper_bound();
	}

	void upper_bound( high_watermark const& hwm)
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->upper_bound( hwm);
	}

	std::size_t lower_bound()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->lower_bound();
	}

	void lower_bound( low_watermark const lwm)
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->lower_bound( lwm);
	}

	template< typename R >
	handle< R > submit( task< R > t)
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->submit( boost::move( t) );
	}

	template< typename R, typename Attr >
	handle< R > submit( task< R > t, Attr const& attr)
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->submit( boost::move( t), attr);
	}

	typedef typename shared_ptr< pool_base >::unspecified_bool_type	unspecified_bool_type;

	operator unspecified_bool_type() const // throw()
	{ return pool_; }

	bool operator!() const // throw()
	{ return ! pool_; }

	void swap( static_pool & other) // throw()
	{ pool_.swap( other.pool_); }
};
}

template< typename Channel >
void swap( task::static_pool< Channel > & l, task::static_pool< Channel > & r)
{ return l.swap( r); }

# if defined(BOOST_HAS_RVALUE_REFS)
template< typename Channel >
task::static_pool< Channel > && move( task::static_pool< Channel > && t)
{ return t; }
# else
template< typename Channel >
task::static_pool< Channel >  move( boost::detail::thread_move_t< task::static_pool< Channel > > t)
{ return task::static_pool< Channel >( t); }
# endif
}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_STATIC_POOL_H

