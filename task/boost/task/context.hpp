
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_CONTEXT_H
#define BOOST_TASK_CONTEXT_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility.hpp>

#include <boost/task/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost { namespace task
{
class BOOST_TASK_DECL context
{
private:
	class impl : private noncopyable
	{
	private:
		bool					requested_;
		mutex					mtx_;
		shared_ptr< thread >	thrd_;

		void reset_( shared_ptr< thread > const& thrd);

		void interrupt_();

	public:
		impl();

		void reset( shared_ptr< thread > const& thrd);

		void interrupt();

		bool interruption_requested();
	};

	shared_ptr< impl >	impl_;

public:
	context();

	void reset( shared_ptr< thread > const& thrd);

	void interrupt();

	bool interruption_requested();

	void swap( context & other);
};
}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_context_H
