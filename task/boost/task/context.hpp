
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_CONTEXT_H
#define BOOST_TASK_CONTEXT_H

#include <boost/shared_ptr.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/detail/interrupter.hpp>
#include <boost/task/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{

class context
{
private:
	detail::interrupter	intr_;

public:
	template< typename R >
	callable get_callable( task< R > t)
	{ return callable( boost::move( t), intr_); }

	template< typename R >
	handle< R > get_handle( shared_future< R > f)
	{ return handle< R >( f, intr_); }
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_CONTEXT_H
