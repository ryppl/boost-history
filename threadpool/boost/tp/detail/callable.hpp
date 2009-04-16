//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_CALLABLE_H
#define BOOST_TP_DETAIL_CALLABLE_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <boost/tp/task.hpp>

namespace boost { namespace tp {
namespace detail
{
class callable
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
		task< R >	tsk_;

	public:
		impl_wrapper( task< R > const& tsk)
		: tsk_( tsk)
		{}

		void run()
		{ tsk_(); }

		void set( shared_ptr< thread > & thrd)
		{ tsk_.impl_->intr.set( thrd); }

		void reset()
		{ tsk_.impl_->intr.reset(); }
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
	callable( task< R > const& tsk)
	: impl_( new impl_wrapper<  R >( tsk) )
	{}

	void operator()();

	bool empty() const;

	void clear();
};
} } }

#endif // BOOST_TP_DETAIL_CALLABLE_H

