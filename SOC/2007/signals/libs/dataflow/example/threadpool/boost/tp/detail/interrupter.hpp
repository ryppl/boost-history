//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_INTERRUPTER_H
#define BOOST_TP_DETAIL_INTERRUPTER_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
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
		bool					is_requested_;
		mutex					mtx_;
		shared_ptr< thread >	thrd_;

	public:
		impl()
		:
		is_requested_( false),
		mtx_(),
		thrd_()
		{}

		void set( shared_ptr< thread > const& thrd)
		{
			BOOST_ASSERT( thrd);
			unique_lock< mutex > lk( mtx_);
			thrd_ = thrd;
			BOOST_ASSERT( thrd_);
			if ( is_requested_) thrd_->interrupt();
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
		}

		void interrupt()
		{
			unique_lock< mutex > lk( mtx_);
			if ( ! is_requested_)
			{
				is_requested_ = true;
				if ( thrd_) thrd_->interrupt();
			}
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
};
}
} }

#endif // BOOST_TP_DETAIL_INTERRUPTER_H
