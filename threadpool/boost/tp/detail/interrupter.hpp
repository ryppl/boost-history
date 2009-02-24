//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_INTERRUPTER_H
#define BOOST_TP_DETAIL_INTERRUPTER_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility.hpp>

namespace boost { namespace tp {
namespace detail
{
class interrupter
{
private:
	class impl
	: private noncopyable
	{
	private:
		bool					interrupt_requested_;
		condition_variable		cond_;
		mutex					mtx_;
		shared_ptr< thread >	thrd_;

		void interrupt_()
		{
			if ( ! interrupt_requested_)
			{
				interrupt_requested_ = true;
				if ( thrd_) thrd_->interrupt();
			}
		}

	public:
		impl()
		:
		interrupt_requested_( false),
		cond_(),
		mtx_(),
		thrd_()
		{}

		void set( shared_ptr< thread > const& thrd)
		{
			BOOST_ASSERT( thrd);
			unique_lock< mutex > lk( mtx_);
			thrd_ = thrd;
			BOOST_ASSERT( thrd_);
			if ( interrupt_requested_) thrd_->interrupt();
		}

		void reset()
		{
			unique_lock< mutex > lk( mtx_);
			thrd_.reset();
			BOOST_ASSERT( ! thrd_);
			try
			{ this_thread::interruption_point(); }
			catch ( thread_interrupted const&)
			{}
			BOOST_ASSERT( ! this_thread::interruption_requested() );
			cond_.notify_all();
		}

		void interrupt()
		{
			unique_lock< mutex > lk( mtx_);
			interrupt_();
		}

		void interrupt_and_wait()
		{
			unique_lock< mutex > lk( mtx_);
			interrupt_();
			cond_.wait( lk);
		}

		void interrupt_and_wait( system_time const& abs_time)
		{
			unique_lock< mutex > lk( mtx_);
			interrupt_();
			cond_.timed_wait( lk, abs_time);
		}

		template< typename DurationType >
		void interrupt_and_wait( DurationType const& rel_time)
		{
			unique_lock< mutex > lk( mtx_);
			interrupt_();
			cond_.timed_wait( lk, rel_time);
		}

		bool interrupt_requested()
		{
			unique_lock< mutex > lk( mtx_);
			return interrupt_requested_;
		}
	};

	shared_ptr< impl >	impl_;

public:
	interrupter()
	: impl_( new impl() )
	{}

	void set( shared_ptr< thread > const& thrd)
	{ impl_->set( thrd); }

	void reset()
	{ impl_->reset();  }

	void interrupt()
	{ impl_->interrupt(); }

	void interrupt_and_wait()
	{ impl_->interrupt_and_wait(); }

	void interrupt_and_wait( system_time const& abs_time)
	{ impl_->interrupt_and_wait( abs_time); }

	template< typename DurationType >
	void interrupt_and_wait( DurationType const& rel_time)
	{ impl_->interrupt_and_wait( rel_time); }

	bool interrupt_requested()
	{ return impl_->interrupt_requested(); }
};
}
} }

#endif // BOOST_TP_DETAIL_INTERRUPTER_H
