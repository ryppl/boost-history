//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_FIXED_H
#define BOOST_TP_FIXED_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/future/future.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/utility.hpp>

#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/tp/poolsize.hpp>
#include <boost/tp/task.hpp>
#include <boost/tp/unbounded_channel.hpp>
#include <boost/tp/watermark.hpp>

namespace boost { namespace tp
{
struct fixed
{
	template<
		typename Channel = unbounded_channel< fifo >
	>
	class impl
	: private noncopyable
	{
	private:
		typedef function< void() >			callable;
		typedef Channel						channel;
		typedef typename channel::item		channel_item;
		typedef typename channel::iterator	channel_iterator;
	
		enum state
		{
			active_state,
			terminateing_state,
			terminated_state
		};

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
				++active_;
				BOOST_ASSERT( active_ > 0);
			}

			~active_guard()
			{
				unique_lock< shared_mutex > lk( mtx_worker_);
				--active_;
				BOOST_ASSERT( active_ >= 0);
			}
		};

		class worker
		{
		private:
			shared_ptr< thread >	thrd_;

		public:
			worker( callable const& ca)
			: thrd_( new thread( ca) )
			{ BOOST_ASSERT( ! ca.empty() ); }

			const shared_ptr< thread > thrd() const
			{ return thrd_; }

			const thread::id get_id() const
			{ return thrd_->get_id(); }

			void join() const
			{ thrd_->join(); }

			void interrupt() const
			{ thrd_->interrupt(); }
		};

		struct id_idx_tag {};

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
				>
			>
		>								worker_list;

		typedef typename worker_list::template index<
			id_idx_tag >::type			id_idx;

		std::size_t		max_size_;
		worker_list		worker_;
		id_idx		&	iidx_;
		std::size_t		active_worker_;
		shared_mutex	mtx_worker_;
		state			state_;
		shared_mutex	mtx_state_;
		channel		 	channel_;

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
			while ( channel_.take( ca, intr) )
			{
				BOOST_ASSERT( ! ca.empty() );
				active_guard ag(
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

		void create_worker_()
		{
			BOOST_ASSERT( ! terminateing_() && ! terminated_() );
			unique_lock< shared_mutex > lk( mtx_worker_);
			worker_.insert(
				worker(
					bind(
						& impl::entry_,
						this) ) );
			lk.unlock();
		}

		std::size_t active_() const
		{ return active_worker_; }

		std::size_t size_()
		{ return worker_.size(); }

		bool terminated_() const
		{ return state_ == terminated_state; }

		bool terminateing_() const
		{ return state_ == terminateing_state; }

	public:
		explicit impl( max_poolsize const& max_size)
		:
		max_size_( max_size),
		worker_(),
		iidx_( worker_.get< id_idx_tag >() ),
		active_worker_( 0),
		mtx_worker_(),
		state_( active_state),
		mtx_state_(),
		channel_()
		{
			channel_.activate();
			for ( std::size_t i( 0); i < max_size; ++i)
				create_worker_();
		}

		explicit impl(
			max_poolsize const& max_size,
			high_watermark const& hwm,
			low_watermark const& lwm)
		:
		max_size_( max_size),
		worker_(),
		iidx_( worker_.get< id_idx_tag >() ),
		active_worker_( 0),
		mtx_worker_(),
		state_( active_state),
		mtx_state_(),
		channel_(
			hwm,
			lwm)
		{
			BOOST_ASSERT( lwm <= hwm);
			channel_.activate();
			for ( std::size_t i( 0); i < max_size; ++i)
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
			return size_() - active_();
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

		std::size_t max_size()
		{ return max_size_; }

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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
			shared_lock< shared_mutex > lk( mtx_state_);
			if ( terminated_() )
				throw task_rejected("pool ist terminated");
			if ( terminateing_() )
				throw task_rejected("pool ist terminateing");

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

#endif // BOOST_TP_FIXED_H

