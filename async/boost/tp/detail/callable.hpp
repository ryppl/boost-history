//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_CALLABLE_H
#define BOOST_TP_DETAIL_CALLABLE_H

#include <boost/future.hpp>

#include <boost/thread/detail/move.hpp>

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
	};

	template< typename T >
	struct impl_wrapper
	: public impl
	{
		packaged_task< T > tsk;

		impl_wrapper( boost::detail::thread_move_t< packaged_task< T > > const& t)
		: tsk( t)
		{}

		void run()
		{ tsk(); }
	};

	boost::shared_ptr< impl >	impl_;

public:
	callable();

	template< typename T >
	callable( boost::detail::thread_move_t< packaged_task< T > > const& t)
	: impl_( new impl_wrapper<  T >( t) )
	{}

	void operator()();

	bool empty() const;

	void clear();
};
} } }

#endif // BOOST_TP_DETAIL_CALLABLE_H

