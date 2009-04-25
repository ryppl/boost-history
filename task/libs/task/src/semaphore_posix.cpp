
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/semaphore.hpp"

#include <cerrno>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace boost { namespace task
{
semaphore::semaphore( int value)
: async_handle_()
{
	if ( ::sem_init( & async_handle_, 0, value) == -1)
		throw system::system_error( errno, system::system_category);
}

semaphore::~semaphore()
{ ::sem_destroy( & async_handle_); }

void
semaphore::post()
{
	if ( ::sem_post( & async_handle_) == -1)
		throw system::system_error( errno, system::system_category);
}

void
semaphore::wait()
{
	if ( ::sem_wait( & async_handle_) == -1)
		throw system::system_error( errno, system::system_category);
}

bool 
semaphore::try_wait()
{
	if ( ::sem_trywait( & async_handle_) == -1)
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
	if ( ::sem_getvalue( & async_handle_, & value) == -1)
		throw system::system_error( errno, system::system_category);
	return value;
}
}}
