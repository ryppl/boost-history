
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_CALLABLE_H
#define BOOST_TASK_DETAIL_CALLABLE_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <boost/task/detail/config.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
class BOOST_TASK_DECL callable
{
private:
	struct impl
	{
		virtual ~impl() {}
		virtual void run() = 0;
		virtual void set( shared_ptr< thread > &) = 0;
		virtual void reset() = 0;
	};

	template< typename R >
	class impl_wrapper : public impl
	{
	private:
		task< R >	t_;

	public:
		impl_wrapper( task< R > const& t)
		: t_( t)
		{}

		void run()
		{ t_(); }

		void set( shared_ptr< thread > & thrd)
		{ t_.impl_->intr.set( thrd); }

		void reset()
		{ t_.impl_->intr.reset(); }
	};

	shared_ptr< impl >	impl_;

public:
	class scoped_lock : public noncopyable
	{
	private:
		callable	&	ca_;

	public:
		scoped_lock( callable &, shared_ptr< thread > &);

		~scoped_lock();
	};

	callable();

	template< typename R >
	callable( task< R > const& t)
	: impl_( new impl_wrapper<  R >( t) )
	{}

	void operator()();

	bool empty() const;

	void clear();
};
} } }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_CALLABLE_H

