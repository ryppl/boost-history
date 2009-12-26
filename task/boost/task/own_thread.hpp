
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_OWN_THREAD_H
#define BOOST_TASKS_OWN_THREAD_H

#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/context.hpp>
#include <boost/thread/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {

struct own_thread
{
	template< typename R >
	handle< R > operator()( task< R > t)
	{
		shared_ptr< shared_future< R > > f(
			new shared_future< R >(
				t.get_future() ) );
		context ctx;
		handle< R > h( f, ctx);
		callable ca( boost::move( t), ctx);
		ca();
		return h;
	}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_OWN_THREAD_H
