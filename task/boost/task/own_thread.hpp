
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_OWN_THREAD_H
#define BOOST_TASK_OWN_THREAD_H

#include <boost/task/detail/interrupter.hpp>
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
		t();
		detail::interrupter intr;
		intr.reset();
		return handle< R >(
			t.get_id(),
			t.get_future(),
			intr);
	}
};

} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_OWN_THREAD_H
