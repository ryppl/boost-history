
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_CALLABLE_H
#define BOOST_TASKS_CALLABLE_H

#include <boost/config.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/task/context.hpp>
#include <boost/task/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace tasks {

class BOOST_TASKS_DECL callable
{
private:
	struct impl
	{
		atomic< unsigned int >	use_count_;

		impl() :
			use_count_( 0)
		{}

		virtual ~impl() {}

		virtual void run() = 0;

		virtual void reset( shared_ptr< thread > const&) = 0;

		inline friend void intrusive_ptr_add_ref( impl * p)
		{ p->use_count_.fetch_add( 1, memory_order_relaxed); }
		
		inline friend void intrusive_ptr_release( impl * p)
		{
			if ( p->use_count_.fetch_sub( 1, memory_order_release) == 1)
			{
				atomic_thread_fence( memory_order_acquire);
				delete p;
			}
		}
	};

	template< typename T >
	class wrapper : public impl
	{
	private:
		T			t_;
		context		ctx_;

	public:
		wrapper(
				BOOST_RV_REF( T) t,
				context const& ctx) :
			t_( t), ctx_( ctx)
		{}

		void run()
		{ t_(); }

		void reset( shared_ptr< thread > const& thrd)
		{ ctx_.reset( thrd); }
	};

	intrusive_ptr< impl >	impl_;

public:
	callable();

	template< typename T >
	callable(
			BOOST_RV_REF( T) t,
			context const& ctx) :
		impl_( new wrapper< T >( t, ctx) )
	{}

	void operator()();

	bool empty() const;

	void clear();

	void reset( shared_ptr< thread > const&);

	void swap( callable &);
};

class context_guard : private noncopyable
{
private:
	callable	&	ca_;

public:
	context_guard( callable & ca, shared_ptr< thread > const& thrd) :
		ca_( ca)
	{ ca_.reset( thrd); }

	~context_guard()
	{ ca_.clear(); }
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_CALLABLE_H

