
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_NEW_THREAD_H
#define BOOST_TASK_NEW_THREAD_H

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/context.hpp>
#include <boost/task/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace task {
namespace detail {

struct joiner
{
	void operator()( thread * thrd)
	{
		try
		{
			BOOST_ASSERT( thrd);
			BOOST_ASSERT( thrd->joinable() );
			thrd->join();
		}
		catch (...)
		{}
		delete thrd;
	}
};

}

class new_thread
{
private:
	struct wrapper
	{
		callable	ca;
		
		wrapper( callable const& ca_) :
			ca( ca_)
		{}
		
		void operator()()
		{
			ca();
			ca.clear();
		}
	};

public:
	template< typename R >
	handle< R > operator()( task< R > t)
	{
		shared_ptr< shared_future< R > > f(
			new shared_future< R >(
				t.get_future() ) );
		context ctx1, ctx2;
		handle< R > h( f, ctx1);
		callable ca( boost::move( t), ctx2);
		shared_ptr< thread > thrd(
			new thread( wrapper( ca) ),
			detail::joiner() );
		ctx1.reset( thrd);

		return h;
	}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_NEW_THREAD_H
