//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_CALLABLE_H
#define BOOST_TP_DETAIL_CALLABLE_H

#include <boost/future.hpp>
#include <boost/thread.hpp>

#include <boost/thread/detail/move.hpp>
#include <boost/tp/exceptions.hpp>

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

	template<
		typename Act,
		typename T
	>
	struct impl_wrapper
	: public impl
	{
		Act act;
		promise< T > prom;

		impl_wrapper(
			Act const& act_,
			promise< T > & prom_)
		: act( act_), prom( move( prom_) )
		{}

		void run()
		{
			try
			{ prom.set_value( act() ); }
			catch ( thread_interrupted const&)
			{ prom.set_exception( copy_exception( task_interrupted() ) ); }
			catch(...)
			{ prom.set_exception( current_exception() ); }
		}
	};

	template< typename Act >
	struct impl_wrapper< Act, void >
	: public impl
	{
		Act act;
		promise< void > prom;

		impl_wrapper(
			Act const& act_,
			promise< void > & prom_)
		: act( act_), prom( move( prom_) )
		{}

		void run()
		{
			try
			{
				act();
				prom.set_value();
			}
			catch ( thread_interrupted const&)
			{ prom.set_exception( copy_exception( task_interrupted() ) ); }
			catch(...)
			{ prom.set_exception( current_exception() ); }
		}
	};

	boost::shared_ptr< impl >	impl_;

public:
	callable();

	template<
		typename Act,
		typename T
	>
	callable(
		Act const& act,
		promise< T > & prom)
	: impl_( new impl_wrapper<  Act, T >( act, prom) )
	{}

	void operator()();

	bool empty() const;

	void clear();
};
} } }

#endif // BOOST_TP_DETAIL_CALLABLE_H

