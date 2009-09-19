
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_OWN_THREAD_H
#define BOOST_TASK_OWN_THREAD_H

#include <boost/config.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/context.hpp>
#include <boost/task/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
struct own_thread
{
	template< typename R >
	handle< R > operator()( task< R > t)
	{
		shared_future< R > f( t.get_future() );
		context ctx;
		handle< R > h( f, ctx);
		callable ca( boost::move( t), ctx);
		ca();
		return h;
	}
};
}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_OWN_THREAD_H
