
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_LOCAL_RR_UMS_H
#define BOOST_TASK_LOCAL_RR_UMS_H

#include <list>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/task/detail/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
class local_rr_ums
{
private:
	thread_specific_ptr< std::list< detail::fiber::sptr_t > >		runnable_;
	thread_specific_ptr< std::list< detail::fiber::sptr_t > >		blocked_;

public:
	local_rr_ums()
	: runnable_(), blocked_()
	{}

	void attach()
	{
		runnable_.reset( new std::list< detail::fiber::sptr_t >() );
		blocked_.reset( new std::list< detail::fiber::sptr_t >() );
	}

	bool has_runnable() const
	{ return ! runnable_->empty(); }

	bool has_blocked() const
	{ return ! blocked_->empty(); }

	void put_runnable( detail::fiber::sptr_t const& fib)
	{ runnable_->push_back( fib); }

	void put_blocked( detail::fiber::sptr_t const& fib)
	{ blocked_->push_back( fib); }

	bool try_take_runnable( detail::fiber::sptr_t & fib)
	{
		if ( ! has_runnable() ) return false;
		fib = runnable_->front();
		runnable_->pop_front();
		return true;
	}

	bool try_take_blocked( detail::fiber::sptr_t & fib)
	{
		if ( ! has_blocked() ) return false;
		fib = blocked_->front();
		blocked_->pop_front();
		return true;
	}
};
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_LOCAL_RR_UMS_H

