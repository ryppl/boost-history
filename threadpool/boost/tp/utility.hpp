//  Copright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_UTILITY_H
#define BOOST_TP_UTILITY_H

#include <boost/assert.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/tp/detail/worker.hpp>

namespace boost {
namespace this_task
{
namespace detail
{
struct time_reached
{
	system_time	abs_time;

	time_reached( system_time & abs_time_)
	: abs_time( abs_time_)
	{}

	bool operator()()
	{ return get_system_time() >= abs_time; }
};

class once_false
{
private:
	bool	result_;

public:
	once_false()
	: result_( false)
	{}
	
	bool operator()()
	{
		if ( ! result_)
		{
			result_ = true;
			return false;
		}
		else
			return true;
	}
};
}

template< typename Pred >
void reschedule_until( Pred const& pred)
{
	tp::detail::worker * w( tp::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	w->reschedule_until( pred);
}

template< typename Pool >
Pool & get_pool()
{
	tp::detail::worker * w( tp::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	return w->get_pool< Pool >();
}

inline
bool runs_in_pool()
{ return tp::detail::worker::tss_get() != 0; }

inline
thread::id worker_id()
{
	tp::detail::worker * w( tp::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	return w->get_id();
}

inline
void delay( system_time abs_time)
{
	if ( runs_in_pool() )
	{
		detail::time_reached t( abs_time);
		reschedule_until( t);
	}
	else
		this_thread::sleep( abs_time);
}

template< typename Duration >
void delay( Duration const& rel_time)
{ delay( get_system_time() + rel_time); }

inline
void yield()
{
	if ( runs_in_pool() )
	{
		detail::once_false t;
		reschedule_until( t);
	}
	else
		this_thread::yield();
}

inline
void interrupt()
{
	tp::detail::worker * w( tp::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	w->interrupt();
	this_thread::interruption_point();
}
}}

#endif // BOOST_TP_UTILITY_H
