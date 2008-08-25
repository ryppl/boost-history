//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_TASK_H
#define BOOST_TP_TASK_H

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/future/future.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <boost/tp/detail/interrupter.hpp>

namespace boost { namespace tp
{
template< typename R >
class task
{
private:
	class impl
	: private noncopyable
	{
	private:
		boost::future< R >	fut_;
		detail::interrupter	intr_;
	
	public:
		impl(
			boost::future< R > const& fut,
			detail::interrupter const& intr)
		:
		fut_( fut),
		intr_( intr)
		{}

		boost::future< R > & get_future()
		{ return fut_; }

		void interrupt()
		{ intr_.interrupt(); }
	};

	shared_ptr< impl >	impl_;

public:
	task(
		boost::future< R > const& fut,
		detail::interrupter const& intr)
	: impl_( new impl( fut, intr) )
	{}

	boost::future< R > & get_future()
	{ return impl_->get_future(); }

	void interrupt()
	{ impl_->interrupt(); }
};
} }

#endif // BOOST_TP_TASK_H
