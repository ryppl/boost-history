
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_CALLABLE_H
#define BOOST_TASK_DETAIL_CALLABLE_H

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
namespace detail
{
class BOOST_TASK_DECL callable
{
private:
	struct impl
	{
		virtual ~impl() {}
		virtual void run() = 0;
		virtual interrupter & get_interrupter() = 0;
	};

	template< typename R >
	class impl_wrapper : public impl
	{
	private:
		task< R >	t_;
		interrupter	i_;

	public:
		impl_wrapper(
			task< R > t,
			interrupter const& i)
		: t_( boost::move( t) ), i_( i)
		{}

		void run()
		{ t_(); }
		
		interrupter & get_interrupter()
		{ return i_; }
	};

	shared_ptr< impl >	impl_;

public:
	callable();

	template< typename R >
	callable(
		task< R > t,
		interrupter const& i)
	: impl_( new impl_wrapper<  R >( boost::move( t), i) )
	{}

	void operator()();

	bool empty() const;

	void clear();
	
	interrupter & get_interrupter();
};
}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_CALLABLE_H

