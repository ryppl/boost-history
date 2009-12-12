
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/spin/manual_reset_event.hpp"

#include <boost/assert.hpp>

#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/utility.hpp>

namespace boost {
namespace fibers {
namespace spin {

manual_reset_event::manual_reset_event( bool isset) :
	state_(
		isset ?
			static_cast< uint32_t >( SET) :
			static_cast< uint32_t >( RESET) ),
	waiters_( 0),
	enter_mtx_()
{}

void
manual_reset_event::set()
{
	enter_mtx_.lock();

	uint32_t expected = static_cast< uint32_t >( RESET);
	if ( ! detail::atomic_compare_exchange_strong(
			& state_, & expected,
			static_cast< uint32_t >( SET) ) ||
		! detail::atomic_load( & waiters_ ) )
		enter_mtx_.unlock();
}

void
manual_reset_event::reset()
{
	mutex::scoped_lock lk( enter_mtx_);
	BOOST_ASSERT( lk);

	detail::atomic_exchange( & state_,
		static_cast< uint32_t >( RESET) );
}

void
manual_reset_event::wait()
{
	{
		mutex::scoped_lock lk( enter_mtx_);
		BOOST_ASSERT( lk);
		detail::atomic_fetch_add( & waiters_, 1);
	}

	while ( static_cast< uint32_t >( RESET) == detail::atomic_load( & state_) )
		this_fiber::yield();	

	if ( 1 == detail::atomic_fetch_sub( & waiters_, 1) )
		enter_mtx_.unlock();
}

bool
manual_reset_event::try_wait()
{
	{
		mutex::scoped_lock lk( enter_mtx_);
		BOOST_ASSERT( lk);
		detail::atomic_fetch_add( & waiters_, 1);
	}

	bool result = static_cast< uint32_t >( SET) == detail::atomic_load( & state_);

	if ( 1 == detail::atomic_fetch_sub( & waiters_, 1) )
		enter_mtx_.unlock();

	return result;
}

}}}
