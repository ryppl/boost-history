
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::interprocess_condition

#ifndef BOOST_FIBERS_CONDITION_H
#define BOOST_FIBERS_CONDITION_H

#include <boost/cstdint.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/mutex.hpp>

namespace boost {
namespace fibers {

class condition : private noncopyable
{
private:
	enum command_t
	{
		SLEEPING = 0,
		NOTIFY_ONE,
		NOTIFY_ALL
	};

	volatile uint32_t	cmd_;
	volatile uint32_t	waiters_;
	mutex				enter_mtx_;
	mutex				check_mtx_;

	void wait_( mutex &);
	void notify_( uint32_t);

public:
	condition();

	~condition();

	void notify_one();

	void notify_all();

	template< typename Lock >
	void wait( Lock & lk)
	{
		if ( ! lk)
			throw lock_error();
		wait_( * lk.mutex() );
	}

	template<
		typename Lock,
		typename Pred
	>
	void wait( Lock & lk, Pred pred)
	{
		if ( ! lk)
			throw lock_error();

		while ( ! pred() )
			wait_( * lk.mutex() );
	}
};

}}

#endif // BOOST_FIBERS_CONDITION_H
