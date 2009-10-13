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
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/tp/detail/callable.hpp>
#include <boost/tp/detail/guard.hpp>
#include <boost/tp/detail/interrupter.hpp>
#ifdef BOOST_BIND_WORKER_TO_PROCESSORS
#include <boost/tp/detail/bind_processor.hpp>
#endif
#include <boost/tp/detail/worker.hpp>
#include <boost/tp/exceptions.hpp>
#include <boost/tp/poolsize.hpp>
#include <boost/tp/scanns.hpp>
#include <boost/tp/task.hpp>
#include <boost/tp/watermark.hpp>

namespace boost {
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
}

namespace tp
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

	enum state
	{
		active_state,
		terminateing_state,
		terminated_state
	};

	struct id_idx_tag {};
	struct rnd_idx_tag {};

	typedef multi_index::multi_index_container<
		detail::worker,
		multi_index::indexed_by<
			multi_index::ordered_unique<
				multi_index::tag< id_idx_tag >,
				multi_index::const_mem_fun<
					detail::worker,
					const thread::id,
					& detail::worker::get_id
				>
			>,
			multi_index::random_access< multi_index::tag< rnd_idx_tag > >
		>
	>										worker_list;

	typedef typename worker_list::template index<
		id_idx_tag >::type					id_idx;
	typedef typename worker_list::template index<
		rnd_idx_tag >::type					rnd_idx;

	class random_idx
	{
	private:
		rand48 rng_;
		uniform_int<> six_;
		variate_generator< rand48 &, uniform_int<> > die_;

	public:
		random_idx( std::size_t size)
		:
		rng_(),
		six_( 0, size - 1),
		die_( rng_, six_)
		{}

		std::size_t operator()()
		{ return die_(); }
	};

	static thread_specific_ptr< random_idx >	tss_rnd_idx_;

	worker_list									worker_;
	shared_mutex								mtx_worker_;
	state										state_;
	shared_mutex								mtx_state_;
	channel		 								channel_;
	posix_time::time_duration					asleep_;
	scanns										scns_;
	volatile uint32_t							active_worker_;
	volatile uint32_t							idle_worker_;
	volatile uint32_t							running_worker_;

	void execute_(
		detail::callable & ca,
		detail::interrupter & intr,
		shared_ptr< thread > const& thrd)
	{
		BOOST_ASSERT( ! ca.empty() );
		detail::guard grd( active_worker_);
		shared_ptr< void > ig(
			static_cast< void * >( 0),
			boost::bind(
				& detail::interrupter::reset,
				intr) );
		intr.set( thrd);
		ca();
		ca.clear();
		BOOST_ASSERT( ca.empty() );
	}

	void next_callable_( detail::worker & w, detail::callable & ca, detail::interrupter & intr)
	{
		rnd_idx & ridx( worker_.get< rnd_idx_tag >() );
		if ( ! w.try_take( ca, intr) )
		{
			if ( ! channel_.try_take( ca, intr) )
			{
				std::size_t idx( ( * tss_rnd_idx_)() );
				for ( std::size_t j( 0); j < worker_.size(); ++j)
				{
					detail::worker other( ridx[idx]);
					if ( this_thread::get_id() == other.get_id() ) continue;
					if ( ++idx >= worker_.size() ) idx = 0;
					if ( other.try_steal( ca, intr) ) break;
				}

				if ( ca.empty() )
				{
					detail::guard grd( idle_worker_);
					if ( shutdown_( w) ) return;
					w.increment_scanns();
					if ( w.scanns() >= scns_)
					{
						if ( size_() == idle_worker_)
							channel_.take( ca, intr, asleep_);
						else
							this_thread::sleep( asleep_);
						w.reset_scanns();
					}
					else
						this_thread::yield();
				}
			}
		}
	}

	void reschedule_until_( function< bool() > const& pred)
	{
		detail::worker * w( detail::worker::tss_get() );
		BOOST_ASSERT( w);
		BOOST_ASSERT( w->get_id() == this_thread::get_id() );
		shared_ptr< thread > thrd( w->thrd() );
		BOOST_ASSERT( thrd);
		detail::interrupter intr;
		detail::callable ca;
		while ( ! pred() )
		{
			next_callable_( * w, ca, intr);
			if( ! ca.empty() )
			{
				execute_( ca, intr, thrd);
				w->reset_scanns();
			}
		}
	}

	void entry_()
	{
		shared_lock< shared_mutex > lk( mtx_worker_);
		id_idx & iidx( worker_.get< id_idx_tag >() );
		typename id_idx::iterator i( iidx.find( this_thread::get_id() ) );
		lk.unlock();
		BOOST_ASSERT( i != iidx.end() );
		detail::worker::tss_reset( new detail::worker( * i) );
		
		detail::worker * w( detail::worker::tss_get() );
		BOOST_ASSERT( w);	
		BOOST_ASSERT( w->get_id() == this_thread::get_id() );
		shared_ptr< thread > thrd( w->thrd() );
		BOOST_ASSERT( thrd);
		detail::callable ca;
		detail::interrupter intr;

		tss_rnd_idx_.reset( new random_idx( worker_.size() ) );

		detail::guard grd( running_worker_);

		while ( ! shutdown_( * w) )
		{
			next_callable_( * w, ca, intr);
			if( ! ca.empty() )
			{
				execute_( ca, intr, thrd);
				w->reset_scanns();
			}
		}
	}

	void create_worker_()
	{
		BOOST_ASSERT( ! terminateing_() && ! terminated_() );
		worker_.insert(
			detail::worker(
				* this,
				boost::bind(
					& pool::entry_,
					this) ) );
	}

#ifdef BOOST_BIND_WORKER_TO_PROCESSORS
	void entry_( std::size_t n)
	{
		this_thread::bind_to_processor( n);
		entry_();
	}

	void create_worker_( std::size_t n)
	{
		BOOST_ASSERT( ! terminateing_() && ! terminated_() );
		worker_.insert(
			detail::worker(
				* this,
				boost::bind(
					& pool::entry_,
					this,
					n) ) );
	}
#endif

	std::size_t active_() const
	{ return active_worker_; }

	std::size_t idle_() const
	{ return size_() - active_(); }

	std::size_t size_() const
	{ return worker_.size(); }

	bool terminated_() const
	{ return state_ == terminated_state; }

	bool terminateing_() const
	{ return state_ == terminateing_state; }

	bool shutdown_( detail::worker & w)
	{
		if ( w.shutdown() && channel_.empty() )
			return true;
		else if ( w.shutdown_now() )
			return true;
		return false;
	}

public:
	explicit pool(
		poolsize const& psize,
		posix_time::time_duration const& asleep = posix_time::microseconds( 10),
		scanns const& scns = scanns( 20) )
	:
	worker_(),
	mtx_worker_(),
	state_( active_state),
	mtx_state_(),
	channel_(),
	asleep_( asleep),
	scns_( scns),
	active_worker_( 0),
	idle_worker_( 0),
	running_worker_( 0)
	{
		if ( asleep_.is_special() || asleep_.is_negative() )
			throw invalid_timeduration("argument asleep is not valid");
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_worker_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_();
		lk.unlock();
	}

	explicit pool(
		poolsize const& psize,
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep = posix_time::microseconds( 100),
		scanns const& scns = scanns( 20) )
	:
	worker_(),
	mtx_worker_(),
	state_( active_state),
	mtx_state_(),
	channel_(
		hwm,
		lwm),
	asleep_( asleep),
	scns_( scns),
	active_worker_( 0),
	idle_worker_( 0),
	running_worker_( 0)
	{
		if ( asleep_.is_special() || asleep_.is_negative() )
			throw invalid_timeduration("argument asleep is not valid");
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_worker_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_();
		lk.unlock();
	}

#ifdef BOOST_BIND_WORKER_TO_PROCESSORS
	explicit pool(
		posix_time::time_duration const& asleep = posix_time::microseconds( 10),
		scanns const& scns = scanns( 20) )
	:
	worker_(),
	mtx_worker_(),
	state_( active_state),
	mtx_state_(),
	channel_(),
	asleep_( asleep),
	scns_( scns),
	active_worker_( 0),
	idle_worker_( 0),
	running_worker_( 0)
	{
		if ( asleep_.is_special() || asleep_.is_negative() )
			throw invalid_timeduration("argument asleep is not valid");
		std::size_t psize( thread::hardware_concurrency() );
		BOOST_ASSERT( psize > 0);
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_worker_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( i);
		lk.unlock();
	}

	explicit pool(
		high_watermark const& hwm,
		low_watermark const& lwm,
		posix_time::time_duration const& asleep = posix_time::microseconds( 100),
		scanns const& scns = scanns( 20) )
	:
	worker_(),
	mtx_worker_(),
	state_( active_state),
	mtx_state_(),
	channel_(
		hwm,
		lwm),
	asleep_( asleep),
	scns_( scns),
	active_worker_( 0),
	idle_worker_( 0),
	running_worker_( 0)
	{
		if ( asleep_.is_special() || asleep_.is_negative() )
			throw invalid_timeduration("argument asleep is not valid");
		std::size_t psize( thread::hardware_concurrency() );
		BOOST_ASSERT( psize > 0);
		channel_.activate();
		unique_lock< shared_mutex > lk( mtx_worker_);
		for ( std::size_t i( 0); i < psize; ++i)
			create_worker_( i);
		lk.unlock();
	}
#endif

	~pool()
	{ shutdown(); }

	std::size_t active()
	{
		shared_lock< shared_mutex > lk( mtx_worker_);
		return active_();
	}

	std::size_t idle()
	{
		shared_lock< shared_mutex > lk( mtx_worker_);
		return idle_();
	}

	void shutdown()
	{
		unique_lock< shared_mutex > lk1( mtx_state_);
		if ( terminateing_() || terminated_() ) return;
		state_ = terminateing_state;
		lk1.unlock();

		channel_.deactivate();
		shared_lock< shared_mutex > lk2( mtx_worker_);
		BOOST_FOREACH( detail::worker w, worker_)
		{ w.signal_shutdown(); }
		BOOST_FOREACH( detail::worker w, worker_)
		{ w.join(); }
		lk2.unlock();

		lk1.lock();
		state_ = terminated_state;
	}

	const std::vector< detail::callable > shutdown_now()
	{
		unique_lock< shared_mutex > lk1( mtx_state_);
		if ( terminateing_() || terminated_() ) return std::vector< detail::callable >();
		state_ = terminateing_state;
		lk1.unlock();

		channel_.deactivate_now();
		shared_lock< shared_mutex > lk2( mtx_worker_);
		BOOST_FOREACH( detail::worker w, worker_)
		{
			w.signal_shutdown_now();
			w.interrupt();
		}
		BOOST_FOREACH( detail::worker w, worker_)
		{ w.join(); }
		lk2.unlock();
		std::vector< detail::callable > drain( channel_.drain() );

		lk1.lock();
		state_ = terminated_state;
		lk1.unlock();

		return drain;
	}

	std::size_t size()
	{
		shared_lock< shared_mutex > lk( mtx_worker_);
		return size_();
	}

	bool terminated()
	{
		shared_lock< shared_mutex > lk( mtx_state_);
		return terminated_();
	}

	bool terminateing()
	{
		shared_lock< shared_mutex > lk( mtx_state_);
		return terminateing_();
	}

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
					( void ( pool::*)( function< bool() > const&) ) & pool::reschedule_until_,
					this,
					wcb) );
			w->put( detail::callable( move( tsk) ), intr);
			return task< R >( f, intr);
		}
		else
		{
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
					( void ( pool::*)( function< bool() > const&) ) & pool::reschedule_until_,
					this,
					wcb) );
			w->put( detail::callable( move( tsk) ), intr);
			return task< R >( f, intr);
		}
		else
		{
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			channel_item itm( detail::callable( move( tsk) ), attr, intr);
			channel_.put( itm);
			return task< R >( f, intr);
		}
	}
    template< typename Pred >
    void reschedule_until( Pred const& pred)
    {
        reschedule_until_(pred);
    }
};

template< typename Channel >
thread_specific_ptr< typename pool< Channel >::random_idx >
pool< Channel >::tss_rnd_idx_;

}}

#endif // BOOST_TP_POOL_H

