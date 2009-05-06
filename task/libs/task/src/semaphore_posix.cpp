
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/semaphore.hpp"

#include <cerrno>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include "boost/task/utility.hpp"

namespace boost { namespace task
{
semaphore::semaphore( int value)
: handle_()
{
	if ( ::sem_init( & handle_, 0, value) == -1)
		throw system::system_error( errno, system::system_category);
}

semaphore::~semaphore()
{ ::sem_destroy( & handle_); }

void
semaphore::post()
{
	if ( ::sem_post( & handle_) == -1)
		throw system::system_error( errno, system::system_category);
}

void
semaphore::wait()
{
	int ret( -1);
	if ( this_task::runs_in_pool() )
	{
		// TODO: use semaphore::try_wait(), create a fiber and do a reschedule until
		// semaphore::try_wait() returns true
		do
		{ ret = ::sem_wait( & handle_); }
		while ( ret == -1 && errno == EINTR);
		if ( ret == -1)
			throw system::system_error( errno, system::system_category);
	}
	else
	{
		do
		{ ret = ::sem_wait( & handle_); }
		while ( ret == -1 && errno == EINTR);
		if ( ret == -1)
			throw system::system_error( errno, system::system_category);
	}
}

bool 
semaphore::try_wait()
{
	int ret( -1);
	do
	{ ret = ::sem_trywait( & handle_); }
	while ( ret == -1 && errno == EINTR);
	if ( ret == -1)
	{
		if ( errno == EAGAIN)
			return false;
		else
			throw system::system_error( errno, system::system_category);
	}
	return true;
}

int
semaphore::value()
{
	int value( 0);
	if ( ::sem_getvalue( & handle_, & value) == -1)
		throw system::system_error( errno, system::system_category);
	return value;
}
}}
