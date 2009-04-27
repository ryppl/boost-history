
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_THREAD_CALLABLE_H
#define BOOST_TASK_DETAIL_THREAD_CALLABLE_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <boost/task/detail/config.hpp>
#include <boost/task/detail/interrupter.hpp>
#include <boost/task/semaphore.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
class BOOST_TASK_DECL thread_callable
{
private:
	struct impl
	{
		virtual ~impl() {}
		virtual void run() = 0;
	};

	template< typename R >
	class impl_wrapper : public impl
	{
	private:
		task< R >		t_;
		interrupter		i_;

	public:
		impl_wrapper(
			task< R > const& t,
			interrupter const& i)
		: t_( t), i_( i)
		{}

		void run()
		{
			t_();
			i_.reset();
		}
	};

	shared_ptr< impl >	impl_;

public:
	template< typename R >
	thread_callable(
		task< R > const& t,
		interrupter const& i)
	: impl_( new impl_wrapper<  R >( t, i) )
	{}

	void operator()();
};
} } }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_THREAD_CALLABLE_H

