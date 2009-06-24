//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_FIBER_H
#define BOOST_FIBERS_FIBER_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <boost/fibers/detail/fiber_base.hpp>
#include <boost/fibers/stacksize.hpp>

namespace boost {
namespace fibers
{
class fiber
: private noncopyable
{
private:
	template< typename Fiber >
	friend
	class detail::context;

	shared_ptr< detail::fiber_base >	impl_;

public:
	enum { default_stack_size = 16384 };

	template< typename Act >
	fiber(
		Act act,
		stacksize const& stack_size = stacksize( default_stack_size) )
	: impl_( new detail::fiber_base( act, stack_size) )
	{}

	bool exited() const
	{ return impl_->exited(); }

	bool ready() const
	{ return impl_->ready(); }

	bool running() const
	{ return impl_->running(); }

	void run()
	{ impl_->run(); }

	void yield()
	{ impl_->yield(); }

	void yield_to( fiber & to)
	{ impl_->yield_to( * to.impl_); }
};

} }

#endif // BOOST_FIBERS_FIBER_H

