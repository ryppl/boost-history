//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_CONTEXT_H
#define BOOST_FIBERS_DETAIL_CONTEXT_H

namespace boost { namespace fibers {
namespace detail
{
template< typename FiberBase >
class context
{
private:
	FiberBase &	fib_;

public:
	context( FiberBase & fib)
	: fib_( fib)
	{}

	void yield()
	{ fib_.yield(); }

	template< typename Fiber >
	void yield_to( Fiber & to)
	{ fib_.yield_to( * to.impl_); }
};
} } }

#endif // BOOST_FIBERS_DETAIL_CONTEXT_H

