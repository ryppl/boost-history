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
		bool					interruption_requested_;
		condition_variable		cond_;
		mutex					mtx_;
		shared_ptr< thread >	thrd_;

		void interrupt_();

	public:
		impl();

		void set( shared_ptr< thread > const& thrd);

		void reset();

		void interrupt();

		void interrupt_and_wait();

		void interrupt_and_wait( system_time const& abs_time);

		template< typename DurationType >
		void interrupt_and_wait( DurationType const& rel_time)
		{
			unique_lock< mutex > lk( mtx_);
			interrupt_();
			cond_.timed_wait( lk, rel_time);
		}

		bool interruption_requested();
	};

	shared_ptr< impl >	impl_;

public:
	interrupter();

	void set( shared_ptr< thread > const& thrd);

	void reset();

	void interrupt();

	void interrupt_and_wait();

	void interrupt_and_wait( system_time const& abs_time);

	template< typename DurationType >
	void interrupt_and_wait( DurationType const& rel_time)
	{ impl_->interrupt_and_wait( rel_time); }

	bool interruption_requested();
};
}
} }

#endif // BOOST_TP_DETAIL_INTERRUPTER_H
