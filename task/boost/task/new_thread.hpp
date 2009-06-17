
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_NEW_THREAD_H
#define BOOST_TASK_NEW_THREAD_H

#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/detail/interrupter.hpp>
#include <boost/task/detail/thread_callable.hpp>
#include <boost/task/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
struct joiner
{
	void operator()( thread * thrd)
	{
		try
		{ thrd->join(); }
		catch (...)
		{}
		delete thrd;
	}
};

}

struct new_thread
{
	template< typename R >
# if defined(BOOST_HAS_RVALUE_REFS)
	handle< R > operator()( task< R > && t_)
# else
	handle< R > operator()( boost::detail::thread_move_t< task< R > > t_)
# endif
	{
		task< R > t( t_);
		shared_future< R > fut( t.get_future() );
		detail::interrupter intr;
		detail::thread_callable ca( boost::move( t), intr);

		shared_ptr< thread > thrd( new thread( ca), detail::joiner() );
		intr.set( thrd);

		return handle< R >( fut, intr);
	}
};
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_NEW_THREAD_H
