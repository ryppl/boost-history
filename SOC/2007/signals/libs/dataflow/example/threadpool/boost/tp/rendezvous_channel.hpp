//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_RENDEZVOUS_CHANNEL_H
#define BOOST_TP_RENDEZVOUS_CHANNEL_H

#include <cstddef>
#include <list>
#include <vector>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/tp/exceptions.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/detail/lock.hpp>

namespace boost { namespace tp
{
template<
	typename QueueingPolicy = fifo
>
class rendezvous_channel
{
private:
	typedef boost::function< void() >	callable;
	typedef QueueingPolicy				queueing_policy;

	template< typename M0, typename M1 >
	class lock_guard
	: private boost::noncopyable
	{
	private:
		M0	&	m0_;
		M1	&	m1_;

	public:
		lock_guard(
			M0 & m0,
			M1 & m1)
		: m0_( m0), m1_( m1)
		{ detail::lock( m0_, m1_); }
	
		~lock_guard()
		{
			m0_.unlock();
			m1_.unlock();
		}
	};

	class producer_slot
	{
	private:
		class impl
		{
		private:
			condition_variable	cond_;
			mutex				mtx_;
			callable			ca_;
			detail::interrupter	intr_;
			bool				transfered_;
	
			bool pred_() const
			{ return transfered_; }
	
		public:
			impl()
			:
			cond_(),
			mtx_(),
			ca_(),
			intr_(),
			transfered_( false)
			{}

			impl( callable const& ca)
			:
			cond_(),
			mtx_(),
			ca_( ca),
			intr_(),
			transfered_( false)
			{}
	
			void notify()
			{
				unique_lock< mutex > lk( mtx_);
				transfered_ = true;
				cond_.notify_all();
			}
	
			void transfer(
				callable & ca,
				detail::interrupter & intr)
			{
				unique_lock< mutex > lk( mtx_);
				ca = ca_;
				intr = intr_;
				transfered_ = true;
				cond_.notify_one();
			}
	
			const callable get() const
			{ return ca_; }
	
			void wait( detail::interrupter const& intr)
			{
				unique_lock< mutex > lk( mtx_);
				intr_ = intr;
				cond_.wait(
					lk,
					bind(
						& impl::pred_,
						this) );
			}
	
			template< typename timed_type >
			bool wait(
				detail::interrupter const& intr,
				timed_type const& dt)
			{
				unique_lock< mutex > lk( mtx_);
				intr_ = intr;
				return cond_.timed_wait(
					lk,
					dt,
					bind(
						& impl::pred_,
						this) );
			}
		};

		shared_ptr< impl >	impl_;

	public:
		producer_slot()
		: impl_( new impl)
		{}

		template< typename T >
		producer_slot( T const& t)
		: impl_( new impl( t) )
		{}

		producer_slot( producer_slot const& rh)
		: impl_( rh.impl_)
		{}

		producer_slot &
		operator=( producer_slot const& rh)
		{
			if ( & rh == this) return * this;
			impl_->notify();
			impl_ = rh.impl_;
			return * this;
		}

		void notify()
		{ impl_->notify(); }

		void transfer(
			callable & ca,
			detail::interrupter & intr)
		{ impl_->transfer( ca, intr); }

		const callable get() const
		{ return impl_->get(); }

		void wait( detail::interrupter const& intr)
		{ impl_->wait( intr);  }

		template< typename timed_type >
		bool wait(
			detail::interrupter const& intr,
			timed_type const& dt)
		{ return impl_->wait( intr, dt);  }
	};

	class consumer_slot
	{
	private:
		class impl
		{
		private:
			condition_variable		cond_;
			mutex					mtx_;
			callable				ca_;
			detail::interrupter		intr_;
			bool					transfered_;
	
			bool pred_() const
			{ return transfered_; }
	
		public:
			impl()
			:
			cond_(),
			mtx_(),
			ca_(),
			intr_(),
			transfered_( false)
			{}
	
			void notify()
			{
				unique_lock< mutex > lk( mtx_);
				transfered_ = true;
				cond_.notify_all();
			}
	
			void transfer(
				callable const& ca,
				detail::interrupter const& intr)
			{
				unique_lock< mutex > lk( mtx_);
				ca_ = ca;
				intr_ = intr;
				transfered_ = true;
				cond_.notify_one();
			}
	
			void wait(
				callable & ca,
				detail::interrupter & intr)
			{
				unique_lock< mutex > lk( mtx_);
				cond_.wait(
					lk,
					bind(
						& impl::pred_,
						this) );
				ca = ca_;
				intr = intr_;
			}
	
			template< typename timed_type >
			bool wait(
				callable & ca,
				detail::interrupter & intr,
				timed_type const& dt)
			{
				unique_lock< mutex > lk( mtx_);
				if ( cond_.timed_wait(
					lk,
					dt,
					bind(
						& impl::pred_,
						this) ) )
				{
					ca = ca_;
					intr = intr_;
					return true;
				}
				return false;
			}
		};

		shared_ptr< impl >	impl_;

	public:
		consumer_slot()
		: impl_( new impl() )
		{}

		void notify()
		{ impl_->notify(); }

		void transfer(
			callable const& ca,
			detail::interrupter const& intr)
		{ impl_->transfer( ca, intr); }

		void wait(
			callable & ca,
			detail::interrupter & intr)
		{ impl_->wait( ca, intr); }

		template< typename timed_type >
		bool wait(
			callable & ca,
			detail::interrupter & intr,
			timed_type const& dt)
		{ return impl_->wait( ca, intr, dt); }
	};

	typedef typename queueing_policy::template impl<
		producer_slot
	>											producer_queue;
	typedef std::list< consumer_slot >			consumer_queue;

public:
	typedef typename producer_queue::item		item;
	typedef typename producer_queue::iterator	iterator;

private:
	bool			active_;
	producer_queue	producer_queue_;
	shared_mutex	producer_mtx_;
	consumer_queue	consumer_queue_;
	shared_mutex	consumer_mtx_;

	void activate_()
	{ active_ = true; }

	void clear_()
	{
		BOOST_ASSERT( ! active_);
		producer_queue_.clear();
		consumer_queue_.clear();
		BOOST_ASSERT( producer_queue_.empty() );
		BOOST_ASSERT( consumer_queue_.empty() );
	}

	void deactivate_()
	{
		if ( active_)
		{
			active_ = false;
			BOOST_FOREACH( item itm, producer_queue_)
			{
				producer_slot slot( itm.ca() );
				slot.notify();
			}
			BOOST_FOREACH( consumer_slot slot, consumer_queue_)
			{ slot.notify(); }
		}

		BOOST_ASSERT( ! active_);
	}

	const std::vector< callable > drain_()
	{
		BOOST_ASSERT( ! active_);
		std::vector< callable > unprocessed;
		unprocessed.reserve( producer_queue_.size() );
		BOOST_FOREACH( item itm, producer_queue_)
		{ if ( ! itm.ca().get().empty() ) unprocessed.push_back( itm.ca().get() ); }
		clear_();
		return unprocessed;
	}

	bool empty_() const
	{ return producer_queue_.empty(); }

	std::size_t size_() const
	{ return producer_queue_.size(); }

public:
	rendezvous_channel()
	:
	active_( false),
	producer_queue_(),
	producer_mtx_(),
	consumer_queue_(),
	consumer_mtx_()
	{}

	bool active()
	{
		lock_guard< shared_mutex, shared_mutex > lk( producer_mtx_, consumer_mtx_);
		return active_;
	}

	void activate()
	{
		lock_guard< shared_mutex, shared_mutex > lk( producer_mtx_, consumer_mtx_);
		activate_();
	}

	void clear()
	{
		lock_guard< shared_mutex, shared_mutex > lk( producer_mtx_, consumer_mtx_);
		clear_();
	}

	bool deactive()
	{ return ! active(); }

	void deactivate()
	{
		lock_guard< shared_mutex, shared_mutex > lk( producer_mtx_, consumer_mtx_);
		deactivate_();
	}

	const std::vector< callable > drain()
	{
		unique_lock< shared_mutex > lk( producer_mtx_);
		return drain_();
	}

	bool empty()
	{
		unique_lock< shared_mutex > lk( producer_mtx_);
		return empty_();
	}

	bool erase(
		iterator &,
		future< void >)
	{ return false; }

	bool full()
	{ return false; }

	std::size_t size()
	{
		unique_lock< shared_mutex > lk( producer_mtx_);
		return size_();
	}

	iterator put( item const& itm)
	{
		unique_lock< shared_mutex > lk1( consumer_mtx_);
		if ( ! active_) throw task_rejected("channel is not active");
		if ( ! consumer_queue_.empty() )
		{
			consumer_queue_.front().transfer( itm.ca().get(), itm.intr() );
			consumer_queue_.pop_front();
			return producer_queue_.end();
		}
		lk1.unlock();
		unique_lock< shared_mutex > lk2( producer_mtx_);
		producer_queue_.push( itm);
		lk2.unlock();
		producer_slot slot( itm.ca() );
		slot.wait( itm.intr() );
		return producer_queue_.end();
	}

	template< typename Tm >
	iterator put(
		item const& itm,
		Tm const& tm)
	{
		unique_lock< shared_mutex > lk1( consumer_mtx_);
		if ( ! active_) throw task_rejected("channel is not active");
		if ( ! consumer_queue_.empty() )
		{
			consumer_queue_.front().transfer( itm.ca().get(), itm.intr() );
			consumer_queue_.pop_front();
			return producer_queue_.end();
		}
		lk1.unlock();
		unique_lock< shared_mutex > lk2( producer_mtx_);
		producer_queue_.push( itm);
		lk2.unlock();
		producer_slot slot( itm.ca() );
		if ( ! slot.wait( itm.intr(), tm) ) throw task_rejected("timed out");
		return producer_queue_.end();
	}

	bool take(
		callable & ca,
		detail::interrupter & intr)
	{
		unique_lock< shared_mutex > lk1( producer_mtx_);
		if ( ! active_) return false;
		if ( ! producer_queue_.empty() )
		{
			producer_slot slot( producer_queue_.pop().ca() );
			slot.transfer( ca, intr);
			return true;
		}
		lk1.unlock();
		consumer_slot slot;
		unique_lock< shared_mutex > lk2( consumer_mtx_);
		consumer_queue_.push_back( slot);
		lk2.unlock();
		try
		{
			slot.wait( ca, intr);
			return ! ca.empty();
		}
		catch ( thread_interrupted const& e)
		{ printf("take: thread_interrupted\n"); return false; }
	}

	template< typename Tm >
	bool take(
		callable & ca,
		detail::interrupter & intr,
		Tm const& tm)
	{
		unique_lock< shared_mutex > lk1( producer_mtx_);
		if ( ! active_) return false;
		if ( ! producer_queue_.empty() )
		{
			producer_slot slot( producer_queue_.pop().ca() );
			slot.transfer( ca, intr);
			return true;
		}
		lk1.unlock();
		consumer_slot slot;
		unique_lock< shared_mutex > lk2( consumer_mtx_);
		consumer_queue_.push_back( slot);
		lk2.unlock();
		try
		{ return slot.wait( ca, intr, tm) && ! ca.empty(); }
		catch ( thread_interrupted const& e)
		{ return false; }
	}
};
} }

#endif // BOOST_TP_RENDEZVOUS_CHANNEL_H
