
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_CALLABLE_H
#define BOOST_TASK_CALLABLE_H

#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/task/detail/config.hpp>
#include <boost/task/detail/interrupter.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost { namespace task
{

class context;

class BOOST_TASK_DECL callable
{
private:
	friend class context;

	struct impl
	{
		virtual ~impl() {}
		virtual void run() = 0;
		virtual void reset() = 0;
		virtual void reset( shared_ptr< thread > const&) = 0;
	};

	template< typename R >
	class impl_wrapper : public impl
	{
	private:
		task< R >			t_;
		detail::interrupter	intr_;

	public:
		impl_wrapper(
			task< R > t,
			detail::interrupter const& intr)
		: t_( boost::move( t) ), intr_( intr)
		{}

		void run()
		{ t_(); }

		void reset()
		{ intr_.reset(); }

		void reset( shared_ptr< thread > const& thrd)
		{ intr_.reset( thrd); }
	};

	shared_ptr< impl >	impl_;

	template< typename R >
	callable(
		task< R > t,
		detail::interrupter const& intr)
	: impl_( new impl_wrapper<  R >( boost::move( t), intr) )
	{}

public:
	class context_guard : private noncopyable
	{
	private:
		callable	&	ca_;

	public:
		context_guard( callable & ca, shared_ptr< thread > const& thrd)
		: ca_( ca)
		{ ca_.reset( thrd); }

		~context_guard()
		{ ca_.reset(); }
	};

	callable();

	void operator()();

	bool empty() const;

	void clear();

	void reset();

	void reset( shared_ptr< thread > const&);
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_CALLABLE_H

