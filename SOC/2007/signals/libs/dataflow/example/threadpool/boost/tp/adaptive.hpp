//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_ADAPTIVE_H
#define BOOST_TP_ADAPTIVE_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/future/future.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/utility.hpp>

#include <boost/tp/adjustment.hpp>
#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/detail/lock.hpp>
#include <boost/tp/detail/lock_guard.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/tp/poolsize.hpp>
#include <boost/tp/task.hpp>
#include <boost/tp/unbounded_channel.hpp>
#include <boost/tp/watermark.hpp>

namespace boost { namespace tp
{
struct keep_core_size {};
struct less_than_core_size {};

template<
	typename AdjustmentPolicy = depend_on_pending,
	typename RecreatePolicy = less_than_core_size
>
struct adaptive
{
	template<
		typename Channel = unbounded_channel< fifo >
	>
	class impl
	: private noncopyable
	{
	private:
		typedef AdjustmentPolicy				adjustment_policy;
		typedef function< void() >				callable;
		typedef Channel							channel;
		typedef typename channel::item			channel_item;
		typedef typename channel::iterator		channel_iterator;
		typedef RecreatePolicy					recreate_policy;

		enum state
		{
			active_state,
			terminateing_state,
			terminated_state
		};

		class worker
		{
		public:
			enum state
			{
				running_state,
				terminate_state
			};

		private:
			shared_ptr< thread >	thrd_;
			state					state_;

		public:
			worker( callable const& ca)
			:
			thrd_( new thread( ca) ),
			state_( running_state)
			{ BOOST_ASSERT( ! ca.empty() ); }

			const shared_ptr< thread > thrd() const
			{ return thrd_; }

			const thread::id get_id() const
			{ return thrd_->get_id(); }

			void join() const
			{ thrd_->join(); }

			void interrupt() const
			{ thrd_->interrupt(); }

			state current_state() const
			{ return state_; }

			void current_state( state value)
			{ state_ = value; }
		};

		struct id_idx_tag {};
		struct state_idx_tag {};

		typedef multi_index::multi_index_container<
			worker,
			multi_index::indexed_by<
				multi_index::ordered_unique<
					multi_index::tag< id_idx_tag >,
					multi_index::const_mem_fun<
						worker,
						const thread::id,
						& worker::get_id
					>
				>,
				multi_index::ordered_non_unique<
					multi_index::tag< state_idx_tag >,
					multi_index::const_mem_fun<
						worker,
						typename worker::state,
						& worker::current_state
					>
				>
			>
		>								worker_list;

		typedef typename worker_list::template index<
			id_idx_tag >::type			id_idx;
		typedef typename worker_list::template index<
			state_idx_tag >::type		state_idx;

		class active_guard
		{
		private:
			std::size_t		&	active_;
			shared_mutex	&	mtx_worker_;

		public:
			active_guard(
				std::size_t & active__,
				shared_mutex & mtx_worker__)
			: active_( active__), mtx_worker_( mtx_worker__)
			{
				unique_lock< shared_mutex > lk( mtx_worker_);
				BOOST_ASSERT( active_ >= 0);
				++active_;
			}

			~active_guard()
			{
				unique_lock< shared_mutex > lk( mtx_worker_);
				--active_;
				BOOST_ASSERT( active_ >= 0);
			}
		};

		class change_state
		{
		private:
			typename worker::state	state_;

		public:
			change_state( typename worker::state value)
			: state_( value)
			{}

			void operator()( worker & w)
			{ w.current_state( state_); }
		};

		class state_guard
		: private noncopyable
		{
		private:
			shared_mutex	&	mtx_worker_;
			id_idx			&	id_idx_;
			state_idx		&	state_idx_;

		public:
			state_guard(
				shared_mutex & mtx,
				id_idx & id_idx__,
				state_idx & state_idx__)
			:
			mtx_worker_( mtx),
			id_idx_( id_idx__),
			state_idx_( state_idx__)
			{}

			~state_guard()
			{
				upgrade_lock< shared_mutex > lk1( mtx_worker_);
				std::for_each(
					state_idx_.lower_bound( worker::terminate_state),
					state_idx_.upper_bound( worker::terminate_state),
					std::mem_fun_ref( & worker::join) );
				unique_lock< shared_mutex > lk2( lk1.move() );
				state_idx_.erase( worker::terminate_state);
				typename id_idx::iterator i( id_idx_.find( this_thread::get_id() ) );
				if ( i != id_idx_.end() ) id_idx_.modify( i, change_state( worker::terminate_state) );
			}
		};

		std::size_t					core_size_;
		std::size_t					max_size_;
		posix_time::time_duration	keep_alive_;
		worker_list					worker_;
		id_idx					&	iidx_;
		state_idx				&	sidx_;
		std::size_t					active_worker_;
		shared_mutex				mtx_worker_;
		state						state_;
		shared_mutex				mtx_state_;
		channel		 				channel_;
		adjustment_policy			adjust_pol_;

		void entry_()
		{
			shared_lock< shared_mutex > lk( mtx_worker_);
			id_idx & iidx( worker_.get< id_idx_tag >() );
			typename id_idx::iterator i( iidx.end() );
			typename id_idx::iterator e( iidx.end() );
			lk.unlock();
			while ( i == e)
			{
				lk.lock();
				i = iidx.find( this_thread::get_id() );
				lk.unlock();
			}
			BOOST_ASSERT( i != e);

			shared_ptr< thread > thrd( i->thrd() );
			BOOST_ASSERT( thrd);
			callable ca;
			detail::interrupter intr;
			while ( channel_.take( ca, intr, keep_alive_) )
			{
				BOOST_ASSERT( ! ca.empty() );
				active_guard guard(
					active_worker_,
					mtx_worker_);
				shared_ptr< void > ig(
					static_cast< void * >( 0),
					bind(
						& detail::interrupter::reset,
						intr) );
				intr.set( thrd);
				ca();
				ca.clear();
				BOOST_ASSERT( ca.empty() );
			}
		}

		void entry_( keep_core_size)
		{
			entry_();
			detail::lockguard< shared_mutex, shared_mutex > lk( mtx_state_, mtx_worker_);
			if ( ! terminateing_() && ! terminated_() && size_() < core_size_)
				create_worker_();
		}

		void entry_( less_than_core_size)
		{ entry_(); }

		void create_worker_()
		{
			BOOST_ASSERT( ! terminateing_() && ! terminated_() );
			unique_lock< shared_mutex > lk( mtx_worker_);
			worker_.insert(
				worker(
					bind(
						( void ( impl::*) ( recreate_policy) ) & impl::entry_,
						this,
						recreate_policy() ) ) );
			lk.unlock();
		}

		std::size_t active_() const
		{ return active_worker_; }

		std::size_t idle_() const
		{ return size_() - active_(); }

		std::size_t size_() const
		{
			state_idx const& idx( worker_.get< state_idx_tag >() );
			return std::distance(
				idx.find( worker::running_state),
				idx.end() );
		}

		void adjust_pool_()
		{
			BOOST_ASSERT( ! terminateing_() && ! terminated_() );
			std::size_t s( size() );
			if ( s < max_size_ &&
				adjust_pol_(
					poolsize( s),
					core_poolsize( core_size() ),
					max_poolsize( max_size_),
					channel_.size(),
					channel_.full() ) )
				create_worker_();
			BOOST_ASSERT( size() <= max_size_ );
		}

		bool terminated_() const
		{ return state_ == terminated_state; }

		bool terminateing_() const
		{ return state_ == terminateing_state; }

	public:
		explicit impl(
			core_poolsize const& core_size,
			max_poolsize const& max_size,
			posix_time::time_duration const& keep_alive,
			adjustment_policy const& adjust_pol = adjustment_policy() )
		:
		core_size_( core_size),
		max_size_( max_size),
		keep_alive_( keep_alive),
		worker_(),
		iidx_( worker_.get< id_idx_tag >() ),
		sidx_( worker_.get< state_idx_tag >() ),
		active_worker_( 0),
		mtx_worker_(),
		state_( active_state),
		mtx_state_(),
		channel_(),
		adjust_pol_( adjust_pol)
		{
			if ( core_size > max_size_)
				throw invalid_poolsize("core poolsize must be less than or equal to max poolsize");
			if ( keep_alive_.is_special() )
				throw invalid_timeout("keep_alive is not valid");
			if ( keep_alive_.is_negative() )
				throw invalid_timeout("keep_alive is negative");
			channel_.activate();
		}

		explicit impl(
			core_poolsize const& core_size,
			max_poolsize const& max_size,
			posix_time::time_duration const& keep_alive,
			high_watermark const& hwm,
			low_watermark const& lwm,
			adjustment_policy const& adjust_pol = adjustment_policy() )
		:
		core_size_( core_size),
		max_size_( max_size),
		keep_alive_( keep_alive),
		worker_(),
		iidx_( worker_.get< id_idx_tag >() ),
		sidx_( worker_.get< state_idx_tag >() ),
		active_worker_( 0),
		mtx_worker_(),
		state_( active_state),
		mtx_state_(),
		channel_(
			hwm,
			lwm),
		adjust_pol_( adjust_pol)
		{
			if ( core_size > max_size_)
				throw invalid_poolsize("core poolsize must be less than or equal to max poolsize");
			if ( keep_alive_.is_special() )
				throw invalid_timeout("keep_alive is not valid");
			if ( keep_alive_.is_negative() )
				throw invalid_timeout("keep_alive is negative");
			channel_.activate();
		}

		explicit impl(
			preallocate const& pre,
			core_poolsize const& core_size,
			max_poolsize const& max_size,
			posix_time::time_duration const& keep_alive,
			adjustment_policy const& adjust_pol = adjustment_policy() )
		:
		core_size_( core_size),
		max_size_( max_size),
		keep_alive_( keep_alive),
		worker_(),
		iidx_( worker_.get< id_idx_tag >() ),
		sidx_( worker_.get< state_idx_tag >() ),
		active_worker_( 0),
		mtx_worker_(),
		state_( active_state),
		mtx_state_(),
		channel_(),
		adjust_pol_( adjust_pol)
		{
			if ( core_size > max_size_)
				throw invalid_poolsize("core poolsize must be less than or equal to max poolsize");
			if ( pre > max_size_)
				throw invalid_poolsize("preallocated poolsize must be less than or equal to max poolsize");
			if ( keep_alive_.is_special() )
				throw invalid_timeout("keep_alive is not valid");
			if ( keep_alive_.is_negative() )
				throw invalid_timeout("keep_alive is negative");
			channel_.activate();
			for ( std::size_t i( 0); i < pre; ++i)
				create_worker_();
		}

		explicit impl(
			preallocate const& pre,
			core_poolsize const& core_size,
			max_poolsize const& max_size,
			posix_time::time_duration const& keep_alive,
			high_watermark const& hwm,
			low_watermark const& lwm,
			adjustment_policy const& adjust_pol = adjustment_policy() )
		:
		core_size_( core_size),
		max_size_( max_size),
		keep_alive_( keep_alive),
		worker_(),
		iidx_( worker_.get< id_idx_tag >() ),
		sidx_( worker_.get< state_idx_tag >() ),
		active_worker_( 0),
		mtx_worker_(),
		state_( active_state),
		mtx_state_(),
		channel_(
			hwm,
			lwm),
		adjust_pol_( adjust_pol)
		{
			if ( core_size > max_size_)
				throw invalid_poolsize("core poolsize must be less than or equal to max poolsize");
			if ( pre > max_size_)
				throw invalid_poolsize("preallocated poolsize must be less than or equal to max poolsize");
			if ( keep_alive_.is_special() )
				throw invalid_timeout("keep_alive is not valid");
			if ( keep_alive_.is_negative() )
				throw invalid_timeout("keep_alive is negative");
			channel_.activate();
			for ( std::size_t i( 0); i < pre; ++i)
				create_worker_();
		}

		~impl()
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

		const posix_time::time_duration keep_alive()
		{
			shared_lock< shared_mutex > lk( mtx_worker_);
			return keep_alive_;
		}

		void keep_alive( posix_time::time_duration const& value)
		{
			unique_lock< shared_mutex > lk( mtx_worker_);
			return keep_alive_ = value;
		}

		void shutdown()
		{
			unique_lock< shared_mutex > lk1( mtx_state_);
			if ( terminateing_() || terminated_() ) return;
			state_ = terminateing_state;
			lk1.unlock();
	
			unique_lock< shared_mutex > lk2( mtx_worker_);
			channel_.deactivate();
			std::vector< worker > lst(
				worker_.begin(),
				worker_.end() );
			worker_.clear();
			lk2.unlock();

			std::for_each(
				lst.begin(),
				lst.end(),
				std::mem_fun_ref( & worker::join) );

			lk1.lock();
			state_ = terminated_state;
			lk1.unlock();
		}

		const std::vector< callable > shutdown_now()
		{
			unique_lock< shared_mutex > lk1( mtx_state_);
			if ( terminateing_() || terminated_() ) return std::vector< callable >();
			state_ = terminateing_state;
			lk1.unlock();

			unique_lock< shared_mutex > lk2( mtx_worker_);
			channel_.deactivate();
			std::for_each(
				worker_.begin(),
				worker_.end(),
				std::mem_fun_ref( & worker::interrupt) );
			std::vector< worker > lst(
				worker_.begin(),
				worker_.end() );
			worker_.clear();
			std::vector< callable > drain( channel_.drain() );
			lk2.unlock();

			std::for_each(
				lst.begin(),
				lst.end(),
				std::mem_fun_ref( & worker::join) );

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

		std::size_t core_size()
		{
			shared_lock< shared_mutex > lk( mtx_worker_);
			return core_size_;
		}
	
		void core_size( std::size_t size)
		{
			shared_lock< shared_mutex > lk( mtx_worker_);
			core_size_ = size;
		}

		std::size_t max_size()
		{
			shared_lock< shared_mutex > lk( mtx_worker_);
			return max_size_;
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

		const std::size_t upper_bound()
		{ return channel_.upper_bound(); }

		void upper_bound( std::size_t hwm)
		{ return channel_.upper_bound( hwm); }

		const std::size_t lower_bound()
		{ return channel_.lower_bound(); }

		void lower_bound( std::size_t lwm)
		{ return channel_.lower_bound( lwm); }

		template< typename Act >
		task< typename result_of< Act() >::type > submit( Act const& act)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				intr);
			channel_.put( itm);
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename Attr
		>
		task< typename result_of< Act() >::type > submit(
			Act const& act,
			Attr const& attr)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				attr,
				intr);
			channel_.put( itm);
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename T
		>
		task< typename result_of< Act() >::type > chained_submit(
			Act const& act,
			task< T > & tsk)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				intr);
			tsk.get_future().add_callback(
				bind(
					( channel_iterator ( channel::*)( channel_item const&) ) & channel::put,
					ref( channel_),
					itm) );
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename Attr,
			typename T
		>
		task< typename result_of< Act() >::type > chained_submit(
			Act const& act,
			Attr const& attr,
			task< T > & tsk)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				attr,
				intr);
			tsk.get_future().add_callback(
				bind(
					( channel_iterator ( channel::*)( channel_item const&) ) & channel::put,
					ref( channel_),
					itm) );
			return task< R >( prom, intr);
		}

		template< typename Act >
		task< typename result_of< Act() >::type > lazy_submit( Act const& act)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			future< void > fg( prom.get_needed_future() );
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				intr);
			fg.add_callback(
				bind(
					( channel_iterator ( channel::*)( channel_item const&) ) & channel::put,
					ref( channel_),
					itm) );
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename Attr
		>
		task< typename result_of< Act() >::type > lazy_submit(
			Act const& act,
			Attr const& attr)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			future< void > fg( prom.get_needed_future() );
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				attr,
				intr);
			fg.add_callback(
				bind(
					( channel_iterator ( channel::*)( channel_item const&) ) & channel::put,
					ref( channel_),
					itm) );
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename Tm
		>
		task< typename result_of< Act() >::type > timed_submit(
			Act const& act,
			Tm const& tm)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				intr);
			channel_.put( itm, tm);
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename Attr,
			typename Tm
		>
		task< typename result_of< Act() >::type > timed_submit(
			Act const& act,
			Attr const& attr,
			Tm const& tm)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				attr,
				intr);
			channel_.put( itm, tm);
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename Tm,
			typename T
		>
		task< typename result_of< Act() >::type > timed_chained_submit(
			Act const& act,
			Tm const& tm,
			task< T > & tsk)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				intr);
			tsk.get_future().add_callback(
				bind(
					( channel_iterator ( channel::*)( channel_item const&, Tm const&) ) & channel::put,
					ref( channel_),
					itm,
					tm) );
			return task< R >( prom, intr);
		}

		template<
			typename Act,
			typename Attr,
			typename Tm,
			typename T
		>
		task< typename result_of< Act() >::type > timed_chained_submit(
			Act const& act,
			Attr const& attr,
			Tm const& tm,
			task< T > & tsk)
		{
			shared_lock< shared_mutex > lk1( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

			adjust_pool_();

			typedef typename result_of< Act() >::type R;
			detail::interrupter intr;
			promise< R > prom;
			channel_item itm(
				future_wrapper< R >(
					act,
					prom),
				attr,
				intr);
			tsk.get_future().add_callback(
				bind(
					( channel_iterator ( channel::*)( channel_item const&, Tm const&) ) & channel::put,
					ref( channel_),
					itm,
					tm) );
			return task< R >( prom, intr);
		}
	};
};
} }

#endif // BOOST_TP_ADAPTIVE_H

